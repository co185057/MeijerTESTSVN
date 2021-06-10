//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMSelectCardType.CPP
//
// TITLE:    Class implementation for Select Card Type State
//
// $Header:
//
// AUTHOR:   Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMEnterAlphaNumericInfo.h"       // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SMEnterAlphaNumericInfo")

IMPLEMENT_DYNCREATE(SMEnterAlphaNumericInfo, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMEnterAlphaNumericInfo::SMEnterAlphaNumericInfo()
{
  IMPLEMENT_TIMESTAMP 
}




