//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMAttract.CPP
//
// TITLE: Class implementation for Attract State
//
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/SCOTSSF/SMAttract.cpp 4     11/25/08 2:19p Sm133076 $
//
// AUTHOR:  Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes


#include "SMAttract.h"
#define COMP_ID ID_SM               // base state component
#define T_ID     _T("Attract")
 

DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMAttract, CObject)// MFC Runtime class/object information

//////////////////////
// Constructor
//////////////////////
SMAttract::SMAttract()
{
  IMPLEMENT_TIMESTAMP
		// We have started the application successfully if we get here, so we can
		// reset the counter used to restart SCOTIS during error conditions.
		// Nothing we can do with the return code.
}

