//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMInsertGiftCard.h
//
// TITLE:	Class header for InsertGiftCard state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _INSERTGIFTCARDSTATE
#define _INSERTGIFTCARDSTATE

#include "SMInsertGiftCardBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMInsertGiftCard : public SMInsertGiftCardBase
{
public:
    SMInsertGiftCard();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);   
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);

protected:
    virtual SMStateBase  *TimedOut(void);      
    virtual SMStateBase  *OnCouponSensor(void);
    virtual void Escalation2RAP(void);
    virtual SMStateBase  *GPNotify(const GPEVT gpEvt);
    virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
    virtual SMStateBase  *stateAfterPasswordVerify();
    virtual void showScreen(bool passwordVerified);

    DECLARE_DYNCREATE(SMInsertGiftCard)// MFC Runtime class/object information		
};

#endif


