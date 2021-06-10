//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMOutOfService.h
//
// TITLE: Class header for wait for helpf for bad card state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _OUTOFSERVICESTATE
#define _OUTOFSERVICESTATE

#include "SMOutOfServiceBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMOutOfService : public SMOutOfServiceBase
{
public:
	SMOutOfService(const int);
	SMOutOfService();
	SMStateBase *Deliver(const int);
	
  DECLARE_DYNCREATE(SMOutOfService)// MFC Runtime class/object information
		
};

#endif
