//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecBagAreaBackup.h
//
// TITLE: Class header for SecBagAreaBackup
//
//
// AUTHOR:    Thanhhieu Pham
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECBAGAREABACKUPSTATE
#define _SECBAGAREABACKUPSTATE

#include "SMSecBagAreaBackupBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSecBagAreaBackup : public SMSecBagAreaBackupBase
{
public:
    SMSecBagAreaBackup();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);
protected:
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);

    virtual SMStateBase* DMTakeawayBagAreaBackup(bool isBackedUp);

    DECLARE_DYNCREATE(SMSecBagAreaBackup)// MFC Runtime class/object information
};

#endif
