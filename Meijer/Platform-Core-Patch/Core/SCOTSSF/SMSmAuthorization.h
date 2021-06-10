//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAuthorization.h
//
// TITLE: Class header for store mode Authorization state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMAUTHORIZATIONSTATE
#define _SMAUTHORIZATIONSTATE

#include "SMSmAuthorizationBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmAuthorization : public SMSmAuthorizationBase
{
public:
	SMSmAuthorization();
	
  DECLARE_DYNCREATE(SMSmAuthorization)// MFC Runtime class/object information
};

#endif