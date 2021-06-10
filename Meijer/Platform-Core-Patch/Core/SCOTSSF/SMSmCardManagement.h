//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCardManagement.h
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMCARDMANAGEMENT
#define _SMCARDMANAGEMENT

#include "SMSmCardManagementBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmCardManagement : public SMSmCardManagementBase
{
public:
	SMSmCardManagement();
	
  DECLARE_DYNCREATE(SMSmCardManagement)// MFC Runtime class/object information
};

#endif