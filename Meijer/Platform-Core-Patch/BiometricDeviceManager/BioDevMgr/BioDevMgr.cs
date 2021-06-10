using System;
using System.Collections;
using System.Runtime.CompilerServices;
using System.Threading;

namespace BioDevMgr
{
	/// <summary>
	/// Summary description for BioDevMgr.
	/// </summary>
	public class BioDevMgr : IDisposable
	{
		private TraceHelper trace = new TraceHelper("BioDevMgr");

		private Plugin [] plugins = null;
        private SyncEvents syncEvents = new SyncEvents();
        private System.Collections.Generic.Queue<PluginEventArgs> pluginEventQueue;
        private Thread dataProcessingThread;
        BioDevMgrForm parent = null;

        public BioDevMgr(BioDevMgrForm parent)
		{
            this.parent = parent;
		}

		/// <summary>
		/// The class destructor.
		/// </summary>
		~BioDevMgr()
		{
			Dispose();
		}

		public void Dispose()
		{
			GC.SuppressFinalize( this );
		}

		public void Initialize()
		{
			trace.Info("+Initialize()");
			
			trace.Info("Load the Configuration data");
			ConfigData.GetConfigData().Load();

            trace.Info("Setting up the data processing thread");
            pluginEventQueue = new System.Collections.Generic.Queue<PluginEventArgs>();
            dataProcessingThread = new Thread(DataProcessingWorker);
            dataProcessingThread.Start();

			trace.Info("Initialize the plugins.");

			// keep counts
			int driverCount = 0;
			int connectorCount = 0;
			// Cycle through the configured plugins and load in order.
			ConfigDataPlugin [] configPlugins = ConfigData.GetConfigData().Plugins;

			if( configPlugins.Length > 0 )
			{
				ArrayList list = new ArrayList();
				foreach(ConfigDataPlugin configPlugin in configPlugins)
				{
					try
					{
						trace.Info("Loading " + configPlugin.PluginTypeName + " plugin " + configPlugin.ModuleName);

						Plugin plugin = Plugin.CreatePlugin(configPlugin, new PluginEventHandler(OnPluginEvent));

						if( configPlugin.PluginType == ConfigDataPlugin.PluginTypes.Connector )
						{
							connectorCount++;
						}
						else if( configPlugin.PluginType == ConfigDataPlugin.PluginTypes.Driver )
						{
							driverCount++;
						}
						list.Add(plugin);
                        UpdatePluginVisualStatus(plugin);
					}
					catch(System.IO.FileNotFoundException fnfe)
					{
						trace.Error("Plugin not found. " + fnfe.Message) ;
					}
					catch(Exception ex)
					{
						trace.Error("Plugin load failed. " + ex.Message);
					}
				}
				if( list.Count > 0 )
				{
					plugins = new Plugin[list.Count];
					list.CopyTo(plugins);
				}
			}

			if( driverCount == 0 )
			{
				trace.Warning("There are no Driver type plugins configured to source events.");
			}
			if( connectorCount == 0)
			{
				trace.Warning("There are no Connector type plugins configured to receive driver events.");
			}

            // Kickoff plugins configured to auto-start
            foreach (Plugin plugin in plugins)
            {
                if (plugin.Config.StartPlugin)
                {
                    trace.Info("Starting " + plugin.Config.PluginTypeName + " plugin " + plugin.Config.ModuleName);
                    StartPlugin(plugin);
                }
                else
                {
                    trace.Info("Plugin configured not to be started.");
                }
            }
		    trace.Info("-Initialize()");
		}


        public void Uninitialize()
        {
            trace.Info("+Uninitialize()");
            // Shutdown all the plugins
            foreach (Plugin plugin in plugins)
            {
                if (plugin.PluginStatus == Plugin.PluginStatusTypes.Started)
                {
                    trace.Info("Stopping and unloading plugin " + plugin.Config.ModuleName);
                    StopPlugin(plugin);
                    UpdatePluginVisualStatus(plugin);
                    UnloadPlugin(plugin);
                }
                else if( plugin.PluginStatus == Plugin.PluginStatusTypes.Stopped ||
                         plugin.PluginStatus == Plugin.PluginStatusTypes.Loaded )
                {
                    trace.Info("Unloading plugin " + plugin.Config.ModuleName);
                    UnloadPlugin(plugin);
                }
            }

            trace.Info("Signaling Data Processing Thread to terminate...");
            syncEvents.ExitThreadEvent.Set();
            dataProcessingThread.Join();

            plugins = null;

            trace.Info("-Uninitialize()");
        }

        private void DataProcessingWorker()
        {
            while (WaitHandle.WaitAny(syncEvents.EventArray) != 1)
            {
                lock (((ICollection)pluginEventQueue).SyncRoot)
                {
                    while (pluginEventQueue.Count > 0)
                    {
                        ProcessPluginEvent(pluginEventQueue.Dequeue());
                    }
                }
            }
            trace.Info("Exiting Data Processing Worker");
        }

        private void StartPlugin(Plugin plugin)
        {
            if (plugin != null)
            {
                if( plugin.PluginStatus == Plugin.PluginStatusTypes.Loaded || 
                    plugin.PluginStatus == Plugin.PluginStatusTypes.Stopped )
                {
                    trace.Info("Plugin " + plugin.GetType().ToString() + " current status is " + plugin.PluginStatusString +  ". Starting plugin.");  
                    plugin.Start();
                    UpdatePluginVisualStatus(plugin);
                }
                else
                {
                    trace.Warning("Plugin " + plugin.GetType().ToString() + " status is " + plugin.PluginStatusString + ". Ignoring start request.");
                }
            }
        }

        private void StopPlugin(Plugin plugin)
        {
            if (plugin != null)
            {
                if (plugin.PluginStatus == Plugin.PluginStatusTypes.Started)
                {
                    trace.Info("Plugin " + plugin.GetType().ToString() + " current status is " + plugin.PluginStatusString + ". Stopping plugin.");
                    plugin.Stop();
                    UpdatePluginVisualStatus(plugin);
                }
                else
                {
                    trace.Warning("Plugin " + plugin.GetType().ToString() + " status is " + plugin.PluginStatusString + ". Ignoring stop request.");
                }
            }
        }

        private void UnloadPlugin(Plugin plugin)
        {
            if (plugin != null)
            {
                if (plugin.PluginStatus == Plugin.PluginStatusTypes.Started)
                {
                    trace.Info("Plugin " + plugin.GetType().ToString() + " current status is " + plugin.PluginStatusString + ". Stopping and unloading plugin.");
                    StopPlugin(plugin);
                    UpdatePluginVisualStatus(plugin);
                    plugin.Unload();
                }
                else if( plugin.PluginStatus == Plugin.PluginStatusTypes.Stopped ||
                         plugin.PluginStatus == Plugin.PluginStatusTypes.Loaded ) 
                {
                    trace.Info("Plugin " + plugin.GetType().ToString() + " current status is " + plugin.PluginStatusString + ". Unloading plugin.");
                    plugin.Unload();
                }
                else
                {
                    trace.Warning("Plugin " + plugin.GetType().ToString() + " status is " + plugin.PluginStatusString + ". Ignoring stop request.");
                }
            }
        }

        private Plugin FindPlugin(string pluginClass)
        {
            if( plugins != null )
            {
                foreach (Plugin plugin in plugins)
                {
                    if (plugin.GetType().ToString() == pluginClass)
                    {
                        return plugin;
                    }
                }
            }
            return null;
        }

        /// <summary>
        /// Method that manages the dispatching of the data between drivers
        /// and connectors. This is the central activity of the BioDevMgr class
        /// </summary>
        /// <param name="e">
        /// The details of the data event in the generic PluginEventArgs format.
        /// </param>		
        private void ProcessPluginEvent(PluginEventArgs e)
        {
            // don't trace OnActivity events
            if (e.PluginEvent != PluginEventArgs.PluginEventTypes.OnActivity)
            {
                trace.Info("+ProcessPluginEvent()");
            }

            if (e.PluginEvent == PluginEventArgs.PluginEventTypes.OnData)
            {
                // fetch the data and pass it up to all the connectors.
                foreach (Plugin plugin in plugins)
                {
                    if (plugin is PluginConnector && plugin.PluginStatus == Plugin.PluginStatusTypes.Started)
                    {
                        if (e.Param1 is byte[])
                        {
                            (plugin as PluginConnector).NewData(e.Param1 as byte []);
                        }
                        else 
                        {
                            trace.Warning("Data type received is invalid. Discarding."); 
                        }
                    }
                }

            }
            else if (e.PluginEvent == PluginEventArgs.PluginEventTypes.OnControl)
            {
                PluginEventArgs.PluginEventControlTypes cmd = 
                            (PluginEventArgs.PluginEventControlTypes)e.Param1;
                string pluginClass = (string)e.Param2;
                Plugin plugin = null;

                switch (cmd)
                {
                    case PluginEventArgs.PluginEventControlTypes.StartPlugin:
                        plugin = FindPlugin(pluginClass);
                        if( plugin != null )
                        {
                            StartPlugin(plugin);
                            UpdatePluginVisualStatus(plugin);
                        }
                        else
                        {
                            trace.Warning("Plugin " + pluginClass + " request to Start failed.  The plugin does not exist.");
                        }
                        break;
                    case PluginEventArgs.PluginEventControlTypes.StopPlugin:
                        plugin = FindPlugin(pluginClass);
                        if( plugin != null )
                        {
                            StopPlugin(plugin);
                            UpdatePluginVisualStatus(plugin);
                        }
                        else
                        {
                            trace.Warning("Plugin " + pluginClass + " request to Stop failed.  The plugin does not exist.");
                        }
                        break;
                    case PluginEventArgs.PluginEventControlTypes.Shutdown:
                        if (parent != null)
                        {
                            trace.Info("Received BioDevMgr shutdown command. Closing parent application.");
                            parent.Close();
                        }
                        else
                        {
                            trace.Warning("There is no parent window assigned to this class. Uninitilizing device manager directly.");
                            Uninitialize();
                        }
                        break;
                };
            }
            else if (e.PluginEvent == PluginEventArgs.PluginEventTypes.OnActivity)
            {
                Plugin.PluginActivityTypes activity = (Plugin.PluginActivityTypes)e.Param1;
                string pluginClass = (string)e.Param2;
                UpdatePluginVisualStatus(FindPlugin(pluginClass));
            }
            else
            {
                trace.Warning("Plugin event passed for processing is not valid. Ignoring event.");
            }
            // don't trace OnActivity events
            if (e.PluginEvent != PluginEventArgs.PluginEventTypes.OnActivity)
            {
                trace.Info("-ProcessPluginEvent()");
            }
        }

        void UpdatePluginVisualStatus(Plugin plugin)
        {
            if (plugin != null)
            {
                parent.UpdatePluginRow(
                    plugin.Config.ClassName, 
                    plugin.Config.PluginTypeName, 
                    plugin.PluginStatusString, 
                    plugin.PluginActivityString);
            }
        }

		/// <summary>
		/// Plugin event handler
		/// </summary>
		/// <param name="sender">
		/// The source of the event.
		/// </param>		
		/// <param name="e">
		/// A PluginEventArgs that contains the event data.
		/// </param>
		private void OnPluginEvent(object sender, PluginEventArgs e)
		{
			Plugin plugin = sender as Plugin;

			string msg = "OnPluginEvent: ";
			switch(e.PluginEvent)
			{
				case PluginEventArgs.PluginEventTypes.OnStarted:
					trace.Info(msg + " " + sender.GetType().ToString() + " started.");
					break;
				case PluginEventArgs.PluginEventTypes.OnLoaded:
                    trace.Info(msg + " " + sender.GetType().ToString() + " loaded.");
					break;
                case PluginEventArgs.PluginEventTypes.OnUnloaded:
                    trace.Info(msg + " " + sender.GetType().ToString() + " unloaded.");
                    break;
				case PluginEventArgs.PluginEventTypes.OnStopped:
                    trace.Info(msg + " " + sender.GetType().ToString() + " stopped.");
					break;
                case PluginEventArgs.PluginEventTypes.OnData:
                    trace.Info(msg + " " + sender.GetType().ToString() + " data received.");
                    pluginEventQueue.Enqueue(e);
                    syncEvents.NewItemEvent.Set();
                    break;
				case PluginEventArgs.PluginEventTypes.OnError:
					trace.Info(msg + "error: " + (string)e.Param1);
					break;
                case PluginEventArgs.PluginEventTypes.OnControl:
                    trace.Info(msg + " " + sender.GetType().ToString() + " control command received.");
                    pluginEventQueue.Enqueue(e);
                    syncEvents.NewItemEvent.Set();
                    break;
                case PluginEventArgs.PluginEventTypes.OnActivity:
                    pluginEventQueue.Enqueue(e);
                    syncEvents.NewItemEvent.Set();
                    break;
			};
		}
	}
}
