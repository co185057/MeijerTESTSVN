//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCollectGiftCard.cpp
//
// TITLE: Class header for CollectGiftCard state
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMCollectGiftCard.h"
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SMCollectGiftCard")


IMPLEMENT_DYNCREATE(SMCollectGiftCard, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP



//////////////////////////////////////////
SMCollectGiftCard::SMCollectGiftCard()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMCollectGiftCard::Initialize(void)
{
    return SMCollectGiftCardBase::Initialize();
}

void SMCollectGiftCard::UnInitialize(void)
{
    SMCollectGiftCardBase::UnInitialize();
}

bool SMCollectGiftCard::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMCollectGiftCardBase::DeliverDescriptorList(dlParameterList);
}

void SMCollectGiftCard::showScreen(bool passwordVerified)
{
    CString csWork;
    nTimeOut = co.getTimeEASDeactivationScreenTimer();
    
    if (passwordVerified)
        return;
    
    if (inputMode == PROMPTING_FOR_OPERATOR)
    {
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
        ps.AllowInput( co.GetnOperationsOperatorIDMaxDigit(), false, false );
        ps.ShowCMFrame( false );  
        
        return;
    }
    else
    {  // Display Assistance Needed for Approval....  
        DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
        ps.SetCMFrame(_T("CollectGiftCard"));
        ps.CMButton(_T("ButtonStoreLogIn"), BTT_STORELOGIN, true);
    }
    
    
    ps.SetCMLeadthruText(LTT_WAITFORASSISTANCE);
    
    SAWLDBScannerDisable();	
    ps.ShowCMTBText(csTBMessage2Scot);	
    csWork = ps.GetPSText(MSG_WAITFORASSISTANCE);
    csWork += _T("\n* ") + ps.GetPSText(MSG_COLLECTGIFTCARD);
    ps.Message12(csWork, true);

	SMState::StoreAssistRequestedTime = CTime::GetCurrentTime();
    ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
    
    trace(L7, _T("+ra.OnTransactionState, Wait For Approval State"));
    ra.OnSetTransactionState(RA_STATE_WAIT_FOR_APPROVAL);
    trace(L7, _T("-ra.OnTransactionState"));
    ps.ShowCMFrame(false);
}

SMStateBase  *SMCollectGiftCard::stateAfterPasswordVerify()
{
    return SMCollectGiftCardBase::stateAfterPasswordVerify();
}

SMStateBase  *SMCollectGiftCard::TimedOut(void)
{
    return SMCollectGiftCardBase::TimedOut();
}

SMStateBase  *SMCollectGiftCard::PSButtonGoBack(void)
{
    return SMCollectGiftCardBase::PSButtonGoBack();
}

SMStateBase  *SMCollectGiftCard::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
    return SMCollectGiftCardBase::OnRAPClearSecurityBtn(lpszAuthorizationType);
}

SMStateBase  *SMCollectGiftCard::OnWtIncreaseNotAllowed(void)
{
    return SMCollectGiftCardBase::OnWtIncreaseNotAllowed();
}

SMStateBase  *SMCollectGiftCard::OnWtDecrease(void)
{
    return SMCollectGiftCardBase::OnWtDecrease();
}
