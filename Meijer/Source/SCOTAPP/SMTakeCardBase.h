//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTakeCardBase.h
//
// TITLE: Class header for Take Card State
//
//
// AUTHOR:    Peter Denhoed, Updated DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TakeCardBaseSTATE
#define _TakeCardBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMTakeCardBase : public SMCustomerModeState
{
public:
	SMTakeCardBase(const bool fEOT, const bool fCardError = false);
	SMTakeCardBase();
	SMStateBase *Deliver(const bool fEOT, const bool fCardError = false);
	virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *Initialize(void);
  virtual void     UnInitialize(void);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
	bool fEndOfTran; // TAR 272140 

protected:
   bool    fTimedOutOnce;
  virtual SMStateBase  *TimedOut(void);
  virtual SMStateBase  *DMCardRemoved(void);
  virtual SMStateBase  *OnWtDecrease(void);

//private:
  bool     fEjectSuccess; //418791
  bool     fCardError;
  bool bCreateAndDispenseCalled; // TAR 272140	
  DECLARE_DYNCREATE(SMTakeCardBase)// MFC Runtime class/object information
};

//DECLARE_STATE(TakeCard)

#endif
