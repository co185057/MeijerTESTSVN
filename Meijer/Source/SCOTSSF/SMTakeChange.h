//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTakeChange.h
//
// TITLE: Class header for Take Change State
//
// This state represents a successful transaction paid for in cash with
// change due back.
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAKECHANGESTATE
#define _TAKECHANGESTATE

#include "SMTakeChangeBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMTakeChange : public SMTakeChangeBase
{
	
public:
    SMTakeChange(const bool fEOT);
    SMTakeChange();
    SMStateBase *Deliver(const bool fEOT);
    virtual SMStateBase * Initialize();
    virtual void UnInitialize();
    virtual SMStateBase * DMCashRemoved(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);

protected:
    virtual SMStateBase  *TimedOut();
    virtual SMStateBase  *OnMotionCashOutSensor(void);
    virtual SMStateBase  *OnNoMotionCashOutSensor(void);
    virtual void Escalation2RAP(void);
    virtual SMStateBase *GPNotify(const GPEVT gpEvt);
    virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual void showScreen(bool passwordVerified);
    virtual SMStateBase  *stateAfterPasswordVerify();

    DECLARE_DYNCREATE(SMTakeChange)// MFC Runtime class/object information
};

#endif
