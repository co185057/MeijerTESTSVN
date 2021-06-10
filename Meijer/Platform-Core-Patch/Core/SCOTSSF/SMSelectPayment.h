//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectPayment.h
//
// TITLE: Class header for select Debit payment amount state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SelectPaymentSTATE
#define _SelectPaymentSTATE

#include "SMSelectPaymentBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSelectPayment : public SMSelectPaymentBase
{

public:
	SMSelectPayment(const TBTENDERTYPE);      // constructor
	SMSelectPayment();
	SMStateBase *Deliver(const TBTENDERTYPE);
	
  DECLARE_DYNCREATE(SMSelectPayment)// MFC Runtime class/object information
		
};

#endif
