#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSecItemRemovedThreshold.h"      

#include "transbrokerssf.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SecItemRemovedThreshold")


IMPLEMENT_DYNCREATE(SMSecItemRemovedThreshold, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP



//////////////////////////////////////////
SMSecItemRemovedThreshold::SMSecItemRemovedThreshold()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMSecItemRemovedThreshold::Initialize(void)
{
    return SMSecItemRemovedThresholdBase::Initialize();
}

void SMSecItemRemovedThreshold::UnInitialize(void)
{
    SMSecItemRemovedThresholdBase::UnInitialize();
}

bool SMSecItemRemovedThreshold::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMSecItemRemovedThresholdBase::DeliverDescriptorList(dlParameterList);
}

void SMSecItemRemovedThreshold::showScreen(bool passwordVerified)
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
        ps.SetCMFrame(_T("SecItemRemovedThreshold"));
        ps.CMButton(_T("ButtonStoreLogIn"), BTT_STORELOGIN, true);
    }
    
    CString csWork;
	
    // POS67184: Do not include assistance-needed text for the instruction area  
    // because the leadthru text already includes the assistance-needed text.
    //csWork = ps.GetPSText(MSG_WAITFORASSISTANCE);
    //csWork += _T("\n* ") + ps.GetPSText(TXT_ITEM_REMOVED_THRESHOLD);
	
    // Retrieve text for instruction area.
    csWork = _T("\n* ") + ps.GetPSText(TXT_ITEM_REMOVED_THRESHOLD);
    
    // Display leadthru containg assistance-needed text.
    ps.SetCMLeadthruText(LTT_WAITFORASSISTANCE);
    
    // End NewUI
    SAWLDBScannerDisable();			// Disable the scanner
    ps.ShowCMTBText(csTBMessage2Scot);	//thp
    ps.Message12(csWork, true); // Display text in instruction area.
    trace(L7, _T("+ra.OnTransactionState, Itemization State"));
    ra.OnSetTransactionState(RA_STATE_SECURITY_VIOLATION);
    trace(L7, _T("-ra.OnTransactionState"));
    ps.ShowCMFrame(false);
}

SMStateBase  *SMSecItemRemovedThreshold::stateAfterPasswordVerify()
{
    return SMSecItemRemovedThresholdBase::stateAfterPasswordVerify();
}

SMStateBase  *SMSecItemRemovedThreshold::TimedOut(void)
{
    return SMSecItemRemovedThresholdBase::TimedOut();
}

SMStateBase  *SMSecItemRemovedThreshold::PSButtonGoBack(void)
{
    return SMSecItemRemovedThresholdBase::PSButtonGoBack();
}

SMStateBase  *SMSecItemRemovedThreshold::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
    return SMSecItemRemovedThresholdBase::OnRAPClearSecurityBtn(lpszAuthorizationType);
}
