using System;
using System.Runtime.InteropServices;
using System.Text;
using RPSWNET;
using PsxNet;

namespace CommonLibsNet
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
            this.scanData = ConfigData.IsProxyServer && ScannerGeneric.EncryptScanData ? ConvertToOposDecimal(scanData) : scanData;
            this.scanDataLabel = ConfigData.IsProxyServer && ScannerGeneric.EncryptScanData ? ConvertToOposDecimal(scanDataLabel) : scanDataLabel;
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
            CmDataCapture.Capture(CmDataCapture.MaskInfo, "ScannerGeneric::ConvertToOposDecimal(): " + result);
            return result;
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
	/// Summary description for ScannerGeneric
	/// </summary>
	abstract public class ScannerGeneric : IDisposable
	{
		/// <summary>
		/// The class constructor.
		/// </summary>
		/// <param name="paramPsx">
		/// Psx instance used to host the scanner.
		/// </param>
		/// <param name="configData">
		/// Configuration data 
		/// </param>
		protected ScannerGeneric(Psx paramPsx, bool encryptScanData, ConfigData configData)
		{
			myPsx = paramPsx;
			myConfigData = configData;
            EncryptScanData = encryptScanData;
		}

		/// <summary>
		/// The class destructor.
		/// </summary>
		~ScannerGeneric()
		{
			Dispose();
		}

		public static ScannerGeneric CreateScanner(string scannerClass, Psx paramPsx, bool encryptScanData, ConfigData configData)
		{
			ScannerGeneric scanner = null;
			try
			{
                object[] args = new object[] { paramPsx, encryptScanData, configData };
				scanner = (ScannerGeneric)Activator.CreateInstance(Type.GetType(scannerClass, true), args);
			}
			catch(Exception ex)
			{
				CmDataCapture.Capture(CmDataCapture.MaskError, "ScannerGeneric::CreateScanner() - Exception during creation: " + ex.Message);
			}
			return scanner;
		}

		protected bool FireScannerEvent(object sender, ScannerEventArgs e)
		{
			if (null != ScannerEvent) 
			{
				ScannerEvent(this, e);
				return true;
			}
			else
			{
				return false;
			}
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

        


		abstract public void Dispose();
		abstract public bool Start();
		abstract public void Disable();
		abstract public bool Enable(string host);
		abstract public bool Open(string regKey);

        private static bool encryptScanData = true;
		/// <summary>
		/// Psx instance.
		/// </summary>
        public static Psx myPsx = null;

		/// <summary>
		/// Configuration data instance.
		/// </summary>
		public ConfigData myConfigData = null;

		
		/// <summary>
		/// Scanner event handler.
		/// </summary>
		public event ScannerEventHandler ScannerEvent;

        private bool _scannerEnabled;
        private bool _scannerOpened;
        private object _scannerLock = new object();

		/// <summary>
		/// Is the Scanner Opened and Enabled
		/// </summary>
        public bool ScannerEnabled 
        { 
            get
            {
                lock (_scannerLock)
                {
                    return _scannerEnabled;
                }
            }
            set
            {
                lock (_scannerLock)
                {
                    if (_scannerEnabled != value)
                    {
                        _scannerEnabled = value;
                    }
                }
            }
        }
        /// <summary>
        /// Gets or sets ScannerOpened
        /// </summary>
        protected bool ScannerOpened
        {
            get
            {
                return _scannerOpened;
            }
            set
            {
                _scannerOpened = value;
            }
        }

		/// <summary>
		/// Interprocess scanner messages
		/// </summary>
		public class RemoteMessages
		{
			public const string ReleaseScanner = "release-scanner";
			public const string ScannerAvailable = "scanner-available";
			public const string ScannerOpenFailed = "scanner-open-failed";
		}

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
        
		[DllImport(scannerDll, CharSet=CharSet.Unicode, SetLastError=false)]
		protected static extern void PPCScanAbort();
        
		[DllImport(scannerDll, CharSet=CharSet.Unicode, SetLastError=false)]
		protected static extern void PPCScanClose();

		[DllImport(scannerDll, CharSet=CharSet.Unicode, SetLastError=false)]
		protected static extern void PPCScanDisable();
        
		[DllImport(scannerDll, CharSet=CharSet.Unicode, SetLastError=false)]
		protected static extern bool PPCScanEnable( string id );
        
		[DllImport(scannerDll, CharSet=CharSet.Unicode, SetLastError=false)]
		protected static extern bool PPCScanOpen( string regKey );

		[DllImport(scannerDll, CharSet=CharSet.Unicode, SetLastError=false)]
		protected static extern int PPCScanRead( StringBuilder scanData, StringBuilder scanDataLabel, uint timeout );

        [DllImport(scannerDll, CharSet = CharSet.Unicode, SetLastError = false)]
        protected static extern void PPCScanSetBinaryConversion(int nNewValue);

        public const int OPOS_BC_NONE = 0;
        public const int OPOS_BC_DECIMAL = 2;
	}
}
