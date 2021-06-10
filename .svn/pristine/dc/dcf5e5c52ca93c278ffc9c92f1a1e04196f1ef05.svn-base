//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmVoidAndRemoveItem.CPP
//
// TITLE: Class implementation for store mode void and remove voided item state
//
//
// AUTHOR:    Virgilio Esquilla, Jr.
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSmVoidAndRemoveItem.h"  // MGV remove voided item state

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SMSmVoidAndRemoveItem")

IMPLEMENT_DYNCREATE(SMSmVoidAndRemoveItem, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////
SMSmVoidAndRemoveItem::SMSmVoidAndRemoveItem()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmVoidAndRemoveItem::Initialize(void)
{
    return SMSmVoidAndRemoveItemBase::Initialize();
}

void SMSmVoidAndRemoveItem::UnInitialize(void)
{
    SMSmVoidAndRemoveItemBase::UnInitialize();
}

SMStateBase * SMSmVoidAndRemoveItem::PSButton1(void)
{
    return SMSmVoidAndRemoveItemBase::PSButton1();
}
