//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMThankYou.h
//
// TITLE: Class header for Thank You State
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _THANKYOUSTATE
#define _THANKYOUSTATE

#include "SMThankYouBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMThankYou : public SMThankYouBase
{
public:
    SMThankYou();
    virtual SMStateBase  *Initialize(void);
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnBackToLGW(void);
    virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
    virtual void UnInitialize();
    virtual SMStateBase* OnTransactionComplete(void);
    virtual SMStateBase *RAParse(MessageElement* );

protected:
    virtual void sayThankYou();
    virtual void Escalation2RAP(void);
    virtual SMStateBase *PSButton1( void );
	virtual SMStateBase *OnRAPUnsolicitedDataReceived(LPCTSTR sData); //tar 302736

    DECLARE_DYNCREATE(SMThankYou)// MFC Runtime class/object information

};

#endif