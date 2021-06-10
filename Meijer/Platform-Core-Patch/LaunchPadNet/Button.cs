using System;

namespace LaunchPadNet
{
  /// </summary>
  public class Button
  {
    
    //private bool m_bIsAbleToBeEnabled; //not yet implemented
    private String m_sName;

    private bool m_bToggleEnabled=false;
    private bool m_bToggleStatus=false;
    
    private bool m_bEnableState=false;

    private String m_sPrimaryText;
    private String m_sSecondaryText;
    
    private String m_sPrimaryFunction;
    private String m_sSecondaryFunction;
    
    
  
	private bool m_bMaintenanceSignOn=false;		// Is there one additional authentication required for this button? Maintenance registry verified.
#if	( !WINCE )
	private bool m_bWindowsSignOn=false;			// Is there one additional authentication required for this button? Windows Log On verified.
#endif
	private bool m_bPrimaryShowConfirmation=false;
    private String m_sPrimaryShowConfirmationMessage="";

    private bool m_bSecondaryShowConfirmation=false;
    private String m_sSecondaryShowConfirmationMessage="";

    private bool m_bPrimaryRequiredSecurity=false;
    private bool m_bSecondaryRequiredSecurity=false;

    private String m_sPrimaryAllowToggleOnButton;
    private String m_sSecondaryAllowToggleOnButton;

    private String m_sPrimaryAllowToggleOnButtonState;
    private String m_sSecondaryAllowToggleOnButtonState;    
    
    
    private String m_sToggleStateOnDeadMainExecutable;      
    private String m_sToggleStateOnAliveMainExecutable;     
    private bool m_bDisabledOnDeadEnabledOnAliveMainExecutable=false;


    private Executable m_executable;  
  
    private int m_nOrigState=0;
    
    private String m_sInitialState="Primary";

    public Button()
    {
      
      // Initialize Additional Member Variables             
      m_bToggleEnabled = false;
      m_bToggleStatus = false;
      m_sPrimaryText = "";
      m_sSecondaryText = "";
      
    }

    // Declares the name and type of the property.
    public Executable Executable
    {     
      get
      {
        return this.m_executable; 
      }
  
      set
      {
        this.m_executable  = value;
      }
    }
        
    public String ToggleStateOnDeadMainExecutable
    {     
      get
      {
        return this.m_sToggleStateOnDeadMainExecutable; 
      }
  
      set
      {
        this.m_sToggleStateOnDeadMainExecutable  = value;
      }
    } 
    
    public String ToggleStateOnAliveMainExecutable
    {     
      get
      {
        return this.m_sToggleStateOnAliveMainExecutable; 
      }
  
      set
      {
        this.m_sToggleStateOnAliveMainExecutable  = value;
      }
    } 
    
    public bool DisabledOnDeadEnabledOnAliveMainExecutable
    {     
      get
      {
        return this.m_bDisabledOnDeadEnabledOnAliveMainExecutable; 
      }
  
      set
      {
        this.m_bDisabledOnDeadEnabledOnAliveMainExecutable  = value;
      }
    }
      
    public String PrimaryAllowToggleOnButton
    {     
      get
      {
        return this.m_sPrimaryAllowToggleOnButton; 
      }
  
      set
      {
        this.m_sPrimaryAllowToggleOnButton  = value;
      }
    }

    public String SecondaryAllowToggleOnButton
    {     
      get
      {
        return this.m_sSecondaryAllowToggleOnButton; 
      }
  
      set
      {
        this.m_sSecondaryAllowToggleOnButton  = value;
      }
    }

    public String PrimaryAllowToggleOnButtonState
    {     
      get
      {
        return this.m_sPrimaryAllowToggleOnButtonState ; 
      }
  
      set
      {
        this.m_sPrimaryAllowToggleOnButtonState  = value;
      }
    }

    public String SecondaryAllowToggleOnButtonState
    {     
      get
      {
        return this.m_sSecondaryAllowToggleOnButtonState; 
      }
  
      set
      {
        this.m_sSecondaryAllowToggleOnButtonState  = value;
      }
    }
    
    public bool PrimaryRequiredSecurity
    {     
      get
      {
        return this.m_bPrimaryRequiredSecurity; 
      }
  
      set
      {
        this.m_bPrimaryRequiredSecurity  = value;
      }
    }

    public bool SecondaryRequiredSecurity
    {     
      get
      {
        return this.m_bSecondaryRequiredSecurity; 
      }
  
      set
      {
        this.m_bSecondaryRequiredSecurity  = value;
      }
    }

	public bool MaintenanceSignOn
	{     
		get
		{
			return this.m_bMaintenanceSignOn; 
		}

		set
		{
			this.m_bMaintenanceSignOn = value;
		}
	}

#if	( !WINCE )
	public bool WindowsSignOn
	{     
		get
		{
			return this.m_bWindowsSignOn; 
		}

		set
		{
			this.m_bWindowsSignOn = value;
		}
	}
#endif

    public bool PrimaryShowConfirmation
    {     
      get
      {
        return this.m_bPrimaryShowConfirmation ; 
      }
  
      set
      {
        this.m_bPrimaryShowConfirmation  = value;
      }
    }

    public bool SecondaryShowConfirmation
    {     
      get
      {
        return this.m_bSecondaryShowConfirmation ; 
      }
  
      set
      {
        this.m_bSecondaryShowConfirmation  = value;
      }
    }

    public bool CurrentShowConfirmation
    {   
      get
      {
        bool bResult;
        if( m_bToggleEnabled)
        {
          if( ToggleStatus)
            bResult = this.m_bSecondaryShowConfirmation;
          else
            bResult = this.m_bPrimaryShowConfirmation ;
        }
        else
        {
          bResult = this.m_bPrimaryShowConfirmation ;
        }
        return bResult;
      }     
    
    }

    public String PrimaryShowConfirmationMessage
    {     
      get
      {
        return this.m_sPrimaryShowConfirmationMessage ; 
      }
  
      set
      {
        this.m_sPrimaryShowConfirmationMessage  = value;
      }
    }

    public String SecondaryShowConfirmationMessage
    {     
      get
      {
        return this.m_sSecondaryShowConfirmationMessage ; 
      }
  
      set
      {
        this.m_sSecondaryShowConfirmationMessage  = value;
      }
    }
    
    public String CurrentShowConfirmationMessage
    {         
      get
      {
        String sResult;
        if( m_bToggleEnabled)
        {
          if( ToggleStatus)
            sResult = this.m_sSecondaryShowConfirmationMessage;
          else
            sResult = this.m_sPrimaryShowConfirmationMessage;
        }
        else
        {
          sResult = this.m_sPrimaryShowConfirmationMessage;
        }
        return sResult;
      }     
    
    }

    public bool ToggleEnabled
    {     
      get
      {
        return m_bToggleEnabled;
      }
    
      set
      {
        m_bToggleEnabled = value;
      }
    }

    public bool ToggleStatus
    {     
      get
      {
        return m_bToggleStatus;
      }   
      
    }

    public String PrimaryText
    {     
      get
      {
        return m_sPrimaryText;
      }     
      set
      {
        m_sPrimaryText = value;       
      }
    }

    public int OrigState
    {     
      get
      {
        return m_nOrigState;
      }     
      set
      {
        m_nOrigState = value;       
      }
    }

    public String SecondaryText
    {     
      get
      {
        return m_sSecondaryText;
      }
      
      set
      {
        
        m_sSecondaryText = value;
      }
    }
  
    public String PrimaryFunction
    {
      get
      {
        return this.m_sPrimaryFunction; 
      }
      set
      {
        this.m_sPrimaryFunction = value;
      }
    }

    public String SecondaryFunction
    {
      get
      {
        return this.m_sSecondaryFunction ;
      }
      set
      {
        this.m_sSecondaryFunction  = value;
      }
    }
    
    public bool EnableState
    {     
      get
      {
        return this.m_bEnableState; 
      }
  
      set
      {
        this.m_bEnableState  = value;
      }
    }

    public String InitialState
    {
      get
      {
        String sResult;
        if( m_bToggleEnabled)
        {
          if ( this.m_sInitialState.ToLower().Trim().Equals("secondary"))
            sResult = "Secondary";
          else
            sResult = "Primary";
        }
        else
        {
          sResult = "Primary" ; //initial state always primary if toggleEnabled==false
        }
        return sResult;
      }     
    }
    
    public String CurrentText
    {
      get
      {
        String sResult;
        if( m_bToggleEnabled)
        {
          if( ToggleStatus)
            sResult = this.m_sSecondaryText;
          else
            sResult = this.m_sPrimaryText ;
        }
        else
        {
          sResult = this.m_sPrimaryText ;
        }
        return sResult;
      }     
    }

    
    public String CurrentFunction
    {
      get
      {
        String sResult;
        if( m_bToggleEnabled && this.SecondaryFunction!=null && !this.SecondaryFunction.Trim().Equals ("") )
        {
          if( ToggleStatus)
            sResult = this.SecondaryFunction;
          else            
            sResult = this.PrimaryFunction; 
        }
        else
        {
          sResult = this.PrimaryFunction; 
        }
        return sResult;
      }

      
    }

    public bool CurrentRequiredSecurity
    {
      
      get
      {
        bool bResult;
        if( m_bToggleEnabled  )
        {
          if( ToggleStatus)
            bResult = this.SecondaryRequiredSecurity ;
          else            
            bResult = this.PrimaryRequiredSecurity ; 
        }
        else
        {
          bResult = this.PrimaryRequiredSecurity; 
        }
        return bResult;
      }
    }

    public String CurrentAllowToggleOnButton
    {     
      get
      {
        String sResult;
        if( m_bToggleEnabled)
        {
          if( ToggleStatus)
            sResult = this.m_sSecondaryAllowToggleOnButton;
          else
            sResult = this.m_sPrimaryAllowToggleOnButton;
        }
        else
        {
          sResult = this.m_sPrimaryAllowToggleOnButton;
        }
        return sResult;
      }     
    }

    public String CurrentAllowToggleOnButtonState
    {     
      get
      {
        String sResult;
        if( m_bToggleEnabled)
        {
          if( ToggleStatus)
            sResult = this.m_sSecondaryAllowToggleOnButtonState;
          else
            sResult = this.m_sPrimaryAllowToggleOnButtonState;
        }
        else
        {
          sResult = this.m_sPrimaryAllowToggleOnButtonState;
        }
        return sResult;
      }     
    }

    public String Name
    {     
      get
      {
        return m_sName;
      }     
      set
      {
        m_sName = value;
      }
    }
    
    public void Toggle()
    {
      if( m_bToggleEnabled)
      {       
        if( m_bToggleStatus)                
          m_bToggleStatus=false;                        
        else        
          m_bToggleStatus = true;                   
      }                       
      
    }
    

  
  }//end class
}

