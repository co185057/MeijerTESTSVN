//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMLaneClosed.h
//
// TITLE: Class header for lane closed state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _LANECLOSED
#define _LANECLOSED

#include "SMLaneClosedBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMLaneClosed : public SMLaneClosedBase
{
public:
    SMLaneClosed();
	virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase *Initialize(void);
    virtual void         UnInitialize(void);
    virtual SMStateBase *PSButtonHelp(void);
    virtual SMStateBase *PSButton1(void);
    virtual SMStateBase *PSButtonGoBack();
    virtual SMStateBase *PSCancelKey(void);
    virtual SMStateBase *stateAfterPasswordVerify(void);
    virtual void         showScreen(bool passwordVerified = false);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
    virtual SMStateBase  *OnWtDecrease(void);

protected:
    virtual SMStateBase * DMScanner(void);

    DECLARE_DYNCREATE(SMLaneClosed) // MFC Runtime class/object information
};

#endif
