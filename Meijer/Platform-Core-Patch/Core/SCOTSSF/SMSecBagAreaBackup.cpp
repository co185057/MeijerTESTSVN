#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

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