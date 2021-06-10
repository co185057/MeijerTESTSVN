using System;
using System.Threading;
using System.Runtime.InteropServices;
using PsxNet;
using RPSWNET;

#if( !WINCE )
using System.Windows.Forms;
#endif

namespace LaunchPadNet
{
	/// <summary>
	/// 
	/// </summary>
	internal class SignOnState : LaunchPadNet.State
	{
		/// <summary>
		/// The class constructor.
		/// </summary>
		/// <param name="paramPsx">
		/// SharedPsx instance used to connect.
		/// </param>
		public SignOnState( SharedPsx paramPsx ) : base( paramPsx )
		{	
			ConfigData configData = ConfigData.GetConfigData();
			contextEnterId = configData.AlphaNumericKeyboard ? "EnterAlphaNumericID" : "EnterID";
			contextEnterPassword = configData.AlphaNumericKeyboard ? "EnterAlphaNumericPassword" : "EnterPassword";			
			
			ResetSignOnContexts(false); //TAR 443494		

			if( configData.AlphaNumericKeyboard )
			{
                SetShiftAndSymbolProperties(false);
                PopulateKeyboard(false, false);

			}
						
			if( configData.BiometricDeviceConfigured )
			{							
				if( myPsx.Biometric.GetFingerPrint() )
				{																							
					myPsx.SetConfigProperty( "Leadthru", contextEnterId, Psx.PropertyTextFormat, myPsx.GetString( "LaunchPadNet_EnterIDFpLeadthru" ) );				
					myPsx.Biometric.BiometricEvent += new BiometricDelegate(Biometric_BiometricEvent);							
				}
				else
				{
					myPsx.SetConfigProperty( "Leadthru", contextEnterId, Psx.PropertyTextFormat, myPsx.GetString( "LaunchPadNet_EnterIDLeadthru" ) );				
					CmDataCapture.Capture( CmDataCapture.MaskError, "GetFingerPrint() failed." );
				}
			}						

			//myPsx = paramPsx;
			CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState BEFORE IF myPsx.Scanner.Enable " );
			if( null != myPsx.Scanner )
			{
				myPsx.Scanner.Enable( String.Empty );
				CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState AFTER myPsx.Scanner.Enable " );

			}            
		}

		/// <summary>
		/// The class constructor.
		/// </summary>
		/// <param name="paramPsx">
		/// SharedPsx instance used to connect.
		/// </param>
		public SignOnState( SharedPsx paramPsx, string ANKIdOverride, string ANKPasswordOverride) : base( paramPsx )
		{	
			contextEnterId = ANKIdOverride;
			contextEnterPassword = ANKPasswordOverride;

			if (ANKIdOverride == "EnterAlphaNumericID")  //SSCOP-1688
			{
                PopulateKeyboard(false, false);
			}

			ConfigData configData = ConfigData.GetConfigData();
			if( configData.BiometricDeviceConfigured )
			{							
				if( myPsx.Biometric.GetFingerPrint() )
				{																							
					myPsx.SetConfigProperty( "Leadthru", contextEnterId, Psx.PropertyTextFormat, myPsx.GetString( "LaunchPadNet_EnterIDFpLeadthru" ) );				
					myPsx.Biometric.BiometricEvent += new BiometricDelegate(Biometric_BiometricEvent);							
				}
				else
				{
					myPsx.SetConfigProperty( "Leadthru", contextEnterId, Psx.PropertyTextFormat, myPsx.GetString( "LaunchPadNet_EnterIDLeadthru" ) );
					CmDataCapture.Capture( CmDataCapture.MaskError, "GetFingerPrint() failed." );
				}
			}		

			if( null != myPsx.Scanner )
			{
				myPsx.Scanner.Enable( String.Empty );
			}            
			//myPsx = paramPsx;
		}

		/// <summary>
		/// Releases the resources used by the SignOnState instance.
		/// </summary>
		public override void Dispose()
		{
			if( null != myPsx.Scanner )
			{
				myPsx.Scanner.Disable();
			}
			if( signOnTimer != null )
			{
				signOnTimer.Dispose();
				signOnTimer = null;
			}
			ResetSignOnContexts( false );
            if (null != myPsx.Biometric)
            {
                if (myPsx.Biometric.BiometricDeviceConfigured)
                {
                    myPsx.Biometric.BiometricEvent -= new BiometricDelegate(Biometric_BiometricEvent);
                    //myPsx.Biometric.Dispose();	//394802			
                }
            }	

			base.Dispose();
		}

		/// <summary>
		/// Execute the signon process.
		/// </summary>		
		public virtual void ProcessSignOn()
		{	
			int signOnLane = 0;
			bool success = false;

			try 
			{
				ConfigData configData = ConfigData.GetConfigData();
								
				string id = "";
				string password = "";
				if( configData.BiometricDeviceConfigured )
				{
                    //394802
                    if (myPsx.Biometric.KeyedIn)   //keyed-in method
                    {
                        id = (string)myPsx.GetConfigProperty("Echo", contextEnterId, Psx.PropertyText);
                        password = (string)myPsx.GetConfigProperty("Echo", contextEnterPassword, Psx.PropertyText);
                    }
                    else
                    {
                        id = myPsx.Biometric.UserID;
                        password = myPsx.Biometric.UserPwd;		
                    }
				}
				else
				{
					id = (string) myPsx.GetConfigProperty( "Echo", contextEnterId, Psx.PropertyText );
					password = (string) myPsx.GetConfigProperty( "Echo", contextEnterPassword, Psx.PropertyText );
				}				

				if	( ( configData.SignOnTBFirst == false )
					|| ( !LanesConfigured( configData ) ) )
				{
					// Attempt to login using the Registry ID/Passwords...
					success = SignOnViaRegistry( configData, id, password );
					//397472, 397387
					if( configData.BiometricDeviceConfigured && success && !myPsx.Biometric.KeyedIn )
					{																
						string scanSound = configData.AudioPath + "\\ValidBiometric.wav";
						if (scanSound.Length > 0 && !myPsx.Biometric.KeyedIn )
						{
							CmDataCapture.Capture( CmDataCapture.MaskInfo, "ProcessSignOn():SignOnViaRegistry played validBiometric.wav ");
							myPsx.PlaySound(scanSound);
						}	
					
					}	
					if( configData.BiometricDeviceConfigured && null != myPsx.Biometric && success && myPsx.Biometric.KeyedIn )
						myPsx.Biometric.KeyedIn = false;  //395787
				}

				CmDataCapture.Capture( CmDataCapture.MaskError, String.Format( "ProcessSignOn(): p1 success {0} {1} Lane Count {2} Lane Name{3}", 
					success, 
					myPsx.ActiveHostName != null ? myPsx.ActiveHostName : "null", 
					configData.LaneConfigList.Count, 
					configData.LaneConfigList.Count > 0 ? (string)configData.LaneConfigList[0] : "none" ) );

				if( !success && (myPsx.ActiveHostName != null) && (myPsx.ActiveHostName != "") )
				{
					CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState::ProcessSignOn() TB ATTEMPT Id = [" + id + "] Lane = [" + myPsx.ActiveHostName + "]" ); 
					if( SendSignOn( myPsx.ActiveHostName ) )
					{
						signOnTimer = new System.Threading.Timer( new TimerCallback( SignOnTimeout ), null, configData.SignOnTimeout, Timeout.Infinite );
						success = true;
					}

				}

				while( !success && signOnLane < configData.LaneConfigList.Count )
				{
					CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState::ProcessSignOn() TB ATTEMPT Id = [" + id + "] Lane = [" + (string)configData.LaneConfigList[signOnLane] + "]" ); 
					if( SendSignOn((string)configData.LaneConfigList[signOnLane]) )
					{
						signOnTimer = new System.Threading.Timer( new TimerCallback( SignOnTimeout ), null, configData.SignOnTimeout, Timeout.Infinite );
						myPsx.ActiveHostName = (string)configData.LaneConfigList[signOnLane];
						success = true;
					}
					signOnLane++;
				}
				if( !success )
				{
#if( ! WINCE ) 
				// If no computers from the config file (<HOST> tag) was successful
				// try the computer we are running LaunchPad on.
				String ComputerName = configData.ComputerName;
					CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState::ProcessSignOn() TB ATTEMPT Id = [" + id + "] Lane = [" + ComputerName + "]" ); 
				if( SendSignOn( ComputerName ) )
				{
					signOnTimer = new System.Threading.Timer( new TimerCallback( SignOnTimeout ), null, configData.SignOnTimeout, Timeout.Infinite );
					myPsx.ActiveHostName = ComputerName;
				    success = true;
				}
#endif
				}

				// If all SendSignon attempts failed 
				if	( !success )
				{
					// And we haven't checked the registry yet 
					if ( configData.SignOnTBFirst == true )
					{ 
						success = SignOnViaRegistry( configData, id, password );

						//397472, 397387
						if( configData.BiometricDeviceConfigured && success && !myPsx.Biometric.KeyedIn )
						{																
							string scanSound = configData.AudioPath + "\\ValidBiometric.wav";
							if (scanSound.Length > 0 && !myPsx.Biometric.KeyedIn )
							{
								CmDataCapture.Capture( CmDataCapture.MaskInfo, "ProcessSignOn():SignOnViaRegistry played validBiometric.wav ");
								myPsx.PlaySound(scanSound);
							}	
					
						}	
						if( configData.BiometricDeviceConfigured && null != myPsx.Biometric && success && myPsx.Biometric.KeyedIn )
							myPsx.Biometric.KeyedIn = false;  //395787
					}
				}

				if ( !success )
				{
					CmDataCapture.Capture( CmDataCapture.MaskError, "ProcessSignOn(): Failed to connect to a lane");

					if( signOnTimer != null )
					{
						signOnTimer.Dispose();
						signOnTimer = null;
					}

					//397472
					if( configData.BiometricDeviceConfigured && null != myPsx.Biometric && myPsx.Biometric.KeyedIn )
						myPsx.Biometric.KeyedIn = false;  //395787

					ResetSignOnContexts( true );
					myPsx.SetContext( configData.AlphaNumericKeyboard ? "EnterAlphaNumericID" : "EnterID", SharedPsx.displayLPStandard, false );
				}

				if( configData.BiometricDeviceConfigured && success )
				{																
					string scanSound = configData.AudioPath + "\\ValidBiometric.wma";
					if (scanSound.Length > 0 && !myPsx.Biometric.KeyedIn )
					{
						myPsx.PlaySound(scanSound);
					}					
				}				
			}
			catch (Exception e)
			{
				//397472
				if( ConfigData.GetConfigData().BiometricDeviceConfigured && null != myPsx.Biometric && myPsx.Biometric.KeyedIn )
					myPsx.Biometric.KeyedIn = false;  //395787

				// Log exception...
				CmDataCapture.Capture( CmDataCapture.MaskError, "ProcessSignOn():EXCEPTION " + e.Message);
			}
            //catch
            //{
            //    //397472
            //    if( ConfigData.GetConfigData().BiometricDeviceConfigured && null != myPsx.Biometric && myPsx.Biometric.KeyedIn )
            //        myPsx.Biometric.KeyedIn = false;  //395787

            //    // Log exception...
            //    CmDataCapture.Capture( CmDataCapture.MaskError, "ProcessSignOn():EXCEPTION caught");
            //}
		}

		/// <summary>
		/// Send signon request to lane.
		/// </summary>		
		public bool SendSignOn(string host)
		{
			bool success = false;
			try
			{
				if( myPsx != null && PSXConnectRemote(host))
				{
					/*string param = "operation=sign-on;UserId=";
					param += myPsx.GetConfigProperty( "Echo", contextEnterId, Psx.PropertyText );
					param += ";UserPwd=";
					param += myPsx.GetConfigProperty( "Echo", contextEnterPassword, Psx.PropertyText );                    
					*/
                    
					ConfigData configData = ConfigData.GetConfigData();
					if( configData.SynchronizeTimeServer )
					{
						Param += ";set-rap-time=true";
					}

					CmDataCapture.Capture( CmDataCapture.MaskError, "SendSignOn(): CALL GenerateEvent()");
					string connection = null;
					try
					{
						connection = myPsx.RemoteConnections[0];
					}
					catch ( Exception e )
					{
						CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState::SendSignOn() accessing myPsx.RemoteConnections[0] - Exception: " + e.Message );                
						throw;
					}

					myPsx.GenerateEvent( connection, "", "", ConfigData.eventRap, ( object ) Param );

					CmDataCapture.Capture( CmDataCapture.MaskError, "SendSignOn(): SUCCESSFUL GenerateEvent()");
					success = true;
				}
				else if ((myPsx != null) && (myPsx.RemoteConnections != null) && (myPsx.RemoteConnections.Length > 0 ))
				{
					CmDataCapture.Capture( CmDataCapture.MaskError, "SendSignOn(): CALL DisconnectRemote()");
					myPsx.DisconnectRemote();
					myPsx.ActiveHostName = null;
				}
			}
			catch( Exception caught )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "SendSignOn(): FAILED GenerateEvent()");
				CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState::SendSignOn() - Exception: " + caught.Message );                
			}

			return success;
		}

		/// <summary>
		/// Gets the RapSingle context name.
		/// </summary>
		public string RapSingleContext
		{
			get
			{
				ConfigData configData = ConfigData.GetConfigData();
                
				string context = "RAPSingle8";
#if( WINCE )                
				if( configData.LaneConfigList.Count <= 5 )
				{
					context = "RAPSingle5";
				}
#else
                if( configData.LaneConfigList.Count <= 4 )
                {
                    context = "RAPSingle4";
                }
                else if( configData.LaneConfigList.Count <= 6 )
                {
                    context = "RAPSingle6";
                }
#endif  
				return context;
			}
		}
			
		/// <summary>
		/// Try to connect our Psx to it's corresponding FastLane Psx.
		/// </summary>
		public bool PSXConnectRemote(string host)
		{
			ConfigData configData = ConfigData.GetConfigData();
            
			try
			{
				if ( (host == null) || (host == "") )
				{
					CmDataCapture.Capture( CmDataCapture.MaskExtensive, "NO CALL TO VerifyRemoteServer because host= EMPTYSTRING");
				}
				else
				{
					// Attempt to connect to Psx server //

					string currentConnection = "";
					if ((myPsx.RemoteConnections != null) && (myPsx.RemoteConnections.Length > 0 ))
					{
						currentConnection = myPsx.RemoteConnections[0];
					}
					if (currentConnection.Length == 0)
					{

						string connectionString = "HandheldRAP=";
#if( WINCE )
						connectionString += "1;";
#else
						    connectionString += "0;";
#endif
						connectionString += "RapSingleSize=" + RapSingleContext;
						object param = connectionString;
						CmDataCapture.Capture( CmDataCapture.MaskExtensive, "CALL ConnectRemote");
						//myPsx.ConnectRemote( host, configData.FastlaneService, "LaunchPadNet." + configData.ComputerName, ref ( object ) param, (uint)configData.ConnectRemoteTimeout );
                        myPsx.ConnectRemote(host, configData.FastlaneService, "LaunchPadNet." + configData.ComputerName, ref param, (uint)configData.ConnectRemoteTimeout);
						CmDataCapture.Capture( CmDataCapture.MaskError, "SUCCESSFULL: CALL ConnectRemote host=" + host + ";connectionName=LaunchPadNet." + configData.ComputerName);
					}
					return true;
				}					
			}
			catch( Exception caught )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "FAILED: CALL ConnectRemote host=" + host + ";connectionName=LaunchPadNet." + configData.ComputerName);
				CmDataCapture.Capture( CmDataCapture.MaskError, "ConnectThread::ThreadMethod() - Exception: " + caught.Message );
			}

			return false;
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
			CmDataCapture.Capture( CmDataCapture.MaskInfo, "SignOnState State_PSXEvent :" + e.ContextName + ":" + e.ControlName + ":" + e.EventName);
            
			if( e.EventName.Equals( Psx.EventClick ) )
			{
				if( e.ContextName.Equals( contextEnterPassword ) && ( e.ControlName.Equals( "KeyBoardP4" ) || e.ControlName.Equals( "AlphaNumKey3LaunchPad" ) ) )
				{
                    
					myPsx.SetControlProperty( Psx.ControlDisplay, Psx.PropertyUserInput, false );
					myPsx.SetControlProperty( "Leadthru", Psx.PropertyText, myPsx.GetString( "LaunchPadNet_LoggingInLeadthru" ) );
                    
					EnterPassword();
                    
					handled = true;
				}
				else if ( e.ContextName.Equals( contextEnterId ) && ( e.ControlName.Equals( "ButtonCommand0" ) ) )
				{
					if ( !ConfigData.HasKeyLockHW )
					{
						myPsx.BackwardsFlag = !myPsx.BackwardsFlag;	// Sync the hardware keypress with the GO BACK button
					}
					// The operator pressed "GoBack" on the enter ID screen.
					// Redisplay the original main context (RapContext, FastlaneContext, etc.)
                    if (!myPsx.SignedOn && myPsx.IsMainExecutableProcessAlive())
                    {
                        myPsx.DisplayLaunchPadNet(false);
                    }
                    else
                    {
                        myPsx.SetConfigProperty("ButtonCommand0", e.ContextName, Psx.PropertyState, Psx.State.Disabled);
                    }
                    SetShiftAndSymbolProperties(false);
                    PopulateKeyboard(false, false);
					handled = true;
				}
				else if (( e.ContextName.Equals(contextEnterId) || e.ContextName.Equals(contextEnterPassword))
                    && (e.ControlName.Equals("ShiftKey")))
				{                   
                    bool uppercase = Psx.State.Pushed == (Psx.State)Convert.ToInt32(myPsx.GetControlProperty("ShiftKey", Psx.PropertyState));
                    PopulateKeyboard(uppercase, false);
					handled = true;
				}
                else if ((e.ContextName.Equals(contextEnterId) || e.ContextName.Equals(contextEnterPassword))
                    && (e.ControlName.Equals("SymbolsKey")))
                {
                    String buttonText = (string)myPsx.GetControlProperty("SymbolsKey", Psx.PropertyText);
                    String symbolTextOff = myPsx.GetString("SymbolButtonTextOff");
                    
                    bool symbolsOn = false;
                    if(buttonText.Equals(symbolTextOff))
                    {
                     symbolsOn =  true;
                    }
                    SetShiftAndSymbolProperties(symbolsOn);
                    PopulateKeyboard(false, symbolsOn);
                    handled = true;
                }
				else if( e.ContextName.Equals(contextEnterPassword) && e.ControlName.Equals("ButtonCommand0") )
				{
					ConfigData configData = ConfigData.GetConfigData();
					if( configData.BiometricDeviceConfigured )
					{							
						if( myPsx.Biometric.GetFingerPrint() )
						{																							
							myPsx.SetConfigProperty( "Leadthru", contextEnterId, Psx.PropertyTextFormat, myPsx.GetString( "LaunchPadNet_EnterIDFpLeadthru" ) );				
							myPsx.Biometric.BiometricEvent += new BiometricDelegate(Biometric_BiometricEvent);							
						}
						else
						{
							myPsx.SetConfigProperty( "Leadthru", contextEnterId, Psx.PropertyTextFormat, myPsx.GetString( "LaunchPadNet_EnterIDLeadthru" ) );
							CmDataCapture.Capture( CmDataCapture.MaskError, "GetFingerPrint() failed." );
						}
					}		
					myPsx.SetConfigProperty( "Echo", contextEnterId, Psx.PropertyTextFormat, String.Empty );
				}
			}
			else if( e.EventName.Equals( ConfigData.eventRap ) )
			{
				string operation = ConfigData.ExtractRapEventData( "operation", e.Param.ToString() );
				if( operation != null && operation.Equals( "sign-on" ) )
				{
					string signOnRes = ConfigData.ExtractRapEventData( "success", e.Param.ToString() );
					if( signOnRes != null )
					{
						if( signOnTimer != null )
						{
							signOnTimer.Dispose();
							signOnTimer = null;
						}
                        
						uint signOnResMask = Convert.ToUInt32( signOnRes );
						if( signOnResMask > 0 )
						{
							ConfigData configData = ConfigData.GetConfigData();
							if( configData.SynchronizeTimeServer )
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
							myPsx.OperatorId = (string)myPsx.GetConfigProperty( "Echo", contextEnterId, Psx.PropertyText );
							myPsx.OperatorRights = signOnResMask;
							myPsx.LogToReportGenerator( true );
							myPsx.SignedOn = true;

							//397387
							if( ConfigData.GetConfigData().BiometricDeviceConfigured && !myPsx.Biometric.KeyedIn )
							{																
								string scanSound = ConfigData.GetConfigData().AudioPath + "\\ValidBiometric.wav";
								if (scanSound.Length > 0 && !myPsx.Biometric.KeyedIn )
								{
									CmDataCapture.Capture( CmDataCapture.MaskInfo, "state_psxevent played validBiometric.wav ");
									myPsx.PlaySound(scanSound);
								}	
					
							}	
							if( ConfigData.GetConfigData().BiometricDeviceConfigured && null != myPsx.Biometric && myPsx.Biometric.KeyedIn )
								myPsx.Biometric.KeyedIn = false;  //395787

							ResetSignOnContexts( false );
						}
						else
						{ 
							if( ConfigData.GetConfigData().BiometricDeviceConfigured && null != myPsx.Biometric && myPsx.Biometric.KeyedIn )
								myPsx.Biometric.KeyedIn = false;  //395787

							ResetSignOnContexts( true );
							myPsx.SetContext( contextEnterId, SharedPsx.displayLPStandard, false );
						}
						if ((myPsx != null) && (myPsx.RemoteConnections != null) && (myPsx.RemoteConnections.Length > 0 ))
						{
							CmDataCapture.Capture( CmDataCapture.MaskError, "State_PsxEvent(): CALL DisconnectRemote()");
							myPsx.DisconnectRemote();
							myPsx.ActiveHostName = null;
						}
					} 
               
					handled = true;
				}
			}
			else if( e.EventName.Equals( Psx.EventChangeContext ) )
			{
				ConfigData configData = ConfigData.GetConfigData();
				if( configData.AlphaNumericKeyboard )
				{
                    if (e.ContextName.Equals(contextEnterId) || e.ContextName.Equals(contextEnterPassword))
					{
                        SetShiftAndSymbolProperties(false);
						PopulateKeyboard( false, false );
						handled = true;
					}
				}

				// Clear out the password field each time the Enter Id context is displayed (this includes when
				// the user presses "go back" from the password screen)
				if ( e.ContextName.Equals( contextEnterId ) )
				{
					myPsx.SetConfigProperty( "Echo", contextEnterPassword, Psx.PropertyTextFormat, String.Empty );
					handled = true;
				}				
				// 418796 - G2_389 - Unfunctional "Go back" button in LaunchPadNet.
				myPsx.SetConfigProperty( "ButtonCommand0", e.ContextName, Psx.PropertyState, (myPsx.IsMainExecutableProcessAlive() ? Psx.State.Normal : Psx.State.Disabled));
				//disable fp scan in enterpassword screen
				if ( e.ContextName.Equals( contextEnterPassword ) )
				{
					if( configData.BiometricDeviceConfigured && null != myPsx.Biometric )
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
			else if( e.EventName.Equals( Psx.EventKeyDown ) )
			{
				if( Keys.Enter == Psx.ObjectToKeys( e.Param ) )
				{
					ConfigData configData = ConfigData.GetConfigData();
					CmDataCapture.Capture( CmDataCapture.MaskError, "Enter key detected");

					if( Psx.State.Normal == (Psx.State)Convert.ToInt32( myPsx.GetControlConfigProperty( configData.AlphaNumericKeyboard ? "AlphaNumKey3LaunchPad" : "KeyBoardP4", Psx.PropertyState ) ) )
					{
						if( e.ContextName.Equals( contextEnterId ) )
						{
							myPsx.SetContext( contextEnterPassword, SharedPsx.displayLPStandard, false );
							handled = true;
						}
						else if( e.ContextName.Equals( contextEnterPassword ) )
						{
							EnterPassword();
							handled = true;
						}					
					}
					else
					{
						CmDataCapture.Capture( CmDataCapture.MaskError, "Enter - ignored disabled");
					}
				}						
			}
#endif
			
			return handled;
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
                myPsx.SetControlProperty("SymbolsKey", Psx.PropertyText, myPsx.GetString("SymbolButtonTextOn"));
                myPsx.SetControlProperty("ShiftKey", Psx.PropertyState, Psx.State.Disabled);
            }
            else
            {
                myPsx.SetControlProperty("SymbolsKey", Psx.PropertyText, myPsx.GetString("SymbolButtonTextOff"));
                myPsx.SetControlProperty("ShiftKey", Psx.PropertyState, Psx.State.Normal);
            }
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
		protected override void State_ScannerEvent( object sender, ScannerEventArgs e )
		{
			Param = "operation=sign-on;scanner-data=" + e.ScanData + ";scanner-data-label=" + e.ScanDataLabel + ";scanner-type=" + e.ScanType.ToString();
			CmDataCapture.Capture( CmDataCapture.MaskError, "State_ScannerEvent - Param: " + Param );

			//myPsx.SetControlProperty( Psx.ControlDisplay, Psx.PropertyUserInput, false );
			//myPsx.SetControlProperty( "Leadthru", Psx.PropertyText, myPsx.GetString( "LaunchPadNet_LoggingInLeadthru" ) );

			//string id = (string) myPsx.GetConfigProperty( "Echo", contextEnterId, Psx.PropertyText );
			//string password = (string) myPsx.GetConfigProperty( "Echo", contextEnterPassword, Psx.PropertyText );
			
			if( null != myPsx.Biometric )
			{				
				if( myPsx.Biometric.BiometricDeviceConfigured )
				{
					bool bRet = myPsx.Biometric.Abort();
					if( false == bRet )
					{
						CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState::State_ScannerEvent() - Abort() failed." );
					}
				}
			}			

			// If we have the Employee ID configuration enabled, we need to perform the two step login process.
			if( ConfigData.GetConfigData().AllowEmplIdSignOn )
			{
                // clean the barcode of spaces and cr/lfs....
                string barcode = e.ScanDataLabel;
                char[] ctrlChars = new char[] { '\n', '\r', ' ' };
                barcode = barcode.Trim(ctrlChars);
                ConfigData.GetConfigData().SavedScannerEventArgs = new ScannerEventArgs(e.ScanData, barcode, e.ScanType);
                myPsx.SetConfigProperty("Echo", contextEnterId, Psx.PropertyTextFormat, barcode);
                myPsx.SetContext(myPsx.GetExpectedPasswordContext(), SharedPsx.displayLPStandard, false);
			}
			else
			{
				ProcessSignOn();
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
		/// None.
		/// </returns>		
		protected void Biometric_BiometricEvent(object sender, BiometricArgs e)
		{
			try
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
								myPsx.SetConfigProperty( "Leadthru", contextEnterId, 
									Psx.PropertyTextFormat, 
									myPsx.GetString( "EnterIDFpLeadthru" ) );				
							}
							break;
						case (int)BIOMETRIC_RC.RC_DEVICE_UNPLUGGED:
						case (int)BIOMETRIC_RC.RC_SERVER_NOTREADY:
							myPsx.Biometric.Abort();
							myPsx.SetConfigProperty( "Leadthru", contextEnterId, 
								Psx.PropertyTextFormat, 
								myPsx.GetString( "LaunchPadNet_EnterIDLeadthru" ) );				
							break;
					}
						break;
						// -TAR 397478 

					case (int)FT_STATUS_CODES.FT_WAITING_FOR_IMAGE:
						//CmDataCapture.Capture( CmDataCapture.MaskInfo, "SignOnState::Biometric_BiometricEvent() FT_WAITING_FOR_IMAGE" );
						break;
					case (int)FT_STATUS_CODES.FT_IMAGE_READY:
						//CmDataCapture.Capture( CmDataCapture.MaskInfo, "SignOnState::Biometric_BiometricEvent() FT_IMAGE_READY" );
						break;
					case (int)FT_STATUS_CODES.FT_FINGER_TOUCHING:
						myPsx.AutoSignOffCounter = 0;
						CmDataCapture.Capture( CmDataCapture.MaskInfo, "SignOnState::Biometric_BiometricEvent() FT_FINGER_TOUCHING" );
						break;
					case (int)FT_STATUS_CODES.FT_GET_FINGERPRINT_COMPLETE:
						CmDataCapture.Capture( CmDataCapture.MaskInfo, "SignOnState::Biometric_BiometricEvent() FT_GET_FINGERPRINT_COMPLETE" );						
						if ( (uint)BIOMETRIC_RC.RC_SUCCESS == e.Param1 )
						{
							CmDataCapture.Capture( CmDataCapture.MaskInfo, "SignOnState::Biometric_BiometricEvent() FT_GET_FINGERPRINT_COMPLETE success" );							
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
							//Tar 395471
							String usrData=Marshal.PtrToStringUni(e.IntPtrParam);
							myPsx.Biometric.UserID = "";
							myPsx.Biometric.UserPwd = "";
							int i = usrData.IndexOf("\\n");
							if (i>0)
							{
								myPsx.Biometric.UserID=usrData.Substring(0,i);
								if (usrData.Length > i+2)
									myPsx.Biometric.UserPwd=usrData.Substring(i+2);
							}
							CmDataCapture.Capture( CmDataCapture.MaskInfo, "SignOnState::Biometric_BiometricEvent() FT_VERIFY_USER_COMPLETE with id=" + myPsx.Biometric.UserID );
						
							Param = "operation[text]=sign-on;UserId[text]=";
							try
							{
								Param += myPsx.Biometric.UserID;		
								Param += ";UserPwd[masked]=";
								Param += myPsx.Biometric.UserPwd;                    
							}
							catch( PsxException caught )
							{
								CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState::Biometric_BiometricEvent() FT_VERIFY_USER_COMPLETE - PsxException: " + caught.Message );
							}
							catch( Exception caught )
							{
								CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState::Biometric_BiometricEvent() FT_VERIFY_USER_COMPLETE - Exception: " + caught.Message );
								throw caught;
							}

							ProcessSignOn();
							
							CmDataCapture.Capture( CmDataCapture.MaskInfo, "SignOnState::Biometric_BiometricEvent() FT_VERIFY_USER_COMPLETE.. success!!!" );							
						}
						else if ( (uint)BIOMETRIC_RC.RC_FP_NO_MATCH == e.Param1 )
						{
							CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState::Biometric_BiometricEvent(), No match found in fingerprint database." );							
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
			catch( Exception ex)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "Biometric Exception: " + ex.Message );
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
		protected void ResetSignOnContexts( bool retry )
		{
			try
			{
				string leadthruText = myPsx.GetExpectedEnterIDLeadthru();
				string leadthruPwdText = myPsx.GetExpectedPasswordLeadthru();

				// Reset the enterid/password contexts They can change during 
				// the lifetime of the class instance in some conditions
				// if handkey emplid sign on is disabled and the Main executable
				// is not running.
				var contextEnterId = myPsx.GetExpectedEnterIDContext();
				var contextEnterPassword = myPsx.GetExpectedPasswordContext();

				if( retry ) leadthruText = "Re" + leadthruText;
				
				myPsx.SetConfigProperty( "Leadthru", contextEnterId, Psx.PropertyTextFormat, myPsx.GetString(leadthruText) );
				myPsx.SetConfigProperty( "Leadthru", contextEnterPassword, Psx.PropertyTextFormat, myPsx.GetString(leadthruPwdText) );
				myPsx.SetConfigProperty( "Echo", contextEnterId, Psx.PropertyTextFormat, "" );
				myPsx.SetConfigProperty( "Echo", contextEnterPassword, Psx.PropertyTextFormat, "" );
				myPsx.SetControlProperty( Psx.ControlDisplay, Psx.PropertyUserInput, true );
				ConfigData.GetConfigData().SavedScannerEventArgs = null;

				if( null != myPsx.Biometric )
				{					
					if( myPsx.Biometric.BiometricDeviceConfigured )
					{						
						myPsx.SetConfigProperty( "Leadthru", contextEnterId, Psx.PropertyTextFormat, retry ? myPsx.GetString( "LaunchPadNet_ReEnterIDFpLeadthru" ) : myPsx.GetString( "EnterIDFpLeadthru" ) );

						if( retry )					
						{													
							bool bRet = myPsx.Biometric.GetFingerPrint();
							if( false == bRet )
							{
                                myPsx.SetConfigProperty("Leadthru", contextEnterId, Psx.PropertyTextFormat, retry ? myPsx.GetString("LaunchPadNet_ReEnterIDLeadthru") : myPsx.GetString("LaunchPadNet_EnterIDLeadthru"));
								CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState::ResetSignOnContexts() - GetFingerPrint() failed." );
							}
						}						
					}					
				}	
				else
				{
                    myPsx.SetConfigProperty("Leadthru", contextEnterId, Psx.PropertyTextFormat, retry ? myPsx.GetString("LaunchPadNet_ReEnterIDLeadthru") : myPsx.GetString("LaunchPadNet_EnterIDLeadthru"));
				}

                //Tar 394802
				if(null != myPsx.Biometric )
				{
					myPsx.Biometric.UserID = "";
					myPsx.Biometric.UserPwd = "";
				}

				myPsx.SetConfigProperty( "Echo", contextEnterId, Psx.PropertyTextFormat, "" );
				myPsx.SetConfigProperty( "Echo", contextEnterPassword, Psx.PropertyTextFormat, "" );
				myPsx.SetControlProperty( Psx.ControlDisplay, Psx.PropertyUserInput, true );
			}
			catch( Exception caught )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "ResetSignOnContexts::ClearEditFields() - Exception: " + caught.Message );
			}
		}

		/// <summary>
		/// Packages UserId and UserPwd for signon.
		/// </summary>		
        public void EnterPassword(string biometData="")
		{			
			if( null != myPsx.Biometric && myPsx.Biometric.BiometricDeviceConfigured )
			{				
				myPsx.Biometric.KeyedIn = true;			
			}
		
			Param = "operation[text]=sign-on;UserId[text]=";
			try
			{
                string tempid = (string)myPsx.GetConfigProperty("Echo", contextEnterId, Psx.PropertyText);
                string temppw = (string)myPsx.GetConfigProperty("Echo", contextEnterPassword, Psx.PropertyText);
                bool encodeOperatorID = ConfigData.GetConfigData().EncodeOperatorIDPW;
                Param += encodeOperatorID ? Base64EncodeWithDefaultPrefix(tempid) : tempid;
				Param += ";UserPwd[masked]=";
				
				// If there was a scan event in the login process and 
				// the text has not been tampered, consider it scanned
				ConfigData configData = ConfigData.GetConfigData();
				if( configData.AllowEmplIdSignOn )
				{
					ScannerEventArgs scanData = configData.SavedScannerEventArgs;
					bool idScanned = (scanData != null) && (scanData.ScanDataLabel == tempid);
					
					// use original implementation whenever possible
					if( !idScanned && (biometData.Length == 0) )
					{
						Param += encodeOperatorID ? Base64EncodeWithDefaultPrefix(temppw) : temppw;
					}
					else
					{
						SignOnBuilder sob = new SignOnBuilder(tempid, encodeOperatorID ? Base64EncodeWithDefaultPrefix(temppw) : temppw, idScanned, biometData);
						Param += sob.ToString();
					}
				}
				else
				{
					// original implementation
					Param += encodeOperatorID ? Base64EncodeWithDefaultPrefix(temppw) : temppw;
				}
			}
			catch( PsxException caught )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState::EnterPassword() - PsxException: " + caught.Message );
			}
			catch( Exception caught )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState::EnterPassword() - Exception: " + caught.Message );
				throw caught;
			}
			ProcessSignOn();
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
                CmDataCapture.Capture(CmDataCapture.MaskApi, String.Format("+SignOnState::PopulateKeyboard shift:{0}, symbols:{1}", shiftOn, symbolsOn));
                myPsx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, false);

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
                    symbols = myPsx.GetCustomDataVariable(customDataText, controlname, contextName);
                    if ( 0 == symbols.Length || (!shiftOn && !symbolsOn))
                    {
                        symbols = myPsx.GetCustomDataVariable("Text", controlname, contextName);
                    }
                    symbolButtonText = symbols.Split(' ');
                    symbolButtonData = symbols.Split(' ');

                    myPsx.SetControlConfigProperty(controlname, Psx.PropertyButtonCount, symbolButtonText.Length);

                    //delete[] param;
                    param = null;
                    param = new object[1] { (object[])symbolButtonText };
                    myPsx.SendControlCommand(controlname, Psx.CommandSetButtonTextList, 1, param);

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
                    myPsx.SendControlCommand(controlname, Psx.CommandSetButtonDataList, 1, param);
                }

                myPsx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, true);
            }
            catch (PsxException caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "SignOnState::PopulateKeyboard() - PsxException: " + caught.Message);
            }
            CmDataCapture.Capture(CmDataCapture.MaskApi, "-SignOnState::PopulateKeyboard");
        }

		/// <summary>
		/// Timeout callback for sign on.
		/// </summary>
		private void SignOnTimeout( object state )
		{
			string id = (string) myPsx.GetConfigProperty( "Echo", contextEnterId, Psx.PropertyText );
			string password = (string) myPsx.GetConfigProperty( "Echo", contextEnterPassword, Psx.PropertyText );

			if( signOnTimer != null )
			{
				signOnTimer.Dispose();
				signOnTimer = null;
			}
			CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnTimeout()");

			ResetSignOnContexts( false );
			if ((myPsx != null) && (myPsx.RemoteConnections != null) && (myPsx.RemoteConnections.Length > 0 ))
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnTimeout(): CALL DisconnectRemote()");
				myPsx.DisconnectRemote();
				myPsx.ActiveHostName = null;
			}

			bool success = false;
			// If we have not tried to signon via the registry
			// let's try that now.  
			
			ConfigData configData = ConfigData.GetConfigData();
			if	( configData.SignOnTBFirst == true )
			{
				// Attempt to login using the Registry ID/Passwords...
				success = SignOnViaRegistry( configData, id, password );
			}

			if	( success == false )
			{
				myPsx.SetContext( configData.AlphaNumericKeyboard ? "EnterAlphaNumericID" : "EnterID", SharedPsx.displayLPStandard, false );
			}
		}

		/// <summary>
		/// Attempt to signon via registry
		/// </summary>
		/// <param name="configData">ConfigData instance</param>
		/// <param name="id">User Id</param>
		/// <param name="password">User Password</param>
		/// <returns></returns>
		
		private bool SignOnViaRegistry( ConfigData configData, string id, string password )
		{
			bool success = false;

			CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState::SignOnViaRegistry() REGISTRY ATTEMPT Id = [" + id + "]" ); 

			if ( configData.ValidateOperatorRegistry( id, password, ConfigData.SWUSER ) )
			{
				success = true;
				if( signOnTimer != null )
				{
					signOnTimer.Dispose();
					signOnTimer = null;
				}

				//myPsx.OperatorId = (string)myPsx.GetConfigProperty( "Echo", contextEnterId, Psx.PropertyText );
                                myPsx.OperatorId = id;                                   //TAR 356775
                                myPsx.OperatorRights = 1;				// Currently OperatorRights is not implemented.
				myPsx.LogToReportGenerator( true );
				myPsx.SignedOn = true;
			}

			CmDataCapture.Capture( CmDataCapture.MaskInfo, "SignOnViaRegistry: " + ( success ? "Success" : "Failed" ) ); 

			return success;
		}

		/// <summary>
		/// Return true if one or more lanes are configured in LaunchPad.xml
		/// </summary>
		/// <param name="configData"></param>
		/// <returns></returns>
		private bool LanesConfigured( ConfigData configData )
		{
			bool success = false;
			int signOnLane = 0;
			while( !success && signOnLane < configData.LaneConfigList.Count )
			{
				string Lane = ( string ) configData.LaneConfigList[signOnLane];
				if	( ( Lane != null ) && ( Lane != "" ) )
				{
					success = true;
					break;
				}
				signOnLane++;
			}

			if( !success ) //395483
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "SignonState::LanesConfigured - No Lanes in LaunchPad.xml"  ); 
			}

			return success;
		}

		private void ShiftButtonList( string strControl, object[] objDataList, string[] strTextList)
		{
			try
			{
				object[] param = new object[2];
				for( int n = 0; n < objDataList.Length; n++ )
				{
					param[0] = n;
					param[1] = objDataList[n];
					myPsx.SendControlCommand( strControl, Psx.CommandSetButtonData, 2, param );
				}
				for( int n = 0; n < strTextList.Length; n++ )
				{
					param[0] = n;
					param[1] = strTextList[n];
					myPsx.SendControlCommand( strControl, Psx.CommandSetButtonText, 2, param );
				}
			}
			catch( Exception caught )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "ShiftButtonList() - Exception: " + caught.Message );
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
		/// Gets the RapSingle context name.
		/// </summary>
		public string Param
		{
			get
			{
				return param;    
			}
			set
			{
				param = value;
			}
		}

		/// <summary>
		/// Parameter string sent to validate sign on.
		/// </summary>	
		string param = null;

        /// <summary>
        /// EnterId Psx Context name.
        /// </summary>		
        protected readonly string contextEnterId;

        /// <summary>
        /// EnterPassword Psx Context name.
        /// </summary>		
        protected readonly string contextEnterPassword;

        /// <summary>
        /// Timer that waits for sign on results.
        /// </summary>
		protected System.Threading.Timer signOnTimer = null;
	}
}
