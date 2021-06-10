//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSellBags.h
//
// TITLE: Class header for Sell Bags state - added for SR713
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SELLBAGSSTATE
#define _SELLBAGSSTATE

#include "SMSellBagsBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
SMSellBags : public SMSellBagsBase
{
public:
	SMSellBags();
	SMSellBags(LPCTSTR sItemCode, const long lQty, LPCTSTR sPrice);
	SMStateBase *Deliver(LPCTSTR sItemCode, const long lQty, LPCTSTR sPrice);
	
  DECLARE_DYNCREATE(SMSellBags)// MFC Runtime class/object information
		
};

#endif