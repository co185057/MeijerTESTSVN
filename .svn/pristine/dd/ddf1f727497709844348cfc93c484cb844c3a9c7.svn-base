//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMFatalError.h
//
// TITLE: Class impliementation for fatal error state
//
//
// AUTHOR:    David Moore
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _FatalErrorSTATE
#define _FatalErrorSTATE

#include "SMOperatorPasswordState.h"
#include "SMFatalErrorBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMFatalError : public SMFatalErrorBase
{
public:
	SMFatalError();
	SMStateBase *Deliver();
	
	
  DECLARE_DYNCREATE(SMFatalError)// MFC Runtime class/object information
};

#endif