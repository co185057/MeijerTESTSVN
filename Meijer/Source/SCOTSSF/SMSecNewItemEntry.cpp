#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSecNewItemEntry.h"      

#include "transbrokerssf.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SecNewItemEntry")


IMPLEMENT_DYNCREATE(SMSecNewItemEntry, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP



//////////////////////////////////////////
SMSecNewItemEntry::SMSecNewItemEntry()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMSecNewItemEntry::Initialize(void)
{
    return SMSecNewItemEntryBase::Initialize();
}

void SMSecNewItemEntry::UnInitialize(void)
{
    SMSecNewItemEntryBase::UnInitialize();
}

bool SMSecNewItemEntry::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMSecNewItemEntryBase::DeliverDescriptorList(dlParameterList);
}

void SMSecNewItemEntry::showScreen(bool passwordVerified)
{
    nTimeOut = co.getTimeEASDeactivationScreenTimer();
    
    if (passwordVerified)
        return;
    
    
    if (inputMode == PROMPTING_FOR_OPERATOR)
    {
        STATE(SecOperatorPWState)::ShowEnterID();  //SR93 Biometrics support		
		
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
        DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));	// play wave here so it doesn't replay on the key pad screen
        // NewUI
        ps.SetCMFrame(_T("SecNewItemEntry"));
        ps.CMButton(_T("ButtonStoreLogIn"), BTT_STORELOGIN, true);
    }
    
    CString csWork;
    csWork = ps.GetPSText(MSG_WAITFORASSISTANCE);
    csWork += _T("\n* ") + ps.GetPSText(TXT_ITEM_WEIGHT_APPROVAL);
    
    ps.SetCMLeadthruText(LTT_WAITFORASSISTANCE);
    // NewUI
    
    SAWLDBScannerDisable();			// Disable the scanner
    ps.ShowCMTBText(csTBMessage2Scot);	//thp
    ps.Message12(csWork, true);
    trace(L7, _T("+ra.OnTransactionState, Itemization State"));
    ra.OnSetTransactionState(RA_STATE_SECURITY_VIOLATION);
    trace(L7, _T("-ra.OnTransactionState"));
    ps.ShowCMFrame(false);
}

SMStateBase  *SMSecNewItemEntry::stateAfterPasswordVerify()
{
    return SMSecNewItemEntryBase::stateAfterPasswordVerify();
}

SMStateBase  *SMSecNewItemEntry::OnWtIncreaseNotAllowed(void)
{
    return SMSecNewItemEntryBase::OnWtIncreaseNotAllowed();
}

SMStateBase  *SMSecNewItemEntry::OnWtDecrease(void)
{
    return SMSecNewItemEntryBase::OnWtDecrease();
}

SMStateBase  *SMSecNewItemEntry::TimedOut(void)
{
    return SMSecNewItemEntryBase::TimedOut();
}

SMStateBase  *SMSecNewItemEntry::PSButtonGoBack(void)
{
    return SMSecNewItemEntryBase::PSButtonGoBack();
}

SMStateBase  *SMSecNewItemEntry::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
    return SMSecNewItemEntryBase::OnRAPClearSecurityBtn(lpszAuthorizationType);
}

SMStateBase  *SMSecNewItemEntry::OnBackToLGW(void)
{
    return SMSecNewItemEntryBase::OnBackToLGW();
}
