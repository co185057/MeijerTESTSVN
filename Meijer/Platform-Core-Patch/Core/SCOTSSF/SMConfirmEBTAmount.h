//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmEBTAmount.h
//
// TITLE: Class header for Confirm EBT
//
//
// AUTHOR:    Michael Prather
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ConfirmEBTAmountSTATE
#define _ConfirmEBTAmountSTATE

#include "SMConfirmEBTAmountBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMConfirmEBTAmount : public SMConfirmEBTAmountBase
{

public:
	SMConfirmEBTAmount(const TBTENDERTYPE, const long, const long);
	SMConfirmEBTAmount();
	SMStateBase *Deliver(const TBTENDERTYPE, const long, const long);
	
  DECLARE_DYNCREATE(SMConfirmEBTAmount)// MFC Runtime class/object information
};

#endif
