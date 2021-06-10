//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMItemQuantityExceededBase.cpp
//
// TITLE: Class implementation for per item quantity exceeded
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMItemQuantityExceeded.h"
#include "DMProcedures.h"
DLLEXPORT extern DMProcedures dm;   // Global DM object

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("ItemQuantityExceededBase")

IMPLEMENT_DYNCREATE(SMItemQuantityExceededBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(ItemQuantityExceeded)


DEFINE_TIMESTAMP

////////////////////////////////////
SMItemQuantityExceededBase::SMItemQuantityExceededBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////////
SMStateBase  *SMItemQuantityExceededBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ItemQuantityExceeded") );


  // +TAR 407200
  if (fInMultiSelectPickList)
  {
      g_bMultiPickSkipBaggingAllowed = true;
  }
  // -TAR 407200


  DMSayPhrase(QUANTITYLIMITREACHED);

  lQuantityRestrictedItems++;
  CString csDesc, csInstruction;
  csDesc = ps.GetPSText(RA_QUANTITYRESTRICTED_ID, SCOT_LANGUAGE_PRIMARY);
  csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_LANEBTT_QUANTITYRESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_ITEM_QUANTITY_EXCEEDED, SCOT_LANGUAGE_PRIMARY), co.IsSecurityDisableOn()==true ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), ps.GetPSText(RA_THIRDLINE_QUANTITYRESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY));
  trace(L7, _T("+ra.OnQuantityRestrictedItem, Message=%s"), csInstruction);
  ra.OnQuantityRestrictedItem(csInstruction);
  trace(L7, _T("-ra.OnQuantityRestrictedItem"));
  if (co.IsImmedInterventionOn())
  {
      return PSButton1();
  }	
  setAnchorState(BES_ITEMQUANTITYEXCEEDED);
  dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                      DM_TRICOLORLIGHT_BLINK_1HZ,
                      DM_TRICOLORLIGHT_NORMALREQUEST);
 

  //@@@INFOPOINT
  ps.SetCMFrame(_T("ItemQuantityExceeded"));
  ps.CMButtonFlash(_T("CMButton1Med"),true);   
  
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  //@@@INFOPOINT
  
  ps.Prompt(io.d.csDescription);
	
  //ps.Message13(MSG_RECALLED);
  ps.Message12(MSG_ITEM_QUANTITY_EXCEEDED, true);
  ps.ShowCMFrame();
  return STATE_NULL;                // stay in ItemNotForSaleBase state
}

SMStateBase  *SMItemQuantityExceededBase::PSButton1(void)// ok
{
  //Tar 123351 
  //SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
  if (fInMultiSelectPickList)
	  return setAndCreateAnchorState( BES_PRODUCEFAVORITES );
  else
  {
  
	return setAndCreateAnchorState(BES_SCANANDBAG);
	
  }
}

SMStateBase  *SMItemQuantityExceededBase::PSEnterKey(void)
{
  DMSayPhrase(KEYPADVALID);
  //Tar 123351 
  //SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
  return PSButton1();
}

void SMItemQuantityExceededBase::UnInitialize()
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ItemQuantityExceeded") );
}

