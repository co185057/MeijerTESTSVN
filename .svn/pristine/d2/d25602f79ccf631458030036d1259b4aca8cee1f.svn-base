//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmSalesAndAssistanceReports.h
//
// TITLE: Class header for store mode SalesAndAssistanceReports state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSalesAndAssistanceReportsSTATE
#define _SMSalesAndAssistanceReportsSTATE

#include "SMSmSalesAndAssistanceReportsBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmSalesAndAssistanceReports : public SMSmSalesAndAssistanceReportsBase
{
public:
  SMSmSalesAndAssistanceReports(REPORTKIND rKind = RP_SALES, REPORTDAY rDay = RP_TODAY);
  SMStateBase *Deliver         (REPORTKIND rKind = RP_SALES, REPORTDAY rDay = RP_TODAY);

  DECLARE_DYNCREATE(SMSmSalesAndAssistanceReports)// MFC Runtime class/object information
};

#endif
