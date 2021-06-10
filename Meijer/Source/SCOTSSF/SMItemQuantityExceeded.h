//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMItemQuantityExceeded.h
//
// TITLE: Class header for per item quantity exceeded
//
//
// AUTHOR:    gary miller
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ITEMQUANTITYEXCEEDEDSTATE
#define _ITEMQUANTITYEXCEEDEDSTATE

#include "SMItemQuantityExceededBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMItemQuantityExceeded : public SMItemQuantityExceededBase
{
public:
    SMItemQuantityExceeded();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize();
    virtual bool PSAssistMenuLegalForState();
protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSEnterKey(void);

    DECLARE_DYNCREATE(SMItemQuantityExceeded)// MFC Runtime class/object information
};

#endif