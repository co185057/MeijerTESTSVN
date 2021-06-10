//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCouponNotAllowedBase.cpp
//
// AUTHOR:    Scott Barnhart
//
// 120799:	  Leila Parker - TAR#102019 all new implementation from initialize to event procedures, 
//							 for AllowCoupon in the ScotOpts file to have 3 options: 
//							 A=All modes, O=Only in assist mode (NOT ZERO), N=No modes 
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMCouponNotAllowed.h" 

#define COMP_ID ID_SM               // Base state component
#define T_ID _T("CouponNotAllowedBase")

IMPLEMENT_DYNCREATE(SMCouponNotAllowedBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(CouponNotAllowed)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMCouponNotAllowedBase::SMCouponNotAllowedBase()
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////
SMStateBase  *SMCouponNotAllowedBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("CouponNotAllowed") );	// RFC 330050
  //TAR209311 Update RAP window with SCOT state for coupons only allowed in assist LPM070902
  CString csAnchorStateName = getSCOTStateName();
  CString csDesc, csInstruction;
  
/*Moved down to co.csTenderAllowCoupons == _T("O") section TAR 233485 
  csDesc = ps.GetPSText(TXT_COUPONSONLYINASSIST,SCOT_LANGUAGE_PRIMARY);
  csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(TXT_COUPON, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, _T(""), ps.GetPSText(BTT_CPNCANT, SCOT_LANGUAGE_PRIMARY));
  trace(L7, _T("+ra.OnCallForHelp, Message=%s"), csInstruction);
  ra.OnCallForHelp(csInstruction);
  trace(L7, _T("-ra.OnCallForHelp"));
*/
  DisplayAuxScreen(AUX_SCREEN_USE_TOUCHSCREEN, true);	// 227303 - change txt on Aux display to "Use touchscreen to continue". LPM021003
  //begin TAR#102019  entire procedure change for "Allow Coupon" has 3 options now 
  setAnchorState(BES_COUPONNOTALLOWED);		//must return to system msg after call for help

  ps.SetCMFrame(_T("CouponNotAllowed"));
  if (!co.IsInAssistMenus())
      ps.CMButtonFlash(_T("CMButton1"),true);

  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp

  if (co.csTenderAllowCoupons == _T("N"))	//coupons Never allowed 
  {
	DMSayPhrase(COUPONSNOTACCEPTED);
	ps.SetCMLeadthruText(LTT_COUPONNOTACCEPTED);
    ps.Message12(MSG_COUPONNOTACCEPTED, true);
    //TAR 233485 040703
    UpdateStateName(ps.GetPSText(LTT_COUPONNOTACCEPTED, SCOT_LANGUAGE_PRIMARY));
  }
  else if (co.csTenderAllowCoupons == _T("O"))	//coupons allowed in Assist Mode ONLY (at end of purchase)
  {
	DMSayPhrase(COUPONSENDOFPURCHASE);
	ps.SetCMLeadthruText(LTT_SETASIDECOUPON);
	ps.Message12(MSG_SETCPNASIDE, true);
	dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
						DM_TRICOLORLIGHT_BLINK_1HZ,
						DM_TRICOLORLIGHT_NORMALREQUEST);
  //TAR 233485 040703
  csDesc = ps.GetPSText(TXT_COUPONSONLYINASSIST,SCOT_LANGUAGE_PRIMARY);
  csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(TXT_COUPON, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, _T(""), ps.GetPSText(BTT_CPNCANT, SCOT_LANGUAGE_PRIMARY));
  trace(L7, _T("+ra.OnCallForHelp, Message=%s"), csInstruction);
  ra.OnCallForHelp(csInstruction);
  trace(L7, _T("-ra.OnCallForHelp"));

	lCouponsInAssist++;
  }
  fSAInItemSale = true;
  SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
  ps.ShowCMFrame();
  return STATE_NULL;
}

///////////////////////////////////////////
// PSButton1 - press OK button
///////////////////////////////////////////
SMStateBase  *SMCouponNotAllowedBase::PSButton1(void) // OK
{
  fItemIsWaiting = false;
  return setAndCreateAnchorState(BES_SCANANDBAG);
} 

///////////////////////////////////////
// PSEnterKey - sell item entered
///////////////////////////////////////
SMStateBase *SMCouponNotAllowedBase::PSEnterKey(void)
{
  fItemIsWaiting = false;
  DMSayPhrase(KEYPADVALID);
  return PSButton1();
}

///////////////////////////////////////////
// PSButtonGoBack - not visible
///////////////////////////////////////////
SMStateBase  *SMCouponNotAllowedBase::PSButtonGoBack(void) // go back not visible
{
  return STATE_NULL;
} //end 102019  LPM120699

//////////////////////////////////////////
void SMCouponNotAllowedBase::UnInitialize()
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("CouponNotAllowed") );	// RFC 330050
}
