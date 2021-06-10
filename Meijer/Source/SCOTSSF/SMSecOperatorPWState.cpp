//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecOperatorPWState.cpp
//
// Description: Derived from SMHelpModeState, all states that need to prompt
//        for an operator and password derive from this state.
//
// AUTHOR: Leila Parker
//
// Change History
//
// CRD 638540 Name: CJ Estrada Date: Apr 23, 2021
// CRD 599686 Name: CJ Estrada Mar 22, 2021
// CRD 574057 Name: CJ Estrada Date: Feb 23, 2021
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "Common.h"
#include "SMSecOperatorPWState.h"
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SecOperatorPWState")

IMPLEMENT_DYNCREATE(SMSecOperatorPWState, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////////////
SMSecOperatorPWState::SMSecOperatorPWState()
: SMSecOperatorPWStateBase()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase *SMSecOperatorPWState::Initialize(void)
{
    return SMSecOperatorPWStateBase::Initialize();
}

void SMSecOperatorPWState::UnInitialize(void)
{
    SMSecOperatorPWStateBase::UnInitialize();
}

SMStateBase * SMSecOperatorPWState::createLevel1Message( LPCTSTR szMsg,
                                           PSTEXT pstMsg,
                                           PSTEXT prompt,
                                           long   value,
                                           long   device,
                                           bool   checkHealth,
                                           bool displayHTML)
{
    return SMSecOperatorPWStateBase::createLevel1Message( szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML);
}

SMStateBase * SMSecOperatorPWState::createLevel2Message( LPCTSTR szMsg,
                                           PSTEXT pstMsg,
                                           PSTEXT prompt,
                                           long   value,
                                           long   device,
                                           bool   checkHealth,
                                           bool displayHTML)
{
    return SMSecOperatorPWStateBase::createLevel2Message( szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML);
}

SMStateBase * SMSecOperatorPWState::createLevel3Message( LPCTSTR szMsg,
                                           PSTEXT pstMsg,
                                           PSTEXT prompt,
                                           long   value,
                                           long   device,
                                           bool   checkHealth, 
                                           bool displayHTML )
{
    return SMSecOperatorPWStateBase::createLevel3Message( szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML );
}

SMStateBase *SMSecOperatorPWState::stateAfterPasswordVerify()
{
    return SMSecOperatorPWStateBase::stateAfterPasswordVerify();
}

bool SMSecOperatorPWState::parentClassCurrentlyHandlingButton2()
{
    return SMSecOperatorPWStateBase::parentClassCurrentlyHandlingButton2();
}

bool SMSecOperatorPWState::parentClassCurrentlyHandlingCancelKey()
{
    return SMSecOperatorPWStateBase::parentClassCurrentlyHandlingCancelKey();
}

void SMSecOperatorPWState::showScreen(bool passwordVerified)
{
    SMSecOperatorPWStateBase::showScreen(passwordVerified );
}

SMStateBase * SMSecOperatorPWState::DMScanner()
{
   return SMSecOperatorPWStateBase::DMScanner();
}

SMStateBase * SMSecOperatorPWState::PSEnterKey()
{
	// Enter key not valid after valid operator/password entered
	if (inputMode != OPERATOR_PASSWORD_ACCEPTED)
	{
		// Start CRD 638540 convert the badge to employee ID
		trace(L6, _T("SMSecOperatorPWState::PSEnterKey(%s)"), csOperator);
		CString csEmplid;
		if(csOperator.Find(_T("TM")) >= 0)
		{
			bool bRet = SendTBCommand(TBCMD_GET_EMPLID_FROM_BADGE, (const TCHAR *)csOperator, &csEmplid);
			csOperator = csEmplid;
		}
		// End CRD 638540
		
		if (getOperatorPassword())
			return stateAfterPasswordVerify();
	}
	
	return STATE_NULL;
   //return SMSecOperatorPWStateBase::PSEnterKey();
}

SMStateBase * SMSecOperatorPWState::PSNumericKey(const long lKey)
{
   return SMSecOperatorPWStateBase::PSNumericKey(lKey);
}

SMStateBase * SMSecOperatorPWState::PSClearKey()
{
   return SMSecOperatorPWStateBase::PSClearKey();
}

SMStateBase * SMSecOperatorPWState::PSButton1()
{
   return SMSecOperatorPWStateBase::PSButton1();
}

SMStateBase * SMSecOperatorPWState::PSButton2()
{
   return SMSecOperatorPWStateBase::PSButton2();
}

SMStateBase * SMSecOperatorPWState::PSCancelKey()
{
   return SMSecOperatorPWStateBase::PSCancelKey();
}

SMStateBase * SMSecOperatorPWState::PSButtonGoBack()
{
   return SMSecOperatorPWStateBase::PSButtonGoBack();
}

SMStateBase * SMSecOperatorPWState::displayInvalidMessage()
{
	//+ TAR [384711]: note: Added to reset inputed operator ID and password
	g_csOperator = _T("");
    g_csPassword = _T("");
	//- TAR [384711]
	
	saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
	
    // Determine error message captions to show
    PSTEXT nLTT = LTT_INCORRECTID; // 505
    PSTEXT nMsg = MSG_OPERATORPASSWORDINVALID;  //547
	//+SR93 Biometrics support	
	if (co.m_bOperationsUseFingerPrintDevice && inputMode != PROMPTING_FOR_PASSWORD && !validatingMeijerCardWebApp)		// CRD 574057
	{
		g_bFPTouched = false;

		CString csCurContext;
		csCurContext = _T("OperatorPasswordStateInvalidPasswordWithFP");

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
		
		
	}else if( (TB_SUCCESS_IMMEDIATE == SendTBCommand(TBCMD_IS_EMPLID_ENABLED)) )
    {
        nLTT = SSF_LTT_INCORRECTLOGIN; // // 9190=Incorrect Login
        CString sOperator = csOperator;
        CString sDummy = _T("");
        if( TB_SUCCESS_IMMEDIATE == SendTBCommand(TBCMD_IS_MEIJER_BADGE_GET_BARCODE, &sOperator, &sDummy) )
        {
            // it was a badge scan 
            if( m_sBiometData.GetLength() )
            {
                // badge scan with biometric
                nMsg = SSF_MSG_BADGEBIOMET_INVALID; // 9194=\nIncorrect Employee badge and fingerprint.\n\nPlease try again.
            }
            else // badge scan with keyed password
            {
                nMsg = SSF_MSG_BADGEPASSWORD_INVALID; // 9192=\nIncorrect Employee badge and password.\n\nPlease try again.
            }
        }
        else // it was a keyed emplid entry
        {
            // keyed emplid with biometric
            if( m_sBiometData.GetLength() )
            {
                nMsg = SSF_MSG_EMPLIDBIOMET_INVALID; // 9193=\nIncorrect Employee ID and fingerpint.\n\nPlease try again.
            }
            else // keyed emplid with keyed password
            {
                nMsg = SSF_MSG_EMPLIDPASSWORD_INVALID; // 9191=\nIncorrect Employee ID and password.\n\nPlease try again.
            }
        }

		// Start CRD 574057
		if(validatingMeijerCardWebApp)
			ps.SetCMFrame(_T("MeijerCardInvalidPassword"));
		else
			ps.SetCMFrame(_T("SecOperatorPWStateInvalidPassword"));
		// End CRD 574057
		ps.SetCMLeadthruText(nLTT);      // 505
		ps.CMButton(_T("ButtonHelp"),BTT_HELP,false);
		ps.Message12(nMsg, true);  // 547
    }else
	{
		// Start CRD 574057
		if(validatingMeijerCardWebApp)
			ps.SetCMFrame(_T("MeijerCardInvalidPassword"));
		else
			ps.SetCMFrame(_T("OperatorPasswordStateInvalidPassword"));	
		// End CRD 574057
		ps.SetCMLeadthruText(LTT_INCORRECTID);
		ps.Message12(MSG_OPERATORPASSWORDINVALID, true);
		
	}
	
	// CRD 574057
	if(validatingMeijerCardWebApp)
		fInValidOperatorPassword = true;

	ps.ShowCMTBText(csTBMessage2Scot);	//thp
	ps.ShowCMFrame(false);
	return STATE_NULL;
}

SMStateBase * SMSecOperatorPWState::OnWtIncreaseNotAllowed(void)
{
   return SMSecOperatorPWStateBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMSecOperatorPWState::OnWtDecrease(void)
{
   return SMSecOperatorPWStateBase::OnWtDecrease();
}

SMStateBase * SMSecOperatorPWState::TareWeightedItem(void)
{
   return SMSecOperatorPWStateBase::TareWeightedItem();
}

SMStateBase * SMSecOperatorPWState::TareOnly(void)
{
   return SMSecOperatorPWStateBase::TareOnly();
}

SMStateBase * SMSecOperatorPWState::TareWLDBItem(void)
{
   return SMSecOperatorPWStateBase::TareWLDBItem();
}

SMStateBase * SMSecOperatorPWState::TareNone(void)
{
   return SMSecOperatorPWStateBase::TareNone();
}

SMStateBase * SMSecOperatorPWState::TareOnlyAtAttract(void)
{
   return SMSecOperatorPWStateBase::TareOnlyAtAttract();
}

void SMSecOperatorPWState::promptForOperator()
{
    SMSecOperatorPWStateBase::promptForOperator();
}

void SMSecOperatorPWState::promptForPassword()
{
    if (co.fStateDualLanguage)
    {
        ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
    }
	
    inputMode = PROMPTING_FOR_PASSWORD;
    saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
	// Start CRD 574057
	CString currentContext = _T("");
	if(validatingMeijerCardWebApp)
	{
		DMScannerDisable();		// CRD 599686
		currentContext = _T("MeijerCardEnterID");
		ps.SetCMFrame( _T("MeijerCardEnterID") );
		m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("MeijerCardEnterID"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
	}
	else
	{
		currentContext = _T("EnterID");
		ps.SetCMFrame( _T("EnterID") );	// EnterPassword //
	}
	//End CRD 574057

    ps.SetCMLeadthruText( TXT_ENTER_PASSWORD );
	
    //ps.ShowNumericKeypad();
    // Ensure the keypad is displayed on the password screen
	
    m_pAppPSX->SetConfigProperty(_T("NumericP1"), currentContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
    m_pAppPSX->SetConfigProperty(_T("NumericP2"), currentContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
    m_pAppPSX->SetConfigProperty(_T("NumericP3"), currentContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
    m_pAppPSX->SetConfigProperty(_T("NumericP4"), currentContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
	
	//(+)POS108118
    m_pAppPSX->SetConfigProperty(_T("Echo"), currentContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
    m_pAppPSX->SetConfigProperty(_T("EchoField"), currentContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
	
	CString csNewPosition;
	csNewPosition = _T("0,262,520,65");
	m_pAppPSX->SetConfigProperty(_T("LeadthruText"), currentContext, UI::PROPERTYPOSITION, csNewPosition);
	
	csNewPosition = _T("0,396,510,250");
	m_pAppPSX->SetConfigProperty(_T("MessageBoxNoEcho"), currentContext, UI::PROPERTYPOSITION, csNewPosition);
	
	m_pAppPSX->SetConfigProperty(_T("LeadthruText"),  currentContext,UI::PROPERTYTEXTALIGNMENT, _T("1"));
	m_pAppPSX->SetConfigProperty(_T("MessageBoxNoEcho"),  currentContext,UI::PROPERTYTEXTALIGNMENT, _T("1"));
	
	//(-)POS108118
	
	
    bool bBiometEnabled = (SendTBCommand(TBCMD_IS_BIOMET_ENABLED) == TB_SUCCESS_IMMEDIATE);
    int iMessage = bBiometEnabled ? MSG_ENTERPASSWORD_ORUSEBIOMETRIC : MSG_ENTERPASSWORD_PRESSENTER;
    ps.Message12(iMessage, false );
	m_pAppPSX->SetTransactionVariable( _T("NextGenData"), COleVariant(_T("EnterPassword")) );//SSCOI-45884

    ps.ShowCMTBText( csTBMessage2Scot );	//thp
	
    ps.AllowInput( co.GetnOperationsOperatorPWMaxDigit(), true, false );
    ps.ShowCMFrame( false );
}

bool SMSecOperatorPWState::getOperatorPassword()
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
        if( m_sBiometData.GetLength() )
        {
            SendTBCommand(TBCMD_SIGNONBUILDER_TOSTRING, csOperator, _T(""), m_sBiometData, &csPassword);
        }
        else
        {
            ps.GetInput(csPassword);
        }
        //+SR93 Biometrics support	
		if (csPassword != _T("") || g_csPassword != _T(""))
		{
			if (g_csPassword == _T("")) // keyed in
			{
				g_csOperator = csOperator;
				g_csPassword = csPassword;
			}
			if (csPassword = _T("")) //finger print login
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

void SMSecOperatorPWState::ShowEnterID()
{
    //SMSecOperatorPWStateBase::ShowEnterID();

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
		// Start CRD 574057
		if(validatingMeijerCardWebApp)
		{
			DMScannerEnable();	// CRD 599686
			ps.SetCMFrame( _T("MeijerCardEnterID") );
		}
		// End CRD 574057
		else
		{
			ps.SetCMFrame( _T("EnterID") );
			m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("MeijerCardEnterID"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
		}
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

SMStateBase *SMSecOperatorPWState::DMScannerEncrypted(void) //SSCOADK-7098
{
    return SMSecOperatorPWStateBase::DMScannerEncrypted();
}