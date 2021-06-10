//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMSecUnExpectedIncreaseBase.h
//
// TITLE:	Class header for Security Violation Screen
//
// AUTHOR:  Tara Duncan
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECUNEXPECTEDINCREASEBASESTATE
#define _SECUNEXPECTEDINCREASEBASESTATE

#ifdef _CPPUNIT
    #include "FakeSMSecOperatorPWStateBase.h"
#else
    #include "SMSecOperatorPWState.h"
#endif // _CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

  SMSecUnExpectedIncreaseBase : public STATE(SecOperatorPWState)
{
public:
#ifndef _CPPUNIT
  SMSecUnExpectedIncreaseBase(const BEST prevState, const BEST approvedNextState);
  SMSecUnExpectedIncreaseBase();
  SMStateBase *Deliver(const BEST argPrevState, const BEST argApprovedNextState);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);          // optional
  int nEscalationRAPTimeOut;				// RFC 233322
  virtual bool IgnoreTBMessageInSecurityState();

#endif // _CPPUNIT
protected:
  virtual void showScreen(bool passwordVerified);
  virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
#ifndef _CPPUNIT
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *stateAfterPasswordVerify();
  virtual SMStateBase  *OnBackToLGW(void);				// Smart Scale reported that the unmatched weight was removed
  virtual SMStateBase  *TimedOut(void);         // optional
  virtual SMStateBase  *PSButtonGoBack(void);

  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *DMTakeawaySendScaleStatus();		//.tve.
  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *DMdf_EASNoMotion();
  virtual SMStateBase  *OnTBNeedData(void);//SR801
  static VOID CALLBACK EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);	// RFC 233322
  virtual void Escalation2RAP(void);	// RFC 233322
  static SMSecUnExpectedIncreaseBase* m_pUnExpectedIncrease;  //TAR379130/379131
#endif // _CPPUNIT
  static VOID CALLBACK ShowScreenTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);//TAR379130/379131
#ifndef _CPPUNIT
  void HandleShowScreenTimedOut(); //TAR379130/379131

#endif // _CPPUNIT
  static bool bTABIsFlushing;
#ifndef _CPPUNIT

  CString csCurContext;    // Tar 238987
  DECLARE_DYNCREATE(SMSecUnExpectedIncreaseBase)// MFC Runtime class/object information
		
#endif //CPPUNIT
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSecUnExpectedIncreaseBase(void);
//		or
//    DLLEXPORT SMStateBase *makeSMSecUnExpectedIncreaseBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSecViolation objects.  This function is called by
// RETURNSTATE(DeactivateTheftTag) instead of calling new STATE(SecViolation) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSecUnExpectedIncreaseBase.cpp and SMSecViolation.cpp
// by using the macro IMPLEMENT_STATE(SecViolation)

//DECLARE_STATE(SecViolation)

#endif
