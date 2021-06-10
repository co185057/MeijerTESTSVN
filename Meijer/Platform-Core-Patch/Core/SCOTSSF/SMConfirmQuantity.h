//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmQuantity.h
//
// TITLE: Class header for Confirm Quantity
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ConfirmQuantitySTATE
#define _ConfirmQuantitySTATE

#include "SMConfirmQuantityBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMConfirmQuantity : public SMConfirmQuantityBase
{
public:
	SMConfirmQuantity();
	
  DECLARE_DYNCREATE(SMConfirmQuantity)// MFC Runtime class/object information
};

#endif