//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMFinish.CPP
//
// TITLE: Class implementation for Finish State
//
// $Header:
//
// AUTHOR:    Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV base state
#include "SMFinish.h"               // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("Finish")

IMPLEMENT_DYNCREATE(SMFinish, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////
// Constructor
////////////////////
SMFinish::SMFinish()
{
  IMPLEMENT_TIMESTAMP
}

