//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSetAllItemsAside.h
//
// TITLE: Class header for unknown price State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SETALLITEMSASIDESTATE
#define _SETALLITEMSASIDESTATE

#include "SMSetAllItemsAsideBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSetAllItemsAside : public SMSetAllItemsAsideBase
{
public:
  SMSetAllItemsAside();             // constructor
	
  DECLARE_DYNCREATE(SMSetAllItemsAside)// MFC Runtime class/object information
		
};

#endif