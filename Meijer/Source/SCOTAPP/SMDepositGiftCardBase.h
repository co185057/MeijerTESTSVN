//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMDepositGiftCardBase.h
//
// TITLE: Class header for DepositGiftCard State
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DEPOSITGIFTCARDBASESTATE
#define _DEPOSITGIFTCARDBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMDepositGiftCardBase : public SMCustomerModeState
{
public:
  SMDepositGiftCardBase();
  virtual SMStateBase  *Initialize(void);
  virtual void         UnInitialize(void);
  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
	
protected:
  virtual SMStateBase *TimedOut(void);
  virtual SMStateBase  *PSButton1(void);
	
  DECLARE_DYNCREATE(SMDepositGiftCardBase)// MFC Runtime class/object information
};

#endif
