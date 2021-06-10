//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMInsertCoupon.h
//
// TITLE:	Class header for Insert Coupon state
//
// AUTHOR:	Aiqin Hou
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _INSERTCOUPONSTATE
#define _INSERTCOUPONSTATE

#include "SMInsertCouponBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMInsertCoupon : public SMInsertCouponBase
{
public:
    SMInsertCoupon();

    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);   
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual bool PSAssistMenuLegalForState();

protected:
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *TimedOut(void);      
    virtual SMStateBase  *OnCouponSensor(void);
    virtual void Escalation2RAP(void);
    virtual SMStateBase  *GPNotify(const GPEVT gpEvt);
    virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
    virtual SMStateBase  *stateAfterPasswordVerify();
    virtual void showScreen(bool passwordVerified);

	virtual long DMSaySecurity(const long nMsg, LPCTSTR szWave, HANDLE hWaitEvent =0);  //POS37681
    virtual SMStateBase *PSButton7(void); //POS33020

    DECLARE_DYNCREATE(SMInsertCoupon)// MFC Runtime class/object information		
};

#endif


