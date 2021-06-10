//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMCustomerModeState.h
//
// Description: Derived from SMState .  All customer mode states are derived from this state.
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMCustomerModeState
#define _SMCustomerModeState

#include "SMState.h"
#include "SigReceiptDataFile.h"

////////////////////////////////////////
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMCustomerModeState : public 
#ifdef _BUILDLIB_
	SMStateBase
#else
	SMState
#endif
{
	
public:
	SMCustomerModeState();
  virtual bool  CustomerMode() { return true; }
  virtual bool PSRemoteLegalForState();
protected:
	SMStateBase *TurnOnSigCap(HWND hDispWnd) ;
	void TurnOffSigCap(void) ;
	static bool issuedSigCapError;					//TAR122724	LPM092999

	DECLARE_DYNCREATE(SMCustomerModeState)// MFC Runtime class/object information
};

#endif
