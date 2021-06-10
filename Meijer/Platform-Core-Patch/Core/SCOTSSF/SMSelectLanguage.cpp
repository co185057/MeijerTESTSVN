//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectLanguage.cpp
//
// TITLE: Class implimentation for Customer Sample state
//
//
// AUTHOR:    John Doe
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSelectLanguage.h"       // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SelectLanguage")

IMPLEMENT_DYNCREATE(SMSelectLanguage, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSelectLanguage:: SMSelectLanguage ()
{
  IMPLEMENT_TIMESTAMP
}
