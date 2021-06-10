#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSecItemRemovedThreshold.h"      

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SecItemRemovedThreshold")


IMPLEMENT_DYNCREATE(SMSecItemRemovedThreshold, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP



//////////////////////////////////////////
SMSecItemRemovedThreshold::SMSecItemRemovedThreshold()
{
  IMPLEMENT_TIMESTAMP
}