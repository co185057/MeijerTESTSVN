//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmSalesAndAssistanceReportsBase.h
//
// TITLE: Class header for Sales And Assistance Reports state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmSalesAndAssistanceReportsBaseSTATE
#define _SmSalesAndAssistanceReportsBaseSTATE

#include "SMStoreModeState.h"
#include "ReportGeneratorBase.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmSalesAndAssistanceReportsBase : public STATE(StoreModeState)
{
public:
  SMSmSalesAndAssistanceReportsBase(REPORTKIND rKind = RP_SALES,
	                                REPORTDAY  rDay  = RP_TODAY);
  SMStateBase *Deliver(REPORTKIND rKind = RP_SALES,
	                   REPORTDAY  rDay  = RP_TODAY);
  virtual SMStateBase  *Initialize(void);
  virtual void	 UnInitialize(void);

protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton2(void);
  virtual SMStateBase  *PSButton7(void);
  virtual SMStateBase  *PSButton8(void);
  virtual long         GenerateReport(REPORTKIND kind = RP_SALES,
	                                  REPORTDAY   day  = RP_TODAY,
	                                  REPORTTYPES type = RP_SUMMARY);
  virtual void         SetLeadThru(REPORTDAY   day  = RP_TODAY,
	                               REPORTTYPES type = RP_SUMMARY); 
  virtual long         PrintReport();

  void    SetReportDay(REPORTDAY day);
  void    SetReportType(REPORTTYPES type);
  void    SetReportKind(REPORTKIND kind);

  REPORTDAY            ReportDay;
  REPORTTYPES          ReportType;
  REPORTKIND           ReportKind;
  ReportGeneratorBase  *pReportGen;

  DECLARE_DYNCREATE(SMSmSalesAndAssistanceReportsBase)    // MFC Runtime class/object information
};

//DECLARE_STATE(SmSalesAndAssistanceReports)

#endif
