//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmEditWtToleranceBase.CPP
//
// TITLE: Class implementation for store mode Security Maintenance Mode Edit wt tolerance
//
// AUTHOR:    Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header
#include "string.h"
#include "afx.h"

#include "Common.h"                 // common includes
#include "SMState.h"					// Base state
#include "SMSmmEditWtTolerance.h"	// This state
#include "SMSmmEditAdd.h"			// Prev state 
#include "SMSmmKeyInWtTol.h"		// Next state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmmEditWtToleranceBase")

IMPLEMENT_DYNCREATE(SMSmmEditWtToleranceBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmmEditWtTolerance)

DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmmEditWtToleranceBase::SMSmmEditWtToleranceBase()
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////////
// Initialize
//////////////////////////////////////////////
SMStateBase  *SMSmmEditWtToleranceBase::Initialize(void)
{
  bOkToDisplayBagScaleInfo = false;
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmmEditWtTolerance") );	// RFC 330050
  DMScannerDisable();

  ps.SetFrame(_T("SmmEditWtTolerance"));

  ps.Prompt(TXT_SMM_INVALID_WTTOL_PROMPT);
  ps.SetLTTinto2(TXT_SMM_WTTOL_INSTR);	//Lead thru text is 2 lines long...
  // button 2 text changes depending on what the default value is
  CString csDefaultBtnTxt;
  csDefaultBtnTxt.Format(ps.GetPSText(BTT_SMM_DEFAULT)); // put txt on button
  ps.SetPSText(PS_WORK1, csDefaultBtnTxt);
  // Changed fix for 203431, 205037, 203879 - now using the default flag from WtInfo object LPM120403 Std Dev
  if (csSMMToleranceType == KEY_OSM_TOLERANCE_TYPE_DEFAULT )
  {
	ps.Button(_T("SMButton2"), PS_WORK1, false);	//then disable the button
  }
  else
  {
	ps.Button(_T("SMButton2"), PS_WORK1, true);	// enable the default button when the an override is currently being used
  }
  csDefaultBtnTxt.Format(ps.GetPSText(BTT_SMM_DEFAULT)); // put txt on button
  ps.SetPSText(PS_WORK1, csDefaultBtnTxt);

  ps.ShowFrame();
  return STATE_NULL;                // stay in SMSmmEditWtToleranceBase state
}

/////////////////////////////////////////////
// uninitialize
//////////////////////////////////////////////
void SMSmmEditWtToleranceBase::UnInitialize(void)
{
  bOkToDisplayBagScaleInfo = true;
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmmEditWtTolerance") );	// RFC 330050
}

/////////////////////////////////////////////
// PSButton1 - key in tolerance value
/////////////////////////////////////////////
SMStateBase  *SMSmmEditWtToleranceBase::PSButton1(void)
{
  g_lUseDefaultTolerance = SMM_FALSE;	// The tolerance to use IS NOT the default
  RETURNSTATE(SmmKeyInWtTol)
}

/////////////////////////////////////////////
// PSButton2 - set to default value
/////////////////////////////////////////////
SMStateBase  *SMSmmEditWtToleranceBase::PSButton2(void)
{
	ChangeSecurityMaintenanceTolerance(KEY_SMM_OPERATION_CHANGE_WT_TOLERANCE, csSMMScannedUPC, _T("default"), io.IsEmbeddedPrice(csSMMScannedUPC) );
	RETURNSTATE(SmmEditAdd)
}

/////////////////////////////////////////////
// PSButton8 - Go Back
/////////////////////////////////////////////
SMStateBase  *SMSmmEditWtToleranceBase::PSButton8(void)
{
  // Tell the security sub-system that we are back to Security Maintenance main state
  CKeyValueParmList parmList;
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_SMM_OPERATION, KEY_SMM_OPERATION_CANCEL);
  _bstr_t bstrParmList((const _TCHAR *)parmList);
  SecMgr.SecurityMaintenance(lSecMgrCookie, bstrParmList);  

  RETURNSTATE(SmmEditAdd)
}
