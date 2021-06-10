//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTimeRestrictedItem.cpp
//
// TITLE: Class implimentation for TimeRestrictedItem State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMTimeRestrictedItem.h"       // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("TimeRestrictedItem")

IMPLEMENT_DYNCREATE(SMTimeRestrictedItem, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////////////
SMTimeRestrictedItem::SMTimeRestrictedItem()
{
  IMPLEMENT_TIMESTAMP
}

