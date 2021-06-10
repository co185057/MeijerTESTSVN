//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMOperatorPasswordStateBase.cpp
//
// Description: Derived from SMHelpModeState, all states that need to prompt
//        for an operator and password derive from this state.
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                          // MS MFC always needed first
#include "Common.h"
#include "SMOperatorPasswordState.h"
#include "SMSystemMessage1.h"
#include "SMSystemMessage2.h"
#include "SMSystemMessage3.h"
#include "SMSystemMessage4.h"
#include "SMSystemMessage5.h"
#include "SMSystemMessage6.h"
#include "DMProcedures.h"
#include "AuthenticationClientLibraryAPI.h"   //SR93 Biometrics support
DLLEXPORT extern DMProcedures dm;             // Global DM object

#define COMP_ID ID_SM                         // Base state component
#define T_ID _T("OperatorPasswordStateBase")

IMPLEMENT_DYNCREATE(SMOperatorPasswordStateBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////////////
SMOperatorPasswordStateBase::SMOperatorPasswordStateBase()
: inputMode(PROMPTING_FOR_OPERATOR), fInValidOperatorPassword(false) // TAR 278085
{
  IMPLEMENT_TIMESTAMP
  saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
}

//////////////////////////////////////////////////
SMStateBase * SMOperatorPasswordStateBase::Initialize(void)
{
  // TAR 397386 - Check if we are in Multi-item picklist before sending state to Security Manager.
  if(!fInMultiSelectPickList)
  {
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("OperatorPasswordState") );	// RFC 330050
  }
  if (co.IsLoginBypassOn() && IsLoginBypassAllowed())
  {
      inputMode = OPERATOR_PASSWORD_ACCEPTED;
      rp.SetOperator(co.GetAttendantID());
      return stateAfterPasswordVerify();
  }
  bool bWaitingOnWLDB = false;
  if (fInWLDBProcess)
  {
      bWaitingOnWLDB = true;
      fInWLDBProcess = false;
  }
  DMScannerEnable(); 
  if (bWaitingOnWLDB)
  {
      fInWLDBProcess = true;
  }
  fInValidOperatorPassword = false;

      if( getAnchorState() == BES_THANKYOU )//rfc209702
      {   // TAR 346636     
		  if (lDMLastCode != DM_CASHACCEPTOR_STATUS_OK )
			  inputMode = SHOW_WAIT_FOR_APPROVAL;
		  else
			  inputMode = PROMPTING_FOR_OPERATOR;
	  }
      else
      {
          inputMode = SHOW_WAIT_FOR_APPROVAL;
      }
      showScreen(false);

  return STATE_NULL;
}

//////////////////////////////////////////////////
void SMOperatorPasswordStateBase::UnInitialize()
{
  ps.SetLanguage(m_customerLanguage);
  SAWLDBScannerDisable();
  
  if (co.m_bOperationsUseFingerPrintDevice)  //SR93 Biometrics
  {
	FT_Abort();
  }

  // TAR 397386 - Check if we are in Multi-item picklist before sending state to Security Manager.
  if(!fInMultiSelectPickList)
  {
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("OperatorPasswordState") );	// RFC 330050
  }
}

//////////////////////////////////////////////
SMStateBase * SMOperatorPasswordStateBase::PSEnterKey()
{
	//tar 244851 - workaround for tri-light yellow/green and red on at same time problem
	//turn the yellow/green light off if Red is already on and any red approval is pending
	if (dm.getTriColorLightColor() == DM_TRICOLORLIGHT_RED  && AreAnyRedApprovalsPending())
	{
		dm.setTriColorLight(DM_TRICOLORLIGHT_GREEN,
						DM_TRICOLORLIGHT_OFF,
						DM_TRICOLORLIGHT_NORMALREQUEST);
		dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                      DM_TRICOLORLIGHT_OFF,
                      DM_TRICOLORLIGHT_NORMALREQUEST);
	}

  // Enter key not valid after valid operator/password entered
  if (inputMode != OPERATOR_PASSWORD_ACCEPTED)
  {
	DisplayAuxScreen(AUX_SCREEN_USE_TOUCHSCREEN, true);	//228072 dislay default msg on enter password. LPM021803
    if (getOperatorPassword())
      return stateAfterPasswordVerify();
  }

  return STATE_NULL;
}

///////////////////////////////////////////////////////////////
SMStateBase * SMOperatorPasswordStateBase::PSNumericKey(const long lKey)
{
  // Don't echo numbers after operator/password accepted
  if (inputMode != OPERATOR_PASSWORD_ACCEPTED)
    PSEchoInput(lKey);
  return STATE_NULL;
}
// NewUI
SMStateBase * SMOperatorPasswordStateBase::PSAlphaNumericKey(const long lKey)
{
  // Don't echo numbers after operator/password accepted
  if (inputMode != OPERATOR_PASSWORD_ACCEPTED)
  {
    long rVal = ps.EchoInput((_TCHAR)lKey, true);
    if (rVal == -2) 
	      DMSayPhrase(KEYPADINVALID);
  }
  return STATE_NULL;
}
// NewUI
//////////////////////////////////////////////
SMStateBase * SMOperatorPasswordStateBase::PSClearKey()
{
  // Clear key not valid after operator/password accepted
  if (inputMode != OPERATOR_PASSWORD_ACCEPTED)
  {
       PSClearAllInput(true);
  } 

  return STATE_NULL;
}

/////////////////////////////////////////////
SMStateBase * SMOperatorPasswordStateBase::PSButton1()
{
  // Cancel only valid while prompting for password.  Go back to
  // prompting for password (and original version of screen.)
  if (inputMode == PROMPTING_FOR_PASSWORD || fInValidOperatorPassword)
  {
    promptForOperator();
  }

  // Either way, stay in same state
  return STATE_NULL;
}

/////////////////////////////////////////////
SMStateBase * SMOperatorPasswordStateBase::PSButton2()
{
  promptForOperator();
  return STATE_NULL;
}

///////////////////////////////////////////////
SMStateBase * SMOperatorPasswordStateBase::PSCancelKey()
{
	//TAR#117398	UITS1700 REVC Cancel should do nothing	LPM092399
	//DMSayPhrase("KeyPadValid");
	//return PSButton1();
	return STATE_NULL;

}

///////////////////////////////////////////////
SMStateBase * SMOperatorPasswordStateBase::PSButtonGoBack()
{

    switch(inputMode)
    {  
	  case SHOW_WAIT_FOR_APPROVAL:
	  case OPERATOR_PASSWORD_ACCEPTED:
        return STATE_RETURN;  
        break;
      case PROMPTING_FOR_OPERATOR:
	  	if (co.m_bOperationsUseFingerPrintDevice)   //SR93 Biometrics
		{
			FT_Abort();
		}

        inputMode = SHOW_WAIT_FOR_APPROVAL;
        ps.SetLanguage(m_customerLanguage);
		break;
      case PROMPTING_FOR_PASSWORD:
        promptForOperator();
		break;
    }
	showScreen(false);
	return STATE_NULL;
 
}
/////////////////////////////////////////////////
void SMOperatorPasswordStateBase::promptForOperator()
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
void SMOperatorPasswordStateBase::promptForPassword()
{
	if( co.fStateDualLanguage )
		ps.SetLanguage( SCOT_LANGUAGE_PRIMARY );

	inputMode = PROMPTING_FOR_PASSWORD;
	

		saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
		ps.SetCMFrame( _T("EnterID") );	// EnterPassword //
		ps.SetCMLeadthruText( TXT_ENTER_PASSWORD );
		ps.ShowNumericKeypad();
		ps.Message12( MSG_ENTERPASSWORD_PRESSENTER, false );

 
	ps.ShowCMTBText( csTBMessage2Scot );	//thp
	ps.SetCMGobackButtonVisible( true );



	ps.AllowInput( MAX_DIGITS_PASSWORD, true, false );
		
	ps.ShowCMFrame( false );

}

/////////////////////////////////////////////////////
bool SMOperatorPasswordStateBase::getOperatorPassword()
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
	//+SR93 Biometrics
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
        BOOL bValidCashier = false;
        //Validate head cashier first if configured to use head cashier context in SCOTOPTS.
        //Normal cashier will be validated if TB returns false for head cashier validation.
        if ( !co.csOperationsHeadCashierContext.IsEmpty() && 
             (co.csOperationsHeadCashierContext != _T("SmAuthorization")) )
        {
            g_bValidHeadCashier = TBValidateHeadCashier(g_csOperator, g_csPassword);
            if (!g_bValidHeadCashier)
            {
                bValidCashier = TBValidateOperator(g_csOperator, g_csPassword);
            }
        }
        else
        {
            bValidCashier = TBValidateOperator(g_csOperator, g_csPassword);
        }
		if ( g_bValidHeadCashier || (bValidCashier == 1) )
        //-SR93 Biometrics
		{
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
	            dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                                    DM_TRICOLORLIGHT_OFF,
                                    DM_TRICOLORLIGHT_NORMALREQUEST);
            }

	        // Reshow screen, noting that password has been verified
	        showScreen(true);
			trace(L7, _T("+rp.m_bTransAssisted"));
            rp.SetInterventionTerminalNumber(rp.GetTerminalNumber());
			rp.m_bTransAssisted = true;
			trace(L7, _T("-rp.m_bTransAssisted<%d>"),rp.m_bTransAssisted);

			// Fix 318447 
			// Output the Sign On signature to the report server.
			//+TAR 343053 
			bool bMakeXMLNow = false;
			if (!rp.IsTransactionInProgress())
			{
				bMakeXMLNow = true;
				rp.TransactionStart(rp.m_csBusinessDate);
			}
			rp.SetOperator(csOperator);
			rp.OperatorLogin(rp.m_lOperatorLoginID, ATTIN, SCOTAPP);
			rp.m_lOperatorLoginID += 1;
			if (bMakeXMLNow)
			{
				rp.TransactionEnd();
			}
			//-TAR 343053 
			
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
SMStateBase * SMOperatorPasswordStateBase::createLevel1Message(LPCTSTR szMsg,
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
SMStateBase * SMOperatorPasswordStateBase::createLevel2Message(LPCTSTR szMsg,
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
SMStateBase * SMOperatorPasswordStateBase::createLevel3Message(LPCTSTR szMsg,
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
SMStateBase * SMOperatorPasswordStateBase::DMScanner(void)
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
		&& (bValidScannedOperator))
	{

		BOOL bValidCashier = false;
        //Validate head cashier first if configured to use head cashier context in SCOTOPTS.
        //Normal cashier will be validated if TB returns false for head cashier validation.
        if ( !co.csOperationsHeadCashierContext.IsEmpty() && 
             (co.csOperationsHeadCashierContext != _T("SmAuthorization")) )
        {
            g_bValidHeadCashier = TBValidateHeadCashier(csOperator, csPassword);
            if (!g_bValidHeadCashier)
            {
                bValidCashier = TBValidateOperator(csOperator, csPassword);
            }
        }
        else
        {
            bValidCashier = TBValidateOperator(csOperator, csPassword);
        }
		if ( g_bValidHeadCashier || (bValidCashier == 1) )
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
	}
    
	DMSayPhrase(KEYPADINVALID);
   	fInValidOperatorPassword = true;    // TAR 253541
    return displayInvalidMessage();     // TAR 253541
  	
}

///////////////////////////////////////////////////////////
SMStateBase *SMOperatorPasswordStateBase::displayInvalidMessage(void)
{
    saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
	//+SR93 Biometrics	
	if (co.m_bOperationsUseFingerPrintDevice && inputMode != PROMPTING_FOR_PASSWORD && g_bFPTouched ) //Tar 396905
	{
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
	ps.Button(_T("ButtonGoBack"), PS_NOTEXT, false);    // TAR 228164	
	ps.ShowCMTBText(csTBMessage2Scot);	 
	ps.ShowCMFrame(false);
	//-SR93 Biometrics

    DisplayAuxScreen(AUX_SCREEN_USE_TOUCHSCREEN, true);	// TAR 228085
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMOperatorPasswordStateBase::OnBackToLGW(void)
{

    //446564+
	//Note
	//secEventParms has the most recent exception not the most recent cleared exception
	//For example, 2 exceptions arraived from SM, secEventParms will have the 2nd exception data.
	//When FL receives SmOnSecurityExceptionCleared for the 1st exception and calls this function, secEventParms
	//has 2nd exception data. 

	//FL is at TakeChange state
	if (bIsInFinalization)	
	{
		// TAR228892/199449: We want to stay at the finalization state we're in now instead
		// of going to AnchorState. If we go to AnchorState, it is going to return to
		// Finish and Pay main and they've already started payment usually which is BAD
		// so returning a STATE_NULL.  LPM022603 CMal
		trace(L2, _T("SMOperatorPasswordStateBase::OnBackToLGW. In finalization states, no need to process BackToLGW."));
		//If there is WaitForApproval state waiting, clear it
		if (g_pDelay_WaitForApprovalStateChange != NULL)
		{
			CString csStateName = g_pDelay_WaitForApprovalStateChange->GetRuntimeClass()->m_lpszClassName;

			delete g_pDelay_WaitForApprovalStateChange;
			g_pDelay_WaitForApprovalStateChange = NULL;
			trace(L6,_T("OnBackToLGW - Delete the state %s saved in g_pDelay_WaitForApprovalStateChange"),csStateName);
		}
		return STATE_NULL;

	}
	//446564-

	if (( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_ITEM_REMOVED_THRESHOLD ) ||
		( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_BAG_BOX_THRESHOLD ) ||
		( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_SKIP_BAGGING_THRESHOLD ))

	{
		BEST PrevState = getNextStateForGoodItem();
		if( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_ITEM_REMOVED_THRESHOLD )
		{
			PrevState = BES_SCANANDBAG;
		}

		return ThresholdViolRAPClearSecBtn(PrevState);
	}
	else if (secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_MISMATCHED ) // + TAR 256154
	{
		SAClearSecurityCondition();	
		return createState(BES_BAGANDEAS);
	} // - TAR 256154
	else
	{
		// TAB re-enable scanner and clear Wait For Approval State
		TABReEnableScannerAndClearWaitForApprovalState();
		return STATE_NULL;
	}
}
//+SR697 gc185038
void SMOperatorPasswordStateBase::EndOfTransactionPrint() 
{

	trace(L6,_T("SMOperatorPasswordStateBase::EndOfTransactionPrint"));
	
	if(co.csOperationsEOTReceiptPrintingState == _T("NonInterventionState"))
	{
		m_bEOTPrintPending = true;
	
	}
	else
	{
		SMStateBase::EndOfTransactionPrint();
	}

	trace(L6,_T("SMOperatorPasswordStateBase::EndOfTransactionPrint"));

}
//-SR697 gc185038

///////////////////////////////////////////////////////////////////////////
//+SR93 Biometrics
void SMOperatorPasswordStateBase::ShowEnterID()
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
			to.Trace(L6, _T("Fingerprint Error: variable is set to false. g_bBiometricACLServer= %d, g_bBiometricACLDevice=%d"),g_bBiometricACLServer, g_bBiometricACLDevice );
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
