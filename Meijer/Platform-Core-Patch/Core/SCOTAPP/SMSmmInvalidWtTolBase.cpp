//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmInvalidWtTolBase.CPP
//
// TITLE: Class implementation for store mode Security Maintenance Mode invalid tolerance
//
// AUTHOR:    Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // common includes
#include "SMState.h"                // Base state
#include "SMSmmInvalidWtTol.h"	// This state
#include "SMSmmKeyInWtTol.h"    // Prev state 

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmmInvalidWtTolBase")

IMPLEMENT_DYNCREATE(SMSmmInvalidWtTolBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmmInvalidWtTol)

DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmmInvalidWtTolBase::SMSmmInvalidWtTolBase()
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////////
// Initialize
//////////////////////////////////////////////
SMStateBase  *SMSmmInvalidWtTolBase::Initialize(void)
{
  bOkToDisplayBagScaleInfo = false;
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmmInvalidWtTol") );	// RFC 330050
  DMScannerDisable();

  ps.SetFrame(_T("SmmInvalidWtTol"));
  ps.SetLTTinto2(csSMMScannedUPC);	//Lead thru text is 2 lines long...
  CString csLTTfmt;
  csLTTfmt.Format(ps.GetPSText(TXT_SMM_INVALID_WTTOL_INSTR), csSMMMinTolerance, csSMMMaxTolerance);
  ps.SetLTTinto2(csLTTfmt);	

  ps.ShowFrame();
  return STATE_NULL;                // stay in state
}

/////////////////////////////////////////////
// uninitialize
//////////////////////////////////////////////
void SMSmmInvalidWtTolBase::UnInitialize(void)
{
  bOkToDisplayBagScaleInfo = true;
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmmInvalidWtTol") );	// RFC 330050
}

/////////////////////////////////////////////
// PSButton1 - edit add entries
/////////////////////////////////////////////
SMStateBase  *SMSmmInvalidWtTolBase::PSButton1(void)
{
  RETURNSTATE(SmmKeyInWtTol)
}
