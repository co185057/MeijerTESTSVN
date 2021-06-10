//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	 SMSelectDebitTypeBase.CPP
//
// TITLE:	Class implementation for Select Debit Type State
//
//
// AUTHOR:   Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>				 // MS MFC always needed first
//			   // MS MFC template header

#include "Common.h"				 // MGV common includes

#include "SMState.h"				// MGV base state
#include "SMSelectDebitType.h"	  // MGV this state
#include "SMEnterPin.h"			 // MGV enter PIN state
#include "SMSelectCardType.h"	   // MGV select card type
#include "SMGetFoodStampState.h"	   //
#include "SMSelectPayment.h"  // MGV select payment amount

#define COMP_ID ID_SM			   // base state component
#define T_ID	_T("SelectDebitTypeBase")

IMPLEMENT_DYNCREATE(SMSelectDebitTypeBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SelectDebitType)

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSelectDebitTypeBase::SMSelectDebitTypeBase(const bool fCshBck)
: fCashBack(fCshBck)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSelectDebitTypeBase::SMSelectDebitTypeBase()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSelectDebitTypeBase::Deliver(const bool fCshBck)
{
  fCashBack = fCshBck;

  return this;
}

/////////////////////////////////////////
bool SMSelectDebitTypeBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    
    // There should be one parameter, a bool passed as word
    if (dlParameterList.GetDescriptorCount() == 1)
    {
        trace(L6, _T("+SMSelectDebitTypeBase::DeliverDescriptorList() - Parameter Count of 3"));
        Deliver((bool)DLParamWORD(dlParameterList));   // fEOT
        return true;
    }
    else
    {
        trace(L2, _T("+SMSelectDebitTypeBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}


//////////////////////////////////////////
SMStateBase  *SMSelectDebitTypeBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SelectDebitType") );	// RFC 330050
  ResetAllTenderValues();


  //@@@INFOPOINT
#ifndef NewUI
  ps.SetCMFrame(DEFAULT_FRAME);
  ps.SetCMButton1Text(BTT_DEBITCHECKING);
  ps.SetCMButton2Text(BTT_DEBITSAVINGS);
  ps.SetCMButton3Text(PS_NOTEXT);
  ps.SetCMButton4Text(PS_NOTEXT);
#endif
  ps.SetCMBackground(BMP_CREDITDEBIT);
  ps.SetCMMascotImage(BMP_MASCOT_CREDITDEBIT);
  ps.SetCMLeadthruText(LTT_SELECTCARD);

  ps.SetCMGobackButtonVisible(false);
  
  ps.ShowCMFrame();

  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);

  CString csWork;
  csWork.Format(
		ps.GetPSText(MSG_CHARGEAMT),
		ps.FmtDollar(lBalanceDue));

  ps.InfoMessage(csWork); 

  return STATE_NULL;
}

/////////////////////////////////////////
SMStateBase  *SMSelectDebitTypeBase::PSButton1(void)// checking account
{
  if(!co.fTenderDebitNeedsPIN)
  {
	  if(fCashBack)
	  {
		CREATE_AND_DISPENSE(SelectPayment)(TB_TENDER_DEBITCHECKING); //With cash back
	  }
	  else
	  {
		return TBEnterTender(TB_TENDER_DEBITCHECKING,lBalanceDue);
	  }
  }
  CREATE_AND_DISPENSE(EnterPin)(TB_TENDER_DEBITCHECKING,fCashBack);
}

/////////////////////////////////////////
SMStateBase  *SMSelectDebitTypeBase::PSButton2(void)// ATM/Debit Checking
{
  if(!co.fTenderDebitNeedsPIN)
  {
	  if(fCashBack)
	  {
		CREATE_AND_DISPENSE(SelectPayment)(TB_TENDER_DEBITSAVING); //With cash back
	  }
	  else
	  {
		return TBEnterTender(TB_TENDER_DEBITSAVING,lBalanceDue);
	  }
  }
  CREATE_AND_DISPENSE(EnterPin)(TB_TENDER_DEBITSAVING,fCashBack);
}

/////////////////////////////////////////
SMStateBase  *SMSelectDebitTypeBase::PSButton3(void)// Cancel
{
//  RETURNSTATE(SelectCardType);
  trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(RA_BEGINCARDTENDER, SCOT_LANGUAGE_PRIMARY));
  ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+ps.GetPSText(RA_BEGINCARDTENDER, SCOT_LANGUAGE_PRIMARY));
  trace(L7, _T("-ra.OnNormalItem"));
  setAnchorState(BES_FINISH);
  RETURNSTATE(GetFoodStampState)
}

///////////////////////////////////////////
SMStateBase  *SMSelectDebitTypeBase::PSCancelKey(void)// cancel
{
  DMSayPhrase(KEYPADVALID);
  return PSButton3();			   // usually same as button 3
}
///////////////////////////////////////////
//@@@INFOPOINT
SMStateBase  *SMSelectDebitTypeBase::PSButtonGoBack(void)// cancel
{
  return PSButton3();			   // usually same as cancelkey
}
//@@@INFOPOINT
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSelectDebitTypeBase::OnWtIncreaseNotAllowed()	 
{
	return	FinalizationWtIncrease(BES_STATERETURN);	
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSelectDebitTypeBase::OnWtDecrease()
{
	if(!g_bIsForgiveUnexpectedDecreaseDuringTendering && !(lBalanceDue < 0.01))
		return FinalizationWtDecreaseNotAllowed(BES_STATERETURN);
	else
		return STATE_NULL;
}
// + RFC 330050
void SMSelectDebitTypeBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SelectDebitType") );	
}
// - RFC 330050
