//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMInsertCouponBase.h
//
// TITLE:	Class header for InsertCoupon state
//
// AUTHOR:  Aiqin Hou
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _INSERTCOUPONBASESTATE
#define _INSERTCOUPONBASESTATE

#include "SMSecOperatorPWState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
SMInsertCouponBase : public STATE(SecOperatorPWState)
{
public:
  SMInsertCouponBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);   
  SMInsertCouponBase(const BEST prevState, const BEST nextState);
  SMStateBase *Deliver(const BEST argPrevState, const BEST argNextState);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual bool PSAssistMenuLegalForState() { return true; };
protected:
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual SMStateBase  *TimedOut(void);      
  virtual SMStateBase  *OnCouponSensor(void);
  static VOID CALLBACK EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);	// RFC 233322
  void Escalation2RAP(void);
  virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
  virtual SMStateBase  *stateAfterPasswordVerify();
  virtual void showScreen(bool passwordVerified);
  bool m_bMotionEscalationFired;

  BEST m_Prevstate, m_Nextstate;
  
  DECLARE_DYNCREATE(SMInsertCouponBase)// MFC Runtime class/object information
		
};

//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMInsertCoupon(void);
// or
//    DLLEXPORT SMStateBase *makeSMInsertCouponBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMInsertCoupon objects.  This function is called by
// RETURNSTATE(InsertCoupon) instead of calling new STATE(InsertCoupon) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMInsertCouponBase.cpp and SMInsertCoupon.cpp
// by using the macro IMPLEMENT_STATE(InsertCoupon)

//DECLARE_STATE(InsertCoupon)

#endif
