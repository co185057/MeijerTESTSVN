//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStoreModeMenu.h
//
// TITLE: Class header for store mode Menu state
//
//
// AUTHOR:  Barbara Dawkins
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _StoreModeMenuSTATE
#define _StoreModeMenuSTATE

#include "SMStoreModeMenuBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMStoreModeMenu : public SMStoreModeMenuBase
{
public:
	SMStoreModeMenu();
	
  DECLARE_DYNCREATE(SMStoreModeMenu)// MFC Runtime class/object information
};

#endif