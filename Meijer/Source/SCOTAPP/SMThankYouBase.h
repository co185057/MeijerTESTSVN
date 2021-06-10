//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMThankYouBase.h
//
// TITLE: Class header for Thank You State
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ThankYouBaseSTATE
#define _ThankYouBaseSTATE

#ifndef _CPPUNIT
#include "SMCustomerModeState.h"
#else
#include "FakeSMCustomerModeState.h"
#endif //_CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMThankYouBase : public SMCUSTOMERMODESTATE
{
public:
#ifndef _CPPUNIT
  SMThankYouBase();
#endif //_CPPUNIT

  virtual SMStateBase  *Initialize(void);
  virtual SMStateBase  *TimedOut(void);
#ifndef _CPPUNIT 
  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *OnBackToLGW(void); //merged TAR232645 to 3.1.1 - fixed by Chen
  virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType); //rfc209702
#endif //_CPPUNIT
  virtual void UnInitialize();
#ifndef _CPPUNIT 
  virtual SMStateBase* OnTransactionComplete(void);
  virtual SMStateBase *RAParse(MessageElement* ); //TAR 371932 & tar 371918
#endif
protected:
#ifndef _CPPUNIT
  virtual void sayThankYou();
  static VOID CALLBACK EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime); //RFC209702
  virtual void Escalation2RAP(void);//RFC209702
#endif
  bool fThankYouAudioPlayed;
  bool m_bNormalExit;
  bool m_bSSFReminder1; // Solution specific reminder if any, associated msgid is 812=MSG_THANKYOUFIGTCARD
  static bool ms_bEscalationTimeOutFired;
  static bool ms_bThankYouTimerNoBagScaleTimeOutFired;
#ifndef _CPPUNIT
	//+SR832	
	static VOID CALLBACK PrintReceiptButtonTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
	void HandlePrintReceiptButtonTimedOut( void );
	void StartTimerForPrintReceiptButton( void ); 
	virtual SMStateBase *PSButton1( void ); //function that handles the Print Receipt button in Thank You screen
#endif
    virtual SMStateBase *GPNotify(const GPEVT gpEvt);
	bool m_bPrintReceiptExpire;	//member variable that handles the Print Receipt button timer expiration in Thank You screen
	static SMThankYouBase* m_pThankYou;
	//-SR832
private:
#ifndef _CPPUNIT
	void displaySuspendVoid( void );//RFC 276161
#endif
  DECLARE_DYNCREATE(SMThankYouBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMThankYou(void);
// or
//    DLLEXPORT SMStateBase *makeSMThankYouBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMThankYou objects.  This function is called by
// RETURNSTATE(ThankYou) instead of calling new STATE(ThankYou) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMThankYouBase.cpp and SMThankYou.cpp
// by using the macro IMPLEMENT_STATE(ThankYou)

//DECLARE_STATE(ThankYou)

#endif
