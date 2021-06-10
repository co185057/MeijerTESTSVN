//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterCoupons.h
//
// TITLE: Class header for Scan and Bag State
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ENTERCOUPONSSTATE
#define _ENTERCOUPONSSTATE

#include "SMEnterCouponsBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMEnterCoupons : public SMEnterCouponsBase
{
public:
    SMEnterCoupons();
    virtual SMStateBase  *Initialize(void);
    virtual void     UnInitialize(void);
protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSButton4(void);
    virtual SMStateBase  *PSCancelKey(void);
    virtual SMStateBase  *DMScanner(void);
    virtual SMStateBase  *DMCardReader(void);
    virtual SMStateBase  *DMAcceptor(void);
    

  DECLARE_DYNCREATE(SMEnterCoupons)// MFC Runtime class/object information
		
};

#endif