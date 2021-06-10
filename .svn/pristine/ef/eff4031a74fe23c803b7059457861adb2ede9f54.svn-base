//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmSalesReports.CPP
//
// TITLE: Class implementation for store mode SalesReports state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

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

SMStateBase * SMSmSalesAndAssistanceReports::Initialize(void)
{
    return SMSmSalesAndAssistanceReportsBase::Initialize();
}

void SMSmSalesAndAssistanceReports::UnInitialize(void)
{
    SMSmSalesAndAssistanceReportsBase::UnInitialize();
}

SMStateBase * SMSmSalesAndAssistanceReports::PSButton1(void)
{
    return SMSmSalesAndAssistanceReportsBase::PSButton1();
}

SMStateBase * SMSmSalesAndAssistanceReports::PSButton2(void)
{
    return SMSmSalesAndAssistanceReportsBase::PSButton2();
}

SMStateBase * SMSmSalesAndAssistanceReports::PSButton7(void)
{
    return SMSmSalesAndAssistanceReportsBase::PSButton7();
}

SMStateBase * SMSmSalesAndAssistanceReports::PSButton8(void)
{
    return SMSmSalesAndAssistanceReportsBase::PSButton8();
}

long SMSmSalesAndAssistanceReports::GenerateReport(REPORTKIND kind = RP_SALES,
                                                   REPORTDAY   day  = RP_TODAY,
                                                   REPORTTYPES type = RP_SUMMARY)
{
    return SMSmSalesAndAssistanceReportsBase::GenerateReport(kind, day, type);
}

void SMSmSalesAndAssistanceReports::SetLeadThru(REPORTDAY   day  = RP_TODAY,
                                                REPORTTYPES type = RP_SUMMARY)
{
    SMSmSalesAndAssistanceReportsBase::SetLeadThru(day, type);
}

long SMSmSalesAndAssistanceReports::PrintReport()
{
    return SMSmSalesAndAssistanceReportsBase::PrintReport();
}

