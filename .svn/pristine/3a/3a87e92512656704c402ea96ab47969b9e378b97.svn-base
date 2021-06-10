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

    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase  *Initialize(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    DECLARE_DYNCREATE(SMLimitedEBTBenefit)// MFC Runtime class/object information
};

#endif
