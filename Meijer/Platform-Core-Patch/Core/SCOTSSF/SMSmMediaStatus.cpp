//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmMediaStatus.CPP
//
// TITLE: Class implementation for store mode Media Status state
//
// AUTHOR: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMSmMediaStatus.h"

#define COMP_ID ID_SM							  // base state component
#define T_ID _T("SmMediaStatus")


IMPLEMENT_DYNCREATE(SMSmMediaStatus, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//================================
// constructor
//================================
SMSmMediaStatus::SMSmMediaStatus()
{
  IMPLEMENT_TIMESTAMP
}
