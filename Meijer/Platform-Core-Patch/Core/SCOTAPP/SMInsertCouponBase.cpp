//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMInsertCouponBase.CPP
//
// TITLE: Class implementation for Insert Coupon state
//
// AUTHOR: Aiqin Hou
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMInsertCoupon.h"      
#include "SMTakeCard.h"             //TAR247289
#include "MPProcedures.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("InsertCouponBase")
DLLEXPORT extern HWND hw;           
DLLEXPORT extern MPProcedures mp;   

IMPLEMENT_DYNCREATE(SMInsertCouponBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

static bool bScreenTimerExpired = false; //39149

//////////////////////////////////////////
SMInsertCouponBase::SMInsertCouponBase()
: m_bMotionEscalationFired(false)
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////
SMInsertCouponBase::SMInsertCouponBase(const BEST prevState, const BEST nextState)
: m_bMotionEscalationFired(false),
  m_Prevstate(prevState),
  m_Nextstate(nextState)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMInsertCouponBase::Deliver(const BEST argPrevState, const BEST argNextState)
{
  m_Prevstate = argPrevState;
  m_Nextstate = argNextState;
  return this;
}
////////////////////////////////////////
bool SMInsertCouponBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be two parameters, both ints
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMInsertCouponBase::DeliverDescriptorList() - Parameter Count of 2"));
        Deliver((BEST)DLParamDWORD(dlParameterList),    
                (BEST)DLParamDWORD(dlParameterList));   
        return true;
    }
    else
    {
        trace(L2, _T("+SMInsertCouponBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}


///////////////////////////////////////
SMStateBase  *SMInsertCouponBase::Initialize(void)
{
    //entering the InsertCoupon context
	dm.EnableCouponProcessing (true);	//enabling coupon processing
    int iReturn;
    PSXRC psxReturn;
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("InsertCoupon") );

	//Update RAP window with SCOT state
    UpdateStateName(ps.GetPSText(LTT_INSERTCOUPON, SCOT_LANGUAGE_PRIMARY));

	nTimeOut = co.getTimeOutCouponSensorScreenTimer();

    setAnchorState(BES_INSERTCOUPON);	   
	iReturn = DMSaySecurity( SEC_NONE, ps.GetSoundName(INSERTCOUPON));
    ASSERT(iReturn == 0); // To make lint happy, should find file name

	ps.SetCMFrame(_T("InsertCoupon"));

    //To ensure store login button is not visable
    psxReturn = m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), _T("InsertCoupon"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
    ASSERT(psxReturn == PSX_SUCCESS);

	SAWLDBScannerDisable();			// Disable the scanner

	ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	ps.ShowCMTBText(csTBMessage2Scot);	
	long lReturn = ps.Message12(MSG_INSERTCOUPON,true);
    ASSERT (lReturn == 0); // Make lint happy
	lReturn = DMCouponLight(DM_MISC_STATE_ONE_HZ); //turn on coupon slot loght
    ASSERT (lReturn == 0); // Make lint happy
	lReturn = ps.ShowCMFrame();
    ASSERT (lReturn == 0); // Make lint happy

    if (!co.IsLoginBypassOn()) 
    {
        nEscalationRAPTimeOut = co.getTimeOutCouponSensorScreenTimer();
        if(nEscalationRAPTimeOut != 0)
        {
            //+ TAR [384711]: note: added this if statement to bypass SetTimer ands to prevent going back to "InsertCoupon" screen
		    if(!((inputMode == PROMPTING_FOR_OPERATOR && m_bMotionEscalationFired) || inputMode == PROMPTING_FOR_PASSWORD))
            {
			    SetTimer(hw, RAP_ESCALATION, (unsigned int)nEscalationRAPTimeOut, &EscalationTimedOut);
		    }
		    //- TAR [384711]
        }
    }

	bScreenTimerExpired = false; //391498
    return STATE_NULL;                // stay in InsertCoupon state
}

//////////////////////////////////////
void SMInsertCouponBase::UnInitialize(void)
{
    dm.EnableCouponProcessing (false);	//disabling coupon processing
    KillTimer(hw,RAP_ESCALATION);   //Ensure timer is dead
    nEscalationRAPTimeOut = 0;
	long lReturn = DMSaySecurity(SEC_NONE, _T("blank.wav"));
    ASSERT(0 == lReturn);
	lReturn = DMCouponLight(DM_MISC_STATE_OFF); //turn off coupon slot light
    ASSERT(0 == lReturn);
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("InsertCoupon") );
}

///////////////////////////////////////
SMStateBase *SMInsertCouponBase::TimedOut(void) // timeout
{
 	long lReturn = DMSaySecurity( SEC_NONE, ps.GetSoundName(INSERTCOUPON));
    ASSERT(0 == lReturn);
    if (!co.IsLoginBypassOn())
    {
        // SA250050: Set timer has been moved to initialize because having it in TimedOut() will constantly 
        // trigger ReportInterventionStart() requests

        //+ TAR [384711]: note: added this if statement to bypass SetTimer ands to prevent going back to "InsertCoupon" screen
		//if(!((inputMode == PROMPTING_FOR_OPERATOR && m_bMotionEscalationFired) || inputMode == PROMPTING_FOR_PASSWORD))
        //{
			//SetTimer(hw, RAP_ESCALATION, (unsigned int)nEscalationRAPTimeOut, &EscalationTimedOut);
		//}
		//- TAR [384711]
        return STATE_NULL;                // stay in InsertCoupon state
    }
    else
    {
        return OnCouponSensor();
    }
}

////////////////////////////////////
SMStateBase  *SMInsertCouponBase::OnCouponSensor(void) 
{
  	bEASReEnableScanner = true;
    if (m_bMotionEscalationFired) 
    {
	    // resets lane light, flushes belt, sets re-enable scanner flag, and tells RAP no data is needed
        ClearSecurityViolationLight();//End of Tar 209034 & 209045

	    //Clear RAP Approval 
	    trace(L2, _T("Calling ra.OnNoRAPDataNeeded()"));
	    ra.OnNoRAPDataNeeded();  
    }
	//TAR247289+
	if ( !fInSignedOn &&  !fInSignedOff ) //TB_CLOSE has called
	{
		//CREATE_AND_DISPENSE(TakeCard)(true);
		setAnchorState(m_Nextstate);
		return TBEndOfTransaction();
	}
	//TAR247289-
    return setAndCreateAnchorState(m_Nextstate);
}
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
VOID CALLBACK SMInsertCouponBase::EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
    trace(L7, _T("+SMInsertCouponBase::EscalationTimedOut"));
	mp.sCurr->Escalation2RAP();
    trace(L7, _T("-SMInsertCouponBase::EscalationTimedOut"));

} //lint !e715 !e818  Calling sequence is used many places and is outside the scope of here to change
/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMTakeChangeBase::Escalation2RAP
//
// Purpose:		2nd timeout routine handling the delayed security notification
//
// Parameters:	none
//
// Returns:		void
//
/////////////////////////////////////////////////////////////////////////////////////
void SMInsertCouponBase::Escalation2RAP()
{ 
    trace(L7, _T("+SMInsertCouponBase::Escalation2RAP"));
    SMStateBase *pReturn;
   
    m_bMotionEscalationFired = true;
    KillTimer(hw,RAP_ESCALATION); 
    nEscalationRAPTimeOut = 0;
    SAEnterSecurityCondition();
	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("InsertCouponTimerExpires"));  //lint !e534 I can't quite figure out what the return code means
                                                                                  // but I also don't care
	m_View = DATANEEDED_INSERTCOUPONTIMEREXPIRED;


	CString csSecInfo ;
    csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), _T(" "), 
        // TODO change this line
        ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_INSERTCOUPON, SCOT_LANGUAGE_PRIMARY), 
        _T(""), ps.GetPSText(RA_TIMEREXPIRED, SCOT_LANGUAGE_PRIMARY));    
    ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View); 
	ra.RequestEnableAssistMode(true); 
	pReturn = STATE(SecOperatorPWState)::Initialize();			
    ASSERT(STATE_NULL == pReturn);  // Make lint happy
    trace(L7, _T("-SMInsertCouponBase::Escalation2RAP"));
 }
/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMInsertCouponBase::OnRAPClearSecurityBtn
//
// Purpose:		Handle the steps in clearing a security violation from RAP
//              Resets lane light,  sets re-enable scanner flag, and tells RAP no data is needed
//
// Parameters:	none
//
// Returns:		
//
/////////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMInsertCouponBase::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
    long lReturn;

	trace(L6, _T("+SMInsertCouponBase::OnRAPClearSecurityBtn"));
	lReturn = DMSaySecurity( SEC_NONE, _T("blank.wav") );			//to interrupt the wave file if it's playing 
    ASSERT(lReturn == 0);
    // resets lane light, sets re-enable scanner flag, and tells RAP no data is needed
	ClearSecurityViolationLight();//End of Tar 209034 & 209045

	//Clear RAP Approval 
	trace(L2, _T("Calling ra.OnNoRAPDataNeeded()"));
	ra.OnNoRAPDataNeeded(); 
        // NewSec
    OverrideSecurityException(lpszAuthorizationType, KEY_EXCEPTION_OPERATION_CLEAR);

    //Let's the customer finish the transaction without unblocking the sensor
    trace(L6, _T("SMInsertCouponBase::OnRAPClearSecurityBtn<Customer permitted to continue without unblocking the sensor>"));
	trace(L6, _T("-SMInsertCouponBase::OnRAPClearSecurityBtn"));

    return (OnCouponSensor());
}
/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMInsertCouponBase::stateAfterPasswordVerify
//
// Purpose:		
//
// Parameters:	none
//
// Returns:		
//
/////////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMInsertCouponBase::stateAfterPasswordVerify()
{
	trace(L2, _T("+SMInsertCouponBase::stateAfterPasswordVerify"));
	trace(L2, _T("-SMInsertCouponBase::stateAfterPasswordVerify"));
	return OnRAPClearSecurityBtn(_T("Lane"));
}
/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMInsertCouponBase::showScreen
//
// Purpose:		
//
// Parameters:	Boolean passwordVerified
//
// Returns:		void
//
/////////////////////////////////////////////////////////////////////////////////////
void SMInsertCouponBase::showScreen(bool passwordVerified)
{
	COleVariant v;
    long lReturn;
    PSXRC psxReturn;

	if (passwordVerified)
		return;
	

		if (inputMode == PROMPTING_FOR_OPERATOR)
		{
			//+SR93 Biometrics support
			STATE(SecOperatorPWState)::ShowEnterID();  
			ps.SetCMLeadthruText( TXT_ENTER_ID );
			ps.AllowInput( MAX_DIGITS_OPERATOR, false, false );
			
			ps.ShowCMFrame(false);  // TAR [384711]: note: added to display "Enter ID" and "Enter Password" screens
			
			//-SR93 Biometrics support

			return;
		}
		else
		{  
			ps.SetCMFrame(_T("InsertCoupon"));
			v = VARIANT_TRUE;
		    psxReturn = m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), _T("InsertCoupon"), UI::PROPERTYVISIBLE, v);
            ASSERT(psxReturn == PSX_SUCCESS);
		}

    ps.ShowCMTBText(csTBMessage2Scot);
	lReturn = ps.ShowCMFrame(false);
    ASSERT(0 == lReturn);
}
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMInsertCouponBase::OnWtDecrease()
{
    // If coupons being entered during finalization, call the special handlers
    // for wt decrease during finalization and ignore the decrease.  Otherwise
    // call the base implementation.
    // SR695-The finalization decrease will not be ignored if we are not forgiving 
    // finalization decreases and the balance is not completely paid.
    if (m_Nextstate == BES_ENTERCOUPONS)
    {
		if(!g_bIsForgiveUnexpectedDecreaseDuringTendering && !(lBalanceDue < 0.01))
			return FinalizationWtDecreaseNotAllowed(getAnchorState());
		else
			return STATE_NULL;
    }
    else
    {
        return SMStateBase::OnWtDecrease();
    }
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMInsertCouponBase::OnWtIncreaseNotAllowed()
{
    // If coupons being entered during finalization, call the special handlers
    // for a wt increase during finalization, othersise call the base implementation
    if (m_Nextstate == BES_ENTERCOUPONS)
    {
	    return FinalizationWtIncrease(getAnchorState());
    }
    else
    {
        return SMStateBase::OnWtIncreaseNotAllowed();
    }
}

//+ TAR [384711]
SMStateBase  *SMInsertCouponBase::PSButtonGoBack(void) // Store Login button
{	
	switch(inputMode)
	{
		case SHOW_WAIT_FOR_APPROVAL:
		case PROMPTING_FOR_PASSWORD:
		{
			break;
		}

		case PROMPTING_FOR_OPERATOR:
		{
			inputMode = SHOW_WAIT_FOR_APPROVAL;

			if (co.fStateDualLanguage)
			{
				ps.SetLanguage(m_customerLanguage);
			}

			showScreen(false);

			return STATE_NULL;
		}
	}

	DMSaySecurity( SEC_NONE, _T("blank.wav") );
	promptForOperator();
	return STATE_NULL;
}
//- TAR [384711]