//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSignCharge.CPP
//
// TITLE: Class implementation for sign charge slip state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSignCharge.h"           // MGV this state

#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SignCharge")

IMPLEMENT_DYNCREATE(SMSignCharge, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////
SMSignCharge::SMSignCharge()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase *SMSignCharge::Initialize(void)
{
    return SMSignChargeBase::Initialize();
}

void SMSignCharge::UnInitialize(void)
{
    SMSignChargeBase::UnInitialize();
}

SMStateBase *SMSignCharge::TimedOut(void)
{
    return SMSignChargeBase::TimedOut();
}

SMStateBase *SMSignCharge::OnWtIncreaseNotAllowed(void)
{
    return SMSignChargeBase::OnWtIncreaseNotAllowed();
}

SMStateBase *SMSignCharge::OnWtDecrease(void)
{
    return SMSignChargeBase::OnWtDecrease();
}

SMStateBase *SMSignCharge::stateAfterPasswordVerify(void)
{
    return SMSignChargeBase::stateAfterPasswordVerify();
}

void SMSignCharge::showScreen(bool passwordVerified)
{
    // This state does not need to reshow if the password
	// was verified, since we'll be leaving now
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
		
		ps.ShowAmountDueAmountPaid(lBalanceDue, lTotalPaid + nDMCashInserted);
		ps.ShowCMTBText(csTBMessage2Scot);	//thp
		
		ps.ShowCMFrame( false );           
		
		return;
	}
	else
	{  // Display initial screen.
        ps.SetCMFrame(_T("SignCharge"));
		ps.SetButtonText(_T("CMButton1StoreLogIn"), BTT_STORELOGIN);
		SAWLDBScannerDisable();		// disable scanner if ini parameter set to 0 or not present
		
	}
	
	
	ps.SetCMLeadthruText(LTT_SIGNCHARGE);
	// End NewUI
	
	ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	ps.Message12(MSG_SIGNRECEIPT, true);
  ps.ShowCMFrame(false);
}

SMStateBase *SMSignCharge::PSButton1(void)
{
    return SMSignChargeBase::PSButton1();
}
