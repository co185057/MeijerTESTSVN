//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMDropoffCoupons.h
//
// TITLE: Class header for dropoff coupons State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DROPOFFCOUPONSSTATE
#define _DROPOFFCOUPONSSTATE

#include "SMDropoffCouponsBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMDropoffCoupons : public SMDropoffCouponsBase
{
public:
    SMDropoffCoupons();
    virtual SMStateBase  *Initialize(void);
    virtual void         UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);

    DECLARE_DYNCREATE(SMDropoffCoupons)// MFC Runtime class/object information
};

#endif
