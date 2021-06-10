//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:   SMSecBagBoxNotValid.h
//
// TITLE:  Class header for Invalid Bag State - Added for SR085
//
//
// AUTHOR: Shakti Kamal
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECBAGBOXNOTVALIDSTATE
#define _SECBAGBOXNOTVALIDSTATE

#include "SMSecBagBoxNotValidBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSecBagBoxNotValid : public SMSecBagBoxNotValidBase
{
public:
    SMSecBagBoxNotValid();
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);
protected:
	//POS105367
    virtual void showScreen(bool passwordVerified);
    virtual SMStateBase *stateAfterPasswordVerify();
    virtual SMStateBase *TimedOut(void);
    virtual SMStateBase *PSButtonGoBack(void);
    virtual SMStateBase *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
    virtual SMStateBase *OnWtDecrease(void);
    virtual SMStateBase *OnBackToLGW(void);
    virtual SMStateBase * TareNone(void);

    DECLARE_DYNCREATE(SMSecBagBoxNotValid)// MFC Runtime class/object information
};

#endif