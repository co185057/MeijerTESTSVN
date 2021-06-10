///////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTransportItem.h
//
// TITLE: Class header for Transport Item state
//
// $Header:
//
// AUTHOR:  Thanhhieu Pham
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TRANSPORTITEMSTATE
#define _TRANSPORTITEMSTATE

#include "SMTransportItemBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMTransportItem : public SMTransportItemBase
{
public:
	SMTransportItem();
	
  DECLARE_DYNCREATE(SMTransportItem)// MFC Runtime class/object information


		
};

#endif
