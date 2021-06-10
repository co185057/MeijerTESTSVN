/////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCashManagement.CPP
//
// TITLE: Class implementation for store mode CashManagement state
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"
#include "SMSmCashManagement.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmCashManagement")

IMPLEMENT_DYNCREATE(SMSmCashManagement, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////
SMSmCashManagement::SMSmCashManagement()
{
  IMPLEMENT_TIMESTAMP
}
