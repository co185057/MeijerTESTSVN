/// <summary>
/// Class: ConfigData
/// Description: This class contains ConfigData.cs file
///     This class is to:
///         - Parse settings in registry
///         - Load config xml files
///         - Load RapConfigureMsg<LCID>.xml to support multi languages
/// Author: Nhuan Nguyen
/// Title:  NCR - Software Engineer
/// Email:  Nhuan.Nguyen@NCR.com
/// Phone:  (770) 813-3741
/// Date:   March 19, 2008
/// Version: 1.0
/// Revised: April 14, 2008
///		   : Edit GetWindowDimension/GetRapConfigMsgXML
///		   : Maribeth Villanueva
/// </summary>
using System ;
using System.Drawing ;
//using System.Collections ;
//using System.Data ;
using System.Runtime.InteropServices ;
using System.IO ;
using System.Threading ;
using System.Xml ;
using System.Text.RegularExpressions ;

namespace RapConfigure
{
    /// <summary>
    /// Summary description for ConfigData.
    /// </summary>
    internal class ConfigData
    {
        // Constants
#if ( WINCE )
        private int DISPLAY_WIDTH        = 240 ; // default - overrided by config file
        private int DISPLAY_HEIGHT       = 320 ; // default - overrided by config file
#else
        private int DISPLAY_WIDTH        = 1024 ; // default - overrided by config file
        private int DISPLAY_HEIGHT       = 768 ;  // default - overrided by config file
#endif
        private const int MAXLANE       = 10 ;  // default - not overrided by config file
        
        private string[] laneInfoArray ;
        private int numLanes = 0 ;

        protected ConfigData()
        {
        }

        /// <summary>
        /// Get/Set value for configPath
        /// </summary>
        public string ConfigPath
        {
            get { return ( this.configPath ) ; }
            set { this.configPath = value ; }
        }

		/// <summary>
		/// Get/Set value for resolution
		/// </summary>
		public string MediaResolution
		{
			get { return ( this.mediaResolution ) ; }
			set { this.mediaResolution = value ; }
		}

        /// <summary>
        /// Device Model
        /// </summary>
        public HHRAPMODEL DeviceModel
        {
            get { return ( this.deviceModel ) ; }
            set { this.deviceModel = value ; }
        }

        /// <summary>
        /// Display Dimension/Resolution
        /// </summary>
        public Size Dimension
        {
            get { return ( new System.Drawing.Size( this.Width, this.Height ) ) ; }
            set { this.Width = value.Width ; this.Height = value.Height ; }
        }

        /// <summary>
        /// Display Width
        /// </summary>
        public int Width
        {
            get { return ( this.DISPLAY_WIDTH ) ; }
            set { this.DISPLAY_WIDTH = value ; }
        }

        /// <summary>
        /// Display Height
        /// </summary>
        public int Height
        {
            get { return ( this.DISPLAY_HEIGHT ) ; }
            set { this.DISPLAY_HEIGHT = value ; }
        }

        /// <summary>
        /// Expand environment variables
        /// </summary>
        /// <param name="sArg"></param>
        /// <returns></returns>
        private string ExpandEnvironmentVars( string strEnvVariable )
        {
            TRACE( MASK.API, "++ExpandEnvironmentVars()" ) ;
            TRACE( MASK.INFO, "Input: " + strEnvVariable ) ;

            string sTmp = strEnvVariable ;
            string sRep = "%(APP|DATA|OS|BOOT|TEMP)_DRIVE%" ;
#if( WINCE )
            sTmp = Regex.Replace( sTmp, sRep, "", RegexOptions.IgnoreCase ) ;
#else
            sTmp = Regex.Replace( sTmp, sRep, "C:", RegexOptions.IgnoreCase ) ;
#endif

            TRACE( MASK.INFO, "Output: " + sTmp ) ;
            TRACE( MASK.API, "--ExpandEnvironmentVars()" ) ;
            
            return sTmp;
        }

        /// <summary>
        /// GetConfigData: returns an unique instance of ConfigData
        /// </summary>
        /// <returns></returns>
        public static ConfigData GetConfigData()
        {
            if (theConfigData == null)
            {
                theConfigData = new ConfigData() ;
            }

            return (theConfigData) ;
        }

        
        /// <summary>
        /// GetDeviceModel: determine if it is MC50, MC70 or other
        /// </summary>
        /// <returns></returns>
        private HHRAPMODEL GetDeviceModel()
        {
            TRACE( MASK.API, "++GetDeviceModel()" ) ;
            HHRAPMODEL deviceModel = HHRAPMODEL.OTHER ;

            try
            {
#if ( WINCE )
                string oemInfo = new string( ' ', 100 ) ;
                if ( SystemParametersInfo4Strings( ( uint )SystemParametersInfoActions.SPI_GETOEMINFO, ( uint )100, oemInfo, 0 ) == 0 )
                {
                    TRACE( MASK.ERROR, "Call SystemParametersInfo4Strings() to get OEM failed!" ) ;
                }
            
                if ( oemInfo.IndexOf( "MC50", 0, oemInfo.Length ) != -1 )
                {
                    deviceModel = HHRAPMODEL.MC50 ;
                }
                else if ( oemInfo.IndexOf( "MC70", 0, oemInfo.Length ) != -1 )
                {
                    deviceModel = HHRAPMODEL.MC70 ;
                }
#endif
            }
            catch ( Exception e )
            {
                TRACE( MASK.ERROR, e.ToString() ) ;
            }
            finally
            {
                TRACE( MASK.INFO, "Model: " + deviceModel.ToString() ) ;
                TRACE( MASK.API, "--GetDeviceModel()" ) ;
            }

            return ( deviceModel ) ;
        }

        
        /// <summary>
        /// Get Window Dimension
        /// </summary>
        private Size GetWindowDimension(string resolution)
        {
            TRACE( MASK.API, "++GetWindowDimension()" ) ;
            int width = 0 ;
            int height = 0 ;
            this.MediaResolution = resolution;
			switch(MediaResolution)
			{
				case "240x320":
				{
					height = 320;
					width = 240;
					break;
				}
				case "8x6": 
				{
					height = 600;
					width = 800;
					break;
				}
				case "10x7": 
				{
					height = 768;
					width = 1024;
					break;
				}
				default:
				{
					TRACE( MASK.INFO, "Default Dimension." ) ;
					break;
				}
			}
			this.Width  = ( width  != 0 ? width  : DISPLAY_WIDTH ) ;
			this.Height = ( height != 0 ? height : DISPLAY_HEIGHT ) ;

            TRACE( MASK.INFO, "Window Dimension: " + this.Width + "x" + this.Height ) ;
            TRACE( MASK.API, "--GetWindowDimension()" ) ;
            
            return (new System.Drawing.Size( this.Width, this.Height ) ) ;
        }

        /// <summary>
        /// Get registry key
        /// </summary>
        /// <param name="registryPathName"></param>
        /// <param name="keyName"></param>
        /// <returns></returns>
        public string GetRegistryKeyValue( string registryPathName, string keyName )
        {
            TRACE( MASK.API, "++GetRegistryKeyValue()" ) ;
            TRACE( MASK.INFO, "registry=" + registryPathName+ "; key="+keyName ) ;
            uint HKEY_LOCAL_MACHINE = 0x80000002 ;
            uint KEY_READ           = 0x00020019 ;
            uint REG_SZ             = 1 ;
            uint hkey               = 0 ;
            string keyValue         = null ;

            try
            {
                // Open registry 
                if ( 0 == RegOpenKeyExW( HKEY_LOCAL_MACHINE, registryPathName, 0, KEY_READ, ref hkey ) )
                {
                    char[] trimChar = { '\0' } ;
                    uint type = 0 ;
                    byte[] data = new byte[1024] ;

                    // Query Key Value 
                    uint len = ( uint )data.Length ;
                    if ( 0 == RegQueryValueExW( hkey, keyName, 0, ref type, data, ref len ) )
                    {
                        if ( type == REG_SZ )
                        {
                            keyValue = System.Text.UnicodeEncoding.Unicode.GetString( data, 0, ( int )len ) ;
                            keyValue = keyValue.TrimEnd( trimChar ) ;
                        }
                    }

                    // close registry
                    RegCloseKey( hkey ) ;
                }
            }
            catch ( Exception e )
            {
                TRACE( MASK.ERROR, e.ToString() ) ;
            }
            finally
            {
                if ( keyValue != null )
                {
                    keyValue = ExpandEnvironmentVars( keyValue ) ;
                }

                TRACE( MASK.INFO, "Output: " + (keyValue != null ? keyValue : "" ) ) ;
                TRACE( MASK.API, "--GetRegistryKeyValue()" ) ;
            }
            return ( keyValue ) ;
        }

        /// <summary>
        /// Get/Set number of lanes
        /// </summary>
        public int NumLanes
        {
            get { return ( this.numLanes ) ; }
            set { this.numLanes = value ; }
        }

        /// <summary>
        /// Get lane info array
        /// </summary>
        /// <returns></returns>
        public string[] GetLaneInfoArray()
        {
            return ( this.laneInfoArray ) ;
        }

        /// <summary>
        /// Load RapNet Config
        /// </summary>
        public bool Load()
        {
            TRACE( MASK.API, "++LoadConfigData()" ) ;
            bool bLoadSuccess = false ;

            try
            {
                // Get Config
                this.ConfigPath = GetRegistryKeyValue( regRapNetPath, "ConfigPath" ) ;
				
#if (!WINCE)
				//get resolution
				this.MediaResolution = GetRegistryKeyValue( regMediaResolution, "Resolution" ) ;
#else
				this.MediaResolution = "240x320";
#endif
                // Get Device Model
                this.DeviceModel = this.GetDeviceModel() ;

                // Get Display dimension
                this.Dimension = this.GetWindowDimension(MediaResolution) ;

                // Load labels and texts from RapConfigureMsg<LCID>.xml file
                this.LoadRapConfigureMsg() ;
				
                // Load RapNet.xml from non-volatile first on MC50
                if ( this.DeviceModel == HHRAPMODEL.MC50 )
                {
                    TRACE( MASK.INFO, "Start loading RapNet.xml for HHRAP MC50" ) ;
                    bLoadSuccess = ( this.LoadRapNetXML( this.nonVolatilePath ) || this.LoadRapNetXML( this.configPath ) ) ;
                }
                else
                {
                    TRACE( MASK.INFO, "Start loading RapNet.xml for HHRAP MC70 or other models" ) ;
                    bLoadSuccess = ( this.LoadRapNetXML( this.configPath ) ) ;
                }
            }
            catch ( Exception e )
            {
                TRACE( MASK.ERROR, e.ToString() ) ;
                return ( bLoadSuccess ) ;
            }
            finally
            {
                TRACE( MASK.API, "--LoadConfigData(): returns " + bLoadSuccess.ToString() ) ;
            }

            return ( bLoadSuccess ) ;
        }

        /// <summary>
        /// Get/Set LoadingConfigPath
        /// </summary>
        public string LoadingPath
        {
            get { return (this.loadingPath) ; }
            set { this.loadingPath = value ; }
        }

        /// <summary>
        /// LoadRapConfigureMsg: Load RapConfigureMsg<LCID>.xml, and
        /// initialize all default texts/labels used on this application
        /// with those defined in RapConfigureMsg<LCID>.xml file
        /// </summary>
        private void LoadRapConfigureMsg()
        {
            TRACE( MASK.API, "++LoadRapConfigureMsg()" ) ;

           // string strConfigLanguageFileName = "ConfigLanguage.xml"; //!!!
            string strConfigMsgFileName = "RapConfigureMsg0409.xml";

            try
            {

				string fileName = strConfigMsgFileName;

                // Get actual language msg name
               // strConfigMsgFileName = this.GetRapConfigMsgXML( this.configPath, strConfigLanguageFileName, strConfigMsgFileName );
            
				string strLCID = this.GetRegistryKeyValue( regLCID, "PrimaryLanguageCode");
				if ( strLCID != null )
				{
					TRACE( MASK.INFO, "Using RapConfigureMsg" + strLCID + ".xml" );
					fileName = fileName.Replace( "0409", strLCID ) ;
				}

                this.ParseRapConfigureMsgXML( this.configPath, fileName ) ;
            }
            catch (Exception e)
            {
                TRACE( MASK.INFO, "Error: " + e.ToString() ) ;
            }
            finally
            {
                TRACE( MASK.API, "--LoadRapConfigureMsg()" ) ;
            }
        }

        /// <summary>
        /// LoadRapNetXML:  Load RapNet.xml file
        /// </summary>
        /// <returns></returns>
        private bool LoadRapNetXML( string strDirectoryPath )
        {
            TRACE( MASK.API, "++LoadRapNetXML()" ) ;
            TRACE( MASK.INFO, "From directory: " + strDirectoryPath );
            
            // Check file
            if ( !File.Exists( strDirectoryPath + "\\RapNet.xml" ) )
            {
                TRACE( MASK.INFO, strDirectoryPath + "\\RapNet.xml" + " does not exist" ) ;
                TRACE( MASK.API, "--LoadRapNetXML()" ) ;
                return ( false ) ;
            }

            // Keep track of which directory the RapNet.xml is loaded for 
            // the Save operation later
            this.LoadingPath = strDirectoryPath ;

            try
            {
                XmlDocument myXmlDocument = new XmlDocument() ;
                myXmlDocument.Load( strDirectoryPath + "\\RapNet.xml" ) ;

                XmlNodeList nodeList = myXmlDocument.GetElementsByTagName( "Lane" ) ;

                if ( ( nodeList == null ) || ( nodeList.Count <= 0 ) )
                {
                    return ( true ) ;
                }

                this.NumLanes = nodeList.Count ;
#if ( WINCE )
                this.NumLanes = System.Math.Min( nodeList.Count, MAXLANE ) ;
#endif
                this.laneInfoArray = new string[this.NumLanes] ;

                int i = 0 ;
                foreach( XmlNode node in nodeList )
                {
                    if ( i < this.NumLanes )
                    {
                        TRACE( MASK.INFO, "Lane["+i+"]="+node.InnerText ) ;
                        laneInfoArray[i++] = node.InnerText ;
                    }
                }

                if ( myXmlDocument != null )
                {
                    myXmlDocument.RemoveAll() ;
                }
            }
            catch ( Exception e )
            {
                TRACE( MASK.ERROR, e.ToString() );
            }
            finally
            {
                TRACE( MASK.API, "--LoadRapNetXML()" ) ;
            }

            return ( true ) ;
        }

        /// <summary>
        /// Parse RapConfigureMsg<LCID>.xml file 
        /// </summary>
        /// <param name="filePath"></param>
        /// <param name="fileNameMsg"></param>
        private void ParseRapConfigureMsgXML( string filePath, string fileNameMsg )
        {
            string filePathName = filePath + "\\" + fileNameMsg ;

            this.TRACE( MASK.API, "++ParseRapConfigureMsgXML()" ) ;
            this.TRACE( MASK.INFO, "Input: " + filePathName ) ;

            if ( !File.Exists( filePathName ) )
            {
                this.TRACE( MASK.INFO, filePathName + " does not exist" ) ;
                this.TRACE( MASK.API, "--ParseRapConfigureMsgXML()" ) ;
                return ;    
            }

            try
            {
                // Parse RapConfigMsg...
                XmlTextReader xmlReader = new XmlTextReader( filePathName );
                int counter = 0 ;

                while ( xmlReader.Read() )
                {
                    if ( ( xmlReader.NodeType == XmlNodeType.Element ) && 
                        ( xmlReader.Name == "String" ) )
                    {
                        for (int nIndex=0; nIndex < xmlReader.AttributeCount; nIndex++)
                        {
                            try
                            {
                                xmlReader.MoveToAttribute( nIndex ) ;
                                this.ProcessTextID( xmlReader.Value, xmlReader.ReadString() ) ;
                                counter++ ;
                            }
                            catch (Exception ex)
                            {
                                this.TRACE( MASK.ERROR, "Errors while parsing " + filePathName ) ;
                                this.TRACE( MASK.ERROR, ex.ToString() ) ;
                                continue ;
                            }
                        }
                    }
                }

                if (xmlReader != null)
                {
                    xmlReader.Close() ;
                }

                this.TRACE( MASK.INFO, "Read " + counter + ( counter > 0 ? " texts and/or labels" : " text and/or label" ) ) ;
            }
            catch( Exception e )
            {
                this.TRACE( MASK.ERROR, "Errors while parsing " + filePathName ) ;
                this.TRACE( MASK.ERROR, e.ToString() ) ;
            }
            finally
            {
                this.TRACE( MASK.API, "--ParseRapConfigureMsgXML()" ) ;
            }
        }

        /// <summary>
        /// Parse each element in the file
        /// </summary>
        /// <param name="strTextID"></param>
        /// <param name="strTextValue"></param>
        private void ProcessTextID( string strTextID, string strTextValue )
        {
            TRACE( MASK.EXTENSIVE, "++ProcessTextID()" ) ;
            TRACE( MASK.EXTENSIVE, strTextID + "=" + strTextValue ) ;

            switch ( strTextID )
            {
                case "MainTitle" :             { this.STRMainTitle             = strTextValue ; break ; }
                case "AddButtonText" :         { this.STRAddButtonText         = strTextValue ; break ; }
                case "AddLaneTitle" :          { this.STRAddLaneTitle          = strTextValue ; break ; }
				case "Backspace" :             { this.STRBackspace             = strTextValue ; break ; }
                case "CancelButtonText" :      { this.STRCancelButtonText      = strTextValue ; break ; }
                case "ConfigTypeText" :        { this.STRConfigTypeText        = strTextValue ; break ; }
                case "ConfirmDeleteTitle" :    { this.STRConfirmDeleteTitle    = strTextValue ; break ; }
                case "ConfirmDeleteText" :     { this.STRConfirmDeleteText     = strTextValue ; break ; }
                case "ConfirmExitTitle" :      { this.STRConfirmExitTitle      = strTextValue ; break ; }
                case "ConfirmExitText" :       { this.STRConfirmExitText       = strTextValue ; break ; }
                case "ConfirmSaveTitle" :      { this.STRConfirmSaveTitle      = strTextValue ; break ; }
                case "ConfirmSaveText" :       { this.STRConfirmSaveText       = strTextValue ; break ; }
                case "DeleteButtonText" :      { this.STRDeleteButtonText      = strTextValue ; break ; }
				case "DuplicateIPTitle" :      { this.STRDuplicateIPTitle      = strTextValue ; break ; }
                case "DuplicateLaneTitle" :    { this.STRDuplicateLaneTitle    = strTextValue ; break ; }
                case "DuplicateLaneText" :     { this.STRDuplicateLaneText     = strTextValue ; break ; }
                case "EditLaneTitle" :         { this.STREditLaneTitle         = strTextValue ; break ; }
                case "EmptyText" :             { this.STREmptyText             = strTextValue ; break ; }
				case "Enter" :             	   { this.STREnter	               = strTextValue ; break ; }
				case "EnterHostNameInstruction": {this.STREnterHostNameInstruction    = strTextValue ; break ; }
				case "EnterIPAddressInstruction": {this.STREnterIPAddressInstruction    = strTextValue ; break ; }
				case "EnterIPOctetInstruction": {this.STREnterIPOctetInstruction    = strTextValue ; break ; }
                case "EnterHostNameText" :     { this.STREnterHostNameText     = strTextValue ; break ; }
                case "EnterIPAddressText" :    { this.STREnterIPAddressText    = strTextValue ; break ; }
                case "EnterIPOctetText" :      { this.STREnterIPOctetText      = strTextValue ; break ; }
                case "Error" :                 { this.STRErrorText             = strTextValue ; break ; }
                case "ExitButtonText" :        { this.STRExitButtonText        = strTextValue ; break ; }
                case "FileErrorTitle" :        { this.STRFileErrorTitle        = strTextValue ; break ; }
                case "FileErrorText" :         { this.STRFileErrorText         = strTextValue ; break ; }
                case "HostNameText" :          { this.STRHostNameText          = strTextValue ; break ; }
                case "IPAddressText" :         { this.STRIPAddressText         = strTextValue ; break ; }
                case "IPOctetText" :           { this.STRIPOctetText           = strTextValue ; break ; }
                case "LaneText" :              { this.STRLaneText              = strTextValue ; break ; }
                case "MoveButtonText" :        { this.STRMoveButtonText        = strTextValue ; break ; }
                case "NoButtonText" :          { this.STRNoButtonText          = strTextValue ; break ; }
				case "NoLane" :                { this.STRNoLane                = strTextValue ; break ; }
				case "NoLaneToDelete" :        { this.STRNoLaneToDelete        = strTextValue ; break ; }
                case "NoLaneWarningTitle" :    { this.STRNoLaneWarningTitle    = strTextValue ; break ; }
                case "NoLaneWarningText" :     { this.STRNoLaneWarningText     = strTextValue ; break ; }
                case "OkButtonText" :          { this.STROkButtonText          = strTextValue ; break ; }
                case "RestartRAPTitle" :       { this.STRRestartRAPTitle       = strTextValue ; break ; }
                case "RestartRAPText" :        { this.STRRestartRAPText        = strTextValue ; break ; }
                case "SameHHRapIPAddress":     { this.STRSameHHRapIPAddress    = strTextValue ; break ; }
				case "SameRapIPAddress":       { this.STRSameRapIPAddress      = strTextValue ; break ; }
                case "SaveButtonText" :        { this.STRSaveButtonText        = strTextValue ; break ; }
				case "SelectLaneToDelete" :    { this.STRSelectLaneToDelete      = strTextValue ; break ; }
                case "SelectSourceText" :      { this.STRSelectSourceText      = strTextValue ; break ; }
                case "SelectDestinationText" : { this.STRSelectDestinationText = strTextValue ; break ; }
                case "YesButtonText" :         { this.STRYesButtonText         = strTextValue ; break ; }
                default:                       { break ; }
            }

            TRACE( MASK.EXTENSIVE, "--ProcessTextID()" ) ;
        }

        /// <summary>
        /// Get RapNet registry path
        /// </summary>
        public string RapNetRegistryPath
        {
            get { return ( regRapNetPath ) ;  }
        }

        /// <summary>
        /// Get RapConfigure registry path
        /// </summary>
        public string RapConfigureRegistryPath
        {
            get { return ( regRapConfigurePath ) ; }
        }

        /// <summary>
        /// Main Title
        /// </summary>
        public string STRMainTitle
        {
            get { return ( this.strMainTitle ) ; }
            set { this.strMainTitle = value ; }
        }
		/// <summary>
		/// Backspace button text
		/// </summary>
		public string STRBackspace
		{
			get { return ( this.strBackspace ) ; }
			set { this.strBackspace = value ; }
		}

        /// <summary>
        /// Add Button Text
        /// </summary>
        public string STRAddButtonText
        {
            get { return ( this.strAddButtonText ) ; }
            set { this.strAddButtonText = value ; }
        }

        /// <summary>
        /// Add Lane Screen Title
        /// </summary>
        public string STRAddLaneTitle
        {
            get { return ( this.strAddLaneTitle ) ; }
            set { this.strAddLaneTitle = value ; }
        }

        /// <summary>
        /// Cancel Button Text
        /// </summary>
        public string STRCancelButtonText
        {
            get { return ( this.strCancelButtonText ) ; }
            set { this.strCancelButtonText = value ; }
        }

        /// <summary>
        /// Config Type Text
        /// </summary>
        public string STRConfigTypeText
        {
            get { return ( this.strConfigTypeText ) ; }
            set { this.strConfigTypeText = value ; }
        }

        /// <summary>
        /// Confirm Deletion Title
        /// </summary>
        public string STRConfirmDeleteTitle
        {
            get { return ( this.strConfirmDeleteTitle ) ; }
            set { this.strConfirmDeleteTitle = value ; }
        }

        /// <summary>
        /// Confirm Deletion Text
        /// </summary>
        public string STRConfirmDeleteText
        {
            get { return ( this.strConfirmDeleteText ) ; }
            set { this.strConfirmDeleteText = value ; }
        }

        /// <summary>
        /// Confirm Exit Title
        /// </summary>
        public string STRConfirmExitTitle
        {
            get { return ( this.strConfirmExitTitle ) ; }
            set { this.strConfirmExitTitle = value ; }
        }

        /// <summary>
        /// Confirm Exit Text
        /// </summary>
        public string STRConfirmExitText
        {
            get { return ( this.strConfirmExitText ) ; }
            set { this.strConfirmExitText = value ; }
        }

        /// <summary>
        /// Confirm Save Title
        /// </summary>
        public string STRConfirmSaveTitle
        {
            get { return ( this.strConfirmSaveTitle ) ; }
            set { this.strConfirmSaveTitle = value ; }
        }

        /// <summary>
        /// Confirm Save Text
        /// </summary>
        public string STRConfirmSaveText
        {
            get { return ( this.strConfirmSaveText ) ; }
            set { this.strConfirmSaveText = value ; }
        }

        /// <summary>
        /// Delete Button Text
        /// </summary>
        public string STRDeleteButtonText
        {
            get { return ( this.strDeleteButtonText ) ; }
            set { this.strDeleteButtonText = value ; }
        }

		/// <summary>
		/// Duplicate IP Title
		/// </summary>
		public string STRDuplicateIPTitle
		{
			get { return ( this.strDuplicateIPTitle ) ; }
			set { this.strDuplicateIPTitle = value ; }
		}        
		
		/// <summary>
		/// Duplicate Lane Title
		/// </summary>
		public string STRDuplicateLaneTitle
		{
			get { return ( this.strDuplicateLaneTitle ) ; }
			set { this.strDuplicateLaneTitle = value ; }
		}

        /// <summary>
        /// Duplicate Lane Text
        /// </summary>
        public string STRDuplicateLaneText
        {
            get { return ( this.strDuplicateLaneText ) ; }
            set { this.strDuplicateLaneText = value ; }
        }

        /// <summary>
        /// Edit Lane Screen Title
        /// </summary>
        public string STREditLaneTitle
        {
            get { return ( this.strEditLaneTitle ) ; }
            set { this.strEditLaneTitle = value ; }
        }

        /// <summary>
        /// Empty Text
        /// </summary>
        public string STREmptyText
        {
            get { return ( this.strEmptyText ) ; }
            set { this.strEmptyText = value ; }
        }

        /// <summary>
        /// Enter Lane ... Text
        /// </summary>
        public string STREnterHostNameText
        {
            get { return ( this.strEnterHostNameText ) ; }
            set { this.strEnterHostNameText = value ; }
        }

        /// <summary>
        /// Enter Lane ... Text
        /// </summary>
        public string STREnterIPAddressText
        {
            get { return ( this.strEnterIPAddressText ) ; }
            set { this.strEnterIPAddressText = value ; }
        }

        /// <summary>
        /// Enter Lane ... Text
        /// </summary>
        public string STREnterIPOctetText
        {
            get { return ( this.strEnterIPOctetText ) ; }
            set { this.strEnterIPOctetText = value ; }
        }

        /// <summary>
        /// Error Text
        /// </summary>
        public string STRErrorText
        {
            get { return ( this.strErrorText ) ; }
            set { this.strErrorText = value ; }
        }

        /// <summary>
        /// Exit Button Text
        /// </summary>
        public string STRExitButtonText
        {
            get { return ( this.strExitButtonText ) ; }
            set { this.strExitButtonText = value ; }
        }

        /// <summary>
        /// File Error Title
        /// </summary>
        public string STRFileErrorTitle
        {
            get { return ( this.strFileErrorTitle ) ; }
            set { this.strFileErrorTitle = value ; }
        }

        /// <summary>
        /// File Error Text
        /// </summary>
        public string STRFileErrorText
        {
            get { return ( this.strFileErrorText ) ; }
            set { this.strFileErrorText = value ; }
        }

        /// <summary>
        /// Host Name Text
        /// </summary>
        public string STRHostNameText
        {
            get { return ( this.strHostNameText ) ; }
            set { this.strHostNameText = value ; }
        }

        /// <summary>
        /// IP Address Text
        /// </summary>
        public string STRIPAddressText
        {
            get { return ( this.strIPAddressText ) ; }
            set { this.strIPAddressText = value ; }
        }

        /// <summary>
        /// IP Octet Text
        /// </summary>
        public string STRIPOctetText
        {
            get { return ( this.strIPOctetText ) ; }
            set { this.strIPOctetText = value ; }
        }

        /// <summary>
        /// Lane Text
        /// </summary>
        public string STRLaneText
        {
            get { return ( this.strLaneText ) ; }
            set { this.strLaneText = value ; }
        }

        /// <summary>
        /// Move Button Text
        /// </summary>
        public string STRMoveButtonText
        {
            get { return ( this.strMoveButtonText ) ; }
            set { this.strMoveButtonText = value ; }
        }

        /// <summary>
        /// No Button Text
        /// </summary>
        public string STRNoButtonText
        {
            get { return ( this.strNoButtonText ) ; }
            set { this.strNoButtonText = value ; }
        }

        /// <summary>
        /// No Lane Warning Title
        /// </summary>
        public string STRNoLaneWarningTitle
        {
            get { return ( this.strNoLaneWarningTitle ) ; }
            set { this.strNoLaneWarningTitle = value ; }
        }

        /// <summary>
        /// No Lane Warning Text
        /// </summary>
        public string STRNoLaneWarningText
        {
            get { return ( this.strNoLaneWarningText ) ; }
            set { this.strNoLaneWarningText = value ; }
        }

        /// <summary>
        /// OK button Text
        /// </summary>
        public string STROkButtonText
        {
            get { return ( this.strOkButtonText ) ; }
            set { this.strOkButtonText = value ; }
        }

        /// <summary>
        /// Restart RAP Title
        /// </summary>
        public string STRRestartRAPTitle
        {
            get { return ( this.strRestartRAPTitle ) ; }
            set { this.strRestartRAPTitle = value ; }
        }

        /// <summary>
        /// Restart RAP Text
        /// </summary>
        public string STRRestartRAPText
        {
            get { return ( this.strRestartRAPText ) ; }
            set { this.strRestartRAPText = value ; }
        }

		/// <summary>
		/// Same as HHRAP IP Address text
		/// </summary>
		public string STRSameHHRapIPAddress
		{
			get { return ( this.strSameHHRapIPAddress ) ; }
			set { this.strSameHHRapIPAddress = value ; }
		}
		
		/// <summary>
		/// Same as RAP IP Address text
		/// </summary>
		public string STRSameRapIPAddress
		{
			get { return ( this.strSameRapIPAddress ) ; }
			set { this.strSameRapIPAddress = value ; }
		}

        /// <summary>
        /// Save Button Text
        /// </summary>
        public string STRSaveButtonText
        {
            get { return ( this.strSaveButtonText ) ; }
            set { this.strSaveButtonText = value ; }
        }

        /// <summary>
        /// Select Source Text
        /// </summary>
        public string STRSelectSourceText
        {
            get { return ( this.strSelectSourceText ) ; }
            set { this.strSelectSourceText = value ; }
        }

		/// <summary>
		/// Select Destination Text
		/// </summary>
		public string STRSelectDestinationText
		{
			get { return ( this.strSelectDestinationText ) ; }
			set { this.strSelectDestinationText = value ; }
		}
        /// <summary>
        /// Yes Button Text
        /// </summary>
        public string STRYesButtonText
        {
            get { return ( this.strYesButtonText ) ; }
            set { this.strYesButtonText = value ; }
        }

		/// <summary>
		/// Enter Button Text
		/// </summary>
		public string STREnter
		{
			get { return ( this.strEnter ) ; }
			set { this.strEnter = value ; }
		}

		/// <summary>
		///  No More Lane instruction message
		/// </summary>
		public string STRNoLane
		{
			get { return ( this.strNoLane ) ; }
			set { this.strNoLane = value ; }
		}
		/// <summary>
		/// No More lane to delete message
		/// </summary>
		public string STRNoLaneToDelete
		{
			get { return ( this.strNoLaneToDelete ) ; }
			set { this.strNoLaneToDelete = value ; }
		}
		/// <summary>
		/// Select lane to delete instruction
		/// </summary>
		public string STRSelectLaneToDelete
		{
			get { return ( this.strSelectLaneToDelete ) ; }
			set { this.strSelectLaneToDelete = value ; }
		}

		/// <summary>
		/// Enter Lane ... Text
		/// </summary>
		public string STREnterHostNameInstruction
		{
			get { return ( this.strEnterHostNameInstruction ) ; }
			set { this.strEnterHostNameInstruction = value ; }
		}

		/// <summary>
		/// Enter IP  ... Instruction
		/// </summary>
		public string STREnterIPAddressInstruction
		{
			get { return ( this.strEnterIPAddressInstruction ) ; }
			set { this.strEnterIPAddressInstruction = value ; }
		}

		/// <summary>
		/// Enter Lane ... Text
		/// </summary>
		public string STREnterIPOctetInstruction
		{
			get { return ( this.strEnterIPOctetInstruction ) ; }
			set { this.strEnterIPOctetInstruction = value ; }
		}

       
        /// <summary>
        /// TRACE: forward log data to Log class
        /// </summary>
        /// <param name="maskLevel"></param>
        /// <param name="logData"></param>
        private void TRACE( MASK maskLevel, string logData )
        {
            DataCapture.TRACE( maskLevel, logData ) ;
        }

        /// <summary>
        /// Private members
        /// </summary>
        private static ConfigData theConfigData ;
#if ( WINCE )
        private string configPath = "\\scot\\config" ;
		private string mediaResolution = "240x320";
#else
        private string configPath = "C:\\scot\\config" ;
		private string mediaResolution = "10x7";
#endif
        private string nonVolatilePath = "C:\\scot\\config" ;
        private string loadingPath = "" ;
        // Model needed for special saving mechanism in HHRAP MC50 
        private HHRAPMODEL deviceModel = HHRAPMODEL.OTHER ;
		
  
        /// <summary>
        /// RapNet
        /// </summary>
        private const string regRapNetPath = "SOFTWARE\\NCR\\RapNet" ;
        private const string regRapConfigurePath = "SOFTWARE\\NCR\\RapConfigure" ;
#if ( WINCE )
		private const string regLCID = "SOFTWARE\\NCR\\SCOT - HandheldRAP\\ObservedOptions";
#else
		private const string regMediaResolution = "SOFTWARE\\NCR\\SCOT - Media\\ObservedOptions";
		private const string regLCID = "SOFTWARE\\NCR\\SCOT - RAP\\ObservedOptions";
#endif

#if ( WINCE )
        private const string regApiDll = "coredll.dll" ;
#else
        private const string regApiDll = "advapi32.dll" ;
#endif
        
        [DllImport( regApiDll, CharSet=CharSet.Unicode, SetLastError=false )]
        private static extern int RegOpenKeyExW( uint hkey, string subKey, uint options, uint sam, ref uint phkResult ) ;

        [DllImport( regApiDll, CharSet=CharSet.Unicode, SetLastError=false )]
        private static extern int RegQueryValueExW( uint hkey, string valueName, int reserved, ref uint type, byte[] data, ref uint len ) ;

        [DllImport( regApiDll, CharSet=CharSet.Unicode, SetLastError=false )]
        private static extern int RegCloseKey( uint hkey ) ;
#if ( WINCE )
        [DllImport( regApiDll, EntryPoint = "SystemParametersInfoW", CharSet = CharSet.Unicode )]
        static extern int SystemParametersInfo4Strings( uint uiAction, uint uiParam, string pvParam, uint fWinIni ) ;
#endif

        // Default Texts and Labels
        private string strMainTitle          = "RAP Configuration" ;
        private string strAddButtonText      = "Add" ;
        private string strAddLaneTitle       = "Add Lane" ;
		private string strBackspace		     = "Backspace" ;
        private string strCancelButtonText   = "Cancel" ;
        private string strConfigTypeText     = "Lane configuration type:" ;
        private string strConfirmDeleteTitle = "Confirm Deletion" ;
        private string strConfirmDeleteText  = "Do you want to delete lane:" ;
        private string strConfirmExitTitle   = "Confirm Exit" ;
        private string strConfirmExitText    = "Are you sure you want to discard these changes?" ;
        private string strConfirmSaveTitle   = "Confirm Save" ;
        private string strConfirmSaveText    = "You are going to save the changes to RapNet.xml.  Do you want to continue?" ;
        private string strDeleteButtonText   = "Delete" ;
		private string strDuplicateIPTitle   = "Duplicate IP Address";
		private string strDuplicateLaneTitle = "Found Duplicate Lane";
		private string strDuplicateLaneText  = "Lane %s already exists!" ;
        private string strEditLaneTitle      = "Edit Lane" ;
        private string strEmptyText          = "Empty" ;
		private string strEnterHostNameText  = "Enter lane’s Host Name";
		private string strEnterIPAddressText = "Enter lane’s IP Address";
		private string strEnterIPOctetText   = "Enter lane’s IP Octet";
		private string strErrorText          = "Error" ;
        private string strExitButtonText     = "Exit" ;
        private string strFileErrorTitle     = "File Not Found" ;
        private string strFileErrorText      = "Error: Could not find RapNet.xml file!" ;
        private string strHostNameText       = "Host Name" ;
        private string strIPAddressText      = "IP Address" ;
        private string strIPOctetText        = "IP Octet" ;
        private string strLaneText           = "Lane" ;
        private string strMoveButtonText     = "Move" ;
        private string strNoButtonText       = "No" ;
        private string strNoLaneWarningTitle = "RapNet.xml Has No Lanes" ;
        private string strNoLaneWarningText  = "Warning: RapNet.xml does not have any lanes configured!" ;
        private string strOkButtonText       = "OK" ;
        private string strRestartRAPTitle    = "Restart RAP" ;
        private string strRestartRAPText     = "You need to restart RAP!" ;
		private string strSameRapIPAddress   = "Cannot use same IP address as the Desktop RAP's IP address: %s";
		private string strSameHHRapIPAddress = "Cannot use same IP address as the Handheld RAP's IP address: %s";
		private string strSaveButtonText     = "Save" ;
        private string strSelectSourceText   = "Select a lane to move" ;
        private string strSelectDestinationText = "Select destination" ;
        private string strYesButtonText      = "Yes" ;
		private string strSelectLaneToDelete = "Select a lane to delete" ;
		private string strEnter				 = "Enter" ;
		private string strNoLane			 = "No more lane!";
		private string strNoLaneToDelete     =  "No more lane to delete.";
		private string strEnterHostNameInstruction  = "Key in Host Name and touch 'Enter'.";
		private string strEnterIPAddressInstruction = "Key in IP Address and touch 'Enter'.";
		private string strEnterIPOctetInstruction   =  "Key in IP Octet and touch 'Enter'.";


    }
}
