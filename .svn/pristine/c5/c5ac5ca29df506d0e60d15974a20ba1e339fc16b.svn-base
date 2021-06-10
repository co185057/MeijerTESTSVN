//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMItemNotForSaleBase.cpp
//
// TITLE: Class implementation for Item Recalled State
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMItemNotForSale.h"   
#include "DMProcedures.h"
DLLEXPORT extern DMProcedures dm;   // Global DM object

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
  if (co.IsImmedInterventionOn())
  {
      return PSButton1();
  }

  setAnchorState(BES_ITEMNOTFORSALE);
  dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                      DM_TRICOLORLIGHT_BLINK_1HZ,
                      DM_TRICOLORLIGHT_NORMALREQUEST);

  ps.SetCMFrame(_T("ItemNotForSale"));
  ps.CMButtonFlash(_T("CMButton1Med"),true);   
  
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  ps.Message12(MSG_RECALLED, true);
  ps.ShowCMFrame();
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

