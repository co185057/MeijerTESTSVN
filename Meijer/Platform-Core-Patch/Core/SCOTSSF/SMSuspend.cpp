//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSuspend.cpp
//
// TITLE: Class implimentation for suspend state
//
//
// AUTHOR:    Ying Xie
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSuspend.h"       // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("Suspend")

IMPLEMENT_DYNCREATE(SMSuspend, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSuspend::SMSuspend()
{
  IMPLEMENT_TIMESTAMP
}
