//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmAbort.h
//
// TITLE: Class header for transaction abort
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CONFIRMABORTSTATE
#define _CONFIRMABORTSTATE

#include "SMConfirmAbortBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMConfirmAbort : public SMConfirmAbortBase
{
public:
	SMConfirmAbort();
	
  DECLARE_DYNCREATE(SMConfirmAbort)// MFC Runtime class/object information
};

#endif