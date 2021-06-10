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


#include "DMProcedures.h"
DLLEXPORT extern DMProcedures dm;             // Global DM object

#define COMP_ID ID_SM                         // Base state component
#define T_ID _T("SecOperatorPWStateBase")

IMPLEMENT_DYNCREATE(SMSecOperatorPWStateBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////////////
SMSecOperatorPWStateBase::SMSecOperatorPWStateBase()
: inputMode(PROMPTING_FOR_OPERATOR),fInValidOperatorPassword(false) // TAR 278085
{
  IMPLEMENT_TIMESTAMP
  saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
}

//////////////////////////////////////////////////
SMStateBase * SMSecOperatorPWStateBase::Initialize(void)
{
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
  return STATE_NULL;
}

//////////////////////////////////////////////////
void SMSecOperatorPWStateBase::UnInitialize()
{
  ps.SetLanguage(m_customerLanguage);
  SAWLDBScannerDisable();
	
  //+SR93 Biometrics support
  if (co.m_bOperationsUseFingerPrintDevice)
  {
	  FT_Abort();
  }
  //-SR93 Biometrics support

  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SecOperatorPWState") );	// RFC 330050
}

//////////////////////////////////////////////
SMStateBase * SMSecOperatorPWStateBase::PSEnterKey()
{

  // Enter key not valid after valid operator/password entered
  if (inputMode != OPERATOR_PASSWORD_ACCEPTED)
  {
    if (getOperatorPassword())
      return stateAfterPasswordVerify();
  }

  return STATE_NULL;
}

///////////////////////////////////////////////////////////////
SMStateBase * SMSecOperatorPWStateBase::PSNumericKey(const long lKey)
{
  // Don't echo numbers after operator/password accepted
  if (inputMode != OPERATOR_PASSWORD_ACCEPTED)
    PSEchoInput(lKey);

  return STATE_NULL;
}

//////////////////////////////////////////////
SMStateBase * SMSecOperatorPWStateBase::PSClearKey()
{
  // Clear key not valid after operator/password accepted
  if (inputMode != OPERATOR_PASSWORD_ACCEPTED)
  {
       PSClearAllInput(true);
  } 

  return STATE_NULL;
}


//TAR198849+
// SR93 Biometrics support -- invalid fp hang-up coz of no button handler
/////////////////////////////////////////////
SMStateBase * SMSecOperatorPWStateBase::PSButton1()
{
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

  // Either way, stay in same state
  return STATE_NULL;
}
//TAR198849-

/////////////////////////////////////////////
SMStateBase * SMSecOperatorPWStateBase::PSButton2()
{
  promptForOperator();
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
    return STATE_NULL;
}
/////////////////////////////////////////////////
void SMSecOperatorPWStateBase::promptForOperator()
{
  DMScannerEnable();
  if (co.fStateDualLanguage)
  {
          ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);

  }

  inputMode = PROMPTING_FOR_OPERATOR;

  // grey out help button
  showScreen(false);
//  ps.CMButton(IDC_BUTTONHELP,BTT_HELP,false);
  ps.AllowInput(MAX_DIGITS_OPERATOR,false,false);



}

/////////////////////////////////////////////////
void SMSecOperatorPWStateBase::promptForPassword()
{
 if (co.fStateDualLanguage)
      ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);

  inputMode = PROMPTING_FOR_PASSWORD;
  

		saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
		ps.SetCMFrame( _T("EnterID") );	// EnterPassword //
		ps.SetCMLeadthruText( TXT_ENTER_PASSWORD );
		ps.Message12( MSG_ENTERPASSWORD_PRESSENTER, false );





 
	ps.ShowCMTBText( csTBMessage2Scot );	//thp
	ps.AllowInput( MAX_DIGITS_PASSWORD, true, false );
		
	ps.ShowCMFrame( false );
}

/////////////////////////////////////////////////////
bool SMSecOperatorPWStateBase::getOperatorPassword()
{
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
			fInValidOperatorPassword = true;
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
					dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
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
			fInValidOperatorPassword = true;
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
		                                                  bool   checkHealth )
{
  if (inputMode == OPERATOR_PASSWORD_ACCEPTED)
  {
    CREATE_AND_DISPENSE(SystemMessage1)(szMsg,pstMsg,prompt,value,device,checkHealth);
  }
  else
  {
    CREATE_AND_DISPENSE(SystemMessage4)(szMsg,pstMsg,prompt,value,device,checkHealth);
  }
}

///////////////////////////////////////////////////////////
SMStateBase * SMSecOperatorPWStateBase::createLevel2Message(LPCTSTR szMsg,
		                                                  PSTEXT pstMsg,
		                                                  PSTEXT prompt,
		                                                  long   value,
		                                                  long   device,
		                                                  bool   checkHealth )
{
  if (inputMode == OPERATOR_PASSWORD_ACCEPTED)
  {
    CREATE_AND_DISPENSE(SystemMessage2)(szMsg,pstMsg,prompt,value,device,checkHealth);
  }
  else
  {
    CREATE_AND_DISPENSE(SystemMessage5)(szMsg,pstMsg,prompt,value,device,checkHealth);
  }
}

///////////////////////////////////////////////////////////
SMStateBase * SMSecOperatorPWStateBase::createLevel3Message(LPCTSTR szMsg,
		                                                  PSTEXT pstMsg,
		                                                  PSTEXT prompt,
		                                                  long   value,
		                                                  long   device,
		                                                  bool   checkHealth )
{
  if (inputMode == OPERATOR_PASSWORD_ACCEPTED)
  {
    CREATE_AND_DISPENSE(SystemMessage3)(szMsg,pstMsg,prompt,value,device,checkHealth);
  }
  else
  {
    CREATE_AND_DISPENSE(SystemMessage6)(szMsg,pstMsg,prompt,value,device,checkHealth);
  }
}

///////////////////////////////////////////////////////////
SMStateBase * SMSecOperatorPWStateBase::DMScanner(void)
{
	//+SR93 Biometrics support
	if (co.m_bOperationsUseFingerPrintDevice)  
	{
	  FT_Abort();
	  g_bFPTouched = false; //397387
	}
	//-SR93 Biometrics support

	if (   (csOperator != _T(""))  // TAR 283345 - Skip validation if 
		&& (csPassword != _T(""))  // TAR 283345 -			null data
		&& (bValidScannedOperator)
		&& (TBValidateOperator(csOperator,csPassword) == 1))	// TAR 409017 
	{
		rp.SetInterventionEndInProgress(true);  // TAR 311902
		// Valid operator/password, can leave prompting mode
		fInValidOperatorPassword = false; // TAR 278085
        inputMode = OPERATOR_PASSWORD_ACCEPTED;
  		dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
     	                    DM_TRICOLORLIGHT_OFF,
       	                    DM_TRICOLORLIGHT_NORMALREQUEST);
        // Reshow screen, noting that password has been verified
  	    showScreen(true);
	    return stateAfterPasswordVerify();
    }
    else
    {
		DMSayPhrase(KEYPADINVALID);
   	    fInValidOperatorPassword = true;    // TAR 253541
        return displayInvalidMessage();     // TAR 253541
  	}
}

///////////////////////////////////////////////////////////
SMStateBase *SMSecOperatorPWStateBase::displayInvalidMessage(void)
{
	//+ TAR [384711]: note: Added to reset inputed operator ID and password
	g_csOperator = _T("");
    g_csPassword = _T("");
	//- TAR [384711]
	
    saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
	
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
		
		
	}
    else
	{
		ps.SetCMFrame(_T("OperatorPasswordStateInvalidPassword"));		
		ps.SetCMLeadthruText(LTT_INCORRECTID);
		ps.Message12(MSG_OPERATORPASSWORDINVALID, true);
		
	}
  	
	ps.CMButtonFlash(_T("CMButton1Med"),true);
	ps.Button(_T("ButtonGoBack"), PS_NOTEXT, false);
	ps.ShowCMTBText(csTBMessage2Scot);	 
	ps.ShowCMFrame(false);
	//-SR93 Biometrics support
		
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecOperatorPWStateBase::OnWtIncreaseNotAllowed()	 
{
	// send to RAP but stay in same state
	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("UnexpectedIncrease"));
	m_View = DATANEEDED_UNEXPECTEDINCREASE;
	CString csInstruction, csDesc;
	csDesc.Format(ps.GetPSText(RA_UNEXPECTEDINCREASEINFO, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits);
	// RA_WEIGHTINCREASEINFO is a string that says "Actual weight: %s %s".  This name should be changed to RA_WEIGHTCHANGEINFO LPM022002
	csInstruction.Format(ps.GetPSText(RA_WEIGHTINCREASEINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTINCREASE, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits, _T(""));
	ra.OnRAPDataNeeded(csInstruction, cDataEntry.ToString(), m_View);
	DMSaySecurity( SEC_NONE, _T("blank.wav") );

	CREATE_AND_DISPENSE(SecUnExpectedIncrease)(m_PrevState, m_NextState);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSecOperatorPWStateBase::OnWtDecrease()
{
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
}
SMStateBase *SMSecOperatorPWStateBase::TareWeightedItem(void)
{
	// ----------------------------------------------
	// 1. Is the S/S Wt Increase = (BS wt Increase - BOX TARE)
	// ----------------------------------------------
	SAClearSecurityCondition();					// tell RAP the person fixed their own error
	//218558 set scanner flag to re-enabled only in state that isn't handling EAS automatically
	trace(L2, _T("bEASReEnableScanner is set to true."));
	bEASReEnableScanner = true;
//	io.lWeightEntered = io.lWeightEntered - co.nOperationsTareWtToIgnore1;	// subtract the Tare in case ever want to update here
    OverrideSecurityException(_T("None"), KEY_EXCEPTION_OPERATION_CLEAR);
	return setAndCreateAnchorState(m_PrevState);	// go to next state = ScanAndBag

}
SMStateBase *SMSecOperatorPWStateBase::TareOnly(void)
{
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
		return createState(m_PrevState);	// delta matched one of the Tares so return to BagAndEAS
	}
}
SMStateBase *SMSecOperatorPWStateBase::TareWLDBItem(void)
{
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
	return createState(BES_SCANANDBAG);
}
SMStateBase *SMSecOperatorPWStateBase::TareNone(void)
{
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
	//SR085 -
}

//SR085 +
SMStateBase *SMSecOperatorPWStateBase::TareOnlyAtAttract(void)
{
	ClearSecurityViolationLight();
	ra.OnNoRAPDataNeeded();// tell RAP the person fixed their own error
	OverrideSecurityException(_T("None"), KEY_EXCEPTION_OPERATION_CLEAR);
	//return createAnchorState();
	return setAndCreateAnchorState(BES_SCANANDBAG);// TareOnlyAtAttract is called only from Attract so the logical transition will be to S&B
}
//SR085 -

/////////////////////////////////////////////////////////////////
//+SR93 Biometrics
void SMSecOperatorPWStateBase::ShowEnterID()
{
	g_csOperator = _T("");
	g_csPassword = _T("");
	ps.Echo(PS_BLANK); //Tar 396211
	ps.SetCMFrame( _T("EnterID") );
	ps.SetCMLeadthruText( TXT_ENTER_ID ); //395105
	if (co.m_bOperationsUseFingerPrintDevice)
	{
		if (g_bBiometricACLServer && g_bBiometricACLDevice)
		{
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