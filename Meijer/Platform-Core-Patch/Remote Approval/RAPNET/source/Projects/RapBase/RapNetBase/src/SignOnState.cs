using System;
using System.Threading;
using System.Runtime.InteropServices;
using PsxNet;
using RPSWNET;

#if (!WINCE)
using System.Windows.Forms;
#endif

namespace RapNet
{
    /// <summary>
    /// 
    /// </summary>
    public class SignOnState : RapNet.State
    {
        /// <summary>
        /// The class constructor.
        /// </summary>
        /// <param name="paramPsx">
        /// SharedPsx instance used to connect.
        /// </param>
        public SignOnState(SharedPsx paramPsx, IPsx psx, bool bScanHndlr)
            : base(paramPsx, psx, bScanHndlr)
        {	            
        }

        /// <summary>
        /// Enters the state.
        /// </summary>
        public override void Enter()
        {
            ConfigData configData = ConfigData.GetConfigData();
            contextEnterId = configData.AlphaNumericKeyboard ? "EnterAlphaNumericID" : "EnterID";
            contextEnterPassword = configData.AlphaNumericKeyboard ? "EnterAlphaNumericPassword" : "EnterPassword";
            if (configData.AlphaNumericKeyboard)
            {
                PopulateKeyboard(false, false);
            }
            if (configData.BiometricDeviceConfigured)
            {
                if (myPsx.Biometric.GetFingerPrint())
                {
                    myPsx.psx.SetConfigProperty("Leadthru", contextEnterId, Psx.PropertyTextFormat, "$EnterIDFpLeadthru$");
                    myPsx.Biometric.BiometricEvent += new BiometricDelegate(Biometric_BiometricEvent);
                }
                else
                {
                    myPsx.psx.SetConfigProperty("Leadthru", contextEnterId, Psx.PropertyTextFormat, "$EnterIDLeadthru$");
                    CmDataCapture.Capture(CmDataCapture.MaskError, "GetFingerPrint() failed.");
                }
            }
            myPsx.ShowLanes(false, 0);
            //if (null != myPsx.Scanner) // SSCOADK-1472 : From 5.1
            //{
            //    myPsx.Scanner.Enable(String.Empty);
            //} 
            base.Enter();  
        }

        /// <summary>
        /// Releases the resources used by the SignOnState instance.
        /// </summary>
        public override void Dispose()
        {
            //if( null != myPsx.Scanner ) // SSCOADK-1472 : From 5.1
            //{
            //    myPsx.Scanner.Disable();
            //}
            ResetSignOnContexts(false);
            if (null != myPsx.Biometric)
            {
                if (myPsx.Biometric.BiometricDeviceConfigured)
                {
                    myPsx.Biometric.BiometricEvent -= new BiometricDelegate(Biometric_BiometricEvent);            
                }
            }	
            if (signOnTimer != null)
            {
                signOnTimer.Dispose();
                signOnTimer = null;
            }            
            base.Dispose();
        }

        /// <summary>
        /// True if scanner is allowed.
        /// </summary>		        
        public override bool AllowScanner
        {
            get
            {
                return true;
            }
        }

        /// <summary>
        /// Execute the signon process.
        /// </summary>		
        /// <param name="param">
        /// SignOn parameter to be passed to FastLane.
        /// </param>
        protected void ProcessSignOn(string param)
        {
            if (signOnTimer == null)
            {
                ConfigData configData = ConfigData.GetConfigData();
                signOnTimer = new System.Threading.Timer(new TimerCallback(SignOnTimeout), null, configData.SignOnTimeout, Timeout.Infinite);
                if (configData.SynchronizeTimeServer)
                {
                    param += ";set-rap-time=true";
                }
                try
                {
                    myPsx.psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyUserInput, false);
                    myPsx.psx.SetControlProperty("Leadthru", Psx.PropertyText, myPsx.psx.GetString("LoggingInLeadthru"));
                    signOnLane = 0;
                    while (signOnLane < myPsx.LanePsxArray.Length)
                    {
                        if (SendSignOn(param))
                        {
                            break;
                        }
                        signOnLane++;
                    }
                    if (signOnLane == myPsx.LanePsxArray.Length)
                    {
                        NavigateFirst();
                    }
                }
                catch (PsxException caught)
                {
                    CmDataCapture.Capture(CmDataCapture.MaskError, "SignOnState::ProcessSignOn() - PsxException: " + caught.Message);
                }
                catch (Exception caught)
                {
                    CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::ProcessSignOn() - Exception: " + caught.Message);
                    throw caught;
                }
            }
        }
        
        /// <summary>
        /// Send signon request to lane.
        /// </summary>
        /// <param name="param">
        /// SignOn parameter to be passed to FastLane.
        /// </param>		
        protected bool SendSignOn(string param)
        {
            bool success = false;            
            try
            {
                if (null != myPsx.LanePsxArray[signOnLane] && null != myPsx.LanePsxArray[signOnLane].psx.RemoteConnections)
                {
                    myPsx.LanePsxArray[signOnLane].psx.GenerateEvent(myPsx.LanePsxArray[signOnLane].RemoteConnection, String.Empty, String.Empty,
                        ConfigData.eventRap, param);
                    success = true;
                }            
            }
            catch (PsxException caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "SignOnState::SendSignOn() - PsxException: " + caught.Message);
            }
			catch (Exception caught)
			{
				CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::SendSignOn() - Exception: " + caught.Message);
				throw caught;
			}
            return success;
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
				if (e.ContextName.Equals(contextEnterPassword) && (e.ControlName.Equals("KeyBoardP4") || e.ControlName.Equals("AlphaNumKey3")))
				{
					EnterPassword();                    
					handled = true;
				}
                else if ((e.ContextName.Equals(contextEnterId) || e.ContextName.Equals(contextEnterPassword))
                    && (e.ControlName.Equals("ShiftKey")))
                {
                    bool uppercase = Psx.State.Pushed == (Psx.State)Convert.ToInt32(myPsx.psx.GetControlProperty("ShiftKey", Psx.PropertyState));
                    PopulateKeyboard(uppercase, false);
                    handled = true;
                }
                else if ((e.ContextName.Equals(contextEnterId) || e.ContextName.Equals(contextEnterPassword))
                    && (e.ControlName.Equals("SymbolsKey")))
                {
                    String buttonText = (string)myPsx.psx.GetControlProperty("SymbolsKey", Psx.PropertyText);
                    String symbolTextOff = myPsx.psx.GetString("SymbolButtonTextOff");

                    bool symbolsOn = false;
                    if (buttonText.Equals(symbolTextOff))
                    {
                        symbolsOn = true;
                    }
                    SetShiftAndSymbolProperties(symbolsOn);
                    PopulateKeyboard(false, symbolsOn);
                    handled = true;
                }
				else if (e.ContextName.Equals(contextEnterPassword) && e.ControlName.Equals("ButtonCommand0"))
				{
					myPsx.psx.SetConfigProperty("Echo", contextEnterId, Psx.PropertyTextFormat, String.Empty);				
					if (ConfigData.GetConfigData().BiometricDeviceConfigured)
					{
						if( myPsx.Biometric.GetFingerPrint() )
						{																							
							myPsx.psx.SetConfigProperty("Leadthru", contextEnterId, Psx.PropertyTextFormat, "$EnterIDFpLeadthru$");
							myPsx.Biometric.BiometricEvent += new BiometricDelegate(Biometric_BiometricEvent);
						}
						else
						{
							myPsx.psx.SetConfigProperty("Leadthru", contextEnterId, Psx.PropertyTextFormat, "$EnterIDLeadthru$");
							CmDataCapture.Capture(CmDataCapture.MaskError, "GetFingerPrint() failed.");
						}
					}
				}			
			}
			else if (e.EventName.Equals(ConfigData.eventRap))
			{
				string operation = ConfigData.ExtractRapEventData(ConfigData.eventRapOperation, e.Param.ToString());
				if (operation != null && operation.Equals("sign-on"))
				{
					string signOnRes = ConfigData.ExtractRapEventData("success", e.Param.ToString());
					if (signOnRes != null)
					{   
						if (signOnTimer != null)
						{
							signOnTimer.Dispose();
							signOnTimer = null;
						}                        
						bool bPassed = false;                 
						uint signOnResMask = Convert.ToUInt32(signOnRes);
						if (signOnResMask == 1)
						{
							bPassed = true;
							ConfigData configData = ConfigData.GetConfigData();
							if (configData.SynchronizeTimeServer)
							{
								string rapTime = ConfigData.ExtractRapEventData("set-rap-time", e.Param.ToString());
								if (rapTime != null)
								{
									string[] timeList = rapTime.Split(new Char[] {','});
									if (null != timeList && timeList.Length >= 6)
									{
										PInvoke.SYSTEMTIME time = new PInvoke.SYSTEMTIME();
										time.Year = Convert.ToInt16(timeList[0]);
										time.Month = Convert.ToInt16(timeList[1]);
										time.Day = Convert.ToInt16(timeList[2]);
										time.Hour = Convert.ToInt16(timeList[3]);
										time.Minute = Convert.ToInt16(timeList[4]);
										time.Second = Convert.ToInt16(timeList[5]);
										time.DayOfWeek = 0;
										time.Milliseconds = 0;
										PInvoke.SetSystemTime(ref time);
									}
								}
							}
							string validHeadCashier = ConfigData.ExtractRapEventData("valid-headcashier", e.Param.ToString());
							string operatorId = ConfigData.ExtractRapEventData("UserId", e.Param.ToString());
							myPsx.OperatorId = operatorId != null ? operatorId : (string)myPsx.psx.GetConfigProperty("Echo", contextEnterId,
                                Psx.PropertyText);
							myPsx.RapValidHeadCashier = validHeadCashier;
							myPsx.OperatorRights = signOnResMask;
							myPsx.LogToReportGenerator(true);
							myPsx.SignedOn = true;
                            if (configData.BiometricDeviceConfigured)
							{	
                                // SSCOADK-1472 [ From 5.1
                                //string scanSound = configData.AudioPath + "\\ValidBiometric.wav";
                                //if (scanSound.Length > 0 && !myPsx.Biometric.KeyedIn)						
                                //{
                                //    try
                                //    {
                                //        myPsx.psx.PlaySound(scanSound);
                                //    }
                                //    catch (Exception caught)
                                //    {
                                //        CmDataCapture.Capture(CmDataCapture.MaskError, "SignOnState::State_PsxEvent() - Exception: " + caught.Message);
                                //    }
                                //}
                                // ] SSCOADK-1472
												
								if (!myPsx.Biometric.KeyedIn)						
								{
									myPsx.PlaySound("ValidBiometric.wav");									
								}
                                else
                                {
                                    myPsx.Biometric.KeyedIn = false;
                                }
							}
                            NavigateFirst();
							myPsx.SendOperatorToLanes(operatorId, validHeadCashier);
						}
						else if (signOnResMask == 2) 
						{
							string param = "operation=sign-on;UserId=";
							try
							{
                                string tempid = (string)myPsx.psx.GetConfigProperty("Echo", contextEnterId, Psx.PropertyText);
                                string temppw = (string)myPsx.psx.GetConfigProperty("Echo", contextEnterPassword, Psx.PropertyText);
                                bool encodeOperatorID = ConfigData.GetConfigData().EncodeOperatorIDPW;
                                param += encodeOperatorID ? Base64EncodeWithDefaultPrefix(tempid) : tempid;
                                param += ";UserPwd=";
                                param += encodeOperatorID ? Base64EncodeWithDefaultPrefix(temppw) : temppw;
							}
							catch (PsxException caught)
							{
								CmDataCapture.Capture(CmDataCapture.MaskError, "SignOnState::EnterPassword() - PsxException: " + caught.Message);
							}
							catch (Exception caught)
							{
								CmDataCapture.Capture(CmDataCapture.MaskError, "SignOnState::EnterPassword() - Exception: " + caught.Message);
								throw caught;
							}                            
							signOnLane++;
							while (signOnLane < myPsx.LanePsxArray.Length)
							{
								if (SendSignOn(param))
								{
									bPassed = true;
									break;
								}
								signOnLane++;
							}
						}                        
						if (!bPassed)
						{
							ResetSignOnContexts(true);
                            NavigateScreen(contextEnterId);
						}
					} 
					else
					{
						if (signOnTimer != null)
						{
							signOnTimer.Dispose();
							signOnTimer = null;
						}
						ResetSignOnContexts(true);
                        NavigateScreen(contextEnterId);
					}               
					handled = true;
				}
			}
			else if (e.EventName.Equals(Psx.EventChangeContext))
			{
				ConfigData configData = ConfigData.GetConfigData();
				if (configData.AlphaNumericKeyboard)
				{
					if (e.ContextName.Equals(contextEnterId))
					{
                        SetShiftAndSymbolProperties(false);
                        PopulateKeyboard(false, false);
						handled = true;
					}
					else if (e.ContextName.Equals(contextEnterPassword))
					{
                        SetShiftAndSymbolProperties(false);
                        PopulateKeyboard(false, false);
						handled = true;
					}
				}
				// Clear out the password field each time the Enter Id context is displayed (this includes when
				// the user presses "go back" from the password screen)
				if (e.ContextName.Equals(contextEnterPassword))
				{	
					myPsx.psx.SetConfigProperty("Echo", contextEnterPassword, Psx.PropertyTextFormat, String.Empty);
					handled = true;
				}
				//disable fp scan in enterpassword screen
				if ( e.ContextName.Equals( contextEnterPassword ) )
				{
					if( configData.BiometricDeviceConfigured )
					{
						bool bRet = myPsx.Biometric.Abort();
						if( false == bRet )
						{
							CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState::State_PsxEvent() - Abort() failed." );
						}
					}
				}				
			}
#if (!WINCE)
			else if (e.EventName.Equals(Psx.EventKeyDown))
			{
				if (Keys.Enter == Psx.ObjectToKeys(e.Param))
				{
					ConfigData configData = ConfigData.GetConfigData();
					if (Psx.State.Normal == (Psx.State)Convert.ToInt32(myPsx.psx.GetControlConfigProperty(configData.AlphaNumericKeyboard ?
                        "AlphaNumKey3" : "KeyBoardP4", Psx.PropertyState)))
					{
						if (e.ContextName.Equals(contextEnterId))
						{
                            NavigateScreen(contextEnterPassword);
							handled = true;
						}
						else if (e.ContextName.Equals(contextEnterPassword))
						{
							EnterPassword();
							handled = true;
						}					
					}
				}						
			}
#endif			
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
            Remote.SendMessage(Remote.LaunchpadApplication, "operation=reset-autosignoff");

            string param = "operation=sign-on;scanner-data=" + e.ScanData +
                ";scanner-data-label=" + e.ScanDataLabel +
                ";scanner-type=" + e.ScanType.ToString();

            if (null != myPsx.Biometric)
            {
                if (myPsx.Biometric.BiometricDeviceConfigured)
                {
                    if (!myPsx.Biometric.Abort())
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "SignOnState::State_ScannerEvent() - Abort() failed.");
                    }
                }
            }
            // If we have the Employee ID configuration enabled, we need to perform the two step login process.
            if (ConfigData.GetConfigData().AllowEmplIdSignOn)
            {
                string barcode = e.ScanDataLabel;
                char[] ctrlChars = new char[] { '\n', '\r', ' ' };
                barcode = barcode.Trim(ctrlChars);
                ConfigData.GetConfigData().SavedScannerEventArgs = new ScannerEventArgs(e.ScanData, barcode, e.ScanType);
                myPsx.psx.SetConfigProperty("Echo", contextEnterId, Psx.PropertyTextFormat, barcode);
                myPsx.psx.SetContext(myPsx.GetExpectedPasswordContext(), Psx.DisplayStandard, false);
            }
            else
            {
                ProcessSignOn(param);
            }
        }

        /// <summary>
        /// Returns a string representation of the object.
        /// </summary>
        public override string ToString()
        {
            return "SignOn";
        }

        /// <summary>
        /// Resets Id and password contexts.
        /// </summary>
        /// <param name="retry">
        /// True if this is after a failed sign on.
        /// </param>				
        private void ResetSignOnContexts(bool retry)
        {
            try
            {
                myPsx.psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyUserInput, true);
                if (ConfigData.IsMobileUI)
                {
                    myPsx.psx.SetConfigProperty("Leadthru", contextEnterId, Psx.PropertyTextFormat, retry ? "$HH_ReEnterIDLeadthru$" :
                        "$EnterIDLeadthru$");
                }
                else
                {
                    if (null != myPsx.Biometric)
                    {					
                        if (myPsx.Biometric.BiometricDeviceConfigured)
                        {					
                            myPsx.psx.SetConfigProperty("Leadthru", contextEnterId, Psx.PropertyTextFormat, retry ? "$ReEnterIDFpLeadthru$" :
                                "$EnterIDFpLeadthru$");
    	                    if (retry)					
                            {									
                                if (!myPsx.Biometric.GetFingerPrint())
                                {
                                    myPsx.psx.SetConfigProperty("Leadthru", contextEnterId, Psx.PropertyTextFormat, retry ? "$ReEnterIDLeadthru$" :
                                        "$EnterIDLeadthru$");    					
                                    CmDataCapture.Capture(CmDataCapture.MaskError, "SignOnState::ResetSignOnContexts() - GetFingerPrint() failed.");
                                }
                            }	
                            myPsx.Biometric.KeyedIn = false;
                        }					
                    }	
                    else
                    {
                        myPsx.psx.SetConfigProperty("Leadthru", contextEnterId, Psx.PropertyTextFormat, retry ? "$ReEnterIDLeadthru$" :
                            "$EnterIDLeadthru$");
                    }
                }
                myPsx.psx.SetConfigProperty("Echo", contextEnterId, Psx.PropertyTextFormat, String.Empty);
                myPsx.psx.SetConfigProperty("Echo", contextEnterPassword, Psx.PropertyTextFormat, String.Empty);
                SetShiftAndSymbolProperties(false);


            }
            catch (PsxException caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "SignOnState::ResetSignOnContexts() - PsxException: " + caught.Message);
            }
			catch (Exception caught)
			{
				CmDataCapture.Capture(CmDataCapture.MaskError, "SignOnState::State_PsxEvent() - Exception: " + caught.Message);
			}
        }

		/// <summary>
		/// Packages UserId and UserPwd for signon.
		/// </summary>		
		private void EnterPassword(string biometData="")
		{
			if (null != myPsx.Biometric && myPsx.Biometric.BiometricDeviceConfigured)
			{				
				myPsx.Biometric.KeyedIn = true;			
			}
			string param = "operation[text]=sign-on;UserId[text]=";
			try
			{
                string tempid = (string)myPsx.psx.GetConfigProperty("Echo", contextEnterId, Psx.PropertyText);
                string temppw = (string)myPsx.psx.GetConfigProperty("Echo", contextEnterPassword, Psx.PropertyText);
                bool encodeOperatorID = ConfigData.GetConfigData().EncodeOperatorIDPW;
                param += encodeOperatorID ? Base64EncodeWithDefaultPrefix(tempid) : tempid;
                param += ";UserPwd[masked]=";

                #region Meijer customization begin

                if (ConfigData.GetConfigData().AllowEmplIdSignOn)
                {
                    ScannerEventArgs scanData = ConfigData.GetConfigData().SavedScannerEventArgs;
                    bool idScanned = (scanData != null) && (scanData.ScanDataLabel == tempid);
                    // use original implementation whenever possible
                    if (!idScanned && (biometData.Length == 0))
                    {
                        param += encodeOperatorID ? Base64EncodeWithDefaultPrefix(temppw) : temppw;
                    }
                    else
                    {
                        SignOnBuilder sob = new SignOnBuilder(tempid, encodeOperatorID ? Base64EncodeWithDefaultPrefix(temppw) : temppw, idScanned, biometData);
                        param += sob.ToString();
                    }
                }
                else
                    param += myPsx.psx.GetConfigProperty("Echo", contextEnterPassword, Psx.PropertyText);

                #endregion Meijer customization end
			}
			catch (PsxException caught)
			{
				CmDataCapture.Capture(CmDataCapture.MaskError, "SignOnState::EnterPassword() - PsxException: " + caught.Message);
			}
			catch (Exception caught)
			{
				CmDataCapture.Capture(CmDataCapture.MaskError, "SignOnState::EnterPassword() - Exception: " + caught.Message);
				throw caught;
			}
			ProcessSignOn(param);
		}

        /// <summary>
        /// Populate AlphaNumeric keyboard .
        /// </summary>
        /// <param name="shiftOn">True if uppercase, false if lowercase.</param>				
        /// <param name="symbolsOn">True if symbol button is activated</param>
        public void PopulateKeyboard(bool shiftOn, bool symbolsOn)
        {
            try
            {
                CmDataCapture.Capture(CmDataCapture.MaskApi, "+SignOnState::PopulateKeyboard");
                myPsx.psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, false);

                string customDataText = "TextShiftOn";
                if (symbolsOn)
                {
                    customDataText = "TextSymbolOn";
                }

                string contextName = "EnterAlphaNumericID";
                string[] controlNames = new string[] { "AlphaNumP1", "AlphaNumP2", "AlphaNumP3", "AlphaNumP4" };
                string[] symbolButtonText = null;
                object[] symbolButtonData = null;
                object[] buttonDataList = null;
                object[] param = null;
                string symbols = "";
                foreach (string controlname in controlNames)
                {
                    symbols = myPsx.psx.GetCustomDataVariable(customDataText, controlname, contextName);
                    if (0 == symbols.Length || (!shiftOn && !symbolsOn))
                    {
                        symbols = myPsx.psx.GetCustomDataVariable("Text", controlname, contextName);
                    }
                    symbolButtonText = symbols.Split(' ');
                    symbolButtonData = symbols.Split(' ');

                    myPsx.psx.SetControlConfigProperty(controlname, Psx.PropertyButtonCount, symbolButtonText.Length);

                    //delete[] param;
                    param = null;
                    param = new object[1] { (object[])symbolButtonText };
                    myPsx.psx.SendControlCommand(controlname, Psx.CommandSetButtonTextList, 1, param);

                    //delete[] param;
                    buttonDataList = null;
                    buttonDataList = new object[symbolButtonData.Length];
                    for (int i = 0; i < symbolButtonData.Length; i++)
                    {
                        buttonDataList[i] = symbolButtonData[i];
                    }

                    //delete[] param;
                    param = null;
                    param = new object[1] { buttonDataList };
                    myPsx.psx.SendControlCommand(controlname, Psx.CommandSetButtonDataList, 1, param);
                }

                myPsx.psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, true);
            }
            catch (PsxException caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "SignOnState::PopulateKeyboard() - PsxException: " + caught.Message);
            }
            CmDataCapture.Capture(CmDataCapture.MaskApi, "-SignOnState::PopulateKeyboard");
        }

        /// <summary>
        /// Sets the properties of shift button and symbol button.
        /// </summary>
        /// <param name="symbolsOn">
        /// True if the symbol button is activated.
        /// </param>		
        protected void SetShiftAndSymbolProperties(bool symbolsOn)
        {
            if (symbolsOn)
            {
                myPsx.psx.SetControlProperty("SymbolsKey", Psx.PropertyText, myPsx.psx.GetString("SymbolButtonTextOn"));
                myPsx.psx.SetControlProperty("ShiftKey", Psx.PropertyState, Psx.State.Disabled);
            }
            else
            {
                myPsx.psx.SetControlProperty("SymbolsKey", Psx.PropertyText, myPsx.psx.GetString("SymbolButtonTextOff"));
                myPsx.psx.SetControlProperty("ShiftKey", Psx.PropertyState, Psx.State.Normal);
            }
        }

        /// <summary>
        /// Get Base64 encoded string
        /// </summary>
        /// <param name="source">
        /// The string to encode
        /// </param>
        public string Base64Encode(string source)
        {
            byte[] stringSource = System.Text.Encoding.UTF8.GetBytes(source);
            return System.Convert.ToBase64String(stringSource);
        }

        public string Base64EncodeWithDefaultPrefix(string source)
        {
            string encodewithprefix = "";
            try
            {
                encodewithprefix = "BASE64:" + Base64Encode(source);
            }
            catch (Exception caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "SignOnState::Base64EncodeWithDefaultPrefix() - Exception: " + caught.Message);
                throw caught;
            }
            return encodewithprefix;
        }

        /// <summary>
        /// Get Base64 decoded string
        /// </summary>
        /// <param name="encodedData">
        /// Encoded string to be decoded.
        /// </param>
        public string Base64Decode(string encodedData)
        {
            byte[] encodedString = System.Convert.FromBase64String(encodedData);
            return System.Text.Encoding.UTF8.GetString(encodedString);
        }

        /// <summary>
        /// Timeout callback for sign on.
        /// </summary>
        private void SignOnTimeout(object state)
        {
            if (signOnTimer != null)
            {
                NavigateFirst();
                myPsx.psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyUserInput, true);
            }
        }

		/// <summary>
		/// Change the Data list and the Text list in the ButtonList control.
		/// </summary>
		private void ShiftButtonList(string strControl, object[] objDataList, string[] strTextList)
		{
			object[] param = new object[2];
			for (int n = 0; n < objDataList.Length; n++)
			{
				param[0] = n;
				param[1] = objDataList[n];
				myPsx.psx.SendControlCommand(strControl, Psx.CommandSetButtonData, 2, param);
			}
			for (int n = 0; n < strTextList.Length; n++)
			{
				param[0] = n;
				param[1] = strTextList[n];
				myPsx.psx.SendControlCommand(strControl, Psx.CommandSetButtonText, 2, param);
			}
		}
		
		/// <summary>
		/// State Biometric event handler.
		/// </summary>
		/// <param name="sender">
		/// 
		/// </param>		
		/// <param name="e">
		/// 
		/// </param>
		/// <returns>
		/// None
		/// </returns>		
		protected void Biometric_BiometricEvent(object sender, BiometricArgs e)
		{
			try
			{	
				if (e != null)
				{
					switch (e.Status)
					{
						case (int)FT_STATUS_CODES.FT_UNSOLICITED_INFO:
						switch (e.Param1)
						{
							case (int)BIOMETRIC_RC.RC_DEVICE_PLUGGED:
							case (int)BIOMETRIC_RC.RC_SERVER_READY:
								if (myPsx.Biometric.GetFingerPrint())
								{	
									myPsx.psx.SetConfigProperty("Leadthru", contextEnterId, Psx.PropertyTextFormat, "$EnterIDFpLeadthru$");				
								}
								break;
							case (int)BIOMETRIC_RC.RC_DEVICE_UNPLUGGED:
							case (int)BIOMETRIC_RC.RC_SERVER_NOTREADY:
								if (myPsx != null)
								{
									if (myPsx.Biometric != null)
									{
										myPsx.Biometric.Abort();
										myPsx.psx.SetConfigProperty("Leadthru", contextEnterId,  Psx.PropertyTextFormat, "$EnterIDLeadthru$");	
									}
								}
								break;
						}
							break;
						case (int)FT_STATUS_CODES.FT_WAITING_FOR_IMAGE:
							CmDataCapture.Capture( CmDataCapture.MaskInfo, "SignOnState::Biometric_BiometricEvent() FT_WAITING_FOR_IMAGE" );
							break;
						case (int)FT_STATUS_CODES.FT_IMAGE_READY:
							CmDataCapture.Capture( CmDataCapture.MaskInfo, "SignOnState::Biometric_BiometricEvent() FT_IMAGE_READY" );
							break;
						case (int)FT_STATUS_CODES.FT_FINGER_TOUCHING:
							Remote.SendMessage(Remote.LaunchpadApplication, "operation=reset-autosignoff");
							CmDataCapture.Capture( CmDataCapture.MaskInfo, "SignOnState::Biometric_BiometricEvent() FT_FINGER_TOUCHING" );
							break;
						case (int)FT_STATUS_CODES.FT_GET_FINGERPRINT_COMPLETE:
							CmDataCapture.Capture( CmDataCapture.MaskInfo, "SignOnState::Biometric_BiometricEvent() FT_GET_FINGERPRINT_COMPLETE" );						
							if ( (uint)BIOMETRIC_RC.RC_SUCCESS == e.Param1 )
							{
								CmDataCapture.Capture( CmDataCapture.MaskInfo, "SignOnState::Biometric_BiometricEvent() FT_GET_FINGERPRINT_COMPLETE... success!!!" );
								Byte[] features = new Byte[e.Param2];							
								Marshal.Copy(e.IntPtrParam, features, features.GetLowerBound(0), features.GetLength(0));																											
							
								if( !myPsx.Biometric.VerifyUser(features, (int)e.Param2) )
								{
									CmDataCapture.Capture( CmDataCapture.MaskInfo, "SignOnState::Biometric_BiometricEvent() Verify User failed." );
								}														
							}						
							break;
						case (int)FT_STATUS_CODES.FT_VERIFY_USER_COMPLETE:
							CmDataCapture.Capture( CmDataCapture.MaskInfo, "SignOnState::Biometric_BiometricEvent() FT_VERIFY_USER_COMPLETE" );
							if ( (uint)BIOMETRIC_RC.RC_SUCCESS == e.Param1 )
							{

								CmDataCapture.Capture( CmDataCapture.MaskInfo, "SignOnState::Biometric_BiometricEvent() FT_VERIFY_USER_COMPLETE.. success!!!" );
								String userData=Marshal.PtrToStringUni(e.IntPtrParam);
								String userID = "";
								String pwd = "";
								int i = userData.IndexOf("\\n");
								if (i>0)
								{
									userID = userData.Substring(0,i);
                                    if (userData.Length > i + 2)
                                    {
                                        pwd = userData.Substring(i + 2);
                                    }
								}
								CmDataCapture.Capture( CmDataCapture.MaskInfo, "SignOnState::Biometric_BiometricEvent() FT_VERIFY_USER_COMPLETE with id=" + userID );
								string param = "operation[text]=sign-on;UserId[text]=" + userID + ";UserPwd[masked]=" + pwd;
								ProcessSignOn(param);														
							}
							else if ( (uint)BIOMETRIC_RC.RC_FP_NO_MATCH == e.Param1 )
							{							
								CmDataCapture.Capture(CmDataCapture.MaskError, "SignOnState::Biometric_BiometricEvent(), No match found in fingerprint database.");
								ResetSignOnContexts(true);														
							}
							else    // RC_FAILURE
							{
								CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState::Biometric_BiometricEvent(), Failure for some unknown reason." );
							}
							break;
						default: 
							break;						
					}
				}
			}
			catch( Exception ex)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "Biometric Exception: " + ex.Message );
			}
		}

        /// <summary>
        /// EnterId Psx Context name.
        /// </summary>		
        private string contextEnterId;

        /// <summary>
        /// EnterPassword Psx Context name.
        /// </summary>		
        private string contextEnterPassword;

        /// <summary>
        /// Current lane index being sent a signon.
        /// </summary>		
        private int signOnLane = 0;

        /// <summary>
        /// Timer that waits for sign on results.
        /// </summary>
        private System.Threading.Timer signOnTimer = null;
	}
}
