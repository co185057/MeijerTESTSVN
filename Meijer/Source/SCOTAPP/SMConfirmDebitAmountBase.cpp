//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmDebitAmountBase.cpp
//
// TITLE: Class implementation for Confirm Debit Amount state
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMConfirmDebitAmount.h" 
#include "SMTakeCard.h"             // Take Card state
#include "SMSwipeCard.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("ConfirmDebitAmountBase")

IMPLEMENT_DYNCREATE(SMConfirmDebitAmountBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(ConfirmDebitAmount)

DEFINE_TIMESTAMP

////////////////////////////////////////
SMConfirmDebitAmountBase::SMConfirmDebitAmountBase(const TBTENDERTYPE nTyp,const long lAmt)
: nTndrType(nTyp),
  lTenderAmt(lAmt)
{
  IMPLEMENT_TIMESTAMP
}
////////////////////////////////////////
bool SMConfirmDebitAmountBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be two parameters, both dwords
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMConfirmDebitAmountBase::DeliverDescriptorList() - Parameter Count of 2"));
        Deliver((TBTENDERTYPE)DLParamDWORD(dlParameterList),    // nTyp
                DLParamDWORD(dlParameterList));                 // lAmt
        return true;
    }
    else
    {
        trace(L2, _T("+SMConfirmDebitAmountBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}
////////////////////////////////////////
SMConfirmDebitAmountBase::SMConfirmDebitAmountBase()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMConfirmDebitAmountBase::Deliver(const TBTENDERTYPE nTyp,const long lAmt)
{
  nTndrType = nTyp;
  lTenderAmt = lAmt;

  return this;
}


/////////////////////////////////////////////
SMStateBase  *SMConfirmDebitAmountBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ConfirmDebitAmount") );	// RFC 330050
	// CANNOT enter Remote Assist Mode at this state
	//ra.OnSetTransactionState(RA_STATE_ASSISTNOTOK);
    ra.RequestEnableAssistMode(false);   //TAR 213249

	//Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(LTT_CONFIRMDEBITAMT, SCOT_LANGUAGE_PRIMARY));

	ps.SetCMFrame(_T("ConfirmDebitAmount"));
    ps.CMButtonFlash(_T("CMButton1Med"),true);   

    ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);

  CString csWork;
  CString csTenderAmt = ps.FmtDollar(lTenderAmt);


  if(nTndrType == TB_TENDER_DEBIT || nTndrType == TB_TENDER_PIP_DEBIT || nTndrType == TB_TENDER_PIP_DEBIT_CASHBACK)
	  csWork.Format(
		ps.GetPSText(MSG_CONFIRMDEBITAMT),
			csTenderAmt);
  else
	  csWork.Format(
		ps.GetPSText(MSG_CONFIRMCREDITCASHBACK),
			csTenderAmt);
	
  ps.Message12(csWork, true);

  DMSayPhrase(CONFIRMAMOUNT);

  DisplayAuxScreen(AUX_SCREEN_CONFIRM_DEBIT_AMT, true, 1, csTenderAmt);
  ps.ShowCMFrame();
  return STATE_NULL;                // stay here
}

SMStateBase  *SMConfirmDebitAmountBase::PSButton1(void)
{ 
  // Yes 
  // the BALANCE/CHANGE message will change us to either
  // STCashPayment or STTakeChange or STTakeReceipt

/*	if (co.fOperationsPartialIntegratedPINPad)   //PIP
	{
     if(co.fOperationsDebitMSR)       
	  {
		 CREATE_AND_DISPENSE(SwipeCard)(TB_TENDER_DEBIT, lTenderAmt);
	  }
     else
	    return TBEnterTender(TB_TENDER_PIP_DEBIT, lTenderAmt);
	}
	*/

  return TBEnterTender(
		nTndrType,
		lTenderAmt);
}

SMStateBase  *SMConfirmDebitAmountBase::PSEnterKey(void)
{ 
  DMSayPhrase(KEYPADVALID);
  return PSButton1();
}

SMStateBase  *SMConfirmDebitAmountBase::PSButtonGoBack(void)
{
  // CAN enter Remote Assist Mode after leaving this state
  //ra.OnSetTransactionState(RA_STATE_ASSISTOK);
  ra.RequestEnableAssistMode(true);   //TAR213249

  setAnchorState(BES_FINISH);
  CREATE_AND_DISPENSE(TakeCard)(false);
}
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMConfirmDebitAmountBase::OnWtIncreaseNotAllowed()	 
{
	return FinalizationWtIncrease(BES_STATERETURN);	
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMConfirmDebitAmountBase::OnWtDecrease()
{
	if(!g_bIsForgiveUnexpectedDecreaseDuringTendering)
		return FinalizationWtDecreaseNotAllowed(BES_STATERETURN);
	else
		return STATE_NULL;
}
// + RFC 330050
void SMConfirmDebitAmountBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ConfirmDebitAmount") );	
}
// - RFC 330050
