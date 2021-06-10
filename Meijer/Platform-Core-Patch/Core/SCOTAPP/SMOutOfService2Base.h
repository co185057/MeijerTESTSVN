//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMOutOfService2Base.h
//
// TITLE: Class header for wait for helpf for bad card state, wait for TS testing
//
// This state accepts and handles inputs from:
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _OUTOFSERVICE2BASESTATE
#define _OUTOFSERVICE2BASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMOutOfService2Base : public SMCustomerModeState
{
public:
	SMOutOfService2Base();
  virtual SMStateBase  *Initialize(void);
  virtual void     UnInitialize(void);
  virtual SMStateBase  *TimedOut(void);
	
protected:
  virtual void TBShow2x20(void);
	
  DECLARE_DYNCREATE(SMOutOfService2Base)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMOutOfService2(void);
// or
//    DLLEXPORT SMStateBase *makeSMOutOfService2Base(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMOutOfService2 objects.  This function is called by
// RETURNSTATE(OutOfService2) instead of calling new STATE(OutOfService2) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMOutOfService2Base.cpp and SMOutOfService2.cpp
// by using the macro IMPLEMENT_STATE(OutOfService2)

//DECLARE_STATE(OutOfService2)

#endif
