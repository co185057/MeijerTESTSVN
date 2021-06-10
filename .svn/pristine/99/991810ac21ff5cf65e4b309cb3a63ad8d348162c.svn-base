//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAuthorizationBase.h
//
// TITLE: Class header for store mode Authorization state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmAuthorizationBaseSTATE
#define _SmAuthorizationBaseSTATE

#include "SMStoreModeState.h"
#include "SMStoreModeMenu.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmAuthorizationBase : public STATE(StoreModeMenu)
{
public:
  SMSmAuthorizationBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);

protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton2(void);
  virtual SMStateBase  *PSButton3(void);
  virtual SMStateBase  *PSButton4(void);
  virtual SMStateBase  *PSButton5(void);
  virtual SMStateBase  *PSButton6(void);
  //Button7 is blank
  virtual SMStateBase  *PSButton8(void);
  // NewUI
  virtual SMStateBase  *PSReceiptUp(void);     // to disable and enable scroll buttons when 
  virtual SMStateBase  *PSReceiptDown(void);   // to disable and enable scroll buttons when 
  // NewUI
  SMStateBase* sReturnState;   // Tar 213624 CHU 041503 
  CString csCurContext;    // Tar 238987
  DECLARE_DYNCREATE(SMSmAuthorizationBase)// MFC Runtime class/object information
  virtual void         InitUI(void);            // RFC 263291
  virtual void         InitButtons(void);       // RFC 263291
  virtual void         InitLeadThruText(void);  // RFC 263291
  virtual void         InitReceiptArea(void);   // RFC 263291
  virtual void         InitLanguage(void);      // RFC 263291
  virtual void         InitDevices(void);       // RFC 263291

  virtual SMStateBase  *OnGoBackButton(void); 

  virtual void ShowSuspendButton();
  virtual void ShowCloseLaneButton();
  virtual void ShowAssistModeButton();
  virtual void ShowMediaStatusButton();
  virtual void ShowReportingButton();
  virtual void ShowSystemFunctionsButton();
  virtual void ShowGoBackButton();

  virtual SMStateBase  *PSSwitchContext(BOOL bRemote, CString csNewContext); //For handle SwitchContext action
  virtual void ShowSMInfo();
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSmAuthorization(void);
// or
//    DLLEXPORT SMStateBase *makeSMSmAuthorizationBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSmAuthorization objects.  This function is called by
// RETURNSTATE(SmAuthorization) instead of calling new STATE(SmAuthorization) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSmAuthorizationBase.cpp and SMSmAuthorization.cpp
// by using the macro IMPLEMENT_STATE(SmAuthorization)

//DECLARE_STATE(SmAuthorization)

#endif
