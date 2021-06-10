//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmAddWeight.CPP
//
// TITLE: Class implementation for store mode Security Maintenance Mode 
//
// AUTHOR: Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // common includes

#include "SMState.h"                // base state
#include "SMSmmAddWeight.h"			// this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmmAddWeight")

IMPLEMENT_DYNCREATE(SMSmmAddWeight, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmmAddWeight::SMSmmAddWeight()
{
  IMPLEMENT_TIMESTAMP
}
