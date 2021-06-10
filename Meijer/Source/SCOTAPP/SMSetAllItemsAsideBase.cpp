//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSetAllItemsAsideBase.cpp
//
// TITLE: Class implementation for set items aside State
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"
#include "SMSetAllItemsAside.h"
#include "SMTakeCard.h"
#include "SMWaitForApproval.h"
#include "SMSecUnExpectedIncrease.h"
#include "SMVoidTransApproval.h"

DLLEXPORT extern DMProcedures dm;   // Global DM object


#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SetAllItemsAsideBase")

DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMSetAllItemsAsideBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SetAllItemsAside)


////////////////////////////////////////
SMSetAllItemsAsideBase::SMSetAllItemsAsideBase()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////////
SMStateBase  *SMSetAllItemsAsideBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SetAllItemsAside") );	// RFC 330050
  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_SETALLASIDE, SCOT_LANGUAGE_PRIMARY));
  setAnchorState(BES_FINISH);
  bIsFromVoidApproval = true;
// NewUI
  ps.SetCMFrame(_T("SetAllItemsAside"));
  if (dm.fStateTakeawayBelt)
  {
    ps.SetCMBackground(BMP_SETASIDE_BELT);
  }
  else
  {
    ps.SetCMBackground(BMP_SETASIDE);
  }
  ps.SetCMMascotImage(BMP_MASCOT_SETASIDE);
  ps.SetCMLeadthruText(LTT_SETALLASIDE);
  ps.CMButtonFlash(_T("CMButton1Med"),true);   
// NewUI

  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp

  DMSayPhrase(SETALLITEMSASIDE);
  ps.Message12(MSG_SETALLASIDE,true);
  ps.ShowCMFrame();
  return STATE_NULL;				// stay in this state
}

//////////////////////////////////////////
SMStateBase  *SMSetAllItemsAsideBase::TimedOut(void)
{
    return SMStateBase ::TimedOut();
}

///////////////////////////////////////////
SMStateBase  *SMSetAllItemsAsideBase::PSButton1(void) // OK Button
{
 //need to delay cancelling transaction until void approval
 /* if ( dm.getTriColorLightColor() == DM_TRICOLORLIGHT_RED )
  {
    // The transaction's over, just a question of what we
    // do between now and ThankYou
    setAnchorState(BES_THANKYOU);
  }

  return TBCancelTransaction(false);*/
	  //go to void approval and send msg to rap  
	  CREATE_AND_DISPENSE(VoidTransApproval)(BES_SETALLITEMSASIDE, BES_TAKERECEIPT);
}

////////////////////////////////////////////
SMStateBase  *SMSetAllItemsAsideBase::PSEnterKey(void) // Yes Button
{
  DMSayPhrase(KEYPADVALID);
  return PSButton1();
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSetAllItemsAsideBase::OnWtDecrease()
{
	return VoidingWtDecrease();
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSetAllItemsAsideBase::OnWtIncreaseNotAllowed()	 
{
	return VoidingWtIncrease(BES_SETALLITEMSASIDE, BES_VOIDTRANSAPPROVAL);
}

// + RFC 330050
void SMSetAllItemsAsideBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SetAllItemsAside") );	
}
// - RFC 330050
