//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMSelectDebitType.CPP
//
// TITLE:    Class implementation for Select Debit Type State
//
//
// AUTHOR:   Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSelectDebitType.h"      // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SelectDebitType")

IMPLEMENT_DYNCREATE(SMSelectDebitType, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSelectDebitType::SMSelectDebitType(const bool fCshBck)
: SMSelectDebitTypeBase(fCshBck)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSelectDebitType::SMSelectDebitType()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSelectDebitType::Deliver(const bool fCshBck)
{
  SMSelectDebitTypeBase::Deliver(fCshBck);

  return this;
}

bool SMSelectDebitType::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMSelectDebitTypeBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase * SMSelectDebitType::Initialize(void)
{
    return SMSelectDebitTypeBase::Initialize();
}

void SMSelectDebitType::UnInitialize(void)
{
    SMSelectDebitTypeBase::UnInitialize();
}

SMStateBase * SMSelectDebitType::OnWtIncreaseNotAllowed(void)
{
    return SMSelectDebitTypeBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMSelectDebitType::OnWtDecrease(void)
{
    return SMSelectDebitTypeBase::OnWtDecrease();
}

SMStateBase * SMSelectDebitType::PSButton1(void)
{
    return SMSelectDebitTypeBase::PSButton1();
}

SMStateBase * SMSelectDebitType::PSButton2(void)
{
    return SMSelectDebitTypeBase::PSButton2();
}

SMStateBase * SMSelectDebitType::PSButton3(void)
{
    return SMSelectDebitTypeBase::PSButton3();
}

SMStateBase * SMSelectDebitType::PSCancelKey(void)
{
    return SMSelectDebitTypeBase::PSCancelKey();
}

SMStateBase * SMSelectDebitType::PSButtonGoBack(void)
{
    return SMSelectDebitTypeBase::PSButtonGoBack();
}
