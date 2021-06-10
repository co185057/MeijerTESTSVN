//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMPickingUpItems.h
//
// TITLE: Class header for PickingUpItems state
//
// $Header:
//
// AUTHOR:  Tara Duncan
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PICKINGUPITEMSSTATE
#define _PICKINGUPITEMSSTATE

#include "SMPickingUpItemsBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMPickingUpItems: public SMPickingUpItemsBase
{
public:
    SMPickingUpItems();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);
protected:
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *PSButton1(void); 
    virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual void showScreen(bool passwordVerified);
    virtual SMStateBase  *stateAfterPasswordVerify();
    virtual SMStateBase * OnBackToLGW(void);
    virtual SMStateBase  *SASecMgrParse( const MessageElement*);
    virtual void Escalation2RAP(void);
    virtual SMStateBase *GPNotify(const GPEVT gpEvt);

    DECLARE_DYNCREATE(SMPickingUpItems)// MFC Runtime class/object information


		
};

#endif