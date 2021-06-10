//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCrateableItem.CPP
//
// TITLE: Class implementation for Crate-able Item state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // common includes

#include "SMState.h"                // base state
#include "SMCrateableItem.h"        // this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("CrateableItem")

IMPLEMENT_DYNCREATE(SMCrateableItem, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMCrateableItem::SMCrateableItem()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMCrateableItem::Initialize(void)
{
    return SMCrateableItemBase::Initialize();
}

void SMCrateableItem::UnInitialize(void)
{
    SMCrateableItemBase::UnInitialize();
}

bool SMCrateableItem::PSAssistMenuLegalForState()
{
    return SMCrateableItemBase::PSAssistMenuLegalForState();
}

SMStateBase * SMCrateableItem::PSButton1(void)
{
    return SMCrateableItemBase::PSButton1();
}

SMStateBase * SMCrateableItem::PSButton2(void)
{
    return SMCrateableItemBase::PSButton2();
}
