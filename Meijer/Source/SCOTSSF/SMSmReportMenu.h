//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmReportMenu.h
//
// TITLE: Class header for store mode ReportMenu state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMReportMenuSTATE
#define _SMReportMenuSTATE
#include "SMSmReportMenuBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmReportMenu : public SMSmReportMenuBase
{
public:
  SMSmReportMenu(REPORTKIND rKind = RP_SALES);
  SMStateBase *Deliver(REPORTKIND rKind = RP_SALES);
  DECLARE_DYNCREATE(SMSmReportMenu)// MFC Runtime class/object information
};

#endif
