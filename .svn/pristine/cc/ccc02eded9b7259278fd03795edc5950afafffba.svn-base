//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMVoidApproval.CPP
//
// TITLE: Class header for PickingUpItems
//
// AUTHOR:    Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMVoidApproval.h"      
#include "transbrokerssf.h"
#include "SMSmGenericDelayed.h"	//CRD 185620

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("VoidApproval")


IMPLEMENT_DYNCREATE(SMVoidApproval, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP



//////////////////////////////////////////
SMVoidApproval::SMVoidApproval()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase *SMVoidApproval::Initialize(void)
{
    // POS30730 - Remove the whitespace as the property "WICFreeItemPhasActive" doesn't have white space
    // POS29825 - use the WICFreeItemPhaseActive property to not display the approval when POS does the auto void during free item phase - rrs
    if ( (GetHookProperty(_T("WICFreeItemPhaseActive")) == _T("1")) )
    {
        // POS19072: The WIC free item phase is active, so do not go thru void approval.
        trace(L7, _T("WIC free item phase is active; Void approval skipped"));
        return STATE_RETURN;
    }
	
	SendTBCommand(TBCMD_NOTIFY_STOREMODE_PROMPT);	//CRD 338206
	
    return SMVoidApprovalBase::Initialize();
}

void SMVoidApproval::UnInitialize(void)
{
    trace(L6, _T("WICFreeItemPhaseActive = %s"), GetHookProperty(_T("WICFreeItemPhaseActive")));
	
    // POS30730 - Remove the whitespace as the property "WICFreeItemPhasActive" doesn't have white space
    //      the "WICFreeItemPhaseActive" doesn't have a white space in the applicationmodel.xml
    // POS19072: Skip base functionality if the WIC free item phase is active.
    if ( (GetHookProperty(_T("WICFreeItemPhaseActive")) == _T("0")) )
    {        
        SMVoidApprovalBase::UnInitialize();
    }    
}

bool SMVoidApproval::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMVoidApprovalBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase *SMVoidApproval::OnWtIncreaseNotAllowed(void)
{
    return SMVoidApprovalBase::OnWtIncreaseNotAllowed();
}

SMStateBase *SMVoidApproval::OnWtDecrease(void)
{
    return SMVoidApprovalBase::OnWtDecrease();
}

void SMVoidApproval::showScreen(bool passwordVerified)
{
    CString csWork;
    nTimeOut = co.getTimeEASDeactivationScreenTimer();
    
    if (passwordVerified)
        return;
    
    if (inputMode == PROMPTING_FOR_OPERATOR)
    {
        STATE(OperatorPasswordState)::ShowEnterID();	//SR93 Biometrics support					
		
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
		
        // from previous SSF
        CString csWork = ps.GetPSText(BTT_ITEMCANCEL);
		csWork += _T("\n* ") + ps.GetPSText(TXT_VOIDEDITEMAPPROVAL);
        ps.Message12(csWork, true);
        
        return;
    }
    else
    {  // Display Wait for Approval....   
        DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
        ps.SetCMFrame(_T("VoidApproval"));
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
	
    csWork = ps.GetPSText(BTT_ITEMCANCEL);
    csWork += _T("\n* ") + ps.GetPSText(TXT_VOIDEDITEMAPPROVAL);
    ps.Message12(csWork, true);
	SMState::StoreAssistRequestedTime = CTime::GetCurrentTime();
}

SMStateBase *SMVoidApproval::stateAfterPasswordVerify()
{
    return SMVoidApprovalBase::stateAfterPasswordVerify();
}

SMStateBase *SMVoidApproval::TimedOut(void)
{
    return SMVoidApprovalBase::TimedOut();
}

SMStateBase *SMVoidApproval::PSButtonGoBack(void)
{
    return SMVoidApprovalBase::PSButtonGoBack();
}

SMStateBase *SMVoidApproval::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
	SMStateBase *pRet = SMVoidApprovalBase::OnRAPClearSecurityBtn(lpszAuthorizationType);

	//CRD 185620 if no more unapproved EAS items after voiding, them complete the intervention
	if(!lUnapprovedEASItems && bEASApproveFromVoid)
	{
		delete getRealStatePointer(pRet);
		m_GenericDelayedInterventionStatus = GENERIC_DELAYED_COMPLETE;
		SendTBCommand(TBCMD_EASCOMPLETE, false);
		RETURNSTATE(SmGenericDelayed);
	}	// (-) VSTS 131568
	else
		return pRet;
}
