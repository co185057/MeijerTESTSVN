//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterQuantity.h
//
// TITLE: Class header for enter quantity state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ENTERQUANTITYSTATE
#define _ENTERQUANTITYSTATE

#include "SMEnterQuantityBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMEnterQuantity : public SMEnterQuantityBase
{
public:
    SMEnterQuantity();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize();
    virtual bool PSAssistMenuLegalForState();
protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSNumericKey(const long);
    virtual SMStateBase  *PSClearKey(void);
    virtual SMStateBase  *PSEnterKey(void);
    virtual SMStateBase  *PSCancelKey(void);
    virtual SMStateBase  *PSBackSpaceKey(void);
    virtual SMStateBase  *PSButtonGoBack(void);


  DECLARE_DYNCREATE(SMEnterQuantity)// MFC Runtime class/object information

};

#endif