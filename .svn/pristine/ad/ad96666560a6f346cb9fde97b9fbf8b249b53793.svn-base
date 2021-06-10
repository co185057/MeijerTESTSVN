//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTakeChangeBase.h
//
// TITLE: Class header for Take Change State
//
// This state represents a successful transaction paid for in cash with
// change due back.
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TakeChangeBaseSTATE
#define _TakeChangeBaseSTATE

#ifndef _CPPUNIT
//#include "SMCustomerModeState.h"
#include "SMSecOperatorPWState.h"
#else
#include "FakeSMSecOperatorPWStateBase.h"
#endif //_CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMTakeChangeBase : public STATE(SecOperatorPWState)  
{
//private:
protected:
  bool fTimedOutOnce;
  bool fEndOfTrans;
  long  saveChangeDue;
  bool m_fBillsDispensed;  //TAR 391559

  virtual SMStateBase  *TimedOut();
#ifndef _CPPUNIT
  virtual SMStateBase  *OnMotionCashOutSensor(void);
#endif

  virtual SMStateBase  *OnNoMotionCashOutSensor(void);
  virtual bool WereBillsDispensed(void);

  static VOID CALLBACK EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);	// RFC 233322
  virtual void Escalation2RAP(void);
  virtual SMStateBase *GPNotify(const GPEVT gpEvt);
#ifndef _CPPUNIT
  virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual void showScreen(bool passwordVerified);
  virtual SMStateBase  *stateAfterPasswordVerify();

  int nCountForPeriodicTakeChangeAudio;
  CString csCurContext;    // Tar 238987
#endif
  bool m_bMotionEscalationFired;
public:
	SMTakeChangeBase(const bool);
	SMTakeChangeBase();
  virtual SMStateBase * Initialize();
#ifndef _CPPUNIT
	SMStateBase *Deliver(const bool);
  virtual void UnInitialize();
#endif //_CPPUNIT
  virtual SMStateBase * DMCashRemoved(void);
#ifndef _CPPUNIT
  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
#endif
	
private:
	
  DECLARE_DYNCREATE(SMTakeChangeBase)// MFC Runtime class/object information
};

//DECLARE_STATE(TakeChange)
// Local function
//bool Is3xHardware();        // To see if it is 3.x hardware - merged tar 232798 to 3.1.1 - fixed by Chen
#endif
