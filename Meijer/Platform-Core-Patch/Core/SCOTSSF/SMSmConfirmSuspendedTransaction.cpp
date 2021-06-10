//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSm//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmConfirmSuspendedTransaction.CPP
//
// TITLE: Class implementation for store mode ConfirmSuspendedTransaction state
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"
#include "SMSmConfirmSuspendedTransaction.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmConfirmSuspendedTransaction")

IMPLEMENT_DYNCREATE(SMSmConfirmSuspendedTransaction, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////
SMSmConfirmSuspendedTransaction::SMSmConfirmSuspendedTransaction()
{
  IMPLEMENT_TIMESTAMP
}
