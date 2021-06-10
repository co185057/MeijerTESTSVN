using System;
using System.Threading;
using System.Runtime.InteropServices;
using PsxNet;
using RPSWNET;
using CommonLibsNet;

namespace LaunchPadNet
{
    /// <summary>
    /// 
    /// </summary>
    internal class WindowsSignOnState : SignOnState
    {
		string m_stateType = "WindowsSignOn";

		private bool   m_windowsSignedOn = false;

        /// <summary>
        /// The class constructor.
        /// </summary>
        /// <param name="paramPsx">
        /// SharedPsx instance used to connect.
        /// </param>
        public WindowsSignOnState( SharedPsx paramPsx ) : base( paramPsx, "EnterAlphaNumericID", "EnterAlphaNumericPassword" )
        {	
			m_windowsSignedOn = false;
        }

        /// <summary>
        /// Releases the resources used by the SignOnState instance.
        /// </summary>
        public override void Dispose()
		{
			base.Dispose();
        }

		/// <summary>
		/// Validate the ID/Password against the Windows logon.
		/// </summary>
		public bool ValidateOperatorWindowsLogon(string sUsername, string sDomain, string sPassword)
		{
			bool rVal = false;

			// initialize tokens
			IntPtr pExistingTokenHandle = new IntPtr(0);
			pExistingTokenHandle = IntPtr.Zero;
    
			// if domain name was blank, assume local machine
			if (sDomain == "")
			{
				ConfigData configData = ConfigDataLaunchPad.GetConfigData();
				sDomain = configData.ComputerName;
			}

			try
			{
				string sResult = null;

				const int LOGON32_PROVIDER_DEFAULT = 0;

				// create token
				const int LOGON32_LOGON_INTERACTIVE = 2;

				// show LogonUser id attempt	          
				CmDataCapture.Capture( CmDataCapture.MaskError, "ValidateOperatorWindowsLogon(): attempt id=" + sUsername);

				// Do not allow Logon Windows of the same user currently logged on.
				string currentUser = System.Environment.UserName;
				if (currentUser.ToLower() == sUsername.ToLower() )
				{
					// show LogonUser failed because it is the same user currently logged on.	          
					CmDataCapture.Capture( CmDataCapture.MaskError, "ValidateOperatorWindowsLogon(): failed - " + sUsername + " is already logged on." );

					return false;
				}

				// get handle to token
				bool bImpersonated = PInvoke.LogonUser(sUsername, sDomain, sPassword, 
					LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, 
					ref pExistingTokenHandle);

				// did impersonation fail?
				if (false != bImpersonated)
				{
					// Successful Login...
					rVal = true;

					// show LogonUser succeeded	          
					CmDataCapture.Capture( CmDataCapture.MaskError, "ValidateOperatorWindowsLogon(): succeeded" );

				}
				else
				{
					int nErrorCode = Marshal.GetLastWin32Error();
					sResult = " failed with error code: " + nErrorCode;

					// show the reason why LogonUser failed	          
					CmDataCapture.Capture( CmDataCapture.MaskError, "ValidateOperatorWindowsLogon():" + sResult );
				}
			}
			catch (Exception e)
			{
				// Log exception...
				CmDataCapture.Capture( CmDataCapture.MaskError, "ValidateOperatorWindowsLogon():EXCEPTION " + e.Message);
			}
			catch
			{
				// Log exception...
				CmDataCapture.Capture( CmDataCapture.MaskError, "ValidateOperatorWindowsLogon():EXCEPTION caught");
			}
			finally
			{
				// close handle(s)
				if (pExistingTokenHandle != IntPtr.Zero)
				{
					PInvoke.CloseHandle(pExistingTokenHandle);
				}
			}
			return rVal;
		}
 
		/// <summary>
		/// Execute the signon process.
		/// </summary>		
		public override void ProcessSignOn()
		{	
			ConfigData configData = ConfigDataLaunchPad.GetConfigData();

			// Attempt to login using the Registry ID/Passwords...
			

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

			// TEMPORARY... until we get the Shift character for alphanum entry.
			// password = password.ToLower();

			if ( ValidateOperatorWindowsLogon( id, String.Empty, password ) )
			{
				if( signOnTimer != null )
				{
					signOnTimer.Dispose();
					signOnTimer = null;
				}
				
				m_windowsSignedOn = true;
				if( configData.BiometricDeviceConfigured && !myPsx.Biometric.KeyedIn ) //395787
				{																
					string scanSound = configData.AudioPath + "\\ValidBiometric.wav";
					if (scanSound.Length > 0 && !myPsx.Biometric.KeyedIn )
					{
						myPsx.PlaySound(scanSound);
					}					
				}

				myPsx.WindowsSignOnFailed = false;
				myPsx.WindowsSignOnId = id;
				myPsx.WindowsSignOnPassword = password;
				myPsx.SetContext( myPsx.TheApp.MainContext, SharedPsx.displayLPStandard, false );
				myPsx.MaintExecuteFunction( myPsx.WindowsButtonPressed );
			}
			else
			{
				m_windowsSignedOn = false;
				myPsx.WindowsSignOnFailed = true;
				myPsx.WindowsSignOnId = "";
				myPsx.WindowsSignOnPassword = "";
				// Always Alphanumeric for Logon Windows
                //397896, swap position ResetSignOnContexts() with myPsx.SetContext(). 
                //Call ResetSignOnContexts() first to clear the inputted ID value then 
                //changed the context name from "EnterAlphaNumericPassword" back to "EnterAlphaNumericID"
                ResetSignOnContexts(true);
				myPsx.SetContext( "EnterAlphaNumericID", SharedPsx.displayLPStandard, false );
			}
			//395787
			if( configData.BiometricDeviceConfigured && myPsx.Biometric.KeyedIn )
				myPsx.Biometric.KeyedIn = false;  
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
			            
			if( e.EventName.Equals( Psx.EventClick ) )
			{
				if ( e.ContextName.Equals( contextEnterId ) && ( e.ControlName.Equals( "ButtonCommand0" ) ) )
				{
					// The operator pressed "GoBack" on the enter ID screen.
					// Redisplay the original main context (RapContext, FastlaneContext, etc.)
					myPsx.ResetSignonContexts();
					myPsx.SetContext( myPsx.TheApp.MainContext, SharedPsx.displayLPStandard, false );
					if ( !ConfigDataLaunchPad.HasKeyLockHW )
					{
						myPsx.BackwardsFlag = !myPsx.BackwardsFlag;	// Sync the hardware keypress with the GO BACK button
					}
					handled = true;
				}
				else
				{
					handled = base.State_PsxEvent( sender, e );
				}
			}
			
			return handled;
		}


		public bool IsWindowsSignedOn
		{
			get { return m_windowsSignedOn; }
		}

        /// <summary>
        /// Returns a string representation of the object.
        /// </summary>
        public override string ToString()
        {
            return m_stateType;
        }

	}
}
