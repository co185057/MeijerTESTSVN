//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTakeReceipt.h
//
// TITLE: Class header for Take Receipt State
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAKERECEIPTSTATE
#define _TAKERECEIPTSTATE

#include "SMTakeReceiptBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMTakeReceipt : public SMTakeReceiptBase
{
public:
    SMTakeReceipt();
    virtual SMStateBase * Initialize(void);
    virtual void UnInitialize();
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);

protected:
    virtual SMStateBase * TimedOut(void);
	SMStateBase * CheckGiftRebateReceipt(void);


    DECLARE_DYNCREATE(SMTakeReceipt)// MFC Runtime class/object information

};

#endif