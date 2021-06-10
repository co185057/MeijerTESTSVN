//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMDegradedContinueTrans.cpp
//
// TITLE: Class implimentation for Degraded Continue Transaction State
//
// AUTHOR:    Allen Jay Cagaanan
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes
 
//#include "SMState.h"                // MGV base state
#include "SMDegradedContinueTrans.h"          // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("DegradedContinueTrans")

IMPLEMENT_DYNCREATE(SMDegradedContinueTrans, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMDegradedContinueTrans::SMDegradedContinueTrans(void)
{
  IMPLEMENT_TIMESTAMP
}

