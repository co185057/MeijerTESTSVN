//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMSecItemRemovedThresholdBase.h
//
// TITLE:	Class header for Item Removed Button Threshold Screen
//
// AUTHOR:  Thanhhieu Pham
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECITEMREMOVEDTHRESHOLDBASESTATE
#define _SECITEMREMOVEDTHRESHOLDBASESTATE
#include "SMSecOperatorPWState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

  SMSecItemRemovedThresholdBase : public STATE(SecOperatorPWState)
{
public:
  SMSecItemRemovedThresholdBase(const BEST prevState);
  SMSecItemRemovedThresholdBase();
  SMStateBase *Deliver(const BEST argPrevState);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);          // optional
protected:
  virtual void showScreen(bool passwordVerified);
  virtual SMStateBase  *stateAfterPasswordVerify();
//  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);	// Smart Scale reported an unmatched wt increase
//  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *TimedOut(void);         // optional
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
  
  DECLARE_DYNCREATE(SMSecItemRemovedThresholdBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSecItemRemovedThresholdBase(void);
//		or
//    DLLEXPORT SMStateBase *makeSMSecItemRemovedThresholdBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSecItemRemovedThreshold objects.  This function is called by
// RETURNSTATE(DeactivateTheftTag) instead of calling new STATE(SecItemRemovedThreshold) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSecItemRemovedThresholdBase.cpp and SMSecItemRemovedThreshold.cpp
// by using the macro IMPLEMENT_STATE(SecItemRemovedThreshold)

//DECLARE_STATE(SecItemRemovedThreshold)

#endif