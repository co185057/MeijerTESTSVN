using System;
using PsxNet;
using RPSWNET;
using CommonLibsNet;

namespace RapNet
{
	/// <summary>
	/// The monitoring state class.
	/// </summary>
	public class MonitoringState : RapNet.State
	{
		/// <summary>
		/// The class constructor.
		/// </summary>
		/// <param name="paramPsx">
		/// SharedPsx instance used to connect.
		/// </param>
        public MonitoringState(SharedPsx paramPsx, IPsx psx, bool bScanHndlr)
            : base(paramPsx, psx, bScanHndlr)
		{			
		}

        /// <summary>
        /// Enters the state.
        /// </summary>
        public override void Enter()
        {
            myPsx.TileLanes();
            base.Enter();            
        }

		/// <summary>
		/// Releases the resources used by the MonitoringState instance.
		/// </summary>
		public override void Dispose()
		{
			base.Dispose();
		}

        /// <summary>
        /// True if scanner is allowed.
        /// </summary>		        
        public override bool AllowScanner
        {
            get
            {
                bool allow = false;
                if (myPsx.CurrentExclusiveLane > 0 && !myPsx.SmallExclusive)
                {
                    LanePsx currentLane = myPsx.LanePsxArray[myPsx.CurrentExclusiveLane - 1];
                    try
                    {
                        allow = currentLane.AssistMode || !currentLane.psx.GetTransactionVariable(("DataNeededScannerInputEnabled")).Equals("0");
                    }
                    catch (PsxException)
                    {
                    }
                }
                return allow;
            }
        }

        /// <summary>
        /// True if alert lanes should be shown.
        /// </summary>		        
        public override bool ShowAlerts
        {
            get
            {
                return myPsx.ExclusiveFullScreen;
            }
        }
        
		/// <summary>
		/// State Psx event handler.
		/// </summary>
		/// <param name="sender">
		/// The source of the event.
		/// </param>		
		/// <param name="e">
		/// A PsxEventArgs that contains the event data.
		/// </param>
		/// <returns>
		/// Return True if we handle the event; otherwise, it should return False.
		/// </returns>
        public override bool State_PsxEvent(object sender, PsxEventArgs e)
		{
			bool handled = false;			
			if (e.EventName.Equals(Psx.EventClick))
			{
				if (e.ControlName.Equals("UtilityButton"))
				{
					if (Remote.IsLaunchPadRunning)
					{
						CmDataCapture.Capture(CmDataCapture.MaskInfo, "MonitoringState::State_PsxEvent() - Activate: ");
						Remote.SendMessage(Remote.LaunchpadApplication, "operation=launchpad-activate");						
					}
					else
					{
                        CmDataCapture.Capture(CmDataCapture.MaskInfo,
                            "MonitoringState::State_PsxEvent() - Got button click from the utility button. Remote.IsLaunchPadRunning is false. Exiting the app");
                        myPsx.Close();                       
					}
					handled = true;
				}
				else if (e.ControlName.Equals("SignOnButton"))
				{
					if (myPsx.SignedOn)
					{
						myPsx.LogToReportGenerator(false);
						myPsx.SignedOn = false;                        
						myPsx.SendOperatorToLanes("", "");
						myPsx.ExclusiveLane = 0;
					}
					else
					{
                        NavigateScreen(ConfigDataRapNet.GetConfigData().AlphaNumericKeyboard ? "EnterAlphaNumericID" : "EnterID");
					}
					handled = true;
				}
                //POS81634 - RAP Attendant tracking - rrs
                else if (e.ControlName.Equals("CheckInButton"))
                {
                    ConfigData configData = ConfigDataRapNet.GetConfigData();
                    myPsx.psx.SetContext(myPsx.GetExpectedEnterIDContext(), Psx.DisplayStandard, false);

                    myPsx.CheckingIn = true;

                    handled = true;
                }
                else if (e.ControlName.Equals("CheckOutButton"))
                {
                    ConfigData configData = ConfigDataRapNet.GetConfigData();
                    myPsx.psx.SetContext(myPsx.GetExpectedEnterIDContext(), Psx.DisplayStandard, false);

                    myPsx.CheckingOut = true;
                    handled = true;
                }
                //+POS92988
                else if (e.ControlName.Equals("ChangePwdButton"))
                {
                    myPsx.ChangePwd = true;
                    ConfigData configData = ConfigDataRapNet.GetConfigData();

                    //POS105314
                    if (configData.AllowEmplIdSignOn)
                    {
                        myPsx.psx.SetContext(myPsx.GetExpectedEnterIDContext(), Psx.DisplayStandard, false);
                    }
                    else
                    {
                        myPsx.psx.SetContext("ChangePWD", Psx.DisplayStandard, false);
                    }
                    //POS105314

                    //-POS92988
                    handled = true;
                }
                //e POS81634 - RAP Attendant tracking - rrs
			}
            base.State_PsxEvent(sender, e);
			return handled;
		}

		/// <summary>
		/// Occurs when Scanner fires an event.
		/// </summary>
		/// <param name="sender">
		/// The source of the event.
		/// </param>		
		/// <param name="e">
		/// A ScannerEventArgs that contains the event data.
		/// </param>
		public override void State_ScannerEvent(object sender, ScannerEventArgs e)
		{
            // +POS181195/POS181197
			if ((myPsx.SignedOn && 0 != myPsx.ExclusiveLane) || myPsx.HHSLane != 0)
			{
				Remote.SendMessage(Remote.LaunchpadApplication, "operation=reset-autosignoff");
				try
				{
                    if (myPsx.HHSLane != 0)
                    {
                        LanePsx ScannerLane = myPsx.LanePsxArray[myPsx.HHSLane - 1];
                        if (null != ScannerLane)
                        {
                            string param = "operation=scanner-data;scanner-data=" + e.ScanData + ";scanner-data-label=" + e.ScanDataLabel + ";scanner-type=" + e.ScanType.ToString();
                            ScannerLane.psx.GenerateEvent(ScannerLane.RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap, param);
                            CmDataCapture.Capture(CmDataCapture.MaskInfo, "MonitoringState::State_ScannerEvent(): called exclusiveLane.GenerateEvent() with param: '" + param + "'");
                        }
                    }
                    else
                    {
                        LanePsx exclusiveLane = myPsx.LanePsxArray[myPsx.ExclusiveLane - 1];
                        if (null != exclusiveLane)
                        {
                            string param = ScannerGeneric.ScanDataToOperation("scanner-data", e);
                            CmDataCapture.Capture(CmDataCapture.MaskInfo, "MonitoringState::State_ScannerEvent() - param: " + param);
                            exclusiveLane.psx.GenerateEvent(exclusiveLane.RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap, param);
                        }
                    }
                    // -POS181195/POS181197
				}
				catch (PsxException caught)
				{
					CmDataCapture.Capture(CmDataCapture.MaskError, "MonitoringState::State_ScannerEvent() - PsxException: " + caught.Message);
				}
				catch (Exception caught)
				{
					CmDataCapture.Capture(CmDataCapture.MaskError, "MonitoringState::State_ScannerEvent() - Exception: " + caught.Message);
					throw caught;
				}
			}
		}
	}
}
