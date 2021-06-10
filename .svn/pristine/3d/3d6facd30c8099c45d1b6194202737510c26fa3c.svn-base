//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectPayment.cpp
//
// TITLE: Class implementation for select a debut payment amount State
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV base state
#include "SMSelectPayment.h"        // MGV this state

#include "transbrokerssf.h"
#include "SMInProgress.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SelectPayment")

IMPLEMENT_DYNCREATE(SMSelectPayment, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////
SMSelectPayment::SMSelectPayment(const TBTENDERTYPE nTyp)
: SMSelectPaymentBase(nTyp)
{
	IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSelectPayment::SMSelectPayment()
{
	IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSelectPayment::Deliver(const TBTENDERTYPE nTyp)
{
	SMSelectPaymentBase::Deliver(nTyp);
	
	return this;
}

SMStateBase * SMSelectPayment::Initialize(void)
{
    CString strPropName = _T("IsDebitSelected");
	CString strIsDebit;
	SendTBCommand(TBCMD_GET_TBPROPERTY, &strPropName, &strIsDebit);
	
    if(SendTBCommand(TBCMD_IS_CUST_REQUEST_CREDIT_ENABLED, NULL) && strIsDebit != _T("1"))
    {
        m_pAppPSX->SetConfigProperty(_T("CMButton1"), _T(""), UI::PROPERTYVISIBLE, VARIANT_TRUE);
    }else
    {
        m_pAppPSX->SetConfigProperty(_T("CMButton1"), _T(""), UI::PROPERTYVISIBLE, VARIANT_FALSE);
    }
	
	SMStateBase  *retState = SMSelectPaymentBase::Initialize();
	
	if (nTndrType != TB_TENDER_EBT)
	{
		ps.LeadThruText(LTT_DEBITAMT);
	}
    m_bMJPlayedUsePinpad = false;
	
	return retState;
}

void SMSelectPayment::UnInitialize(void)
{
    SMSelectPaymentBase::UnInitialize();
}

SMStateBase * SMSelectPayment::OnWtIncreaseNotAllowed(void)
{
    return SMSelectPaymentBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMSelectPayment::OnWtDecrease(void)
{
    return SMSelectPaymentBase::OnWtDecrease();
}

bool SMSelectPayment::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMSelectPaymentBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase *SMSelectPayment::PSButton1(void)
{
	ps.LeadThruText(LTT_CASHPROCESSING);
	
	CString strTenderPropName = _T("TenderType");
	CString strTenderValue = _T("");
	SendTBCommand(TBCMD_SET_HOOKPROPERTY, &strTenderPropName, &strTenderValue);
	
    SendTBCommand(TBCMD_SET_TB_STATE,_T("TENDER_CUSTOMER_REQUESTED_CREDIT"));
    
	RETURNSTATE(InProgress);
}

SMStateBase * SMSelectPayment::PSButton3(void)
{
    return SMSelectPaymentBase::PSButton3();
}

SMStateBase * SMSelectPayment::PSListLookup(CString csItemInfo)
{
    long lSelection = 0;
	lSelection = _ttoi(csItemInfo);
	//tar314183
	CString strPropName = _T("CashBackValue");
	CString strValue;
	if (lSelection == 1)
		strValue = _T("0");
	else 
		strValue = _T("1");
	
	SendTBCommand(TBCMD_SET_TBPROPERTY, &strPropName, &strValue);
	
	return SMSelectPaymentBase::PSListLookup(csItemInfo);
}

SMStateBase * SMSelectPayment::PSNumericKey(const long lKey)
{
    return SMSelectPaymentBase::PSNumericKey(lKey);
}

SMStateBase * SMSelectPayment::HandleAmount(const long lAmount)
{
    return SMSelectPaymentBase::HandleAmount(lAmount);
}

SMStateBase * SMSelectPayment::PSButtonGoBack(void)
{
    // Cancel Tender - Including Swipe!
	CString strTenderPropName = _T("TenderType");
	CString strTenderValue = _T("");
	SendTBCommand(TBCMD_SET_HOOKPROPERTY, &strTenderPropName, &strTenderValue);
    SendTBCommand(TBCMD_SET_TB_STATE,_T("CANCEL_TENDER"));
	//Go Back to SMFinish!
	
	// CAN enter Remote Assist Mode after leaving this state
	//ra.OnSetTransactionState(RA_STATE_ASSISTOK);
	ra.RequestEnableAssistMode(true);   //TAR213249
	
	setAnchorState(BES_FINISH);
	return SMSelectPaymentBase::PSButtonGoBack();
}

SMStateBase * SMSelectPayment::PSCancelKey(void)
{
    return SMSelectPaymentBase::PSCancelKey();
}
