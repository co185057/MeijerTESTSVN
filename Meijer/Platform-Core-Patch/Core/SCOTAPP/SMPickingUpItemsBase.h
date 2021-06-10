//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMPickingUpItemsBase.h
//
// TITLE:	Class header forPickingUpItems Base Screen
//
// AUTHOR:  Tara Duncan
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PICKINGUPITEMSBASESTATE
#define _PICKINGUPITEMSBASESTATE

#include "SMSecOperatorPWState.h"	//BZ 06/07/02 RFC 203538: Change base class

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
SMPickingUpItemsBase : public STATE(SecOperatorPWState) //BZ 06/07/02 RFC 203538: Change base class to SMSecOperatorPWState
{
public:
  SMPickingUpItemsBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);          // optional
protected:
  virtual SMStateBase  *TimedOut(void);         // optional
  static VOID CALLBACK EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);	// RFC 202655 & 202656
  virtual SMStateBase  *PSButton1(void); 
  virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *OnWtDecrease(void);
  //BZ 06/07/02 RFC 203538 Begin: Allow store approvals at the lane
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual void showScreen(bool passwordVerified);
  virtual SMStateBase  *stateAfterPasswordVerify();
  virtual SMStateBase * OnBackToLGW(void);				// Smart Scale reported that the unmatched weight was removed
  //RFC 203538 End
  void Escalation2RAP(void);	// RFC 202655 & 202656

  CString m_sButton;       // String of button clicked by RAP operator
  CString csCurContext;    // Tar 238987
  int nCountForPeriodicRemovingItemsAudio;

  DECLARE_DYNCREATE(SMPickingUpItemsBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMPickingUpItemsBase(void);
//		or
//    DLLEXPORT SMStateBase *makeSMPickingUpItemsBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates theSMPickingUpItemsBase objects.  This function is called by
// RETURNSTATE(SMPickingUpItems) instead of calling new STATE(SMPickingUpItems) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMPickingUpItemsBase.cpp and SMPickingUpItems.cpp
// by using the macro IMPLEMENT_STATE(SMPickingUpItems)

//DECLARE_STATE(SMPickingUpItems)

#endif
