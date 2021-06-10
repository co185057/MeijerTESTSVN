//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmSecurityLogs.CPP
//
// TITLE: Class implementation for store mode Coupon Items state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSmSecurityLogs.h"       // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmSecurityLogs")

IMPLEMENT_DYNCREATE(SMSmSecurityLogs, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////
SMSmSecurityLogs::SMSmSecurityLogs()
{
  IMPLEMENT_TIMESTAMP
}

