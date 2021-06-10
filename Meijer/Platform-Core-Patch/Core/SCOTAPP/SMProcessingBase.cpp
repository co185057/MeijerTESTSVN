//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMProcessingBase.cpp
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMProcessingBase.h"   
#include "SMInProgress.h"           // In progress state

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("ProcessingBase")

IMPLEMENT_DYNCREATE(SMProcessingBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////
SMProcessingBase::SMProcessingBase()
{
  IMPLEMENT_TIMESTAMP
}

/////////////////////////////////////////////
SMStateBase  *SMProcessingBase::Initialize(void)
{
   //@@@INFOPOINT

	ps.SetCMFrame(_T("Processing"));
	ps.Message12(LTT_CASHPAYMENT, true);
	ps.ShowCMFrame();

	CREATE_AND_DISPENSE(InProgress)(storeMode(), helpMode(), 0);
}
