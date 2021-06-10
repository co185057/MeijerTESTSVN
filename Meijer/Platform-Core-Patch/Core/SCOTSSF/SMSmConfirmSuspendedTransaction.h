//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmConfirmSuspendedTransaction.h
//
// TITLE: Class header for store mode ConfirmSuspendedTransaction state
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMCONFIRMSUSPENDEDTRANSACTIONSTATE
#define _SMCONFIRMSUSPENDEDTRANSACTIONSTATE

#include "SMSmConfirmSuspendedTransactionBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmConfirmSuspendedTransaction : public SMSmConfirmSuspendedTransactionBase
{
public:
	SMSmConfirmSuspendedTransaction();
	
  DECLARE_DYNCREATE(SMSmConfirmSuspendedTransaction)// MFC Runtime class/object information
};

#endif