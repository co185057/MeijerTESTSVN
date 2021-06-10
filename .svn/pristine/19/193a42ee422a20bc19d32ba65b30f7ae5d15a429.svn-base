//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMDegradedContinueTransBase.h
//
// TITLE: Class header for Degraded Continue Transaction State
//
// AUTHOR:    Allen Jay Cagaanan
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DegradedContinueTransBaseSTATE
#define _DegradedContinueTransBaseSTATE

#include "SMHelpModeState.h"


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMDegradedContinueTransBase : public STATE(HelpModeState)
{
public:
	SMDegradedContinueTransBase(void);
  virtual SMStateBase *Initialize(void);
  virtual void UnInitialize(void);
  
protected:
  virtual SMStateBase *TimedOut(void);
  virtual SMStateBase *PSButton1(void);
  virtual SMStateBase *PSButton2(void);
  virtual SMStateBase *DMCardReader(void);
	
  DECLARE_DYNCREATE(SMDegradedContinueTransBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMDegradedContinueTrans(void);
// or
//    DLLEXPORT SMStateBase *makeSMDegradedContinueTransBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMDegradedContinueTrans objects.  This function is called by
// RETURNSTATE(DegradedContinueTrans) instead of calling new STATE(DegradedContinueTrans) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMDegradedContinueTransBase.cpp and SMDegradedContinueTrans.cpp
// by using the macro IMPLEMENT_STATE(DegradedContinueTrans)

//DECLARE_STATE(DegradedContinueTrans)

#endif
