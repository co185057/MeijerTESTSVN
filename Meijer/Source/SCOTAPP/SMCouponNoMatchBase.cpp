//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCouponNoMatchBase.cpp
//
// TITLE: Class implementation for coupon no match State
//
// This state accepts and handles inputs from:
// PSButton2 :  OK
// PSEnterKey:  like button 2
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// CHANGES: Start
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMCouponNoMatch.h"    

#define COMP_ID ID_SM               // Base state component
#define T_ID _T("CouponNoMatchBase")

IMPLEMENT_DYNCREATE(SMCouponNoMatchBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(CouponNoMatch)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMCouponNoMatchBase::SMCouponNoMatchBase()
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////
SMStateBase  *SMCouponNoMatchBase::Initialize(void)
{
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("CouponNoMatch") );
	
	UpdateStateName(ps.GetPSText(LTT_CPNNOITEM, SCOT_LANGUAGE_PRIMARY)); // mm185096 TAR 330414	

	DMSayPhrase(UNMATCHEDCOUPON);
	setAnchorState(BES_COUPONNOMATCH);// Tar 265390

	//@@@INFOPOINT
	ps.SetCMFrame(_T("CouponNoMatch"));
	ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	ps.ShowCMTBText(csTBMessage2Scot);	//thp
	//@@@INFOPOINT
	
	DisplayAuxScreen(AUX_SCREEN_USE_TOUCHSCREEN, true);	// 227236 - change txt on Aux display to "Use touchscreen to continue". LPM021003
	//ps.Message23(MSG_CPNNOITEM);
    ps.Message12(MSG_CPNNOITEM,true);

    if (!co.IsInAssistMenus())
        ps.CMButtonFlash(_T("CMButton1"),true);  
	ps.ShowCMFrame();
	return STATE_NULL;
}

/////////////////////////////////////////
SMStateBase  *SMCouponNoMatchBase::PSButton1(void) // OK
{
  	return setAndCreateAnchorState(nSellState);    //Tar 265390
}

// Begin TAR#116939  

/////////////////////////////////////////
SMStateBase  *SMCouponNoMatchBase::PSEnterKey(void)// ATM OK 
{
  DMSayPhrase(KEYPADVALID);
  return setAndCreateAnchorState(nSellState);     // return to last sell state
}

////////////////////////////////////////
//@@@INFOPOINT
SMStateBase  *SMCouponNoMatchBase::PSButtonGoBack(void) // cancel all
{
  return STATE_NULL;     // return to last sell state
}
//@@@INFOPOINT

//////////////////////////////////////////
SMStateBase  *SMCouponNoMatchBase::PSCancelKey(void) // ATM Cancel
{
  DMSayPhrase(KEYPADINVALID);		
  return STATE_NULL;

  //DMSayPhrase("KeyPadValid");
  //return PSButton3();
  // End TAR#116939	LPM081299
}

/////////////////////////////////////////
void SMCouponNoMatchBase::UnInitialize() 
{
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("CouponNoMatch") );
}
