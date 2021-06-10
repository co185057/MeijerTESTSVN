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

//#include "SMCustomerModeState.h"
#include "SMSecOperatorPWState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMTakeChangeBase : public STATE(SecOperatorPWState)  
{
//private:
protected:
  bool fEndOfTrans;
  bool  fPaperMoneyReturned;
  long  saveChangeDue;
  bool fTimedOutOnce;
  bool fIs3xHardware;  //merged TAR232798 to 3.1.1 - fixed by Chen
  bool m_fBillsDispensed;  //TAR 391559
 
  virtual SMStateBase  *TimedOut();
  virtual SMStateBase  *OnMotionCashOutSensor(void);
  virtual SMStateBase  *OnNoMotionCashOutSensor(void);
  static VOID CALLBACK EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);	// RFC 233322
  void Escalation2RAP(void);
  virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual void showScreen(bool passwordVerified);
  virtual SMStateBase  *stateAfterPasswordVerify();

  int nCountForPeriodicTakeChangeAudio;
  CString csCurContext;    // Tar 238987
  bool m_bMotionEscalationFired;
	
public:
	SMTakeChangeBase(const bool);
	SMTakeChangeBase();
	SMStateBase *Deliver(const bool);
  virtual SMStateBase * Initialize();
  virtual void UnInitialize();
  virtual SMStateBase * DMCashRemoved(void);
  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);

	
private:
	
  DECLARE_DYNCREATE(SMTakeChangeBase)// MFC Runtime class/object information
};

//DECLARE_STATE(TakeChange)

// Local function
bool WereBillsDispensed();  // Dynamic Bill Detection
bool Is3xHardware();        // To see if it is 3.x hardware - merged tar 232798 to 3.1.1 - fixed by Chen

#endif
