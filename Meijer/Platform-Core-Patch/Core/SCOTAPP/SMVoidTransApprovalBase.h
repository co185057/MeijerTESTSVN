//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMVoidTransApprovalBase.h
//
// TITLE:	Class header for Assistance Needed for Voided transaction state
//
// AUTHOR:  Aiqin Hou
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _VOIDTRANSAPPROVALBASESTATE
#define _VOIDTRANSAPPROVALBASESTATE
#include "SMOperatorPasswordState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

  SMVoidTransApprovalBase : public STATE(OperatorPasswordState)
{
public:
  SMVoidTransApprovalBase(const BEST prevState, const BEST approvedNextState);
  SMVoidTransApprovalBase();
  SMStateBase *Deliver(const BEST argPrevState, const BEST approvedNextState);
  virtual SMStateBase  *Initialize(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *OnWtDecrease(void);
  virtual void UnInitialize(void);          // optional
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual void         ClearSecurityViolationLight(void); 
protected:
  virtual void showScreen(bool passwordVerified);
  virtual SMStateBase * stateAfterPasswordVerify();

  virtual SMStateBase  *TimedOut(void);         // optional
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
  virtual SMStateBase *HandleCancelTrasaction(CString csBttID);
  
  BEST Prevstate, Nextstate;

  DECLARE_DYNCREATE(SMVoidTransApprovalBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMVoidTransApprovalBase(void);
//		or
//    DLLEXPORT SMStateBase *makeSMVoidTransApprovalBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMVoidApproval objects.  This function is called by
// RETURNSTATE(DeactivateTheftTag) instead of calling new STATE(VoidApproval) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMVoidTransApprovalBase.cpp and SMVoidApproval.cpp
// by using the macro IMPLEMENT_STATE(VoidApproval)

//DECLARE_STATE(VoidApproval)

#endif
