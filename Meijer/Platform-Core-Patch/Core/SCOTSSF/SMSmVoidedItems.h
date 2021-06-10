//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmVoidedItems.h
//
// TITLE: Class header for store mode Voided Items state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMVOIDEDITEMSSTATE
#define _SMVOIDEDITEMSSTATE

#include "SMSmVoidedItemsBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmVoidedItems : public SMSmVoidedItemsBase
{
public:
	SMSmVoidedItems();							  // constructor
	
  DECLARE_DYNCREATE(SMSmVoidedItems)// MFC Runtime class/object information
};

#endif