//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTakeReceipt.cpp
//
// TITLE: Class impliementation for Take Receipt State
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMTakeReceipt.h"          // MGV take receipt state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("TakeReceipt")

IMPLEMENT_DYNCREATE(SMTakeReceipt, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////
SMTakeReceipt::SMTakeReceipt()
: SMTakeReceiptBase()
{
  IMPLEMENT_TIMESTAMP
}

