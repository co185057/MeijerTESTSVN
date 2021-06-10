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
#include <afxtempl.h>               // MS MFC template header

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

