//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTakeCardBase.cpp
//
// TITLE: Class implementation file for Take Card State
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "DMProcedures.h"           // DM procedures

#include "SMState.h"                // Base state
#include "SMTakeCard.h"         // Take Card state
#include "SMTakeChange.h"
#include "SMTakeReceipt.h"          // Take receipt state
#include "SMThankYou.h"
#include "SMSystemMessage4.h"
#include "SMSecUnExpectedIncrease.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("TakeCardBase")


DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMTakeCardBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(TakeCard)

///////////////////////////////////////
SMTakeCardBase::SMTakeCardBase(const bool fEOT, const bool fCardError)
: fTimedOutOnce(false),
  fEndOfTran(fEOT),					//TAR 272140
  bCreateAndDispenseCalled(false),	//TAR 272140
  fEjectSuccess(true),
  fCardError(fCardError)
{
  IMPLEMENT_TIMESTAMP

}

////////////////////////////////////////
SMTakeCardBase::SMTakeCardBase()
: bCreateAndDispenseCalled(false),	//TAR 272140
  fEndOfTran(false)					//TAR 272140
{
  IMPLEMENT_TIMESTAMP
  fTimedOutOnce = false;
  fCardError = false;
}

////////////////////////////////////////
SMStateBase  *SMTakeCardBase::Deliver(const bool fEOT, const bool fCardErr)
{
  fEndOfTran = fEOT;
  fTimedOutOnce = false;
  fCardError = fCardErr;
  bCreateAndDispenseCalled = true; //TAR 272140
  return this;
}
/////////////////////////////////////////
bool SMTakeCardBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    
    // There should be one or two parameters, all bools passed as word
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMTakeCardBase::DeliverDescriptorList() - Parameter Count of 2"));
        Deliver((bool)DLParamWORD(dlParameterList),    // fEOT
                (bool)DLParamWORD(dlParameterList));   // fCardErr
        return true;
    }
    else if (dlParameterList.GetDescriptorCount() == 1)
    {
        trace(L6, _T("+SMTakeCardBase::DeliverDescriptorList() - Parameter Count of 3"));
        Deliver((bool)DLParamWORD(dlParameterList));   // fEOT
        return true;
    }
    else
    {
        trace(L2, _T("+SMTakeCardBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}

////////////////////////////////////
SMStateBase  *SMTakeCardBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("TakeCard") );	// RFC 330050
	ra.RequestEnableAssistMode(false); //Tar 277637
    if(fEndOfTran || (bIsInFinalization && !bCreateAndDispenseCalled)) //tar 252987 and TAR 272140
    {
		setAnchorState(BES_TAKECARD);
        SMStateBase *sReturnState = CkStateStackAndReturn(); 
        if (sReturnState != STATE_NULL)
        {
            return sReturnState;
        }
    }
 
	//Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(LTT_TAKECARD, SCOT_LANGUAGE_PRIMARY));
	if (!co.fTenderIsMotorizedMSR)
	{
		if (cDMLastTrack)
		{
		  delete cDMLastTrack;
		  cDMLastTrack = NULL;
		  nDMLastTrackLen = 0;
		  csDMEncryptedPin = EMPTY_STRING;
		}
	    return DMCardRemoved();	// already taken
	} 
	else
	{
		// must Enable() device before calling Eject() so that the Enable() while in Eject() 
		// is ignored. Otherwise, the Enable() is processed too late and captures the card.  LPM100202 215897
		DMMSREnable();					// enable the card reader, set track, turn on MEI
	}

	// There's no data because the card isn't there, or is there but couldn't be read.
	if (!waitingForCardRemoval && !fCardError) //TAR260141
		return DMCardRemoved();	// already taken
	
	// MSR should be enabled, light on, change it to blinking
	DMMSRLight(DM_MISC_STATE_ONE_HZ);

	// entry into this method is assuming MSR is still enabled.
	nTimeOut = co.getTimeOutTakeCard();

	//418791
	long retc= DMMSREject();					  // eject card
	if (!retc)
		fEjectSuccess = true;
	else
		fEjectSuccess = false;

	if (!cDMLastTrack)				  // +TAR 313903
	{
		DMMSRDisable();	// if no card inserted, disable, no problems
	} // -TAR 313903

//	waitingForCardRemoval = true; // TAR 260141

// NewUI
	ps.SetCMFrame(_T("TakeCard"));
// NewUI

	if (lBalanceDue > 0)   //TAR#117296 BP 8/11/99 fixed to sometimes display change due
		ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	else 
		ps.ShowChangeDueAmountPaid(lChangeDue,lTotalPaid);
  
	DMSayPhrase(TAKEYOURCARD);
	ps.ShowCMFrame();

	return STATE_NULL;
}

/////////////////////////////////////////////////////
// This state allows 2 timeouts before taking action.
SMStateBase  *SMTakeCardBase::TimedOut(void) // timeout
{
  if (fTimedOutOnce)
  {
	  //Tar 418791
	  //if eject success(fEjectSuccess=true) and card not removed, need to capture the card
	  //if eject failure(fEjectSuccess=false), most likely taken already
	  if (!fEjectSuccess)  
	 {
		 return DMCardRemoved();	// already taken
	 }
	  
    // This is the second timeout in this state
    // capture the card because it has not been taken
    DMMSRCapture();
	// log event 
	ScotError(INFO, SM_CAT_INTERNAL_ERROR, SCOTAPP_CARD_PURGED);
	// report to app that card is out of the way and continue with transaction
    return DMCardRemoved();
  }
  else
  {
	  //Tar 418791
	  //if eject success(fEjectSuccess=true), need to wait to take card if card not removed
	  //if eject failure(fEjectSuccess=false), most likely taken already
	 if (!fEjectSuccess)   
	 {
		 return DMCardRemoved();
	 }

    fTimedOutOnce = true;
    DMSayPhrase(TAKECARD);
	DMSayPhrase(TAKEYOURCARD); //tar 228498
    return STATE_NULL;
  }
}

///////////////////////////////////
void SMTakeCardBase::UnInitialize(void)
{
	fEjectSuccess = true;  //418791
	if (!cDMLastTrack)
	{
		DMMSRDisable();	// if no card inserted, disable, no problems
	}
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("TakeCard") );	// RFC 330050
}

///////////////////////////////////////
SMStateBase  *SMTakeCardBase::DMCardRemoved(void)
{
  trace(L6, _T("SMTakeCardBase::DMCardRemoved()"));
  waitingForCardRemoval = false;

  if (!fEndOfTran)
  {
	  if (bIsInFinalization)    //TAR245485
		return setAndCreateAnchorState(BES_FINISH);
	  return createAnchorState();
  }
  
  if (   fInVoidTrans //if Voided Tranx
      || (fInSuspendTrans && co.fOperationsIfSuspendedDispensePartiallyPaidCash /*RFC 267820 */) )	// or suspended tranx and configured to return cash
  {	// if they have tendered something already, return it all
    if (nDMCashInserted+nDMCashTendered+nDMCashInDrawer>0)
	{
		lChangeDue = nDMCashInserted+nDMCashTendered+nDMCashInDrawer;
		nDMCashInserted = 0;
		nDMCashTendered = 0;
		nDMCashInDrawer = 0;
		CREATE_AND_DISPENSE(TakeChange)(true);  // Coin Acceptor Modifications, TAR 107249
	}
	else if (lChangeDue)
	{//TAR403962--if change hasn't been dispensed in the 1st attempt due to some system error, then dispense it since the trx is already voided. 
	    CREATE_AND_DISPENSE(TakeChange)(true);    
	}
  }
  else if (lChangeDue || !DMWasCashTaken())	//Need to give back any money since tranx was completed with a card TAR 127340 
  {
	if (!co.fTenderCashAllowed && !co.fTenderCreditCashBack &&	// if cash tendering not allowed, & cashback not allowed 
		!co.fTenderDebitCashBack && !co.fTenderEBTCBCashBack)	// from credit card, debit card or EBT then goto system msg
	{
		CString csWork;
		csWork.Format(ps.GetPSText(MSG_NEGATIVE_BALANCE), ps.FmtDollar(lChangeDue));
		CREATE_AND_DISPENSE(SystemMessage4)(csWork, PS_NOTEXT, PS_NOTEXT, 0, -1, false); //requires login
	}
	else
    {
		CREATE_AND_DISPENSE(TakeChange)(true);    // Coin Acceptor Modifications, TAR 107249
    }
  }
  // don't show Deposit Coupons screen if coupon sensor is configured
  if (lCoupons && (!fInSuspendTrans) && (!IsUsingCouponSensor()) )
  {
	if (bDropoffRequired)
	     return DepositCouponsIfNecessary();
  }
  //SR697 issue adding (co.fOperationsPrintReceiptOnCancelledTransaction)
  if (isAnySoldVoidedItems() || nTrxReceiptLength || (co.fOperationsPrintReceiptOnCancelledTransaction)
          || ((co.fOperationsTBControlsPrinter) && AreThereAnySoldVoidedItems())) //Fiscal Printer changes
    RETURNSTATE(TakeReceipt)			  // from the StoryBoard end of transaction

  RETURNSTATE(ThankYou)
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMTakeCardBase::OnWtDecrease()
{
	if(!g_bIsForgiveUnexpectedDecreaseDuringTendering && !(lBalanceDue < 0.01))
		return FinalizationWtDecreaseNotAllowed(BES_STATERETURN);
	else
		return STATE_NULL;
}
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMTakeCardBase::OnWtIncreaseNotAllowed()	 
{
	return FinalizationWtIncrease(BES_STATERETURN);	
}

