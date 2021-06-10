//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMSelectCardType.H
//
// TITLE:    Class header for Select Card Type State
//
// $Header:
//
// AUTHOR:   Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SELECTCARDTYPESTATE
#define _SELECTCARDTYPESTATE

#include "SMSelectCardTypeBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMSelectCardType : public SMSelectCardTypeBase
{
public:
	SMSelectCardType();
	
  DECLARE_DYNCREATE(SMSelectCardType)// MFC Runtime class/object information
		
};

#endif
