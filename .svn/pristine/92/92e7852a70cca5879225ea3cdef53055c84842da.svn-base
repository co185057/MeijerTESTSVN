//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmAbort.h
//
// TITLE: Class header for transaction abort
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CONFIRMABORTSTATE
#define _CONFIRMABORTSTATE

#include "SMConfirmAbortBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMConfirmAbort : public SMConfirmAbortBase
{
public:
    SMConfirmAbort();
    virtual SMStateBase *Initialize(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase *TimedOut(void);
    virtual SMStateBase *PSButton1(void);
    virtual SMStateBase *PSButton2(void);
    virtual SMStateBase *PSButtonHelp(void);

    DECLARE_DYNCREATE(SMConfirmAbort)// MFC Runtime class/object information
};

#endif