//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecSkipBaggingThreshold.h
//
// TITLE: Class header for SecSkipBaggingThreshold
//
//
// AUTHOR:    Thanhhieu Pham
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECSKIPBAGGINGTHRESHOLDSTATE
#define _SECSKIPBAGGINGTHRESHOLDSTATE

#include "SMSecSkipBaggingThresholdBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSecSkipBaggingThreshold : public SMSecSkipBaggingThresholdBase
{
public:
    SMSecSkipBaggingThreshold();
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);
    virtual SMStateBase  *OnMatchedWt(void);
protected:
    virtual void showScreen(bool passwordVerified);
    virtual SMStateBase  *stateAfterPasswordVerify();
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);

    DECLARE_DYNCREATE(SMSecSkipBaggingThreshold)// MFC Runtime class/object information
};

#endif
