#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSecSkipBaggingThreshold.h"      
#include "SMSecUnexpectedDecrease.h"
#include "SMSecUnExpectedIncrease.h"
#include "SMScaleBroken.h"          // Reset scale
//#include "PSInterface.h"
#else
#include "SMSecSkipBaggingThresholdBase.h"
#endif //_CPPUNIT
#ifndef _CPPUNIT
#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SecSkipBaggingThreshold")
#endif //_CPPUNIT


IMPLEMENT_DYNCREATE(SMSecSkipBaggingThresholdBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP
#ifndef _CPPUNIT
//////////////////////////////////////////
SMSecSkipBaggingThresholdBase::SMSecSkipBaggingThresholdBase(const BEST PrevState)
{
	m_PrevState = PrevState;
	m_NextState = BES_SECSKIPBAGGINGTHRESHOLD;
  IMPLEMENT_TIMESTAMP
}
//////////////////////////////////////////
SMSecSkipBaggingThresholdBase::SMSecSkipBaggingThresholdBase()
{
  IMPLEMENT_TIMESTAMP
}
///////////////////////////////////
////////////////////////////////////////
SMStateBase  *SMSecSkipBaggingThresholdBase::Deliver(const BEST PrevState)
{
  m_PrevState = PrevState;
  m_NextState = BES_SECSKIPBAGGINGTHRESHOLD;
  return this;
}
////////////////////////////////////////
bool SMSecSkipBaggingThresholdBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be one parameter, an ints
    if (dlParameterList.GetDescriptorCount() == 1)
    {
        trace(L6, _T("+SMSecSkipBaggingThresholdBase::DeliverDescriptorList() - Parameter Count of 1"));
        Deliver((BEST)DLParamDWORD(dlParameterList));   
        return true;
    }
    else
    {
        trace(L2, _T("+SMSecSkipBaggingThresholdBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}
///////////////////////////////////////
SMStateBase  *SMSecSkipBaggingThresholdBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SecSkipBaggingThreshold") );	// RFC 330050
//	ra.RequestEnableAssistMode(false); //TAR 418121
	//The text here is just the name of the section in scotdataentry.ini.  Don't need to be in SCOTMSGE.DAT
	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("SkipBaggingBtnThreshold"));
	m_View = DATANEEDED_DONTBAGTHRESHOLDEXCEEDED;
	CString csSecInfo, csDesc;
	csDesc.Format(ps.GetPSText(MSG_SKIP_BAGGING_THRESHOLD_EXCEEDED, SCOT_LANGUAGE_PRIMARY));

	if (dm.fStateTakeawayBelt)
		csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_WAITFORASSISTANCE, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_SKIPBELTTHRESHOLD, SCOT_LANGUAGE_PRIMARY));
	else
		csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_WAITFORASSISTANCE, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_SKIPBAGGINGTHRESHOLD, SCOT_LANGUAGE_PRIMARY));

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
void SMSecSkipBaggingThresholdBase::showScreen(bool passwordVerified)
{
    ps.SetPSText(PS_WORK1,ps.GetPSText(BTT_STORELOGIN,SCOT_LANGUAGE_PRIMARY));
    ps.SetButtonText(_T("ButtonStoreLogIn"),PS_WORK1);
	nTimeOut = co.getTimeEASDeactivationScreenTimer();

	if (passwordVerified)
		return;


		if (inputMode == PROMPTING_FOR_OPERATOR)
		{
            // NewUI
			STATE(SecOperatorPWState)::ShowEnterID();  //SR93 Biometrics support
			ps.SetCMLeadthruText( TXT_ENTER_ID );
			ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
			ps.ShowCMFrame(false);
            // NewUI
			return;
		}
		else
		{  // Display Wait for Approval....   
			DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));	// play wave here so it doesn't replay on the key pad screen
            // NewUI
			ps.SetCMFrame(_T("SecSkipBaggingThreshold"));
			ps.CMButton(_T("ButtonGoBack"), BTT_STORELOGIN, true);
		}

	CString csWork;
	if (dm.fStateTakeawayBelt)
	{
	 csWork = ps.GetPSText(MSG_WAITFORASSISTANCE);
	 csWork += _T("\n* ") + ps.GetPSText(TXT_SKIP_BELT_THRESHOLD);
	}
	
	else
	{
		csWork = ps.GetPSText(MSG_WAITFORASSISTANCE);
		csWork += _T("\n* ") + ps.GetPSText(TXT_SKIP_BAGGING_THRESHOLD);
	}
	ps.SetCMLeadthruText(LTT_WAITFORASSISTANCE);
    // NewUI
	SAWLDBScannerDisable();			// Disable the scanner
	ps.ShowCMTBText(csTBMessage2Scot);	//thp
	ps.Message12(csWork, true);
	trace(L7, _T("+ra.OnTransactionState, Itemization State"));
	ra.OnSetTransactionState(RA_STATE_SECURITY_VIOLATION);
	trace(L7, _T("-ra.OnTransactionState"));
	ps.ShowCMFrame(false);
}
//////////////////////////////////////
void SMSecSkipBaggingThresholdBase::UnInitialize(void)
{
//	ra.RequestEnableAssistMode(true);//TAR 418121
    DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 
	ClearSecurityViolationLight();
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SecSkipBaggingThreshold") );	// RFC 330050
}

//////////////////////////////////////
SMStateBase *SMSecSkipBaggingThresholdBase::stateAfterPasswordVerify()
{
//+TAR 444373 - DO NOT CALL SetLanguage() to change the language code to get the right message.  We should 
// pass in the language code to GetPSText()
//  ps.SetLanguage(m_customerLanguage);
//-TAR 444373

    lSARedNeedingApproval = false;   // TAR 197015 Clear alert flag.
	
	// do same thing as OnRAPClearSecurityBtn() does
	return OnRAPClearSecurityBtn(_T("Lane"));
}

///////////////////////////////////////
SMStateBase *SMSecSkipBaggingThresholdBase::TimedOut(void) // timeout
{
	DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
    return STATE_NULL;
}

///////////////////////////////////////
SMStateBase  *SMSecSkipBaggingThresholdBase::PSButtonGoBack(void) // store login
{
	//TAR193505+: At EnterID screen, "go back" should be back to SMSecUnExpectedIncreaseBase screen

		switch(inputMode)
		{  
			case SHOW_WAIT_FOR_APPROVAL:
			case PROMPTING_FOR_PASSWORD:
				break;
			case PROMPTING_FOR_OPERATOR:
				{
					//+SR93 Biometrics support
					if (co.m_bOperationsUseFingerPrintDevice)
					{
						FT_Abort();
					}
					//-SR93 Biometrics support

					inputMode = SHOW_WAIT_FOR_APPROVAL;
					if (co.GetfStateDualLanguage())
						ps.SetLanguage(m_customerLanguage);  //TAR 196315: Set back to customerLanuguage for dual language
					showScreen(false);
					return STATE_NULL;  
				}
		}

	//TAR193505-

    DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 
	promptForOperator();
    return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecSkipBaggingThresholdBase::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
#ifndef NewSec
	if(dm.fStateTakeawayBelt)	// is on a take away system
	{
	    ss.SetSkipBagging(false); //TAR199154 false: tell SS not stop all timers
	}
	ss.SetLastGoodWeight(0x8000000);  // Remove the expectation
#endif
	if(dm.fStateTakeawayBelt)
	{
		TABstateBeforeTransport = BES_EMPTY_STATE;
	}
	
    // NewSec
    OverrideSecurityException(lpszAuthorizationType, KEY_EXCEPTION_OPERATION_CLEAR);

	return ThresholdViolRAPClearSecBtn(m_PrevState);
}


/*
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecSkipBaggingThresholdBase::OnWtDecrease()	 
{
	return ThresholdViolWtDecrease(m_PrevState, BES_SECSKIPBAGGINGTHRESHOLD);
}
*/
#endif //_CPPUNIT

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecSkipBaggingThresholdBase::OnWtIncreaseNotAllowed()	 
{
	cDataEntry.FromConfigData(co.GetcsDataEntryINIFile(), _T("UnexpectedIncrease"));
	m_View = DATANEEDED_UNEXPECTEDINCREASE;
	CString csInstruction, csDesc;
	csDesc.Format(ps.GetPSText(RA_UNEXPECTEDINCREASEINFO, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits);
	// RA_WEIGHTINCREASEINFO is a string that says "Actual weight: %s %s".  This name should be changed to RA_WEIGHTCHANGEINFO LPM022002
	csInstruction.Format(ps.GetPSText(RA_WEIGHTINCREASEINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTINCREASE, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits, _T(""));
	ra.OnRAPDataNeeded(csInstruction, cDataEntry.ToString(), m_View);
	DMSaySecurity( SEC_NONE, _T("blank.wav") );

	CREATE_AND_DISPENSE(SecUnExpectedIncrease)(getAnchorState(), BES_SECSKIPBAGGINGTHRESHOLD);

	//return ThresholdViolWtIncreaseNotAllowed(m_PrevState, BES_SECSKIPBAGGINGTHRESHOLD);	// set state to return to 
}
