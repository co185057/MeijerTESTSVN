//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmMediaStatus.h
//
// TITLE: Class header for store mode MediaStatus state
//
// AUTHOR:  Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMMEDIASTATUSSTATE
#define _SMMEDIASTATUSSTATE

#include "SMSmMediaStatusBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmMediaStatus : public SMSmMediaStatusBase
{
public:
	SMSmMediaStatus();
  DECLARE_DYNCREATE(SMSmMediaStatus)// MFC Runtime class/object information
};

#endif