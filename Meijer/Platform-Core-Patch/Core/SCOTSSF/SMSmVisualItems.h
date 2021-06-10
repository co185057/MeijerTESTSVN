//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmVisualItems.h
//
// TITLE: Class header for store mode Visual Items state
//
//
// AUTHOR: gary miller
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMVISUALITEMSSTATE
#define _SMVISUALITEMSSTATE

#include "SMSmVisualItemsBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmVisualItems : public SMSmVisualItemsBase
{
public:
	SMSmVisualItems();							  // constructor

	
  DECLARE_DYNCREATE(SMSmVisualItems)// MFC Runtime class/object information
};

#endif