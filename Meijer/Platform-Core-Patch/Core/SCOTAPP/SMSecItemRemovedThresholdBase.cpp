#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSecItemRemovedThreshold.h"      
#include "SMSecUnexpectedDecrease.h"
#include "SMScaleBroken.h"          // Reset scale
//#include "PSInterface.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SecItemRemovedThreshold")


IMPLEMENT_DYNCREATE(SMSecItemRemovedThresholdBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSecItemRemovedThresholdBase::SMSecItemRemovedThresholdBase(const BEST prevState)
{
  m_PrevState = prevState;
  m_NextState = BES_SECITEMREMOVEDTHRESHOLD;

  IMPLEMENT_TIMESTAMP
}
//////////////////////////////////////////
SMSecItemRemovedThresholdBase::SMSecItemRemovedThresholdBase()
{
  IMPLEMENT_TIMESTAMP
}
///////////////////////////////////
////////////////////////////////////////
SMStateBase  *SMSecItemRemovedThresholdBase::Deliver(const BEST argPrevState)
{
  m_PrevState = argPrevState;
  m_NextState = BES_SECITEMREMOVEDTHRESHOLD;
  return this;
}
////////////////////////////////////////
bool SMSecItemRemovedThresholdBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be one parameter, an ints
    if (dlParameterList.GetDescriptorCount() == 1)
    {
        trace(L6, _T("+SMSecItemRemovedThresholdBase::DeliverDescriptorList() - Parameter Count of 1"));
        Deliver((BEST)DLParamDWORD(dlParameterList));   
        return true;
    }
    else
    {
        trace(L2, _T("+SMSecItemRemovedThresholdBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}
///////////////////////////////////////
SMStateBase  *SMSecItemRemovedThresholdBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SecItemRemovedThreshold") );	// RFC 330050
	ra.RequestEnableAssistMode(false);
	//The text here is just the name of the section in scotdataentry.ini.  Don't need to be in SCOTMSGE.DAT
	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("ItemRemovedBtnThreshold"));
	m_View = DATANEEDED_DONEREMOVINGTHRESHOLDEXCEEDED;
	CString csSecInfo, csDesc;
	csDesc.Format(ps.GetPSText(MSG_ITEM_REMOVED_THRESHOLD_EXCEEDED, SCOT_LANGUAGE_PRIMARY));
	csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_WAITFORASSISTANCE, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_REMOVEITMESTHRESHOLD, SCOT_LANGUAGE_PRIMARY));
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
void SMSecItemRemovedThresholdBase::showScreen(bool passwordVerified)
{
	nTimeOut = co.getTimeEASDeactivationScreenTimer();

	if (passwordVerified)
		return;


		if (inputMode == PROMPTING_FOR_OPERATOR)
		{
			STATE(SecOperatorPWState)::ShowEnterID();  //SR93 Biometrics support
			ps.SetCMLeadthruText( TXT_ENTER_ID );			
			ps.AllowInput( MAX_DIGITS_OPERATOR, false, false );
			ps.ShowCMFrame( false );   
			return;
		}
		else
		{  // Display Wait for Approval....   
			DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));	// play wave here so it doesn't replay on the key pad screen
            // NewUI
			ps.SetCMFrame(_T("SecItemRemovedThreshold"));
			ps.CMButton(_T("ButtonStoreLogIn"), BTT_STORELOGIN, true);
		}



	CString csWork;
	csWork = ps.GetPSText(MSG_WAITFORASSISTANCE);
	csWork += _T("\n* ") + ps.GetPSText(TXT_ITEM_REMOVED_THRESHOLD);

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
void SMSecItemRemovedThresholdBase::UnInitialize(void)
{
	ra.RequestEnableAssistMode(true);
    DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 
	ClearSecurityViolationLight();
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SecItemRemovedThreshold") );	// RFC 330050
}

//////////////////////////////////////
SMStateBase *SMSecItemRemovedThresholdBase::stateAfterPasswordVerify()
{
	ps.SetLanguage(m_customerLanguage); //tar 192275
    lSARedNeedingApproval = false;   // TAR 197015 Clear alert flag.

	// do same thing as OnRAPClearSecurityBtn() does
	return OnRAPClearSecurityBtn(_T("Lane"));
}

///////////////////////////////////////
SMStateBase *SMSecItemRemovedThresholdBase::TimedOut(void) // timeout
{
	DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
    return STATE_NULL;
}

///////////////////////////////////////
SMStateBase  *SMSecItemRemovedThresholdBase::PSButtonGoBack(void) // store login
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
					if (co.fStateDualLanguage)
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
SMStateBase *SMSecItemRemovedThresholdBase::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
        // NewSec
    OverrideSecurityException(lpszAuthorizationType, KEY_EXCEPTION_OPERATION_CLEAR);

    return ThresholdViolRAPClearSecBtn(m_PrevState);
}

/*
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecItemRemovedThresholdBase::OnWtIncreaseNotAllowed()	 
{
	return ThresholdViolWtIncreaseNotAllowed(Prevstate, BES_SECITEMREMOVEDTHRESHOLD);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecItemRemovedThresholdBase::OnWtDecrease()	 
{
	return ThresholdViolWtDecrease(Prevstate, BES_SECITEMREMOVEDTHRESHOLD);
}
*/
