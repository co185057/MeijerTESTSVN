//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmQuantity.cpp
//
// TITLE: Class implimentation for Confirm Quantity State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMConfirmQuantity.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("ConfirmQuantity")

DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMConfirmQuantity, CObject)// MFC Runtime class/object information

//////////////////////////////////////////
SMConfirmQuantity::SMConfirmQuantity()
{
  IMPLEMENT_TIMESTAMP
}

