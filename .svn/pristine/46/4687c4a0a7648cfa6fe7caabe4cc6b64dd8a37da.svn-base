#undef WINCE
using System;
using System.Collections;
using System.Runtime.InteropServices;
using System.Text;
using System.Xml;
using System.Text.RegularExpressions;
using CommonLibsNet;

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
	internal class ConfigDataLaunchPad : ConfigData
	{
		/// <summary>
		/// The class constructor.
		/// </summary>
		protected ConfigDataLaunchPad()
		{
			Load();	
		}

		/// <summary>
		/// The class destructor.
		/// </summary>
		~ConfigDataLaunchPad()
		{
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

		public int AutoSignOffSeconds
		{
			get
			{
				return autoSignOffSeconds;
			}
		}

		private int autoSignOffSeconds = 0;

		private static bool bHasKeyLockHW = false;							

		/// <summary>
		/// returns true if terminal has keylock hardware
		/// </summary>
		public static bool HasKeyLockHW 
		{
			get { return bHasKeyLockHW; }
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
		}        
		
		/// <summary>
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
		/// Get the one and only ConfigDataLaunchPad instance.
		/// </summary>
		/// <returns>
		/// ConfigData instance.
		/// </returns>
		public static ConfigDataLaunchPad GetConfigData()
		{
			if( theConfigData == null )
			{
				theConfigData = new ConfigDataLaunchPad();
			}

			return theConfigData;
		}
		
        /// <summary>
		/// Loads configuration data from persistant storage.
		/// </summary>
		public override void Load()
		{
			LoadCommonRegistrySettings();
			XmlDocument myXmlDocument = LoadCommonXmlSettings("LaunchPad.xml");

			// Get LaunchPad specific registry settings
			char[] trimChar = { '\0' };
			byte[] data = new byte[1024];
			uint len = (uint)data.Length;
			uint hkey = 0;
			uint type = 0;

			// Collect the OS data
			OperatingSystem OS = Environment.OSVersion;
			string tmpStr1="";
			string tmpStr2="";
			if(( OS.Platform == PlatformID.Win32NT)&&(OS.Version.Major == 5))
			{
				tmpStr1 = "Config_XP";
				tmpStr2 = "PSXConfig_XP";
			}
			else if(( OS.Platform == PlatformID.Win32NT)&&(OS.Version.Major == 6))  //POS184615
			{
				tmpStr1 = "Config_NT";
				tmpStr2 = "PSXConfig_NT";
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

			//POS184615
			if( 0 == PInvoke.RegOpenKeyExW( PInvoke.HKEY_LOCAL_MACHINE, registryPath, 0, PInvoke.KEY_READ, ref hkey ) ) 
			{
				if( 0 == PInvoke.RegQueryValueExW( hkey, tmpStr1, 0, ref type, data, ref len ) )
				{
					if( type == PInvoke.REG_SZ )
					{
						configXML = System.Text.UnicodeEncoding.Unicode.GetString( data, 0, (int)len );
						configXML = configXML.TrimEnd( trimChar );
					}
				}
				len = (uint)data.Length;
				if( 0 == PInvoke.RegQueryValueExW( hkey, tmpStr2, 0, ref type, data, ref len ) )
				{
					if( type == PInvoke.REG_SZ )
					{
						psxConfigXML = System.Text.UnicodeEncoding.Unicode.GetString( data, 0, (int)len );
						psxConfigXML = psxConfigXML.TrimEnd( trimChar );
					}
				}
			}
			len = (uint)data.Length;
			if( -1 == configXML.IndexOf("\\") && ! configPath.Equals ("")) 
				configXML = configPath + "\\" + configXML;     
    
			if( -1 == psxConfigXML.IndexOf("\\") && ! configPath.Equals("") )
				psxConfigXML = configPath + "\\" + psxConfigXML;      

            PInvoke.RegCloseKey( hkey );
			hkey = 0;

			// Collect other SCOTTB registry entries
			len = (uint)data.Length;
			if( 0 == PInvoke.RegOpenKeyExW( PInvoke.HKEY_LOCAL_MACHINE, scotTBRegistryPath, 0, PInvoke.KEY_READ, ref hkey ) )
			{
				// Get Terminal ID
				len = (uint)data.Length;
				if( 0 == PInvoke.RegQueryValueExW( hkey, "TerminalNumber", 0, ref type, data, ref len ) )
				{
					if( type == PInvoke.REG_SZ )
					{
						terminalId = System.Text.UnicodeEncoding.Unicode.GetString( data, 0, (int)len );
						terminalId = terminalId.TrimEnd( trimChar );
					}
				}

				// Get Software Version	Rfc 343789	
				len = (uint)data.Length;
				if( 0 == PInvoke.RegQueryValueExW( hkey, "InterfaceVersion", 0, ref type, data, ref len ) )
				{
					if( type == PInvoke.REG_SZ )
					{
						interfaceVersion = System.Text.UnicodeEncoding.Unicode.GetString( data, 0, (int)len );
						interfaceVersion = interfaceVersion.TrimEnd( trimChar );
					}
				}

				PInvoke.RegCloseKey( hkey );
				hkey = 0;
			}

			// Find out if this is 3.0 Hardware or not 
			// if it is, assume it will have a keylock.
			// Unfortunately, the keylock specific registry settings
			// have no indication whether a button or a real keylock 
			// exists.  
			if( 0 == PInvoke.RegOpenKeyExW( PInvoke.HKEY_LOCAL_MACHINE, hardwareTypeRegistryPath, 0, PInvoke.KEY_READ, ref hkey ) )
			{
				len = (uint)data.Length;
				if( 0 == PInvoke.RegQueryValueExW( hkey, "Type", 0, ref type, data, ref len ) )
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

				PInvoke.RegCloseKey( hkey );
			}

			if( myXmlDocument != null )
			{
				// Load additional Launchpad specific settings
				try
				{
					// Allow return to ScotApp/RapNet when a utility executable is running? (default is false)
					XmlNodeList nodeList = myXmlDocument.GetElementsByTagName( "UtilityReturnAlwaysEnabled" );
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

					// Is it required to display the legal caption prior to the sign on menu? (default is false).
					nodeList = myXmlDocument.GetElementsByTagName( "LegalCaptionRequired" );
					if( nodeList.Count >= 1 )
					{
						LegalCaptionRequired = Convert.ToBoolean( nodeList[0].InnerText );
					}

					nodeList = myXmlDocument.GetElementsByTagName( "ShutDownTimeout" );
					if( nodeList.Count >= 1 )
					{
						shutDownTimeout = Convert.ToInt32( nodeList[0].InnerText );
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
				}
				catch( XmlException caught )
				{
					CmDataCapture.Capture(CmDataCapture.MaskError, "ConfigDataLaunchPad::Load() - XmlException: " + caught.Message);
				}
			}
			else
			{
				CmDataCapture.Capture(CmDataCapture.MaskError, "ConfigDataLaunchPad::Load() - LaunchPad XML settings document not loaded.");
			}
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

			if( 0 == PInvoke.RegOpenKeyExW( HKEY_LOCAL_MACHINE, keyPath, 0, KEY_READ, ref hkey ) )
			{
				char[] trimChar = { '\0' };
				uint type = 0;
				byte[] data = new byte[1024];
				uint len = (uint)data.Length;
				if( 0 == PInvoke.RegQueryValueExW( hkey, id, 0, ref type, data, ref len ) )
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

				PInvoke.RegCloseKey( hkey );
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

			CmDataCapture.Capture( CmDataCapture.MaskError, "ConfigDataLaunchPad::SetStringRegistry() - Attempt write to registry:" + @keyPath + "=" + keyName );

			RegistryKey rk = Registry.LocalMachine.CreateSubKey( @keyPath );
			// Write registry information
			if ( rk == null )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "ConfigDataLaunchPad::SetStringRegistry() - Unable to write to registry:" + @keyPath + "=" + keyName );
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
        /// Read-Only RegistryPath property accessor.
        /// </summary>
        public override string RegistryPath
        {
            get
            {
                return "SOFTWARE\\NCR\\LaunchPadNet";
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
		/// Read-Only property that determines if LaunchPad is running.
		/// </summary>
		public bool IsRapNetRunning
		{
			get
			{
				if( -1 == isRapNetRunning )
				{
					isRapNetRunning = ( IntPtr.Zero == PInvoke.FindWindow( null, "RAP.NET" ) ) ? 0 : 1;
				}                
				return ( isRapNetRunning == 1 ) ? true : false;
			}
		}
        
		/// <summary>
		/// Module name of the RAP Application
		/// </summary>
		public const string rapApplication = "RapNet.exe";

		/// <summary>
		/// Allow return to ScotApp/RapNet when a utility executable is running? (default is false).
		/// </summary>
		private bool utilityReturnAlwaysEnabled = false;
		
		/// <summary>
		/// Is it required to display the legal caption prior to the sign on menu? (default is false).
		/// </summary>
		private bool legalCaptionRequired=false;

		/// <summary>
		/// Timeout in ms for the application to shutdown. If the application is not dead after this time, call process.kill().
		/// </summary>
		private int shutDownTimeout = 15000;

		/// <summary>
		/// LaunchPadNet configuration filename.
		/// </summary>
		private string configXML = "LaunchPadConfig_XP.xml";
		
		/// <summary>
		/// LaunchPadNet PSX display filename.
		/// </summary>
		private string psxConfigXML = "LaunchPadPSX.xml";

		/// <summary>
		/// SCOTTB registry path.
		/// </summary>
		private const string scotTBRegistryPath = "SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTTB";

		/// <summary>
		/// Application registry path.
		/// </summary>
		private const string registryPath = "SOFTWARE\\NCR\\LaunchPadNet";

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

		public const int SWUSER	  = 0;
		public const int MAINTUSER = 1;
		
		/// <summary>
		/// The one and only ConfigData instance.
		/// </summary>
		private static ConfigDataLaunchPad theConfigData;

		/// <summary>
		/// Hash algorithm type ("NONE", "SHA1Button", "SHA256Button", "SHA384Button", "SHA512Button", "MD5Button")
		/// </summary>
		private string hashAlgorithm = "NONE";

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
		/// Flag that determines if RapNet is running.
		/// </summary>
		private int isRapNetRunning = -1;
	}

}
