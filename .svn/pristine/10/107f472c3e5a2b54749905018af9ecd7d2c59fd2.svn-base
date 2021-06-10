///////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTransportItem.h
//
// TITLE: Class header for Transport Item state
//
// $Header:
//
// AUTHOR:  Thanhhieu Pham
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TRANSPORTITEMSTATE
#define _TRANSPORTITEMSTATE

#include "SMTransportItemBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMTransportItem : public SMTransportItemBase
{
public:
    SMTransportItem();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);
    virtual SMStateBase  *TimedOut(void);
protected:
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnBackToLGW(void);

    virtual SMStateBase  *DMTakeawayReadyForItem();
    virtual SMStateBase  *DMScanner(void);

    DECLARE_DYNCREATE(SMTransportItem)// MFC Runtime class/object information

};

#endif
