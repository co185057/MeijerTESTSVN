//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAssitMode.CPP
//
// TITLE: Class implementation for Assist Mode keyboard
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "TransBrokerSSF.h"
#include "SMSmHandHeldAssistMode.h"
#include "SMSmAssistMode.h"
#include "SMWaitForRemoteAssistance.h"
#include "SMSmAssistMenu.h"

#define TB_DEV_TOTAL_DETAILS 10006
#define TIMEOUT_DISABLED 4
#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmHandHeldAssistMode")

IMPLEMENT_DYNCREATE(SMSmHandHeldAssistMode, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP
#define MAX_LEADTHRU_TEXT 30

static bool g_bRefreshAfterSoftKeyDisable = false;

////////////////////////////////
SMSmHandHeldAssistMode::SMSmHandHeldAssistMode()
{
	IMPLEMENT_TIMESTAMP

	m_csScreenContext = _T("HHSmAssistMode");
	ps.SetFrame(m_csScreenContext); 
		
		//Create display string for testing until display message flow complete
	m_csLastDisplay = _T("");
	m_csLastTitle1 = _T("");
	m_bTransactionEnding = false;
    m_bHideInput = false;
    m_bAssistModeExitAllowed = false;
    m_iBtnIndex = 0;
    //disable all the softkeys until they are ready to be displayed
    DisableSoftKeys();

    for(int i=0; i<9; i++)
    {
        m_bButtonEnable[i] = false;
        m_csButtonText[i] = _T("");
    }
} 

void SMSmHandHeldAssistMode::EnableDisableAllKeys(bool bFlag)
{
    CString csKeyCount, csKey;
    int i, nKeyCount =2;
   

    
    for(i=1; i <= nKeyCount; i++)
    {
        csKey.Format(_T("SMButton%d"),i);
        ps.ButtonEnable(csKey, bFlag);
		ps.ButtonFlash(csKey, false);
    }
}

void SMSmHandHeldAssistMode::DisableSoftKeys()
{
    
   EnableDisableAllKeys(false);

   // TAR 359256 - To prevent having all keys disabled on next refresh if 
    //              The same display comes in only with keys enabled.
    //              This will avoid hanging when we get the ACS screen bugs
    //              where a dynakey needs to be pressed twice to work.
    g_bRefreshAfterSoftKeyDisable = true;
}

void SMSmHandHeldAssistMode::EnableAllowedSoftKeys(void)
{
    //now show the only 3 buttons that will fit on the screen
    CString csButtonControlName;
   
    for(int i = 1; i<3; i++)
    {
        if((m_iBtnIndex + i) <= MAX_DYNA_SOFTKEY)
        {

            csButtonControlName.Format(_T("%s%d"), _T("SMButton"), (i));
            //if btn is enabled and not the more button
            if(m_bButtonEnable[m_iBtnIndex + i]) 
            {
            
		        ps.Button(csButtonControlName, m_csButtonText[m_iBtnIndex + i], true);
				// if button has radioon Icon make it blink
	            if(m_bButtonIconOn[m_iBtnIndex + i]) 
				{
					ps.ButtonFlash(csButtonControlName, true);
				}else
				{
					ps.ButtonFlash(csButtonControlName, false);
				}
					
            }
			else
            {
            
                ps.Button(csButtonControlName, m_csButtonText[m_iBtnIndex + i], false);
            }
        }

        //figure out what to do button 2 on the HH if we're painting POS button 7 
        //POS softkey 8 can be more/back or a real key like 'other'
        if((m_iBtnIndex + i) == 8) 
        {
            if((m_csButtonText[8].Find(_T("More")) != -1) ||
               (m_csButtonText[8].Find(_T("Back")) != -1))
            {
                ps.Button(_T("SMButton2"), _T(" "), false);
                

            }
            
        }
       

    }
    
}

void SMSmHandHeldAssistMode::UnInitialize(void)
{
	bool bTBExitAlready = false;
	fStateAssist = false;

	SendTBCommand(TBCMD_EXIT_ASSISTMODE, NULL);
    
    
}
////////////////////////////////////
SMStateBase  *SMSmHandHeldAssistMode::Initialize()
{
    trace(L0, _T("+SMSmHandHeldAssistMode::Initialize()")); 
    fStateAssist = true;
	co.SetInAssistMenus(false);
   

    ps.Echo(PS_BLANK);
    EnableClearKey();
   
   
    SAWLDBScannerDisable();
    ps.ShowSMTotal(true);

    if(fInEndorsementState == ENDORSE_NONE)
    {
        SAClearSecurityCondition();
    }

    if ( TB_SUCCESS_IMMEDIATE == SendTBCommand(TBCMD_ENTER_ASSISTMODE, false))
	{
		m_bTransactionEnding = true;
	}

    HRESULT hr;
    hr = m_POSDisplay.CreateInstance(__uuidof( MSXML2::DOMDocument40) );
    if(FAILED(hr))
    {
		trace(L1, _T("ERROR: Failed to create XML DOM Object. IS MS XML INSTALLED??"));
        return PSButton5();
    }
	ps.AllowInput( MAX_DIGITS_ITEMCODE, false, false );
    AllowAssistmodeExit();
	
    

	if(SendTBCommand(TBCMD_TENDER_IN_PROGRESS, NULL) == 0)
    {
	    TBShowDisplay();	   
	    ps.ShowFrame();     
        EnableClearKey();
        return STATE_NULL;

    }else
    {
        m_bTenderInProgress = true;


        CString csIsOpGuid;
        SendTBCommand(TBCMD_GET_HOOKPROPERTY, &CString(_T("PropIsOperGuid")), &csIsOpGuid);
        if  (csIsOpGuid == _T("0")) 
        {
            CString csForceDisplay;
            SendTBCommand(TBCMD_GET_HOOKPROPERTY, &CString(_T("ForceAssistDisplay")), &csForceDisplay);
            if ( csForceDisplay == _T("0"))
            {
                
                //don't let operator leave until the partial cash tender is complete
                
                m_bAssistModeExitAllowed = false;
                ShowProcssingScreen();       
                return TimedOut();
            }
        }
        trace( L4, _T("Found ForceDisplay/OperGuid Display") );

        //turn on the display so the operator can deal with it
        TBShowDisplay();	   
	    ps.ShowFrame();     
        EnableClearKey();

        SMStateBase* pCheck = TimedOut();
        delete getRealStatePointer( pCheck );

        return STATE_NULL;
    }
}

//////////////////////////////////////////////
// PSClearKey
//////////////////////////////////////////////
SMStateBase  *SMSmHandHeldAssistMode::PSClearKey(void)
{
  PSClearAllInput();
  EnableClearKey();
  CString csPOSString = _T("{CLEAR}");	

  SendTBCommand(TBCMD_SENDKEYEDDATA, csPOSString);
  return STATE_NULL;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Method:		SMStateBase  *SMSmHandHeldAssistMode::PSNumericKey
//
// Purpose:		Find keycode for numeric keys. Send these keycode to TB 
//
// Parameters:	const long lKey
//
// Returns:		SMStateBase
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SMStateBase  *SMSmHandHeldAssistMode::PSNumericKey(const long lKey)
{
	trace(L0,_T("+SMSmHandHeldAssistMode::PSNumericKey %d"),lKey);

    trace(L0,_T("+HandHeldAssistMode %d"),lKey);

    static CString csData = _T("");
	CString csPOSString = TBGetKeyPOSString(lKey);

	if (csPOSString.IsEmpty())
	{
		csPOSString = (_TCHAR)lKey;
	}

    if ((csPOSString != _T("")) && 
		(nTimeOut != TIMEOUT_DISABLED)) 
	{
        
    		SendTBCommand(TBCMD_SENDKEYEDDATA, csPOSString);
        
	}
	
    //PSEchoInput(lKey);   
	ps.EchoInput((_TCHAR)lKey, true);;    

   
	return STATE_NULL;
}


SMStateBase  *SMSmHandHeldAssistMode::PSAssistKey(const long nKey)
{
    SMStateBase *pNextState = STATE_NULL;


    if (m_nAssistKeyVirtualKeyValue[nKey] >= VK_NUMPAD0 && 
		m_nAssistKeyVirtualKeyValue[nKey] <= VK_NUMPAD9)   //numeric key
	{
        PSEchoInput(VirtKeyToTChar(m_nAssistKeyVirtualKeyValue[nKey],0));
        CString csPOSString = TBGetKeyPOSString(nKey);
        SendTBCommand(TBCMD_SENDKEYEDDATA, csPOSString);
        
		
    }else
    {
        pNextState = SMSmAssistMode::PSAssistKey(nKey);
    }

    EnableClearKey();
    return pNextState;
}

SMStateBase  *SMSmHandHeldAssistMode::PSBackSpaceKey(void)
{
	trace(L0,_T("+SMSmHandHeldAssistMode::PSBackSpaceKey")); 
	
    
	return PSClearKey();
}

SMStateBase *SMSmHandHeldAssistMode::PSAlphaNumericKey(const long lKey)
{
	long lKey2;

	if(SMSmAssistMode::IsSpecialHandlingScreen())
	{
	    trace(L0,_T("+HandHeldAssistMode::PSAlphaNumericKey %d"),lKey);
		CString csPOSString = TBGetKeyPOSString(lKey);

		if (csPOSString.IsEmpty())
		{  
			CString csPOSString2;
			lKey2 = lKey;


			// customer wants all alphabetic letters from the HandHeld RAP to be in upper case
			// so the code below first checks to see if alphabetic, then converts all to lower
			// case, then sends the lower case with a "CAPS LOCK" keycode to ensure upper case   

			if (((lKey2 > 0x60) && (lKey2 < 0x7b))		// if alphabetic
		        || ((lKey2 > 0x40) && (lKey2 < 0x5b)))
			{
				if ((lKey2 > 0x40) && (lKey2 < 0x5b))	//if upper case
				{
					lKey2 = lKey2 + 0x20;     // if upper case, make lower case
					trace(L0,_T("+HandHeldAssistMode::PSAlphaKey 1 %s"),csPOSString);
				}
				csPOSString2 = (_TCHAR)lKey2;
				csPOSString = (_T("{CAPS}"));
				csPOSString = csPOSString + csPOSString2;
			}

			else
			{
				csPOSString = (_TCHAR)lKey;
			}
		}

		if ((csPOSString != _T("")) && 
		(nTimeOut != TIMEOUT_DISABLED)) 
		{
		    trace(L0,_T("+HandHeldAssistMode::PSAlphaNumericKey %s"),csPOSString);
        
    		SendTBCommand(TBCMD_SENDKEYEDDATA, csPOSString);
        
		}
	
		ps.EchoInput((_TCHAR)lKey, true);;    
	
	}

	return STATE_NULL;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Method:		SMStateBase  *SMSmHandHeldAssistMode::PSEnterKey
//
// Purpose:		Find keycode for numeric keys. Send these keycode to TB 
//
// Parameters:	const long lKey
//
// Returns:		SMStateBase
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SMStateBase *SMSmHandHeldAssistMode::PSEnterKey(void)
{
	trace(L0,_T("+SMSmHandHeldAssistMode::PSEnterKey"));

    PSClearAllInput();
    EnableClearKey();

    CString csPOSString = _T("{ENTER}");	

    SendTBCommand(TBCMD_SENDKEYEDDATA, csPOSString);
    
    
	return STATE_NULL;                 
}

//dynakey softkey
SMStateBase  *SMSmHandHeldAssistMode::PSButton1(void)
{
	trace(L0,_T("+SMSmHandHeldAssistMode::PSButton1"));


    CString csPOSString;
    csPOSString.Format(_T("{F%d}"), (1 +m_iBtnIndex ));        
	SendTBCommand(TBCMD_SENDKEYEDDATA, csPOSString);

	
    return STATE_NULL;     
}

//dynakey softkey
SMStateBase  *SMSmHandHeldAssistMode::PSButton2(void)
{
	trace(L0,_T("+SMSmHandHeldAssistMode::PSButton2"));
     

    CString csPOSString;
    csPOSString.Format(_T("{F%d}"), (2 +m_iBtnIndex ));       
	SendTBCommand(TBCMD_SENDKEYEDDATA, csPOSString);

	return STATE_NULL;          
}


//go back/exit assistmode
SMStateBase  *SMSmHandHeldAssistMode::PSButton5(void)
{
	trace(L0,_T("+SMSmHandHeldAssistMode::PSButton5"));

    fWaitingToExit = true;
    SAClearSecurityCondition();
    if(m_bAssistModeExitAllowed == false)
    {
        //SendTBCommand(TBCMD_SET_TB_STATE,_T("ASSISTMODE_NEEDED"));
		trace(L0,_T("+SMSmHandHeldAssistMode::PSButton5 m_bAssistModeExitAllowed = false"));
    }
    

    return TimedOut();
	
	          
}




//more button- scroll btn for dynakeys
SMStateBase  *SMSmHandHeldAssistMode::PSButton8(void)
{
	trace(L0,_T("+SMSmHandHeldAssistMode::PSButton8"));

    bool bNoRealMoreBtn = false;

    //if there is no back or more button on the POS then disable the HH's one
    if((m_csButtonText[8].Find(_T("More")) == -1) &&
       (m_csButtonText[8].Find(_T("Back")) == -1))
    {
        bNoRealMoreBtn = true;
    }

    if((m_iBtnIndex >= 6) && (!bNoRealMoreBtn))
    {
        m_iBtnIndex =0;
        //now make the POS scroll and resend a display
        if(m_bButtonEnable[8])
        {
            CString csPOSString = _T("{F8}");
	        SendTBCommand(TBCMD_SENDKEYEDDATA, csPOSString);
            return STATE_NULL;
        }

    }else 
    {
        m_iBtnIndex +=2;

        //if we go over the number of btns possible reset
        if(m_iBtnIndex >= 8)
        {
            m_iBtnIndex = 0;
        }
    }
    EnableAllowedSoftKeys();
    	
	return STATE_NULL;          
}

/////////////////////////////////////////////
SMStateBase *SMSmHandHeldAssistMode::PSReceiptDown(void)
{ 
	//*** NewUI ***// 
	ps.ScrollDown(_T("SMReceipt"));
	ps.ScrollDown(_T("AssistReceipt"));
	SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(_T("AssistReceipt")));
	
	return STATE_NULL;
}

/////////////////////////////////////////////
SMStateBase *SMSmHandHeldAssistMode::PSReceiptUp(void)
{
	//*** NewUI ***//
	ps.ScrollUp(_T("SMReceipt"));
	ps.ScrollUp(_T("AssistReceipt"));
	SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(_T("AssistReceipt")));
	return STATE_NULL;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Method:		SMStateBase  *SMSmHandHeldAssistMode::TBShowDisplay
//
// Purpose:		Retrieve latest POS screen info and display it
//
// Parameters:	N/A
//
// Returns:		N/A
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SMSmHandHeldAssistMode::TBShowDisplay(bool bRefresh)
{
    trace( L4, _T("+SMSmHandHeldAssistMode::TBShowDisplay()")); 
	
	

    //need to hide input?
    if(SendTBCommand(TBCMD_HIDEINPUT, NULL))
    {
        if(m_bHideInput != true)
        {
            m_bHideInput = true;
            ps.AllowInput( MAX_DIGITS_ITEMCODE, true, false );

        }
    }else
    {
        if(m_bHideInput != false)
        {
            m_bHideInput = false;
            ps.AllowInput( MAX_DIGITS_ITEMCODE, false, false );

        }
    }
    EnableClearKey();

	if (nTimeOut == TIMEOUT_DISABLED)
	{
		//TBLoadAssistModeKeyboard();	// KSo - This fixes the "Clear" key problem
		nTimeOut = 0;
	}

    // TAR 346579: Disable the 'Return To Shopping' button if we're
    //             currently ending the transaction
    CString csTBState(_T(""));
    SendTBCommand(TBCMD_GET_TB_STATE, &csTBState);

	if ( fInVoidTrans || (-1 != csTBState.Find(_T("ASSISTMODE_TRXEND"))) )
	{
		m_bTransactionEnding = true;
	}
    
    m_POSDisplay->async = VARIANT_FALSE; // need to use sync mode if accessing DOM immediately after Load
    
	CString strDisplay = _T("");
	SendTBCommand(TBCMD_GETDISPLAYXML, &strDisplay);
	
    
	if(strDisplay == _T(""))
	{
        return;
	}
	if((strDisplay != _T("")) && (m_csLastDisplay == strDisplay) && !bRefresh && !g_bRefreshAfterSoftKeyDisable)
	{ 
		return;
	}
	
	g_bRefreshAfterSoftKeyDisable = false;

	m_csLastDisplay = strDisplay;
	
    TraceDisplayMessage();
	
	strDisplay = _T(""); 
	if( m_POSDisplay->loadXML( (_bstr_t) m_csLastDisplay ) != VARIANT_TRUE )
	{
		trace(L1, _T("ERROR: Load display xml failed"));
		return;
	}
	
    
	AllowAssistmodeExit();

    //need to decide before we start painting the screen which context
	//we are going to show
	SetupScreenContext();
    SetScreenText();
    SetOperatorGuidance();		
	SetDynakeys();
	m_pAppPSX->SetTransactionVariable( _T("NextGenData"), COleVariant(_T("NextGenUIDynaKeysUpdateComplete")) );
	m_pAppPSX->SetTransactionVariable( _T("NextGenData"), COleVariant(_T("")) ); //Reset to blank to prevent multiple updates to NGUI
	SetupGraphicalScreen();   
	
	m_csLastTitle1 = GetDisplayText(_T("Title1"));    

    ps.ShowFrame();
	trace( L4, _T("-SMSmHandHeldAssistMode::TBShowDisplay()"));
}



void SMSmHandHeldAssistMode::SetupGraphicalScreen()
{
	CString csVoidPrepaidCardsProp = _T("VoidPrepaidCards");
    CString csIsVoidPrepaidCards;
    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csVoidPrepaidCardsProp, &csIsVoidPrepaidCards);

	PSXRC rc;

    // If voiding prepaid cards 
    if (csIsVoidPrepaidCards == _T("1")) 
    {

        // Make ACSList visible here
        m_pAppPSX->SetConfigProperty( _T("PrepaidCardList"), m_csScreenContext, UI::PROPERTYVISIBLE, VARIANT_TRUE );
        m_pAppPSX->SetConfigProperty( _T("SMReceipt"), m_csScreenContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );

        CString csPrepaidCardProp = _T("PrepaidCardList");
        CString csPrepaidCard;
        SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csPrepaidCardProp, &csPrepaidCard);


		 rc = m_pAppPSX->CreateReceiptItem( _T("PrepaidCardList"), _T("item1") );
        if ( PSX_SUCCESS != rc )
        {
            // kaboom!
        }
        rc = m_pAppPSX->SetReceiptItemVariable( _T("PrepaidCardList"), _T("item1"), _T("ITEM_DESCRIPTION"), csPrepaidCard );

		// create more
        rc = m_pAppPSX->UpdateReceiptControls( _T("PrepaidCardList") );
    }
    else
    {
		
        // Make PrepaidList not visible here
        m_pAppPSX->SetConfigProperty( _T("PrepaidCardList"), m_csScreenContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );
        m_pAppPSX->SetConfigProperty( _T("SMReceipt"), m_csScreenContext, UI::PROPERTYVISIBLE, VARIANT_TRUE );
        
		SetHookProperty(_T("PrepaidCardList"), _T(""));
		rc = m_pAppPSX->ClearReceipt( _T("PrepaidCardList") );

    }

}


void SMSmHandHeldAssistMode::SetDynakeys()
{
    CString csLabel, csEnabled, csText, csGrayed, csIcon;
	COleVariant vReturn, v;
	long nButtonCount = MAX_DYNA_SOFTKEY; 
    bool bNeedMoreBtn = false;

	for(int i=0; i<9; i++)
    {
        m_bButtonEnable[i] = false;
        m_bButtonIconOn[i] = false;
        m_csButtonText[i] = _T("");
    }

	
    bNeedMoreBtn = false;

	for (int nKey=1; nKey <= nButtonCount; nKey++)
	{	 
		
		csLabel.Format(_T("DK%d_Label"),nKey);
		csEnabled.Format(_T("DK%d_Enabled"), nKey);
		csGrayed.Format(_T("DK%d_Grayed"), nKey);
		csIcon.Format(_T("DK%d_Icon"), nKey);
		
		
		csText = GetDisplayText(csLabel);  //Dynakey1
		if (csText == _T(""))
		{
			csLabel.Format(_T("DK%d_Label_Alt"), nKey);
			csText = GetDisplayText(csLabel);
			if (csText == _T("")) 
				csText = _T(" ");
		}

       
		
		_variant_t vEnabled = GetFieldValue(csEnabled);
		_variant_t vGrayed = GetFieldValue(csGrayed);

		// if Tax Exempt button has radioon bmp for Icon set bool so we can make it blink 
		csIcon = GetDisplayText(csIcon);
		if((csIcon.Find(_T("radioon")) != -1) && (csText.Find(_T("Taxes")) != -1))
		{
	        m_bButtonIconOn[nKey] = true;
		}else
		{
			m_bButtonIconOn[nKey] = false;
		}

		long lButtonState;
		if (vEnabled.vt != VT_NULL && vEnabled.boolVal)
			lButtonState = (long) UI::Normal; 
		else if ((vGrayed.vt != VT_NULL) && (vGrayed.intVal == 0 || vGrayed.intVal == 2))
		{
			
			lButtonState = (long) UI::Normal;
		}
		else 
			lButtonState = (long) UI::Disabled;

        //check and see if it is one of the buttons that TB always wants
        //disabled for assistmode
        if(SendTBCommand(TBCMD_ASSISTMODE_BUTTON_ALLOWED, csText) == 1)
        {
            trace(L0, _T("HH Button not allowed Index = %d, Button Name = %s, button state = %d"), nKey, csText ,m_bButtonEnable[nKey] ); 

            lButtonState = (long) UI::Disabled;
        }
		

        //save the button text in the array
        m_csButtonText[nKey] = csText;

        //save the btn state in the array
		if(lButtonState == (long) UI::Normal)
        {
            m_bButtonEnable[nKey] = true;
            //do we need the more btn?
            if(nKey >= 3) 
            {
                bNeedMoreBtn = true;
                
            }
			
        }else
        {
            m_bButtonEnable[nKey] = false;
            
        }

        
        trace(L0, _T("HH Button Index = %d, Button Name = %s, button state = %d"), nKey, m_csButtonText[nKey],m_bButtonEnable[nKey] ); 
        

	}

    m_iBtnIndex = 0;

    //turn off the HH 'more' btn if we don't need it
    if(bNeedMoreBtn)
    {
        //ps.Button(_T("SMButton8"), ps.GetPSText(9133), true);
		ps.Button(_T("SMButton8"), BTT_NEXT, true);
		ps.ButtonState(_T("SMButton8"), false, true);   
        ps.ButtonState(_T("SMButton8"), true, true);
    }else
    {
         ps.ButtonEnable(_T("SMButton8"), false);
    }
    EnableAllowedSoftKeys();
}
bool SMSmHandHeldAssistMode::IsOperatorGuidance(void)
{
	bool bIsOperGuid = false;
	COleVariant vValue = GetFieldValue(_T("IsOperGuid"));
	if (vValue.vt != VT_NULL && vValue.boolVal)
	{
		bIsOperGuid = true;
	}

	return bIsOperGuid;

}
void SMSmHandHeldAssistMode::SetOperatorGuidance() 
{
	CString csOperatorGuidance = _T("");


	if (IsOperatorGuidance())
	{

		csOperatorGuidance = GetDisplayText(_T("OperGuid_Txt"));
		if (csOperatorGuidance == _T(""))
		{
			csOperatorGuidance = GetDisplayText(_T("OperGuid_Prompt"));
		}

		ps.Echo(4, csOperatorGuidance);
	}	
	
}

void SMSmHandHeldAssistMode::SetScreenText(void)
{
	CString csLTT1 = GetDisplayText(_T("LTT1"));
    CString csLTT2 = GetDisplayText(_T("LTT2")); 
	ps.SetTitle(GetDisplayText(_T("Title1")), GetDisplayText(_T("Title2")));

	CString csCurPosState = GetHookProperty(_T("__POSState"));
    
	if  (csCurPosState == _T("NEED_SECOND_BARCODE"))
	{
		csLTT2 = _T("");
	}
	// TAR 368411 - only prompt for ID or password otherwise text overlaps on HH
    if  (csCurPosState == _T("SUPERVISOR_ID") || csCurPosState == _T("SUPERVISOR_PWD"))
	{
		csLTT2.MakeLower();
	}
	ps.SetLeadthruText(csLTT1, csLTT2);
}

void SMSmHandHeldAssistMode::ShowProcssingScreen(void)
{
    EnableDisableAllKeys(false);
    DisableSoftKeys();    
    ps.SetTitle(_T(""));
    ps.SetLeadthruText(ps.GetPSText(LTT_CASHPROCESSING, SCOT_LANGUAGE_PRIMARY)); //processing screen

}

SMStateBase *SMSmHandHeldAssistMode::TBParse(MessageElement *me)
{
   
    
	if ((me->nDeviceID == TB_DEV_DISPLAYPRIMARY) && (m_bTenderInProgress != true)) 
	{
		TBShowDisplay();
	}
    else if ((me->nDeviceID == TB_DEV_DISPLAYPRIMARY) && (m_bTenderInProgress == true))
    {
        CString csIsOpGuid;
        SendTBCommand(TBCMD_GET_HOOKPROPERTY, &CString(_T("PropIsOperGuid")), &csIsOpGuid);
        if  (csIsOpGuid == _T("0")) 
        {
            CString csForceDisplay;
            SendTBCommand(TBCMD_GET_HOOKPROPERTY, &CString(_T("ForceAssistDisplay")), &csForceDisplay);
            if ( csForceDisplay == _T("0") )
            {
                //don't let operator leave until the partial cash tender is complete
                trace(L6,_T("SMSmAssistMode::TBParse Tender in progress, showing processing screen"));
                ShowProcssingScreen();          
                return TimedOut();
            }
        }

        trace(L6,_T("SMSmAssistMode::Got ForcedAssist/OperGuid message while trying to tender in assistmode"));
        EnableDisableAllKeys(true);           
        TBShowDisplay();
    }
    else if (me->nDeviceID == TB_DEV_TOTAL_DETAILS)
	{
        //in order to prevent the constant setting of the current item on the POS
        //check if the totals have changed since the last time we set the selected
        //item.  If so then change the selected item.
        long lPrevTax, lPrevTotal, lCurrTax, lCurrTotal;

        //get the previous totals
        lPrevTax = lTaxDue;
        lPrevTotal = lTotalSale;

        SMStateBase *pState = SMState::TBParse(me);
	    
        //now get the new totals
        lCurrTax = lTaxDue;
        lCurrTotal = lTotalSale;        
        
        return pState;
	}

	SMStateBase *pState = SMState::TBParse(me);
	return pState;
}


SMStateBase  *SMSmHandHeldAssistMode::TimedOut()
{
	trace(L6,_T("+SMSmHandHeldAssistMode::TimedOut()"));
    //if a tender is in progress then don't let the user out of assistmode
    //until the tender has finished.
    if(m_bTenderInProgress)
    {
       

        if(SendTBCommand(TBCMD_TENDER_IN_PROGRESS, NULL) == 0)
        {
            //cash tender not in progress
            m_bTenderInProgress = false; 
            m_bAssistModeExitAllowed =true;
            EnableDisableAllKeys(true);            
            TBShowDisplay(); 
            
        }
        return STATE_NULL;
    }   
	

	if (fWaitingForEOT)
	{

		fWaitingToExit = true;
		m_bTrxnEndedInHandHeld = true;
	
	}

	if(fWaitingToExit)
	{
		fStateAssist = false;
		RETURNSTATE(SmAssistMenu);

	}

	return STATE_NULL;
}


bool SMSmHandHeldAssistMode::AllowAssistmodeExit(void)
{

    bool bAllowExitAM = true;

   	_variant_t bAllowExit = GetFieldValue(_T("AllowAMExit"));
    ps.Button(_T("SMButton5"), BTT_GOBACK, true);
    ps.ButtonEnable(_T("SMButton5"), true);

	if (bAllowExit.vt == VT_NULL)
	{ 
        m_bAssistModeExitAllowed = true;
	}
    else if ( bAllowExit.boolVal  && !m_bTransactionEnding)
	{
		
        m_bAssistModeExitAllowed = true;
	}
	else
	{		
        m_bAssistModeExitAllowed = false;
	}


    bAllowExitAM = m_bAssistModeExitAllowed;

    return bAllowExitAM;
}



void SMSmHandHeldAssistMode::EnableClearKey(void)
{
    COleVariant v1;
    v1 = (long) UI::Normal;
    CString csClearCtlName = _T("SMNumericKeyBoard");
//    m_pAppPSX->SetControlProperty(csClearCtlName, UI::PROPERTYSTATE, v1);
//    m_pAppPSX->SetControlProperty(csClearCtlName, UI::PROPERTYSTATE, v1);
}


// Get the HHRapVirtualKeyboardNeeded property from ApplicationModel for this state to see
// if we should set up the keyboard or the EReceipt
void SMSmHandHeldAssistMode::SetupScreenContext() 
{

        CString csHHRAPKeyboard, csHHRAPDataNeeded;
        CString csCurContext, csValue;
	    COleVariant v;

		// get the hook property to see if data needed (supervisor ID or password) 
        SendTBCommand(TBCMD_GET_HOOKPROPERTY, &CString(_T("HHRapDataNeeded")), &csHHRAPDataNeeded);
		// get the hook property to see if numeric keypad should be displayed 
        SendTBCommand(TBCMD_GET_HOOKPROPERTY, &CString(_T("HHRapVirtualKeyboardNeeded")), &csHHRAPKeyboard);
		
		if  (csHHRAPKeyboard == _T("1"))
		{
			m_csScreenContext = _T("HHSmAssistModeWithKeyboard");
            ps.SetFrame(m_csScreenContext);

			m_pAppPSX->SetConfigProperty(_T("KeyBoardP1"), m_csScreenContext, UI::PROPERTYSTATE, (long) UI::Normal);
			m_pAppPSX->SetConfigProperty(_T("KeyBoardP2"), m_csScreenContext, UI::PROPERTYSTATE, (long) UI::Normal);
			m_pAppPSX->SetConfigProperty(_T("KeyBoardP4"), m_csScreenContext, UI::PROPERTYSTATE, (long) UI::Normal);
			m_pAppPSX->SetConfigProperty(_T("SMNumericKeyBoard"), m_csScreenContext, UI::PROPERTYSTATE, (long) UI::Normal);
			

			if (csHHRAPDataNeeded != _T("1"))
			{
				m_pAppPSX->SetConfigProperty(_T("SMButton5"), m_csScreenContext, UI::PROPERTYSTATE, (long) UI::Normal);
			}
			else
			{
				m_pAppPSX->SetConfigProperty(_T("SMButton5"), m_csScreenContext, UI::PROPERTYSTATE, (long) UI::Disabled);
			}

			v = VARIANT_TRUE;
			m_pAppPSX->SetConfigProperty(_T("KeyBoardP1"), m_csScreenContext, UI::PROPERTYVISIBLE, v);
			m_pAppPSX->SetConfigProperty(_T("KeyBoardP2"), m_csScreenContext, UI::PROPERTYVISIBLE, v);
			m_pAppPSX->SetConfigProperty(_T("KeyBoardP4"), m_csScreenContext, UI::PROPERTYVISIBLE, v);
			m_pAppPSX->SetConfigProperty(_T("SMNumericKeyBoard"), m_csScreenContext, UI::PROPERTYVISIBLE, v);

		}else if(IsOperatorGuidance())
		{
			m_csScreenContext = _T("HHSmAssistModeWithOperatorGuidance"); 
			ps.SetFrame(m_csScreenContext);
			m_pAppPSX->SetConfigProperty(_T("SMButton5"), m_csScreenContext, UI::PROPERTYSTATE, (long) UI::Disabled);
		}else
		{
			m_csScreenContext = _T("HHSmAssistMode"); 
			ps.SetFrame(m_csScreenContext);
				
		}	
		 		


}
