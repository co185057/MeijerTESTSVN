//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectLanguageBase.cpp
//
// TITLE: Class implementation for Customer language selection state
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
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
  ps.SetCMFrame(_T("SelectLanguage"));
  DMScannerDisable();
  ps.LoadLanguageList();
  DMSayPhrase(SELECTFROMLIST);  //TAR 275455
  ps.ShowAmountDueAmountPaid(lBalanceDue - nDMCashInserted, // TAR275456
                             lTotalPaid  + nDMCashInserted);
  ps.ShowCMFrame(false);
  return STATE_NULL;                // stay in Suspend state
}

//////////////////////////////////////////
void SMSelectLanguageBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SelectLanguage") );	// RFC 330050
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
	CString str = co.language[m_customerLanguage].szCode;
	if(	tmpLanguage != m_customerLanguage)
	{
		SendTBCommand(TBCMD_CUSTOMERLANGUAGE, co.language[m_customerLanguage].szCode);
	}

    ps.SetLanguage((SCOT_LANGUAGE_TYPE)buttonindex);

    SetPreferredLanguage(m_customerLanguage, _T("Button") );

	co.csCMCustomerMessage = ps.GetPSText(co.nCMCustomerMessage);
	CustomerMessage = co.csCMCustomerMessage;

	if(gpSCOTAuxDisp)
	{
		gpSCOTAuxDisp->SetLanguage(m_customerLanguage);
	}
	ps.SetTareListToCurrentLanguage();
    ps.AssignTextToCtlsInCmCtxs(NONEPRIMARYCONTEXT);
	COleVariant v;
        v = ps.GetPSText(TXT_AMOUNTDUE);
        m_pAppPSX->SetControlProperty(_T("Amount1Label"), UI::PROPERTYTEXT, v); //TAR 277815

	
   	// Get the language name
   	CString csLanguageName = co.language[buttonindex].csName;
	if(_istdigit(csLanguageName[0])) //if its a msg ID
	{ 
		int iMsgID = _ttoi(co.language[buttonindex].csName);
		csLanguageName = ps.GetPSText(iMsgID, (SCOT_LANGUAGE_TYPE) buttonindex);
	}
	
	rp.SetLanguageName(csLanguageName);


	if(bIsTransactionStarted)
		return setAndCreateAnchorState(BES_SCANANDBAG);  
	else
		return setAndCreateAnchorState(BES_START);  
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
