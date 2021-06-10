//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAssistMenu.h
//
// TITLE: Class header for store mode AssistMenu state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMAssistMenuSTATE
#define _SMAssistMenuSTATE

#include "SMSmAssistMenuBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmAssistMenu : public SMSmAssistMenuBase
{
public:
	SMSmAssistMenu();
	
  DECLARE_DYNCREATE(SMSmAssistMenu)// MFC Runtime class/object information
};

#endif