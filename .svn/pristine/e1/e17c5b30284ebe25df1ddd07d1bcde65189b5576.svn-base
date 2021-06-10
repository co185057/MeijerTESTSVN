//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCouponItemsBase.CPP
//
// TITLE: Class implementation for store mode Coupon Items state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSmCouponItems.h"        // MGV this state
#include "SMSmAuthorization.h"      // MGV auth state
#include "SMSmAssistMenu.h"      // MGV auth state

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmCouponItemsBase")

IMPLEMENT_DYNCREATE(SMSmCouponItemsBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmCouponItems)


DEFINE_TIMESTAMP

//////////////////////////////////
SMSmCouponItemsBase::SMSmCouponItemsBase()
{
  IMPLEMENT_TIMESTAMP
}

/////////////////////////////////////////
SMStateBase  *SMSmCouponItemsBase::Initialize(void)
{
  CString csNumCoupons;
  bStoreModeApprovals = true;
	
  csNumCoupons.Format(_T("%s (%d/%d)"), 
                      ps.GetPSText(LTT_COUPONITEMS, SCOT_LANGUAGE_PRIMARY),
                      lUnapprovedCoupons, 
                      lCoupons+lDepositedCoupons);
  // NewUI
  ps.SetFrame(_T("SmCouponItems"));
  ps.SetLeadthruText(csNumCoupons, _T(""));
  ps.Echo(PS_BLANK);
  ps.SetButtonText(_T("SMButton1"),BTT_APPCOUPS);
    // End NewUI
  ps.ShowSMTotal(true);
    ps.ShowSMTBText(csTBMessage2Scot);	//thp

	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmCouponItems") );	// RFC 330050
#if 0
  ps.SMShow(
    BMP_SMDEFAULT,                  // background
    TTL_STOREAUTH,                  // title 1
    TTL_COUPONITEMS,                // title 2
    LTT_COUPONITEMS,                // leadthrutext 1
    PS_BLANK,                       // leadthrutext 2
    true,                           // show total
    PS_WORK1,                       // button 1
    PS_NOTEXT,                      // button 2
    PS_NOTEXT,                      // button 3
    PS_NOTEXT,                      // button 4
    PS_NOTEXT,                      // button 5
    PS_NOTEXT,                      // button 6
    PS_NOTEXT,										  // button 7
    BTT_GOBACK);                    // button 8
#endif
  ps.SMShowCouponReceipt(true);
  ps.ShowFrame();	
  //+dp185016 tar 286976
  dm.turnOffTriColorLight();
  dm.setTriColorLight(DM_TRICOLORLIGHT_GREEN,
                         DM_TRICOLORLIGHT_ON,
                         DM_TRICOLORLIGHT_NORMALREQUEST);
  //-dp185016
  return STATE_NULL;
}


////////////////////////////////////////
SMStateBase  *SMSmCouponItemsBase::PSButton1(void) // approve coupons
{
  lCoupons=0; //TAR 397911
  bInApprovalNeeded = false; //TAR 279092
  lUnapprovedCoupons = 0;
  lExceptionItemsRequiringApproval = 0;
  if(nCountPossibleDuplicateCoupon >= co.nOperationsDuplicateCouponMax)
  {
	  csPrevCoupon = EMPTY_STRING;
	  nCountPossibleDuplicateCoupon = 0;
  }
	// remote approval
  trace(L7, _T("+ra.OnCouponApproval"));
  ra.OnCouponApproval();
  trace(L7, _T("-ra.OnCouponApproval"));
  RETURNSTATE(SmAuthorization)
}

////////////////////////////////////////
SMStateBase  *SMSmCouponItemsBase::PSButton8(void) // cancel
{
  bForceApprovalBeforeSuspend = false; //TAR 364905
  bInApprovalNeeded = false; //TAR 279092  
  trace(L7, _T("Cashier selected Store Authorization button instead of approving coupon(s)"));
  bStoreModeGoBack = true;
  //+dp185016 tar 286976
  trace(L6, _T("Restore tri-Color light.") );
  dm.restoreTriColorLight();
  //-dp185016
  //TAR 290433
  if (co.IsInAssistMenus())
  {
	  RETURNSTATE(SmAssistMenu)
  }
  RETURNSTATE(SmAuthorization)
}
////////////////////////////////////////
// NewUI
SMStateBase  *SMSmCouponItemsBase::PSReceiptUp(void)
{
  ps.ScrollUp(_T("SMReceiptCoupon"));
  return STATE_NULL;

}
SMStateBase  *SMSmCouponItemsBase::PSReceiptDown(void)
{
  ps.ScrollDown(_T("SMReceiptCoupon"));
  return STATE_NULL;

}
// End NewUI
//SMStateBase  *SMSmCouponItemsBase::PSCancelKey(void) // cancel
//{
//  DMSayPhrase("KeyPadValid");
//  return PSButton8();
//} ----> removed per tar 121487
// + RFC 330050
void SMSmCouponItemsBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmCouponItems") );	
}
// - RFC 330050
