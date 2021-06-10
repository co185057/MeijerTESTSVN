//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAssistMenu.CPP
//
// TITLE: Class implementation for store mode AssistMenu state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"
#include "SMSmAssistMenu.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmAssistMenu")

IMPLEMENT_DYNCREATE(SMSmAssistMenu, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////
SMSmAssistMenu::SMSmAssistMenu()
{
  IMPLEMENT_TIMESTAMP
}

