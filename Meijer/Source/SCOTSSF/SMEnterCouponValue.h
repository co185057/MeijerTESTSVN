//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterCouponValue.h
//
// TITLE: Class header for enter Coupon value state
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ENTERCOUPONVALUESTATE
#define _ENTERCOUPONVALUESTATE

#include "SMEnterCouponValueBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMEnterCouponValue : public SMEnterCouponValueBase
{
public:
    SMEnterCouponValue();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize();
    virtual bool PSAssistMenuLegalForState();
protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSNumericKey(const long);
    virtual SMStateBase  *PSEnterKey(void);
    virtual SMStateBase  *PSClearKey(void);
    virtual SMStateBase  *PSBackSpaceKey(void);
    virtual SMStateBase  *PSCancelKey(void);
    virtual SMStateBase  *PSButtonGoBack(void);

  DECLARE_DYNCREATE(SMEnterCouponValue)// MFC Runtime class/object information
		
};

#endif