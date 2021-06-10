//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCouponTooHighBase.cpp
//
// TITLE: Class implementation for coupon too high state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMCouponTooHigh.h"      // Header file
#include "SMEnterCouponValue.h"             // Prev state

int SMCouponTooHighBase::bestSaved = BES_STATERETURN; //dp185016 tar 288377
#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("CouponTooHighBase")

IMPLEMENT_DYNCREATE(SMCouponTooHighBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(CouponTooHigh)


DEFINE_TIMESTAMP

//////////////////////////////
SMCouponTooHighBase::SMCouponTooHighBase()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase *SMCouponTooHighBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("CouponTooHigh") );

  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_CPNTOOHIGH, SCOT_LANGUAGE_PRIMARY));
  DMSayPhrase(SECURITYLOW);
  DMSayPhrase(COUPONSVALUETOOHIGH);      // TAR 158489

  if (getAnchorState() != BES_COUPONTOOHIGH)
    bestSaved = getAnchorState(); //dp185016 tar 288377

  setAnchorState(BES_COUPONTOOHIGH);     // TAR 164014

  ps.SetCMFrame(_T("CouponTooHigh"));
  if (!co.IsInAssistMenus())
      ps.CMButtonFlash(_T("CMButton1Med"), true);

  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  ps.Message12(MSG_CPNTOOHIGH, true);

  fSAInItemSale = true;  // TAR 164015
  SASendSCOTInput(COUPON_REFUSED, io.csItemCode, 0, 0, 1);	
  ps.ShowCMFrame();
  return STATE_NULL;
}

SMStateBase *SMCouponTooHighBase::PSButton1(void) // continue
{
  if (bgAlreadySubmittedWithOverride)
  {
 	// coupon was submitted to TB but TB returns TB_COUPONEXCEEDED
    bgAlreadySubmittedWithOverride = false;
	return setAndCreateAnchorState(BES_SCANANDBAG);  // allow to buy other items
  }
  else
  {
    setAnchorState(bestSaved); //dp185016 tar 288377
	RETURNSTATE(EnterCouponValue)
  }
}

SMStateBase *SMCouponTooHighBase::PSEnterKey(void)
{
  DMSayPhrase(KEYPADVALID);
  return PSButton1();
}

void SMCouponTooHighBase::UnInitialize()
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("CouponTooHigh") );
}
