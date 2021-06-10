//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMKeyInCodeBase.CPP
//
// TITLE: Class implementation for Key In Code state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMKeyInCode.h"      
#include "SMInProgress.h"           // In progress state
#include "SMEnterQuantity.h"
#include "SMSmAssistMenu.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("KeyInCodeBase")

IMPLEMENT_DYNCREATE(SMKeyInCodeBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(KeyInCode)

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMKeyInCodeBase::SMKeyInCodeBase()
{
  IMPLEMENT_TIMESTAMP
  saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
}

/////////////////////////////////////////
SMStateBase  *SMKeyInCodeBase::Initialize(void)
{

	if(!fInMultiSelectPickList)//SR679
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("KeyInCode") );	// RFC 330050
	}
	//Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(LTT_KEYINCODE, SCOT_LANGUAGE_PRIMARY));
	if (!co.IsInAssistMenus())
	{
	  SAWLDBScannerDisable();   // Disable the scanner
	}

	ps.SetCMFrame(_T("KeyInCode"));

	//Tar 417933
	SCOT_LANGUAGE_TYPE nLang = ps.GetLanguage();
    trace(L6, _T("SMKeyInCodeBase::Initialize() with language=%d"), nLang);
	
	ps.Message12(MSG_KEYINCODE, false);
	DMSayPhrase(KEYINCODETOUCHENTER);
	ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	ps.ShowCMTBText(csTBMessage2Scot);	//thp

	// allow the EchoInput() func to accept up to max digits
	ps.AllowInput(MAX_DIGITS_ITEMCODE,false,false);
	ps.ShowCMFrame();
	return STATE_NULL;    
}

//////////////////////////////////////////////////////
SMStateBase  *SMKeyInCodeBase::PSButtonGoBack(void) // go to S&B
{
	if (fInMultiSelectPickList)
	{
		g_bMultiPickSkipBaggingAllowed = true; // TAR417235
		return setAndCreateAnchorState( BES_PRODUCEFAVORITES );
	}
	else
		return setAndCreateAnchorState(BES_SCANANDBAG);	  
}

//////////////////////////////////////////////////////
SMStateBase  *SMKeyInCodeBase::PSNumericKey(const long lKey)
{
  PSEchoInput(lKey);
  nTimeIdleMsec=0;//TAR 400286
  return STATE_NULL;
}

/////////////////////////////////////////
SMStateBase  *SMKeyInCodeBase::PSClearKey(void)
{

    PSClearAllInput(true);
 
  return STATE_NULL;
}

/////////////////////////////////////////
SMStateBase  *SMKeyInCodeBase::PSEnterKey(void)// enter key pressed, sell this item
{
  long lLen = ps.GetInput(csItemSent);
  if (lLen==0)
  {
    DMSayPhrase(KEYPADINVALID);
    return PSClearAllInput(FALSE);
  }

  ps.DisableButtons();	
  
  // REQ 20060728-986 Start Special Transaction
  bool bAllowDefaultProcessing = true;
  tb.PreprocessData(csItemSent, _T("KEYBOARD_DATA"), &bAllowDefaultProcessing);
  if (!bAllowDefaultProcessing)
  {
    trace(L4, _T("PreprocessData() processing returns STATE_NULL"));
	return PSClearAllInput(FALSE);
  }

  //+dp185016 tar 288891
  if (InvalidCouponPrefix(csItemSent))
  {
      PSClearKey();
      dm.SaySecurity(SEC_NONE, ps.GetSoundName(COUPONSNOTACCEPTED));
      return STATE_NULL;
  }
  //-dp185016
  //SASendSCOTInput(ITEMIZATION);
  if (VerifyQtyItem(csItemSent))     //Tar 143309 
  {
      io.Reset();
	  io.csOrigCode = csItemSent;	   // item code keyed
	  io.csItemCode = csItemSent;	   // assumed to be the same
	  SASendSCOTInput( UPC_MANUAL, csItemSent);
	  RETURNSTATE(EnterQuantity)
  }
  g_bMultiPickSkipBaggingAllowed = false;// TAR 398289
  // + TAR 399975	
  if(co.IsInAssistMenus())
  {
	setAnchorState(BES_SCANANDBAG);		
  }
  // - TAR 399975
  return TBItemSale(false,0,false);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMKeyInCodeBase::OnBackToLGW(void)
{
	// If TAB, Key In code screen refreshes when Security calls OnBackToLGW, looses keyed in data LPM092302 214653
	if (dm.fStateTakeawayBelt)
	{
		trace(L6, _T("SMKeyInCodeBase::OnBackToLGW, returning STATE_NULL"));
		return STATE_NULL;
	}
	// else
	trace(L6, _T("SMKeyInCodeBase::OnBackToLGW, calling SMStateBase::OnBackToLGW()"));
	return SMStateBase::OnBackToLGW();
}

SMStateBase  *SMKeyInCodeBase::PSButton8(void)
{
  
  RETURNSTATE(SmAssistMenu)
}

SMStateBase *SMKeyInCodeBase::DMScanner(void)
{
  csItemSent = csDMLastBarCode;
  // + TAR 399975	
  if(co.IsInAssistMenus())
  {
	setAnchorState(BES_SCANANDBAG);		
  }
  // - TAR 399975

  return TBItemSale(false,0,true);
 
}
// + RFC 330050
void SMKeyInCodeBase::UnInitialize(void)
{
	if(!fInMultiSelectPickList)//SR679
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("KeyInCode") );	
	}
}
// - RFC 330050
