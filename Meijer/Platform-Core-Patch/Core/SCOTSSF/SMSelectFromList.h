//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectFromList.h
//
// TITLE: Class implementation for Select From List Screen
//
// TIME:  07/12/98
//
// AUTHOR:   Feng Zhang

//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SELECTFROMLISTSTATE
#define _SELECTFROMLISTSTATE

#include "SMSelectFromListBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMSelectFromList : public SMSelectFromListBase
{
public:
	SMSelectFromList();               // constructor
	
  DECLARE_DYNCREATE(SMSelectFromList)// MFC Runtime class/object information
		
};

#endif
