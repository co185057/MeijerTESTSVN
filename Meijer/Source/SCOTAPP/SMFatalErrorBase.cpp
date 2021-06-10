//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMFatalErrorBase.cpp
//
// TITLE: Class implementation for Fatal Error state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMFatalError.h"
#include "SMHelpOnWay.h"
#include "SMContinueTrans.h"
#include "SMSmAuthorization.h"		//Tar 194454 RF020702
#include "LaneStateData.h"


#define COMP_ID ID_SM               // Base state component
#define T_ID _T("FatalErrorBase")

// Welcome to the Hotel California

IMPLEMENT_DYNCREATE(SMFatalErrorBase, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////
SMFatalErrorBase::SMFatalErrorBase()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////////////////////
// Initialize()
// You can enter any time you like
SMStateBase *SMFatalErrorBase::Initialize()
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("FatalError") );	// RFC 330050
  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_FATALERROR, SCOT_LANGUAGE_PRIMARY));

  SMStateBase *retState = STATE(OperatorPasswordState)::Initialize();

  return retState;
}
 

////////////////////////////////////////////////////////
void SMFatalErrorBase::showScreen(bool passwordVerified)
{
    CString csWork;
	ps.SetPSText(PS_WORK1,ps.GetPSText(BTT_STORELOGIN,SCOT_LANGUAGE_PRIMARY));
    ps.SetButtonText(_T("ButtonStoreLogIn"),PS_WORK1);

    saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;


  if (passwordVerified)
    return;



     if (inputMode == PROMPTING_FOR_OPERATOR)
     {
        saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
		
		STATE(OperatorPasswordState)::ShowEnterID();  //393757
		ps.SetCMLeadthruText( TXT_ENTER_ID ); //Tar 395105
		ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
		ps.ShowCMTBText(csTBMessage2Scot);
		ps.ShowCMFrame( false );
        return;
     }
     else
     {  // Display initial  screen. 
        // NewUI
        ps.SetCMFrame(_T("FatalError"));
		ps.SetButtonText(_T("CMButton2"), BTT_STORELOGIN);
        // End NewUI
     }
 

  ps.CMButtonFlash(_T("CMButton1"),true);   
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  // End NewUI	
	
	ps.SetCMLeadthruText(LTT_WAITFORASSISTANCE);
    // End NewUI
	SAWLDBScannerDisable();			// Disable the scanner
	ps.ShowCMTBText(csTBMessage2Scot);	//thp
	csWork = ps.GetPSText(MSG_WAITFORASSISTANCE);
	csWork += _T("\n* ") + ps.GetPSText(LTT_FATALERROR);
    ps.Message12(csWork, true);
	trace(L7, _T("+ra.OnTransactionState, Itemization State"));
	ra.OnSetTransactionState(RA_STATE_WAIT_FOR_APPROVAL);
	trace(L7, _T("-ra.OnTransactionState"));
	ps.ShowCMFrame(false);
}

SMStateBase *SMFatalErrorBase::PSButtonHelp(void)
{
    return STATE_NULL;  // But you can never leave
}


///////////////////////////////////////
SMStateBase *SMFatalErrorBase::PSButton2(void) // Store Log In
{

  promptForOperator();
  return STATE_NULL;
}

///////////////////////////////////////
SMStateBase *SMFatalErrorBase::PSButton3(void) // exit help
{
    return STATE_NULL;
}

///////////////////////////////////////
SMStateBase *SMFatalErrorBase::PSButtonGoBack(void) // exit help
{

		switch(inputMode)
		{  
			case SHOW_WAIT_FOR_APPROVAL:
			case PROMPTING_FOR_PASSWORD:
				break;
			case PROMPTING_FOR_OPERATOR:
				inputMode = SHOW_WAIT_FOR_APPROVAL;
				if (co.GetfStateDualLanguage())
					ps.SetLanguage(m_customerLanguage);  //TAR 196315: Set back to customerLanuguage for dual language
				showScreen(false);
				return STATE_NULL;  
		}

   return PSButton2();
}

/////////////////////////////////////////
SMStateBase *SMFatalErrorBase::PSCancelKey(void)
{
  if (parentClassCurrentlyHandlingCancelKey())
    return STATE(OperatorPasswordState)::PSCancelKey();

  return STATE_NULL;
}

//////////////////////////////////////////
SMStateBase *SMFatalErrorBase::PSListLookup(CString cItemCode) // list or lookup selection
{
    return STATE_NULL;  // But you can never leave
}

/////////////////////////////////////////////
SMStateBase *SMFatalErrorBase::TimedOut(void)
{
    return STATE_NULL;  // But you can never leave
}

SMStateBase *SMFatalErrorBase::stateAfterPasswordVerify(void) 
{ 
	COleVariant v;
    fStateFreeze = true;    // But you can never leave
	ps.SetCMFrame(_T("SystemMessage"));
	ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	ps.SetCMLeadthruText(ps.GetPSText(LTT_FATALERROR, SCOT_LANGUAGE_PRIMARY));
	v = VARIANT_FALSE;
    m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), _T("SystemMessage"), UI::PROPERTYVISIBLE, v);
    m_pAppPSX->SetConfigProperty(_T("ButtonHelp"), _T("SystemMessage"), UI::PROPERTYVISIBLE, v);
    m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("SystemMessage"), UI::PROPERTYVISIBLE, v);
	
	ps.Message12(ps.GetPSText(MSG_FATALERROR,SCOT_LANGUAGE_PRIMARY), true);

    CLaneStateData laneStateData(m_pAppPSX);
    laneStateData.SendLaneStateData(_T(""), _T(""), false, true);

	ps.ShowCMFrame();

    return STATE_NULL;  // But you can never leave
}

//////////////////////////////////////////////////////////////
SMStateBase *SMFatalErrorBase::OnWtIncreaseNotAllowed() //tar 197857
{

    return STATE_NULL;  // But you can never leave
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMFatalErrorBase::OnWtDecrease() //tar 197857
{
    return STATE_NULL;  // But you can never leave
}

// the only reason for this method is to allow me to use create and assign
SMStateBase  *SMFatalErrorBase::Deliver()
{
  return this;
}
// + RFC 330050
void SMFatalErrorBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("FatalError") );	
}
// - RFC 330050
