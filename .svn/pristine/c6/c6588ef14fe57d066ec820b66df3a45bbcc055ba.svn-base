//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMDropoffCouponsBase.cpp
//
// TITLE: Class implementation for Deposit Coupons state
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMFinish.h"               // Finish state
#include "SMDropoffCoupons.h"   // This state
#include "SMTakeReceipt.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("DropoffCouponsBase")

IMPLEMENT_DYNCREATE(SMDropoffCouponsBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(DropoffCoupons)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMDropoffCouponsBase::SMDropoffCouponsBase()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMDropoffCouponsBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("DropoffCoupons") );	// RFC 330050
  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_DROPOFFCOUPONS, SCOT_LANGUAGE_PRIMARY));
  //TAR#126499  play a different wave file since this screen is now shown before tendering LPM120699
  DMSayPhrase(TAKERECEIPTCOUPONDROP);
  trace(L7, _T("+ra.OnSetTransactionState, Inactive State"));
  ra.OnSetTransactionState(RA_STATE_INACTIVE);
  trace(L7, _T("-ra.OnSetTransactionState"));

  //@@@INFOPOINT

  ps.SetCMFrame(_T("DropoffCoupons"));
  ps.CMButtonFlash(_T("CMButton1Med"),true);   

  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  //@@@INFOPOINT

  //ps.Message13(RA_DROPOFF_ID);
  
  ps.Message12(RA_DROPOFF_ID, true);

  //TAR 174904 Turned Coupon MEI OFF
  DMCouponLight(DM_MISC_STATE_OFF);
  ps.ShowCMFrame();
  return STATE_NULL;                // stay in DropoffCoupons state
}

void SMDropoffCouponsBase::UnInitialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("DropoffCoupons") );	// RFC 330050
}

SMStateBase  *SMDropoffCouponsBase::PSButton1(void)// ok
{
  UpdateDepositedCouponVars();

  //TAR126499  DropOffCoupons screen now displays before finish state (tendering)  LPM111899
  lCoupons = 0;
  return setAndCreateAnchorState(BES_FINISH);
}
