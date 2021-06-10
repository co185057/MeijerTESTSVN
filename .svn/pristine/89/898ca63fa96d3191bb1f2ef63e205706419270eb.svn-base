//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmEBTAmountBase.h
//
// TITLE: Class header for Confirm EBT
//
//
// AUTHOR:    Michael Prather
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ConfirmEBTAmountBaseSTATE
#define _ConfirmEBTAmountBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 SMConfirmEBTAmountBase : public SMCustomerModeState
{
protected:
  TBTENDERTYPE nTndrType;
	long lEBTTotal;
  long lCashBack;

public:
	SMConfirmEBTAmountBase(const TBTENDERTYPE nTyp,const long lEBT, const long lCash);
	SMConfirmEBTAmountBase();
	SMStateBase *Deliver(const TBTENDERTYPE nTyp,const long lEBT, const long lCash);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
	virtual SMStateBase  *Initialize(void);
	virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
	virtual void UnInitialize(void);		// RFC 330050
	
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton2(void);
  virtual SMStateBase  *PSButton3(void);
  virtual SMStateBase  *PSButtonGoBack(void);

  DECLARE_DYNCREATE(SMConfirmEBTAmountBase)// MFC Runtime class/object information
};

//DECLARE_STATE(ConfirmEBTAmount)

#endif
