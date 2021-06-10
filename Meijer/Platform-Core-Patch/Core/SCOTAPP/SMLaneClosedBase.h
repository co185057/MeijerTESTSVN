//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMLaneClosedBase.h
//
// TITLE: Class header for lane closed state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _LaneClosedBaseSTATE
#define _LaneClosedBaseSTATE

#include "SMOperatorPasswordState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMLaneClosedBase : public STATE(OperatorPasswordState)
{
public:
	SMLaneClosedBase();
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
   	CString csCurContext;    // Tar279101

  DECLARE_DYNCREATE(SMLaneClosedBase) // MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMLaneClosed(void);
// or
//    DLLEXPORT SMStateBase *makeSMLaneClosedBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMLaneClosed objects.  This function is called by
// RETURNSTATE(LaneClosed) instead of calling new STATE(LaneClosed) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMLaneClosedBase.cpp and SMLaneClosed.cpp
// by using the macro IMPLEMENT_STATE(LaneClosed)

//DECLARE_STATE(LaneClosed)

#endif
