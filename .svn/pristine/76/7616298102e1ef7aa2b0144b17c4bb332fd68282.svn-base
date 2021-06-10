//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMPickingUpItemsBase.CPP
//
// TITLE: Class implementation for Removing Items when it is allowed base state
//
// AUTHOR: Tara Duncan, Thanhhieu Pham, Leila Parker
//
/////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMPickingUpItemsBase.h"      
#include "SMScaleBroken.h"          // Reset scale
#include "SMSecUnExpectedIncrease.h"
#include "SMSecItemRemovedThreshold.h"


#include "MPProcedures.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("PickingUpItems")
DLLEXPORT extern CSCOTDataEntry cDataEntry;
DLLEXPORT extern HWND hw;           // RJF 5/28/02 RFC 202656
DLLEXPORT extern MPProcedures mp;   // RJF 5/28/02 RFC 202655


IMPLEMENT_DYNCREATE(SMPickingUpItemsBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP
//T.D 05/14/02 204132  
static bool bScreenTimerExpired = false;
static bool bIsAutoAdvanceInPickingUpItem = false;


//////////////////////////////////////////
SMPickingUpItemsBase::SMPickingUpItemsBase()
: nCountForPeriodicRemovingItemsAudio(0), csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
}
///////////////////////////////////
///////////////////////////////////////
SMStateBase  *SMPickingUpItemsBase::Initialize(void)
{	
	//Tar 238294
	ra.RequestEnableAssistMode(false); 
    csCurContext = _T("PickingUpItems");  //  Tar 238987
	// Tell the security sub-system that we are in picking up item for the 
	// Auto Advance logic
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, csCurContext );

	bIsAutoAdvanceInPickingUpItem = GetSecurityConfig(CONFIG_KEY_REMOVING_ITEMS_AUTO_ADVANCE);

		// SR682-Show/hide Weight Removed and Number Of Items Sold under Picking Up Items screen.
	if(co.fOperationsShowWeightInfoOnLane == true)
		ps.ShowCMWeightNumber(g_csExpectedWeight, g_csDeltaWeight, lItems);
	else
		ps.HideCMWeightNumber();


	//Update RAP window with SCOT state
	if (dm.fStateTakeawayBelt)
		UpdateStateName(ps.GetPSText(MSG_ATTENTION_ALERT, SCOT_LANGUAGE_PRIMARY));
    else
		UpdateStateName(ps.GetPSText(RA_WTDECREASE, SCOT_LANGUAGE_PRIMARY));
	SAWLDBScannerDisable();			// Disable the scanner
	g_lPreLRW = g_lLRW; //tar 197339
	nTimeOut = 1;
	trace(L7, _T("+ra.OnTransactionState, Itemization State"));
	ra.OnSetTransactionState(RA_STATE_PICKING_UP_ITEMS);
	trace(L7, _T("-ra.OnTransactionState"));
	bScreenTimerExpired = false;
    dm.saveTriColorLightStatus(); //TAR209295
    return STATE_NULL;
}

//////////////////////////////////////
void SMPickingUpItemsBase::UnInitialize(void)
{
	ra.RequestEnableAssistMode(true); //Tar 238294

    KillTimer(hw,RAP_ESCALATION);       // RFC 202656 ensure that the RAP escalation timer has stopped
	nCountForPeriodicRemovingItemsAudio = 0;
    DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 
	bQuietNeedSecApproval = false; //aiqin

	// Tell the security sub-system that we are leaving picking up item for the 
	// Auto Advance logic
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, csCurContext );

    //TAR 234657 041703
	//ClearSecurityViolationLight();
}

///////////////////////////////////////
SMStateBase  *SMPickingUpItemsBase::PSButton1(void) // large item
{
	//+TAR 397952
	if(co.m_bOperationsUseFingerPrintDevice)
	{
		if(fInValidOperatorPassword)
		{
			return STATE(SecOperatorPWState)::PSButton1();
		}
	}
	

	
	if (inputMode == PROMPTING_FOR_PASSWORD || fInValidOperatorPassword)
	{
		return STATE(SecOperatorPWState)::PSButton1();
	}
	//-TAR 397952
    KillTimer(hw,RAP_ESCALATION);       // RFC 202656 ensure that the RAP escalation timer has stopped
	//nItemRemovedBtnPresses++;		// should increment on the other side of the if statement LPM021102
	DMSaySecurity( SEC_NONE, _T("blank.wav") );			//to interrupt the wave file if it's playing 153796
	if((nCountForPeriodicRemovingItemsAudio >= 1) || (m_View == DATANEEDED_WALKAWAYTIMEREXPIRED))
	{

        //TAR 234657 041703
        //SAEnterSecurityCondition(); 
        SAClearSecurityCondition();

	}
  //  +TAR378215
  /*
	CKeyValueParmList parmList;
  //+dp185016 tar 285550
  CKeyValue<long>::Put(parmList, KEY_OSE_EXCEPTION_ID, secEventParms.m_lExceptionId);
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_AUTHORIZATION, KEY_EXCEPTION_AUTHORIZATION_LANE);
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_OPERATION, KEY_EXCEPTION_OPERATION_CLEAR);
  _bstr_t bstrParmList2((const _TCHAR *)parmList);
  SecMgr.OverrideException(lSecMgrCookie, bstrParmList2);
  parmList.Clear();
  //-dp185016
  */
    OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR, KEY_EXCEPTION_CATEGORY_IMMEDIATE_WT_CHANGES);
	CKeyValueParmList parmList;
	
 // -TAR378215
 
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_ITEM_REMOVED);
	_bstr_t bstrParmList((const _TCHAR *)parmList);
	BSTR bstrResult;
	bstrResult = NULL;
	SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
	SysFreeString(bstrResult);
	return createAnchorState();

}

///////////////////////////////////////
VOID CALLBACK SMPickingUpItemsBase::EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	//dm.saveTriColorLightStatus();
	mp.sCurr->Escalation2RAP();
}

void SMPickingUpItemsBase::Escalation2RAP()
{   // RJF 5/28/02 RFC 202656 - separate timer for escalation to RAP
    KillTimer(hw,RAP_ESCALATION);       // ensure that the RAP escalation timer has stopped
	dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
             DM_TRICOLORLIGHT_BLINK_1HZ,
             DM_TRICOLORLIGHT_NORMALREQUEST);

	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("PickingUpItemsTimerExpires"));
	m_View = DATANEEDED_WALKAWAYTIMEREXPIRED;
	CString csSecInfo;
	if (dm.fStateTakeawayBelt)
		csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_PICKINGUPITEMTIMEREXPIREDINFO, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(MSG_ATTENTION_ALERT, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_TIMEREXPIRED, SCOT_LANGUAGE_PRIMARY));
	else
    	csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_PICKINGUPITEMTIMEREXPIREDINFO, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTDECREASE, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_TIMEREXPIRED, SCOT_LANGUAGE_PRIMARY));
	ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);

	ra.RequestEnableAssistMode(true); //Tar 238294
	//BZ 06/06/02 RFC 203538 Begin: After escalation to RAP, show a Store Login button on the screen
	if (co.fOperationsAllowStoreLoginBtnOnRemoveItemScreen) 
			STATE(SecOperatorPWState)::Initialize();		
	//BZ 06/06/02 RFC 203538 End
}

///////////////////////////////////////
SMStateBase *SMPickingUpItemsBase::TimedOut(void) // timeout
{
	if (nCountForPeriodicRemovingItemsAudio  >= 1)	// this will occur the 3rd time thru the nth time until cleared or press btn 1
	{
		//BZ 06/07/02 TAR 203537 begin
		if(co.fOperationsAllowItemRemovedBtn)
			DMSaySecurity( SEC_NONE, ps.GetSoundName(SEC_PICKINGUPITEMS));
		else
			DMSaySecurity(SEC_NONE, ps.GetSoundName(SEC_REMOVEDITEM));
		//TAR 203537 end
		nTimeOut = co.getTimeOutPeriodicRemovingItemsAudio();
		return STATE_NULL;                // stay in Pickingupitems state
	}
	
	//T.D 05/14/02 204132 
	else if (bScreenTimerExpired)		// this only occurs the second timeout, after the screen is shown
	{
		trace(L7, _T("+ra.OnTransactionState, Itemization State"));
		ra.OnSetTransactionState(RA_STATE_PICKING_UP_ITEMS);
		trace(L7, _T("-ra.OnTransactionState"));

		// don't play the wave file until the 'initial' timer has expired
		//BZ 06/07/02 TAR 203537 begin
		if(co.fOperationsAllowItemRemovedBtn)
			DMSaySecurity( SEC_NONE, ps.GetSoundName(SEC_PICKINGUPITEMS));
		else
			DMSaySecurity(SEC_NONE, ps.GetSoundName(SEC_REMOVEDITEM));
		//TAR 203537 end

		nCountForPeriodicRemovingItemsAudio = 1;		// set to 1 so the 1st else will be hit after the next timeout
		
		nTimeOut = co.getTimeOutPeriodicRemovingItemsAudio();
		return STATE_NULL;                // stay in Picking Up Items state
	}
	else
	{
        // NewUI
		ps.SetCMFrame(_T("PickingUpItems"));
		if (dm.fStateTakeawayBelt)
			ps.SetCMLeadthruText(MSG_ATTENTION_ALERT);
		else
			ps.SetCMLeadthruText(MSG_BAGGING_ALERT);
		//BZ 06/07/02 TAR 203537 begin
		COleVariant v;
		if(co.fOperationsAllowItemRemovedBtn)
		{
			v=VARIANT_TRUE;
			ps.SetButtonText(_T("CMButton1Med"), BTT_ITEM_REMOVED);
		}
		else
		{
			v=VARIANT_FALSE;
		}
		m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, v);
        // End NewUI
		//TAR 203537 end

		// TAR 237694 disable Store Log-in Button when the first time going in Picking up item screen.
		v = VARIANT_FALSE;
	    m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), csCurContext, UI::PROPERTYVISIBLE, v);
		// end TAR 237694


		nTimeOut = co.getTimeOutInitialRemovingItemsAudio();
		if(nTimeOut == 0)
		{
			trace(L6, _T("InitialRemovingItemsAudio is immediate"));
			//BZ 06/07/02 TAR 203537 begin
			if(co.fOperationsAllowItemRemovedBtn)
				DMSaySecurity( SEC_NONE, ps.GetSoundName(SEC_PICKINGUPITEMS));
			else
				DMSaySecurity(SEC_NONE, ps.GetSoundName(SEC_REMOVEDITEM));
			//TAR 203537 end
			nCountForPeriodicRemovingItemsAudio = 1;// set to 1 so the 1st else will be hit after the next timeout
			nTimeOut = co.getTimeOutPeriodicRemovingItemsAudio();
		}

		bScreenTimerExpired= true;
		//Tar 204989 AH 052102 - need to show the current frame
		ps.ShowCMTBText(csTBMessage2Scot);
		if (dm.fStateTakeawayBelt)
		{
			//BZ 06/07/02 TAR 203537 begin
			if(co.fOperationsAllowItemRemovedBtn)
				ps.Message12(MSG_RETURNITEMS_TO_BELT,true);
			else
				ps.Message12(MSG_OVERALL_WEIGHT_DECREASE, true);
			//TAR 203537 end
		}
		else
		{
			//BZ 06/07/02 TAR 203537 begin
			if(co.fOperationsAllowItemRemovedBtn)
				ps.Message12(MSG_RETURNITEMS_TO_BAGGING,true);
			else
				ps.Message12(MSG_OVERALL_WEIGHT_DECREASE, true);
			//TAR 203537 end
		}
		//end of Tar 204989
		// RJF 5/28/02 RFC 202656 - separate timer for escalation to RAP
		if(!bIsAutoAdvanceInPickingUpItem)
		{
			nEscalationRAPTimeOut = co.getTimeOutRemovingItemsEscalation();  // time is milliseconds
			if(nEscalationRAPTimeOut == 0)   // if 0, immediate escalation 
			{
				SAEnterSecurityCondition();
				cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("PickingUpItemsTimerExpires"));
				m_View = DATANEEDED_WALKAWAYTIMEREXPIRED;
				CString csSecInfo;
				if (dm.fStateTakeawayBelt)
					csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_PICKINGUPITEMTIMEREXPIREDINFO, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(MSG_ATTENTION_ALERT, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_TIMEREXPIRED, SCOT_LANGUAGE_PRIMARY));
				else
    				csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_PICKINGUPITEMTIMEREXPIREDINFO, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTDECREASE, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_TIMEREXPIRED, SCOT_LANGUAGE_PRIMARY));
				if(co.fOperationsAllowItemRemovedBtn)
					DMSaySecurity( SEC_NONE, ps.GetSoundName(SEC_PICKINGUPITEMS));
				else
					DMSaySecurity(SEC_NONE, ps.GetSoundName(SEC_REMOVEDITEM));

				ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
				
				ra.RequestEnableAssistMode(true);  //Tar 238294
				//BZ 06/02/02 RFC 203538 Begin: After escalation to RAP, show a Store Login button on the screen
				if (co.fOperationsAllowStoreLoginBtnOnRemoveItemScreen) 
					STATE(SecOperatorPWState)::Initialize();		
				//RFC 203538 End
			}
			else
				SetTimer(hw, RAP_ESCALATION, nEscalationRAPTimeOut, &EscalationTimedOut);
		}
	    ps.ShowCMFrame(false);
		return STATE_NULL;  
	}
}
	//T.D 05/14/02 204132 
//////////////////////////////////////////////////////////////////////////
SMStateBase *SMPickingUpItemsBase::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
	trace(L2, _T("+SMPickingUpItemsBase::OnRAPClearSecurityBtn"));
	DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 
	if(m_View == DATANEEDED_WALKAWAYTIMEREXPIRED)
	{
	    nCountForPeriodicRemovingItemsAudio = 0;
		CKeyValueParmList parmList;
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, _T("item-expired"));
		_bstr_t bstrParmList((const _TCHAR *)parmList);
		BSTR bstrResult;
		bstrResult = NULL;
		SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
		SysFreeString(bstrResult);
		// +TAR 310988
		if (ImmediateInterventionStack.GetCount() >= 1)
		{
			OverrideSecurityException(lpszAuthorizationType, KEY_EXCEPTION_OPERATION_CLEAR);
		}
		// -TAR 310988
	}
	else
	{
		OverrideSecurityException(lpszAuthorizationType, KEY_EXCEPTION_OPERATION_CLEAR);
	}

	// resets lane light, flushes belt, sets re-enable scanner flag, and tells RAP no data is needed
	SAClearSecurityCondition();	 
	g_bMatchedWtWaitingOnEAS = false;

	//218558 set scanner flag to re-enabled only in state that isn't handling EAS automatically
	trace(L2, _T("bEASReEnableScanner is set to true."));
	bEASReEnableScanner = true;

	trace(L2, _T("-SMPickingUpItemsBase::OnRAPClearSecurityBtn"));
	return setAndCreateAnchorState(BES_SCANANDBAG);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMPickingUpItemsBase::OnWtIncreaseNotAllowed()	 
{
	// if not from Bag&EAS, then the wt is UnExpected and unallowed
	CString csInstruction;
	csInstruction.Format(ps.GetPSText(RA_WEIGHTINCREASEINFO, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTINCREASE, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits, _T(""));
	ra.OnNormalItem(csInstruction);
	CREATE_AND_DISPENSE(SecUnExpectedIncrease)(BES_SCANANDBAG, BES_SCANANDBAG); // TAR 394784

}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMPickingUpItemsBase::OnWtDecrease()
{

	// SR682-Refresh Weight Removed in the event of a weight change.
	if(co.fOperationsShowWeightInfoOnLane == true)
		ps.ShowCMWeightNumber(g_csExpectedWeight, g_csDeltaWeight, lItems);

	//tar 197339
	if ((abs(g_lPreLRW) >= abs(g_lLRW)) || (g_lPreLRW >= g_lLRW) ) //216800
	{
		g_lPreLRW = g_lLRW;
		return STATE_NULL;
	}
	else
		return OnWtIncreaseNotAllowed();
}
//BZ 06/06/02 RFC 203538 Begin
///////////////////////////////////////
SMStateBase  *SMPickingUpItemsBase::PSButtonGoBack(void) // store login
{

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
						ps.SetLanguage(m_customerLanguage);  //Set back to customerLanuguage for dual language
					showScreen(false);
					return STATE_NULL;  
				}
		}

    DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 
	promptForOperator();

    return STATE_NULL;	
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMPickingUpItemsBase::stateAfterPasswordVerify()
{
	trace(L2, _T("+SMPickingUpItemsBase::stateAfterPasswordVerify"));
	trace(L2, _T("-SMPickingUpItemsBase::stateAfterPasswordVerify"));
	//do same thing that OnRAPClearSecurityBtn() does
	return OnRAPClearSecurityBtn(_T("Lane"));
}

/////////////////////////////////////////////////////////////
void SMPickingUpItemsBase::showScreen(bool passwordVerified)
{	
	if (passwordVerified)
		return;
	

		if (inputMode == PROMPTING_FOR_OPERATOR)
		{
            // NewUI
			STATE(SecOperatorPWState)::ShowEnterID();   //SR93 Biometrics support			
			ps.ShowNumericKeypad();
			ps.SetCMLeadthruText( TXT_ENTER_ID );
			ps.AllowInput(MAX_DIGITS_OPERATOR,false,false);
			ps.ShowCMFrame(false);
            // End NewUI
			return;
		}
		else
		{
			// TAR 200470 - this problem was discovered as a side effect of the show being slow
            // NewUI
			ps.SetCMFrame(_T("PickingUpItems"));
			// the wave and background changes depending on if Belt is installed or not
            if (dm.fStateTakeawayBelt)
				ps.SetCMBackground(BMP_SECVIOLATIONRETURNINGITEMS);
			else
				ps.SetCMBackground(BMP_SECVIOLATIONRETURNINGITEMS);			
            // Show the Store Login button 
			// TAR 237964 Enable Store Log-in button
		    m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
			// end TAR 237964
            // End NewUI
		}

	
    if (dm.fStateTakeawayBelt)
    {
    	ps.SetCMLeadthruText(LTT_BELT_ATTENTION);
        ps.Message12(MSG_BELT_ITEM_REMOVED, true);
    }
    else
    {
    	ps.SetCMLeadthruText(MSG_BAGGING_ALERT);
		//BZ 06/07/02 TAR 203537 begin
		if(co.fOperationsAllowItemRemovedBtn)
			ps.Message12(MSG_RETURNITEMS_TO_BAGGING,true);
		else
			ps.Message12(MSG_OVERALL_WEIGHT_DECREASE, true);
		//TAR 203537 end 
    }

	//BZ 06/07/02 TAR 203537 begin
	COleVariant v;
	if(co.fOperationsAllowItemRemovedBtn)
	{
		v=VARIANT_TRUE;
		ps.SetButtonText(_T("CMButton1Med"), BTT_ITEM_REMOVED);	
	}
	else
	{
		v=VARIANT_FALSE;
	}
	m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, v);
	//TAR 203537 end
	ps.ShowCMTBText(csTBMessage2Scot);	//thp
    // End NewUI

    DisplayAuxScreen(AUX_SCREEN_DEFAULT, true); //tar 193987
	ps.ShowCMFrame(false);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMPickingUpItemsBase::OnBackToLGW(void)
{
	trace(L2, _T("+SMPickingUpItemsBase::OnBackToLGW"));

	DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 

	// TAB re-enable scanner and clear Wait For Approval State
	TABReEnableScannerAndClearWaitForApprovalState();

	//219883 should not use g_bMatchedWtWaitingOnEAS like this LPM121002
	// see if need to wait on EAS because returning to the bag and EAS screen 
	// so if the EAS event has not been received, 
	//if (!bEASReEnableScanner)				// this is set to true once the EAS event is received
	//	g_bMatchedWtWaitingOnEAS = true;	// set the waiting on EAS back to true when leaving this state.

	SAClearSecurityCondition();

	//218558 set scanner flag to re-enabled only in state that isn't handling EAS automatically
	trace(L2, _T("bEASReEnableScanner is set to true."));
	bEASReEnableScanner = true;
	trace(L2, _T("-SMPickingUpItemsBase::OnBackToLGW"));

	return setAndCreateAnchorState(BES_SCANANDBAG);	// go back to state after approval - ScanAndBag
}
//BZ 06/06/02 RFC 203538 End

