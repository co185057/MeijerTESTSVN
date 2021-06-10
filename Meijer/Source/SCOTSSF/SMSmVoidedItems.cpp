//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmVoidedItems.CPP
//
// TITLE: Class implementation for store mode voided Items state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSmVoidedItems.h"        // MGV this state
#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmVoidedItems")

IMPLEMENT_DYNCREATE(SMSmVoidedItems, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////
SMSmVoidedItems::SMSmVoidedItems()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmVoidedItems::Initialize(void)
{
    return SMSmVoidedItemsBase::Initialize();
}

SMStateBase * SMSmVoidedItems::PSButton1(void)
{
    return SMSmVoidedItemsBase::PSButton1();
}

SMStateBase * SMSmVoidedItems::PSButton8(void)
{
    return SMSmVoidedItemsBase::PSButton8();
}

