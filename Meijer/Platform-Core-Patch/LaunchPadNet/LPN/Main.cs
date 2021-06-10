using System;
using System.Threading;
using System.Runtime.InteropServices;
using System.Text;
using System.Diagnostics;
using RPSWNET;
using System.Reflection;
using Microsoft.Win32;
using System.IO;

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

            ConfigData configData = ConfigData.GetConfigData();
            CmDataCapture.ReadRegistry(configData.RegistryPath + "\\", "DataCapture");
            CmDataCapture.Initialize("-- LaunchPadNet Start Tracing --");

            myPsx = new SharedPsx();
            exitEvent = PInvoke.CreateEvent(IntPtr.Zero, true, false, null);
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
                CmDataCapture.Capture(CmDataCapture.MaskExtensive, "Dispose Starting");

                PInvoke.CloseHandle(exitEvent);
                exitEvent = IntPtr.Zero;
                //Thread.Sleep(1000);			// Add this til the PSX dispose is fixed...
                myPsx.Dispose();
                myPsx = null;

                CmDataCapture.Capture(CmDataCapture.MaskExtensive, "Dispose After Psx Dispose");
                CmDataCapture.Cleanup();

                GC.SuppressFinalize(this);
            }
            catch (Exception e)
            {
                PInvoke.MessageBox(IntPtr.Zero, "Exception " + e.Message, "Dispose", PInvoke.MB_YESNO);
            }
        }

#if( WINCE )
		private const string kernelApiDll = "coredll.dll";        
#else
        private const string kernelApiDll = "kernel32.dll";
#endif
        [DllImport(kernelApiDll, CharSet = CharSet.Unicode, SetLastError = false)]
        private static extern IntPtr CreateEvent(IntPtr lpEventAttributes, bool bManualReset, bool bInitialState, string lpName);

        [DllImport(kernelApiDll, CharSet = CharSet.Unicode, SetLastError = false)]
        private static extern bool CloseHandle(IntPtr handle);

        [DllImport("kernel32.dll")]
        static extern uint WaitForMultipleObjects(uint nCount, IntPtr[] lpHandles,
            bool bWaitAll, uint dwMilliseconds);


        /// <summary>
        /// Logs system memory use.
        /// </summary>
        /// <param name="mask">
        /// The data capture mask.
        /// </param>   
        static public void LogMemoryUse(uint mask)
        {
            PInvoke.MEMORYSTATUS stat = new PInvoke.MEMORYSTATUS();
            PInvoke.GlobalMemoryStatus(out stat);
            CmDataCapture.Capture(mask, stat.MemoryLoad.ToString() + " percent of memory is in use.");
            uint mem = stat.TotalPhysical.ToUInt32() / 1024;
            CmDataCapture.Capture(mask, "There are " + mem + " total KB of physical memory.");
            mem = stat.AvailablePhysical.ToUInt32() / 1024;
            CmDataCapture.Capture(mask, "There are " + mem + " free KB of physical memory.");
            mem = stat.AvailableVirtual.ToUInt32() / 1024;
            CmDataCapture.Capture(mask, "There are " + mem + " free KB of virtual memory.");
            if (stat.MemoryLoad > 95)
            {
                CmDataCapture.Capture(CmDataCapture.MaskWarning, "Low memory warning! " + stat.MemoryLoad.ToString() + " percent of memory is in use.");
            }
        }

        /// <summary>
        /// Runs the application.
        /// </summary>
        protected void Run()
        {
            LogMemoryUse(CmDataCapture.MaskInfo);

            while (PInvoke.WAIT_TIMEOUT == PInvoke.WaitForSingleObject(exitEvent, 50))
            {
                try
                {
#if	(!WINCE)
                    CmDataCapture.Capture(CmDataCapture.MaskExtensive, "Run():try START");
#endif
                    StringBuilder sender = new StringBuilder(2048);//Meijer customization
                    int senderLen = sender.Capacity;
                    StringBuilder message = new StringBuilder(2048);//Meijer customization
                    int messageLen = message.Capacity;

#if	(!WINCE)
                    CmDataCapture.Capture(CmDataCapture.MaskExtensive, "Run():BEFORE GetProcessMessage");
#endif
                    if (GetProcessMessage(sender, ref senderLen, message, ref messageLen))
                    {
#if	(!WINCE)
                        CmDataCapture.Capture(CmDataCapture.MaskExtensive, "Run():TRUE GetProcessMessage");
#endif
                        string strSrc = sender.ToString();
                        string strMsg = message.ToString();

                        CmDataCapture.Capture(CmDataCapture.MaskExtensive, "Run():GetProcessMessage - Received from '" + strSrc + "' message: " + strMsg);
                        string strOp = ConfigData.ExtractRapEventData(ConfigData.eventRapOperation, strMsg);
                        if (strOp == null)
                        {
                            CmDataCapture.Capture(CmDataCapture.MaskError, "MyApplication::Run() - An operation was not included in the message. Discarding...");
                        }
                        else if (strOp == "set-signed-off")
                        {
                            myPsx.WasSyncSignOnOff = true;
                            myPsx.SignedOn = false;
                            //myPsx.ActivateLaunchPad(true,false);
                        }
                        else if (strOp == "set-signed-on")
                        {
                            string strId = ConfigData.ExtractRapEventData("operator", strMsg);
                            string strRights = ConfigData.ExtractRapEventData("rights", strMsg);
                            if (strId == null || strRights == null)
                            {
                                CmDataCapture.Capture(CmDataCapture.MaskError, "MyApplication::Run() - OperatorID && Rights were not included in the message. Discarding...");
                            }
                            else
                            {
                                myPsx.WasSyncSignOnOff = true;
                                myPsx.WasSignOnRemote = true;
                                myPsx.OperatorId = strId;
                                myPsx.OperatorRights = Convert.ToUInt32(strRights);
                                myPsx.SignedOn = true;
                                //myPsx.ActivateLaunchPad(true,true);
                            }
                        }
                        else if (strOp == "stop-app")
                        {
                            CmDataCapture.Capture(CmDataCapture.MaskError, "Received Stop command via RemoteMessage");
                            MyApplication.GetApplication().Exit();
                        }
                        else if (strOp == "launchpad-activate")
                        {
                            CmDataCapture.Capture(CmDataCapture.MaskInfo, "Run():DisplayLaunchPadNet(true)");
                            // This will display LaunchPadNet
                            myPsx.DisplayLaunchPadNet(true);
                        }
                        else if (strOp == "set-active-host")
                        {
                            string strHost = ConfigData.ExtractRapEventData("host", strMsg);
                            if (strHost == null)
                            {
                                CmDataCapture.Capture(CmDataCapture.MaskError, "MyApplication::Run() - host name not included in the message. Discarding...");
                            }
                            else
                            {
                                myPsx.ActiveHostName = strHost;
                            }
                        }
                        else if (strOp == "reset-autosignoff") //395483
                        {
                            myPsx.AutoSignOffCounter = 0;
                        }
                        else if (strOp == "switch-to-companion")
                        {
                            if (myPsx.TheApp.AllowSwitchTo())
                            {
                                Executable exec = myPsx.m_executablesTable[myPsx.TheApp.CurrentExecutable] as Executable;
                                if (strSrc.ToLower().Equals(exec.Filename.ToLower()))
                                {
                                    myPsx.ActivateLaunchPad(false, false);
                                    myPsx.SwitchTo(false);
                                    myPsx.SetContext(myPsx.TheApp.CurrentContext, SharedPsx.displayLPStandard, false);
                                    myPsx.TheApp.Minimized = false;
                                    myPsx.ActivateLaunchPad(true, true);
                                    myPsx.UpdateButtonsState();
                                }
                            }
                        }
                        else if (strOp == "biomet-data")  //Meijer customization
                        {
                            CmDataCapture.Capture(CmDataCapture.MaskInfo, "Received biometric data via RemoteMessage");
                            string encodedData = ConfigData.ExtractRapEventData("encoded-data", strMsg);
                            myPsx.ProcessBiometricData(encodedData);
                        }
                        else
                        {
                            CmDataCapture.Capture(CmDataCapture.MaskWarning, "MyApplication::Run() - received an unknown operation from '" + strSrc + "'. Discarding...");
                        }
                    }
#if	(!WINCE)
                    CmDataCapture.Capture(CmDataCapture.MaskExtensive, "Run():AFTER GetProcessMessage");
#endif
                }
                catch (DllNotFoundException e)
                {
                    CmDataCapture.Capture(CmDataCapture.MaskError, "MyApplication::Run() - DllNotFoundException caught: " + e.Message);
                    break;
                }
                catch (Exception e)
                {
                    CmDataCapture.Capture(CmDataCapture.MaskError, "Run():EXCEPTION " + e.Message);
                }
            }
#if	(!WINCE)
            PInvoke.WaitForSingleObject(exitEvent, PInvoke.INFINITE);
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
                CmDataCapture.Capture(CmDataCapture.MaskError, "MyApplication::Exit() calling Abort()");
                Abort();
            }
            catch (DllNotFoundException e)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "MyApplication::Exit() - DllNotFoundException caught: " + e.Message);
            }
#if	(!WINCE)
            PInvoke.SetEvent(exitEvent);
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
        static void Main(string[] args)
        {

            int instanceCount = 0;
            try
            {
                instanceCount = Process.GetProcessesByName(Process.GetCurrentProcess().ProcessName).Length;
            }
            catch (Exception e)
            {
                // XPe bug. Fixed in XPe service pack 2.
                CmDataCapture.Capture(CmDataCapture.MaskError, "Main(): EXCEPTION " + e.Message);
            }
            if (instanceCount <= 1)
            {
                // Preload DLLs //
                remoteMessageDllModule = PInvoke.LoadLibrary(remoteMessageDll);
                scannerDllModule = PInvoke.LoadLibrary(ScannerThread.scannerDll);
#if( WINCE )
				// Preload DLLs //
				simpleHashDllModule = PInvoke.LoadLibrary( SHA256Managed.simpleHashDll );
#endif

                //Update Hand scanner profile if needed
                ConfigureHandScannerProfile();

                theApplication = new MyApplication();

                theApplication.Run();


                if (IntPtr.Zero != remoteMessageDllModule)
                {
                    PInvoke.FreeLibrary(remoteMessageDllModule);
                }
                if (IntPtr.Zero != scannerDllModule)
                {
                    PInvoke.FreeLibrary(scannerDllModule);
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
                CmDataCapture.Capture(CmDataCapture.MaskError, "MyApplication::Main() - More than one instance of application running. Exiting...");
            }
        }

        private static void ConfigureHandScannerProfile()
        {
            if (Environment.MachineName.ToUpper().Contains("RAP"))
            {
                var deviceSvrConfigured = Registry.GetValue(@"HKEY_LOCAL_MACHINE\SOFTWARE\NCR\SCOT\DeviceServer\Devices\Scanner\Profiles\PSC_SCANNER", "Configure", "") as string;
                var currentScnrProfile = Registry.GetValue(@"HKEY_LOCAL_MACHINE\Software\NCR\NCRRapRemoteScanner", "ScannerProfile", "") as string;
                var pscScnrProfile = Registry.GetValue(@"HKEY_LOCAL_MACHINE\Software\OleForRetail\ServiceOPOS\Scanner\PSC_SCANNER", "Version", "") as string;
                var rapScannerProfile = Registry.GetValue(@"HKEY_LOCAL_MACHINE\Software\OleForRetail\ServiceOPOS\Scanner\RAPScanner", "RemoteProfile", "") as string;

                //If all the following conditions met,
                //NCRDeviceServer is configured to load PSC_SCANNER
                //NCRRAPRemoteScanner is set to RAPScanner
                //RAPScanner OPOS profile points to PSC_SCANNER
                //PSC_SCANNER profile is installed
                if ((!string.IsNullOrEmpty(deviceSvrConfigured) && deviceSvrConfigured.Equals("yes", StringComparison.OrdinalIgnoreCase)) &&
                    (!string.IsNullOrEmpty(currentScnrProfile) && currentScnrProfile.Equals("RAPScanner", StringComparison.OrdinalIgnoreCase)) &&
                    !string.IsNullOrEmpty(rapScannerProfile) && rapScannerProfile.Contains("PSC_SCANNER") &&
                    !string.IsNullOrEmpty(pscScnrProfile))
                {

                    Registry.SetValue(@"HKEY_LOCAL_MACHINE\SOFTWARE\NCR\SCOT\DeviceServer\Devices\Scanner\Profiles\PSC_SCANNER", "Configure", "No");
                    Registry.SetValue(@"HKEY_LOCAL_MACHINE\Software\NCR\NCRRapRemoteScanner", "ScannerProfile", "PSC_SCANNER");
                    Registry.SetValue(@"HKEY_LOCAL_MACHINE\Software\NCR\SCOT\Installation\RAPScanner", "Configure", "No");
                    Registry.SetValue(@"HKEY_LOCAL_MACHINE\Software\NCR\NCRLaunchPadScanner", "ScannerProfile", "");
                }
            }

            //Below change is needed on both Lane & RAP, if the below registry key exists and set to Yes
            var remoteRAPScanConfigured = Registry.GetValue(@"HKEY_LOCAL_MACHINE\Software\NCR\SCOT\CurrentVersion\RAP\Remote\Devices\Scanner\RAPScanner", "Configure", "") as string;
            if (!string.IsNullOrEmpty(remoteRAPScanConfigured) && remoteRAPScanConfigured.Equals("Yes", StringComparison.OrdinalIgnoreCase))
            {
                Registry.SetValue(@"HKEY_LOCAL_MACHINE\Software\NCR\SCOT\CurrentVersion\RAP\Remote\Devices\Scanner\RAPScanner", "Configure", "No");
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

        [DllImport(remoteMessageDll, CharSet = CharSet.Unicode, SetLastError = false)]
        private static extern void Abort();

        [DllImport(remoteMessageDll, CharSet = CharSet.Unicode, SetLastError = false)]
        private static extern bool GetProcessMessage(StringBuilder sender, ref int senderLen, StringBuilder message, ref int messageLen);

        [DllImport(remoteMessageDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern bool SendProcessMessage(string target, string message);

#if( DEBUG )
		private const string remoteMessageDll = "RemoteMessageUD.dll";
#else
        private const string remoteMessageDll = "RemoteMessageU.dll";
#endif

    }
}
