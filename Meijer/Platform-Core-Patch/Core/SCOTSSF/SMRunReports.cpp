//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:		SMRunReports.CPP
//
// TITLE:		Class implementation for Run Reports state
//
//
// AUTHOR:		Erick Kobres
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMRunReports.h"      // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID _T("RunReports")

IMPLEMENT_DYNCREATE(SMRunReports, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////
SMRunReports::SMRunReports()
{
  IMPLEMENT_TIMESTAMP
}

