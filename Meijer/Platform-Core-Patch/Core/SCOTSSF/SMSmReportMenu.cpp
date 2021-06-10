//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmReportMenu.CPP
//
// TITLE: Class implementation for store mode ReportMenu state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMSmReportMenu.h"

#define COMP_ID ID_SM              // base state component
#define T_ID _T("SmReportMenu")

IMPLEMENT_DYNCREATE(SMSmReportMenu, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//===================================================================
// constructor
//===================================================================
SMSmReportMenu::SMSmReportMenu(REPORTKIND rKind)
:SMSmReportMenuBase(rKind)
{
  IMPLEMENT_TIMESTAMP
}

//===================================================================
// Deliver
//===================================================================
SMStateBase  *SMSmReportMenu::Deliver(REPORTKIND rKind)

{
  SMSmReportMenuBase::Deliver(rKind);
  return this;
}