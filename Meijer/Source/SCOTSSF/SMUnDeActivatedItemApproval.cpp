//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMUnDeActivatedItemApproval.CPP
//
// TITLE:    Class implementation for SMUnDeActivatedItemApproval
//
// AUTHOR:   Anurag Mishra
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMUnDeActivatedItemApproval.h"

#include "transbrokerssf.h"

#define COMP_ID ID_SM                        // Base state component
#define T_ID    _T("UnDeActivatedItemApproval")


IMPLEMENT_DYNCREATE(SMUnDeActivatedItemApproval, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

 

//////////////////////////////////////////
SMUnDeActivatedItemApproval::SMUnDeActivatedItemApproval()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase *SMUnDeActivatedItemApproval::Initialize(void)
{
    return SMUnDeActivatedItemApprovalBase::Initialize();
}

void SMUnDeActivatedItemApproval::UnInitialize(void)
{
    SMUnDeActivatedItemApprovalBase::UnInitialize();
}

bool SMUnDeActivatedItemApproval::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMUnDeActivatedItemApprovalBase::DeliverDescriptorList(dlParameterList);
}

void SMUnDeActivatedItemApproval::showScreen(bool passwordVerified)
{
    CString csWork;
    nTimeOut = co.getTimeEASDeactivationScreenTimer();
    if (passwordVerified)
        return;
	
    if (inputMode == PROMPTING_FOR_OPERATOR)
    {
        STATE(OperatorPasswordState)::ShowEnterID();	 //SR93 Biometrics support		
		
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
        
        return;
    }
    else
    {  // Display Wait for Approval....
        DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
        ps.SetCMFrame(_T("UnDeActivatedItemApproval"));
        ps.CMButton(_T("ButtonStoreLogIn"), BTT_STORELOGIN, true);
    }
    
    ps.SetCMLeadthruText(LTT_WAITFORASSISTANCE);
    // End NewUI
    
    SAWLDBScannerDisable();			// Disable the scanner
    ps.ShowCMTBText(csTBMessage2Scot);	//thp
    csWork = ps.GetPSText(MSG_WAITFORASSISTANCE);
    csWork += _T("\n* ") + ps.GetPSText(RA_THIRDLINE_UNDEACTIVATED);
    ps.Message12(csWork, true);
    trace(L7, _T("+ra.OnTransactionState, Itemization State"));
    ra.OnSetTransactionState(RA_STATE_WAIT_FOR_APPROVAL);
    trace(L7, _T("-ra.OnTransactionState"));
    ps.ShowCMFrame(false);
	SMState::StoreAssistRequestedTime = CTime::GetCurrentTime();
}

SMStateBase *SMUnDeActivatedItemApproval::stateAfterPasswordVerify()
{
    return SMUnDeActivatedItemApprovalBase::stateAfterPasswordVerify();
}

SMStateBase *SMUnDeActivatedItemApproval::TimedOut(void)
{
    return SMUnDeActivatedItemApprovalBase::TimedOut();
}

SMStateBase *SMUnDeActivatedItemApproval::PSButtonGoBack(void)
{
    return SMUnDeActivatedItemApprovalBase::PSButtonGoBack();
}

SMStateBase *SMUnDeActivatedItemApproval::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
    return SMUnDeActivatedItemApprovalBase::OnRAPClearSecurityBtn(lpszAuthorizationType);
}

SMStateBase *SMUnDeActivatedItemApproval::OnUnDeActivatedTheftTag(void)
{
    return SMUnDeActivatedItemApprovalBase::OnUnDeActivatedTheftTag();
}

SMStateBase *SMUnDeActivatedItemApproval::OnMatchedWt(void)
{
    return SMUnDeActivatedItemApprovalBase::OnMatchedWt();
}