using System;
using System.Collections;

namespace BioDevMgr
{
	/// <summary>
	/// Configuration object pertaining to each plugin item entry
	/// in the configuration file to be parsed and possible loaded
	/// </summary>
	public class ConfigDataPlugin
	{
		/// <summary>
		/// Trace helper object
		/// </summary>
		private TraceHelper trace = new TraceHelper("ConfigDataPlugin");

		public ConfigDataPlugin()
		{
			
		}

		public enum PluginTypes
		{
			Connector,
			Driver,
            Controller,
			Undefined
		}

		private PluginTypes pluginType = PluginTypes.Undefined;
		private string moduleName = "";
		private string className = "";
		private bool startPlugin = false;
		private Hashtable Parameters = new Hashtable(); 

		protected string GetPluginTypeName(PluginTypes type)
		{
			switch(type)
			{
				case PluginTypes.Connector: return "Connector";
				case PluginTypes.Driver: return "Driver";
                case PluginTypes.Controller: return "Controller";
				case PluginTypes.Undefined: return "Undefined";
				default: return "UNKNOWN";
			};
		}


		public PluginTypes PluginType
		{
			get
			{
				return pluginType;
			}
			set
			{
				trace.Info("Setting plugin type to: " + GetPluginTypeName(value));
				pluginType = value;
			}
		}

		public string PluginTypeName
		{
			get
			{
				return GetPluginTypeName(pluginType);
			}
		}


		public string ModuleName
		{
			get
			{
				return moduleName;
			}
			set
			{
				trace.Info("Setting plugin module: " + value);
				moduleName = value;
			}
		}

		public string ClassName
		{
			get
			{
				return className;
			}
			set
			{
				trace.Info("Setting plugin class name: " + value);
				className = value;
			}
		}

		public bool StartPlugin
		{
			get
			{
				return startPlugin;
			}
			set
			{
				trace.Info("Setting start plugin value to " + value.ToString());
				startPlugin = value;
			}
		}

		public void AddParameter(string name, string val)
		{
			trace.Info("Adding plugin parameter: " + name + "=" + val);
			Parameters.Add(name, val);
		}

		public string GetParameter(string name)
		{
			object obj = Parameters[name];
			return obj == null ? "" : obj.ToString();
		}
	}
}
