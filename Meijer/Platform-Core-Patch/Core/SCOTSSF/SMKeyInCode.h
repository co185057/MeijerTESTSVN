//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMKeyInCode.h
//
// TITLE: Class header for Key In Code state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _KEYINCODESTATE
#define _KEYINCODESTATE

#include "SMKeyInCodeBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMKeyInCode : public SMKeyInCodeBase
{
public:
	SMKeyInCode();
	
  DECLARE_DYNCREATE(SMKeyInCode)// MFC Runtime class/object information
};

#endif