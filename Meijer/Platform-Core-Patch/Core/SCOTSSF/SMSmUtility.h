//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmUtility.h
//
// TITLE: Class header for store mode Utility state
//
// Description: This state inherits from StoreModeState.  
//
//
// AUTHOR:  Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMUtilitySTATE
#define _SMUtilitySTATE

#include "SMSmUtilityBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmUtility : public SMSmUtilityBase
{
public:
	SMSmUtility();
	
  DECLARE_DYNCREATE(SMSmUtility)// MFC Runtime class/object information
};

#endif