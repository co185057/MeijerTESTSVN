using System;
using System.Reflection;

namespace BioDevMgr
{
	/// <summary>
	/// Generic plugin interface that Connectors, Drivers and Controllers will derive from.  
	/// This class implement the parameter passing and storage interfaces, initializers
	/// and other generic service applicable to both.
	/// </summary>
	public abstract class Plugin : IDisposable
	{
		protected ConfigDataPlugin configData = null;
		protected PluginEventHandler pluginEventHandler = null;

		protected Plugin()
		{
			
		}
		/// <summary>
		/// The class constructor.
		/// </summary>
		/// <param name="configData">
		/// The plugin configuration details and load parameters
		/// </param>		
		static public Plugin CreatePlugin(ConfigDataPlugin configData, PluginEventHandler eventHandler)
		{
			string pluginPath = System.IO.Path.Combine(
									ConfigData.GetConfigData().XmlPluginsPath,
									configData.ModuleName
									);

			Assembly assembly = Assembly.LoadFrom(pluginPath);
			Type t = assembly.GetType (configData.ClassName);
			Plugin plugin = (Plugin) Activator.CreateInstance(t);
			plugin.configData = configData;
			plugin.pluginEventHandler = eventHandler;
			plugin.PluginEvent += eventHandler;

			// hook event to biometric device manager
			plugin.Load();
			return plugin;
		}

		/// <summary>
		/// Load() method is called internally when object is first 
		/// created and configurationd details are loaded.
		/// </summary>
		protected virtual void Load()
		{
            pluginStatus = PluginStatusTypes.Loaded;
			OnPluginEvent(new PluginEventArgs(PluginEventArgs.PluginEventTypes.OnLoaded, null, null));
		}

		/// <summary>
		/// Starts the plugin.
		/// </summary>
		public virtual void Start()
		{
            pluginStatus = PluginStatusTypes.Started;
			OnPluginEvent(new PluginEventArgs(PluginEventArgs.PluginEventTypes.OnStarted, null, null));
		}

		/// <summary>
		/// Stops the plugin
		/// </summary>
		public virtual void Stop()
		{
            pluginStatus = PluginStatusTypes.Stopped;
			OnPluginEvent(new PluginEventArgs(PluginEventArgs.PluginEventTypes.OnStopped, null, null));
		}

        /// <summary>
        /// Load() method is called internally when object is first 
        /// created and configurationd details are loaded.
        /// </summary>
        public virtual void Unload()
        {
            pluginStatus = PluginStatusTypes.NotLoaded;
            OnPluginEvent(new PluginEventArgs(PluginEventArgs.PluginEventTypes.OnUnloaded, null, null));
        }

		
		protected virtual void OnPluginEvent(PluginEventArgs e)
		{
			// Make a temporary copy of the event to avoid possibility of
			// a race condition if the last subscriber unsubscribes
			// immediately after the null check and before the event is raised.
			PluginEventHandler handler = PluginEvent;
			if (handler != null)
			{
				handler(this, e);
			}
		}

		public enum PluginStatusTypes
		{
			NotLoaded,
			Loaded,
			Started,
			Stopped,
			Undefined
		}

		protected string GetPluginStatusString(PluginStatusTypes type)
		{
			switch(type)
			{
				case PluginStatusTypes.NotLoaded: return "NotLoaded";
				case PluginStatusTypes.Loaded: return "Loaded";
				case PluginStatusTypes.Started: return "Started";
				case PluginStatusTypes.Stopped: return "Stopped";
				case PluginStatusTypes.Undefined: return "Undefined";
				default: return "UNKNOWN";
			};
		}

		private PluginStatusTypes pluginStatus = PluginStatusTypes.NotLoaded;

		/// <summary>
		/// Current plugin status
		/// </summary>
		public PluginStatusTypes PluginStatus
		{
			get
			{
				return pluginStatus;
			}
		}

		/// <summary>
		/// Plugin status in string format
		/// </summary>
		public string PluginStatusString
		{
			get
			{
				return GetPluginStatusString(pluginStatus);
			}
		}

        public enum PluginActivityTypes
        {
            Idle,
            Processing
        }

        protected string GetPluginActivityString(PluginActivityTypes activity)
        {
            switch (activity)
            {
                case PluginActivityTypes.Idle: return "Idle";
                case PluginActivityTypes.Processing: return "Processing";
                default: return "UNKNOWN";
            };
        }

        private PluginActivityTypes pluginActivity = PluginActivityTypes.Idle;

        /// <summary>
        /// Current plugin activity, gets and sets it.
        /// </summary>
        public PluginActivityTypes PluginActivity
        {
            get
            {
                return pluginActivity;
            }
            set
            {
                pluginActivity = value;
                OnPluginEvent(new PluginEventArgs(PluginEventArgs.PluginEventTypes.OnActivity, pluginActivity, configData.ClassName));
            }
        }

        /// <summary>
        /// Plugin status in string format
        /// </summary>
        public string PluginActivityString
        {
            get
            {
                return GetPluginActivityString(pluginActivity);
            }
        }

        /// <summary>
        /// Get plugin's configuraton info
        /// </summary>
        public ConfigDataPlugin Config
        {
            get
            {
                return configData;
            }
        }

		/// <summary>
		/// The class destructor.
		/// </summary>
		~Plugin()
		{
			Dispose();
		}

		/// <summary>
		/// Releases the resources used by the Plugin instance.
		/// </summary>
		public virtual void Dispose()
		{
			PluginEvent -= pluginEventHandler;
			GC.SuppressFinalize( this );
		}

		/// <summary>
		/// Plugin event handler.
		/// </summary>

		public event PluginEventHandler PluginEvent;
	}
}
