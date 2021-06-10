//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSavePreferences.cpp
//
// TITLE: Class implimentation for Save Preferences State
//
//
// AUTHOR:    Barbara Dawkins
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSavePreferences.h"          // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SavePreferences")

IMPLEMENT_DYNCREATE(SMSavePreferences, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSavePreferences::SMSavePreferences()
{
  IMPLEMENT_TIMESTAMP
}

