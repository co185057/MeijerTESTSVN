//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterPin.h
//
// TITLE: Class header for entring PIN state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ENTERPINSTATE
#define _ENTERPINSTATE

#include "SMEnterPinBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMEnterPin : public SMEnterPinBase
{
public:
    SMEnterPin(const TBTENDERTYPE,const bool);
    SMEnterPin();
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

    DECLARE_DYNCREATE(SMEnterPin)// MFC Runtime class/object information

};

#endif
