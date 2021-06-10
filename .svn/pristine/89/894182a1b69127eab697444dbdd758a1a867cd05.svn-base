//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMFatalError.h
//
// TITLE: Class impliementation for fatal error state
//
//
// AUTHOR:    David Moore
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _FatalErrorSTATE
#define _FatalErrorSTATE

#include "SMOperatorPasswordState.h"
#include "SMFatalErrorBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMFatalError : public SMFatalErrorBase
{
public:
    SMFatalError();
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase *Initialize(void);
    virtual SMStateBase *PSButtonHelp(void);
    virtual SMStateBase *PSButton2(void);
    virtual SMStateBase *PSButton3(void);
    virtual SMStateBase *PSCancelKey(void);
    virtual SMStateBase *PSListLookup(CString);
    virtual SMStateBase *TimedOut(void);
    virtual SMStateBase *stateAfterPasswordVerify(void) ;
    virtual void     showScreen(bool passwordVerified);
    virtual SMStateBase *PSButtonGoBack(void);

    DECLARE_DYNCREATE(SMFatalError)// MFC Runtime class/object information
};

#endif