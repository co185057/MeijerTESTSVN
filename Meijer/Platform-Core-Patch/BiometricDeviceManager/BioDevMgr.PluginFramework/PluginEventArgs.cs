using System;

namespace BioDevMgr
{
	/// <summary>
	/// Class that contains the data for the Plugin event that occured. Derives from System.EventArgs.
	/// </summary>
	public class PluginEventArgs : EventArgs
	{
		public enum PluginEventTypes
		{
			OnStarted,
			OnStopped,
			OnLoaded,
            OnUnloaded,
			OnError,
			OnData,
            OnControl,
            OnActivity
		}

        public enum PluginEventControlTypes
        {
            StartPlugin,
            StopPlugin,
            Shutdown
        }

		public PluginEventArgs(PluginEventTypes pluginEvent, object param1, object param2)
		{
			this.pluginEvent = pluginEvent;
			this.param1 = param1;
			this.param2 = param2;
		}

		private readonly PluginEventTypes pluginEvent;
		private readonly object param1;
		private readonly object param2;

		public PluginEventTypes PluginEvent
		{
			get
			{
				return pluginEvent;
			}
		}
		public object Param1
		{     
			get 
			{ 
				return param1; 
			}      
		}
		public object Param2
		{     
			get 
			{ 
				return param2; 
			}      
		}
	}

	/// <summary>
	/// Plugin Event delegate.
	/// </summary>
	public delegate void PluginEventHandler( object sender, PluginEventArgs e );
}
