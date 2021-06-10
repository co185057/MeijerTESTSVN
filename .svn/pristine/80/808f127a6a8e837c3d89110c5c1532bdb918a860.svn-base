using System;
using RPSWNET;
using System.Threading;

namespace BioDevMgr
{
    public class SyncEvents
    {
        public SyncEvents()
        {
            newItemEvent = new AutoResetEvent(false);
            exitThreadEvent = new ManualResetEvent(false);
            eventArray = new WaitHandle[2];
            eventArray[0] = newItemEvent;
            eventArray[1] = exitThreadEvent;
        }

        public EventWaitHandle ExitThreadEvent
        {
            get
            {
                return exitThreadEvent;
            }
        }
        public EventWaitHandle NewItemEvent
        {
            get
            {
                return newItemEvent;
            }
        }
        public WaitHandle[] EventArray
        {
            get
            {
                return eventArray;
            }
        }

        public int WaitAny()
        {
            return WaitHandle.WaitAny(EventArray);
        }

        private EventWaitHandle newItemEvent;
        private EventWaitHandle exitThreadEvent;
        private WaitHandle[] eventArray;
    }

    public class SyncEventsLegacy : IDisposable
    {
        public SyncEventsLegacy()
        {
            newItemEvent = PInvoke.CreateEventW(IntPtr.Zero, false, false, null);
            exitThreadEvent = PInvoke.CreateEventW(IntPtr.Zero, true, false, null);
            eventArray = new IntPtr[2];
            eventArray[0] = newItemEvent;
            eventArray[1] = exitThreadEvent;
        }

        ~SyncEventsLegacy()
        {
            Dispose();
        }

        public void Dispose()
        {
            if( IntPtr.Zero != exitThreadEvent )
            {
                PInvoke.CloseHandle(exitThreadEvent);
                exitThreadEvent = IntPtr.Zero;
            }
            if (IntPtr.Zero != newItemEvent)
            {
                PInvoke.CloseHandle(newItemEvent);
                newItemEvent = IntPtr.Zero;
            }
        }

        public IntPtr ExitThreadEvent
        {
            get
            {
                return exitThreadEvent;
            }
        }
        public IntPtr NewItemEvent
        {
            get
            {
                return newItemEvent;
            }
        }
        public IntPtr[] EventArray
        {
            get
            {
                return eventArray;
            }
        }

        public bool SetNewItemEvent()
        {
            return PInvoke.SetEvent(newItemEvent);
        }

        public bool SetExitThreadEvent()
        {
            return PInvoke.SetEvent(exitThreadEvent);
        }

        public uint WaitAny()
        {
            uint ret;
            do
            {
                ret = PInvoke.MsgWaitForMultipleObjects(2, eventArray, false, PInvoke.INFINITE, PInvoke.WakeMask.QS_ALLINPUT);
                if (ret == (PInvoke.WAIT_OBJECT_0 + 2))
                {
                    PInvoke.MSG msg = new PInvoke.MSG();
                    while (PInvoke.PeekMessage(ref msg, 0, 0, 0, PInvoke.PeekMessageOption.PM_REMOVE))
                    {
                        if (msg.message == PInvoke.WM_QUIT)
                        {
                            break;
                        }
                        PInvoke.TranslateMessage(ref msg);
                        PInvoke.DispatchMessage(ref msg);
                    }
                }
            }
            while (ret == (PInvoke.WAIT_OBJECT_0 + 2));
            
            return ret;
        }

        private IntPtr newItemEvent = IntPtr.Zero;
        private IntPtr exitThreadEvent = IntPtr.Zero;
        private IntPtr[] eventArray;
    }

    /// <summary>
    /// Summary description for TraceHelper.
    /// </summary>
    public class TraceHelper
    {
        protected string tracePrefix = "BioDevMgr";

        public TraceHelper(string prefix)
        {
            tracePrefix = prefix;
        }

        public void Error(string msg)
        {
            CmDataCapture.Capture(CmDataCapture.MaskError, tracePrefix + ": " + msg);
        }

        public void Warning(string msg)
        {
            CmDataCapture.Capture(CmDataCapture.MaskWarning, tracePrefix + ": " + msg);
        }
        public void Info(string msg)
        {
            CmDataCapture.Capture(CmDataCapture.MaskInfo, tracePrefix + ": " + msg);
        }
    }
}

