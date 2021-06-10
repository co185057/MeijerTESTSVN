using System;
using System.Threading;
using PsxNet;
using RPSWNET;
using CommonLibsNet;

namespace LaunchPadNet
{
    /// <summary>
    /// 
    /// </summary>
    internal class LegalCaptionState : LaunchPadNet.State
    {
        /// <summary>
        /// The class constructor.
        /// </summary>
        /// <param name="paramPsx">
        /// SharedPsx instance used to connect.
        /// </param>
        public LegalCaptionState( SharedPsx paramPsx ) : base( paramPsx )
        {
			string text = myPsx.GetString( "LegalCaptionText", 10000);
			myPsx.SetControlConfigProperty( "LegalCaptionMessage", Psx.PropertyTextFormat, text );
		}

        /// <summary>
        /// Releases the resources used by the LegalCaptionState instance.
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
			            
            if( ( e.ContextName.Equals( "LegalCaptionContext" )) && e.EventName.Equals( Psx.EventClick ) )
			{
				if( e.ControlName.Equals( "UtilityButton" ) )
				{ 
					CmDataCapture.Capture( CmDataCapture.MaskInfo, "State_PsxEvent():DisplayLaunchPadNet(false)");
					myPsx.DisplayLaunchPadNet( false );
					if ( !ConfigDataLaunchPad.HasKeyLockHW )
					{
						myPsx.BackwardsFlag = !myPsx.BackwardsFlag;	// Sync the hardware keypress with the GO BACK button
					}
					handled = true;
				}
				else if ( ( e.ContextName.Equals( "LegalCaptionContext" )) && ( e.ControlName.Equals( "LegalCaptionOk" ) ) )
				{
					myPsx.SetContext( myPsx.GetExpectedEnterIDContext(), SharedPsx.displayLPStandard, false);
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
			// Do nothing...
		}

        /// <summary>
        /// Returns a string representation of the object.
        /// </summary>
        public override string ToString()
        {
            return "LegalCaption";
        }
	}
}
