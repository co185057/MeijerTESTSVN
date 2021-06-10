//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMUnDeActivatedItemApprovalBase.h
//
// TITLE:	Class header for Assistance Needed for UnDeActivated item state
//
// AUTHOR:  Anurag Mishra
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SMUNDEACTIVATEDITEMAPPROVALBASE__
#define __SMUNDEACTIVATEDITEMAPPROVALBASE__
#include "SMOperatorPasswordState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

  SMUnDeActivatedItemApprovalBase : public STATE(OperatorPasswordState)
{
public:
  SMUnDeActivatedItemApprovalBase(const BEST prevState, const BEST approvedNextState);
  SMUnDeActivatedItemApprovalBase();
  SMStateBase *Deliver(const BEST argPrevState, const BEST approvedNextState);
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);          // optional
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
protected:
  virtual void showScreen(bool passwordVerified);
  virtual SMStateBase * stateAfterPasswordVerify();

  virtual SMStateBase  *TimedOut(void);         // optional
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
  virtual SMStateBase  *OnUnDeActivatedTheftTag(void);
  virtual SMStateBase  *OnMatchedWt(void);		// Smart Scale reported a matched weight TAR203166

  BEST Prevstate, Nextstate;

private:

	int m_tempView;

  DECLARE_DYNCREATE(SMUnDeActivatedItemApprovalBase)// MFC Runtime class/object information

};

//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMUnDeActivatedItemApprovalBase(void);
//		or
//    DLLEXPORT SMStateBase *makeSMUnDeActivatedItemApprovalBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMVoidApproval objects.  This function is called by
// RETURNSTATE(DeactivateTheftTag) instead of calling new 
// STATE(UnDeActivatedItemApproval) so that SCOTSSF works correctly.
//
// The function is implemented in SMUnDeActivatedItemApprovalBase.cpp and 
// SMUnDeActivatedItemApproval.cpp
// by using the macro IMPLEMENT_STATE(UnDeActivatedItemApproval)

// DECLARE_STATE(UnDeActivatedItemApproval)

#endif
