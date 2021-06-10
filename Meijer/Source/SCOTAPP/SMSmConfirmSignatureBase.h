//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSMSmConfirmSignatureBase.h
//
// TITLE: Class header for Confirm Signature
//
//
// AUTHOR:    Peter Denhoed
//
// TAR#127527	LPM122299 ConfirmSignatureBase is a store mode screen
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CONFIRMSIGNATUREBASESTATE
#define _CONFIRMSIGNATUREBASESTATE

#include "SMStoreModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
SMSmConfirmSignatureBase : public STATE(StoreModeState)	//SMCustomerModeState TAR#127527
{
public:
  SMSmConfirmSignatureBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);

  virtual SMStateBase  *OnWtDecrease(void);

protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton2(void);
  virtual SMStateBase  *TimedOut(void);
  virtual SMStateBase  *PSButtonHelp(void);
  CString csCurContext;    // Tar 238987

  DECLARE_DYNCREATE(SMSmConfirmSignatureBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMConfirmSignature(void);
// or
//    DLLEXPORT SMStateBase *makeSMConfirmSignatureBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMConfirmSignature objects.  This function is called by
// RETURNSTATE(ConfirmSignature) instead of calling new STATE(ConfirmSignature) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMConfirmSignatureBase.cpp and SMConfirmSignature.cpp
// by using the macro IMPLEMENT_STATE(ConfirmSignature)

//DECLARE_STATE(ConfirmSignature)

#endif
