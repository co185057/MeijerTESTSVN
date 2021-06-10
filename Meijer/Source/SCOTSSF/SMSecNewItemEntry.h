//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecNewItemEntry.h
//
// TITLE: Class header for SecNewItemEntry
//
//
// AUTHOR:    Thanhhieu Pham
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECNEWITEMENTRYSTATE
#define _SECNEWITEMENTRYSTATE

#include "SMSecNewItemEntryBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSecNewItemEntry : public SMSecNewItemEntryBase
{
public:
    SMSecNewItemEntry();
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);
protected:
    virtual void showScreen(bool passwordVerified);
    virtual SMStateBase  *stateAfterPasswordVerify();
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
    virtual SMStateBase  *OnBackToLGW(void);
    DECLARE_DYNCREATE(SMSecNewItemEntry)// MFC Runtime class/object information
};

#endif