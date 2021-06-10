using System;
using System.Xml;
using System.Threading;
using System.Runtime.InteropServices;
using System.Text;
using RPSWNET;

namespace RapNet
{
	/// <summary>
	/// Remote communication singleton used by the application.
	/// </summary>
    public class Remote : IDisposable
	{
        /// <summary>
        /// The class constructor.
        /// </summary>
        public Remote()
        {
        }

        /// <summary>
        /// The class destructor.
        /// </summary>
        ~Remote()
        {
            Dispose();
        }

        /// <summary>
        /// Releases the resources used by the Remote instance.
        /// </summary>
        public virtual void Dispose()
        {
            disposing = true;            
            GC.SuppressFinalize(this); 
        }

        /// <summary>
        /// Get the one and only Remote instance.
        /// </summary>
        /// <returns>
        /// Remote instance.
        /// </returns>
        public static Remote GetRemote()
        {
            if (theRemote == null)
            {
                theRemote = ConfigData.LoadClass("Remote") as Remote;
            }
            return theRemote;
        }

        /// <summary>
        /// Read-Only property that determines if LaunchPad is running.
        /// </summary>
        public static bool IsLaunchPadRunning
        {
            get
            {
                if (-1 == isLaunchPadRunning)
                {
                    isLaunchPadRunning = (IntPtr.Zero == PInvoke.FindWindow(null, "LaunchPadNet")) ? 0 : 1;
                }
                return (isLaunchPadRunning == 1);
            }
        }

        public static bool SendMessage(string target, string message)
        {
            try
            {
                return SendProcessMessage(target, message);
            }
            catch (SystemException e)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "Remote::SendMessage() - Exception caught: " + e.Message);
            }
            return false;
        }
                
        [DllImport(RemoteMessageDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern void Abort();

        [DllImport(RemoteMessageDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern bool GetProcessMessage(StringBuilder sender, ref int senderLen, StringBuilder message, ref int messageLen);
        
        [DllImport(RemoteMessageDll, CharSet = CharSet.Unicode, SetLastError = false)]
        protected static extern bool SendProcessMessage(string target, string message);

        /// <summary>
        /// FastLane Psx service.
        /// </summary>
        public const string FastlaneService = "FastLaneRemoteServer";
        
        /// <summary>
        /// Module name of the LaunchPad Application
        /// </summary>
        public const string LaunchpadApplication = "LaunchPadNet.exe";

#if (DEBUG)
        public const string RemoteMessageDll = "RemoteMessageUD.dll";
#else
        public const string RemoteMessageDll = "RemoteMessageU.dll";
#endif

        protected bool disposing = false;

        /// <summary>
        /// Flag that determines if LaunchPad is running.
        /// </summary>
        static private int isLaunchPadRunning = -1;
               
        // <summary>
        /// The one and only Remote instance.
        /// </summary>
        private static Remote theRemote;
    }
}
