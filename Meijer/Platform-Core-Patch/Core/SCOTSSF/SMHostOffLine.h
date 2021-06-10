//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMHostOffLine.h
//
// TITLE: Class header for host offline state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _HOSTOFFLINE
#define _HOSTOFFLINE

#include "SMHostOffLineBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMHostOffLine : public SMHostOffLineBase
{
public:
	SMHostOffLine();
  
  DECLARE_DYNCREATE(SMHostOffLine) // MFC Runtime class/object information
};

#endif
