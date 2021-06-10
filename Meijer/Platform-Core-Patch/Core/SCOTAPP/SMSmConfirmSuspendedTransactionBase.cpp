//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmConfirmSuspendedTransactionBase.CPP
//
// TITLE: Class implementation for store mode ConfirmSuspendedTransaction state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"
#include "SMSmConfirmSuspendedTransaction.h"
#include "SMSmAuthorizationBase.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmConfirmSuspendedTransactionBase")

IMPLEMENT_DYNCREATE(SMSmConfirmSuspendedTransactionBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmConfirmSuspendedTransaction)


DEFINE_TIMESTAMP

//////////////////////////////////////
SMSmConfirmSuspendedTransactionBase::SMSmConfirmSuspendedTransactionBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////////
SMStateBase  *SMSmConfirmSuspendedTransactionBase::Initialize(void)
{	
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmConfirmSuspendedTransaction") );	// RFC 330050
	ps.SetFrame(_T("SmConfirmSuspendedTransaction"));
	ps.SetTitle(TTL_SUSPENDCONFIRMATION, PS_BLANK);
	ps.SetLeadthruText(LTT_SUSPENDCONFIRMORCANCEL, PS_BLANK);

	ps.SetButtonText(_T("SMButton1"),BTT_CONFIRMSUSPEND);
	ps.SetButtonText(_T("SMButton2"),BTT_CANCELSUSPEND);

   	// TAR 236945 - Needed for Remote, this api will check option whether to display or not.
    ps.Weight(lDMScaleWeight);

	ps.ShowSMTotal(true);
	ps.ShowSMTBText(csTBMessage2Scot);	//thp
	ps.SMShowItemReceipt(true);
	CString csInfo;
	csInfo = ps.GetPSText(MSG_SUSPENDCONFIRMATION);
	ps.SMInfo(csInfo);
	ps.ShowFrame();
	return STATE_NULL;
}

/////////////////////////////////////////
SMStateBase *SMSmConfirmSuspendedTransactionBase::PSButton1(void)
{
	  //Tar 126382
  bSuspendFailure = false;
  SynchronizeStoreloginWithLaunchPad(_T("operation=set-signed-off")); // TAR 301706
  //Begin_Suspend  
  if( lCoupons && co.fOperationsVoidCouponBeforeSuspend)   
		VoidAllCoupons();   // Void all coupons before suspend transaction
	
  //TAR213483+ CHU 090402 
  //We don't need to call TBEnterTender to void cash payment
  //
  //if( nDMCashInDrawer )   // already paid cash
  //		TBEnterTender(TB_TENDER_CASH, -1*nDMCashInDrawer);  // void these cash payment
  //TAR213483-

  //Tar 120428
  //DMSayPhrase("TransactionSuspended");

  if (!ps.RemoteMode())
  {
     trace(L7, _T("+ra.OnRequestSuspend"));
     ra.OnRequestSuspend(ps.GetPSText(RA_SUSPENDEDATLANE));
     trace(L7, _T("-ra.OnRequestSuspend"));
     trace(L7, _T("+ra.OnReadyToSuspend"));
     ra.OnReadyToSuspend();
     trace(L7, _T("-ra.OnReadyToSuspend"));
	 //TAR 371932 & tar 371918+
	 trace(L7, _T("+ra.OnSuspendApproval"));
     ra.OnSuspendApproval();
     trace(L7, _T("-ra.OnSuspendApproval"));
	 //TAR 371932 & tar 371918-
  }
  
  //TAR286065: clear any saved state
  ClearStateStackEOT();

  return TBSuspendTransaction();
  //End_Suspend
}

/////////////////////////////////////////
SMStateBase *SMSmConfirmSuspendedTransactionBase::PSButton2(void)
{
	fCancelSuspend = true;
    //Return to current store mode menu state instead of createAnchorState()
    return STATE(StoreModeMenu)::CreateStoreModeMenu();
}
////////////////////////////////////////
// NewUI
SMStateBase  *SMSmConfirmSuspendedTransactionBase::PSReceiptUp(void)
{
  ps.ScrollUp(_T("SMReceipt"));
  COleVariant v = VARIANT_FALSE;
  m_pAppPSX->SetControlProperty(_T("SMInfoBoxField"), UI::PROPERTYVISIBLE, v);
  m_pAppPSX->SetControlProperty(_T("SMInfoBox"), UI::PROPERTYVISIBLE, v);
  return STATE_NULL;
}

SMStateBase  *SMSmConfirmSuspendedTransactionBase::PSReceiptDown(void)
{
  ps.ScrollDown(_T("SMReceipt"));
  COleVariant v = VARIANT_FALSE;
  m_pAppPSX->SetControlProperty(_T("SMInfoBoxField"), UI::PROPERTYVISIBLE, v);
  m_pAppPSX->SetControlProperty(_T("SMInfoBox"), UI::PROPERTYVISIBLE, v);
  return STATE_NULL;
}
// End NewUI
// + RFC 330050
void SMSmConfirmSuspendedTransactionBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmConfirmSuspendedTransaction") );	
}
// - RFC 330050
