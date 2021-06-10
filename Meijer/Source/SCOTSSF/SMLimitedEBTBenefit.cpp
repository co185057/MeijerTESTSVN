//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMLimitedEBTBenefit.cpp
//
// TITLE: Class impliementation for Insufficient EBT or FoodStamps
//
//
// AUTHOR:    Michael Prather
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMLimitedEBTBenefit.h"     // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("LimitedEBTBenefit")

IMPLEMENT_DYNCREATE(SMLimitedEBTBenefit, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////
SMLimitedEBTBenefit::SMLimitedEBTBenefit(const TBTENDERTYPE nTyp, const long lEBT)
: SMLimitedEBTBenefitBase(nTyp, lEBT)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMLimitedEBTBenefit::SMLimitedEBTBenefit()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMLimitedEBTBenefit::Deliver(const TBTENDERTYPE nTyp, const long lEBT)
{
  SMLimitedEBTBenefitBase::Deliver(nTyp, lEBT);

  return this;
}

bool SMLimitedEBTBenefit::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMLimitedEBTBenefitBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase * SMLimitedEBTBenefit::Initialize(void)
{
    return SMLimitedEBTBenefitBase::Initialize();
}

void SMLimitedEBTBenefit::UnInitialize(void)
{
    SMLimitedEBTBenefitBase::UnInitialize();
}

SMStateBase * SMLimitedEBTBenefit::OnWtIncreaseNotAllowed(void)
{
    return SMLimitedEBTBenefitBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMLimitedEBTBenefit::OnWtDecrease(void)
{
    return SMLimitedEBTBenefitBase::OnWtDecrease();
}

SMStateBase * SMLimitedEBTBenefit::PSButton1(void)
{
    return SMLimitedEBTBenefitBase::PSButton1();
}

SMStateBase * SMLimitedEBTBenefit::PSButton2(void)
{
    return SMLimitedEBTBenefitBase::PSButton2();
}
