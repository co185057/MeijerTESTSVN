//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCMOperatorPWState.cpp
//
// Description: This state handles athentication in Cash Management.
//
// AUTHOR: Jeffrey Yu
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"                 // MS MFC always needed first

#include "Common.h"
#include "SMCMOperatorPWState.h"
#include "transbrokerssf.h" // POS305393 / SSCOI-47284

#define COMP_ID ID_SM               // base state component
#define T_ID _T("CMOperatorPWState")

IMPLEMENT_DYNCREATE(SMCMOperatorPWState, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////////////
SMCMOperatorPWState::SMCMOperatorPWState()
: SMCMOperatorPWStateBase()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase *SMCMOperatorPWState::Initialize(void)
{
    return SMCMOperatorPWStateBase::Initialize();
}

void SMCMOperatorPWState::UnInitialize(void)
{
    SMCMOperatorPWStateBase::UnInitialize();
}

SMStateBase * SMCMOperatorPWState::createLevel1Message( LPCTSTR szMsg,
                                           PSTEXT pstMsg,
                                           PSTEXT prompt,
                                           long   value,
                                           long   device,
                                           bool   checkHealth,
                                           bool displayHTML)
{
    return SMCMOperatorPWStateBase::createLevel1Message( szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML);
}

SMStateBase * SMCMOperatorPWState::createLevel2Message( LPCTSTR szMsg,
                                           PSTEXT pstMsg,
                                           PSTEXT prompt,
                                           long   value,
                                           long   device,
                                           bool   checkHealth,
                                           bool displayHTML)
{
    return SMCMOperatorPWStateBase::createLevel2Message( szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML);
}

SMStateBase * SMCMOperatorPWState::createLevel3Message( LPCTSTR szMsg,
                                           PSTEXT pstMsg,
                                           PSTEXT prompt,
                                           long   value,
                                           long   device,
                                           bool   checkHealth, 
                                           bool displayHTML )
{
    return SMCMOperatorPWStateBase::createLevel3Message( szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML );
}

SMStateBase *SMCMOperatorPWState::stateAfterPasswordVerify()
{
    return SMCMOperatorPWStateBase::stateAfterPasswordVerify();
}

bool SMCMOperatorPWState::parentClassCurrentlyHandlingButton2()
{
    return SMCMOperatorPWStateBase::parentClassCurrentlyHandlingButton2();
}

bool SMCMOperatorPWState::parentClassCurrentlyHandlingCancelKey()
{
    return SMCMOperatorPWStateBase::parentClassCurrentlyHandlingCancelKey();
}

void SMCMOperatorPWState::showScreen(bool passwordVerified)
{
    // +POS305393 / SSCOI-47284
    if (!passwordVerified)
    {
        if (inputMode == PROMPTING_FOR_OPERATOR)
        { 
            saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
			//+SR93 Biometrics support		
			STATE(OperatorPasswordState)::ShowEnterID();
			ps.SetCMLeadthruText( TXT_ENTER_ID );
			
			ps.AllowInput( co.GetnOperationsOperatorIDMaxDigit(), false, false );
			//-SR93 Biometrics support
			
			
			// Determine the leadthru text to show
			int iMessage = MSG_ENTERID_PRESSENTER;
			COleVariant v = VARIANT_TRUE;
			bool bEmplid = (SendTBCommand(TBCMD_IS_EMPLID_ENABLED) == TB_SUCCESS_IMMEDIATE);
			if( bEmplid )
			{
				bool bAllowHandKey = (SendTBCommand(TBCMD_IS_EMPLID_ALLOWHANDKEY) == TB_SUCCESS_IMMEDIATE);
				iMessage = bAllowHandKey ? MSG_ENTERID_SCANIDORKEY : MSG_ENTERID_SCANIDONLY;
				v = bAllowHandKey ? VARIANT_TRUE : VARIANT_FALSE;
				
				//(+)POS108118
				if(!bAllowHandKey)
				{
					CString csNewPosition;
					csNewPosition = _T("400,262,520,65");
					m_pAppPSX->SetConfigProperty(_T("LeadthruText"), _T("EnterID"), UI::PROPERTYPOSITION, csNewPosition);
					
					csNewPosition = _T("265,396,510,250");
					m_pAppPSX->SetConfigProperty(_T("MessageBoxNoEcho"), _T("EnterID"), UI::PROPERTYPOSITION, csNewPosition);
					
					m_pAppPSX->SetConfigProperty(_T("LeadthruText"),  _T("EnterID"),UI::PROPERTYTEXTALIGNMENT, _T("0"));
					m_pAppPSX->SetConfigProperty(_T("MessageBoxNoEcho"),  _T("EnterID"),UI::PROPERTYTEXTALIGNMENT, _T("0"));
				}
				//(-)POS108118
			}
			
			m_pAppPSX->SetConfigProperty(_T("NumericP1"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
			m_pAppPSX->SetConfigProperty(_T("NumericP2"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
			m_pAppPSX->SetConfigProperty(_T("NumericP3"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
			m_pAppPSX->SetConfigProperty(_T("NumericP4"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
			
			//(+)POS108118
			m_pAppPSX->SetConfigProperty(_T("Echo"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
			m_pAppPSX->SetConfigProperty(_T("EchoField"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
			//(-)POS108118
			
			m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
			
			ps.Message12(iMessage, false);
			//ps.AllowInput( MAX_DIGITS_OPERATOR, false, false );
			ps.ShowCMTBText(csTBMessage2Scot);
			ps.ShowCMFrame( false );
        return;
        }
    }
    else
    {
        trace(L6, _T("SMCMOperatorPWStateBase::showScreen password verified"));        
    }
    // -POS305393 / SSCOI-47284
}

SMStateBase * SMCMOperatorPWState::DMScanner()
{
   return SMCMOperatorPWStateBase::DMScanner();
}

SMStateBase * SMCMOperatorPWState::PSEnterKey()
{
	// (-) POS309451 / SSCOI-47853
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
					CString strEntryName(_T("")), strGrocery, strGM, strScanPortal, strRAPID;
					
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
					//trace(L6, _T("Please DELETE THIS LINE, Operator(%s), Password(%s)"), csOperator, csLocalPassword);
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
								(strScanPortal.Find(csRAPOriginID) != -1 && strScanPortal.Find(csRAPCurrentID) != -1))
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
									csCurrentVirtualRAPID = GetHookProperty(csRAPCurrentID); //CRD 345698
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
							csCurrentVirtualRAPID = GetHookProperty(csRAPCurrentID);	//CRD 345698
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
						 return SMCMOperatorPWStateBase::PSEnterKey();
					}
					else
					{
						trace(L6, _T("CheckIn Not Succesfull, pass it through"));
					}              
				}
				else 
				{
					trace(L6, _T("RAP Attendant Tracking is  - %d"), bAttendantTrackingEnabled);
					return SMCMOperatorPWStateBase::PSEnterKey();
				}
            }
            
            
        }
        else
        {
            return SMCMOperatorPWStateBase::PSEnterKey();
        }        
    }

    return STATE_NULL;
	// (-) POS309451 / SSCOI-47853
}

SMStateBase * SMCMOperatorPWState::PSNumericKey(const long lKey)
{
   return SMCMOperatorPWStateBase::PSNumericKey(lKey);
}

SMStateBase * SMCMOperatorPWState::PSClearKey()
{
   return SMCMOperatorPWStateBase::PSClearKey();
}

SMStateBase * SMCMOperatorPWState::PSButton1()
{
   return SMCMOperatorPWStateBase::PSButton1();
}

SMStateBase * SMCMOperatorPWState::PSButton2()
{
   return SMCMOperatorPWStateBase::PSButton2();
}

SMStateBase * SMCMOperatorPWState::PSCancelKey()
{
   return SMCMOperatorPWStateBase::PSCancelKey();
}

SMStateBase * SMCMOperatorPWState::PSButtonGoBack()
{
   return SMCMOperatorPWStateBase::PSButtonGoBack();
}

SMStateBase * SMCMOperatorPWState::displayInvalidMessage()
{
   return SMCMOperatorPWStateBase::displayInvalidMessage();
}

SMStateBase * SMCMOperatorPWState::OnWtIncreaseNotAllowed(void)
{
   return SMCMOperatorPWStateBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMCMOperatorPWState::OnWtDecrease(void)
{
   return SMCMOperatorPWStateBase::OnWtDecrease();
}

SMStateBase * SMCMOperatorPWState::TareWeightedItem(void)
{
   return SMCMOperatorPWStateBase::TareWeightedItem();
}

SMStateBase * SMCMOperatorPWState::TareOnly(void)
{
   return SMCMOperatorPWStateBase::TareOnly();
}

SMStateBase * SMCMOperatorPWState::TareWLDBItem(void)
{
   return SMCMOperatorPWStateBase::TareWLDBItem();
}

SMStateBase * SMCMOperatorPWState::TareNone(void)
{
   return SMCMOperatorPWStateBase::TareNone();
}

SMStateBase * SMCMOperatorPWState::TareOnlyAtAttract(void)
{
   return SMCMOperatorPWStateBase::TareOnlyAtAttract();
}

void SMCMOperatorPWState::promptForOperator()
{
    SMCMOperatorPWStateBase::promptForOperator();
}

void SMCMOperatorPWState::promptForPassword()
{
    // +POS305393 / SSCOI-47284
	if (co.fStateDualLanguage)
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
	m_pAppPSX->SetTransactionVariable( _T("NextGenData"), COleVariant(_T("EnterPassword")) );//SSCOI-45884
	
    ps.ShowCMTBText( csTBMessage2Scot );	//thp
	
    ps.AllowInput( co.GetnOperationsOperatorPWMaxDigit(), true, false );
    ps.ShowCMFrame( false );
    //SMCMOperatorPWStateBase::promptForPassword();
    // -POS305393 / SSCOI-47284
}

bool SMCMOperatorPWState::getOperatorPassword()
{
    return SMCMOperatorPWStateBase::getOperatorPassword();
}

void SMCMOperatorPWState::ShowEnterID()
{
    SMCMOperatorPWStateBase::ShowEnterID();
}

bool SMCMOperatorPWState::OnUserCredentialsAquired()
{
    return SMCMOperatorPWStateBase::OnUserCredentialsAquired();
}