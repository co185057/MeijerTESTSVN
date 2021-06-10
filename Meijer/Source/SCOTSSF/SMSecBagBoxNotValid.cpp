//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecBagBoxNotValid.cpp
//
// TITLE: Class implimentation for Invalid Bag State - Added for SR085
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSecBagBoxNotValid.h"      

#include "transbrokerssf.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SecBagBoxNotValid")


IMPLEMENT_DYNCREATE(SMSecBagBoxNotValid, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP



//////////////////////////////////////////
SMSecBagBoxNotValid::SMSecBagBoxNotValid()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMSecBagBoxNotValid::Initialize(void)
{
    return SMSecBagBoxNotValidBase::Initialize();
}

void SMSecBagBoxNotValid::UnInitialize(void)
{
    SMSecBagBoxNotValidBase::UnInitialize();
}

bool SMSecBagBoxNotValid::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMSecBagBoxNotValidBase::DeliverDescriptorList(dlParameterList);
}

//POS105367
void SMSecBagBoxNotValid::showScreen(bool passwordVerified)
{
    nTimeOut = co.getTimeEASDeactivationScreenTimer();
	
	if (passwordVerified)
		return;
	
	
	if (inputMode == PROMPTING_FOR_OPERATOR)
	{	
		//Tar 393757
		STATE(SecOperatorPWState)::ShowEnterID();   //SR93 Biometrics support	
		ps.SetCMLeadthruText( TXT_ENTER_ID );		//TAR395610
		
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
		
		//ps.AllowInput( MAX_DIGITS_OPERATOR, false, false );
		//ps.ShowCMFrame( false );   
		//return;
	}
	else
	{  
		// NewUI
		ps.SetCMFrame(_T("SecBagBoxNotValid"));
		ps.CMButton(_T("ButtonStoreLogIn"), BTT_STORELOGIN, true);
	}
	
	ps.SetCMFrame(_T("SecBagBoxNotValid"));
	ps.SetCMLeadthruText(LTT_BAGBOXNOTVALID);
    SAWLDBScannerDisable();//Disable the scanner
	ps.ShowCMTBText(csTBMessage2Scot);
	ps.Message12(MSG_BAGBOXNOTVALID, true); //  Remove for TAR391244 , roll back for tar 392689
	trace(L7, _T("+ra.OnTransactionState, Itemization State"));
	ra.OnSetTransactionState(RA_STATE_SECURITY_VIOLATION);
	trace(L7, _T("-ra.OnTransactionState"));
	DMSayPhrase(VERIFYBAGS);//need to get this from HFE
	ps.ShowCMFrame();
}
//e - POS105367

SMStateBase  *SMSecBagBoxNotValid::stateAfterPasswordVerify()
{
    return SMSecBagBoxNotValidBase::stateAfterPasswordVerify();
}

SMStateBase  *SMSecBagBoxNotValid::TimedOut(void)
{
    return SMSecBagBoxNotValidBase::TimedOut();
}

SMStateBase  *SMSecBagBoxNotValid::PSButtonGoBack(void)
{
    return SMSecBagBoxNotValidBase::PSButtonGoBack();
}

SMStateBase  *SMSecBagBoxNotValid::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
    return SMSecBagBoxNotValidBase::OnRAPClearSecurityBtn(lpszAuthorizationType);
}

SMStateBase  *SMSecBagBoxNotValid::OnWtDecrease(void)
{
    return SMSecBagBoxNotValidBase::OnWtDecrease();
}

SMStateBase  *SMSecBagBoxNotValid::OnBackToLGW(void)
{
    return SMSecBagBoxNotValidBase::OnBackToLGW();
}

SMStateBase  *SMSecBagBoxNotValid::TareNone(void)
{
    return SMSecBagBoxNotValidBase::TareNone();
}
