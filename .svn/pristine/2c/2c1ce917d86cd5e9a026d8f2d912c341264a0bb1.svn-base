//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTenderBalanceBase.h
//
// TITLE: Class header for TenderBalance State
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TENDERBALANCEBASESTATE
#define _TENDERBALANCEBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMTenderBalanceBase : public SMCustomerModeState
{
public:
	SMTenderBalanceBase();
  virtual SMStateBase *Initialize(void);
  virtual void UnInitialize(void);
  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);

protected:
  virtual SMStateBase *TimedOut(void);
  virtual SMStateBase *PSButton1(void);
  virtual SMStateBase *PSEnterKey(void);

  long m_lGiftCardBalance;
	
  DECLARE_DYNCREATE(SMTenderBalanceBase)// MFC Runtime class/object information
		
};

#endif
