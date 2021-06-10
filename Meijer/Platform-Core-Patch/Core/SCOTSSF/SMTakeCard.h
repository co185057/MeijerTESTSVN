//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTakeCard.h
//
// TITLE: Class header for Take Card State
//
//
// AUTHOR:    Peter Denhoed, Updated DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAKECARDSTATE
#define _TAKECARDSTATE

#include "SMTakeCardBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMTakeCard : public SMTakeCardBase
{
public:
	SMTakeCard(const bool fEOT, const bool fCardError = false);
	SMTakeCard();
	SMStateBase *Deliver(const bool fEOT, const bool fCardError = false);
	
  DECLARE_DYNCREATE(SMTakeCard)// MFC Runtime class/object information
};

#endif
