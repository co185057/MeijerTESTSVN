//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCashManagement.h
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMCASHMANAGEMENTSP
#define _SMCASHMANAGEMENTSP

#include "SMSmCashManagementSafePayBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmCashManagementSafePay : public SMSmCashManagementSafePayBase
{
public:
	SMSmCashManagementSafePay();
	
  DECLARE_DYNCREATE(SMSmCashManagementSafePay)// MFC Runtime class/object information
};

#endif