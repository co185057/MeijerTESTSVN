//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMHelpOnWay.CPP
//
// TITLE: Class implementation for help is on the way state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMHelpOnWay.h"            // MGV this state
#define COMP_ID ID_SM               // base state component
#define T_ID _T("HelpOnWay")

IMPLEMENT_DYNCREATE(SMHelpOnWay, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////
SMHelpOnWay::SMHelpOnWay()
{
  IMPLEMENT_TIMESTAMP
}

