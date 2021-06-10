using System;
using System.Threading;

namespace BioDevMgr
{
	/// <summary>
	/// Summary description for TestDriver.
	/// </summary>
	public class TestDriver : PluginDriver
	{
        private TraceHelper trace = new TraceHelper("TestDriver");
        TimerCallback timerDelegate;
        AutoResetEvent autoEvent = new AutoResetEvent(false);
        Timer stateTimer;

        /// <summary>
		/// Releases the resources used by the Plugin instance.
		/// </summary>
		public override void Dispose()
		{
			base.Dispose();
		}

        void TestDriverTimerCallback(Object state)
        {
            trace.Info("Timer callback triggered...");
            
            OnPluginEvent(
                new PluginEventArgs(
                        PluginEventArgs.PluginEventTypes.OnData, 
                        new byte[] { 0xba, 0xad, 0xf0, 0x0d}, 
                        new byte[] { 0xde, 0xad, 0xbe, 0xef }
                        )
                );
        }

		public override void Start()
		{
            try
            {
                long dueTime = Int32.Parse(configData.GetParameter("DueTime"));
                long period = Int32.Parse(configData.GetParameter("Period"));

                timerDelegate = new TimerCallback(this.TestDriverTimerCallback);
                stateTimer = new Timer(timerDelegate, autoEvent, dueTime, period);
            }
            catch (Exception ex)
            {
                trace.Error("Exception occured fetching timer parameters. " + ex.Message);
                // default timer in case of exception
                stateTimer = new Timer(timerDelegate, autoEvent, 1000, 2000);
            }
			base.Start();
		}


		public override void Stop()
		{
            stateTimer.Dispose();
			base.Stop();
		}

		protected override void Load()
		{
			base.Load();
		}
	}
}
