//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMContinueTrans.cpp
//
// TITLE: Class implimentation for contionue transaction state
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMContinueTrans.h"        // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID _T("ContinueTrans")

IMPLEMENT_DYNCREATE(SMContinueTrans, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////
SMContinueTrans::SMContinueTrans(void)
{
  IMPLEMENT_TIMESTAMP
}

