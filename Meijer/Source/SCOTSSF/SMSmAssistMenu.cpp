//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAssistMenu.CPP
//
// TITLE: Class implementation for store mode AssistMenu state
//
//
// AUTHOR:    Peter Denhoed
//
// CRD124746 Work Request:Sprint 10.3 Name:Aparna Tunuguntla Date: March 19,2018
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"
#include "SMSmAssistMenu.h"

#include "transbrokerssf.h"
#include "SMInProgress.h"
#include "SMSmHandHeldAssistMode.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmAssistMenu")
#define MAX_HANDHELD_BUTTONS 4

DLLIMPORT extern HWND hw; //CRD124746


IMPLEMENT_DYNCREATE(SMSmAssistMenu, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

CString SMSmAssistMenu::m_csOptionValue = _T("");
CString SMSmAssistMenu::m_csBtnStateOptionValue = _T("");
bool SMSmAssistMenu::m_bTBDefinesButton = false;
std::vector<TBButtonList> SMSmAssistMenu::m_SButtonList;

//////////////////////////////////////
SMSmAssistMenu::SMSmAssistMenu()
:m_bUsingHandHeldWithSpecialScreen(false),
m_bHHMoreBtnOn(false),
m_csDefaultBTn(_T("")),
m_lHHBtnIndex(0)
{
	IMPLEMENT_TIMESTAMP
		
	CString csOptionName = _T("HandHeldAssistMenuButtonList");
	CString csOptionValue(_T(""));
	SendTBCommand(TBCMD_GET_OPTIONS, &csOptionName, &csOptionValue);
	
	csOptionName = _T("HandHeldAssistMenuInitialButtonState");
	CString csBtnStateOptionValue(_T(""));
	SendTBCommand(TBCMD_GET_OPTIONS, &csOptionName, &csBtnStateOptionValue);
	
	//if the TB sent back a btn list compare it to the one we previously
	//had.  If no changes then no need to do this work.
	if ((m_csOptionValue != csOptionValue) || 
		(m_csBtnStateOptionValue != csBtnStateOptionValue))
	{
		m_SButtonList.clear();
		
		m_csOptionValue.Format(_T("%s"), csOptionValue);
		m_csBtnStateOptionValue.Format(_T("%s"), csBtnStateOptionValue);
		
		CString csTempBtnList;
		CString csTempBtnState;
		
		csTempBtnList.Format(_T("%s"), csOptionValue);
		csTempBtnState.Format(_T("%s"), csBtnStateOptionValue);;
		
		m_bTBDefinesButton = false;
		if(!csOptionValue.IsEmpty())
		{
			m_bTBDefinesButton = true;
			
			//make sure the string returned ends with ';'
			if(csTempBtnList[csTempBtnList.GetLength() - 1] != _T(';'))
			{
				csTempBtnList += _T(';');
			}
			
			if((!csTempBtnState.IsEmpty()) && 
				(csTempBtnState[csTempBtnState.GetLength() - 1] != _T(';')))
			{
				csTempBtnState += _T(';');
			}
			
			//now add the buttons to the array
			TBButtonList tbblButton;
			
			LPCTSTR psz;
			
			psz = _tcstok((LPTSTR)(LPCTSTR)csTempBtnList, _T(";"));
			CString csButton; 
			while(psz)
			{	
				
				csButton = psz;
				int i = csButton.Find(',');
				if(i != -1)
				{
					tbblButton.csButtonName = csButton.Left(i);
					tbblButton.csKeySeq = csButton.Mid(i +1);
					
				}else
				{
					tbblButton.csButtonName = csButton;
					tbblButton.csKeySeq = _T("");
				}
				
				m_SButtonList.push_back(tbblButton);
				
				psz = _tcstok(NULL, _T(";"));
				
				
				
			}
			
			//setup button states
			psz = _tcstok((LPTSTR)(LPCTSTR)csTempBtnState, _T(";"));
			CString csButtonState; 
			while(psz)
			{	
				CString csButtonName, csButtonStatus;
				csButtonState = psz;
				int i = csButtonState.Find(',');
				if(i != -1)
				{
					csButtonName = csButtonState.Left(i);
					csButtonStatus = csButtonState.Mid(i +1);
					csButtonName.Delete(0, CString(_T("Button")).GetLength());
					if(_istdigit(csButtonName[0]))
					{
						
						long lBtnIndex = _ttol(csButtonName);
						if(lBtnIndex <= m_SButtonList.size())
						{
							if(lBtnIndex > 0)
							{
								lBtnIndex--;
							}
							m_SButtonList[lBtnIndex].csBtnState = csButtonStatus;
						}
					}
					
					
				}
				
				psz = _tcstok(NULL, _T(";"));
				
				
				
			}
			
		}
	}
}

SMStateBase * SMSmAssistMenu::Initialize(void)
{
	
	//(+) CRD124746
	StopHHScannerTimer();
	trace(L6, _T("*** Assistmenu killing timer intialize"));
	//(-) CRD124746

	//POS54351 - go directly to full AM during mobile order trx
    if( ps.RemoteMode() && GetHookProperty(_T("PropMobShopOrderPrompt")) == _T("1") )
    {
        return OnAssistModeButton();
    }
    //e POS54351 
	//	start CRD 125377
	trace(L6, _T("m_bShopAndDropEnabled=%d m_bSNDAllowItemMenu=%d"), m_bShopAndDropEnabled, m_bSNDAllowItemMenu);
	if (m_bShopAndDropEnabled)
	{
		m_bSNDAllowItemMenu = true;		// CRD 125377
		trace(L6, _T("m_bSNDAllowItemMenu=%d"), m_bSNDAllowItemMenu);
	}
	//  end CRD 125377
	
	// if this is a Handheld connection and the TB wants to define the 
	if(( ra.IsRemoteConnectionHandheld(ra.GetRemoteConnectionName())) && 
		(ps.RemoteMode()) &&
		(m_bTBDefinesButton))
    {
		m_bUsingHandHeldWithSpecialScreen = true;
	}else
	{
		//if we are not a remote HH connection and not using the Meijer
		//customized screens ensure that we are using the right context
		//other the system may not know how to display the screen
		if(m_csMenuContext == _T("HHSmAssistMenu"))
		{
			m_csMenuContext = _T("SmAssistMenu");
		}
	}

    SMStateBase *pNextState =  SMSmAssistMenuBase::Initialize();
	SetHookProperty(_T("IsInStoreMode"),_T("1"));	//CRD 232267
	
    if (ms_bKeepRemoteDevices)
    {
        trace (L6, _T("SMSmAssistMenu::Initialize() Re-entering AssistMenu State after keeping remote devices."));
        // We are coming back into AssistMode state from another remote state
        // for which we didn't unremote the devices, so there is no need to re-remote them.
        //tar 228763 && 272625
        // +SSCOI-45075
		 if (ps.RemoteMode())
		 {
			trace (L6, _T("Scanner is disabled."));
			SAWLDBScannerDisable();
		 }
		 else
		 {
			 trace (L6, _T("Scanner is enabled."));
			 SAWLDBScannerEnable();
		 }
		//- SSCOI-45075	 
        trace(L0, _T("SMSmAssistMenu::Initialize() Kept Remote Devices"));
    } // of if (ms_bKeepRemoteDevices)
    else
    {
        if (ps.RemoteMode())
        {
            //SAWLDBScannerDisable();							  // disable SCOT scanner in remote mode
            trace (L6, _T("SMSmAssistMenu::Initialize() Remoting devices..."));
            
            dm.BeginRemote();
			//dm.LoadPrintObjects();  //load bitmap to memory //never used
            ms_bKeepRemoteDevices = true;
            // one can do some 2x20 display msg specific processing here
            
            //For Japan and other countries/customers, it is possible to have an MSR without
            //having a keylock.
            if (dm.IsDeviceRemote(DMCLASS_MSR, 0))  // found the need of this condition while merging customer changes to Patch C
            {
                DMMSREnable();
            }
            else
            {
                trace (L0, _T("SMSmAssistMenu::Initialize() Remote device MSR failed"));
            }
	        //+ SSCOI-45075	 
            //SAWLDBScannerEnable();
	        trace (L6, _T("Scanner is disabled."));
	        SAWLDBScannerDisable();
	        //- SSCOI-45075	
        }// end of if (ps.RemoteMode())  
        else
        {
            SAWLDBScannerEnable();
        } // else if (ps.RemoteMode())  
        
    } // else if (ms_bKeepRemoteDevices)

	//The m_bNeedAssistMode flag is only used for the HandHeld
	//to determine if we are really trying to go into AM at the HH
	//base FL does not support going directly to AM on the HH
	if(m_bNeedAssistMode)
	{
		m_bNeedAssistMode = false;
		trace(L0, _T("Hand Held Assist Mode requested"));
		delete getRealStatePointer( pNextState);
        RETURNSTATE( SmHandHeldAssistMode)
	}

	if(m_bTrxnEndedInHandHeld)
	{
		m_bTrxnEndedInHandHeld = false;
		trace(L0, _T("Trxn ended in Hand Held exiting"));
		delete getRealStatePointer( pNextState);

		return PSButton5();

	}

	//get the defaults buttons states here before we try to setup the TB BTNS
	
	//we are staying in AssistMenu screen and need to show the right buttons
	if((pNextState == STATE_NULL) && (m_bUsingHandHeldWithSpecialScreen))
	{	
		
		SetupHHButtons();
	}
	
	return pNextState;
}

void SMSmAssistMenu::UnInitialize(void)
{
    m_bHHMoreBtnOn = 0;
	m_lHHBtnIndex = 0;
	SMSmAssistMenuBase::UnInitialize();

	//Check if the devices (printer & cash drawer) are still remote
	if( !ps.RemoteMode() && (dm.IsDeviceRemote(DMCLASS_RECEIPT, 0) || dm.IsDeviceRemote(DMCLASS_CASHDRAWER, 0)))
	{
		//We're not in remote mode but the devices are still remote devices are connected
		trace (L6, _T("SMSmAssistMenu::UnInitialize. We're not in remote mode. Disconnecting remote devices."));		
        dm.EndRemote();
		ms_bKeepRemoteDevices = false;
	}
}

SMStateBase * SMSmAssistMenu::DMScanner(void)
{
    return SMSmAssistMenuBase::DMScanner();
}

SMStateBase * SMSmAssistMenu::TBParse(MessageElement* me)
{
    return SMSmAssistMenuBase::TBParse(me);
}

SMStateBase * SMSmAssistMenu::TimedOut(void)
{
    return SMSmAssistMenuBase::TimedOut();
}

SMStateBase * SMSmAssistMenu::PSNumericKey(const long lKey)
{
    return SMSmAssistMenuBase::PSNumericKey(lKey);
}

SMStateBase * SMSmAssistMenu::PSAlphaNumericKey(const long lKey)
{
    return SMSmAssistMenuBase::PSAlphaNumericKey(lKey);
}

SMStateBase * SMSmAssistMenu::PSButton1(void)
{
    if((m_csDefaultBTn.Find(_T("SMButton1")) != -1) || (!m_bUsingHandHeldWithSpecialScreen))
	{
		return SMSmAssistMenuBase::PSButton1();
	}
	
	return SendBtnCmdtoTB(HH_BUTTON1);
}

SMStateBase * SMSmAssistMenu::PSButton2(void)
{
    if((m_csDefaultBTn.Find(_T("SMButton2"))!= -1) || (!m_bUsingHandHeldWithSpecialScreen))
	{
		return SMSmAssistMenuBase::PSButton2();
	}
	
	return SendBtnCmdtoTB(HH_BUTTON2);
}

SMStateBase * SMSmAssistMenu::PSButton3(void)
{
	if((m_csDefaultBTn.Find(_T("SMButton3"))!= -1) || (!m_bUsingHandHeldWithSpecialScreen))
	{
		return SMSmAssistMenuBase::PSButton3();
	}
	return SendBtnCmdtoTB(HH_BUTTON3);
}

SMStateBase * SMSmAssistMenu::PSButton4(void)
{
    if((m_csDefaultBTn.Find(_T("SMButton4"))!= -1) || (!m_bUsingHandHeldWithSpecialScreen))
	{
		return SMSmAssistMenuBase::PSButton4();
	}
	
	
	return SendBtnCmdtoTB(HH_BUTTON4);
}

//return to shopping button for HH
SMStateBase * SMSmAssistMenu::PSButton5(void)
{
    if(!m_bUsingHandHeldWithSpecialScreen)
	{
		return SMSmAssistMenuBase::PSButton5();
	}else		
	{
		
		return SMSmAssistMenuBase::PSButton8();;
	}

}

//more button for HH
SMStateBase * SMSmAssistMenu::PSButton8(void)
{
	//(+)CRD124746
	if(SMState::m_bActivateHHScanner)
	{	
	CString csRemoteConnectionName = ra.GetRemoteConnectionName();
	CString csOperation=_T("operation=release-scanner;");
	m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), csOperation, csRemoteConnectionName );
	trace(L6, _T("Release scanner."));
	}
	//(-)CRD124746
		
	//Start CRD 366323
	NotifyExitStoreMode();
	//End CRD 366323

    if (!m_bUsingHandHeldWithSpecialScreen)
	{
		return SMSmAssistMenuBase::PSButton8();
	}
	
	if(m_bHHMoreBtnOn)
	{
		SetupHHButtons((m_lHHBtnIndex + MAX_HANDHELD_BUTTONS ) );
		
	}
	
	return STATE_NULL;
}

SMStateBase * SMSmAssistMenu::PSRemoteButton1(void)
{
    return SMSmAssistMenuBase::PSRemoteButton1();
}

SMStateBase * SMSmAssistMenu::PSCmdButton(CString csContext, CString csControl, CString csCommand)
{
    return SMSmAssistMenuBase::PSCmdButton(csContext, csControl, csCommand);
}

SMStateBase * SMSmAssistMenu::PSSwitchContext(BOOL bRemote, CString csNewContext)
{
    return SMSmAssistMenuBase::PSSwitchContext(bRemote, csNewContext);
}

SMStateBase * SMSmAssistMenu::PSOnRemoteDisconnected(const long lParm)
{
    return SMSmAssistMenuBase::PSOnRemoteDisconnected(lParm);
}

SMStateBase * SMSmAssistMenu::OnRAPUnsolicitedDataReceived(LPCTSTR sData)
{
    return SMSmAssistMenuBase::OnRAPUnsolicitedDataReceived(sData);
}

SMStateBase * SMSmAssistMenu::OnChangePrice()
{
    return SMSmAssistMenuBase::OnChangePrice();
}

SMStateBase * SMSmAssistMenu::OnChangeQuantity()
{
    return SMSmAssistMenuBase::OnChangeQuantity();
}

//Start CRD 185620
SMStateBase * SMSmAssistMenu::OnReturnToShopping(void)
{
    SMStateBase *pRet = SMSmAssistMenuBase::OnReturnToShopping();

	//return to Finish if Generic delayed is cleared instead of S&B
	if(!isProcessingGenericDelayedIntervention() && !bEASApproveFromVoid)
    {
        setAnchorState(BES_FINISH);
    }

	return pRet;
}
//End CRD 185620

SMStateBase * SMSmAssistMenu::OnVoidButton()
{
    return SMSmAssistMenuBase::OnVoidButton();
}

SMStateBase * SMSmAssistMenu::OnAssistModeButton()
{
    return SMSmAssistMenuBase::OnAssistModeButton();
}

SMStateBase * SMSmAssistMenu::OnKeyInCode()
{
    return SMSmAssistMenuBase::OnKeyInCode();
}

CString SMSmAssistMenu::ExtractString(CString strData, CString sStringName)
{
    return SMSmAssistMenuBase::ExtractString(strData, sStringName);
}

void SMSmAssistMenu::SetButtonStates()
{
    trace(L6,_T("+SMSmAssistMenu::SetButtonStates()"));
    
    SMSmAssistMenuBase::SetButtonStates();
	
    
	
	if(m_bUsingHandHeldWithSpecialScreen)
	{	
		
		SetupHHButtons(m_lHHBtnIndex);
	}
	
    //RFQ 986
    trace(L6,_T("-SMSmAssistMenu::SetButtonStates()"));
}

bool SMSmAssistMenu::IsFullAssistNeeded()
{
    return SMSmAssistMenuBase::IsFullAssistNeeded();
}

SMStateBase * SMSmAssistMenu::OnWtDecrease(void)
{
    return SMSmAssistMenuBase::OnWtDecrease();
}

void SMSmAssistMenu::ShowAssistModeButton()
{
    SMSmAssistMenuBase::ShowAssistModeButton();
}

void SMSmAssistMenu::SetupHHButtons(long lBtnIndex)
{
	
	CString csButtonName;
	long lTotalButtons = m_SButtonList.size();
	bool bMoreBtnNeed = false;
	
	m_csDefaultBTn.Empty();
	
	m_lHHBtnIndex = lBtnIndex;
	
	//if the global btn index is greater than the total btn
	//then we need to go to the begining of the btn list
	if(m_lHHBtnIndex > lTotalButtons)
	{
		m_lHHBtnIndex = 0;
	}
	
	//switch to the correct context for HH	
	m_csMenuContext = _T("HHSmAssistMenu");
	//ps.GetPSXObject()->SetControlProperty( _T("Display"), UI::PROPERTYREDRAW, VARIANT_TRUE );
	//ps.GetPSXObject()->SetControlProperty(_T("HHSmAssistMenu"), UI::PROPERTYREDRAW, VARIANT_TRUE);
	
	ps.SetFrame(_T("HHSmAssistMenu"));
	
	ps.SetTitle(LTT_ASSISTMENUTITLE);
	
	
	
	
	
	//turn on the more button
	if(lTotalButtons > MAX_HANDHELD_BUTTONS) 
	{
		ps.Button(_T("SMButton8"), BTT_NEXT, true);
		ps.ButtonState(_T("SMButton8"), true, true);
		
		m_bHHMoreBtnOn = true;
	}else
	{
		m_bHHMoreBtnOn = false;
		ps.Button(_T("SMButton8"), BTT_NEXT, false);
		ps.ButtonState(_T("SMButton8"), false, true);
	}
	
	for(int i=1; (i<= MAX_HANDHELD_BUTTONS && ((m_lHHBtnIndex + i) <= lTotalButtons)) ; i++)
	{
		
		csButtonName.Format(_T("SMButton%d"), i);
		
		
		//if this is a button that FL controls make it as default and don't force it to
		//be on.  Place it in a list so we can check it later when the actual button is pressed
		//to see how to handle its behavior
		if(m_SButtonList[m_lHHBtnIndex + i -1].csKeySeq.Find(_T("Default")) != -1)		
		{
			m_csDefaultBTn += csButtonName;
			
		}
		
		ps.Button(csButtonName, m_SButtonList[m_lHHBtnIndex + i-1].csButtonName, !m_SButtonList[m_lHHBtnIndex + i-1].csButtonName.IsEmpty());
		
		if(m_SButtonList[m_lHHBtnIndex + i-1].csBtnState == _T("Item"))
		{
			
			ps.ButtonState(csButtonName, AreThereAnySoldVoidedItems(), true);
			
		}else
		{
			ps.ButtonState(csButtonName, !m_SButtonList[m_lHHBtnIndex + i-1].csButtonName.IsEmpty(), true);
		}
		
		
		
		
		
	}
	
	//clear off the buttons all the rest of the buttons if we did not 
	//set them
	for(i; ((i<= MAX_HANDHELD_BUTTONS) && (m_bHHMoreBtnOn)) ; i++)
	{
		csButtonName.Format(_T("SMButton%d"), i);
		
		ps.Button(csButtonName, _T(""), false);			
		ps.ButtonState(csButtonName, false, true);
		
		
	}
	
   	ps.ShowFrame();
	
}

SMStateBase  * SMSmAssistMenu::SendBtnCmdtoTB(long lBtnIndex)
{
	CString csKeyCmd = m_SButtonList[m_lHHBtnIndex + lBtnIndex -1].csKeySeq;
	
	if(SendTBCommand(TBCMD_SEND_ASSISTMENU_KEY_PRESS, csKeyCmd, NULL) == 1)
	{
		CREATE_AND_DISPENSE(InProgress)(storeMode(), helpMode(), co.getTimeOutSignoff());
	}
	
	return STATE_NULL;
}

SMStateBase * SMSmAssistMenu::DepositCouponsIfNecessary()
{
    return SMSmAssistMenuBase::DepositCouponsIfNecessary();
}