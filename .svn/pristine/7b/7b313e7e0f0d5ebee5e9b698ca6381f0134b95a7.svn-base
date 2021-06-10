//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMCollectGiftCardBase.h
//
// TITLE:	Class header for CollectGiftCard state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _COLLECTGIFTCARDSTATEBASE
#define _COLLECTGIFTCARDSTATEBASE
#include "SMSecOperatorPWState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

  SMCollectGiftCardBase : public STATE(SecOperatorPWState)
{
public:
  SMCollectGiftCardBase(const BEST prevState, const BEST nextState);
  SMCollectGiftCardBase();
  SMStateBase *Deliver(const BEST prevState, const BEST nextState);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);          // optional

protected:
  virtual void showScreen(bool passwordVerified);
  virtual SMStateBase  *stateAfterPasswordVerify();
  virtual SMStateBase  *TimedOut(void);         // optional
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *OnWtDecrease(void);

  BEST m_Prevstate, m_Nextstate;

  DECLARE_DYNCREATE(SMCollectGiftCardBase)// MFC Runtime class/object information
		
};

#endif
