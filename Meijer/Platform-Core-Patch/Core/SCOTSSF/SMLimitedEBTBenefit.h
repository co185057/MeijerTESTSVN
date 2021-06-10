//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMLimitedEBTBenefit.h
//
// TITLE: Class header for Insufficient EBT or FoodStamps
//
//
// AUTHOR:    Michael Prather
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _LimitedEBTBenefitSTATE
#define _LimitedEBTBenefitSTATE

#include "SMLimitedEBTBenefitBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMLimitedEBTBenefit : public SMLimitedEBTBenefitBase
{

public:
	SMLimitedEBTBenefit(const TBTENDERTYPE, const long);
	SMLimitedEBTBenefit();
	SMStateBase *Deliver(const TBTENDERTYPE, const long);
	
  DECLARE_DYNCREATE(SMLimitedEBTBenefit)// MFC Runtime class/object information
};

#endif
