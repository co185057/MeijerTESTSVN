#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSecBagBoxThreshold.h"      
#include "SMSecUnexpectedDecrease.h"
#include "SMScaleBroken.h"          // Reset scale
//#include "PSInterface.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SecBagBoxThreshold")


IMPLEMENT_DYNCREATE(SMSecBagBoxThresholdBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSecBagBoxThresholdBase::SMSecBagBoxThresholdBase(const BEST prevState)
{
  m_PrevState = prevState;
  m_NextState = BES_SECBAGBOXTHRESHOLD;

  IMPLEMENT_TIMESTAMP
}
//////////////////////////////////////////
SMSecBagBoxThresholdBase::SMSecBagBoxThresholdBase()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSecBagBoxThresholdBase::Deliver(const BEST argPrevState)
{
  m_PrevState = argPrevState;
  m_NextState = BES_SECBAGBOXTHRESHOLD;
  return this;
}
////////////////////////////////////////
bool SMSecBagBoxThresholdBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be one parameter, an ints
    if (dlParameterList.GetDescriptorCount() == 1)
    {
        trace(L6, _T("+SMSecBagBoxThresholdBase::DeliverDescriptorList() - Parameter Count of 1"));
        Deliver((BEST)DLParamDWORD(dlParameterList));   
        return true;
    }
    else
    {
        trace(L2, _T("+SMSecBagBoxThresholdBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}
///////////////////////////////////////
SMStateBase  *SMSecBagBoxThresholdBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SecBagBoxThreshold") );	// RFC 330050
	ra.RequestEnableAssistMode(false);
	//The text here is just the name of the section in scotdataentry.ini.  Don't need to be in SCOTMSGE.DAT
	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("BagBoxBtnThreshold"));
	m_View = DATANEEDED_BAGBOXTHRESHOLDEXCEEDED;	// the name of the RAP view
	CString csSecInfo, csDesc;
	csDesc.Format(ps.GetPSText(MSG_BAG_BOX_THRESHOLD_EXCEEDED, SCOT_LANGUAGE_PRIMARY));
	csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_WAITFORASSISTANCE, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_BAGBOXTHRESHOLD, SCOT_LANGUAGE_PRIMARY));
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
void SMSecBagBoxThresholdBase::showScreen(bool passwordVerified)
{
	nTimeOut = co.getTimeEASDeactivationScreenTimer();

	if (passwordVerified)
		return;


		if (inputMode == PROMPTING_FOR_OPERATOR)
		{
			STATE(SecOperatorPWState)::ShowEnterID();  //SR93 Biometrics support
			ps.SetCMLeadthruText( TXT_ENTER_ID );
			ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
			ps.ShowCMFrame( false );   
			return;
		}
		else
		{  // Display Wait for Approval....   
			if (secAppControlEventParms.m_csTare == _T("")) // TAR 294023
			{
				DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));	// play wave here so it doesn't replay on the key pad screen
			}
            // NewUI
			ps.SetCMFrame(_T("SecBagBoxThreshold"));
			ps.CMButton(_T("ButtonStoreLogIn"), BTT_STORELOGIN, true);
            ps.SetPSText(PS_WORK1,ps.GetPSText(BTT_STORELOGIN,SCOT_LANGUAGE_PRIMARY));
            ps.SetButtonText(_T("ButtonStoreLogIn"),PS_WORK1);
		}


	CString csWork;
	csWork = ps.GetPSText(MSG_WAITFORASSISTANCE);
	csWork += _T("\n* ") + ps.GetPSText(TXT_BAGBOX_THRESHOLD);

	ps.SetCMLeadthruText(LTT_WAITFORASSISTANCE);
    // End NewUI
	SAWLDBScannerDisable();			// Disable the scanner
	ps.ShowCMTBText(csTBMessage2Scot);	//thp
	ps.Message12(csWork, true);
	trace(L7, _T("+ra.OnTransactionState, Itemization State"));
	ra.OnSetTransactionState(RA_STATE_SECURITY_VIOLATION);
	trace(L7, _T("-ra.OnTransactionState"));
	ps.ShowCMFrame(false);
}
//////////////////////////////////////
void SMSecBagBoxThresholdBase::UnInitialize(void)
{
	ra.RequestEnableAssistMode(true);
	DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 
	ClearSecurityViolationLight();
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SecBagBoxThreshold") );	// RFC 330050
}

//////////////////////////////////////
SMStateBase *SMSecBagBoxThresholdBase::stateAfterPasswordVerify()
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
SMStateBase *SMSecBagBoxThresholdBase::TimedOut(void) // timeout
{
	DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
    return STATE_NULL;
}

///////////////////////////////////////
SMStateBase  *SMSecBagBoxThresholdBase::PSButtonGoBack(void) // store login
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
SMStateBase *SMSecBagBoxThresholdBase::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
#ifndef NewSec
	if(dm.fStateTakeawayBelt)	// is on a take away system
	{
        ss.SetSkipBagging(false); //TAR199154 false: tell SS not stop all timers
	}
	ss.SetLastGoodWeight(0x8000000);		// remove expectation
#endif
    // NewSec
    OverrideSecurityException(lpszAuthorizationType, KEY_EXCEPTION_OPERATION_CLEAR);

    //SSCOADK-5362: If there is WaitForApproval state waiting, clear it
    if (g_pDelay_WaitForApprovalStateChange != NULL)
    {
        CString csStateName = g_pDelay_WaitForApprovalStateChange->GetRuntimeClass()->m_lpszClassName;
        delete g_pDelay_WaitForApprovalStateChange;
        g_pDelay_WaitForApprovalStateChange = NULL;
        trace(L6,_T("Delete the state %s saved in g_pDelay_WaitForApprovalStateChange"),csStateName);
    }
   
	return ThresholdViolRAPClearSecBtn(m_PrevState);
}

/*
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecBagBoxThresholdBase::OnWtIncreaseNotAllowed()	 
{
	return ThresholdViolWtIncreaseNotAllowed(m_PrevState, BES_SECBAGBOXTHRESHOLD);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecBagBoxThresholdBase::OnWtDecrease()	 
{
	return ThresholdViolWtDecrease(Prevstate, BES_SECBAGBOXTHRESHOLD);
}
*/

///////////////////////////////////////////////////////////////////////////
//TAR377288
SMStateBase *SMSecBagBoxThresholdBase::OnBackToLGW(void)	
{
	return createState(BES_SCANANDBAG);
}
