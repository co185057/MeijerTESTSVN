//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMSecBagBoxNotValidBase.h
//
// TITLE:	Class header for Bag Box Not Valid Screen (Done for SR085)
//
// AUTHOR:  Shakti Kamal
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECBAGBOXNOTVALIDBASESTATE
#define _SECBAGBOXNOTVALIDBASESTATE
#include "SMSecOperatorPWState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

  SMSecBagBoxNotValidBase : public STATE(SecOperatorPWState)
{
public:
  SMSecBagBoxNotValidBase(const BEST prevState);
  SMSecBagBoxNotValidBase();
  SMStateBase *Deliver(const BEST argPrevState);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);          // optional
protected:
  virtual void showScreen(bool passwordVerified);
  virtual SMStateBase *stateAfterPasswordVerify();
  virtual SMStateBase *TimedOut(void);         // optional
  virtual SMStateBase *PSButtonGoBack(void);
  virtual SMStateBase *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
  virtual SMStateBase *OnWtDecrease(void);
  virtual SMStateBase *OnBackToLGW(void);
  virtual SMStateBase * TareNone(void);


  DECLARE_DYNCREATE(SMSecBagBoxNotValidBase)// MFC Runtime class/object information
		
};

#endif