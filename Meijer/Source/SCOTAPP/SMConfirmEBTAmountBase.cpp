//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmEBTAmountBase.cpp
//
// TITLE: Class impliementation for EBT Confirmation
//
//
// AUTHOR:    Michael Prather
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMConfirmEBTAmount.h"     // MGV this state
#include "SMTakeCard.h"             // MGV take Card state
#include "SMSwipeCard.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("ConfirmEBTAmountBase")

IMPLEMENT_DYNCREATE(SMConfirmEBTAmountBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(ConfirmEBTAmount)

DEFINE_TIMESTAMP

////////////////////////////////////////
SMConfirmEBTAmountBase::SMConfirmEBTAmountBase(const TBTENDERTYPE nTyp,const long lEBT, const long lCash)
: nTndrType(nTyp),
  lEBTTotal(lEBT),
  lCashBack(lCash)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMConfirmEBTAmountBase::SMConfirmEBTAmountBase()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMConfirmEBTAmountBase::Deliver(const TBTENDERTYPE nTyp,const long lEBT, const long lCash)
{
  nTndrType = nTyp;
  lEBTTotal = lEBT;
  lCashBack = lCash;

  return this;
}

bool SMConfirmEBTAmountBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be three parameters, both dwords
    if (dlParameterList.GetDescriptorCount() == 3)
    {
        trace(L6, _T("+SMConfirmEBTAmountBase::DeliverDescriptorList() - Parameter Count of 3"));
        Deliver((TBTENDERTYPE)DLParamDWORD(dlParameterList),    // nTyp
                DLParamDWORD(dlParameterList),                  // lEBT
                DLParamDWORD(dlParameterList));                 // lCash
        return true;
    }
    else
    {
        trace(L2, _T("+SMConfirmEBTAmountBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}
/////////////////////////////////////////////
SMStateBase  *SMConfirmEBTAmountBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ConfirmEBTAmount") );	// RFC 330050
  // CANNOT enter Remote Assist Mode at this state
  //ra.OnSetTransactionState(RA_STATE_ASSISTNOTOK);
    ra.RequestEnableAssistMode(false);   //TAR 213249

  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_CONFIRMEBTCBAMT, SCOT_LANGUAGE_PRIMARY));
  DMSayPhrase(CONFIRMAMOUNT);
  PSTEXT ltt = LTT_CONFIRMEBTCBAMT;
  PSTEXT msg = MSG_CONFIRMEBTCBAMT;
  SCOT_AUX_SCREEN eScreen = AUX_SCREEN_CONFIRM_EBT_AMT;

  if (nTndrType==TB_TENDER_FOODSTAMPS || nTndrType==TB_TENDER_PIP_FOODSTAMPS)
  {
    ltt = LTT_CONFIRMEBTFSAMT;
    msg = MSG_CONFIRMEBTFSAMT;
    eScreen = AUX_SCREEN_CONFIRM_FS_AMT;
  }

  ps.SetCMFrame(_T("ConfirmEBTAmount"));
  ps.SetCMLeadthruText(ltt);
  ps.CMButtonFlash(_T("CMButton1Med"),true);   
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);

  //SSCOP-376
  SetTenderStarted(false);
  SMStateBase *tempSMReturn = TBGetTotalDetails();
  delete getRealStatePointer(tempSMReturn);
  //SSCOP-376
  
  long lRemaining;
	// calculate the remaining balance
	//TAR#124396 Wasn't taking into account that part of balance might already be
	//paid. 
	// customer has already paid part of balance off or used some coupons
	if (lBalanceDue < lEBTTotal) 
	{
		lRemaining = 0;
		// TAR: 165687
		bool bTBNeedsHigherTotal = co.fTenderSubmitFSTtotalIfMorethanTrxTotal; 
		bool bTenderFoodStampsType = (nTndrType==TB_TENDER_FOODSTAMPS || nTndrType==TB_TENDER_PIP_FOODSTAMPS);
		if (!(bTBNeedsHigherTotal && bTenderFoodStampsType))
		{
			lEBTTotal = lBalanceDue; //TAR#124396 lEBTTotal gets passed in as the available EBT balance
		}
		//else
		//{
			// submit food stamp total instead of balancedue to let customer
			// get change for his/her coupons
		//}
	}
	else
	{
		lRemaining = lBalanceDue - lEBTTotal;
	}

  // for food stamps, if all that remains is the tax due
  // then since smkt will reduce the tax, we must reduce 
  // this remaining balance as well.

  CString csWork;
	
  //  csWork.Format(
  //    ps.GetPSText(msg),              //$$$ MSG_CONFIRMEBTAMT 
  //    ps.FmtDollar(lEBTTotal));
  //  ps.Message23(csWork);

    CString csRemaining;														
	if ((lRemaining==lTaxDue) || (lRemaining==0))		
	{																								
		lRemaining = 0;																
		csRemaining = ps.GetPSText(MSG_NOBALANCE);		
		// ps.Total(lEBTTotal+lCashBack);		
		ps.Total(lEBTTotal);		// return back to original TOTAL receipt should show only 
									// transaction total amount not the cash back amount
									// cash back shows in the pop up window....this is the same 
									// as the debit state
	}																								
	else																						
	{																								
		csRemaining = ps.GetPSText(MSG_BALANCEREMAINS);
    eScreen = AUX_SCREEN_CONFIRM_FS_BAL;
	}

	CString csAmt = ps.FmtDollar(lEBTTotal + lCashBack); 
	CString csRem = ps.FmtDollar(lRemaining);
	csWork.Format(ps.GetPSText(msg),csAmt,csRem);

	
  if (nTndrType==TB_TENDER_FOODSTAMPS || nTndrType==TB_TENDER_PIP_FOODSTAMPS)
	{
		csWork.Format(ps.GetPSText(msg),csAmt,csRemaining);
	}
  CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMConfirmEBTAmountBase::Initialize"));          
  ps.Message12(csWork, true);

  DisplayAuxScreen(eScreen, true, 1, csAmt);
  ps.ShowCMFrame();

  return STATE_NULL;                // stay here
}

SMStateBase  *SMConfirmEBTAmountBase::PSButton1(void)  // OK
{
  SetTenderStarted(false); //SR797
  // the BALANCE/CHANGE message will change us to either
  // STCashPayment or STTakeChange or STTakeReceipt
  return TBEnterTender(	nTndrType,
		                lEBTTotal+lCashBack); //change for tar 120604
}

SMStateBase  *SMConfirmEBTAmountBase::PSButton2(void) // No, Cancel
{
  // CAN enter Remote Assist Mode after leaving this state
  //ra.OnSetTransactionState(RA_STATE_ASSISTOK);
    ra.RequestEnableAssistMode(true);   //TAR213249

  SetTenderStarted(false); //SR797
  setAnchorState(BES_FINISH);
  CREATE_AND_DISPENSE(TakeCard)(false);
}

SMStateBase  *SMConfirmEBTAmountBase::PSButton3(void) // used to be No
{
  return STATE_NULL;
}

SMStateBase  *SMConfirmEBTAmountBase::PSButtonGoBack(void) // Cancel Payment
{
  // CAN enter Remote Assist Mode after leaving this state
  //ra.OnSetTransactionState(RA_STATE_ASSISTOK);
  
  SetTenderStarted(false); //SSCOP-373
  ra.RequestEnableAssistMode(true);   //TAR213249
  setAnchorState(BES_FINISH);
  CREATE_AND_DISPENSE(TakeCard)(false);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMConfirmEBTAmountBase::OnWtDecrease()
{
	if(!g_bIsForgiveUnexpectedDecreaseDuringTendering)
		return FinalizationWtDecreaseNotAllowed(BES_STATERETURN);
	else
		return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMConfirmEBTAmountBase::OnWtIncreaseNotAllowed()	 
{
	return FinalizationWtIncrease(BES_STATERETURN);	
}

// + RFC 330050
void SMConfirmEBTAmountBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ConfirmEBTAmount") );	
}
// - RFC 330050
