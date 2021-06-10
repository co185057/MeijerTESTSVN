//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCouponTooHigh.h
//
// TITLE: Class header for coupon too high State
//
//
// AUTHOR:   Gary Miller
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _COUPONTOOHIGHSTATE
#define _COUPONTOOHIGHSTATE

#include "SMCouponTooHighBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMCouponTooHigh : public SMCouponTooHighBase
{
public:
    SMCouponTooHigh();

    virtual SMStateBase  *Initialize(void);
    virtual bool PSAssistMenuLegalForState();
    virtual void UnInitialize();
protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSEnterKey(void);

  DECLARE_DYNCREATE(SMCouponTooHigh)// MFC Runtime class/object information
		
};

#endif