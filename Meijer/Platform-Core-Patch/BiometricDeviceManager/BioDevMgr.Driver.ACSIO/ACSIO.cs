using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Threading;

namespace BioDevMgr
{
    /// <summary>
    /// Plugin that implements the ACSIO channel driver for biometrics
    /// This plugin is active when the sales application is running and 
    /// we are not in direct contro of the physical biometric reader 
    /// instead we receive ACS contexts that contain the biometric
    /// fingerprint data
    /// </summary>
    public class ACSIO : PluginDriver
    {
        private TraceHelper trace = new TraceHelper("ACSIO");

        protected override void Load()
        {
            try
            {
                PluginActivity = PluginActivityTypes.Processing;
                contextName = Config.GetParameter("Context");
                idField = Config.GetParameter("MatchField");
                idFieldMatch = Config.GetParameter("MatchFieldValue");
                dataField = Config.GetParameter("DataField");
                sizeField = Config.GetParameter("SizeField");

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

        public override void Start()
        {
            try
            {
                PluginActivity = PluginActivityTypes.Processing;
                trace.Info("Loading Context & Sequence data structures");
                
                uint ret = DkInitialize();

                if (FAILED(ret))
                {
                    string errMsg = "Failed to initialize the ACSIODataKit DLL.";
                    trace.Error(errMsg);
                    OnPluginEvent(new PluginEventArgs(PluginEventArgs.PluginEventTypes.OnError, errMsg, null));
                    return;
                }

                ret = DkLoadContextStructures();

                if (FAILED(ret))
                {
                    string errMsg = "Failed to load Context structures via ACSIODataKit DLL.";
                    trace.Error(errMsg);
                    OnPluginEvent(new PluginEventArgs(PluginEventArgs.PluginEventTypes.OnError, errMsg, null));
                    return;
                }


                ushort type = 0;
                uint size = 0;
                uint offset = 0;

                /* Cache the indices of the fields we want to extract from the contextValues array. 
                 The other parameters (type, size, offset) aren't used, but I can't find a way to handle 
                 optional parameters. */
                ret = DkGetContextFieldInfoEx(
                    contextName, idField, out type, out size, out offset, out idFieldIndex);

                if (SUCCEEDED(ret))
                {
                    ret = DkGetContextFieldInfoEx(
                        contextName, dataField, out type, out size, out offset, out dataFieldIndex);

                    if (SUCCEEDED(ret))
                    {
                        ret = DkGetContextFieldInfoEx(
                            contextName, sizeField, out type, out size, out offset, out sizeFieldIndex);
                    }
                }
                
                if (FAILED(ret))
                {
                    string errMsg = "Failed to load requested field indices via ACSIODataKit DLL.";
                    trace.Error(errMsg);
                    DkUnloadContextStructures();
                    DkUninitialize();
                    OnPluginEvent(new PluginEventArgs(PluginEventArgs.PluginEventTypes.OnError, errMsg, null));
                    return;
                }

                trace.Info("Initialize the ACSIO Context channel");
                ret = ACSIOInitialize(ACSIO_SINGLETHREADED, ref contextHandle);

                if ((ret != RPC_E_CHANGED_MODE) && FAILED(ret))
                {
                    string errMsg = "ACSIOInitialize failed with error: " + ret.ToString();
                    trace.Error(errMsg);
                    DkUnloadContextStructures();
                    DkUninitialize();
                    OnPluginEvent(new PluginEventArgs(PluginEventArgs.PluginEventTypes.OnError, errMsg, null));
                    return;
                }

                trace.Info("Connect to the ACSIO Context channel");
                ret = ACSIOConnectContextChannel(contextHandle);

                if (FAILED(ret))
                {
                    string errMsg = "ACSIOConnectContextChannel failed to connect to the context channel. ";
                    errMsg = errMsg + GetErrorText(contextHandle);
                    trace.Error(errMsg);
                    ACSIOUninitialize(contextHandle);
                    contextHandle = IntPtr.Zero;
                    DkUnloadContextStructures();
                    DkUninitialize();
                    OnPluginEvent(new PluginEventArgs(PluginEventArgs.PluginEventTypes.OnError, errMsg, null));
                    return;
                }

                trace.Info("Setup and begin listening for ACSIOClient Contexts");
                int maxEntries = 0;
                if( int.TryParse(Config.GetParameter("ACSIOQueueEntries"), out maxEntries) )
                {
                    trace.Info("Setting ACSIOQueueEntries to " + maxEntries.ToString());
                    ACSIOSetMaxQueueEntries(contextHandle, maxEntries);
                }

                ret = ACSIOAdviseEx(contextHandle);

                if (FAILED(ret))
                {
                    string errMsg = "ACSIOAdviseEx failed with error " + GetErrorText(contextHandle);
                    trace.Error(errMsg);
                    OnPluginEvent(new PluginEventArgs(PluginEventArgs.PluginEventTypes.OnError, errMsg, null));
                    ACSIOUninitialize(contextHandle);
                    contextHandle = IntPtr.Zero;
                    DkUnloadContextStructures();
                    DkUninitialize();
                    OnPluginEvent(new PluginEventArgs(PluginEventArgs.PluginEventTypes.OnError, errMsg, null));
                    return;
                }

                dispatchContextsThread = new Thread(DispatchContextsProc);
                dispatchContextsThread.Start();

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
                trace.Info("Signaling DispatchContexts Thread to terminate...");
                exitEvent.Set();

                /* Wait for thread to terminate before continuing the tear-down. */
                dispatchContextsThread.Join();
                dispatchContextsThread = null;

                uint ret = ACSIOUnadviseEx(contextHandle);

                if (FAILED(ret))
                {
                    trace.Error("ACSIOUnadviseEx failed. " + GetErrorText(contextHandle));
                }
                
                ret = ACSIOUninitialize(contextHandle);

                if (FAILED(ret))
                {
                    trace.Error("ACSIOUninitialize failed. " + GetErrorText(contextHandle));
                }
                
                contextHandle = IntPtr.Zero;

                /* Due to vagaries in how ACSIOClient.dll currently uses ACSIODataKit, it isn't necessary 
                to unload the context structures explicitly. */
                DkUninitialize();

                base.Stop();
            }
            finally
            {
                PluginActivity = PluginActivityTypes.Idle;
            }
        }

        private void DispatchContextsProc()
        {
            IntPtr [] waitHandles = { exitEvent.Handle };
            bool exitLoop = false;

            while (!exitLoop)
            {
                /* Wait for either a new ACSIO context message or a signal that the thread should terminate. */
                uint ret = ACSIOWaitForDataEventMsg(
                    contextHandle, 
                    (uint)waitHandles.Length, 
                    waitHandles, 
                    INFINITE, 
                    (uint)QueueStatusFlags.QS_ALLEVENTS, 
                    0);

                if (ret == ACSIO_OBJECT_0)
                {
                    string newContextName;
                    object contextData;

                    /* Retrieve the name of the new context and an array of its field values. */
                    ret = ACSIOGetContextFields(contextHandle, out newContextName, out contextData);

                    if (FAILED(ret))
                    {
                        trace.Error("An error occured retrieving a context with ACSIOGetContext. " + GetErrorText(contextHandle));
                    }
                    else
                    {
                        object[] contextValues = (object[])contextData;
                        ProcessContextsProc(newContextName, contextValues);
                    }
                }
                else if (ret == WAIT_OBJECT_0)
                {
                    exitLoop = true;
                }
                else
                {
                    MSG msg = new MSG();

                    while (GetMessage(out msg, IntPtr.Zero, 0, 0))
                    {
                        TranslateMessage(ref msg);
                        DispatchMessage(ref msg);
                    }
                }
            }

            trace.Info("Exiting Dispatch Contexts Processor Thread");
        }

        private void ProcessContextsProc(string newContext, object[] contextValues)
        {
            try
            {
                PluginActivity = PluginActivityTypes.Processing;
                trace.Info("Received context " + newContext);

                /* If this is the context we're looking for... */
                if (contextName == newContext)
                {
                    try
                    {
                        /* The VARIANT value we want is at position denoted by the index. */
                        object fieldValue = contextValues[idFieldIndex];

                        /* If the string value of the field is what we're looking for...*/
                        if (fieldValue.ToString() == idFieldMatch)
                        {
                            /* Retrieve the data field. */
                            fieldValue = contextValues[dataFieldIndex];

                            /* Get the field that contains the size of the data field */
                            object fieldValueSize = contextValues[sizeFieldIndex];

                            if ((Int32)fieldValueSize > 0)
                            {
                                // build a byte array with the string pairs
                                char[] hexPairs = ((string)fieldValue).ToUpper().ToCharArray();

                                // fetch the data and in the array
                                Int32 fieldSize = (Int32)fieldValueSize;
                                byte[] data = new byte[fieldSize / 2];

                                for (int i = 0; i < (fieldSize - 1); i += 2)
                                {
                                    char high = hexPairs[i];
                                    char low = hexPairs[i + 1];

                                    // high order nibble
                                    if (high >= '0' && high <= '9')
                                    {
                                        data[i / 2] = (byte)((high - '0') << 4);
                                    }
                                    else if (high >= 'A' && high <= 'F')
                                    {
                                        data[i / 2] = (byte)(((high - 'A') + 10) << 4);
                                    }

                                    // low order nibble
                                    if (low >= '0' && low <= '9')
                                    {
                                        data[i / 2] |= (byte)(low - '0');
                                    }
                                    else if (low >= 'A' && low <= 'F')
                                    {
                                        data[i / 2] |= (byte)((low - 'A') + 10);
                                    }
                                }

                                OnPluginEvent(new PluginEventArgs(PluginEventArgs.PluginEventTypes.OnData, data, null));
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        string errMsg = "ProcessContextsProc exception occured. " + ex.Message;
                        trace.Error(errMsg);
                        OnPluginEvent(new PluginEventArgs(PluginEventArgs.PluginEventTypes.OnError, errMsg, null));
                    }
                }
            }
            finally
            {
                PluginActivity = PluginActivityTypes.Idle;
            }
        }

        private string GetErrorText(IntPtr client)
        {
            StringBuilder buffer = new StringBuilder(4000);
            ACSIOGetErrorText(client, buffer, buffer.Capacity);
            return buffer.ToString();
        }

        private IntPtr contextHandle = IntPtr.Zero;
        private Thread dispatchContextsThread = null;

        // following fields are used for context match & data extraction
        private string contextName;
        private string idFieldMatch;
        
        private string idField;
        private uint idFieldIndex;

        private string dataField;
        private uint dataFieldIndex;

        private string sizeField;
        private uint sizeFieldIndex;

        private const uint COINIT_APARTMENTTHREADED = 0x00000002;
        private const uint ACSIO_SINGLETHREADED = COINIT_APARTMENTTHREADED;
        private const uint RPC_E_CHANGED_MODE = 0x80010106;
        
        private static bool FAILED(uint ret) 
        { 
            return (ret & 0xF0000000) == 0xF0000000; 
        }

        private static bool SUCCEEDED(uint Status) 
        {
            return (uint)(Status) >= 0; 
        }

        private const uint INFINITE = 0xFFFFFFFF;

        private EventWaitHandle exitEvent = new ManualResetEvent(false);

        const uint SEVERITY_SUCCESS = 0;
        const uint SEVERITY_ERROR = 1;
        const uint FACILITY_ITF = 4;

        private static uint MAKE_HRESULT(uint sev, uint fac, uint code) 
        {
            return ((uint)(((uint)(sev) << 31) | ((uint)(fac) << 16) | ((uint)(code))));
        }

        /* Wait flags */
        const uint WAIT_ABANDONED = 0x00000080;
        const uint WAIT_OBJECT_0 = 0x00000000;
        const uint WAIT_TIMEOUT = 0x00000102;

        static uint ACSIO_NO_DATA = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 900);
        static uint ACSIO_NOT_INITIALIZED = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 901);
        static uint ACSIO_NOT_CONNECTED = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 902);
        static uint ACSIO_ALREADY_CONNECTED = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 903);

        static uint ACSIO_OBJECT_0 = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, 950);

        [Flags]
        public enum QueueStatusFlags : uint
        {
            QS_KEY = 0x0001,
            QS_MOUSEMOVE = 0x0002,
            QS_MOUSEBUTTON = 0x0004,
            QS_POSTMESSAGE = 0x0008,
            QS_TIMER = 0x0010,
            QS_PAINT = 0x0020,
            QS_SENDMESSAGE = 0x0040,
            QS_HOTKEY = 0x0080,
            QS_ALLPOSTMESSAGE = 0x0100,
            QS_RAWINPUT = 0x0400,
            QS_MOUSE = QS_MOUSEMOVE | QS_MOUSEBUTTON,
            QS_INPUT = QS_MOUSE | QS_KEY | QS_RAWINPUT,
            QS_ALLEVENTS = QS_INPUT | QS_POSTMESSAGE | QS_TIMER | QS_PAINT | QS_HOTKEY,
            QS_ALLINPUT = QS_INPUT | QS_POSTMESSAGE | QS_TIMER | QS_PAINT | QS_HOTKEY | QS_SENDMESSAGE
        }


        /* ACSIOClient.dll exports */

        [DllImport("ACSIOClient.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint ACSIOInitialize(uint dwCoInit, ref IntPtr hClient);

        [DllImport("ACSIOClient.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint ACSIOUninitialize(IntPtr hClient);

        [DllImport("ACSIOClient.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint ACSIOConnectContextChannel(IntPtr hClient);

        [DllImport("ACSIOClient.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint ACSIOAdviseEx(IntPtr hClient);

        [DllImport("ACSIOClient.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint ACSIOUnadviseEx(IntPtr hClient);

        [DllImport("ACSIOClient.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint ACSIOGetErrorText([In] IntPtr hClient, StringBuilder text, int bufferSize);

        [DllImport("ACSIOClient.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint ACSIOSetMaxQueueEntries([In] IntPtr hClient, int maxEntries);

        /* NOTE: The charset here is Unicode ONLY for this call */
        [DllImport("ACSIOClient.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint ACSIOGetContextFields(
            IntPtr hClient, 
            out string contextName, 
            [MarshalAs(UnmanagedType.Struct)] out object contextValues);

        [DllImport("ACSIOClient.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint ACSIOWaitForDataEventMsg(
            IntPtr hClient, 
            uint handleCount,
            IntPtr [] handles,
            uint timeout,
            uint wakeMask,
            uint flags);
        
        /* ACSIODataKit.dll exports */
        [DllImport("ACSIODataKit.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint DkInitialize();

        [DllImport("ACSIODataKit.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DkUninitialize();

        [DllImport("ACSIODataKit.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint DkLoadContextStructures();

        [DllImport("ACSIODataKit.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint DkUnloadContextStructures();

        [DllImport("ACSIODataKit.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint DkLoadSequenceStructures();

        [DllImport("ACSIODataKit.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint DkUnloadSequenceStructures();

        [DllImport("ACSIODataKit.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint DkGetContextFieldEx([In] string contextName, [In] string fieldName, IntPtr buffer, [MarshalAs(UnmanagedType.Struct)] out object fieldValue);

        [DllImport("ACSIODataKit.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint DkGetContextFieldInfoEx(string contextName, string fieldName, out ushort type, out uint size, out uint offset, out uint index);


        /* Windows APIs */
        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool PeekMessage(out MSG lpMsg, HandleRef hWnd, uint wMsgFilterMin,
           uint wMsgFilterMax, uint wRemoveMsg);

        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool GetMessage(out MSG lpMsg, IntPtr hWnd, uint wMsgFilterMin,
           uint wMsgFilterMax);

        [DllImport("user32.dll")]
        static extern IntPtr DispatchMessage([In] ref MSG lpmsg);

        [DllImport("user32.dll")]
        static extern bool TranslateMessage([In] ref MSG lpMsg);

        public struct MSG
        {
            public IntPtr hwnd;
            public uint message;
            public IntPtr wParam;
            public IntPtr lParam;
            public uint time;
            public POINT pt;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct POINT
        {
            public int X;
            public int Y;

            public POINT(int x, int y)
            {
                this.X = x;
                this.Y = y;
            }
        }
    }
}
