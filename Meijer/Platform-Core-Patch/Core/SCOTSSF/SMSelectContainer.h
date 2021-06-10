//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectContainer.h
//
// TITLE: Class header for void item state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SELECTCONTAINERSTATE
#define _SELECTCONTAINERSTATE

#include "SMSelectContainerBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSelectContainer : public SMSelectContainerBase
{
public:
	SMSelectContainer();
	
  DECLARE_DYNCREATE(SMSelectContainer)// MFC Runtime class/object information
		
};

#endif