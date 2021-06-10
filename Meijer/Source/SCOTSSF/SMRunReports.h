//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMRunReports.h
//
// TITLE:	Class header for run reports state
//
//
// AUTHOR:  Tara Duncan
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _RunReportsSTATE
#define _RunReportsSTATE


#include "SMSmRunReportsBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMRunReports : public SMSmRunReports
{
public:
	SMRunReports();
   DECLARE_DYNCREATE(SMRunReports)// MFC Runtime class/object information
};

#endif
