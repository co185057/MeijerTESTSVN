using System;
using System.Runtime.InteropServices;
using System.Threading;
using System.Text;
using PsxNet;
using RPSWNET;

namespace RapNet
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
        public ScannerEventArgs(string scanData, string scanDataLabel, int scanType)
        {
            ConfigData configData = ConfigData.GetConfigData();
            this.scanData = ConfigData.IsProxyServer && ScannerThread.EncryptScanData ? ConvertToOposDecimal(scanData) : scanData;
            this.scanDataLabel = ConfigData.IsProxyServer && ScannerThread.EncryptScanData ? ConvertToOposDecimal(scanDataLabel) : scanDataLabel;
            this.scanType = scanType;
        }
        
        /// <summary>
        /// Gets the scan data from the scan event.
        /// </summary>
        public string ScanData
        {     
            get
            {
                return scanData;
            }      
        }
        
        /// <summary>
        /// Gets the scan data label from the scan event.
        /// </summary>
        public string ScanDataLabel
        {     
            get
            { 
                return scanDataLabel;
            }      
        }

        /// <summary>
        /// Gets the scan type from the scan event.
        /// </summary>
        public int ScanType
        {     
            get
            { 
                return scanType;
            }
        }

        /// <summary>
        /// Convert scanData and scanDataLabel to OPOSAscii.
        /// </summary>
        static public string ConvertToOposAscii(string data)
        {
            int newData = 0;
            string result = "";
            for (int i = 0; i < data.Length; i += 3)
            {
                newData = Convert.ToInt32(data.Substring(i, 3));
                result += Convert.ToChar(newData);
            }
            return result;
        }

        /// <summary>
        /// Convert scanData and scanDataLabel to OPOSDecimal.
        /// </summary>
        static public string ConvertToOposDecimal(string data)
        {
            int newData = 0;
            string result = String.Empty;
            for (int i = 0; i < data.Length; ++i)
            {
                newData = data[i];                
                result += newData.ToString().PadLeft(3, '0');
            }
            CmDataCapture.Capture(CmDataCapture.MaskInfo, "ScannerThread::ConvertToOposDecimal(): " + result);
            return result;
        }
              
        private readonly string scanData;
        private readonly string scanDataLabel;
        private readonly int scanType;        
    }
   
    /// <summary>
    /// ScannerEvent delegate.
    /// </summary>
    public delegate void ScannerEventHandler(object sender, ScannerEventArgs e);
        
    /// <summary>
	/// Encapsulates scanner worker thread.
	/// </summary>
	public class ScannerThread : IDisposable
	{
        /// <summary>
        /// The class constructor.
        /// </summary>
        /// <param name="paramPsx">
        /// Psx instance used to host the scanner thread.
        /// </param>
        /// <param name="encryptScanData">
        /// Flag indicating if scan data should be encrypted.
        /// </param>
        public ScannerThread(SharedPsx paramPsx, bool encryptScanData)
        {
            myPsx = paramPsx;
            EncryptScanData = encryptScanData;
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
            if (null != myPsx)
            {
                PPCScanAbort();                
            }
            autoEvent.Set();
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Disables the scanner.
        /// </summary>        
        public void Disable()
        {
            if (!ConfigData.IsProxyServer)
            {
                if (null != myPsx)
                {
                    CmDataCapture.Capture(CmDataCapture.MaskWarning, "ScannerThread::Disable() - PPCScanDisable is called");
                    PPCScanDisable();                
                }
                else
                {
                    CmDataCapture.Capture(CmDataCapture.MaskWarning, "ScannerThread::Disable() - Scanner is not opened");
                }
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
        public bool Enable(string host)
        {
            bool success = false;
            if (null != myPsx)
            {
                CmDataCapture.Capture(CmDataCapture.MaskWarning, "ScannerThread::Enable() - PPCScanEnable is called");
                try
                {
                    success = PPCScanEnable(host); //might caused for a hang
                    if (!success)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "ScannerThread::Enable() - PPCScanEnable failed on host: " + host);
                    }
                    else
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "ScannerThread::Enable() - PPCScanEnable succeeded on host: " + host);
                    }
                }
                catch (SystemException e)
                {
                    success = false;
                    CmDataCapture.Capture(CmDataCapture.MaskError, "ScannerThread::Enable() - SystemException caught: " + e.Message);             
                }
            }
            else
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "ScannerThread::Enable() - Scanner is not opened");
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
        public bool Open(string regKey)
        {
            try
            {
                if (!PPCScanOpen(regKey))
                {
                    myPsx = null;                
                }
            }
            catch (SystemException e)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "ScannerThread::Open() - SystemException caught: " + e.Message);
                myPsx = null;              
            }
            if (null != myPsx)
            {
                new Thread(new ThreadStart(ThreadMethod)).Start();
                return true;
            }
            return false;
        }

        /// <summary>
        /// Accessor for ScannerEvent.
        /// </summary>
        public void GenerateScannerEvent(Object sender, ScannerEventArgs e)
        {
            CmDataCapture.Capture(CmDataCapture.MaskInfo, "ScannerThread::GenerateScannerEvent()");
            ScannerEvent(sender, e);
        }

        public static string ScanDataToOperation(string operation, ScannerEventArgs e)
        {
            StringBuilder scanString = new StringBuilder();
            scanString.Append("operation=");
            scanString.Append(operation);
            scanString.Append(";scanner-data=");
            scanString.Append(e.ScanData);
            scanString.Append(";scanner-data-label=");
            scanString.Append(e.ScanDataLabel);
            scanString.Append(";scanner-type=");
            scanString.Append(e.ScanType.ToString());
            scanString.Append(";encoding-type=");
            scanString.Append(EncryptScanData ? OPOS_BC_DECIMAL.ToString() : OPOS_BC_NONE.ToString());
            return scanString.ToString();
		}

        /// <summary>
        /// Gets or sets the scan data encryption flag.
        /// </summary>
        public static bool EncryptScanData
        {
            get
            {
                return encryptScanData;
            }
            set
            {
                if (encryptScanData != value)
                {
                    encryptScanData = value;
                    if (myPsx != null)
                    {
                        PPCScanAbort();
                    }
                }
            }
        }
        
        /// <summary>
        /// Worker thread method that monitors incoming scanner data.
        /// </summary>
        private void ThreadMethod()
        {
            try
            {
                ConfigData configData = ConfigData.GetConfigData();            
                int scanResult = 0;
                StringBuilder scanData = new StringBuilder(2048);
                StringBuilder scanDataLabel = new StringBuilder(2048);                                
                while (myPsx != null)
                {
                    // try
                    // {
                        // PPCScanSetBinaryConversion(EncryptScanData ? OPOS_BC_DECIMAL : OPOS_BC_NONE);
                    // }
                    // catch (Exception)
                    // {
                        // CmDataCapture.Capture(CmDataCapture.MaskWarning, "ScannerThread::ThreadMethod() - PPCScanSetBinaryConversion API not supported");
                    // }
                    scanResult = PPCScanRead(scanData, scanDataLabel, PInvoke.INFINITE);
					if (scanResult >= 0)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskInfo, "ScannerThread::ThreadMethod() - PPCScanRead succeeded with scan type: " + scanResult.ToString());
                        myPsx.PlaySound(configData.ScanSound);
                        if (null != ScannerEvent) 
                        {
                            CmDataCapture.Capture(CmDataCapture.MaskError, string.Format("Triggering ScannerEvent with data - Data= {0}  Label= {1} Result= {2} ", scanData.ToString(), scanDataLabel.ToString(), scanResult.ToString()));
                            ScannerEvent(this, new ScannerEventArgs(scanData.ToString(), scanDataLabel.ToString(), scanResult));
                        }
                    }
                    else if (-1 == scanResult)
                    {
                        CmDataCapture.Capture( CmDataCapture.MaskError, "ScannerThread::ThreadMethod() - PPCScanRead timed out");
                    }
                    else
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "ScannerThread::ThreadMethod() - PPCScanRead failed with error: " +
                            scanResult.ToString());
                    }    			
                    if (autoEvent.WaitOne(500, false))
                    {
						myPsx = null;
						CmDataCapture.Capture(CmDataCapture.MaskWarning, "ScannerThread::ThreadMethod() - PPCScanDisable is called");
                        PPCScanDisable();
                        PPCScanClose();                            
                    }
                }                
            }
            catch (SystemException e)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "ThreadMethod::ScannerThread() - SystemException caught: " + e.Message);
                myPsx = null;
            }
        }
        
        /// <summary>
        /// Event handle used to signal the exit of ThreadMethod.
        /// </summary>
        private AutoResetEvent autoEvent = new AutoResetEvent(false);
		        
        private static bool encryptScanData = true;

        /// <summary>
        /// Psx instance.
        /// </summary>
        private static SharedPsx myPsx = null;

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
        private static extern bool PPCScanOpen(string regKey);

        [DllImport(scannerDll, CharSet=CharSet.Unicode, SetLastError=false)]
        private static extern int PPCScanRead(StringBuilder scanData, StringBuilder scanDataLabel, uint timeout);

		[DllImport(scannerDll, CharSet=CharSet.Unicode, SetLastError=false)]
		private static extern void PPCScanSetBinaryConversion(int nNewValue);

		public const int OPOS_BC_NONE = 0;
        public const int OPOS_BC_DECIMAL = 2;
              
#if (WINCE)
#if (DEBUG)
        public const string scannerDll = "NCRScannerServerCED.dll";
#else
        public const string scannerDll = "NCRScannerServerCE.dll";
#endif
#else
#if (DEBUG)
        public const string scannerDll = "NCRRapRemoteScannerDLLUD.dll";
#else
        public const string scannerDll = "NCRRapRemoteScannerDLLU.dll";
#endif
#endif
	}
}
