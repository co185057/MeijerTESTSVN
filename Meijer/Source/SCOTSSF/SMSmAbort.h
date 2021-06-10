//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAbort.h
//
// TITLE: Class header for store mode abort state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMABORTSTATE
#define _SMABORTSTATE

#include "SMSmAbortBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmAbort : public SMSmAbortBase
{
public:
    SMSmAbort();
     virtual SMStateBase  *Initialize(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);
protected:
    virtual void UnInitialize(void);
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSEnterKey(void);
    virtual SMStateBase  *PSCancelKey(void);
    virtual SMStateBase *PSReceiptUp(void);
    virtual SMStateBase *PSReceiptDown(void);

    DECLARE_DYNCREATE(SMSmAbort)// MFC Runtime class/object information
};

#endif