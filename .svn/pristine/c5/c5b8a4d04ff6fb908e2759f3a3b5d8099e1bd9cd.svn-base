//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMVoidApproval.h
//
// TITLE: Class header for VoidApproval
//
// AUTHOR:    Leila Parker
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _VOIDAPPROVALSTATE
#define _VOIDAPPROVALSTATE

#include "SMVoidApprovalBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMVoidApproval : public SMVoidApprovalBase
{
public:
    SMVoidApproval();
    virtual SMStateBase  *Initialize(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual void UnInitialize(void);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
protected:
    virtual void showScreen(bool passwordVerified);
    virtual SMStateBase * stateAfterPasswordVerify();
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);

    DECLARE_DYNCREATE(SMVoidApproval)// MFC Runtime class/object information
};

#endif
