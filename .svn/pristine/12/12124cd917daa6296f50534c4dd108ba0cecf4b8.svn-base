//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTenderBalanceBase.cpp
//
// TITLE: Class implementation for TenderBalance State
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMTenderBalance.h"		// This state
#include "DMProcedures.h"
#include "SMFinish.h"
#include "SMInsertGiftCard.h"
#include "SMDepositGiftCard.h"
#include "SMCollectGiftCard.h"

DLLEXPORT extern DMProcedures dm;   // Global DM object

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("TenderBalanceBase")

IMPLEMENT_DYNCREATE(SMTenderBalanceBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMTenderBalanceBase::SMTenderBalanceBase()
: m_lGiftCardBalance(0)
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////
SMStateBase *SMTenderBalanceBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("TenderBalance") );
  // CANNOT enter Remote Assist Mode at this state
  ra.RequestEnableAssistMode(false); 

  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_YOURBALANCE, SCOT_LANGUAGE_PRIMARY));

  nTimeOut = co.getTimeOutTenderBalanceScreenTimer();

  DMSayPhrase(PAYMENTBALANCE); 
      
  ps.SetCMFrame(_T("TenderBalance"));
  ps.SetLeadthruText(LTT_YOURBALANCE);
  ps.CMButtonFlash(_T("CMButton1Med"),true);

  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);

  //Get remaining balance on the gift card
  TBRC rc = TBGetGiftCardBalance(&m_lGiftCardBalance);

  if(rc!=TB_SUCCESS)
  {
	  ProcessTBReturn(rc);   //Trace TB return code 
  }

  if (m_lGiftCardBalance < 0)
  {
	  m_lGiftCardBalance = 0;
  }

  //Display gift card balance
  CString csMsg;
  csMsg.Format(ps.GetPSText(MSG_GIFTCARDAMOUNT),ps.FmtDollar(lLastTender), ps.FmtDollar(m_lGiftCardBalance));
  ps.Message12(csMsg, true);

  ps.ShowCMFrame();

  return STATE_NULL;                // stay in TenderBalance state
}


void SMTenderBalanceBase::UnInitialize(void)
{
	//do nothing
}

///////////////////////////////////////
SMStateBase *SMTenderBalanceBase::TimedOut(void)
{
    return PSButton1();
}

/////////////////////////////////////
SMStateBase *SMTenderBalanceBase::PSButton1(void) // OK Button
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("TenderBalance") );

	trace(L6, _T("SMTenderBalanceBase::PSButton1"));
    DMMSREject();    //tar 269534
	if (m_lGiftCardBalance <= 0 && 
		co.nOperationsMethodOfCollectionForEmptyGiftCard == 1)
	{
		if (IsUsingCouponSensor())
			RETURNSTATE(InsertGiftCard)
		else
			RETURNSTATE(DepositGiftCard)
	}
	else if (m_lGiftCardBalance <= 0 &&
			 co.nOperationsMethodOfCollectionForEmptyGiftCard == 2)
	{
		RETURNSTATE(CollectGiftCard)
	}
	else
	{
		return ReturnDelayedStateForGCTender();
	}
}

////////////////////////////////////////
SMStateBase *SMTenderBalanceBase::PSEnterKey(void)
{
  DMSayPhrase(KEYPADVALID);
  return PSButton1();          
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMTenderBalanceBase::OnWtDecrease()
{
	if(!g_bIsForgiveUnexpectedDecreaseDuringTendering && !(lBalanceDue < 0.01))
		return FinalizationWtDecreaseNotAllowed(BES_TENDERBALANCE);
	else
		return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMTenderBalanceBase::OnWtIncreaseNotAllowed()	 
{
  	return FinalizationWtIncrease(BES_TENDERBALANCE);  
}
