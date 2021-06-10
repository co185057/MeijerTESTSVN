//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMSecUnexpectedDecreaseBase.h
//
// TITLE:	Class header for Security Violation Screen-MisMatchWeight
//
// AUTHOR:  Thanhhieu Pham
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECUNEXPECTEDDECREASEBASESTATE
#define _SECUNEXPECTEDDECREASEBASESTATE
#include "SMSecOperatorPWState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

  SMSecUnexpectedDecreaseBase : public STATE(SecOperatorPWState)
{
public:
  SMSecUnexpectedDecreaseBase(const BEST prevState, const BEST approvedNextState);
  SMSecUnexpectedDecreaseBase();
  SMStateBase *Deliver(const BEST argPrevState, const BEST argApprovedNextState);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);
  int nEscalationRAPTimeOut;			// RFC 233322
  bool bPrevStateBagAndEAS;				// TAR 253626
protected:
  virtual SMStateBase  *stateAfterPasswordVerify();
  virtual void showScreen(bool passwordVerified);
  virtual SMStateBase  *OnBackToLGW(void);				// Smart Scale reported that the unmatched weight was removed
  virtual SMStateBase  *TimedOut(void);
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual SMStateBase  *PSButton1(void); //CHU 072502 RFC 208874 
  virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
  virtual SMStateBase  *OnMatchedWt(void);		// 218558 Added override function.  EAS motion sensor received in the wrong order must behave differently LPM103002
  virtual SMStateBase  *DMdf_EASNoMotion();
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);	//229084 Override unexpected increase if coming from BagAndEAS after a decrease LPM022503
  virtual SMStateBase  *OnWtDecrease();
  static VOID CALLBACK EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);	// RFC 233322
  void Escalation2RAP(void);	// RFC 233322
  CString csCurContext;    // Tar 238987


  DECLARE_DYNCREATE(SMSecUnexpectedDecreaseBase)// MFC Runtime class/object information

};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSecUnexpectedDecreaseBase(void);
//		or
//    DLLEXPORT SMStateBase *makeSMSecUnexpectedDecreaseBase(void);
//
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSecViolation objects.  This function is called by
// RETURNSTATE(DeactivateTheftTag) instead of calling new STATE(SecViolation) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSecMisMatchWeightBase.cpp and SMSecMisMatchWeight.cpp
// by using the macro IMPLEMENT_STATE(SecUnexpectedDecreaseBase)

//DECLARE_STATE(SecUnexpectedDecreaseBase)

#endif