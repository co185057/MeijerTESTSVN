//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmEnterBirthdate.CPP
//
// TITLE: Class implementation for store mode Enter Birthdate state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMSmEnterBirthdate.h"     // This state

#define COMP_ID ID_SM               // Base state component
#define T_ID _T("SmEnterBirthdate")

IMPLEMENT_DYNCREATE(SMSmEnterBirthdate, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmEnterBirthdate::SMSmEnterBirthdate()
{
  IMPLEMENT_TIMESTAMP
}
