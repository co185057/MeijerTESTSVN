//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMUpdateItem.cpp
//
// TITLE: Class impliementation for update item state
//
//
// AUTHOR:    Barbara Dawkins
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSmUpdateItem.h"             // MGV this

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmUpdateItem")

IMPLEMENT_DYNCREATE(SMSmUpdateItem, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSmUpdateItem::SMSmUpdateItem()
{
  IMPLEMENT_TIMESTAMP
}

bool SMSmUpdateItem::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMSmUpdateItemBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase * SMSmUpdateItem::Initialize(void)
{
    return SMSmUpdateItemBase::Initialize();
}

void SMSmUpdateItem::UnInitialize(void)
{
    SMSmUpdateItemBase::UnInitialize();
}

bool SMSmUpdateItem::PSAssistMenuLegalForState()
{
    return SMSmUpdateItemBase::PSAssistMenuLegalForState();
}

SMStateBase * SMSmUpdateItem::OnChangeItem(void)
{
    return SMSmUpdateItemBase::OnChangeItem();
}

SMStateBase * SMSmUpdateItem::PSCancelKey(void)
{
    return SMSmUpdateItemBase::PSCancelKey();
}

SMStateBase * SMSmUpdateItem::PSButton1(void)
{
    return SMSmUpdateItemBase::PSButton1();
}

SMStateBase * SMSmUpdateItem::PSButton8(void)
{
    return SMSmUpdateItemBase::PSButton8();
}

SMStateBase * SMSmUpdateItem::PSNumericKey(const long lKey)
{
    return SMSmUpdateItemBase::PSNumericKey(lKey);
}

SMStateBase * SMSmUpdateItem::PSClearKey(void)
{
    return SMSmUpdateItemBase::PSClearKey();
}

SMStateBase * SMSmUpdateItem::PSEnterKey(void)
{
    return SMSmUpdateItemBase::PSEnterKey();
}

SMStateBase * SMSmUpdateItem::PSReceiptUp(void)
{
    return SMSmUpdateItemBase::PSReceiptUp();
}

SMStateBase * SMSmUpdateItem::PSReceiptDown(void)
{
    return SMSmUpdateItemBase::PSReceiptDown();
}
