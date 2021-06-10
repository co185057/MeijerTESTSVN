//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMInsertGiftCardBase.CPP
//
// TITLE: Class implementation for InsertGiftCard state
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "MPProcedures.h"
#else
#include "TestMacros.h"
#endif
#include "SMInsertGiftCard.h" 

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("InsertGiftCardBase")

DLLEXPORT extern HWND hw;  
#ifndef _CPPUNIT         
DLLEXPORT extern MPProcedures mp;   
#endif //_CPPUNIT

IMPLEMENT_DYNCREATE(SMInsertGiftCardBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

#ifndef _CPPUNIT
//////////////////////////////////////////
SMInsertGiftCardBase::SMInsertGiftCardBase()
: m_bMotionEscalationFired(false)
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////
SMInsertGiftCardBase::SMInsertGiftCardBase(const BEST prevState, const BEST nextState)
: m_bMotionEscalationFired(false),
  m_Prevstate(prevState),
  m_Nextstate(nextState)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMInsertGiftCardBase::Deliver(const BEST argPrevState, const BEST argNextState)
{
  m_Prevstate = argPrevState;
  m_Nextstate = argNextState;
  return this;
}
////////////////////////////////////////
bool SMInsertGiftCardBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be two parameters, both ints
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMInsertGiftCardBase::DeliverDescriptorList() - Parameter Count of 2"));
        Deliver((BEST)DLParamDWORD(dlParameterList),    
                (BEST)DLParamDWORD(dlParameterList));   
        return true;
    }
    else
    {
        trace(L2, _T("+SMInsertGiftCardBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}


///////////////////////////////////////
SMStateBase  *SMInsertGiftCardBase::Initialize(void)
{
	//entering the InsertGiftCard context
	dm.EnableCouponProcessing (true);	//enabling coupon processing
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("InsertGiftCard") );	// RFC 330050
    int iReturn;
    PSXRC psxReturn;

	//Update RAP window with SCOT state
    UpdateStateName(ps.GetPSText(LTT_DEPOSITGIFTCARD, SCOT_LANGUAGE_PRIMARY));

	nTimeOut = co.getTimeOutCouponSensorScreenTimer();

    setAnchorState(BES_INSERTGIFTCARD);	   
	iReturn = DMSaySecurity( SEC_NONE, ps.GetSoundName(DEPOSITYOURCARD));
    ASSERT(iReturn == 0); // To make lint happy, should find file name

	ps.SetCMFrame(_T("InsertGiftCard"));

    //To ensure store login button is not visable
    psxReturn = m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), _T("InsertGiftCard"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
    ASSERT(psxReturn == PSX_SUCCESS);

	SAWLDBScannerDisable();			// Disable the scanner

	ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	ps.ShowCMTBText(csTBMessage2Scot);	
	long lReturn = ps.Message12(MSG_DEPOSTITGIFTCARD,true);
    ASSERT (lReturn == 0); // Make lint happy
	lReturn = DMCouponLight(DM_MISC_STATE_ONE_HZ); //turn on coupon slot light
    ASSERT (lReturn == 0); // Make lint happy
	lReturn = ps.ShowCMFrame();
    ASSERT (lReturn == 0); // Make lint happy

    return STATE_NULL;                // stay in InsertGiftCard state
}

//////////////////////////////////////
void SMInsertGiftCardBase::UnInitialize(void)
{
    dm.EnableCouponProcessing (false);	//disabling coupon processing
    KillTimer(hw,RAP_ESCALATION);   //Ensure timer is dead
	long lReturn = DMSaySecurity(SEC_NONE, _T("blank.wav"));
    ASSERT(0 == lReturn);
	lReturn = DMCouponLight(DM_MISC_STATE_OFF); //turn off coupon slot light
    ASSERT(0 == lReturn);
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("InsertGiftCard") );	// RFC 330050
}

///////////////////////////////////////
SMStateBase *SMInsertGiftCardBase::TimedOut(void) // timeout
{
 	long lReturn = DMSaySecurity( SEC_NONE, ps.GetSoundName(DEPOSITYOURCARD));
    ASSERT(0 == lReturn);
    SetTimer(hw, RAP_ESCALATION, (unsigned int)nEscalationRAPTimeOut, &EscalationTimedOut);
	return STATE_NULL;                // stay in InsertGiftCard state
}

////////////////////////////////////
SMStateBase  *SMInsertGiftCardBase::OnCouponSensor(void) 
{
  	bEASReEnableScanner = true;
    if (m_bMotionEscalationFired) 
    {
	    // resets lane light, flushes belt, sets re-enable scanner flag, and tells RAP no data is needed
        ClearSecurityViolationLight();

	    //Clear RAP Approval 
	    trace(L2, _T("Calling ra.OnNoRAPDataNeeded()"));
	    ra.OnNoRAPDataNeeded();  
    }

	return ReturnDelayedStateForGCTender();
}
#endif _CPPUNIT
/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMTakeChangeBase::EscalationTimedOut
//
// Purpose:		handles passing timeout event to Escalation2RAP using the current state pointer
//
// Parameters:	none
//
// Returns:		void
//
/////////////////////////////////////////////////////////////////////////////////////
VOID CALLBACK SMInsertGiftCardBase::EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
    trace(L7, _T("+SMInsertGiftCardBase::EscalationTimedOut"));
    KillTimer(hw,RAP_ESCALATION); 
    mo.PostGP(GP_ESCALATE_TO_RAP);
    trace(L7, _T("-SMInsertGiftCardBase::EscalationTimedOut"));

} //lint !e715 !e818  Calling sequence is used many places and is outside the scope of here to change
/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void MInsertGiftCardBase::Escalation2RAP
//
// Purpose:		2nd timeout routine handling the delayed security notification
//
// Parameters:	none
//
// Returns:		void
//
/////////////////////////////////////////////////////////////////////////////////////
void SMInsertGiftCardBase::Escalation2RAP()
{
	//dm.saveTriColorLightStatus();
    return;  //Handle escalation in GPNotify
}


/////////////////////////////////////////////////////
SMStateBase *SMInsertGiftCardBase::GPNotify(const GPEVT gpEvt)
{
    SMStateBase *retState = STATE_NULL;
    trace(L6, _T("+SMInsertGiftCardBase::GPNotify(%d)"), gpEvt);
    if (gpEvt == GP_ESCALATE_TO_RAP)
    {
        m_bMotionEscalationFired = true;
        SAEnterSecurityCondition();
        cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("InsertCouponTimerExpires"));  //lint !e534 I can't quite figure out what the return code means
        // but I also don't care
        m_View = DATANEEDED_INSERTCOUPONTIMEREXPIRED;
        
        
        CString csSecInfo ;
        csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), _T(" "), 
            // TODO change this line
            ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_DEPOSITGIFTCARD, SCOT_LANGUAGE_PRIMARY), 
            _T(""), ps.GetPSText(RA_TIMEREXPIRED, SCOT_LANGUAGE_PRIMARY));    
        ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View); 
        ra.RequestEnableAssistMode(true); 
        retState = STATE(SecOperatorPWState)::Initialize();			
        trace(L7, _T("-SMInsertGiftCardBase::Escalation2RAP"));
    }
	trace(L6, _T("-SMInsertGiftCardBase::GPNotify"));
    return retState;
}

#ifndef _CPPUNIT
/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMInsertGiftCardBase::OnRAPClearSecurityBtn
//
// Purpose:		Handle the steps in clearing a security violation from RAP
//              Resets lane light,  sets re-enable scanner flag, and tells RAP no data is needed
//
// Parameters:	none
//
// Returns:		
//
/////////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMInsertGiftCardBase::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
    long lReturn;

	trace(L6, _T("+SMInsertGiftCardBase::OnRAPClearSecurityBtn"));
	lReturn = DMSaySecurity( SEC_NONE, _T("blank.wav") );			//to interrupt the wave file if it's playing 
    ASSERT(lReturn == 0);
    // resets lane light, sets re-enable scanner flag, and tells RAP no data is needed
	ClearSecurityViolationLight();

	//Clear RAP Approval 
	trace(L2, _T("Calling ra.OnNoRAPDataNeeded()"));
	ra.OnNoRAPDataNeeded(); 

        // NewSec
    OverrideSecurityException(lpszAuthorizationType, KEY_EXCEPTION_OPERATION_CLEAR);
	
    //Let's the customer finish the transaction without unblocking the sensor
    trace(L6, _T("SMInsertGiftCardBase::OnRAPClearSecurityBtn<Customer permitted to continue without unblocking the sensor>"));
	trace(L6, _T("-SMInsertGiftCardBase::OnRAPClearSecurityBtn"));
    return (OnCouponSensor());
}
/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMInsertGiftCardBase::stateAfterPasswordVerify
//
// Purpose:		
//
// Parameters:	none
//
// Returns:		
//
/////////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMInsertGiftCardBase::stateAfterPasswordVerify()
{
	trace(L2, _T("+SMInsertGiftCardBase::stateAfterPasswordVerify"));
	trace(L2, _T("-SMInsertGiftCardBase::stateAfterPasswordVerify"));
	return OnRAPClearSecurityBtn(_T("Lane"));
}
/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMInsertGiftCardBase::showScreen
//
// Purpose:		
//
// Parameters:	Boolean passwordVerified
//
// Returns:		void
//
/////////////////////////////////////////////////////////////////////////////////////
void SMInsertGiftCardBase::showScreen(bool passwordVerified)
{
    ps.SetPSText(PS_WORK1,ps.GetPSText(BTT_STORELOGIN,SCOT_LANGUAGE_PRIMARY));
        ps.SetButtonText(_T("ButtonStoreLogIn"),PS_WORK1);
	COleVariant v;
    long lReturn;
    PSXRC psxReturn;

	if (passwordVerified)
		return;
	

		if (inputMode == PROMPTING_FOR_OPERATOR)
		{
			//Tar 393757
			STATE(SecOperatorPWState)::ShowEnterID(); //SR93 Biometrics
			ps.SetCMLeadthruText( TXT_ENTER_ID ); //Tar 395105
			lReturn = ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
            ASSERT(lReturn <= co.GetnOperationsOperatorIDMaxDigit());
            lReturn = ps.ShowCMFrame(false);
            ASSERT(0 == lReturn);
			return;
		}
		else
		{  
			ps.SetCMFrame(_T("InsertGiftCard"));
			v = VARIANT_TRUE;
		    psxReturn = m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), _T("InsertGiftCard"), UI::PROPERTYVISIBLE, v);
            ASSERT(psxReturn == PSX_SUCCESS);
		}

    ps.ShowCMTBText(csTBMessage2Scot);
	lReturn = ps.ShowCMFrame(false);
    ASSERT(0 == lReturn);
}
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMInsertGiftCardBase::OnWtDecrease()
{
	if(!g_bIsForgiveUnexpectedDecreaseDuringTendering && !(lBalanceDue < 0.01))
		return FinalizationWtDecreaseNotAllowed(BES_INSERTGIFTCARD);
	else
		return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMInsertGiftCardBase::OnWtIncreaseNotAllowed()
{
 	return FinalizationWtIncrease(BES_INSERTGIFTCARD);
}
#endif _CPPUNIT