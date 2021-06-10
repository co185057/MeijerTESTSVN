//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSellBags.cpp
//
// TITLE: Class implimentation for Sell Bags State - Added for SR713
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes
 
#include "SMState.h"                // MGV base state
#include "SMSellBags.h"          // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SellBags")

IMPLEMENT_DYNCREATE(SMSellBags, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSellBags::SMSellBags()
{
  IMPLEMENT_TIMESTAMP
}


SMSellBags::SMSellBags(LPCTSTR sItemCode, const long lQty, LPCTSTR sPrice):
	SMSellBagsBase(sItemCode,lQty,sPrice)
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMSellBags::Deliver(LPCTSTR sItemCode, const long lQty, LPCTSTR sPrice)
{
  SMSellBagsBase::Deliver(sItemCode,lQty,sPrice);

  return this;
}
