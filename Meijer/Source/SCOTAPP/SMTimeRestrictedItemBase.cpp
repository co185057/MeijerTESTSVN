//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTimeRestrictedItemBase.cpp
//
// TITLE: Class implementation for Time Restricted Item State
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMTimeRestrictedItem.h"   
#include "DMProcedures.h"
DLLEXPORT extern DMProcedures dm;   // Global DM object
#else
#include "SMTimeRestrictedItemBase.h"
#endif // _CPPUNIT

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("TimeRestrictedItemBase")

IMPLEMENT_DYNCREATE(SMTimeRestrictedItemBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(TimeRestrictedItem)


DEFINE_TIMESTAMP

////////////////////////////////////
SMTimeRestrictedItemBase::SMTimeRestrictedItemBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////////
SMStateBase  *SMTimeRestrictedItemBase::Initialize(void)
{
#ifndef _CPPUNIT
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("TimeRestrictedItem") );


  // +TAR 407200
  if (fInMultiSelectPickList)
  {
      g_bMultiPickSkipBaggingAllowed = true;
  }
  // -TAR 407200

  DMSayPhrase(RESTRICTEDITEM);

  if( lTimeRestrictedItems > 0 )
  {
      // remote approval
      CString csDesc, csInstruction;
      csDesc = ps.GetPSText(RA_THIRDLINE_TIMERESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY);
      csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_LANEBTT_TIMERESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_RESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY), 
          co.IsSecurityDisableOn()==true || co.GetSecurityDisableSubCheckOn() ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), ps.GetPSText(RA_THIRDLINE_TIMERESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY));
      trace(L7, _T("+ra.OnTimeRestrictedItem, Message=%s"), csInstruction);
      ra.OnTimeRestrictedItem(csInstruction);
      trace(L7, _T("-ra.OnTimeRestrictedItem"));

      // (*) SR898 ====================
      DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                 DM_TRICOLORLIGHT_BLINK_1HZ,
                 DM_TRICOLORLIGHT_NORMALREQUEST);
  }

  ps.SetCMFrame(_T("TimeRestrictedItem"));

#endif // _CPPUNIT
  if (g_bInRecallMode)
  {
    SetInRecalledModeReceiptMessage(RA_THIRDLINE_TIMERESTRICTEDITEM, MSG_TIMERESTRICTEDITEM);
    //ps.CMButton(_T("ButtonHelp"),BTT_HELP,false); 
    ps.ButtonState(_T("ButtonHelp"), false, false);
  }

#ifndef _CPPUNIT
  if (co.IsImmedInterventionOn())
  {
      return PSButton1();
  }
  
  setAnchorState(BES_TIMERESTRICTEDITEM);                 

// NewUI
  ps.CMButtonFlash(_T("CMButton1"),true);
// NewUI
  DisplayAuxScreen(AUX_SCREEN_USE_TOUCHSCREEN, true);	// TAR 227236
  
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  ps.Message12(MSG_TIMERESTRICTEDITEM, true);
  ps.ShowCMFrame();
#endif // _CPPUNIT
  return STATE_NULL;                
}

SMStateBase  *SMTimeRestrictedItemBase::PSButton1(void)// ok
{
	if (fInMultiSelectPickList)
		return setAndCreateAnchorState( BES_PRODUCEFAVORITES );
    else
	{
		return setAndCreateAnchorState(BES_SCANANDBAG);
	}

}
#ifndef _CPPUNIT

SMStateBase  *SMTimeRestrictedItemBase::PSEnterKey(void)
{
  DMSayPhrase(KEYPADVALID);
  //Tar 123351
  //SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
  return PSButton1();
}

void SMTimeRestrictedItemBase::UnInitialize()
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("TimeRestrictedItem") );
}

#endif // _CPPUNIT
SMStateBase *SMTimeRestrictedItemBase::TimedOut(void)
{
    if (g_bInRecallMode)
    {
        trace(L6, _T("SMTimeRestrictedItemBase::TimedOut in recall mode, pressing OK button"));
        return PSButton1();
    }
    else
        return STATE(State)::TimedOut();
}
