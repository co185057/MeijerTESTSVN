//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmQuantityBase.h
//
// TITLE: Class header for Acknowledge Signature State
//
//
// AUTHOR:    Yafu Wang
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ACKNOWLEDGESIGBASESTATE
#define _ACKNOWLEDGESIGBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 SMAcknowledgeSigBase : public SMCustomerModeState
{
public:
	SMAcknowledgeSigBase();
  virtual SMStateBase  *Initialize(void);
  virtual void	 UnInitialize(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *DMSigcapStarted();
  static bool bAlreadyApproved;	
protected:
  virtual BOOL FileToSafeArray( COleVariant& vPngFile, CString& csTempFile );
  virtual SMStateBase	*PSButton1(void);
  virtual SMStateBase	*PSButton2(void);
  virtual SMStateBase	*PSEnterKey(void);
  virtual SMStateBase  *PSButtonHelp(void);

  virtual SMStateBase	*TimedOut(void);
	
  DECLARE_DYNCREATE(SMAcknowledgeSigBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMAcknowledgeSig(void);
// or
//    DLLEXPORT SMStateBase *makeSMAcknowledgeSigBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMAcknowledgeSig objects.  This function is called by
// RETURNSTATE(AcknowledgeSig) instead of calling new STATE(AcknowledgeSig) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMAcknowledgeSigBase.cpp and SMAcknowledgeSig.cpp
// by using the macro IMPLEMENT_STATE(AcknowledgeSig)

//DECLARE_STATE(AcknowledgeSig)

#endif // _ACKNOWLEDGESIGBASESTATE
