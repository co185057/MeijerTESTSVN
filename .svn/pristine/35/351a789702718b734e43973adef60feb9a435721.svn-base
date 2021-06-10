//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCouponItems.h
//
// TITLE: Class header for store mode Coupon Items state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMCOUPONITEMSSTATE
#define _SMCOUPONITEMSSTATE

#include "SMSmCouponItemsBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmCouponItems : public SMSmCouponItemsBase
{
public:
    SMSmCouponItems();							  // constructor
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton8(void);
    virtual SMStateBase *PSReceiptUp(void);
    virtual SMStateBase *PSReceiptDown(void);
    //virtual SMStateBase *RAParse(MessageElement* ); sscoadk-5431

    DECLARE_DYNCREATE(SMSmCouponItems)// MFC Runtime class/object information
};

#endif