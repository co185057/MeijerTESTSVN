//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMUnknownPriceBase.cpp
//
// TITLE: Class implimentation for unknown price State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMUnknownPrice.h"         // MGV this state
#include "DMProcedures.h"
DLLEXPORT extern DMProcedures dm;             // global DM object

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("UnknownPriceBase")

IMPLEMENT_DYNCREATE(SMUnknownPriceBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(UnknownPrice)


DEFINE_TIMESTAMP

////////////////////////////////
SMUnknownPriceBase::SMUnknownPriceBase()
{
  IMPLEMENT_TIMESTAMP
}

/////////////////////////////////////////
SMStateBase *SMUnknownPriceBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("UnknownPrice") );	// RFC 330050
  DMSayPhrase(UNKNOWNPRICE);
	
  dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                      DM_TRICOLORLIGHT_BLINK_1HZ,
                      DM_TRICOLORLIGHT_NORMALREQUEST);
	
// NewUI
  ps.SetCMFrame(_T("UnknownPrice"));
  ps.CMButtonFlash(_T("CMButton1Med"),true);
// NewUI
  
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  ps.Message12(MSG_NOPRICE, true);
  ps.ShowCMFrame();
  return STATE_NULL;                // stay in UnknownPriceBase state
}

SMStateBase *SMUnknownPriceBase::PSButton1(void)// ok
{
  return setAndCreateAnchorState(BES_SCANANDBAG); //Tar 127805
}

SMStateBase *SMUnknownPriceBase::PSEnterKey(void)
{
  DMSayPhrase(KEYPADVALID);
  return PSButton1();
}
// + RFC 330050
void SMUnknownPriceBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("UnknownPrice") );	
}
// - RFC 330050
