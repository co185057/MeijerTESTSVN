//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMKeyInCodeBase.CPP
//
// TITLE: Class implementation for Key In Code state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#ifndef _CPPUNIT
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMInProgress.h"           // In progress state
#include "SMEnterQuantity.h"
#include "SMSmAssistMenu.h"
#include "SMPLAEnterWeight.h"          //	enter wgt state

#else  // _CPPUNIT
#endif // _CPPUNIT

#include "SMKeyInCode.h"      

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("KeyInCodeBase")

IMPLEMENT_DYNCREATE(SMKeyInCodeBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(KeyInCode)

DEFINE_TIMESTAMP

static bool bReturnToProduceFavorites = false; //SSCOP-986
//////////////////////////////////////////
SMKeyInCodeBase::SMKeyInCodeBase()
{
  IMPLEMENT_TIMESTAMP
#ifndef _CPPUNIT
  saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
#endif // _CPPUNIT
}

#ifndef _CPPUNIT
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
	if (fInMultiSelectPickList || csLastNonHelpStateName == _T("SMProduceFavorites") /*SSCOP-827*/ ||
		bReturnToProduceFavorites ) //SSCOP-986 
	{
		bReturnToProduceFavorites = false; //SSCOP-986
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

#endif // _CPPUNIT
/////////////////////////////////////////
SMStateBase  *SMKeyInCodeBase::PSEnterKey(void)// enter key pressed, sell this item
{

#ifndef _CPPUNIT
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
  SAWLDBScannerDisable();   // Disable the scanner
  
#endif // _CPPUNIT
  if (VerifyQtyItem(csItemSent))     //Tar 143309 
  {
      SMStateBase::ms_bIsCurrentItemFromPickList = true;
      io.Reset();
	  io.csOrigCode = csItemSent;	   // item code keyed
	  io.csItemCode = csItemSent;	   // assumed to be the same
      io.d.bPickListItem = true;
      if (!co.IsPicklistAssistEnable()) // dont Send SASendScot when PLA is enable, Let TBItemSale called by PLAEnterWeight do
      {
          SASendSCOTInput( UPC_MANUAL, csItemSent);
      }
      SetNextGenUICurrentItem(io.csOrigCode);
	  RETURNSTATE(EnterQuantity)
  }
  g_bMultiPickSkipBaggingAllowed = false;// TAR 398289
  // + TAR 399975	
  if(co.IsInAssistMenus())
  {
	setAnchorState(BES_SCANANDBAG);		
  }
  // - TAR 399975
  if (co.IsPicklistAssistEnable() && ps.IsPickListItem(csItemSent))
  {
      io.Reset();
      io.csOrigCode = csItemSent;
      io.csItemCode = csItemSent;
      RETURNSTATE(PLAEnterWeight)
  }
  else
  {
      return TBItemSale(false,0,false);
  }
}
#ifndef _CPPUNIT

//+SR828
//////////////////////////////////////////////
SMStateBase *SMKeyInCodeBase::PSBackSpaceKey()
{
    PSClearInput();

    CString csValue;
    long lLen = ps.GetInput(csValue);
        
    if (lLen == 0) 
        ps.ClearInput();

    return STATE_NULL;
}
//-SR828

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
	return STATE(State)::OnBackToLGW();
}

SMStateBase  *SMKeyInCodeBase::PSButton8(void)
{
  
  RETURNSTATE(SmAssistMenu)
}

#endif// _CPPUNIT
SMStateBase *SMKeyInCodeBase::DMScanner(void)
{
  csItemSent = csDMLastBarCode;
  // + TAR 399975	
  if(co.IsInAssistMenus())
  {
	setAnchorState(BES_SCANANDBAG);		
  }
  // - TAR 399975

  if (co.IsPicklistAssistEnable() && ps.IsPickListItem(csDMLastBarLabel))
  {
      io.Reset();
      csItemSent = csDMLastBarLabel;
      io.csOrigCode = csItemSent;
      io.csItemCode = csItemSent;
      RETURNSTATE(PLAEnterWeight)
  }
  else
  {
      return TBItemSale(false,0,true);
  }
 
}
#ifndef _CPPUNIT

// + RFC 330050
void SMKeyInCodeBase::UnInitialize(void)
{
	if(!fInMultiSelectPickList)//SR679
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("KeyInCode") );	
	}
}
// - RFC 330050

//+SSCOP-986
SMStateBase *SMKeyInCodeBase::PSButtonHelp(void)
{
	if(csLastNonHelpStateName == _T("SMProduceFavorites"))
	{
		bReturnToProduceFavorites = true;
	}

	return STATE(State)::PSButtonHelp();
}
//-SSCOP-986

/////////////////////////////////////
// SSCOP-1754
// PSSwitchContext 
// 
// Purpose: Handle Switch Context event from PSX
// Parameters
//   bRemote - whether the switch occurred on the remote display
//   csNewContext - the name of the new context
SMStateBase *SMKeyInCodeBase::PSSwitchContext(BOOL bRemote, CString csNewContext)
{
    trace(L6, _T("+SMKeyInCodeBase::PSSwitchContext(%d, %s)"),bRemote, csNewContext);
    // Set Frame if context change is in the active context
    if( bRemote == ps.RemoteMode())
    {
        if (csNewContext.Find(_T("AM_KeyInCode")) != 0) 
        {
			if (ps.GetInput(csItemSent) >0)
			{
				trace(L7, _T("SMKeyInCodeBase::PSSwitchContext() - Clear previous Input"));
				PSClearKey();
			}
        }
    }

    trace(L6, _T("-SMKeyInCodeBase::PSSwitchContext()"));

    return STATE_NULL;
}


#endif // _CPPUNIT