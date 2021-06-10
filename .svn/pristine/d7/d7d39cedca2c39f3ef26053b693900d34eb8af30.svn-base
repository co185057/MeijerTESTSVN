//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMSecUnExpectedDecreaseBase.cpp
//
// TITLE:	Class header for Sec Violation-SMSecUnExpectedDecrease
// AUTHOR:  Tara Duncan
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSecBagBoxThreshold.h"      
#include "SMSecItemRemovedThreshold.h"      
#include "SMSecNewItemEntry.h"      
#include "SMSecSkipBaggingThreshold.h"      
#include "SMSecUnexpectedDecrease.h"  
#include "SMSecUnexpectedIncrease.h"          
#include "SMScanAndBag.h"      
#include "SMScaleBroken.h"          // Reset scale
#include "SMSecMisMatchWeight.h"	// 229084 goto mismatched from decrease if coming from BagAndEAS LPM022503
#include "MPProcedures.h"			// RFC 233322

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SMSecUnexpectedDecrease")
DLLEXPORT extern HWND hw;           // RFC 233322
DLLEXPORT extern MPProcedures mp;   // RFC 233322

IMPLEMENT_DYNCREATE(SMSecUnexpectedDecreaseBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSecUnexpectedDecreaseBase::SMSecUnexpectedDecreaseBase(const BEST prevState, const BEST approvedNextState)
: csCurContext(EMPTY_STRING),
  bPrevStateBagAndEAS(false)
{
  m_PrevState = prevState;		// the state we were in when the unexpected decrease occurred 
  m_NextState = approvedNextState;	// the state we should goto when the unexpected decrease is approved
  IMPLEMENT_TIMESTAMP
}
//////////////////////////////////////////
SMSecUnexpectedDecreaseBase::SMSecUnexpectedDecreaseBase()
: csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSecUnexpectedDecreaseBase::Deliver(const BEST argPrevState, const BEST argApprovedNextState)
{
  m_PrevState = argPrevState;
  m_NextState = argApprovedNextState;
  return this;
}
////////////////////////////////////////
bool SMSecUnexpectedDecreaseBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be two parameters, both ints
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMSecUnexpectedDecreaseBase::DeliverDescriptorList() - Parameter Count of 2"));
        Deliver((BEST)DLParamDWORD(dlParameterList),    
                (BEST)DLParamDWORD(dlParameterList));   
        return true;
    }
    else
    {
        trace(L2, _T("+SMSecUnexpectedDecreaseBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}
///////////////////////////////////
SMStateBase  *SMSecUnexpectedDecreaseBase::Initialize(void)
{
	ra.RequestEnableAssistMode(false); //Tar 238294

    if(DATANEEDED_MOREDATA == m_View) //tar 252987 && 272557
    {
       m_PrevView = m_View;
    }

	// SR682-Show/hide Weight Removed and Number Of Items Sold under Unexpected Decrease screen.	
	if(co.fOperationsShowWeightInfoOnLane == true)
		ps.ShowCMWeightNumber(g_csExpectedWeight, g_csDeltaWeight, lItems);
	else
		ps.HideCMWeightNumber();

	//The text here is just the name of the section in scotdataentry.ini.  Don't need to be in SCOTMSGE.DAT
	CString csInstruction, csDesc;
	if (bIsInFinalization && g_bAllowRemove) 
		g_bAllowRemove = false;
	// TAR 253626 - sets a flag to true if we came to this
	// state from Bag Item state. This flag is later used 
	// to show Item Removed Violation state with Item Removed button
	CString cstemp = ps.GetCurrentContext();
	if (cstemp == _T("BagAndEAS") || cstemp == _T("SecMisMatchWeight")) //TAR379001
	{
		bPrevStateBagAndEAS = true;
	}
	else
	{
		bPrevStateBagAndEAS = false;
	}
	// end TAR 253626
    csCurContext = _T("SecUnexpectedDecrease");  // Tar 238987

	if ( bPrevStateBagAndEAS && co.fOperationsShowItemRemovedButtonOnRemoveItemViolationScreen)
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, csCurContext );
	}

	// RFC 233322 RJF 042103
	// Need to check if we should delay notifying RAP/pager of increase
	// in order to give customer chance to fix it themselves
	nEscalationRAPTimeOut = co.getTimeOutDelaySecurityNotification();
	if(nEscalationRAPTimeOut != 0)
	{
		SetTimer(hw, RAP_ESCALATION, nEscalationRAPTimeOut, &EscalationTimedOut);
	}
	else		// don't delay, turn on red
	{
		// TAR 314552 - removed the check from here. Since with new security, it sends 
		// intervention type of unexpected-dec if decrease and msimatch if mismatch weight.
		cDataEntry.FromConfigData(DATAENTRYINIFILE, _T("UnexpectedDecrease"));
		m_View = DATANEEDED_UNEXPECTEDDECREASE;
		csInstruction.Format(ps.GetPSText(RA_WEIGHTDECREASEINFO, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTDECREASE, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits, _T(""));
		ra.RequestEnableAssistMode(true); //Tar 238294

		if( secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_OUTSTANDING )
		{
			ra.OnRAPDataNeeded(csInstruction, cDataEntry.ToString(), m_View);
			SAEnterSecurityCondition();
		}
		else if( secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_DELAYED )
		{
		  trace(L7, _T("+ra.OnNormalItem, Message = %s"), csInstruction);
		  ra.OnNormalItem(csInstruction);
		  trace(L7, _T("-ra.OnNormalItem"));
		}
	}
	return STATE(SecOperatorPWState)::Initialize();
}

////////////////////////////////////////////
void SMSecUnexpectedDecreaseBase::showScreen(bool passwordVerified)
{
	nTimeOut = co.getTimeOutPeriodicSecurityAudio();		// RFC 233322

	if (passwordVerified)
		return;
	

		if (inputMode == PROMPTING_FOR_OPERATOR)
		{
            // NewUI
			STATE(SecOperatorPWState)::ShowEnterID();   //SR93 Biometrics support			
			ps.SetCMLeadthruText( TXT_ENTER_ID );
			ps.AllowInput(MAX_DIGITS_OPERATOR,false,false);
			ps.ShowCMFrame(false);
            // End NewUI
			return;
		}
		else
		{  // Display unexpected decrease screen....   
			//CHU 072502 RFC 208874+
			//DMSaySecurity(SEC_NONE, ps.GetSoundName(SEC_REMOVEDITEM));	// play wave here so it doesn't replay on the key pad screen
			// TAR 253626 changed the check from (m_PrevState == BES_BAGANDEAS) to use bPrevStateBagAndEAS flag
			if ( bPrevStateBagAndEAS && co.fOperationsShowItemRemovedButtonOnRemoveItemViolationScreen)
			{
				 if (!co.fOperationsSuppressAudioInItemRemoveViolation ) 
				 {
					DMSaySecurity(SEC_NONE, ps.GetSoundName(SEC_ITEMREMOVEDONREMOVEVIOLATION));
				 }
			}
			else
				DMSaySecurity(SEC_NONE, ps.GetSoundName(SEC_REMOVEDITEM));	
			//CHU 072502 RFC 208874-
            // NewUI
			ps.SetCMFrame(_T("SecUnexpectedDecrease"));
			COleVariant v = VARIANT_FALSE;
			if(nEscalationRAPTimeOut == 0)			// RFC 233322
			{
				v = VARIANT_TRUE;				// show the store mode button
			}
			m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), csCurContext, UI::PROPERTYVISIBLE, v);

			//CHU 072502 RFC 208874+
			// TAR 253626 changed the check from (m_PrevState == BES_BAGANDEAS) to use bPrevStateBagAndEAS flag
			if ( bPrevStateBagAndEAS  && co.fOperationsShowItemRemovedButtonOnRemoveItemViolationScreen)
			{	v = VARIANT_TRUE;
				ps.CMButton(_T("CMButton1Med"), BTT_ITEMREMOVED_ONITEMREMOVEVIOLATION, true);
			}
			else
			{
				v = VARIANT_FALSE;
			}
			//CHU 072502 RFC 208874-
			m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, v);
            // End NewUI
		}


	if (dm.fStateTakeawayBelt)
		ps.SetCMLeadthruText(MSG_ATTENTION_ALERT);
	else
		ps.SetCMLeadthruText(MSG_BAGGING_ALERT);
    // End NewUI
	SAWLDBScannerDisable();			// Disable the scanner
	ps.ShowCMTBText(csTBMessage2Scot);	//thp
	//CHU 072502 RFC 208874+
	//ps.Message12(MSG_OVERALL_WEIGHT_DECREASE, true);
	// TAR 253626 changed the check from (m_PrevState == BES_BAGANDEAS) to use bPrevStateBagAndEAS flag
	if ( bPrevStateBagAndEAS && co.fOperationsShowItemRemovedButtonOnRemoveItemViolationScreen)
		ps.Message12(MSG_RETURNITEMS_TO_BAGGING_ONITEMREMOVEVIOLATION, true);
	else
		ps.Message12(MSG_OVERALL_WEIGHT_DECREASE, true);
    //CHU 072502 RFC 208874-
	trace(L7, _T("+ra.OnTransactionState, Itemization State"));
	ra.OnSetTransactionState(RA_STATE_SECURITY_VIOLATION);
	trace(L7, _T("-ra.OnTransactionState"));
	ps.ShowCMFrame(false);
}

//////////////////////////////////////
void SMSecUnexpectedDecreaseBase::UnInitialize(void)
{
	if ( bPrevStateBagAndEAS && co.fOperationsShowItemRemovedButtonOnRemoveItemViolationScreen)
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, csCurContext );
	}
	ra.RequestEnableAssistMode(true); //Tar 238294
	// RFC 233322 RJF 041603 ensure that the RAP escalation timer has stopped
    KillTimer(hw,RAP_ESCALATION);
	KillTimer(hw, ITEM_REMOVE_VIOLATION);
	if (bIsInFinalization) 
		g_bAllowRemove = true;
    DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 
	ClearSecurityViolationLight();
}

///////////////////////////////////////
SMStateBase  *SMSecUnexpectedDecreaseBase::PSButtonGoBack(void) // store login
{
	//TAR193505+: At EnterID screen, "go back" should be back to SMSecUnExpectedIncreaseBase screen

		switch(inputMode)
		{  
			case SHOW_WAIT_FOR_APPROVAL:
			case PROMPTING_FOR_PASSWORD:
				break;
			case PROMPTING_FOR_OPERATOR:
				{
					//+SR93 Biometrics support
					if (co.m_bOperationsUseFingerPrintDevice)
					{
						FT_Abort();
					}
					//-SR93 Biometrics support

					inputMode = SHOW_WAIT_FOR_APPROVAL;
					if (co.fStateDualLanguage)
						ps.SetLanguage(m_customerLanguage);  //TAR 196315: Set back to customerLanuguage for dual language
					nEscalationRAPTimeOut = 0;
					showScreen(false);
					return STATE_NULL;  
				}
		}

	//TAR193505-

    DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 
	promptForOperator();
    return STATE_NULL;
}
  
/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMSecUnexpectedDecreaseBase::EscalationTimedOut
//
// Purpose:		handles passing timeout event to Escalation2RAP using the current state pointer
//
// Parameters:	none
//
// Returns:		void
//
/////////////////////////////////////////////////////////////////////////////////////
VOID CALLBACK SMSecUnexpectedDecreaseBase::EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	mp.sCurr->Escalation2RAP();
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMSecUnexpectedDecreaseBase::Escalation2RAP
//
// Purpose:		2nd timeout routine handling the delayed security notification
//
// Parameters:	none
//
// Returns:		void
//
/////////////////////////////////////////////////////////////////////////////////////
void SMSecUnexpectedDecreaseBase::Escalation2RAP()
{   // RFC 233322 - separate timer for escalation to RAP
    KillTimer(hw,RAP_ESCALATION);       // ensure that the RAP escalation timer has stopped
	//TAR240514 CHU reset this so if we come back from EnterPWD or EnterID we show
	//"store login" button.
	nEscalationRAPTimeOut = 0;

	CString csInstruction, csDesc;
	// TAR 314552 - removed the check from here. Since with new security, it sends 
	// intervention type of unexpected-dec if decrease and msimatch if mismatch weight.
	cDataEntry.FromConfigData(DATAENTRYINIFILE, _T("UnexpectedDecrease"));
	m_View = DATANEEDED_UNEXPECTEDDECREASE;
	csInstruction.Format(ps.GetPSText(RA_WEIGHTDECREASEINFO, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTDECREASE, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits, _T(""));

	ra.RequestEnableAssistMode(true); //Tar 238294

	if( secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_OUTSTANDING )
	{
		ra.OnRAPDataNeeded(csInstruction, cDataEntry.ToString(), m_View);
		SAEnterSecurityCondition();
	}
	else if( secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_DELAYED )
	{
	  trace(L7, _T("+ra.OnNormalItem, Message = %s"), csInstruction);
	  ra.OnNormalItem(csInstruction);
	  trace(L7, _T("-ra.OnNormalItem"));
	}
	m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
}

///////////////////////////////////////
SMStateBase *SMSecUnexpectedDecreaseBase::TimedOut(void) // timeout
{

	//CHU 072502 RFC 208874+
	//DMSaySecurity(SEC_NONE, ps.GetSoundName(SEC_REMOVEDITEM));	// play wave here so it doesn't replay on the key pad screen
	// TAR 253626 changed the check from (m_PrevState == BES_BAGANDEAS) to use bPrevStateBagAndEAS flag
    if ( bPrevStateBagAndEAS && co.fOperationsShowItemRemovedButtonOnRemoveItemViolationScreen )
	{
		 if (!co.fOperationsSuppressAudioInItemRemoveViolation ) 
		 {
			DMSaySecurity(SEC_NONE, ps.GetSoundName(SEC_ITEMREMOVEDONREMOVEVIOLATION));
		 }
	}
	else
		DMSaySecurity(SEC_NONE, ps.GetSoundName(SEC_REMOVEDITEM));	// play wave here so it doesn't replay on the key pad screen
	//CHU 072502 RFC 208874-

	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecUnexpectedDecreaseBase::stateAfterPasswordVerify()
{
	ps.SetLanguage(m_customerLanguage); //tar 192275
	lSARedNeedingApproval = false;   // TAR 197015 Clear alert flag.
	// do same thing that OnRAPClearSecurityBtn() does
	return OnRAPClearSecurityBtn(_T("Lane"));
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecUnexpectedDecreaseBase::OnBackToLGW(void)
{

    DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 

	// TAB re-enable scanner and clear Wait For Approval State
	TABReEnableScannerAndClearWaitForApprovalState();

	// +218558 EAS motion sensor received in the wrong order. don't need this code anymore LPM103002
	// see if need to wait on EAS because returning to the bag and EAS screen LPM022002 TAR096236
	// so if the EAS event has not been received, 
	//if (!bEASReEnableScanner)				// this is set to true once the EAS event is received
	//	g_bMatchedWtWaitingOnEAS = true;	// set the waiting on EAS back to true when leaving this state.
	//218558 if matched waiting on EAS, and received EAS and customer corrected
	// their own violation (BackToLGW), then return to ScanAndBag
	if (g_bMatchedWtWaitingOnEAS && m_bEASHappened)
		m_PrevState = BES_SCANANDBAG;
	//TAR213414+ CHU 08/30/02: the next state should be the anchorstate
	//if there is no approval left. We should go back to anchorstate which is the state 
	//before we get in Approval state(s)
	else if ( !isAnyApprovalRequired() ) //&& m_PrevState != BES_RAPDATANEEDED) dale no TAR LPM093002
	{
		if (m_PrevState == BES_WAITFORAPPROVAL )
		{
		     m_PrevState = getAnchorState();
		}
    //TAR213414-
		//+tar410912
		if(getAnchorState() == BES_UNKNOWNITEM)
		{
			setAnchorState(BES_SECUNEXPECTEDDECREASE);
		}
		//-tar410912
	}

	return ClearedSecViolSetReturnState(m_PrevState, m_NextState);
	//TAR213414-
	
}

///////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSecUnexpectedDecreaseBase::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
	//Clear RAP Approval
#ifndef NewSec

	if(dm.fStateTakeawayBelt)	// is on a take away system
	{
		ss.SetSkipBagging(false); //TAR199154 false: tell SS not stop all timers
	}
#endif
	if ( (m_PrevState == BES_BAGANDEAS) && (!m_bEASHappened) )	// don't want to set bEASReEnableScanner to true
	{
		trace(L2, _T("Special case, EAS not detected yet. Returning to Bag Item. Clearing RAP DataNeeded."));
		bEASReEnableScanner = false;	// we know better, cause the EAS hasn't happened yet 
	}
	else
	{
		trace(L2, _T("bEASReEnableScanner is set to true. Clearing RAP DataNeeded."));
		bEASReEnableScanner = true;
	}
	ra.OnNoRAPDataNeeded();  
    // NewSec
    OverrideSecurityException(lpszAuthorizationType, KEY_EXCEPTION_OPERATION_CLEAR);
	
	//+tar410912/412614
	if(getAnchorState() == BES_UNKNOWNITEM || getAnchorState() == BES_DEGRADEDCONTINUETRANS || 
	   getAnchorState() == BES_TIMERESTRICTEDITEM || getAnchorState() == BES_COUPONTOOHIGH ||
	   getAnchorState() == BES_COUPONNOMATCH || getAnchorState() == BES_ITEMNOTFORSALE ||
	   getAnchorState() == BES_ITEMQUANTITYEXCEEDED )
	{
		return STATE_NULL;
	}
	//-tar410912/412614

	//427943+
	//wait here so SM will tell FL where to go next once SM processes OverrideSecurityException
	return STATE_NULL;
//USSF START
	//USSFHOOK(_T("OnBackToLGW"), nextState = OnBackToLGW(), return nextState;);
	//return OnBackToLGW();
//USSF END
	//427943-	
}

///////////////////////////////////////
//CHU 072502 RFC 208874
SMStateBase  *SMSecUnexpectedDecreaseBase::PSButton1(void) //Skip bagging button
{
    
  DMSaySecurity( SEC_NONE, _T("blank.wav") );			//to interrupt the wave file if it's playing 153796
  
 if(fInValidOperatorPassword)
  {
	return STATE(SecOperatorPWState)::PSButton1();
  }

  //Set prevstate is BES_PICKINGUPITEMS and anchor state to ScanAndBag, so we go back to ScanAndBag
  //if m_PrevState is BES_SCANANDBBAG, it means that OnItemOK has been received 
  if (m_PrevState != BES_SCANANDBAG ) //407358
  {
	m_PrevState = BES_PICKINGUPITEMS;
  }
  trace(L6, _T("m_PrevState is %d"), m_PrevState);//407358
  setAnchorState(BES_SCANANDBAG);

  CKeyValueParmList parmList;
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_SKIP_BAGGING);
  _bstr_t bstrParmList((const _TCHAR *)parmList);
  BSTR bstrResult;
  bstrResult = NULL;
  SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
  SysFreeString(bstrResult);
  // + TAR 311901
  if ((secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_UNEX_DECREASE) && 
	  (secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_OUTSTANDING) )
  {
    OverrideSecurityException(_T("None"), KEY_EXCEPTION_OPERATION_CLEAR);

  }
  // - TAR 311901
  // do same thing that OnBackToLGW does
  return OnBackToLGW();
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecUnexpectedDecreaseBase::OnMatchedWt()
{
	// +218558 Added override function.  EAS motion sensor received in the wrong order must behave differently LPM103002
	if (!m_bEASHappened)	// the matched wt IS waiting on an EAS event
	{
		trace(L2, _T("SMSecUnexpectedDecreaseBase::OnMatchedWt, waiting for EAS motion sensor, returning to BagAndEAS (Bag Item)"));
		return setAndCreateAnchorState(BES_BAGANDEAS);
	}
	else
	{
		trace(L2, _T("SMSecUnexpectedDecreaseBase::OnMatchedWt, Calling base behavior."));
		return SMStateBase::OnMatchedWt();
	}
}

///////////////////////////////////////////
// EAS Motion Sensor detected motion ended
///////////////////////////////////////////
SMStateBase  *SMSecUnexpectedDecreaseBase::DMdf_EASNoMotion(void) 
{
	//218558 allow EAS to occur in unexpected increase  LPM103002
	m_bEASHappened = true;	// remember this for when the violation is corrected by the customer	
	// The sensor has been detected, remember it 
	SASendSCOTInput(PRODUCT_TAG_DEACTIVATED, io.csItemCode, 0, 0, 0, 0, 0);
	// wait here until the increase in corrected
	return STATE_NULL;
}

////////////////////////////////////////////////////////////////////////////////
// 229084 Override unexpected increase if coming from BagAndEAS after a decrease
///////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecUnexpectedDecreaseBase::OnWtIncreaseNotAllowed()	 
{
	if (   ((m_PrevState == m_NextState) && (m_PrevState == BES_ASSISTEDTENDER))
	    || ((m_PrevState == m_NextState) && (m_PrevState == BES_RAPDATANEEDED)) )
	{
		return VoidingWtIncrease(m_PrevState, m_NextState);
	}
	if ( BES_BAGANDEAS == m_PrevState )   //Fix TAR330891
	{	
		//unmatched weights while on the bag and EAS mean a mismatched wt so return here, else if cleared, return to s&b
		CREATE_AND_DISPENSE(SecMisMatchWeight)(BES_BAGANDEAS, getNextStateForGoodItem() );
	}

    return SMStateBase::OnWtIncreaseNotAllowed();
}

SMStateBase *SMSecUnexpectedDecreaseBase::OnWtDecrease()
{
	// normal case where removing is not allowed.
	CString csInstruction, csDesc;

	// SR682-Refresh Weight Removed in the event of a weight change.
	if(co.fOperationsShowWeightInfoOnLane == true)
		ps.ShowCMWeightNumber(g_csExpectedWeight, g_csDeltaWeight, lItems);

	// TAR 314552 - removed the check from here. Since with new security, it sends 
	// intervention type of unexpected-dec if decrease and msimatch if mismatch weight.
	cDataEntry.FromConfigData(DATAENTRYINIFILE, _T("UnexpectedDecrease"));
	m_View = DATANEEDED_UNEXPECTEDDECREASE;
	csInstruction.Format(ps.GetPSText(RA_WEIGHTDECREASEINFO, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTDECREASE, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits, _T(""));
	ra.RequestEnableAssistMode(true); //Tar 238294

	if( secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_OUTSTANDING )
	{
		ra.OnRAPDataNeeded(csInstruction, cDataEntry.ToString(), m_View);
	}
	else if( secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_DELAYED )
	{
	  trace(L7, _T("+ra.OnNormalItem, Message = %s"), csInstruction);
	  ra.OnNormalItem(csInstruction);
	  trace(L7, _T("-ra.OnNormalItem"));
	}
	return STATE_NULL;
}
