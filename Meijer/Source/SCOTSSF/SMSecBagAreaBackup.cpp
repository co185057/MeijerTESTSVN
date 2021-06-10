#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSecBagAreaBackup.h"      

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SecBagAreaBackup")


IMPLEMENT_DYNCREATE(SMSecBagAreaBackup, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP



//////////////////////////////////////////
SMSecBagAreaBackup::SMSecBagAreaBackup()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSecBagAreaBackup::Initialize(void)
{
    return SMSecBagAreaBackupBase::Initialize();
}

void SMSecBagAreaBackup::UnInitialize(void)
{
    SMSecBagAreaBackupBase::UnInitialize();
}

SMStateBase * SMSecBagAreaBackup::TimedOut(void)
{
    return SMSecBagAreaBackupBase::TimedOut();
}

SMStateBase * SMSecBagAreaBackup::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
    return SMSecBagAreaBackupBase::OnRAPClearSecurityBtn( lpszAuthorizationType);
}

SMStateBase * SMSecBagAreaBackup::OnWtIncreaseNotAllowed(void)
{
    return SMSecBagAreaBackupBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMSecBagAreaBackup::OnWtDecrease(void)
{
    return SMSecBagAreaBackupBase::OnWtDecrease();
}

SMStateBase * SMSecBagAreaBackup::DMTakeawayBagAreaBackup(bool isBackedUp)
{
    return SMSecBagAreaBackupBase::DMTakeawayBagAreaBackup(isBackedUp);
}
