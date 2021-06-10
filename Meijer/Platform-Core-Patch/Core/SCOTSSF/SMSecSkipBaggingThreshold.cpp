#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSecSkipBaggingThreshold.h"      

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SecSkipBaggingThreshold")


IMPLEMENT_DYNCREATE(SMSecSkipBaggingThreshold, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP



//////////////////////////////////////////
SMSecSkipBaggingThreshold::SMSecSkipBaggingThreshold()
{
  IMPLEMENT_TIMESTAMP
}

