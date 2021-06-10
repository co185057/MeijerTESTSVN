//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMSecSkipBaggingThresholdBase.h
//
// TITLE:	Class header for Skip Bagging Button Threshold Screen
//
// AUTHOR:  Thanhhieu Pham
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECSKIPBAGGINGTHRESHOLDBASESTATE
#define _SECSKIPBAGGINGTHRESHOLDBASESTATE
#include "SMSecOperatorPWState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

  SMSecSkipBaggingThresholdBase : public STATE(SecOperatorPWState)
{
public:
  SMSecSkipBaggingThresholdBase(const BEST PrevState);
  SMSecSkipBaggingThresholdBase();
  SMStateBase *Deliver(const BEST PrevState);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);          // optional
  virtual SMStateBase  *OnMatchedWt(void);  // TAR 304285
protected:
  virtual void showScreen(bool passwordVerified);
  virtual SMStateBase  *stateAfterPasswordVerify();
//  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);	// Smart Scale reported an unmatched wt increase
//  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *TimedOut(void);         // optional
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);

  DECLARE_DYNCREATE(SMSecSkipBaggingThresholdBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSecSkipBaggingThresholdBase(void);
//		or
//    DLLEXPORT SMStateBase *makeSMSecSkipBaggingThresholdBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSecSkipBaggingThreshold objects.  This function is called by
// RETURNSTATE(DeactivateTheftTag) instead of calling new STATE(SecSkipBaggingThreshold) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSecSkipBaggingThresholdBase.cpp and SMSecSkipBaggingThreshold.cpp
// by using the macro IMPLEMENT_STATE(SecSkipBaggingThreshold)

//DECLARE_STATE(SecSkipBaggingThreshold)

#endif
