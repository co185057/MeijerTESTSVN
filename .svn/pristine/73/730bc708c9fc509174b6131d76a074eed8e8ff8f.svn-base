//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmKeyInWtTolBase.CPP
//
// TITLE: Class implementation for store mode Security Maintenance Mode key in tolerance
//
// AUTHOR:    Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // common includes
#include "SMState.h"                // Base state
#include "SMSmmKeyInWtTol.h"	// This state
#include "SMSmmEditAdd.h"		// Prev state
#include "SMSmmInvalidWtTol.h"	// Error state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmmKeyInWtTolBase")

IMPLEMENT_DYNCREATE(SMSmmKeyInWtTolBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmmKeyInWtTol)

DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmmKeyInWtTolBase::SMSmmKeyInWtTolBase()
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////////
// Initialize
//////////////////////////////////////////////
SMStateBase  *SMSmmKeyInWtTolBase::Initialize(void)
{
  bOkToDisplayBagScaleInfo = false;
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmmKeyInWtTol") );	// RFC 330050
  DMScannerDisable();

  ps.SetFrame(_T("SmmKeyInWtTol"));

  CString csDisplayUPD;
  csDisplayUPD.Format(_T("%s%s%s"), _T("<"), csSMMScannedUPC, _T(">"));
  ps.SetTitle(ps.GetPSText(LTL_SMM_KEYINWTTOL_TITLE), (LPCTSTR)csDisplayUPD);	//Title text is 2 lines and shows the UPC New UI doesn't do this.
  ps.SetLTTinto2(TXT_SMM_KEYINWTTOL_INSTR);	//Lead thru text is 2 lines long...

  COleVariant v;
  v = (long) UI::Normal;
  m_pAppPSX->SetControlProperty(_T("KeyBoardP3"), UI::PROPERTYSTATE, v);
  ps.AllowInput(MAX_DIGITS,false,false);	// allow the EchoInput() func to accept data
  ps.ShowFrame();
  return STATE_NULL;                // stay in SMSmmKeyInWtTolBase state
}

/////////////////////////////////////////////
// uninitialize
//////////////////////////////////////////////
void SMSmmKeyInWtTolBase::UnInitialize(void)
{
  bOkToDisplayBagScaleInfo = true;
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmmKeyInWtTol") );	// RFC 330050
}

/////////////////////////////////////////////
// PSEnterKey - enter on the pinpad
//////////////////////////////////////////////
SMStateBase * SMSmmKeyInWtTolBase::PSEnterKey()
{
	CString csValue;
	long lLen = ps.GetInput(csValue);
	// +TAR225992 - when the tolerance is out or range (invalid, the global should not be used LPM020703
	long lTolVal;
	int nCommaPos = csValue.Replace(_T(","), _T("."));	//added for foreign language support 
	double fToleranceValue = _tcstod(csValue, NULL);		//retain decimal value
	double ftempVal = fToleranceValue * 1000;
	lTolVal = (long)ftempVal;
	// -TAR225992

	if (lLen==0)		//make sure there is a value in the echo box
	{
		DMSayPhrase(KEYPADINVALID);
		return PSClearKey();
	}
	CString csToleranceValue;
	csToleranceValue.Format(_T("%d"),lTolVal);

	PSClearAllInput();	// TAR 276882

	ChangeSecurityMaintenanceTolerance(KEY_SMM_OPERATION_CHANGE_WT_TOLERANCE, csSMMScannedUPC, csToleranceValue, io.IsEmbeddedPrice(csSMMScannedUPC) );

	return STATE_NULL;
}

/////////////////////////////////////////////
// PSNumericKey - numbers on the pinpad
///////////////////////////////////////////////////////////////
SMStateBase * SMSmmKeyInWtTolBase::PSNumericKey(const long lKey)
{
  PSEchoInput(lKey);
  return STATE_NULL;
}

/////////////////////////////////////////////
// PSDecimalKey - decimal or period
///////////////////////////////////////////////////////////////
SMStateBase * SMSmmKeyInWtTolBase::PSDecimalKey(const long lKey)
{
  PSEchoDecimalInput(lKey);
  COleVariant v;
  v = (long) UI::Disabled;
  m_pAppPSX->SetControlProperty(_T("KeyBoardP3"), UI::PROPERTYSTATE, v);
  return STATE_NULL;
}

/////////////////////////////////////////////
// PSClearKey - Clear all input (on the pinpad)
//////////////////////////////////////////////
SMStateBase * SMSmmKeyInWtTolBase::PSClearKey()
{
  PSClearAllInput();
  COleVariant v;
  v = (long) UI::Normal;
  m_pAppPSX->SetControlProperty(_T("KeyBoardP3"), UI::PROPERTYSTATE, v);
  return STATE_NULL;
}

/////////////////////////////////////////////
// PSButton8 - Go Back
/////////////////////////////////////////////
SMStateBase  *SMSmmKeyInWtTolBase::PSButton8(void)
{
  // Tell the security sub-system that we are back to Security Maintenance main state
  CKeyValueParmList parmList;
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_SMM_OPERATION, KEY_SMM_OPERATION_CANCEL);
  _bstr_t bstrParmList((const _TCHAR *)parmList);
  SecMgr.SecurityMaintenance(lSecMgrCookie, bstrParmList);  

  ps.PSHideSMMNumericKeypad();
  PSClearInput();
  RETURNSTATE(SmmEditAdd)
}
