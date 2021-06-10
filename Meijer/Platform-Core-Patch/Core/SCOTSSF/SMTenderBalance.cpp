//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTenderBalance.cpp
//
// TITLE: Class implimentation for TenderBalance State
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes
 
#include "SMState.h"                // MGV base state
#include "SMTenderBalance.h"          // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("TenderBalance")

IMPLEMENT_DYNCREATE(SMTenderBalance, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMTenderBalance::SMTenderBalance()
{
  IMPLEMENT_TIMESTAMP
}

