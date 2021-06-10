using System;
using System.Threading;
using System.Runtime.InteropServices;
using System.Text;
using System.Diagnostics;
using RPSWNET;
using System.Reflection;
using CommonLibsNet;

namespace LaunchPadNet
{
	/// <summary>
	/// The class that handles the creation of the application windows.
	/// </summary>
	internal class MyApplication : IDisposable
	{
		/// <summary>
		/// The class constructor.
		/// </summary>		
		protected MyApplication() 
		{
			theApplication = this;
            
			ConfigData configData = ConfigDataLaunchPad.GetConfigData();
			CmDataCapture.ReadRegistry( configData.RegistryPath + "\\", "DataCapture" );
			CmDataCapture.Initialize( "-- LaunchPadNet Start Tracing --" );        

			myPsx = new SharedPsx();
			exitEvent = PInvoke.CreateEvent( IntPtr.Zero, true, false, null );
		}	
	
		/// <summary>
		/// The class destructor.
		/// </summary>
		~MyApplication()
		{
			Dispose();
		}
		
		/// <summary>
		/// Get the one and only MyApplication instance.
		/// </summary>
		/// <returns>
		/// MyApplication instance.
		/// </returns>
		public static MyApplication GetApplication()
		{
			return theApplication;
		}
                
		/// <summary>
		/// Releases the resources used by the MyApplication instance.
		/// </summary>
		public void Dispose()
		{
			try
			{
				CmDataCapture.Capture( CmDataCapture.MaskExtensive, "Dispose Starting");

				PInvoke.CloseHandle( exitEvent );
				exitEvent = IntPtr.Zero;
				//Thread.Sleep(1000);			// Add this til the PSX dispose is fixed...
				myPsx.Dispose();
				myPsx = null;
            
				CmDataCapture.Capture( CmDataCapture.MaskExtensive, "Dispose After Psx Dispose");
				CmDataCapture.Cleanup();

				GC.SuppressFinalize( this );
			}
			catch ( Exception e )
			{
				PInvoke.MessageBox( IntPtr.Zero, "Exception " + e.Message, "Dispose", PInvoke.MB_YESNO );
			}
		}

#if( WINCE )
		private const string kernelApiDll = "coredll.dll";        
#else
        private const string kernelApiDll = "kernel32.dll";
#endif
		[DllImport(kernelApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
		private static extern IntPtr CreateEvent( IntPtr lpEventAttributes, bool bManualReset, bool bInitialState, string lpName );
	                
		[DllImport(kernelApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
		private static extern bool CloseHandle( IntPtr handle );

		[DllImport("kernel32.dll")]
		static extern uint WaitForMultipleObjects(uint nCount, IntPtr [] lpHandles,
			bool bWaitAll, uint dwMilliseconds);


        /// <summary>
        /// Logs system memory use.
        /// </summary>
        /// <param name="mask">
        /// The data capture mask.
        /// </param>   
        static public void LogMemoryUse( uint mask )
		{
			PInvoke.MEMORYSTATUS stat = new PInvoke.MEMORYSTATUS();
			PInvoke.GlobalMemoryStatus( out stat );
			CmDataCapture.Capture( mask, stat.MemoryLoad.ToString() + " percent of memory is in use." );
			uint mem = stat.TotalPhysical.ToUInt32() / 1024;
			CmDataCapture.Capture( mask, "There are " + mem + " total KB of physical memory." );
			mem = stat.AvailablePhysical.ToUInt32() / 1024;
			CmDataCapture.Capture( mask, "There are " + mem + " free KB of physical memory." );
			mem = stat.AvailableVirtual.ToUInt32() / 1024;
			CmDataCapture.Capture( mask, "There are " + mem + " free KB of virtual memory." );
			if( stat.MemoryLoad > 95 )
			{
				CmDataCapture.Capture( CmDataCapture.MaskWarning, "Low memory warning! " + stat.MemoryLoad.ToString() + " percent of memory is in use." );
			}     
        }

		/// <summary>
		/// Runs the application.
		/// </summary>
		protected void Run()
		{
			LogMemoryUse( CmDataCapture.MaskInfo );		
	
			while ( PInvoke.WAIT_TIMEOUT == PInvoke.WaitForSingleObject( exitEvent, 50 ) )
			{
				try
				{
#if	(!WINCE)
					CmDataCapture.Capture( CmDataCapture.MaskExtensive, "Run():try START");
#endif
					StringBuilder sender = new StringBuilder( 2048 );
					int senderLen = sender.Capacity;
					StringBuilder message = new StringBuilder( 2048 );
					int messageLen = message.Capacity;
					
#if	(!WINCE)
					CmDataCapture.Capture( CmDataCapture.MaskExtensive, "Run():BEFORE GetProcessMessage");
#endif
					if( GetProcessMessage( sender, ref senderLen, message, ref messageLen ) )
					{
#if	(!WINCE)
						CmDataCapture.Capture( CmDataCapture.MaskExtensive, "Run():TRUE GetProcessMessage");
#endif
						string strSrc = sender.ToString();
						string strMsg = message.ToString();

						CmDataCapture.Capture( CmDataCapture.MaskExtensive, "Run():GetProcessMessage - Received from '" + strSrc + "' message: " + strMsg);
						string strOp = ConfigData.ExtractRapEventData( ConfigData.eventRapOperation, strMsg );
						if( strOp == null )
						{
							CmDataCapture.Capture( CmDataCapture.MaskError, "MyApplication::Run() - An operation was not included in the message. Discarding..." );
						}
						else if( strOp == "set-signed-off" )
						{
							myPsx.WasSyncSignOnOff = true;
							myPsx.SignedOn = false;
							//myPsx.ActivateLaunchPad(true,false);
						}
						else if( strOp == "set-signed-on" )
						{
							string strId = ConfigData.ExtractRapEventData( "operator", strMsg );
							string strRights = ConfigData.ExtractRapEventData( "rights", strMsg );
							if( strId == null || strRights == null )
							{
								CmDataCapture.Capture( CmDataCapture.MaskError, "MyApplication::Run() - OperatorID && Rights were not included in the message. Discarding..." );
							}
							else
							{
								myPsx.WasSyncSignOnOff = true;
								myPsx.WasSignOnRemote = true;
								myPsx.OperatorId = strId;
								myPsx.OperatorRights = Convert.ToUInt32( strRights );
								myPsx.SignedOn = true;
								//myPsx.ActivateLaunchPad(true,true);
							}
						}
						else if ( strOp == "stop-app" )
						{
							CmDataCapture.Capture( CmDataCapture.MaskError, "Received Stop command via RemoteMessage" );
							MyApplication.GetApplication().Exit();
						}
						else if( strOp == "launchpad-activate" )
						{
							CmDataCapture.Capture( CmDataCapture.MaskInfo, "Run():DisplayLaunchPadNet(true)");
							// This will display LaunchPadNet
							myPsx.DisplayLaunchPadNet(true);
						}
						else if( strOp == "set-active-host" )
						{
							string strHost = ConfigData.ExtractRapEventData( "host", strMsg );
							if( strHost == null )
							{
								CmDataCapture.Capture( CmDataCapture.MaskError, "MyApplication::Run() - host name not included in the message. Discarding..." );
							}
							else
							{
								myPsx.ActiveHostName = strHost;
							}
						}
						else if( strOp == "reset-autosignoff" )
						{
							myPsx.AutoSignOffCounter = 0;
						}
						else if( strOp == ScannerGeneric.RemoteMessages.ReleaseScanner )
						{
							myPsx.ProcessReleaseScannerRequest();
						}
						else if( strOp == ScannerGeneric.RemoteMessages.ScannerAvailable )
						{
							myPsx.ProcessResponseScannerAvailable();
						}
						else if( strOp == ScannerGeneric.RemoteMessages.ScannerOpenFailed )
						{
							CmDataCapture.Capture(CmDataCapture.MaskWarning, "MyApplication::Run() - received " + strOp + " ignoring message.");
						}
						else if( strOp == "biomet-data" )
						{
							CmDataCapture.Capture(CmDataCapture.MaskInfo, "Received biometric data via RemoteMessage");
							string encodedData = ConfigData.ExtractRapEventData( "encoded-data", strMsg );
							myPsx.ProcessBiometricData(encodedData);
						}				
						else
						{
							CmDataCapture.Capture( CmDataCapture.MaskWarning, "MyApplication::Run() - received an unknown operation from '" + strSrc + "'. Discarding..." );
						}
					}
#if	(!WINCE)
					CmDataCapture.Capture( CmDataCapture.MaskExtensive, "Run():AFTER GetProcessMessage");
#endif
				}
				catch( DllNotFoundException e )
				{
					CmDataCapture.Capture( CmDataCapture.MaskError, "MyApplication::Run() - DllNotFoundException caught: " + e.Message );
                    break;
				}
				catch (Exception e)
				{
					CmDataCapture.Capture( CmDataCapture.MaskError, "Run():EXCEPTION " + e.Message);
				}
			}
#if	(!WINCE)
			PInvoke.WaitForSingleObject( exitEvent, PInvoke.INFINITE );
#else
			CmDataCapture.Capture( CmDataCapture.MaskError, "Run(): Event Posted - Exiting");
#endif 
		}

		/// <summary>
		/// Exits the application.
		/// </summary>
		public void Exit()
		{
			try
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "MyApplication::Exit() calling Abort()"  );
				Abort();
			}
			catch( DllNotFoundException e )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "MyApplication::Exit() - DllNotFoundException caught: " + e.Message );
			}
#if	(!WINCE)
			PInvoke.SetEvent( exitEvent );
#else
			PInvoke.EventModify( exitEvent, PInvoke.EVENT_SET );
#endif
		}
      
		/// <summary>
		/// Gets the SharedPsx Owner.
		/// </summary>
		public SharedPsx MyPsx
		{
			get
			{
				return myPsx;
			}
		}

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
#if( !WINCE )
		[STAThread]
#endif
		static void Main( string[] args ) 
		{

			int instanceCount = 0;
			try
			{
				instanceCount = Process.GetProcessesByName( Process.GetCurrentProcess().ProcessName ).Length;
			}
			catch( Exception e)
			{
				// XPe bug. Fixed in XPe service pack 2.
				CmDataCapture.Capture( CmDataCapture.MaskError, "Main(): EXCEPTION " + e.Message);
			}
			if( instanceCount <= 1 )
			{
				// Preload DLLs //
				remoteMessageDllModule = PInvoke.LoadLibrary( remoteMessageDll );
				scannerDllModule = PInvoke.LoadLibrary( ScannerThread.scannerDll );
#if( WINCE )
				// Preload DLLs //
				simpleHashDllModule = PInvoke.LoadLibrary( SHA256Managed.simpleHashDll );
#endif

				theApplication = new MyApplication();

				theApplication.Run();
			
				if( IntPtr.Zero != remoteMessageDllModule )
				{
					PInvoke.FreeLibrary( remoteMessageDllModule );
				}
				if( IntPtr.Zero != scannerDllModule )
				{
					PInvoke.FreeLibrary( scannerDllModule );
				}
#if( WINCE )
				if( IntPtr.Zero != simpleHashDllModule )
				{
					PInvoke.FreeLibrary( simpleHashDllModule );
				}
#endif

				theApplication.Dispose();
				theApplication = null;
			}
			else
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "MyApplication::Main() - More than one instance of application running. Exiting..." );
			}
		}

		/// <summary>
		/// Native Event handle used to signal the exit of the application.
		/// </summary>
		private IntPtr exitEvent = IntPtr.Zero;
       
		/// <summary>
		/// The SharedPsx instance.
		/// </summary>
		private SharedPsx myPsx = null;

		/// <summary>
		/// RemoteMessage DLL handle used for preloading.
		/// </summary>
		static private IntPtr remoteMessageDllModule = IntPtr.Zero;
        
		/// <summary>
		/// Scanner DLL handle used for preloading.
		/// </summary>
		static private IntPtr scannerDllModule = IntPtr.Zero;

#if( WINCE )	
		/// <summary>
		/// SimpleHash DLL handle used for preloading.
		/// </summary>
		static private IntPtr simpleHashDllModule = IntPtr.Zero;
#endif

		/// <summary>
		/// The one and only MyApplication instance.
		/// </summary>
		static private MyApplication theApplication = null;

		[DllImport(remoteMessageDll, CharSet=CharSet.Unicode, SetLastError=false)]
		private static extern void Abort();

		[DllImport(remoteMessageDll, CharSet=CharSet.Unicode, SetLastError=false)]
		private static extern bool GetProcessMessage( StringBuilder sender, ref int senderLen, StringBuilder message, ref int messageLen );

		[DllImport(remoteMessageDll, CharSet=CharSet.Unicode, SetLastError=false)]
		public static extern bool SendProcessMessage( string target, string message );

#if( DEBUG )
		private const string remoteMessageDll = "RemoteMessageUD.dll";
#else
		private const string remoteMessageDll = "RemoteMessageU.dll";
#endif
	}		
}
