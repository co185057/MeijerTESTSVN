//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMFatalError.cpp
//
// TITLE: Class impliementation for fatal error state
//
//
// AUTHOR:    David Moore
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMFatalError.h"
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("FatalError")

IMPLEMENT_DYNCREATE(SMFatalError, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////
SMFatalError::SMFatalError()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase *SMFatalError::Initialize(void)
{
    return SMFatalErrorBase::Initialize();
}

void SMFatalError::UnInitialize(void)
{
    SMFatalErrorBase::UnInitialize();
}

SMStateBase *SMFatalError::OnWtIncreaseNotAllowed(void)
{
    return SMFatalErrorBase::OnWtIncreaseNotAllowed();
}

SMStateBase *SMFatalError::OnWtDecrease(void)
{
    return SMFatalErrorBase::OnWtDecrease();
}

SMStateBase *SMFatalError::PSButtonHelp(void)
{
    return SMFatalErrorBase::PSButtonHelp();
}

SMStateBase *SMFatalError::PSButton2(void)
{
    return SMFatalErrorBase::PSButton2();
}

SMStateBase *SMFatalError::PSButton3(void)
{
    return SMFatalErrorBase::PSButton3();
}

SMStateBase *SMFatalError::PSCancelKey(void)
{
    return SMFatalErrorBase::PSCancelKey();
}

SMStateBase *SMFatalError::PSListLookup(CString csItemInfo)
{
    return SMFatalErrorBase::PSListLookup(csItemInfo);
}

SMStateBase *SMFatalError::TimedOut(void)
{
    return SMFatalErrorBase::TimedOut();
}

SMStateBase *SMFatalError::stateAfterPasswordVerify(void)
{
    return SMFatalErrorBase::stateAfterPasswordVerify();
}

void SMFatalError::showScreen(bool passwordVerified)
{
    CString csWork;
    saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
    
    if (passwordVerified)
        return;
    
    if (inputMode == PROMPTING_FOR_OPERATOR)
    {
        saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
        ps.SetCMFrame( _T("EnterID") );
        ps.SetCMLeadthruText( TXT_ENTER_ID );
		
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
        
        ps.Message12(iMessage, false);
        // Leadthru text message set
		
        ps.AllowInput( co.GetnOperationsOperatorIDMaxDigit(), false, false );
        ps.ShowCMTBText(csTBMessage2Scot);
        ps.ShowCMFrame( false );
        return;
    }
    else
    {  // Display initial  screen. 
        // NewUI
        ps.SetCMFrame(_T("FatalError"));
        ps.SetButtonText(_T("CMButton2"), BTT_STORELOGIN);
        // End NewUI
    }
    
    
    ps.CMButtonFlash(_T("CMButton1"),true);   
    ps.ShowCMTBText(csTBMessage2Scot);	//thp
    // End NewUI	
    
    ps.SetCMLeadthruText(LTT_WAITFORASSISTANCE);
    // End NewUI
    SAWLDBScannerDisable();			// Disable the scanner
    ps.ShowCMTBText(csTBMessage2Scot);	//thp
    csWork = ps.GetPSText(MSG_WAITFORASSISTANCE);
    csWork += _T("\n* ") + ps.GetPSText(LTT_FATALERROR);
    ps.Message12(csWork, true);
    trace(L7, _T("+ra.OnTransactionState, Itemization State"));
    ra.OnSetTransactionState(RA_STATE_WAIT_FOR_APPROVAL);
    trace(L7, _T("-ra.OnTransactionState"));
    ps.ShowCMFrame(false);
	SMState::StoreAssistRequestedTime = CTime::GetCurrentTime();
}

SMStateBase *SMFatalError::PSButtonGoBack(void)
{
    return SMFatalErrorBase::PSButtonGoBack();
}
