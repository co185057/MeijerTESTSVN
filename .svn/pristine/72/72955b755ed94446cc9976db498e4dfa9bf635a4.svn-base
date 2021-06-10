//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCmDataEntry.CPP
//
// TITLE: Class implementation for customer mode Data Entry state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMCmDataEntry.h"	    // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("CmDataEntry")

IMPLEMENT_DYNCREATE(SMCmDataEntry, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMCmDataEntry::SMCmDataEntry()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMCmDataEntry::Deliver(LPCTSTR pPtr)
{
  SMCmDataEntryBase::Deliver(pPtr);

  return this;
}

