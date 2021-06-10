//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMRequestSigBase.h
//
// TITLE: Class header for requesting a signature from the customer
//
//
// AUTHOR:    Yafu Wang
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REQUESTSIGBASESTATE
#define _REQUESTSIGBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 SMRequestSigBase : public SMCustomerModeState
{

public:
  SMRequestSigBase();
  virtual SMStateBase  *Initialize(void);
  virtual SMStateBase  *TimedOut(void);
  virtual SMStateBase  *PSButtonHelp(void);
  virtual SMStateBase  *DMSigcapStarted();
  virtual SMStateBase  *DMSigcapEnded();
  virtual void UnInitialize(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);

  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *OnBackToLGW(void);		//Tar199449 Smart Scale reported that the unmatched weight was removed


protected:
  BOOL m_bSigStarted;
  DECLARE_DYNCREATE(SMRequestSigBase)  // MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMRequestSig(void);
// or
//    DLLEXPORT SMStateBase *makeSMRequestSigBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMRequestSig objects.  This function is called by
// RETURNSTATE(RequestSig) instead of calling new STATE(RequestSig) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMRequestSigBase.cpp and SMRequestSig.cpp
// by using the macro IMPLEMENT_STATE(RequestSig)

//DECLARE_STATE(RequestSig)

#endif // _REQUESTSIGBASESTATE
