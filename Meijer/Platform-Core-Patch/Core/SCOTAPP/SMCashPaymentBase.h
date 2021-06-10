//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCashPaymentBase.h
//
// TITLE: Class header for Cash Payment State
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CASHPAYMENTBASESTATE
#define _CASHPAYMENTBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 SMCashPaymentBase : public SMCustomerModeState
{
public:
  SMCashPaymentBase();
  virtual SMStateBase  *Initialize(void);
  virtual void     UnInitialize(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *OnWtDecrease(void);
protected:
  virtual SMStateBase  *PSButton3(void);
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *DMAcceptor(void);
  virtual SMStateBase  *SAEventOccurred(const long);
  virtual SMStateBase  *TimedOut(void);  
  //@@@INFOPOINT
virtual SMStateBase  *PSCancelKey(void);/// override where needed
  virtual SMStateBase  *PSButtonGoBack(void);
  //@@@INFOPOINT
  DECLARE_DYNCREATE(SMCashPaymentBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMCashPayment(void);
// or
//    DLLEXPORT SMStateBase *makeSMCashPaymentBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMCashPayment objects.  This function is called by
// RETURNSTATE(CashPayment) instead of calling new STATE(CashPayment) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMCashPaymentBase.cpp and SMCashPayment.cpp
// by using the macro IMPLEMENT_STATE(CashPayment)

//DECLARE_STATE(CashPayment)

#endif
