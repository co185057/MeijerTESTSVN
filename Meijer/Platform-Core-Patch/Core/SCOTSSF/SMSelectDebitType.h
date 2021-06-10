//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMSelectDebitType.H
//
// TITLE:    Class header for Select Debit Type State
//
//
// AUTHOR:   Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SELECTDEBITTYPESTATE
#define _SELECTDEBITTYPESTATE

#include "SMSelectDebitTypeBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSelectDebitType : public SMSelectDebitTypeBase
{
public:
	SMSelectDebitType(const bool);
	SMSelectDebitType();
	SMStateBase *Deliver(const bool);
	
  DECLARE_DYNCREATE(SMSelectDebitType)// MFC Runtime class/object information
		
};

#endif
