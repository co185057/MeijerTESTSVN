//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmSalesReports.CPP
//
// TITLE: Class implementation for store mode SalesReports state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMSmSalesAndAssistanceReports.h"

#define COMP_ID ID_SM              // base state component
#define T_ID _T("SmSalesAndAssistanceReports")

IMPLEMENT_DYNCREATE(SMSmSalesAndAssistanceReports, CObject)// MFC Runtime class/object information
DEFINE_TIMESTAMP

//===================================================================
// constructor
//===================================================================
SMSmSalesAndAssistanceReports::SMSmSalesAndAssistanceReports(REPORTKIND rKind, 
															 REPORTDAY  rDay)
                              : SMSmSalesAndAssistanceReportsBase(rKind, rDay)
{
  IMPLEMENT_TIMESTAMP
}

//===================================================================
// Deliver
//===================================================================
SMStateBase  *SMSmSalesAndAssistanceReports::Deliver(REPORTKIND rKind, REPORTDAY rDay)
{
  SMSmSalesAndAssistanceReportsBase::Deliver(rKind, rDay);
  return this;
}
