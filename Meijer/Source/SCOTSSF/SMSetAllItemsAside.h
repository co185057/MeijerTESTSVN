//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSetAllItemsAside.h
//
// TITLE: Class header for unknown price State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SETALLITEMSASIDESTATE
#define _SETALLITEMSASIDESTATE

#include "SMSetAllItemsAsideBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSetAllItemsAside : public SMSetAllItemsAsideBase
{
public:
    SMSetAllItemsAside();             // constructor
    virtual SMStateBase  *Initialize(void);
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSEnterKey(void);


    DECLARE_DYNCREATE(SMSetAllItemsAside)// MFC Runtime class/object information

};

#endif