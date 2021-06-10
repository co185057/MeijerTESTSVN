/////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCardManagement.CPP
//
// TITLE: Class implementation for store mode CardManagement state
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"
#include "SMSmCardManagement.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmCardManagement")

IMPLEMENT_DYNCREATE(SMSmCardManagement, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////
SMSmCardManagement::SMSmCardManagement()
{
  IMPLEMENT_TIMESTAMP
}
