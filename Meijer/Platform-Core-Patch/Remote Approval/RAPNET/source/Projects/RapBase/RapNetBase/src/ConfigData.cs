using System;
using System.Collections;
using System.Text;
using System.Xml;
using RPSWNET;
using System.Text.RegularExpressions;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Reflection;
using System.IO;

namespace RapNet
{
	/// <summary>
	/// Configuration data singleton used by the application.
	/// </summary>
	public class ConfigData
	{
		/// <summary>
		/// The class constructor.
		/// </summary>
		public ConfigData()
		{			
		}

        /// <summary>
        /// Used to replace any partition environment variables in sArg with their corresponding values.
        /// The partition drive variables are currently:
        /// 
        /// %APP_DRIVE%
        /// %DATA_DRIVE%
        /// %OS_DRIVE%
        /// %BOOT_DRIVE%
        /// %TEMP_DRIVE%
        /// 
        /// If any of the above variables are found in the input string, they are replaced
        /// with the value (drive letter) associated with the environment variable.
        /// If a value is not defined for the partition environment variable(s), a default value
        /// of "C:" is used ("\" for WINCE).
        /// 
        /// For WINCE, no other variable substitution occurs.
        /// For non-WINCE, any other defined environment variables within sArg will also be 
        /// replaced with their respective values.
        /// </summary>
        /// <param name="sArg">The string to be searched.</param>
        /// <exception cref="ArgumentNullException"></exception>
        /// <returns>String containing the expanded environment variables.</returns>
        /// <remarks>jv185012</remarks>
        public static string ExpandPartitionVars(string sArg)
        {
            string sTmp = sArg;
#if( !WINCE )
            sTmp = Environment.ExpandEnvironmentVariables(sArg);
#endif
            string sRep = "%(APP|DATA|OS|BOOT|TEMP)_DRIVE%";  // Regex pattern.

            // If any %???_DRIVE% strings are still in sTmp, then the environment variables
            // for the drive partitions have not been set (or this is WINCE).
            // Replace the drive partition env. vars with "C:" (or "\" for WINCE).
#if( WINCE )
			sTmp = Regex.Replace(sTmp, sRep, "", RegexOptions.IgnoreCase);
#else
            sTmp = Regex.Replace(sTmp, sRep, "C:", RegexOptions.IgnoreCase);
#endif

            return sTmp;
        }

        /// <summary>
        /// Extracts a value from RapEvent data.
        /// </summary>
        /// <param name="key">
        /// The key of the key-value pair being queried for.
        /// </param>
        /// <param name="eventData">
        /// The RapEvent data.
        /// </param>
        /// <returns>
        /// The RapEvent value associated with the key or null if not available.
        /// </returns>
        public static string ExtractRapEventData(string key, string eventData)
        {
            string strVal = null;
            string strSearchKey = null;
            int nIndex = eventData.IndexOf(key);
            while (nIndex > -1)
            {
                int nTemp = eventData.IndexOf("=", nIndex);
                strSearchKey = eventData.Substring(nIndex, (nTemp - nIndex) + 1);
                if (strSearchKey[key.Length] == '=' || strSearchKey[key.Length] == '[')
                {
                    int nStart = nIndex + strSearchKey.Length;
                    int nEnd = eventData.IndexOf(";", nStart);
                    if (nEnd > -1)
                    {
                        strVal = eventData.Substring(nStart, nEnd - nStart);
                    }
                    else
                    {
                        strVal = eventData.Substring(nStart);
                    }
                    break;
                }
                nIndex = eventData.IndexOf(key, nIndex + 1);
            }
            return strVal;						
        }

        /// <summary>
        /// Get the one and only ConfigData instance.
        /// </summary>
        /// <returns>
        /// ConfigData instance.
        /// </returns>
        public static ConfigData GetConfigData()
        {
            if (theConfigData == null)
            {
                theConfigData = LoadClass("ConfigData") as ConfigData;
            }
            return theConfigData;
        }
        
        /// <summary>
        /// Load class dynamically.
        /// </summary>
        public static Object LoadClass(string className)
        {
            return LoadClass(className, null);
        }

        /// <summary>
        /// Load class dynamically with the specified constructor parameters
        /// </summary>
        public static Object LoadClass(string className, Object[] args)
        {
            if (ExtensionAssembly.Length > 0)
            {
                Assembly assembly = Assembly.LoadFrom(ExtensionAssembly);
                try
                {
                    string classPrefix = ExtensionAssembly;
                    int startIndex = ExtensionAssembly.LastIndexOf("RapNet");
                    if (startIndex > -1)
                    {
                        startIndex += 6;
                        int endIndex = ExtensionAssembly.LastIndexOf(".");
                        classPrefix = endIndex > -1 ? ExtensionAssembly.Substring(startIndex, endIndex - startIndex) :
                            ExtensionAssembly.Substring(startIndex);                        
                    }
                    return assembly.GetType("RapNet." + classPrefix + className).GetConstructors()[0].Invoke(args);
                }
                catch (Exception)
                {
                    try
                    {
                        if (null != assembly)
                        {
                            return assembly.GetType("RapNet." + className).GetConstructors()[0].Invoke(args);
                        }
                    }
                    catch (Exception)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskWarning,
                            "ConfigData::LoadClass() - Failed to load " + className + " class from " + ExtensionAssembly + ". Using base class.");
                    }
                }
            }
            return Type.GetType("RapNet." + className).GetConstructors()[0].Invoke(args);
        }

        /// <summary>
        /// Converts Object to DateTime
        /// </summary>        
        public static DateTime ObjectToDateTime(Object o)
        {
            if (null == o)
            {
                throw (new ArgumentNullException());
            }
            DateTime dt = DateTime.Now;
            string[] timeList = o.ToString().Split(new Char[] { ',' });
            if (null != timeList && timeList.Length >= 6)
            {
                dt = new DateTime(Convert.ToInt32(timeList[0]), Convert.ToInt32(timeList[1]),
                    Convert.ToInt32(timeList[2]), Convert.ToInt32(timeList[3]),
                    Convert.ToInt32(timeList[4]), Convert.ToInt32(timeList[5]));
            }
            return dt;
        }
               
        /// <summary>
        /// Read-Only AlphaNumericKeyboard property accessor.
        /// </summary>
        public bool AlphaNumericKeyboard
        {
            get
            {
                return alphaNumericKeyboard;
            }
        }

        /// <summary>
        /// Read-Only EncodeOperatorIDPW property accessor.
        /// </summary>
        public bool EncodeOperatorIDPW
        {
            get
            {
                return encodeOperatorIDPW;
            }
        }

		/// <summary>
		/// Read-Only EnterIdUppercase property accessor.
		/// </summary>
		public bool EnterIdUppercase
		{
			get
			{
				return enterIdUppercase;
			}
		}

		/// <summary>
		/// Read-Only EnterPasswordUppercase property accessor.
		/// </summary>
		public bool EnterPasswordUppercase
		{
			get
			{
				return enterPasswordUppercase;
			}
		}

		/// <summary>
		/// Allow the Alt-F4 keystroke entry to exit the application? (default is true).
		/// </summary>
		public bool AltF4ExitAllowed
		{
			get
			{
				return altF4ExitAllowed; 
			}
            set
            {
                altF4ExitAllowed = value;     
            }
		}

		/// <summary>
		/// Allow multiple Exclusive Lanes simultaneously.
		/// ie. Interventions from a 2nd lane can be processed during 
		/// Interventions from the 1st Lane, without exiting exclusive lane
		/// mode on the 1st Lane.  Intervention handling on the 1st lane
		/// is simply suspended temporarily.
		/// </summary>
		static public bool MultiExclusiveLanes
		{
			get
            {
                return IsMobileUI ? true : multiExclusiveLanes;
            }
            set
            {
                multiExclusiveLanes = value;
            }
		}
        
        /// <summary>
        /// Read-Only AudioPath property accessor.
        /// </summary>
        public string AudioPath
        {
            get
            {
                return audioPath;
            }
        }

        /// <summary>
        /// Read-Only ComputerName property accessor.
        /// </summary>
        public string ComputerName
        {
            get
            {
                if (0 == computerName.Length)
                {
                    byte[] host = new byte[250];
                    int len = host.Length;
                    if( 0 == PInvoke.gethostname( host, len ) )
                    {
                        computerName = System.Text.ASCIIEncoding.ASCII.GetString( host, 0, len );
                        char[] trimChar = { '\0' };
                        computerName = computerName.TrimEnd( trimChar );
                    }
                }                
                return computerName;
            }
        }
        
        /// <summary>
		/// Read-Only ConfigPath property accessor.
		/// </summary>
		public string ConfigPath
		{
			get
			{
				return configPath;
			}
		}

        public string RapNetConfigFileName
        {
            get
            {
                return rapNetConfigFileName;
            }
        }

        /// <summary>
        /// Read-Only ConnectionPeriod property accessor.
        /// </summary>
        public int ConnectionPeriod
        {
            get
            {
                return connectionPeriod;
            }
        }

        /// <summary>
        /// Read-Only property accessor to the full path to the Rapnet config file that is currently loaded..
        /// </summary>
        public string LoadedConfigFilePath
        {
            get
            {
                return loadedConfigFilePath;
            }
        }

		/// <summary>
		/// Read-Only ConnectionPeriod property accessor.
		/// </summary>
		public string BinPath
		{
			get
			{
				return binPath;
			}
		}       

		/// <summary>
		/// Read-Only report data path accessor.
		/// </summary>
		public string ReportDataPath
		{
			get
			{
				return reportDataPath;
			}
		}

        /// <summary>
        /// Read-Only ConnectRemoteTimeout property accessor.
        /// </summary>
        public int ConnectRemoteTimeout
        {
            get
            {
                return connectRemoteTimeout;
            }
        }

        /// <summary>
        /// Read-Only DelayedInterventionSound property accessor.
        /// </summary>
        public string DelayedInterventionSound
        {
            get
            {
                return delayedInterventionSound;
            }
        }
				
		/// <summary>
		/// Read-Only BiometricDeviceConfigured property accessor.		
		/// </summary>
		public bool BiometricDeviceConfigured
		{
			get
			{
				return biometricDeviceConfigured;
			}
		}
		
        /// <summary>
        /// Checks RAP Scanner is configured in the registry.
        /// Loaction: HKLM\SOFTWARE\NCR\SCOT - RAP\ObservedOptions
        /// "Scanner"="Y" or "N"
        /// </summary>
        public bool IsRAPScannerConfigured
        {
            get
            {
                if (null == rapScannerConfigured)
                {
                    rapScannerConfigured = false; // default to false

                    uint hkey = 0;
                    uint REG_SZ = 1;
                    uint KEY_READ = 0x00020019;
                    uint HKEY_LOCAL_MACHINE = 0x80000002;

                    // Open the registry location.
                    if (0 == RegOpenKeyExW(HKEY_LOCAL_MACHINE, REG_RAP_SCNANNER_PATH, 0, KEY_READ, ref hkey))
                    {
                        uint type = 0;
                        byte[] data = new byte[5];
                        uint len = (uint)data.Length;

                        // Read the registry location.
                        if (0 == RegQueryValueExW(hkey, "Scanner", 0, ref type, data, ref len))
                        {
                            if (type == REG_SZ)
                            {
                                string val = System.Text.UnicodeEncoding.Unicode.GetString(data, 0, (int)len).Trim();
                                if (val.Length > 0 && (0 == String.Compare(val, "Y", true)))
                                {
                                    rapScannerConfigured = true;
                                }
                            }
                        }
                        else
                        {
                            CmDataCapture.Capture(CmDataCapture.MaskInfo, "ConfigData::IsRAPScannerConfigured - Unable to READ RAP scanner configuration.");
                        }

                        RegCloseKey(hkey);
                    }
                    else
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskInfo, "ConfigData::IsRAPScannerConfigured - Unable to OPEN RAP scanner configuration.");
                    }

                    CmDataCapture.Capture(CmDataCapture.MaskInfo, 
                        String.Format("ConfigData::IsRAPScannerConfigured - [{0}]", rapScannerConfigured.Value));
                }

                return rapScannerConfigured.Value;
            }
        }
                
        
        /// <summary>
        /// Shows a mobile device user interface if true.
        /// </summary>
        static public bool IsMobileUI = false;

        /// <summary>
        /// Is RapNetProxy server if true.
        /// </summary>
        static public bool IsProxyServer = false;

        /// <summary>
        /// Optional full path to application settings .xml file.
        /// </summary>
        static public string ApplicationSettingsFile = null;
        
        /// <summary>
        /// Optional full path to application settings .xml file.
        /// </summary>
        static public string ExtensionAssembly = String.Empty;
        		
        /// <summary>
        /// Read-Only ImmediateInterventionSound property accessor.
        /// </summary>
        public string ImmediateInterventionSound
        {
            get
            {
                return immediateInterventionSound;
            }
        }

        /// <summary>
        /// Read-Only ImmediateInterventionSoundSecondary property accessor.
        /// </summary>
        public string ImmediateInterventionSoundSecondary
        {
            get
            {
                return immediateInterventionSoundSecondary;
            }
        }

        /// <summary>
        /// Read-Only InterventionSoundDelay property accessor.
        /// </summary>
        public int InterventionSoundDelay
        {
            get
            {
                return interventionSoundDelay;
            }
        }

        /// <summary>
        /// Read-Only InterventionSoundDelaySecondary property accessor.
        /// </summary>
        public int InterventionSoundDelaySecondary
        {
            get
            {
                return interventionSoundDelaySecondary;
            }
        }

		/// <summary>
		/// LaneConfigList property accessor.
		/// </summary>
		public ArrayList LaneConfigList
		{
			get
			{
				return laneConfigList;
			}			
		}

        /// <summary>
        /// LaneResponseTimeout property accessor.
        /// </summary>
        public int LaneResponseTimeout
        {
            get
            {
                return laneResponseTimeout;
            }
        }

        /// <summary>
        /// EncryptScanData property accessor.
		/// </summary>
        public bool EncryptScanData
		{
			get
			{
                return encryptScanData;
			}			
		}

        /// <summary>
        /// Read-Only MaxNumberOfLanesShownForRAPLite property accessor.
        /// </summary>
        public int MaxNumberOfLanesShownForRAPLite
        {
            get
            {
                return maxNumberOfLanesShownForRAPLite;
            }
        }

        /// <summary>
        /// Read-Only EnableRAPLite property accessor.
        /// </summary>
        public bool EnableRAPLite
        {
            get
            {
                return bEnableRAPLite;
            }
        }

        /// <summary>
        /// Loads configuration data from registry.
        /// </summary>
        static public void LoadRegistry()
        {
            uint hKey = OpenRegistry(true);
            string reg = RegistryPath + "\\" + CurrentProcessName() + "\\";
            if (0 == hKey)
            {
                CmDataCapture.ReadRegistry(reg, "DataCapture");
                hKey = OpenRegistry(false);
            }
            else
            {
                CmDataCapture.ReadRegistry(reg, "DataCapture\\" + CurrentProcessName() + ".exe");
            }
            if (0 != hKey)
            {
                ReadRegistrySettings(hKey);
                PInvoke.RegCloseKey(hKey);
            }
            else
            {
                throw new ArgumentNullException("Null registry key");
            }
        }
                
        /// <summary>
		/// Loads configuration data from persistant storage.
		/// </summary>
		public void Load()
		{
            // Resolve any environment variables that may exist within the ???Path variables.
			try
			{
				audioPath = ExpandPartitionVars(audioPath);
				configPath = ExpandPartitionVars(configPath);
				mediaPath = ExpandPartitionVars(mediaPath);
				binPath = ExpandPartitionVars(binPath);
                reportDataPath = ExpandPartitionVars(reportDataPath);
                if (null != ApplicationSettingsFile)
                {
                    ApplicationSettingsFile = ExpandPartitionVars(ApplicationSettingsFile);
                }
			}
			catch (ArgumentNullException argE)
			{
				CmDataCapture.Capture(CmDataCapture.MaskError, "ConfigData::Load() - ArgumentNullException: " + argE.Message);
			}
            // Load XML settings //
            try
            {
                XmlDocument myXmlDocument = new XmlDocument();
                try
                {
                    String fileToLoad = (null != ApplicationSettingsFile && ApplicationSettingsFile.Length > 0) ? ApplicationSettingsFile :
                        (ConfigPath + "\\" + RapNetConfigFileName);
                    myXmlDocument.Load(fileToLoad);
                    loadedConfigFilePath = fileToLoad;
                }
                catch (System.IO.IOException caught)
                {
                    if (null != ApplicationSettingsFile && ApplicationSettingsFile.Length > 0)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskWarning, "ConfigData::Load() - failed to load " + ApplicationSettingsFile +
                            ". Attempting to load from ConfigPath.");
                        string fileToLoad = ConfigPath + "\\" + RapNetConfigFileName;
                        myXmlDocument.Load(fileToLoad);
                        loadedConfigFilePath = fileToLoad;
                    }
                    else
                    {
                        throw caught;
                    }                    
                }

                XmlDocument xmlOverrideDoc = null;
                if (File.Exists(ConfigPath + "\\" + "RapNet.000"))
                {
                    try
                    {
                        xmlOverrideDoc = new XmlDocument();
                        xmlOverrideDoc.Load(ConfigPath + "\\" + "RapNet.000");

                        // Check if it has valid nodes
                        if (!xmlOverrideDoc.HasChildNodes)
                        {
                            xmlOverrideDoc = null;
                        }
                        else
                        {
                            XmlElement root = xmlOverrideDoc.DocumentElement;
                            if (!root.Name.Equals("Options"))
                            {
                                xmlOverrideDoc = null;
                            }
                         }
                    }
                    catch (Exception ex)
                    {
                        xmlOverrideDoc = null;
                        CmDataCapture.Capture(CmDataCapture.MaskWarning, "Exception: " + ex.Message);
                        CmDataCapture.Capture(CmDataCapture.MaskWarning, "Unable to load RapNet.000");
                    }
                }
                else
                {
                    CmDataCapture.Capture(CmDataCapture.MaskInfo, "No override file, RapNet.000, found.");
                }
                LoadXml(myXmlDocument, xmlOverrideDoc);
                
            }
            catch (XmlException caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "ConfigData::Load() - XmlException: " + caught.Message);
            }
            catch (System.IO.IOException caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "ConfigData::Load() - IOException: " + caught.Message);
            }			
		}
                	
		/// <summary>
		/// Is this terminal a RAP terminal?
		/// </summary>
		public bool ValidateRapTerminalRegistry()
		{
			// Get registry settings //
			uint HKEY_LOCAL_MACHINE = 0x80000002;
			uint KEY_READ = 0x00020019;
			uint REG_SZ = 1;
			uint hkey = 0;
			bool rVal = false;

			string keyPath = loadControllerRegistryPath;

			if( 0 == RegOpenKeyExW( HKEY_LOCAL_MACHINE, keyPath, 0, KEY_READ, ref hkey ) )
			{
				char[] trimChar = { '\0' };
				uint type = 0;
				byte[] data = new byte[1024];
				uint len = (uint)data.Length;
				if( 0 == RegQueryValueExW( hkey, "RapTerminal", 0, ref type, data, ref len ) )
				{
					string rapTerminalRegistry = "";
					if( type == REG_SZ )
					{
						rapTerminalRegistry = System.Text.UnicodeEncoding.Unicode.GetString( data, 0, (int)len );
						rapTerminalRegistry = rapTerminalRegistry.TrimEnd( trimChar );
					}
					rVal = rapTerminalRegistry.ToUpper().Equals("YES");
				}

				RegCloseKey( hkey );
			}

			return rVal;
		}

        /// <summary>
        /// Initializes laneConfigList from node list
        /// </summary>
        public void IntializeLaneConfigList(XmlNodeList nodeList)
        {
            try
            {
                laneConfigList.Clear();
                StringBuilder laneConfig = new StringBuilder(); 
                foreach (XmlNode node in nodeList)
                {
                    string hostName = "";
                    string ipAdrs = "";
                    string service = "";                
                    foreach (XmlNode childNode in node.ChildNodes)
                    {
                        if (childNode.Name == "Host")
                        {
                            hostName = childNode.InnerText;
                        }
                        else if (childNode.Name == "IPAdrs")
                        {
                            ipAdrs = childNode.InnerText;
                        }
                        else if (childNode.Name == "Service")
                        {
                            service = childNode.InnerText;
                        }
                    }
                    laneConfig.Length = 0;
                    laneConfig.Append("host=");
                    laneConfig.Append(ipAdrs.Length == 0 ? hostName : ipAdrs);
                    if (service.Length > 0)
                    {
                        laneConfig.Append(";service=");
                        laneConfig.Append(service);
                    }
                    laneConfigList.Add(laneConfig.ToString());
                }
            }
            catch (XmlException caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "ConfigData::Load() - XmlException: " + caught.Message);
            }
        }

        /// <summary>
        /// Validate the ID/Password against the Windows registry.
        /// </summary>
        public bool ValidateOperatorRegistry(string id, string password)
        {
            // Get registry settings //
            uint HKEY_LOCAL_MACHINE = 0x80000002;
            uint KEY_READ = 0x00020019;
            uint REG_SZ = 1;
            uint hkey = 0;
            bool rVal = false;
            if( 0 == RegOpenKeyExW(HKEY_LOCAL_MACHINE, swUsersRegistryPath, 0, KEY_READ, ref hkey ) )
            {
                char[] trimChar = { '\0' };
                uint type = 0;
                byte[] data = new byte[1024];
                uint len = (uint)data.Length;
                if( 0 == RegQueryValueExW( hkey, id, 0, ref type, data, ref len ) )
                {
                    string passwordRegistry = "";
                    if( type == REG_SZ )
                    {
                        passwordRegistry = System.Text.UnicodeEncoding.Unicode.GetString( data, 0, (int)len );
                        passwordRegistry = passwordRegistry.TrimEnd( trimChar );
                    }

                    // Size of hash is based on the specified algorithm.
                    switch (HashAlgorithm.ToUpper())
                    {
                        case "SHA1":
                            rVal = SimpleHash.VerifyHash( password, "SHA1", passwordRegistry);
                            break;
                        case "SHA256":
                            rVal = SimpleHash.VerifyHash( password, "SHA256", passwordRegistry);
                            break;
                        case "SHA384":
                            rVal = SimpleHash.VerifyHash( password, "SHA384", passwordRegistry);
                            break;
                        case "SHA512":
                            rVal = SimpleHash.VerifyHash( password, "SHA512", passwordRegistry);
                            break;
                        case "MD5":
                            rVal = SimpleHash.VerifyHash( password, "MD5", passwordRegistry);
                            break;
                        default: // Must be NO hashing
                            rVal = passwordRegistry.Equals( password );
                            break;
                    }
                }

                RegCloseKey( hkey );
            }

            return rVal;
        }

        /// <summary>
        /// Hash algorithm type ("NONE", "SHA1Button", "SHA256Button", "SHA384Button", "SHA512Button", "MD5Button")
        /// </summary>
        public string HashAlgorithm
        {
            get
            {
                return hashAlgorithm;
            }
        }
	        
        /// <summary>
		/// Read-Only MediaPath property accessor.
		/// </summary>
		public string MediaPath
		{
			get
			{
				return mediaPath;
			}
		}

        /// <summary>
        /// Read-Only ScanSound property accessor.
        /// </summary>
        public string ScanSound
        {
            get
            {
                return scanSound;
            }
        }

        /// <summary>
        /// Read-Only ShowCursor property accessor.
        /// </summary>
        public bool ShowCursor
        {
            get
            {
                return showCursor;
            }
        }

        /// <summary>
        /// Read-Only SignOnTimeout property accessor.
        /// </summary>
        public int SignOnTimeout
        {
            get
            {
                return signOnTimeout;
            }
        }

        /// <summary>
        /// Read-Only SynchronizeTimeServer property accessor.
        /// </summary>
        public bool SynchronizeTimeServer
        {
            get
            {
                return synchronizeTimeServer;
            }
        }

		/// <summary>
		/// Read-Only DisplayTransactionCount property accessor.
		/// </summary>
		public bool DisplayTransactionCount
		{
			get
			{
				return displayTransactionCount;
			}
		}

		/// <summary>
		/// Read-Only TimePeriodForThroughputCalculation property accessor.
		/// </summary>
		public int TimePeriodForThroughputCalculation
		{
			get
			{
				return timePeriodForThroughputCalculation;
			}
		}

		/// <summary>
		/// Read-Only TrxCountThresholdForHighAlert property accessor.
		/// </summary>
		public int TrxCountThresholdForHighAlert
		{
			get
			{
				return trxCountThresholdForHighAlert;
			}
		}

		/// <summary>
		/// Read-Only TrxCountThresholdForCaution property accessor.
		/// </summary>
		public int TrxCountThresholdForCaution
		{
			get
			{
				return trxCountThresholdForCaution;
			}
		}

		/// <summary>
		/// Read-Only DataModel property accessor.
		/// </summary>
		public bool DataModel
		{
			get
			{
				return dataModel;
			}
		}

        #region Meijer customization begin

        public bool UseBiometrics { get; private set; }

        public bool AllowEmplIdSignOn { get; private set; }

        public bool AllowHandKeyEMPLID { get; private set; }

        public ScannerEventArgs SavedScannerEventArgs { get; set; }

        public const string LAUNCHPAD_APP = "LaunchPadNet.exe";

        #endregion Meijer customization end

        static public string CurrentProcessName()
        {
            string procName = Process.GetCurrentProcess().ProcessName;
            int pos = procName.IndexOf(".");
            if (pos > 0)
            {
                procName = procName.Substring(0, pos);
            }
            return procName;
        }


         private const string REG_RAP_SCNANNER_PATH = @"SOFTWARE\NCR\SCOT - RAP\ObservedOptions";

		/// <summary>
		/// Load controller registry path.
		/// </summary>
		private const string loadControllerRegistryPath = "SOFTWARE\\NCR\\SCOT\\CurrentVersion\\Load Controller";

        /// <summary>
        /// Name of Rap event.
        /// </summary>
        public const string eventRap = "RemoteData";

        /// <summary>
        /// Rap event operation.
        /// </summary>
        public const string eventRapOperation = "operation";

        /// <summary>
        /// Loads configuration data from application xml.
        /// </summary>
        protected virtual void LoadXml(XmlDocument myXmlDocument, XmlDocument xmlOverrideDoc)
        {
            XmlNodeList nodeList = myXmlDocument.GetElementsByTagName("Lane");
            
            IntializeLaneConfigList(nodeList);

            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("AlphaNumericKeyboard")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("AlphaNumericKeyboard")).Count > 0)
            {
                alphaNumericKeyboard = Convert.ToBoolean(nodeList[0].InnerText);
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("EncodeOperatorIDPW")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("EncodeOperatorIDPW")).Count > 0)
            {
                encodeOperatorIDPW = Convert.ToBoolean(nodeList[0].InnerText);
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("EnterIdUppercase")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("EnterIdUppercase")).Count > 0)
            {
                enterIdUppercase = Convert.ToBoolean(nodeList[0].InnerText);
            }

            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("EnterPasswordUppercase")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("EnterPasswordUppercase")).Count > 0)
            {
                enterPasswordUppercase = Convert.ToBoolean(nodeList[0].InnerText);
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("AltF4ExitAllowed")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("AltF4ExitAllowed")).Count > 0)
            {
                altF4ExitAllowed = Convert.ToBoolean(nodeList[0].InnerText);
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("SignOnTimeout")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("SignOnTimeout")).Count > 0)
            {
                signOnTimeout = Convert.ToInt32(nodeList[0].InnerText);
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("ConnectionPeriod")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("ConnectionPeriod")).Count > 0)
            {
                connectionPeriod = Convert.ToInt32(nodeList[0].InnerText);
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("ConnectRemoteTimeout")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("ConnectRemoteTimeout")).Count > 0)
            {
                connectRemoteTimeout = Convert.ToInt32(nodeList[0].InnerText);
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("SynchronizeTimeServer")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("SynchronizeTimeServer")).Count > 0)
            {
                synchronizeTimeServer = Convert.ToBoolean(nodeList[0].InnerText);
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("DisplayTransactionCount")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("DisplayTransactionCount")).Count > 0)
            {
                displayTransactionCount = Convert.ToBoolean(nodeList[0].InnerText);
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("TimePeriodForThroughputCalculation")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("TimePeriodForThroughputCalculation")).Count > 0)
            {
                timePeriodForThroughputCalculation = Convert.ToInt32(nodeList[0].InnerText);
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("TrxCountThresholdForHighAlert")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("TrxCountThresholdForHighAlert")).Count > 0)
            {
                trxCountThresholdForHighAlert = Convert.ToInt32(nodeList[0].InnerText);
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("TrxCountThresholdForCaution")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("TrxCountThresholdForCaution")).Count > 0)
            {
                trxCountThresholdForCaution = Convert.ToInt32(nodeList[0].InnerText);
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("ScanSound")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("ScanSound")).Count > 0)
            {
                scanSound = nodeList[0].InnerText;
            }
            if ( (null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("ImmediateInterventionSound")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("ImmediateInterventionSound")).Count > 0)
            {
                immediateInterventionSound = nodeList[0].InnerText;
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("ImmediateInterventionSoundSecondary")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("ImmediateInterventionSoundSecondary")).Count > 0)
            {
                immediateInterventionSoundSecondary = nodeList[0].InnerText;
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("DelayedInterventionSound")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("DelayedInterventionSound")).Count > 0)
            {
                delayedInterventionSound = nodeList[0].InnerText;
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("InterventionSoundDelay")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("InterventionSoundDelay")).Count > 0)
            {
                interventionSoundDelay = Convert.ToInt32(nodeList[0].InnerText);
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("InterventionSoundDelaySecondary")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("InterventionSoundDelaySecondary")).Count > 0)
            {
                interventionSoundDelaySecondary = Convert.ToInt32(nodeList[0].InnerText);
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("BiometricDeviceConfigured")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("BiometricDeviceConfigured")).Count > 0)
            {
                biometricDeviceConfigured = Convert.ToBoolean(nodeList[0].InnerText);
            }
            if (!IsMobileUI)
            {
                if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("MultipleInterventionHandling")).Count > 0) ||
                        (nodeList = myXmlDocument.GetElementsByTagName("MultipleInterventionHandling")).Count > 0)
                {
                    multiExclusiveLanes = Convert.ToBoolean(nodeList[0].InnerText);
                }
            }
            else
            {
                multiExclusiveLanes = true;
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("DataModel")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("DataModel")).Count > 0)
            {
                dataModel = Convert.ToBoolean(nodeList[0].InnerText);
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("LaneResponseTimeout")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("LaneResponseTimeout")).Count > 0)
            {
                laneResponseTimeout = Convert.ToInt32(nodeList[0].InnerText);
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("HashAlgorithm")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("HashAlgorithm")).Count > 0)
            {
                hashAlgorithm = Convert.ToString(nodeList[0].InnerText);
            }
            if ( (null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("EncryptScanData")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("EncryptScanData")).Count > 0)
            {
                encryptScanData = Convert.ToBoolean(nodeList[0].InnerText);
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("MaxNumberOfLanesShownForRAPLite")).Count > 0) ||
                (nodeList = myXmlDocument.GetElementsByTagName("MaxNumberOfLanesShownForRAPLite")).Count > 0)
            {
                maxNumberOfLanesShownForRAPLite = Convert.ToInt32(nodeList[0].InnerText);
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("EnableRAPLite")).Count > 0) ||
                (nodeList = myXmlDocument.GetElementsByTagName("EnableRAPLite")).Count > 0)
            {
                bEnableRAPLite = Convert.ToBoolean(nodeList[0].InnerText);
            }
            
            #region Meijer customization begin
            // +RFQ 7477
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("UseBiometrics")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("UseBiometrics")).Count > 0)
            {
                UseBiometrics = Convert.ToBoolean(nodeList[0].InnerText);
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("AllowEmplIdSignOn")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("AllowEmplIdSignOn")).Count > 0)
            {
                AllowEmplIdSignOn = Convert.ToBoolean(nodeList[0].InnerText);
            }
            if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("AllowHandKeyEMPLID")).Count > 0) ||
                    (nodeList = myXmlDocument.GetElementsByTagName("AllowHandKeyEMPLID")).Count > 0)
            {
                AllowHandKeyEMPLID = Convert.ToBoolean(nodeList[0].InnerText);
            }                    
            // -RFQ 7477
            #endregion Meijer customization end
        }

	    static private uint OpenRegistry(bool withProcessKey)
        {
            uint hKey = 0;
            string registryPath = RegistryPath + "\\" + CurrentProcessName();
            if (withProcessKey)
            {
                registryPath += "\\";
                registryPath += CurrentProcessName();
                registryPath += ".exe";
            }
            PInvoke.RegOpenKeyExW(PInvoke.HKEY_LOCAL_MACHINE, registryPath, 0, PInvoke.KEY_READ, ref hKey);            
            return hKey;
        }

        static private void ReadRegistrySettings(uint hKey)
        {
            char[] trimChar = {'\0'};
            uint type = 0;
            byte[] data = new byte[1024];
            uint len = (uint)data.Length;
            if (0 == PInvoke.RegQueryValueExW(hKey, "AudioPath", 0, ref type, data, ref len))
            {
                if (type == PInvoke.REG_SZ)
                {
                    audioPath = System.Text.UnicodeEncoding.Unicode.GetString(data, 0, (int)len);
                    audioPath = audioPath.TrimEnd(trimChar);
                }
            }
            len = (uint)data.Length;
            if (0 == PInvoke.RegQueryValueExW(hKey, "ConfigPath", 0, ref type, data, ref len))
            {
                if (type == PInvoke.REG_SZ)
                {
                    configPath = System.Text.UnicodeEncoding.Unicode.GetString(data, 0, (int)len);
                    configPath = configPath.TrimEnd(trimChar);
                }
            }
            len = (uint)data.Length;
            if (0 == PInvoke.RegQueryValueExW(hKey, "MediaPath", 0, ref type, data, ref len))
            {
                if (type == PInvoke.REG_SZ)
                {
                    mediaPath = System.Text.UnicodeEncoding.Unicode.GetString(data, 0, (int)len);
                    mediaPath = mediaPath.TrimEnd(trimChar);
                }
            }
            len = (uint)data.Length;
            if (0 == PInvoke.RegQueryValueExW(hKey, "ShowCursor", 0, ref type, data, ref len))
            {
                if (type == PInvoke.REG_DWORD)
                {
                    if (1 == (byte)data.GetValue(0))
                    {
                        showCursor = true;
                    }
                }
            }
            len = (uint)data.Length;
            if (0 == PInvoke.RegQueryValueExW(hKey, "ApplicationSettingsFile", 0, ref type, data, ref len))
            {
                if (type == PInvoke.REG_SZ)
                {
                    ApplicationSettingsFile = System.Text.UnicodeEncoding.Unicode.GetString(data, 0, (int)len);
                    ApplicationSettingsFile = ApplicationSettingsFile.TrimEnd(trimChar);
                }
            }
            len = (uint)data.Length;
            if (0 == PInvoke.RegQueryValueExW(hKey, "ExtensionAssembly", 0, ref type, data, ref len))
            {
                if (type == PInvoke.REG_SZ)
                {
                    ExtensionAssembly = System.Text.UnicodeEncoding.Unicode.GetString(data, 0, (int)len);
                    ExtensionAssembly = ExtensionAssembly.TrimEnd(trimChar);
                    if (ExtensionAssembly.Length > 0 && -1 == ExtensionAssembly.IndexOf('\\'))
                    {
                        ExtensionAssembly = Path.Combine(Path.GetDirectoryName(new Uri(Assembly.GetExecutingAssembly().GetName().CodeBase).LocalPath), ExtensionAssembly);
                    }
                }
            }   
        }

        /// <summary>
        /// Flag indicating RAP Scanner is configured on the system.
        /// </summary>
        private bool? rapScannerConfigured = null;

        /// <summary>
        /// Flag indicating if we should use the alphanumeric keyboard.
        /// </summary>
        private bool alphaNumericKeyboard = false;

        /// <summary>
        /// Flag indicating if we send encoded id/pw to lane during signon.
        /// </summary>
        private bool encodeOperatorIDPW = false;

		/// <summary>
		/// Flag indicating if EnterId screen is initially uppercase.
		/// </summary>
		private bool enterIdUppercase = false;

		/// <summary>
		/// Flag indicating if EnterPassword screen is initially uppercase.
		/// </summary>
		private bool enterPasswordUppercase = false;
		
		/// <summary>
		/// Allow the Alt-F4 keystroke entry to exit the application? (default is true).
		/// </summary>
		private bool altF4ExitAllowed = true;

		/// <summary>
		/// Allow multiple Exclusive Lanes simultaneously.
		/// ie. Interventions from a 2nd lane can be processed during 
		/// Interventions from the 1st Lane, without exiting exclusive lane
		/// mode on the 1st Lane.  Intervention handling on the 1st lane
		/// is simply suspended temporarily, until the 2nd lane has 
		/// exited exclusive lane mode.  
		/// </summary>
		static protected bool multiExclusiveLanes = false;
        
        /// <summary>
        /// Full path to audio files.
        /// </summary>
        static private string audioPath = String.Empty;

        /// <summary>
        /// Computer name.
        /// </summary>
        private string computerName = String.Empty;
        
        /// <summary>
		/// Full path to config files.
		/// </summary>
		static private string configPath = String.Empty;

        /// <summary>
        /// Rapnet config filename.
        /// </summary>
        private string rapNetConfigFileName = "RapNet.xml";

        /// <summary>
        /// The full path to the Rapnet config file that is currently loaded.
        /// </summary>
        private string loadedConfigFilePath = "";

        /// <summary>
        /// Time in ms between Psx ConnectRemote invocations.
        /// </summary>
        private int connectionPeriod = 5000;

        /// <summary>
        /// Timeout in ms for Psx ConnectRemote operation.
        /// </summary>
        private int connectRemoteTimeout = 5000;

        /// <summary>
        /// Sound played when a delayed intervention is received.
        /// </summary>
        private string delayedInterventionSound = String.Empty;
        
        /// <summary>
        /// Sound played when a immediate intervention is received.
        /// </summary>
        private string immediateInterventionSound = String.Empty;

        /// <summary>
        /// Sound played when a immediate intervention is received and not cleared within x seconds by the attendant.
        /// </summary>
        private string immediateInterventionSoundSecondary = String.Empty;

        /// <summary>
        /// Delay in ms before an intervention sound is played.
        /// </summary>
        private int interventionSoundDelay = 2000;

        /// <summary>
        /// Delay in ms before a secondary intervention sound is played. Immediate intervention only. Used if sound is not cleared by attendant.
        /// </summary>
        private int interventionSoundDelaySecondary = 30000;

        /// <summary>
        /// Configured number of Lanes to be shown for RAPLite
        /// </summary>
        private int maxNumberOfLanesShownForRAPLite = 5;

        /// <summary>
        /// Configure/Enable this device as RAPLite 
        /// </summary>
        private bool bEnableRAPLite = false;

        /// <summary>
		/// List of FastLanes.
		/// </summary>
		private ArrayList laneConfigList = new ArrayList();		

		/// <summary>
		/// Full path to bin files.
		/// </summary>
		static private string binPath = "%APP_DRIVE%\\scot\\Bin";

		/// <summary>
		/// Full path to report data files.
		/// </summary>
		static private string reportDataPath = "%DATA_DRIVE%\\scot\\Report";

		/// <summary>
		/// Full path to media files.
		/// </summary>
		static private string mediaPath = String.Empty;

        /// <summary>
        /// Sound played when a barcode is scanned.
        /// </summary>
        private string scanSound = String.Empty;
        
        /// <summary>
        /// Flag indicating if we should show the mouse cursor.
        /// </summary>
        static private bool showCursor = false;
        
        /// <summary>
        /// Timeout in ms for sign on.
        /// </summary>
        private int signOnTimeout = 5000;

        /// <summary>
        /// Flag indicating if we should synchronize our time with FastLane's during signon.
        /// </summary>
        private bool synchronizeTimeServer = false;

		/// <summary>
		/// Flag indicating if we should display the transaction count
		/// </summary>
		private bool displayTransactionCount = true;

		/// <summary>
		/// Time in minutes for the calculation of the throughput.
		/// </summary>
		private int timePeriodForThroughputCalculation = 60;
		
		/// <summary>
		/// Transaction count threshold for displaying high alert color
		/// </summary>
		private int trxCountThresholdForHighAlert = 20;
	
		/// <summary>
		/// Transaction count threshold for displaying caution color
		/// </summary>
		private int trxCountThresholdForCaution = 40;
		
		/// <summary>
		/// The one and only ConfigData instance.
		/// </summary>
		private static ConfigData theConfigData;        
      
		/// <summary>
		/// Has a Biometric device? (default is false)
		/// </summary>
		private bool biometricDeviceConfigured = false;

		/// <summary>
		/// Build data model from config xml file, and use it to clone data for other PSX instances
		/// </summary>
		private bool dataModel = false;

        /// <summary>
        /// Timeout in ms for Lane response (AssistMode, LaneStatus).
        /// </summary>
        private int laneResponseTimeout = 4000;

        /// <summary>
        /// Hash algorithm type ("NONE", "SHA1Button", "SHA256Button", "SHA384Button", "SHA512Button", "MD5Button")
        /// </summary>
        private string hashAlgorithm = "NONE";

        /// <summary>
        /// Encrypts scan data
        /// </summary>        
        private bool encryptScanData = true;

        /// <summary>
        /// Application registry path.
        /// </summary>
        private const string RegistryPath = "SOFTWARE\\NCR";
        
        /// <summary>
        /// SWUsers (ID/Password) registry path.
        /// </summary>
        private const string swUsersRegistryPath = "SOFTWARE\\NCR\\SCOT\\CurrentVersion\\Switch\\SWUsers";        

#if( WINCE )
		private const string regApiDll = "coredll.dll";
		private const string winsockDll = "coredll.dll";
#else
        private const string regApiDll = "advapi32.dll";
        private const string winsockDll = "ws2_32.dll";
#endif

		[DllImport(regApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
		private static extern int RegOpenKeyExW( uint hkey, string subKey, uint options, uint sam, ref uint phkResult );

		[DllImport(regApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
		private static extern int RegQueryValueExW( uint hkey, string valueName, int reserved, ref uint type, byte[] data, ref uint len );

		[DllImport(regApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
		private static extern int RegCloseKey( uint hkey );
	}
}
