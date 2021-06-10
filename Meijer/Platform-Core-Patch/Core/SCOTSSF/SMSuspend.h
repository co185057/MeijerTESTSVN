//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSuspend.h
//
// TITLE: Class header for suspend State
//
//
// AUTHOR:    Ying Xie
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SUSPENDSTATE
#define _SUSPENDSTATE

#include "SMSuspendBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSuspend : public SMSuspendBase
{
public:
	SMSuspend();
	
  DECLARE_DYNCREATE(SMSuspend)// MFC Runtime class/object information
};

#endif
