//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMVoucherPayment.h
//
// TITLE: Class header for Voucher Payment State
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _VOUCHERPAYMENTSTATE
#define _VOUCHERPAYMENTSTATE

#include "SMVoucherPaymentBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMVoucherPayment : public SMVoucherPaymentBase
{
public:
    SMVoucherPayment();
    virtual SMStateBase  *Initialize(void);
    virtual void     UnInitialize(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);
protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *DMScanner(void);
    virtual SMStateBase  *SAEventOccurred(const long);
    virtual SMStateBase  *PSCancelKey(void);
    virtual SMStateBase  *PSButtonGoBack(void);

  DECLARE_DYNCREATE(SMVoucherPayment)// MFC Runtime class/object information
};

#endif