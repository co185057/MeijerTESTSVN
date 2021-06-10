//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMWaitForApproval.h
//
// TITLE: Class header for wait for approval state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _WAITFORAPPROVALSTATE
#define _WAITFORAPPROVALSTATE


#include "SMWaitForApprovalBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMWaitForApproval : public SMWaitForApprovalBase
{
public:
	SMWaitForApproval(const bool fTV, // transaction verify
	                  const bool fVT,
					  const bool bVerifySig=false);// void transaction
	SMWaitForApproval();
	SMStateBase *Deliver(const bool fTV, // transaction verify
	                  const bool fVT,
					  const bool bVerifySig=false);
	
  DECLARE_DYNCREATE(SMWaitForApproval)// MFC Runtime class/object information
};

#endif
