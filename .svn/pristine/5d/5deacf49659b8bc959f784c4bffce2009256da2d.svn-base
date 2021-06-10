//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmEditWtSubCkBase.CPP
//
// TITLE: Class implementation for store mode Security Maintenance Mode Edit wt substitution check
//
// AUTHOR:    Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header
#include "string.h"
#include "afx.h"

#include "Common.h"                 // common includes
#include "SMState.h"				// Base state
#include "SMSmmEditWtSubCk.h"	// This state
#include "SMSmmEditAdd.h"		// Prev state 

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmmEditWtSubCkBase")

IMPLEMENT_DYNCREATE(SMSmmEditWtSubCkBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmmEditWtSubCk)

DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmmEditWtSubCkBase::SMSmmEditWtSubCkBase()
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////////
// Initialize
//////////////////////////////////////////////
SMStateBase  *SMSmmEditWtSubCkBase::Initialize(void)
{
  bOkToDisplayBagScaleInfo = false;
  DMScannerDisable();
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmmEditWtSubCk") );	// RFC 330050
  // NewUI
  ps.SetFrame(_T("SmmEditWtSubCk"));
  // End NewUI

  ps.SetLTTinto2(TXT_SMM_WTSUBCK_INSTR);	//Lead thru text - 225840 LPM021103

  // button 3 text changes depending on what the default value is
  CString csDefaultBtnTxt, csDefSetting;
  if (lSMMglWtSubCk==SMM_FALSE)//TAR 198092
  {
	csDefSetting = ps.GetPSText(BTT_NO);
  }
  else		
	csDefSetting = ps.GetPSText(BTT_YES);
  csDefaultBtnTxt.Format(ps.GetPSText(BTT_SMM_DEFAULT), csDefSetting); 
  ps.SetPSText(PS_WORK1, csDefaultBtnTxt);
  if ( (csSMMSubstitutionCheck == _T("default-y")) || (csSMMSubstitutionCheck == _T("default-n")) )
  {
	ps.SetButtonText(_T("SMButton1"), BTT_YES);
	ps.SetButtonText(_T("SMButton2"), BTT_NO);
	ps.Button(_T("SMButton3"), PS_WORK1, false);
  }
  else if ( csSMMSubstitutionCheck == _T("y") )
  {
	ps.Button(_T("SMButton1"), BTT_YES, false);
	ps.SetButtonText(_T("SMButton2"), BTT_NO);
  	ps.SetButtonText(_T("SMButton3"), PS_WORK1);
  }
  else if ( csSMMSubstitutionCheck == _T("n") )
  {
	ps.SetButtonText(_T("SMButton1"), BTT_YES);
	ps.Button(_T("SMButton2"), BTT_NO, false);
  	ps.SetButtonText(_T("SMButton3"), PS_WORK1);
  }
  else
  {
	ps.SetButtonText(_T("SMButton1"), BTT_YES);
	ps.SetButtonText(_T("SMButton2"), BTT_NO);
	ps.SetButtonText(_T("SMButton3"), PS_WORK1);
  }
  ps.ShowFrame();
  return STATE_NULL;                // stay in SMSmmEditWtExpBase state
}

/////////////////////////////////////////////
// uninitialize
//////////////////////////////////////////////
void SMSmmEditWtSubCkBase::UnInitialize(void)
{
  bOkToDisplayBagScaleInfo = true;
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmmEditWtSubCk") );	// RFC 330050
}

/////////////////////////////////////////////
// PSButton1 - Wt Expectation = Yes
/////////////////////////////////////////////
SMStateBase  *SMSmmEditWtSubCkBase::PSButton1(void)
{
	ChangeSecurityMaintenanceWtFlags(KEY_SMM_OPERATION_CHANGE_SUBSTITUTION_CHECK, csSMMScannedUPC, _T("y"), io.IsEmbeddedPrice(csSMMScannedUPC) );
	RETURNSTATE(SmmEditAdd)
}

/////////////////////////////////////////////
// PSButton2 - Wt Expectation = No
/////////////////////////////////////////////
SMStateBase  *SMSmmEditWtSubCkBase::PSButton2(void)
{
	ChangeSecurityMaintenanceWtFlags(KEY_SMM_OPERATION_CHANGE_SUBSTITUTION_CHECK, csSMMScannedUPC, _T("n"), io.IsEmbeddedPrice(csSMMScannedUPC) );
	RETURNSTATE(SmmEditAdd)
}

/////////////////////////////////////////////
// PSButton3 - set to default value
/////////////////////////////////////////////
SMStateBase  *SMSmmEditWtSubCkBase::PSButton3(void)
{
	ChangeSecurityMaintenanceWtFlags(KEY_SMM_OPERATION_CHANGE_SUBSTITUTION_CHECK, csSMMScannedUPC, KEY_SMM_FLAG_VALUE_DEFAULT, io.IsEmbeddedPrice(csSMMScannedUPC) );
	RETURNSTATE(SmmEditAdd)
}

/////////////////////////////////////////////
// PSButton8 - Go Back
/////////////////////////////////////////////
SMStateBase  *SMSmmEditWtSubCkBase::PSButton8(void)
{
  // Tell the security sub-system that we are back to Security Maintenance main state
  CKeyValueParmList parmList;
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_SMM_OPERATION, KEY_SMM_OPERATION_CANCEL);
  _bstr_t bstrParmList((const _TCHAR *)parmList);
  SecMgr.SecurityMaintenance(lSecMgrCookie, bstrParmList);  

  RETURNSTATE(SmmEditAdd)
}
