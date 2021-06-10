using System;
using System.Diagnostics;
using System.Collections ;

namespace LaunchPadNet
{
	/// <summary>
	/// Summary description for App.
	/// </summary>
	public class App
	{
		private String m_sMainExecutable="";
		private String m_sMainContext="";
		private String m_sMainContextNotLoaded;

		private String m_sCompanionExecutable="";
		private String m_sCompanionContext="";
		private String m_sCompanionContextNotLoaded="";

		private String m_sForegroundWindowTitle="";

		private String m_sKeyLock="";
		private bool m_bLoadOnStartUp=false;

		/// <summary>
		/// Utility (RETURN) button visible.
		/// </summary>
		private bool utilityButtonVisible=false;
		
		private bool m_bSwitched=false;		
		private bool m_bKillAppOnSwitch=false; 
		
		private bool m_bMinimized=false;				

		private String m_sSecurityContext="";
		
					
		private Executable m_appExecutable=null;		//Current Executable running
		private Executable m_appPrevExecutable=null;	//Previous Executable

		private bool m_bLaunchPadSecurityOn=false;

		private Hashtable m_buttonsTableHolder=new Hashtable(); //buttonsTable per Context		
		private String m_sInitialButtonState="Primary";

	    public App()
		{
			//
			// TODO: Add constructor logic here
			//
		}		
		
		public void SwitchTo()
		{
			//valid companion
			if ( this.m_sCompanionExecutable!=null || !this.m_sCompanionExecutable.Equals("")  )
			{
				this.m_bSwitched = !this.m_bSwitched;		
			}
		}
		
		public bool AllowSwitchTo()
		{
			if ( this.m_sCompanionExecutable!=null || !this.m_sCompanionExecutable.Equals("") )			
				return true;			
			else
				return false;
		}		
		
		public Executable AppExecutable
		{
			set
			{
				this.m_appExecutable  =value;
			}
			get
			{
				return this.m_appExecutable;
			}
		}
		
		public Executable AppPrevExecutable
		{
			set
			{
				this.m_appPrevExecutable  =value;
			}
			get
			{
				if ( this.m_bKillAppOnSwitch ) //always return null if killAppOnSwitch, means that no previous application
					return null;
				else
					return this.m_appPrevExecutable;
			}
		}
				
		public Hashtable ButtonsTableHolder
		{
			get
			{
				return this.m_buttonsTableHolder;
			}
			
		}
		
		public string MainExecutable
		{
			get
			{
				return this.m_sMainExecutable; 
			}
			set
			{
				this.m_sMainExecutable=value;
			}
		}
		
		public string MainContext
		{
			get
			{
				return this.m_sMainContext; 
			}
			set
			{
				this.m_sMainContext=value;
			}
		}
		
		public String MainContextNotLoaded
		{
			get
			{
				return this.m_sMainContextNotLoaded; 
			}
			set
			{
				this.m_sMainContextNotLoaded=value;
			}
		}

		public string CompanionExecutable
		{
			get
			{
				return this.m_sCompanionExecutable ; 
			}
			set
			{
				this.m_sCompanionExecutable=value;
			}
		}
			
		public string CompanionContext
		{
			get
			{
				return this.m_sCompanionContext; 
			}
			set
			{
				this.m_sCompanionContext=value;
			}
		}
						
		public string CompanionContextNotLoaded
		{
			get
			{
				return this.m_sCompanionContextNotLoaded; 
			}
			set
			{
				this.m_sCompanionContextNotLoaded=value;
			}
		}
		
		public string ForegroundWindowTitle
		{
			get
			{
				return this.m_sForegroundWindowTitle; 
			}
			set
			{
				this.m_sForegroundWindowTitle=value;
			}
		}

		public string SecurityContext
		{
			get
			{
				return this.m_sSecurityContext; 
			}
			set
			{
				this.m_sSecurityContext=value;
			}
		}
			
		public string CurrentContext
		{
			get
			{
				if ( this.m_bSwitched ==false)
				{	
					return this.m_sMainContext;
				}
				else
				{
					return this.m_sCompanionContext ;
				}
			}
			
		}
		
		public string CurrentExecutable
		{
			get
			{
				if ( this.m_bSwitched ==false)
					return this.m_sMainExecutable ;
				else
					return this.m_sCompanionExecutable ;
			}
			
		}
	
		public string SwitchToExecutable
		{
			get
			{
				if ( this.m_bSwitched ==true)
					return this.m_sMainExecutable ;
				else
					return this.m_sCompanionExecutable ;
			}
			
		}

		public string KeyLock
		{
			get
			{
				return this.m_sKeyLock; 
			}
			set
			{
				this.m_sKeyLock =value;
			}
		}

		public bool LoadOnStartUp
		{
			get
			{
				return this.m_bLoadOnStartUp; 
			}
			set
			{
				this.m_bLoadOnStartUp =value;
			}
		}
		
		public bool UtilityButtonVisible
		{
			get
			{
				return this.utilityButtonVisible; 
			}
			set
			{
				this.utilityButtonVisible=value;
			}
		}

		public bool LaunchPadSecurityOn
		{
			get
			{
				return this.m_bLaunchPadSecurityOn; 
			}
			set
			{
				this.m_bLaunchPadSecurityOn =value;
			}
		}
		
		public bool KillAppOnSwitch
		{
			get
			{
				return this.m_bKillAppOnSwitch; 
			}
			set
			{
				this.m_bKillAppOnSwitch =value;
			}
		}
		
		public bool Minimized
		{
			get
			{
				return this.m_bMinimized;
			}
			set
			{
				this.m_bMinimized=value;
			}
		}
		
		public String InitialButtonState
		{
			get
			{
				return this.m_sInitialButtonState;
			}
			set
			{
				this.m_sInitialButtonState=value;
			}
		}
		
		
	}
}
