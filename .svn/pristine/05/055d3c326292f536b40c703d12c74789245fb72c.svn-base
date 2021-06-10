//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMContextHelp.h
//
// TITLE: Class impliementation for contextual help state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CONTEXTHELPSTATE
#define _CONTEXTHELPSTATE

#include "SMOperatorPasswordState.h"
#include "SMContextHelpBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMContextHelp : public SMContextHelpBase
{
public:
    SMContextHelp();
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
    virtual SMStateBase  *OnWtDecrease(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase *Initialize(void);
    virtual SMStateBase *PSButtonHelp(void);
    virtual SMStateBase *PSButton1(void);
    virtual SMStateBase *PSButton2(void);
    virtual SMStateBase *PSButton3(void);
	virtual SMStateBase *PSButton4();	// POS181195
    virtual SMStateBase *PSCancelKey(void);
    virtual SMStateBase *PSListLookup(CString);
    virtual SMStateBase *TimedOut(void);
    virtual SMStateBase *stateAfterPasswordVerify(void) ;
    virtual void     showScreen(bool passwordVerified);
    virtual SMStateBase *PSButtonGoBack(void); // POS181195

  DECLARE_DYNCREATE(SMContextHelp)// MFC Runtime class/object information
};

#endif