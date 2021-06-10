//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSuspendBase.h
//
// TITLE: Class header for suspend State
//
//
// AUTHOR:    Ying Xie
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SUSPENDBASESTATE
#define _SUSPENDBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSuspendBase : public SMCustomerModeState
{
public:
	SMSuspendBase();
  virtual SMStateBase  *Initialize(void);
  virtual void     UnInitialize(void);
	
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *TimedOut(void);
	
  DECLARE_DYNCREATE(SMSuspendBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSuspend(void);
// or
//    DLLEXPORT SMStateBase *makeSMSuspendBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSuspend objects.  This function is called by
// RETURNSTATE(Suspend) instead of calling new STATE(Suspend) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSuspendBase.cpp and SMSuspend.cpp
// by using the macro IMPLEMENT_STATE(Suspend)

//DECLARE_STATE(Suspend)

#endif
