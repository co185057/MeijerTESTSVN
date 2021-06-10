//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCashPaymentBase.cpp
//
// TITLE: Class implementation for Cash Payment State
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"
#include "SMCashPayment.h"
#include "SMCashPayment.h"
#include "SMTakeChange.h"			// Coin Acceptor Modifications
#include "SMWaitForApproval.h"      // Wait for approval state

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("CashPaymentBase")

//TAR 196279 start
#define TIMES_TO_REPEAT_PAY_MESSAGE 1
//TAR 196279 end
DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMCashPaymentBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(CashPayment)


//////////////////////////////////////////
SMCashPaymentBase::SMCashPaymentBase()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMCashPaymentBase::Initialize(void)
{  
   TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("CashPayment") );	// RFC 330050
   SMStateBase *sReturnState = CkStateStackAndReturn();
   if (sReturnState != STATE_NULL)
      return sReturnState;
    fBeyondFinishAndPayState = true;
  //TAR# 195639 Money accepted while system was entering assist mode should also be checked for.
  //If cash payment is greater than amount due, system should go to Finish state upon return.
    if ((lBalanceDue - nDMCashInserted) <= 0)
       return setAndCreateAnchorState(BES_FINISH);

  //Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(LTT_CASHINSERT, SCOT_LANGUAGE_PRIMARY));
    
	//SR742+ if were in cash only mode, don't show "PayWithCard" button
	if ( co.IsDegradedModeOn() && ( dm.GetDegradedModeType() == DEGRADEDMODE_CASH_ONLY ) )
	{
		m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), _T("CashPayment"), UI::PROPERTYVISIBLE, VARIANT_FALSE); 
		DMSayPhrase(INSERTCASHPAYMENT); //TAR412065
	}
	else //SR742-
	{
		m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), _T("CashPayment"), UI::PROPERTYVISIBLE, VARIANT_TRUE);  //TAR412062
		DMSayPhrase(INSERTCASHPAYWITHCARD); // TAR 164839 before playing InsertCash.wav
	}
   //@@@INFOPOINT

	ps.SetCMFrame(_T("CashPayment"));

	ps.SetCMLeadthruText(LTT_CASHINSERT);

	// Here we directly set the text of the go back button to say cancelPayment
	// This will also set it to visible.
	ps.SetCMGobackButtonText(BTT_CANCELPAY);

  ps.Message12(MSG_CASHINSERT, true);

  ps.ShowAmountDueAmountPaid(lBalanceDue - nDMCashInserted, lTotalPaid + nDMCashInserted);
  //  ps.Payment(lTotalPaid+nDMCashInserted,lBalanceDue-nDMCashInserted,TXT_BALANCEDUE);
  // pass type of cash in dispenser ";100,500,1000,2000,5000,10000"

  DMCashAcceptorEnable(co.csTenderDenominations);	// Coin Acceptor Modifications
  DMCoinAcceptorEnable();							// Coin Acceptor Modifications
  DMAcceptorLight(DM_MISC_STATE_ONE_HZ);
  DMCoinAcceptorLight(DM_MISC_STATE_ONE_HZ);        // Tar 139796 Coin Acceptor Light
  if (short(lTotalPaid) == nDMCashInDrawer)
  {
	// it means whatever was paid towards this transaction is only cash,not partially paid
	// with other(credit, ATM or EBT payment)
	// partial cash tenders can be undone as assumed in other places in core app (ex. Scan and Bag)
	
	  /* TAR 346592 begin */
	if(TBIsAllowed(TB_FT_SUSPEND))
	{//TB allows the Suspend Button to be enabled so App can too
		trace(L7, _T("+ra.OnCanSuspendNow, CanSuspend=true"));
		ra.OnCanSuspendNow(true); 
		trace(L7, _T("-ra.OnCanSuspendNow"));
	}
	else
	{//TB disabled Suspend Button so App can not enable it
		trace(L7, _T("+ra.OnCanSuspendNow, CanSuspend=false"));
		ra.OnCanSuspendNow(false); 
		trace(L7, _T("-ra.OnCanSuspendNow"));
	}
	  
	//trace(L7, _T("+ra.OnCanSuspendNow, CanSuspend=true"));
	//ra.OnCanSuspendNow(true); 
	//trace(L7, _T("-ra.OnCanSuspendNow"));
	/* TAR 346592 end */
	
  }
  ps.ShowCMFrame();
  return STATE_NULL;
}

void SMCashPaymentBase::UnInitialize(void)
{
  if(dm.m_csCashRecyclerModel.IsEmpty())
  {
	  DMCashAcceptorDisable();	// Coin Acceptor Modifications
	  DMCoinAcceptorDisable();	// Coin Acceptor Modifications
  }
  else
  {
	  //Cash Recycler //tar 300580
	CString csDepositCashList;

	if(! m_bCashRecyclerStopDeposit)	// TAR 310659
	{
		dm.CashRecyclerStopDeposit(csDepositCashList);  //tar 300580
	}
  }
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("CashPayment") );	// RFC 330050
}

SMStateBase  *SMCashPaymentBase::PSButton1(void) // ok done cash tender
{
  if (nDMCashInserted)
  {
	trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(RA_ENDCASHTENDER_BALANCEDUE, SCOT_LANGUAGE_PRIMARY));
	ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY +
                    ps.GetPSText(RA_ENDCASHTENDER_BALANCEDUE, SCOT_LANGUAGE_PRIMARY));
	trace(L7, _T("-ra.OnNormalItem"));

    //SR742 - hiding pay with card button is not enough if were in cash only mode. So, we added a check
	//to only call TBEnterTender() if degraded mode option is off or degraded mode option is on while 
	//degraded mode type is off or cash only, otherwise not to call TBEnterTender().
	if ( !co.IsDegradedModeOn()  || 
	   (  co.IsDegradedModeOn()  && 
	   (  dm.GetDegradedModeType() == DEGRADEDMODE_OFF || dm.GetDegradedModeType() == DEGRADEDMODE_CASH_ONLY ) ) )
	{
		return TBEnterTender(TB_TENDER_CASH, nDMCashInserted);
	}
  }
  return setAndCreateAnchorState(BES_FINISH);
}

SMStateBase  *SMCashPaymentBase::PSButton3(void) 
{
	CString csDepositStr;
	if(nDMCashInserted)
	{
		// TAR 305659 Begin
		if(dm.m_csCashRecyclerModel.IsEmpty() == FALSE)
		{
			long lDeposit;

			if(m_bCashRecyclerStopDeposit)
			{
				trace(L6, _T("Cancel payment button already pressed. Ignoring subsequent button press."));
				return STATE_NULL;
			}

			m_bCashRecyclerStopDeposit = true;
			lDeposit = dm.CashRecyclerStopDeposit(csDepositStr) - m_nDMCashPaid;

			lChangeDue = lDeposit > nDMCashInserted ? lDeposit : nDMCashInserted;
		    nDMCashInserted = 0;

			trace(L6, _T("Cancel Payment pressed - nDMCashInserted: %d; lChangeDue: %d, lDeposit: %d"), 
				nDMCashInserted, lChangeDue, lDeposit);

			CREATE_AND_DISPENSE(TakeChange)(false);
		}
		// TAR 305659 End
	
		fCancelPayment = true;
		lChangeDue = nDMCashInserted;
	    nDMCashInserted = 0;
		CREATE_AND_DISPENSE(TakeChange)(false);	// Coin Acceptor Modifications  // TAR 107249
	}
	else
		return setAndCreateAnchorState(BES_FINISH);
}

//@@@INFOPOINT
SMStateBase  *SMCashPaymentBase::PSButtonGoBack(void)
{
    return PSButton3();
}
//@@@INFOPOINT


SMStateBase  *SMCashPaymentBase::DMAcceptor(void)
{

	//TAR 196279 start
	static int ipayreminder = TIMES_TO_REPEAT_PAY_MESSAGE;
	//TAR 196279 end
  if (nDMCashInserted<lBalanceDue)
  {
    // RETURNSTATE(CashPayment);
	DMCashAcceptorEnable(co.csTenderDenominations);	// Coin Acceptor Modifications
	DMCoinAcceptorEnable();							// Coin Acceptor Modifications
	ps.ShowAmountDueAmountPaid(lBalanceDue - nDMCashInserted, lTotalPaid + nDMCashInserted);
	//TAR 196279 start
	if (ipayreminder > 0)
	{
	//TAR 196279 end
    DMSayPhrase(INSERTCASHPAYWITHCARD);  // TAR 164839 before playing InsertCash.wav
	//TAR 196279 start
	ipayreminder--;
	}
	//TAR 196279 end
	return STATE_NULL;
  }
  //TAR 196279 start
	ipayreminder = TIMES_TO_REPEAT_PAY_MESSAGE;
  //TAR 196279 end
  ps.SetCMBackground(BMP_CARDPROCESSING);
  ps.Message12(LTT_CASHPAYMENT, true);
  ps.SetCMGobackButtonVisible(false);
  ps.HideButtons();
  // TK - Migrating TAR 200329 & 200889
  // If amount inserted is > balance due, show change due rather than amount due
  COleVariant v;
  v = ps.GetPSText(TXT_CHANGEDUE);
  m_pAppPSX->SetControlProperty(_T("Amount1Label"), UI::PROPERTYTEXT, v);
  ps.ShowChangeDueAmountPaid(nDMCashInserted-lBalanceDue, lTotalPaid+nDMCashInserted);
  //ps.Payment(lTotalPaid+nDMCashInserted,nDMCashInserted-lBalanceDue,TXT_CHANGEDUE);
	
  return TBEnterTender(TB_TENDER_CASH, nDMCashInserted);
}

//////////////////////////////////
SMStateBase  *SMCashPaymentBase::SAEventOccurred(const long)
{
	/******cm150000: Modified In Merge 2******/
	//Security Log button is gone, this should be gone also
	//if (isSecurityApprovalRequired())
	//{
    //CREATE_AND_DISPENSE(WaitForApproval)(false,false);
	//}
	/******cm150000: End Modified in Merge 2******/
	return STATE_NULL;
}



////////////////////////////////////////
SMStateBase  *SMCashPaymentBase::PSCancelKey(void)/// override where needed
{
  DMSayPhrase(KEYPADVALID);
  //Tar 252846 return PSButtonGoBack();               // usually same as button 1
  return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMCashPaymentBase::OnWtIncreaseNotAllowed()	 
{
	return FinalizationWtIncrease(BES_CASHPAYMENT);	

}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMCashPaymentBase::OnWtDecrease()
{
	if(!g_bIsForgiveUnexpectedDecreaseDuringTendering)
		return FinalizationWtDecreaseNotAllowed(BES_CASHPAYMENT);
	else
		return STATE_NULL;
}

SMStateBase  *SMCashPaymentBase::TimedOut(void)
{
	// m_nDMCashPaid = nDMCashInserted;  
	return SMStateBase::TimedOut();
}
