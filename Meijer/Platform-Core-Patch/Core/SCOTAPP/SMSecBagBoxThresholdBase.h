//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMSecBagBoxThresholdBase.h
//
// TITLE:	Class header for Bag/Box Button Threshold Screen
//
// AUTHOR:  Thanhhieu Pham
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECBAGBOXTHRESHOLDBASESTATE
#define _SECBAGBOXTHRESHOLDBASESTATE
#include "SMSecOperatorPWState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

  SMSecBagBoxThresholdBase : public STATE(SecOperatorPWState)
{
public:
  SMSecBagBoxThresholdBase(const BEST prevState);
  SMSecBagBoxThresholdBase();
  SMStateBase *Deliver(const BEST argPrevState);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);          // optional
protected:
  virtual void showScreen(bool passwordVerified);
  virtual SMStateBase  *stateAfterPasswordVerify();
//  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);	// Smart Scale reported an unmatched wt increase
//  virtual SMStateBase  *OnWtDecrease(void);				// Smart Scale reported an unmatched wt decrease
  virtual SMStateBase  *TimedOut(void);         // optional
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
  virtual SMStateBase  *SMSecBagBoxThresholdBase::OnBackToLGW(void);	//TAR377288
  
  DECLARE_DYNCREATE(SMSecBagBoxThresholdBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSecBagBoxThresholdBase(void);
//		or
//    DLLEXPORT SMStateBase *makeSMSecBagBoxThresholdBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSecBagBoxThreshold objects.  This function is called by
// RETURNSTATE(DeactivateTheftTag) instead of calling new STATE(SecBagBoxThreshold) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSecBagBoxThresholdBase.cpp and SMSecBagBoxThreshold.cpp
// by using the macro IMPLEMENT_STATE(SecBagBoxThreshold)

//DECLARE_STATE(SecBagBoxThreshold)

#endif