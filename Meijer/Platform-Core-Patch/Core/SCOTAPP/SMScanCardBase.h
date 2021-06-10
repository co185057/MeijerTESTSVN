//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMScanCardBase.H
//
// TITLE:    Class header for Select Card Type State
//
//
// AUTHOR:   Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SCANCARDBASESTATE
#define _SCANCARDBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
SMScanCardBase : public SMCustomerModeState
{
	protected:
  	TBTENDERTYPE nTndrType;
	long lTenderAmt;
public:
  SMScanCardBase(const TBTENDERTYPE, const long );
  SMScanCardBase();
  SMStateBase *Deliver(const TBTENDERTYPE, const long );
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase *Initialize(void);
  virtual void     UnInitialize(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);

  virtual SMStateBase  *OnWtDecrease(void);
protected:
  virtual SMStateBase *PSCancelKey(void);

  virtual SMStateBase *SAEventOccurred(const long);
  //@@@INFOPOINT
  virtual SMStateBase  *PSButtonGoBack(void);         // override for GoBack Key
  //@@@INFOPOINT
   virtual SMStateBase  *DMScanner(void);
	
  DECLARE_DYNCREATE(SMScanCardBase)// MFC Runtime class/object information
		
};

//DECLARE_STATE(ScanCard)

#endif
