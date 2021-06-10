//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMInProgress.h
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

#ifndef _INPROGRESSSTATE
#define _INPROGRESSSTATE

#include "SMInProgressBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMInProgress : public SMInProgressBase
{
public:
  // Constructor - must tell InProgress the mode of the
  // current state that's creating it.
	SMInProgress(const bool storeMode,
		const bool helpMode,
		const int  timeout);
	SMInProgress();
	SMStateBase *Deliver(const bool storeMode,
		const bool helpMode,
		const int  timeout);
 	
  DECLARE_DYNCREATE(SMInProgress)// MFC Runtime class/object information
};

#endif
