//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTenderBalance.h
//
// TITLE: Class header for TenderBalance State
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TENDERBALANCESTATE
#define _TENDERBALANCESTATE

#include "SMTenderBalanceBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMTenderBalance : public SMTenderBalanceBase
{
public:
	SMTenderBalance();
	
  DECLARE_DYNCREATE(SMTenderBalance)// MFC Runtime class/object information
		
};

#endif