//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMVoidApprovalBase.h
//
// TITLE:	Class header for Assistance Needed for Voided item state
//
// AUTHOR:  Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _VOIDAPPROVALBASESTATE
#define _VOIDAPPROVALBASESTATE

#ifndef _CPPUNIT
#include "SMOperatorPasswordState.h"
#else
#include "FakeSMOperatorPasswordStateBase.h"
#endif // _CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

  SMVoidApprovalBase : public STATE(OperatorPasswordState)
{
public:
  SMVoidApprovalBase();
#ifndef _CPPUNIT
  SMVoidApprovalBase(const BEST prevState, const BEST approvedNextState);
  SMStateBase *Deliver(const BEST argPrevState, const BEST approvedNextState);
  virtual SMStateBase  *Initialize(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *OnWtDecrease(void);
#endif // _CPPUNIT
  virtual void UnInitialize(void);          // optional
#ifndef _CPPUNIT
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
protected:
  virtual void showScreen(bool passwordVerified);
  virtual SMStateBase * stateAfterPasswordVerify();

  virtual SMStateBase  *TimedOut(void);         // optional
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
    virtual SMStateBase *SASecMgrParse( const MessageElement *me);
  
  BEST Prevstate, Nextstate;

#endif // _CPPUNIT
  DECLARE_DYNCREATE(SMVoidApprovalBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMVoidApprovalBase(void);
//		or
//    DLLEXPORT SMStateBase *makeSMVoidApprovalBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMVoidApproval objects.  This function is called by
// RETURNSTATE(DeactivateTheftTag) instead of calling new STATE(VoidApproval) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMVoidApprovalBase.cpp and SMVoidApproval.cpp
// by using the macro IMPLEMENT_STATE(VoidApproval)

//DECLARE_STATE(VoidApproval)

#endif
