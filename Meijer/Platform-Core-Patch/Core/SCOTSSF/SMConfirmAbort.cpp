//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmAbort.cpp
//
// TITLE: Class implimentation for trans abort State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "Common.h"                 // MGV common includes

#include "SMConfirmAbort.h"
#define COMP_ID ID_SM               // base state component
#define T_ID _T("ConfirmAbort")

IMPLEMENT_DYNCREATE(SMConfirmAbort, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////
SMConfirmAbort::SMConfirmAbort()
{
  IMPLEMENT_TIMESTAMP
}

