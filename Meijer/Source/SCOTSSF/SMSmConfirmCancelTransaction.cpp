//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmConfirmCancelTransaction.CPP
//
// TITLE: Class implementation for store mode ConfirmCancelTransaction state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"
#include "SMSmConfirmCancelTransaction.h"

#define COMP_ID ID_SM               // base state component
#define T_ID "SmConfirmCancelTransaction"

IMPLEMENT_DYNCREATE(SMSmConfirmCancelTransaction, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP
//////////////////////////////////////
SMSmConfirmCancelTransaction::SMSmConfirmCancelTransaction()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////////
SMStateBase  *SMSmConfirmCancelTransaction::Initialize(void)
{
	ps.SetFrame(STORE_MODE_FRAME);
	ps.SetBackground(BMP_SMDEFAULT);
	ps.SetTitle(RA_CANCELTRANSACTION, PS_BLANK);
	ps.SetLeadthruText(LTT_CANCELCONFIRM, PS_BLANK);
	
	// Prompt is blank initially.
	ps.Prompt(PS_BLANK);

	ps.SetButtonText(_T("SMButton1"), SSF_BTT_CONFIRMCANCEL);
	ps.SetButtonText(_T("SMButton2"), SSF_BTT_ABORTCANCEL);
	ps.SetButtonText(_T("SMButton3"), PS_NOTEXT);
	ps.SetButtonText(_T("SMButton4"), PS_NOTEXT);
	ps.SetButtonText(_T("SMButton5"), PS_NOTEXT);
	ps.SetButtonText(_T("SMButton6"), PS_NOTEXT);
	ps.SetButtonText(_T("SMButton7"), PS_NOTEXT);
	ps.SetButtonText(_T("SMButton8"), PS_NOTEXT);

	ps.ShowFrame();
	ps.ShowSMTotal(true);
	ps.ShowSMTBText(csTBMessage2Scot);	//thp
	ps.SMShowItemReceipt(true);

	return STATE_NULL;
}

/////////////////////////////////////////
SMStateBase *SMSmConfirmCancelTransaction::PSButton1(void)
{

  //this code copied from suspend-assume it applies for cancel
  if( lCoupons && co.fOperationsVoidCouponBeforeSuspend)   
		VoidAllCoupons();   // Void all coupons before cancel transaction

  return TBCancelTransaction(true);
}

/////////////////////////////////////////
SMStateBase *SMSmConfirmCancelTransaction::PSButton2(void)
{
	fCancelSuspend = true;
	return createAnchorState(); 
}
