//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmRunReports.h
//
// TITLE: Class header for store mode RunReports state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMRunReportsSTATE
#define _SMRunReportsSTATE

#include "SMSmRunReportsBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmRunReports : public SMSmRunReportsBase
{
public:
        SMSmRunReports();
  DECLARE_DYNCREATE(SMSmRunReports)// MFC Runtime class/object information
};

#endif
