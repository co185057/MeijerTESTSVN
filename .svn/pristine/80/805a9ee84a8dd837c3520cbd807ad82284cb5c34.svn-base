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
SMPickingUpItemsBase : public STATE(SecOperatorPWState) //BZ 06/07/02 RFC 203538: Change base class to SMSecOperatorPWState
{
public:
#ifndef _CPPUNIT
  SMPickingUpItemsBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);          // optional
#endif //_CPPUNIT
protected:
#ifndef _CPPUNIT
  virtual SMStateBase  *TimedOut(void);         // optional
  virtual SMStateBase  *PSButton1(void); 
#endif
  virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
#ifndef _CPPUNIT
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *OnWtDecrease(void);
  //BZ 06/07/02 RFC 203538 Begin: Allow store approvals at the lane
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual void showScreen(bool passwordVerified);
  virtual SMStateBase  *stateAfterPasswordVerify();
#endif //_CPPUNIT
  virtual SMStateBase * OnBackToLGW(void);				// Smart Scale reported that the unmatched weight was removed
#ifndef _CPPUNIT
  virtual SMStateBase  *SASecMgrParse( const MessageElement*);	//427281  
  //RFC 203538 End
#endif //_CPPUNIT
  static VOID CALLBACK EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);	// RFC 202655 & 202656
  virtual void Escalation2RAP(void);	// RFC 202655 & 202656
  virtual SMStateBase *GPNotify(const GPEVT gpEvt);

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
