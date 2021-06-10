//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSetAllItemsAside.cpp
//
// TITLE: Class implimentation for set items aside State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"
#include "SMSetAllItemsAside.h"


#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SetAllItemsAside")

DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMSetAllItemsAside, CObject)// MFC Runtime class/object information


////////////////////////////////////////
SMSetAllItemsAside::SMSetAllItemsAside()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSetAllItemsAside::Initialize(void)
{
    return SMSetAllItemsAsideBase::Initialize();
}

void SMSetAllItemsAside::UnInitialize(void)
{
    SMSetAllItemsAsideBase::UnInitialize();
}

SMStateBase * SMSetAllItemsAside::TimedOut(void)
{
    return SMSetAllItemsAsideBase::TimedOut();
}

SMStateBase * SMSetAllItemsAside::OnWtDecrease(void)
{
    return SMSetAllItemsAsideBase::OnWtDecrease();
}

SMStateBase * SMSetAllItemsAside::OnWtIncreaseNotAllowed(void)
{
    return SMSetAllItemsAsideBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMSetAllItemsAside::PSButton1(void)
{
    return SMSetAllItemsAsideBase::PSButton1();
}

SMStateBase * SMSetAllItemsAside::PSEnterKey(void)
{
    return SMSetAllItemsAsideBase::PSEnterKey();
}
