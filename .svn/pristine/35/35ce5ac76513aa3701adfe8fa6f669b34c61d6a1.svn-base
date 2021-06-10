using System;
using System.Threading;
using System.Runtime.InteropServices;
using PsxNet;
using RPSWNET;
using CommonLibsNet;
using System.Diagnostics;
using System.IO;

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
			ConfigDataLaunchPad configData = ConfigDataLaunchPad.GetConfigData();
			configData.SavedScannerEventArgs = null;
			ResetSignOnContexts(false);
			
			if( configData.AlphaNumericKeyboard )
			{
				KeyboardUppercase( configData.EnterIdUppercase );
			}

			if( configData.BiometricDeviceConfigured )
			{							
				if( myPsx.Biometric.GetFingerPrint() )
				{																							
					myPsx.SetConfigProperty( "Leadthru", contextEnterId, Psx.PropertyTextFormat, myPsx.GetString( "EnterIDFpLeadthru" ) );				
					myPsx.Biometric.BiometricEvent += new BiometricDelegate(Biometric_BiometricEvent);							
				}
				else
				{
					myPsx.SetConfigProperty( "Leadthru", contextEnterId, Psx.PropertyTextFormat, myPsx.GetString( "EnterIDLeadthru" ) );				
					CmDataCapture.Capture( CmDataCapture.MaskError, "GetFingerPrint() failed." );
				}
			}						
			
			if( null != myPsx.Scanner && myPsx.DisplayingLaunchpad )
			{
				if( myPsx.IsScannerMsgTargetRunning )
				{
					myPsx.RequestAccessToScanner();
				}
				else
				{
					myPsx.Scanner.Enable( String.Empty );
				}
			}            
		}

		/// <summary>
		/// The class constructor.
		/// </summary>
		/// <param name="paramPsx">
		/// SharedPsx instance used to connect.
		/// </param>
		/// /// <param name="ANKIdOverride">
		/// Override EnterId Context
		/// </param>
		/// /// <param name="ANKPasswordOverride">
		/// Override EnterId Password
		/// </param>
		public SignOnState( SharedPsx paramPsx, string ANKIdOverride, string ANKPasswordOverride) : base( paramPsx )
		{	
			contextEnterId = ANKIdOverride;
			contextEnterPassword = ANKPasswordOverride;
			ConfigDataLaunchPad.GetConfigData().SavedScannerEventArgs = null;
			ResetSignOnContexts(false);
			
			if( null != myPsx.Scanner && myPsx.DisplayingLaunchpad )
			{
				if( myPsx.IsScannerMsgTargetRunning )
				{
					myPsx.RequestAccessToScanner();
				}
				else
				{
					myPsx.Scanner.Enable( String.Empty );
				}
			}            
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
			// let scanner sharing application know we're letting go of the scanner, in case it needs it
			if( myPsx.Scanner is ScannerShared )
			{
				string msg = ConfigData.eventRapOperation + "=" + ScannerGeneric.RemoteMessages.ScannerAvailable + ";";
				myPsx.SendProcessMessage(myPsx.ScannerMsgTarget, msg);
			}

			if( signOnTimer != null )
			{
				signOnTimer.Dispose();
				signOnTimer = null;
			}
			myPsx.ClearScannerMsgTimer();
			myPsx.waitingForScannerMsgResponse = false;
			ResetSignOnContexts( false );

			if( null != myPsx.Biometric )
			{						
				if( myPsx.Biometric.BiometricDeviceConfigured )
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
				ConfigDataLaunchPad configData = ConfigDataLaunchPad.GetConfigData();

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
					success = SignOnViaRegistry( id, password );
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
						success = SignOnViaRegistry( id, password );
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
					myPsx.SetContext(myPsx.GetExpectedEnterIDContext(), SharedPsx.displayLPStandard, false);
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
				if( ConfigDataLaunchPad.GetConfigData().BiometricDeviceConfigured && null != myPsx.Biometric && myPsx.Biometric.KeyedIn )
					myPsx.Biometric.KeyedIn = false;  //395787

				// Log exception...
				CmDataCapture.Capture( CmDataCapture.MaskError, "ProcessSignOn():EXCEPTION " + e.Message);
			}
			catch
			{
				//397472
				if( ConfigDataLaunchPad.GetConfigData().BiometricDeviceConfigured && null != myPsx.Biometric && myPsx.Biometric.KeyedIn )
					myPsx.Biometric.KeyedIn = false;  //395787

				// Log exception...
				CmDataCapture.Capture( CmDataCapture.MaskError, "ProcessSignOn():EXCEPTION caught");
			}
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
					ConfigData configData = ConfigDataLaunchPad.GetConfigData();
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
				ConfigData configData = ConfigDataLaunchPad.GetConfigData();
                
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
			ConfigData configData = ConfigDataLaunchPad.GetConfigData();
            
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
						myPsx.ConnectRemote( host, configData.FastlaneService, "LaunchPadNet." + configData.ComputerName, ref ( object ) param, (uint)configData.ConnectRemoteTimeout );
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
					myPsx.SetControlProperty( "Leadthru", Psx.PropertyText, myPsx.GetString( "LoggingInLeadthru" ) );
                    
					EnterPassword("");
                    
					handled = true;
				}
				else if ( e.ContextName.Equals( contextEnterId ) && ( e.ControlName.Equals( "ButtonCommand0" ) ) )
				{
					if ( !ConfigDataLaunchPad.HasKeyLockHW )
					{
						myPsx.BackwardsFlag = !myPsx.BackwardsFlag;	// Sync the hardware keypress with the GO BACK button
					}
					// The operator pressed "GoBack" on the enter ID screen.
					// Redisplay the original main context (RapContext, FastlaneContext, etc.)
					if (!myPsx.SignedOn && myPsx.IsMainExecutableProcessAlive())
					{
						myPsx.DisplayLaunchPadNet( false );
					}
					handled = true;
				}
				else if (( e.ContextName.Equals( contextEnterId ) || e.ContextName.Equals( contextEnterPassword ) ) && ( e.ControlName.Equals( "ShiftKey" ) ) )
				{
					bool uppercase = Psx.State.Pushed == (Psx.State)Convert.ToInt32( myPsx.GetControlProperty( "ShiftKey", Psx.PropertyState ));
					KeyboardUppercase( uppercase );
					handled = true;
				}
				else if( e.ContextName.Equals( contextEnterPassword ) && e.ControlName.Equals("ButtonCommand0") )
				{
					ConfigData configData = ConfigDataLaunchPad.GetConfigData();
					if( configData.BiometricDeviceConfigured )
					{							
						if( myPsx.Biometric.GetFingerPrint() )
						{																							
							myPsx.SetConfigProperty( "Leadthru", contextEnterId, Psx.PropertyTextFormat, myPsx.GetString( "EnterIDFpLeadthru" ) );				
							myPsx.Biometric.BiometricEvent += new BiometricDelegate(Biometric_BiometricEvent);							
						}
						else
						{
							myPsx.SetConfigProperty( "Leadthru", contextEnterId, Psx.PropertyTextFormat, myPsx.GetString( "EnterIDLeadthru" ) );
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
							ConfigData configData = ConfigDataLaunchPad.GetConfigData();
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
							if( ConfigDataLaunchPad.GetConfigData().BiometricDeviceConfigured && !myPsx.Biometric.KeyedIn )
							{																
								string scanSound = ConfigDataLaunchPad.GetConfigData().AudioPath + "\\ValidBiometric.wav";
								if (scanSound.Length > 0 && !myPsx.Biometric.KeyedIn )
								{
									CmDataCapture.Capture( CmDataCapture.MaskInfo, "state_psxevent played validBiometric.wav ");
									myPsx.PlaySound(scanSound);
								}	
					
							}	
							if( ConfigDataLaunchPad.GetConfigData().BiometricDeviceConfigured && null != myPsx.Biometric && myPsx.Biometric.KeyedIn )
								myPsx.Biometric.KeyedIn = false;  //395787

							ResetSignOnContexts( false );
						}
						else
						{                      
							if( ConfigDataLaunchPad.GetConfigData().BiometricDeviceConfigured && null != myPsx.Biometric && myPsx.Biometric.KeyedIn )
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
				ConfigData configData = ConfigDataLaunchPad.GetConfigData();
				if( configData.AlphaNumericKeyboard )
				{
					if( e.ContextName.Equals( contextEnterId ) )
					{
						KeyboardUppercase( configData.EnterIdUppercase );
						handled = true;
					}
					else if( e.ContextName.Equals( contextEnterPassword ) )
					{
						KeyboardUppercase( configData.EnterPasswordUppercase );
						handled = true;
					}
				}

				// Clear out the password field each time the Enter Id context is displayed (this includes when
				// the user presses "go back" from the password screen)
				if ( e.ContextName.Equals( contextEnterId ) )
				{
					// Enable the scanner for the EnterID portion of the 
					if( null != myPsx.Scanner && myPsx.DisplayingLaunchpad ) 
					{
						if( myPsx.IsScannerMsgTargetRunning )
						{
							myPsx.RequestAccessToScanner();
						}
						else
						{
							myPsx.Scanner.Enable( String.Empty );
						}
					}
					//PatchG B337 - myPsx.SetConfigProperty( "Echo", contextEnterId, Psx.PropertyTextFormat, String.Empty );
					myPsx.SetConfigProperty( "Echo", contextEnterPassword, Psx.PropertyTextFormat, String.Empty );
					handled = true;
				}
				// 418796 - G2_389 - Unfunctional "Go back" button in LaunchPadNet.
				myPsx.SetConfigProperty( "ButtonCommand0", e.ContextName, Psx.PropertyState, (myPsx.IsMainExecutableProcessAlive() ? Psx.State.Normal : Psx.State.Disabled));
				if( e.ContextName.Equals( contextEnterPassword ) )
				{
					if( configData.AllowEmplIdSignOn && (null != myPsx.Scanner) )
					{
						myPsx.Scanner.Disable();
						if( myPsx.Scanner is ScannerShared )
						{
							string msg = ConfigData.eventRapOperation + "=" + ScannerGeneric.RemoteMessages.ScannerAvailable + ";";
							myPsx.SendProcessMessage(myPsx.ScannerMsgTarget, msg);
						}
					}
					//disable fp scan in enterpassword screen
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
					ConfigData configData = ConfigDataLaunchPad.GetConfigData();
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
							EnterPassword("");
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
			CmDataCapture.Capture( CmDataCapture.MaskInfo, "State_ScannerEvent - Param: " + Param );
			ConfigData configData = ConfigDataLaunchPad.GetConfigData();

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
			if( configData.AllowEmplIdSignOn )
			{
                // clean the barcode of spaces and cr/lfs....
                string barcode = e.ScanDataLabel;
                char[] ctrlChars = new char[] { '\n', '\r', ' ' };
                barcode = barcode.Trim(ctrlChars);
                configData.SavedScannerEventArgs = new ScannerEventArgs(e.ScanData, barcode, e.ScanType);
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
								myPsx.GetString( "EnterIDLeadthru" ) );				
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
				contextEnterId = myPsx.GetExpectedEnterIDContext();
				contextEnterPassword = myPsx.GetExpectedPasswordContext();

				if( retry ) leadthruText = "Re" + leadthruText;
				
				myPsx.SetConfigProperty( "Leadthru", contextEnterId, Psx.PropertyTextFormat, myPsx.GetString(leadthruText) );
				myPsx.SetConfigProperty( "Leadthru", contextEnterPassword, Psx.PropertyTextFormat, myPsx.GetString(leadthruPwdText) );
				myPsx.SetConfigProperty( "Echo", contextEnterId, Psx.PropertyTextFormat, "" );
				myPsx.SetConfigProperty( "Echo", contextEnterPassword, Psx.PropertyTextFormat, "" );
				myPsx.SetControlProperty( Psx.ControlDisplay, Psx.PropertyUserInput, true );
				ConfigDataLaunchPad.GetConfigData().SavedScannerEventArgs = null;

				if( null != myPsx.Biometric )
				{					
					if( myPsx.Biometric.BiometricDeviceConfigured )
					{						
						myPsx.SetConfigProperty( "Leadthru", contextEnterId, Psx.PropertyTextFormat, retry ? myPsx.GetString( "ReEnterIDFpLeadthru" ) : myPsx.GetString( "EnterIDFpLeadthru" ) );

						if( retry )					
						{													
							bool bRet = myPsx.Biometric.GetFingerPrint();
							if( false == bRet )
							{
								myPsx.SetConfigProperty( "Leadthru", contextEnterId, Psx.PropertyTextFormat, retry ? myPsx.GetString( "ReEnterIDLeadthru" ) : myPsx.GetString( "EnterIDLeadthru" ) );
								CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState::ResetSignOnContexts() - GetFingerPrint() failed." );
							}
						}						
					}					
				}	
                //Tar 394802
				if(null != myPsx.Biometric )
				{
					myPsx.Biometric.UserID = "";
					myPsx.Biometric.UserPwd = "";
				}
			}
			catch( Exception caught )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "ResetSignOnContexts::ClearEditFields() - Exception: " + caught.Message );
			}
		}

		/// <summary>
		/// Packages UserId and UserPwd for signon.
		/// </summary>		
		public void EnterPassword(string biometData)
		{
			if( null != myPsx.Biometric && myPsx.Biometric.BiometricDeviceConfigured )
			{				
				myPsx.Biometric.KeyedIn = true;			
			}

			Param = "operation[text]=sign-on;UserId[text]=";
			try
			{
				string userId = (string)myPsx.GetConfigProperty( "Echo", contextEnterId, Psx.PropertyText );
				string userPwd = (string)myPsx.GetConfigProperty( "Echo", contextEnterPassword, Psx.PropertyText );
				Param += userId;
				Param += ";UserPwd[masked]=";
				
				// If there was a scan event in the login process and 
				// the text has not been tampered, consider it scanned
				ConfigData configData = ConfigDataLaunchPad.GetConfigData();
				if( configData.AllowEmplIdSignOn )
				{
					ScannerEventArgs scanData = configData.SavedScannerEventArgs;
					bool idScanned = (scanData != null) && (scanData.ScanDataLabel == userId);
					
					// use original implementation whenever possible
					if( !idScanned && (biometData.Length == 0) )
					{
						Param += userPwd;
					}
					else
					{
						SignOnBuilder sob = new SignOnBuilder(userId, userPwd, idScanned, biometData);
						Param += sob.ToString();
					}
				}
				else
				{
					// original implementation
					Param += userPwd;
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
		/// Sets AlphaNumeric keyboard to Uppercase or Lowercase.
		/// </summary>
		/// <param name="uppercase">
		/// True if uppercase, false if lowercase.
		/// </param>				
		private void KeyboardUppercase( bool uppercase )
		{
			try
			{
				myPsx.SetControlProperty( Psx.ControlDisplay, Psx.PropertyRedraw, false );
				myPsx.SetControlProperty( "ShiftKey", Psx.PropertyState, uppercase ? Psx.State.Pushed : Psx.State.Normal );

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

				ShiftButtonList("AlphaNumP2", dataAlphaNumP2, textAlphaNumP2);
				ShiftButtonList("AlphaNumP3", dataAlphaNumP3, textAlphaNumP3);
				ShiftButtonList("AlphaNumP4", dataAlphaNumP4, textAlphaNumP4);

				myPsx.SetControlProperty( Psx.ControlDisplay, Psx.PropertyRedraw, true );
			}
			catch( PsxException caught )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState::KeyboardUppercase() - PsxException: " + caught.Message );
			}
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
			
			ConfigDataLaunchPad configData = ConfigDataLaunchPad.GetConfigData();
			if	( configData.SignOnTBFirst == true )
			{
				// Attempt to login using the Registry ID/Passwords...
				success = SignOnViaRegistry(id, password);
			}

			if	( success == false )
			{
				myPsx.SetContext(myPsx.GetExpectedEnterIDContext(), SharedPsx.displayLPStandard, false);
			}
		}

		/// <summary>
		/// Attempt to signon via registry
		/// </summary>
		/// <param name="id">User Id</param>
		/// <param name="password">User Password</param>
		/// <returns></returns>
		
		private bool SignOnViaRegistry(string id, string password)
		{
			bool success = false;

			CmDataCapture.Capture( CmDataCapture.MaskError, "SignOnState::SignOnViaRegistry() REGISTRY ATTEMPT Id = [" + id + "]" ); 

			if ( ConfigDataLaunchPad.GetConfigData().ValidateOperatorRegistry( id, password, ConfigDataLaunchPad.SWUSER ) )
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

			if( !success )
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
        protected string contextEnterId;

        /// <summary>
        /// EnterPassword Psx Context name.
        /// </summary>		
        protected string contextEnterPassword;

        /// <summary>
        /// Timer that waits for sign on results.
        /// </summary>
		protected System.Threading.Timer signOnTimer = null;
	}
}
