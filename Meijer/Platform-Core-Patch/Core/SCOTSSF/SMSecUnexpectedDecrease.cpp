#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSecUnexpectedDecrease.h"      

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SecUnexpectedDecrease")


IMPLEMENT_DYNCREATE(SMSecUnexpectedDecrease, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP



//////////////////////////////////////////
SMSecUnexpectedDecrease::SMSecUnexpectedDecrease()
{
  IMPLEMENT_TIMESTAMP
}