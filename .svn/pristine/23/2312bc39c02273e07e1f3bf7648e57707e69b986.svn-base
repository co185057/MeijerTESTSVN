//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMSecNewItemEntryBase.h
//
// TITLE:	Class header for New Item Entry Threshold Screen
//
// AUTHOR:  Thanhhieu Pham
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECNEWITEMENTRYBASESTATE
#define _SECNEWITEMENTRYBASESTATE
#include "SMSecOperatorPWState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

  SMSecNewItemEntryBase : public STATE(SecOperatorPWState)
{
public:
  SMSecNewItemEntryBase(const BEST prevState, const BEST approvedNextState);
  SMSecNewItemEntryBase();
  SMStateBase *Deliver(const BEST argPrevState, const BEST argApprovedNextState);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);          // optional
protected:
  virtual void showScreen(bool passwordVerified);
  virtual SMStateBase  *stateAfterPasswordVerify();
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);	// 04/19/02     // Smart Scale reported an unmatched wt increase
  virtual SMStateBase  *OnWtDecrease(void);			//Tar 204711
  virtual SMStateBase  *TimedOut(void);         // optional
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
  virtual SMStateBase  *OnBackToLGW(void);  		//Tar 204711
  
  DECLARE_DYNCREATE(SMSecNewItemEntryBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSecNewItemEntryBase(void);
//		or
//    DLLEXPORT SMStateBase *makeSMSecNewItemEntryBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSecNewItemEntry objects.  This function is called by
// RETURNSTATE(DeactivateTheftTag) instead of calling new STATE(SecNewItemEntry) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSecNewItemEntryBase.cpp and SMSecNewItemEntry.cpp
// by using the macro IMPLEMENT_STATE(SecNewItemEntry)

//DECLARE_STATE(SecNewItemEntry)

#endif