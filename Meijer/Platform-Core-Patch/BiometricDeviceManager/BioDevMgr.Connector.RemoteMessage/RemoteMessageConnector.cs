using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Threading;
using System.IO;
using System.Diagnostics;

namespace BioDevMgr
{
    public class RemoteMessageConnector : PluginConnector
    {
        private TraceHelper trace = new TraceHelper("RemoteMessageConnector");

        /// <summary>
        /// Event handle used to signal the unloading of the plugin.
        /// </summary>
        private AutoResetEvent stopRemoteMessage = new AutoResetEvent(false);
        private Thread remoteMessageProcessingThread;

        public override void Start()
        {
            try
            {
                PluginActivity = PluginActivityTypes.Processing;
                remoteMessageProcessingThread = new Thread(RemoteMessageProcessor);
                remoteMessageProcessingThread.Start();
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
                trace.Info("Signaling the Remote Message Processor to stop");
                stopRemoteMessage.Set();
                string procName = Path.GetFileName(Process.GetCurrentProcess().ProcessName);
                procName += ".exe";
                SendProcessMessage(procName, "operation=stop-plugin");
                remoteMessageProcessingThread.Join();
                remoteMessageProcessingThread = null;
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
                trace.Info("Loading the " + remoteMessageDll + " library.");
                remoteMessageDllModule = PInvoke.LoadLibrary(remoteMessageDll);

                if (IntPtr.Zero != remoteMessageDllModule)
                {
                    base.Load();
                }
                else
                {
                    trace.Error("Failed to load library " + remoteMessageDll);
                }
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
                if (IntPtr.Zero != remoteMessageDllModule)
                {
                    trace.Info("Unloading the " + remoteMessageDll + " library.");
                    PInvoke.FreeLibrary(remoteMessageDllModule);
                    remoteMessageDllModule = IntPtr.Zero;
                }
                base.Unload();
            }
            finally
            {
                PluginActivity = PluginActivityTypes.Idle;
            }
        }

        public override void NewData(byte[] data)
        {
            try
            {
                PluginActivity = PluginActivityTypes.Processing;
                trace.Info("+NewData()");
                // Get RemoteMessage operation name
                string operPrefix = Config.GetParameter("OperationPrefix");
                string msg = Convert.ToBase64String(data, Base64FormattingOptions.None);

                string sendEventsTo = Config.GetParameter("SendEventsTo");
                string[] targets = sendEventsTo.Split(';');

                foreach (string target in targets)
                {
                    try
                    {
                        trace.Info("Sending data using SendProcessMessage to " + target);
                        // operation=biomet-data; encodedData=34a48d84b9c7d9e0f7a80081
                        SendProcessMessage(target, "operation=" + operPrefix + msg);
                    }
                    catch (SystemException ex)
                    {
                        trace.Error("Error occured in SendProcessMessage(" + target + ", ...): " + ex.Message);
                    }
                }
                trace.Info("-NewData()");
            }
            finally
            {
                PluginActivity = PluginActivityTypes.Idle;
            }
        }

        private void RemoteMessageProcessor()
        {
            trace.Info("+RemoteMessageProcessor()");
            while (!stopRemoteMessage.WaitOne(0, false))
            {
                try
                {
                    StringBuilder sender = new StringBuilder(250);
                    int senderLen = sender.Capacity;
                    StringBuilder message = new StringBuilder(250);
                    int messageLen = message.Capacity;
                    PluginActivity = PluginActivityTypes.Idle;
                    if (GetProcessMessage(sender, ref senderLen, message, ref messageLen))
                    {
                        PluginActivity = PluginActivityTypes.Processing;
                        string strSrc = sender.ToString();
                        string strMsg = message.ToString();
                        trace.Info("GetProcessMessage - Received from '" + strSrc + "' message: " + strMsg);

                        if (strMsg == "operation=stop-plugin")
                        {
                            // we got a message to stop the plugin so exit the message processor
                            continue;
                        }
                        else if (strMsg.Contains(Config.GetParameter("OperationPrefix")))
                        {
                            trace.Info("Test sink - received biometric data in remote message.");
                        }
                    }
                }
                catch (SystemException se)
                {
                    trace.Error("SystemException was caught: " + se.Message);
                    stopRemoteMessage.WaitOne();
                    break;
                }
                catch (Exception e)
                {
                    trace.Error("General Exception occured: " + e.Message);
                    throw e;
                }
                finally
                {
                    PluginActivity = PluginActivityTypes.Idle;
                }
            }
            trace.Info("-RemoteMessageProcessor()");
        }
        /// <summary>
        /// RemoteMessage DLL handle used for preloading.
        /// </summary>
        static private IntPtr remoteMessageDllModule = IntPtr.Zero;

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
