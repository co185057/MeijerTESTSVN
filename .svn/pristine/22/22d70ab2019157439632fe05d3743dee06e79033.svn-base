using System;
//using System.Collections;
using System.Text;
using System.Threading;
using PsxNet;
using RPSWNET;
using System.Diagnostics;
using System.Reflection;


namespace RapNet
{
    public class ExtSharedPsx : SharedPsx
    {
        private const string MESG_SIGNED_ON = "Normal";

        private const string MESG_SIGNED_OFF = "Closed";


        public ExtSharedPsx(IPsx psx)
            : base(psx)
		{
            configFile = @"\RapNetExtSharedRAP.xml";
            mergeConfigFile = @"\RapNetExtSharedRAP.000";
        }

        /// <summary>
        /// Intializes instance
        /// </summary>
        public override bool Initialize()
        {
            CmDataCapture.Capture(CmDataCapture.MaskInfo, "ExtSharedPsx::Initialize()");
            if (base.Initialize())
            {
                ExtConfigData configData = ConfigData.GetConfigData() as ExtConfigData;

                // +RFQ 8503 Walmart SSCO Goal 
                if (!configData.SSCOGoalOn) 
                {
                    CmDataCapture.Capture(CmDataCapture.MaskInfo, "ExtSharedPsx::Initialize() => SSCOGoal is DISABLED");
                    psx.SetControlConfigProperty("TransactionGoal", Psx.PropertyVisible, false); 
                    psx.SetControlConfigProperty("TransactionStore", Psx.PropertyVisible, false); 
                    psx.SetControlConfigProperty("SSCOGoalLeadThru", Psx.PropertyVisible, false);
                    psx.SetControlConfigProperty("TransactionGoalText", Psx.PropertyVisible, false); 
                    psx.SetControlConfigProperty("TransactionStoreText", Psx.PropertyVisible, false); 
                } 
                else 
                {
                    CmDataCapture.Capture(CmDataCapture.MaskInfo, "ExtSharedPsx::Initialize() => SSCOGoal is ENABLED");
                    psx.SetControlConfigProperty("TransactionCount", Psx.PropertyVisible, false);
                    psx.SetControlConfigProperty("TransactionCountText", Psx.PropertyVisible, false); 

                    // Load extension class of the "TransactionCount" in case it is not already loaded
                    if (null == trxCount)
                    {
                        trxCount = ConfigData.LoadClass("TransactionCount", new object[] { this }) as TransactionCount;
                    }
                    ((ExtTransactionCount)trxCount).ShowSSCOGoalCount();
                }
                // -RFQ 8503 Walmart SSCO Goal   

                // Check DataPump Messagin being enabled
                if (configData.IsDPMesgEnabled)
                {
                    CmDataCapture.Capture(CmDataCapture.MaskInfo, "ExtSharedPsx::Initialize() => DataPump is ENABLED");
                    if (!InitDataPump())
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "ExtSharedPsx::Initialize() - Unable to initialize DataPump Client messaging");
                    }
                    else
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskInfo, String.Format("RAP DPC Heartbeat Freq: {0} secs", configData.DPHBeatFreq));
                    }
                }
                else
                {
                    CmDataCapture.Capture(CmDataCapture.MaskError, "ExtSharedPsx::Initialize() - DataPump Client is NOT configured to run.");
                }

                return true;
            }
            return false;
        }

        /// <summary>
        /// Initialize DataPump
        /// </summary>
        /// <returns></returns>
       
        private bool InitDataPump()
        {
            bool ret = false;
            try
            {
                ExtConfigData configData = ConfigData.GetConfigData() as ExtConfigData;

                if (0 == configData.DPServerIP.Length)
                {
                    CmDataCapture.Capture(CmDataCapture.MaskError, "ExtSharedPsx::InitDataPump() - DataPump Server IP ddress cannot be empty. Please check config file.");
                    return false;
                }

                Assembly asmbly = Assembly.LoadFile(ConfigData.GetConfigData().BinPath + @"\DPClientNet.dll");

                string typeName = "DataPumpClient.DPUdpClient";

                if (null != asmbly)
                {
                    dpClientType = asmbly.GetType(typeName);

                    if (null != dpClientType)
                    {
                        // Call the constructor of the class which takes (ipAddress , port)
                        ConstructorInfo ctor = dpClientType.GetConstructor(new Type[] { typeof(string), typeof(int) });

                        if (null != ctor)
                        {
                            object obj = ctor.Invoke(new object[] { configData.DPServerIP, configData.DPServerPort });

                            if (null != obj)
                            {
                                MethodInfo InitMethod = dpClientType.GetMethod("Init");

                                // Call Init() method of the object
                                if (null != InitMethod)
                                {
                                    object methodRet = InitMethod.Invoke(obj, null);

                                    ret = (bool)methodRet;
                                    if (ret)
                                    {
                                        dataPumpClient = obj;
                                        CmDataCapture.Capture(CmDataCapture.MaskInfo, "DataPumpClient is loaded sucessfully.");
                                    }
                                }
                                else
                                {
                                    CmDataCapture.Capture(CmDataCapture.MaskError, "ExtSharedPsx::InitDataPump() - Unable to find method 'Init()' in " + typeName);
                                }
                            }
                            else
                            {
                                CmDataCapture.Capture(CmDataCapture.MaskError, "ExtSharedPsx::InitDataPump() - Unable to create object : " + typeName);
                            }
                        }
                    }
                    else
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "ExtSharedPsx::InitDataPump() - Unable to find type: " + typeName);
                    }
                }
            }
            catch (Exception e)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "ExtSharedPsx::InitDataPump - EXCEPTION : " + e.Message);
            }

            return ret;
        }

        public void BroadcastMesg(short type, string mesg)
        {
            if (null == IsDPEnabled)
            {
                if (null != dataPumpClient)
                {
                    try
                    {
                        SendMesg = dataPumpClient.GetType().GetMethod("SendMesg");
                        if (null != SendMesg)
                        {
                            SendMesg.Invoke(dataPumpClient, new object[] { (short)type, mesg });
                            IsDPEnabled = true;
                        }
                        else
                        {
                            IsDPEnabled = false;
                            CmDataCapture.Capture(CmDataCapture.MaskError,
                                    "ExtSharedPsx::BroadcastMesg - Unable to find 'SendMesg()' in " + dataPumpClient.GetType().ToString());
                        }
                    }
                    catch (Exception e)
                    {
                        IsDPEnabled = false;
                        CmDataCapture.Capture(CmDataCapture.MaskError, "ExtSharedPsx::BroadcastMesg - EXCEPTION : " + e.Message);
                    }

                    if (!((bool)IsDPEnabled))
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "ERROR: Unable to broadcast messsage. !!! Please check the DataPumpClient binary.");
                    }
                }
            }
            else
            {
                if (((bool)IsDPEnabled))
                {
                    SendMesg.Invoke(dataPumpClient, new object[] { type, mesg });
                }
            }
        }

        protected override void ShowTime(object state)
        {
            base.ShowTime(state);

            ExtConfigData configData = ConfigData.GetConfigData() as ExtConfigData;

            if (configData.IsDPMesgEnabled)
            {
                if ((heartBeatCntr % configData.DPHBeatFreq) == 0)
                {
                    heartBeatCntr = 1;
                    this.BroadcastMesg((short)MsgTypes.TYPE_JSON,
                        String.Format("{{\"mode\":\"{0}\"}}", SignedOn ? MESG_SIGNED_ON : MESG_SIGNED_OFF));
                }
                else
                {
                    heartBeatCntr++;
                }
            }
        }

        public override bool SignedOn
        {
            set
            {
                if (value != base.SignedOn)
                {
                    base.SignedOn = value;
                    ExtConfigData configData = ConfigData.GetConfigData() as ExtConfigData;
                    if (configData.IsDPMesgEnabled)
                    {
                        BroadcastMesg((short)MsgTypes.TYPE_JSON, String.Format("{{\"mode\":\"{0}\"}}", value ? MESG_SIGNED_ON : MESG_SIGNED_OFF));
                    }
                }
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private int heartBeatCntr = 0;
        /// <summary>
        /// 
        /// </summary>
        private object IsDPEnabled = null;

        /// <summary>
        /// 
        /// </summary>
        private MethodInfo SendMesg = null;

        /// <summary>
        /// 
        /// </summary>
        private object dataPumpClient = null;

        /// <summary>
        /// 
        /// </summary>
        private Type dpClientType = null;
    }

    internal enum MsgTypes
    {
        TYPE_UDPServiceControl = (short)0,  // used by UDP Receiver plugin
        TYPE_InterventionStart,				// used by Interventions RadioCtrlr plugin
        TYPE_InterventionEnd,				// used by Interventions RadioCtrlr plugin
        TYPE_State,
        TYPE_EReceipt,
        TYPE_UPCandWeight,
        TYPE_HeartBeat,
        TYPE_ItemWeight,
        TYPE_Mode,
        TYPE_FTR,
        TYPE_JSON,
        Other
    };
}
