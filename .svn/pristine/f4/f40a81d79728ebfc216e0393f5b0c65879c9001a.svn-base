//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMRAPDataNeeded.CPP
//
// TITLE: Class implementation for RAP Data Needed state
//
//
// AUTHOR:    Erick Kobres
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMRAPDataNeeded.h"      // MGV this state
//#include "SMInProgress.h"			// Tar 194454 RF020702

#define COMP_ID ID_SM               // base state component
#define T_ID _T("RAPDataNeeded")

IMPLEMENT_DYNCREATE(SMRAPDataNeeded, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////////////////
SMRAPDataNeeded::SMRAPDataNeeded(LPCTSTR sDesc, int View)
		: SMRAPDataNeededBase(sDesc, View)
{
  IMPLEMENT_TIMESTAMP
		
}

////////////////////////////////////////
SMRAPDataNeeded::SMRAPDataNeeded()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase * SMRAPDataNeeded::Initialize()
{
  return SMRAPDataNeededBase::Initialize();
}

////////////////////////////////////////
SMStateBase  *SMRAPDataNeeded::Deliver(LPCTSTR sDesc, int View)
{
  SMRAPDataNeededBase::Deliver(sDesc, View);

  return this;
}

////////////////////////////////////////
SMStateBase  *SMRAPDataNeeded::Deliver(LPCTSTR sDesc, int View, LPCTSTR pPtr, const bool storeMode)
{
  SMRAPDataNeededBase::Deliver(sDesc, View, pPtr, storeMode);

  return this;
}



