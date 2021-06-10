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
#include <afxtempl.h>               // MS MFC template header

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

