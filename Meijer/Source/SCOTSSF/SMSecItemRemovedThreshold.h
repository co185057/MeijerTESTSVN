//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecItemRemovedThreshold.h
//
// TITLE: Class header for SecItemRemovedThreshold
//
//
// AUTHOR:    Thanhhieu Pham
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECITEMREMOVEDTHRESHOLDSTATE
#define _SECITEMREMOVEDTHRESHOLDSTATE

#include "SMSecItemRemovedThresholdBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSecItemRemovedThreshold : public SMSecItemRemovedThresholdBase
{
public:
    SMSecItemRemovedThreshold();
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);
protected:
    virtual void showScreen(bool passwordVerified);
    virtual SMStateBase  *stateAfterPasswordVerify();
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);

    DECLARE_DYNCREATE(SMSecItemRemovedThreshold)// MFC Runtime class/object information
};

#endif