//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAuthorization.CPP
//
// TITLE: Class implementation for store mode Authorization state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"
#include "SMSmAuthorization.h"

#include "transbrokerssf.h"
#include "SMSmAuthorization.h"
#include "SMInProgress.h"
#include "SMSmRestrictedItems.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmAuthorization")

IMPLEMENT_DYNCREATE(SMSmAuthorization, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////
SMSmAuthorization::SMSmAuthorization()
{
  IMPLEMENT_TIMESTAMP
}

//+SOTF 7384
SMStateBase * SMSmAuthorization::Initialize(void)
{
	// Start SSCOI-72966 / CRD 407201
	//    The commented code resets the variables to restricted item even if the Restricted Item is not yet approved.
	//	  It causes SCO to stay on Authorization state which should show the Age Verify screen in comparison to Base Flow.
	//	  It also causes SCO to Exit Assist Mode even if there is still outstanding Restricted Item if cashier cancels the Age Verify.

	//(+) POS309588 / SSCOI-47854
	// 	if(isRestrictedItemApprovalRequired() && GetHookProperty(_T("IsDelayedAgeRestrictedProp")) == _T("1"))
	//     {
	//         trace(L6, _T("isRestrictedItemApprovalRequired is true. Clearing it to let the POS triggered an intervention to Assistmode."));
	//         lUnapprovedRestrictedItems =0;    
	//         fApprovedForRestrictedItems = true;
	//     }
	// (-) POS309588 / SSCOI-47854
	// End SSCOI-72966 / CRD 407201
	if( getAnchorState() == BES_BAGANDEAS)
	{
		trace(L0,_T("anchor state was set to [%d]. We do not need to return to this state. Return to ScanAngBag."), getAnchorState());
		setAnchorState(BES_SCANANDBAG);
	}
	//SSCOI-44595
	
	// SSCOIA-39570 - remove this condition because we don't need it anymore
	//(+) POS181195
    //if(SMState::m_bActivateHHScanner)
    //{
    //    return PSButton6();
    //}
	//(-) POS181195

    CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMSmAuthorization::Initialize"));
    SMStateBase  *pNextState = SMSmAuthorizationBase::Initialize();
	
	SetHookProperty(_T("IsInStoreMode"),_T("1"));	//CRD 232267
    
    if((m_bGoBackAfterAgeRestrictedItem) && 
        (pNextState == STATE_NULL)       &&
        (!isAnyApprovalRequired()))      //SOTF 8553
    {
        m_bGoBackAfterAgeRestrictedItem = false;
        delete getRealStatePointer( pNextState);
		
        return OnGoBackButton();
    }

	if(!_tcsicmp(co.csHWType, _T("SS90")))
		m_pAppPSX->SetConfigProperty( _T("SMButton5"), csCurContext, UI::PROPERTYSTATE, (long)UI::Disabled );
    //SOTF 8553
    m_bGoBackAfterAgeRestrictedItem = false;
	ps.ShowCMTotal(true ,false); //SSCOI-47559
    return pNextState;
}
//-SOTF 7384

void SMSmAuthorization::UnInitialize(void)
{
	if(SMState::bVoidApprovalDelayed)  //VSTS 131568
	{
		SMState::bVoidApprovalDelayed = false;
	}
    SMSmAuthorizationBase::UnInitialize();
}

SMStateBase * SMSmAuthorization::RAParse(MessageElement* me)
{
    return SMSmAuthorizationBase::RAParse(me);
}

SMStateBase * SMSmAuthorization::PSButton1(void)
{
    return SMSmAuthorizationBase::PSButton1();
}

SMStateBase * SMSmAuthorization::PSButton2(void)
{
    return SMSmAuthorizationBase::PSButton2();
}

SMStateBase * SMSmAuthorization::PSButton3(void)
{
    return SMSmAuthorizationBase::PSButton3();
}

SMStateBase * SMSmAuthorization::PSButton4(void)
{
    return SMSmAuthorizationBase::PSButton4();
}

SMStateBase * SMSmAuthorization::PSButton5(void)
{
    return SMSmAuthorizationBase::PSButton5();
}

SMStateBase * SMSmAuthorization::PSButton6(void)
{
    if (co.fOperationsForceDelayedApprovalBeforeSuspend)
	{
		bForceApprovalBeforeSuspend = true;
		if (isRestrictedItemApprovalRequired())
			RETURNSTATE(SmRestrictedItems)
			
    }
	
    return SMSmAuthorizationBase::PSButton6();
}

SMStateBase  *SMSmAuthorization::PSButton7(void)
{
    CString csData = "{MISC}";
	SendTBCommand(TBCMD_DUPLICATE_RECEIPT_PRNT, csData);
	RETURNSTATE(InProgress);
}

SMStateBase * SMSmAuthorization::PSButton8(void)
{
    bool bCameFromStoreModeApproval = bStoreModeApprovals;
	
	SMStateBase  *pNextState= SMSmAuthorizationBase::PSButton8();
	
	if(pNextState)
	{
		CString csStateName = pNextState->GetRuntimeClass()->m_lpszClassName;
		
		//if we are tendering and we have a operator approval needed for something like
		//coupon ratio, once we are finished we should not return to card processing.  We
		if((bEnterTenderState) && 
			(bStoreModeApprovals) && 
			(csStateName == _T("SMCardProcessing")))
		{
			delete getRealStatePointer( pNextState);
			return setAndCreateAnchorState(BES_FINISH);
			
			
		}
	}
	
	
	// (+) SSCOI-49249: Explicitly call ra.OnExitStoreMode so that everytime SCO return to customer mode,
	// RCM will also be exited in from Store mode
	if(fRAPIsInStoreMode)
	{
		fRAPIsInStoreMode = false;
		
		trace(L6, _T("+ra.OnExitStoreMode in SMState::ResetAllTransactionValues"));
		ra.OnExitStoreMode();
		trace(L6, _T("-ra.OnExitStoreMode"));
	}
	// (-) SSCOI-49249
	return pNextState;
}

SMStateBase * SMSmAuthorization::PSReceiptUp(void)
{
    return SMSmAuthorizationBase::PSReceiptUp();
}

SMStateBase * SMSmAuthorization::PSReceiptDown(void)
{
    return SMSmAuthorizationBase::PSReceiptDown();
}

void SMSmAuthorization::InitUI(void)
{
    SMSmAuthorizationBase::InitUI();
}

void SMSmAuthorization::InitButtons(void)
{
    SMSmAuthorizationBase::InitButtons();
	
	CString strOptionName = _T("DuplicateEFTReceipt");
    CString strDupEFTRcpt;
    SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &strDupEFTRcpt);
	if(strDupEFTRcpt == _T("Y"))
		ps.Button(_T("SMButton7"),SSF_BTT_DUPLICATERECEIPT,!fInSignedOn);
	else
		ps.Button(_T("SMButton7"),SSF_BTT_DUPLICATERECEIPT,false);
	
	trace(L6, _T("co.IsInAssistMenus()=%d fInSignedOn=%d lBalanceDue=%d nDMCashInserted=%d getAnchorState()=%d"), 
		co.IsInAssistMenus(), fInSignedOn, lBalanceDue, nDMCashInserted, getAnchorState());
	
    if (!co.IsInAssistMenus())//TAR351364
    {
		ps.Button(_T("SMButton6"), BTT_ASSISTMODE, 
			(co.fStateAssistMode  && fInSignedOn && !ps.RemoteMode() && !nDMCashInserted));
	}
}

void SMSmAuthorization::InitLeadThruText(void)
{
    SMSmAuthorizationBase::InitLeadThruText();
}

void SMSmAuthorization::InitReceiptArea(void)
{
    SMSmAuthorizationBase::InitReceiptArea();
}

void SMSmAuthorization::InitLanguage(void)
{
    SMSmAuthorizationBase::InitLanguage();
}

void SMSmAuthorization::InitDevices(void)
{
    SMSmAuthorizationBase::InitDevices();
}

SMStateBase * SMSmAuthorization::OnGoBackButton(void)
{
	//Start CRD 366323
	NotifyExitStoreMode();
	//End CRD 366323
	
    return SMSmAuthorizationBase::OnGoBackButton();
}

void SMSmAuthorization::ShowSuspendButton() // POS39475
{
    SMSmAuthorizationBase::ShowSuspendButton();
	
	CString csPOSState = GetHookProperty(_T("__POSState"));
	if ( ( (csPOSState.Find(_T("HIGHVALUE_PROMO_CHOICE")) != -1) && (GetHookProperty(_T("HighValuePromoChoice")) == _T("1")) ) ||
        ( (csPOSState.Find(_T("VENDOR_COUPON_CHOICE")) != -1) && (GetHookProperty(_T("CouponChoice")) == _T("1")) ) || 
		( fSAInTransaction && GetHookProperty(_T("PropTransMode")).Find(_T("wicmode")) != -1 ) // SSCOI-44660
		)
	{
		// High value mPerks promo choice (POS39475) or vendor coupon choice (POS33020)
		// is active, so don't allow transaction to be suspended by disabling the
		// Suspend button on the Authorization screen.
		trace(L6, _T("%s pending so disable Suspend button"), csPOSState);
		ps.Button(_T("SMButton2"), BTT_SUSPEND, false);
   }
}

void SMSmAuthorization::ShowCloseLaneButton()
{
    SMSmAuthorizationBase::ShowCloseLaneButton();
}

void SMSmAuthorization::ShowAssistModeButton()
{
    SMSmAuthorizationBase::ShowAssistModeButton();
}

void SMSmAuthorization::ShowMediaStatusButton()
{
    SMSmAuthorizationBase::ShowMediaStatusButton();
}

void SMSmAuthorization::ShowReportingButton()
{
    SMSmAuthorizationBase::ShowReportingButton();
}

void SMSmAuthorization::ShowSystemFunctionsButton()
{
    SMSmAuthorizationBase::ShowSystemFunctionsButton();
}

void SMSmAuthorization::ShowGoBackButton()
{
    SMSmAuthorizationBase::ShowGoBackButton();
}

SMStateBase * SMSmAuthorization::PSSwitchContext(BOOL bRemote, CString csNewContext)
{
    return SMSmAuthorizationBase::PSSwitchContext(bRemote, csNewContext);
}

void SMSmAuthorization::ShowSMInfo()
{
    SMSmAuthorizationBase::ShowSMInfo();
}

void SMSmAuthorization::HandleApprovalsInSM()
{
    SMSmAuthorizationBase::HandleApprovalsInSM();
}

/////////////////////////////////////////////////
SMStateBase *SMSmAuthorization::TBParse(MessageElement *me)
{ 
    return SMSmAuthorizationBase::TBParse(me);
}

SMStateBase  *SMSmAuthorization::TimedOut(void)
{
    if ( m_bIsNoSale )
    {
        long lDrawerState;
        TBGetExpectedCashDrawerState(&lDrawerState);
		
        if ( lDrawerState & 1 )
        {
            m_bIsNoSale = false;
			
            CString csRemoteConnectionName = ra.GetRemoteConnectionName();
			
            trace(L7, _T("+ra.OnSetTransactionState, Cash Drawer Failed "));
            ra.OnSetTransactionState(RA_STATE_CASHDRAWER_CLOSE);
            trace(L7, _T("-ra.OnSetTransactionState"));
			
            CString csNoSale = _T("Cash Drawer Failed! Please check the devices and try again.");
            trace(L7, _T("+ra.OnNormalItem, Message=%s"), csNoSale);
            ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csNoSale);
            trace(L7, _T("-ra.OnNormalItem")); 
			
            SendTBCommand(TBCMD_CASHDRAWER_FAILED, NULL );
			
            dm.EndRemote();
            m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), _T("operation=assist-release;"), csRemoteConnectionName );
        }
    }
    
    return SMSmAuthorizationBase::TimedOut();
}
