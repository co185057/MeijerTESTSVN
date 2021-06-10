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
//               // MS MFC template header

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

bool SMSellBags::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMSellBagsBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase * SMSellBags::Initialize(void)
{
    return SMSellBagsBase::Initialize();
}

void SMSellBags::UnInitialize(void)
{
    SMSellBagsBase::UnInitialize();
}

SMStateBase * SMSellBags::PSNumericKey(const long lKey)
{
    return SMSellBagsBase::PSNumericKey(lKey);
}

SMStateBase * SMSellBags::PSBackSpaceKey(void)
{
    return SMSellBagsBase::PSBackSpaceKey();
}

SMStateBase * SMSellBags::PSClearKey(void)
{
    return SMSellBagsBase::PSClearKey();
}

SMStateBase * SMSellBags::PSEnterKey(void)
{
    return SMSellBagsBase::PSEnterKey();
}

SMStateBase * SMSellBags::PSButtonGoBack(void)
{
    return SMSellBagsBase::PSButtonGoBack();
}

SMStateBase * SMSellBags::PSButton1(void)
{
    return SMSellBagsBase::PSButton1();
}

SMStateBase * SMSellBags::PSButton2(void)
{
    return SMSellBagsBase::PSButton2();
}

SMStateBase * SMSellBags::ConfirmQtyForBags(void)
{
    return SMSellBagsBase::ConfirmQtyForBags();
}

