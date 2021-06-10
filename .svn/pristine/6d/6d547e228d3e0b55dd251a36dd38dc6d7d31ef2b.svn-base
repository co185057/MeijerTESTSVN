//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTenderBalance.h
//
// TITLE: Class header for TenderBalance State
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TENDERBALANCESTATE
#define _TENDERBALANCESTATE

#include "SMTenderBalanceBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMTenderBalance : public SMTenderBalanceBase
{
public:
    SMTenderBalance();
    virtual SMStateBase *Initialize(void);
    virtual void UnInitialize(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);

protected:
    virtual SMStateBase *TimedOut(void);
    virtual SMStateBase *PSButton1(void);
    virtual SMStateBase *PSEnterKey(void);

    DECLARE_DYNCREATE(SMTenderBalance)// MFC Runtime class/object information

};

#endif