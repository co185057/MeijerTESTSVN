//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTakeReceiptBase.h
//
// TITLE: Class header for Take Receipt State
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TakeReceiptBaseSTATE
#define _TakeReceiptBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMTakeReceiptBase : public SMCustomerModeState
{
public:
  SMTakeReceiptBase();
  virtual SMStateBase * Initialize(void);   // required override
  virtual void UnInitialize();
  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);

protected:
  virtual SMStateBase * TimedOut(void);

//private:
  int  retryReceiptAttempts;
	
  DECLARE_DYNCREATE(SMTakeReceiptBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMTakeReceipt(void);
// or
//    DLLEXPORT SMStateBase *makeSMTakeReceiptBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMTakeReceipt objects.  This function is called by
// RETURNSTATE(TakeReceipt) instead of calling new STATE(TakeReceipt) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMTakeReceiptBase.cpp and SMTakeReceipt.cpp
// by using the macro IMPLEMENT_STATE(TakeReceipt)

//DECLARE_STATE(TakeReceipt)

#endif
