//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTransportItemBase.CPP
//
// TITLE: Class implementation for Transport Item state
//
// AUTHOR: Thanhhieu Pham
//
/////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMTransportItem.h"      
#include "SMSecUnExpectedIncrease.h"
#include "SMSecUnexpectedDecrease.h"
#include "SMVoidApproval.h"
#include "SMVoidTransApproval.h"      

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("TransportItem")

IMPLEMENT_DYNCREATE(SMTransportItemBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

static bool bScreenIsShown = false;
extern bool bAlreadyInSnB;
bool bGotDMTakeawayReadyForItemEvent = false; //TAR 222452

//////////////////////////////////////////
SMTransportItemBase::SMTransportItemBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////
SMStateBase  *SMTransportItemBase::Initialize(void)
{
   	if(g_bReadyForItem) //scop-2470
    	return DMTakeawayReadyForItem();
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("TransportItem") );

	SAWLDBScannerDisable();			  // tar 201359
    //Update RAP window with SCOT state
    UpdateStateName(ps.GetPSText(LTT_TRANSPORT_ITEM, SCOT_LANGUAGE_PRIMARY));

	
	//TAR228475+ CHu
	if (co.fCMRewardInfo)
	 	ps.SetCMFrame(_T("TransportItemWithReward"));
	else
    //TAR228475-
     	ps.SetCMFrame(_T("TransportItem"));

	ps.SetCMGobackButtonVisible(false);  // Disable GoBack button
	ps.ShowCMTotal(true ,false); //tar 229349
	ps.ShowCMTBText(csTBMessage2Scot);	
    //ps.Message12(MSG_TRANSPORT_ITEM, true);	//228293 this text is showing on the screen, should not show LPM021903
	// NewUI
    
	nTimeOut = co.getTimeOutTransportItemTimer();
	bDoNotResetTimeOutOnDMEvent = true; //TAR284378
	DisplayAuxScreen(AUX_SCREEN_DEFAULT, true); //tar 193987
	ps.ShowCMFrame(false);

	bGotDMTakeawayReadyForItemEvent = false;  //TAR222452 CHU 12/17/02, used in Uninitialize()

    return STATE_NULL;                // stay in SMTransportItemBase state
}

///////////////////////////////////
void SMTransportItemBase::UnInitialize(void)
{
	//TAR222452+ CHU 12/12/02
	//If TAB is ready for the next item which means the item is off the belt, reset the
	//Smart Scale to the initial zero weight of this transaction so that security 
	//will be ready to detect the next item.
	if ( bGotDMTakeawayReadyForItemEvent ) // is set in DMTakeawayReadyForItem()
	{
#ifndef NewSec
		ss.SetLastGoodWeight(g_lBeginningTransLGW);
		g_lCTW = ss.GetCurrentTotalWeight();
#endif
		trace(L6, _T("SMTransportItemBase::UnInitialize: Resetting SmartScale LGW to beginning virtual zero: VZero=%d, CTW=%d, LGW=%d"), g_lBeginningTransLGW, g_lCTW, g_lLGW);
	}
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("TransportItem") );
	//TAR222452-
	bDoNotResetTimeOutOnDMEvent = false; //TAR284378

}

////////////////////////////////////////////////////////////////////////////////
// Procedure ::DMTakeawayReadyForItem
// Handles the event that indicates the Takeaway Belt control is ready to
// receive the next item.  Note that this event can only occur if the
// Takeaway Belt is configured, so we do not check for the presence of the
// belt here.
////////////////////////////////////////////////////////////////////////////////
SMStateBase* SMTransportItemBase::DMTakeawayReadyForItem()
{
    trace(L6, _T("+DMTakeawayReadyForItem"));

	bGotDMTakeawayReadyForItemEvent = true; //TAR222452 CHU 12/17/02 used in Uninitialize()

	g_bReadyForItem = true;//TAR 363023
	// always re-enable the scanner at this point
	trace(L2, _T("::DMTakeawayReadyForItem - bEASReEnableScanner is set to true."));
    bEASReEnableScanner = true;
	// +TAR 298301
	if (ImmediateInterventionStack.GetCount() >= 1)  
	{
		SecImmediateIntervention *psecImmediateIntervention;
		psecImmediateIntervention = ImmediateInterventionStack.GetAt( 
			ImmediateInterventionStack.GetHeadPosition() );
		if (psecImmediateIntervention->csSecInterventionType == KEY_OSE_EXC_TYPE_UNEX_INCREASE )
		{
			return OnWtIncreaseNotAllowed();
		}
	}
	// -TAR 298301

	//TAR221089+ CHU 112502
	if (TABstateBeforeTransport == BES_EMPTY_STATE )
	{
		//from ScanAndBag -->Transport --> back to ScanAndBag
		return setAndCreateAnchorState(BES_SCANANDBAG);
	}
    else
	{
		BEST tempState;
		// +TAR 298096
		if (bIsFromVoidApproval) // if still need to approve void
		{
			TABstateBeforeTransport = BES_EMPTY_STATE;
			if (bCancelTransApproval) // voiding the entire transaction
			{
				CREATE_AND_DISPENSE(VoidTransApproval)(BES_STATERETURN, BES_TAKERECEIPT);
			}
			CREATE_AND_DISPENSE(VoidApproval)(BES_STATERETURN, BES_SCANANDBAG); // otherwise voiding one item
		}
		// -TAR 298096
		//Have been in UnexpectedIncrease, now we want to go back
		//we come from anystate-->UnExpectedIncrease-->Transport-->anystate
		tempState = TABstateBeforeTransport;
		//+tar 410912
		if(tempState == BES_UNKNOWNITEM)
			return createState(tempState);
		//-tar 410912
		TABstateBeforeTransport = BES_EMPTY_STATE;
		return setAndCreateAnchorState(tempState);

	}
	//TAR221089- CHU 112502
	
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMTransportItemBase::OnWtIncreaseNotAllowed()	 
{
	//TAR220160+ CHU 11/27/02
	//For Quantity items, weight is changing while items pass belt1. By current design,
	//we don't have a good way to tell the increase is valid or not. So for now, we just
	//ignore it. It should be redesigned in 3.1.
	if (io.lQuantityEntered > 1 )
	{
		// +TAR 298301
		OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR);
		return STATE_NULL;
	}
	//TAR220160-

	DMSaySecurity( SEC_NONE, _T("blank.wav") );
	// Unmatched weight on the Transport Item screen means an unexpected
    // item.  Always return here after correcting the error to move good
    // item down to bagging area.
	CREATE_AND_DISPENSE(SecUnExpectedIncrease)(BES_TRANSPORTITEM, BES_TRANSPORTITEM);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMTransportItemBase::OnWtDecrease()
{
	//removing is not processed on TAB
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMTransportItemBase::OnBackToLGW(void)
{
	trace(L6, _T("::OnBackToLGW in TransportItem state means a decrease occurred on belt1. Staying in Transport Item state."));
	
	//TAR224537 CHu 011503: We will let TAB decide when to leave Transport not Security
	//return DMTakeawayReadyForItem();	//Chu 

	if( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_SKIP_BAGGING_THRESHOLD )
		return DMTakeawayReadyForItem();
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMTransportItemBase::TimedOut(void)
{	//Tar202552+ added TimedOut function
	trace(L6, _T("::TimedOut in TransportItem state."));
	bDoNotResetTimeOutOnDMEvent = false; //TAR284378
	return DMTakeawayReadyForItem();	//Chu Tar202552-
}

///////////////////////////////////////////////////////////////////////////
// DMScanner - item scanned	
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMTransportItemBase::DMScanner(void)
{	//211050+/- added DMScanner function	LPM081602
	csItemSent = csDMLastBarCode;
	return TBItemSale(false,0,true);	
}

