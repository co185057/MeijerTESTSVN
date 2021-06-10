//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecOperatorPWStateBase.cpp
//
// Description: Derived from SMOperatorPasswordState which derives from SMHelpModeState, 
//				This state handles the smartscale events in one location for it's derived
//				security violation states.
//
// Author:	Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                          // MS MFC always needed first
#ifndef _CPPUNIT
#include "Common.h"
#include "SMSystemMessage1.h"
#include "SMSystemMessage2.h"
#include "SMSystemMessage3.h"
#include "SMSystemMessage4.h"
#include "SMSystemMessage5.h"
#include "SMSystemMessage6.h"
#include "SMSecOperatorPWState.h"
#include "SMSecUnExpectedIncrease.h"
#include "SMSecUnexpectedDecrease.h"
#include "AuthenticationClientLibraryAPI.h"   //SR93 Biometrics support
#include "SMSecBagBoxNotValid.h"//SR085
#include "SMSmAssistMenu.h" //SSCOADK-7098

#include "DMProcedures.h"
DLLEXPORT extern DMProcedures dm;             // Global DM object

#else
#include "SMSecOperatorPWStateBase.h"
#endif //CPPUNIT


#define COMP_ID ID_SM                         // Base state component
#define T_ID _T("SecOperatorPWStateBase")

IMPLEMENT_DYNCREATE(SMSecOperatorPWStateBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////////////
SMSecOperatorPWStateBase::SMSecOperatorPWStateBase()
: inputMode(PROMPTING_FOR_OPERATOR),fInValidOperatorPassword(false), // TAR 278085
 m_HKStatus(IPSProcedures::HK_NORMAL),
 m_bShiftOn(false),
 m_bAltGrOn(false),
 m_bSymbolOn(false)
{
  IMPLEMENT_TIMESTAMP
  m_nCharLimit = 3;
  saMsgFuncPtr = ps.SecurityMessageModal;
}

//////////////////////////////////////////////////
SMStateBase * SMSecOperatorPWStateBase::Initialize(void)
{
#ifndef _CPPUNIT
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SecOperatorPWState") );	// RFC 330050
  if (co.IsLoginBypassOn() && IsLoginBypassAllowed())
  {
      inputMode = OPERATOR_PASSWORD_ACCEPTED;
      rp.SetOperator(co.GetAttendantID());
      return stateAfterPasswordVerify();
  }
  bool bWaitingOnWLDB = false;

  lSARedNeedingApproval = true;  // TAR 197015 Set alert flag for assist mode
  if (fInWLDBProcess)
  {
      bWaitingOnWLDB = true;
      fInWLDBProcess = false;
  }
//  DMScannerEnable();  // TAR 212825  Don't need enable the scanner at this time
  if (bWaitingOnWLDB)
  {
      fInWLDBProcess = true;
  }
  fInValidOperatorPassword = false;

      inputMode = SHOW_WAIT_FOR_APPROVAL;
      showScreen(false);
#endif //CPPUNIT
  return STATE_NULL;
}

//////////////////////////////////////////////////
void SMSecOperatorPWStateBase::UnInitialize()
{
#ifndef _CPPUNIT
  ps.SetLanguage(m_customerLanguage);
  SAWLDBScannerDisable();
	
  //+SR93 Biometrics support
  if (co.m_bOperationsUseFingerPrintDevice)
  {
	  FT_Abort();
  }
  //-SR93 Biometrics support

  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SecOperatorPWState") );	// RFC 330050
#endif //CPPUNIT
}

//////////////////////////////////////////////
SMStateBase * SMSecOperatorPWStateBase::PSEnterKey()
{
#ifndef _CPPUNIT
  // Enter key not valid after valid operator/password entered
  if (inputMode != OPERATOR_PASSWORD_ACCEPTED)
  {
    if (getOperatorPassword())
      return stateAfterPasswordVerify();
  }
#endif //CPPUNIT
  return STATE_NULL;
}

///////////////////////////////////////////////////////////////
SMStateBase * SMSecOperatorPWStateBase::PSNumericKey(const long lKey)
{
#ifndef _CPPUNIT
  // Don't echo numbers after operator/password accepted
  if (inputMode != OPERATOR_PASSWORD_ACCEPTED)
    PSEchoInput(lKey);
#endif //CPPUNIT
  return STATE_NULL;
}

//////////////////////////////////////////////
SMStateBase * SMSecOperatorPWStateBase::PSClearKey()
{
#ifndef _CPPUNIT
  // Clear key not valid after operator/password accepted
  if (inputMode != OPERATOR_PASSWORD_ACCEPTED)
  {
       PSClearAllInput(true);
  } 
#endif //CPPUNIT
  return STATE_NULL;
}


//TAR198849+
// SR93 Biometrics support -- invalid fp hang-up coz of no button handler
/////////////////////////////////////////////
SMStateBase * SMSecOperatorPWStateBase::PSButton1()
{
#ifndef _CPPUNIT
  // Cancel only valid while prompting for password.  Go back to
  // prompting for password (and original version of screen.)
  if (inputMode == PROMPTING_FOR_PASSWORD || fInValidOperatorPassword)
  {
	if(fInValidOperatorPassword)
	{
		fInValidOperatorPassword = false; // tar417029 since we are returning to EnterPassword, reset also the variable
	}
    promptForOperator();
  }
#endif //CPPUNIT
  // Either way, stay in same state
  return STATE_NULL;
}
//TAR198849-

/////////////////////////////////////////////
SMStateBase * SMSecOperatorPWStateBase::PSButton2()
{
#ifndef _CPPUNIT
  promptForOperator();
#endif //CPPUNIT
  return STATE_NULL;
}

///////////////////////////////////////////////
SMStateBase * SMSecOperatorPWStateBase::PSCancelKey()
{
	//TAR#117398	UITS1700 REVC Cancel should do nothing	LPM092399
	//DMSayPhrase("KeyPadValid");
	//return PSButton1();
	return STATE_NULL;

}

///////////////////////////////////////////////
SMStateBase * SMSecOperatorPWStateBase::PSButtonGoBack()
{
#ifndef _CPPUNIT
	//+TAR193505: At EnterID screen, "go back" should be back to SMSecUnExpectedIncreaseBase screen

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
					showScreen(false);
					return STATE_NULL;  
				}
		}
	//-TAR193505

	promptForOperator();
#endif //CPPUNIT
    return STATE_NULL;
}
/////////////////////////////////////////////////
void SMSecOperatorPWStateBase::promptForOperator()
{
  DMScannerEnable();
  //+TAR 444373
  CPSXREDRAWLOCK  csRedrawLock(_T("Display"), _T("SMSecOperatorPWStateBase::promptForOperator"));
  //-TAR 444373
  if (co.GetfStateDualLanguage())
  {
          ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);

  }

  inputMode = PROMPTING_FOR_OPERATOR;

  // grey out help button
  showScreen(false);
//  ps.CMButton(IDC_BUTTONHELP,BTT_HELP,false);
  //ps.AllowInput(MAX_DIGITS_OPERATOR,false,false);
  m_nCharLimit = co.GetnOperationsOperatorIDMaxDigit();
  ps.AllowInput(m_nCharLimit,false,false);


}

/////////////////////////////////////////////////
void SMSecOperatorPWStateBase::promptForPassword()
{
  DMScannerDisable();
  if (co.GetfStateDualLanguage())
      ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);

  inputMode = PROMPTING_FOR_PASSWORD;
  

		saMsgFuncPtr = ps.SecurityMessageNonmodal;
        if(!co.GetfOperationsOperatorNumericKeyPad())
        {
            ShowAlphaNumericKeypad();
            m_pAppPSX->SetConfigProperty(_T("InstructionScreenTitle"), m_csFrameName, UI::PROPERTYTEXT, ps.GetPSText(TXT_ENTER_PASSWORD));
        }
        else
        {
            ps.SetCMFrame( _T("EnterID") );	// EnterPassword //
            ps.SetCMLeadthruText( TXT_ENTER_PASSWORD );
        }

		ps.Message12( MSG_ENTERPASSWORD_PRESSENTER, false );

		// ++ NextGenUI: Fix issue on pressing Go Back will not go back to EnterID context
		m_pAppPSX->SetTransactionVariable( _T("NextGenData"), COleVariant(_T("EnterPassword")) );



 
	ps.ShowCMTBText( csTBMessage2Scot );	//thp
	//ps.AllowInput( MAX_DIGITS_PASSWORD, true, false );
    m_nCharLimit = co.GetnOperationsOperatorPWMaxDigit();
    ps.AllowInput(m_nCharLimit, true, false );

		
	ps.ShowCMFrame( false );
}

/////////////////////////////////////////////////////
bool SMSecOperatorPWStateBase::getOperatorPassword()
{
#ifndef _CPPUNIT
	//+SR93 Biometrics support  
	if( co.m_bOperationsUseFingerPrintDevice && g_bFPTouched )
	{
		if ( g_bFPValid )  
		{
			inputMode = PROMPTING_FOR_PASSWORD;
			g_bFPValid = false;
		}  
		else
		{
			displayInvalidMessage();
			g_bFPTouched = false; //TAR395787
			return false;
		}
	}
	//-SR93 Biometrics support

  if (inputMode == PROMPTING_FOR_OPERATOR)
  {
    ps.GetInput(csOperator);
	if (csOperator != _T(""))
    // Operator entered, now prompt for password
    {
		//+SR93 Biometrics support
		if (co.m_bOperationsUseFingerPrintDevice )
		{
			FT_Abort();
			g_csOperator = csOperator;
		}
		//-SR93 Biometrics support

		promptForPassword();
    }
    else
    // Nothing entered - leave and continue to prompt for operator
	  DMSayPhrase(KEYPADINVALID);
      return false;
  }

  else if (inputMode == PROMPTING_FOR_PASSWORD)
  {
    ps.GetInput(csPassword);
    // If password entered, validate operator and password
	//+SR93 Biometrics support	
	if (csPassword != _T("") || g_csPassword != _T(""))
	{
		if (g_csPassword == _T("")) // keyed in
		{
			g_csOperator = csOperator;
			g_csPassword = csPassword;
		}
		if (csPassword == _T("")) //finger print login
		{
			csOperator = g_csOperator;
			csPassword = g_csPassword;
		}

      // read 4690 operator file
		if (TBValidateOperator(g_csOperator, g_csPassword) == 1)  // ... operator ID, ASCII 1-8 digits	// TAR 409017
		//-SR93 Biometrics support      
		{
			//+ TAR [384711]: note: Added to reset inputed operator ID and password
			g_csOperator = _T("");
			g_csPassword = _T("");
			//- TAR [384711]
	
			//+SR93 Biometrics
			if (co.m_bOperationsUseFingerPrintDevice && g_bFPTouched) //TAR 395787
			{
				//+TAR394645 
				g_bFPTouched = false; //TAR395787
				_TCHAR  szSoundPath[_MAX_PATH] = _T("%APP_DRIVE%\\scot\\sound");
				_TCHAR szPath[_MAX_PATH];
			
				_tmakepath(szPath,NULL,szSoundPath, ps.GetSoundName(VALIDBIOMETRIC), _T("WAV"));
				CString csSoundFile = szPath;
				GET_PARTITION_PATH(csSoundFile , csSoundFile.GetBuffer(_MAX_PATH));
				csSoundFile.ReleaseBuffer();
				ps.GetPSXObject()->PlaySound( csSoundFile );
				//-TAR394645
			}
			//-SR93 Biometrics

			rp.SetInterventionEndInProgress(true);  // TAR 311902
	        // Valid operator/password, can leave prompting mode
			fInValidOperatorPassword = false;
		    inputMode = OPERATOR_PASSWORD_ACCEPTED;
            if (dm.IsOpen())     //TAR#125161 don't use dm if it's not open b/c of an error
            {
				// tar 211746 don't turn off red if there are other red interventions waiting
				// because turning off red can let any pending virtual yellow be turned on which
				// means both red and yellow will be on if we immediately turned red back on
				// also will help prevent extra pages that can occur when turning it off and then on
				if(!AreAnyRedApprovalsPending())
				{
					// (*) SR898 ====================
                    DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                                       DM_TRICOLORLIGHT_OFF,
                                       DM_TRICOLORLIGHT_NORMALREQUEST);
				}
            }

	        // Reshow screen, noting that password has been verified
	        showScreen(true);
				return true;
		}
		else  // No invalid
		{
              displayInvalidMessage();
			  g_bFPTouched = false; //397387
			return false;
		}
    }

    // If we haven't returned yet, there was no password or it was invalid.
    // Start over and ask for operator again
    DMSayPhrase(KEYPADINVALID);
    promptForOperator();
    return false;

  } // else if prompting for password
#endif //CPPUNIT
  return false;
}


////////////////////////////////////////////////////////////////////////////
// If an operator/password state is going to issue a system message, and the
// password has already been verified, there is no need to issue a system
// message version that requires operator/password to see.
////////////////////////////////////////////////////////////////////////////
SMStateBase * SMSecOperatorPWStateBase::createLevel1Message(LPCTSTR szMsg,
		                                                  PSTEXT pstMsg,
		                                                  PSTEXT prompt,
		                                                  long   value,
		                                                  long   device,
		                                                  bool   checkHealth,
														  bool displayHTML) //SR672
{
#ifndef _CPPUNIT
  if (inputMode == OPERATOR_PASSWORD_ACCEPTED)
  {
    CREATE_AND_DISPENSE(SystemMessage1)(szMsg,pstMsg,prompt,value,device,checkHealth, displayHTML);
  }
  else
  {
    CREATE_AND_DISPENSE(SystemMessage4)(szMsg,pstMsg,prompt,value,device,checkHealth, displayHTML);
  }
#else
  return STATE_NULL;
#endif //CPPUNIT
}

///////////////////////////////////////////////////////////
SMStateBase * SMSecOperatorPWStateBase::createLevel2Message(LPCTSTR szMsg,
		                                                  PSTEXT pstMsg,
		                                                  PSTEXT prompt,
		                                                  long   value,
		                                                  long   device,
		                                                  bool   checkHealth,
														  bool displayHTML) //SR672
{
#ifndef _CPPUNIT
  if (inputMode == OPERATOR_PASSWORD_ACCEPTED)
  {
    CREATE_AND_DISPENSE(SystemMessage2)(szMsg,pstMsg,prompt,value,device,checkHealth, displayHTML);
  }
  else
  {
    CREATE_AND_DISPENSE(SystemMessage5)(szMsg,pstMsg,prompt,value,device,checkHealth, displayHTML);
  }
#else
  return STATE_NULL;
#endif //CPPUNIT
}

///////////////////////////////////////////////////////////
SMStateBase * SMSecOperatorPWStateBase::createLevel3Message(LPCTSTR szMsg,
		                                                  PSTEXT pstMsg,
		                                                  PSTEXT prompt,
		                                                  long   value,
		                                                  long   device,
		                                                  bool   checkHealth,
														  bool displayHTML) //SR672
{
#ifndef _CPPUNIT
  if (inputMode == OPERATOR_PASSWORD_ACCEPTED)
  {
    CREATE_AND_DISPENSE(SystemMessage3)(szMsg,pstMsg,prompt,value,device,checkHealth, displayHTML);
  }
  else
  {
    CREATE_AND_DISPENSE(SystemMessage6)(szMsg,pstMsg,prompt,value,device,checkHealth, displayHTML);
  }
#else
  return STATE_NULL;
#endif //CPPUNIT
}

///////////////////////////////////////////////////////////
SMStateBase * SMSecOperatorPWStateBase::DMScanner(void)
{
#ifndef _CPPUNIT
	//+SR93 Biometrics support
	if (co.m_bOperationsUseFingerPrintDevice)  
	{
	  FT_Abort();
	  g_bFPTouched = false; //397387
	}
	//-SR93 Biometrics support

	if (   (csOperator != _T(""))  // TAR 283345 - Skip validation if 
		&& (csPassword != _T(""))  // TAR 283345 -			null data
		&& (bValidScannedOperator))
	{
		rp.SetInterventionEndInProgress(true);  // TAR 311902
		// Valid operator/password, can leave prompting mode
		fInValidOperatorPassword = false; // TAR 278085
        inputMode = OPERATOR_PASSWORD_ACCEPTED;
        
  		// (*) SR898 ====================
        DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                           DM_TRICOLORLIGHT_OFF,
                           DM_TRICOLORLIGHT_NORMALREQUEST);
                           
        // Reshow screen, noting that password has been verified
  	    showScreen(true);
	    return stateAfterPasswordVerify();
    }
    else
    {
		DMSayPhrase(KEYPADINVALID);
        return displayInvalidMessage();     // TAR 253541
  	}
#else
  return STATE_NULL;
#endif //CPPUNIT
}

///////////////////////////////////////////////////////////
SMStateBase *SMSecOperatorPWStateBase::displayInvalidMessage(void)
{
#ifndef _CPPUNIT
	//+ TAR [384711]: note: Added to reset inputed operator ID and password
	g_csOperator = _T("");
    g_csPassword = _T("");
	//- TAR [384711]
	
    saMsgFuncPtr = ps.SecurityMessageModal;
    fInValidOperatorPassword = true;
	//+SR93 Biometrics support	
	if (co.m_bOperationsUseFingerPrintDevice && inputMode != PROMPTING_FOR_PASSWORD && g_bFPTouched )  //Tar 396905
	{
		g_bFPTouched = false;
		CString csCurContext = _T("OperatorPasswordStateInvalidPasswordWithFP");
		ps.SetCMFrame(csCurContext); //Tar 394751
		
		if (!g_bBiometricACLDevice)
		{
			ps.Message12(MSG_FINGERPRINT_DEVICE_ERROR, true);
		}
		else if (!g_bBiometricACLServer)
		{
			ps.Message12(MSG_FINGERPRINT_SERVER_ERROR,true);
		}
		else
		{
			ps.Message12(MSG_INVALID_FINGERPRINT, true);
		}
		ps.SetCMLeadthruText(LTT_INVALID_FINGERPRINT);
		//Tar 396903
		COleVariant vValue;
		vValue = VARIANT_FALSE;
		m_pAppPSX->SetConfigProperty(_T("Title2Area"), csCurContext, UI::PROPERTYVISIBLE, vValue);
		ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid); //TAR 435471
		
	}
    else
	{
		ps.SetCMFrame(_T("OperatorPasswordStateInvalidPassword"));		
		ps.SetCMLeadthruText(LTT_INCORRECTID);
		ps.Message12(MSG_OPERATORPASSWORDINVALID, true);
		
	}
  	
	ps.CMButtonFlash(_T("CMButton1Med"),true);
#endif //CPPUNIT		
    ps.ButtonState(_T("ButtonGoBack"), false, false);
	ps.ShowCMTBText(csTBMessage2Scot);	 
	ps.ShowCMFrame(false);
	//-SR93 Biometrics support
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecOperatorPWStateBase::OnWtIncreaseNotAllowed()	 
{
#ifndef _CPPUNIT
	// send to RAP but stay in same state
	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("UnexpectedIncrease"));
	m_View = DATANEEDED_UNEXPECTEDINCREASE;
	CString csInstruction, csDesc;
	csDesc.Format(ps.GetPSText(RA_UNEXPECTEDINCREASEINFO, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits);
	// RA_WEIGHTINCREASEINFO is a string that says "Actual weight: %s %s".  This name should be changed to RA_WEIGHTCHANGEINFO LPM022002
	csInstruction.Format(ps.GetPSText(RA_WEIGHTINCREASEINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTINCREASE, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits, _T(""));
	ra.OnRAPDataNeeded(csInstruction, cDataEntry.ToString(), m_View);
	DMSaySecurity( SEC_NONE, _T("blank.wav") );

	CREATE_AND_DISPENSE(SecUnExpectedIncrease)(getAnchorState(), getAnchorState());
#else
  return STATE_NULL;
#endif //CPPUNIT
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecOperatorPWStateBase::OnWtDecrease()
{
#ifndef _CPPUNIT
	// send to RAP but stay in same state
	//The text here is just the name of the section in scotdataentry.ini.  Don't need to be in SCOTMSGE.DAT
	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("UnexpectedDecrease"));
	m_View = DATANEEDED_UNEXPECTEDDECREASE;
	CString csInstruction, csDesc;
	// RA_WEIGHTINCREASEINFO is a string that says "Actual weight: %s %s".  This name should be changed to RA_WEIGHTCHANGEINFO LPM022002
	csDesc.Format(ps.GetPSText(RA_UNEXPECTEDINCREASEINFO, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits);
	csInstruction.Format(ps.GetPSText(RA_WEIGHTDECREASEINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTDECREASE, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits, _T(""));
	ra.OnRAPDataNeeded(csInstruction, cDataEntry.ToString(), m_View);

	DMSaySecurity( SEC_NONE, _T("blank.wav") );

	if(m_PrevState == BES_PICKINGUPITEMS)
	{
		//tar 197339
		if(	g_lPreLRW >= g_lLRW )
		{
			g_lPreLRW = g_lLRW;
			return createState(m_PrevState);
		}
		else
			return OnWtIncreaseNotAllowed();
	}

	CREATE_AND_DISPENSE(SecUnexpectedDecrease)(m_PrevState, m_NextState);
#else
  return STATE_NULL;
#endif //CPPUNIT
}
SMStateBase *SMSecOperatorPWStateBase::TareWeightedItem(void)
{
#ifndef _CPPUNIT
	// ----------------------------------------------
	// 1. Is the S/S Wt Increase = (BS wt Increase - BOX TARE)
	// ----------------------------------------------
	SAClearSecurityCondition();					// tell RAP the person fixed their own error
	//218558 set scanner flag to re-enabled only in state that isn't handling EAS automatically
	trace(L2, _T("bEASReEnableScanner is set to true."));
	bEASReEnableScanner = true;
//	io.lWeightEntered = io.lWeightEntered - co.nOperationsTareWtToIgnore1;	// subtract the Tare in case ever want to update here
    OverrideSecurityException(_T("None"), KEY_EXCEPTION_OPERATION_CLEAR);
//	return setAndCreateAnchorState(m_PrevState);	// go to next state = ScanAndBag
#endif //CPPUNIT
    return STATE_NULL; //SSCOP-483
}
SMStateBase *SMSecOperatorPWStateBase::TareOnly(void)
{
#ifndef _CPPUNIT
	// ----------------------------------------------
	// 2. Is the BS wt Increase = BOX TARE
	// ----------------------------------------------
	bool bMisMatchWt = false;

	if(secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_MISMATCHED)
	{
		bMisMatchWt = true;
	}
	ClearSecurityViolationLight();
	ra.OnNoRAPDataNeeded();						// tell RAP the person fixed their own error
	//Tar 200814 BZ 04/02/02 Pressing Bag/Box after placing item on scale at Attract should go to S&B
    OverrideSecurityException(_T("None"), KEY_EXCEPTION_OPERATION_CLEAR);
	if (getAnchorState() == BES_START)
	{
		// enable the sensormatic EAS deactivator while in a transaction
		dm.EASDeactivatorEnable( EASEnable );
		SASendSCOTInput( ITEMIZATION );
		return TBStart();
	}
    	//TAR403945+
	//matched event processed right before TareOnly
	else if ((m_PrevState == BES_BAGANDEAS) && (getAnchorState() == BES_SCANANDBAG) && bMisMatchWt)
	{
		return createState(BES_SCANANDBAG);
	}
	else if ((m_PrevState == BES_BAGANDEAS) && (getAnchorState() == BES_TRANSPORTITEM ) && bMisMatchWt)
	{	
		return createState(BES_TRANSPORTITEM);
	}
	//TAR403945-
	else
	{
		//sscoadk-7442+
		CString csCurrentStateName = this->GetRuntimeClass()->m_lpszClassName;
		  if ( (csCurrentStateName.Find( _T("SecMisMatchWeight") ) != -1) )        
		{
			return createState(m_PrevState);	// delta matched one of the Tares so return to BagAndEAS
		}
		else
		{
		  return STATE_NULL; //wait on SM's reply for OverrideSecurityException  
		}
		//sscoadk-7442-
	}
#else
  return STATE_NULL;
#endif //CPPUNIT
}
SMStateBase *SMSecOperatorPWStateBase::TareWLDBItem(void)
{
#ifndef _CPPUNIT

	// ----------------------------------------------
	// 3. Is the BS wt Increase = (WLDB + BOX TARE)
	// ----------------------------------------------
	SAClearSecurityCondition();					// tell RAP the person fixed their own error
	//218558 set scanner flag to re-enabled only in state that isn't handling EAS automatically
	trace(L2, _T("bEASReEnableScanner is set to true."));
	bEASReEnableScanner = true;
    OverrideSecurityException(_T("None"), KEY_EXCEPTION_OPERATION_CLEAR);
//	g_bOkToUpdate = true;					// ok to update because came from BagAndEAS
	//return setAndCreateAnchorState(m_NextState);	// go to next state = ScanAndBag
	//return createState(BES_SCANANDBAG);
#endif //CPPUNIT
	return STATE_NULL; //SSCOP-2330
}
SMStateBase *SMSecOperatorPWStateBase::TareNone(void)
{
#ifndef _CPPUNIT
	// ----------------------------------------------
	// 4. Otherwise...
	// ----------------------------------------------
	/*SR085 - Removed for SR085 as now no longer stay on the same screen but move onto SMSecBagBoxNotValid
	// No TAREs established in the SCOTOpts file or no matches, disable button and stay here
	PSTEXT nBtt1;
	if(co.fOperationsAllowContainerBtn)
	{
   		nBtt1 = BTT_BAGBOX;
	}

	trace(L3, _T("Bag/Box btn pressed, No TAREs matched weight difference.  TAREs might be set to zero in SCOTOpts file"));
	ps.CMButton(_T("CMButton1Med"), nBtt1, false);
	m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), ps.GetCurrentContext(), UI::PROPERTYSTATE, ( long ) UI::Disabled );       // TAR326941
	trace(L6, _T("-SMSecUnExpectedIncreaseBase Bag/Box btn pressed"));
	return STATE_NULL;
	*/

	//SR085 +
	trace(L3, _T("Bag/Box btn pressed, No TAREs matched weight difference.  TAREs might be set to zero in SCOTOpts file"));
    trace(L6, _T("-SMSecUnExpectedIncreaseBase Bag/Box btn pressed"));

	//417990 Move this logic to SMSecBagBoxNotValidBase::OnRAPClearSecurityBtn
	//We only need to set anchor state when we approve the invalid bag intervention.
//	if(m_PrevState==BES_BAGANDEAS)//anchoring to BagAndEAS will cause a problem as SM will move onto S&B (or some other prev state) on clearing this exception
//		setAnchorState(m_NextState);
	if((m_PrevState != BES_STATERETURN) && (m_PrevState != BES_WAITFORAPPROVAL)) //Tar 412129
		setAnchorState(m_PrevState);

	CString csCurrentStateName= this->GetRuntimeClass()->m_lpszClassName;
	if ( (csCurrentStateName.Find(_T("SecMisMatchWeight")) != -1) || ((m_PrevState==BES_BAGANDEAS) && (csCurrentStateName.Find(_T("SecUnExpectedIncrease")) != -1))) 
	{
		CREATE_AND_DISPENSE(SecBagBoxNotValid)(BES_BAGANDEAS);
	}
	else
	{
		return createState(BES_SECBAGBOXNOTVALID);
	}
#else
  return STATE_NULL;
#endif //CPPUNIT
	//SR085 -
}

//SR085 +
SMStateBase *SMSecOperatorPWStateBase::TareOnlyAtAttract(void)
{
#ifndef _CPPUNIT
	ClearSecurityViolationLight();
	ra.OnNoRAPDataNeeded();// tell RAP the person fixed their own error
	OverrideSecurityException(_T("None"), KEY_EXCEPTION_OPERATION_CLEAR);
	//return createAnchorState();
#endif //CPPUNIT
    return STATE_NULL;

}
//SR085 -

/////////////////////////////////////////////////////////////////
//+SR93 Biometrics
void SMSecOperatorPWStateBase::ShowEnterID()
{
	g_csOperator = _T("");
	g_csPassword = _T("");
	ps.Echo(PS_BLANK); //Tar 396211
    if(!co.GetfOperationsOperatorNumericKeyPad())
    {
        ShowAlphaNumericKeypad();
        m_pAppPSX->SetConfigProperty(_T("InstructionScreenTitle"), m_csFrameName, UI::PROPERTYTEXT, ps.GetPSText(TXT_ENTER_ID));
    }
    else
    {
        ps.SetCMFrame( _T("EnterID") );
        ps.SetCMLeadthruText( TXT_ENTER_ID ); //395105
    }
	// ++ NextGenUI: Fix issue on pressing Go Back will not go back to EnterID context
	m_pAppPSX->SetTransactionVariable( _T("NextGenData"), COleVariant(_T("EnterID")) );

	if (co.GetbOperationsUseFingerPrintDevice())
	{
		if (g_bBiometricACLServer && g_bBiometricACLDevice)
		{
#ifndef _CPPUNIT
			//Tar 394580
			BOOL bFTGetFP=FALSE;
			try
			{
				bFTGetFP= FT_GetFingerprint();
				to.Trace(L6, _T("FT_GetFingerprint() with ret= %d"), bFTGetFP );
			}
			catch(CException* e)  
			{
				_TCHAR szWork[255];
				memset(szWork, 0, 255);
				e->GetErrorMessage( szWork, sizeof( szWork ) );
				to.Trace(L6, _T("Exception in FT_GetFingerprint(): %s"), szWork);
				bFTGetFP = FALSE;
				e->Delete();
			}
			catch(...)
			{
				long le = GetLastError();   
				to.Trace(L6, _T("Exception in FT_GetFingerprint(): %d"), le);
				bFTGetFP = FALSE;

			}
			if (bFTGetFP)
				ps.Message12( MSG_FINGERPRINT_LOGIN, false ); 
			else
				ps.Message12( MSG_ENTERID_PRESSENTER, false );
#endif  //_CPPUNIT
		}
		else
		{			
			to.Trace(L6, _T("Fingerprint ERROR: variable is set to false. g_bBiometricACLServer= %d, g_bBiometricACLDevice=%d"),g_bBiometricACLServer, g_bBiometricACLDevice );
			to.Trace(L6, _T("Check fingerprint device and ACL server is connected."));
			ps.Message12( MSG_ENTERID_PRESSENTER, false );
		}
	}
	else
	{			
		ps.Message12( MSG_ENTERID_PRESSENTER, false );
	}		
}
//-SR93 Biometrics

//************************************************************************ 
//
// Function:		PSCharKey
// Purpose:			This function handles the button clicks from the alphanumeric keys
// Parameters:    [in] TCHAR c - The character on the alphanumeric key that has been clicked
// Returns:	      SMStateBase  * 
// Calls:
// Side Effects:
// Notes:           
//
//************************************************************************
SMStateBase  *SMSecOperatorPWStateBase::PSCharKey(TCHAR c)
{
   ps.EchoInput(c, true);
   ps.GetInput(csItemSent);
   int nLen=csItemSent.GetLength();
//   if( nLen == m_nCharLimit)
   {
      //return PSEnterKey();
   }
   return STATE_NULL;//remain in same state
}

//************************************************************************ 
//
// Function:		PSHotKey
// Purpose:			This function handles the button clicks from the hotkeys control
// Parameters:    [in] CString & csHotKey - The name of the hot key that has been clicked
// Returns:	      SMStateBase  * 
// Calls:         PopulateHKSensitiveControls
// Side Effects:
// Notes:           
//
//************************************************************************
SMStateBase  *SMSecOperatorPWStateBase::PSHotKey(CString & csHotKey) 
{
  IPSProcedures::HKStatus newStatus=IPSProcedures::HK_NORMAL;
  if(csHotKey==_T("HKAcute"))
  {
    newStatus=IPSProcedures::HK_ACUTEON;
  }
  else if(csHotKey==_T("HKGrave"))
  {
    newStatus=IPSProcedures::HK_GRAVEON;
  }
  else if(csHotKey==_T("HKCircumflex"))
  {
    newStatus=IPSProcedures::HK_CIRCUMFLEXON;
  }
  else if(csHotKey==_T("HKUmlaut"))
  {
    newStatus=IPSProcedures::HK_UMLAUTON;
  }
  else if(csHotKey==_T("HKTilde"))
  {
    newStatus=IPSProcedures::HK_TILDEON;
  }

  COleVariant vIndex(0L), vReturn;
  
  if(m_HKStatus==newStatus)
  {
    m_HKStatus=IPSProcedures::HK_NORMAL;
  }
  else
  {
    m_HKStatus=newStatus;
  }


   PSXRC rc=PSX_SUCCESS;
   rc=m_pAppPSX->GetControlProperty( _T("HotKeys"), UI::PROPERTYBUTTONCOUNT, vIndex );
   if(rc==PSX_SUCCESS)
   {
      long nButtonCount = vIndex.lVal;
      if(nButtonCount)
      {
         m_pAppPSX->SetConfigProperty(_T("HotKeys"), m_csFrameName, UI::PROPERTYREDRAW, VARIANT_FALSE);
         if(m_HKStatus == IPSProcedures::HK_NORMAL)
         {
            for( long i = 0L; i < nButtonCount; i++ )
            {
               COleVariant pvParams[2];
               pvParams[0]=i;
               pvParams[1]=(long) UI::Normal;
               m_pAppPSX->SendCommand( _T("HotKeys"), UI::COMMANDSETBUTTONSTATE, vReturn, 2, pvParams );
            }
         }
         m_pAppPSX->SetConfigProperty(_T("HotKeys"), m_csFrameName, UI::PROPERTYREDRAW, VARIANT_TRUE);
      }

      ps.PopulateHKSensitiveControls(m_csFrameName, m_HKStatus, m_bShiftOn, m_bAltGrOn, m_bSymbolOn);
   }


   return STATE_NULL;//remain in same state

}

SMStateBase  *SMSecOperatorPWStateBase::PSShiftKey(void)
{
  
  m_bShiftOn=!m_bShiftOn;
  SetAlphaShiftState(m_csFrameName, m_bShiftOn);

  ps.PopulateHKSensitiveControls(m_csFrameName, m_HKStatus, m_bShiftOn, m_bAltGrOn, m_bSymbolOn);

  return STATE_NULL;
}

/////////////////////////////////////
SMStateBase  *SMSecOperatorPWStateBase::PSAltGrKey(void)
{
  
  m_bAltGrOn=!m_bAltGrOn;

  ps.PopulateHKSensitiveControls(m_csFrameName, m_HKStatus, m_bShiftOn, m_bAltGrOn, m_bSymbolOn);

  return STATE_NULL;
}

SMStateBase  *SMSecOperatorPWStateBase::PSSymbolKey(void)
{
  
  m_bSymbolOn=!m_bSymbolOn;
  if(m_bSymbolOn)
  {
      m_bShiftOn = false;
  }

  ps.PopulateHKSensitiveControls(m_csFrameName, m_HKStatus, m_bShiftOn, m_bAltGrOn, m_bSymbolOn);
  ps.SetShiftAndSymbolState(m_csFrameName, m_bSymbolOn);

  return STATE_NULL;
}

SMStateBase  *SMSecOperatorPWStateBase::PSSpaceKey(void)// 
{
  ps.EchoInput((_TCHAR)(0x20), true);
  return STATE_NULL;
}


void SMSecOperatorPWStateBase::ShowAlphaNumericKeypad(void)
{
   m_csFrameName = ps.GetOperatorKeyboardContextOfCurrentLang();

   ps.SetCMFrame(m_csFrameName);

   m_HKStatus=IPSProcedures::HK_NORMAL;

   m_bShiftOn=false;
   m_bAltGrOn=false;
   m_bSymbolOn=false;

   SetAlphaShiftState(m_csFrameName, m_bShiftOn);


   ps.PopulateHotKeys(m_csFrameName);
   ps.PopulateHKSensitiveControls(m_csFrameName, m_HKStatus, m_bShiftOn, m_bAltGrOn, m_bSymbolOn);
   ps.SetShiftAndSymbolState(m_csFrameName, m_bSymbolOn);
}

SMStateBase *SMSecOperatorPWStateBase::DMScannerEncrypted(void) //SSCOADK-7098
{
    if (co.IsInAssistMenus() || fStateAssist)
    {
        SMSmAssistMenu::SetAssistMessage(LTT_INVALIDBARCODESCAN, SETITEMASIDECONTINUEWITHPURCHASES);
	    bEASReEnableScanner = true;
        RETURNSTATE(SmAssistMenu)
    } 
    else
    {
        return displayInvalidMessage();
    }
}