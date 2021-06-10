//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMDepositGiftCard.h
//
// TITLE: Class header for DepositGiftCard State
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DEPOSITGIFTCARDSTATE
#define _DEPOSITGIFTCARDSTATE

#include "SMDepositGiftCardBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMDepositGiftCard : public SMDepositGiftCardBase
{
public:
    SMDepositGiftCard();
    virtual SMStateBase  *Initialize(void);
    virtual void         UnInitialize(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);

protected:
    virtual SMStateBase *TimedOut(void);
    virtual SMStateBase  *PSButton1(void);
    

  DECLARE_DYNCREATE(SMDepositGiftCard)// MFC Runtime class/object information
};

#endif