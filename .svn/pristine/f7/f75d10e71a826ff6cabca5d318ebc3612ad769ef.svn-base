//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMContextHelp.cpp
//
// TITLE: Class impliementation for contextual help state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMContextHelp.h"
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("ContextHelp")

IMPLEMENT_DYNCREATE(SMContextHelp, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////
SMContextHelp::SMContextHelp()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase *SMContextHelp::Initialize(void)
{
	//(+) CRD188817
	SMStateBase* returnState = SMContextHelpBase::Initialize();

	m_pAppPSX->SetConfigProperty(_T("CMButton4MidiList"), _T("ContextHelp"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
	m_pAppPSX->SetConfigProperty(_T("CMButton4MidiList"), _T("ContextHelp"), UI::PROPERTYSTATE, (long) UI::Disabled);

	SendTBCommand(TBCMD_NOTIFY_STOREMODE_PROMPT);	//CRD 338206
    return returnState;
	//return SMContextHelpBase::Initialize();
	//(-) CRD188817
}

void SMContextHelp::UnInitialize(void)
{
    SMContextHelpBase::UnInitialize();
}

SMStateBase *SMContextHelp::OnWtIncreaseNotAllowed(void)
{
    return SMContextHelpBase::OnWtIncreaseNotAllowed();
}

SMStateBase *SMContextHelp::OnWtDecrease(void)
{
    return SMContextHelpBase::OnWtDecrease();
}

SMStateBase *SMContextHelp::PSButtonHelp(void)
{
    return SMContextHelpBase::PSButtonHelp();
}

SMStateBase *SMContextHelp::PSButton1(void)
{
    return SMContextHelpBase::PSButton1();
}

SMStateBase *SMContextHelp::PSButton2(void)
{
    return SMContextHelpBase::PSButton2();
}

SMStateBase *SMContextHelp::PSButton3()
{
    if (inputMode == PROMPTING_FOR_PASSWORD) //TAR 117419
	{
		promptForOperator();
	}
	else
	{
		showScreen(false);
		return STATE_NULL;
	}
	fFromRequestSignature = false;	//TAR#140156  LPM051000

	//Start CRD 366323
	NotifyExitStoreMode();
	//End CRD 366323

    return STATE_RETURN;
}

//(+) POS181195
SMStateBase *SMContextHelp::PSButton4()
{
    SMState::m_bActivateHHScanner = true;
    return SMContextHelp::PSButton2();
}
//(-) POS181195

SMStateBase *SMContextHelp::PSCancelKey(void)
{
    return SMContextHelpBase::PSCancelKey();
}

SMStateBase *SMContextHelp::PSListLookup(CString csItemInfo)
{
    return SMContextHelpBase::PSListLookup(csItemInfo);
}

SMStateBase *SMContextHelp::TimedOut(void)
{
    return SMContextHelpBase::TimedOut();
}

SMStateBase *SMContextHelp::stateAfterPasswordVerify(void)
{
    return SMContextHelpBase::stateAfterPasswordVerify();
}

void SMContextHelp::showScreen(bool passwordVerified)
{
    // This state does not need to reshow if the password
    // was verified, since we'll be leaving now
    saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
    
    if (passwordVerified)
        return;
    
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
        
        
        m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("EnterID"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
        ps.Message12(iMessage, false);
        //ps.AllowInput( MAX_DIGITS_OPERATOR, false, false );
        ps.ShowCMTBText(csTBMessage2Scot);
        ps.ShowCMFrame( false );
        return;
    }
    else
    {  // Display initial Help screen. 
        // NewUI
//(+) POS181195  //commented for CRD124746 
      /*  CString csPrevState = this->csLastNonHelpStateName;
        trace(L6, _T("SMContextHelp --> csPrevState = %s"), csPrevState);
        if(csPrevState == _T("SMScanAndBag"))
        {
        CString csCompName=_T("");

        csCompName = SMState::GetLaneName();
            csCompName += _T(";");

        CString csOperation=_T("operation=scanner-status-");
        csOperation = csOperation + csCompName;
        CString csRAPName = SMState::GetRAPConnectionName();

        m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), csOperation, csRAPName );            
        m_pAppPSX->SetConfigProperty(_T("CMButton4MidiList"), _T("ContextHelp"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
        }
        else
        {
      	    m_pAppPSX->SetConfigProperty(_T("CMButton4MidiList"), _T("ContextHelp"), UI::PROPERTYSTATE, (long) UI::Disabled);
        }*/
//(-) POS181195
        ps.SetCMFrame(_T("ContextHelp"));
        ps.SetButtonText(_T("CMButton2"), BTT_STORELOGIN);
        // End NewUI
        DMSayPhrase(CALLFORHELP);  //TAR 156263
    }
    
    
    ps.CMButtonFlash(_T("CMButton1"),true);   
    ps.ShowCMTBText(csTBMessage2Scot);	//thp
    ps.Message12(TXT_ENTER_OPERATOR_PROMPT, true);
    ps.ShowCMFrame(false);
    // End NewUI	
    
#ifdef _DEBUG
    CString csWork;
    csWork.Format(_T("\n\nCalling state: %s\nBMP file:  %s\nTimeout: %d; StoreMode: %c"),
        csLastNonHelpStateName,
        csLastNonHelpStateBMP,
        nLastNonHelpStateTimeOut,
        (fLastNonHelpStateStoreMode?_T('Y'):_T('N')));
    // ps.Message23(csWork);
#endif
    ps.ClearInput();
}

//(+) POS181195
SMStateBase *SMContextHelp::PSButtonGoBack(void)
{
    if(SMState::m_bActivateHHScanner)
    {
        SMState::m_bActivateHHScanner = false;
    }
	
    return SMContextHelpBase::PSButtonGoBack();
}
//(-) POS181195
