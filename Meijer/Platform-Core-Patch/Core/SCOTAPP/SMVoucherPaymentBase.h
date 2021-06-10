//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMVoucherPaymentBase.h
//
// TITLE: Class header for Voucher Payment State
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _VOUCHERPAYMENTBASESTATE
#define _VOUCHERPAYMENTBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 SMVoucherPaymentBase : public SMCustomerModeState
{
public:
  SMVoucherPaymentBase();
  virtual SMStateBase  *Initialize(void);
  virtual void     UnInitialize(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *OnWtDecrease(void);
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *DMScanner(void);
  virtual SMStateBase  *SAEventOccurred(const long);

  //@@@INFOPOINT
virtual SMStateBase  *PSCancelKey(void);/// override where needed
  virtual SMStateBase  *PSButtonGoBack(void);
  //@@@INFOPOINT
  DECLARE_DYNCREATE(SMVoucherPaymentBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMVoucherPayment(void);
// or
//    DLLEXPORT SMStateBase *makeSMVoucherPaymentBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMVoucherPayment objects.  This function is called by
// RETURNSTATE(VoucherPayment) instead of calling new STATE(VoucherPayment) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMVoucherPaymentBase.cpp and SMVoucherPayment.cpp
// by using the macro IMPLEMENT_STATE(VoucherPayment)

//DECLARE_STATE(VoucherPayment)

#endif
