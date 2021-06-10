//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMVoidTransApproval.h
//
// TITLE: Class header for VoidTransApproval
//
// AUTHOR:    Aiqin Hou
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _VOIDTRANSAPPROVALSTATE
#define _VOIDTRANSAPPROVALSTATE

#include "SMVoidTransApprovalBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMVoidTransApproval : public SMVoidTransApprovalBase
{
public:
    SMVoidTransApproval();
    virtual SMStateBase  *Initialize(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual void UnInitialize(void);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual void ClearSecurityViolationLight(void); 
protected:
    virtual void showScreen(bool passwordVerified);
    virtual SMStateBase * stateAfterPasswordVerify();

    virtual SMStateBase  *TimedOut(void);         // optional
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
    virtual SMStateBase *HandleCancelTrasaction(CString csBttID);

    DECLARE_DYNCREATE(SMVoidTransApproval)// MFC Runtime class/object information
};

#endif
