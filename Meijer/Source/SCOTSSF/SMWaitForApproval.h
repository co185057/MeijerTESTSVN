//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMWaitForApproval.h
//
// TITLE: Class header for wait for approval state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _WAITFORAPPROVALSTATE
#define _WAITFORAPPROVALSTATE


#include "SMWaitForApprovalBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMWaitForApproval : public SMWaitForApprovalBase
{
public:
    SMWaitForApproval(const bool fTV, // transaction verify
        const bool fVT,
        const bool bVerifySig=false);// void transaction
    SMWaitForApproval();
    SMStateBase *Deliver(const bool fTV, // transaction verify
        const bool fVT,
        const bool bVerifySig=false);
    virtual SMStateBase * Initialize(void);
    virtual SMStateBase * TimedOut(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
    virtual SMStateBase  *OnWtDecrease(void);
    virtual void UnInitialize(void);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase  *OnCouponSensor(void);

protected:
    virtual SMStateBase  *PSButtonHelp(void);
    virtual SMStateBase  *PSCancelKey(void);
    virtual SMStateBase  *PSButton1();
    virtual SMStateBase  *OnBackToLGW(void);
    virtual void showScreen(bool passwordVerified);
    virtual SMStateBase  *stateAfterPasswordVerify();
    virtual SMStateBase  *OnUnDeActivatedTheftTag(void);

    DECLARE_DYNCREATE(SMWaitForApproval)// MFC Runtime class/object information
};

#endif
