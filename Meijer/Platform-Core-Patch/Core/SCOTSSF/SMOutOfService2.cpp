//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMOutOfService2.cpp
//
// TITLE: Class implementation for out of service state, waiting for TS testing
//
//
// AUTHOR:    Peter Denhoed, Updated DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMOutOfService2.h"        // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("OutOfService2")

IMPLEMENT_DYNCREATE(SMOutOfService2, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMOutOfService2::SMOutOfService2()
{
  IMPLEMENT_TIMESTAMP
}

