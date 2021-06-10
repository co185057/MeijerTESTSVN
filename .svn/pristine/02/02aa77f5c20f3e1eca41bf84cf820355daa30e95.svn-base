/// <summary>
/// Class: DataCapture
/// Description: This class contains DataCapture.cs file
///     This class is to:
///         - Create and intialize RapConfigue.log file
///         - Format output data
///         - Backup file when it reaches maximum size
///     Notes: This class still has option to use RPSW to capture
///            data by adding RPSW constant to the project's configuration.
///            I strongly recommend not to use RPSW to capture log data
/// Author: Nhuan Nguyen
/// Title:  NCR - Software Engineer
/// Email:  Nhuan.Nguyen@NCR.com
/// Phone:  (770) 813-3741
/// Date:   March 19, 2008
/// Version: 1.0
/// </summary>
using System ;
using System.Runtime.InteropServices ;
using System.IO ;
using System.Threading ;
using System.Xml ;
using System.Text.RegularExpressions ;
using System.Windows.Forms ;
#if ( RPSW )
using RPSWNET ;
#endif

namespace RapConfigure
{
    /// <summary>
    /// Summary description for DataCapture.
    /// </summary>
    public class DataCapture
    {
        private static int nDcapControl = 0 ;           // DcapControl
        private static string strDcapFile = "" ;        // DcapFile
        private static int nDcapFileMax = 0 ;           // DcapFileMax
        private static uint uDcapMask = 0x0 ;           // DcapMask
        private static string strDcapLinePrefix = "" ;  // DcapLinePrefix
        private static string strDcapTime = "" ;        // DcapTime - omit if not using RPSW
        private static string strDcapVersion = "" ;     // DcapVersion - omit if not using RPSW
        private static StreamWriter hDcapStreamWriter = null ;
        
        /// <summary>
        /// DataCapture File
        /// </summary>
        public DataCapture()
        {
        }

        /// <summary>
        /// Close DataCapture File
        /// </summary>
        public static void CloseFile()
        {
            // Set indent to top level
            TRACE( MASK.API, "Exit Application" ) ;
            indent = 0 ;
            TRACE( MASK.API, "### RapConfigure End Tracing ###" ) ;
#if ( !RPSW )
            if ( hDcapStreamWriter != null )
            {
                hDcapStreamWriter.Flush() ;
                hDcapStreamWriter.Close() ;
                hDcapStreamWriter = null ;
            }
#endif
        }

        /// <summary>
        /// CretaeFile: RapConfigure.log
        /// set in registry "SOFTWARE\\NCR\\RapConfigure" 
        /// </summary>
        public static void CreateFile(string registryPath)
        {
            try
            {
#if ( RPSW )
                CmDataCapture.ReadRegistry( registryPath, "DataCapture" );
                CmDataCapture.Initialize( "### RapConfigure Start Tracing ###" );
#else
                ReadDataCapture(registryPath + "\\DataCapture") ;
                hDcapStreamWriter = new System.IO.StreamWriter( strDcapFile, true, System.Text.Encoding.Unicode ) ;
                TRACE( MASK.API, "### RapConfigure Start Tracing ###" ) ;
#endif
            }
            catch ( Exception e )
            {
                if ( hDcapStreamWriter != null )
                {
                    hDcapStreamWriter.Close() ;
                    hDcapStreamWriter = null ;
                }
#if ( DEBUG )
                MessageBox.Show( "Error: in CreateFile() - " + e.Message ) ;
#endif
            }
        }

#if ( !RPSW )

        /// <summary>
        /// Read DataCapture settings in registry
        /// </summary>
        /// <param name="regDataCapturePath"></param>
        private static void ReadDataCapture(string regDataCapturePath)
        {
            ConfigData myConfig = ConfigData.GetConfigData() ;
            nDcapControl = ConvertStringToInteger( myConfig.GetRegistryKeyValue( regDataCapturePath, "DcapControl" ) ) ;
            uDcapMask    = ConvertStringToHex( myConfig.GetRegistryKeyValue( regDataCapturePath, "DcapMask" ) ) ;
            nDcapFileMax = ConvertStringToInteger( myConfig.GetRegistryKeyValue( regDataCapturePath, "DcapFileMax" ) ) ;
            strDcapFile  = myConfig.GetRegistryKeyValue( regDataCapturePath, "DcapFile" ) ;
            strDcapTime    = myConfig.GetRegistryKeyValue( regDataCapturePath, "DcapTime" ) ;
            strDcapVersion = myConfig.GetRegistryKeyValue( regDataCapturePath, "DcapVersion" ) ;
            strDcapLinePrefix = myConfig.GetRegistryKeyValue( regDataCapturePath, "DcapLinePrefix" ) ;
        }

        /// <Summary>
        /// ParseInteger: Helper function to parse hex from input string
        /// </Summary>
        private static uint ConvertStringToHex(string inputString)
        {
            uint hexValue = 0x0 ;

            if ( ( inputString == null ) || ( inputString.Length <= 0 ) )
            {
                return ( hexValue ) ;
            }

            try
            {
                string convertString = inputString.Trim() ;
                convertString = convertString.Substring( 0, convertString.IndexOf( "->" ) ) ;
                convertString = convertString.Trim() ;
                hexValue = ( uint )System.Int32.Parse( convertString, System.Globalization.NumberStyles.HexNumber ) ;
            }
            catch (Exception e)
            {
                TRACE( MASK.ERROR, e.ToString() ) ;
                return ( 0x0 ) ;
            }
        
            return ( hexValue ) ;
        }

        /// <Summary>
        /// ParseInteger: Helper function to parse int from input string
        /// </Summary>
        private static int ConvertStringToInteger(string inputString)
        {
            int intValue = -1 ;

            if ( ( inputString == null ) || ( inputString.Length <= 0 ) )
            {
                return ( intValue ) ;
            }

            try
            {
                string convertString = inputString.Trim() ;
                convertString = convertString.Substring( 0, convertString.IndexOf( "->" ) ) ;
                convertString = convertString.Trim() ;
                intValue = System.Int32.Parse( convertString ) ;
            }
            catch (Exception e)
            {
                TRACE( MASK.ERROR, e.ToString() ) ;
                return ( -1 ) ;
            }
        
            return ( intValue ) ;
        }

        /// <summary>
        /// Backup file
        /// </summary>
        private static void BackupFile()
        {
            if ( ( hDcapStreamWriter == null ) || ( nDcapFileMax <= 0 ) || 
                ( hDcapStreamWriter.BaseStream.Length < ( nDcapFileMax * 1024 ) ) )
            {
                return ;
            }

            // Backup
            try
            {
                // Flush data to current file
                hDcapStreamWriter.Flush() ;
                hDcapStreamWriter.Close() ;

                // Backup
                string bakFilePath = strDcapFile + ".BAK" ;
                File.Move( strDcapFile, bakFilePath ) ;

                // Create new file
                hDcapStreamWriter = new System.IO.StreamWriter( strDcapFile, true, System.Text.Encoding.Unicode ) ;
            }
            catch (Exception e)
            {
                TRACE( MASK.ERROR, e.ToString() ) ;
            }
        }

#endif

        /// <summary>
        /// Get/Set Indent
        /// </summary>
        public static int Indent
        {
            get { return ( indent ) ; }
            set { indent = value ; }
        }

        /// <summary>
        /// TRACE: write log data to file
        /// </summary>
        /// <param name="logData"></param>
        public static void TRACE( MASK maskLevel, string logData )
        {
#if ( !RPSW )
            if ( ( hDcapStreamWriter == null ) || ( nDcapControl != 4 ) )    
            {
                return ;
            }
            // Backup file if it is necessary
            BackupFile() ;
#endif

            try
            {
                // Indent: exit function
                if ( logData.StartsWith( "--" ) ) 
                {
                    indent = System.Math.Max( 0, --indent ) ;
                }

                // Format string: why PadLeft or PadRight does not work?!
                string formatString = logData ;
                for ( int i = 0; i < indent*2; i++ )
                {
                    formatString = "." + formatString ;
                }

                // Capture
#if ( RPSW )
                CmDataCapture.Capture( ( uint )maskLevel, formatString );
#else
                System.DateTime myTime = System.DateTime.Now ;
                
                if ( ( uDcapMask & (byte)maskLevel ) != 0x0 )
                {
                    hDcapStreamWriter.WriteLine( strDcapLinePrefix + "{0:d} {0:HH:mm:ss}> " + formatString, myTime, myTime ) ;
                }
#endif
                // Indent: enter function
                if ( logData.StartsWith( "++" ) || logData.StartsWith( "###" ) ) 
                { 
                    indent++ ; 
                }
            }
            catch ( Exception e )
            {
#if ( DEBUG )
                MessageBox.Show( "Error: " + logData + "; " + e.ToString() ) ;
#endif
            }
        }

        /// <summary>
        /// indent to format log file
        /// </summary>
        private static int indent = 0 ;
    }
}
