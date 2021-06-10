//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMDepositCoupons.h
//
// TITLE: Class header for deposit coupons State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DEPOSITCOUPONSSTATE
#define _DEPOSITCOUPONSSTATE

#include "SMDepositCouponsBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMDepositCoupons : public SMDepositCouponsBase
{
public:
    SMDepositCoupons();

    virtual SMStateBase  *Initialize(void);
    virtual void         UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);

  DECLARE_DYNCREATE(SMDepositCoupons)// MFC Runtime class/object information
};

#endif