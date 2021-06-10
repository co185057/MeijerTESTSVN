using System;
using System.Drawing;
using System.Threading;
using System.Diagnostics;
using System.Collections;
using System.Text;
using System.Xml;
using PsxNet;
using RPSWNET;
using System.Linq;

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
                ConfigData configData = ConfigData.GetConfigData();

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
                        NavigateScreen(ConfigData.GetConfigData().AlphaNumericKeyboard ? "EnterAlphaNumericID" : "EnterID");
                    }
                    handled = true;
                }

                #region Meijer customization for RAP Attendant tracking - Begin
                //POS81634 - RAP Attendant tracking - rrs
                else if (e.ControlName.Equals("CheckInButton"))
                {
                    myPsx.psx.SetContext(myPsx.GetExpectedEnterIDContext(), Psx.DisplayStandard, false);
                    myPsx.CheckingIn = true;
                    handled = true;
                }
                else if (e.ControlName.Equals("CheckOutButton"))
                {
                    myPsx.psx.SetContext(myPsx.GetExpectedEnterIDContext(), Psx.DisplayStandard, false);
                    myPsx.CheckingOut = true;
                    handled = true;
                }
                //+POS92988
                else if (e.ControlName.Equals("ChangePwdButton"))
                {
                    myPsx.ChangePwd = true;

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
                #endregion Meijer customization for RAP Attendant tracking - End
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
            //(+) CRD124746
            CmDataCapture.Capture(CmDataCapture.MaskInfo, "MonitoringState::State_ScannerEvent(): e.scandata'" + e.ScanData + "'");
            CmDataCapture.Capture(CmDataCapture.MaskInfo, "MonitoringState::State_ScannerEvent(): e.ScanDataLabel'" + e.ScanDataLabel + "'");

            if (e.ScanDataLabel.StartsWith("TER", StringComparison.OrdinalIgnoreCase))
            {
                string terminalID = e.ScanDataLabel.Remove(0, 3);
                CmDataCapture.Capture(CmDataCapture.MaskInfo, "MonitoringState::State_ScannerEvent(): after removing TER with barcode'" + terminalID + "'");
                LanePsx lanePsx = myPsx.LanePsxArray.SingleOrDefault(lane => lane != null && terminalID.Equals(lane.HostName.Substring(lane.HostName.Length - 3), StringComparison.OrdinalIgnoreCase));
                LanePsx currentlanePsx = myPsx.LanePsxArray.SingleOrDefault(lane => lane != null && lane.HostName.Equals(myPsx.HandScannerAcquiredLane, StringComparison.OrdinalIgnoreCase)); // CRD196895
                try
                {
                    // +CRD196895
                    if (currentlanePsx != null)
                    {
                        currentlanePsx.psx.GenerateEvent(currentlanePsx.RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap, "operation=scanner-deactivated");
                    }// -CRD196895

                    if (lanePsx != null && 0 == myPsx.ExclusiveLane) // CRD197801
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskInfo, "MonitoringState::State_ScannerEvent(): sending scanner engaged to lane: " + lanePsx.HostName);
                        myPsx.AssignHandScannerToLane(lanePsx.HostName);
                        lanePsx.psx.GenerateEvent(lanePsx.RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap, "operation=scanner-engaged");
                    }
                    else  // +CRD197801
                    {
                        if (lanePsx.LaneNumber != myPsx.ExclusiveLane)
                        {
                            CmDataCapture.Capture(CmDataCapture.MaskError, "MonitoringState::State_ScannerEvent(): sending scanner busy to lane " + lanePsx.HostName);
                            lanePsx.psx.GenerateEvent(lanePsx.RemoteConnection, string.Empty, string.Empty, ConfigData.eventRap, "operation=scanner-unavailable");
                        }
                    }// -CRD197801
                }
                catch (Exception caught)
                {

                    CmDataCapture.Capture(CmDataCapture.MaskError, "MonitoringState::Monitoring HHScanner- Exception Error: " + caught.Message);
                }

                return;
            }


            if (myPsx.ExclusiveLane != 0)
            {
                myPsx.ReleaseScanner();
                CmDataCapture.Capture(CmDataCapture.MaskInfo, "MonitoringState::State_ScannerEvent(): release scanner");
            }
            else
            {

                CmDataCapture.Capture(CmDataCapture.MaskInfo, "MonitoringState::State_ScannerEvent(): attach scanner");
                myPsx.AttachScanner(sender, e);
            }
            //(-) CRD124746

            CmDataCapture.Capture(CmDataCapture.MaskInfo, "MonitoringState::State_ScannerEvent(): ExclusiveLane: " + myPsx.ExclusiveLane);
            if (myPsx.SignedOn && 0 != myPsx.ExclusiveLane)
            {
                //myPsx.ReleaseHandScanner(); //CRD124746
                Remote.SendMessage(Remote.LaunchpadApplication, "operation=reset-autosignoff");
                try
                {
                    string param = ScannerThread.ScanDataToOperation("scanner-data", e);
                    LanePsx exclusiveLane = myPsx.LanePsxArray[myPsx.ExclusiveLane - 1];
                    if (null != exclusiveLane)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskInfo, string.Format("MonitoringState::State_ScannerEvent() - Passing the scan event to Exclusive lane: {0}", exclusiveLane.LaneNumber) + param);
                        exclusiveLane.psx.GenerateEvent(exclusiveLane.RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap, param);
                    }
                    else
                    {
                        LanePsx assistLane = myPsx.LanePsxArray[myPsx.AssistLane - 1];
                        if (null != assistLane)
                        {
                            CmDataCapture.Capture(CmDataCapture.MaskInfo, string.Format("MonitoringState::State_ScannerEvent() - Passing the scan event to Assist lane: {0}", assistLane.LaneNumber) + param);
                            assistLane.psx.GenerateEvent(assistLane.RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap, param);
                        }
                    }
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
