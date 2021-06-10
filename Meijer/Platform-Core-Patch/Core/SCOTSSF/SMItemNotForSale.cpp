//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMItemNotForSale.cpp
//
// TITLE: Class implimentation for unknown price State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMItemNotForSale.h"       // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("ItemNotForSale")

IMPLEMENT_DYNCREATE(SMItemNotForSale, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////////////
SMItemNotForSale::SMItemNotForSale()
{
  IMPLEMENT_TIMESTAMP
}

