using System;
using System.Text;
using System.Globalization;
using PsxNet;
using RPSWNET;
using SSCOGoalClientNet;

namespace RapNet
{
    public class ExtTransactionCount : TransactionCount
    {
        public ExtTransactionCount(ExtSharedPsx paramPsx)
            : base(paramPsx)
        {
            myPsx = paramPsx;
        }

        //+RFQ 8503 Walmart SSCO Goals 
        public void ShowSSCOGoalCount()
        {
            string sscoGoalPrefixError = string.Empty;
            string sscoActualPrefixError = string.Empty;
            string walmartGoal = string.Empty;
            string walmartActual = string.Empty;

            ExtConfigData configData = ConfigData.GetConfigData() as ExtConfigData;

            dtRetrievalDate = DateTime.Now.ToString("yyyy-MM-dd", CultureInfo.InvariantCulture);
            CmDataCapture.Capture(CmDataCapture.MaskError, "SSCOGoalOn: " +
                                    configData.SSCOGoalOn + "dtRetrievalDate: " + dtRetrievalDate);

            // Create a copy of the SCO Object being called 
            SSCOGoal _sscoGoal = null;

            _sscoGoal = WalmartSSCOGoals.GetWalmartSSCOGoals(@"http://" +
                            configData.SSCOGoalURL, configData.SSCOGoalStoreNumber,
                            configData.SSCOGoalCountryCode, dtRetrievalDate);

            // Get the previous object with @ symbol if web service is unavailable. 
            if (_sscoGoal == null)
            {
                // call ftp module 
                CmDataCapture.Capture(CmDataCapture.MaskError, "+SSCOGoalOn: FTP Module is called since webservice is failing");
                SSCOGoal _sscoGoalFtp = WalmartSSCOGoals.GetWalmartSSCOGoalsFTP(configData.SSCOGoalStoreNumber,
                                                        configData.SSCOGoalCountryCode,
                                                        configData.ISPFTPServerIP);

                CmDataCapture.Capture(CmDataCapture.MaskError, "-SSCOGoalOn: FTP Module ");

                //if web service object doesn't contain a data 
                if (_sscoGoalFtp != null)
                {
                    _sscoGoal = _sscoGoalFtp;
                }
                else
                {
                    sscoActualPrefixError = "@";
                    sscoGoalPrefixError = "@";
                    _sscoGoal = sscoGoals;
                }
            }
            else
            {
                //since we don't ping to FTP anymore when we are connected to web service, make this variable to true 
                //so that once we are connected back to the webservice, @ sign will be remove 
                WalmartSSCOGoals.pingFTP = true;
            }

            //copy of the object created when RAPNet is running 
            sscoGoals = _sscoGoal;

            //bool  ping = WalmartSSCOGoals.PingHost("isp.s" + ConfigData.GetConfigData().SSCOGoalStoreNumber + "." + ConfigData.GetConfigData().SSCOGoalCountryCode + ".wal-mart.com"); 
            //check if web object is existing but content is null 
            if (_sscoGoal.Actual == string.Empty || _sscoGoal.Actual == null || !WalmartSSCOGoals.pingFTP)
                sscoActualPrefixError = "@";

            if (_sscoGoal.Goal == string.Empty || _sscoGoal.Goal == null || !WalmartSSCOGoals.pingFTP)
                sscoGoalPrefixError = "@";

            //show @ sign only if both actual SSCO Goal object and copy of object is empty 
            //during start up and service is not available 
            walmartGoal = sscoGoalPrefixError;
            walmartActual = sscoActualPrefixError;

            if (_sscoGoal != null)
            {
                walmartGoal += _sscoGoal.Goal;
                walmartActual += _sscoGoal.Actual;
            }

            if (myPsx != null)
            {
                myPsx.psx.SetControlConfigProperty("TransactionStore", Psx.PropertyTextFormat, walmartActual);
                myPsx.psx.SetControlConfigProperty("TransactionGoal", Psx.PropertyTextFormat, walmartGoal);
            }
        }
        //-RFQ 8503 Walmart SSCO Goals

        public override void RefreshTransactionCount(int day)
        {
            try
            {
                ExtConfigData configData = ConfigData.GetConfigData() as ExtConfigData;

                CheckClear(day);
                IncreaseTransactionCountCounter();
                int bRefreshToUpdateGoalCount = configData.SSCOGoalUpdateTimer * secondsToUpdateTrxCount; //multiply sscogoal timer to 60 second
                if (bRefreshToUpdateGoalCount == TransactionCountCounter)//Update the ssco goal display per minute //-RFQ 8503 Walmart SSCO Goals
                {
                    TransactionCountCounter = 0;
                    ShowSSCOGoalCount(); 
                    ShowTransactionCount();
                    int totalTransactionCount = 0;
                    totalTransactionCount = Convert.ToInt32(myPsx.psx.GetControlConfigProperty("TransactionCount", Psx.PropertyTextFormat));
                    TotalTransactionCountList.Add(totalTransactionCount - SaveTotalTransactionCount);
                    SaveTotalTransactionCount = totalTransactionCount;
                    if (TotalTransactionCountList.Count >= ConfigData.GetConfigData().TimePeriodForThroughputCalculation)
                    {
                        ShowTransactionCountColor();
                        TotalTransactionCountList.RemoveAt(0);
                    }
                    else if ((TotalTransactionCountByPeriod() >= ConfigData.GetConfigData().TrxCountThresholdForCaution) && !bReachGoodThreshold)
                    {
                        myPsx.psx.SetControlConfigProperty("TransactionCount", Psx.PropertyBackgroundColor,
                            myPsx.psx.GetConfigColor("NormalTrxCountThresholdColor"));
                        bReachGoodThreshold = true; //already change color
                    }
                }
            }
            catch (PsxException caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "TransactionCount::RefreshTransactionCount() - PsxException: " + caught.Message);
            }
            catch (Exception caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "TransactionCount::RefreshTransactionCount() - Exception: " + caught.Message);
                throw caught;
            }  
        }

        /// <summary>
        /// SharedPsx instance.
        /// </summary>
        private ExtSharedPsx myPsx = null;

        //+RFQ 8503 Walmart SSCO Goals 
        /// <summary> 
        /// separate the date from the actual time given by the system 
        /// </summary> 
        private string dtRetrievalDate;

        /// <summary> 
        /// Walmart SSCOGoal object 
        /// </summary> 
        private SSCOGoal sscoGoals = null;
        //-RFQ 8503 Walmart SSCO Goals
    }
}
