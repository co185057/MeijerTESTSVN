//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmConfirmSignature.cpp
//
// TITLE: Class implementation for check signature state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSmConfirmSignature.h"     // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmConfirmSignature")

IMPLEMENT_DYNCREATE(SMSmConfirmSignature, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////
SMSmConfirmSignature::SMSmConfirmSignature()
{
  IMPLEMENT_TIMESTAMP
}

