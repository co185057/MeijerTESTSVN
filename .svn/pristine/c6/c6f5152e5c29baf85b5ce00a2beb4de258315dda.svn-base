//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMInsertGiftCard.CPP
//
// TITLE:	Class implementation for InsertGiftCard state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV base state
#include "SMInsertGiftCard.h"           // MGV this state

#include "transbrokerssf.h"


#define COMP_ID ID_SM               // base state component
#define T_ID    _T("InsertGiftCard")

IMPLEMENT_DYNCREATE(SMInsertGiftCard, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////
SMInsertGiftCard::SMInsertGiftCard()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMInsertGiftCard::Initialize(void)
{
    return SMInsertGiftCardBase::Initialize();
}

void SMInsertGiftCard::UnInitialize(void)
{
    SMInsertGiftCardBase::UnInitialize();
}

bool SMInsertGiftCard::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMInsertGiftCardBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase  *SMInsertGiftCard::OnWtDecrease(void)
{
    return SMInsertGiftCardBase::OnWtDecrease();
}

SMStateBase  *SMInsertGiftCard::OnWtIncreaseNotAllowed(void)
{
    return SMInsertGiftCardBase::OnWtIncreaseNotAllowed();
}

SMStateBase  *SMInsertGiftCard::TimedOut(void)
{
    return SMInsertGiftCardBase::TimedOut();
}

SMStateBase  *SMInsertGiftCard::OnCouponSensor(void)
{
    return SMInsertGiftCardBase::OnCouponSensor();
}

void SMInsertGiftCard::Escalation2RAP(void)
{
    SMInsertGiftCardBase::Escalation2RAP();
}

SMStateBase *SMInsertGiftCard::GPNotify(const GPEVT gpEvt)
{
    return SMInsertGiftCardBase::GPNotify(gpEvt);
}

SMStateBase  *SMInsertGiftCard::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
    return SMInsertGiftCardBase::OnRAPClearSecurityBtn(lpszAuthorizationType);
}

SMStateBase  *SMInsertGiftCard::stateAfterPasswordVerify()
{
    return SMInsertGiftCardBase::stateAfterPasswordVerify();
}

void SMInsertGiftCard::showScreen(bool passwordVerified)
{
    COleVariant v;
    long lReturn;
    PSXRC psxReturn;
    
    if (passwordVerified)
        return;
    
    if (inputMode == PROMPTING_FOR_OPERATOR)
    {
        ps.SetCMFrame(_T("EnterID"));
		
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
        
        lReturn = ps.Message12(iMessage, false);
        // Leadthru text message set
        ASSERT(0 == lReturn);
        lReturn = ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
        ASSERT(lReturn <= MAX_DIGITS_OPERATOR);
        lReturn = ps.ShowCMFrame(false);
        ASSERT(0 == lReturn);
        return;
    }
    else
    {  
        ps.SetCMFrame(_T("InsertGiftCard"));
        v = VARIANT_TRUE;
        psxReturn = m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), _T("InsertGiftCard"), UI::PROPERTYVISIBLE, v);
        ASSERT(psxReturn == PSX_SUCCESS);
    }
    
    ps.ShowCMTBText(csTBMessage2Scot);
    lReturn = ps.ShowCMFrame(false);
    ASSERT(0 == lReturn);
}
