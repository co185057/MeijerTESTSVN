//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMKeyInCode.CPP
//
// TITLE: Class implementation for Key In Code state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMKeyInCode.h"            // This state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("KeyInCode")

IMPLEMENT_DYNCREATE(SMKeyInCode, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMKeyInCode::SMKeyInCode()
{
  IMPLEMENT_TIMESTAMP
}

