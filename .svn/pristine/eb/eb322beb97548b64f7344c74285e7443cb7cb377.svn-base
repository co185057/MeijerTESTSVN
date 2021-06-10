//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterPin.h
//
// TITLE: Class header for entring PIN state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ENTERPINSTATE
#define _ENTERPINSTATE

#include "SMEnterPinBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMEnterPin : public SMEnterPinBase
{
public:
	SMEnterPin(const TBTENDERTYPE,const bool);
	SMEnterPin();
	SMStateBase *Deliver(const TBTENDERTYPE,const bool);
	
  DECLARE_DYNCREATE(SMEnterPin)// MFC Runtime class/object information
		
};

#endif
