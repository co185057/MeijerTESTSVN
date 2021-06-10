//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmConfirmSuspendedTransaction.h
//
// TITLE: Class header for store mode ConfirmSuspendedTransaction state
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMCONFIRMSUSPENDEDTRANSACTIONSTATE
#define _SMCONFIRMSUSPENDEDTRANSACTIONSTATE

#include "SMSmConfirmSuspendedTransactionBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmConfirmSuspendedTransaction : public SMSmConfirmSuspendedTransactionBase
{
public:
    SMSmConfirmSuspendedTransaction();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase *PSReceiptUp(void);
    virtual SMStateBase *PSReceiptDown(void);

    DECLARE_DYNCREATE(SMSmConfirmSuspendedTransaction)// MFC Runtime class/object information
};

#endif