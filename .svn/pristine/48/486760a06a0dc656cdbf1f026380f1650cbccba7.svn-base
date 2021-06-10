//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMBagAndEAS.h
//
// TITLE: Class header for Bag and EAS state
//
// $Header:
//
// AUTHOR:  Tara Duncan
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _BAGANDEASSTATE
#define _BAGANDEASSTATE

#include "SMBagAndEASBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMBagAndEAS : public SMBagAndEASBase
{
public:
	SMBagAndEAS();

	SMStateBase* Initialize();

	static long GetDWORDValue(const CString& key,const CString& value);

	
  DECLARE_DYNCREATE(SMBagAndEAS)// MFC Runtime class/object information


		
};

#endif