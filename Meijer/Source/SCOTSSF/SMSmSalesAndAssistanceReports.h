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
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton7(void);
    virtual SMStateBase  *PSButton8(void);
    virtual long GenerateReport(REPORTKIND kind = RP_SALES,
        REPORTDAY   day  = RP_TODAY,
        REPORTTYPES type = RP_SUMMARY);
    virtual void SetLeadThru(REPORTDAY   day  = RP_TODAY,
        REPORTTYPES type = RP_SUMMARY); 
    virtual long PrintReport();


    DECLARE_DYNCREATE(SMSmSalesAndAssistanceReports)// MFC Runtime class/object information
};

#endif
