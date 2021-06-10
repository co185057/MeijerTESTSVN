using System;
using PsxNet;
using RPSWNET;

namespace LaunchPadNet
{
	/// <summary>
	/// The monitoring state class.
	/// </summary>
	internal class MonitoringState : LaunchPadNet.State
	{
		/// <summary>
		/// The class constructor.
		/// </summary>
		/// <param name="paramPsx">
		/// SharedPsx instance used to connect.
		/// </param>
		public MonitoringState( SharedPsx owner ) : base( owner )
		{
			myOwner = owner;
			//myPsx.Visible = true;
            //myPsx.TileLanes();
		}

		/// <summary>
		/// Releases the resources used by the MonitoringState instance.
		/// </summary>
		public override void Dispose()
		{
			base.Dispose();
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
                if( e.ControlName.Equals( "UtilityButton" ) )
				{ 
					CmDataCapture.Capture( CmDataCapture.MaskInfo, "State_PsxEvent():DisplayLaunchPadNet(false)");
					myOwner.DisplayLaunchPadNet( false );
					if ( !ConfigData.HasKeyLockHW )
					{
						myPsx.BackwardsFlag = !myPsx.BackwardsFlag;	// Sync the hardware keypress with the GO BACK button
					}
                    handled = true;
                }
                else if( e.ControlName.Equals( "SignOnButton" ) )
                {
                    if( myPsx.SignedOn )
                    {
						myPsx.LogToReportGenerator( false );
						myPsx.SignedOn = false;
                    }
                    handled = true;
                }
				else if( e.ControlName.Equals( "ConfirmYes" ) )
				{
					myPsx.ConfirmationApproved = true;
					myPsx.SetContext(myPsx.TheApp.CurrentContext, SharedPsx.displayLPStandard, false);
					myPsx.MaintExecuteFunction( myPsx.MaintButtonPressed );
					handled = true;
				}
				else if( e.ControlName.Equals( "ConfirmNo" ) )
				{
					myPsx.SetContext(myPsx.TheApp.CurrentContext, SharedPsx.displayLPStandard, false);
					myPsx.ConfirmationApproved = false;
					handled = true;
				}
			}
			
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
			// scanner activity... reset the auto sign off timer
			SharedPsx.autoSignOffCounter = 0;
		}

        public override string ToString()
        {
            return "Monitoring";
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
		
		// Data Members
		/// <summary>
		/// SharedPsx owner.
		/// </summary>
		private readonly SharedPsx myOwner;
	}
}
