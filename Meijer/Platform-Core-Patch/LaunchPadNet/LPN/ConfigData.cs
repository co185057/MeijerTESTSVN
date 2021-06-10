using System;
using System.Collections;
using System.Runtime.InteropServices;
using System.Text;
using System.Xml;
using System.Text.RegularExpressions;

#if ( !WINCE )
using Microsoft.Win32;
#endif
#if ( WINCE )
using System.Net;
#endif 
using RPSWNET;

namespace LaunchPadNet
{
	/// <summary>
	/// Configuration data singleton used by the application.
	/// </summary>
	internal class ConfigData
	{
		/// <summary>
		/// The class constructor.
		/// </summary>
		protected ConfigData()
		{
			Load();	
		}

		/// <summary>
		/// The class destructor.
		/// </summary>
		~ConfigData()
		{
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
		/// Allow the Alt-F4 keystroke entry to exit the application? (default is false).
		/// </summary>
		public bool AltF4ExitAllowed
		{
			get
			{
				return this.altF4ExitAllowed; 
			}
			set
			{
				this.altF4ExitAllowed = value;
			}
		}

        /// <summary>
        /// Determine if current system is configured as ScotPC through Environment Variable.
        /// </summary>
        public bool IsScotPC
        {
            get
            {
                return this.isScotPC; 
            }
            set
            {
                this.isScotPC = value;
            }
        }
        
		/// <summary>
		/// Allow return to ScotApp/RapNet when a utility executable is running? (default is false).
		/// </summary>
		public bool UtilityReturnAlwaysEnabled
		{
			get
			{
				return this.utilityReturnAlwaysEnabled; 
			}
			set
			{
				this.utilityReturnAlwaysEnabled = value;
			}
		}

		/// <summary>
		/// Is it required to display the legal caption prior to the sign on menu? (default is false).
		/// </summary>
		public bool LegalCaptionRequired
		{
			get
			{
				return this.legalCaptionRequired; 
			}
			set
			{
				this.legalCaptionRequired = value;
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

		public int AutoSignOffSeconds
		{
			get
			{
				return autoSignOffSeconds;
			}
		}

		private int autoSignOffSeconds = 0;


        /// <summary>
        /// Read-Only ComputerName property accessor.
        /// </summary>
        public string ComputerName
        {
            get
            {
                if( 0 == computerName.Length )
                {
                    StringBuilder buffer = new StringBuilder( 100 );
                    int len = buffer.Capacity;
#if ( WINCE )
					computerName = Dns.GetHostName();
#else
                    if( 0 == gethostname( buffer, len ) )
                    {
                        computerName = buffer.ToString();
                    }
#endif 

                }
                
                return computerName;
            }
        }

		private static bool bHasKeyLockHW = false;

		/// <summary>
		/// returns true if terminal has keylock hardware
		/// </summary>
		public static bool HasKeyLockHW 
		{
			get { return bHasKeyLockHW; }
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
		/// Read-Only ConnectionPeriod property accessor.
		/// </summary>
		public string ConfigXML
		{
			get
			{
				return configXML;
			}
		}        /// <summary>
		
		/// Read-Only ConnectionPeriod property accessor.
		/// </summary>
		public string PsxConfigXML
		{
			get
			{
				return psxConfigXML;
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
		/// Timeout in ms for the application to shutdown. If the application is not dead after this time, call process.kill().
		/// </summary>
		public int ShutDownTimeout
		{
			get
			{
				return shutDownTimeout;
			}
		}

        /// <summary>
		/// Get the one and only ConfigData instance.
		/// </summary>
		/// <returns>
		/// ConfigData instance.
		/// </returns>
		public static ConfigData GetConfigData()
		{
			if( theConfigData == null )
			{
				theConfigData = new ConfigData();
			}

			return theConfigData;
		}
        
        public bool IsKeyLockConfigured()
        {
            uint HKEY_LOCAL_MACHINE = 0x80000002;
            uint KEY_READ = 0x00020019;
            
            uint hkey = 0;

            bool bReturn = false;
            if( 0 == RegOpenKeyExW( HKEY_LOCAL_MACHINE, "SOFTWARE\\NCR\\DeviceManagerEx\\Devices\\Local\\Keylock\\SCOTSwitch", 0, KEY_READ, ref hkey ) )
            {
                char[] trimChar = { '\0' };
                uint type = 0;
                byte[] data = new byte[1024];
                uint len = (uint)data.Length;

                if( 0 == RegQueryValueExW( hkey, "Configure", 0, ref type, data, ref len ) )
                {
                    string isConfigure = "";
                    isConfigure = System.Text.UnicodeEncoding.Unicode.GetString( data, 0, (int)len );
                    isConfigure = isConfigure.ToUpper().TrimEnd( trimChar );
                    if  ( ( isConfigure == "YES" ) )
                    {
                        bReturn = true;
                    }
                }

                RegCloseKey( hkey );
            }
            // if there are not Actual Keylock Switch, check Emulator
            if( !bReturn && 0 == RegOpenKeyExW( HKEY_LOCAL_MACHINE, "SOFTWARE\\NCR\\DeviceManagerEx\\Devices\\Local\\Keylock\\Emulator", 0, KEY_READ, ref hkey ) )
            {
                char[] trimChar = { '\0' };
                uint type = 0;
                byte[] data = new byte[1024];
                uint len = (uint)data.Length;

                if( 0 == RegQueryValueExW( hkey, "Configure", 0, ref type, data, ref len ) )
                {
                    string isConfigure = "";
                    isConfigure = System.Text.UnicodeEncoding.Unicode.GetString( data, 0, (int)len );
                    isConfigure = isConfigure.ToUpper().TrimEnd( trimChar );
                    if  ( ( isConfigure == "YES" ) )
                    {
                        bReturn = true;
                    }
                }

                RegCloseKey( hkey );
            }
            return bReturn;
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
			// Replace the drive partition env. vars with "C:" (or "" for WINCE).
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
        public static string ExtractRapEventData( string key, string eventData )
        {
			string strVal = null;			
			try
			{
				string  strSearchKey = key;
				int nIndex = eventData.IndexOf( strSearchKey );														
				if( nIndex > -1 )
				{
					int nTemp = eventData.IndexOf( "=", nIndex );			
					strSearchKey = eventData.Substring( nIndex, (nTemp - nIndex) + 1 );

					int nStart = nIndex + strSearchKey.Length;			
					int nEnd = eventData.IndexOf( ";", nStart );
					if( nEnd > -1 )
					{
						strVal = eventData.Substring( nStart, nEnd - nStart );
					}
					else
					{
						strVal = eventData.Substring( nStart );
					}
				}						
			}
			catch(Exception ex)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "ConfigData::ExtractRapEventData() Exception: " + ex.Message );
				strVal= "";				
			}
			return strVal;            			
        }
        
        /// <summary>
        /// Read-Only FastlaneService property accessor.
        /// </summary>
        public string FastlaneService
        {
            get
            {
                return fastlaneService;
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
			set
			{
				laneConfigList = value;
			}
		}
		
        /// <summary>
		/// Loads configuration data from persistant storage.
		/// </summary>
		public void Load()
		{
			// Get registry settings //
            uint HKEY_LOCAL_MACHINE = 0x80000002;
            uint KEY_READ = 0x00020019;
			uint REG_SZ = 1;
			const uint REG_DWORD = 4;  
                        
            uint hkey = 0;
            if( 0 == RegOpenKeyExW( HKEY_LOCAL_MACHINE, RegistryPath, 0, KEY_READ, ref hkey ) )
            {
                char[] trimChar = { '\0' };
                uint type = 0;
                byte[] data = new byte[1024];
                uint len = (uint)data.Length;
                if( 0 == RegQueryValueExW( hkey, "AudioPath", 0, ref type, data, ref len ) )
                {
                    if( type == REG_SZ )
                    {
                        audioPath = System.Text.UnicodeEncoding.Unicode.GetString( data, 0, (int)len );
                        audioPath = audioPath.TrimEnd( trimChar );
                    }
                }
                len = (uint)data.Length;
                if( 0 == RegQueryValueExW( hkey, "ConfigPath", 0, ref type, data, ref len ) )
                {
                    if( type == REG_SZ )
                    {
                        configPath = System.Text.UnicodeEncoding.Unicode.GetString( data, 0, (int)len );
                        configPath = configPath.TrimEnd( trimChar );
                    }
                }
                len = (uint)data.Length;
                if( 0 == RegQueryValueExW( hkey, "MediaPath", 0, ref type, data, ref len ) )
                {
                    if( type == REG_SZ )
                    {
                        mediaPath = System.Text.UnicodeEncoding.Unicode.GetString( data, 0, (int)len );
                        mediaPath = mediaPath.TrimEnd( trimChar );
                    }
                }

				// dtb START
				if( 0 == RegQueryValueExW( hkey, "BinPath", 0, ref type, data, ref len ) )
				{
					if( type == REG_SZ )
					{
						binPath = System.Text.UnicodeEncoding.Unicode.GetString( data, 0, (int)len );
						binPath = binPath.TrimEnd( trimChar );
					}
				}
				len = (uint)data.Length;

				// Resolve any environment variables that may exist within the ???Path variables.
				try
				{
					audioPath = ExpandPartitionVars(audioPath);
					configPath = ExpandPartitionVars(configPath);
					mediaPath = ExpandPartitionVars(mediaPath);
					binPath = ExpandPartitionVars(binPath);
				}
				catch( ArgumentNullException argE )
				{
					CmDataCapture.Capture( CmDataCapture.MaskError, "ConfigData::Load() - ArgumentNullException: " + argE.Message );
				}

                // Collect the OS data
                OperatingSystem OS = Environment.OSVersion;
                string tmpStr1="";
                string tmpStr2="";
                if(( OS.Platform == PlatformID.Win32NT)&&(OS.Version.Major == 5))
                {
                    tmpStr1 = "Config_XP";
                    tmpStr2 = "PSXConfig_XP";
                }
                else if(( OS.Platform == PlatformID.Win32NT)&&(OS.Version.Major == 6))
                {
                    tmpStr1 = "Config_WIN7";
                    tmpStr2 = "PSXConfig_WIN7";
                }
                else if( OS.Platform == PlatformID.WinCE)
                {
                    tmpStr1 = "Config_CE";
                    tmpStr2 = "PSXConfig_CE";
                }
                else
                {
                    tmpStr1 = "Config_NT";
                    tmpStr2 = "PSXConfig_NT";
                }
				if( 0 == RegQueryValueExW( hkey, tmpStr1, 0, ref type, data, ref len ) )
				{
					if( type == REG_SZ )
					{
						configXML = System.Text.UnicodeEncoding.Unicode.GetString( data, 0, (int)len );
						configXML = configXML.TrimEnd( trimChar );
					}
				}
				len = (uint)data.Length;
				if( 0 == RegQueryValueExW( hkey, tmpStr2, 0, ref type, data, ref len ) )
				{
					if( type == REG_SZ )
					{
						psxConfigXML = System.Text.UnicodeEncoding.Unicode.GetString( data, 0, (int)len );
						psxConfigXML = psxConfigXML.TrimEnd( trimChar );
					}
				}
				len = (uint)data.Length;
				if( -1 == configXML.IndexOf("\\") && ! configPath.Equals ("")) 
					configXML = configPath + "\\" + configXML;     
      
				if( -1 == psxConfigXML.IndexOf("\\") && ! configPath.Equals("") )
					psxConfigXML = configPath + "\\" + psxConfigXML;      
     
				len = (uint)data.Length;
				if( 0 == RegQueryValueExW( hkey, "ShowCursor", 0, ref type, data, ref len ) )
				{
					if( type == REG_DWORD )
					{
						if( 1 == (byte)data.GetValue( 0 ) )
						{
							showCursor = true;
						}
					}
				}
				// dtb END

                RegCloseKey( hkey );
            }

			// Collect other SCOTTB registry entries
			if( 0 == RegOpenKeyExW( HKEY_LOCAL_MACHINE, scotTBRegistryPath, 0, KEY_READ, ref hkey ) )
			{
				char[] trimChar = { '\0' };
				uint type = 0;
				byte[] data = new byte[1024];

				// Get Terminal ID
				uint len = (uint)data.Length;
				if( 0 == RegQueryValueExW( hkey, "TerminalNumber", 0, ref type, data, ref len ) )
				{
					if( type == REG_SZ )
					{
						terminalId = System.Text.UnicodeEncoding.Unicode.GetString( data, 0, (int)len );
						terminalId = terminalId.TrimEnd( trimChar );
					}
				}

				// Get Software Version	Rfc 343789	
				len = (uint)data.Length;
				if( 0 == RegQueryValueExW( hkey, "InterfaceVersion", 0, ref type, data, ref len ) )
				{
					if( type == REG_SZ )
					{
						interfaceVersion = System.Text.UnicodeEncoding.Unicode.GetString( data, 0, (int)len );
						interfaceVersion = interfaceVersion.TrimEnd( trimChar );
					}
				}
				RegCloseKey( hkey );
			}

			// Find out if this is 3.0 Hardware or not 
			// if it is, assume it will have a keylock.
			// Unfortunately, the keylock specific registry settings
			// have no indication whether a button or a real keylock 
			// exists.  
			if( 0 == RegOpenKeyExW( HKEY_LOCAL_MACHINE, hardwareTypeRegistryPath, 0, KEY_READ, ref hkey ) )
			{
				char[] trimChar = { '\0' };
				uint type = 0;
				byte[] data = new byte[1024];
				uint len = (uint)data.Length;

				if( 0 == RegQueryValueExW( hkey, "Type", 0, ref type, data, ref len ) )
				{
					string hwType = "";
					hwType = System.Text.UnicodeEncoding.Unicode.GetString( data, 0, (int)len );
					hwType = hwType.ToUpper().TrimEnd( trimChar );

					if	( ( hwType == "SCOT3" ) 
							|| ( hwType == "MINI" ) 
							|| ( hwType == "SCOTA" )	// 2.0 HW Types 
							|| ( hwType == "SCOTB" ) 
							|| ( hwType == "SCOTC" ) 
							|| ( hwType == "SCOTD" ) 
							|| ( hwType == "SCOTE" ) 
							|| ( hwType == "SCOTF" ) )
					{
						bHasKeyLockHW = true;							
					}
				}

				RegCloseKey( hkey );
			}

            //////////////////////////
            // Load XML settings //
            try
            {
                XmlDocument myXmlDocument = new XmlDocument();
                myXmlDocument.Load( configPath + "\\LaunchPad.xml" );

				XmlNodeList nodeList = myXmlDocument.GetElementsByTagName( "Lane" );
				foreach( XmlNode node in nodeList )
				{
					string hostNode = "";
					string ipNode = "";
					foreach( XmlNode childNode in node.ChildNodes )
					{
						if( childNode.Name == "Host" )
						{
							hostNode = childNode.InnerText;
						}
						else if( childNode.Name == "IPAdrs" )
						{
							ipNode = childNode.InnerText;
						}
					}
					if( ipNode.Length == 0 )
					{
						laneConfigList.Add( hostNode );
					}
					else
					{
						laneConfigList.Add( ipNode );
					}
				}

                nodeList = myXmlDocument.GetElementsByTagName( "AlphaNumericKeyboard" );
                if( nodeList.Count >= 1 )
                {
                    alphaNumericKeyboard = Convert.ToBoolean( nodeList[0].InnerText );
				}

                nodeList = myXmlDocument.GetElementsByTagName("EncodeOperatorIDPW");
                if (nodeList.Count >= 1)
                {
                    encodeOperatorIDPW = Convert.ToBoolean(nodeList[0].InnerText);
                }

				nodeList = myXmlDocument.GetElementsByTagName( "EnterIdUppercase" );
				if( nodeList.Count >= 1 )
				{
					enterIdUppercase = Convert.ToBoolean( nodeList[0].InnerText );
				}

				nodeList = myXmlDocument.GetElementsByTagName( "EnterPasswordUppercase" );
				if( nodeList.Count >= 1 )
				{
					enterPasswordUppercase = Convert.ToBoolean( nodeList[0].InnerText );
				}

				// Allow the Alt-F4 keystroke entry to exit the application? (default is false)
				nodeList = myXmlDocument.GetElementsByTagName( "AltF4ExitAllowed" );
				if( nodeList.Count >= 1 )
				{
					AltF4ExitAllowed = Convert.ToBoolean( nodeList[0].InnerText );
				}

				// Allow return to ScotApp/RapNet when a utility executable is running? (default is false)
				nodeList = myXmlDocument.GetElementsByTagName( "UtilityReturnAlwaysEnabled" );
				if( nodeList.Count >= 1 )
				{
					utilityReturnAlwaysEnabled = Convert.ToBoolean( nodeList[0].InnerText );
				}

				// Hash algorithm type ("NONE", "SHA1Button", "SHA256Button", "SHA384Button", "SHA512Button", "MD5Button")
				nodeList = myXmlDocument.GetElementsByTagName( "HashAlgorithm" );
				if( nodeList.Count >= 1 )
				{
					hashAlgorithm = Convert.ToString( nodeList[0].InnerText );
				}

				// Launch Pad scanner profile (default: SOFTWARE\\NCR\\NcrLaunchPadRemoteScanner)
				nodeList = myXmlDocument.GetElementsByTagName( "NcrRemoteScanner  " );
				if( nodeList.Count >= 1 )
				{
					ncrRemoteScanner  = Convert.ToString( nodeList[0].InnerText );
				}

				// Is it required to display the legal caption prior to the sign on menu? (default is false).
				nodeList = myXmlDocument.GetElementsByTagName( "LegalCaptionRequired" );
				if( nodeList.Count >= 1 )
				{
					LegalCaptionRequired = Convert.ToBoolean( nodeList[0].InnerText );
				}

                nodeList = myXmlDocument.GetElementsByTagName( "SignOnTimeout" );
                if( nodeList.Count >= 1 )
                {
                    signOnTimeout = Convert.ToInt32( nodeList[0].InnerText );
                }

                nodeList = myXmlDocument.GetElementsByTagName( "ConnectionPeriod" );
                if( nodeList.Count >= 1 )
                {
                    connectionPeriod = Convert.ToInt32( nodeList[0].InnerText );
                }

                nodeList = myXmlDocument.GetElementsByTagName( "ShutDownTimeout" );
                if( nodeList.Count >= 1 )
                {
                    shutDownTimeout = Convert.ToInt32( nodeList[0].InnerText );
                }

                nodeList = myXmlDocument.GetElementsByTagName( "ConnectRemoteTimeout" );
                if( nodeList.Count >= 1 )
                {
                    connectRemoteTimeout = Convert.ToInt32( nodeList[0].InnerText );
                }
                
                nodeList = myXmlDocument.GetElementsByTagName( "SynchronizeTimeServer" );
                if( nodeList.Count >= 1 )
                {
                    synchronizeTimeServer = Convert.ToBoolean( nodeList[0].InnerText );
                }

				nodeList = myXmlDocument.GetElementsByTagName( "ScanSound" );
				if( nodeList.Count >= 1 )
				{
					scanSound = nodeList[0].InnerText;
				}

				nodeList = myXmlDocument.GetElementsByTagName( "SignOnTBFirst" );
				if( nodeList.Count >= 1 )
				{
					signonTBFirst = Convert.ToBoolean( nodeList[0].InnerText );
				}

				nodeList = myXmlDocument.GetElementsByTagName( "LaunchPadRAPAutoSignOffSeconds" );
				if( nodeList.Count >= 1 )
				{
					autoSignOffSeconds = Convert.ToInt32( nodeList[0].InnerText );
				}

                 #region Meijer customization begin

                nodeList = myXmlDocument.GetElementsByTagName("UseBiometrics");
                if (nodeList.Count >= 1)
                {
                    UseBiometrics = Convert.ToBoolean(nodeList[0].InnerText);
                }

                nodeList = myXmlDocument.GetElementsByTagName("AllowEmplIdSignOn");
                if (nodeList.Count >= 1)
                {
                    AllowEmplIdSignOn = Convert.ToBoolean(nodeList[0].InnerText);
                }
                nodeList = myXmlDocument.GetElementsByTagName("AllowHandKeyEMPLID");
                if (nodeList.Count >= 1)
                {
                    AllowHandKeyEMPLID = Convert.ToBoolean(nodeList[0].InnerText);
                }
                #endregion Meijer customization end
				
#if	( !WINCE )
				nodeList = myXmlDocument.GetElementsByTagName( "BiometricDeviceConfigured" );
				if( nodeList.Count >= 1 )
				{
					biometricDeviceConfigured = Convert.ToBoolean( nodeList[0].InnerText );
				}
#endif				
            }
            catch( XmlException caught )
            {
                CmDataCapture.Capture( CmDataCapture.MaskError, "ConfigData::Load() - XmlException: " + caught.Message );
			}
#if ( !WINCE )
            // get Environment Variable Data: ScotPC
            String csScotPC = Environment.GetEnvironmentVariable("SCOTPC");
            if (csScotPC != null && csScotPC == "Y")
                IsScotPC = true;
#endif
            // Expand to remove %DATA_PATH%
			reportDataPath = ExpandPartitionVars(reportDataPath);
		}
 
		/// <summary>
		/// Validate the ID/Password against the Windows registry.
		/// </summary>
		public bool ValidateOperatorRegistry( string id, string password, int keyType )
		{
			// Get registry settings //
			uint HKEY_LOCAL_MACHINE = 0x80000002;
			uint KEY_READ = 0x00020019;
			uint REG_SZ = 1;
			uint hkey = 0;
			bool rVal = false;

			string keyPath = swUsersRegistryPath;
			if	( keyType == MAINTUSER )
			{
				keyPath	= maintUsersRegistryPath;
			}

			if( 0 == RegOpenKeyExW( HKEY_LOCAL_MACHINE, keyPath, 0, KEY_READ, ref hkey ) )
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

#if ( !WINCE )

        /// <summary>
        /// Application registry path.
        /// </summary>
		private const string WinloginRegistryPath = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon";

        /// <summary>
		/// Set a registry key value for LOCAL MACHINE.
		/// </summary>
		public bool SetStringRegistry( string keyPath, string keyName, string strData )
		{
			bool rVal = false;

			CmDataCapture.Capture( CmDataCapture.MaskError, "ConfigData::SetStringRegistry() - Attempt write to registry:" + @keyPath + "=" + keyName );

            RegistryKey rk = null;
            
            //Check to see if the OS is 64-bit 
            if (Environment.Is64BitOperatingSystem)
            {
                //Open 64-bit registry hive
                rk = RegistryKey.OpenBaseKey(Microsoft.Win32.RegistryHive.LocalMachine, RegistryView.Registry64);

                //Create Subkey under 64-bit registry hive
                if (rk == null)
                {
                    CmDataCapture.Capture(CmDataCapture.MaskError, "ConfigData::SetStringRegistry() - Unable to write to 64bit registry hive");
                }
                else
                {
                    rk = rk.CreateSubKey(@keyPath);
                }
            }
            else
            {
                rk = Registry.LocalMachine.CreateSubKey(@keyPath);
            }
			
			// Write registry information
			if ( rk == null )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "ConfigData::SetStringRegistry() - Unable to write to registry:" + @keyPath + "=" + keyName );
			}
			else
			{
				try
				{
					rk.SetValue( keyName, strData );
					rVal = true;
				}
				finally
				{
					rk.Close();
				}
			}
			return rVal;
		}

        /// <summary>
		/// Set a registry key value for LOCAL MACHINE.
		/// </summary>
		public bool SetWindowsLogOn( string id, string password )
		{
			bool bResult = false;

			try
			{
				SetStringRegistry( WinloginRegistryPath, "DefaultDomainName", "" );
				SetStringRegistry( WinloginRegistryPath, "DefaultUserName", id );
				SetStringRegistry( WinloginRegistryPath, "DefaultPassword", password );
				SetStringRegistry( WinloginRegistryPath, "AutoAdminLogon", "1" );
				SetStringRegistry( WinloginRegistryPath, "ForceAutoLogon", "1" );
                bResult = true;
			}
			catch  ( Exception e )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "SetWindowsLogOn: EXCEPTION " + e.Message );
				bResult = false;
			}

			return bResult;
		}
#endif

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
        /// Read-Only RegistryPath property accessor.
        /// </summary>
        public string RegistryPath
        {
            get
            {
                return registryPath;
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
		/// Read-Only Terminal ID accessor.
		/// </summary>
		public string TerminalId
		{
			get
			{
				return terminalId;
			}
		}

		/// <summary>
		/// Read-Only Interface Version accessor	//Rfc 343789
		/// </summary>
		public string InterfaceVersion
		{
			get
			{
				return interfaceVersion;
			}
		}

		/// <summary>
		/// Read- Only - True by default - 
		/// Signon to TB first, if timed out, try registry
		/// </summary>
		public bool SignOnTBFirst
		{
			get
			{
				return signonTBFirst;
			}
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
		/// Launch Pad scanner profile (default: SOFTWARE\\NCR\\NcrLaunchPadRemoteScanner)
		/// </summary>
		public string NcrRemoteScanner 
		{
			get
			{
				return ncrRemoteScanner ;
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
        /// Name of Rap event.
        /// </summary>
        public const string eventRap = "RemoteData";
        
		/// <summary>
		/// Rap event operation.
		/// </summary>
		public const string eventRapOperation = "operation";
        
		/// <summary>
		/// Module name of the RAP Application
		/// </summary>
		public const string rapApplication = "RapNet.exe";

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
        /// Allow the Alt-F4 keystroke entry to exit the application? (default is false).
        /// </summary>
        private bool altF4ExitAllowed=false;
		
        /// <summary>
        /// Determine if current system is configured as ScotPC through Environment Variable.
        /// </summary>
        private bool isScotPC=false;
        
		/// <summary>
		/// Allow return to ScotApp/RapNet when a utility executable is running? (default is false).
		/// </summary>
		private bool utilityReturnAlwaysEnabled=false;

		/// <summary>
		/// Is it required to display the legal caption prior to the sign on menu? (default is false).
		/// </summary>
		private bool legalCaptionRequired=false;

        /// <summary>
        /// Full path to audio files.
        /// </summary>
        private string audioPath = "";

        /// <summary>
        /// Computer name.
        /// </summary>
        private string computerName = "";
        
        /// <summary>
        /// Time in ms between Psx ConnectRemote invocations.
        /// </summary>
        private int connectionPeriod = 5000;

        /// <summary>
        /// Timeout in ms for Psx ConnectRemote operation.
        /// </summary>
		private int connectRemoteTimeout = 5000;

		/// <summary>
		/// Timeout in ms for the application to shutdown. If the application is not dead after this time, call process.kill().
		/// </summary>
		private int shutDownTimeout = 15000;

        /// <summary>
        /// FastLane Psx service.
        /// </summary>
        private const string fastlaneService = "FastLaneRemoteServer";

		/// <summary>
		/// List of FastLanes.
		/// </summary>
		private ArrayList laneConfigList = new ArrayList();		
		
		/// <summary>
		/// Full path to bin files.
		/// </summary>
		private string binPath = "%APP_DRIVE%\\scot\\Bin";

		/// <summary>
		/// Full path to report data files.
		/// </summary>
		private string reportDataPath = "%DATA_DRIVE%\\scot\\Report";
		
		/// <summary>
		/// Full path to config files.
		/// </summary>
		private string configPath = "%APP_DRIVE%\\scot\\config";

		/// <summary>
		/// Full path to media files.
		/// </summary>
		private string mediaPath = "%APP_DRIVE%\\scot\\image";

		/// <summary>
		/// LaunchPadNet configuration filename.
		/// </summary>
		private string configXML = "LaunchPadConfig_XP.xml";
		
		/// <summary>
		/// LaunchPadNet PSX display filename.
		/// </summary>
		private string psxConfigXML = "LaunchPadPSX.xml";

		/// <summary>
		/// Sound played when a barcode is scanned.
		/// </summary>
		private string scanSound = String.Empty;

        /// <summary>
        /// Application registry path.
        /// </summary>
		private const string registryPath = "SOFTWARE\\NCR\\LaunchPadNet";

		/// <summary>
		/// SCOTTB registry path.
		/// </summary>
		private const string scotTBRegistryPath = "SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTTB";

		/// <summary>
		/// SWUsers (ID/Password) registry path.
		/// </summary>
		private const string swUsersRegistryPath = "SOFTWARE\\NCR\\SCOT\\CurrentVersion\\Switch\\SWUsers";

		/// <summary>
		/// SWUsers (ID/Password) registry path.
		/// </summary>
		private const string maintUsersRegistryPath = "SOFTWARE\\NCR\\SCOT\\CurrentVersion\\Switch\\MaintUsers";

		/// <summary>
		/// Reg string of the hardware type we are installed on:
		/// expected values: "MINI", "SCOT3", "MINI4", "SCOT4"
		/// </summary>
		private const string hardwareTypeRegistryPath = "SOFTWARE\\NCR\\SCOT - Platform\\ObservedOptions";

		/// <summary>
		/// Load controller registry path.
		/// </summary>
		private const string loadControllerRegistryPath = "SOFTWARE\\NCR\\SCOT\\CurrentVersion\\Load Controller";

        /// <summary>
        /// Timeout in ms for sign on.
        /// </summary>
        private int signOnTimeout = 5000;

		public const int SWUSER	  = 0;
		public const int MAINTUSER = 1;
		
		/// <summary>
		/// The one and only ConfigData instance.
		/// </summary>
		private static ConfigData theConfigData;

        /// <summary>
		/// Flag indicating if we should show the mouse cursor.
		/// </summary>
		private bool showCursor = false;

		/// <summary>
		/// Hash algorithm type ("NONE", "SHA1Button", "SHA256Button", "SHA384Button", "SHA512Button", "MD5Button")
		/// </summary>
		private string hashAlgorithm = "NONE";

		/// <summary>
		/// Launch Pad scanner profile (default: SOFTWARE\\NCR\\NcrLaunchPadRemoteScanner)
		/// </summary>
		private string ncrRemoteScanner = "SOFTWARE\\NCR\\NcrLaunchPadRemoteScanner";

		/// <summary>
		/// Flag indicating if we should synchronize our time with FastLane's during signon.
		/// </summary>
		private bool synchronizeTimeServer = false;
		
		/// <summary>
		/// Name of Terminal ID.
		/// </summary>
		private string terminalId = "";

		/// <summary>
		/// Version of the interface	// Rfc 343789
		/// </summary>
		private string interfaceVersion = "";

		/// <summary>
		/// Signon TB first - default to true
		/// </summary>
		private bool signonTBFirst = true;

		/// <summary>
		/// Has a Biometric device? (default is false)
		/// </summary>
		private bool biometricDeviceConfigured = false;

        /// <summary>
        /// returns picklist editor window name
        /// </summary>
        public static string GetPickListEditor 
        {
            get { return "Pick List Editor"; }
        }

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

#if( WINCE )
        [DllImport(winsockDll, SetLastError=false)]
#else
        [DllImport(winsockDll, CharSet=CharSet.Ansi, SetLastError=false)]
#endif
        private static extern int gethostname( StringBuilder host, int len );


         #region Meijer Customization

        public bool UseBiometrics { get; private set; }

        public bool AllowEmplIdSignOn { get; private set; }

        public bool AllowHandKeyEMPLID { get; private set; }

        public ScannerEventArgs SavedScannerEventArgs { get; set; }

        #endregion Meijer Customization
	}
}
