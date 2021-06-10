using System;
using Microsoft.Win32;
using System.Xml;
using RPSWNET;
using System.Text.RegularExpressions;
using System.Collections;

namespace BioDevMgr
{
	/// <summary>
	/// Class that loads the application's configuration data and provides
	/// client friendly accessors.
	/// </summary>
	public class ConfigData
	{
		/// <summary>
		/// The one and only ConfigData instance.
		/// </summary>
		private static ConfigData configData = null;

		/// <summary>
		/// Biometric Device Manager registry key
		/// </summary>
		private const string regPath = @"SOFTWARE\NCR\BiometricDeviceManager";

		/// <summary>
		/// XML Configuration File key words
		/// </summary>
		private const string XML_PLUGINS = "/BiometricDeviceManager/Plugins/Plugin";
		private const string XML_PLUGINS_PATH = "/BiometricDeviceManager/Plugins/@Path";
		private const string XML_PLUGINS_CONNECTOR = "Connector";
		private const string XML_PLUGINS_DRIVER = "Driver";
        private const string XML_PLUGINS_CONTROLLER = "Controller";
		private const string XML_PLUGINS_START = "StartPlugin";
		private const string XML_PLUGINS_CLASS = "Class";


		/// <summary>
		/// Trace helper object
		/// </summary>
		private TraceHelper trace = new TraceHelper("ConfigData");

		/// <summary>
		/// Configuration file.
		/// </summary>
		private string configFile;

		/// <summary>
		/// Directory where all the Connector, Driver and Controller plugins are stored
		/// </summary>
		private string xmlPluginsPath;

		/// <summary>
		/// Ordered list of configured plugins and their parameters 
		/// </summary>
		private ConfigDataPlugin [] plugins = null; 

		protected ConfigData()
		{
			// protected to block object creation
		}

		public ConfigDataPlugin [] Plugins
		{
			get
			{
				return plugins;
			}
		}

		public string ConfigFile
		{
			get
			{
				return configFile;
			}
		}

		public string XmlPluginsPath
		{
			get
			{
				return xmlPluginsPath;
			}
		}

		public string RegPath
		{
			get 
			{
				return regPath;
			}
		}

		public static ConfigData GetConfigData()
		{
			if( configData == null )
			{
				configData = new ConfigData();
			}
			return configData;
		}


		public void Load()
		{
			try
			{
				trace.Info("+Load()");
				
				RegistryKey key = Registry.LocalMachine.OpenSubKey(RegPath);
				string path = key.GetValue("ConfigFile").ToString();
				key.Close();
				configFile = ExpandPartitionVars(path);
				trace.Info("Configuration file location: " + configFile);


				trace.Info("Loading XML Configuraton file.");
				XmlDocument doc = new XmlDocument();
				doc.Load(configFile);

				path = doc.SelectSingleNode(XML_PLUGINS_PATH).Value;
				xmlPluginsPath = ExpandPartitionVars(path);
				trace.Info("Plugin module location: " + xmlPluginsPath);
								
				// Load the plugin configuration in an ordered list
				XmlNodeList nodeList = doc.SelectNodes(XML_PLUGINS);
				if( nodeList.Count > 0 )
				{
					ArrayList list = new ArrayList();
					foreach(XmlNode node in nodeList)
					{
						try
						{
							//
							// Get the Plugin type information
							//
							ConfigDataPlugin plugin = new ConfigDataPlugin();
							XmlAttribute attr = node.Attributes[XML_PLUGINS_CONNECTOR];
							if( attr == null )
							{
								attr = node.Attributes[XML_PLUGINS_DRIVER];
								if( attr == null )
								{
                                    attr = node.Attributes[XML_PLUGINS_CONTROLLER];
                                    if (attr == null)
                                    {
                                        plugin.PluginType = ConfigDataPlugin.PluginTypes.Undefined;
                                        trace.Error("Configured plugin is not defined: " + node.ToString());
                                    }
                                    else
                                    {
                                        plugin.PluginType = ConfigDataPlugin.PluginTypes.Controller;
                                        plugin.ModuleName = attr.Value;
                                    }
								}
								else
								{
									plugin.PluginType = ConfigDataPlugin.PluginTypes.Driver;
									plugin.ModuleName = attr.Value;
								}
							}
							else
							{
								plugin.PluginType = ConfigDataPlugin.PluginTypes.Connector;
								plugin.ModuleName = attr.Value;
							}

							plugin.ClassName = node.Attributes[XML_PLUGINS_CLASS].Value;

							//
							// Is the plugin configured to be loaded or not? It loads by default.
							//
							attr = node.Attributes[XML_PLUGINS_START];
							if( (attr != null) && ( attr.Value == "0" || attr.Value.ToLower() == "n" ) )
							{
								plugin.StartPlugin = false;
							}
							else
							{
								plugin.StartPlugin = true;
							}

							//
							// Load any parameters specified - ignoring keywords
							//
							foreach( XmlAttribute a in node.Attributes )
							{
								if( a.Name != XML_PLUGINS_CONNECTOR &&
									a.Name != XML_PLUGINS_DRIVER &&
                                    a.Name != XML_PLUGINS_CONTROLLER &&
									a.Name != XML_PLUGINS_START &&
									a.Name != XML_PLUGINS_CLASS)
								{
									plugin.AddParameter(a.Name, a.Value);
								}
							}
							list.Add(plugin);

						}
						catch(Exception ex)
						{
							trace.Error("Error parsing configured plugin. " + ex.Message);
						}
					}
					if( list.Count > 0 )
					{
						plugins = new ConfigDataPlugin[list.Count];
						list.CopyTo(plugins);
					}
				}
				
				if( plugins == null )
				{
					trace.Warning("There are no valid plugin configuratons in the XML file.");
				}
			}
			catch( System.NullReferenceException nre )
			{
				trace.Error("Configuration file registry setting not found. " + nre.Message);
			}
			catch( System.IO.DirectoryNotFoundException dnfe )
			{
				trace.Error("Configuration file not does not exist. " + dnfe.Message);
			}
			catch( ArgumentNullException ane )
			{
				trace.Error("Argument exception error. " + ane.Message);
			}
			catch( UnauthorizedAccessException uae )
			{
				trace.Error("Configuration file path is invalid. " + uae.Message);
			}
			catch( XmlException xe )
			{
				trace.Error("Configuration file XML content is invalid. " + xe.Message);
			}
			finally
			{
				trace.Info("-Load()");
			}
		}

		/// <summary>
		/// Used to replace any partition environment variables in sArg with their corresponding values.
		/// The partition drive variables are currently:
		/// 
		/// %APP_DRIVE%
		/// %DATA_DRIVE%
		/// %OS_DRIVE%
		/// %BOOT_DRIVE%
		/// %TEMP_DRIVE%
		/// 
		/// If any of the above variables are found in the input string, they are replaced
		/// with the value (drive letter) associated with the environment variable.
		/// If a value is not defined for the partition environment variable(s), a default value
		/// of "C:" is used ("\" for WINCE).
		/// 
		/// For WINCE, no other variable substitution occurs.
		/// For non-WINCE, any other defined environment variables within sArg will also be 
		/// replaced with their respective values.
		/// </summary>
		/// <param name="sArg">The string to be searched.</param>
		/// <exception cref="ArgumentNullException"></exception>
		/// <returns>String containing the expanded environment variables.</returns>
		/// <remarks>jv185012</remarks>
		public static string ExpandPartitionVars(string sArg)
		{
			string sTmp = sArg;
#if( !WINCE )
			sTmp = Environment.ExpandEnvironmentVariables(sArg);
#endif
			string sRep = "%(APP|DATA|OS|BOOT|TEMP)_DRIVE%";  // Regex pattern.

			// If any %???_DRIVE% strings are still in sTmp, then the environment variables
			// for the drive partitions have not been set (or this is WINCE).
			// Replace the drive partition env. vars with "C:" (or "\" for WINCE).
#if( WINCE )
				sTmp = Regex.Replace(sTmp, sRep, "", RegexOptions.IgnoreCase);
#else
			sTmp = Regex.Replace(sTmp, sRep, "C:", RegexOptions.IgnoreCase);
#endif

			return sTmp;
		}
	}
}
