//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmDebitAmountBase.h
//
// TITLE: Class header for Confirm Debit Amount state.
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ConfirmDebitAmountBaseSTATE
#define _ConfirmDebitAmountBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 SMConfirmDebitAmountBase : public SMCustomerModeState
{
protected:
  	TBTENDERTYPE nTndrType;
	long lTenderAmt;

public:
  SMConfirmDebitAmountBase(const TBTENDERTYPE, const long);
  SMConfirmDebitAmountBase();
  SMStateBase *Deliver(const TBTENDERTYPE, const long);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase  *Initialize(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *OnWtDecrease(void);
  virtual void UnInitialize(void);		// RFC 330050
	
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSEnterKey(void);
  virtual SMStateBase  *PSButtonGoBack(void);
	
  DECLARE_DYNCREATE(SMConfirmDebitAmountBase)// MFC Runtime class/object information
};

//DECLARE_STATE(ConfirmDebitAmount)

#endif
