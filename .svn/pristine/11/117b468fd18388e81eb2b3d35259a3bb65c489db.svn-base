//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecMisMatchWeight.h
//
// TITLE: Class header for SMSecMisMatchWeight
//
//
// AUTHOR:    Tara Duncan
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECMISMATCHWEIGHTSTATE
#define _SECMISMATCHWEIGHTSTATE

#include "SMSecMisMatchWeightBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSecMisMatchWeight : public SMSecMisMatchWeightBase
{
public:
    SMSecMisMatchWeight();
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);

protected:
    virtual void showScreen(bool passwordVerified);
    virtual SMStateBase  *stateAfterPasswordVerify();
    virtual SMStateBase  *OnBackToLGW(void);
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnMatchedWt(void);
    virtual void Escalation2RAP(void);


	bool m_bPasswordVerified;

    DECLARE_DYNCREATE(SMSecMisMatchWeight)// MFC Runtime class/object information
};

#endif
