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
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize();
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
	virtual SMStateBase *PSButton1(void);

protected:
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSListLookup(CString);
    virtual SMStateBase  *PSNumericKey(const long);
    virtual SMStateBase  *HandleAmount(const long);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *PSCancelKey(void);

    DECLARE_DYNCREATE(SMSelectPayment)// MFC Runtime class/object information

};

#endif
