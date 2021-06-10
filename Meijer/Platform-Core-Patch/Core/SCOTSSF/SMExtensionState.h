//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMExtensionState.h
//
// TITLE: Class header for customer mode Data Entry state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMEXTENSIONSTATE_H
#define _SMEXTENSIONSTATE_H

#include "SMExtensionStateBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMExtensionState : public SMExtensionStateBase
{
public:
	SMExtensionState();
	~SMExtensionState(){};
	// For nStateMode, 1 = customer, 2 = help, 3 = store, other/default = customer
	SMExtensionState(const int nStateMode, const bool bHoldSecWeight,const int nTimeOutSec);		  // constructor

  DECLARE_DYNCREATE(SMExtensionState)	// MFC Runtime class/object information
};

#endif   // _SMEXTENSIONSTATE_H
