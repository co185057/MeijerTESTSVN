//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSignCharge.CPP
//
// TITLE: Class implementation for sign charge slip state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSignCharge.h"           // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SignCharge")

IMPLEMENT_DYNCREATE(SMSignCharge, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////
SMSignCharge::SMSignCharge()
{
  IMPLEMENT_TIMESTAMP
}

