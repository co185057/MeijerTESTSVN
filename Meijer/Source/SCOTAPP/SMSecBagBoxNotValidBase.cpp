//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMSecBagBoxNotValidBase.h
//
// TITLE:	Class implementation for Bag Box Not Valid Screen (Done for SR085)
//
// AUTHOR:  Shakti Kamal
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#include <stdio.h>
#include <stdlib.h>
#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSecUnexpectedDecrease.h"
#include "SMScaleBroken.h"          // Reset scale
#endif
#include "SMSecBagBoxNotValid.h"      

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SecBagBoxNotValid")


IMPLEMENT_DYNCREATE(SMSecBagBoxNotValidBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

SMSecBagBoxNotValidBase::SMSecBagBoxNotValidBase(const BEST prevState)
{
  m_PrevState = prevState;
  m_NextState = BES_SECBAGBOXNOTVALID;

  IMPLEMENT_TIMESTAMP
}

SMSecBagBoxNotValidBase::SMSecBagBoxNotValidBase()
{
  IMPLEMENT_TIMESTAMP
}
#ifndef _CPPUNIT
SMStateBase  *SMSecBagBoxNotValidBase::Deliver(const BEST argPrevState)
{
  m_PrevState = argPrevState;
  m_NextState = BES_SECBAGBOXNOTVALID;
  return this;
}

bool SMSecBagBoxNotValidBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    if (dlParameterList.GetDescriptorCount() == 1)
    {
        trace(L6, _T("+SMSecBagBoxNotValidBase::DeliverDescriptorList() - Parameter Count of 1"));
        Deliver((BEST)DLParamDWORD(dlParameterList));   
        return true;
    }
    else
    {
        trace(L2, _T("+SMSecBagBoxNotValidBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}

SMStateBase  *SMSecBagBoxNotValidBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SecBagBoxNotValid") );
	ra.RequestEnableAssistMode(false);
	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("BagBoxNotValid"));//let it be this for the time being - ask HFE for a new c:/scot/config/scotdataentry.ini entry
	m_View = DATANEEDED_BAGBOXNOTVALID;
	CString csSecInfo, csExpectedTareWts = _T(""), csTareWt1, csTareWt2, csTareWt3, csObservedWt;
	
	//converting the tare wts to 0.00 lb or 0.000 kg and setting up the RAP display
	if (co.fOperationsScaleMetric)
    {
		//+sr714.. add condition and cstarewt3
      if( g_csTareWt1 != _T("") )
	  {
		csTareWt1 = FmtWtToString(_ttol(g_csTareWt1));
		csExpectedTareWts = csTareWt1;
	  }
	  if( g_csTareWt2 != _T("") )
	  {
		csTareWt2 = FmtWtToString(_ttol(g_csTareWt2));
		csExpectedTareWts = csExpectedTareWts +  _T(", ") + csTareWt2;
	  }
	  if( g_csTareWt3 != _T("") )
	  {
		csTareWt3 = FmtWtToString(_ttol(g_csTareWt3));
		csExpectedTareWts = csExpectedTareWts + _T(", ") + csTareWt3;
	  }

	  if(g_csTareWt1 != _T("") || g_csTareWt2 != _T("") || g_csTareWt3 != _T("") )
		  csExpectedTareWts = csExpectedTareWts + _T(" ") + csScaleUnits;		 
	  else
		  csExpectedTareWts = ps.GetPSText(MSG_INVALIDWEIGHT,SCOT_LANGUAGE_PRIMARY);
		//sr714-

	  csObservedWt = secEventParms.m_csDeltaWeight;
    }
    else
    {
		//sr714.. add condition and cstarewt3
       if( g_csTareWt1 != _T("") )
	  {
		csTareWt1.Format(_T("%4.2f"),1.0*_ttol(g_csTareWt1)/1000.0);
		csExpectedTareWts = csTareWt1;
	  }
	  if( g_csTareWt2 != _T("") )
	  {
		csTareWt2.Format(_T("%4.2f"),1.0*_ttol(g_csTareWt2)/1000.0);
		csExpectedTareWts = csExpectedTareWts + _T(", ") + csTareWt2;
	  }
	  if( g_csTareWt3 != _T("") )
	  {
		csTareWt3.Format(_T("%4.2f"),1.0*_ttol(g_csTareWt3)/1000.0);
		csExpectedTareWts = csExpectedTareWts + _T(", ") + csTareWt3;
	  }

	  if(g_csTareWt1 != _T("") || g_csTareWt2 != _T("") || g_csTareWt3 != _T("") )
		  csExpectedTareWts = csExpectedTareWts + _T(" ") + csScaleUnits;		 
	  else
		  csExpectedTareWts = ps.GetPSText(MSG_INVALIDWEIGHT,SCOT_LANGUAGE_PRIMARY);
		//sr714-
	  csObservedWt = secEventParms.m_csDeltaWeight;
    }	
	//sr714+
	if( co.fOperationsShowWeightInfoOnLane )
		ps.ShowCMWeightNumber(csExpectedTareWts, secEventParms.m_csDeltaWeight + _T(" ") + csScaleUnits, lItems);
    else
		ps.HideCMWeightNumber();
	//sr714-

	csSecInfo.Format(ps.GetPSText(RA_INVALIDBAGINFO, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_BAGBOXNOTVALID, SCOT_LANGUAGE_PRIMARY), csExpectedTareWts, _T(""), csObservedWt, csScaleUnits);
	
	if( secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_OUTSTANDING )
	{
		ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
		SAEnterSecurityCondition();
	}
	else if( secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_DELAYED )
	{
	  trace(L7, _T("+ra.OnNormalItem, Message = %s"), csSecInfo);
	  ra.OnNormalItem(csSecInfo);
	  trace(L7, _T("-ra.OnNormalItem"));
	}
	return STATE(SecOperatorPWState)::Initialize();
}

////////////////////////////////////////////
void SMSecBagBoxNotValidBase::showScreen(bool passwordVerified)
{
	nTimeOut = co.getTimeEASDeactivationScreenTimer();

	if (passwordVerified)
		return;


		if (inputMode == PROMPTING_FOR_OPERATOR)
		{	
			//Tar 393757
			STATE(SecOperatorPWState)::ShowEnterID();   //SR93 Biometrics support	
			ps.SetCMLeadthruText( TXT_ENTER_ID );		//TAR395610
			ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
			ps.ShowCMFrame( false );   
			return;
		}
		else
		{  
            // NewUI
			ps.SetCMFrame(_T("SecBagBoxNotValid"));
			ps.CMButton(_T("ButtonStoreLogIn"), BTT_STORELOGIN, true);
		}
	
	ps.SetCMFrame(_T("SecBagBoxNotValid"));
	ps.SetCMLeadthruText(LTT_BAGBOXNOTVALID);
    SAWLDBScannerDisable();//Disable the scanner
//+SSCOP-2528
	COleVariant vReturn;
	if( ps.GetLCID(ps.GetLanguage()) == _T("0401"))
	{
		m_pAppPSX->GetConfigProperty(_T("CMWeightObservedAmount"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT,vReturn);
		CString csTemp( _T("") );
		csTemp = vReturn.bstrVal;
		if(csTemp != _T("2"))
			m_pAppPSX->SetConfigProperty(_T("CMWeightObservedAmount"), ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT, _T("2")); 
		m_pAppPSX->GetConfigProperty(_T("CMItemNumberAmount"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT,vReturn);
		csTemp = vReturn.bstrVal;
		if(csTemp != _T("2"))
			m_pAppPSX->SetConfigProperty(_T("CMItemNumberAmount"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT, _T("2"));
		m_pAppPSX->GetConfigProperty(_T("CMWeightExpectedAmount"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT,vReturn);
		csTemp = vReturn.bstrVal;
		if(csTemp != _T("2"))
			m_pAppPSX->SetConfigProperty(_T("CMWeightExpectedAmount"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT, _T("2"));
		m_pAppPSX->GetConfigProperty(_T("CMWeightRemovedText"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT,vReturn);
		csTemp = vReturn.bstrVal;
		if(csTemp != _T("0"))
			m_pAppPSX->SetConfigProperty(_T("CMWeightRemovedText"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT, _T("0"));
		m_pAppPSX->GetConfigProperty(_T("CMWeightAddedText"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT,vReturn);
		csTemp = vReturn.bstrVal;
		if(csTemp != _T("0"))
			m_pAppPSX->SetConfigProperty(_T("CMWeightAddedText"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT, _T("0"));
		m_pAppPSX->GetConfigProperty(_T("CMItemNumberText"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT,vReturn);
		csTemp = vReturn.bstrVal;
		if(csTemp != _T("0"))
			m_pAppPSX->SetConfigProperty(_T("CMItemNumberText"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT, _T("0"));
		m_pAppPSX->GetConfigProperty(_T("CMItemNumberText"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT,vReturn);
		csTemp = vReturn.bstrVal;
		if(csTemp != _T("0"))
			m_pAppPSX->SetConfigProperty(_T("CMWeightExpectedText"),ps.GetCurrentContext(), UI::PROPERTYTEXTALIGNMENT, _T("0"));
		
	}
//-SSCOP-2528

	ps.ShowCMTBText(csTBMessage2Scot);
	ps.Message12(MSG_BAGBOXNOTVALID, true); //  Remove for TAR391244 , roll back for tar 392689
	trace(L7, _T("+ra.OnTransactionState, Itemization State"));
	ra.OnSetTransactionState(RA_STATE_SECURITY_VIOLATION);
	trace(L7, _T("-ra.OnTransactionState"));
	DMSayPhrase(VERIFYBAGS);//need to get this from HFE
	ps.ShowCMFrame();
}
//////////////////////////////////////
void SMSecBagBoxNotValidBase::UnInitialize(void)
{
	ra.RequestEnableAssistMode(true);
    DMSaySecurity( SEC_NONE, _T("blank.wav") );//to interrupt the wave file if it's playing 
	ClearSecurityViolationLight();
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SecBagBoxNotValid") );
}

//////////////////////////////////////
SMStateBase *SMSecBagBoxNotValidBase::stateAfterPasswordVerify()
{
//+TAR 444373 - DO NOT CALL SetLanguage() to change the language code to get the right message.  We should 
// pass in the language code to GetPSText()
//  ps.SetLanguage(m_customerLanguage);
//-TAR 444373
    lSARedNeedingApproval = false;

	// do same thing as OnRAPClearSecurityBtn() does
	return OnRAPClearSecurityBtn(_T("Lane"));
}

///////////////////////////////////////
SMStateBase *SMSecBagBoxNotValidBase::TimedOut(void) // timeout
{
	DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
    return STATE_NULL;
}

///////////////////////////////////////
SMStateBase  *SMSecBagBoxNotValidBase::PSButtonGoBack(void) // store login
{

	switch(inputMode)
	{  
		case SHOW_WAIT_FOR_APPROVAL:
		case PROMPTING_FOR_PASSWORD:
			break;
		case PROMPTING_FOR_OPERATOR:
			inputMode = SHOW_WAIT_FOR_APPROVAL;
			if (co.GetfStateDualLanguage())
				ps.SetLanguage(m_customerLanguage);
			showScreen(false);
			return STATE_NULL;  
	}
    DMSaySecurity( SEC_NONE, _T("blank.wav") );//to interrupt the wave file if it's playing 
	promptForOperator();
    return STATE_NULL;
}
#endif

///////////////////////////////////////
SMStateBase *SMSecBagBoxNotValidBase::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
#ifndef _CPPUNIT
	//417990 Only set the pre state to Scan and bag when having a mismatch before the invalid bag
	if(secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_MISMATCHED)
	{
		m_PrevState = BES_SCANANDBAG;
	}
    OverrideSecurityException(lpszAuthorizationType, KEY_EXCEPTION_OPERATION_CLEAR);
	ra.OnNoRAPDataNeeded();
    
    //428563+
	if (getAnchorState() == BES_START)
	{
		// enable the sensormatic EAS deactivator while in a transaction
		dm.EASDeactivatorEnable( EASEnable );
		SASendSCOTInput( ITEMIZATION );
		return TBStart();
	}
#endif
	//428563-
	//SSCOADK-919
    if (g_bOnItemOK) 
    {
#ifndef _CPPUNIT
        return ClearedSecViolSetReturnState(m_PrevState, m_NextState);
#endif
    }
    else
    {
        return STATE_NULL; //waiting on Security Manager to send OnItemOK
    }
}
#ifndef _CPPUNIT
///////////////////////////////////////
SMStateBase *SMSecBagBoxNotValidBase::OnWtDecrease()	 
{
    setAnchorState(BES_SCANANDBAG);
	return STATE(SecOperatorPWState)::OnWtDecrease();
}

///////////////////////////////////////
SMStateBase *SMSecBagBoxNotValidBase::OnBackToLGW()	 
{
	return ClearedSecViolSetReturnState(m_PrevState, m_NextState);
}

///////////////////////////////////////
SMStateBase *SMSecBagBoxNotValidBase::TareNone(void)
{
	return STATE_NULL;
}
#endif