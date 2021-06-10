//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMThankYou.h
//
// TITLE: Class header for Thank You State
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _THANKYOUSTATE
#define _THANKYOUSTATE

#include "SMThankYouBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMThankYou : public SMThankYouBase
{
public:
	SMThankYou();
  	
  DECLARE_DYNCREATE(SMThankYou)// MFC Runtime class/object information
		
};

#endif