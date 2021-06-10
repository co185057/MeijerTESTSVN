//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMUnknownPrice.h
//
// TITLE: Class header for unknown price State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _UNKNOWNPRICESTATE
#define _UNKNOWNPRICESTATE

#include "SMUnknownPriceBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMUnknownPrice : public SMUnknownPriceBase
{
public:
    SMUnknownPrice();                 // constructor
    virtual SMStateBase *Initialize(void);
    virtual bool PSAssistMenuLegalForState();
    virtual void UnInitialize(void);
protected:
    virtual SMStateBase *PSButton1(void);
    virtual SMStateBase *PSEnterKey(void);

    DECLARE_DYNCREATE(SMUnknownPrice)// MFC Runtime class/object information

};

#endif