//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMSecMisMatchWeightBase.h
//
// TITLE:	Class header for Security Violation Screen-MisMatchWeight 
//
// AUTHOR:  Tara Duncan
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECMISMATCHWEIGHTBASESTATE
#define _SECMISMATCHWEIGHTBASESTATE

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

  SMSecMisMatchWeightBase : public STATE(SecOperatorPWState)
{
public:
#ifndef _CPPUNIT
  SMSecMisMatchWeightBase(const BEST prevState, const BEST approvedNextState);
  SMSecMisMatchWeightBase();
  SMStateBase *Deliver(const BEST argPrevState, const BEST argApprovedNextState);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);          // optional
  int nEscalationRAPTimeOut;				// RFC 233322

#endif // _CPPUNIT

protected:
#ifndef _CPPUNIT
  static VOID CALLBACK EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);	// RFC 233322
#endif // _CPPUNIT
  virtual void showScreen(bool passwordVerified);
#ifndef _CPPUNIT
  virtual SMStateBase  *stateAfterPasswordVerify();
  virtual SMStateBase  *OnBackToLGW(void);				// Smart Scale reported that the unmatched weight was removed
  virtual SMStateBase  *TimedOut(void);         // optional
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);	// Smart Scale reported an unmatched wt increase
  virtual SMStateBase  *OnMatchedWt(void);		// 218558 Added override function.  EAS motion sensor received in the wrong order must behave differently LPM103002
  virtual void Escalation2RAP(void);	// RFC 233322
  CString csCurContext;    // Tar 238987
  static SMSecMisMatchWeightBase* m_pMisMatch;  //TAR379130/379131
#endif //CPPUNIT
  static VOID CALLBACK ShowScreenTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);//TAR379130/379131
#ifndef _CPPUNIT
  void HandleShowScreenTimedOut(); //TAR379130/379131

  DECLARE_DYNCREATE(SMSecMisMatchWeightBase)// MFC Runtime class/object information

#endif //CPPUNIT
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSecMisMatchWeightBase(void);
//		or
//    DLLEXPORT SMStateBase *makeSMSecMisMatchWeightBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSecViolation objects.  This function is called by
// RETURNSTATE(DeactivateTheftTag) instead of calling new STATE(SecViolation) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSecMisMatchWeightBase.cpp and SMSecMisMatchWeight.cpp
// by using the macro IMPLEMENT_STATE(SecMisMatchWeightBase)

//DECLARE_STATE(SecMisMatchWeightBase)

#endif
