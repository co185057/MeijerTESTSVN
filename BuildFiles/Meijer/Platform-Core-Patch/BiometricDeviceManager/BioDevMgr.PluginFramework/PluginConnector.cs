using System;

namespace BioDevMgr
{
	/// <summary>
	/// Summary description for PluginConnector.
	/// </summary>
	public abstract class PluginConnector : Plugin
	{
		protected PluginConnector()
		{
			//
			// prevent instantiation 
			//
		}

		protected override void OnPluginEvent(PluginEventArgs e)
		{
			// Do any Connector specific processing here.

			// Call the base class event invocation method.
			base.OnPluginEvent(e);
		}

        public abstract void NewData(byte [] data);

	}
}
