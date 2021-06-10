using System;
using System.Collections;
using System.Drawing;
using System.Threading;
using PsxNet;
using RPSWNET;
using System.Diagnostics;
using System.Text;
using CommonLibsNet;
using System.IO;
#if !WINCE  // JZB_In
	using OposKeylock_1_7_Lib;
    using System.Runtime.InteropServices;
#endif

namespace LaunchPadNet
{
	/// <summary>
	/// The shared Psx class.
	/// </summary>
	internal class SharedPsx : Psx
	{
		/// <summary>
		/// The class constructor.
		/// </summary>
		public SharedPsx()
		{
			myself = this;
			ConfigDataLaunchPad configData = ConfigDataLaunchPad.GetConfigData();
	
			try
			{
#if	( WINCE )
				DeleteUpdateFlagFile( configData.BinPath );
#endif

				Psx.SetEnvironmentVariable( "AudioPath", configData.AudioPath );
				Psx.SetEnvironmentVariable( "ConfigPath", configData.ConfigPath );
				Psx.SetEnvironmentVariable( "MediaPath", configData.MediaPath );

				if( !StartBiometric() )
				{
					CmDataCapture.Capture( CmDataCapture.MaskError, "SharedPsx::SharedPsx() - Biometric not configured." );
				}

				PsxEvent += new PsxEventHandler( SharedPsx_PsxEvent );
				CreateDisplay( new Rectangle( 0, 0, 0, 0 ) );
				SetControlProperty( Psx.ControlDisplay, Psx.PropertyCursor, configData.ShowCursor );
				Text = "LaunchPadNet";
				DisplayView = displayLPStandard;  
				string customFile = Path.ChangeExtension(configData.PsxConfigXML,"000");
				LoadConfigFile( configData.PsxConfigXML, customFile );
				SetContext( "StartupContext", SharedPsx.displayLPStandard, false );

				OperatorId = "";
				TerminalId = configData.TerminalId;
				InterfaceVersion = configData.InterfaceVersion;			// Rfc 343789
#if ( !WINCE )				
                autoSignOffSeconds = configData.AutoSignOffSeconds;

                if(autoSignOffSeconds > 0)
				{
					SetMouseKeyboardHook();
				}
#endif

#if	 ( WINCE )
				TopMost = true;
#endif
				if( scanner != null )
				{
					scanner.Dispose();
					scanner = null;
				}
				scanner = ScannerGeneric.CreateScanner(configData.ScannerClass, this, ConfigDataLaunchPad.GetConfigData());
                
				if( scanner != null )
				{
					CmDataCapture.Capture(CmDataCapture.MaskInfo, "SharedPsx::SharedPsx() - Created scanner using class " + configData.ScannerClass);
					if( !scanner.Start() )
					{
						CmDataCapture.Capture(CmDataCapture.MaskError, "SharedPsx::SharedPsx() - StartScannerThread failed");
					}
				}

				MyApplication_Load();
				MyApplication_Activated();
			}
			catch( PsxException caught )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "SharedPsx::SharedPsx() - PsxException: " + caught.Message );
				MyApplication.GetApplication().Exit();                
			}

			showTimeTimer = new Timer( new TimerCallback( ShowTime ), null , 0, 1000 );

		}
        
		/// <summary>
		/// The class destructor.
		/// </summary>
		~SharedPsx()
		{
#if ( !WINCE )
			if(autoSignOffSeconds > 0)
			{
				PInvoke.UnhookWindowsHookEx(_mousehookID);
				PInvoke.UnhookWindowsHookEx(_keyboardhookID);	
			}
#endif
			Dispose();
		}
				
		/// <summary>
		/// Releases the resources used by the LanePsx instance.
		/// </summary>
		public override void Dispose()
		{
			PsxEvent -= new PsxEventHandler(SharedPsx_PsxEvent); //SASA - added since RapNet has it.
			if( null != scanner )
			{
				scanner.Dispose();
				scanner = null;            
			}

			if( biometric != null )
			{
				if( biometric.BiometricDeviceConfigured )
				{
					Biometric.BiometricEvent -= new BiometricDelegate(Biometric_BiometricEvent);
					biometric.Dispose();
					biometric = null;
				}
			}

#if		( WINCE )
			TopMost = false;
#endif
			m_bUnload=true;

			CurrentState.Dispose();
			CurrentState = null;

			if( showTimeTimer != null )
			{
				showTimeTimer.Dispose();
				showTimeTimer = null;
			}

#if		( !WINCE )
            // +TAR 409851
            if (bInitializeKeyDeviceFlag)
			{
			    Thread.Sleep(1000);
			    myself.m_keyLock.DeviceEnabled=false; 
			    myself.m_keyLock.ReleaseDevice();
			    myself.m_keyLock.Close();
			}
			// -TAR 409851
#endif
            			
			base.Dispose();
			GC.SuppressFinalize( this );
		}

		/// <summary>
		/// Gets the exclusive lane number.
		/// </summary>
		public int ExclusiveLane
		{
			get
			{
				return exclusiveLane;
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
					operatorId = "";
				}
				else
				{
					operatorId = value;
				}
#if( HH_UI )                
                string text = "";
#else
				string text = GetString( "OperatorIDText" );
#endif
				text += ( operatorId.Length > 0 ) ? operatorId : GetString( "SignedOffButtonText" );

				SetControlConfigProperty( "OperationID", Psx.PropertyTextFormat, text );
			}
		}
		
		/// <summary>
		/// Gets or sets the terminal id.
		/// </summary>
		public string TerminalId
		{
			get
			{
				return terminalId;
			}			
			set
			{
				if( value == null )
				{
					terminalId = "";
				}
				else
				{
					terminalId = value;
				}
#if( HH_UI )                
                // Per email from Marcia Crosland, we do not need this information on the HandHeld
#else
				// Display nothing if empty string (e.g. RAP machine)
				if ( !terminalId.Equals( "" ) )
				{
					string text = GetString( "TerminalID" );
					text += terminalId;
					SetControlConfigProperty( "TerminalID", Psx.PropertyTextFormat, text );
				}
#endif
			}
		}

		/// <summary>
		/// Gets or sets the version of the interface.		Rfc 343789
		/// </summary>
		public string InterfaceVersion
		{
			get
			{
				return interfaceVersion;
			}			
			set
			{
				if( value == null )
				{
					interfaceVersion = "";
				}
				else
				{
					interfaceVersion = value;
				}
#if( HH_UI )                
                
#else
				// Display nothing if empty string (e.g. RAP machine)
				if ( !interfaceVersion.Equals( "" ) )
				{
					string text = GetString( "InterfaceVersion" );
					text += interfaceVersion;
					SetControlConfigProperty( "InterfaceVersion", Psx.PropertyTextFormat, text );
				}
				else
					CmDataCapture.Capture ( CmDataCapture.MaskInfo, "InterfaceVersion Key in the registry is empty." );

#endif
			}
		}

		/// <summary>
		/// Checks whether or not the last signon or signoff was 
		/// performed automatically by synching Launchpad/RapNet
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
		/// Checks whether or not the sign On was from a remote application (from RapNet or Fastlane).
		/// </summary>
		public bool WasSignOnRemote
		{
			get
			{
				return wasSignOnRemote;
			}
			set
			{
				wasSignOnRemote = value;				
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
		public LaunchPadNet.State CurrentState
		{
			get
			{
				return currentState;
			}			
			set
			{
				if( null != currentState )
				{
					LaunchPadNet.State oldState = currentState;
					currentState = value;
					oldState.Dispose();
					oldState = null;
				}
				currentState = value;
			}
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
		/// Occurs when Psx fires an event.
		/// </summary>
		/// <param name="sender">
		/// The source of the event.
		/// </param>		
		/// <param name="e">
		/// A PsxEventArgs that contains the event data.
		/// </param>
		public void SharedPsx_PsxEvent( object sender, PsxEventArgs e )
		{
			ConfigData configData = ConfigDataLaunchPad.GetConfigData();

			if( Psx.EventChangeContext == e.EventName )
			{
				string newState = "";                
				try
				{
					newState = GetContextCustomDataVariable( "State", e.ContextName ); 
				}
				catch( PsxException caught )
				{
					CmDataCapture.Capture( CmDataCapture.MaskError, "SharedPsx::SharedPsx_PsxEvent() - PsxException: " + caught.Message );                    
				}
				catch( Exception caught )
				{
					CmDataCapture.Capture( CmDataCapture.MaskError, "SharedPsx::SharedPsx_PsxEvent() - Exception: " + caught.Message );
					throw caught;
				}
				CmDataCapture.Capture( CmDataCapture.MaskError, "SharedPsx::SharedPsx_PsxEvent: newState = " + newState + "   CurrentState = " + ((CurrentState == null) ? "" : CurrentState.ToString()) );
				if( newState.Length > 0 && ( CurrentState == null || !newState.Equals( CurrentState.ToString() ) ) )
				{
					switch( newState )
					{
						case "LegalCaption" :
							CurrentState = new LegalCaptionState( this );
							ShowCurrentExecutableWindows( false );
							break;
						case "Monitoring":
							CurrentState = new MonitoringState( this );
							ShowCurrentExecutableWindows( true );
							break;
						case "SignOn":
							CurrentState = new SignOnState( this );
							ShowCurrentExecutableWindows( wasAutoSignedOff ? true : false );
							break;
						case MaintSignOn :
							CurrentState = new MaintSignOnState( this );
							ShowCurrentExecutableWindows( false );
							break;
#if	( !WINCE )
						case WindowsSignOn :
							CurrentState = new WindowsSignOnState( this );
							break;
#endif
						default:
							CmDataCapture.Capture( CmDataCapture.MaskWarning, "SharedPsx::SharedPsx_PsxEvent() - Unknow State: " + newState );
							break;
					}
				}   

				// Forward this event to State //
				if( CurrentState != null )
				{
					CurrentState.State_PsxEvent( sender, e );
				}
			}
			else if( configData.AltF4ExitAllowed && e.EventName.Equals( Psx.EventQuit ) )
			{
				#if	( !WINCE )
				//+TAR392125
				//This will close RapNet application
				if (IsMainExecutableProcessAlive())
				{
					ExecuteFunctionProcessor( this.TheApp.AppExecutable,"Stop"); 
				}

				//if LP is going to close then we will also close local processes running (i.e get diag, volume, etc.)
				if ( IsCurrentExecutableProcessAlive() )
				{
					ExecuteFunctionProcessor( this.m_currentExecutable,"Stop"); 
				}
				//+TAR392125

				//This will close LP application
				MyApplication.GetApplication().Exit();
				#else
				CmDataCapture.Capture( CmDataCapture.MaskWarning, "SharedPsx::SharedPsx_PsxEvent() - Event Quit Received from PSX - ignoring" );
				#endif
			}
			else
			{
				if( CurrentState != null && !CurrentState.State_PsxEvent( sender, e ) )
				{
					if (e.ControlName != null)
					{
						this.m_sCurrentButtonPressed =e.ControlName; 
						ExecuteFunctions();
					}
				}
			}
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
				if( value != signedOn )
				{
					signedOn = value;
					SetConfigProperty( "SignOnButton", this.TheApp.MainContext, Psx.PropertyTextFormat, signedOn ? GetString( "SignOffButtonText" ) : GetString( "SignOnButtonText" ) );
                                   
					if( !signedOn )
					{
						if (ConfigDataLaunchPad.GetConfigData().LegalCaptionRequired)
						{
							SetContext( "LegalCaptionContext", SharedPsx.displayLPStandard, false );
						}
						else
						{
							SetContext( GetExpectedEnterIDContext(), SharedPsx.displayLPStandard, false);
						}
						OperatorId = "";
						OperatorRights = 0;
						WasSignOnRemote = false;

						// since we've signed off, synchronize Rap to do the same
						if( wasSyncSignOnOff )
						{
							wasSyncSignOnOff = false;
							wasAutoSignedOff = false;
						}
						else
						{
							if(!wasAutoSignedOff) 
							{ 
								MyApplication.SendProcessMessage( ConfigDataLaunchPad.rapApplication, "operation=set-signed-off" );
							}
						}
					}
					else
					{
						SetContext( TheApp.MainContext, SharedPsx.displayLPStandard, false );
												
						wasAutoSignedOff = false;
	
						if( wasSyncSignOnOff )
						{
							wasSyncSignOnOff = false;
						}
						else
						{
							// we've signed on, synchronize Rap to do the same
							string signOn = "operation=set-signed-on;operator=" + operatorId + ";rights=" + operatorRights.ToString();
							SendProcessMessage( ConfigDataLaunchPad.rapApplication, signOn );
						}
					}
				}
			}
		}

		/// <summary>
		/// Gets or sets the currently active host name.
		/// </summary>
		public string ActiveHostName
		{
			get
			{
				return activeHost;
			}
			set
			{
				activeHost = value;				
			}
		}

		/// <summary>
		/// Shows the time on the Rap UI.
		/// </summary>
		private void ShowTime( object state )
		{
			try
			{
				if( showTimeTimer != null )
				{
					heartbeatCounter++;
					if( 5 == heartbeatCounter )
					{
						heartbeatCounter = 0;
						MyApplication.LogMemoryUse( CmDataCapture.MaskExtensive );
					}

					// +Auto sign off feature
					if(autoSignOffSeconds > 0)
					{
						autoSignOffCounter++;
						if(autoSignOffCounter >= autoSignOffSeconds)
						{
							ResetSignonContexts();
							MyApplication.SendProcessMessage( ConfigDataLaunchPad.rapApplication, "operation=auto-signoff" );

							if(SignedOn) 
							{
								CmDataCapture.Capture( CmDataCapture.MaskError, "SharedPSX::AutoSignOffCounter - LaunchPad automatically signed off, autoSignOffCounter=" + autoSignOffCounter + "     Current state:" + CurrentState.ToString() );
								wasAutoSignedOff = true;
								SignedOn = false;
							}							
							autoSignOffCounter = 0;
						}
					}
					// -Auto sign off feature
	                                
					DateTime dt = DateTime.Now;
#if( HH_UI )
					SetControlConfigProperty( "DateText", Psx.PropertyTextFormat, dt.ToString( "d" ) );
					SetControlConfigProperty( "TimeText", Psx.PropertyTextFormat, dt.ToString( "T" ) );
#else
					string dateText = dt.ToString( "F" );
					SetControlConfigProperty( "DateText", Psx.PropertyTextFormat, dateText );                
#endif
				}
			}
			catch( PsxException caught )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "SharedPsx::ShowTime() - PsxException: " + caught.Message );
			}
			catch( Exception caught )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "SharedPsx::ShowTime() - Exception: " + caught.Message );
			}
		}

		/// <summary>
		/// Set the timeout to zero in order to make SetForegroundWindow succeed.
		/// </summary>
		/// 
		/// On some systems the foreground lock timeout is not zero. This can cause
		/// the method SetForegroundWindow to fail. LaunchPad uses SetForegroundWindow
		/// to display LaunchPad on top of all other windows. If SetForegroundWindow
		/// fails than WindowsExplorer, regedit, etc. windows will appear on top of
		/// LaunchPad. See TAR294081. This method must be called when LaunchPad has
		/// focus.
		private void ZeroForegroundLockTimeout()
		{
#if	( !WINCE )
			bool success = false;
			try 
			{
				success = PInvoke.SystemParametersInfoSet(PInvoke.SPI_SETFOREGROUNDLOCKTIMEOUT, 0, 0, PInvoke.SPIF_SENDWININICHANGE | PInvoke.SPIF_UPDATEINIFILE);
			}
			catch 
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "ZeroForegroundLockTimeout Exception");
			}
			CmDataCapture.Capture( CmDataCapture.MaskInfo, "ZeroForegroundLockTimeout success=" + success.ToString());
#endif
		}

		// dtb START
		private void MyApplication_Load(/* dtb OLD object sender, System.EventArgs e*/)
		{
			try
			{
				ConfigDataLaunchPad configData = ConfigDataLaunchPad.GetConfigData();

				ConfigReader configReader= new ConfigReader();
				bool readConfigSuccess=configReader.ReadXMLFile (configData.ConfigXML);  
				this.m_executablesTable=configReader.GetExecutables();                    
				this.TheApp = configReader.App;      

				if ( readConfigSuccess )
				{
					this.m_buttonsTable =CreateButtons();               
					this.TheApp.ButtonsTableHolder.Add(this.TheApp.CurrentContext,this.m_buttonsTable);         
					
					// If UtilityButton (RETURN) is never accessable, set it invisible.
					if ( !this.TheApp.UtilityButtonVisible )
					{
						SetControlConfigProperty("UtilityButton","Visible",false); 
					}
					// If SignOnButton is never accessable, set it invisible.
					if ( !this.TheApp.LaunchPadSecurityOn )
					{
						SetControlConfigProperty("SignOnButton","Visible",false); 
					}

					// While LP has focus, set the foreground window timeout to zero
					ZeroForegroundLockTimeout();

					// dtb: Should we not show the Signon Button if the operator cannot ever click it?
					// For now, we will show it disabled. 
					//ActivateLaunchPad(false,false); 
					//ActivateLaunchPad(true,!this.TheApp.LaunchPadSecurityOn);

					if ( this.TheApp.LoadOnStartUp )   
					{
						Thread mainExecutableThread = new Thread(new ThreadStart(InitializeMainExecutable));
#if (!WINCE)
						mainExecutableThread.Name = "InitializeMainExecutable";
#endif
						mainExecutableThread.Start();                                       
					}
					else
					{
						UpdateButtonsState();
//#if ( WINCE )
						myself.DisplayLaunchPadNet(true);
//#endif
					}
					
					// OPOS - Keylock thread 
#if (!WINCE)
					if( (TheApp.KeyLock.Trim().Length > 0) && (TheApp.KeyLock.Trim().ToUpper() != "NONE") )
					{
						if	( ConfigDataLaunchPad.HasKeyLockHW )
						{
							CmDataCapture.Capture( CmDataCapture.MaskInfo, "3.0 Hardware detected - LaunchPad will signon users with Keylock usage" ); 
						}
						CmDataCapture.Capture( CmDataCapture.MaskExtensive, "keyLockThread:BEFORE");
						Thread keyLockThread = new Thread(new ThreadStart(KeyDetect));
						keyLockThread.Name = "KeyDetect";
						keyLockThread.Start();
						CmDataCapture.Capture( CmDataCapture.MaskExtensive, "keyLockThread:AFTER");
					}
					else
					{
						CmDataCapture.Capture(CmDataCapture.MaskInfo, 
								"No Keylock configured. Configuration file '" + configData.ConfigXML + "' value KeyLock='" + TheApp.KeyLock + "'");
					}
#endif

					ActivateLaunchPad(true,true);
					SetDefaultVolume();  //set the default volume
				}
				else
				{
					throw new Exception("Error loading Configuration file.");         
				}           
			}     
			catch(Exception ex)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, String.Format ("<Error> Unloading Main Form:Exception Occured: {0}", ex.Message ));
				// dtb OLD this.Close ();
				// dtb NEW
				MyApplication.GetApplication().Exit();
			}
      
		}
		
		private void MyApplication_Activated(/* dtb OLD object sender, System.EventArgs e*/)
		{
			try
			{
				// Enabled is causing an exception here. 
				// Unable to convert to bool in PsxNet.dll
				if(/* dtb this. */ Enabled ==false)
				{
					this.MinimizeMainExecutable();        
				}
			}
			catch( Exception e )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError,  String.Format("<Exception> [MyApplication.MyApplication_Activated()] [{0}]", e.Message ) );            
			}

			this.MinimizeMainExecutable();        

		}
		
		public void MinimizeMainExecutable()
		{
			Executable exec = this.m_executablesTable[this.TheApp.CurrentExecutable] as Executable;      
			if ( exec!=null)
				exec.MinimizeWindow();
		} 

		private Hashtable CreateButtons()
		{
			Hashtable buttonsTable = new Hashtable();     
      
			String []buttons=GetControlNameList(this.TheApp.CurrentContext);
			for ( int x=0;x<buttons.Length ;x++)
			{
				if ( buttons[x] !=null && !buttons[x].Trim().Equals("") )
				{         
					if ( GetControlConfigProperty(buttons[x],"ControlType").ToString().ToLower().Equals("button") )
					{         
						CreateButton(buttons[x],ref buttonsTable);        
					}
				}

			}
			return buttonsTable;
		}   

		///This function creates a 'Button object' associated with a PSX button
		///and gives functionalities to it.   
    
		///If button created has no critical error such as ExecutableName error, then it is added to a hashtable
		///or else it will be discarded
    
		///Since and we don't have control over PSX, it will display all PSX buttons
		///regardless whether we have discarded the 'Button object' and its functionalities.
		private void CreateButton(String buttonName,ref Hashtable buttonsTable)
		{     
			Button buttonTemp= new Button();
			try
			{
				buttonTemp.Name=buttonName;
                
				buttonTemp.PrimaryText =GetTextData("PrimaryText",buttonName,this.TheApp.CurrentContext) ;
				buttonTemp.SecondaryText =GetTextData("SecondaryText",buttonName,this.TheApp.CurrentContext);
        
				buttonTemp.PrimaryFunction =GetStringData("PrimaryFunction",buttonName,this.TheApp.CurrentContext);
				buttonTemp.SecondaryFunction =GetStringData("SecondaryFunction",buttonName,this.TheApp.CurrentContext);
                
				try
				{
					// Is there one additional authentication required for this button? Maintenance registry verified.
					buttonTemp.MaintenanceSignOn = GetBooleanData("MaintenanceSignOn",buttonName,this.TheApp.CurrentContext,false);
				}
				catch( Exception e )
				{
					CmDataCapture.Capture( CmDataCapture.MaskError,  String.Format("<Error> [MyApplication.CreateButton()] Button:{0} - defaulting to MaintenanceSignon - false - Exception {1}", buttonName , e.Message));            
					buttonTemp.MaintenanceSignOn = false;
				}
				
#if	( !WINCE )
				// Is there one additional authentication required for this button? Windows Log On verified.
				buttonTemp.WindowsSignOn = GetBooleanData("WindowsSignOn",buttonName,this.TheApp.CurrentContext,false);
#endif
				buttonTemp.PrimaryShowConfirmation   =GetBooleanData("PrimaryShowConfirmation",buttonName,this.TheApp.CurrentContext,false);
				buttonTemp.SecondaryShowConfirmation  =GetBooleanData("SecondaryShowConfirmation",buttonName,this.TheApp.CurrentContext,false);

				buttonTemp.PrimaryShowConfirmationMessage	=GetTextData("PrimaryShowConfirmationMessage",buttonName,this.TheApp.CurrentContext);
				buttonTemp.SecondaryShowConfirmationMessage =GetTextData("SecondaryShowConfirmationMessage",buttonName,this.TheApp.CurrentContext);
        
				buttonTemp.PrimaryAllowToggleOnButton   =GetStringData("PrimaryAllowToggleOnButton",buttonName,this.TheApp.CurrentContext);
				buttonTemp.SecondaryAllowToggleOnButton  =GetStringData("SecondaryAllowToggleOnButton",buttonName,this.TheApp.CurrentContext);
        
				buttonTemp.PrimaryAllowToggleOnButtonState		=	GetStringData("PrimaryAllowToggleOnButtonState",buttonName,this.TheApp.CurrentContext);
				buttonTemp.SecondaryAllowToggleOnButtonState	=	GetStringData("SecondaryAllowToggleOnButtonState",buttonName,this.TheApp.CurrentContext);
        
				buttonTemp.PrimaryRequiredSecurity		=	GetBooleanData("PrimaryRequiredSecurity",buttonName,this.TheApp.CurrentContext,false);
				buttonTemp.SecondaryRequiredSecurity	=	GetBooleanData("SecondaryRequiredSecurity",buttonName,this.TheApp.CurrentContext,false);
        
				buttonTemp.DisabledOnDeadEnabledOnAliveMainExecutable =GetBooleanData("DisabledOnDeadEnabledOnAliveMainExecutable",buttonName,this.TheApp.CurrentContext,false);
				buttonTemp.ToggleStateOnDeadMainExecutable  =GetStringData("ToggleStateOnDeadMainExecutable",buttonName,this.TheApp.CurrentContext);
				buttonTemp.ToggleStateOnAliveMainExecutable  =GetStringData("ToggleStateOnAliveMainExecutable",buttonName,this.TheApp.CurrentContext);
				buttonTemp.ToggleEnabled =GetBooleanData("ToggleEnabled",buttonTemp.Name,this.TheApp.CurrentContext,false);                       

				try
				{
					buttonTemp.OrigState = Convert.ToInt32( GetControlConfigProperty(buttonName,"State").ToString() );     
				}
				catch (InvalidCastException e )
				{
					CmDataCapture.Capture( CmDataCapture.MaskError,  String.Format("<Error> [MyApplication.CreateButton()] cast to OrigState failed Button:{0} - defaulting to Normal State Error:{1}",buttonName ,e.Message));            
					buttonTemp.OrigState = (int) Psx.State.Normal;
				}
        
				if ( buttonTemp.PrimaryFunction ==null || buttonTemp.PrimaryFunction.Equals("") )
					buttonTemp.PrimaryFunction="Start";           
        
				if (buttonTemp.ToggleEnabled && buttonTemp.SecondaryFunction==null && buttonTemp.SecondaryFunction.Equals(""))
					buttonTemp.SecondaryFunction ="Start";
        
                    
				Executable execTemp=null;
        
				//Blank Config Executable name is allowed if Primary function is SwitchTo 
				//buttonTemp is discarded
				if ( this.TheApp.AllowSwitchTo() && buttonTemp.PrimaryFunction.Equals("SwitchTo") ||
					buttonTemp.PrimaryFunction.Equals("WindowsLogOn"))
				{
					SetControlConfigProperty( buttonName, "Text", buttonTemp.CurrentText );
					buttonsTable.Add (buttonName,buttonTemp); 
				}       
				else
				{
					//Check if invalid executables from Config
					String configExecutableName =GetStringData("ExecutableName",buttonName,this.TheApp.CurrentContext);                       
					execTemp=(Executable)this.m_executablesTable[configExecutableName];         
					if (execTemp==null )    
					{       
						SetControlConfigProperty(buttonName,"Visible",false);                                                                 
						CmDataCapture.Capture( CmDataCapture.MaskError, String.Format ("<Error> [MyApplication.CreateButton()] Executable Name Error(either invalid or null) Could not create Button:{0}",buttonName)); 
					}
					else
					{
						buttonTemp.Executable  =execTemp;       
						if ( buttonTemp.InitialState.ToLower().Equals("secondary"))
							buttonTemp.Toggle();
 
						SetControlConfigProperty(buttonName,"Text", buttonTemp.CurrentText  );                                                                                                                                                                      
						buttonsTable.Add (buttonName,buttonTemp); //successfully added to Button List
					} 
				}   

			}

			catch(Exception ex)
			{       
				CmDataCapture.Capture( CmDataCapture.MaskError,  String.Format("<Error> [MyApplication.CreateButton()] Button:{0} Error:{1}",buttonName ,ex.Message));            
			}
		}
    
		//Update Enable and Toggle state of buttons depending from the state of the main executable
		//Configuration in LaunchPadPSX.xml:
		//      <DisabledOnDeadMainExecutable>[false/true]</DisabledOnDeadMainExecutable>
		//      <ToggleStateOnDeadMainExecutable>[Primary/Secondary]</ToggleStateOnDeadMainExecutable>    
		//      <ToggleStateOnAliveMainExecutable>[Primary/Secondary]</ToggleStateOnAliveMainExecutable>    
		private void UpdateButtonsState()
		{
			ICollection keys=this.m_buttonsTable.Keys;
			try
			{   
				foreach(String key in keys)
				{       
					Button b= (Button)this.m_buttonsTable[key]; 

					if ( b.DisabledOnDeadEnabledOnAliveMainExecutable )
					{
						if ( this.TheApp.AppExecutable!=null && this.TheApp.AppExecutable.LocalProcess !=null && !this.TheApp.AppExecutable.LocalProcess.HasExited )                     
						{
							b.OrigState = (int)Psx.State.Normal;
							if ( /* dtb this. */ Enabled )
								SetControlConfigProperty(key,"State",Psx.State.Normal);                             
						} 
						else
						{
							b.OrigState = (int) Psx.State.Disabled;
							if ( /* dtb this. */ Enabled )
								SetControlConfigProperty(key,"State",Psx.State.Disabled);              
						}           
					}             

					if ( b.ToggleStateOnDeadMainExecutable!=null && !b.ToggleStateOnDeadMainExecutable.Trim().Equals("") )
					{       
						if ( this.TheApp.AppExecutable==null || this.TheApp.AppExecutable.LocalProcess ==null || this.TheApp.AppExecutable.LocalProcess.HasExited )                      
						{
							if (  b.ToggleStateOnDeadMainExecutable.ToLower().Equals("primary") && b.ToggleStatus==true )
								b.Toggle();

							if (  b.ToggleStateOnDeadMainExecutable.ToLower().Equals("secondary") && b.ToggleStatus==false )
								b.Toggle();
							SetConfigProperty(b.Name,this.TheApp.CurrentContext,"Text",b.CurrentText ) ;
							/*this.*///SetControlConfigProperty(b.Name, "Text",b.CurrentText);                
						}                               
					}

					if ( b.ToggleStateOnAliveMainExecutable!=null && !b.ToggleStateOnAliveMainExecutable.Trim().Equals("") )
					{       
						if ( this.TheApp.AppExecutable!=null && this.TheApp.AppExecutable.LocalProcess !=null && !this.TheApp.AppExecutable.LocalProcess.HasExited )                     
						{
							if (  b.ToggleStateOnAliveMainExecutable.ToLower().Equals("primary") && b.ToggleStatus==true )
								b.Toggle();

							if (  b.ToggleStateOnAliveMainExecutable.ToLower().Equals("secondary") && b.ToggleStatus==false )
								b.Toggle();

							SetConfigProperty(b.Name,this.TheApp.CurrentContext,"Text",b.CurrentText ) ;
							/*this.*///SetControlConfigProperty(b.Name, "Text",b.CurrentText);                
						}                               
					}
				}      
			}
			catch(Exception ex)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, String.Format ("<Error> UpdateButtonsState(): {0}", ex.Message ));
			}
		}
    
		//All buttons will be disabled/enabled such as when the keylock is in Normal/Supervisory mode.
		//or other situation which does not allow launchpad for any user interaction
		private void EnableButtons(bool enableButtons)
		{
      
			try
			{
				String []buttons=GetControlNameList(this.TheApp.CurrentContext);
				for ( int x=0;x<buttons.Length ;x++)
				{
					if ( buttons[x]!=null && !buttons[x].Trim().Equals("") )
					{
						if ( GetControlConfigProperty(buttons[x],"ControlType").ToString().ToLower().Equals("button") )
						{
							if (!enableButtons )
							{
								if (buttons[x]=="SignOnButton" || buttons[x]=="UtilityButton")
								{
								}
								else
								{
									SetControlConfigProperty(buttons[x],"State",Psx.State.Disabled);
								}
							}
							else //restore original value       
							{
								Button b= (Button)this.m_buttonsTable[buttons[x]];
								if ( b != null)
								{
									if (buttons[x]=="SignOnButton" ||buttons[x]== "UtilityButton")
									{
									}
									else
									{
										SetControlConfigProperty(buttons[x],"State",b.OrigState);
									}

								}
							}
						}
					}
				}
			}
			catch (Exception e)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "EnableButtons():EXCEPTION " + e.Message);
			}
		}

		//Load Executables from Config where LoadOnStartup="true"
		private void LoadMainExecutable()
		{
			CmDataCapture.Capture( CmDataCapture.MaskError, "Starting Thread - Load Main Executeable:" + Thread.CurrentThread.ToString() );

			try
			{
				ICollection keys=this.m_executablesTable.Keys ;

				foreach(String key in keys)
				{       
					if ( key.ToString().Equals(this.TheApp.MainExecutable) )
					{
						Executable execTemp=(Executable)this.m_executablesTable[key];         
          
						if( ExecuteFunctionProcessor(execTemp ,"Start") )
						{   
							CmDataCapture.Capture( CmDataCapture.MaskError, "LoadMainExecutable(): p1");

							this.TheApp.AppExecutable=execTemp;
							execTemp.MaximizeWindow();
							CmDataCapture.Capture( CmDataCapture.MaskError, "LoadMainExecutable(): p2");

							Thread tCheckMainExecutable = new Thread( new ThreadStart(this.CheckMainExecutable));
							CmDataCapture.Capture( CmDataCapture.MaskError, "LoadMainExecutable(): p3");
#if (!WINCE)
							tCheckMainExecutable.Name = "CheckMainExecutable";
#endif
							tCheckMainExecutable.Start();
							CmDataCapture.Capture( CmDataCapture.MaskError, "LoadMainExecutable(): p4");
						}
						else
						{
							UpdateButtonsState();
							DisplayLaunchPadNet( true );
							//this.ActivateLaunchPad( true, true );
						}
						break;
					}
				}
			}
			catch (Exception e)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "LoadMainExecutable():EXCEPTION " + e.Message);
			}
		}

		private void CheckMainExecutable()
		{
			try
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "Starting Thread - Check Main Executable:" + Thread.CurrentThread.ToString() );
				SetConfigProperty( "UtilityButton", this.TheApp.MainContext, Psx.PropertyState, Psx.State.Normal);
				UpdateButtonsState();

#if ( !WINCE )
				int		Seconds = 0;
				bool	bCheckStartup = true;
#endif
				while (IsMainExecutableProcessAlive())
				{
					if ( myself.m_bUnload != false )
					{
						break;
					}

#if ( !WINCE )
					Thread.Sleep(1000); 
					Seconds++;

					// 3 Seconds after main app startup. 
					// Force Change Context away from StartupContext 
					// So that we may use LaunchPad.  
					if	( bCheckStartup )
					{
						if	( Seconds > 2 )
						{

							ConfigDataLaunchPad configData = ConfigDataLaunchPad.GetConfigData();
							if	( myself.GetContext( SharedPsx.displayLPStandard ) == "StartupContext" )
							{
								DisplayLaunchPadNet( false );
								if	( !SignedOn )
								{
									if (configData.LegalCaptionRequired)
									{
										SetContext( "LegalCaptionContext", SharedPsx.displayLPStandard, false );
									}
									else
									{
										SetContext( GetExpectedEnterIDContext(), SharedPsx.displayLPStandard, false);
									}
								}
								else
								{
									myself.SetContext( myself.TheApp.MainContext, SharedPsx.displayLPStandard, false );
								}
							}
						}
					}
#else
					Thread.Sleep(3000); 
#endif
				}

				
				if ( myself.m_bUnload==false )
				{
//#if ( WINCE )
					// DisplayLaunchPadNet( true ); PatchG - commented out, verify if adverse effect with shared scanning
					//this.ActivateLaunchPad( true, true );
//#endif
					SetConfigProperty( "UtilityButton", this.TheApp.MainContext, Psx.PropertyState, Psx.State.Disabled);
					UpdateButtonsState();
				}
			}
			catch (Exception e)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "CheckMainExecutable():EXCEPTION " + e.Message);
			}
		}

		private void InitializeMainExecutable()
		{
			try
			{
				CmDataCapture.Capture( CmDataCapture.MaskInfo, "InitializeMainExecutable():START");
				LoadMainExecutable();
				// SA250050: Commented out this patch since fix was made to set the 
				//           DisplayLAunchPadNet(false) once the main application is activated
				// Temporary fix to display lane/rap on top at start.
				/*
#if	( !WINCE )
				CmDataCapture.Capture( CmDataCapture.MaskInfo, "InitializeMainExecutable():DisplayLaunchPadNet(false)");
				myself.DisplayLaunchPadNet(false);
#endif
*/
				CmDataCapture.Capture( CmDataCapture.MaskInfo, "InitializeMainExecutable():END");
			}
			catch (Exception e)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "InitializeMainExecutable():EXCEPTION " + e.Message);
			}
		}

		//Getting CustomData String values for control
		private String GetStringData(String propName,String controlName,String context)
		{
			String sRetVal="";
			try
			{         
				sRetVal=GetCustomDataVariable(propName,controlName,context);
				if ( sRetVal==null)
					sRetVal="";
			}
			catch (Exception ex)      
			{             
				CmDataCapture.Capture( CmDataCapture.MaskWarning,  String.Format("<Warning> [MyApplication.GetStringData()] PSX Property Error: {0} Control:{1} Property:{2}",ex.Message,controlName,propName ));     
				CmDataCapture.Capture( CmDataCapture.MaskWarning,  String.Format("<Warning> [MyApplication.GetStringData()] Using default String value[\"\"] for Control:{0} Property:{1} \r\n",controlName,propName ));      
			}
			return sRetVal;
		}
    
		//Getting CustomData String values for control
		private String GetTextData(String propName,String controlName,String context)
		{
			String sRetVal = "";
			string sInternationalID = "";

			try
			{         
				sInternationalID = /*psx.*/GetCustomDataVariable(propName,controlName,context);
				if(sInternationalID != null)
				{
					sRetVal = /*psx.*/GetString( sInternationalID);
					if ( sRetVal == null)
						sRetVal="";
				}
			}
			catch (Exception ex)      
			{             
				CmDataCapture.Capture( CmDataCapture.MaskWarning,  String.Format("<Warning> [MyApplication.GetStringData()] PSX Property Error: {0} Control:{1} Property:{2}",ex.Message,controlName,propName ));     
				CmDataCapture.Capture( CmDataCapture.MaskWarning,  String.Format("<Warning> [MyApplication.GetStringData()] Using default String value[\"\"] for Control:{0} Property:{1} \r\n",controlName,propName ));      
			}
			return sRetVal;
		}
    
		//Getting CustomData Bool values only for control
		private bool GetBooleanData(String propName,String controlName,String context,bool defaultValue )
		{
			bool bRetVal=defaultValue;
			try
			{         
				bRetVal= Boolean.Parse (GetControlCustomDataVariable(propName,controlName).ToLower());        
        
			}
			catch(Exception ex)
			{
				CmDataCapture.Capture( CmDataCapture.MaskWarning,  String.Format("<Warning> [MyApplication.GetBooleanData()] PSX Property Error:{0} Control:{1} Property:{2}",ex.Message,controlName,propName )); 
				CmDataCapture.Capture( CmDataCapture.MaskWarning,  String.Format("<Warning> [MyApplication.GetBooleanData()] Using default bool value[{2}] for Control:{0} Property:{1}\r\n",controlName,propName,defaultValue ));      
			}
			return bRetVal;
		}
		
		public void ActivateLaunchPad(bool enabledScreen, bool enableButtons)
		{   
			try
			{
				CmDataCapture.Capture( CmDataCapture.MaskInfo, "ActivateLaunchPad():START");
				Enabled = enabledScreen;
				CmDataCapture.Capture( CmDataCapture.MaskInfo, "ActivateLaunchPad(): enableScreen =" + enabledScreen.ToString());
				CmDataCapture.Capture( CmDataCapture.MaskInfo, "ActivateLaunchPad(): enableButtons =" + enableButtons.ToString());
				SetConfigProperty( "UtilityButton", this.TheApp.MainContext, Psx.PropertyState, (IsMainExecutableProcessAlive() ? Psx.State.Normal : Psx.State.Disabled));
				if ( enabledScreen )
				{
					SetConfigProperty( "SignOnButton", this.TheApp.MainContext, Psx.PropertyState, this.TheApp.LaunchPadSecurityOn ? Psx.State.Normal : Psx.State.Disabled );
				}
				else
				{
					SetConfigProperty( "SignOnButton", this.TheApp.MainContext, Psx.PropertyState, Psx.State.Disabled);
				}
				EnableButtons(enableButtons);
				CmDataCapture.Capture( CmDataCapture.MaskInfo, "ActivateLaunchPad():END");
			}
			catch (Exception e)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "ActivateLaunchPad():EXCEPTION " + e.Message);
			}
		}
		     
		//This is a recursive function checking each executable until it reaches to the top level executable before it executes it
		//Configuration of executables should be carefully examined inorder to avoid infinite calling with each other
		private bool ExecuteFunctionProcessor(Executable executable,String function)
		{
			bool bRetVal=false;
			try
			{
				CmDataCapture.Capture( CmDataCapture.MaskExtensive,  "ExecuteFunctionProcessor():<<<<<");
				if ( function!=null )
				{
					CmDataCapture.Capture( CmDataCapture.MaskExtensive,  "ExecuteFunctionProcessor():function=" + function);
				}

				if ( executable==null )
				{
					CmDataCapture.Capture( CmDataCapture.MaskExtensive,  "ExecuteFunctionProcessor():>>>>>TOP " + false.ToString());
					return false;
				}

				ArrayList arrayList= executable.FunctionProcessor;
      
				Executable preExecutable =null;
				Executable actualExecutable =null;
				Executable postExecutable =null;

				String preExecute =null;
				String actualExecute =null;
				String postExecute =null;
			
				CmDataCapture.Capture( CmDataCapture.MaskExtensive,  "ExecuteFunctionProcessor():A");
				if ( arrayList !=null || arrayList.Count >0 )
				{
					CmDataCapture.Capture( CmDataCapture.MaskExtensive,  "ExecuteFunctionProcessor():B");
					IEnumerator enumProcessor= arrayList.GetEnumerator();
        
					while ( enumProcessor.MoveNext())
					{ 
						CmDataCapture.Capture( CmDataCapture.MaskExtensive,  "ExecuteFunctionProcessor():C");
						FunctionProcessor fnc= (FunctionProcessor)enumProcessor.Current ;
						if (function.Equals(fnc.OnEvent) )
						{
							CmDataCapture.Capture( CmDataCapture.MaskExtensive,  "ExecuteFunctionProcessor():D");
							switch( fnc.ExecuteOn.ToLower() )
							{
								case "pre" :            
									preExecutable =((Executable)this.m_executablesTable[fnc.ExecuteableName]);
									preExecute=fnc.Execute ;
									break;
								case "replaceactual" :
									actualExecutable =((Executable)this.m_executablesTable[fnc.ExecuteableName]);
									actualExecute=fnc.Execute ;
									break;            
								case "post" :
									postExecutable=((Executable)this.m_executablesTable[fnc.ExecuteableName]);
									postExecute=fnc.Execute ;
									break;              
							}
						}
					}
    
				}
				CmDataCapture.Capture( CmDataCapture.MaskExtensive,  "ExecuteFunctionProcessor():E");

				if (preExecutable!=null)  //execute PreProcessor
				{
					//If main Executable, Display Main app on top to show shutdown process.
					if	( IsMainExecutableProcessAlive() )
					{
						if (  preExecute == "Stop" && preExecutable.ExecutableName.Equals(this.TheApp.CurrentExecutable) )
						{
							CmDataCapture.Capture( CmDataCapture.MaskInfo, "ExecuteFunctionProcessor():Display Main app on top to show shutdown process.");
							ShowMainExecutable();
						}
					}
					ExecuteFunctionProcessor(preExecutable,preExecute);       //recursive calling //check if executable has some pre/actual/stop      
				}
          
				CmDataCapture.Capture( CmDataCapture.MaskExtensive,  "ExecuteFunctionProcessor():F");
				if (actualExecutable!=null)  //execute replacement for ActualProcessor
				{
					CmDataCapture.Capture( CmDataCapture.MaskExtensive,  "ExecuteFunctionProcessor():F1");
					ExecuteFunctionProcessor(actualExecutable,actualExecute); //recursive calling //check if executable has some pre/actual/stop            
				}
				else      
				{
					CmDataCapture.Capture( CmDataCapture.MaskExtensive,  "ExecuteFunctionProcessor():F2");
					bRetVal=executable.Execute(function);  
				}
          
				CmDataCapture.Capture( CmDataCapture.MaskExtensive,  "ExecuteFunctionProcessor():G");
				if (postExecutable!=null)  //execute PostProcessor            
					ExecuteFunctionProcessor(postExecutable,postExecute);   //recursive calling //check if executable has some pre/actual/stop      
            
				CmDataCapture.Capture( CmDataCapture.MaskExtensive,  "ExecuteFunctionProcessor():>>>>>BOTTOM " + bRetVal.ToString());
			}
			catch (Exception e)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "ExecuteFunctionProcessor():EXCEPTION " + e.Message);
			}
			return bRetVal;
		}
		
		//All functions are process here ( Minimize,Maximize,SwitcTo,Start,Stop )
		private void ExecuteFunctions()
		{
			try
			{
				ConfigDataLaunchPad configData = ConfigDataLaunchPad.GetConfigData();

				String controlName=this.m_sCurrentButtonPressed ;
				this.m_sCurrentButtonPressed="";
      
				Button buttonTemp;
				buttonTemp=(Button)this.m_buttonsTable[controlName];          

				// dtb We can enter this method without having a valid button. Thus we must ignore the button if it is not mapped.
				if (buttonTemp == null)
					return;

				if	( !ConfirmationApproved )
				{
					if ( buttonTemp.MaintenanceSignOn )
					{
						// Prompt user for additional sign on authentication.
						bool MaintenanceSignOnActive = MaintSignOn.Equals(GetContextCustomDataVariable( "State", "EnterID"));

						if ( !MaintenanceSignOnActive )
						{
							SetContextCustomDataVariable( "State", MaintSignOn, "EnterID" );
							SetContextCustomDataVariable( "State", MaintSignOn, "EnterPassword"  );
							SetContextCustomDataVariable( "State", MaintSignOn, "EnterAlphaNumericID"  );
							SetContextCustomDataVariable( "State", MaintSignOn, "EnterAlphaNumericPassword" );
							MaintButtonPressed = controlName; 
							SetContext( GetExpectedEnterIDContext(), SharedPsx.displayLPStandard, false);
							return;
						}
						else if ( MaintenanceSignOnFailed )
						{
							return;
						}
					}
#if	( !WINCE )
					else if ( buttonTemp.WindowsSignOn )
					{
						// Prompt user for additional sign on authentication.
						bool WindowsSignOnActive = WindowsSignOn.Equals(GetContextCustomDataVariable( "State", "EnterID"));

						if ( !WindowsSignOnActive )
						{
							SetContextCustomDataVariable( "State", WindowsSignOn, "EnterID" );
							SetContextCustomDataVariable( "State", WindowsSignOn, "EnterPassword"  );
							SetContextCustomDataVariable( "State", WindowsSignOn, "EnterAlphaNumericID"  );
							SetContextCustomDataVariable( "State", WindowsSignOn, "EnterAlphaNumericPassword" );
							WindowsButtonPressed = controlName; 
							// Always Alphanumeric for Logon Windows
							SetContext( "EnterAlphaNumericID", SharedPsx.displayLPStandard, false );

							return;
						}
						else if ( WindowsSignOnFailed )
						{
							return;
						}
					}
#endif
				}

				ResetSignonContexts();

				// Ask for confirmation - do you really want to do this?

				if	( buttonTemp.CurrentShowConfirmation )
				{
					if	( !ConfirmationApproved )
					{
						// if( PInvoke.IDYES != PInvoke.MessageBox( Handle, buttonTemp.CurrentShowConfirmationMessage, GetString( "UtilityFunctionsTitle" ), PInvoke.MB_YESNO | PInvoke.MB_ICONQUESTION ) )
						// {
						//		ActivateLaunchPad(true,true); 
						//		return;
						// }

						MaintButtonPressed = controlName;	// So Monitoring State knows how to call back 
						string question = buttonTemp.CurrentShowConfirmationMessage;
						this.SetControlConfigProperty("ConfirmMessage", "TextFormat", question );
						SetContext( "ConfirmationContext", SharedPsx.displayLPStandard, false );			
						return;
					}
					else	// Approved, continue request 
					{
						// Reset for next time
						ConfirmationApproved = false;
					}
				}

				switch(buttonTemp.CurrentFunction)
				{
					case "Minimize": 
						CmDataCapture.Capture( CmDataCapture.MaskInfo, "ExecuteFunctions():MINIMIZE-" + buttonTemp.Executable.Filename);
						ExecuteFunctionProcessor(buttonTemp.Executable ,buttonTemp.CurrentFunction);          
						ToggleButton(ref buttonTemp);                 
              
						this.TheApp.Minimized =true;
						SetConfigProperty(controlName,this.TheApp.CurrentContext,"Text",buttonTemp.CurrentText/*.Replace("{CurrentExecutable}",this.TheApp.CurrentExecutable)*/ ) ;                                 
						break;        
        
					case "Maximize":
						CmDataCapture.Capture( CmDataCapture.MaskInfo, "ExecuteFunctions():MAXIMIZE-" + buttonTemp.Executable.Filename);
						ExecuteFunctionProcessor(buttonTemp.Executable ,buttonTemp.CurrentFunction);
						ToggleButton(ref buttonTemp);
						this.TheApp.Minimized =false;
						break;      
        
					case "SwitchTo":
						CmDataCapture.Capture( CmDataCapture.MaskInfo, "ExecuteFunctions():SWITCHTO-" + buttonTemp.Executable.Filename);
						if (this.TheApp.AllowSwitchTo())
						{           
							SwitchTo();                       
							SetContext(this.TheApp.CurrentContext,displayLPStandard,false);
							this.TheApp.Minimized =false;                  
						}
						break;
        
					case "Start":
						//
						// 5/1/2005 TAR299642 dtb 
						// It looks like we are recieving two events for starting the same application. Maybe this
						// is caused by a double click prior to the control being disabled. Make the next block
						// a critical area to prevent it from being entered twice. In addition, add a condition
						// to prevent more than one application from executing.
						// 
						lock( this )
						{
							if (IsCurrentExecutableProcessAlive())
							{
								CmDataCapture.Capture( CmDataCapture.MaskError, "ExecuteFunctions():START ABORTED-" + buttonTemp.Executable.Filename + "- caused by doubleclick sending two events? ");
							}
							else
							{
								CmDataCapture.Capture( CmDataCapture.MaskInfo, "ExecuteFunctions():START-" + buttonTemp.Executable.Filename);
								
								if(configData.UtilityReturnAlwaysEnabled)
								{
									ActivateLaunchPad(true,false);
								}
								else
								{
									ActivateLaunchPad(false,false);
								}

								MinimizeMainExecutable();       

								bool result=ExecuteFunctionProcessor(buttonTemp.Executable ,"Start");
								// Fix 317613 - Since this thread and the CheckMainExecutable thread both
								// update the buttons, call a method that has some logic built in to determine
								// the button status.
								UpdateButtonsState();

								// In CE, we must set TopMost attribute of the Window off 
								// in order to show the App otherwise, it never shows. 
#if	( WINCE )					
								IntPtr hWnd = PInvoke.FindWindow( "UIControlWindowClass", "LaunchPadNet" );
								PInvoke.SetWindowPos( hWnd,( IntPtr ) ( PInvoke.SetWindowPosInsertAfter.HWND_NOTOPMOST ),0,0,0,0,(uint)(PInvoke.SetWindowPosFlags.SWP_NOMOVE|PInvoke.SetWindowPosFlags.SWP_NOSIZE)); 
								PInvoke.EnableWindow( hWnd, false );
#endif
         
								//If not the main Executable
								//Process is being checked by a thread if still alive.      
								if ( ! buttonTemp.Executable.ExecutableName.Equals(this.TheApp.CurrentExecutable) )
								{
									this.m_currentExecutable  = (Executable)this.m_executablesTable[buttonTemp.Executable.ExecutableName];                                                          
									this.m_currentExecutable.MaximizeWindow();        

									Thread threadProcessChecker= new Thread( new ThreadStart(this.CheckProcesses));   
#if (!WINCE)
									threadProcessChecker.Name = "CheckProcesses";
#endif
									threadProcessChecker.Start();                             
								}         
								else
								{
									ActivateMainExecutable();
									ResetBackwardsFlag = true;           // Sync the hardware keypress with the GO BACK button       
									ActivateLaunchPad(true,true); 
									// Automatically sign off when LP is not displayed. Only Sign Off if we signed on
									// from within LP.
									if ( SignedOn && !ConfigDataLaunchPad.HasKeyLockHW )
									{
										WasSyncSignOnOff = true;
										LogToReportGenerator( false );
										SignedOn = false;
									}
									// SA250050: once main app is activated it becomes topmost
									//  so LP display state needs to be updated accordingly.
									DisplayLaunchPadNet(false);
									
								}
							}
						}
						break;

					case "Stop":
						ActivateLaunchPad(false,false); 
						CmDataCapture.Capture( CmDataCapture.MaskInfo, "ExecuteFunctions():STOP-" + buttonTemp.Executable.Filename);
        
						//If main Executable, Display Main app on top to show shutdown process.
						if	( IsMainExecutableProcessAlive() )
						{
							if ( buttonTemp.Executable.ExecutableName.Equals(this.TheApp.CurrentExecutable) )
							{
								CmDataCapture.Capture( CmDataCapture.MaskInfo, "ExecuteFunctions():Display Main app on top to show shutdown process.");
								ShowMainExecutable();
							}
						}
                        ExecuteFunctionProcessor(buttonTemp.Executable ,buttonTemp.CurrentFunction);
                        
                        // Fix 411708 - G2_376 - HHRAP: Task bar will overlapped on the Utility Screen when Stop RAP.
						#if ( WINCE )
                            // Hide Window Task Bar
                            IntPtr hWnd1 = PInvoke.FindWindow("HHTaskBar", null);
                            if (hWnd1 != IntPtr.Zero)
                            {
                                PInvoke.ShowWindow(hWnd1, (int)PInvoke.ShowWindowFlags.SW_HIDE);
                            }
                        #endif
						CmDataCapture.Capture( CmDataCapture.MaskInfo, "ExecuteFunctions():STOP-UpdateButtonsState()" + buttonTemp.Executable.Filename);
						// Fix 317613 - Since this thread and the CheckMainExecutable thread both
						// update the buttons, call a method that has some logic built in to determine
						// the button status.
						UpdateButtonsState();
						CmDataCapture.Capture( CmDataCapture.MaskInfo, "ExecuteFunctions():STOP-this.TheApp.AppExecutable=" + buttonTemp.Executable.Filename);
						this.TheApp.AppExecutable=null;

						ActivateLaunchPad(true,true); 
						break;

					case "StopAll":
						ActivateLaunchPad(false,false); 
						CmDataCapture.Capture( CmDataCapture.MaskInfo, "ExecuteFunctions():STOPALL-" + buttonTemp.Executable.Filename);

						if	( IsMainExecutableProcessAlive() )
						{
							//If main Executable, Display Main app on top to show shutdown process.
							CmDataCapture.Capture( CmDataCapture.MaskInfo, "ExecuteFunctions():Display Main app on top to show shutdown process.");
							ShowMainExecutable();

							ExecuteFunctionProcessor(buttonTemp.Executable ,"Stop"/*buttonTemp.CurrentFunction*/);
						}
						ToggleButton(ref buttonTemp);                                       
						this.TheApp.AppExecutable=null;
						MyApplication.GetApplication().Exit();
						break;
#if (!WINCE)
					case "WindowsLogOn":
						ActivateLaunchPad(false,false); 
						CmDataCapture.Capture( CmDataCapture.MaskInfo, "ExecuteFunctions():WindowsLogOn id=" + WindowsSignOnId);

						if	( IsMainExecutableProcessAlive() )
						{
							ExecuteFunctionProcessor(this.TheApp.AppExecutable ,"Stop"/*buttonTemp.CurrentFunction*/);
						}
						ToggleButton(ref buttonTemp);                                       
						this.TheApp.AppExecutable=null;

						if ( true == configData.SetWindowsLogOn( WindowsSignOnId, WindowsSignOnPassword ) )
						{
							// LogOff the computer...
							PInvoke.ExitWindowsEx(PInvoke.ExitWindows.LogOff, PInvoke.ShutdownReason.MajorOther & PInvoke.ShutdownReason.MinorOther);
						}
						else	// Failed to set the logon registry setting.  
						{
							CmDataCapture.Capture( CmDataCapture.MaskError, "ExecuteFunctions():Windows Logon EXCEPTION - Permissions denied for setting the logon registry ");
							ActivateLaunchPad( true, true ); 
						}
						break;
#endif
				}
			}
			catch (Exception e)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "ExecuteFunctions():EXCEPTION " + e.Message);
			}
		}

		///Thread function for checking All Processes running
		private void CheckProcesses()
		{
			try
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "Starting Thread - Check Processes: " + Thread.CurrentThread.ToString() );

				// Wait for the current executable file to exit, then redisplay LaunchPadNet
				if ( this.m_currentExecutable!=null )
				{
					if ( this.m_currentExecutable.LocalProcess!=null && !this.m_currentExecutable.LocalProcess.HasExited )
					{
						CmDataCapture.Capture( CmDataCapture.MaskInfo, "CheckProcesses():BEFORE WaitForExit");
						this.m_currentExecutable.LocalProcess.WaitForExit();
						//+4.2 provide subtle on-screen volume control
						//if current executable file is volume then set the default volume.
						if (m_currentExecutable.ExecutableName.ToLower().Equals("volume"))
						{
							SetDefaultVolume();
						}
						//-4.2 provide subtle on-screen volume control
						CmDataCapture.Capture( CmDataCapture.MaskInfo, "CheckProcesses():AFTER WaitForExit");
					}
				}

				ActivateLaunchPad(true,true); 

				// In CE, we turned the TopMost attribute of the Window off
				// to show the App.  
				// Otherwise, it never shows. 
				// Now, here, we must turn TopMost on 
				// Launched App has finished and we want to show
				// LaunchPad
#if	( WINCE )
				IntPtr hWnd = PInvoke.FindWindow( "UIControlWindowClass", "LaunchPadNet" );
				PInvoke.SetWindowPos( hWnd,( IntPtr ) PInvoke.SetWindowPosInsertAfter.HWND_TOPMOST,0,0,0,0,(uint)(PInvoke.SetWindowPosFlags.SWP_NOMOVE|PInvoke.SetWindowPosFlags.SWP_NOSIZE)); 
				PInvoke.EnableWindow( hWnd, true );
#endif

				this.m_currentExecutable=null;
				/*
				while(true)

				{
					// dtb... FORMS... We don't do forms! Instead we should sleep a little time
					// so that the CPU will not MAX out due to this while loop.
					//Application.DoEvents();               
					Thread.Sleep(100);
					if (!IsCurrentExecutableProcessAlive() )
					{ 
						ActivateLaunchPad(true,true); 
						this.m_currentExecutable=null;
						break;            
					}                 
				}
				*/
			}
			catch (Exception e)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "CheckProcesses():EXCEPTION " + e.Message);
			}
		}

		//Checking if button is allowed to toggle, even if it's toggleEnabled is set to true,
		//it doesn't mean it can toggle all the time, 
		//there might be some restriction if other button state does not meet its criteria    
		//e.g.  From 'Profile Manager' state cannot change to an 'Exit FastLane and Run Profile Manager' when 
		//    fastlane is not running.
		private void ToggleButton(ref Button button)
		{
			try
			{
				if ( button.ToggleEnabled )
				{
					Button buttonToCheck =(Button)this.m_buttonsTable[button.CurrentAllowToggleOnButton];
					if (buttonToCheck !=null  )
					{ 
						if ( button.ToggleStatus ==false && button.PrimaryAllowToggleOnButtonState!=null ) 
						{
							if ( buttonToCheck.ToggleStatus ==false  && button.PrimaryAllowToggleOnButtonState.Trim().Equals("0" ) )
								button.Toggle();
						}
						else if(button.ToggleStatus ==true && button.PrimaryAllowToggleOnButtonState!=null )
						{
							if ( buttonToCheck.ToggleStatus ==true  && button.PrimaryAllowToggleOnButtonState.Trim().Equals("1" ) )
								button.Toggle();
						}
					}
					else
					{
						button.Toggle();
					}
				}
			}
			catch (Exception e)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "ToggleButton():EXCEPTION " + e.Message);
			}
		}

		public void ShowMainExecutable()
		{
			try
			{
				if ( this.TheApp.Minimized ==false)
				{
					Executable exec= this.m_executablesTable[this.TheApp.CurrentExecutable] as Executable;
					CmDataCapture.Capture( CmDataCapture.MaskError, "ShowMainExecutable() - Maximizing: " + exec.ExecutableName );
					if ( exec!=null)
						exec.MaximizeWindow();
				}
			}
			catch (Exception e)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "ShowMainExecutable():EXCEPTION " + e.Message);
			}
		} 

		//Function for switching between main executable and companion executable vice versa.
		private void SwitchTo()
		{
			try
			{
            
				Executable execToRun=null;
      
				bool stillAlive=false;
                        
				if ( this.TheApp.KillAppOnSwitch )
				{           
					ExecuteFunctionProcessor(this.TheApp.AppExecutable ,"Stop");       
				}
				else
				{
					execToRun =this.TheApp.AppPrevExecutable; //get Previous executable
        
					if ( execToRun!=null && execToRun.LocalProcess !=null && !execToRun.LocalProcess.HasExited )
					{
						stillAlive=true;
						execToRun.MaximizeWindow();           
                
					}   
					if ( this.TheApp.AppExecutable!=null && this.TheApp.AppExecutable.LocalProcess !=null && !this.TheApp.AppExecutable.LocalProcess.HasExited )
					{               
						this.TheApp.AppExecutable.MinimizeWindow();                
						this.TheApp.AppPrevExecutable =this.TheApp.AppExecutable; //save current executable to prevExecutable                                         
					}
					else
					{
						this.TheApp.AppExecutable=null;
					}
        
				}
          
          
				this.TheApp.SwitchTo();                                                        
      
				if ( !this.TheApp.ButtonsTableHolder.ContainsKey(this.TheApp.CurrentContext )) 
				{
					this.m_buttonsTable=CreateButtons();
					this.TheApp.ButtonsTableHolder.Add (this.TheApp.CurrentContext,this.m_buttonsTable );       
				}
				else
				{
					this.m_buttonsTable =(Hashtable)this.TheApp.ButtonsTableHolder[this.TheApp.CurrentContext ];
				}
      
				if(!stillAlive)
				{
					execToRun = this.m_executablesTable[this.TheApp.CurrentExecutable] as Executable;
              
					if (ExecuteFunctionProcessor(execToRun ,"Start"))
					{               
						execToRun.MaximizeWindow();                                           
					}           
				}
      
				this.TheApp.AppExecutable= execToRun;    

				ActivateLaunchPad(true,false);

			}
			catch (Exception e)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "SwitchTo():EXCEPTION " + e.Message);
			}
		}

		private void ActivateMainExecutable()
		{
			try
			{
				Executable exec = this.m_executablesTable[this.TheApp.CurrentExecutable] as Executable;
				this.TheApp.AppExecutable= exec;
				exec.MaximizeWindow();
				this.TheApp.Minimized =false;
				Thread tCheckMainExecutable= new Thread( new ThreadStart(this.CheckMainExecutable));
#if (!WINCE)
				tCheckMainExecutable.Name = "CheckMainExecutable";
#endif
				tCheckMainExecutable.Start();
			}
			catch (Exception e)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "ActivateMainExecutable():EXCEPTION " + e.Message);
			}
		}

		//Current Executable process could be main executable or any other process such as volume or eventviewer
		public bool IsCurrentExecutableProcessAlive()
		{
			try
			{
				if ( this.m_currentExecutable==null )
				{
					CmDataCapture.Capture( CmDataCapture.MaskError, "IsCurrentExecutableProcessAlive(): m_currentExecutable is null - returning false" );
					return false;
				}
				else
				{
					if ( this.m_currentExecutable.LocalProcess == null )
					{
						CmDataCapture.Capture( CmDataCapture.MaskError, "IsCurrentExecutableProcessAlive(): LocalProcess is null - returning false" );
					}

					CmDataCapture.Capture( CmDataCapture.MaskError, "IsCurrentExecutableProcessAlive(): m_currentExecutable is null - returning false" );

					if ( this.m_currentExecutable.LocalProcess!=null && !this.m_currentExecutable.LocalProcess.HasExited )
						return true;
					else
						return false;   
				}
			}
			catch (Exception e)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "IsCurrentExecutableProcessAlive():EXCEPTION " + e.Message);
			}
			return false;
		}

		//Current Executable process could be main executable or any other process such as volume or eventviewer
		public bool IsMainExecutableProcessAlive()
		{
			try
			{
				if ( this.TheApp.AppExecutable==null )
				{
					return false;
				}
				else
				{
					if ( this.TheApp.AppExecutable.LocalProcess!=null && !this.TheApp.AppExecutable.LocalProcess.HasExited )
						return true;
					else
						return false;   
				}
			}
			catch (Exception e)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "IsMainExecutableProcessAlive():EXCEPTION " + e.Message);
			}
			return false;
		}

		public void DisplayLaunchPadNet( bool flag )
		{
			try
			{
				displayingLaunchpad = flag;

				ConfigDataLaunchPad configData = ConfigDataLaunchPad.GetConfigData();

				bool currentAlive;
				if(configData.UtilityReturnAlwaysEnabled)
				{
					currentAlive = false;
					if (flag && CurrentState is MonitoringState)
					{
						ShowCurrentExecutableWindows( true );
					}
					else
					{
						ShowCurrentExecutableWindows( false );
					}
				}
				else
				{
					currentAlive = IsCurrentExecutableProcessAlive();
				}
				CmDataCapture.Capture( CmDataCapture.MaskInfo, "DisplayLaunchPadNet():START flag=" + flag);
				CmDataCapture.Capture( CmDataCapture.MaskInfo, "DisplayLaunchPadNet():IsCurrentExecutableProcessAlive()" + currentAlive.ToString());

				if (flag)
				{
					// Do not allow key changes if Executable is showing   
					if (!currentAlive)
					{
#if	( !WINCE )
						// TAR 325529
						// The ScotInputWindow was flashing on the screen on the first display of LaunchPad.
						Executable exec = this.m_executablesTable[this.TheApp.CurrentExecutable] as Executable;      
						if (( exec!=null) && (!exec.StopWindow.Equals("")))
						{
							IntPtr hWnd = PInvoke.FindWindow(null,exec.StopWindow);
							IntPtr hParentWnd = PInvoke.GetParent( hWnd);
							if( hParentWnd == IntPtr.Zero)
								hParentWnd = hWnd;
							int nFlag = (int) PInvoke.ShowWindowFlags.SW_MINIMIZE; 
							//int nFlag = (int) PInvoke.ShowWindowFlags.SW_HIDE; 
							PInvoke.ShowWindow( hParentWnd, nFlag);
						}
#endif
						if	( !SignedOn )
						{
							if (configData.LegalCaptionRequired)
							{
								SetContext( "LegalCaptionContext", SharedPsx.displayLPStandard, false );
							}
							else
							{
								SetContext( GetExpectedEnterIDContext(), SharedPsx.displayLPStandard, false);
							}
						}
						else
						{
							SetContext( this.TheApp.MainContext, SharedPsx.displayLPStandard, false );
						}

						MinimizeMainExecutable();

#if	( WINCE )					
						IntPtr hWnd = PInvoke.FindWindow( "UIControlWindowClass", "LaunchPadNet" );
						int nFlag = (int) PInvoke.ShowWindowFlags.SW_SHOWNORMAL;
						PInvoke.ShowWindow(hWnd, nFlag);                                            
						PInvoke.SetForegroundWindow(hWnd);
						PInvoke.EnableWindow( hWnd, true );

						IntPtr hWndInsertAfter = IntPtr.Zero;
						hWndInsertAfter = ( IntPtr ) PInvoke.SetWindowPosInsertAfter.HWND_TOPMOST; 
       
						int windowWidth = 240;
						int windowHeight = 320;
						int newXPosition = 0;
						int newYPosition = 0;
						PInvoke.SetWindowPos(hWnd,hWndInsertAfter,newXPosition,newYPosition,windowWidth,windowHeight,(uint)(PInvoke.SetWindowPosFlags.SWP_SHOWWINDOW));                       
						PInvoke.SHFullScreen( hWnd, PInvoke.SHFS_HIDEALL );

						// Hide Window Task Bar
						hWnd = PInvoke.FindWindow("HHTaskBar", null);
						if (hWnd != IntPtr.Zero)
						{
							PInvoke.ShowWindow(hWnd, (int)PInvoke.ShowWindowFlags.SW_HIDE);
						}
#endif

						// TAR294081 Display LaunchPad in front of other applications such as Windows Explorer, etc.
						bool success = PInvoke.SetForegroundWindow(Handle);
						CmDataCapture.Capture( CmDataCapture.MaskInfo, "DisplayLaunchPadNet():SetForegroundWindow=" + success.ToString());
					}
					if( (null != scanner) && (CurrentState is SignOnState) )
					{
						if( IsScannerMsgTargetRunning )
						{
							RequestAccessToScanner();
						}
						else
						{
							scanner.Enable( String.Empty );
						}
					}
				}
				else
				{

					//Get the most Recent Process and show it 
					if ( currentAlive )
					{
						this.m_currentExecutable.MaximizeWindow();              
					}
					else
					{
						ShowMainExecutable();

						// In CE, we must minimize LaunchPad to show the Main App 
						// otherwise, it never shows. 
#if	( WINCE )
						IntPtr hWnd = PInvoke.FindWindow( "UIControlWindowClass", "LaunchPadNet" );
						int nFlag = (int) PInvoke.ShowWindowFlags.SW_MINIMIZE; 
						PInvoke.ShowWindow( hWnd, nFlag);
#endif

						// Automatically sign off when LP is not displayed. Only Sign Off if we signed on
						// from within LP.
						if ( SignedOn && !WasSignOnRemote )
						{
							WasSyncSignOnOff = true;
							LogToReportGenerator( false );
							SignedOn = false;
							// The operator pressed "Sign Off"
							//ActivateLaunchPad(true,false);
						}
					}
					if( (null != scanner) && IsScannerMsgTargetRunning && (scanner is ScannerShared) )
					{
						scanner.Disable();
						string msg = ConfigData.eventRapOperation + "=" + ScannerGeneric.RemoteMessages.ScannerAvailable + ";";
						SendProcessMessage(ScannerMsgTarget, msg);
					}
				}
				CmDataCapture.Capture( CmDataCapture.MaskInfo, "DisplayLaunchPadNet():END");
			}
			catch (Exception e)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "DisplayLaunchPadNet():EXCEPTION " + e.Message);
			}
		}

		//Thread function for KeyLock change
		public void KeyDetect()
		{
			if(0 < autoSignOffSeconds) { autoSignOffCounter = 0; }

			try
			{
				CmDataCapture.Capture( CmDataCapture.MaskInfo, "KeyDetect():START");

				CmDataCapture.Capture( CmDataCapture.MaskInfo, "KeyDetect():BEFORE InitializeKeyDevice()");
				bInitializeKeyDeviceFlag = myself.InitializeKeyDevice();
				CmDataCapture.Capture( CmDataCapture.MaskInfo, "KeyDetect():AFTER InitializeKeyDevice()");

				//CmDataCapture.Capture( CmDataCapture.MaskExtensive, "KeyDetect():BEFORE ActivateLaunchPad()");
				//myself.ActivateLaunchPad(true,!myself.TheApp.LaunchPadSecurityOn);
				//CmDataCapture.Capture( CmDataCapture.MaskExtensive, "KeyDetect():AFTER ActivateLaunchPad()");

				// If we do not have a keyswitch or it does not initialize successfully
				// Do not enter the loop to monitor the keyswitch.
				if ( bInitializeKeyDeviceFlag )
				{

					// SA250050: Commented out this patch since fix was made to set the 
					//           DisplayLaunchPadNet(false) once the main application is activated
					// Temporary fix to display lane/rap on top at start.
					// CmDataCapture.Capture( CmDataCapture.MaskInfo, "KeyDetect():DisplayLaunchPadNet(false)");
					// myself.DisplayLaunchPadNet(false);
#if !WINCE
					// Fix 295556 and 318422
					// 2/24/2006 dtb
					// The pushbutton on a 4.0 system (any system where !ConfigData.HasKeyLockHW) can be set
					// to normal or lock when the system starts. Also, when the system starts the scotapp 
					// application is always displayed. Thus we will ignore the actual status of the keylock
					// and treat the pushbutton as a toggle. This will insure that 2 button press are not
					// requred to display the Utility (LaunchPad) application the first time.
					// NOTE: With this addition, 4.0 systems with a keylock (not pushbutton) will now
					// be treated as a toggle. Thus whenever the key is turned (right or left) the display
					// will switch from Utility to/from scotapp WITHOUT regard to if it is turned right
					// or left. This is ok because these systems should only exist in the QA lab and not in 
					// the field.
					myself.m_keyLock.WaitForKeylockChange( LOCK_KP_LOCK, 100 );		// Do this ONLY to init the myself.m_keyLock.KeyPosition
																					// (when starting myself.m_keyLock.KeyPosition equals 0)
#endif
					while(true)
					{
						try
						{
#if !WINCE  // JZB_In
							// Each time the main application is restarted, we have to readjust the flag.
							if ( ResetBackwardsFlag )
							{
								BackwardsFlag = (!ConfigDataLaunchPad.HasKeyLockHW) && (myself.m_keyLock.KeyPosition!=LOCK_KP_LOCK);
								CmDataCapture.Capture( CmDataCapture.MaskError,  "KeyDetect(): BackwardsFlag = " + BackwardsFlag.ToString());
								ResetBackwardsFlag = false;
							}

							int retValKeyLockChange=-1;
							int nextKeyPosition=(myself.m_keyLock.KeyPosition==LOCK_KP_LOCK ? LOCK_KP_NORMAL : LOCK_KP_LOCK);

							// We need to close the Keylock device when quitting the app. 
							if ( myself.m_bUnload == true )
							{
								break;
							}

							CmDataCapture.Capture( CmDataCapture.MaskExtensive , "KeyDetect():BEFORE WaitForKeylockChange - nextKeyPosition=" + nextKeyPosition + " retValKeyLockChange=" + retValKeyLockChange + " myself.m_keyLock.KeyPosition=" + myself.m_keyLock.KeyPosition + " LOCK_KP_LOCK=" + LOCK_KP_LOCK+" LOCK_KP_NORMAL=" + LOCK_KP_NORMAL);
							// Fix 318078
							// 2/24/2006 dtb
							// Wait 5 seconds (as opposed to 1 second) within the OPOS method because
							// it appears when the FastLane system is running or idle a long time 
							// (someimes memory usage on the system is > 95%) the OPOS method 
							// WaitForKeylockChange does not return for all button presses. It seems 
							// to work much better when giving the OPOS method more time.
							retValKeyLockChange=myself.m_keyLock.WaitForKeylockChange( nextKeyPosition, 5000 );
							CmDataCapture.Capture( CmDataCapture.MaskExtensive , "KeyDetect():AFTER WaitForKeylockChange - nextKeyPosition=" + nextKeyPosition + " retValKeyLockChange=" + retValKeyLockChange + " myself.m_keyLock.KeyPosition=" + myself.m_keyLock.KeyPosition + " LOCK_KP_LOCK=" + LOCK_KP_LOCK+" LOCK_KP_NORMAL=" + LOCK_KP_NORMAL);
							if ( myself.m_bUnload == false && retValKeyLockChange==OPOS_SUCCESS && (myself.IsMainExecutableProcessAlive() || ConfigDataLaunchPad.HasKeyLockHW))
							{
								CmDataCapture.Capture( CmDataCapture.MaskInfo, "KeyDetect():retValKeyLockChange==OPOS_SUCCESS, myself.m_keyLock.KeyPosition=" + myself.m_keyLock.KeyPosition);
								CmDataCapture.Capture( CmDataCapture.MaskInfo, "KeyDetect():DisplayLaunchPadNet( " + (BackwardsFlag ? myself.m_keyLock.KeyPosition == LOCK_KP_LOCK : myself.m_keyLock.KeyPosition != LOCK_KP_LOCK).ToString() + " )");

								// If running on 3.0 hw
								// we assume they have a keylock. 
								// If they have a keylock, log them on when they use it. 
								// Currently, we have no way of knowing definitively 
								// if there is actually a keylock or just a button.  
								// We can only assume that 3.0 HW (only) has a keylock. 
								if	( ConfigDataLaunchPad.HasKeyLockHW )
								{
									if	( myself.m_keyLock.KeyPosition != LOCK_KP_LOCK )
									{
										CmDataCapture.Capture( CmDataCapture.MaskInfo,  "KeyDetect(): KeyLock switch used on 3.0 Hardware - we'll log user on");           
										myself.SetContext( myself.TheApp.MainContext, SharedPsx.displayLPStandard, false );
										// Don't change operator settings if they 
										// are already signed on.  
										if	( !myself.signedOn )
										{
											myself.OperatorId = myself.GetString("KeySwitchOperatorID");	// Tar 343053
											myself.OperatorRights = 1;
											myself.LogToReportGenerator( true );
											myself.OperatorId = "1";										
											myself.signedOn = true;
											myself.WasSignOnRemote = false;
											myself.SetConfigProperty( "SignOnButton", myself.TheApp.MainContext, Psx.PropertyTextFormat, myself.signedOn ? myself.GetString( "SignOffButtonText" ) : myself.GetString( "SignOnButtonText" ) );
											//myself.ActivateLaunchPad(true,true);
										}
									}
								}												
								bool displayLaunchpadNetFlag =  ( BackwardsFlag ? myself.m_keyLock.KeyPosition == LOCK_KP_LOCK : myself.m_keyLock.KeyPosition != LOCK_KP_LOCK );

								// In the scotapp assist mode, sometimes the POS solution window is displayed on top
								// of the regular scotapp window. If that happens, and the keylock button is pressed
								// the customer will want to return to the POS window on the next button press. Save
								// the handle of that window so that the window can be redisplayed on the next button press.
								// After the button is pressed, restore the window pointer to zero.
								if ( displayLaunchpadNetFlag )
								{
									bool currentAlive = IsCurrentExecutableProcessAlive();
									// Do not allow key changes if Executable is showing   
									if (!currentAlive)
									{
										if ( this.TheApp.ForegroundWindowTitle != String.Empty )
										{
											IntPtr foreWnd = PInvoke.FindWindow(null,this.TheApp.ForegroundWindowTitle);

											if ( foreWnd != IntPtr.Zero )
											{
												CmDataCapture.Capture( CmDataCapture.MaskError, "KeyDetect(): ForegroundWindowTitle - foreWnd=\'" + foreWnd.ToString() + "\'");
												if (PInvoke.IsWindowVisible(foreWnd))
													{
														CmDataCapture.Capture( CmDataCapture.MaskError, "KeyDetect(): ForegroundWindowTitle - ForegroundWindowHandle SET=\'" + foreWnd.ToString() + "\'");
														this.TheApp.AppExecutable.ForegroundWindowHandle = foreWnd;

														// Hide the window because the Retalix POS has a timer that
														// redisplays it on top of all windows every minute.
														int nFlag= (int) PInvoke.ShowWindowFlags.SW_HIDE;
														PInvoke.ShowWindow( this.TheApp.AppExecutable.ForegroundWindowHandle, nFlag);  
													}
											}
										}
									}
								}

								myself.DisplayLaunchPadNet( displayLaunchpadNetFlag );
							}
#endif
						}
						catch(Exception ex)
						{       
							CmDataCapture.Capture( CmDataCapture.MaskError,  String.Format("<Error> [MyApplication.KeyDetect()] KeyLock Device Error:{0}",ex.Message));           
						}
					}
				}

				CmDataCapture.Capture( CmDataCapture.MaskInfo, "KeyDetect():END");
			}
			catch (Exception e)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "KeyDetect():EXCEPTION " + e.Message);
			}
		}   

		//Initialization of OPOS Keylock device
		private bool InitializeKeyDevice() //Modified for TAR370130
		{
			bool deviceStatus=false;
			long healthStatus;

			try
			{	
				CmDataCapture.Capture( CmDataCapture.MaskInfo, "InitializeKeyDevice():START");
      
#if !WINCE  // JZB_In
				int retries=0;
				while(!deviceStatus && retries < DEVICE_READ_MAX_RETRIES )
				{
					try
					{
						m_keyLock= new OPOSKeylockClass();
						CmDataCapture.Capture( CmDataCapture.MaskInfo, "InitializeKeyDevice():OPOSKeylockClass Attempt Open:" + this.TheApp.KeyLock);
						if (m_keyLock.Open(this.TheApp.KeyLock)==0)
						{
							CmDataCapture.Capture( CmDataCapture.MaskInfo, "InitializeKeyDevice():OPOSKeylockClass Open - SUCCESS");
							m_keyLock.ClaimDevice(0);
 			                m_keyLock.DeviceEnabled=true; 
							healthStatus=m_keyLock.CheckHealth(1);//1 = OPOS_CH_INTERNAL
				            if(healthStatus==OPOS_SUCCESS)
				            {
				               CmDataCapture.Capture( CmDataCapture.MaskInfo, "InitializeKeyDevice():OPOSKeylockClass HealthCheck Success!");                   
				               deviceStatus=true;    
				               break;
				            }
				            else
				            {
				               CmDataCapture.Capture( CmDataCapture.MaskInfo, String.Format ("InitializeKeyDevice():OPOSKeylockClass HealthCheck failed:{0}", healthStatus));  
                               m_keyLock.Close(); 
				            }
						}
						else
						{
							CmDataCapture.Capture( CmDataCapture.MaskInfo, "InitializeKeyDevice():OPOSKeylockClass Open - FAILURE");
						}
		                for(int x=0;x<20;x++)
				        {
           				   if ( myself.m_bUnload == true )
						   {
							  CmDataCapture.Capture( CmDataCapture.MaskInfo, "InitializeKeyDevice() App Closing, exit loop");
				              retries=DEVICE_READ_MAX_RETRIES;
							  break;
						   }
		                   Thread.Sleep(1000);
		                }				               
				    }       
					catch(Exception ex)
					{
						// dtb OLD MessageBox.Show(this,ex.Message );
						CmDataCapture.Capture( CmDataCapture.MaskError,  String.Format("<Error> [MyApplication.KeyDetect()] KeyLock Device Error:{0}",ex.Message));           
						break;
					}       
					finally
					{
						CmDataCapture.Capture( CmDataCapture.MaskInfo, "InitializeKeyDevice():finally");

						// IMPORTANT: We ALWAYS get here... Even when SUCCESS!
						if (deviceStatus==false)
						{
							retries++;
						}
					}
				}

#endif      
				if (deviceStatus==false)
				{
					CmDataCapture.Capture( CmDataCapture.MaskError,  "<Error> [MyApplication.KeyDetect()] Error opening KeyLock Device: " + this.TheApp.KeyLock );     
					// dtb OLD this.Close();
					// dtb NEW
					//Exit();
				}
			}
			catch (Exception e)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "InitializeKeyDevice():EXCEPTION " + e.Message);
			}

			CmDataCapture.Capture( CmDataCapture.MaskInfo, "InitializeKeyDevice():END");
			return deviceStatus;
		}

		/// <summary>
		/// Reset the signon contexts to not be maintenance 
		/// </summary>
	
		public void ResetSignonContexts()
		{
			SetContextCustomDataVariable( "State", SignOn, "EnterID" );
			SetContextCustomDataVariable( "State", SignOn, "EnterPassword" );
			SetContextCustomDataVariable( "State", SignOn, "EnterAlphaNumericID" );
			SetContextCustomDataVariable( "State", SignOn, "EnterAlphaNumericPassword" );
			SetContextCustomDataVariable( "State", SignOn, "ScanIDOrKey" );
			SetContextCustomDataVariable( "State", SignOn, "ScanIDOnly" );
			SetContextCustomDataVariable( "State", SignOn, "EnterPasswordOrUseBiometricFromScanIDOrKey" );
			SetContextCustomDataVariable( "State", SignOn, "EnterPasswordOrUseBiometricFromScanIDOnly" );
		}

		public void MaintExecuteFunction( string function )
		{
			this.m_sCurrentButtonPressed = function;
			ExecuteFunctions();
		}

		/// <summary>
		/// Shows the window of the current executable if flag is true, otherwise hidea all windows.
		/// </summary>
		public void ShowCurrentExecutableWindows( bool flag )
		{	
			try
			{
				if(m_currentExecutable != null)
				{
					IntPtr hWnd;
					int nFlag;
					String []windowTitles = m_currentExecutable.WindowTitle.Split(',');

					if( flag  && displayingLaunchpad )
					{
						for ( int count = 0; count<windowTitles.Length ; count++ )
						{
							hWnd = PInvoke.FindWindow( null, windowTitles[count] );

							if(hWnd != IntPtr.Zero)
							{
								nFlag = (int) PInvoke.ShowWindowFlags.SW_SHOWNORMAL;
								PInvoke.ShowWindow(hWnd, nFlag);
								break;
							}
						}
					}
					else
					{
						for ( int count = 0; count<windowTitles.Length ; count++ )
						{
							hWnd = PInvoke.FindWindow( null, windowTitles[count] );

							if(hWnd != IntPtr.Zero)
							{
								nFlag = (int) PInvoke.ShowWindowFlags.SW_HIDE;
								PInvoke.ShowWindow(hWnd, nFlag);
							}
						}
					}
				}
			}
			catch (Exception e)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "showExecutables():EXCEPTION " + e.Message);
			}
		}

		/// <summary>
		/// Determines the appropriate User/ID login screen to be displayed based on configuration 
		/// </summary>
		public string GetExpectedEnterIDContext()
		{
			ConfigData configData = ConfigDataLaunchPad.GetConfigData();
			string context = "";

			if( configData.AllowEmplIdSignOn )
			{
				if( configData.AllowHandKeyEMPLID || !IsMainExecutableProcessAlive() ) 
				{
					context = "ScanIDOrKey";
				}
				else
				{
					context = "ScanIDOnly";
				}
			}
			else
			{
				if( configData.AlphaNumericKeyboard )
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
			ConfigData configData = ConfigDataLaunchPad.GetConfigData();
			string context = "";

			// Use the AllowEmplIdSignOn flag to determine the context
			// the biometrics flag should only determine the leadthru
			if( configData.AllowEmplIdSignOn )
			{
				if( configData.AllowHandKeyEMPLID || !IsMainExecutableProcessAlive() ) 
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
				if( configData.AlphaNumericKeyboard )
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
			ConfigData configData = ConfigDataLaunchPad.GetConfigData();
			string leadthru = "";

			if( configData.AllowEmplIdSignOn )
			{
				if( configData.AllowHandKeyEMPLID || !IsMainExecutableProcessAlive() ) 
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
			ConfigData configData = ConfigDataLaunchPad.GetConfigData();
			string leadthru = "";

			if( configData.UseBiometrics )
			{
				leadthru = "EnterPasswordOrUseBiometricLeadthru";
			}
			else
			{
				leadthru = "EnterPasswordLeadthru";
			}

			return leadthru;
		}


		public App TheApp
		{
			get
			{
				return theApp;
			}
			set
			{
				theApp= value;
			}
		}

		/// <summary>
		/// Set to true when confirmation to a functon 
		/// was asked and user said yes.
		/// Set to no if user not yet asked or user declined
		/// Defaults to no at start 
		/// Reset to no after approved confirmation starts the function, this 
		/// will force the confirmation question to be asked again next time.
		/// </summary>
		public bool ConfirmationApproved
		{
			get
			{
				return confirmationApproved;
			}
			set
			{
				confirmationApproved = value;
			}
		}

		/// <summary>
		/// Call the report generator utility to generate a Sign On/Off xml file and to transfer the file to the Report Server.
		/// </summary>
		/// <param name="OnOffFlag"></param>
		/// <returns></returns>
		public void LogToReportGenerator( bool OnOffFlag )
		{
			string path = ConfigDataLaunchPad.GetConfigData().BinPath + "\\";
			string filename = "CreateReportFile.exe";
			string arguments = "";
	
#if WINCE
			arguments += "/R"; // TAR 348682. Assumption: HH is always a RAP terminal. 
#else
    		arguments += (ConfigDataLaunchPad.GetConfigData().ValidateRapTerminalRegistry() ? "/R" : "");// RAP terminal?
#endif
			arguments += " /C";																	// Copy to report server now
			arguments += " /P=" + ConfigDataLaunchPad.GetConfigData().ReportDataPath;					// Path to copy report data file (typically c:\scot\report)
			arguments += " /T=OperatorLogin";													// OperatorLogins database table
			arguments += " /F=Operator:SignOn:Application";										// Field Names
			arguments += " /V=" + OperatorId + ":" + ( OnOffFlag ? "1" : "0") + ":" + "UTILITY";// Values to match field names

			CmDataCapture.Capture( CmDataCapture.MaskInfo, "LogToReportGenerator():START");
			try
			{
				ProcessStartInfo startInfo = new ProcessStartInfo(path + filename);
				startInfo.Arguments = arguments;		//arguments;
				startInfo.WindowStyle = ProcessWindowStyle.Hidden;

				Process locProcess = new Process();
				locProcess.StartInfo = startInfo;
				CmDataCapture.Capture( CmDataCapture.MaskError, String.Format("LogToReportGenerator() About to start process..." + path + filename + " " + arguments ));
				locProcess.Start();

/* I'm not sure we want this...
				// Let executable complete before continuing...
				if ( locProcess != null && !locProcess.HasExited )
				{
					locProcess.WaitForExit();
				}
*/
			}
			catch( Exception ex)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, String.Format("<Exception> [LogToReportGenerator()] Error starting process: {0} EXCEPTION:{1}", path + filename + " " + arguments, ex.Message ));
			}
			CmDataCapture.Capture( CmDataCapture.MaskInfo, "LogToReportGenerator():END");
		}

#if ( WINCE )

		// On the handheld a signal file will tell LP to shutdown 
		// and shutdown Rap so an upgrade install can run. 
		// 
		/// <summary>
		/// Check to see if the update tag file
		/// is on the unit.  If so, quit the main app
		/// and LaunchPad to allow the update.  
		/// </summary>
		private void UpdateFlagCheck()
		{
			if	( File.Exists( UpdateSignalFile ) )
			{
				ActivateLaunchPad(false,false); 
				CmDataCapture.Capture( CmDataCapture.MaskAll, "Stopping All Apps for code update" );

				// Shutdown the main app ( RAP )

				if	( IsMainExecutableProcessAlive() )
				{
					Executable exec = this.m_executablesTable[this.TheApp.CurrentExecutable] as Executable;
					if	( exec != null )
					{
						exec.Execute( "stop" );
					}
					this.TheApp.AppExecutable=null;
				}

				MyApplication.GetApplication().Exit();
			}
		}


		private static string updateFilePath = "\\scot\\bin";
		private static string signalBaseName = "UpdSig.dat";
		private static string UpdateSignalFile = updateFilePath + signalBaseName;

		private void DeleteUpdateFlagFile( string binPath )
		{
			updateFilePath = binPath;
			UpdateSignalFile = updateFilePath + "\\" + signalBaseName; 
			if	( File.Exists( UpdateSignalFile ) )
			{
				File.Delete( UpdateSignalFile );
			}
		}
#endif

		/// <summary>
		/// Starts the biometric.
		/// </summary>		
		private bool StartBiometric()
		{
			ConfigData configData = ConfigDataLaunchPad.GetConfigData();
			bool bRet = false; 
			
			
			if( configData.BiometricDeviceConfigured )
			{
				if( null != biometric )
				{
					biometric.Dispose();
					biometric = null;
				}
				biometric = new Biometric( this );
			
				if( null != Biometric ) 
				{									
					Biometric.BiometricEvent += new BiometricDelegate(Biometric_BiometricEvent);
				}							
				
				bRet = Biometric.Open();
				if( bRet )
				{
					Biometric.BiometricDeviceConfigured = true; 
				}
			}
			return bRet; 
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

		public bool ResetBackwardsFlag
		{
			get
			{
				return resetBackwardsFlag;
			}
			set
			{
				resetBackwardsFlag= value;
			}
		}
		private static bool resetBackwardsFlag = true;

		public bool BackwardsFlag
		{
			get
			{
				return backwardsFlag;
			}
			set
			{
				backwardsFlag= value;
			}
		}
		private static bool backwardsFlag = false;

		public bool DisplayingLaunchpad
		{
			get
			{
				return displayingLaunchpad;
			}
		}

		//OPOS
		private const int OPOS_SUCCESS=0;

		public const int LOCK_KP_LOCK=1;
		public const int LOCK_KP_NORMAL=2;
		public const int LOCK_KP_SUPERVISOR=3;
		private const int DEVICE_READ_MAX_RETRIES=60; //TAR370130
		private bool m_bUnload=false;
#if !WINCE  // JZB_In
		public OPOSKeylockClass m_keyLock;
#endif

		private App theApp;
		private Hashtable m_buttonsTable=null;  
		private Hashtable m_executablesTable=null;

		private String m_sCurrentButtonPressed;
		public Executable m_currentExecutable=null;  //current Executables other than the main executable (i.e. volume,event viewer, etc...)
		// dtb END

		/// <summary>
		/// Current state.
		/// </summary>
		private LaunchPadNet.State currentState = null;
		
		/// <summary>
		/// Exclusive lane number.
		/// </summary>
		private int exclusiveLane = -1;
        
		/// <summary>
		/// Array of lane specific Psx instances.
		/// </summary>
		//private LanePsx[] lanePsxArray;		

		/// <summary>
		/// Size of the area that is reserved for RapSingle contexts.
		/// </summary>
		private Rectangle laneViewPosition = new Rectangle( 0, 0, 0, 0 );
        
		/// <summary>
		/// Id of currently signed on operator.
		/// </summary>
		private string operatorId = "";

		/// <summary>
		/// Id of the terminal.
		/// </summary>
		private string terminalId = "";

		/// <summary>
		/// Interface version of the terminal.		Rfc 343789
		/// </summary>
		private string interfaceVersion = "";

		/// <summary>
		/// Checks whether or not the last signon or signoff was 
		/// performed automatically by synching Launchpad/RapNet
		/// </summary>
		public bool wasSyncSignOnOff = false;
		
		/// <summary>
		/// Checks whether or not the sign On was from a remote application (from RapNet or Fastlane).
		/// </summary>
		public bool wasSignOnRemote = false;

		/// <summary>
		/// Id of currently signed on operator.
		/// </summary>
		private uint operatorRights = 0;
        
		/// <summary>
		/// Encapsulates our scanner object.
		/// </summary>
		private ScannerGeneric scanner = null;

		/// <summary>
		/// Timer that shows the time on the Rap UI.
		/// </summary>
		private Timer showTimeTimer = null;

		private int autoSignOffSeconds = 0;

		/// <summary>
		/// Flag indicating if we are signed on.
		/// </summary>
		private bool signedOn = false;
		
		/// <summary>
		/// Name of currently active host.
		/// </summary>
		private string activeHost = "";

        /// <summary>
        /// Count in seconds since last heartbeat.
        /// </summary>
        private int heartbeatCounter = 0;

		/// <summary>
		/// Me
		/// </summary>
		public static SharedPsx myself = null;

		/// <summary>
		/// Encapsulates our Biometric object.
		/// </summary>
		private Biometric biometric = null;

		/// <summary>
		/// Display used for Launchpad.
		/// </summary>
		public const uint displayLPStandard = Psx.DisplayAlternate3;

		/// <summary>
		/// If confirmation of function was not approved
		/// </summary>
		private bool confirmationApproved = false;

		//TAR 409851
		private bool bInitializeKeyDeviceFlag = false;
		/// <summary>
		/// If Launchpad is currently being displayed
		/// </summary>
		private bool displayingLaunchpad = false;

		const string SignOn = "SignOn";
		const string MaintSignOn = "MaintSignOn";
#if	( !WINCE )
		const string WindowsSignOn = "WindowsSignOn";
#endif

		private bool maintenanceSignOnFailed = false;

		public bool MaintenanceSignOnFailed
		{
			get { return maintenanceSignOnFailed; }
			set { maintenanceSignOnFailed = value; }
		}

		private string maintButtonPressed = ""; 

		public string MaintButtonPressed 
		{
			get { return maintButtonPressed; }
			set { maintButtonPressed = value; }
		}

#if	( !WINCE )
		private bool windowsSignOnFailed = false;
		private string windowsSignOnId = "";
		private string windowsSignOnPassword = "";

		public bool WindowsSignOnFailed
		{
			get { return windowsSignOnFailed; }
			set { windowsSignOnFailed = value; }
		}

		public string WindowsSignOnId
		{
			get { return windowsSignOnId; }
			set { windowsSignOnId = value; }
		}

		public string WindowsSignOnPassword
		{
			get { return windowsSignOnPassword; }
			set { windowsSignOnPassword = value; }
		}

		private string windowsButtonPressed = ""; 

		public string WindowsButtonPressed 
		{
			get { return windowsButtonPressed; }
			set { windowsButtonPressed = value; }
		}
#endif

		//******************************************************
		//Method : SetDefaultVolume
		//Purpose: Get the volume level and write the value to registry
		//Parameters:  
		//Return Values: 
		//******************************************************
		private void SetDefaultVolume()
		{
			uint hkey = 0;
			int nLeftVolume = 0;
			int nRightVolume = 0;
			Psx.GetVolume(out nLeftVolume, out nRightVolume );
			if( 0 == PInvoke.RegOpenKeyExW( PInvoke.HKEY_LOCAL_MACHINE, "SOFTWARE\\NCR\\LaunchPadNet", 0, PInvoke.KEY_SET_VALUE, ref hkey ) )
			{
					byte[] bVolume = System.BitConverter.GetBytes(nLeftVolume);
					PInvoke.RegSetValueExW(hkey, "VolumeLevel", 0, PInvoke.REG_DWORD, bVolume , (uint)bVolume.GetLength(0));
			}
			PInvoke.RegCloseKey(hkey);			
			
		}

		
		private void Biometric_BiometricEvent(object sender, BiometricArgs e)
		{			
			try
			{								
				switch( e.Status )
				{
					case (int)FT_STATUS_CODES.FT_UNSOLICITED_INFO:
					switch ( e.Param1 )
					{
						case (int)BIOMETRIC_RC.RC_DEVICE_PLUGGED:
							if ( null != biometric )
							{
								biometric.DeviceReady = true;
							}
							break;
						case (int)BIOMETRIC_RC.RC_DEVICE_UNPLUGGED:
							if ( null != biometric )
							{
								biometric.DeviceReady = false;
							}
							break;
						case (int)BIOMETRIC_RC.RC_SERVER_READY:
							if ( null != biometric )
							{
								biometric.ServerReady = true;
							}								
							break;
						case (int)BIOMETRIC_RC.RC_SERVER_NOTREADY:
							if ( null != biometric )
							{
								biometric.ServerReady = false;
							}								
							break;
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

		//+Auto sign off feature

		/// <summary>
		/// Auto sign off counter in seconds.
		/// </summary>
		public static int autoSignOffCounter = 0;

		public int AutoSignOffCounter
		{
			get
			{
				return autoSignOffCounter;
			}
			set
			{
				autoSignOffCounter = value;				
			}
		}

		/// <summary>
		/// Auto sign off flag.
		/// </summary>
		private bool wasAutoSignedOff = false;

		// desktop specific hooks
#if ( !WINCE )
		/// <summary>
		/// Monitors low-level mouse and keyboard events.
		/// </summary>		
		private static void SetMouseKeyboardHook()
		{
			using (Process curProcess = Process.GetCurrentProcess())
			using (ProcessModule curModule = curProcess.MainModule)
			{
				_mousehookID = PInvoke.SetWindowsHookEx(PInvoke.WH_MOUSE_LL, _mouseproc, PInvoke.GetModuleHandle(curModule.ModuleName), 0);
				_keyboardhookID = PInvoke.SetWindowsHookEx(PInvoke.WH_KEYBOARD_LL, _keyboardproc, PInvoke.GetModuleHandle(curModule.ModuleName), 0);
			}
		}

		private static PInvoke.LowLevelMouseKeyboardProc _keyboardproc = new PInvoke.LowLevelMouseKeyboardProc(KeyboardHookCallback);
		private static PInvoke.LowLevelMouseKeyboardProc _mouseproc = new PInvoke.LowLevelMouseKeyboardProc(MouseHookCallback);

		private static IntPtr _mousehookID = IntPtr.Zero;
		private static IntPtr _keyboardhookID = IntPtr.Zero;

		private static IntPtr MouseHookCallback(int nCode, IntPtr wParam, IntPtr lParam)
		{
			if (nCode >= 0 &&
				(PInvoke.MouseMessages.WM_LBUTTONDOWN == (PInvoke.MouseMessages)wParam.ToInt32() ||
				PInvoke.MouseMessages.WM_LBUTTONUP == (PInvoke.MouseMessages)wParam.ToInt32() ||
				PInvoke.MouseMessages.WM_RBUTTONDOWN == (PInvoke.MouseMessages)wParam.ToInt32() ||
				PInvoke.MouseMessages.WM_RBUTTONUP == (PInvoke.MouseMessages)wParam.ToInt32() ||
				PInvoke.MouseMessages.WM_MOUSEMOVE == (PInvoke.MouseMessages)wParam.ToInt32() ) )
			{
				PInvoke.MSLLHOOKSTRUCT hookStruct = (PInvoke.MSLLHOOKSTRUCT)Marshal.PtrToStructure(lParam, typeof(PInvoke.MSLLHOOKSTRUCT));
				
				autoSignOffCounter = 0;
			}
			return PInvoke.CallNextHookEx(_mousehookID, nCode, wParam, lParam);
		}

		private static IntPtr KeyboardHookCallback(int nCode, IntPtr wParam, IntPtr lParam)
		{
			if (nCode >= 0 && wParam == (IntPtr)PInvoke.WM_KEYDOWN)
			{
				int vkCode = Marshal.ReadInt32(lParam);
				
				autoSignOffCounter = 0;
			}
			return PInvoke.CallNextHookEx(_keyboardhookID, nCode, wParam, lParam);
		}
#endif
		//-Auto sign off feature

		/// <summary>
		/// Process has requested we let go of the scanner.
		/// Release the device and respond.
		/// </summary>
		public void ProcessReleaseScannerRequest()
		{
			CmDataCapture.Capture( CmDataCapture.MaskInfo, "ProcessReleaseScannerRequest()");
			if( Scanner is ScannerShared )
			{
				if( null != scanner )
				{
					scanner.Disable();
				}
				string msg = ConfigData.eventRapOperation + "=" + ScannerGeneric.RemoteMessages.ScannerAvailable + ";";
				SendProcessMessage(ScannerMsgTarget, msg);
			}
			else
			{
				CmDataCapture.Capture( CmDataCapture.MaskWarning, "ProcessReleaseScannerRequest(): Scanner sharing is not enabled. Ignoring remote message.");
			}
		}

		/// <summary>
		/// Received remote message, that the scanner is now available. 
		/// Process if we're in a valid state
		/// </summary>
		public void ProcessResponseScannerAvailable()
		{
			CmDataCapture.Capture( CmDataCapture.MaskInfo, "ProcessResponseScannerAvailable()");
			if( Scanner is ScannerShared )
			{
				ClearScannerMsgTimer();
				if( waitingForScannerMsgResponse )
				{
					waitingForScannerMsgResponse = false;
					if( null != scanner )
					{
						if( !scanner.Enable(String.Empty) )
						{
							// Go ahead and send a failure remote message to originator.
							string msg = ConfigData.eventRapOperation + "=" + ScannerGeneric.RemoteMessages.ScannerOpenFailed + ";";
							SendProcessMessage(ScannerMsgTarget, msg);
							CmDataCapture.Capture( CmDataCapture.MaskError, "ProcessReceivedScannerAvailable(): Attempted to open scanner and failed.");
						}
					}
				}
			}
			else
			{
				CmDataCapture.Capture( CmDataCapture.MaskWarning, "ProcessResponseScannerAvailable(): Scanner sharing is not enabled. Ignoring remote message.");
			}
		}

		/// <summary>
		/// Process biometric data received 
		/// </summary>
		public void ProcessBiometricData(string encodedData)
		{
			CmDataCapture.Capture( CmDataCapture.MaskInfo, "ProcessBiometricData()");
			if( ConfigDataLaunchPad.GetConfigData().UseBiometrics )
			{
				if( CurrentState is SignOnState && DisplayingLaunchpad )
				{
					string context = GetContext(SharedPsx.displayLPStandard);
					if( context == GetExpectedPasswordContext() )
					{
                        SetControlProperty(Psx.ControlDisplay, Psx.PropertyUserInput, false);
                        SetControlProperty("Leadthru", Psx.PropertyText, GetString("LoggingInLeadthru"));
						(CurrentState as SignOnState).EnterPassword(encodedData);
					}
					else
					{
						CmDataCapture.Capture( CmDataCapture.MaskWarning, "ProcessBiometricData(): Must be in password context to process biometric data, ignoring biometric event.");
					}
				}
				else
				{
					CmDataCapture.Capture( CmDataCapture.MaskWarning, "ProcessBiometricData(): Either we are not in a SignOn state and/or LaunchPad is not in the foreground. Ignoring biometric event.");
				}
			}
			else
			{
				CmDataCapture.Capture( CmDataCapture.MaskWarning, "ProcessBiometricData(): Biometric functionality is not enabled, ignoring fingerprint event.");
			}
		}

		/// <summary>
		/// Request the scanner be made available for LaunchPad to claim it.
		/// Method is intelligent and will work only when ScannerShared class is being used
		/// </summary>
		public void RequestAccessToScanner()
		{
			CmDataCapture.Capture( CmDataCapture.MaskError, "RequestAccessToScanner()");
			if( !scanner.ScannerEnabled )
			{
				if( scanner is ScannerShared )
				{
					// Send request for target to let go of scanner
					waitingForScannerMsgResponse = true;
					string msg = ConfigData.eventRapOperation + "=" + ScannerGeneric.RemoteMessages.ReleaseScanner + ";";
					SendProcessMessage(ScannerMsgTarget, msg);
					// Initialize the remote message timeout.
					lock(scannerMsgTimerLock)
					{
						scannerMsgTimer = new System.Threading.Timer(new TimerCallback(ScannerMsgTimeout), null, scannerMsgTimeout, Timeout.Infinite);
					}
				}
				else
				{
					CmDataCapture.Capture( CmDataCapture.MaskError, 
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
		private void ScannerMsgTimeout( object state )
		{
			ClearScannerMsgTimer();
			if( waitingForScannerMsgResponse )
			{
				CmDataCapture.Capture( CmDataCapture.MaskWarning, "ScannerMsgTimeout()");
				waitingForScannerMsgResponse = false;
				
				// We requested the scanner and didn't get a response. Try to enable it
				if( null != scanner )
				{
					CmDataCapture.Capture( CmDataCapture.MaskWarning, "ScannerMsgTimeout(): Didn't receive response from scanner sharing app. Attempting to open scanner anyway.");
					if( !scanner.Enable(String.Empty) )
					{
						if( scanner is ScannerShared )
						{
							// Go ahead and send a failure remote message to originator.
							string msg = ConfigData.eventRapOperation + "=" + ScannerGeneric.RemoteMessages.ScannerOpenFailed + ";";
							SendProcessMessage(ScannerMsgTarget, msg);
						}
						CmDataCapture.Capture( CmDataCapture.MaskError, "ScannerMsgTimeout(): Attempted to open scanner and failed");
					}
				}
			}
		}

		/// <summary>
		/// Wrapper for RemoteMessage library function 
		/// SendProcessMessage(). Adds tracing.
		/// </summary>
		public bool SendProcessMessage(string target, string message)
		{
			CmDataCapture.Capture(CmDataCapture.MaskExtensive, "SendProcessMessage(" + target + ", " + message + ")");
			return MyApplication.SendProcessMessage(target, message);
		}

		/// <summary>
		/// Method is used to clear the Scanner Message Timer
		/// </summary>
		public void ClearScannerMsgTimer()
		{
			CmDataCapture.Capture(CmDataCapture.MaskExtensive, "ClearScannerMsgTimer()");
			lock(scannerMsgTimerLock)
			{
				if( scannerMsgTimer != null )
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
				if( scannerMsgTarget == "" )
				{
					// determine the target
					if( ConfigDataLaunchPad.GetConfigData().ValidateRapTerminalRegistry() )
					{
						scannerMsgTarget = ConfigDataLaunchPad.rapApplication;
					}
					else
					{
						// determine the SCOTAPP process that exists
						string [] scotAppList = new string[]{"SCOTAPP.EXE", "SCOTAPPD.EXE", "SCOTAPPU.EXE", "SCOTAPPUD.EXE"};
						foreach(string scotApp in scotAppList)
						{
							if( Process.GetProcessesByName(Path.GetFileNameWithoutExtension(scotApp)).Length > 0 )
							{
								scannerMsgTarget = scotApp;
								break;
							}
						}
					}
				}
				return scannerMsgTarget;
			}
		}

		public bool IsScannerMsgTargetRunning
		{
			get
			{
				if( ScannerMsgTarget != "" )
				{
					if( Process.GetProcessesByName(Path.GetFileNameWithoutExtension(ScannerMsgTarget)).Length > 0 )
					{
						return true;
					}
				}
				return false;
			}
		}

		private const string startupContext = "StartupContext";

		/// <summary>
		/// Timer that waits for RemoteMessage response from Rap or SCOTAPP
		/// </summary>
		public System.Threading.Timer scannerMsgTimer = null;

		/// <summary>
		/// Timer for RemoteMessage timeout locking object
		/// </summary>
		public object scannerMsgTimerLock = new object();

		/// <summary>
		/// Target for scanner remote messages
		/// </summary>
		protected string scannerMsgTarget = "";
		
		/// <summary>
		/// Timeout in ms for Scanner remote message response
		/// </summary>
		protected int scannerMsgTimeout = 5000;

		/// <summary>
		/// Waiting for Scanner remote message response
		/// </summary>
		public bool waitingForScannerMsgResponse = false;
	}
}
