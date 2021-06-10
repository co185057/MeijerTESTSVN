//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMProcessing.cpp
//
// TITLE: Class impliementation for check signature state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"                // base state

#include "SMProcessing.h"     // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("Processing")

IMPLEMENT_DYNCREATE(SMProcessing, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////
SMProcessing::SMProcessing()
{
  IMPLEMENT_TIMESTAMP
}

