//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecUnExpectedIncrease.h
//
// TITLE: Class header for SMSecUnExpectedIncrease
//
//
// AUTHOR:    Tara Duncan
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECUNEXPECTEDINCREASESTATE
#define _SECUNEXPECTEDINCREASESTATE

#include "SMSecUnExpectedIncreaseBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSecUnExpectedIncrease : public SMSecUnExpectedIncreaseBase
{
public:
    SMSecUnExpectedIncrease();
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);
    virtual bool IgnoreTBMessageInSecurityState();

protected:
    virtual void showScreen(bool passwordVerified);
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *stateAfterPasswordVerify();
    virtual SMStateBase  *OnBackToLGW(void);
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *DMTakeawaySendScaleStatus();
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *DMdf_EASNoMotion();
    virtual SMStateBase  *OnTBNeedData(void);
    virtual void Escalation2RAP(void);

	virtual SMStateBase  *OnNoMotionCashOutSensor(void);

    DECLARE_DYNCREATE(SMSecUnExpectedIncrease)// MFC Runtime class/object information
};

#endif
