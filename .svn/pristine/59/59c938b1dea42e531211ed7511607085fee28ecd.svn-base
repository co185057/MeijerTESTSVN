//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectLanguageBase.cpp
//
// TITLE: Class implementation for Customer language selection state
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#ifndef _CPPUNIT
#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMAttract.h"

#else  // _CPPUNIT
#endif // _CPPUNIT
#include "SMSelectLanguage.h"       // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SelectLanguageBase")

IMPLEMENT_DYNCREATE(SMSelectLanguageBase, CObject)// MFC Runtime class/object information
//IMPLEMENT_STATE(CustomerSample)

DEFINE_TIMESTAMP


//////////////////////////////////////////
SMSelectLanguageBase:: SMSelectLanguageBase ()
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////
SMStateBase  * SMSelectLanguageBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SelectLanguage") );	// RFC 330050
  //+SSCOP-523
  ps.SetLanguage(m_customerLanguage); 

  //-SSCOP-523
  ps.SetCMFrame(_T("SelectLanguage"));
  DMScannerDisable();

  // ++ Commented out for NXTUI-73 SystemFunctions buttons
  //ps.LoadLanguageList();
  // -- NXTUI-73

  DMSayPhrase(SELECTFROMLIST);  //TAR 275455
  ps.ShowAmountDueAmountPaid(lBalanceDue - nDMCashInserted, // TAR275456
                             lTotalPaid  + nDMCashInserted);
  ps.ShowCMFrame(false);
  return STATE_NULL;                // stay in Suspend state
}

#ifndef _CPPUNIT
//////////////////////////////////////////
void SMSelectLanguageBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SelectLanguage") );	// RFC 330050
	//sscoadk-5890+
    if(bIsTransactionStarted)
		 setAnchorState(BES_SCANANDBAG);  
	else
         setAnchorState(BES_START);
    //sscoadk-5890-
}
SMStateBase  * SMSelectLanguageBase:: PSButtonGoBack (void)
{
	if(bIsTransactionStarted)
		return setAndCreateAnchorState(BES_SCANANDBAG);  
	else
		return setAndCreateAnchorState(BES_START);  
	//return setAndCreateAnchorState(BES_SCANANDBAG);
}

SMStateBase *SMSelectLanguageBase::ButtonPressed(int buttonindex) 
{
  	SCOT_LANGUAGE_TYPE tmpLanguage = m_customerLanguage;
	m_customerLanguage = (SCOT_LANGUAGE_TYPE)buttonindex;
	CString str = co.Getlanguage(m_customerLanguage).szCode;
	if(	tmpLanguage != m_customerLanguage)
	{
		SendTBCommand(TBCMD_CUSTOMERLANGUAGE, CString(co.Getlanguage(m_customerLanguage).szCode));
	}

    ps.SetNextGenUICustomerLanguage(m_customerLanguage);

    ps.SetLanguage((SCOT_LANGUAGE_TYPE)buttonindex);
    SetPreferredLanguage(m_customerLanguage, _T("Button") );
    ps.SetCMReceiptToCurrentLanguage();  //SSCOP-836
   
    //SSCOADK-5989 + 
    if (co.fStateDualLanguage && co.fStateDisplayWeightsAndMeasures 
      && dm.IsDeviceConfigured(DMCLASS_SCALE))
    {
    // If healthy, then set scale display for weights and measures 
      long retErrorCode = DMConfigureScaleDisplay();
      if( retErrorCode > -1)
      {
	    fStateFreeze = false;
	    CString csWork = ps.GetPSText(retErrorCode);

	    return SMCustomerModeState::createLevel1Message(
                    csWork,  // char *message
				    PS_NOTEXT,         // no canned message
				    PS_NOTEXT,         // no prompt
				    0,                 // no prompt value
				    -1,                // no device
				    false, false);     // no DM check health needed //SR672
      }
    }
    //SSCOADK-5989 -

	co.csCMCustomerMessage = ps.GetCustomerMessageDefault(); //SSCOB-1860
	CustomerMessage = co.csCMCustomerMessage;

	if(gpSCOTAuxDisp)
	{
		gpSCOTAuxDisp->SetLanguage(m_customerLanguage);
	}
	ps.SetTareListToCurrentLanguage();

	COleVariant v;
        v = ps.GetPSText(TXT_AMOUNTDUE);
        m_pAppPSX->SetControlProperty(_T("Amount1Label"), UI::PROPERTYTEXT, v); //TAR 277815

	
    //Load Quickpick if the Pref is empty
    if (co.fOperationsDisplayQuickPickItems)
    {
	    ps.DisplayQuickPickItem();
    }


   	// Get the language name
   	CString csLanguageName = co.Getlanguage(buttonindex).csName;
	if(_istdigit(csLanguageName[0])) //if its a msg ID
	{ 
		int iMsgID = _ttoi(co.Getlanguage(buttonindex).csName);
		csLanguageName = ps.GetPSText(iMsgID, (SCOT_LANGUAGE_TYPE) buttonindex);
	}
	
	rp.SetLanguageName(csLanguageName);


	if(bIsTransactionStarted)
		return setAndCreateAnchorState(BES_SCANANDBAG);  
	else
    {
        SMAttractBase::m_bLanguageButtonClicked = true;
		return setAndCreateAnchorState(BES_START);  
    }
	 //createAnchorState();//setAndCreateAnchorState(BES_SCANANDBAG);  

}

SMStateBase *SMSelectLanguageBase::PSListLookup(CString cItemCode) // list or lookup selection
{
// NewUI
	long lItemCode = 0;
	lItemCode = _ttoi(cItemCode);
	return ButtonPressed(lItemCode);
// NewUI
}
#endif // _CPPUNIT