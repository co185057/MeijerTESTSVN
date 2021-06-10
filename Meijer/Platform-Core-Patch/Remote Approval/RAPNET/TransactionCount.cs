using System;
using System.Collections;
using PsxNet;
using RPSWNET;

namespace RapNet
{
	/// <summary>
	/// A transaction count class
	/// </summary>
	public class TransactionCount
	{
        /// <summary>
        /// The class constructor.
        /// </summary>
		public TransactionCount(SharedPsx paramPsx)
		{
			myPsx = paramPsx;
            DateTime dt = DateTime.Now;
            StartDay = dt.Day;
		}
        /// <summary>
        /// The class destructor.
        /// </summary>
        ~TransactionCount()
        {
        }
        /// <summary>
        /// Clear the transaciton count data after midnight
        /// </summary>
        public void CheckClear(int day)
        {
            if (day != StartDay)
            {
                TransactionCountCounter = 0;
                TotalTransactionCountList.Clear();
                SaveTotalTransactionCount = 0;
				bReachGoodThreshold = false;
                for (int n = 0; n < myPsx.LanePsxArray.Length; n++)
                {
                    if (null !=myPsx.LanePsxArray[n])
                        myPsx.LanePsxArray[n].LastValidTransactionCount = 0;
                }
                StartDay = day;
            }
        }
        /// <summary>
        /// Update the total transaction count and background color
        /// </summary>
        public void RefreshTransactionCount(int day)
        {
            try
            {
                CheckClear(day);
                IncreaseTransactionCountCounter();
                if (secondsToUpdateTrxCount == TransactionCountCounter)//Update the trx count per minute
                {
                    TransactionCountCounter = 0;
                    ShowTransactionCount();
                    int totalTransactionCount = 0;
                    totalTransactionCount = Convert.ToInt32(myPsx.psx.GetControlConfigProperty("TransactionCount", Psx.PropertyTextFormat));
                    TotalTransactionCountList.Add(totalTransactionCount - SaveTotalTransactionCount);
                    SaveTotalTransactionCount = totalTransactionCount;
                    if (TotalTransactionCountList.Count >= ConfigDataRapNet.GetConfigData().TimePeriodForThroughputCalculation)
                    {
                        ShowTransactionCountColor();
                        TotalTransactionCountList.RemoveAt(0);	
                    }
                    else if ((TotalTransactionCountByPeriod() >= ConfigDataRapNet.GetConfigData().TrxCountThresholdForCaution) && !bReachGoodThreshold)
					{
						myPsx.psx.SetControlConfigProperty("TransactionCount", Psx.PropertyBackgroundColor,
                            myPsx.psx.GetConfigColor("NormalTrxCountThresholdColor"));
						bReachGoodThreshold = true; //already change color
					}
                }
            }
            catch( PsxException caught )
            {
                CmDataCapture.Capture( CmDataCapture.MaskError, "TransactionCount::RefreshTransactionCount() - PsxException: " + caught.Message );                
            }	
            catch( Exception caught )
            {
                CmDataCapture.Capture( CmDataCapture.MaskError, "TransactionCount::RefreshTransactionCount() - Exception: " + caught.Message );
                throw caught;
            }  
        }
        /// <summary>
        /// Update the total transaction count of the lane which currently connected
        /// </summary>
        public void ShowTransactionCount()
        {
			try
			{
				int totalTransactionCount = 0;
				bool connected = false;
				if( myPsx.LanePsxArray != null )
				{
					for( int n = 0; n < myPsx.LanePsxArray.Length; n++ )
					{
						if( null != myPsx.LanePsxArray[n])
						{
							connected = (null!= myPsx.LanePsxArray[n].psx.RemoteConnections && myPsx.LanePsxArray[n].psx.RemoteConnections.Length > 0);
							if (connected)
							{
								myPsx.LanePsxArray[n].LastValidTransactionCount = Convert.ToInt32(
                                    myPsx.LanePsxArray[n].psx.GetTransactionVariable(("TransactionCount")));
							}
							totalTransactionCount += myPsx.LanePsxArray[n].LastValidTransactionCount;
						}
					}
					myPsx.psx.SetControlConfigProperty("TransactionCount", Psx.PropertyTextFormat, Convert.ToString(totalTransactionCount));
				}
			}
            catch( PsxException caught )
            {
                CmDataCapture.Capture( CmDataCapture.MaskError, "TransactionCount::ShowTransactionCount() - PsxException: " + caught.Message );                
            }	
            catch( Exception caught )
            {
                CmDataCapture.Capture( CmDataCapture.MaskError, "TransactionCount::ShowTransactionCount() - Exception: " + caught.Message );
                throw caught;
            }

			
        }

        /// <summary>
        /// Update the background color of the total transaction count
        /// </summary>
        private void ShowTransactionCountColor()
        {
            try
            {
                int totalTransactionCountByPeriod = TotalTransactionCountByPeriod();

                if (ConfigDataRapNet.GetConfigData().TrxCountThresholdForHighAlert <= ConfigDataRapNet.GetConfigData().TrxCountThresholdForCaution)
                {
                    if (totalTransactionCountByPeriod <= ConfigDataRapNet.GetConfigData().TrxCountThresholdForHighAlert)
                    {
                        myPsx.psx.SetControlConfigProperty("TransactionCount", Psx.PropertyBackgroundColor,
                            myPsx.psx.GetConfigColor("HighAlertTrxCountThresholdColor"));
                    }
                    else if (totalTransactionCountByPeriod <= ConfigDataRapNet.GetConfigData().TrxCountThresholdForCaution)
                    {
                        myPsx.psx.SetControlConfigProperty("TransactionCount", Psx.PropertyBackgroundColor,
                            myPsx.psx.GetConfigColor("CautionTrxCountThresholdColor"));
                    }
                    else
                    {
                        myPsx.psx.SetControlConfigProperty("TransactionCount", Psx.PropertyBackgroundColor,
                            myPsx.psx.GetConfigColor("NormalTrxCountThresholdColor"));
                    }
                }
                else //another choice for the user to monitor if the transaction is too busy
                {
                    if (totalTransactionCountByPeriod >= ConfigDataRapNet.GetConfigData().TrxCountThresholdForHighAlert)
                    {
                        myPsx.psx.SetControlConfigProperty("TransactionCount", Psx.PropertyBackgroundColor,
                            myPsx.psx.GetConfigColor("HighAlertTrxCountThresholdColor"));
                    }
                    else if (totalTransactionCountByPeriod >= ConfigDataRapNet.GetConfigData().TrxCountThresholdForCaution)
                    {
                        myPsx.psx.SetControlConfigProperty("TransactionCount", Psx.PropertyBackgroundColor,
                            myPsx.psx.GetConfigColor("CautionTrxCountThresholdColor"));
                    }
                    else
                    {
                        myPsx.psx.SetControlConfigProperty("TransactionCount", Psx.PropertyBackgroundColor,
                            myPsx.psx.GetConfigColor("NormalTrxCountThresholdColor"));
                    }
                }
            }
            catch (PsxException caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "TransactionCount::ShowTransactionCountColor() - PsxException: " + caught.Message);                
            }	
            catch (Exception caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "TransactionCount::ShowTransactionCountColor() - Exception: " + caught.Message);
                throw caught;
            }
        }
		
		/// <summary>
		/// Total Transaction count by period
		/// </summary>
		/// From ShowTransactionCountColor()
		public int TotalTransactionCountByPeriod()
		{
			int totalTransactionCountByPeriod = 0;
            for (int n = 0; n < TotalTransactionCountList.Count; n++)
            {
                totalTransactionCountByPeriod += (int)TotalTransactionCountList[n];
            }
			return totalTransactionCountByPeriod;
		}
        /// <summary>
        /// Increase transactionCountCounter by 1
        /// </summary>
        public void IncreaseTransactionCountCounter()
        {
            TransactionCountCounter ++;
        }
        /// <summary>
        /// TransactionCountCounter property accessor.
        /// </summary>
        public int TransactionCountCounter
        {
            get
            {
                return transactionCountCounter;
            }
            set
            {
                transactionCountCounter = value;
            }
        }
        /// <summary>
        /// SaveTotalTransactionCount property accessor.
        /// </summary>
        public int SaveTotalTransactionCount
        {
            get
            {
                return saveTotalTransactionCount;
            }
            set
            {
                saveTotalTransactionCount = value;
            }
        }
        /// <summary>
        /// TotalTransactionCountList property accessor.
        /// </summary>
        public ArrayList TotalTransactionCountList
        {
            get
            {
                return totalTransactionCountList;
            }
            set
            {
                totalTransactionCountList = value;
            }
        }
        /// <summary>
        /// StartDay property accessor.
        /// </summary>
        public int StartDay
        {
            get
            {
                return startDay;
            }
            set
            {
                startDay = value;
            }
        }
        /// <summary>
        /// Count in seconds since the last updating the transaction count
        /// </summary>
        private int transactionCountCounter = 0;

        /// <summary>
        /// Save the previous total transaction count
        /// </summary>
        private int saveTotalTransactionCount = 0;

        /// <summary>
        /// Array of total transaction count per minute
        /// </summary>
        private ArrayList totalTransactionCountList = new ArrayList();

        /// <summary>
        /// system start day
        /// </summary>
        private int startDay = 0;

        /// <summary>
        /// SharedPsx instance.
        /// </summary>
        private SharedPsx myPsx = null;

        /// <summary>
        /// Seconds in a minute
        /// </summary>
        private const int secondsToUpdateTrxCount = 60;
		
		/// <summary>
		/// use to determine if color is already change 
		/// the first time the totalTransactionCount > TrxCountThresholdForCaution
		/// </summary>
		private bool bReachGoodThreshold = false; 

	}
}
