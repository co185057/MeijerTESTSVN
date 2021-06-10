//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmDebitAmount.h
//
// TITLE: Class header for Confirn Signature
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ConfirmDebitAmountSTATE
#define _ConfirmDebitAmountSTATE

#include "SMConfirmDebitAmountBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMConfirmDebitAmount : public SMConfirmDebitAmountBase
{

public:
	SMConfirmDebitAmount(const TBTENDERTYPE, const long);
	SMConfirmDebitAmount();
	SMStateBase *Deliver(const TBTENDERTYPE, const long);
	
  DECLARE_DYNCREATE(SMConfirmDebitAmount)// MFC Runtime class/object information
};

#endif
