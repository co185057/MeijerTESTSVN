//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMInProgress.cpp
//
// Description: The InProgress state is used when the transition from a
//        state to the next state might take some time.  InProgress is a
//        "chameleon" state with respect to Customer Mode, Store Mode,
//        and Help Mode - it becomes the mode of the creating state and
//        therefore does not inherit from the above classes.
//
//
// Author:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////
 
#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMInProgress.h"           // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID _T("InProgress")

IMPLEMENT_DYNCREATE(SMInProgress, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////////////
SMInProgress::SMInProgress(const bool storeMode,
                           const bool helpMode,
                           const int timeout)
		:SMInProgressBase(storeMode, helpMode, timeout)
{
  IMPLEMENT_TIMESTAMP
		
}

////////////////////////////////////////
SMInProgress::SMInProgress()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMInProgress::Deliver(const bool storeMode,
                           const bool helpMode,
                           const int timeout)
{
  SMInProgressBase::Deliver(storeMode, helpMode, timeout);

  return this;
}


