//
// CheckOutState.cs
//
// Change history:
//
// POS81634/82550 Work Request:59313  Name:Robert Susanto  Date:December 12, 2013
// POS81634/82550 Work Request:59313  Name:Robert Susanto  Date:December 10, 2013
//
//////////////////////////////////////////////////////////////////////

using System;
using System.Threading;
using System.Runtime.InteropServices;  //SR93- Biometrics
using PsxNet;
using RPSWNET;

#if( !WINCE )
using System.Windows.Forms;
#endif

namespace RapNet
{
    /// <summary>
    /// 
    /// </summary>
    internal class CheckOutState : RapNet.SignOnState
    {
        /// <summary>
		/// The class constructor.  The CheckOutState is a class to handle the check out operation when attendant pressing the Check out button at the RAP station.
		/// It would send the "check-out" message to the SSCO through RCM and the SSCO would send back the response to this class again through RCM.
		/// The response from SSCO would be processed in the state_psxevent function.
        /// </summary>
        /// <param name="paramPsx">
        /// SharedPsx instance used to connect.
        /// </param>
        public CheckOutState( SharedPsx paramPsx, IPsx psx, bool bScanHndlr ) : base( paramPsx, psx, bScanHndlr )
        {	
			contextEnterId = myPsx.GetExpectedEnterIDContext();
			contextEnterPassword = myPsx.GetExpectedPasswordContext();

            if (ConfigData.GetConfigData().AlphaNumericKeyboard)
			{
                KeyboardUppercase(ConfigData.GetConfigData().EnterIdUppercase);
			}
			ResetCheckOutContexts(false);

			//+SR93- Biometrics 
            if (ConfigData.GetConfigData().BiometricDeviceConfigured)
			{								
				if( myPsx.Biometric.GetFingerPrint() )
				{
                    myPsx.psx.SetConfigProperty("Leadthru", contextEnterId, Psx.PropertyTextFormat, myPsx.psx.GetString("EnterIDFpLeadthru"));
					myPsx.Biometric.BiometricEvent += new BiometricDelegate(Biometric_BiometricEvent);
				}
				else
				{
                    myPsx.psx.SetConfigProperty("Leadthru", contextEnterId, Psx.PropertyTextFormat, myPsx.psx.GetString("EnterIDLeadthru"));
					CmDataCapture.Capture( CmDataCapture.MaskError, "GetFingerPrint() failed." );
				}
			}			
			//-SR93- Biometrics

            myPsx.ShowLanes( false, 0 );               
        }

        /// <summary>
        /// Releases the resources used by the CheckOut instance.
        /// </summary>
        public override void Dispose()
        {           
            ResetCheckOutContexts( false );

            if (null != myPsx.Biometric)
            {
                if (myPsx.Biometric.BiometricDeviceConfigured)
                {
                    myPsx.Biometric.BiometricEvent -= new BiometricDelegate(Biometric_BiometricEvent);
                    //myPsx.Biometric.Dispose();	//394802			
                }
            }	

            if( checkOutTimer != null )
            {
                checkOutTimer.Dispose();
                checkOutTimer = null;
            }

            myPsx.CheckingOut = false;

            base.Dispose();
        }

        /// <summary>
        /// Execute the signon process.
        /// </summary>		
        /// <param name="param">
        /// SignOn parameter to be passed to FastLane.
        /// </param>
        public void ProcessCheckOut( string param )
        {
            if( checkOutTimer == null )
            {
                checkOutTimer = new System.Threading.Timer(new TimerCallback(CheckOutTimeout), null, ConfigData.GetConfigData().SignOnTimeout, Timeout.Infinite);

                if (ConfigData.GetConfigData().SynchronizeTimeServer)
                {
                    param += ";set-rap-time=true";
                }

                try
                {
                    myPsx.psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyUserInput, false);
                    myPsx.psx.SetControlProperty("Leadthru", Psx.PropertyText, myPsx.psx.GetString("CheckingOutLeadthru"));
                            
                    signOnLane = 0;
                    
                    while( signOnLane < myPsx.LanePsxArray.Length )
                    {
						CmDataCapture.Capture( CmDataCapture.MaskError, "CheckOutState::ProcessCheckOut() - SendCheckOut " + param.ToString() );

                        if( SendCheckOut( param ) )
                        {
                            break;
                        }
                        signOnLane++;
                    }                                       
                    if( signOnLane == myPsx.LanePsxArray.Length )
                    {
                        myPsx.psx.SetContext("Startup", Psx.DisplayStandard, false);
                    }
                }
                catch( PsxException caught )
                {
                    CmDataCapture.Capture( CmDataCapture.MaskError, "CheckOutState::ProcessCheckOut() - PsxException: " + caught.Message );
                }
				catch( Exception caught )
				{
					CmDataCapture.Capture( CmDataCapture.MaskError, "SharedPsx::ProcessCheckOut() - Exception: " + caught.Message );
					throw caught;
				}
            }
        }
        
        /// <summary>
        /// Send checkout request to lane.
        /// </summary>
        /// <param name="param">
        /// SignOn parameter to be passed to FastLane.
        /// </param>		
        public bool SendCheckOut( string param )
        {
            bool success = false;            
            try
            {
                if (null != myPsx.LanePsxArray[signOnLane] && null != myPsx.LanePsxArray[signOnLane].psx.RemoteConnections)
                {
                    myPsx.LanePsxArray[signOnLane].psx.GenerateEvent(myPsx.LanePsxArray[signOnLane].RemoteConnection, String.Empty, String.Empty, ConfigData.eventRap, param);
                    success = true;

					CmDataCapture.Capture( CmDataCapture.MaskError, "CheckOutState::SendCheckOut() - SendCheckOut " + param.ToString() );
                }            
            }
            catch( PsxException caught )
            {
                CmDataCapture.Capture( CmDataCapture.MaskError, "CheckOutState::SendCheckOut() - PsxException: " + caught.Message );
            }
			catch( Exception caught )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "SharedPsx::SendCheckOut() - Exception: " + caught.Message );
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
        public override bool State_PsxEvent( object sender, PsxEventArgs e )
		{
			bool handled = false;
			            
			CmDataCapture.Capture( CmDataCapture.MaskError, "CheckOutState::State_PsxEvent() - EventName =  " + e.EventName + ", EventContext = " + e.ContextName + "EventRAP = " + ConfigData.eventRap);

            string userId = (string)myPsx.psx.GetConfigProperty("Echo", contextEnterId, Psx.PropertyText);
			CmDataCapture.Capture( CmDataCapture.MaskError, "CheckOutState::State_PsxEvent() - ContextEnterID = " + contextEnterId + "User ID =  " + userId);

			if( e.EventName.Equals( Psx.EventClick ) )
			{
				if( e.ContextName.Equals( contextEnterPassword ) && ( e.ControlName.Equals( "KeyBoardP4" ) || e.ControlName.Equals( "AlphaNumKey3" ) ) )
				{					
					CmDataCapture.Capture( CmDataCapture.MaskError, "CheckOutState::State_PsxEvent() - ContextEnterID = " + contextEnterId + "User ID =  " + userId);

					EnterPassword("");
                    
					handled = true;
				}
				else if( ( e.ContextName.Equals( contextEnterId ) || e.ContextName.Equals( contextEnterPassword ) ) && ( e.ControlName.Equals( "ShiftKey" ) ) )
				{
                    bool uppercase = Psx.State.Pushed == (Psx.State)Convert.ToInt32(myPsx.psx.GetControlProperty("ShiftKey", Psx.PropertyState));
					KeyboardUppercase( uppercase );

					handled = true;
				}
				else if( e.ContextName.Equals( contextEnterPassword ) && e.ControlName.Equals("ButtonCommand0") )
				{
                    myPsx.psx.SetConfigProperty("Echo", contextEnterId, Psx.PropertyTextFormat, String.Empty);

                    if (ConfigData.GetConfigData().BiometricDeviceConfigured)
					{
						if( myPsx.Biometric.GetFingerPrint() )
						{
                            myPsx.psx.SetConfigProperty("Leadthru", contextEnterId, Psx.PropertyTextFormat, myPsx.psx.GetString("EnterIDFpLeadthru"));
							myPsx.Biometric.BiometricEvent += new BiometricDelegate(Biometric_BiometricEvent);
						}
						else
						{
                            myPsx.psx.SetConfigProperty("Leadthru", contextEnterId, Psx.PropertyTextFormat, myPsx.psx.GetString("EnterIDLeadthru"));
							CmDataCapture.Capture( CmDataCapture.MaskError, "GetFingerPrint() failed." );
						}
					}
				}			
			}
			else if( e.EventName.Equals( ConfigData.eventRap ) )
			{
				string operation = ConfigData.ExtractRapEventData( ConfigData.eventRapOperation, e.Param.ToString() );
				string auxOperation = ConfigData.ExtractRapEventData( "aux-data", e.Param.ToString() );
				CmDataCapture.Capture( CmDataCapture.MaskError, "CheckOutState::State_PsxEvent() - RemoteData operation = " + operation + " AUX operation = " + auxOperation);
				if( operation != null && ( operation.Equals( "sign-on" )) )
				{
					string signOnRes = ConfigData.ExtractRapEventData( "success", e.Param.ToString() );
					CmDataCapture.Capture( CmDataCapture.MaskError, "CheckOutState::State_PsxEvent() - Check Out Success = " + signOnRes);

					if( signOnRes != null )
					{  						
						if( checkOutTimer != null )
						{
							checkOutTimer.Dispose();
							checkOutTimer = null;
						}
                        
						bool bPassed = false;                        

						uint checkOutResMask = Convert.ToUInt32( signOnRes );
						if( checkOutResMask == 1 )
						{
                            bPassed = true;
                            if (ConfigData.GetConfigData().SynchronizeTimeServer)
							{
								string rapTime = ConfigData.ExtractRapEventData( "set-rap-time", e.Param.ToString() );
								if( rapTime != null )
								{
									string[] timeList = rapTime.Split( new Char[] {','} );
									if( null != timeList && timeList.Length >= 6 )
									{
										PInvoke.SYSTEMTIME time = new PInvoke.SYSTEMTIME();
										time.Year = Convert.ToInt16( timeList[0] );
										time.Month = Convert.ToInt16( timeList[1] );
										time.Day = Convert.ToInt16( timeList[2] );
										time.Hour = Convert.ToInt16( timeList[3] );
										time.Minute = Convert.ToInt16( timeList[4] );
										time.Second = Convert.ToInt16( timeList[5] );
										time.DayOfWeek = 0;
										time.Milliseconds = 0;
										PInvoke.SetSystemTime( ref time );
									}
								}
							}

                            myPsx.psx.SetContext("Startup", Psx.DisplayStandard, false);
							
						}
						else if (checkOutResMask == 2) 
						{
							//RFC 2054
							string param = "operation=check-in;UserId=";
							try
							{
                                param += myPsx.psx.GetConfigProperty("Echo", contextEnterId, Psx.PropertyText);
								param += ";UserPwd=";
                                param += myPsx.psx.GetConfigProperty("Echo", contextEnterPassword, Psx.PropertyText);
							}
							catch (PsxException caught)
							{
								CmDataCapture.Capture(CmDataCapture.MaskError, "CheckOutState::EnterPassword() - PsxException: " + caught.Message);
							}
							catch (Exception caught)
							{
								CmDataCapture.Capture(CmDataCapture.MaskError, "CheckOutState::EnterPassword() - Exception: " + caught.Message);
								throw caught;
							}
                            
							signOnLane++;
							while (signOnLane < myPsx.LanePsxArray.Length)
							{
								if (SendCheckOut(param))
								{
									bPassed = true;
									break;
								}
								signOnLane++;
							}
						}
                        
						if(!bPassed)
						{
							ResetCheckOutContexts(true);
                            myPsx.psx.SetContext(contextEnterId, Psx.DisplayStandard, false);
						}
					} 
					else	// TAR - Lane not sending success= string
					{
						if( checkOutTimer != null )
						{
							checkOutTimer.Dispose();
							checkOutTimer = null;
						}
						ResetCheckOutContexts( true );
                        myPsx.psx.SetContext(contextEnterId, Psx.DisplayStandard, false);
					}
               
					handled = true;
				}
			}
			else if( e.EventName.Equals( Psx.EventChangeContext ) )
			{
                if (ConfigData.GetConfigData().AlphaNumericKeyboard)
				{
					if( e.ContextName.Equals( contextEnterId ) )
					{
                        KeyboardUppercase(ConfigData.GetConfigData().EnterIdUppercase);
						handled = true;
					}
					else if( e.ContextName.Equals( contextEnterPassword ) )
					{
                        KeyboardUppercase(ConfigData.GetConfigData().EnterPasswordUppercase);
						handled = true;
					}
				}

				// Clear out the password field each time the Enter Id context is displayed (this includes when
				// the user presses "go back" from the password screen)
				if ( e.ContextName.Equals( contextEnterPassword ) )
				{	
					//(remove for PatchG Build 337) myPsx.SetConfigProperty( "Echo", contextEnterId, Psx.PropertyTextFormat, String.Empty );
                    myPsx.psx.SetConfigProperty("Echo", contextEnterPassword, Psx.PropertyTextFormat, String.Empty);
					handled = true;
				}
				
			}
#if( !WINCE )
			else if( e.EventName.Equals( Psx.EventKeyDown ) )
			{
				if( Keys.Enter == Psx.ObjectToKeys( e.Param ) )
				{
                    if (Psx.State.Normal == (Psx.State)Convert.ToInt32(myPsx.psx.GetControlConfigProperty(ConfigData.GetConfigData().AlphaNumericKeyboard ? "AlphaNumKey3" : "KeyBoardP4", Psx.PropertyState)))
					{
						if( e.ContextName.Equals( contextEnterId ) )
						{
                            myPsx.psx.SetContext(contextEnterPassword, Psx.DisplayStandard, false);
							handled = true;
						}
						else if( e.ContextName.Equals( contextEnterPassword ) )
						{
							EnterPassword("");
							handled = true;
						}					
					}
				}						
			}
#endif
			
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
        public override void State_ScannerEvent( object sender, ScannerEventArgs e )
        {			
			// tell launchpad to reset the auto sign off counter
            Remote.SendMessage(ConfigData.LAUNCHPAD_APP, "operation=reset-autosignoff");

            string param = "operation=check-in;scanner-data=" + e.ScanData + ";scanner-data-label=" + e.ScanDataLabel + ";scanner-type=" + e.ScanType.ToString();
			
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
				ProcessCheckOut(param);
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
        private void ResetCheckOutContexts( bool retry )
        {
            try
            {
				string leadthruText = myPsx.GetExpectedEnterIDLeadthru();
				string leadthruPwdText = myPsx.GetExpectedPasswordLeadthru();
				if( retry ) 
				{
					leadthruText = "Re" + leadthruText;
#if( HH_UI )
					leadthruText = "HH_" + leadThruText;
#endif
				}

                myPsx.psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyUserInput, true);
                myPsx.psx.SetConfigProperty("Leadthru", contextEnterId, Psx.PropertyTextFormat, myPsx.psx.GetString(leadthruText));
                myPsx.psx.SetConfigProperty("Leadthru", contextEnterPassword, Psx.PropertyTextFormat, myPsx.psx.GetString(leadthruPwdText));
                myPsx.psx.SetConfigProperty("Echo", contextEnterId, Psx.PropertyTextFormat, String.Empty);
                myPsx.psx.SetConfigProperty("Echo", contextEnterPassword, Psx.PropertyTextFormat, String.Empty);
#if( !HH_UI )
				//+SR93 Biometrics
				if( null != myPsx.Biometric )
				{					
					if( myPsx.Biometric.BiometricDeviceConfigured )
					{
                        myPsx.psx.SetConfigProperty("Leadthru", contextEnterId, Psx.PropertyTextFormat, retry ? myPsx.psx.GetString("ReEnterIDFpLeadthru") : myPsx.psx.GetString("EnterIDFpLeadthru"));
					
						if( retry )					
						{									
							bool bRet = myPsx.Biometric.GetFingerPrint();
							if( false == bRet )
							{
                                myPsx.psx.SetConfigProperty("Leadthru", contextEnterId, Psx.PropertyTextFormat, retry ? myPsx.psx.GetString("ReEnterIDLeadthru") : myPsx.psx.GetString("EnterIDLeadthru"));
					
								CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState::ResetSignOnContexts() - GetFingerPrint() failed." );
							}
						}	
					if (myPsx.Biometric.KeyedIn)
						myPsx.Biometric.KeyedIn = false; //419076							
					}					
				}
				//-SR93 Biometrics
#endif
            }
            catch( PsxException caught )
            {
                CmDataCapture.Capture( CmDataCapture.MaskError, "CheckOutState::ResetSignOnContexts() - PsxException: " + caught.Message );
            }
			catch( Exception caught )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "CheckOutState::State_PsxEvent() - Exception: " + caught.Message );
			}
        }

		/// <summary>
		/// Packages UserId and UserPwd for signon.
		/// </summary>		
		public void EnterPassword(string biometData) // leave blank if none available
		{
			//+SR93- Biometrics
			if( null != myPsx.Biometric && myPsx.Biometric.BiometricDeviceConfigured )
			{				
				myPsx.Biometric.KeyedIn = true;			
			}
			//-SR93- Biometrics
			string param = "operation[text]=check-out;UserId[text]=";
			try
			{
                string userId = (string)myPsx.psx.GetConfigProperty("Echo", contextEnterId, Psx.PropertyText);
                string userPwd = (string)myPsx.psx.GetConfigProperty("Echo", contextEnterPassword, Psx.PropertyText);
				param += userId;
				param += ";UserPwd[masked]=";

				// If there was a scan event in the login process and 
				// the text has not been tampered, consider it scanned				
                if (ConfigData.GetConfigData().AllowEmplIdSignOn)
				{
                    ScannerEventArgs scanData = ConfigData.GetConfigData().SavedScannerEventArgs;
					bool idScanned = (scanData != null) && (scanData.ScanDataLabel == userId);
					// use original implementation whenever possible
					if( !idScanned && (biometData.Length == 0) )
					{
						param += userPwd;
					}
					else
					{
						SignOnBuilder sob = new SignOnBuilder(userId, userPwd, idScanned, biometData);
						param += sob.ToString();
					}
				}
				else
				{
					// original implementation
					param += userPwd;
				}
			}
			catch( PsxException caught )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "CheckOutState::EnterPassword() - PsxException: " + caught.Message );
			}
			catch( Exception caught )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "CheckOutState::EnterPassword() - Exception: " + caught.Message );
				throw caught;
			}
			ProcessCheckOut( param );
		}

		/// <summary>
		/// Sets AlphaNumeric keyboard to Uppercase or Lowercase.
		/// </summary>
		/// <param name="uppercase">
		/// True if uppercase, false if lowercase.
		/// </param>				
		private void KeyboardUppercase( bool uppercase )
		{
			try
			{
                myPsx.psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, false);

                myPsx.psx.SetControlProperty("ShiftKey", Psx.PropertyState, uppercase ? Psx.State.Pushed : Psx.State.Normal);

				object[] param = new object[1];
				object[] dataAlphaNumP2 = null;
				object[] dataAlphaNumP3 = null;
				object[] dataAlphaNumP4 = null;
				string[] textAlphaNumP2 = null;
				string[] textAlphaNumP3 = null;
				string[] textAlphaNumP4 = null;

				if( !uppercase )
				{
					dataAlphaNumP2 = new object[] { "q", "w", "e", "r", "t", "y", "u", "i", "o", "p" };
					dataAlphaNumP3 = new object[] { "a", "s", "d", "f", "g", "h", "j", "k", "l" };
					dataAlphaNumP4 = new object[] { "z", "x", "c", "v", "b", "n", "m", "." };

					textAlphaNumP2 = new string[] { "q", "w", "e", "r", "t", "y", "u", "i", "o", "p" };
					textAlphaNumP3 = new string[] { "a", "s", "d", "f", "g", "h", "j", "k", "l" };
					textAlphaNumP4 = new string[] { "z", "x", "c", "v", "b", "n", "m", "." };
				}
				else
				{
					dataAlphaNumP2 = new object[] { "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P" };
					dataAlphaNumP3 = new object[] { "A", "S", "D", "F", "G", "H", "J", "K", "L" };
					dataAlphaNumP4 = new object[] { "Z", "X", "C", "V", "B", "N", "M", "." };

					textAlphaNumP2 = new string[] { "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P" };
					textAlphaNumP3 = new string[] { "A", "S", "D", "F", "G", "H", "J", "K", "L" };
					textAlphaNumP4 = new string[] { "Z", "X", "C", "V", "B", "N", "M", "." };
				}

				ShiftButtonList( "AlphaNumP2", dataAlphaNumP2, textAlphaNumP2 );
				ShiftButtonList( "AlphaNumP3", dataAlphaNumP3, textAlphaNumP3 );
				ShiftButtonList( "AlphaNumP4", dataAlphaNumP4, textAlphaNumP4 );

                myPsx.psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, true);
			}
			catch( PsxException caught )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState::KeyboardUppercase() - PsxException: " + caught.Message );
			}
		}

        /// <summary>
        /// Timeout callback for sign on.
        /// </summary>
        private void CheckOutTimeout( object state )
        {
            if( checkOutTimer != null )
            {
                myPsx.psx.SetContext("Startup", Psx.DisplayStandard, false);
                myPsx.psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyUserInput, true);
            }
        }

		/// <summary>
		/// Change the Data list and the Text list in the ButtonList control.
		/// </summary>
		private void ShiftButtonList( string strControl, object[] objDataList, string[] strTextList )
		{
			object[] param = new object[2];
			for( int n = 0; n < objDataList.Length; n++ )
			{
				param[0] = n;
				param[1] = objDataList[n];
                myPsx.psx.SendControlCommand(strControl, Psx.CommandSetButtonData, 2, param);
			}
			for( int n = 0; n < strTextList.Length; n++ )
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
		protected new void Biometric_BiometricEvent(object sender, BiometricArgs e)  //SR93 Biometrics
		{
			try
			{	
				if (e !=null) //407084
				{
					switch( e.Status )
					{
							// +TAR 397478 - Update leadthru for device errors.
						case (int)FT_STATUS_CODES.FT_UNSOLICITED_INFO:
						switch ( e.Param1 )
						{
							case (int)BIOMETRIC_RC.RC_DEVICE_PLUGGED:
							case (int)BIOMETRIC_RC.RC_SERVER_READY:
								if( myPsx.Biometric.GetFingerPrint() )
								{
                                    myPsx.psx.SetConfigProperty("Leadthru", contextEnterId, 
										Psx.PropertyTextFormat,
                                        myPsx.psx.GetString("EnterIDFpLeadthru"));				
								}
								break;
							case (int)BIOMETRIC_RC.RC_DEVICE_UNPLUGGED:
							case (int)BIOMETRIC_RC.RC_SERVER_NOTREADY:
								if(myPsx != null)
								{
									if (myPsx.Biometric != null)
									{
										myPsx.Biometric.Abort();
                                        myPsx.psx.SetConfigProperty("Leadthru", contextEnterId, 
											Psx.PropertyTextFormat,
                                            myPsx.psx.GetString("EnterIDLeadthru"));	
									}
								}
								break;
						}
							break;
							// -TAR 397478 
						case (int)FT_STATUS_CODES.FT_WAITING_FOR_IMAGE:
							CmDataCapture.Capture( CmDataCapture.MaskInfo, "SignOnState::Biometric_BiometricEvent() FT_WAITING_FOR_IMAGE" );
							break;
						case (int)FT_STATUS_CODES.FT_IMAGE_READY:
							CmDataCapture.Capture( CmDataCapture.MaskInfo, "SignOnState::Biometric_BiometricEvent() FT_IMAGE_READY" );
							break;
						case (int)FT_STATUS_CODES.FT_FINGER_TOUCHING:
							Remote.SendMessage( ConfigData.LAUNCHPAD_APP, "operation=reset-autosignoff" );
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
								//Tar 395471
								String userData=Marshal.PtrToStringUni(e.IntPtrParam);
								String userID = "";
								String pwd = "";
								int i = userData.IndexOf("\\n");
								if (i>0)
								{
									userID=userData.Substring(0,i);
									if (userData.Length > i+2)
										pwd=userData.Substring(i+2);
								}
								CmDataCapture.Capture( CmDataCapture.MaskInfo, "SignOnState::Biometric_BiometricEvent() FT_VERIFY_USER_COMPLETE with id=" + userID );
						
								//myPsx.SignedOn = true;	//394802						
								string param = "operation[text]=sign-on;UserId[text]=" + userID + ";UserPwd[masked]=" + pwd;
								ProcessCheckOut(param);														
							}
							else if ( (uint)BIOMETRIC_RC.RC_FP_NO_MATCH == e.Param1 )
							{							
								CmDataCapture.Capture(CmDataCapture.MaskError, "SignOnState::Biometric_BiometricEvent(), No match found in fingerprint database.");
								ResetCheckOutContexts(true);														
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
        private readonly string contextEnterId;

        /// <summary>
        /// EnterPassword Psx Context name.
        /// </summary>		
        private readonly string contextEnterPassword;

        /// <summary>
        /// Current lane index being sent a signon.
        /// </summary>		
        private int signOnLane = 0;

        /// <summary>
        /// Timer that waits for sign on results.
        /// </summary>
        private System.Threading.Timer checkOutTimer = null;
	}
}
