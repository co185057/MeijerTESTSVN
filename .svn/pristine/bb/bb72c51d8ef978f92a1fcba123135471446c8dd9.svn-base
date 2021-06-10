//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTakeChange.h
//
// TITLE: Class header for Take Change State
//
// This state represents a successful transaction paid for in cash with
// change due back.
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAKECHANGESTATE
#define _TAKECHANGESTATE

#include "SMTakeChangeBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMTakeChange : public SMTakeChangeBase
{
	
public:
	SMTakeChange(const bool fEOT);
	SMTakeChange();
	SMStateBase *Deliver(const bool fEOT);
	
  DECLARE_DYNCREATE(SMTakeChange)// MFC Runtime class/object information
};

#endif
