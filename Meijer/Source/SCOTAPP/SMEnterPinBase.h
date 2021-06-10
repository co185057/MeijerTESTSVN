//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterPinBase.h
//
// TITLE: Class header for Enter PIN state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ENTERPINBASESTATE
#define _ENTERPINBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMEnterPinBase : public SMCustomerModeState
{
//private:
protected:
	CString csWorkingKey;
	bool fCashBackFlag;
	TBTENDERTYPE nTndrType;
    bool bPINCompleted;

public:
  SMEnterPinBase(const TBTENDERTYPE,const bool);
  SMEnterPinBase();
  SMStateBase *Deliver(const TBTENDERTYPE,const bool);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase  *Initialize(void);
  virtual void          UnInitialize(void);
  virtual SMStateBase  *TimedOut(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *OnWtDecrease(void);
protected:
  virtual SMStateBase  *PSButtonHelp(void);
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual SMStateBase  *PSNumericKey(const long);
  virtual SMStateBase  *DMEncryptorSecured(void);
  virtual SMStateBase  *DMEncryptorCanceled(void);
  virtual SMStateBase  *DMEncryptorCompleted(void);
  virtual SMStateBase  *DMEncryptorPinIsTooShort(void);
  virtual SMStateBase  *PSClearKey(void);

  DECLARE_DYNCREATE(SMEnterPinBase)// MFC Runtime class/object information
		
};

//DECLARE_STATE(EnterPin)

#endif
