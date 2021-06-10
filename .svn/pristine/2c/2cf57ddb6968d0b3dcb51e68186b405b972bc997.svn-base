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
#include <afxtempl.h>               // MS MFC template header

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

SMStateBase  *SMLookUpItem::Initialize()
{
  return SMLookUpItemBase::Initialize();
}
