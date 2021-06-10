//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMWaitForApproval.CPP
//
// TITLE: Class implementation for wait for approval state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMWaitForApproval.h"      // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID _T("WaitForApproval")

IMPLEMENT_DYNCREATE(SMWaitForApproval, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////////////////
SMWaitForApproval::SMWaitForApproval(const bool fTV,
                                     const bool fVT, const bool bVerifySig)
		: SMWaitForApprovalBase(fTV, fVT, bVerifySig)
{
  IMPLEMENT_TIMESTAMP
		
}

////////////////////////////////////////
SMWaitForApproval::SMWaitForApproval()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMWaitForApproval::Deliver(const bool fTV,
                                     const bool fVT, const bool bVerifySig)
{
  SMWaitForApprovalBase::Deliver(fTV, fVT, bVerifySig);

  return this;
}


