//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMSelectDebitType.H
//
// TITLE:    Class header for Select Debit Type State
//
//
// AUTHOR:   Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SELECTDEBITTYPESTATE
#define _SELECTDEBITTYPESTATE

#include "SMSelectDebitTypeBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSelectDebitType : public SMSelectDebitTypeBase
{
public:
    SMSelectDebitType(const bool);
    SMSelectDebitType();
    SMStateBase *Deliver(const bool);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase  *Initialize(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual void UnInitialize(void);
    virtual SMStateBase  *OnWtDecrease(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSCancelKey(void);
    virtual SMStateBase  *PSButtonGoBack(void);

    
    DECLARE_DYNCREATE(SMSelectDebitType)// MFC Runtime class/object information

};

#endif
