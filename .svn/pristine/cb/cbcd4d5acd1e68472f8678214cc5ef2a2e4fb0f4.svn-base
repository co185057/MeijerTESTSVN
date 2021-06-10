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
#ifndef _CPPUNIT
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
#include "SMRAPDataNeeded.h"
#include "SMSmAssistMenu.h"
DLLEXPORT extern DMProcedures dm;             // Global DM object
#else
#include "SMOperatorPasswordStateBase.h"
#endif //CPPUNIT


#define COMP_ID ID_SM                         // Base state component
#define T_ID _T("OperatorPasswordStateBase")

IMPLEMENT_DYNCREATE(SMOperatorPasswordStateBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////////////
SMOperatorPasswordStateBase::SMOperatorPasswordStateBase()
: inputMode(PROMPTING_FOR_OPERATOR), 
 fInValidOperatorPassword(false),
 m_HKStatus(IPSProcedures::HK_NORMAL),
 m_bShiftOn(false),
 m_bAltGrOn(false),
 m_bSymbolOn(false),
 m_csItemtSentTmp(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
  m_nCharLimit = 3;	
  saMsgFuncPtr = ps.SecurityMessageModal;
}

//////////////////////////////////////////////////
SMStateBase * SMOperatorPasswordStateBase::Initialize(void)
{
#ifndef _CPPUNIT
    ps.SetPSText(PS_WORK1,ps.GetPSText(BTT_STORELOGIN,SCOT_LANGUAGE_PRIMARY));
    ps.SetButtonText(_T("ButtonStoreLogIn"),PS_WORK1);
  // TAR 397386 - Check if we are in Multi-item picklist before sending state to Security Manager.
  if(!fInMultiSelectPickList)
  {
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("OperatorPasswordState") );	// RFC 330050
  }
  if (co.IsLoginBypassOn() && IsLoginBypassAllowed())
  {
      inputMode = OPERATOR_PASSWORD_ACCEPTED;
      if (co.GetAttendantID().GetLength())  //Might be handling Generic Delayed
      {
          rp.SetOperator(co.GetAttendantID());
      }
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

  inputMode = SHOW_WAIT_FOR_APPROVAL;
  showScreen(false);

#endif //CPPUNIT
  if(csItemSent != EMPTY_STRING)
  {
    m_csItemtSentTmp = csItemSent;
    csItemSent = EMPTY_STRING;
  }


  return STATE_NULL;
}

//////////////////////////////////////////////////
void SMOperatorPasswordStateBase::UnInitialize()
{   
    if(m_csItemtSentTmp != EMPTY_STRING)
    {
        csItemSent = m_csItemtSentTmp;
    }

#ifndef _CPPUNIT
//+TAR 444373 - DO NOT CALL SetLanguage() to change the language code to get the right message.  We should 
// pass in the language code to GetPSText()
//  ps.SetLanguage(m_customerLanguage);
//-TAR 444373

  SAWLDBScannerDisable();
  
  if (co.GetbOperationsUseFingerPrintDevice())  //SR93 Biometrics
  {
	FT_Abort();
  }

  // TAR 397386 - Check if we are in Multi-item picklist before sending state to Security Manager.
  if(!fInMultiSelectPickList)
  {
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("OperatorPasswordState") );	// RFC 330050
  }
#endif //CPPUNIT
}

//////////////////////////////////////////////
SMStateBase * SMOperatorPasswordStateBase::PSEnterKey()
{
#ifndef _CPPUNIT
	//tar 244851 - workaround for tri-light yellow/green and red on at same time problem
	//turn the yellow/green light off if Red is already on and any red approval is pending
	if (dm.getTriColorLightColor() == DM_TRICOLORLIGHT_RED  && AreAnyRedApprovalsPending())
	{
		// (*) SR898 ====================
        DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                           DM_TRICOLORLIGHT_OFF,
                           DM_TRICOLORLIGHT_NORMALREQUEST);
                           
		// (*) SR898 ====================
        DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
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

#endif //CPPUNIT
  return STATE_NULL;
}

///////////////////////////////////////////////////////////////
SMStateBase * SMOperatorPasswordStateBase::PSNumericKey(const long lKey)
{
#ifndef _CPPUNIT
  // Don't echo numbers after operator/password accepted
  if (inputMode != OPERATOR_PASSWORD_ACCEPTED)
    PSEchoInput(lKey);
#endif //CPPUNIT
  return STATE_NULL;
}

// NewUI
SMStateBase * SMOperatorPasswordStateBase::PSAlphaNumericKey(const long lKey)
{
#ifndef _CPPUNIT
  // Don't echo numbers after operator/password accepted
  if (inputMode != OPERATOR_PASSWORD_ACCEPTED)
  {
    long rVal = ps.EchoInput((_TCHAR)lKey, true);
    if (rVal == -2) 
	      DMSayPhrase(KEYPADINVALID);
  }
#endif //CPPUNIT
  return STATE_NULL;
}

// NewUI
//////////////////////////////////////////////
SMStateBase * SMOperatorPasswordStateBase::PSClearKey()
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
#ifndef _CPPUNIT

    switch(inputMode)
    {  
	  case SHOW_WAIT_FOR_APPROVAL:
	  case OPERATOR_PASSWORD_ACCEPTED:
        return STATE_RETURN;  
        break;
      case PROMPTING_FOR_OPERATOR:
	  	if (co.GetbOperationsUseFingerPrintDevice())   //SR93 Biometrics
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
#endif //CPPUNIT
	return STATE_NULL;
 
}

/////////////////////////////////////////////////
void SMOperatorPasswordStateBase::promptForOperator()
{
  DMScannerEnable();
  //+TAR 444373
  CPSXREDRAWLOCK  csRedrawLock(_T("Display"), _T("SMOperatorPasswordStateBase::promptForOperator"));
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
void SMOperatorPasswordStateBase::promptForPassword()
{
    DMScannerDisable();
	if( co.GetfStateDualLanguage() )
		ps.SetLanguage( SCOT_LANGUAGE_PRIMARY );

	inputMode = PROMPTING_FOR_PASSWORD;
	

		saMsgFuncPtr = ps.SecurityMessageNonmodal;
        if(!co.GetfOperationsOperatorNumericKeyPad())
        {
            ShowAlphaNumericKeypad();
            m_pAppPSX->SetConfigProperty(_T("InstructionScreenTitle"), m_csFrameName, UI::PROPERTYTEXT, ps.GetPSText(TXT_ENTER_PASSWORD));
        }
        else
        {
            m_csFrameName = _T("EnterID");
            ps.SetCMFrame(m_csFrameName);	// EnterPassword //
            ps.SetCMLeadthruText( TXT_ENTER_PASSWORD );
        }
		ps.ShowNumericKeypad();
		ps.Message12( MSG_ENTERPASSWORD_PRESSENTER, false );

		// ++ NextGenUI: Fix issue on pressing Go Back will not go back to EnterID context
		m_pAppPSX->SetTransactionVariable( _T("NextGenData"), COleVariant(_T("EnterPassword")) );

 
	ps.ShowCMTBText( csTBMessage2Scot );	//thp
	ps.SetCMGobackButtonVisible( true );



	//ps.AllowInput( MAX_DIGITS_PASSWORD, true, false );
    m_nCharLimit = co.GetnOperationsOperatorPWMaxDigit();
    ps.AllowInput(m_nCharLimit, true, false );
		
	ps.ShowCMFrame( false );

}

bool SMOperatorPasswordStateBase::GetUserInfo()
{
#ifndef _CPPUNIT
    trace(L7, _T("SMOperatorPasswordStateBase::GetUserInfo() g_csOperator[%s], g_csPassword[%s], inputMode[%d] "), g_csOperator, g_csPassword, inputMode);  
    if( co.GetbOperationsUseFingerPrintDevice() && g_bFPTouched )
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


    if (inputMode == PROMPTING_FOR_OPERATOR)
    {
        ps.GetInput(csOperator);
        if (! csOperator.IsEmpty())
        // Operator entered, now prompt for password
        {

            if (co.GetbOperationsUseFingerPrintDevice() )
            {
                FT_Abort();
                g_csOperator = csOperator;
            }
            promptForPassword();
        }
        else
        {
            // Nothing entered - leave and continue to prompt for operator
            DMSayPhrase(KEYPADINVALID);
            return false;
        }
    }

    else if (inputMode == PROMPTING_FOR_PASSWORD)
    {
        ps.GetInput(csPassword);
        // If password entered, validate operator and password
        //+SR93 Biometrics
        if (! csPassword.IsEmpty() || ! g_csPassword.IsEmpty())
        {
            if (g_csPassword.IsEmpty()) // keyed in
            {
                g_csOperator = csOperator;
                g_csPassword = csPassword;
            }
            if (csPassword.IsEmpty()) //finger print login
            {
                csOperator = g_csOperator;
                csPassword = g_csPassword;
            }
            return true;
        }
        else
        {
            // If we haven't returned yet, there was no password or it was invalid.
            // Start over and ask for operator again
            DMSayPhrase(KEYPADINVALID);
            promptForOperator();
        }
    }
#endif
    return false;
}

bool SMOperatorPasswordStateBase::OnUserCredentialsAquired()
{
#ifndef _CPPUNIT
    trace(L7, _T("SMOperatorPasswordStateBase::OnUserCredentialsAquired()"));
    // read 4690 operator file
    BOOL bValidCashier = false;
    //Validate head cashier first if configured to use head cashier context in SCOTOPTS.
    //or Validate Head Cashier if configured to use context sensitive menus - SR819
    //Normal cashier will be validated if TB returns false for head cashier validation.
    if ( co.fStateContextSensitiveMenus || (!co.csOperationsHeadCashierContext.IsEmpty() && 
         (co.csOperationsHeadCashierContext != _T("SmAuthorization"))) )
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
        if (co.GetbOperationsUseFingerPrintDevice() && g_bFPTouched) //TAR 395787
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
            if(!isAnyApprovalRequired())
            {
                // (*) SR898 ====================
                DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                                   DM_TRICOLORLIGHT_OFF,
                                   DM_TRICOLORLIGHT_NORMALREQUEST);
            }
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
        //TAR 445626 - restore lines commented out by 429111
        if (bMakeXMLNow)
        {
            rp.TransactionEnd();
        }
        //-TAR 343053 
        
        return true;
    }
    else  // No invalid
    {
        displayInvalidMessage();
        g_bFPTouched = false; //397387
        return false;
    }
#endif
    return false;
}
/////////////////////////////////////////////////////
bool SMOperatorPasswordStateBase::getOperatorPassword()
{
#ifndef _CPPUNIT
    if(GetUserInfo())
    {
        if(OnUserCredentialsAquired())
        {
            return true;
        }
    }

#endif //CPPUNIT
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
SMStateBase * SMOperatorPasswordStateBase::createLevel2Message(LPCTSTR szMsg,
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
SMStateBase * SMOperatorPasswordStateBase::createLevel3Message(LPCTSTR szMsg,
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
    CREATE_AND_DISPENSE(SystemMessage3)(szMsg,pstMsg,prompt,value,device,checkHealth,displayHTML);
  }
  else
  {
    CREATE_AND_DISPENSE(SystemMessage6)(szMsg,pstMsg,prompt,value,device,checkHealth,displayHTML);
  }
#else
  return STATE_NULL;
#endif //CPPUNIT
}


///////////////////////////////////////////////////////////
SMStateBase * SMOperatorPasswordStateBase::DMScanner(void)
{
#ifndef _CPPUNIT
	//+SR93 Biometrics support
	if (co.GetbOperationsUseFingerPrintDevice())  
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
    
	DMSayPhrase(KEYPADINVALID);
    return displayInvalidMessage();     // TAR 253541
  	
#else
    return STATE_NULL;
#endif //CPPUNIT
}


///////////////////////////////////////////////////////////
SMStateBase *SMOperatorPasswordStateBase::displayInvalidMessage(void)
{
#ifndef _CPPUNIT
    saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
    fInValidOperatorPassword = true;
	//+SR93 Biometrics	
	if (co.GetbOperationsUseFingerPrintDevice() && inputMode != PROMPTING_FOR_PASSWORD && g_bFPTouched ) //Tar 396905
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
		ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid); //TAR 435471
	}
    else
	{
		//+SR879
		CString csCurrentStateName, csContextName; 
		csCurrentStateName = this->GetRuntimeClass()->m_lpszClassName; 
        if ( !csOperator.IsEmpty() &&
            ( csCurrentStateName.Find(_T("RAPDataNeeded")) != -1 ||csCurrentStateName.Find(_T("ContextHelp")) != -1 ) &&
            csPassword == _T("") || ( ra.GetRAPDataPendingCount() == 0 ) ) //sscoadk-7272: need to generate intervention if count is 0
		{ 
	
			trace(L6, _T("+ra.OnRAPDataNeeded")); 

			cDataEntry.FromConfigData(co.csDataEntryINIFile, "InvalidBarcode"); 
		
			m_View = DATANEEDED_INVALIDBARCODE;
			
			CString csSecInfo; 
			csSecInfo.Format(ps.GetPSText(MSG_DATANEEDED_INVALIDBARCODE, SCOT_LANGUAGE_PRIMARY));

			ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View); 

			trace(L6, _T("-ra.OnRAPDataNeeded")); 

			CREATE_AND_DISPENSE(RAPDataNeeded)(ps.GetPSText(MSG_INVALIDCODE,SCOT_LANGUAGE_PRIMARY), m_View);
		}
		//-SR879
		ps.SetCMFrame(_T("OperatorPasswordStateInvalidPassword"));
		ps.SetCMLeadthruText(LTT_INCORRECTID);
		ps.Message12(MSG_OPERATORPASSWORDINVALID, true);
	}

  				
	ps.CMButtonFlash(_T("CMButton1Med"),true);
#endif //CPPUNIT
    ps.ButtonState(_T("ButtonGoBack"), false, false);    // TAR 228164
	ps.ShowCMTBText(csTBMessage2Scot);	 
	ps.ShowCMFrame(false);
	//-SR93 Biometrics

    DisplayAuxScreen(AUX_SCREEN_USE_TOUCHSCREEN, true);	// TAR 228085
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMOperatorPasswordStateBase::OnBackToLGW(void)
{
#ifndef _CPPUNIT
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
#else
    return STATE_NULL;
#endif //CPPUNIT
}

//+SR697 gc185038
void SMOperatorPasswordStateBase::EndOfTransactionPrint() 
{
#ifndef _CPPUNIT

	trace(L6,_T("SMOperatorPasswordStateBase::EndOfTransactionPrint"));
	
	if(co.csOperationsEOTReceiptPrintingState == _T("NonInterventionState"))
	{
		m_bEOTPrintPending = true;
	
	}
	else
	{
		STATE(HelpModeState)::EndOfTransactionPrint();
	}

	trace(L6,_T("SMOperatorPasswordStateBase::EndOfTransactionPrint"));

#endif //CPPUNIT
}
//-SR697 gc185038

///////////////////////////////////////////////////////////////////////////
//+SR93 Biometrics
void SMOperatorPasswordStateBase::ShowEnterID()
{
    DMScannerEnable();
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
        m_csFrameName = _T("EnterID");
        ps.SetCMFrame(m_csFrameName);  
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

SMStateBase *SMOperatorPasswordStateBase::OnSecMgrNeedData(CString exceptionType, long lItemID)
{
    if (!g_bIsSmartAssistImmediate && !bIsInFinalization)
        return STATE(HelpModeState)::OnSecMgrNeedData(exceptionType, lItemID);
    else
    {
        trace (L6, _T("SMOperatorPasswordStateBase::OnSecMgrNeedData() - suppressing SecMgr DataNeeded"));
        return STATE_NULL;
    }
}

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
SMStateBase  *SMOperatorPasswordStateBase::PSCharKey(TCHAR c)
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
SMStateBase  *SMOperatorPasswordStateBase::PSHotKey(CString & csHotKey) 
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

SMStateBase  *SMOperatorPasswordStateBase::PSShiftKey(void)
{
  
  m_bShiftOn=!m_bShiftOn;
  SetAlphaShiftState(m_csFrameName, m_bShiftOn);

  ps.PopulateHKSensitiveControls(m_csFrameName, m_HKStatus, m_bShiftOn, m_bAltGrOn, m_bSymbolOn);

  return STATE_NULL;
}

/////////////////////////////////////
SMStateBase  *SMOperatorPasswordStateBase::PSAltGrKey(void)
{
  
  m_bAltGrOn=!m_bAltGrOn;

  ps.PopulateHKSensitiveControls(m_csFrameName, m_HKStatus, m_bShiftOn, m_bAltGrOn, m_bSymbolOn);

  return STATE_NULL;
}

SMStateBase  *SMOperatorPasswordStateBase::PSSymbolKey(void)
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

SMStateBase  *SMOperatorPasswordStateBase::PSSpaceKey(void)// 
{
  ps.EchoInput((_TCHAR)(0x20), true);
  return STATE_NULL;
}


void SMOperatorPasswordStateBase::ShowAlphaNumericKeypad(void)
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

//////////////////////////////////////////////
SMStateBase *SMOperatorPasswordStateBase::PSBackSpaceKey()
{
    PSClearInput();

    CString csValue;
    long lLen = ps.GetInput(csValue);
        
    if (lLen == 0) 
        ps.ClearInput();

    return STATE_NULL;
}

SMStateBase* SMOperatorPasswordStateBase::HandleScannerData(const MessageElement* message)
{
   CString Saved_csDMLastBarLabel = _T("");
   long    Saved_csDMLastBarType = 0;
   CString Saved_csItemSent= _T("");
   CString Saved_csDMLastBarCode= _T("");
   
   if (fItemIsWaiting) 
   {
       Saved_csDMLastBarLabel = csDMLastBarLabel;
       Saved_csDMLastBarType = csDMLastBarType;
       Saved_csItemSent = csItemSent;
       Saved_csDMLastBarCode = csDMLastBarCode;
   }
   SMStateBase *pState = STATE(HelpModeState)::HandleScannerData( message);

   if (fItemIsWaiting)
   {
       csDMLastBarLabel = Saved_csDMLastBarLabel;
       csDMLastBarType = Saved_csDMLastBarType;
       csItemSent = Saved_csItemSent;
       csDMLastBarCode = Saved_csDMLastBarCode;
   }
   return pState;
}


SMStateBase *SMOperatorPasswordStateBase::DMScannerEncrypted(void) //sscoadk-7272
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