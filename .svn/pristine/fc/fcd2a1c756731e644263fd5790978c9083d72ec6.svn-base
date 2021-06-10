//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectPaymentBase.h
//
// TITLE: Class header for select Debit payment amount state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SelectPaymentBaseSTATE
#define _SelectPaymentBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSelectPaymentBase : public SMCustomerModeState
{
public:
  SMSelectPaymentBase(const TBTENDERTYPE);      // constructor
  SMSelectPaymentBase();
  SMStateBase *Deliver(const TBTENDERTYPE);
  virtual SMStateBase  *Initialize(void);        // required override
  virtual void UnInitialize();
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);

  virtual SMStateBase  *OnWtDecrease(void);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
protected:
  virtual SMStateBase  *PSButton3(void);         // button 3
  virtual SMStateBase  *PSListLookup(CString);      // a selection was made on a list

  virtual SMStateBase  *PSNumericKey(const long);	// spoofed cashback amt from AuxDisp
  virtual SMStateBase  *HandleAmount(const long);	

  //@@@INFOPOINT
  virtual SMStateBase  *PSButtonGoBack(void);         // override for GoBack Key
  //@@@INFOPOINT
  virtual SMStateBase  *PSCancelKey(void);/// override where needed

  
  //private:
	TBTENDERTYPE nTndrType;	
  int *m_pCashBackList;
  int m_CashBackListNum;

  DECLARE_DYNCREATE(SMSelectPaymentBase)// MFC Runtime class/object information
		
};

//DECLARE_STATE(SelectPayment)

#endif
