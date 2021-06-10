using Microsoft.Win32;
using RapNet;
using RPSWNET;
using System;
using System.Collections;
using System.Text;
using System.Xml;
using System.Security.Permissions;
using System.Text.RegularExpressions;
using System.Runtime.InteropServices;
using System.Diagnostics;
using Xunit;

namespace UnitTests
{
    public class ConfigDataTest
    {
        private ConfigData configDataChild;
        private RegistryKey RapNetKey = Registry.LocalMachine.OpenSubKey(@"SOFTWARE\NCR\RapNet", true);
        private String defaultApplicationSettingsFileValue = "";
        private const String APPLICATION_SETTINGS_FILE = "ApplicationSettingsFile";

        public ConfigDataTest()
        {
            configDataChild = ConfigData.GetConfigData();
        }

        ~ConfigDataTest()
        {
            configDataChild = null;
        }

        [Fact]
        public void TestLoad_RemoteRapNetXMLLocation()
        {
            if (RapNetKey != null)
            {
                object ApplicationSettingsKey = RapNetKey.GetValue(APPLICATION_SETTINGS_FILE);
                if (ApplicationSettingsKey != null)
                {
                    String remoteRapNetXMLLocation = ApplicationSettingsKey.ToString();
                    configDataChild.Load();
                    //ConfigData.ApplicationSettingsFile is the remote path of the RapNet.xml.
                    Assert.Equal(remoteRapNetXMLLocation, ConfigData.ApplicationSettingsFile);
                }
                else
                {
                    Console.WriteLine("\nApplicationSettingsFile does not exists! Please create it first under HKEY_LOCAL_MACHINE\\SOFTWARE\\NCR\\RapNet !");
                }
            }
            else
            {
                Console.WriteLine("\nRapNet key does not exists! Please create it first or install RapNet in your dev environment!");
            }
        }

        [Fact]
        public void TestLoad_RemoteRapNetXMLLanes()
        {
            if (RapNetKey != null)
            {
                object ApplicationSettingsKey = RapNetKey.GetValue(APPLICATION_SETTINGS_FILE);
                if (ApplicationSettingsKey != null)
                {
                    String remoteRapNetXMLLocation = ApplicationSettingsKey.ToString();
                    int numOfRemoteLanesConfigured = 5;

                    configDataChild.Load();

                    //ConfigData.LaneConfigList contains the SSCO lanes configured in the remote RapNet.xml.
                    //The remote RapNet.xml file must be configured with only 5 lanes for testing purposes only.
                    ArrayList laneConfigList = configDataChild.LaneConfigList;
                    Assert.Equal(numOfRemoteLanesConfigured, laneConfigList.Count);
                }
                else
                {
                    Console.WriteLine("ApplicationSettingsFile does not exists! Please create it first under HKEY_LOCAL_MACHINE\\SOFTWARE\\NCR\\RapNet !");
                }
            }
            else
            {
                Console.WriteLine("RapNet key does not exists! Please create it first or install RapNet in your dev environment!");
            }
        }

        [Fact]
        public void TestLoad_DefaultRapNetXML()
        {
            if (RapNetKey != null)
            {
                object ApplicationSettingsKey = RapNetKey.GetValue(APPLICATION_SETTINGS_FILE);
                if (ApplicationSettingsKey != null)
                {
                    String remoteRapNetXMLLocation = ApplicationSettingsKey.ToString();

                    //save the default value of the ApplicationSettingsFile registry key before overwriting the value of it.
                    defaultApplicationSettingsFileValue = remoteRapNetXMLLocation;

                    //set the value of the ApplicationSettingsFile to an inaccessible path so that the default RapNet.xml will be loaded.
                    RapNetKey.SetValue(APPLICATION_SETTINGS_FILE, "\\\\inaccessible\\remote\\path\\xxx.xml");

                    configDataChild.Load();

                    //ConfigData.LaneConfigList contains the SSCO lanes configured in the remote RapNet.xml.
                    ArrayList laneConfigList = configDataChild.LaneConfigList;
                    RapNetKey.SetValue(APPLICATION_SETTINGS_FILE, defaultApplicationSettingsFileValue);

                    //Default RapNet.xml contains 10 lanes.
                    Assert.Equal(10, laneConfigList.Count);
                }
                else
                {
                    Console.WriteLine("ApplicationSettingsFile does not exists! Please create it first under HKEY_LOCAL_MACHINE\\SOFTWARE\\NCR\\RapNet !");
                }
            }
            else
            {
                Console.WriteLine("RapNet key does not exists! Please create it first or install RapNet in your dev environment!");
            }
        }

    }
}
