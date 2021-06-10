#undef WINCE

using System;
using System.Collections;
using System.Text;
using System.Xml;
using RPSWNET;
using System.Runtime.InteropServices;
using CommonLibsNet;
using System.Reflection;
using System.Diagnostics;

namespace RapNet
{
	/// <summary>
	/// Configuration data singleton used by the application.
	/// </summary>
	internal class ConfigDataRapNet : ConfigData
	{
		/// <summary>
		/// The class constructor.
		/// </summary>
		protected ConfigDataRapNet()
		{
			//Load();	
		}

		/// <summary>
		/// The class destructor.
		/// </summary>
		~ConfigDataRapNet()
		{
		}

		/// <summary>
		/// Get the one and only ConfigDataRapNet instance.
		/// </summary>
		public static ConfigDataRapNet GetConfigData()
		{
			if( theConfigData == null )
			{
                theConfigData = new ConfigDataRapNet();
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

        static public void LoadRegistry()
        {
            LoadCommonRegistry(CurrentProcessName(), RegPath);
        }

		/// <summary>
		/// Loads configuration data from persistant storage.
		/// </summary>
		public override void Load()
		{   
			XmlDocument myXmlDocument = LoadCommonXmlSettings("RapNet.xml", "RapNet.000");

			// Load RapNet specific XML settings
			try
			{
				XmlNodeList nodeList = myXmlDocument.GetElementsByTagName( "ImmediateInterventionSound" );
				if( nodeList.Count >= 1 )
				{
					immediateInterventionSound = nodeList[0].InnerText;
				}

				nodeList = myXmlDocument.GetElementsByTagName( "DelayedInterventionSound" );
				if( nodeList.Count >= 1 )
				{
					delayedInterventionSound = nodeList[0].InnerText;
				}

				nodeList = myXmlDocument.GetElementsByTagName( "InterventionSoundDelay" );
				if( nodeList.Count >= 1 )
				{
					interventionSoundDelay = Convert.ToInt32( nodeList[0].InnerText );
				}

				nodeList = myXmlDocument.GetElementsByTagName( "DisplayTransactionCount" );
				if( nodeList.Count >= 1 )
				{
					displayTransactionCount = Convert.ToBoolean( nodeList[0].InnerText );
				}

				nodeList = myXmlDocument.GetElementsByTagName( "TimePeriodForThroughputCalculation" );
				if( nodeList.Count >= 1 )
				{
					timePeriodForThroughputCalculation = Convert.ToInt32( nodeList[0].InnerText );
				}

				nodeList = myXmlDocument.GetElementsByTagName( "TrxCountThresholdForHighAlert" );
				if( nodeList.Count >= 1 )
				{
					trxCountThresholdForHighAlert = Convert.ToInt32( nodeList[0].InnerText );
				}

				nodeList = myXmlDocument.GetElementsByTagName( "TrxCountThresholdForCaution" );
				if( nodeList.Count >= 1 )
				{
					trxCountThresholdForCaution = Convert.ToInt32( nodeList[0].InnerText );
				}

#if	(!HH_UI)
				// Allow user to switch from the current Exclusive Lane Intervention handling to 
				// handle another Intervention of another Lane. 
				nodeList = myXmlDocument.GetElementsByTagName( "MultipleInterventionHandling" );
				if( nodeList.Count >= 1 )
				{
					multiExclusiveLanes = Convert.ToBoolean( nodeList[0].InnerText );
				}
#endif
			}
			catch( XmlException caught )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "ConfigDataRapNet::Load() - XmlException: " + caught.Message );
			}
		}

		/// <summary>
		/// Allow multiple Exclusive Lanes simultaneously.
		/// ie. Interventions from a 2nd lane can be processed during 
		/// Interventions from the 1st Lane, without exiting exclusive lane
		/// mode on the 1st Lane.  Intervention handling on the 1st lane
		/// is simply suspended temporarily.
		/// </summary>
		public bool MultiExclusiveLanes
		{
			get { return multiExclusiveLanes; }
			set { multiExclusiveLanes = value; }
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
        /// Read-Only property that determines if LaunchPad is running.
        /// </summary>
        public bool IsLaunchPadRunning
        {
            get
            {
                if( -1 == isLaunchPadRunning )
                {
                    isLaunchPadRunning = ( IntPtr.Zero == PInvoke.FindWindow( null, "LaunchPadNet" ) ) ? 0 : 1;
                }                
                return ( isLaunchPadRunning == 1 ) ? true : false;
            }
        }
   
		/// <summary>
		/// Read-Only RegistryPath property accessor.
		/// </summary>
		public override string RegistryPath
		{
			get
			{
				return "SOFTWARE\\NCR";
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

        private const string RegPath = "SOFTWARE\\NCR";

		/// <summary>
		/// Module name of the LaunchPad Application
		/// </summary>
		public const string launchpadApplication = "LaunchPadNet.exe";
        
		/// <summary>
		/// Flag that determines if LaunchPad is running.
		/// </summary>
		private int isLaunchPadRunning = -1;

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
		private static ConfigDataRapNet theConfigData = null;

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
	}
}
