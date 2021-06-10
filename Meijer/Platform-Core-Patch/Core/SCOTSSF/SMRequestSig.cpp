//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmVoid.cpp
//
// TITLE: Class implimentation for Confirm Void State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMRequestSig.h"          // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("RequestSig")

IMPLEMENT_DYNCREATE(SMRequestSig, SMRequestSigBase)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMRequestSig::SMRequestSig()
{
  IMPLEMENT_TIMESTAMP
}

