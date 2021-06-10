//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMItemNotForSaleBase.cpp
//
// TITLE: Class implementation for Item Recalled State
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMItemNotForSale.h"   
#include "DMProcedures.h"
DLLEXPORT extern DMProcedures dm;   // Global DM object
#else
#include "SMItemNotForSaleBase.h"
#endif // _CPPUNIT

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("ItemNotForSaleBase")

IMPLEMENT_DYNCREATE(SMItemNotForSaleBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(ItemNotForSale)


DEFINE_TIMESTAMP

////////////////////////////////////
SMItemNotForSaleBase::SMItemNotForSaleBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////////
SMStateBase  *SMItemNotForSaleBase::Initialize(void)
{
#ifndef _CPPUNIT
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ItemNotForSale") );

  // +TAR 407200
  if (fInMultiSelectPickList)
  {
      g_bMultiPickSkipBaggingAllowed = true;
  }
  // -TAR 407200

  DMSayPhrase(RECALLEDITEM);

  // remote approval
  CString csDesc, csInstruction;
  csDesc = ps.GetPSText(RA_LANEBTT_TIMERESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY); // tar 205236
  //TAR 235519
  csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_LANEBTT_TIMERESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_RECALLED, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_LANEBTT_TIMERESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY));
  trace(L7, _T("+ra.OnRecalledItem, Message=%s"), csInstruction);
  ra.OnRecalledItem(csInstruction);
  trace(L7, _T("-ra.OnRecalledItem"));

  ps.SetCMFrame(_T("ItemNotForSale"));

#endif // _CPPUNIT
  if (g_bInRecallMode)
  {
    SetInRecalledModeReceiptMessage(LTT_UNSELLABLEITEM, MSG_RECALLED);
    //ps.CMButton(_T("ButtonHelp"),BTT_HELP,false); 
    ps.ButtonState(_T("ButtonHelp"), false, false);
  }

#ifndef _CPPUNIT
  if (co.IsImmedInterventionOn())
  {
      return PSButton1();
  }

  setAnchorState(BES_ITEMNOTFORSALE);
  
  // (*) SR898 ====================
  DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                     DM_TRICOLORLIGHT_BLINK_1HZ,
                     DM_TRICOLORLIGHT_NORMALREQUEST);

  ps.CMButtonFlash(_T("CMButton1Med"),true);   
  
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  ps.Message12(MSG_RECALLED, true);
  ps.ShowCMFrame();
#endif // _CPPUNIT
  return STATE_NULL;
}

SMStateBase  *SMItemNotForSaleBase::PSButton1(void)// ok
{
  if (fInMultiSelectPickList)
	  return setAndCreateAnchorState( BES_PRODUCEFAVORITES );
  else
  {
 
	  return setAndCreateAnchorState(BES_SCANANDBAG);

  }
}

#ifndef _CPPUNIT
SMStateBase  *SMItemNotForSaleBase::PSEnterKey(void)
{
  DMSayPhrase(KEYPADVALID);
  //Tar 122622 SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
  return PSButton1();
}

void SMItemNotForSaleBase::UnInitialize()
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ItemNotForSale") );
}

#endif // _CPPUNIT
SMStateBase *SMItemNotForSaleBase::TimedOut(void)
{
    if (g_bInRecallMode)
    {
        trace(L6, _T("SMItemNotForSaleBase::TimedOut in recall mode, pressing OK button"));
        return PSButton1();
    }
    else
        return STATE(State)::TimedOut();
}

