//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAborted.h
//
// TITLE: Class header for store mode aborted state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMABORTEDSTATE
#define _SMABORTEDSTATE

#include "SMSmAbortedBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmAborted : public SMSmAbortedBase
{
public:
	SMSmAborted();									  // constructor
	
  DECLARE_DYNCREATE(SMSmAborted)// MFC Runtime class/object information
		
};

#endif