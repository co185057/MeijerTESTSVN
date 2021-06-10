//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMSelectCardType.H
//
// TITLE:    Class header for Select Card Type State
//
// $Header:
//
// AUTHOR:   Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SELECTCARDTYPESTATE
#define _SELECTCARDTYPESTATE

#include "SMSelectCardTypeBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMSelectCardType : public SMSelectCardTypeBase
{
public:
    SMSelectCardType();
    virtual SMStateBase *Initialize(void);
    virtual void     UnInitialize(void);
    virtual SMStateBase  *OnWtDecrease(void);
protected:
    virtual SMStateBase *PSButton1(void);
    virtual SMStateBase *PSButton2(void);
    virtual SMStateBase *PSButton3(void);
    virtual SMStateBase *PSButton4(void);
    virtual SMStateBase *PSCancelKey(void);
    virtual SMStateBase *SAEventOccurred(const long);
    virtual SMStateBase  *PSButtonGoBack(void);

    DECLARE_DYNCREATE(SMSelectCardType)// MFC Runtime class/object information

};

#endif
