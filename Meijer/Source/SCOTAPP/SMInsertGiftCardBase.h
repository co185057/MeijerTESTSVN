//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMInsertGiftCardBase.h
//
// TITLE:	Class header for InsertGiftCard state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _INSERTGIFTCARDBASESTATE
#define _INSERTGIFTCARDBASESTATE

#ifndef _CPPUNIT
#include "SMSecOperatorPWState.h"
#else
#include "FakeSMSecOperatorPWStateBase.h"
#endif

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
SMInsertGiftCardBase : public STATE(SecOperatorPWState)
{
public:
#ifndef _CPPUNIT
  SMInsertGiftCardBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);   
  SMInsertGiftCardBase(const BEST prevState, const BEST nextState);
  SMStateBase *Deliver(const BEST argPrevState, const BEST argNextState);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
#endif //_CPPUNIT
protected:
#ifndef _CPPUNIT
  virtual SMStateBase  *TimedOut(void);      
  virtual SMStateBase  *OnCouponSensor(void);
#endif //_CPPUNIT
  static VOID CALLBACK EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);	// RFC 233322
  virtual void Escalation2RAP(void);
  virtual SMStateBase *GPNotify(const GPEVT gpEvt);
#ifndef _CPPUNIT
  virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
  virtual SMStateBase  *stateAfterPasswordVerify();
  virtual void showScreen(bool passwordVerified);
#endif //_CPPUNIT
  bool m_bMotionEscalationFired;
  BEST m_Prevstate, m_Nextstate;
  
  DECLARE_DYNCREATE(SMInsertGiftCardBase)// MFC Runtime class/object information
		
};

#endif