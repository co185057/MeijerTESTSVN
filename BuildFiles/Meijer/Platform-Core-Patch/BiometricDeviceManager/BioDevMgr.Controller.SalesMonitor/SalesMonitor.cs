using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.IO;
using System.Diagnostics;

namespace BioDevMgr
{
    /// <summary>
    /// Plugin that monitors if the Sales Application
    /// is running and signals the BioDevMgr to toggle
    /// between the ACSIO and ACSBiomet driver.
    /// </summary>
    public class SalesMonitor : PluginController
    {
        private TraceHelper trace = new TraceHelper("SalesMonitor");

        /// <summary>
        /// Releases the resources used by the Plugin instance.
        /// </summary>
        public override void Dispose()
        {
            base.Dispose();
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

        public override void Start()
        {
            try
            {
                PluginActivity = PluginActivityTypes.Processing;
                // Start the monitoring thread
                trace.Info("Starting the MonitorSales thread");
                monitorSalesThread = new Thread(MonitorSalesProc);
                monitorSalesThread.Start();

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
                trace.Info("Signaling the MonitorSales thread to stop");
                exitMonitorSalesThreadEvent.Set();
                monitorSalesThread.Join();
                monitorSalesThread = null;
                base.Stop();
            }
            finally
            {
                PluginActivity = PluginActivityTypes.Idle;
            }
        }


        private void MonitorSalesProc()
        {

            trace.Info("Entering MonitorSalesProc() thread");

            string pluginWhenRunning = Config.GetParameter("PluginWhenRunning");
            string pluginWhenStopped = Config.GetParameter("PluginWhenStopped");
            string processToMonitor = Config.GetParameter("ProcessToMonitor");

            trace.Info("Monitoring process " + processToMonitor);

            if (Path.HasExtension(processToMonitor))
            {
                processToMonitor = Path.GetFileNameWithoutExtension(processToMonitor);
            }

            bool processRunning = false;
            bool bFirst = true;

            while (!exitMonitorSalesThreadEvent.WaitOne(0, false))
            {
                PluginActivity = PluginActivityTypes.Processing;
                Process[] processes = Process.GetProcessesByName(processToMonitor);

                if (processes.Length > 0 && !processRunning)
                {
                    // Monitored process has started, signal plugin to be stopped and plugin to be started
                    trace.Info("Process " + processToMonitor + " has started, sending request to stop plugin " + pluginWhenStopped);
                    OnPluginEvent(
                        new PluginEventArgs(
                                    PluginEventArgs.PluginEventTypes.OnControl,
                                    PluginEventArgs.PluginEventControlTypes.StopPlugin,
                                    pluginWhenStopped
                                    )
                                );

                    trace.Info("Process " + processToMonitor + " has started, sending request to start plugin " + pluginWhenRunning);
                    OnPluginEvent(
                        new PluginEventArgs(
                                    PluginEventArgs.PluginEventTypes.OnControl,
                                    PluginEventArgs.PluginEventControlTypes.StartPlugin,
                                    pluginWhenRunning
                                    )
                                );

                    processRunning = true;
                    bFirst = false;
                }
                else if (processes.Length == 0 && (processRunning || bFirst))
                {
                    // Monitored process has stopped, signal plugin to be stopped and plugin to be started
                    trace.Info("Process " + processToMonitor + " is stopped, sending request to stop plugin " + pluginWhenRunning);
                    OnPluginEvent(
                        new PluginEventArgs(
                                    PluginEventArgs.PluginEventTypes.OnControl,
                                    PluginEventArgs.PluginEventControlTypes.StopPlugin,
                                    pluginWhenRunning
                                    )
                                );

                    trace.Info("Process " + processToMonitor + " is stopped, sending request to start plugin " + pluginWhenStopped);
                    OnPluginEvent(
                        new PluginEventArgs(
                                    PluginEventArgs.PluginEventTypes.OnControl,
                                    PluginEventArgs.PluginEventControlTypes.StartPlugin,
                                    pluginWhenStopped
                                    )
                                );

                    processRunning = false;
                    bFirst = false;
                }
                PluginActivity = PluginActivityTypes.Idle;
                Thread.Sleep(Int32.Parse(Config.GetParameter("PollPeriod")));
            }
            trace.Info("Exiting MonitorSalesProc() thread");
        }

        Thread monitorSalesThread = null;
        EventWaitHandle exitMonitorSalesThreadEvent = new AutoResetEvent(false);
    }
}
