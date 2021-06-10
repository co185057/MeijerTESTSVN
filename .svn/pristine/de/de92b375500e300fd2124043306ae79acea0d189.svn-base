//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCouponNotAllowed.h
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _COUPONNOTALLOWED
#define _COUPONNOTALLOWED
#include "SMCouponNotAllowedBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMCouponNotAllowed : public SMCouponNotAllowedBase
{
public:
    SMCouponNotAllowed();                // constructor

    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize();

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *PSEnterKey(void);

    DECLARE_DYNCREATE(SMCouponNotAllowed)// MFC Runtime class/object information
};

#endif
