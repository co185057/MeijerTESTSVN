//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMExtensionState.CPP
//
// TITLE: Class implementation for customer mode Data Entry state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 

#include "SMState.h"                
#include "SMExtensionState.h"	    

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SMExtensionState")

IMPLEMENT_DYNCREATE(SMExtensionState, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMExtensionState::SMExtensionState()
{
  IMPLEMENT_TIMESTAMP
}



SMExtensionState::SMExtensionState(const int nStateMode, const bool bHoldSecWeight,const int nTimeOutSec)
{
  SMExtensionStateBase::Deliver(nStateMode, bHoldSecWeight, nTimeOutSec);
}
