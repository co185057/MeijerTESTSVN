//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMFatalError.cpp
//
// TITLE: Class impliementation for fatal error state
//
//
// AUTHOR:    David Moore
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMFatalError.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("FatalError")

IMPLEMENT_DYNCREATE(SMFatalError, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////
SMFatalError::SMFatalError()
{
  IMPLEMENT_TIMESTAMP
}

// the only reason for this method is to allow me to use create and assign
SMStateBase  *SMFatalError::Deliver()
{
  return this;
}