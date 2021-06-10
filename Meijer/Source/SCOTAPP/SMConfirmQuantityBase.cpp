//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmQuantityBase.cpp
//
// TITLE: Class implementation for Confirm Quantity State
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMConfirmQuantity.h"  

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("ConfirmQuantityBase")

DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMConfirmQuantityBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(ConfirmQuantity)


//////////////////////////////////////////
SMConfirmQuantityBase::SMConfirmQuantityBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////////
SMStateBase  *SMConfirmQuantityBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ConfirmQuantity") );	// RFC 330050
	//Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(LTT_QTYCONFIRM, SCOT_LANGUAGE_PRIMARY));
   //@@@INFOPOINT

	ps.SetCMFrame(_T("ConfirmQuantity"));
	ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);

	ps.ShowCMTBText(csTBMessage2Scot);	//thp
	//@@@INFOPOINT
  
  DMSayPhrase(CONFIRMQTY);

  CString csWork;
  if (co.IsInAssistMenus())
      csWork.Format(ps.GetPSText(MSG_QTYCONFIRM_AM), io.d.csDescription,io.d.lQtySold);
  else
      csWork.Format(ps.GetPSText(MSG_QTYCONFIRM), io.d.csDescription,io.d.lQtySold);
	
  // ps.Message23(csWork);
  ps.Message12(csWork, true);
  csWork.Format(_T("%d"),io.d.lQtySold);
  //ps.Echo(csWork);
  ps.ShowCMFrame();
  return STATE_NULL;                // stay in ConfirmQuantity state
}

//////////////////////////////////////////
SMStateBase  *SMConfirmQuantityBase::PSButton1(void) // Yes, Quantity is ok
{
  io.d.fQuantityConfirmed = true;
  return TBEnterItem(true, io.lUpdateFields);
}

//////////////////////////////////////////
SMStateBase  *SMConfirmQuantityBase::PSButton2(void) // no, Quantity is wrong
{
  io.lQuantityEntered = 0;            // sell item again after clearing qty
  return createAnchorState(); //SSCOP-525
  //return TBEnterItem(false, io.lUpdateFields);
}
// + RFC 330050
void SMConfirmQuantityBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ConfirmQuantity") );	
}
// - RFC 330050
