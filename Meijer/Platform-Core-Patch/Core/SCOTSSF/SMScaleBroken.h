//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMScaleBroken.h
//
// TITLE: Class header for scale broken State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SCALEBROKENSTATE
#define _SCALEBROKENSTATE

#include "SMScaleBrokenBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMScaleBroken : public SMScaleBrokenBase
{
public:
	SMScaleBroken();
	
  DECLARE_DYNCREATE(SMScaleBroken)// MFC Runtime class/object information
};

#endif
