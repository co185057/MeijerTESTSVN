//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCashManagement.h
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMCASHMANAGEMENT
#define _SMCASHMANAGEMENT

#include "SMSmCashManagementBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmCashManagement : public SMSmCashManagementBase
{
public:
	SMSmCashManagement();
	
  DECLARE_DYNCREATE(SMSmCashManagement)// MFC Runtime class/object information
};

#endif