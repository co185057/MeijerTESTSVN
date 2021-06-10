using System;
using System.Drawing;
using System.Threading;
using System.Text;
using PsxNet;
using RPSWNET;
using CommonLibsNet;

namespace RapNet
{
	/// <summary>
    /// The Lane display class.
	/// </summary>
    public class LanePsx : IDisposable
    {
        /// <summary>
        /// The class constructor.
        /// </summary>
        /// <param name="hostName">
        /// The FastLane host name.
        /// </param>
        /// <param name="laneNumber">
        /// Index of this Psx lane.
        /// </param>
        /// <param name="owner">
        /// Owner of this Psx instance.
        /// </param>
        public LanePsx(string hostName, int laneNumber, SharedPsx owner)
        {
            try
            {
                this.psx = (IPsx)Activator.CreateInstance(owner.psx.GetType());
            }
            catch (Exception)
            {
                // this is used for unit tests //
                this.psx = owner.psx;
            }
            this.laneNumber = laneNumber;
            myHostName = hostName;
            myOwner = owner;            
        }
        
        /// <summary>
        /// The class destructor.
        /// </summary>
        ~LanePsx()
        {
            Dispose();
        }

        /// <summary>
        /// Intializes instance
        /// </summary>
        public virtual bool Initialize()
        {
            ConfigDataRapNet configData = ConfigDataRapNet.GetConfigData();
            try
            {
                psx.CreateChildDisplay(new Rectangle(0, 0, 0, 0), myOwner.psx);
                psx.Visible = false;
                psx.DisplayView = displayRapStandard;
                defaultZOrder = Convert.ToInt32(psx.GetControlProperty(Psx.ControlDisplay, Psx.PropertyZOrder));
                psx.LoadConfigFile(configData.ConfigPath + configFile, configData.ConfigPath + mergeConfigFile, configData.DataModel);
                ResetUi();

                if (configData.EnableRAPLite)
                {
                    Owner.UpdateInterventionsArray();

                    if (!ConfigData.IsMobileUI)
                    {
                        LaneIntervention = new Intervention(Intervention.Type.None, DateTime.Now);
                    }
                }

                psx.PsxEvent += new PsxEventHandler(LanePsx_PsxEvent);
                if (ConfigData.IsProxyServer)
                {
                    psx.SetThemesDir(configData.ConfigPath + "\\..\\themes");
                    psx.SetTheme("RapnetProxy");
                }
            }
            catch (PsxException caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "LanePsx::Initialize(" + LaneNumber.ToString() + ") - PsxException: " +
                    caught.Message);
                return false;
            }
            catch (Exception caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "LanePsx::Initialize(" + LaneNumber.ToString() + ") - Exception: " +
                    caught.Message);
                return false;
            }
            return true;
        }
        
        /// <summary>
        /// Gets or sets the local disable Context flag.
        /// </summary>
        public bool DisableContextLocal
        {
            get
            {
                return disableContextLocal;
            }
            set
            {
                if (disableContextLocal != value)
                {
                    disableContextLocal = value;
                    RefreshContext();
                }
            }
        }

        /// <summary>
        /// Gets or sets the remote disable Context flag.
        /// </summary>
        public bool DisableContextRemote
        {
            get
            {
                return disableContextRemote;
            }
            set
            {
                if (disableContextRemote != value)
                {
                    disableContextRemote = value;
					if (!ExclusiveMode)
					{
						Owner.PrepAndShowAlerts(); 
					}
					RefreshContext();
                }
            }
        }
                
        /// <summary>
        /// Releases the resources used by the LanePsx instance.
        /// </summary>
        public virtual void Dispose()
        {
            disposing = true;
            if (ConfigData.IsProxyServer)
            {
                psx.Visible = false;
            }
            psx.PsxEvent -= new PsxEventHandler(LanePsx_PsxEvent);
            if (null != playInterventionTimer)
            {
                playInterventionTimer.Dispose();
                playInterventionTimer = null;
            }
            if (null != playInterventionTimerSecondary)
            {
                playInterventionTimerSecondary.Dispose();
                playInterventionTimerSecondary = null;
            }
            if (null != connectThread)
            {
                connectThread.Dispose();
                connectThread = null;
            }
            psx.Dispose();
            GC.SuppressFinalize(this);
        }
		/// <summary>
		/// Gets or sets the lane in Assist mode.
		/// </summary>
		public virtual bool AssistMode
		{
			get
			{
				return assistMode;
			}
			set
			{
				if( value != assistMode )
				{
					assistMode = value;
				}
			}
		}
		/// <summary>
		/// Sets or gets the last valid transaction count of lane
		/// </summary>
		public int LastValidTransactionCount
		{
			get
			{
				return lastValidTransactionCount;
			}
			set
			{
				if( value != lastValidTransactionCount )
				{
					lastValidTransactionCount = value;
				}
			}
		}
        /// <summary>
        /// Gets or sets the lane in exclusive mode.
        /// </summary>
        public bool ExclusiveMode
        {
            get
            {
                return exclusiveMode;
            }
            set
            {
                if( value != exclusiveMode )
                {
                    exclusiveMode = value;
                    psx.DisplayView = exclusiveMode ? displayRapExclusive : displayRapStandard;
                    if( -1 != restoreLocation.X )
                    {
                        restoreLocation = new Point( -1, -1 );                        
                    }
                }
            }
        }

        /// <summary>
        /// Gets the full screen mode flag.
        /// </summary>
        public bool FullScreen
        {
            get
            {
                return ExclusiveMode && ( restoreLocation.X != -1 );
            }
        }

        /// <summary>
        /// Gets the remote host name.
        /// </summary>
        public string HostName
        {
            get
            {
                return myHostName;
            }
        }

        /// <summary>
        /// Is lane remotely connected.
        /// </summary>
        public bool IsConnected
        {
            get
            {
                return psx.RemoteConnections != null && psx.RemoteConnections.Length > 0;
            }
        }
        
        /// <summary>
        /// Gets or sets the current lane intervention.
        /// </summary>
        public Intervention LaneIntervention
        {
            get
            {
                return laneIntervention;
            }
            set
            {
                if (value != laneIntervention)
                {
                    if (null != playInterventionTimer)
                    {
                        playInterventionTimer.Dispose();
                        playInterventionTimer = null;
                    }
                    if (null != playInterventionTimerSecondary)
                    {
                        playInterventionTimerSecondary.Dispose();
                        playInterventionTimerSecondary = null;
                    }

                    ConfigDataRapNet configData = ConfigDataRapNet.GetConfigData();
                    switch (value.type)       
                    {         
                        case Intervention.Type.None:
                            if (laneIntervention.type == Intervention.Type.None)
                            {
                                laneIntervention = value;
                                return;
                            }    
                            break;
                        case Intervention.Type.Delayed:
                            if (configData.DelayedInterventionSound.Length > 0)
                            {
                                playInterventionTimer = new Timer(new TimerCallback(PlayInterventionSound), configData.DelayedInterventionSound,
                                    configData.InterventionSoundDelay, 0);
                            }
                            break;
                        case Intervention.Type.Immediate:
                            {
                                // First sound is InterventionSoundDelay ms after intervention displayed. 
                                // Second sound is InterventionSoundDelaySecondary ms after first sound (not after intervention)
                                int iDueTime = 0;
                                if (configData.ImmediateInterventionSound.Length > 0)
                                {
                                    iDueTime = configData.InterventionSoundDelay;
                                    playInterventionTimer = new Timer(new TimerCallback(PlayInterventionSound), configData.ImmediateInterventionSound, iDueTime, 0);
                                }
                                if (configData.ImmediateInterventionSoundSecondary.Length > 0)
                                {
                                    iDueTime += configData.InterventionSoundDelaySecondary;
                                    playInterventionTimerSecondary = new Timer(new TimerCallback(PlayInterventionSound), configData.ImmediateInterventionSoundSecondary, iDueTime, 0);
                                }
                            }
                            break;
                    }                                        
                    laneIntervention = value;
					Owner.PrepAndShowAlerts();
                }
            }
        }
        
        /// <summary>
        /// Gets the index of the Psx lane.
        /// </summary>
        public int LaneNumber
        {
            get
            {
                return laneNumber;
            }    
        }

        /// <summary>
        /// Gets the SharedPsx Owner.
        /// </summary>
        public SharedPsx Owner
        {
            get
            {
                return myOwner;
            }
        }
        
        /// <summary>
        /// Gets the remote connection name.
        /// </summary>
        public string RemoteConnection
        {
            get
            {
                string remoteConnection = "invalid";
                try
                {
                    string[] remoteConnections = psx.RemoteConnections;
                    if (remoteConnections.Length > 0)
                    {
                        remoteConnection = remoteConnections[0];
                    }                
                }
                catch (Exception)
                {
                }
                return remoteConnection;
            }
        }

        /// <summary>
        /// Occurs when Psx fires an event.
        /// </summary>
        /// <param name="sender">
        /// The source of the event.
        /// </param>		
        /// <param name="e">
        /// A PsxEventArgs that contains the event data.
        /// </param>
        public virtual void LanePsx_PsxEvent(object sender, PsxEventArgs e)
        {
            if (disposing)
            {
                return;
            }
            bool handled = false;
            try
            {
                if (e.EventName.Equals(Psx.EventChangeContext))
                {
                    if (ExclusiveMode)
                    {
                        if (psx.Bounds.Height > Owner.LaneViewPosition.Height)
                        {
                            if (!FullScreen)
                            {
                                restoreLocation = psx.Location;
                                Owner.psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, false);
                                psx.Location = new Point(0, 0);
                                Owner.RecalculateAlertLanes();
                                Owner.ShowLanes(false, LaneNumber);
                                Owner.SetAlertLanes(true);
                                Owner.psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, true);
                            }
                        }
                        else
                        {
                            if (FullScreen)
                            {
                                Owner.psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, false);
                                psx.Location = restoreLocation;
                                restoreLocation = new Point(-1, -1);
                                Owner.ShowLanes(true, 0);
                                Owner.SetAlertLanes(false);
                                Owner.psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, true);
                            }
                        }
                    }
                }
                else if (e.EventName.Equals(Psx.EventClick))
                {
                    CmDataCapture.Capture(CmDataCapture.MaskExtensive, String.Format("Lane Button Clicked => [{0}]", e.ControlName));

                    if (ConfigDataRapNet.GetConfigData().MultiExclusiveLanes && (e.ControlName.Equals("LaneButton") || 
                        e.ControlName.Equals(LanePsx.LANE_STATUS_BUTTON)) &&
                        e.ContextName.StartsWith("RAPSingle") && !ExclusiveMode && Owner.IsAnExclusiveLane(LaneNumber))
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskInfo, "LanePsx::LanePsx_PsxEvent Lane: " + LaneNumber.ToString() +
                            ", Lane Button click detected while Exclusive - trying to activate");
                        Owner.ExclusiveLane = LaneNumber;
                        handled = true;
                    }
                    if (e.ControlName.Equals("LaneButton") || 
                        e.ControlName.Equals(LanePsx.LANE_STATUS_BUTTON) ||
                        e.ControlName.Equals(SharedPsx.assistModeButton))
                    {
                        Owner.WaitForResponse(LaneNumber, e.ContextName.Equals("Alert"));
                    }
                    if (e.ContextName.Equals("Alert") && e.ControlName.Equals("LaneButton"))
                    {
                        try
                        {
                            LanePsx oldExclusiveLane = Owner.ExclusiveLanePsx;
                            Owner.ExclusiveLane = 0;
                            if (null != oldExclusiveLane)
                            {
                                oldExclusiveLane.psx.GenerateEvent(oldExclusiveLane.RemoteConnection, 
                                    String.Empty, String.Empty, ConfigData.eventRap, "operation=release");
                            }
                            if (Owner.AssistLane != 0)
                            {
                                LanePsx assistLane = Owner.LanePsxArray[Owner.AssistLane - 1];
                                if (null != assistLane)
                                {
                                    assistLane.psx.GenerateEvent(assistLane.RemoteConnection, String.Empty, 
                                        String.Empty, ConfigData.eventRap,
                                        "operation=toggle;destination=main;");
                                }
                            }
                        }
                        catch (PsxException caught)
                        {
                            CmDataCapture.Capture(CmDataCapture.MaskError, "LanePsx::LanePsx_PsxEvent(" + LaneNumber.ToString() +
                                ") - PsxException: " + caught.Message);
                        }
                        handled = true;
                    }
                    else if (AssistMode && e.ControlName.Equals(SharedPsx.assistModeButton))
                    {
                        psx.GenerateEvent(RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap,
                            "operation=toggle;destination=assistmode;");
                        handled = true;
                    }
                }
                //else if (e.EventName.Equals("ScanEvent")) // SSCOADK-1472 : From 5.1
                //{
                //    Owner.SharedPsx_PsxEvent(sender, e);
                //}
                else if (e.EventName.Equals(Psx.EventDisconnectRemote))
                {
                    CmDataCapture.Capture(CmDataCapture.MaskInfo, "LanePsx::LanePsx_PsxEvent(" + LaneNumber.ToString() + ") - DisconnectRemote event");
                    try
                    {
                        Owner.CancelWaitForResponse(LaneNumber);
                        if (Owner.ExclusiveLane == LaneNumber)
                        {
                            Owner.ExclusiveLane = 0;
                        }
                        else if (Owner.IsAnExclusiveLane(LaneNumber))
                        {
                            Owner.RemoveExclusive(LaneNumber);
                        }
                        if (Owner.AssistLane == LaneNumber)
                        {
                            Owner.AssistLane = 0;
                        }
                        // (+) POS181195/POS181197
                        if (Owner.HHSacquiredbyLane == true && Owner.HHSLane == LaneNumber)
                        {
                            Owner.HHSLane = 0;
                            Owner.HHSLaneName = "";
                            Owner.HHSacquiredbyLane = false;
                            Owner.Scanner.Disable();
                        }
                        // (-) POS181195/POS181197
                        ConfigData configData = ConfigDataRapNet.GetConfigData();
                        psx.LoadConfigFile(configData.ConfigPath + configFile, configData.ConfigPath + mergeConfigFile, configData.DataModel);
                        if (ConfigData.IsProxyServer)
                        {
                            // restore themes
                            psx.SetThemesDir(configData.ConfigPath + "\\..\\themes");
                            psx.SetTheme("RapnetProxy");
                        }
                        RefreshContext();

                        if (ConfigDataRapNet.GetConfigData().EnableRAPLite && !ConfigData.IsProxyServer)
                        {
                            psx.Visible = false;
                            LaneIntervention = new Intervention(Intervention.Type.None, DateTime.Now);
                        }

                        ResetUi();
                        if (ConfigDataRapNet.GetConfigData().EnableRAPLite)
                        {
                            Owner.UpdateInterventionsArray();

                            if (!ConfigData.IsMobileUI)
                            {
                                LaneIntervention = new Intervention(Intervention.Type.None, DateTime.Now);
                            }
                        }
                        Owner.TileLanes();
                        Owner.RefreshUi();
                    }
                    catch (PsxException caught)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "LanePsx::LanePsx(" + LaneNumber.ToString() + ") - PsxException: " +
                            caught.Message);
                    }
                    StartConnectionThread(Owner.LanePsxArray);
                }
                else if (e.EventName.Equals(ConfigData.eventRap))
                {
                    string operation = ConfigData.ExtractRapEventData(ConfigData.eventRapOperation, e.Param.ToString());
                    if (operation != null)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskInfo, "LanePsx::LanePsx_PsxEvent(" + LaneNumber.ToString() +
                            ") - RemoteData operation(" + operation + ")");
                        // (+) POS181195/POS181197
                        if (operation.StartsWith("request-scanner") || operation.StartsWith("release-scanner"))
                        {
                            string substr = operation;
                            substr = substr.Substring(0, 15);

                            if (substr.Equals("request-scanner"))
                            {
                                string requestingLaneHostName = operation.Remove(0, "request-scanner".Length + 1); //+1 is for elemenating the delimiter                                
                                CmDataCapture.Capture(CmDataCapture.MaskInfo, "Scanner acquire request from Lane :" + requestingLaneHostName);
                                requestingLaneHostName = "host=" + requestingLaneHostName;//Considering the case, where host name coming from the config data that is used to initialize LanePsx will be in the form of HOST=HOSTNAME                                
								LanePsx requestingLane = null;
								for (int i = 0; i <Owner.LanePsxArray.Length; i++)
								{
									if (Owner.LanePsxArray[i] == null)
									{
                                        continue;
									}
									else
									{
										if ( Owner.LanePsxArray[i].HostName.ToUpper().Equals(requestingLaneHostName.ToUpper()))
										{
											requestingLane = Owner.LanePsxArray[i];
											break;
										}
									}
								}
                                if (requestingLane != null)
                                {
                                    Owner.HHSacquiredbyLane = true;
                                    Owner.HHSLane = requestingLane.LaneNumber;
                                    Owner.HHSLaneName = requestingLaneHostName;
                                    requestingLane.psx.GenerateEvent(RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap, "operation=scanner-engaged");
                                    Owner.Scanner.Enable(string.Empty);
                                }
                                else
                                {
                                    CmDataCapture.Capture(CmDataCapture.MaskInfo, "Scanner acquire request from an unknown Lane :" + requestingLaneHostName);
                                }
                            }
                            else if (substr.Equals("release-scanner"))
                            {
                                Owner.HHSacquiredbyLane = false;
                                Owner.HHSLane = 0;
                                Owner.HHSLaneName = "";
                                if (Owner.ExclusiveLane != 0)
                                {
                                    Owner.Scanner.Enable(string.Empty);
                                }
                                else
                                {
                                    Owner.Scanner.Disable();
                                }
                            }
                        }
                        if (operation.StartsWith("scanner-status"))
                        {
                            string requestingLaneHostName = operation.Remove(0, "scanner-status".Length + 1); //+1 is for elemenating the delimiter                          
                            CmDataCapture.Capture(CmDataCapture.MaskInfo, "Request for scanner status from Lane :" + requestingLaneHostName);
                            requestingLaneHostName = "host=" + requestingLaneHostName;
                            LanePsx requestingLane = null;
                            for (int i = 0; i < Owner.LanePsxArray.Length; i++)
                            {
                                // (+) POS217027
                                if (Owner.LanePsxArray[i] == null)
                                {
                                    continue;
                                }
                                else
                                {
									if (Owner.LanePsxArray[i].HostName.ToUpper().Equals(requestingLaneHostName.ToUpper())) //POS181195.
                                    {
                                        requestingLane = Owner.LanePsxArray[i];
                                        break;
                                    }
                                    // (-) POS217027
                                }
                            }
                            if (requestingLane != null)                           
                            {
                                if (Owner.HHSacquiredbyLane) //POS181195 - Removed extra condition as it is disabling the button everytime.
                                {
                                    if (Owner.HHSLane != requestingLane.laneNumber || Owner.ExclusiveLane != LaneNumber) //POS181195 - changed condition to not to display unavailable message when AM accessed from RAP.
                                    {
                                        CmDataCapture.Capture(CmDataCapture.MaskInfo, "scanner-status event handler - scanner unavailable");
                                        requestingLane.psx.GenerateEvent(RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap, "operation=scanner-unavailable");
                                    }
                                    else
                                    {
                                        CmDataCapture.Capture(CmDataCapture.MaskInfo, "scanner-status event handler - scanner engaged");
                                        requestingLane.psx.GenerateEvent(RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap, "operation=scanner-engaged");
                                    }
                                }
                                else
                                {
                                    CmDataCapture.Capture(CmDataCapture.MaskInfo, "scanner-status event handler - scanner available");
                                    requestingLane.psx.GenerateEvent(RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap, "operation=scanner-available");
                                }
                            }
                            else
                            {
                                CmDataCapture.Capture(CmDataCapture.MaskInfo, "Scanner status request from an unknown Lane :" + requestingLaneHostName);
                            }
                        }
                        // (-) POS181195/POS181197
                        if (operation.Equals("acquire"))
                        {
                            Owner.CancelWaitForResponse(LaneNumber);
                            Owner.ExclusiveLane = LaneNumber;
                            handled = true;
                        }
                        else if (operation.Equals("enable-all-contexts"))
                        {
                            Owner.CancelWaitForResponse(LaneNumber);
                            DisableContextRemote = false;
                            handled = true;
                        }
                        else if (operation.Equals("disable-all-contexts"))
                        {
                            Owner.CancelWaitForResponse(LaneNumber);
                            DisableContextRemote = true;
                            handled = true;
                        }
                        else if (operation.Equals("intervention"))
                        {
                            Owner.CancelWaitForResponse(LaneNumber);
                            LaneIntervention = new Intervention(e.Param.ToString());
                            handled = true;
                        }
                        else if (operation.Equals("release"))
                        {
                            Owner.CancelWaitForResponse(LaneNumber);
                            if (Owner.ExclusiveLane == LaneNumber)
                            {
                                Owner.ExclusiveLane = 0;
                            }
                            else if (Owner.IsAnExclusiveLane(LaneNumber))
                            {
                                Owner.RemoveExclusive(LaneNumber);
                            }

                            if (ConfigDataRapNet.GetConfigData().EnableRAPLite && !ConfigData.IsProxyServer)
                            {
                                Owner.RefreshLaneButtons();
                            }

                            Owner.RefreshUi();
                            handled = true;
                        }
                        else if (operation.Equals("assist-acquire"))
                        {
                            if ((Owner.AssistLane == 0) || (Owner.AssistLane == LaneNumber))
                            {
                                Owner.AssistLane = LaneNumber;
                            }
                            else
                            {
                                CmDataCapture.Capture(CmDataCapture.MaskWarning, "LanePsx op = " + operation +
                                    ", More than one Lane trying to acquire assist mode. Cancelling operation for Lane:" + LaneNumber.ToString());
                                psx.GenerateEvent(RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap, "operation=assist-denied");
                            }
                            handled = true;
                        }
                        else if (operation.Equals("assist-release"))
                        {
                            Owner.CancelWaitForResponse(LaneNumber);
                            if (AssistMode)
                            {
                                Owner.AssistLane = 0;
                            }
                            handled = true;
                        }
                    }
                    if (!handled)
                    {
                        Owner.SharedPsx_PsxEvent(this, e);
                    }
                }
            }
            catch (Exception caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "LanePsx::LanePsx_PsxEvent(" + LaneNumber.ToString() + ") - Exception: " +
                    caught.Message);
                CmDataCapture.Capture(CmDataCapture.MaskError, "LanePsx::LanePsx_PsxEvent(" + LaneNumber.ToString() + ") - StackTrace: " +
                   caught.StackTrace);
            }
        }
                        
        /// <summary>
        /// Starts the Psx connection worker thread.
        /// </summary>
        /// <param name="paramPsx">
		/// Psx instances used to connect.
		/// </param>		
        static public void StartConnectionThread(LanePsx[] paramPsx)
        {
            try
            {
                if (connectThread == null)
                {
                    connectThread = ConfigDataRapNet.LoadClass("ConnectThread") as ConnectThread;
                }
                if (!connectThread.IsRunning())
                {
                    connectThread.LanePsxArray = paramPsx;
                    Thread t = new Thread(new ThreadStart(connectThread.ThreadMethod));
                    t.Start();                    
                }
            }
            catch (Exception caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "LanePsx::StartConnectionThread() - Exception: " + caught.Message);
            }            
        }


		/// <summary>
		/// Enable the lane status button
		/// </summary>
		public Psx.State LaneStatusState
		{
			get
            { 
                Psx.State state = (Psx.State)Convert.ToInt32(psx.GetControlConfigProperty(LanePsx.LANE_STATUS_BUTTON, Psx.PropertyState));
                return state;
            }
			set
            { 
                psx.SetControlConfigProperty(LanePsx.LANE_STATUS_BUTTON, Psx.PropertyState, value);
            }
		}
		
        /// <summary>
        /// Gets the RapSingle context name.
        /// </summary>
        public virtual string RapSingleContext
        {
            get
            {
                ConfigData configData = ConfigDataRapNet.GetConfigData();                
                string context;
                if (ConfigData.IsMobileUI && !ConfigData.IsProxyServer)
                {
                    int LaneDisplayCount = configData.EnableRAPLite ? configData.MaxNumberOfLanesShownForRAPLite : configData.LaneConfigList.Count;
                    CmDataCapture.Capture(CmDataCapture.MaskInfo, "MaxNumberOfLanesShownForRAPLite =" + configData.MaxNumberOfLanesShownForRAPLite);

                    if (LaneDisplayCount <= 5)
                    {
                        context = "RAPSingle5";
                    }
                    else if (LaneDisplayCount <= 8)
                    {
                        context = "RAPSingle8";
                    }
                    else if (LaneDisplayCount <= 10)
                    {
                        context = "RAPSingle10";
                    }
                    // For more than 10 lanes, application will use RAPSingle12,..14,..16,..18,..20
                    // Make sure context definitions for such configuration exists in the PSX XML.
                    else
                    {
                        if (LaneDisplayCount % 2 == 1)
                        {
                            context = "RAPSingle" + (LaneDisplayCount + 1);
                        }
                        else
                        {
                            context = "RAPSingle" + LaneDisplayCount;
                        }                 
                    }
                }
                else
                {
                    if (configData.LaneConfigList.Count <= 4)
                    {
                        context = "RAPSingle4";
                    }
                    else if (configData.LaneConfigList.Count <= 6)
                    {
                        context = "RAPSingle6";
                    }
                    else if (configData.LaneConfigList.Count <= 8)
                    {
                        context = "RAPSingle8";
                    }
                    else if (configData.LaneConfigList.Count <= 10)
                    {
                        context = "RAPSingle10";
                    }
                    // For more than 10 lanes, application will use RAPSingle12,..14,..16,..18,..20
                    // Make sure context definitions for such configuration exists in the PSX XML.
                    else 
                    {
                        if (configData.LaneConfigList.Count % 2 == 1)
                        {
                            context =  "RAPSingle" + (configData.LaneConfigList.Count + 1);
                        }
                        else
                        {
                            context = "RAPSingle" + configData.LaneConfigList.Count;
                        }
                    }
                    bool changeContext = false;
                    // Should this lane be shown smaller, since some other 
                    // lane is shown bigger 
                    if	(0 != Owner.ExclusiveLane && !ExclusiveMode && !Owner.SmallExclusive && Owner.ExclusiveLane != LaneNumber &&
                        (((Owner.ExclusiveLane - 1) % 2) == ((LaneNumber - 1) % 2)))
                    {
                        changeContext = true; 
                    }
                    if (changeContext) 
                    {
                        if (context.Equals("RAPSingle4"))
                        {
                            context = "RAPSingle4";
                        }
                        else if (context.Equals("RAPSingle8"))
                        {
                            context = "RAPSingle12";
                        }
                        else if (context.Equals("RAPSingle6"))
                        {
                            context = "RAPSingle8";
                        }
                        else if (context.Equals("RAPSingle10"))
                        {
                            context = "RAPSingle16";
                        }
                        else
                        {  
                            int n1 = Convert.ToInt32(context.Substring(9, 2), 10);
                            int n2 =  (n1 - 2) * 2;
                            context = "RAPSingle" + n2;
                        }
                    }
                }
				return context;
            }
        }
                
        /// <summary>
        /// Plays an intervention sound.
        /// </summary>
        private void PlayInterventionSound(object state)
        {
            Owner.PlaySound((string)state);
        }

        /// <summary>
        /// Refreshes a lane's context.
        /// </summary>
        public void RefreshContext()
        {
            try
            {
                if (Owner.RapState != null && Owner.RapState.ShowAlerts && psx.GetContext(displayRapStandard).Equals("Alert"))
                {
					return;
                }
                string context = !disableContextLocal && !disableContextRemote && IsConnected && Owner.SignedOn ? RapSingleContext :
                    RapSingleContext + "SignOff";
                psx.SetContext(context, displayRapStandard, false);                
            }
            catch (PsxException caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "LanePsx::RefreshContext(" + LaneNumber.ToString() + ") - PsxException: " +
                    caught.Message);
            }
			catch (Exception caught)
			{
				CmDataCapture.Capture(CmDataCapture.MaskError, "LanePsx::RefreshContext(" + LaneNumber.ToString() + ") - Exception: " +
                    caught.Message);
				throw caught;
			}
        }

        /// <summary>
        /// Sets the initial disconnected ui.
        /// </summary>
        public virtual void ResetUi()
        {
            try
            {				
                if (!ConfigDataRapNet.GetConfigData().EnableRAPLite)
                {

                    string offlineText = psx.GetString("OffLineLaneButtonText");
                    psx.SetTransactionVariable("LaneStateWindow1", offlineText);
                    psx.SetControlConfigProperty("LaneButtonText", Psx.PropertyTextFormat, offlineText); 
                    LaneIntervention = new Intervention(Intervention.Type.None, DateTime.Now);
                }
                disableContextLocal = false;
                disableContextRemote = false;
            }
            catch (PsxException caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "LanePsx::ResetUi(" + LaneNumber.ToString() + ") - PsxException: " + caught.Message);
            }
        }

        /// <summary>
        /// Sets our lane in alert mode.
        /// </summary>
        /// <param name="alert">
        /// Flag indicating if we are setting the lane in alert mode.
        /// </param>
        /// <param name="alertPosition">
        /// The alert lane position.
        /// </param>		
        public void SetAlert(bool alert, int alertPosition)
        {
            try
            {
                if (alert)
                {
                    if (alertPosition <= Owner.AlertLanes) 
                    {
                        if (!psx.GetContext(displayRapStandard).Equals("Alert"))
                        {
                            restoreLocation = psx.Location;
                            psx.SetContext("Alert", displayRapStandard, false);
                            psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyZOrder, defaultZOrder + 1);
                        }
                        string alertLocation = Owner.psx.GetContextCustomDataVariable("LocationAlert" + alertPosition.ToString(), "Startup");
                        alertLocation += ",0,0";
                        psx.Location = Psx.ObjectToRectangle(alertLocation).Location;
                        psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyUserInput, true);
                    }
                }
                else
                {
                    psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyUserInput, false);
                    if (-1 != restoreLocation.X)
                    {
                        psx.Location = restoreLocation;
                        restoreLocation = new Point(-1, -1);                        
                    }
                    psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyZOrder, defaultZOrder);
                    RefreshContext();
                }               
            }
            catch (PsxException caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "LanePsx::SetAlert(" + LaneNumber.ToString() + ") - PsxException: " + caught.Message);
            }
			catch (Exception caught)
			{
				CmDataCapture.Capture(CmDataCapture.MaskError, "LanePsx::SetAlert(" + LaneNumber.ToString() + ") - Exception: " + caught.Message);
				throw caught;
			}
        }

		/// <summary>
		/// Determines whether to show alerts in the current context
		/// </summary>
		public bool CanShowAlerts()
		{
			bool showAlerts = Owner.ShowAlertDefault;
			try
			{
				try
				{
					showAlerts = Convert.ToBoolean(psx.GetContextCustomDataVariable("ShowAlerts", psx.GetContext(displayRapExclusive)));
				}
				catch (FormatException)
				{                            
				}
			}
			catch (PsxException caught)
			{
				CmDataCapture.Capture(CmDataCapture.MaskError, "LanePsx::CanShowAlert(" + LaneNumber.ToString() + ") - PsxException: " +
                    caught.Message);
			}
			catch (Exception caught)
			{
				CmDataCapture.Capture(CmDataCapture.MaskError, "LanePsx::CanShowAlert(" + LaneNumber.ToString() + ") - Exception: " +
                    caught.Message);
				throw caught;
			}
			return showAlerts;
		}
        		
        /// <summary>
        /// Psx instance.
        /// </summary>                
        public IPsx psx = null;

        /// <summary>
        /// Set to true when Dispose has been called.
        /// </summary>
        protected bool disposing = false;
        
        /// <summary>
		/// Encapsulates our Psx connection worker thread.
		/// </summary>
		static private ConnectThread connectThread = null;

        /// <summary>
        /// Starting default Psx instance ZOrder.
        /// </summary>
        private int defaultZOrder = 0;
        
        /// <summary>
        /// Flag indicating that our Context has been disabled locally (by Rap).
        /// </summary>
        private bool disableContextLocal = false;

        /// <summary>
        /// Flag indicating that our Context has been disabled remotely (by FastLane).
        /// </summary>
        private bool disableContextRemote = false;
        
        /// <summary>
        /// Display used for Rap in exclusive mode.
        /// </summary>
        private const uint displayRapExclusive = Psx.DisplayAlternate1;

        /// <summary>
        /// Display used for Rap in non-exclusive mode.
        /// </summary>
        private const uint displayRapStandard = Psx.DisplayAlternate2;
                
        /// <summary>
		/// Flag indicating if we are in assist mode.
		/// Assist mode can only be active on one lane per RAP, and might be still
		/// active when exclusive mode has been released (during toggle). 
		/// Assist mode lane owns devices until exit.
		/// </summary>
		private bool assistMode = false;

		/// <summary>
		/// Last valid transaction count of lane
		///</summary>
		private int lastValidTransactionCount = 0;

		/// <summary>
        /// Flag indicating if we are in exclusive mode.
        /// </summary>
        
        private bool exclusiveMode = false;

        /// <summary>
        /// Indicates what the current lane intervention status is.
        /// </summary>
        private Intervention laneIntervention = new Intervention(Intervention.Type.None, DateTime.Now);
        
        /// <summary>
		/// Index of the Psx lane.
		/// </summary>
		private readonly int laneNumber = 0;
		
		/// <summary>
		/// Fastlane host name.
		/// </summary>
		private readonly string myHostName = String.Empty;        
        
        /// <summary>
		/// SharedPsx owner.
		/// </summary>
		private readonly SharedPsx myOwner;

        /// <summary>
        /// Timer that plays intervention sounds.
        /// </summary>
        private Timer playInterventionTimer = null;

        /// <summary>
        /// Timer that plays intervention second sounds.
        /// </summary>
        private Timer playInterventionTimerSecondary = null;

        /// <summary>
        /// Previous restore location for full screen mode. Value other than 0,0 means we are in full screen mode.
        /// </summary>
        private Point restoreLocation = new Point(-1, -1);

        /// <summary>
        /// The configuration file, which should be overriden by derived implementation (ScanPortal)
        /// </summary>
        protected string configFile = @"\Rap.xml";

        /// <summary>
        /// The merge gconfiguration file, which should be overriden by derived implementation (ScanPortal)
        /// </summary>
        protected string mergeConfigFile = @"\Rap.000";

#if (HHRAP)
        public const string LANE_STATUS_BUTTON = "HHLaneStatusButton";
#else
        public const string LANE_STATUS_BUTTON = "LaneStatusButton";
#endif
	}

    /// <summary>
    /// The Lane Intervention class.
    /// </summary>
    public struct Intervention : IComparable
    {
        /// <summary>
        /// The struct constructor.
        /// </summary>
        /// <param name="type">
        /// Intervention type.
        /// </param>        
        /// <param name="time">
        /// Intervention time.
        /// </param>
        public Intervention( Type type, DateTime time )
        {
            this.type = type;
            this.time = time;
        }

        /// <summary>
        /// The struct constructor.
        /// </summary>
        /// <param name="param">
        /// Intervention data in named-value pairs.
        /// </param>        
        public Intervention(string param)
        {
            type = Type.None;
            string str = ConfigData.ExtractRapEventData("intervention-type", param);
            if (str != null)
            {
                if (str.Equals("delayed"))
                {
                    this.type = Type.Delayed;
                }
                else if (str.Equals("immediate"))
                {
                    this.type = Type.Immediate;
                }
            }
            time = DateTime.Now;
            str = ConfigData.ExtractRapEventData("intervention-time", param);
            if (str != null)
            {
                time = ConfigData.ObjectToDateTime(str);
            }
        }
                   
        public int CompareTo( object obj ) 
        {
            if( obj is Intervention ) 
            {
                if( this.Equals( obj ) )
                {
                    return 0;
                }
                return this < (Intervention)obj ? 1 : -1;
            }        
            throw new ArgumentException();
        }        
        public override bool Equals( object obj )
        {
            return obj is Intervention && this == (Intervention)obj;            
        }
        public override int GetHashCode()
        {
            return type.GetHashCode() ^ time.GetHashCode();
        }
        public static bool operator ==( Intervention x, Intervention y ) 
        {
            return ( x.type == y.type ) && ( x.time == y.time );            
        }
        public static bool operator !=( Intervention x, Intervention y ) 
        {
            return !( x == y );
        }
        public static bool operator >( Intervention x, Intervention y ) 
        {
            return ( x.type > y.type ) || ( ( x.type == y.type ) && ( x.time > y.time ) );
        }
        public static bool operator >=( Intervention x, Intervention y ) 
        {
            return ( x > y ) || ( x == y );
        }
        public static bool operator <( Intervention x, Intervention y ) 
        {
            return !( x > y );
        }
        public static bool operator <=( Intervention x, Intervention y ) 
        {
            return ( x < y ) || ( x == y );
        }
        
        /// <summary>
        /// Intervention type enumeration.
        /// </summary>
        public enum Type { None = 0, Delayed, Immediate }
            
        public Type type;
        public DateTime time;
    }
}
