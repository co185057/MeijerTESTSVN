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
	
  DECLARE_DYNCREATE(SMCashPayment)// MFC Runtime class/object information
};

#endif