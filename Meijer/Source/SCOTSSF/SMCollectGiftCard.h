//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCollectGiftCard.h
//
// TITLE: Class header for CollectGiftCard state
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _COLLECTGIFTCARDSTATE
#define _COLLECTGIFTCARDSTATE

#include "SMCollectGiftCardBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMCollectGiftCard : public SMCollectGiftCardBase
{
public:
    SMCollectGiftCard();
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);

protected:
    virtual void showScreen(bool passwordVerified);
    virtual SMStateBase  *stateAfterPasswordVerify();
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);

    DECLARE_DYNCREATE(SMCollectGiftCard)// MFC Runtime class/object information
};

#endif
