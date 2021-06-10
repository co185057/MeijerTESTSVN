using System;
using System.Threading;
using System.Runtime.InteropServices;
using System.Text;
using System.Diagnostics;
using RPSWNET;
using PsxNet;
using System.ServiceProcess;

namespace RapNet
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
        }

        /// <summary>
        /// The class destructor.
        /// </summary>
        ~MyApplication()
        {
            Dispose();
        }

        /// <summary>
        /// Releases the resources used by the MyApplication instance.
        /// </summary>
        public void Dispose()
        {
            Remote.GetRemote().Dispose();

            myPsx.Closed -= new EventHandler(myPsx_Closed);
            myPsx.Dispose();
            myPsx = null;
#if (!WINCE)
            GC.SuppressFinalize(this);
#endif
        }

        /// <summary>
        /// Initialize the application.
        /// </summary>
        private bool Initialize()
        {
            myPsx = ConfigData.LoadClass("SharedPsx", new Object[] { new Psx() }) as SharedPsx;
            myPsx.Closed += new EventHandler(myPsx_Closed);
            if (myPsx.Initialize())
            {
                SharedPsx.LogMemoryUse(CmDataCapture.MaskInfo);
                return true;
            }
            return false;
        }

        private void myPsx_Closed(Object sender, EventArgs args)
        {
            Exit();
        }

        /// <summary>
        /// Runs the application.
        /// </summary>
        protected void Run()
        {
            while (!autoEvent.WaitOne(0, false))
            {
                try
                {
                    StringBuilder sender = new StringBuilder(250);
                    int senderLen = sender.Capacity;
                    StringBuilder message = new StringBuilder(250);
                    int messageLen = message.Capacity;
                    if (Remote.GetProcessMessage(sender, ref senderLen, message, ref messageLen))
                    {
                        string strSrc = sender.ToString();
                        string strMsg = message.ToString();
                        CmDataCapture.Capture(CmDataCapture.MaskExtensive,
                            "Run():GetProcessMessage - Received from '" + strSrc + "' message: " + strMsg);
                        myPsx.HandleProcessMessage(strSrc, strMsg);
                    }
                }
                catch (SystemException e)
                {
                    CmDataCapture.Capture(CmDataCapture.MaskWarning, "MyApplication::Run() - SystemException caught: " + e.Message);
                    autoEvent.WaitOne();
                    break;
                }
                catch (Exception caught)
                {
                    CmDataCapture.Capture(CmDataCapture.MaskError, "MyApplication::Run() - Exception: " + caught.Message);
                    throw caught;
                }
            }
        }

        /// <summary>
        /// Exits the application.
        /// </summary>
        private void Exit()
        {
            try
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "MyApplication::Exit()");
                SharedPsx.LogMemoryUse(CmDataCapture.MaskInfo);

                // Disrupt socket traffic to accelerate shutdown //
                WSACleanup();
                Remote.Abort();
            }
            catch (SystemException e)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "MyApplication::Exit() - SystemException caught: " + e.Message);
            }
            catch (Exception caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "MyApplication::Exit() - Exception caught:" + caught.Message);
                throw caught;
            }
            autoEvent.Set();
        }

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
#if (!WINCE)
        [STAThread]
#endif
        static void Main(string[] args)
        {
            ConfigData.LoadRegistry();
            CmDataCapture.Initialize("-- RapNet Start Tracing --");
            CmDataCapture.Capture(CmDataCapture.MaskInfo, "Extenstion Assembly: " + ConfigData.ExtensionAssembly);
            bool bConfigError = true;

            try
            {
                ConfigData configData = ConfigData.GetConfigData();
                configData.Load();
                CmDataCapture.Capture(CmDataCapture.MaskInfo, "Configuration loaded from config file.");
                bConfigError = false;
            }
            catch (Exception excp)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, String.Format("Error: {0}", excp.Message));
                CmDataCapture.Capture(CmDataCapture.MaskError, String.Format("Stack Trace: {0}", excp.StackTrace));
                CmDataCapture.Capture(CmDataCapture.MaskError, "MyApplication::Main(): Unable to load configuration, Exiting application.");
            }

            try
            {
                ServiceController sc = new ServiceController("SAServer");
                if (sc.Status == ServiceControllerStatus.Stopped)
                {
                    sc.Start();
                }
            }
            catch (Exception ex)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, String.Format("Failed to start SAServer: {0}", ex.Message));
                CmDataCapture.Capture(CmDataCapture.MaskError, String.Format("Stack Trace: {0}", ex.StackTrace));
            }

            if (!bConfigError)
            {
                int instanceCount = 0;
                try
                {
                    instanceCount = Process.GetProcessesByName(Process.GetCurrentProcess().ProcessName).Length;
                }
                catch
                { /* Nothing to be done */}

                try
                {
                    if (instanceCount <= 1)
                    {
                        // Preload DLLs
                        remoteMessageDllModule = LoadLibrary(Remote.RemoteMessageDll);
                        scannerDllModule = LoadLibrary(ScannerThread.scannerDll);

                        // Load the application
                        MyApplication theApplication = new MyApplication();
                        if (theApplication.Initialize())
                        {
                            theApplication.Run();
                        }
                        theApplication.Dispose();
                    }
                    else
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "MyApplication::Main() - More than one instance of application running. Exiting...");
                    }
                    if (IntPtr.Zero != remoteMessageDllModule)
                    {
                        FreeLibrary(remoteMessageDllModule);
                    }
                    if (IntPtr.Zero != scannerDllModule)
                    {
                        FreeLibrary(scannerDllModule);
                    }
                }
                catch (Exception excp)
                {
                    CmDataCapture.Capture(CmDataCapture.MaskError, String.Format("Error: {0}", excp.Message));
                    CmDataCapture.Capture(CmDataCapture.MaskError, String.Format("Stack Trace: {0}", excp.StackTrace));
                    CmDataCapture.Capture(CmDataCapture.MaskError, "MyApplication::Main(): Unexpected exception occurreed, Exiting application.");
                }
            }

            CmDataCapture.Flush();
            CmDataCapture.Cleanup();
        }

        /// <summary>
        /// Event handle used to signal the exit of the application.
        /// </summary>
        private AutoResetEvent autoEvent = new AutoResetEvent(false);

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

        [DllImport(PInvoke.kernelApiDll, CharSet = CharSet.Unicode, SetLastError = false)]
        private static extern IntPtr LoadLibrary(string lpFileName);

        [DllImport(PInvoke.kernelApiDll, CharSet = CharSet.Unicode, SetLastError = false)]
        private static extern bool FreeLibrary(IntPtr hModule);

        [DllImport(PInvoke.winsockDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern int WSACleanup();
    }
}

