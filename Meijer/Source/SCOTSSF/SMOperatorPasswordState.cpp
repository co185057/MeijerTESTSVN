//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMOperatorPasswordState.cpp
//
// Description: Derived from SMHelpModeState, all states that need to prompt
//        for an operator and password derive from this state.
//
// AUTHOR: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "Common.h"
#include "SMOperatorPasswordState.h"
#include "transbrokerssf.h"
#include "SMRAPDataNeeded.h"
#define COMP_ID ID_SM               // base state component
#define T_ID _T("OperatorPasswordState")

IMPLEMENT_DYNCREATE(SMOperatorPasswordState, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////////////
SMOperatorPasswordState::SMOperatorPasswordState()
: SMOperatorPasswordStateBase(), m_sBiometData(_T(""))
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMOperatorPasswordState::Initialize(void)
{
    return SMOperatorPasswordStateBase::Initialize();
}

void SMOperatorPasswordState::UnInitialize(void)
{
	//m_pAppPSX->SetTransactionVariable( _T("NextGenData"), COleVariant(_T("EnterID")) );//SSCOI-45123
    SMOperatorPasswordStateBase::UnInitialize();
}

SMStateBase * SMOperatorPasswordState::createLevel1Message( LPCTSTR szMsg,
                                           PSTEXT pstMsg,
                                           PSTEXT prompt,
                                           long   value,
                                           long   device,
                                           bool   checkHealth,
                                           bool displayHTML)
{
    return SMOperatorPasswordStateBase::createLevel1Message( szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML);
}

SMStateBase * SMOperatorPasswordState::createLevel2Message( LPCTSTR szMsg,
                                           PSTEXT pstMsg,
                                           PSTEXT prompt,
                                           long   value,
                                           long   device,
                                           bool   checkHealth,
                                           bool displayHTML)
{
    return SMOperatorPasswordStateBase::createLevel2Message( szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML);
}

SMStateBase * SMOperatorPasswordState::createLevel3Message( LPCTSTR szMsg,
                                           PSTEXT pstMsg,
                                           PSTEXT prompt,
                                           long   value,
                                           long   device,
                                           bool   checkHealth, 
                                           bool displayHTML )
{
    return SMOperatorPasswordStateBase::createLevel3Message( szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML );
}

SMStateBase * SMOperatorPasswordState::stateAfterPasswordVerify()
{
    return SMOperatorPasswordStateBase::stateAfterPasswordVerify();
}

bool SMOperatorPasswordState::parentClassCurrentlyHandlingButton2()
{
    return SMOperatorPasswordStateBase::parentClassCurrentlyHandlingButton2();
}

bool SMOperatorPasswordState::parentClassCurrentlyHandlingCancelKey()
{
    return SMOperatorPasswordStateBase::parentClassCurrentlyHandlingCancelKey();
}

void SMOperatorPasswordState::showScreen(bool passwordVerified)
{
    SMOperatorPasswordStateBase::showScreen(passwordVerified);
}

SMStateBase * SMOperatorPasswordState::DMScanner(void)
{
    return SMOperatorPasswordStateBase::DMScanner();
}

//POS81634
SMStateBase * SMOperatorPasswordState::PSEnterKey()
{
    // Enter key not valid after valid operator/password entered
    if (inputMode != OPERATOR_PASSWORD_ACCEPTED)
    {
        int nRet =  -99;

        // If the enter key is pressed during the password prompt, then call the checkin function first
        if( inputMode == PROMPTING_FOR_PASSWORD )
        {
			
            
            int currentRAPid =  -99;
            int offlineStatus = -99;

            CString csLocalPassword;
            ps.GetInput(csLocalPassword);

//(+) POS99103 - To identify if the login is a badge and if so, we query employee ID for this badge and use this employee ID for subsequent calls to TMS.

            bool bAllowHandKey = (SendTBCommand(TBCMD_IS_EMPLID_ALLOWHANDKEY) == TB_SUCCESS_IMMEDIATE);
            CString csEmplid;

            trace(L6, _T("csOperator=%s"), csOperator);
			
            if(!bAllowHandKey && csOperator.Find(_T("TM")) >= 0)
            {
                bool bRet = SendTBCommand(TBCMD_GET_EMPLID_FROM_BADGE, (const TCHAR *)csOperator, &csEmplid);
                csOperator = csEmplid;
            }
			
			//CRD 189086 - move saving operator ID after getting empl ID from badge number
			csFLMSOperatorID = csOperator;	//crd 145555

//(-) POS99103 - To identify if the login is a badge and if so, we query employee ID for this badge and use this employee ID for subsequent calls to TMS.

//(+)POS92988

            // POS97215 EXAMPLE ONLY FOR TESTING PURPOSES
            int nYear   =  -99;
            int nMonth  =  -99;
            int nDay    =  -99;
            int nHour   =  -99;
            int nMinute =  -99;
            int nSecond =  -99;
            int nReturnCode = SendTBCommand(TBCMD_GET_OPERATOR_LAST_PWD_CHG_TIME, (const TCHAR *)csOperator, &nYear, &nMonth, &nDay, &nHour, &nMinute, &nSecond);
            // nReturnCode:
            //               0 = Successful
            //              -1 = Unexpected error. Unable to access BIOMET file.
            //              -2 = Invalid operator. Operator not found in BIOMET file, or csOperator parameter is NULL.
            //              -9 = Invalid parameter. One or more of the parameters are NULL.
            trace(L6, _T("Get Last Password Change Time for operator %s: returncode=%d; year=%d; month=%d; day=%d; hour=%d; minute=%d; second=%d"),csOperator,nReturnCode,nYear,nMonth,nDay,nHour,nMinute,nSecond);



            COleDateTime dt_emplLastPwdChng;

//(+) POS99103
            nYear = 2000 + nYear;
//(-) POS99103

           dt_emplLastPwdChng.SetDateTime(nYear, nMonth, nDay, nHour, nMinute, nSecond);

            COleDateTime dtNow;
            dtNow = dtNow.GetCurrentTime();

            COleDateTimeSpan dtSpan;

            dtSpan = dtNow - dt_emplLastPwdChng;

            double nDays = dtSpan.GetTotalDays();
            
            CString csOptionName=_T("ExpiryDuration");
            CString csOptionValue;

            SendTBCommand(TBCMD_GET_OPTIONS, &csOptionName, &csOptionValue);

            long lOptionValue = _ttol(csOptionValue);
			
			trace(L6, _T("*_* Password Expired!!! nDays[%d]; lOptionValue[%d]"), nDays, lOptionValue);

            if(nDays > lOptionValue)
            {
                trace(L6, _T("Password Expired!!!"));
                        m_pAppPSX->SetContext( _T("PasswordExpired") );
                        ps.SetCMFrame( _T("PasswordExpired") );
		                //ps.SetCMLeadthruText( TXT_ENTER_PASSWORD );
                        ps.ShowFrame();

                        return STATE_NULL;
            }

            else
            {
				//CRD 392418
				bool bAttendantTrackingEnabled = GetHookProperty(_T("RAPAttendantTrackingEnabled")) == _T("1") ? true: false;

				if(bAttendantTrackingEnabled)
				{
					//Start CRD 345698
					CString strEntryName(_T("")), strSectionName(_T("")), strGrocery, strGM, strScanPortal, strRAPID;					
					strGrocery = GetHookProperty(_T("RAPAttendantTrackingGrocery"));
					strGM = GetHookProperty(_T("RAPAttendantTrackingGM"));
					strScanPortal = GetHookProperty(_T("RAPAttendantTrackingScanPortal"));
					//End CRD 345698
					
					CString csTerminalNumber(_T(""));
					csTerminalNumber.Format(_T("%d"), TBGetTerminalNumber());
					
					int iRAPID = 1;
					for(iRAPID=1;iRAPID<8;iRAPID++)
					{
						strEntryName.Format(_T("RAP%d"), iRAPID);
						strRAPID = GetHookProperty(strEntryName);	//CRD 345698
						if( strRAPID.Find(csTerminalNumber) != -1 )
							break;
					}             
					
					trace(L6, _T("Store Section RAP, Grocery=%s, GM=%s, ScanPortal=%s, Origin=%s,%d"), strGrocery, strGM, strScanPortal, strRAPID,iRAPID);


					//(-)POS92988
					nRet = SendTBCommand(TBCMD_CHECKINOUT_OPERATOR, 1, (const TCHAR *)csOperator, (const TCHAR *)csLocalPassword, iRAPID, &currentRAPid, &offlineStatus);
					trace(L6, _T("Checkin on RAP%d: nRet=%d; currentRAPid=%d; offlineStatus=%d"),iRAPID, nRet,currentRAPid,offlineStatus);

					CString csRAPOriginID,csRAPCurrentID;
					csRAPOriginID = strEntryName;
					csRAPCurrentID.Format(_T("RAP%d"), currentRAPid);
    
					// logic if the checkin is successfull or attendant has checked in to other system
					if( nRet == 0 || nRet == 2 )
					{
						CString csTimeStamp;
						SYSTEMTIME st;
						GetLocalTime(&st);
						csTimeStamp.Format(_T("%02d/%02d/%02d %02d:%02d:%02d"), st.wMonth, st.wDay, st.wYear, st.wHour, st.wMinute, st.wSecond);
						CString csCurrentVirtualRAPID;

						// If attendant has checked in to other store section, then we need to check if the attendant is checked in to the same store section area ( Grocery or GM ), then don't bother.
						// If the attendant has checked in to different store section, then check out from that store section first then check in to the current one
						if (nRet == 2 )
						{
							trace(L6, _T("RAPOrigin(%s), RAPCurrent(%s)"), csRAPOriginID, csRAPCurrentID);
							// if the attendant has checked in to the different RAP but in the same section, then just pass it through and no need to check out
							if( (strGrocery.Find(csRAPOriginID) != -1 && strGrocery.Find(csRAPCurrentID) != -1) ||
								(strGM.Find(csRAPOriginID) != -1 && strGM.Find(csRAPCurrentID) != -1) ||
								(strScanPortal.Find(csRAPOriginID) != -1 && strScanPortal.Find(csRAPCurrentID) != -1) )
							{
								trace(L6, _T("RAP Attendant has been checked in to the same store section area, check in successfull, not doing anything else"));
							}
							else
							{ 
								trace(L6, _T("RAP Attendant has been checked in to the different store section area, check in not successfull, checkout first"));
								int nNewCurrentRAPid =  -99;
								nRet = SendTBCommand(TBCMD_CHECKINOUT_OPERATOR, 0, (const TCHAR *)csOperator, (const TCHAR *)csLocalPassword, currentRAPid, &nNewCurrentRAPid, &offlineStatus);
								trace(L6, _T("Checkout from RAP%d: nRet=%d; currentRAPid=%d; offlineStatus=%d"),iRAPID, nRet,nNewCurrentRAPid,offlineStatus);
								if(nRet == 0)
								{
									CString csCurrentVirtualRAPID;
									csRAPCurrentID.Format(_T("RAP%d"), nNewCurrentRAPid);
									csCurrentVirtualRAPID = GetHookProperty(csRAPCurrentID);	//CRD 3456798
									csCurrentVirtualRAPID = csCurrentVirtualRAPID.Mid(0,csCurrentVirtualRAPID.Find(_T(":")));
									trace(L6, _T("Valid operator, Checked in to %s, Virtual RAPID(%s), TimeStamp(%s)"),csRAPCurrentID,csCurrentVirtualRAPID,csTimeStamp);
                        
									int nVirtualRAPID = _ttol(csCurrentVirtualRAPID);
									SendTBCommand(TBCMD_NOTIFY_CHECKINOUT, (const TCHAR *)csOperator, (const TCHAR *)csTimeStamp, 0, nNewCurrentRAPid, nVirtualRAPID, offlineStatus);

									//check in to this RAP
									nRet = SendTBCommand(TBCMD_CHECKINOUT_OPERATOR, 1, (const TCHAR *)csOperator, (const TCHAR *)csLocalPassword, iRAPID, &currentRAPid, &offlineStatus);
									trace(L6, _T("Checkin on RAP%d: nRet=%d; currentRAPid=%d; offlineStatus=%d"),iRAPID, nRet,currentRAPid,offlineStatus);

									//POS85750 - TLOG CheckIn record
									if( nRet == 0 )
									{
										csRAPCurrentID.Format(_T("RAP%d"), currentRAPid);
										csCurrentVirtualRAPID = _T("");
										csCurrentVirtualRAPID = GetHookProperty(csRAPCurrentID);	//CRD 3456798
										csCurrentVirtualRAPID = csCurrentVirtualRAPID.Mid(0,csCurrentVirtualRAPID.Find(_T(":")));		
										trace(L6, _T("Valid operator, Checked in to %s, Virtual RAPID(%s), TimeStamp(%s)"),csRAPCurrentID,csCurrentVirtualRAPID,csTimeStamp);
                        
										GetLocalTime(&st);
										csTimeStamp.Format(_T("%02d/%02d/%02d %02d:%02d:%02d"), st.wMonth, st.wDay, st.wYear, st.wHour, st.wMinute, st.wSecond);

										int nVirtualRAPID = _ttol(csCurrentVirtualRAPID);
										SendTBCommand(TBCMD_NOTIFY_CHECKINOUT, (const TCHAR *)csOperator, (const TCHAR *)csTimeStamp, 1, nNewCurrentRAPid, nVirtualRAPID, offlineStatus);
									}
									//e POS85750 - TLOG CheckIn record
								}

								m_pAppPSX->SetContext( _T("CheckinConfirmation") );
								ps.SetCMFrame( _T("CheckinConfirmation") );
								//ps.SetCMLeadthruText( TXT_ENTER_PASSWORD );
								ps.ShowFrame();

								return STATE_NULL;
							}
						}
                
						if( nRet == 0 )
						{                    
							csCurrentVirtualRAPID = GetHookProperty(csRAPCurrentID);
							csCurrentVirtualRAPID = csCurrentVirtualRAPID.Mid(0,csCurrentVirtualRAPID.Find(_T(":")));
							trace(L6, _T("Valid operator, Checked in to %s, Virtual RAPID(%s), TimeStamp(%s)"),csRAPCurrentID,csCurrentVirtualRAPID,csTimeStamp);
      
							int nVirtualRAPID = _ttol(csCurrentVirtualRAPID);
							SendTBCommand(TBCMD_NOTIFY_CHECKINOUT, (const TCHAR *)csOperator, (const TCHAR *)csTimeStamp, 1, currentRAPid, nVirtualRAPID, offlineStatus);

							//POS85750 - Display the message
							m_pAppPSX->SetContext( _T("CheckinConfirmation") ); //POS87145 - use CheckinConfirmation context
							ps.SetCMFrame( _T("CheckinConfirmation") ); //POS87145 - use CheckinConfirmation context
						
							ps.ShowFrame();
							//e POS85750 - Display the message

							return STATE_NULL;
						}			

						return SMOperatorPasswordStateBase::PSEnterKey();                
					}
					else
					{
						trace(L6, _T("CheckIn Not Succesfull, pass it through"));
					}
				}
				trace(L6, _T("RAP Attendant Tracking is  - %d"), bAttendantTrackingEnabled);
				return SMOperatorPasswordStateBase::PSEnterKey();				
			}
            
            
        }
        else
        {
            return SMOperatorPasswordStateBase::PSEnterKey();
        }        
    }

    return STATE_NULL;
}
//e POS81634

SMStateBase * SMOperatorPasswordState::PSNumericKey(const long lKey)
{
    return SMOperatorPasswordStateBase::PSNumericKey(lKey);
}

SMStateBase * SMOperatorPasswordState::PSAlphaNumericKey(const long lKey)
{
    return SMOperatorPasswordStateBase::PSAlphaNumericKey(lKey);
}

SMStateBase * SMOperatorPasswordState::PSClearKey()
{
    return SMOperatorPasswordStateBase::PSClearKey();
}

SMStateBase * SMOperatorPasswordState::PSButton1()
{
    return SMOperatorPasswordStateBase::PSButton1();
}

SMStateBase * SMOperatorPasswordState::PSButton2()
{
    return SMOperatorPasswordStateBase::PSButton2();
}

SMStateBase * SMOperatorPasswordState::PSCancelKey()
{
    return SMOperatorPasswordStateBase::PSCancelKey();
}

SMStateBase * SMOperatorPasswordState::PSButtonGoBack()
{
    return SMOperatorPasswordStateBase::PSButtonGoBack();
}

SMStateBase * SMOperatorPasswordState::displayInvalidMessage()
{
    saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
    // Determine error message captions to show
    PSTEXT nLTT = LTT_INCORRECTID; // 505
    PSTEXT nMsg = MSG_OPERATORPASSWORDINVALID;  //547
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
    }else
	{
		ps.SetCMFrame(_T("OperatorPasswordStateInvalidPassword"));
		ps.SetCMLeadthruText(LTT_INCORRECTID);
		ps.Message12(MSG_OPERATORPASSWORDINVALID, true);
	}
	
	
	ps.SetCMFrame(_T("OperatorPasswordStateInvalidPassword"));
	ps.SetCMLeadthruText(nLTT);
	ps.SetButtonText(_T("CMButton1"), BTT_OK);
    ps.CMButtonFlash(_T("CMButton1"),true);   
	ps.Button(_T("ButtonGoBack"), PS_NOTEXT, false);    // TAR 228164	
	ps.ShowCMTBText(csTBMessage2Scot);	 
    ps.Message12(nMsg, true);
	ps.ShowCMFrame(false);
	
	fInValidOperatorPassword = true;
    DisplayAuxScreen(AUX_SCREEN_USE_TOUCHSCREEN, true);	// TAR 228085
	return STATE_NULL;
}

SMStateBase * SMOperatorPasswordState::OnBackToLGW(void)
{
    return SMOperatorPasswordStateBase::OnBackToLGW();
}

void SMOperatorPasswordState::EndOfTransactionPrint()
{
    SMOperatorPasswordStateBase::EndOfTransactionPrint();
}

void SMOperatorPasswordState::promptForOperator()
{
    SMOperatorPasswordStateBase::promptForOperator();
}

void SMOperatorPasswordState::promptForPassword()
{
    if( co.fStateDualLanguage )
    {
        ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
    }
    
    inputMode = PROMPTING_FOR_PASSWORD;
    saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
    ps.SetCMFrame( _T("EnterID") );	// EnterPassword //
    ps.SetCMLeadthruText( TXT_ENTER_PASSWORD );
    
    //ps.ShowNumericKeypad();
    // Ensure the keypad is displayed on the password screen
    m_pAppPSX->SetConfigProperty(_T("NumericP1"), _T("EnterID"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
    m_pAppPSX->SetConfigProperty(_T("NumericP2"), _T("EnterID"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
    m_pAppPSX->SetConfigProperty(_T("NumericP3"), _T("EnterID"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
    m_pAppPSX->SetConfigProperty(_T("NumericP4"), _T("EnterID"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
	
	//(+)POS108118
    m_pAppPSX->SetConfigProperty(_T("Echo"), _T("EnterID"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
    m_pAppPSX->SetConfigProperty(_T("EchoField"), _T("EnterID"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
	
	m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("EnterID"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
	CString csNewPosition;
	csNewPosition = _T("0,262,520,65");
	m_pAppPSX->SetConfigProperty(_T("LeadthruText"), _T("EnterID"), UI::PROPERTYPOSITION, csNewPosition);
	
	csNewPosition = _T("0,396,510,250");
	m_pAppPSX->SetConfigProperty(_T("MessageBoxNoEcho"), _T("EnterID"), UI::PROPERTYPOSITION, csNewPosition);
	
	m_pAppPSX->SetConfigProperty(_T("LeadthruText"),  _T("EnterID"),UI::PROPERTYTEXTALIGNMENT, _T("1"));
	m_pAppPSX->SetConfigProperty(_T("MessageBoxNoEcho"),  _T("EnterID"),UI::PROPERTYTEXTALIGNMENT, _T("1"));
	
	//(-)POS108118
	
    
    bool bBiometEnabled = (SendTBCommand(TBCMD_IS_BIOMET_ENABLED) == TB_SUCCESS_IMMEDIATE);
    int iMessage = bBiometEnabled ? MSG_ENTERPASSWORD_ORUSEBIOMETRIC : MSG_ENTERPASSWORD_PRESSENTER;
    ps.Message12(iMessage, false );
    m_pAppPSX->SetTransactionVariable( _T("NextGenData"), COleVariant(_T("EnterPassword")) );//SSCOI-45123
	
    ps.ShowCMTBText( csTBMessage2Scot );	//thp
    ps.SetCMGobackButtonVisible( true );
    ps.AllowInput( co.GetnOperationsOperatorIDMaxDigit(), true, false );
    ps.ShowCMFrame( false );
}

bool SMOperatorPasswordState::getOperatorPassword()
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
        // If password entered, validate operator and password
        if (csPassword != _T("") || g_csPassword != _T(""))        
		{            
			//(+) CRD 351393 / POS99103 - To identify if the login is a badge and if so, we query employee ID for this badge and use this employee ID for subsequent calls to TMS.
			CString csEmplid;			
            if(csOperator.Find(_T("TM")) >= 0)
            {
                bool bRet = SendTBCommand(TBCMD_GET_EMPLID_FROM_BADGE, (const TCHAR *)csOperator, &csEmplid);
                csOperator = csEmplid;
            }
			//(-) CRD 351393 / POS99103 - To identify if the login is a badge and if so, we query employee ID for this badge and use this employee ID for subsequent calls to TMS.

            //+SR93 Biometrics	
	
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
				if (co.m_bOperationsUseFingerPrintDevice )
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

				if(SMState::StoreAssistRequestedTime != NULL)
				{
					//StoreAssistRequestedTime is not null means it was initialized during store assistance needed prompt. Calculate the timespan from the time assistance requested
					//and successful sign-in
					CTimeSpan tTimeElapsed = CTime::GetCurrentTime() - SMState::StoreAssistRequestedTime;
					//Update the wait times for all the outstanding interventions
					ra.UpdateInterventionWaitTimes(tTimeElapsed.GetTotalSeconds());
					SMState::StoreAssistRequestedTime = NULL;					
				}

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

void SMOperatorPasswordState::ShowEnterID()
{
    SMOperatorPasswordStateBase::ShowEnterID();
}

SMStateBase * SMOperatorPasswordState::OnSecMgrNeedData(CString exceptionType, long lItemID)
{
    return SMOperatorPasswordStateBase::OnSecMgrNeedData( exceptionType, lItemID);
}


SMStateBase* SMOperatorPasswordState::HandleScannerData(const MessageElement* message)
{
    return SMOperatorPasswordStateBase::HandleScannerData(message);
}

//POs81634 - rrs
SMStateBase * SMOperatorPasswordState::PSButton5()
{
    trace(L6, _T("SMOperatorPasswordState::PSButton5"));
    return SMOperatorPasswordStateBase::PSEnterKey();
}
//e - POS81634

bool SMOperatorPasswordState::GetUserInfo()
{
    return SMOperatorPasswordStateBase::GetUserInfo();
}

bool SMOperatorPasswordState::OnUserCredentialsAquired()
{
    return SMOperatorPasswordStateBase::OnUserCredentialsAquired();
}

SMStateBase *SMOperatorPasswordState::DMScannerEncrypted(void) //sscoadk-7272
{
    return SMOperatorPasswordStateBase::DMScannerEncrypted();
}