using System;
using System.Runtime.InteropServices;
using System.Text;
using PsxNet;
using RPSWNET;

namespace LaunchPadNet
{
    /// <summary>
    /// Class that contains the data for the Scanner events. Derives from System.EventArgs.
    /// </summary>
    public class ScannerEventArgs : EventArgs
    {  
        /// <summary>
        /// The class constructor.
        /// </summary>
        /// <param name="scanData">
        /// The scan data.
        /// </param>
        /// <param name="scanDataLabel">
        /// The scan data label.
        /// </param>
        /// <param name="scanType">
        /// The scan type.
        /// </param>
        public ScannerEventArgs( string scanData, string scanDataLabel, int scanType )
        {
            this.scanData = scanData;
            this.scanDataLabel = scanDataLabel;
            this.scanType = scanType;
        }
      
        /// <summary>
        /// Gets the scan data from the scan event.
        /// </summary>
        public string ScanData
        {     
            get { return scanData; }      
        }
        
        /// <summary>
        /// Gets the scan data label from the scan event.
        /// </summary>
        public string ScanDataLabel
        {     
            get { return scanDataLabel; }      
        }

        /// <summary>
        /// Gets the scan type from the scan event.
        /// </summary>
        public int ScanType
        {     
            get { return scanType; }
        }    

        private readonly string scanData;
        private readonly string scanDataLabel;
        private readonly int scanType;        
    }
   
    /// <summary>
    /// ScannerEvent delegate.
    /// </summary>
    public delegate void ScannerEventHandler( object sender, ScannerEventArgs e );
        
    /// <summary>
	/// Encapsulates scanner worker thread.
	/// </summary>
	internal class ScannerThread : IDisposable
	{
        /// <summary>
        /// The class constructor.
        /// </summary>
        /// <param name="paramPsx">
        /// Psx instance used to host the scanner thread.
        /// </param>
        public ScannerThread( SharedPsx paramPsx )
        {
            exitThreadEvent = PInvoke.CreateEvent( IntPtr.Zero, true, false, null );
            myPsx = paramPsx;
        }

        /// <summary>
        /// The class destructor.
        /// </summary>
        ~ScannerThread()
        {
            Dispose();
        }

        /// <summary>
        /// Releases the resources used by the ScannerThread instance.
        /// </summary>
        public void Dispose()
        {
            if( null != myPsx )
            {
                PPCScanAbort();                
            }            
            PInvoke.SetEvent( exitThreadEvent );            
            GC.SuppressFinalize( this );
        }

        /// <summary>
        /// Disables the scanner.
        /// </summary>        
        public void Disable()
        {
            if( null != myPsx )
			{
				CmDataCapture.Capture( CmDataCapture.MaskWarning, "ScannerThread::Disable() - PPCScanDisable is called" );
                PPCScanDisable();                
            }
            else
            {
                CmDataCapture.Capture( CmDataCapture.MaskWarning, "ScannerThread::Disable() - Scanner is not opened" );
            }            
        }
        
        /// <summary>
        /// Enables the scanner.
        /// </summary>
        /// <param name="host">
        /// Machine name we are binding the scanner to.
        /// </param>
        /// <returns>
        /// Return true if successful.
        /// </returns>
        public bool Enable( string host )
        {
            bool success = false;
            if( null != myPsx )
			{
				CmDataCapture.Capture( CmDataCapture.MaskWarning, "ScannerThread::Enable() - PPCScanEnable is called" );
                success = PPCScanEnable( host );
                if( !success )
                {
                    CmDataCapture.Capture( CmDataCapture.MaskError, "ScannerThread::Enable() - PPCScanEnable failed on host: " + host );
				}
				else
				{
					CmDataCapture.Capture( CmDataCapture.MaskError, "ScannerThread::Enable() - PPCScanEnable succeeded on host: " + host );
				}
            }
            else
            {
                CmDataCapture.Capture( CmDataCapture.MaskError, "ScannerThread::Enable() - Scanner is not opened" );
            }            
            return success;
        }

        /// <summary>
        /// Loads the scanner.
        /// </summary>
        /// <param name="regKey">
        /// Registry key where configuration information is stored.
        /// </param>
        /// <returns>
        /// Return true if successful.
        /// </returns>
        public bool Open( string regKey )
        {
            try
            {
                if( !PPCScanOpen( regKey ) )
                {
                    myPsx = null;                
                }
            }
            catch( SystemException e )
            {
                CmDataCapture.Capture( CmDataCapture.MaskError, "ScannerThread::Open() - SystemException caught: " + e.Message );
                myPsx = null;              
            }
            return null != myPsx;
        }
				
        /// <summary>
        /// Worker thread method that monitors incoming scanner data.
        /// </summary>
        public void ThreadMethod(object stateInfo)
        {
            try
            {
                ConfigData configData = ConfigData.GetConfigData();            
                string scanSound = configData.ScanSound.Length > 0 ? configData.AudioPath + "\\" + configData.ScanSound : "";
                int scanResult = 0;
                StringBuilder scanData = new StringBuilder( 250 );
                StringBuilder scanDataLabel = new StringBuilder( 250 );
                                
                while( myPsx != null )
                {
                    scanResult = PPCScanRead( scanData, scanDataLabel, PInvoke.INFINITE );
                    if( scanResult >= 0 )
                    {
                        CmDataCapture.Capture( CmDataCapture.MaskInfo, "ScannerThread::ThreadMethod() - PPCScanRead succeeded with scan type: " + scanResult.ToString() );
                        if( scanSound.Length > 0 )
                        {
#if (PATCH_C)
                            myPsx.PlaySound( scanSound, Psx.SoundImmediate, Psx.DisplayAll );
#else
                            myPsx.PlaySound( scanSound );
#endif

                        }
                        
                        if( null != ScannerEvent ) 
                        {
                            ScannerEvent( this, new ScannerEventArgs( scanData.ToString(), scanDataLabel.ToString(), scanResult ) );
                        }
                    }
                    else if( -1 == scanResult )
                    {
                        CmDataCapture.Capture( CmDataCapture.MaskError, "ScannerThread::ThreadMethod() - PPCScanRead timed out" );
                    }
                    else
                    {
                        CmDataCapture.Capture( CmDataCapture.MaskError, "ScannerThread::ThreadMethod() - PPCScanRead failed with error: " + scanResult.ToString() );
                    }
    			
                    if( 0 == PInvoke.WaitForSingleObject( exitThreadEvent, 500 ) )
                    {
						myPsx = null;
						CmDataCapture.Capture( CmDataCapture.MaskWarning, "ScannerThread::ThreadMethod() - PPCScanDisable is called" );
                        PPCScanDisable();
                        PPCScanClose();                            
                    }
                }                
            }
            catch( SystemException e )
            {
                CmDataCapture.Capture( CmDataCapture.MaskError, "ThreadMethod::ScannerThread() - SystemException caught: " + e.Message );
                myPsx = null;
            }
			
            PInvoke.CloseHandle( exitThreadEvent );
            exitThreadEvent = IntPtr.Zero;
        }

        /// <summary>
        /// Native Event handle used to signal the exit of ThreadMethod.
        /// </summary>
        private IntPtr exitThreadEvent = IntPtr.Zero;
		        
        /// <summary>
        /// Psx instance.
        /// </summary>
        private SharedPsx myPsx = null;

        /// <summary>
        /// Scanner event handler.
        /// </summary>
        public event ScannerEventHandler ScannerEvent;
        
        [DllImport(scannerDll, CharSet=CharSet.Unicode, SetLastError=false)]
        private static extern void PPCScanAbort();
        
        [DllImport(scannerDll, CharSet=CharSet.Unicode, SetLastError=false)]
        private static extern void PPCScanClose();

        [DllImport(scannerDll, CharSet=CharSet.Unicode, SetLastError=false)]
        private static extern void PPCScanDisable();
        
        [DllImport(scannerDll, CharSet=CharSet.Unicode, SetLastError=false)]
        private static extern bool PPCScanEnable( string id );
        
        [DllImport(scannerDll, CharSet=CharSet.Unicode, SetLastError=false)]
        private static extern bool PPCScanOpen( string regKey );

        [DllImport(scannerDll, CharSet=CharSet.Unicode, SetLastError=false)]
        private static extern int PPCScanRead( StringBuilder scanData, StringBuilder scanDataLabel, uint timeout );
              
#if( WINCE )
#if( DEBUG )
        public const string scannerDll = "NCRScannerServerCED.dll";
#else
        public const string scannerDll = "NCRScannerServerCE.dll";
#endif
#else
#if( DEBUG )
        public const string scannerDll = "NCRRapRemoteScannerDLLUD.dll";
#else
        public const string scannerDll = "NCRRapRemoteScannerDLLU.dll";
#endif
#endif
	}
}
