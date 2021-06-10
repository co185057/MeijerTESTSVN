using System;
using PsxNet;
using System.Collections;
using CommonLibsNet;

namespace RapNet
{
	/// <summary>
	/// The state base class.
	/// </summary>
	public abstract class State : IDisposable
	{
		/// <summary>
		/// The class constructor.
		/// </summary>
		/// <param name="paramPsx">
		/// SharedPsx instance used to connect.
		/// </param>
		public State(SharedPsx paramPsx, IPsx psx, bool bActivateScanner)
		{
			myPsx = paramPsx;
            this.psx = psx;
            if (null != myPsx.scanner && bActivateScanner)
            {
                myPsx.scanner.ScannerEvent += new ScannerEventHandler(State_ScannerEvent);
            }            
		}

        /// <summary>
        /// Enters the state.
        /// </summary>
        public virtual void Enter()
        {
            if (ShowAlerts)
            {
                psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, false);
                myPsx.RecalculateAlertLanes();
                myPsx.ShowLanes(false, 0);
                myPsx.SetAlertLanes(true);
                psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyRedraw, true);            
            }            
        }

		/// <summary>
		/// The class destructor.
		/// </summary>
		~State()
		{
			Dispose();
		}

        /// <summary>
		/// Releases the resources used by the State instance.
		/// </summary>
		public virtual void Dispose()
		{
            if (null != myPsx.scanner)
            {
                myPsx.scanner.ScannerEvent -= new ScannerEventHandler(State_ScannerEvent);
            }
            GC.SuppressFinalize(this);
		}

        /// <summary>
        /// 
        /// </summary>
        public void NavigateBack()
        {
            string context = startupContext;
            if (screens.Count > 0)
            {
                screens.Pop();
                if (screens.Count > 0)
                {
                    context = (string)screens.Peek();
                }
            }
            psx.SetContext(context, Psx.DisplayStandard, false);
        }

        /// <summary>
        /// 
        /// </summary>
        public void NavigateFirst()
        {
            screens.Clear();
            psx.SetContext(startupContext, Psx.DisplayStandard, false);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="screen"></param>
        public void NavigateScreen(string screen)
        {
            if (0 == screens.Count || !screens.Peek().Equals(screen))
            {
                psx.SetContext(screen, Psx.DisplayStandard, false);
            }
        }

        /// <summary>
        /// True if scanner is allowed.
        /// </summary>		        
        public virtual bool AllowScanner
        {
            get
            {
                return false;                
            }
        }

        /// <summary>
        /// True if alert lanes should be shown.
        /// </summary>		        
        public virtual bool ShowAlerts
        {
            get
            {
                return false;
            }
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
		public virtual bool State_PsxEvent(object sender, PsxEventArgs e)
        {
            if (e.EventName.Equals(Psx.EventChangeContext))
            {
                if (0 == screens.Count || !screens.Peek().Equals(e.ContextName))
                {
                    screens.Push(e.ContextName);
                }
            }
            return false;
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
        public abstract void State_ScannerEvent(object sender, ScannerEventArgs e);
       
        /// <summary>
        /// Psx instance.
        /// </summary>
        protected SharedPsx myPsx;

        /// <summary>
        /// 
        /// </summary>
        /// 
        protected IPsx psx = null;

        private const string startupContext = "Startup";
        
        private Stack screens = new Stack();

        // +RFQ 7477
        // POS92988
        protected static POSFileAccessService.POSFileAccessws client = new POSFileAccessService.POSFileAccessws();
        public static CAppSettings cAppConfigObj = ConfigDataRapNet.LoadClass("CAppSettings") as CAppSettings;
        protected static bool bEmplPwdExpired = false;
        // POS92988
        // -RFQ 7477
	}
}
