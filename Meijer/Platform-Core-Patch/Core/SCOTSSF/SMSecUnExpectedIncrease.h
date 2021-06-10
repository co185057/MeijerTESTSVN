//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecUnExpectedIncrease.h
//
// TITLE: Class header for SMSecUnExpectedIncrease
//
//
// AUTHOR:    Tara Duncan
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECUNEXPECTEDINCREASESTATE
#define _SECUNEXPECTEDINCREASESTATE

#include "SMSecUnExpectedIncreaseBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSecUnExpectedIncrease : public SMSecUnExpectedIncreaseBase
{
public:
	SMSecUnExpectedIncrease();
	
  DECLARE_DYNCREATE(SMSecUnExpectedIncrease)// MFC Runtime class/object information
};

#endif
