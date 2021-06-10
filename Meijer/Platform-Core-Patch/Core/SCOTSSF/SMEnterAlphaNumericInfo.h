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

#ifndef _SMTypeJobInformationSTATE
#define _SMTypeJobInformationSTATE

#include "SMEnterAlphaNumericInfoBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMEnterAlphaNumericInfo : public SMEnterAlphaNumericInfoBase
{
public:
	SMEnterAlphaNumericInfo();

	
  DECLARE_DYNCREATE(SMEnterAlphaNumericInfo)// MFC Runtime class/object information
		
};

#endif
