using System;
using System.Drawing;
using System.Threading;
using System.Collections;
using System.Xml;
using PsxNet;
using RPSWNET;
using System.Diagnostics;
using System.IO;
using CommonLibsNet;

namespace RapNet
{
	/// <summary>
	/// The shared display class.
	/// </summary>
	public class SharedPsx : IDisposable
	{
		/// <summary>
		/// The class constructor.
		/// </summary>
		public SharedPsx(IPsx psx)
		{
            this.psx = psx;            
		}
        
		/// <summary>
		/// The class destructor.
		/// </summary>
		~SharedPsx()
		{
			Dispose();
		}

        /// <summary>
        /// Closes the display.
        /// </summary>
        public void Close()
        {
            if (null != Closed)
            {
                Closed(this, new EventArgs());
            }
        }

        /// <summary>
        /// Handles message from interprocess communication.
        /// </summary>
        /// <param name="strSrc">
        /// The source process name.
        /// </param>   
        /// <param name="strMsg">
        /// The message.
        /// </param>   
        public virtual void HandleProcessMessage(string strSrc, string strMsg)
        {
            if (strSrc.ToUpper() == Remote.LaunchpadApplication.ToUpper())
            {
                string strOp = ConfigData.ExtractRapEventData(ConfigData.eventRapOperation, strMsg);
                if (strOp == null)
                {
                    CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::HandleProcessMessage() - An operation was not included in the message. Discarding...");
                }
                else if (strOp.Equals("set-signed-off") || strOp.Equals("auto-signoff"))
                {
                    if (strOp.Equals("auto-signoff"))
                    { 
                        WasAutoSignedOff = true;
                    }
                    WasSyncSignOnOff = true;
                    SignedOn = false;
                }
                else if (strOp.Equals("set-signed-on"))
                {
                    string strId = ConfigData.ExtractRapEventData("operator", strMsg);
                    string strRights = ConfigData.ExtractRapEventData("rights", strMsg);
                    if (strId == null || strRights == null)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError,
                            "SharedPsx::HandleProcessMessage() - OperatorID && Rights were not included in the message. Discarding...");
                    }
                    else
                    {
                        WasSyncSignOnOff = true;
                        OperatorId = strId;
                        OperatorRights = Convert.ToUInt32(strRights);
                        SignedOn = true;
                    }
                }
                else if (strOp.Equals("stop-app"))
                {
                    CmDataCapture.Capture(CmDataCapture.MaskWarning, "SharedPsx::HandleProcessMessage() - Received Stop command via RemoteMessage");
                    Close();
                }
                else if (strOp == ScannerGeneric.RemoteMessages.ReleaseScanner)
                {
                }
                else
                {
                    CmDataCapture.Capture(CmDataCapture.MaskWarning,
                        "SharedPsx::HandleProcessMessage() - received an unknown operation from '" + strSrc + "'. Discarding...");
                }
            }
            else
            {
                CmDataCapture.Capture(CmDataCapture.MaskWarning,
                    "SharedPsx::HandleProcessMessage() - received a message from an unexpected source: '" + strSrc + "'. Discarding...");
            }
        }

        /// <summary>
        /// Intializes instance
        /// </summary>
        public virtual bool Initialize()
        {
            ConfigDataRapNet configData = ConfigDataRapNet.GetConfigData();
            try
            {
                psx.SetEnvironmentVariable2("AudioPath", configData.AudioPath);
                psx.SetEnvironmentVariable2("ConfigPath", configData.ConfigPath);
                psx.SetEnvironmentVariable2("MediaPath", configData.MediaPath);
                psx.CreateDisplay(new Rectangle(0, 0, 0, 0));
                psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyCursor, configData.ShowCursor);
                psx.Text = "RAP.NET";
                psx.LoadConfigFile(configData.ConfigPath + configFile, configData.ConfigPath + mergeConfigFile);
                ConfigData.IsMobileUI = (psx.Bounds.Width < 800);
                ConfigData.IsProxyServer = psx.GetControlProperty("UtilityButton", Psx.PropertyPosition).Equals("0,0,0,0");
                CmDataCapture.Capture(CmDataCapture.MaskInfo, "ConfigData.IsMobileUI = " + ConfigData.IsMobileUI + "; ConfigData.IsProxyServer = " + ConfigData.IsProxyServer);
                if (ConfigData.IsProxyServer)
                {
                    psx.SetThemesDir(configData.ConfigPath + "\\..\\themes");
                    psx.SetTheme("RapnetProxy");
                }
#if (WINCE)
				psx.TopMost = true;
#endif
                psx.SetControlConfigProperty("UtilityButton", Psx.PropertyTextFormat, Remote.IsLaunchPadRunning ? "$UtilityButtonText$" :
                    "$ExitText$");
                OperatorId = String.Empty;
                if (!configData.DisplayTransactionCount)
                {
                    psx.SetControlConfigProperty("TransactionCountText", Psx.PropertyVisible, false);
                    psx.SetControlConfigProperty("TransactionCount", Psx.PropertyVisible, false);
                    psx.SetControlConfigProperty("DateText", Psx.PropertyVisible, ConfigData.IsMobileUI);
                    psx.SetControlConfigProperty("TimeText", Psx.PropertyVisible, ConfigData.IsMobileUI);
                }
                else
                {
                    trxCount = ConfigDataRapNet.LoadClass("TransactionCount", new object[] { this }) as TransactionCount;
                }
                if (!LoadLanePsxArray())
                {
                    return false;
                }
                dynamicAlertLanes = GetDynamicAlertLanes;
                showAlertDefault = GetShowAlertDefault;
                laneViewPosition = Psx.ObjectToRectangle(psx.GetContextCustomDataVariable("LaneViewPosition", "Startup"));
                // DM180015: Psx has a bug where it doesnt always fire initial ChangeContext event //                            
                SharedPsx_PsxEvent(psx, new PsxEventArgs("", Psx.ControlDisplay, "Startup", Psx.EventChangeContext, ""));
                if (!StartScannerThread())
                {
                    CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::Initialize() - StartScannerThread failed");
                }
                if (!StartBiometric())
                {
                    CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::Initialize() - Biometric not configured.");
                }
                TileLanes();
                RefreshUi();
                psx.PsxEvent += new PsxEventHandler(SharedPsx_PsxEvent);
                psx.SetControlConfigProperty("UtilityButton", Psx.PropertyState, Psx.State.Normal);
                psx.SetControlConfigProperty("SignOnButton", Psx.PropertyFlash, true);

                //POS81634 - RAP Attendant tracking - rrs
                psx.SetControlConfigProperty("CheckInButton", Psx.PropertyFlash, false);
                psx.SetControlConfigProperty("CheckOutButton", Psx.PropertyFlash, false);
                //e POS81634 - RAP Attendant tracking - rrs
            }
            catch (PsxException caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::Initialize() - PsxException: " + caught.Message);
                return false;
            }
            catch (Exception caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::Initialize() - Exception: " + caught.Message);
                CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::Initialize() - Exception1: " + caught.StackTrace);
                return false;
            }
            showTimeTimer = new Timer(new TimerCallback(ShowTime), null, 0, 1000);
            LanePsx.StartConnectionThread(lanePsxArray);
            return true;
        }

        /// <summary>
        /// Logs system memory use.
        /// </summary>
        /// <param name="mask">
        /// The data capture mask.
        /// </param>   
        static public void LogMemoryUse(uint mask)
        {
            PInvoke.MEMORYSTATUS stat = new PInvoke.MEMORYSTATUS();
            PInvoke.GlobalMemoryStatus(out stat);
            CmDataCapture.Capture(mask, stat.MemoryLoad.ToString() + " percent of memory is in use.");
            uint mem = stat.TotalPhysical.ToUInt32() / 1024;
            CmDataCapture.Capture(mask, "There are " + mem + " total KB of physical memory.");
            mem = stat.AvailablePhysical.ToUInt32() / 1024;
            CmDataCapture.Capture(mask, "There are " + mem + " free KB of physical memory.");
            mem = stat.AvailableVirtual.ToUInt32() / 1024;
            CmDataCapture.Capture(mask, "There are " + mem + " free KB of virtual memory.");
            if (stat.MemoryLoad > 95)
            {
                CmDataCapture.Capture(CmDataCapture.MaskWarning, "Low memory warning! " + stat.MemoryLoad.ToString() +
                    " percent of memory is in use.");
#if (WINCE)
                // Try to get 5% of total memory back //
                PInvoke.SHCloseApps((uint)(stat.TotalPhysical.ToUInt32() * 0.05));
#endif
            }
            if (mem < 2048)
            {
                CmDataCapture.Capture(CmDataCapture.MaskWarning, "Low virtual memory warning! There are " + mem + " free KB of virtual memory.");
            }
        }

        /// <summary>
        /// Plays a sound.
        /// </summary>
        /// <param name="sound">
        /// The sound name.
        /// </param>		
        public void PlaySound(string sound)
        {
            if (sound.Length > 0)
            {
                try
                {
                    psx.PlaySound(ConfigDataRapNet.GetConfigData().AudioPath + "\\" + sound);
                }
                catch (PsxException caught)
                {
                    CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::PlaySound() - PsxException: " + caught.Message);
                }
            }
        }
                
        /// <summary>
		/// Occurs when the Display is closed.
		/// </summary>
        public event EventHandler Closed;

		/// <summary>
		/// Gets the number of alert lanes.
		/// </summary>
		public int AlertLanes
		{
			get
			{
				if( -1 == alertLanes )
				{
					alertLanes = 0;
					try
					{
						alertLanes = Convert.ToInt32(psx.GetContextCustomDataVariable("AlertLanes", "Startup"));
					}
					catch( FormatException )
					{                            
					}
					catch( PsxException caught )
					{
						CmDataCapture.Capture( CmDataCapture.MaskError, "SharedPsx::AlertLanes() - PsxException: " + caught.Message );
					}                                        
				}                
				return alertLanes;
			}            
		}

        /// <summary>
        /// The last exclusive lane in our array 
        /// </summary>
        public int CurrentExclusiveLane
        {
            get
            {
                if (ConfigDataRapNet.GetConfigData().MultiExclusiveLanes)
                {
                    lock (exclusiveLaneArray.SyncRoot)
                    {
                        if (exclusiveLaneArray.Count > 0)
                        {
                            return (int)exclusiveLaneArray[exclusiveLaneArray.Count - 1];
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    return exclusiveLaneCurrentValue;
                }
            }
            set
            {
                if (!ConfigDataRapNet.GetConfigData().MultiExclusiveLanes)
                {
                    exclusiveLaneCurrentValue = value;
                }
                else
                {
                    PushExclusiveLaneArray(value);
                }
            }
        }
        
        public bool WasAutoSignedOff
        {
            set
            {
                wasAutoSignedOff = value;
            }
            get
            {
                return wasAutoSignedOff;
            }
        }

		/// <summary>
		/// Cancels a lane's wait for response.
		/// </summary>
		/// <param name="laneNumber">
		/// The lane number that is cancelling the wait. A value of 0 cancels any lane's wait for response.
		/// </param>        
		public void CancelWaitForResponse(int laneNumber)
		{
			if (0 == laneNumber || waitForResponseLane == laneNumber)
			{
				lock (lanePsxArray.SyncRoot)
				{
					if (null != waitForResponseTimer)
					{
						waitForResponseLane = 0;                    
						waitForResponseTimer.Dispose();
						waitForResponseTimer = null;
                        psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyUserInput, true);
						if (waitForResponseFreezeRedraw)
						{
							psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, true);
							waitForResponseFreezeRedraw = false;
						}
					}
				}
			}
		}

        /// <summary>
        /// Releases the resources used by the SharedPsx instance.
        /// </summary>
        public virtual void Dispose()
        {
            disposing = true;
            psx.PsxEvent -= new PsxEventHandler(SharedPsx_PsxEvent);
            if (null != scanner)
            {
                scanner.Dispose();
                scanner = null;
            }
            if (biometric != null)
            {
                if (biometric.BiometricDeviceConfigured)
                {
                    Biometric.BiometricEvent -= new BiometricDelegate(Biometric_BiometricEvent);
                    biometric.Dispose();
                    biometric = null;
                }
            }
            DisposeLanePsxArray();
#if (WINCE)
			psx.TopMost = false;
#endif
            psx.Dispose();
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Sets or gets the assist lane number.
        /// </summary>
        public virtual int AssistLane
        {
            get
            {
                return assistLane;
            }
            set
            {
                if (assistLane != value)
                {
                    try
                    {
                        int oldAssistLane = assistLane;
                        if (value > 0)
                        {
                            if (0 != oldAssistLane)
                            {
                                CmDataCapture.Capture(CmDataCapture.MaskWarning,
                                    "SharedPsx::AssistLane() - More than one Lane trying to acquire assist mode. Cancelling operation for Lane:" + value.ToString());
                                LanePsx lane = LanePsxArray[value - 1];
                                if (null != lane)
                                {
                                    lane.psx.GenerateEvent(lane.RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap,
                                        "operation=assist-denied");
                                    if (!ConfigData.IsMobileUI)
                                    {
                                        lane.psx.SetControlConfigProperty(assistModeButton, Psx.PropertyFlash, false);
                                    }
                                }
                                return;
                            }
                        }

                        assistLane = value;
                        CmDataCapture.Capture(CmDataCapture.MaskInfo, "SharedPsx::AssistLane() - AssistLane changing to Lane:" +
                            assistLane.ToString());
                        psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, false);
                        if (assistLane > 0)
                        {
                            LanePsx lane = LanePsxArray[assistLane - 1];
                            lane.AssistMode = true;
                            if (null != lane)
                            {
                                lane.psx.GenerateEvent(lane.RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap,
                                    "operation=assist-granted");
                                if (!ConfigData.IsMobileUI)
                                {
                                    lane.psx.SetControlConfigProperty(assistModeButton, Psx.PropertyFlash, true);
                                }
                            }
                        }
                        else if (oldAssistLane > 0)
                        {
                            LanePsx lane = lanePsxArray[oldAssistLane - 1];
                            lane.AssistMode = false;
                            if (!ConfigData.IsMobileUI)
                            {
                                lane.psx.SetControlConfigProperty(assistModeButton, Psx.PropertyFlash, false);
                            }
                        }
                        if (!ConfigData.IsMobileUI)
                        {
                            for (int n = 0; n < lanePsxArray.Length; n++)
                            {
                                if (null != lanePsxArray[n])
                                {
                                    // disabling assist //
                                    if (assistLane > 0 && assistLane != (n + 1))
                                    {
                                        lanePsxArray[n].psx.SetControlConfigProperty(assistModeBlockButton, Psx.PropertyVisible, true);
                                        if (!ConfigDataRapNet.GetConfigData().MultiExclusiveLanes)
                                        {
                                            lanePsxArray[n].DisableContextLocal = true;
                                        }
                                    }
                                    // reenabling assist //
                                    else if (0 == assistLane && oldAssistLane != (n + 1))
                                    {
                                        lanePsxArray[n].psx.SetControlConfigProperty(assistModeBlockButton, Psx.PropertyVisible, false);
                                        if (ConfigDataRapNet.GetConfigData().MultiExclusiveLanes)
                                        {
                                            lanePsxArray[n].psx.SetControlConfigProperty(assistModeButton, Psx.PropertyVisible, true);
                                        }
                                        lanePsxArray[n].DisableContextLocal = false;
                                    }
                                }
                            }
                        }
                        psx.SetControlConfigProperty("SignOnButton", Psx.PropertyState, (AssistLane > 0 && !wasAutoSignedOff) ? Psx.State.Disabled :
                            Psx.State.Normal);

                        //POS81634 - RAP Attendant tracking - rrs
                        psx.SetControlConfigProperty("CheckInButton", Psx.PropertyState, (AssistLane > 0 && !wasAutoSignedOff) ? Psx.State.Disabled : Psx.State.Normal);
                        psx.SetControlConfigProperty("CheckOutButton", Psx.PropertyState, (AssistLane > 0 && !wasAutoSignedOff) ? Psx.State.Disabled : Psx.State.Normal);
                        //e POS81634 - RAP Attendant tracking - rrs

                        TileLanes();
                        psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, true);
                    }
                    catch (PsxException caught)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::AssistLane() - PsxException: " + caught.Message);
                    }
                    catch (Exception caught)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::AssistLane() - Exception: " + caught.Message);
                        throw caught;
                    }
                }
            }
        }
		
		/// <summary>
		/// Gets whether to dynamically reorder alert lanes
		/// </summary>
		public bool DynamicAlertLanes
		{
			get
			{              
				return dynamicAlertLanes;
			}            
		}

        /// <summary>
        /// Sets or gets the exclusive lane number.
        /// </summary>
        public int ExclusiveLane
        {
            get
            {
                return CurrentExclusiveLane;
            }
            set
            {
                if (ConfigDataRapNet.GetConfigData().MultiExclusiveLanes)
                {
                    AssignMultiExclusiveLane(value);
                }
                else
                {
                    if (CurrentExclusiveLane != value)
                    {
                        AssignSingleExclusiveLane(value);
                    }
                }
            }
        }
                
		/// <summary>
		/// Gets whether to dynamically reorder alert lanes
		/// </summary>
		public bool ShowAlertDefault
		{
			get
			{              
				return showAlertDefault;
			}            
		}
                
		/// <summary>
		/// This function handles the old functionality option where 
		/// only one lane can in exclusive lane at any one time, 
		/// Assign the exclusive lane in this case and refresh the UI as need 
		/// </summary>
		/// <param name="lane"> Lane number to make exclusive lane, 0 if none</param>
		private void AssignSingleExclusiveLane(int lane)
		{
			try
			{
				int oldExclusiveLane = CurrentExclusiveLane;
				if (lane > 0)
				{
					if (0 != oldExclusiveLane)
					{
						CmDataCapture.Capture(CmDataCapture.MaskWarning,
                            "SharedPsx::ExclusiveLane() - More than one Lane trying to acquire exclusive mode. Cancelling operation for Lane:"
                            + lane.ToString());
						LanePsx lanePsx = LanePsxArray[lane - 1];
						if (null != lanePsx)
						{                                    
							lanePsx.psx.GenerateEvent(lanePsx.RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap,
                                "operation=release");
						}
						return;
					}                            
				}
				else
				{
					SetAlertLanes(false);
				}	                                        
				psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, false);	            
				CurrentExclusiveLane = lane;
				CmDataCapture.Capture(CmDataCapture.MaskInfo, "SharedPsx::ExclusiveLane() - ExclusiveLane changing to Lane:"
                    + CurrentExclusiveLane.ToString());
				psx.SetControlConfigProperty("SignOnButton", Psx.PropertyState,
                    (((AssistLane > 0) || (CurrentExclusiveLane > 0)) && !wasAutoSignedOff)
                    ? Psx.State.Disabled : Psx.State.Normal);

                //POS81634 - RAP Attendant tracking - rrs
                psx.SetControlConfigProperty("CheckInButton", Psx.PropertyState,
                    (((AssistLane > 0) || (CurrentExclusiveLane > 0)) && !wasAutoSignedOff)
                    ? Psx.State.Disabled : Psx.State.Normal);
                psx.SetControlConfigProperty("CheckOutButton", Psx.PropertyState,
                    (((AssistLane > 0) || (CurrentExclusiveLane > 0)) && !wasAutoSignedOff)
                    ? Psx.State.Disabled : Psx.State.Normal);
                //e POS81634 - RAP Attendant tracking - rrs

                //if (null != scanner) // SSCOADK-1472: From 5.1
                //{
                //    if (0 == CurrentExclusiveLane)
                //    {
                //        scanner.Disable();
                //    }
                //    else
                //    {
                //        scanner.Enable(String.Empty);
                //    }
                //}

                for (int n = 0; n < lanePsxArray.Length; n++)
				{
					if (null != lanePsxArray[n])
					{
						// disabling lane //
						if (CurrentExclusiveLane > 0 && CurrentExclusiveLane != (n + 1))
						{
                            lanePsxArray[n].psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyUserInput, false);
                            if (!ConfigData.IsMobileUI)
                            {
                                lanePsxArray[n].DisableContextLocal = true;
                            }
						}
							// reenabling lane //
						else if (0 == CurrentExclusiveLane && oldExclusiveLane != (n + 1))
						{
                            lanePsxArray[n].psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyUserInput, true);
                            if (!ConfigData.IsMobileUI)
                            {
                                lanePsxArray[n].DisableContextLocal = false;
                            }
						}
					}
				}
				smallExclusive = false;
				if (CurrentExclusiveLane > 0)
				{
					ExclusiveLanePsx.ExclusiveMode = true;
				}
				else if (oldExclusiveLane > 0)
				{
					lanePsxArray[oldExclusiveLane - 1].ExclusiveMode = false;
				}
				TileLanes();
				psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, true);
			}
			catch (PsxException caught)
			{
				CmDataCapture.Capture(CmDataCapture.MaskError,
                    "SharedPsx::ExclusiveLane() - PsxException: " + caught.Message);
			}
			catch (Exception caught)
			{
				CmDataCapture.Capture(CmDataCapture.MaskError,
                    "SharedPsx::ExclusiveLane() - Exception: " + caught.Message);
				throw caught;
			}
		}
	
		/// <summary>
		/// Set an exclusive lane in a multi exclusive lane scenario.  
		/// We'll push the current exclusive lane ( if any ) back on 
		/// our array to deal with later.  
		/// Or if finished with the current exclusive lane, release 
		/// it and set our array to the previous ( still active ) 
		/// exclusive lane 
		/// </summary>
		/// <param name="lane"></param>
		private void AssignMultiExclusiveLane(int lane)
		{
			try
			{
				int oldExclusiveLane = CurrentExclusiveLane;
				// Are we assigning one not already in the list? 
				if	(!IsAnExclusiveLane(lane))	
				{
					if (lane > 0)
					{
						smallExclusive = false;	// Draw current Exclusive lane big 
						PushExclusiveLaneArray(lane);
						CmDataCapture.Capture(CmDataCapture.MaskInfo, "SharedPsx::ExclusiveLane() - ExclusiveLane changing to Lane: " +
                            lane.ToString());
					}
					else
					{
						PopExclusiveLaneArray();
						CmDataCapture.Capture(CmDataCapture.MaskInfo,
                            "SharedPsx::ExclusiveLane - ExclusiveLane returning to previous exclusive lane: " + CurrentExclusiveLane.ToString() +
                            " but not activating ");
						// We've just finished with the previous ExclusiveLane 
						// Draw any exclusive lanes small until the user 
						// selects them via the Lane button. 
						smallExclusive = true;
						SetAlertLanes(false);						
					}
				}
				else	// Reactivating the one in our list 
				{
					smallExclusive = false;			// Draw the current exclusive lane big
					if (lane != CurrentExclusiveLane)
					{
						CmDataCapture.Capture(CmDataCapture.MaskInfo,
                            "SharedPsx::ExclusiveLane - ExclusiveLane returning to previous exclusive lane: " + CurrentExclusiveLane.ToString() +
                            " and activating ");
						MoveExclusiveLaneToEnd(lane);
					}
				}
				ActivateExclusiveLane(oldExclusiveLane);
			}
			catch (PsxException caught)
			{
				CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::ExclusiveLane() - PsxException: " + caught.Message);
			}
			catch (Exception caught)
			{
				CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::ExclusiveLane() - Exception: " + caught.Message);
				throw caught;
			}
		}

		// return true if this lane is currently in the exclusiveLane list
		internal bool IsAnExclusiveLane(int lane)
		{
			if (lane == 0)
			{
				return false;
			}
            if (!ConfigDataRapNet.GetConfigData().MultiExclusiveLanes)
			{
				return CurrentExclusiveLane == lane;
			}
			lock (exclusiveLaneArray.SyncRoot)
			{
				if (exclusiveLaneArray.Count == 0)
				{
					return false;
				}
				for (int i = 0; i < exclusiveLaneArray.Count; i++)
				{
					if ((int)exclusiveLaneArray[ i ] == lane)
					{
						return true;
					}
				}
			}
			return false;
		}

		/// <summary>
		/// How many exclusive lanes do we have? 
		/// </summary>
		private int ExclusiveLaneCount
		{
			get
			{
                if (!ConfigDataRapNet.GetConfigData().MultiExclusiveLanes)
				{
					return ((CurrentExclusiveLane == 0) ? 0 : 1);
				}
				else
				{
					lock (exclusiveLaneArray.SyncRoot)
					{
						return exclusiveLaneArray.Count;
					}
				}
			}
		}
        		        
		/// <summary>
		/// Move the specified lane from the middle of the array 
		/// to the end because the user has selected it out of order.
		/// </summary>
		/// <param name="lane">Lane number</param>
		private void MoveExclusiveLaneToEnd(int lane)
		{
			lock (exclusiveLaneArray.SyncRoot)
			{
				// no need to move it if there's only one
				if (ExclusiveLaneCount < 2)
				{
					return;
				}
				// if the one we want is already last, we're done
				if (CurrentExclusiveLane == lane)
				{
					return;
				}
				for (int i = 0; i < ExclusiveLaneCount; i++)
				{
					// find the specified lane in the middle of the array.  
					if ((int)exclusiveLaneArray[i] == lane)
					{
						// make it the last element in the array
						exclusiveLaneArray.RemoveAt(i);
						exclusiveLaneArray.Add(lane);
						break;
					}
				}
			}
		}


		/// <summary>
		/// Move the specified lane from the middle of the array 
		/// to the end because the user has selected it out of order.
		/// </summary>
		/// <param name="lane">Lane number</param>
		internal void RemoveExclusive( int lane )
		{
			lock( exclusiveLaneArray.SyncRoot )
			{
				int i;

				for( i = 0; i < ExclusiveLaneCount; i++ )
				{
					// find the specified lane in the middle of the array.  
					if	( ( int ) exclusiveLaneArray[ i ] == lane )
					{
						// make it the last element in the array
						exclusiveLaneArray.RemoveAt( i );
						ActivateExclusiveLane( lane );
						break;
					}
				}
			}
		}



		// Pop the next saved ExclusiveLane back to active. 
		private void PopExclusiveLaneArray()
		{
			lock( exclusiveLaneArray.SyncRoot )
			{
				if	( ExclusiveLaneCount > 1 )
				{
					exclusiveLaneArray.RemoveAt( ExclusiveLaneCount - 1 );
				}
				else
				{
					exclusiveLaneArray.Clear();
				}
			}
		}

		// Push a new Lane to the array and activate it
		private void PushExclusiveLaneArray( int lane )
		{
			lock( exclusiveLaneArray.SyncRoot )
			{
				if	( !IsAnExclusiveLane( lane ) )
				{
					exclusiveLaneArray.Add( lane );
				}
			}
		}
        		
		/// <summary>
		/// Disable the assist mode button on all exclusive lanes 
		/// except the current one ( top one ).  And also on the 
		/// current one if it is shown small.  
		/// </summary>
		internal void DisableSmallExLanesAssistMode()
		{
            if (!ConfigData.IsMobileUI)
            {    
                int skipExLane = 0;

                // Disable assist mode button on all lanes but assist mode lane 
                if	( AssistLane > 0 )
                {
                    skipExLane = AssistLane;
                }
                    // or if Exclusive lane is not small, disable assist mode button on
                    // all lanes but the exclusive lane 
                else if	( ( !SmallExclusive ) && ( CurrentExclusiveLane > 0 ) )
                {
                    skipExLane = CurrentExclusiveLane;
                }

                for( int n = 0; n < lanePsxArray.Length; n++ )
                {
                    bool bState = false;
                    if	(AssistLane > 0) 
                    {
                        if	( ( skipExLane == 0 ) || ( skipExLane != ( n + 1 ) ) )
                        {
                            bState = true;
                        }
                    }	

                    if	(lanePsxArray[n] != null)
                    {
                        lanePsxArray[n].psx.SetControlConfigProperty(assistModeBlockButton, Psx.PropertyVisible, bState);
                        lanePsxArray[n].psx.SetControlConfigProperty(assistModeButton, Psx.PropertyVisible, !bState);
                    }
                }
            }
		}

		/// <summary>
		/// Make the lane number passed the active exclusive lane.
		/// Enlarge it's window if necessary and give it keyboard focus
		/// </summary>
		internal void ActivateExclusiveLane(int oldExclusiveLane)
		{
			psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, false);

			if ((IsConnected() || (SignedOn && !IsConnected())) && !((AssistLane > 0) && !wasAutoSignedOff))
			{
				psx.SetControlConfigProperty("SignOnButton", Psx.PropertyState, Psx.State.Normal);
			}
			else
			{
				psx.SetControlConfigProperty("SignOnButton", Psx.PropertyState, Psx.State.Disabled);
			}

            //POS81634 - RAP Attendant tracking - rrs
            psx.SetControlConfigProperty("CheckInButton", Psx.PropertyState, ((AssistLane > 0) && !wasAutoSignedOff) ? Psx.State.Disabled : Psx.State.Normal);
            psx.SetControlConfigProperty("CheckOutButton", Psx.PropertyState, ((AssistLane > 0) && !wasAutoSignedOff) ? Psx.State.Disabled : Psx.State.Normal);
            //e POS81634 - RAP Attendant tracking - rrs

            if (null != scanner)
            {
                // (+) POS181195/POS181197
                if (HHSacquiredbyLane == false)
                {
                    if ((SmallExclusive) || (0 == ExclusiveLaneCount))
                    {
                        scanner.Disable();
                        waitingForScannerMsgResponse = false;
                    }
                    else
                    {
                        //Tar300600
                        if (CurrentExclusiveLane > 0)
                        {
                            LanePsx currentLane = lanePsxArray[CurrentExclusiveLane - 1];
                            if (currentLane.AssistMode)
                            {                                
								scanner.Enable(String.Empty);                            
                            }
                            else
                            {
                                scanner.Disable();
                                waitingForScannerMsgResponse = false;
                            }
                        }
                        //Tar300600
                        // (-) POS181195/POS181197
                    }
                }
            }

            // SSCOADK-1472 : From 5.1
            //if( null != scanner ) 
            //{
            //    if	( ( SmallExclusive ) || ( 0 == ExclusiveLaneCount ) )
            //    {
            //        scanner.Disable();
            //    }
            //    else
            //    {
            //        if (CurrentExclusiveLane > 0)
            //        {
            //            LanePsx currentLane = lanePsxArray[ CurrentExclusiveLane - 1 ];
            //            bool bScannerInputEnabled = false;
            //            try
            //            {
            //                object obj = currentLane.psx.GetTransactionVariable(("DataNeededScannerInputEnabled"));
            //                bScannerInputEnabled = ("0" != obj.ToString());
            //            }
            //            catch (PsxException)
            //            {
            //            }						
            //            if (currentLane.AssistMode || bScannerInputEnabled)
            //            {
            //                scanner.Enable(String.Empty);
            //            }
            //            else
            //            {
            //                scanner.Disable();
            //            }
            //        }
            //    }
            //}

            if (CurrentExclusiveLane > 0)
			{
				ExclusiveLanePsx.ExclusiveMode = !SmallExclusive;
			}
			if ((oldExclusiveLane > 0) && (oldExclusiveLane != CurrentExclusiveLane))
			{
				LanePsx lane = lanePsxArray[oldExclusiveLane - 1];
				lane.ExclusiveMode = false;
                if (lane.LaneStatusState != Psx.State.Normal)
                {
                    lane.LaneStatusState = Psx.State.Normal;
                }
			}			
			for (int n = 0; n < lanePsxArray.Length; n++)
			{
				if	(null != lanePsxArray[n])
				{
					lanePsxArray[n].DisableContextLocal = false;
				}
			}
			TileLanes();
			DisableSmallExLanesAssistMode();
			psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, true);
		}

		/// <summary>
		/// Find the latest used Exclusive Lane in each display column, 
		/// from the Exclusive lane array. 
		/// This will be the last one in the array that would appear 
		/// in the column that matches the param.  
		/// </summary>
		/// <param name="findColumn">Find column 1 lane or column 2 lane</param>
		/// <returns></returns>
		internal int GetExclusiveLaneInColumn(int findColumn)
		{
            if ((ExclusiveLaneCount < 1) || !ConfigDataRapNet.GetConfigData().MultiExclusiveLanes || (findColumn < 1) || (findColumn > 2))
			{
				return 0;
			}
            lock (exclusiveLaneArray.SyncRoot)
			{
				// Look at exclusive lanes backwards in the list 
				// because the most recently used ones are at the end 
				for (int i = ExclusiveLaneCount - 1; i >= 0; i--)
				{
					int excLane = ( int ) exclusiveLaneArray[ i ];
					if	( excLane > 0 )
					{
						excLane--;	// Convert to lanePsxArray index 
						if	( excLane < lanePsxArray.Length ) 
						{
							int laneColumn = ( excLane % 2 ) + 1;                
							if	( laneColumn == findColumn )
							{
								return excLane + 1;	
							}
						}
					}
				}
			}
			return 0; 
		}

		/// <summary>
		/// Returns the LanePsx for the current Exclusive Lane 
		/// </summary>
		public LanePsx ExclusiveLanePsx 
		{
			get
			{
				int excLane = ExclusiveLane; 
				if	( ( excLane > 0 ) 
					&& ( excLane <= LanePsxArray.Length ) )
				{
					return LanePsxArray[ excLane - 1 ];
				}
				else
				{
					return null;
				}
			}
		}

        /// <summary>
        /// Property indicates whether the exclusive lane will be drawn Large 
        /// </summary>
        public bool SmallExclusive
        {
            get
            {
                return ConfigDataRapNet.GetConfigData().MultiExclusiveLanes && smallExclusive;
            }
        }
                        		
		/// <summary>
		/// Make the exclusive lane appear raised, adjust it's location
		/// </summary>
		/// <param name="drawBig"> true if the lane will be drawn enlarged</param>
		/// <param name="laneIdx"> int index in lanePsxArray of the lane being drawn</param>
		/// <param name="count">total number of lanes shown</param>
		/// <param name="point">location where to draw the lane</param>
		internal System.Drawing.Point RaiseExclusiveLocation (bool drawBig, int laneIdx, int count, System.Drawing.Point point)
		{
            if (ConfigData.IsMobileUI)
            {
                return point;
            }
            if ( ( !drawBig ) || ( count > 4 ) )
            {
                return point;
            }    		
            int xoffset = 0, yoffset = 2; 
            switch ( laneIdx )			
            {
                case 0 :			// down and right
                    xoffset = 2;
                    yoffset = 2;
                    break;
                case 1 :			// down and left 
                    xoffset = -2;
                    yoffset = 2;
                    break;
                case 2 :			// down and right
                    xoffset = 2;
                    yoffset = -2;
                    break;
                case 3 :			// down and left 
                    xoffset = -2;
                    yoffset = -2;
                    break;
            }
            return new Point(point.X + xoffset, point.Y + yoffset);            
		}
        		
		/// <summary>
		/// Gets the LanePsx array.
		/// </summary>
		public LanePsx[] LanePsxArray
		{
			get
			{
				return lanePsxArray;
			}
		}
        
		/// <summary>
		/// Gets the area that is reserved for RapSingle contexts.
		/// </summary>
		public Rectangle LaneViewPosition
		{
			get
			{
				return laneViewPosition;
			}
		}
        
		/// <summary>
		/// Gets or sets the operator id.
		/// </summary>
		public string OperatorId
		{
			get
			{
				return operatorId;
			}			
			set
			{
				if( value == null )
				{
					operatorId = String.Empty;
				}
				else
				{
					operatorId = value;
				}
				string text = ConfigData.IsMobileUI ? String.Empty : psx.GetString("OperatorIDText");
				text += (operatorId.Length > 0) ? operatorId : psx.GetString("SignedOffButtonText");
				psx.SetControlConfigProperty("OperationID", Psx.PropertyTextFormat, text);
			}
		}

		/// SR819
		/// <summary>	 
		/// Gets or sets operator type if a valid Head Cashier
		/// </summary>
		public string RapValidHeadCashier
		{
			get
			{
				return rapValidHeadCashier;
			}
			set
			{
				rapValidHeadCashier = value;				
			}
		}
        
		/// <summary>
		/// Gets or sets the operator rights.
		/// </summary>
		public uint OperatorRights
		{
			get
			{
				return operatorRights;
			}
			set
			{
				operatorRights = value;				
			}
		}

		/// <summary>
		/// Gets or sets the current state.
		/// </summary>
		public RapNet.State RapState
		{
			get
			{
				return currentState;
			}			
			set
			{
				if (null != currentState)
				{
					currentState.Dispose();
				}
				currentState = value;
                currentState.Enter();
			}
		}

		/// <summary>
		/// Recalculates our set of alert lanes.
		/// </summary>		
		public void RecalculateAlertLanes()
		{
			if( AlertLanes > 0 )
			{
				lock( alertLaneNumbers.SyncRoot )
				{
					alertLaneNumbers.Clear();
					if (DynamicAlertLanes )
					{
						for( int i = 0; i < lanePsxArray.Length; i++ )
						{
							if( ( null != lanePsxArray[i] ) && ( i + 1 ) != ExclusiveLane &&
                                ( Intervention.Type.None != lanePsxArray[i].LaneIntervention.type )
								&& !lanePsxArray[i].DisableContextRemote )
							{
								alertLaneNumbers.Add( i + 1 );
							}
						}  
						alertLaneNumbers.Sort();
					}
					else
					{
						for ( int i = 0; i < lanePsxArray.Length; i++)
						{
							if( ( null != lanePsxArray[i] ) && ( i + 1 ) != ExclusiveLane )
							{
								alertLaneNumbers.Add( i + 1 );
							}
							else
							{
								alertLaneNumbers.Add( 0 );
							}
						}
					}
					if( alertLaneNumbers.Count > AlertLanes )
					{
						int itemsToRemove = alertLaneNumbers.Count - AlertLanes;
						alertLaneNumbers.RemoveRange( alertLaneNumbers.Count - itemsToRemove, itemsToRemove );
					}
				}                
			}
			if (AssistLane != 0)
			{
				String status = GetAllLaneStatusSummary();
				LanePsx lane = lanePsxArray[ AssistLane - 1];
				if (null != lane && lane.IsConnected)
				{
					lane.psx.GenerateEvent(lane.psx.RemoteConnections[0], String.Empty, String.Empty, ConfigData.eventRap, status);
				}
			}
		}

        /// <summary>
        /// Updates UI controls.
        /// </summary>		
        public virtual void RefreshUi()
        {
            try
            {
                psx.SetControlConfigProperty("SignOnButton", Psx.PropertyState, ((AssistLane == 0) && (IsConnected() || (SignedOn && !IsConnected()))) ? 
                    Psx.State.Normal : Psx.State.Disabled);

                //POS81634 - RAP Attendant tracking - rrs
                psx.SetControlConfigProperty("CheckInButton", Psx.PropertyState, !SignedOn ? Psx.State.Disabled : Psx.State.Normal);
                psx.SetControlConfigProperty("CheckOutButton", Psx.PropertyState, !SignedOn ? Psx.State.Disabled : Psx.State.Normal);
                psx.SetControlConfigProperty("ChangePwdButton", Psx.PropertyState, !SignedOn ? Psx.State.Disabled : Psx.State.Normal);	//POS92988
                //e POS81634 - RAP Attendant tracking - rrs
            }
            catch (PsxException caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::RefreshUi() - PsxException: " + caught.Message);
            }
            catch (Exception caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::RefreshUi() - Exception: " + caught.Message);
                throw caught;
            }
        }
        
        /// <summary>
        /// True if there are connected Lanes.
        /// </summary>		
        public bool IsConnected()
        {
            bool connected = false;
            try
            {
                if (0 == ExclusiveLane)
                {
                    if (lanePsxArray != null)
                    {
                        for (int n = 0; n < lanePsxArray.Length; n++)
                        {
                            if (null != lanePsxArray[n] && lanePsxArray[n].IsConnected)
                            {
                                connected = true;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    connected = true;
                }
            }
            catch (PsxException caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::IsConnected() - PsxException: " + caught.Message);
            }
            catch (Exception caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::IsConnected() - Exception: " + caught.Message);
                throw caught;
            }
            return connected;
        }
        
        /// <summary>
        /// Disposes the items in the lane psx array
        /// </summary>
        private void DisposeLanePsxArray()
        {
            CancelWaitForResponse(0);
            if (null != showTimeTimer)
            {
                showTimeTimer.Dispose();
                showTimeTimer = null;
            }
            if (lanePsxArray != null)
            {
                for (int n = 0; n < lanePsxArray.Length; n++)
                {
                    if (null != lanePsxArray[n])
                    {
                        lanePsxArray[n].Dispose();
                        lanePsxArray[n] = null;
                    }
                }
            }
            lanePsxArray = new LanePsx[0];
            exclusiveLaneArray.Clear();
            exclusiveLaneCurrentValue = 0;
            alertLaneNumbers.Clear();
            assistLane = 0;
        }

        /// <summary>
        /// Loads the items into the lane psx array using a given ConfigData
        /// </summary>
        public bool LoadLanePsxArray()
        {
            ConfigDataRapNet configData = ConfigDataRapNet.GetConfigData();
            lanePsxArray = new LanePsx[configData.LaneConfigList.Count];            
            string host = String.Empty;
            LanePsx lane = null;
            for( int n = 0; n < configData.LaneConfigList.Count; n++ )
            {
                host = (string)configData.LaneConfigList[n];
                lane = host.Length > 0 ? ConfigDataRapNet.LoadClass("LanePsx", new object[] { host, n + 1, this }) as LanePsx : null;
                LanePsxArray[n] = lane;
                if (null != lane)
                {
                    if (!lane.Initialize())
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        /// <summary>
        /// Gets our scanner object.
        /// </summary>
        public ScannerGeneric Scanner
        {
            get
            {
                return scanner;
            }
        }
		        
		/// <summary>
		/// Set/Gets our biometric object.
		/// </summary>
		public Biometric Biometric
		{
			get
			{
				return biometric;
			}				
		}
                
        /// <summary>
		/// Occurs when success returned for RAP Sign-on.
		/// </summary>
		/// <param name="operatorID">
		/// valid operator to be pushed to the lanes
		/// </param>
		/// <param name="rapValidHeadCashier"> 
		/// operator type if valid Head Cashier to be pushed to the lanes 
		/// </param>		
		public void SendOperatorToLanes(string operatorID, string rapValidHeadCashier)
		{
			int signOnLane = 0;
			string param = "operation[text]=sign-on-auto;UserId[text]=" + operatorID + ";RapValidHeadCashier[text]=" + rapValidHeadCashier + ";";
			while (signOnLane < LanePsxArray.Length)
			{
                try
				{
					if (null != LanePsxArray[signOnLane] && LanePsxArray[signOnLane].IsConnected)
					{
						LanePsxArray[signOnLane].psx.GenerateEvent(LanePsxArray[signOnLane].RemoteConnection, String.Empty, String.Empty,
                            ConfigData.eventRap, param);
					}
				}
				catch (PsxException caught)
				{
                    CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::SendOperatorToLanes() - PsxException: " + caught.Message);
				}
				catch (Exception caught)
				{
                    CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::SendOperatorToLanes() - Exception: " + caught.Message);
					throw caught;
				}
				signOnLane++;
			}
		}
        
        /// <summary>
        /// Sets the alert lanes.
        /// </summary>
        /// <param name="alert">
        /// Flag indicating if we are setting or clearing the alert lanes.
        /// </param>
        public void SetAlertLanes(bool alert)
        {
            lock (alertLaneNumbers.SyncRoot)
            {
                psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, false);
				for (int n = 0; n < alertLaneNumbers.Count; n++)
				{
                    if ((int)alertLaneNumbers[n] != 0)
                    {
                        lanePsxArray[(int)alertLaneNumbers[n] - 1].SetAlert(alert, n + 1);
                    }
				}                
                psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, true);
            }            
        }
        
        /// <summary>
		/// Shows or hides all lanes.
		/// </summary>
        /// <param name="show">
        /// Flag indicating if we are showing or hiding the lanes.
        /// </param>
        /// <param name="laneNumber">
        /// Optional lane that will not be shown or hidden. Use 0 for all lanes.
        /// </param>
		public void ShowLanes(bool show, int laneNumber)
		{
            psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, false);
            bool exception = false;
			bool showAlerts = true;
			bool hideAlert = false;
			if (laneNumber != 0)
			{
				showAlerts = lanePsxArray[laneNumber - 1].CanShowAlerts() && lanePsxArray[laneNumber - 1].FullScreen;
			}
			for (int n = 0; n < lanePsxArray.Length; n++)
			{
				if (null != lanePsxArray[n])
				{
					lock (alertLaneNumbers.SyncRoot)
					{
						hideAlert =  (alertLaneNumbers.Contains(n + 1) && showAlerts && !show);
						exception = 0 != laneNumber && ((laneNumber - 1) == n || hideAlert);
					}
					lanePsxArray[n].psx.Visible = (exception ^ show);
				}
			} 
            psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, true);
		}
        
        /// <summary>
		/// Gets or sets a value indicating whether we are signed on.
		/// </summary>
		public bool SignedOn
		{
			get
			{
				return signedOn;
			}			
			set
			{
				if (value != signedOn)
				{
					// Signoff from LaunchPad then Sign back on
					// should not alter Rap in AssistMode.  During 
					// AssistMode, signoff Rap should not occur.  Signoff 
					// Rap button is disabled.  
					// LaunchPad subsequent re sign on enabled signon button
					// on Rap when AssistMode was in progress. But LP signoff 
					// didn't disable the Rap AssistMode screen.  
					if (signedOn && (AssistLane != 0) && !wasAutoSignedOff)
					{
						return;
					}
					signedOn = value;
					for (int n = 0; n < lanePsxArray.Length; n++)
					{
						if (null != lanePsxArray[n])
						{
							lanePsxArray[n].RefreshContext();
						}
					}				
                    try
                    {
                        psx.SetControlConfigProperty("SignOnButton", Psx.PropertyTextFormat, signedOn ? "$SignOffButtonText$" : "$SignOnButtonText$");
                        psx.SetControlConfigProperty("SignOnButton", Psx.PropertyFlash, !signedOn);                        						
                    }
                    catch (PsxException caught)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::SignedOn() - PsxException: " + caught.Message);
                    }
					catch (Exception caught)
					{
						CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::SignedOn() - Exception: " + caught.Message);
						throw caught;
					}					
					if (!signedOn)
					{
						SetLanesToMain();
						OperatorId = String.Empty;
						OperatorRights = 0;						
						if (wasSyncSignOnOff)
						{
							if (wasAutoSignedOff)
							{
								CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::SignedOn() - RAPNet automatically signed off.");
							}
							wasSyncSignOnOff = false;
						}
						else
						{
                            Remote.SendMessage(Remote.LaunchpadApplication, "operation=set-signed-off");                            
						}
					}
					else
					{
						if (wasSyncSignOnOff)
						{
							wasSyncSignOnOff = false;
						}
						else
						{
                            Remote.SendMessage(Remote.LaunchpadApplication, "operation=set-signed-on;operator=" + operatorId + ";rights=" + operatorRights.ToString());                            
						}
						if (wasAutoSignedOff)
						{
							if (AssistLane > 0)
							{
								lanePsxArray[AssistLane - 1].psx.GenerateEvent(lanePsxArray[AssistLane-1].RemoteConnection,
                                    String.Empty, String.Empty, ConfigData.eventRap, "operation=toggle;destination=assistmode;");
							}
							if (reportingLane > 0)
							{
                                lanePsxArray[reportingLane - 1].psx.GenerateEvent(LanePsx.LANE_STATUS_BUTTON, "RapSingleContext", Psx.EventClick,
                                    String.Empty);
								lanePsxArray[reportingLane - 1].psx.GenerateEvent("ButtonCommand2", "LaneStatus", Psx.EventClick, String.Empty);
								reportingLane = 0;
							}
							wasAutoSignedOff = false;
						}
					}
                    RefreshUi();                    
				}
			}
		}

		/// <summary>
		/// Tile all Psx displays.
		/// </summary>		
        public virtual void TileLanes()
		{
			// Only tile the lanes if we are in the Startup context and not full screen exclusive mode //
			if	(!psx.GetContext(Psx.DisplayStandard).Equals("Startup") || (ExclusiveLane > 0 && ExclusiveLanePsx.FullScreen))
			{
				return;
			}              
            bool use2Columns = false;
            Size laneSizeColumn1 = new Size( laneViewPosition.Width, laneViewPosition.Height );
            Size laneSizeColumn2 = laneSizeColumn1;                        
            if (ConfigData.IsMobileUI)
            {
                if( lanePsxArray.Length <= 5 )
                {
                    laneSizeColumn1.Height /= 5;
               
                }
                else if( lanePsxArray.Length <= 8 )
                {     
                    use2Columns = true;
                    laneSizeColumn1.Width /= 2;
                    laneSizeColumn1.Height /= 4;
                    laneSizeColumn2 = laneSizeColumn1;
                    
                }
                else if( lanePsxArray.Length <= 10 )
                {     
                    use2Columns = true;
                    laneSizeColumn1.Width /= 2;
                    laneSizeColumn1.Height /= 5;
                    laneSizeColumn2 = laneSizeColumn1;
                }
                else
                {
                    int lane_configuration;
                    if(lanePsxArray.Length % 2 == 1 )
                    {
                        lane_configuration = lanePsxArray.Length + 1;
                    }
                    else 
                    {
                        lane_configuration = lanePsxArray.Length; 
                    }
                    use2Columns = true;
                    laneSizeColumn1.Width /= 2;
                    laneSizeColumn1.Height /= lane_configuration/2;
                    laneSizeColumn2 = laneSizeColumn1;
                }
            }
            else
            {
                use2Columns = true;
                laneSizeColumn1.Width /= 2;
                laneSizeColumn2.Width = laneSizeColumn1.Width;

                if( lanePsxArray.Length <= 4 )
                {
                    laneSizeColumn1.Height /= 2;
                    laneSizeColumn2.Height = laneSizeColumn1.Height;
                }
                else if( lanePsxArray.Length <= 6 )
                {
                    if	( ( !SmallExclusive ) && ( ExclusiveLane > 0 ) )
                    {
                        laneSizeColumn1.Height /= ( 1 == ExclusiveLane % 2 ) ? 4 : 3;
                        laneSizeColumn2.Height /= ( 0 == ExclusiveLane % 2 ) ? 4 : 3;
                    }
                    else
                    {
                        laneSizeColumn1.Height /= 3;
                        laneSizeColumn2.Height = laneSizeColumn1.Height;
                    }
                }
                else if( lanePsxArray.Length <= 8 )
                {
                    if	( ( !SmallExclusive ) && ( ExclusiveLane > 0 ) )
                    {
                        laneSizeColumn1.Height /= ( 1 == ExclusiveLane % 2 ) ? 6 : 4;
                        laneSizeColumn2.Height /= ( 0 == ExclusiveLane % 2 ) ? 6 : 4;
                    }
                    else
                    {
                        laneSizeColumn1.Height /= 4;
                        laneSizeColumn2.Height = laneSizeColumn1.Height;
                    }
                }
                else if( lanePsxArray.Length <= 10 )
                {
                    if	( ( !SmallExclusive ) && ( ExclusiveLane > 0 ) )
                    {
                        laneSizeColumn1.Height /= ( 1 == ExclusiveLane % 2 ) ? 8 : 5;
                        laneSizeColumn2.Height /= ( 0 == ExclusiveLane % 2 ) ? 8 : 5;
                    }
                    else
                    {
                        laneSizeColumn1.Height /= 5;
                        laneSizeColumn2.Height = laneSizeColumn1.Height;
                    }
                }
                // For more than 10 lanes, application will use configurations 12,..14,..16,..18,..20..so on
                // Make sure context definitions for such configuration exists in the PSX XML.
                else
                {
                    int lane_configuration;
                    if(lanePsxArray.Length % 2 == 1 )
                    {
                        lane_configuration = lanePsxArray.Length + 1;
                    }
                    else 
                    {
                        lane_configuration = lanePsxArray.Length; 
                    }
                    int nLanesPerColumn = lane_configuration/2;
                    int nDivisions = lane_configuration - 2;  //simplified relation
                    if	( ( !SmallExclusive ) && ( ExclusiveLane > 0 ) )
                    {
                        laneSizeColumn1.Height /= ( 1 == ExclusiveLane % 2 ) ? nDivisions : nLanesPerColumn;
                        laneSizeColumn2.Height /= ( 0 == ExclusiveLane % 2 ) ? nDivisions : nLanesPerColumn;
                    }
                    else
                    {
                        laneSizeColumn1.Height /= nLanesPerColumn;
                        laneSizeColumn2.Height = laneSizeColumn1.Height;
                    }
                }
            }
            psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, false);
            Point ptLaneColumn1 = new Point( laneViewPosition.X, laneViewPosition.Y );
            Point ptLaneColumn2 = new Point( laneViewPosition.X + laneSizeColumn1.Width, laneViewPosition.Y );
            bool usingColumn2 = false;
            CmDataCapture.Capture(CmDataCapture.MaskInfo, "SharedPsx::TileLanes " + lanePsxArray.Length.ToString() + " lanes, ExclusiveLane: " +
                ExclusiveLane.ToString() + " SmallExclusive: " + SmallExclusive.ToString());
			for( int n = 0; n < lanePsxArray.Length; n++ )
			{
				usingColumn2 = use2Columns ? 1 == ( n % 2 ) : false;                
				bool drawBig = ( ( !SmallExclusive ) && ( ExclusiveLane == ( n + 1 ) ) );
				if( null != lanePsxArray[n] )
				{
					lanePsxArray[n].RefreshContext();

                    bool showsInterventionLane = true;

                    if (ConfigDataRapNet.GetConfigData().EnableRAPLite && ConfigData.IsMobileUI)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskInfo, "TileLanes() - LanePsxArray[" + n + "] Intervention Type = " + LanePsxArray[n].LaneIntervention.type);
                        showsInterventionLane = LanePsxArray[n].LaneIntervention.type == Intervention.Type.Immediate ? true : false;
                    }
                    else 
                    {
                        if(!ConfigData.IsMobileUI)
                        {
                            lanePsxArray[n].psx.SetControlConfigProperty("LaneButton", Psx.PropertyFlash, IsLaneButtonFlashing(n));
                        }                    
                        lanePsxArray[n].psx.Location = RaiseExclusiveLocation(drawBig, n, lanePsxArray.Length, usingColumn2 ? ptLaneColumn2 : ptLaneColumn1);
                    }					
                    lanePsxArray[n].psx.Visible = showsInterventionLane;
				}                
				if (usingColumn2)
				{
					ptLaneColumn2.Y += ( drawBig ) ? laneViewPosition.Height / 2 : laneSizeColumn2.Height;
				}
				else	// Handle column 1 
				{
					ptLaneColumn1.Y += ( drawBig ) ? laneViewPosition.Height / 2 : laneSizeColumn1.Height;
				}
			}
            psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, true);
		}

        /// <summary>
        /// Shows the time on the Rap UI.
        /// </summary>
        private void ShowTime(object state)
        {
            try
			{
				if (showTimeTimer != null)
				{
					heartbeatCounter++;
					if (5 == heartbeatCounter)
					{
                        if (!ConfigData.IsMobileUI)
                        {
                            // Fix TAR325901 - PP: RAP many places in AM where there is no indication of lane #
                            // Modify the RAP in one location as opposed to 
                            // modifying SCOTAPP in many places. The correct
                            // fix is to change SCOTAPP, but this fix will work. The
                            // summary status of lanes were sometimes incorrect because
                            // SCOTAPP did not update the text prior to generating the
                            // "Generate Alert Event" intervention. Read the STAR TAR 
                            // descriptions for the long history on this tar.
                            if (AssistLane != 0)
                            {
                                RecalculateAlertLanes();
                            }
                        }
						heartbeatCounter = 0;
						LogMemoryUse(CmDataCapture.MaskExtensive);
					}
                    UpdateDateTime();
					if (null != trxCount)
                    {
                        trxCount.RefreshTransactionCount(DateTime.Now.Day);
                    }
				}
			}
			catch (Exception caught)
			{
				CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::ShowTime() - Exception: " + caught.Message);
			}
        }

        /// <summary>
        /// Flag indicating if last signon or signoff was performed automatically by synching with Launchpad
        /// </summary>
        public bool WasSyncSignOnOff
        {
            get
            {
                return wasSyncSignOnOff;
            }
            set
            {
                wasSyncSignOnOff = value;				
            }
        }

        /// <summary>
        /// Starts the scanner worker thread.
        /// </summary>
        private bool StartScannerThread()
        {
            ConfigDataRapNet configData = ConfigDataRapNet.GetConfigData();
            if (scanner != null)
            {
                scanner.Dispose();
                scanner = null;
            }
            scanner = ScannerGeneric.CreateScanner(configData.ScannerClass, (Psx)psx, configData.EncryptScanData, configData);
            bool open = ConfigData.IsProxyServer ? true : scanner.Open("SOFTWARE\\NCR\\NcrRapRemoteScanner");
            if (open)
            {
                open = scanner.Enable(String.Empty);
            }
            return open;
        }

		/// <summary>
		/// Is exclusive lane in fullscreen mode
		/// </summary>
		public bool ExclusiveFullScreen
		{
			get
			{
				bool fullscreen = false;
				if	(ExclusiveLane != 0)
				{
					fullscreen = ExclusiveLanePsx.FullScreen;
				}                                                     
				return fullscreen;
			}            
		}

        /// <summary>
        /// Gather and display alerts 
        /// </summary>
        public void PrepAndShowAlerts()
        {
            if (ConfigDataRapNet.GetConfigData().EnableRAPLite && !ConfigData.IsProxyServer)
            {
                RefreshLaneButtons();
            }

            if (RapState != null && RapState.ShowAlerts)
            {
                RecalculateAlertLanes();
                psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, false);
                ShowLanes(false, ExclusiveFullScreen ? ExclusiveLane : 0);
                SetAlertLanes(true);
                psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, true);
            }
        }

        /// <summary>
        /// Disable all contexts while waiting for a response to a button click.
        /// </summary>
        /// <param name="laneNumber">
        /// The lane number that is waiting for a response.
        /// </param>   
        /// <param name="bFreezeRedraw">
        /// Disable redraws while waiting for response
        /// </param>     
        public void WaitForResponse(int laneNumber, bool bFreezeRedraw)
        {
            if (null != waitForResponseTimer)
            {
				return;
            }
			lock (lanePsxArray.SyncRoot)
			{
				waitForResponseFreezeRedraw = bFreezeRedraw;
				if (waitForResponseFreezeRedraw)
				{
					psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, false);
				}
                psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyUserInput, false);
				waitForResponseLane = laneNumber;
				waitForResponseTimer = new Timer(new TimerCallback(WaitResponseTimeout), null, ConfigDataRapNet.GetConfigData().LaneResponseTimeout, 0);
			}
        }

        /// <summary>
        /// Call the report generator utility to generate a Sign On/Off xml file and to transfer the file to the Report Server.
        /// </summary>
        /// <param name="OnOffFlag"></param>
        /// <returns></returns>
        public void LogToReportGenerator(bool OnOffFlag)
        {
            string path = ConfigDataRapNet.GetConfigData().BinPath + "\\";
            string filename = "CreateReportFile.exe";
            string arguments = "";
#if( WINCE )
            arguments += "/R";
#else
            arguments += (ConfigDataRapNet.GetConfigData().ValidateRapTerminalRegistry() ? "/R" : "");// RAP terminal?
#endif
            arguments += " /C";																	// Copy to report server now
            arguments += " /P=" + ConfigDataRapNet.GetConfigData().ReportDataPath;					// Path to copy report data file (typically c:\scot\report)
            arguments += " /T=OperatorLogin";													// OperatorLogins database table
            arguments += " /F=Operator:SignOn:Application";										// Field Names
            arguments += " /V=" + OperatorId + ":" + (OnOffFlag ? "1" : "0") + ":" + "RAP";	// Values to match field names
            CmDataCapture.Capture(CmDataCapture.MaskInfo, "LogToReportGenerator():START");
            try
            {
                ProcessStartInfo startInfo = new ProcessStartInfo(path + filename);
                startInfo.Arguments = arguments;
                startInfo.WindowStyle = ProcessWindowStyle.Hidden;
                Process locProcess = new Process();
                locProcess.StartInfo = startInfo;
                CmDataCapture.Capture(CmDataCapture.MaskError, String.Format("LogToReportGenerator() About to start process..." +
                    path + filename + " " + arguments));
                locProcess.Start();
            }
            catch (Exception ex)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError,
                    String.Format("<Exception> [LogToReportGenerator()] Error starting process: {0} EXCEPTION:{1}", path + filename +
                    " " + arguments, ex.Message));
            }
            CmDataCapture.Capture(CmDataCapture.MaskInfo, "LogToReportGenerator():END");
        }

        // +RFQ 7477
        /// <summary>
        /// Process has requested we let go of the scanner.
        /// Release the device and respond.
        /// </summary>
        public void ProcessReleaseScannerRequest()
        {
            CmDataCapture.Capture(CmDataCapture.MaskInfo, "ProcessReleaseScannerRequest()");
            if (Scanner is ScannerShared)
            {
                if (null != Scanner)
                {
                    scanner.Disable();
                }
                string msg = ConfigData.eventRapOperation + "=" + ScannerGeneric.RemoteMessages.ScannerAvailable + ";";
                Remote.SendMessage(ScannerMsgTarget, msg);
            }
            else
            {
                CmDataCapture.Capture(CmDataCapture.MaskWarning, "ProcessReleaseScannerRequest(): Scanner sharing is not enabled. Ignoring remote message.");
            }
        }

        /// <summary>
        /// Received remote message, that the scanner is now available. 
        /// Process if we're in a valid state
        /// </summary>
        public void ProcessResponseScannerAvailable()
        {
            CmDataCapture.Capture(CmDataCapture.MaskInfo, "ProcessResponseScannerAvailable()");
            if (Scanner is ScannerShared)
            {
                bool waiting = waitingForScannerMsgResponse;
                waitingForScannerMsgResponse = false;
                ClearScannerMsgTimer();

                if (null != scanner)
                {
                    if (waiting || (RapState is SignOnState) || EnableScannerIfApplicable())
                    {
                        if (!Scanner.Enable(String.Empty))
                        {
                            // Go ahead and send a failure remote message to originator.
                            string msg = ConfigData.eventRapOperation + "=" + ScannerGeneric.RemoteMessages.ScannerOpenFailed + ";";
                            Remote.SendMessage(ScannerMsgTarget, msg);
                            CmDataCapture.Capture(CmDataCapture.MaskError, "ProcessResponseScannerAvailable(): Attempted to open scanner and failed.");
                        }
                    }
                }
            }
            else
            {
                CmDataCapture.Capture(CmDataCapture.MaskWarning, "ProcessResponseScannerAvailable(): Scanner sharing is not enabled. Ignoring remote message.");
            }
        }

        public void ProcessBiometricData(string encodedData)
        {
            CmDataCapture.Capture(CmDataCapture.MaskInfo, "ProcessBiometricData()");
            if (ConfigDataRapNet.GetConfigData().UseBiometrics)
            {
                if (RapState is SignOnState)
                {
                    string context = psx.GetContext(Psx.DisplayStandard);
                    if (context == GetExpectedPasswordContext())
                    {
                        (RapState as SignOnState).EnterPassword(encodedData);
                    }
                    else
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskWarning, "ProcessBiometricData(): Must be in password context to process biometric data, ignoring fingerprint event.");
                    }
                }
                else if (RapState is MonitoringState)
                {
                    if ((!SmallExclusive && (0 != ExclusiveLaneCount)) && (CurrentExclusiveLane > 0))
                    {
                        LanePsx currentLane = lanePsxArray[CurrentExclusiveLane - 1];
                        if (currentLane.AssistMode && currentLane.FullScreen)
                        {
                            // remote the biometric data to the FastLane to be passed on to the POS.
                            try
                            {
                                string param = "operation=biomet-data;encoded-data=" + encodedData;
                                currentLane.psx.GenerateEvent(currentLane.RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap, param);
                            }
                            catch (PsxException caught)
                            {
                                CmDataCapture.Capture(CmDataCapture.MaskError, "ProcessBiometricData() - PsxException: " + caught.Message);
                            }
                            catch (Exception caught)
                            {
                                CmDataCapture.Capture(CmDataCapture.MaskError, "ProcessBiometricData() - Exception: " + caught.Message);
                                throw caught;
                            }
                        }
                    }
                }
                else
                {
                    CmDataCapture.Capture(CmDataCapture.MaskWarning, "ProcessBiometricData(): Must be in SignOn state to process biometric data, ignoring fingerprint event.");
                }
            }
            else
            {
                CmDataCapture.Capture(CmDataCapture.MaskWarning, "ProcessBiometricData(): Biometric functionality is not enabled, ignoring fingerprint event.");
            }
        }

        /// <summary>
        /// Verifies if we're in a state where the scanner should be enabled and proceeds 
        /// to attempt to enable it. This method is called as a result of a remote 
        /// message from the app that's sharing the scanner saying that it has released it.
        /// </summary>
        protected bool EnableScannerIfApplicable()
        {
            CmDataCapture.Capture(CmDataCapture.MaskInfo, "EnableScannerIfApplicable()");
            if ((!SmallExclusive && (0 != ExclusiveLaneCount)) && (CurrentExclusiveLane > 0))
            {
                LanePsx currentLane = lanePsxArray[CurrentExclusiveLane - 1];
                if (currentLane.AssistMode)
                {
                    CmDataCapture.Capture(CmDataCapture.MaskInfo, "EnableScannerIfApplicable() -- Enabling scanner.");
                    if (!scanner.Enable(String.Empty))
                    {
                        // Go ahead and send a failure remote message to originator.
                        if (scanner is ScannerShared)
                        {
                            string msg = ConfigData.eventRapOperation + "=" + ScannerGeneric.RemoteMessages.ScannerOpenFailed + ";";
                            Remote.SendMessage(ScannerMsgTarget, msg);
                        }
                        CmDataCapture.Capture(CmDataCapture.MaskError, "EnableScannerIfApplicable() -- Attempted to open scanner and failed.");
                    }
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// Received remote message, that the other process failed to clain the scanner. 
        /// If we're in a state that should have the scanner claimed, take it back.
        /// </summary>
        public void ProcessResponseScannerOpenFailed()
        {
            CmDataCapture.Capture(CmDataCapture.MaskInfo, "ProcessResponseScannerOpenFailed() -- Launchpad notified that it failed to open the scanner");
            EnableScannerIfApplicable();
        }

        /// <summary>
        /// Request the scanner be made available for LaunchPad to claim it.
        /// </summary>
        public void RequestAccessToScanner()
        {
            CmDataCapture.Capture(CmDataCapture.MaskError, "RequestAccessToScanner()");
            if (!scanner.ScannerEnabled)
            {
                if (scanner is ScannerShared)
                {
                    // Send request for target to let go of scanner
                    waitingForScannerMsgResponse = true;
                    string msg = ConfigData.eventRapOperation + "=" + ScannerGeneric.RemoteMessages.ReleaseScanner + ";";
                    Remote.SendMessage(ScannerMsgTarget, msg);
                    // Initialize the remote message timeout.
                    lock (scannerMsgTimerLock)
                    {
                        scannerMsgTimer = new System.Threading.Timer(new TimerCallback(ScannerMsgTimeout), null, scannerMsgTimeout, Timeout.Infinite);
                    }
                }
                else
                {
                    CmDataCapture.Capture(CmDataCapture.MaskError,
                        "RequestAccessToScanner() -- we're not using the shared scanner class - default to standard Enable/Disable of device.");
                    scanner.Enable(String.Empty);
                }
            }
            else
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "RequestAccessToScanner() -- The scanner is already enabled.");
            }
        }

        /// <summary>
        /// Timeout callback for sign on.
        /// </summary>
        private void ScannerMsgTimeout(object state)
        {
            ClearScannerMsgTimer();
            if (waitingForScannerMsgResponse)
            {
                CmDataCapture.Capture(CmDataCapture.MaskWarning, "ScannerMsgTimeout()");
                waitingForScannerMsgResponse = false;
                // We requested the scanner and didn't get a response. Try to enable it anyway.
                if (null != scanner)
                {
                    CmDataCapture.Capture(CmDataCapture.MaskWarning, "ScannerMsgTimeout(): Didn't receive response from scanner sharing app. Attempting to open scanner anyway.");
                    if (!scanner.Enable(String.Empty))
                    {
                        // Go ahead and send a failure remote message to originator.
                        if (scanner is ScannerShared)
                        {
                            string msg = ConfigData.eventRapOperation + "=" + ScannerGeneric.RemoteMessages.ScannerOpenFailed + ";";
                            Remote.SendMessage(ScannerMsgTarget, msg);
                        }
                        CmDataCapture.Capture(CmDataCapture.MaskError, "ScannerMsgTimeout(): Attempted to open scanner and failed");
                    }
                }
            }
        }

        /// <summary>
        /// Method is used to clear the Scanner Message Timer
        /// </summary>
        public void ClearScannerMsgTimer()
        {
            CmDataCapture.Capture(CmDataCapture.MaskExtensive, "ClearScannerMsgTimer()");
            lock (scannerMsgTimerLock)
            {
                if (scannerMsgTimer != null)
                {
                    CmDataCapture.Capture(CmDataCapture.MaskExtensive, "ClearScannerMsgTimer() -- disposing and deleting timer.");
                    // needed because Dispose may not work and timer can still fire after deletion.
                    scannerMsgTimer.Change(Timeout.Infinite, Timeout.Infinite);

                    scannerMsgTimer.Dispose();
                    scannerMsgTimer = null;
                }
            }
        }

        /// <summary>
        /// Determine target for scanner remote messages
        /// </summary>
        public string ScannerMsgTarget
        {
            get
            {
                return Remote.LaunchpadApplication;
            }
        }

        public bool IsScannerMsgTargetRunning
        {
            get
            {
                if (ScannerMsgTarget != "")
                {
                    if (Process.GetProcessesByName(Path.GetFileNameWithoutExtension(ScannerMsgTarget)).Length > 0)
                    {
                        return true;
                    }
                }
                return false;
            }
        }
        // -RFQ 7477

        /// <summary>
        /// Gets our trxCount object.
        /// </summary>
        public TransactionCount TrxCount
        {
            get
            {
                return trxCount;
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
        public virtual void SharedPsx_PsxEvent(object sender, PsxEventArgs e)
        {
            if (disposing)
            {
                return;
            }  
            bool handled = false;
            try
            {
                ConfigData configData = ConfigDataRapNet.GetConfigData();
                if (e.EventName.Equals(Psx.EventChangeContext))
                {
                    string newState = String.Empty;
                    try
                    {
                        newState = psx.GetContextCustomDataVariable("State", e.ContextName);
                    }
                    catch (PsxException caught)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::SharedPsx_PsxEvent() - PsxException: " + caught.Message);
                    }
                    newState = newState + "State";
                    if (newState.Length > 0 && (RapState == null || -1 == RapState.GetType().Name.IndexOf(newState)))
                    {
                        try
                        {
                            CmDataCapture.Capture(CmDataCapture.MaskInfo, "SharedPsx_PsxEvent() - State Transition to : " + newState);
                            if (newState.StartsWith("SignOn"))
                            {
                                if (this.CheckingIn)
                                    RapState = ConfigDataRapNet.LoadClass("CheckInState", new object[] { this, this.psx, true }) as State;
                                else if (this.CheckingOut)
                                    RapState = ConfigDataRapNet.LoadClass("CheckOutState", new object[] { this, this.psx, true }) as State;
                                else
                                    RapState = ConfigDataRapNet.LoadClass(newState, new object[] { this, this.psx, true }) as State;
                            }
                            else
                            {
                                RapState = ConfigDataRapNet.LoadClass(newState, new object[] { this, this.psx, true }) as State;
                            }
                        }
                        catch (Exception)
                        {
                            CmDataCapture.Capture(CmDataCapture.MaskWarning, "SharedPsx::SharedPsx_PsxEvent() - Unknown State: " + newState);
                        }
                    }
                }
                else if (e.EventName.Equals(Psx.EventKeyDown))
                {
                    if (0 != ExclusiveLane)
                    {
                        LanePsx exclusiveLane = ExclusiveLanePsx;
                        if (null != exclusiveLane)
                        {
                            exclusiveLane.psx.GenerateEvent(exclusiveLane.RemoteConnection, Psx.ControlDisplay,
                                exclusiveLane.psx.GetContext(exclusiveLane.psx.DisplayView), e.EventName, e.Param);
                            handled = true;
                        }
                    }
                }
                else if (configData.AltF4ExitAllowed && e.EventName.Equals(Psx.EventQuit))
                {
                    Close();
                    handled = true;
                }
                else if (e.EventName.Equals("ScanEvent"))
                {
                    CmDataCapture.Capture(CmDataCapture.MaskInfo, "SharedPsx_PsxEvent::Received scan event, param: " + e.Param.ToString());
                    string source = e.Param.ToString();
                    string[] stringSeparators = new string[] { ":" };
                    string[] result = source.Split(new char[] { ',' });
                    if (null != result && result.Length >= 1 && null != scanner)
                    {
                        scanner.GenerateScannerEvent(this, new ScannerEventArgs(result[0], result[0], Int32.Parse(result[1])));
                    }
                    handled = true;
                }
                else if (e.EventName.Equals(EventLoadRapNetXml))
                {
                    try
                    {
                        if (configData.LoadedConfigFilePath != "")
                        {
                            XmlDocument myXmlDocument = new XmlDocument();
                            myXmlDocument.Load(configData.LoadedConfigFilePath);
                            XmlNode currentLanesNode = myXmlDocument.SelectSingleNode("RapNet/Lanes");
                            psx.SetControlProperty("LanesXMLData", Psx.PropertyText, currentLanesNode.OuterXml);
                        }
                        else
                        {
                            CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::SharedPsx_PsxEvent(EventLoadRapNetXml) - No Config File");
                        }
                    }
                    catch (XmlException caught)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::SharedPsx_PsxEvent(EventLoadRapNetXml) - XmlException: " +
                            caught.Message);
                    }
                    catch (System.IO.IOException caught)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::SharedPsx_PsxEvent(EventLoadRapNetXml) - IOException: " +
                            caught.Message);
                    }
                    handled = true;
                }
                else if (e.EventName.Equals(EventSaveRapNetXml))
                {
                    try
                    {
                        if (configData.LoadedConfigFilePath != "")
                        {
                            XmlDocument rapNetXmlDocument = new XmlDocument();
                            XmlDocument lanesXmlData = new XmlDocument();
                            rapNetXmlDocument.Load(configData.LoadedConfigFilePath);
                            lanesXmlData.LoadXml(e.Param.ToString());
                            XmlNode rapNetNode = rapNetXmlDocument.SelectSingleNode("RapNet");
                            XmlNode currentLanesNode = rapNetNode.SelectSingleNode("Lanes");
                            XmlNode newLanesNode = lanesXmlData.SelectSingleNode("Lanes");
                            if (newLanesNode != null)
                            {
                                newLanesNode = rapNetXmlDocument.ImportNode(newLanesNode, true);
                                if (currentLanesNode != null)
                                {
                                    rapNetNode.ReplaceChild(newLanesNode, currentLanesNode);
                                }
                                else
                                {
                                    rapNetNode.AppendChild(newLanesNode);
                                }
                            }
                            rapNetXmlDocument.Save(configData.LoadedConfigFilePath);
                        }
                        else
                        {
                            CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::SharedPsx_PsxEvent(EventSaveRapNetXml) - No Config File");
                        }
                    }
                    catch (XmlException caught)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::SharedPsx_PsxEvent(EventSaveRapNetXml) - XmlException: " +
                            caught.Message);
                    }
                    catch (System.IO.IOException caught)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::SharedPsx_PsxEvent(EventSaveRapNetXml) - IOException: " +
                            caught.Message);
                    }
                    catch (System.ArgumentException caught)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::SharedPsx_PsxEvent(EventSaveRapNetXml) - IOException: " +
                            caught.Message);
                    }
                    handled = true;
                }
                else if (e.EventName.Equals(EventReinitializeLanes))
                {
                    try
                    {
                        DisposeLanePsxArray();
                        if (SignedOn)
                        {
                            LogToReportGenerator(false);
                            SignedOn = false;
                            SendOperatorToLanes("", "");
                            ExclusiveLane = 0;
                        }
                        XmlDocument myXmlDocument = new XmlDocument();
                        myXmlDocument.Load(configData.LoadedConfigFilePath);
                        XmlNodeList nodeList = myXmlDocument.GetElementsByTagName("Lane");
                        configData.IntializeLaneConfigList(nodeList);
                        if (!LoadLanePsxArray())
                        {
                            throw new Exception("LoadLanePsxArray failed");
                        }
                        showTimeTimer = new Timer(new TimerCallback(ShowTime), null, 0, 1000);
                        LanePsx.StartConnectionThread(lanePsxArray);                        
                    }
                    catch (Exception caught)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::SharedPsx_PsxEvent(EventReinitializeLanes) - Exception: " +
                            caught.Message);
                        Close();
                        return;
                    }
                    TileLanes();
                    RefreshUi();
                    psx.SetControlProperty("LanesXMLData", Psx.PropertyText, "ReinitializeComplete");
                    psx.SetControlProperty("LanesXMLData", Psx.PropertyText, "");
                    handled = true;
                }
                else if (e.EventName.Equals(Psx.EventClick))
                {
                    if (e.ControlName.Equals("ReleaseExclusiveAccessBtn"))
                    {
                        LanePsx lane = ExclusiveLanePsx;
                        if (null != lane)
                        {
                            if (IsAnExclusiveLane(lane.LaneNumber))
                            {
                                lane.psx.GenerateEvent(lane.RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap, "operation=release");
                            }
                        }
                        handled = true;
                    }
                }
                else if (e.EventName.Equals(EventValidateOperatorRegistry))
                {
                    String eventValue = e.Param.ToString();
                    String[] parameters = eventValue.Split(',');
                    String userID = parameters[0];
                    String userPassword = parameters[1];
                    if (configData.ValidateOperatorRegistry(userID, userPassword))
                    {
                        psx.SetControlProperty("RapConfigureAuthenticate", Psx.PropertyText, "isAuthenticated");
                    }
                    else
                    {
                        psx.SetControlProperty("RapConfigureAuthenticate", Psx.PropertyText, "isNotAuthenticated");
                    }
                    handled = true;
                }
            }
            catch (Exception caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::SharedPsx_PsxEvent() - Exception: " + caught.Message);
            }
            if (!handled && RapState != null)
            {
                RapState.State_PsxEvent(sender, e);
            }
        }
                
        

        /// <summary>
        /// Psx instance.
        /// </summary>        
        public IPsx psx = null;

#if (HHRAP)
        public const string assistModeButton = "HHAssistModeButton";
#else
        public const string assistModeButton = "AssistModeButton";
#endif
        public const string assistModeBlockButton = "AssistModeBlockButton";
                
        /// <summary>		
        /// Returns true if the Lane Button should flash
        /// Conditions:
        /// A - Check if there is an intervention or not. A fix for Tar 339324
        /// B - After switching alternatingly quads with interventions, flash the Lane Buttons of the current unexclusive lane 		 
        /// C - After the intervention is cleared, flash the Lane Button of the deactivated current exclusive lane 
        ///	D - Special case for Check Payment.	
        /// </summary>
        protected bool IsLaneButtonFlashing(int lane)
        {
            if (LanePsxArray[lane].LaneIntervention.type != Intervention.Type.None)
            {
                if ((IsAnExclusiveLane(lane + 1) && CurrentExclusiveLane != (lane + 1) && !SmallExclusive)
                    || (IsAnExclusiveLane(lane + 1) && CurrentExclusiveLane == (lane + 1) && SmallExclusive)
                    || (!IsAnExclusiveLane(lane + 1) && CurrentExclusiveLane != (lane + 1) && SmallExclusive
                        && (LanePsxArray[lane].psx.GetControlConfigProperty("LaneButtonText", Psx.PropertyText).ToString()) == "Assisted Tender")
                   )
                {
                    return true;
                }
            }
            return false;
        }

        protected virtual void UpdateDateTime()
        {
            try
            {
                DateTime dt = DateTime.Now;
                if (ConfigData.IsMobileUI)
                {
                    psx.SetControlConfigProperty("DateText", Psx.PropertyTextFormat, dt.ToString("d"));
                    psx.SetControlConfigProperty("TimeText", Psx.PropertyTextFormat, dt.ToString("T"));
                }
                else
                {
                    psx.SetControlConfigProperty("DateText", Psx.PropertyTextFormat, dt.ToString("F"));
                }
            }
            catch (PsxException caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::UpdateDateTime() - PsxException: " + caught.Message);
            }			
        }

        /// <summary>
        /// Set to true when Dispose has been called.
        /// </summary>
        protected bool disposing = false;
		                        
        /// <summary>
        /// Encapsulates our scanner object.
        /// </summary>
        protected internal ScannerGeneric scanner = null;

        /// <summary>
        /// Indicates the exclusive lane will not be drawn large
        /// </summary>
        protected bool smallExclusive = false;

        		
        /// <summary>
        /// Gets the lane status summary for assist mode.
        /// </summary>
        /// <returns>
        /// Lane status summary.
        /// </returns>		
        private String GetAllLaneStatusSummary()
        {
            string status = "COMMAND=AllLaneStatusSummary;";
            try 
            {
				for( int n = 0; n < lanePsxArray.Length; n++ )
				{
					if( null != lanePsxArray[n] )
					{
						string lStat = lanePsxArray[n].psx.GetControlConfigProperty("LaneButtonText", Psx.PropertyTextFormat).ToString();
						string lName = lanePsxArray[n].psx.GetControlConfigProperty("LaneCaption", Psx.PropertyTextFormat).ToString();
						status = status + "LANE" + n.ToString() + "=" + lName + ";";
						status = status + "STATUS" + n.ToString()+ "=" + lStat + ";";
					}
				}
            }
            catch (PsxException caught)
            {
                CmDataCapture.Capture( CmDataCapture.MaskError, "SharedPsx::GetAllLaneStatusSummary() - PsxException: " + caught.Message );
            }
			catch( Exception caught )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "SharedPsx::GetAllLaneStatusSummary() - Exception: " + caught.Message );
				throw caught;
			}
            return status;
        }
		
        /// <summary>
        /// Gets whether to dynamically reorder alert lanes
        /// </summary>
        private bool GetDynamicAlertLanes
        {
            get
            {
                bool dynamic = true;
                try
                {
                    dynamic = Convert.ToBoolean(psx.GetContextCustomDataVariable("DynamicAlertLanes", "Startup"));
                }
                catch( FormatException )
                {                            
                }
                catch( PsxException caught )
                {
                    CmDataCapture.Capture( CmDataCapture.MaskError, "SharedPsx::getDynamicAlertLanes() - PsxException: " + caught.Message );
                }                                                      
                return dynamic;
            }            
        }
        
		/// <summary>
		/// Gets showAlertDefault value
		/// </summary>
		private bool GetShowAlertDefault
		{
			get
			{
				bool showAlert = true;
				try
				{
					showAlert = Convert.ToBoolean(psx.GetContextCustomDataVariable("ShowAlertDefault", "Startup"));
				}
				catch( FormatException )
				{                            
				}
				catch( PsxException caught )
				{
					CmDataCapture.Capture( CmDataCapture.MaskError, "SharedPsx::getShowAlertDefault() - PsxException: " + caught.Message );
				}                                                      
				return showAlert;
			}            
		}

        /// <summary>
        /// Re-enable contexts after timeout waiting for response.
        /// </summary>
        protected virtual void WaitResponseTimeout(object state)
        {
            lock (lanePsxArray.SyncRoot)
            {
                if (null == waitForResponseTimer)
                {
                    return;
                }
                waitForResponseLane = 0;
				waitForResponseTimer.Dispose();
				waitForResponseTimer = null;
				if (waitForResponseFreezeRedraw)
				{
					waitForResponseFreezeRedraw = false;
					psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, true);
				}
                
                /// SSCOADK-1472 : Fromn 5.1
                //for (int n = 0; n < lanePsxArray.Length; n++)
                //{
                //    if (null != lanePsxArray[n])
                //    {
                //        lanePsxArray[n].psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyUserInput, true);
                //    }
                //}
                psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyUserInput, true);
            }
		}
        		        
        /// <summary>
        /// Sets the lanes to main screen.
        /// </summary>		
        private void SetLanesToMain()
        {
            for (int n = 0; n < lanePsxArray.Length; n++)
            {
                if (null != lanePsxArray[n])
                {
                    if (lanePsxArray[n].ExclusiveMode)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::SetLanesToMain() - Setting lane to main screen... Lane " +
                            n + " current state: " + lanePsxArray[n].psx.GetContext(Psx.DisplayAlternate1).ToString());
                        if (lanePsxArray[n].psx.GetContext(Psx.DisplayAlternate1).Equals("SmAssistMode"))
                        {
                            lanePsxArray[n].psx.GenerateEvent(lanePsxArray[n].RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap,
                                "operation=toggle;destination=main;");
                        }
                        else if (lanePsxArray[n].psx.GetContext(Psx.DisplayAlternate1).Equals("SmReportsMenu") ||
                            lanePsxArray[n].psx.GetContext(Psx.DisplayAlternate1).Equals("SmRunReports") ||
                            lanePsxArray[n].psx.GetContext(Psx.DisplayAlternate1).Equals("SmPrintReports"))
                        {
                            lanePsxArray[n].psx.GenerateEvent(lanePsxArray[n].RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap,
                                "operation=MainScreen;");
                            reportingLane = n + 1;
                        }
                        else
                        {
                            lanePsxArray[n].psx.GenerateEvent(lanePsxArray[n].RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap,
                                "operation=MainScreen;");
                            lanePsxArray[n].psx.GenerateEvent(lanePsxArray[n].RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap,
                                "operation=release;");
                            lanePsxArray[n].ExclusiveMode = false;
                        }
                        break;
                    }
                }
            }
        }

        // <summary>
		/// Starts the biometric.
		/// </summary>		
		private bool StartBiometric()
		{
			ConfigData configData = ConfigDataRapNet.GetConfigData();
			bool bRet = false; 			
			if (configData.BiometricDeviceConfigured)
			{
				if (null != biometric)
				{
					biometric.Dispose();
					biometric = null;
				}
                biometric = ConfigDataRapNet.LoadClass("Biometric", new object[] { this }) as Biometric;			
				if (null != Biometric) 
				{				
					Biometric.BiometricEvent += new BiometricDelegate(Biometric_BiometricEvent);
				}									
				bRet = Biometric.Open();
				if (bRet)
				{
					Biometric.BiometricDeviceConfigured = true; 
				}
			}
			return bRet; 
		}

        private void Biometric_BiometricEvent(object sender, BiometricArgs e)
        {
            try
            {
                if (e != null)
                {
                    switch (e.Status)
                    {
                        case (int)FT_STATUS_CODES.FT_UNSOLICITED_INFO:
                            CmDataCapture.Capture(CmDataCapture.MaskInfo, "SignOnState::Biometric_BiometricEvent() FT_UNSOLICITED_INFO");
                            switch (e.Param1)
                            {
                                case (int)BIOMETRIC_RC.RC_DEVICE_PLUGGED:
                                    if (null != biometric)
                                    {
                                        biometric.DeviceReady = true;
                                    }
                                    break;
                                case (int)BIOMETRIC_RC.RC_DEVICE_UNPLUGGED:
                                    if (null != biometric)
                                    {
                                        biometric.DeviceReady = false;
                                        CmDataCapture.Capture(CmDataCapture.MaskError, "Biometric error. Device unplugged.");
                                    }
                                    break;
                                case (int)BIOMETRIC_RC.RC_SERVER_READY:
                                    if (null != biometric)
                                    {
                                        biometric.ServerReady = true;
                                    }
                                    break;
                                case (int)BIOMETRIC_RC.RC_SERVER_NOTREADY:
                                    if (null != biometric)
                                    {
                                        biometric.ServerReady = false;
                                        CmDataCapture.Capture(CmDataCapture.MaskError, "Biometric error. Server not ready.");
                                    }
                                    break;
                            }
                            break;
                        default:
                            break;

                    }
                }
            }
            catch (Exception ex)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "Biometric Exception: " + ex.Message);
            }
        }

        /// <summary>
        /// Updates the interventions lane array
        /// </summary>
        public void UpdateInterventionsArray()
        {
            if (lanePsxArray != null)
            {
                // iterate through all of the array elements

                for (int n = 0; n < lanePsxArray.Length; n++)
                {

                    if (lanePsxArray[n] != null &&
                        lanePsxArray[n].psx.RemoteConnections != null &&
                        lanePsxArray[n].psx.RemoteConnections.Length > 0)
                    {
                        bool inList = interventionLanesIndexArray.Contains(n);
                        //check if the intervention is new then add the index of the lane in the list 
                        if ((LanePsxArray[n].LaneIntervention.type == Intervention.Type.Immediate) && !(LanePsxArray[n].ExclusiveMode))
                        {
                            if (!inList)
                            {
                                lanePsxArray[n].psx.Visible = true;
                                string sAlertStatusText = "";
                                try
                                {
                                    sAlertStatusText = (string)lanePsxArray[n].psx.GetConfigProperty("AlertStatusText",
                                        lanePsxArray[n].RapSingleContext,
                                        Psx.PropertyText);
                                }
                                catch (PsxException e)
                                {
                                    CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::UpdateInterventionsArray() => PSXException: " + e.Message);
                                }
                                if (0 == sAlertStatusText.Trim().Length)
                                {
                                    CmDataCapture.Capture(CmDataCapture.MaskInfo, "Viewable Intervention from Lane index[" + n + "] added to interventionLanesIndexArray.");
                                    ViewableCountInter++;
                                    CmDataCapture.Capture(CmDataCapture.MaskInfo, "ViewableCountInter = " + ViewableCountInter);
                                    interventionLanesIndexArray.Add(n);
                                }
                                else
                                {
                                    if (ViewableCountInter > 0)
                                    {
                                        interventionLanesIndexArray.Insert((interventionLanesIndexArray.Count - ViewableCountInter), n);
                                        CmDataCapture.Capture(CmDataCapture.MaskInfo, "Clearable intervention from Lane index[" + n + "] inserted to interventionLanesIndexArray at index [" + (interventionLanesIndexArray.Count - ViewableCountInter) + "].");

                                    }
                                    else
                                    {
                                        CmDataCapture.Capture(CmDataCapture.MaskInfo, "Clearable intervention from Lane index[" + n + "] added to interventionLanesIndexArray.");
                                        interventionLanesIndexArray.Add(n);

                                    }
                                }
                            }
                        }
                        //checks if the intervention is cleared then remove the index of the lane in the list                       
                        else
                        {
                            if (inList)
                            {
                                lanePsxArray[n].psx.Visible = false;
                                interventionLanesIndexArray.Remove(n);
                                CmDataCapture.Capture(CmDataCapture.MaskInfo, "Lane index[" + n + "] removed from interventionLanesIndexArray.");
                                string sAlertStatusText = "";
                                try
                                {
                                    sAlertStatusText = (string)lanePsxArray[n].psx.GetConfigProperty("AlertStatusText",
                                        lanePsxArray[n].RapSingleContext,
                                        Psx.PropertyText);
                                }
                                catch (PsxException e)
                                {
                                    CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::UpdateInterventionsArray() => PSXException: " + e.Message);
                                }

                                if (0 == sAlertStatusText.Trim().Length)
                                {
                                    ViewableCountInter--;
                                    CmDataCapture.Capture(CmDataCapture.MaskInfo, "Decremented ViewableCountInter:" + ViewableCountInter);

                                }

                            }
                        }
                    }
                }

            }
        }

        /// <summary>
        /// Show all lanes with immediate intervention.
        /// </summary>      
        public void RefreshLaneButtons()
        {
            psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, false);
            
            UpdateInterventionsArray();

            do // dummy loop
            {
                if (interventionLanesIndexArray.Count >= 1)
                {
                    int displayLanesCount;

                    //determine up to how many lanes to be displayed in the screen
                    if (ConfigDataRapNet.GetConfigData().MaxNumberOfLanesShownForRAPLite >= interventionLanesIndexArray.Count)
                    {
                        displayLanesCount = interventionLanesIndexArray.Count;
                    }
                    else
                    {
                        displayLanesCount = ConfigDataRapNet.GetConfigData().MaxNumberOfLanesShownForRAPLite;
    					
					    // Don't bother to display intervention if it exceeds maximum number(default 5) 
					    // of lanes with intervention to be shown at a time in RAPLite.
					    // return;
                        break;
                    }

                    //tile arrangement
                    bool use2Columns = false;
                    Size laneSizeColumn1 = new Size(laneViewPosition.Width, laneViewPosition.Height);
                    Size laneSizeColumn2 = laneSizeColumn1;

                    if (ConfigDataRapNet.GetConfigData().MaxNumberOfLanesShownForRAPLite <= 5)
                    {
                        laneSizeColumn1.Height /= 5;
                    }
                    else if (ConfigDataRapNet.GetConfigData().MaxNumberOfLanesShownForRAPLite <= 8)
                    {
                        use2Columns = true;
                        laneSizeColumn1.Width /= 2;
                        laneSizeColumn1.Height /= 4;
                        laneSizeColumn2 = laneSizeColumn1;
                    }
                    else if (ConfigDataRapNet.GetConfigData().MaxNumberOfLanesShownForRAPLite <= 10)
                    {
                        use2Columns = true;
                        laneSizeColumn1.Width /= 2;
                        laneSizeColumn1.Height /= 5;
                        laneSizeColumn2 = laneSizeColumn1;
                    }
                    else
                    {
                        int lane_configuration;
                        if (ConfigDataRapNet.GetConfigData().MaxNumberOfLanesShownForRAPLite % 2 == 1)
                        {
                            lane_configuration = ConfigDataRapNet.GetConfigData().MaxNumberOfLanesShownForRAPLite + 1;
                        }
                        else
                        {
                            lane_configuration = ConfigDataRapNet.GetConfigData().MaxNumberOfLanesShownForRAPLite;
                        }
                        use2Columns = true;
                        laneSizeColumn1.Width /= 2;
                        laneSizeColumn1.Height /= lane_configuration / 2;
                        laneSizeColumn2 = laneSizeColumn1;
                    }

                    Point ptLaneColumn1 = new Point(laneViewPosition.X, laneViewPosition.Y);
                    Point ptLaneColumn2 = new Point(laneViewPosition.X + laneSizeColumn1.Width, laneViewPosition.Y);
                    bool usingColumn2 = false;
                    //end of tile arrangement

                    // display all lanes with immediate interventions
                    for (int n = 0; n < displayLanesCount; n++)
                    {
                        usingColumn2 = use2Columns ? 1 == (n % 2) : false;

                        int indexOfLane = Int32.Parse(interventionLanesIndexArray[n].ToString());
                        lanePsxArray[indexOfLane].psx.Location = usingColumn2 ? ptLaneColumn2 : ptLaneColumn1;


                        if (lanePsxArray[indexOfLane].psx.Visible == true)
                        {
                            if (usingColumn2)
                            {
                                ptLaneColumn2.Y += laneSizeColumn2.Height;

                            }
                            else    // Handle column 1 
                            {
                                ptLaneColumn1.Y += laneSizeColumn1.Height;
                            }
                        }
                    }
                }
            } while (false); // end of dummy loop

            psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, true);
        }

        // +RFQ 7477
        /// <summary>
        /// Determines the appropriate User/ID login screen to be displayed based on configuration 
        /// </summary>
        public string GetExpectedEnterIDContext()
        {
            ConfigData configData = ConfigDataRapNet.GetConfigData();
            string context = "";

            if (configData.AllowEmplIdSignOn)
            {
                if (configData.AllowHandKeyEMPLID)
                {
                    //POS105314
                    if (ChangePwd)
                    {
                        context = "ScanIDOrKeyChangePWD";
                    }
                    else
                    {
                        context = "ScanIDOrKey";
                    }
                    //POS105314
                }
                else
                {
                    //POS105314
                    if (ChangePwd)
                    {
                        context = "ScanIDOnlyChangePWD";
                    }
                    else
                    {
                        context = "ScanIDOnly";
                    }
                    //POS105314
                }
            }
            else
            {
                if (configData.AlphaNumericKeyboard)
                {
                    context = "EnterAlphaNumericID";
                }
                else
                {
                    context = "EnterID";
                }
            }
            return context;
        }

        /// <summary>
        /// Determines the appropriate Password/credential login screen to be displayed based on configuration
        /// </summary>
        public string GetExpectedPasswordContext()
        {
            ConfigData configData = ConfigDataRapNet.GetConfigData();
            string context = "";

            // Use the AllowEmplIdSignOn flag to determine the context
            // the biometrics flag should only determine the leadthru
            if (configData.AllowEmplIdSignOn)
            {
                if (configData.AllowHandKeyEMPLID)
                {
                    context = "EnterPasswordOrUseBiometricFromScanIDOrKey";
                }
                else
                {
                    context = "EnterPasswordOrUseBiometricFromScanIDOnly";
                }
            }
            else
            {
                if (configData.AlphaNumericKeyboard)
                {
                    context = "EnterAlphaNumericPassword";
                }
                else
                {
                    context = "EnterPassword";
                }
            }
            return context;
        }

        /// <summary>
        /// Determines the appropriate User/ID login leadthru text to be displayed based on configuration 
        /// </summary>
        public string GetExpectedEnterIDLeadthru()
        {
            ConfigData configData = ConfigDataRapNet.GetConfigData();
            string leadthru = "";

            if (configData.AllowEmplIdSignOn)
            {
                if (configData.AllowHandKeyEMPLID)
                {
                    leadthru = "ScanIDOrKeyLeadthru";
                }
                else
                {
                    leadthru = "ScanIDOnlyLeadthru";
                }
            }
            else
            {
                leadthru = "EnterIDLeadthru";
            }
            return leadthru;
        }

        /// <summary>
        /// Determines the appropriate Password/credential leadthru text to be displayed based on configuration
        /// </summary>
        public string GetExpectedPasswordLeadthru()
        {
            ConfigData configData = ConfigDataRapNet.GetConfigData();
            string leadthru = "";

            if (configData.UseBiometrics)
            {
                leadthru = "EnterPasswordOrUseBiometricLeadthru";
            }
            else
            {
                leadthru = "EnterPasswordLeadthru";
            }

            return leadthru;
        }

        // -RFQ 7477

        private const string EventLoadRapNetXml = "LoadRapNetXml";
        private const string EventSaveRapNetXml = "SaveRapNetXml";
        private const string EventReinitializeLanes = "ReinitializeLanes";
        private const string EventValidateOperatorRegistry = "ValidateOperatorRegistry";

        /// <summary>
        /// Counter for viewable interventions
        /// </summary>
        private int ViewableCountInter = 0;

        /// <summary>
        /// List of alert lanes.
        /// </summary>
        private ArrayList alertLaneNumbers = new ArrayList();

        /// <summary>
        /// Number of alert lanes.
        /// </summary>
        private int alertLanes = -1;
        
        /// <summary>
		/// Current state.
		/// </summary>
		private RapNet.State currentState = null;
        /// <summary>
        /// Exclusive lane number.
        /// </summary>
        private int assistLane = 0;

        /// <summary>
        /// Exclusive lane number.
        /// </summary>
        private int exclusiveLaneCurrentValue = 0;

		/// <summary>
		/// Array of currently exclusive lanes. 
		/// </summary>
		private ArrayList exclusiveLaneArray = new ArrayList();

        /// <summary>
        /// Count in seconds since last heartbeat.
        /// </summary>
        private int heartbeatCounter = 0;

        /// <summary>
		/// Array of lane specific Psx instances.
		/// </summary>
		protected LanePsx[] lanePsxArray = new LanePsx[0];		

        /// <summary>
        /// Size of the area that is reserved for RapSingle contexts.
        /// </summary>
        private Rectangle laneViewPosition = new Rectangle(0, 0, 0, 0);
        
        /// <summary>
        /// Id of currently signed on operator.
        /// </summary>
        private string operatorId = String.Empty;

		///SR819 
		///<summary>
		/// Operator type if a valid Head Cashier.
		/// </summary>
		private string rapValidHeadCashier = String.Empty;

		/// <summary>
		/// Access rights of the currently signed on operator.
		/// </summary>
		private uint operatorRights = 0;
                
		/// <summary>
		/// Encapsulates our Biometric object.
		/// </summary>
		private Biometric biometric = null;   //SR93- Biometrics

        /// <summary>
        /// Timer that shows the time on the Rap UI.
        /// </summary>
        private Timer showTimeTimer = null;
		
        /// <summary>
		/// Flag indicating if we are signed on.
		/// </summary>
		private bool signedOn = false;        

        /// <summary>
        /// Checks whether or not the last signon or signoff was performed automatically by synching with Launchpad
        /// </summary>
        private bool wasSyncSignOnOff = false;
        /// <summary>
        /// Check whether to dynamically reorder and display the alert lanes
        /// </summary>
        private bool dynamicAlertLanes = true;
		/// <summary>
		/// Check whether default is show or not show alert buttons.
		/// </summary>
		private bool showAlertDefault = true;

        /// <summary>
        /// Lane number waiting for a remote response.
        /// </summary>
        protected int waitForResponseLane = 0;

		/// <summary>
		/// Freeze redraw while waiting for a remote response.
		/// </summary>
		private bool waitForResponseFreezeRedraw = false;

		/// <summary>
		/// Timer that reenables contexts after button press.
		/// </summary>
		protected Timer waitForResponseTimer = null;

        /// <summary>
        /// Encapsulates the TransactionCount object.
        /// </summary>
        private TransactionCount trxCount = null;

        /// <summary>
        /// Array of the index of lanes with intervention.
        /// </summary>
        private ArrayList interventionLanesIndexArray = new ArrayList();

		/// <summary>
		/// Auto sign off flag.
		/// </summary>
		private bool wasAutoSignedOff = false;

		/// <summary>
		/// The lane number in reporting mode.
		/// </summary>
		private int reportingLane = 0;

        /// <summary>
        /// The configuration file, which should be overriden by derived implementation (ScanPortal)
        /// </summary>
        protected string configFile = @"\SharedRap.xml";

        /// <summary>
        /// The merge gconfiguration file, which should be overriden by derived implementation (ScanPortal)
        /// </summary>
        protected string mergeConfigFile = @"\SharedRap.000";

        // +RFQ 7477
        //POS81634 	
        /// <summary>
        /// Flag indicating if we are checking in.
        /// </summary>
        private bool checkingIn = false;

        public bool CheckingIn
        {
            set
            {
                checkingIn = value;
            }
            get
            {
                return checkingIn;
            }
        }

        /// <summary>
        /// Flag indicating if we are checking out.
        /// </summary>
        private bool checkingOut = false;

        public bool CheckingOut
        {
            set
            {
                checkingOut = value;
            }
            get
            {
                return checkingOut;
            }
        }
        //e POS81634 	
        //+POS92988
        private bool changePwd = false;

        public bool ChangePwd
        {
            set
            {
                changePwd = value;
            }
            get
            {
                return changePwd;
            }

        }
        //-POS92988
        // (+) POS181195/POS181197
        private bool bHHScanneracquiredbyLane = false;
        public bool HHSacquiredbyLane
        {
            set
            {
                bHHScanneracquiredbyLane = value;
            }
            get
            {
                return bHHScanneracquiredbyLane;
            }
        }


        private long HHScannerLane = 0;
        public long HHSLane
        {
            set
            {
                HHScannerLane = value;
            }
            get
            {
                return HHScannerLane;
            }
        }

        private string HHScannerLaneName = "";
        public string HHSLaneName
        {
            set
            {
                HHScannerLaneName = value;
            }
            get
            {
                return HHScannerLaneName;
            }
        }

        // (-) POS181195/POS181197

        /// <summary>
        /// Timer that waits for RemoteMessage response from LaunchPad
        /// </summary>
        protected System.Threading.Timer scannerMsgTimer = null;

        /// <summary>
        /// Timer for RemoteMessage timeout locking object
        /// </summary>
        protected object scannerMsgTimerLock = new object();

        /// <summary>
        /// Timeout in ms for Scanner remote message response
        /// </summary>
        protected int scannerMsgTimeout = 500; //SSCOI-47808 (was changed from 5000 Ms)

        /// <summary>
        /// Waiting for Scanner remote message response
        /// </summary>
        protected bool waitingForScannerMsgResponse = false;
        // -RFQ 7477

    }
}
