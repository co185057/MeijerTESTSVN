//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecMisMatchWeight.h
//
// TITLE: Class header for SMSecMisMatchWeight
//
//
// AUTHOR:    Tara Duncan
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECMISMATCHWEIGHTSTATE
#define _SECMISMATCHWEIGHTSTATE

#include "SMSecMisMatchWeightBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSecMisMatchWeight : public SMSecMisMatchWeightBase
{
public:
	SMSecMisMatchWeight();
	
  DECLARE_DYNCREATE(SMSecMisMatchWeight)// MFC Runtime class/object information
};

#endif
