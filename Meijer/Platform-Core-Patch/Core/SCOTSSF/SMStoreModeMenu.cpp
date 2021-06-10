//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStoreModeMenu.CPP
//
// TITLE: Class implementation for store mode Menu states
//
//
// AUTHOR:    Barbara Dawkins
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"
#include "SMStoreModeMenu.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("StoreModeMenu")

IMPLEMENT_DYNCREATE(SMStoreModeMenu, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////
SMStoreModeMenu::SMStoreModeMenu()
{
  IMPLEMENT_TIMESTAMP
}

