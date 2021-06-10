//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMCollectGiftCardBase.cpp
//
// TITLE:	Class implementation for CollectGiftCard state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMCollectGiftCard.h"      
#include "MPProcedures.h"		


#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("CollectGiftCardBase")
         
DLLEXPORT extern MPProcedures mp;  

IMPLEMENT_DYNCREATE(SMCollectGiftCardBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMCollectGiftCardBase::SMCollectGiftCardBase(const BEST prevState, const BEST nextState)
: m_Prevstate(prevState),
  m_Nextstate(nextState)
{
  IMPLEMENT_TIMESTAMP
}
//////////////////////////////////////////
SMCollectGiftCardBase::SMCollectGiftCardBase()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMCollectGiftCardBase::Deliver(const BEST prevState, const BEST nextState)
{
  m_PrevState = prevState;
  m_NextState = nextState;
  return this;
}
////////////////////////////////////////
bool SMCollectGiftCardBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be two parameters, both ints
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMCollectGiftCardBase::DeliverDescriptorList() - Parameter Count of 2"));
        Deliver((BEST)DLParamDWORD(dlParameterList),    
                (BEST)DLParamDWORD(dlParameterList));   
        return true;
    }
    else
    {
        trace(L2, _T("+SMCollectGiftCardBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}


///////////////////////////////////////
SMStateBase  *SMCollectGiftCardBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("CollectGiftCard") );	// RFC 330050
	//The text here is just the name of the section in scotdataentry.ini.  Don't need to be in SCOTMSGE.DAT
	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("CollectGiftCard"));
	m_View = DATANEEDED_COLLECTGIFTCARD;
	CString csSecInfo, csDesc;
	csDesc.Format(ps.GetPSText(RA_COLLECTGIFTCARD, SCOT_LANGUAGE_PRIMARY));
	csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_COLLECTGIFTCARD_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_COLLECTGIFTCARD, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""));
	ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);	
	SAEnterSecurityCondition();
	return STATE(SecOperatorPWState)::Initialize();
}

////////////////////////////////////////////
void SMCollectGiftCardBase::showScreen(bool passwordVerified)
{ 
    ps.SetPSText(PS_WORK1,ps.GetPSText(BTT_STORELOGIN,SCOT_LANGUAGE_PRIMARY));
    ps.SetButtonText(_T("ButtonStoreLogIn"),PS_WORK1);
	CString csWork;
	nTimeOut = co.getTimeEASDeactivationScreenTimer();

	if (passwordVerified)
		return;

	if (inputMode == PROMPTING_FOR_OPERATOR)
		{	
			//Tar 393757
			STATE(SecOperatorPWState)::ShowEnterID();  //SR93 Biometrics 393757
			ps.SetCMLeadthruText( TXT_ENTER_ID );  //Tar 395105
			ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
			ps.ShowCMFrame( false );  

            return;
		}
		else
		{  // Display Assistance Needed for Approval....  
			DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
			ps.SetCMFrame(_T("CollectGiftCard"));
			ps.CMButton(_T("ButtonStoreLogIn"), BTT_STORELOGIN, true);
		}


	ps.SetCMLeadthruText(LTT_WAITFORASSISTANCE);

	SAWLDBScannerDisable();	
	ps.ShowCMTBText(csTBMessage2Scot);	
	csWork = ps.GetPSText(MSG_WAITFORASSISTANCE);
	csWork += _T("\n* ") + ps.GetPSText(MSG_COLLECTGIFTCARD);
    ps.Message12(csWork, true);

	ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);

	trace(L7, _T("+ra.OnTransactionState, Wait For Approval State"));
	ra.OnSetTransactionState(RA_STATE_WAIT_FOR_APPROVAL);
	trace(L7, _T("-ra.OnTransactionState"));
	ps.ShowCMFrame(false);
}
//////////////////////////////////////
void SMCollectGiftCardBase::UnInitialize(void)
{
	DMSaySecurity( SEC_NONE, _T("blank.wav") );
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("CollectGiftCard") );	// RFC 330050
}

///////////////////////////////////////
SMStateBase  *SMCollectGiftCardBase::PSButtonGoBack(void) //Store login
{
	//At EnterID screen, "go back" should be back to SMCollectGiftCardBase screen
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


    DMSaySecurity( SEC_NONE, _T("blank.wav") );
	promptForOperator();
    return STATE_NULL;
}

///////////////////////////////////////
SMStateBase *SMCollectGiftCardBase::TimedOut(void)
{
	DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
    return STATE_NULL;
}

//////////////////////////////////////
SMStateBase *SMCollectGiftCardBase::stateAfterPasswordVerify()
{
	ps.SetLanguage(m_customerLanguage); 
	lSARedNeedingApproval = false;   //Clear alert flag.

	return OnRAPClearSecurityBtn(_T("Lane"));
}

///////////////////////////////////////////////////////////////////////////
SMStateBase  *SMCollectGiftCardBase::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
	ClearSecurityViolationLight();
	ra.OnNoRAPDataNeeded();
	return ReturnDelayedStateForGCTender();
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMCollectGiftCardBase::OnWtIncreaseNotAllowed()	 
{
	return FinalizationWtIncrease(BES_COLLECTGIFTCARD);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMCollectGiftCardBase::OnWtDecrease()
{	
	if(!g_bIsForgiveUnexpectedDecreaseDuringTendering && !(lBalanceDue < 0.01))
		return FinalizationWtDecreaseNotAllowed(BES_COLLECTGIFTCARD);
	else
		return STATE_NULL;
}
