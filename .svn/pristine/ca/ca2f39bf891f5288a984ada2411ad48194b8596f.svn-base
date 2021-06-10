//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectContainer.cpp
//
// TITLE: Class impliementation for void item state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSelectContainer.h"      // MGV this

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SelectContainer")

IMPLEMENT_DYNCREATE(SMSelectContainer, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSelectContainer::SMSelectContainer()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSelectContainer::Initialize(void)
{
    return SMSelectContainerBase::Initialize();
}

void SMSelectContainer::UnInitialize(void)
{
    SMSelectContainerBase::UnInitialize();
}

bool SMSelectContainer::PSAssistMenuLegalForState()
{
    return SMSelectContainerBase::PSAssistMenuLegalForState();
}

SMStateBase * SMSelectContainer::PSButton1(void)
{
    return SMSelectContainerBase::PSButton1();
}

SMStateBase * SMSelectContainer::PSButton2(void)
{
    return SMSelectContainerBase::PSButton2();
}

SMStateBase * SMSelectContainer::PSButton3(void)
{
    return SMSelectContainerBase::PSButton3();
}

SMStateBase * SMSelectContainer::PSButton4(void)
{
    return SMSelectContainerBase::PSButton4();
}

SMStateBase * SMSelectContainer::PSButtonGoBack(void)
{
    return SMSelectContainerBase::PSButtonGoBack();
}

SMStateBase * SMSelectContainer::PSCancelKey(void)
{
    return SMSelectContainerBase::PSCancelKey();
}
