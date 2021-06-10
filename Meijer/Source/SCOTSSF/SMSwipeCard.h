//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSwipeCard.h
//
// TITLE: Class header for Confirn Signature
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SwipeCardSTATE
#define _SwipeCardSTATE

#include "SMSwipeCardBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSwipeCard : public SMSwipeCardBase
{

public:

    SMSwipeCard(const TBTENDERTYPE, const long);
    SMSwipeCard();
    SMStateBase *Deliver(const TBTENDERTYPE, const long);
    virtual SMStateBase *Initialize(void);
    virtual void     UnInitialize(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
protected:
    virtual SMStateBase *PSCancelKey(void);
    virtual SMStateBase *PSButton1(void);
    virtual SMStateBase *SAEventOccurred(const long);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *DMCardReader(void);
    virtual SMStateBase  *DMAcceptor(void);
    virtual SMStateBase *OnRAPUnsolicitedDataReceived(LPCTSTR sData);

    DECLARE_DYNCREATE(SMSwipeCard)// MFC Runtime class/object information
};


#endif