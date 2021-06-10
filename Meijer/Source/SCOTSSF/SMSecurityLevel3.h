//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecurityLevel3.h
//
// TITLE: Class header for Level 3 Security agent State.
//
//
// AUTHOR:    DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECURITYLEVEL3
#define _SECURITYLEVEL3

#include "SMSecurityLevel3Base.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSecurityLevel3 : public SMSecurityLevel3Base
{
public:
    SMSecurityLevel3(CString csBar, CString csPop);
    SMSecurityLevel3();
    SMStateBase *Deliver(CString csBar, CString csPop);
    virtual SMStateBase  *Initialize(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
    virtual SMStateBase  *OnWtDecrease(void);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSCancelKey(void);
    virtual void     showScreen(bool passwordVerified);
    virtual SMStateBase * stateAfterPasswordVerify();
    virtual SMStateBase  *PSButtonGoBack(void);

  DECLARE_DYNCREATE(SMSecurityLevel3)// MFC Runtime class/object information
};

#endif
