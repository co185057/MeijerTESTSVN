//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSignCharge.h
//
// TITLE: Class header for sign charge state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SIGNCHARGESTATE
#define _SIGNCHARGESTATE

#include "SMSignChargeBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSignCharge : public SMSignChargeBase
{
public:
    SMSignCharge();                   // constructor
    virtual SMStateBase * Initialize(void);
    virtual SMStateBase * TimedOut(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
    virtual SMStateBase  *OnWtDecrease(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase * stateAfterPasswordVerify(void); 
    virtual void          showScreen(bool passwordVerified);
    virtual SMStateBase * PSButton1(void);

    DECLARE_DYNCREATE(SMSignCharge)// MFC Runtime class/object information

};

#endif