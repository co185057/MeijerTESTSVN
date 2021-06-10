//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmEditWtExpBase.CPP
//
// TITLE: Class implementation for store mode Security Maintenance Mode edit wt expectation
//
// AUTHOR:    Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>             // MS MFC always needed first
//           // MS MFC template header
#include "string.h"
#include "afx.h"

#include "Common.h"             // common includes
#include "SMState.h"			// Base state
#include "SMSmmEditWtExp.h"		// This state
#include "SMSmmEditAdd.h"		// Prev state 

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmmEditWtExpBase")

IMPLEMENT_DYNCREATE(SMSmmEditWtExpBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmmEditWtExp)

DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmmEditWtExpBase::SMSmmEditWtExpBase()
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////////
// Initialize
//////////////////////////////////////////////
SMStateBase  *SMSmmEditWtExpBase::Initialize(void)
{
  bOkToDisplayBagScaleInfo = false;
  DMScannerDisable();
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmmEditWtExp") );	// RFC 330050
  // NewUI
  ps.SetFrame(_T("SmmEditWtExp"));
  // End NewUI
  if (dm.fStateTakeawayBelt)//TD TAR 205490
	 ps.SetLTTinto2(TXT_SMM_WTEXPBELT_INSTR);	//Lead thru text is 2 lines long...
  else
	ps.SetLTTinto2(TXT_SMM_WTEXP_INSTR);	//Lead thru text is 2 lines long...

  //+SSCOP-1036 Remove unused code
  // button 3 text changes depending on what the default value is
  /*CString csDefaultBtnTxt, csDefSetting;

  if (lSMMglWtExp == SMM_FALSE)	
	csDefSetting = ps.GetPSText(BTT_NO);
  else		
	csDefSetting = ps.GetPSText(BTT_YES);

  csDefaultBtnTxt.Format(ps.GetPSText(BTT_SMM_DEFAULT), csDefSetting); 
  ps.SetPSText(PS_WORK1, csDefaultBtnTxt);*/
  //-SSCOP-1036
  if ( (csSMMWeightRequired == _T("default-y")) || (csSMMWeightRequired == _T("default-n")))
  {
	ps.SetButtonText(_T("SMButton1"), BTT_YES);
	ps.SetButtonText(_T("SMButton2"), BTT_NO);
	ps.SetButtonText(_T("SMButton3"), BTT_OPTIONAL);
	ps.Button(_T("SMButton4"), BTT_SMM_DEFAULT, false);//SSCOP-1036
  }
  else if ( csSMMWeightRequired == _T("y") )
  {
	ps.Button(_T("SMButton1"), BTT_YES, false);
	ps.SetButtonText(_T("SMButton2"), BTT_NO);
	ps.SetButtonText(_T("SMButton3"), BTT_OPTIONAL);
  	ps.SetButtonText(_T("SMButton4"), BTT_SMM_DEFAULT);//SSCOP-1036
  }
  else if ( csSMMWeightRequired == _T("n") )
  {
	ps.SetButtonText(_T("SMButton1"), BTT_YES);
	ps.Button(_T("SMButton2"), BTT_NO, false);
	ps.SetButtonText(_T("SMButton3"), BTT_OPTIONAL);
  	ps.SetButtonText(_T("SMButton4"), BTT_SMM_DEFAULT);//SSCOP-1036
  }
  else if ( csSMMWeightRequired == _T("o") )
  {
	ps.SetButtonText(_T("SMButton1"), BTT_YES);
	ps.SetButtonText(_T("SMButton2"), BTT_NO);
	ps.Button(_T("SMButton3"), BTT_OPTIONAL, false);
  	ps.SetButtonText(_T("SMButton4"), BTT_SMM_DEFAULT);//SSCOP-1036
  }
  else
  {
	ps.SetButtonText(_T("SMButton1"), BTT_YES);
	ps.SetButtonText(_T("SMButton2"), BTT_NO);
	ps.SetButtonText(_T("SMButton3"), BTT_OPTIONAL);
	ps.SetButtonText(_T("SMButton4"), BTT_SMM_DEFAULT);//SSCOP-1036
  }
  ps.ShowFrame();
  return STATE_NULL;                // stay in SMSmmEditWtExpBase state
}

/////////////////////////////////////////////
// uninitialize
//////////////////////////////////////////////
void SMSmmEditWtExpBase::UnInitialize(void)
{
  bOkToDisplayBagScaleInfo = true;
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmmEditWtExp") );	// RFC 330050
}

/////////////////////////////////////////////
// PSButton1 - Wt Expectation = Yes
/////////////////////////////////////////////
SMStateBase  *SMSmmEditWtExpBase::PSButton1(void)
{
	// Tell the security sub-system to change Wt Expectation to y for the item
	ChangeSecurityMaintenanceWtFlags(KEY_SMM_OPERATION_CHANGE_WT_REQUIRED, csSMMScannedUPC, _T("y"), io.IsEmbeddedPrice(csSMMScannedUPC) );
	RETURNSTATE(SmmEditAdd)
}

/////////////////////////////////////////////
// PSButton2 - Wt Expectation = No
/////////////////////////////////////////////
SMStateBase  *SMSmmEditWtExpBase::PSButton2(void)
{
	// Tell the security sub-system to change Wt Expectation to n for the item
	ChangeSecurityMaintenanceWtFlags(KEY_SMM_OPERATION_CHANGE_WT_REQUIRED, csSMMScannedUPC, _T("n"), io.IsEmbeddedPrice(csSMMScannedUPC) );
	RETURNSTATE(SmmEditAdd)
}

/////////////////////////////////////////////
// PSButton3 - Wt Expectation = Optional
/////////////////////////////////////////////
SMStateBase  *SMSmmEditWtExpBase::PSButton3(void)
{
	// Tell the security sub-system to change Wt Expectation to o for the item
	ChangeSecurityMaintenanceWtFlags(KEY_SMM_OPERATION_CHANGE_WT_REQUIRED, csSMMScannedUPC, _T("o"), io.IsEmbeddedPrice(csSMMScannedUPC) );
	RETURNSTATE(SmmEditAdd)
}


/////////////////////////////////////////////
// PSButton4 - set to default value
/////////////////////////////////////////////
SMStateBase  *SMSmmEditWtExpBase::PSButton4(void)
{
	// Tell the security sub-system to change Wt Expectation to n for the item
	ChangeSecurityMaintenanceWtFlags(KEY_SMM_OPERATION_CHANGE_WT_REQUIRED, csSMMScannedUPC, KEY_SMM_FLAG_VALUE_DEFAULT, io.IsEmbeddedPrice(csSMMScannedUPC) );
	RETURNSTATE(SmmEditAdd)
}

/////////////////////////////////////////////
// PSButton8 - Go Back
/////////////////////////////////////////////
SMStateBase  *SMSmmEditWtExpBase::PSButton8(void)
{
  // Tell the security sub-system that we are back to Security Maintenance main state
  CKeyValueParmList parmList;
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_SMM_OPERATION, KEY_SMM_OPERATION_CANCEL);
  _bstr_t bstrParmList((const _TCHAR *)parmList);
  SecMgr.SecurityMaintenance(lSecMgrCookie, bstrParmList);  

  RETURNSTATE(SmmEditAdd)
}
