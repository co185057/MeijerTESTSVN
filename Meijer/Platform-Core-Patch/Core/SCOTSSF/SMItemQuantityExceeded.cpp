//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMItemQuantityExceeded.cpp
//
// TITLE: Class implementation for per item quantity exceeded
//
//
// AUTHOR: Gary Miller
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMItemQuantityExceeded.h"       // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("QuantityExceeded")

IMPLEMENT_DYNCREATE(SMItemQuantityExceeded, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////////////
SMItemQuantityExceeded::SMItemQuantityExceeded()
{
  IMPLEMENT_TIMESTAMP
}

