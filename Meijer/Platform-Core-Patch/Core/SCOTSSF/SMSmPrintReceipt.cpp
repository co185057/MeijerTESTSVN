//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmPrintReceipt.CPP
//
// TITLE: Class implementation for store mode Print Receipt state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMSmPrintReceipt.h"

#define COMP_ID ID_SM                                                     // base state component
#define T_ID _T("SmPrintReceipt")


IMPLEMENT_DYNCREATE(SMSmPrintReceipt, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//================================
// constructor
//================================
SMSmPrintReceipt::SMSmPrintReceipt()
{
  IMPLEMENT_TIMESTAMP
}
