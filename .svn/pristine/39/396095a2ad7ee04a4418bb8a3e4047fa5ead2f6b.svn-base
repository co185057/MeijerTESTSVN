using System;

namespace BioDevMgr
{
	/// <summary>
	/// Summary description for TestConnector.
	/// </summary>
	public class TestConnector : PluginConnector
	{
        private TraceHelper trace = new TraceHelper("TestConnector");

		public override void Start()
		{
			base.Start();
		}


		public override void Stop()
		{
			base.Stop();
		}

		protected override void Load()
		{
			base.Load();
		}

        public override void NewData(byte[] data)
        {
            trace.Info("Received new data for processing: " + Convert.ToBase64String(data, Base64FormattingOptions.None));
        }
    }
}
