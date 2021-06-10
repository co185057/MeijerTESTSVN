//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCheckBasketReminder.cpp
//
// TITLE: Class implimentation for Check Basket Reminder State
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMCheckBasketReminder.h"          // MGV this state

#include "transbrokerssf.h"
#include "SMInProgress.h"

//pos18905
#include "SMAssistedTender.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("CheckBasketReminder")

IMPLEMENT_DYNCREATE(SMCheckBasketReminder, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

bool SMCheckBasketReminder::bCheckTAB = false;

//////////////////////////////////////////
SMCheckBasketReminder::SMCheckBasketReminder()
{
	IMPLEMENT_TIMESTAMP
}

SMStateBase * SMCheckBasketReminder::Initialize(void)
{
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("CheckBasketReminder") );
	//Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(TTL_CHECKYOURCART, SCOT_LANGUAGE_PRIMARY));
	nTimeOut = co.getTimeOutCheckBasketReminder();
	DisplayAuxScreen(AUX_SCREEN_USE_TOUCHSCREEN, true);	// 227527 - change txt on Aux display to "Use touchscreen to continue". LPM021203
	
	// if TAB unit and PromptCheckBasketOnTAB=Y, display new TAB screen
	if(!bCheckTAB)
		ps.SetCMFrame(_T("CheckBasketReminderTAB"));
	else	// otherwise, display default screen
		ps.SetCMFrame(_T("CheckBasketReminder"));
	
	// TAR 228760 - Need to take into account the cash inserted
	ps.ShowAmountDueAmountPaid(lBalanceDue - nDMCashInserted, lTotalPaid + nDMCashInserted);
	// TAR 228760
	ps.ShowCMTBText(csTBMessage2Scot);	//thp
	ps.Message12(MSG_CHECKYOURCART, true);
	DMSayPhrase(CHECKBASKETREMINDER);
	
	trace(L2, _T("::Initialize - bEASReEnableScanner is set to true."));
	bEASReEnableScanner = true;
	
	// POS16782 - Disable the scanner during the CheckBasketReminder screen but it will be reenabled on the next state
	//SAWLDBScannerEnable();	// enable the scanner
	SAWLDBScannerDisable();	// disable the scanner
	// e pos16782
	
	ps.ShowCMFrame();
	return STATE_NULL;                // stay in CheckBasketReminder state
}

void SMCheckBasketReminder::UnInitialize(void)
{
    SMCheckBasketReminderBase::UnInitialize();
}

SMStateBase * SMCheckBasketReminder::OnWtIncreaseNotAllowed(void)
{
    return SMCheckBasketReminderBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMCheckBasketReminder::TimedOut(void)
{
    return SMCheckBasketReminderBase::TimedOut();
}

SMStateBase * SMCheckBasketReminder::PSButtonHelp(void)
{
    return SMCheckBasketReminderBase::PSButtonHelp();
}

//+SOTF6376
SMStateBase * SMCheckBasketReminder::PSButton2(void) // OK Button
{
    bIsInFinalization = true; 
	
    // POS18905
	if( !isAnyApprovalRequired() && GetHookProperty(_T("PropHasRebateCheck")) == _T("1") && lBalanceDue > 0)
	{
		CString sDesc;
		sDesc.Format(ps.GetPSText(RA_ASSISTED_TENDER,SCOT_LANGUAGE_PRIMARY), ps.GetPSText(SSF_BTT_REBATECHECK,SCOT_LANGUAGE_PRIMARY));
		int nOtherPaymentIndex = GetButtonIndex(TB_TENDER_ASSISTTENDER);
		
		SetTenderButton(nOtherPaymentIndex);
		nTenderType = TB_TENDER_ASSISTTENDER;
		setAnchorState(BES_FINISH);
		
		trace(L6, _T("RebateCheck Desc (%s)"), sDesc);
		CREATE_AND_DISPENSE(AssistedTender)(sDesc);
	}
	// e pos 18905
	
    CString csPOSState(_T(""));
    SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);
    //SOTF 6226
    //if we have any approvals needed we need to handle
    //them first
    if(( csPOSState.Find(_T("PAYMENTMENU_")) == -1 ) && (!isAnyApprovalRequired(false)))
    {
        trace(L6, _T("POSState at CheckBasket(%s)"), csPOSState);
        //POS139881
        if( csPOSState.Find(_T("WAITING_ON_PINPAD")) >= 0 )
        {
            return setAndCreateAnchorState(BES_FINISH); 
		}
		//e POS139881
        
        // let's have TB drive to payment before transitioning FL
        SendTBCommand(TBCMD_ENTERING_FINISH_AND_PAY, true);
        setAnchorState(BES_FINISH);
        RETURNSTATE(InProgress);
    }
    else
    {
        return setAndCreateAnchorState(BES_FINISH);  
    }
}
//-SOTF6376

SMStateBase * SMCheckBasketReminder::PSEnterKey(void)
{
    return SMCheckBasketReminderBase::PSEnterKey();
}

SMStateBase * SMCheckBasketReminder::PSButtonGoBack(void)
{
    return SMCheckBasketReminderBase::PSButtonGoBack();
}

SMStateBase * SMCheckBasketReminder::DMScanner(void)
{
    return SMCheckBasketReminderBase::DMScanner();
}

SMStateBase * SMCheckBasketReminder::OnBackToLGW(void)
{
    return SMCheckBasketReminderBase::OnBackToLGW();
}

