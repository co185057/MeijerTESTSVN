//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmDebitAmount.cpp
//
// TITLE: Class impliementation for check signature state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMConfirmDebitAmount.h"     // MGV this state
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("ConfirmDebitAmount")

IMPLEMENT_DYNCREATE(SMConfirmDebitAmount, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////
SMConfirmDebitAmount::SMConfirmDebitAmount(const TBTENDERTYPE nTyp,const long lAmt)
: SMConfirmDebitAmountBase(nTyp, lAmt)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMConfirmDebitAmount::SMConfirmDebitAmount()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMConfirmDebitAmount::Deliver(const TBTENDERTYPE nTyp,const long lAmt)
{
  SMConfirmDebitAmountBase::Deliver(nTyp, lAmt);

  return this;
}

bool SMConfirmDebitAmount::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMConfirmDebitAmountBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase * SMConfirmDebitAmount::Initialize(void)
{
    return SMConfirmDebitAmountBase::Initialize();
}

void SMConfirmDebitAmount::UnInitialize(void)
{
    SMConfirmDebitAmountBase::UnInitialize();
}

SMStateBase * SMConfirmDebitAmount::OnWtIncreaseNotAllowed(void)
{
    return SMConfirmDebitAmountBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMConfirmDebitAmount::OnWtDecrease(void)
{
    return SMConfirmDebitAmountBase::OnWtDecrease();
}

SMStateBase * SMConfirmDebitAmount::PSButton1(void)
{
    lBalanceDue = lTenderAmt;
	return SMConfirmDebitAmountBase::PSButton1();
}

SMStateBase * SMConfirmDebitAmount::PSEnterKey(void)
{
    return SMConfirmDebitAmountBase::PSEnterKey();
}

SMStateBase * SMConfirmDebitAmount::PSButtonGoBack(void)
{
    // Cancel Tender - Including Swipe!
	CString strTenderPropName = _T("TenderType");
	CString strTenderValue = _T("");
	SendTBCommand(TBCMD_SET_HOOKPROPERTY, &strTenderPropName, &strTenderValue);
    SendTBCommand(TBCMD_SET_TB_STATE,_T("CANCEL_TENDER"));
	//Go Back to SMFinish!
	return SMConfirmDebitAmountBase::PSButtonGoBack();
}
