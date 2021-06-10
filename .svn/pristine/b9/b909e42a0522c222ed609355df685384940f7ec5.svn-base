//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecBagBoxThreshold.h
//
// TITLE: Class header for SecBagBoxThreshold
//
//
// AUTHOR:    Thanhhieu Pham
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECBAGBOXTHRESHOLDSTATE
#define _SECBAGBOXTHRESHOLDSTATE

#include "SMSecBagBoxThresholdBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSecBagBoxThreshold : public SMSecBagBoxThresholdBase
{
public:
    SMSecBagBoxThreshold();
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);
protected:
    virtual void showScreen(bool passwordVerified);
    virtual SMStateBase  *stateAfterPasswordVerify();
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
    virtual SMStateBase  *OnBackToLGW(void);

    DECLARE_DYNCREATE(SMSecBagBoxThreshold)// MFC Runtime class/object information
};

#endif