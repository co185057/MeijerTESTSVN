//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCrateableItem.CPP
//
// TITLE: Class implementation for Crate-able Item state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

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

