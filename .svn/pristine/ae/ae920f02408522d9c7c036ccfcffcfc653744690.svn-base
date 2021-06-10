//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMDegradedContinueTrans.h
//
// TITLE: Class header for Degraded Continue Transaction State
//
//
// AUTHOR:    Allen Jay Cagaanan
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DEGRADEDCONTINUETRANSSTATE
#define _DEGRADEDCONTINUETRANSSTATE

#include "SMDegradedContinueTransBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMDegradedContinueTrans : public SMDegradedContinueTransBase
{
public:
    SMDegradedContinueTrans(void);
    virtual SMStateBase *Initialize(void);
    virtual void UnInitialize(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  
protected:
    virtual SMStateBase *TimedOut(void);
    virtual SMStateBase *PSButton1(void);
    virtual SMStateBase *PSButton2(void);
    virtual SMStateBase *DMCardReader(void);

    DECLARE_DYNCREATE(SMDegradedContinueTrans)// MFC Runtime class/object information

};

#endif