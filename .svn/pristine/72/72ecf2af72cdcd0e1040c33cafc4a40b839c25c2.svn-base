//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStatePS.CPP
//
// TITLE: PS SMState functions, implement all SMState::PS* functions
//
//
// AUTHOR:  Peter Denhoed
//
// CRD124746 Work Request:Sprint 10.3 Name:Aparna Tunuguntla Date: March 19,2018
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "Common.h"
#include "PSProcedures.h"
#include "SMState.h"
#include "transbrokerssf.h"

PSProcedures myPS;
DLLEXPORT PSProcedures &ps = myPS;
DLLIMPORT extern HWND hw;   //CRD124746

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmStatePS")

#define RA_CAT_SCANNER_ACTIVATED DATANEEDED_LASTVIEW+2 //POS181201
#define INFO_BUFFER_SIZE 32767 // POS181195/POS181197
#ifdef _TRADESHOWDEMO
#include "..\TradeShowDemo\SMStatePSTSDemo.cpp"
#endif

bool SMState::PSRemoteLegalForState()
{
    return SMStateBase::PSRemoteLegalForState();
}

bool SMState::PSAssistMenuLegalForState()
{
    return SMStateBase::PSAssistMenuLegalForState();
}

SMStateBase * SMState::PSParse( const MessageElement* me)
{
    SMStateBase *retState = SMStateBase::PSParse(me);
	
	CString csEverseenEnabled = GetHookProperty(_T("EverseenEnabled"));	//CRD 313028
	CString csIsEverseenProcessing = GetHookProperty(_T("IsEverseenAlert"));	//CRD 347135
	
    switch(SMStateBase::nPSLastEvt)
    {
	case PS_RAPSCANNERENGAGED:
		{
		    //(+) CRD124746
			SMState::m_bActivateHHScanner = true;
			CString csCurrentStateName = this->GetRuntimeClass()->m_lpszClassName;
			trace(L6, _T("SMState Parse csCurrentStateName= %s "),csCurrentStateName);
			
			if( !ps.RemoteMode() )  
			{
				if ((csCurrentStateName.Find(_T("AssistMode")) != -1) || 
					(csCurrentStateName.Find(_T("AssistMenu")) != -1) || 
					(csCurrentStateName.Find(_T("ScanAndBag")) != -1) || 
					(csCurrentStateName.Find(_T("MobileAudit")) != -1) || //CRD189075
					((csCurrentStateName.Find(_T("SmDataEntry")) != -1) && csIsEverseenProcessing == _T("1")))  //CRD 347135
				{
					trace(L6, _T("SMState::Scanner Engaged "));
					m_pAppPSX->SetConfigProperty(_T("CMButton4MidiList"), _T("ContextHelp"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
					m_pAppPSX->SetConfigProperty(_T("CMButton4MidiList"), _T("ContextHelp"), UI::PROPERTYSTATE, (long) UI::Disabled);
					COleVariant v;
					v=ps.GetPSText(SSF_TXT_SCANNER_ACTIVE);
					m_pAppPSX->SetConfigProperty(_T("CMButton4MidiList"), _T("ContextHelp"), UI::PROPERTYTEXTFORMAT, v);
					//ra.OnNormalItem(_T("[highlight]") + 
					//	ps.GetPSText(SSF_TXT_SCANNER_ACTIVE));  //VSTS124746
					SendTBCommand(TBCMD_TLOG_OVERRIDE, rp.GetOperator(), RA_CAT_SCANNER_ACTIVATED, 1, 0, _ttol(rp.GetTerminalNumber()), 0);		//POS181201
					ps.AddMessageToReceipt(_T("CMSMReceipt"), ps.GetPSText(SSF_TXT_SCANNER_ACTIVE));  //CRD124746
					
					if(csCurrentStateName.Find(_T("ScanAndBag")) != -1)
					{
						StartHHScannerTimer();
						trace(L6, _T("*** Scanner engaged so started timer"));
					}

					//CRD 313028 - we don't have to disable security again if everseen is enabled
					if(csCurrentStateName.Find(_T("MobileAudit")) == -1 && !SMState::g_bInRecallMode && csEverseenEnabled == _T("0"))
					{
						
						// +CRD201407
						trace(L6, _T("SMState::Parse() Disabling Security when HHScanner is currently active"));
						ra.OnDisableSecurity();
						if(g_bIsSecurityDisableSubCheckOnly)
						{	
							co.SetSecurityDisableSubCheck(true);
							SetSecurityConfig(CONFIG_KEY_IS_SECURITYDISABLE_SUB_CHECK_ON, _T("true"));	
						}
						else
						{	
							co.SetSecurityDisable(true);
							
							CKeyValueParmList parmList;
							CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_ENTER_DISABLE_SECURITY_MODE);
							_bstr_t bstrParmList((const _TCHAR *)parmList);
							BSTR bstrResult = NULL;
							bstrResult = NULL;
							SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
							SysFreeString(bstrResult);	
						}

						if(csOperator != _T("")) 
						{ rp.SetOperator(csOperator); }
						rp.ModeChange(rp.m_lAttendantEntryID, SECURITYDISABLED, ENTER);
						// -CRD201407
					}
				}
				else
				{
					trace(L6, _T("*_* SMStatePS::PSParse(): Releasing HHScanner immediately when lane is not in correct state"));
					if(SMState::m_bActivateHHScanner)
					{
						
						SMState::m_bActivateHHScanner = false;
						CString csRemoteConnectionName = ra.GetRemoteConnectionName();
						CString csOperation=_T("operation=release-scanner;");
						m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), csOperation, csRemoteConnectionName );
						trace(L6, _T("Release scanner."));
					}
				}
            }
			//else // CRD196066: Remove the else statement here that releases the HHScanner causing the HHScanner not to work
			// when AM access on RAP.
		}
		break;
	case PS_RAPSCANNERAVAILABLE:
		{
			if(!ps.RemoteMode())
			{
				m_pAppPSX->SetConfigProperty(_T("CMButton4MidiList"), _T("ContextHelp"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
				m_pAppPSX->SetConfigProperty(_T("CMButton4MidiList"), _T("ContextHelp"), UI::PROPERTYSTATE, (long) UI::Normal);
				COleVariant v;
				v=ps.GetPSText(SSF_TXT_REQUEST_SCANNER);
				m_pAppPSX->SetConfigProperty(_T("CMButton4MidiList"), _T("ContextHelp"), UI::PROPERTYTEXTFORMAT, v);
			}
			//          SMState::m_bScannerUnavailable = false;
		}
		break;
	case PS_RAPSCANNERUNAVAILABLE:
		{
            COleVariant v;
			if(!ps.RemoteMode())
			{
				m_pAppPSX->SetConfigProperty(_T("CMButton4MidiList"), _T("ContextHelp"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
				m_pAppPSX->SetConfigProperty(_T("CMButton4MidiList"), _T("ContextHelp"), UI::PROPERTYSTATE, (long) UI::Disabled);
				v=ps.GetPSText(SSF_TXT_SCANNER_UNAVAILABLE);
				m_pAppPSX->SetConfigProperty(_T("CMButton4MidiList"), _T("ContextHelp"), UI::PROPERTYTEXTFORMAT, v);
			}
			else
			{
				v=ps.GetPSText(SSF_TXT_SCANNER_UNAVAILABLE);
				m_pAppPSX->SetConfigProperty(_T("Title2"), _T("SmAssistMode"), UI::PROPERTYTEXTFORMAT, v);
			}
			//            SMState::m_bScannerUnavailable = true;
			// +CRD197801: Display Scanner unavailable on e-receipt when RAP is in Full AM
			trace(L6, _T("*** Scanner not available"));
			ps.AddMessageToReceipt(_T("CMSMReceipt"), ps.GetPSText(SSF_TXT_SCANNER_UNAVAILABLE));
            // -CRD197801
			
		}
		break;
	case PS_RAPPRINTERBITMAPNOTLOADED:
	{
		if (ps.RemoteMode())
		{
			trace(L6, _T("RAP Event PS_RAPPRINTERBITMAPNOTLOADED has been received."));
			bRAPPrinterBitmapStatus = true; //RAP requesting the lane to load bitmap now
		}
	}
	break;
	case PS_RAPSCANNERDEACTIVATED:        //CRD149415
		{

			//if(!ps.RemoteMode())
			CString csCurrentStateName = this->GetRuntimeClass()->m_lpszClassName;
			trace(L6, _T("SMState Parse::PS_RAPSCANNERDEACTIVATED csCurrentStateName= %s "),csCurrentStateName);

			if ((csCurrentStateName.Find(_T("AssistMode")) != -1) || 
				(csCurrentStateName.Find(_T("AssistMenu")) != -1) || 
				(csCurrentStateName.Find(_T("ScanAndBag")) != -1) || 
				(csCurrentStateName.Find(_T("MobileAudit")) != -1) ||  //CRD189075
				((csCurrentStateName.Find(_T("SmDataEntry")) != -1) && csIsEverseenProcessing == _T("1")))  //CRD 347135
			{
				// +CRD209937: When ever lane received a deactivation message from RAP, display a Scanner Deactivate message.
				ps.AddMessageToReceipt(_T("CMSMReceipt"), ps.GetPSText(SSF_TXT_SCANNER_DEACTIVATED));
			}

			StopHHScannerTimer();
			
			// +CRD 225060: Enable security only when not in recall mode.
			//  CRD 313028: do not enable security if everseen is enabled

			//if security was disabled before && not in recall mode && not in mobile audit
			if((co.IsSecurityDisableOn() || co.GetSecurityDisableSubCheckOn()) && (!SMState::g_bInRecallMode) && (csCurrentStateName.Find(_T("MobileAudit")) == -1) && csEverseenEnabled == _T("0"))
			{
				trace(L6, _T("*** Scanner deactivated. Enable Security"));
				// +CRD201407
				ra.OnEnableSecurity();
				if(g_bIsSecurityDisableSubCheckOnly)
				{
					trace(L6, _T("call exit disable security when g_bIsSecurityDisableSubCheckOnly."));
					
					co.SetSecurityDisableSubCheck(false);
					SetSecurityConfig(CONFIG_KEY_IS_SECURITYDISABLE_SUB_CHECK_ON, _T("false"));
				}
				else
				{
					trace(L6, _T("call exit disable security when fStateSecurityDisableNoReset."));
					
					co.SetSecurityDisable(false);
					
					CKeyValueParmList parmList;
					CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_EXIT_DISABLE_SECURITY_MODE);
					_bstr_t bstrParmList((const _TCHAR *)parmList);
					BSTR bstrResult = NULL;
					bstrResult = NULL;
					SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
					SysFreeString(bstrResult);
				}

				if(csOperator != _T(""))
				{ rp.SetOperator(csOperator); }
				
				rp.ModeChange(rp.m_lAttendantEntryID, SECURITYDISABLED, EXIT);
				// -CRD201407
			}// -CRD225060
			SMState::m_bActivateHHScanner = false;
		}
		break;
	}
	
    return retState;
}

long SMState::PSInitialize(void)
{
    return SMStateBase::PSInitialize();
}

long SMState::PSUnInitialize(void)
{
    return SMStateBase::PSUnInitialize();
}

SMStateBase * SMState::PSButtonHelp(void)
{
    return SMStateBase::PSButtonHelp();
}

SMStateBase * SMState::PSButtonGoBack(void)
{
    return SMStateBase::PSButtonGoBack();
}

SMStateBase * SMState::PSButton1(void)
{
    return SMStateBase::PSButton1();
}

SMStateBase * SMState::PSButton2(void)
{
    return SMStateBase::PSButton2();
}

SMStateBase * SMState::PSButton3(void)
{
    return SMStateBase::PSButton3();
}

SMStateBase * SMState::PSButton4(void)
{
    return SMStateBase::PSButton4();
}

SMStateBase * SMState::PSButton5(void)
{
    return SMStateBase::PSButton5();
}

SMStateBase * SMState::PSButton6(void)
{
    return SMStateBase::PSButton6();
}

SMStateBase * SMState::PSButton7(void)
{
    return SMStateBase::PSButton7();
}

SMStateBase * SMState::PSButton8(void)
{
    return SMStateBase::PSButton8();
}

SMStateBase * SMState::PSMenuKey1(void)
{
    return SMStateBase::PSMenuKey1();
}

SMStateBase * SMState::PSMenuKey2(void)
{
    return SMStateBase::PSMenuKey2();
}

SMStateBase * SMState::PSMenuKey3(void)
{
    return SMStateBase::PSMenuKey3();
}

SMStateBase * SMState::PSMenuKey4(void)
{
    return SMStateBase::PSMenuKey4();
}

SMStateBase * SMState::PSMenuKey5(void)
{
    return SMStateBase::PSMenuKey5();
}

SMStateBase * SMState::PSMenuKey6(void)
{
    return SMStateBase::PSMenuKey6();
}

SMStateBase * SMState::PSMenuKey7(void)
{
    return SMStateBase::PSMenuKey7();
}

SMStateBase * SMState::PSMenuKey8(void)
{
    return SMStateBase::PSMenuKey8();
}

SMStateBase * SMState::PSMenuKey9(void)
{
    return SMStateBase::PSMenuKey9();
}

SMStateBase * SMState::PSMenuKey10(void)
{
    return SMStateBase::PSMenuKey10();
}

SMStateBase * SMState::PSMenuKey11(void)
{
    return SMStateBase::PSMenuKey11();
}

SMStateBase * SMState::PSMenuKey12(void)
{
    return SMStateBase::PSMenuKey12();
}

SMStateBase * SMState::PSMenuKey13(void)
{
    return SMStateBase::PSMenuKey13();
}

SMStateBase * SMState::PSMenuKey14(void)
{
    return SMStateBase::PSMenuKey14();
}

SMStateBase * SMState::PSMenuKey15(void)
{
    return SMStateBase::PSMenuKey15();
}

SMStateBase * SMState::PSAssistKey(const long nKey)
{
    return SMStateBase::PSAssistKey(nKey);
}

#ifndef _TRADESHOWDEMO
SMStateBase * SMState::PSCmdButton(CString csContext, CString csControl, CString csCommand)
{
    return SMStateBase::PSCmdButton(csContext, csControl, csCommand);
}
#endif //_TRADESHOWDEMO

SMStateBase * SMState::PSCmdList(CString csContext, CString csControl,  const long nIndex )
{
    return SMStateBase::PSCmdList(csContext, csControl,  nIndex );
}

SMStateBase * SMState::PSMainMenuKey(void)
{
    return SMStateBase::PSMainMenuKey();
}

SMStateBase * SMState::PSHelpTopics(void)
{
    return SMStateBase::PSHelpTopics();
}

SMStateBase * SMState::PSSwitchContext(BOOL bRemote, CString csNewContext)
{
    return SMStateBase::PSSwitchContext(bRemote, csNewContext);
}

SMStateBase * SMState::PSListLookup(CString csItemInfo)
{
    return SMStateBase::PSListLookup(csItemInfo);
}

SMStateBase * SMState::PSListLookupTab(LPCTSTR lpszTabValue) 
{
    return SMStateBase::PSListLookupTab(lpszTabValue);
}

SMStateBase * SMState::PSListScroll(const long lScroll)
{
    return SMStateBase::PSListScroll(lScroll);
}

SMStateBase * SMState::PSConnectRemote( CString& strMachineName, CString& strRemoteContext )
{
    return SMStateBase::PSConnectRemote( strMachineName, strRemoteContext );
}

SMStateBase * SMState::PSAlphaNumericKey(const long lKey)
{
    return SMStateBase::PSAlphaNumericKey(lKey);
}

SMStateBase * SMState::PSReceiptUp(void)
{
    return SMStateBase::PSReceiptUp();
}

SMStateBase * SMState::PSReceiptDown(void)
{
    return SMStateBase::PSReceiptDown();
}

SMStateBase * SMState::PSVideoDone(void)
{
    return SMStateBase::PSVideoDone();
}

SMStateBase * SMState::PSNumericKey(const long lKey)
{
    return SMStateBase::PSNumericKey(lKey);
}

SMStateBase * SMState::PSBackSpaceKey(void)
{
    return SMStateBase::PSBackSpaceKey();
}

SMStateBase * SMState::PSShiftKey(void)
{
    return SMStateBase::PSShiftKey();
}

SMStateBase * SMState::PSDecimalKey(const long ch)
{
    return SMStateBase::PSDecimalKey(ch);
}

SMStateBase * SMState::PSOtherSpecialKeys(const long ch)
{
    return SMStateBase::PSOtherSpecialKeys(ch);
}

SMStateBase * SMState::PSEnterKey(void)
{
    return SMStateBase::PSEnterKey();
}

SMStateBase * SMState::PSCancelKey(void)
{
    return SMStateBase::PSCancelKey();
}

SMStateBase * SMState::PSClearKey(void)
{
    return SMStateBase::PSClearKey();
}

SMStateBase * SMState::PSSpaceKey(void)
{
    return SMStateBase::PSSpaceKey();
}

SMStateBase * SMState::PSAssistAccepted(void)
{
    return SMStateBase::PSAssistAccepted();
}

SMStateBase * SMState::PSAssistDenied(void)
{
    return SMStateBase::PSAssistDenied();
}

SMStateBase * SMState::PSKeystroke(const long wParam, const long lParam)
{
    return SMStateBase::PSKeystroke(wParam, lParam);
}

SMStateBase * SMState::PSHotKey(CString & csHotKey) 
{
    return SMStateBase::PSHotKey(csHotKey) ;
}

SMStateBase * SMState::PSCharKey(TCHAR c)
{
    return SMStateBase::PSCharKey(c);
}

SMStateBase * SMState::PSAltGrKey(void)
{
    return SMStateBase::PSAltGrKey();
}

TCHAR SMState::VirtKeyToTChar(UINT vKey, long shiftState)
{
    return SMStateBase::VirtKeyToTChar(vKey, shiftState);
}

long SMState::PSEchoInput(const long lKey)
{
    return SMStateBase::PSEchoInput(lKey);
}

long SMState::PSEchoDecimalInput(const long lKey)
{
    return SMStateBase::PSEchoDecimalInput(lKey);
}

long SMState::PSClearInput(void)
{
    return SMStateBase::PSClearInput();
}

long SMState::PSBackSpaceInput(void)
{
    return SMStateBase::PSBackSpaceInput();
}

SMStateBase * SMState::PSClearAllInput(BOOL bSayValid)
{
    return SMStateBase::PSClearAllInput(bSayValid);
}

SMStateBase * SMState::PSAlphaKey(const long lKey)
{
    return SMStateBase::PSAlphaKey(lKey);
}

void SMState::VoidAllCoupons()
{
    SMStateBase::VoidAllCoupons();
}

long SMState::SetButton(LPCTSTR szControlName,
                        LPCTSTR     szContent,
                        const bool fEnable)
{
    return SMStateBase::SetButton(szControlName, szContent, fEnable);
}

void SMState::PSSortQtyList(CStringArray & csPickQtyList)
{
    SMStateBase::PSSortQtyList(csPickQtyList);
}

bool SMState::VerifyQtyItem(CString csItemCode)
{
    return SMStateBase::VerifyQtyItem(csItemCode);
}

bool SMState::SCOTIsMinimized()
{
    return SMStateBase::SCOTIsMinimized();
}

void SMState::SetFocusToVideo()
{
    SMStateBase::SetFocusToVideo();
}

SMStateBase * SMState::PSOnRemoteDisconnected(const long lParm)
{
    return SMStateBase::PSOnRemoteDisconnected(lParm);
}

SMStateBase * SMState::PSButtonToggle(void)
{
    return SMStateBase::PSButtonToggle();
}

SMStateBase * SMState::PSButtonLaneStatus(long lLaneIndex)
{
    return SMStateBase::PSButtonLaneStatus(lLaneIndex);
}

SMStateBase * SMState::OnExternalUIRequest(MessageElement* me)
{
    return SMStateBase::OnExternalUIRequest(me);
}

bool SMState::FindItemID(CString& csGetItemID, CString csReceipt, CString csReceiptSrc, long lExtPrice, long lUnitPrice, CString csItemCode)
{
    return SMStateBase::FindItemID(csGetItemID, csReceipt, csReceiptSrc, lExtPrice, lUnitPrice, csItemCode);
}

void SMState::TrimSymbology(CString& csItemCode)
{    
	if( (csItemCode.GetLength() > 2) &&
        (csItemCode.Left(3) == _T("]e0") || csItemCode.Left(3) == _T("]E0")) )
    {
        trace(L6, _T("SMState::TrimSymbology trimmed DataBar (RSS) symbology."));
        csItemCode = csItemCode.Mid(3); // Remove ']e0' DataBar (RSS) symbology
    }
	else if(csDMLastBarType == SCAN_SDT_UPCE_S && !_tcsicmp(co.csHWType, _T("SCOT6")))
	{
		//remove E
		if(csItemCode.Find(_T("E")) == 0)
			csItemCode = csItemCode.Right(csItemCode.GetLength() - 1);
		//the length of the code should be 10 for 2 digit supplement (i.e. 7+ 1 check digit + 2 supplement)
		//or 13 for 5 digit supplement (i.e. 7+ 1 check digit + 5 supplement) when the check digit is included for the
		//first bar code. We need to remove that check digit
		int length = csItemCode.GetLength();
		if(length == 10 || length == 13)
		{					
			csItemCode = csItemCode.Mid(0,7) + csItemCode.Mid(8, length - 8); //dropping the check digit if its present
			csDMLastBarCode = csDMLastBarLabel = csItemCode;
		}
	}
    else
    {
        SMStateBase::TrimSymbology(csItemCode);
    }
}

SMStateBase * SMState::HandleNewContext()
{
    return SMStateBase::HandleNewContext();
}

SMStateBase * SMState::PreNotifySCOTEvent2TB(const MessageElement *me, sExtStateEventNotifyIn& sNotifyIn, sExtStateEventNotifyOut& sNotifyOut)
{
    return SMStateBase::PreNotifySCOTEvent2TB(me, sNotifyIn, sNotifyOut);
}

SMStateBase * SMState::FillPSEvent2NotifyTB(const MessageElement* pEvent, sExtStateEventNotifyIn& sNotifyIn, sExtStateEventNotifyOut& sNotifyOut)
{
    return SMStateBase::FillPSEvent2NotifyTB(pEvent, sNotifyIn, sNotifyOut);;
}

void SMState::SetAlphaShiftState(CString csContext, bool bShiftOn)
{
    SMStateBase::SetAlphaShiftState(csContext, bShiftOn);
}

void SMState::BuildCmdButtonList(CSCOTDataEntry& cDataEntry, CString csCtxName, CString csCtlName)
{
    SMStateBase::BuildCmdButtonList(cDataEntry, csCtxName, csCtlName);
}

void SMState::BuildCmdList(CSCOTDataEntry& cDataEntry, const CString& csReceiptSrc)
{
    SMStateBase::BuildCmdList(cDataEntry,  csReceiptSrc);
}

void SMState::PSParseFinalProcessing(const MessageElement* me, SMStateBase* pRetState )
{
    SMStateBase::PSParseFinalProcessing(me, pRetState);
}

bool SMState::ShouldDiscardEvent(const PSInfo &psInfo) const
{
    return SMStateBase::ShouldDiscardEvent(psInfo);
}

//(+) CRD124746
VOID CALLBACK SMState::RAPHHScannerTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	// CRD196066: Release HHScanner when the timer passed out.
	KillTimer(hw,HHSCANNER_TIMER);

	// append the computer-name of the lane currently active HHScanner
	CString csCompName = SMState::GetLaneName();
    csCompName += _T(";");
	
    CString csOperation=_T("operation=release-scanner-");
    csOperation = csOperation + csCompName;
    CString csRAPName = SMState::GetRAPConnectionName();
	SMState::m_bActivateHHScanner = false;
    m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), csOperation, csRAPName );
	// - append
	
	//m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), _T("operation=release-scanner-"), ra.GetRemoteConnectionName() );
	trace(L6, _T("SMState::RAPHHScannerTimedOut(): Release scanner."));
}
//(-) CRD124746

// +CRD196066: Created a helper function to start/stop timer when HHScanner is deactivated or when HHScanner is idle.
void SMState::StartHHScannerTimer()
{
	CString csHHScannerTimeOut;
	int nHHScannerTimeOut;
	SendTBCommand(TBCMD_GET_OPTIONS, &CString(_T("HHScannerTimeOut")), &csHHScannerTimeOut);

	nHHScannerTimeOut = _ttoi(csHHScannerTimeOut);	
	SetTimer(hw, HHSCANNER_TIMER, nHHScannerTimeOut*1000, &RAPHHScannerTimedOut);
	trace(L6, _T("*** Scanner engaged so started timer"));
}

void SMState::StopHHScannerTimer()
{
	trace(L6, _T("*** stopped timer"));
	KillTimer(hw,HHSCANNER_TIMER);
}
// -CRD196066
//(-) CRD124746
//(-) VSTS124746

//UploadTicket Port +
void SMState::TrimSymbologyForSSF(CString& csItemCode)
{
    SMState::TrimSymbology(csItemCode);
}
//UploadTicket Port - 


