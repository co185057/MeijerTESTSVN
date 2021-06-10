//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:   SMSecBagBoxNotValid.h
//
// TITLE:  Class header for Invalid Bag State - Added for SR085
//
//
// AUTHOR: Shakti Kamal
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECBAGBOXNOTVALIDSTATE
#define _SECBAGBOXNOTVALIDSTATE

#include "SMSecBagBoxNotValidBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSecBagBoxNotValid : public SMSecBagBoxNotValidBase
{
public:
	SMSecBagBoxNotValid();
	
  DECLARE_DYNCREATE(SMSecBagBoxNotValid)// MFC Runtime class/object information
};

#endif