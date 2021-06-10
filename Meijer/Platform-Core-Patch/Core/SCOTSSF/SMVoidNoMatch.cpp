//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMVoidNoMatch.cpp
//
// TITLE: Class impliementation for void item no match State
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMVoidNoMatch.h"          // MGV this

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("VoidNoMatch")

IMPLEMENT_DYNCREATE(SMVoidNoMatch, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////
SMVoidNoMatch::SMVoidNoMatch()
{
  IMPLEMENT_TIMESTAMP
}

