//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmDataEntry.h
//
// TITLE: Class header for store mode Restricted Items state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMDataEntrySTATE
#define _SMDataEntrySTATE

#include "SMSmDataEntryBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmDataEntry : public SMSmDataEntryBase
{
public:
	         SMSmDataEntry();					  // constructor

  DECLARE_DYNCREATE(SMSmDataEntry)// MFC Runtime class/object information
};

#endif
