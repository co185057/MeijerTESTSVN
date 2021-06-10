using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Threading;

namespace BioDevMgr
{
    public class ACSBiomet : PluginDriver
    {
        private TraceHelper trace = new TraceHelper("ACSBiomet");

        /// <summary>
        /// Releases the resources used by the Plugin instance.
        /// </summary>
        public override void Dispose()
        {
            base.Dispose();
        }

        public override void Start()
        {
            try
            {
                PluginActivity = PluginActivityTypes.Processing;

                if (PluginStatus == PluginStatusTypes.Started)
                {
                    trace.Warning("Plugin is already running. Ingnoring request to start.");
                    return;
                }

                LoadLibraryAndFunctions();

                trace.Info("Calling BiometInitialize(true) from biomet.dll");
                int ret = BiometInitialize(true);
                if (ret != 0)
                {
                    ret = BiometInitialize(false);
                    string errMsg = "BiometInitialize() failed with error " + ret.ToString();
                    trace.Error(errMsg);
                    OnPluginEvent(new PluginEventArgs(PluginEventArgs.PluginEventTypes.OnError, errMsg, null));
                    return;
                }
                trace.Info("Calling BiometFlush() from biomet.dll");
                BiometFlush();

                // Required to ensure biomet events don't get filtered out.
                trace.Info("Calling BiometSetDFType(DP_DF_TYPE.DP_DF_SUPV_OVERRIDE) from biomet.dll");
                BiometSetDFType(DP_DF_TYPE.DP_DF_SUPV_OVERRIDE);

                // Start the monitoring thread
                trace.Info("Starting the MonitorBiometDevice thread");
                monitorBiometDeviceThread = new Thread(MonitorBiometDeviceProc);
                monitorBiometDeviceThread.Start();

                base.Start();
            }
            finally
            {
                PluginActivity = PluginActivityTypes.Idle;
            }
        }

        public override void Stop()
        {
            try
            {
                PluginActivity = PluginActivityTypes.Processing;

                if (PluginStatus != PluginStatusTypes.Started)
                {
                    trace.Warning("Plugin is not currently running ignoring request to stop.");
                    return;
                }

                trace.Info("Signaling the MonitorBiometDevice thread to stop");
                exitMonitorThreadEvent.Set();
                monitorBiometDeviceThread.Join();
                monitorBiometDeviceThread = null;
                trace.Info("Calling BiometFlush() from biomet.dll");
                BiometFlush();
                trace.Info("Calling BiometInitialize(false) from biomet.dll");
                BiometInitialize(false);
                UnloadLibraryAndFunctions();
                base.Stop();
            }
            finally
            {
                PluginActivity = PluginActivityTypes.Idle;
            }
        }

        protected override void Load()
        {
            try
            {
                PluginActivity = PluginActivityTypes.Processing;

                base.Load();
            }
            finally
            {
                PluginActivity = PluginActivityTypes.Idle;
            }
        }

        public override void Unload()
        {
            try
            {
                PluginActivity = PluginActivityTypes.Processing;

                base.Unload();
            }
            finally
            {
                PluginActivity = PluginActivityTypes.Idle;
            }
        }

        private void MonitorBiometDeviceProc()
        {
            trace.Info("Entering MonitorBiometDeviceProc() thread");

            while (!exitMonitorThreadEvent.WaitOne(0, false))
            {
                int ret = BiometCasinputProcess();
                PluginActivity = PluginActivityTypes.Processing;

                bool dataAvailable = false;
                int biometStat = 0;
                int featuresSize = 0;
                IntPtr featuresBuf = IntPtr.Zero;
                BiometGetTlogInfo(ref dataAvailable, ref biometStat, ref featuresSize, ref featuresBuf);

                // Send an event passing the data
                if (dataAvailable)
                {
                    trace.Info("Biometric data is available - biometStat: ");
                    trace.Info("Biometric data received of size " + featuresSize.ToString() + " bytes. Notifying BioDevMgr.");
                    byte[] data = new byte[featuresSize];
                    System.Runtime.InteropServices.Marshal.Copy(featuresBuf, data, 0, featuresSize);
                    trace.Info("Calling BiometFlush() from biomet.dll");
                    BiometFlush();
                    OnPluginEvent(new PluginEventArgs(PluginEventArgs.PluginEventTypes.OnData, data, null));
                }
                PluginActivity = PluginActivityTypes.Idle;
                Thread.Sleep(Int32.Parse(Config.GetParameter("PollPeriod")));
            }
            trace.Info("Exiting MonitorBiometDeviceProc() thread");
        }

        private void LoadLibraryAndFunctions()
        {
            trace.Info("+LoadLibraryAndFunctions()");
            biometDll = Config.GetParameter("BiometLibraryPath");
            trace.Info("Loading the " + biometDll + " library.");
            biometDllModule = PInvoke.LoadLibrary(biometDll);

            if (IntPtr.Zero != biometDllModule)
            {
                IntPtr address = PInvoke.GetProcAddress(biometDllModule, "BiometCasinputProcess");
                if (IntPtr.Zero != address)
                {
                    BiometCasinputProcess =
                        (BiometCasinputProcessDelegate)Marshal.GetDelegateForFunctionPointer(
                                                                address, typeof(BiometCasinputProcessDelegate)
                                                                );
                }
                address = PInvoke.GetProcAddress(biometDllModule, "BiometFlush");
                if (IntPtr.Zero != address)
                {

                    BiometFlush = (BiometFlushDelegate)Marshal.GetDelegateForFunctionPointer(
                                                                address, typeof(BiometFlushDelegate)
                                                                );
                }
                address = PInvoke.GetProcAddress(biometDllModule, "BiometGetMatchOpAcct");
                if (IntPtr.Zero != address)
                {

                    BiometGetMatchOpAcct = (BiometGetMatchOpAcctDelegate)Marshal.GetDelegateForFunctionPointer(
                                                                address, typeof(BiometGetMatchOpAcctDelegate)
                                                                );
                }
                address = PInvoke.GetProcAddress(biometDllModule, "BiometGetTlogInfo");
                if (IntPtr.Zero != address)
                {

                    BiometGetTlogInfo = (BiometGetTlogInfoDelegate)Marshal.GetDelegateForFunctionPointer(
                                                                address, typeof(BiometGetTlogInfoDelegate)
                                                                );
                }
                address = PInvoke.GetProcAddress(biometDllModule, "BiometInitialize");
                if (IntPtr.Zero != address)
                {

                    BiometInitialize = (BiometInitializeDelegate)Marshal.GetDelegateForFunctionPointer(
                                                                address, typeof(BiometInitializeDelegate)
                                                                );
                }
                address = PInvoke.GetProcAddress(biometDllModule, "BiometSetDFType");
                if (IntPtr.Zero != address)
                {

                    BiometSetDFType = (BiometSetDFTypeDelegate)Marshal.GetDelegateForFunctionPointer(
                                                                address, typeof(BiometSetDFTypeDelegate)
                                                                );
                }
                address = PInvoke.GetProcAddress(biometDllModule, "BiometSetOpAcct");
                if (IntPtr.Zero != address)
                {

                    BiometSetOpAcct = (BiometSetOpAcctDelegate)Marshal.GetDelegateForFunctionPointer(
                                                                address, typeof(BiometSetOpAcctDelegate)
                                                                );
                }
                address = PInvoke.GetProcAddress(biometDllModule, "BiometTrace");
                if (IntPtr.Zero != address)
                {

                    BiometTrace = (BiometTraceDelegate)Marshal.GetDelegateForFunctionPointer(
                                                                address, typeof(BiometTraceDelegate)
                                                                );
                }
            }
            else
            {
                trace.Error("Failed to load library " + biometDll);
            }
            trace.Info("-LoadLibraryAndFunctions()");
        }

        private void UnloadLibraryAndFunctions()
        {
            trace.Info("+UnloadLibraryAndFunctions()");
            if (IntPtr.Zero != biometDllModule)
            {
                trace.Info("Unloading the " + biometDll + " library.");
                PInvoke.FreeLibrary(biometDllModule);
                biometDllModule = IntPtr.Zero;
            }
            trace.Info("-UnloadLibraryAndFunctions()");
        }

        static private IntPtr biometDllModule = IntPtr.Zero;
        private string biometDll = "";
        private Thread monitorBiometDeviceThread = null;
        private EventWaitHandle exitMonitorThreadEvent = new AutoResetEvent(false);

        public enum DP_DF_TYPE
        {
            DP_DF_NONE,
            DP_DF_SIGNIN,
            DP_DF_PASSWORD,
            DP_DF_SUPV_OVERRIDE,
            DP_DF_SUPV_PASSWORD,
            DP_DF_CUST_ID,
            DP_DF_PUNCH_FP_ID
        };

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate int BiometCasinputProcessDelegate();

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void BiometFlushDelegate();

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void BiometGetMatchOpAcctDelegate([Out] string opAcct);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        //private delegate void BiometGetTlogInfoDelegate(ref bool tlogDataAvailable, ref int biometStat, ref int featBufSize, [MarshalAs(UnmanagedType.LPArray)] ref byte[] featuresBuf); 
        //private delegate void BiometGetTlogInfoDelegate(ref bool tlogDataAvailable, ref int biometStat, ref int featBufSize, ref byte[] featuresBuf);
        private delegate void BiometGetTlogInfoDelegate(ref bool tlogDataAvailable, ref int biometStat, ref int featBufSize, ref IntPtr featuresBuf);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate int BiometInitializeDelegate([In] bool useIt);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void BiometSetDFTypeDelegate([In] DP_DF_TYPE dfType);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void BiometSetOpAcctDelegate([In] string opAcct);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void BiometTraceDelegate([In] string buf);

        private BiometCasinputProcessDelegate BiometCasinputProcess = null;
        private BiometFlushDelegate BiometFlush = null;
        private BiometGetMatchOpAcctDelegate BiometGetMatchOpAcct = null;
        private BiometGetTlogInfoDelegate BiometGetTlogInfo = null;
        private BiometInitializeDelegate BiometInitialize = null;
        private BiometSetDFTypeDelegate BiometSetDFType = null;
        private BiometSetOpAcctDelegate BiometSetOpAcct = null;
        private BiometTraceDelegate BiometTrace = null;
    }
}
