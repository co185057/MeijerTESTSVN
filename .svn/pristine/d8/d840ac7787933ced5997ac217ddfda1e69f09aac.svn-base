//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMVoidTarnsApproval.CPP
//
// TITLE: Class header for Void transaction
//
// AUTHOR:    Aiqin Hou
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMVoidTransApproval.h"      

#include "transbrokerssf.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("VoidTransApproval")


IMPLEMENT_DYNCREATE(SMVoidTransApproval, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP



//////////////////////////////////////////
SMVoidTransApproval::SMVoidTransApproval()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase *SMVoidTransApproval::Initialize(void)
{
    return SMVoidTransApprovalBase::Initialize();
}

void SMVoidTransApproval::UnInitialize(void)
{
    SMVoidTransApprovalBase::UnInitialize();
}

bool SMVoidTransApproval::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMVoidTransApprovalBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase *SMVoidTransApproval::OnWtIncreaseNotAllowed(void)
{
    return SMVoidTransApprovalBase::OnWtIncreaseNotAllowed();
}

SMStateBase *SMVoidTransApproval::OnWtDecrease(void)
{
    return SMVoidTransApprovalBase::OnWtDecrease();
}

SMStateBase *SMVoidTransApproval::stateAfterPasswordVerify()
{
    return SMVoidTransApprovalBase::stateAfterPasswordVerify();
}

void SMVoidTransApproval::ClearSecurityViolationLight(void)
{
    SMVoidTransApprovalBase::ClearSecurityViolationLight();
}

void SMVoidTransApproval::showScreen(bool passwordVerified)
{
    CString csWork;
    nTimeOut = co.getTimeEASDeactivationScreenTimer();
    if (passwordVerified)
        return;
    
    
    if (inputMode == PROMPTING_FOR_OPERATOR)
    {
        STATE(OperatorPasswordState)::ShowEnterID();  //SR93 Biometrics support			
		
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
        ps.ShowCMFrame( false );  
        
        // Appended from previous SSF
        CString csWork = ps.GetPSText(MSG_WAITFORASSISTANCE);
        csWork += _T("\n* ") + ps.GetPSText(TXT_CANCELALL);
        ps.Message12(csWork, true);
        
        return;
    }
    else
    {  // Display Assistance Needed for Approval....  
        DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
        ps.SetCMFrame(_T("VoidTransApproval"));
        ps.CMButton(_T("ButtonStoreLogIn"), BTT_STORELOGIN, true);
    }
    
    
    ps.SetCMLeadthruText(LTT_WAITFORASSISTANCE);
    // End NewUI
    
    SAWLDBScannerDisable();			// Disable the scanner
    ps.ShowCMTBText(csTBMessage2Scot);	//thp
    csWork = ps.GetPSText(MSG_WAITFORASSISTANCE);
    csWork += _T("\n* ") + ps.GetPSText(TXT_VOIDEDITEMAPPROVAL);
    ps.Message12(csWork, true);
    trace(L7, _T("+ra.OnTransactionState, Itemization State"));
    ra.OnSetTransactionState(RA_STATE_WAIT_FOR_APPROVAL);
    trace(L7, _T("-ra.OnTransactionState"));
    ps.ShowCMFrame(false);
    
    // Appended from previous SSF
    csWork = ps.GetPSText(MSG_WAITFORASSISTANCE);
    csWork += _T("\n* ") + ps.GetPSText(TXT_CANCELALL);
    ps.Message12(csWork, true);
	SMState::StoreAssistRequestedTime = CTime::GetCurrentTime();
}

SMStateBase *SMVoidTransApproval::TimedOut(void)
{
    return SMVoidTransApprovalBase::TimedOut();
}

SMStateBase *SMVoidTransApproval::PSButtonGoBack(void)
{
    return SMVoidTransApprovalBase::PSButtonGoBack();
}

SMStateBase *SMVoidTransApproval::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
    return SMVoidTransApprovalBase::OnRAPClearSecurityBtn(lpszAuthorizationType);
}

SMStateBase *SMVoidTransApproval::HandleCancelTrasaction(CString csBttID)
{
    return SMVoidTransApprovalBase::HandleCancelTrasaction(csBttID);
}