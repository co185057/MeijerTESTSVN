//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmSecurityLogs.h
//
// TITLE: Class header for store mode Security Logs state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSECURITYLOGSSTATE
#define _SMSECURITYLOGSSTATE

#include "SMSmSecurityLogsBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmSecurityLogs : public SMSmSecurityLogsBase
{
public:
	SMSmSecurityLogs();
 	
  DECLARE_DYNCREATE(SMSmSecurityLogs)// MFC Runtime class/object information
};

#endif