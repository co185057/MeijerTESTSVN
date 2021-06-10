//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMWaitForRemoteAssistance.h
//
// TITLE: Class header for wait for approval state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _WaitForRemoteAssistanceSTATE
#define _WaitForRemoteAssistanceSTATE


#include "SMWaitForRemoteAssistanceBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMWaitForRemoteAssistance : public SMWaitForRemoteAssistanceBase
{
public:
	SMWaitForRemoteAssistance();
	SMWaitForRemoteAssistance( LPCTSTR sMachineName, LPCTSTR sRemoteContext );
	SMStateBase *Deliver( LPCTSTR sMachineName, LPCTSTR sRemoteContext );
   DECLARE_DYNCREATE(SMWaitForRemoteAssistance)// MFC Runtime class/object information
};

#endif
