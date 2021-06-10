//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SmCashierPassword.CPP
//
// TITLE: Class implementation for store mode Authenticate state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMSmCashierPassword.h"     // This state

#define COMP_ID ID_SM               // Base state component
#define T_ID _T("SmEnterBirthdate")

IMPLEMENT_DYNCREATE(SMSmCashierPassword, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmCashierPassword::SMSmCashierPassword()
{
  IMPLEMENT_TIMESTAMP
}
