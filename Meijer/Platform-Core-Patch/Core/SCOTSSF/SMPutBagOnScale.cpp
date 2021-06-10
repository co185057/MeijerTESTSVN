//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMPutBagOnScale.cpp
//
// TITLE: Class implimentation for Put Bag On Scale State - Added for SR085
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes
 
#include "SMState.h"                // MGV base state
#include "SMPutBagOnScale.h"          // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("PutBagOnScale")

IMPLEMENT_DYNCREATE(SMPutBagOnScale, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMPutBagOnScale::SMPutBagOnScale()
{
  IMPLEMENT_TIMESTAMP
}
