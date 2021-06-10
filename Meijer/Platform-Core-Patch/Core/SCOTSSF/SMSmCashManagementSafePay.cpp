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
#include "SMSmCashManagementSafePay.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmCashManagementSP")

IMPLEMENT_DYNCREATE(SMSmCashManagementSafePay, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////
SMSmCashManagementSafePay::SMSmCashManagementSafePay()
{
  IMPLEMENT_TIMESTAMP
}
