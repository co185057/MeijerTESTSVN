//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMSelectDebitTypeBase.H
//
// TITLE:    Class header for Select Debit Type State
//
//
// AUTHOR:   Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SelectDebitTypeBaseSTATE
#define _SelectDebitTypeBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSelectDebitTypeBase : public SMCustomerModeState
{
private:
  bool fCashBack;

public:
  SMSelectDebitTypeBase(const bool);
  SMSelectDebitTypeBase();
  SMStateBase *Deliver(const bool);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase  *Initialize(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual void UnInitialize(void);		// RFC 330050

  virtual SMStateBase  *OnWtDecrease(void);
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton2(void);
  virtual SMStateBase  *PSButton3(void);
  virtual SMStateBase  *PSCancelKey(void);
  //@@@INFOPOINT
  virtual SMStateBase  *PSButtonGoBack(void);         // override for GoBack Key
  //@@@INFOPOINT
	
  DECLARE_DYNCREATE(SMSelectDebitTypeBase)// MFC Runtime class/object information
		
};

//DECLARE_STATE(SelectDebitType)

#endif
