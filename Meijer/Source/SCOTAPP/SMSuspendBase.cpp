//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSuspendBase.cpp
//
// TITLE: Class implimentation for suspend state
//
//
// AUTHOR:    Ying Xie
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMFinish.h"               // MGV Finish state
#include "SMSuspend.h"       // MGV this state
#include "SMTakeReceipt.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SuspendBase")

IMPLEMENT_DYNCREATE(SMSuspendBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(Suspend)

DEFINE_TIMESTAMP


//////////////////////////////////////////
SMSuspendBase::SMSuspendBase()
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////
SMStateBase  *SMSuspendBase::Initialize(void)
{
  SMStateBase *sReturnState = CkStateStackAndReturn();
  if (sReturnState != STATE_NULL)
    return sReturnState;
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("Suspend") );

  // +RFC 294962
  CString csInstruction;
  csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), 
			                 _T(""), 
							 ps.GetPSText(RA_LANEBTT_SUSPEND, SCOT_LANGUAGE_PRIMARY), 
							 _T(""), 
							 _T(""), 
							 _T(""));
  trace(L7, _T("+ra.OnNormalItem, Message = %s"), csInstruction);
  ra.OnNormalItem(csInstruction);
  trace(L7, _T("-ra.OnNormalItem"));

  ra.RequestEnableAssistMode(false);
  DMSayPhrase(TOUCHOK);
  nTimeOut = co.getTimeOutExitSuspendScreenTimer();			
  // -RFC 294962

  trace(L7, _T("+ra.OnSetTransactionState, Inactive State"));
  ra.OnSetTransactionState(RA_STATE_INACTIVE);
  trace(L7, _T("-ra.OnSetTransactionState"));

  //@@@INFOPOINT
// NewUI
  ps.SetCMFrame(_T("Suspend"));
  ps.CMButtonFlash(_T("CMButton1Med"),true);   
// NewUI
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  //@@@INFOPOINT

  ps.Message12(RA_SUSPEND_ID, true);

  // (*) SR898 ====================
  DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                     DM_TRICOLORLIGHT_BLINK_1HZ,
                     DM_TRICOLORLIGHT_NORMALREQUEST);
                     
  ps.ShowCMFrame(false);

  return STATE_NULL;                // stay in Suspend state
}

//////////////////////////////////////////
void SMSuspendBase::UnInitialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("Suspend") );

  DMCouponLight(DM_MISC_STATE_OFF);
}

//////////////////////////////////////////
SMStateBase  *SMSuspendBase::PSButton1(void)// ok
{
  bSuspendFailure = false;

  if( lCoupons && co.fOperationsVoidCouponBeforeSuspend)   
		VoidAllCoupons();   // Void all coupons before suspend transaction
	
  // TAR 236567 Let TB handle refund cash
  //if(nDMCashInDrawer)   // void already paid cash
		//TBEnterTender(TB_TENDER_CASH, -1*nDMCashInDrawer);
  // End TAR 236567
  trace(L7, _T("+ra.OnRequestSuspend"));
  ra.OnRequestSuspend(ps.GetPSText(RA_SUSPENDEDATLANE, SCOT_LANGUAGE_PRIMARY));
  trace(L7, _T("-ra.OnRequestSuspend"));
  trace(L7, _T("+ra.OnReadyToSuspend"));
  ra.OnReadyToSuspend();
  trace(L7, _T("-ra.OnReadyToSuspend"));

  //+SSCOADK-1342 
  trace(L7, _T("+ra.OnSuspendApproval"));
  ra.OnSuspendApproval();
  trace(L7, _T("-ra.OnSuspendApproval"));
  //-SSCOADK-1342

  // tell TB to suspend the transaction
  return TBSuspendTransaction(TRUE);  // force suspension
}
SMStateBase *SMSuspendBase::TimedOut(void)
{
	Sleep(2000); //353054
	return PSButton1();	//RFC 294962
}
