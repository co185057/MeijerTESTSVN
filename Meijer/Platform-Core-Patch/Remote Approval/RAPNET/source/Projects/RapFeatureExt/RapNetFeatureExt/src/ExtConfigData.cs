using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using RapNet;

namespace RapNet
{
    public class ExtConfigData : ConfigData
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="myXmlDocument"></param>
        /// <param name="xmlOverrideDoc"></param>
        protected override void LoadXml(XmlDocument myXmlDocument, XmlDocument xmlOverrideDoc)
        {
            try
            {
                base.LoadXml(myXmlDocument, xmlOverrideDoc);

                XmlNodeList nodeList = null;

                // Load Datapump Configuration
                if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("EnableDataPumpMesg")).Count > 0) ||
                       (nodeList = myXmlDocument.GetElementsByTagName("EnableDataPumpMesg")).Count > 0)
                {
                    bEnableDataPumpMesg = Convert.ToBoolean(nodeList[0].InnerText);
                }

                if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("DataPumpServerIP")).Count > 0) ||
                       (nodeList = myXmlDocument.GetElementsByTagName("DataPumpServerIP")).Count > 0)
                {
                    if (nodeList[0].InnerText.Length > 7)
                    {
                        dpSrvrIP = nodeList[0].InnerText;
                    }
                }

                if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("DataPumpServerPort")).Count > 0) ||
                       (nodeList = myXmlDocument.GetElementsByTagName("DataPumpServerPort")).Count > 0)
                {
                    if (nodeList[0].InnerText.Length > 0)
                    {
                        dpSrvrPort = Convert.ToInt32(nodeList[0].InnerText);
                    }
                }

                if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("DataPumpHBeatFreq")).Count > 0) ||
                       (nodeList = myXmlDocument.GetElementsByTagName("DataPumpHBeatFreq")).Count > 0)
                {
                    if (nodeList[0].InnerText.Length > 0)
                    {
                        dpHBeatFreq = Convert.ToInt32(nodeList[0].InnerText);
                    }
                }

                // +RFQ 8503 SSCO Goals
 
                // Load SSCO Goals Configuration
                if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("SSCOGoalOn")).Count > 0) || 
                    (nodeList = myXmlDocument.GetElementsByTagName("SSCOGoalOn")).Count > 0) 
                { 
                    sscoGoalOn = Convert.ToBoolean(nodeList[0].InnerText); 
                } 
    
                if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("ISPFTPServerIP")).Count > 0) || 
                        (nodeList = myXmlDocument.GetElementsByTagName("ISPFTPServerIP")).Count > 0) 
                { 
                    ispFTPServerIP = Convert.ToString(nodeList[0].InnerText); 
                } 
        
                if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("SSCOGoalUpdateTimer")).Count > 0) || 
                        (nodeList = myXmlDocument.GetElementsByTagName("SSCOGoalUpdateTimer")).Count > 0) 
                { 
                    sscoGoalUpdateTimer = Convert.ToInt32(nodeList[0].InnerText); 
                } 
        
                if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("SSCOGoalCountryCode")).Count > 0) || 
                        (nodeList = myXmlDocument.GetElementsByTagName("SSCOGoalCountryCode")).Count > 0) 
                { 
                    sscoGoalCountryCode = Convert.ToString(nodeList[0].InnerText); 
                } 
        
                if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("SSCOGoalStoreNumber")).Count > 0) || 
                        (nodeList = myXmlDocument.GetElementsByTagName("SSCOGoalStoreNumber")).Count > 0) 
                { 
                    sscoGoalStoreNumber = Convert.ToString(nodeList[0].InnerText); 
                } 
        
                if ((null != xmlOverrideDoc && (nodeList = xmlOverrideDoc.GetElementsByTagName("SSCOGoalURL")).Count > 0) || 
                        (nodeList = myXmlDocument.GetElementsByTagName("SSCOGoalURL")).Count > 0) 
                { 
                    sscoGoalURL = Convert.ToString(nodeList[0].InnerText); 
                }
                // -RFQ 8503 SSCO Goals 
            }
            catch
            { }
        }

        /// <summary>
        /// Tied to <EnableDataPumpMesg></EnableDataPumpMesg>
        /// </summary>
        public bool IsDPMesgEnabled
        {
            get
            {
                return bEnableDataPumpMesg;
            }
        }

        /// <summary>
        /// Tied to <EnableDataPumpMesg></EnableDataPumpMesg>
        /// </summary>
        public string DPServerIP
        {
            get
            {
                return dpSrvrIP;
            }
        }

        /// <summary>
        /// Tied to <EnableDataPumpMesg></EnableDataPumpMesg>
        /// </summary>
        public int DPServerPort
        {
            get
            {
                return dpSrvrPort;
            }
        }

        // <summary>
        /// Tied to <DataPumpHBeatFreq></DataPumpHBeatFreq>
        /// </summary>
        public int DPHBeatFreq
        {
            get
            {
                return dpHBeatFreq;
            }
        }

        // +RFQ 8503 SSCO Goals 
        /// <summary> 
        /// Read-Only SSCOGoalOn property accessor. 
        /// </summary> 
        /// </summary> 
        public bool SSCOGoalOn 
        { 
            get 
            { 
                return sscoGoalOn; 
            } 
        } 
         
        /// <summary> 
        /// Read-Only SSCOGoalOn property accessor. 
        /// </summary> 
        public string ISPFTPServerIP 
        { 
            get 
            { 
                return ispFTPServerIP; 
            } 
        } 
                         
        /// <summary> 
        /// Read-Only SSCOGoalUpdateTimer property accessor. 
        /// </summary> 
        public int SSCOGoalUpdateTimer 
        { 
            get 
            { 
                return sscoGoalUpdateTimer; 
            } 
        } 
                         
        /// <summary> 
        /// Read-Only SSCOGoalCountryCode property accessor. 
        /// </summary> 
        public string SSCOGoalCountryCode 
        { 
            get 
            { 
                return sscoGoalCountryCode; 
            } 
        } 
                         
        /// <summary> 
        /// Read-Only SSCOGoalStoreNumber property accessor. 
        /// </summary> 
        public string SSCOGoalStoreNumber 
        { 
            get 
            { 
                return sscoGoalStoreNumber; 
            } 
        } 
                         
        /// <summary> 
        /// Read-Only SSCOGoalCountryCode property accessor. 
        /// </summary> 
        public string SSCOGoalURL 
        { 
            get 
            { 
                return sscoGoalURL; 
            } 
        } 
        // -RFQ 8503 SSCO Goals 
         

        /// <summary>
        /// 
        /// </summary>
        private int dpHBeatFreq = 60; // Default set to 60 secs

        /// <summary>
        /// DataPump Server IP
        /// </summary>
        private string dpSrvrIP = String.Empty;

        /// <summary>
        /// DataPump Server Port
        /// </summary>
        private int dpSrvrPort = 6982;

        /// <summary>
        /// Enable/Disable Data Pump Messaging.
        /// </summary>
        private bool bEnableDataPumpMesg = false;

        ///////////////////////////////////////
        // +RFQ 8503 SSCO Goals

        /// <summary> 
        /// Flag for turning on/off the SSCO Goal feature. 
        /// </summary> 
        private bool sscoGoalOn = false; 
  
        /// <summary> 
        /// Flag for turning on/off the SSCO Goal FTP feature. 
        /// </summary> 
        private string ispFTPServerIP = String.Empty; 
  
        /// <summary> 
        /// Time in seconds in order to refresh the current count and goal count. 
        /// </summary> 
        private int sscoGoalUpdateTimer = 15;                            
  
        /// <summary> 
        /// Country code to use to query the SSCO Goal. 
        /// </summary> 
        private string sscoGoalCountryCode = "US";       
  
        /// <summary> 
        /// Store number to use. 
        /// </summary> 
        private string sscoGoalStoreNumber = String.Empty;       
  
        /// <summary> 
        /// Url of the webservice to get the data from 
        /// </summary> 
        private string sscoGoalURL = String.Empty; 
        
        // -RFQ 8503 SSCO Goals 
        ////////////////////////////////////////////////

    }
}
