/// <summary>
/// Class: RapConfigureApp
/// Description: This class contains Main.cs file which 
///     is the main or driver of RAP Configure application
///     This class is to:
///         - Create and initialize RapConfigure.log file
///         - Load Config Data from RapNet.xml file
///         - Invoke RAP Configure application
/// Author: Nhuan Nguyen
/// Title:  NCR - Software Engineer
/// Email:  Nhuan.Nguyen@NCR.com
/// Phone:  (770) 813-3741
/// Date:   March 19, 2008
/// Version: 1.0
/// </summary>
using System ;
using System.Windows.Forms ;

namespace RapConfigure
{
    /// <summary>
    /// Driver/Main class
    /// </summary>
    internal class RapConfigureDriver
    {
        /// <summary>
        /// Main: Driver of RAP Configure Application
        /// </summary>
        static void Main()
        {
            try
            {
                // Init Log
                DataCapture.CreateFile( regRapConfigurePath ) ;

                // Init ConfigData
                ConfigData configData = ConfigData.GetConfigData() ;

                // Load config data
                if ( !configData.Load() )
                {
                    DataCapture.TRACE( MASK.ERROR, "Could not load RapNet.xml; file does not exist" ) ;
                    MessageBox.Show( configData.STRFileErrorText, configData.STRFileErrorTitle, 
                        MessageBoxButtons.OK, MessageBoxIcon.Hand, MessageBoxDefaultButton.Button1 ) ;

                    return ;
                }

                // Invoke application
                Application.Run( new RapConfigure() ) ;
            }
            catch ( Exception e )
            {
                DataCapture.TRACE( MASK.ERROR, e.ToString() ) ;
            }
            finally
            {
                DataCapture.CloseFile() ;
            }
        }

        /// <summary>
        /// RapConfigure's registry path
        /// </summary>
        private const string regRapConfigurePath = "SOFTWARE\\NCR\\RapConfigure" ;
    }
}
