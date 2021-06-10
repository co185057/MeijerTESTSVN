//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCashPayment.h
//
// TITLE: Class header for Cash Payment State
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CASHPAYMENTSTATE
#define _CASHPAYMENTSTATE

#include "SMCashPaymentBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMCashPayment : public SMCashPaymentBase
{
public:
	SMCashPayment();

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
    virtual SMStateBase  *PSCancelKey(void);
    virtual SMStateBase  *PSButtonGoBack(void);

	SMStateBase *PSButtonHelp();
	static bool m_bIsHelpClick;

  DECLARE_DYNCREATE(SMCashPayment)// MFC Runtime class/object information
};

#endif