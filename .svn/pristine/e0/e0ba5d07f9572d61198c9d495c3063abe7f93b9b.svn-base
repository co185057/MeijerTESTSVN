//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmEBTAmount.cpp
//
// TITLE: Class impliementation for Confirm EBT
//
//
// AUTHOR:    Michael Prather
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMConfirmEBTAmount.h"     // MGV this state
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("ConfirmEBTAmount")

IMPLEMENT_DYNCREATE(SMConfirmEBTAmount, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////
SMConfirmEBTAmount::SMConfirmEBTAmount(const TBTENDERTYPE nTyp,const long lEBT, const long lCash)
: SMConfirmEBTAmountBase(nTyp, lEBT, lCash)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMConfirmEBTAmount::SMConfirmEBTAmount()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMConfirmEBTAmount::Deliver(const TBTENDERTYPE nTyp,const long lEBT, const long lCash)
{
  SMConfirmEBTAmountBase::Deliver(nTyp, lEBT, lCash);

  return this;
}

bool SMConfirmEBTAmount::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMConfirmEBTAmountBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase * SMConfirmEBTAmount::Initialize(void)
{
    return SMConfirmEBTAmountBase::Initialize();
}

void SMConfirmEBTAmount::UnInitialize(void)
{
    SMConfirmEBTAmountBase::UnInitialize();
}

SMStateBase * SMConfirmEBTAmount::OnWtIncreaseNotAllowed(void)
{
    return SMConfirmEBTAmountBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMConfirmEBTAmount::OnWtDecrease(void)
{
    return SMConfirmEBTAmountBase::OnWtDecrease();
}

SMStateBase * SMConfirmEBTAmount::PSButton1(void)
{
    lBalanceDue += lCashBack; 
	return SMConfirmEBTAmountBase::PSButton1();
}

SMStateBase * SMConfirmEBTAmount::PSButton2(void)
{
    return SMConfirmEBTAmountBase::PSButton2();
}

SMStateBase * SMConfirmEBTAmount::PSButton3(void)
{
    return SMConfirmEBTAmountBase::PSButton3();
}

SMStateBase * SMConfirmEBTAmount::PSButtonGoBack(void)
{
    // Cancel Tender - Including Swipe!
	CString strTenderPropName = _T("TenderType");
	CString strTenderValue = _T("");
	SendTBCommand(TBCMD_SET_HOOKPROPERTY, &strTenderPropName, &strTenderValue);
    SendTBCommand(TBCMD_SET_TB_STATE,_T("CANCEL_TENDER"));
	//Go Back to SMFinish!
	return SMConfirmEBTAmountBase::PSButtonGoBack();
}
