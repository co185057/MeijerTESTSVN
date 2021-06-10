//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMElectronicBenefitsTransfer.CPP
//
// TITLE:    Class implementation for Electronic Benefits Transfer State
//
//
// AUTHOR:   Michael Prather
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMElectronicBenefitsTransfer.h"      // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("ElectronicBenefitsTransfer")

IMPLEMENT_DYNCREATE(SMElectronicBenefitsTransfer, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMElectronicBenefitsTransfer::SMElectronicBenefitsTransfer(const bool fCshBck)
: SMElectronicBenefitsTransferBase(fCshBck)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMElectronicBenefitsTransfer::SMElectronicBenefitsTransfer()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMElectronicBenefitsTransfer::Deliver(const bool fCshBck)
{
  SMElectronicBenefitsTransferBase::Deliver(fCshBck);

  return this;
}

bool SMElectronicBenefitsTransfer::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMElectronicBenefitsTransferBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase * SMElectronicBenefitsTransfer::Initialize(void)
{
    return SMElectronicBenefitsTransferBase::Initialize();
}

void SMElectronicBenefitsTransfer::UnInitialize(void)
{
    SMElectronicBenefitsTransferBase::UnInitialize();
}

SMStateBase * SMElectronicBenefitsTransfer::OnWtIncreaseNotAllowed(void)
{
    return SMElectronicBenefitsTransferBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMElectronicBenefitsTransfer::OnWtDecrease(void)
{
    return SMElectronicBenefitsTransferBase::OnWtDecrease();
}

SMStateBase * SMElectronicBenefitsTransfer::PSButton1(void)
{
    return SMElectronicBenefitsTransferBase::PSButton1();
}

SMStateBase * SMElectronicBenefitsTransfer::PSButton2(void)
{
    return SMElectronicBenefitsTransferBase::PSButton2();
}

SMStateBase * SMElectronicBenefitsTransfer::PSButtonGoBack(void)
{
    return SMElectronicBenefitsTransferBase::PSButtonGoBack();
}

SMStateBase * SMElectronicBenefitsTransfer::PSCancelKey(void)
{
    return SMElectronicBenefitsTransferBase::PSCancelKey();
}
