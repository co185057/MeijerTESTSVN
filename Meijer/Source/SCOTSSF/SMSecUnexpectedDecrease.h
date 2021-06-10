//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecUnexpectedDecrease.h
//
// TITLE: Class header for SMSecUnexpectedDecrease
//
//
// AUTHOR:    Thanhhieu Pham
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECUNEXPECTEDDECREASESTATE
#define _SECUNEXPECTEDDECREASESTATE

#include "SMSecUnexpectedDecreaseBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSecUnexpectedDecrease : public SMSecUnexpectedDecreaseBase
{
public:
    SMSecUnexpectedDecrease();
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *stateAfterPasswordVerify();
    virtual void showScreen(bool passwordVerified);
    virtual SMStateBase  *OnBackToLGW(void);
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
    virtual SMStateBase  *OnMatchedWt(void);
    virtual SMStateBase  *DMdf_EASNoMotion();
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease();
    virtual SMStateBase  *OnTBNeedData(void);
    virtual void Escalation2RAP(void);

    DECLARE_DYNCREATE(SMSecUnexpectedDecrease)// MFC Runtime class/object information
};

#endif