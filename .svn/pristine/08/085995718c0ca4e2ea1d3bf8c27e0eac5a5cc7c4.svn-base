//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSignChargeBase.CPP
//
// TITLE: Class implementation for Sign Charge state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMSignCharge.h"       // This state
#include "DMProcedures.h"
#include "SMSmConfirmSignature.h"		// Tar 194454 RF020702
DLLEXPORT extern DMProcedures dm;   // Global DM object

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SignChargeBase")

IMPLEMENT_DYNCREATE(SMSignChargeBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SignCharge)


DEFINE_TIMESTAMP

////////////////////////////
SMSignChargeBase::SMSignChargeBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////
SMStateBase  *SMSignChargeBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SignCharge") );	// RFC 330050
	//Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(LTT_SIGNCHARGE, SCOT_LANGUAGE_PRIMARY));
	setAnchorState(BES_SIGNCHARGE);
    DMSayPhrase(SIGNRECEIPTWAITAPPROVAL);

	if (fDMReceiptError)
	{
		DMReceiptReprint();
	}

	if (fDMJournalError)
	{
		DMJournalReprint();
	}
	
  nTimeOut = co.getTimeOutHelp();

  dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                      DM_TRICOLORLIGHT_BLINK_1HZ,
                      DM_TRICOLORLIGHT_NORMALREQUEST);
	
  //awk.3.27.2 start TAR199004
  trace(L7, _T("+ra.OnRAPDataNeeded,Verify Signature\n"));
  // send a DataNeeded Approval
  cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("VerifySignature"));
  m_View = DATANEEDED_VERIFYSIGNATURE;
  CString csSecInfo;
  csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),
	  ps.GetPSText(LTT_SIGNCONFIRM, SCOT_LANGUAGE_PRIMARY), //Description
	  ps.GetPSText(RA_LANEBTT_VERIFYSIGNATURE, SCOT_LANGUAGE_PRIMARY), //Button
	  ps.GetPSText(RA_FIRSTLINE_VERIFYSIGNATURE, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""), _T("")); //Status line
  ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
  trace(L7, _T("-ra.OnRAPDataNeeded, Verify Signature\n"));
  //awk.3.27.2 end TAR199004

  return STATE(OperatorPasswordState)::Initialize();
}

///////////////////////////////////////
SMStateBase  *SMSignChargeBase::stateAfterPasswordVerify(void)
{
	RETURNSTATE(SmConfirmSignature);
}

////////////////////////////////////////////////////
void SMSignChargeBase::showScreen(bool passwordVerified)
{
  // This state does not need to reshow if the password
  // was verified, since we'll be leaving now
  if (passwordVerified)
    return;


     if (inputMode == PROMPTING_FOR_OPERATOR)
     {
		STATE(OperatorPasswordState)::ShowEnterID();  //SR93 Biometrics support
		ps.SetCMLeadthruText( TXT_ENTER_ID );
		ps.AllowInput( MAX_DIGITS_OPERATOR, false, false );
		
		ps.ShowAmountDueAmountPaid(lBalanceDue, lTotalPaid + nDMCashInserted);
		ps.ShowCMTBText(csTBMessage2Scot);	//thp
		
		ps.ShowCMFrame( false );           
		 
		return;
     }
     else
     {  // Display initial screen.
        ps.SetCMFrame(_T("SignCharge"));
		ps.SetButtonText(_T("CMButton1StoreLogIn"), BTT_STORELOGIN);
		SAWLDBScannerDisable();		// disable scanner if ini parameter set to 0 or not present

     }
 

  ps.SetCMLeadthruText(LTT_SIGNCHARGE);
  // End NewUI
  
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.Message12(MSG_SIGNRECEIPT, true);
  ps.ShowCMFrame(false);
}

/////////////////////////////////////
SMStateBase  *SMSignChargeBase::TimedOut(void)
{
  DMSayPhrase(SIGNRECEIPTWAITAPPROVAL);
  return STATE_NULL;
}

///////////////////////////////////////
SMStateBase *SMSignChargeBase::PSButton1(void) // Store Log In
{
  dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                      DM_TRICOLORLIGHT_BLINK_1HZ,


                      DM_TRICOLORLIGHT_NORMALREQUEST);

  promptForOperator();
  return STATE_NULL;
}

//////////////////////////////////////////////////////////////
SMStateBase *SMSignChargeBase::OnWtIncreaseNotAllowed() //tar 197857
{
	return VoidingWtIncrease(BES_SIGNCHARGE, BES_SIGNCHARGE);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSignChargeBase::OnWtDecrease() //tar 197857
{
	return ThresholdViolWtDecrease(BES_SIGNCHARGE, BES_SIGNCHARGE);
}
// + RFC 330050
void SMSignChargeBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SignCharge") );	
}
// - RFC 330050
