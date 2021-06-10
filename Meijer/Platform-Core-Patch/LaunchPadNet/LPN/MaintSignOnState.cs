using System;
using System.Threading;
using System.Runtime.InteropServices;
using PsxNet;
using RPSWNET;

namespace LaunchPadNet
{
    /// <summary>
    /// 
    /// </summary>
    internal class MaintSignOnState : SignOnState
    {
		string m_stateType = "MaintSignOn";

		private bool   m_maintenanceSignedOn = false;

        /// <summary>
        /// The class constructor.
        /// </summary>
        /// <param name="paramPsx">
        /// SharedPsx instance used to connect.
        /// </param>
        public MaintSignOnState( SharedPsx paramPsx ) : base( paramPsx )
        {	
			m_maintenanceSignedOn = false;
        }

        /// <summary>
        /// Releases the resources used by the SignOnState instance.
        /// </summary>
        public override void Dispose()
		{
			base.Dispose();
        }


		/// <summary>
		/// Execute the signon process.
		/// </summary>		
		public override void ProcessSignOn()
		{	
			ConfigData configData = ConfigData.GetConfigData();

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


			if ( configData.ValidateOperatorRegistry( id, password, ConfigData.MAINTUSER ) )
			{
				if( signOnTimer != null )
				{
					signOnTimer.Dispose();
					signOnTimer = null;
				}

				if( configData.BiometricDeviceConfigured  && !myPsx.Biometric.KeyedIn  )
				{																
					string scanSound = configData.AudioPath + "\\ValidBiometric.wav";
					if (scanSound.Length > 0 && !myPsx.Biometric.KeyedIn )
					{
						myPsx.PlaySound(scanSound);
					}					
				}		
				
				m_maintenanceSignedOn = true;

				myPsx.MaintenanceSignOnFailed = false;
				myPsx.SetContext( myPsx.TheApp.MainContext, SharedPsx.displayLPStandard, false );
				myPsx.MaintExecuteFunction( myPsx.MaintButtonPressed );
			}
			else
			{
				m_maintenanceSignedOn = false;
				myPsx.MaintenanceSignOnFailed = true;
                //397896, swap position ResetSignOnContexts() with myPsx.SetContext(). 
                //Call ResetSignOnContexts() first to clear the inputted ID value then 
                //changed the context name from "EnterPassword" back to "EnterID"
                ResetSignOnContexts(true);  
				myPsx.SetContext( configData.AlphaNumericKeyboard ? "EnterAlphaNumericID" : "EnterID", SharedPsx.displayLPStandard, false );			
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
					if ( !ConfigData.HasKeyLockHW )
					{
						myPsx.BackwardsFlag = !myPsx.BackwardsFlag;	// Sync the hardware keypress with the GO BACK button
					}
                    SetShiftAndSymbolProperties(false);
                    PopulateKeyboard(false, false);
					handled = true;
				}
				else
				{
					handled = base.State_PsxEvent( sender, e );
				}
			}
            else if (e.EventName.Equals(Psx.EventChangeContext))
            {
                if (e.ContextName.Equals("EnterAlphaNumericID") || e.ContextName.Equals("EnterAlphaNumericPassword"))
                {
                    SetShiftAndSymbolProperties(false);
                    PopulateKeyboard(false, false);
                    handled = true;
                }
            }
			
			return handled;
		}


		public bool IsMaintSignedOn
		{
			get { return m_maintenanceSignedOn; }
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
