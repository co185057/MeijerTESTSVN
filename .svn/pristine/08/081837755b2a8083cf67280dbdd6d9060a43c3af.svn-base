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

#ifdef _CPPUNIT
    #include "FakeSMSecOperatorPWStateBase.h"
#else
    #include "SMSecOperatorPWState.h"
#endif // _CPPUNIT

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
#ifndef _CPPUNIT
  SMStateBase *Deliver(const BEST argPrevState);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);          // optional
#endif
protected:
#ifndef _CPPUNIT
  virtual void showScreen(bool passwordVerified);
  virtual SMStateBase *stateAfterPasswordVerify();
  virtual SMStateBase *TimedOut(void);         // optional
  virtual SMStateBase *PSButtonGoBack(void);
#endif
  virtual SMStateBase *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
#ifndef _CPPUNIT
  virtual SMStateBase *OnWtDecrease(void);
  virtual SMStateBase *OnBackToLGW(void);
  virtual SMStateBase * TareNone(void);
#endif


  DECLARE_DYNCREATE(SMSecBagBoxNotValidBase)// MFC Runtime class/object information
		
};

#endif