//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmAbortBase.h
//
// TITLE: Class header for Transaction Abort
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ConfirmAbortBaseSTATE
#define _ConfirmAbortBaseSTATE

#include "SMHelpModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMConfirmAbortBase : public STATE(HelpModeState)
{
public:
	SMConfirmAbortBase();
 virtual SMStateBase *Initialize(void);
 virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
 virtual SMStateBase  *OnWtDecrease(void);
 virtual void UnInitialize(void);		// RFC 330050
	
protected:
  virtual SMStateBase *TimedOut(void);
  virtual SMStateBase *PSButton1(void);
  virtual SMStateBase *PSButton2(void);
  virtual SMStateBase *PSButtonHelp(void);//TAR400959
	
  DECLARE_DYNCREATE(SMConfirmAbortBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMConfirmAbort(void);
// or
//    DLLEXPORT SMStateBase *makeSMConfirmAbortBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMConfirmAbort objects.  This function is called by
// RETURNSTATE(ConfirmAbort) instead of calling new STATE(ConfirmAbort) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMConfirmAbortBase.cpp and SMConfirmAbort.cpp
// by using the macro IMPLEMENT_STATE(ConfirmAbort)

//DECLARE_STATE(ConfirmAbort)

#endif
