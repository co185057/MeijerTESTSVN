using System;
using System.Threading;
using System.Runtime.InteropServices;
using PsxNet;
using RPSWNET;

//SR93- Biometrics
namespace CommonLibsNet
{				
	[StructLayout( LayoutKind.Sequential )]
	public struct FT_STATUS
	{
		public int Code;
		public uint Param1; 		
		public uint Param2;
	}
	
	[StructLayout( LayoutKind.Sequential )]
	public struct FT_USERDATA
	{		
		public String userID;
		public String password;
	}
	

	/// <summary>
	/// FT_ACTION enum
	/// </summary>
	public enum FT_ACTION
	{
		FT_ID_CANCEL,   // cancel
		FT_ID_CONTINUE  // go on
	};	
	
	/// <summary>
	/// Return Codes enum
	/// </summary>
	public enum BIOMETRIC_RC
	{
		RC_SUCCESS,				///< Operation was successful.
		RC_FAILURE,				///< General failure
		RC_FP_NO_MATCH,			///< No match found for fingerprint data.
		RC_DUPLICATE,			///< More than one potential match found in fingerprint verify.
		RC_DEVICE_PLUGGED=1000,	///< Fingerprint device plugged in.
		RC_DEVICE_UNPLUGGED,	///< Fingerprint device unplugged.
		RC_SERVER_READY,		///< ASA Server is ready.
		RC_SERVER_NOTREADY		///< ASA Server is not ready.		
	}

	public enum FT_STATUS_CODES
	{
		FT_WAITING_FOR_IMAGE,		// now waiting for an image from the fingerprint reader
									//	param1 = area acquired given as % ;
									//	param2 = time in ms since FT_isImage was called
		FT_IMAGE_READY,				// an image has been acquired
									//	param1 = raw image size; 
									//	param2 = raw image (the same returned by FT_acquireImage functino) address
		FT_FINGER_TOUCHING,
		FT_FINGER_REMOVED,
		FT_READY_TO_FILL_BUF,		// ready to fill the application display buffer
									//	param1 = buffer address,
									//	param2 = unused
		FT_BUF_FILLED,				// the application display buffer has been filled
									//	param1 = buffer address,
									//	param2 = unused
		FT_IMAGE_INFO,				// provides the info about the acquired image
									//	param1 = FT_IMG_QUALITY
									//	param2 = unused
		FT_FEATURES_INFO,			// provides info about the extracted features
									//	param1 = FT_FTR_QUALITY
									//	param2 = unused
		FT_DEVICE_CONNECTION_ERROR,	// provides info about an error in the connection with the device
		FT_UNSOLICITED_INFO=1000,
		FT_GET_FINGERPRINT_COMPLETE,
		FT_REGISTER_FINGERPRINT_COMPLETE,
		FT_SAVE_USER_COMPLETE,
		FT_DELETE_USER_COMPLETE,
		FT_VERIFY_USER_COMPLETE,
		FT_GET_USERS_COMPLETE
	}
	
	//class BiometricArgs
	public class BiometricArgs : EventArgs
	{
		private readonly int status; 
		private readonly uint param1;		
		private readonly uint param2;
		private readonly IntPtr intPtrParam;

		public BiometricArgs(ref FT_STATUS pFTSTATUS, IntPtr param)
		{
			status = pFTSTATUS.Code;
			param1 = pFTSTATUS.Param1;
			param2 = pFTSTATUS.Param2;
			intPtrParam = param; 
		}		
		
		public int Status
		{
			get { return status; }
		}
		
		public uint Param1
		{
			get { return param1; }
		}

		public uint Param2
		{
			get { return param2; }
		}

		public IntPtr IntPtrParam
		{
			get { return intPtrParam; }
		}
	}

	/// <summary>
	/// BiometricEvent delegate.
	/// </summary>				
	public delegate void BiometricDelegate(object sender, BiometricArgs e);	
	
	/// <summary>
	/// Encapsulates Biometric.
	/// </summary>
    public class Biometric : IDisposable
	{
		/// <summary>
		/// The class constructor.
		/// </summary>		
		public Biometric(Psx paramPsx)
		{
			myPsx = paramPsx;			
		}
		
		/// <summary>
		/// The class destructor.
		/// </summary>
		~Biometric()
		{
			Dispose();
		}

		/// <summary>
		/// Releases the resources used by the Biometric instance.
		/// </summary>
		public void Dispose()
		{			
			if( null != myPsx )
			{				
				if( !Close() )
				{					
					CmDataCapture.Capture( CmDataCapture.MaskError, "Biometric::Close() - Close() failed." );
				}
			}
			GC.SuppressFinalize(this);
		}
		
		//*******************************************************************************
		// Function:   	Open
		// Purpose:		Open/initialize the fingerprint device
		// Parameters:	
		// Returns:		bool
		// Calls:		FT_Open_Net - AuthenticationClientLibrary
		// Side Effects:	
		// Notes:		
		//*******************************************************************************
		public bool Open()
		{
			bool bReturn = false; 
			try
			{
				objBiometricEventHandler = new BiometricEventHandler(internalEventHandler);				
				bReturn = FT_Open_Net(objBiometricEventHandler);
				if( !bReturn )
				{
					BiometricDeviceConfigured = false;
					DeviceReady = false;
					ServerReady = false;
					CmDataCapture.Capture( CmDataCapture.MaskError, "Biometric::Open() Failed." );
				}
			}
			catch( SystemException e )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "Biometric::Open() - SystemException caught: " + e.Message );
				myPsx = null;
			}
			return bReturn; 
		}

		//*******************************************************************************
		// Function:   	GetFingerPrint
		// Purpose:		Get fingerprint
		// Parameters:	
		// Returns:		bool
		// Calls:		FT_GetFingerprint - AuthenticationClientLibrary
		// Side Effects:	
		// Notes:		
		//*******************************************************************************
		public bool GetFingerPrint() 
		{			
			bool bReturn = false;
		    
			if ( isDeviceReady() && isServerReady() )
			{				
				bReturn = FT_GetFingerprint();					
			}	
			
			CmDataCapture.Capture( CmDataCapture.MaskInfo, "Biometric::GetFingerPrint() return: " + bReturn );
			return bReturn;
		}
		
		//*******************************************************************************
		// Function:   	VerifyUser
		// Purpose:		Verify User
		// Parameters:	
		// Returns:		bool
		// Calls:		FT_VerifyUser - AuthenticationClientLibrary
		// Side Effects:	
		// Notes:		
		//*******************************************************************************		
		public bool VerifyUser(byte[] features, int recommendedFtrLen)
		{
			bool bReturn = false;
			
			if ( isDeviceReady() && isServerReady() )	
			{				
				bReturn = FT_VerifyUser(features, recommendedFtrLen);			
			}
			
			CmDataCapture.Capture( CmDataCapture.MaskInfo, "Biometric::VerifyUser() return: " + bReturn );
			return bReturn;
		}

		//*******************************************************************************
		// Function:   	Abort
		// Purpose:		Disable the fingerprint device 
		// Parameters:	
		// Returns:		bool
		// Calls:		FT_Abort - AuthenticationClientLibrary
		// Side Effects:	
		// Notes:		
		//*******************************************************************************		
		public bool Abort()
		{			
			bool bReturn = false;						
			bReturn = FT_Abort();							

			CmDataCapture.Capture( CmDataCapture.MaskInfo, "Biometric::Abort() return: " + bReturn );
			return bReturn;
		}

		//*******************************************************************************
		// Function:   	Close
		// Purpose:		Exit the fingerprint device client application
		// Parameters:	
		// Returns:		bool
		// Calls:		FT_Close - AuthenticationClientLibrary
		// Side Effects:	
		// Notes:		
		//*******************************************************************************		
		public bool Close()
		{			
			bool bReturn = false;
			if( null != myPsx )
			{			
				bReturn = FT_Close();
				myPsx = null;			
			}
			CmDataCapture.Capture( CmDataCapture.MaskInfo, "Biometric::Close() return: " + bReturn );
			return bReturn;
		}

		//*******************************************************************************
		// Function:   	isDeviceReady
		// Purpose:		Check if fingerprint device is ready
		// Parameters:	
		// Returns:		bool
		// Calls:		
		// Side Effects:	
		// Notes:		
		//*******************************************************************************		
		public bool isDeviceReady()
		{
			if ( !DeviceReady )	
			{				
				CmDataCapture.Capture( CmDataCapture.MaskError, "Fingerprint Device disconnected." );				
			}

			return DeviceReady;
		}

		//*******************************************************************************
		// Function:   	isServerReady
		// Purpose:		Check if server is ready
		// Parameters:	
		// Returns:		bool
		// Calls:		
		// Side Effects:	
		// Notes:		
		//*******************************************************************************		
		public bool isServerReady()
		{
			if( !ServerReady )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "Authentication Server is not accessible." );
			}

			return ServerReady;
		}
						
		/// <summary>
		/// Psx instance.
		/// </summary>
		private Psx myPsx = null;

		/// <summary>
		/// Biometric event object.
		/// </summary>
		public event BiometricDelegate BiometricEvent;

		private BiometricEventHandler objBiometricEventHandler = null;		

		//typedef  FT_ACTION  (*FT_GET_ACTION) (IN FT_STATUS_PT, void *);  //callback function in C++
		public delegate FT_ACTION BiometricEventHandler(ref FT_STATUS status, IntPtr param);	

		//this is the delegate method handler
		private FT_ACTION internalEventHandler(ref FT_STATUS pFTSTATUS, IntPtr param)
		{			
			if(null != BiometricEvent)
			{				
				BiometricEvent(this, new BiometricArgs(ref pFTSTATUS, param));				
			}

			return FT_ACTION.FT_ID_CONTINUE;		
		}		
		
#if( DEBUG )
		private const string authenticationClientLibraryDll = "AuthenticationClientLibraryUD.dll";
#else
        private const string authenticationClientLibraryDll = "AuthenticationClientLibraryU.dll";
#endif
		

#if( !WINCE )
		[DllImport(authenticationClientLibraryDll, CallingConvention=CallingConvention.Cdecl, CharSet=CharSet.Unicode, SetLastError=false)]  //orig
#else
        [DllImport(authenticationClientLibraryDll, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, SetLastError = false)]
#endif
        private static extern bool FT_Open_Net(BiometricEventHandler objBiometricEventHandler);
	
#if( !WINCE )			
		[DllImport(authenticationClientLibraryDll, CallingConvention=CallingConvention.Cdecl, CharSet=CharSet.Unicode, SetLastError=false)]
#else
        [DllImport(authenticationClientLibraryDll, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, SetLastError = false)]
#endif
        private static extern bool FT_GetFingerprint();	
	
#if( !WINCE )	
		[DllImport(authenticationClientLibraryDll, CallingConvention=CallingConvention.Cdecl, CharSet=CharSet.Unicode, SetLastError=false)]
#else
        [DllImport(authenticationClientLibraryDll, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, SetLastError = false)]
#endif
		private static extern bool FT_VerifyUser(byte[] features, int recommendedFtrLen);

#if( !WINCE )
		[DllImport(authenticationClientLibraryDll, CallingConvention=CallingConvention.Cdecl, CharSet=CharSet.Unicode, SetLastError=false)]
#else
        [DllImport(authenticationClientLibraryDll, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, SetLastError = false)]
#endif
		private static extern bool FT_Abort();

#if( !WINCE )
		[DllImport(authenticationClientLibraryDll, CallingConvention=CallingConvention.Cdecl, CharSet=CharSet.Unicode, SetLastError=false)]
#else
        [DllImport(authenticationClientLibraryDll, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, SetLastError = false)]
#endif
        private static extern bool FT_Close();


		//other properties...
		private bool deviceReady;
		private bool serverReady;
		private bool biometricDeviceConfigured;		
		private bool keyedIn;  //flag to determine user keyed-in method of login

		public bool DeviceReady
		{
			set{ deviceReady = value; }
			get { return deviceReady; }
		}

		public bool ServerReady
		{
			set{ serverReady = value; }
			get { return serverReady; }
		}		
		
		public bool BiometricDeviceConfigured
		{
			set{ biometricDeviceConfigured = value; }
			get{ return biometricDeviceConfigured; }
		}

		public bool KeyedIn
		{
			set{ keyedIn = value; }
			get{ return keyedIn; }
		}		
	}	
}