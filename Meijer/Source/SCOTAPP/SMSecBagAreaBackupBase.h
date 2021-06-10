//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMSecBagAreaBackupBase.h
//
// TITLE:	Class header for Bag Area Backup Screen
//
// AUTHOR:  Thanhhieu Pham, Kenny Pitt
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECBAGAREABACKUPBASESTATE
#define _SECBAGAREABACKUPBASESTATE
#include "SMHelpModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

  SMSecBagAreaBackupBase : public STATE(HelpModeState)
{
public:
    SMSecBagAreaBackupBase();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);            // optional
protected:
    virtual SMStateBase  *TimedOut(void);       // optional
    virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);	// Smart Scale reported an unmatched wt increase
    virtual SMStateBase  *OnWtDecrease(void);

    virtual SMStateBase* DMTakeawayBagAreaBackup(bool isBackedUp);  // takeaway belt event
  
    int nCountForPeriodicClearBagAreaAudio;

    DECLARE_DYNCREATE(SMSecBagAreaBackupBase)   // MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSecBagAreaBackupBase(void);
//		or
//    DLLEXPORT SMStateBase *makeSMSecBagAreaBackupBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSecBagAreaBackup objects.  This function is called by
// RETURNSTATE(SecBagAreaBackup) instead of calling new STATE(SecBagAreaBackup) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSecBagAreaBackupBase.cpp and SMSecBagAreaBackup.cpp
// by using the macro IMPLEMENT_STATE(SecBagAreaBackup)

//DECLARE_STATE(SecSkipBaggingThreshold)

#endif
