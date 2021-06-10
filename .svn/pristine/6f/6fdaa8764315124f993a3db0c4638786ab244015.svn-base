//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterWtForPriceEmbeddedBase.CPP
//
// TITLE: Class implementation for Forced Weighing of Price Embedded Items state
//
// AUTHOR:  Leila Parker
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>							// MS MFC always needed first
#include <afxtempl.h>						// MS MFC template header

#include "Common.h"							// Common includes
#include "SMState.h"						// Base state
#include "SMEnterWtForPriceEmbedded.h"      
#include "SMScaleBroken.h"					// Reset scale
//#include "PSInterface.h"

#define COMP_ID ID_SM						// Base state component
#define T_ID    _T("EnterWtForPriceEmbeddedBase")

IMPLEMENT_DYNCREATE(SMEnterWtForPriceEmbeddedBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(EnterWtForPriceEmbedded)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMEnterWtForPriceEmbeddedBase::SMEnterWtForPriceEmbeddedBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////
SMStateBase  *SMEnterWtForPriceEmbeddedBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("EnterWtForPriceEmbedded") );	// RFC 330050
  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_PUTONSCALE, SCOT_LANGUAGE_PRIMARY)); 
  setAnchorState(BES_ENTERWTFORPRICEEMBEDDED);  //TAR#154147 return to this state from help on way
  //+tar 330491
  COleVariant v;
  m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("EnterWtForPriceEmbedded"), UI::PROPERTYVISIBLE, VARIANT_TRUE); 
  //-tar 330491
  // if we have the weight aleady because this is a
  // void and we have the original weight from the
  // receipt we can just use that weight
  if (io.fVoid && lVoidWgt>0)
  {
	ASSERT(0);
	trace(L3, _T("Voided Price Embedded item caused error. %s"), io.csItemCode);
  }
  DMScaleEnable();
  DMSayPhrase(PLACEITEMONSCALE);
  ra.RequestEnableAssistMode(false);  //TAR 297347

  ps.SetCMFrame(_T("EnterWtForPriceEmbedded"));
  if (dm.fStateTakeawayBelt)
  {
    ps.SetCMBackground(BMP_PUTONSCALE_BELT);
  }
  else
  {
    ps.SetCMBackground(BMP_PUTONSCALE);
  }

  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  ps.ShowCMFrame();
  return STATE_NULL;                // stay in EnterWtForPriceEmbedded state
}

////////////////////////////////////
SMStateBase  *SMEnterWtForPriceEmbeddedBase::PSButtonGoBack(void) // go back
{
  fItemIsWaiting = false;	// canceling item, don't want to go into TBItemSale again
  trace(L2, _T("Cancelling pending item %s"), io.csItemCode);
  SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
  return setAndCreateAnchorState(BES_SCANANDBAG);    
}

////////////////////////////////////
SMStateBase  *SMEnterWtForPriceEmbeddedBase::DMScale(void) 
{
  // let the base prime the 'virtual' scale
  SMStateBase  *sExit = SMStateBase ::DMScale();

  if (sExit!=STATE_NULL)
	return sExit;
	
  if (lDMScaleWeight==0)
    return STATE_NULL;	// shakey scale, wait for good weight
	
  if (lDMScaleWeight<0)
  {
    setAnchorState(BES_SCANANDBAG);
    RETURNSTATE(ScaleBroken)
  }

  nSellState = getAnchorState();		  // save current sell entry state

  io.l_gPriceEmbeddedWt = lDMScaleWeight; 
  m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("EnterWtForPriceEmbedded"), UI::PROPERTYVISIBLE, VARIANT_FALSE);  //tar 330491
  return TBEnterItem();
}
// + RFC 330050
void SMEnterWtForPriceEmbeddedBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("EnterWtForPriceEmbedded") );	
}
// - RFC 330050
