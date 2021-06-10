//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMLookUpItem.cpp
//
// TITLE: Class implementation for select item state
//
// TIME:  01/14/99
//
// AUTHOR:    Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // common includes

#include "SMState.h"                // base state
#include "SMLookUpItem.h"           // this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("LookUpItem")


DLLEXPORT LPTSTR PRS_CMKEYBOARD=_T("wCMKeyboard"); // Std KeyBoard
											                   

IMPLEMENT_DYNCREATE(SMLookUpItem, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////
SMLookUpItem::SMLookUpItem()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMLookUpItem::Initialize(void)
{
    return SMLookUpItemBase::Initialize();
}

void SMLookUpItem::UnInitialize(void)
{
    SMLookUpItemBase::UnInitialize();
}

SMStateBase * SMLookUpItem::PSButton1(void)
{
    return SMLookUpItemBase::PSButton1();
}

SMStateBase * SMLookUpItem::PSSpaceKey(void)
{
    return SMLookUpItemBase::PSSpaceKey();
}

SMStateBase * SMLookUpItem::PSEnterKey(void)
{
    return SMLookUpItemBase::PSEnterKey();
}

SMStateBase * SMLookUpItem::PSClearKey(void)
{
    return SMLookUpItemBase::PSClearKey();
}

SMStateBase * SMLookUpItem::DMScanner(void)
{
    return SMLookUpItemBase::DMScanner();
}

SMStateBase * SMLookUpItem::PSButtonGoBack(void)
{
    return SMLookUpItemBase::PSButtonGoBack();
}

SMStateBase * SMLookUpItem::PSShiftKey(void)
{
    return SMLookUpItemBase::PSShiftKey();
}

SMStateBase * SMLookUpItem::PSAltGrKey(void)
{
    return SMLookUpItemBase::PSAltGrKey();
}

SMStateBase * SMLookUpItem::PSHotKey(CString & csHotKey) 
{
    return SMLookUpItemBase::PSHotKey(csHotKey) ;
}

SMStateBase * SMLookUpItem::PSCharKey(TCHAR c)
{
    return SMLookUpItemBase::PSCharKey(c);
}

void SMLookUpItem::SetPSContext()
{
    SMLookUpItemBase::SetPSContext();
}
