//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmKeyInItemCode.CPP
//
// TITLE: Class implementation for store mode Security Maintenance Mode key in tolerance
//
// AUTHOR:  Rhonah Entize
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // common includes
#include "SMState.h"                // Base state
#include "SMSmmKeyInItemCode.h"	// This state
#include "SMSmmEditAdd.h"		// Prev state


#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmmKeyInItemCodeBase")

//Align with SAServer to have a maximum of 25 chars since
//Database only has 25 chars len for UPC	
#define UPC_MAX_CHAR	25	

IMPLEMENT_DYNCREATE(SMSmmKeyInItemCodeBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmmKeyInItemCode)

DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmmKeyInItemCodeBase::SMSmmKeyInItemCodeBase()
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////////
// Initialize
//////////////////////////////////////////////
SMStateBase  *SMSmmKeyInItemCodeBase::Initialize(void)
{
  bOkToDisplayBagScaleInfo = false;
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmmKeyInItemCode") );	
  DMScannerDisable();

  ps.SetFrame(_T("SmmKeyInItemCode"));
  ps.SetTitle(LTT_KEYINCODE);
  ps.SetLeadthruText(MSG_KEYINCODE, PS_BLANK);
  
  ps.SetButtonText(_T("SMButton8"),BTT_GOBACK);
  ps.AllowInput(UPC_MAX_CHAR,false,false);	
  ps.ShowFrame();
  return STATE_NULL;                
}

/////////////////////////////////////////////
// uninitialize
//////////////////////////////////////////////
void SMSmmKeyInItemCodeBase::UnInitialize(void)
{
  bOkToDisplayBagScaleInfo = true;
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmmKeyInItemCode") );
}

/////////////////////////////////////////////
// PSEnterKey - enter on the pinpad
//////////////////////////////////////////////
SMStateBase * SMSmmKeyInItemCodeBase::PSEnterKey()
{

  ps.GetInput(csItemSent);
   
  //Keyed-in items will be treated the same way as those scanned.
  //Better use csSMMScannedUPC instead of declaring another variable
  //for keyed-in upcs to make it less complicated
  csSMMScannedUPC = csItemSent;
  ResetMLBCValues();

  // Tell the security sub-system to query for the item
  CKeyValueParmList parmList;
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_SMM_OPERATION, KEY_SMM_OPERATION_QUERY);
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_UPC,csSMMScannedUPC );
  CKeyValue<bool>::Put(parmList, KEY_SMM_PRICE_EMBEDDED, io.b_gIsThisItemPriceEmbedded);

  _bstr_t bstrParmList((const _TCHAR *)parmList);
  long lrc = SecMgr.SecurityMaintenance(lSecMgrCookie, bstrParmList);

  bIsSMMItemAdded = false;
  bIsSMMItemDeleted = false;
  bSMMPreScanState = false;

  return PSButton8();
}

/////////////////////////////////////////////
// PSNumericKey - numbers on the pinpad
///////////////////////////////////////////////////////////////
SMStateBase * SMSmmKeyInItemCodeBase::PSNumericKey(const long lKey)
{
  PSEchoInput(lKey);
  
  COleVariant v;
  v = (long) UI::Normal;
  m_pAppPSX->SetControlProperty(_T("Backspace"), UI::PROPERTYSTATE, v);
  
  return STATE_NULL;
}


/////////////////////////////////////////////
// PSClearKey - Clear all input (on the pinpad)
//////////////////////////////////////////////
SMStateBase * SMSmmKeyInItemCodeBase::PSClearKey()
{
  PSClearAllInput();
  COleVariant v;
  v = (long) UI::Disabled;
  m_pAppPSX->SetControlProperty(_T("Backspace"), UI::PROPERTYSTATE, v);
  
  return STATE_NULL;
}

SMStateBase  * SMSmmKeyInItemCodeBase::PSBackSpaceKey(void)
{
  PSBackSpaceInput();

  CString csValue;
  long lLen = ps.GetInput(csValue);
  
  if (lLen==0)	
  {
	  COleVariant v;
	  v = (long) UI::Disabled;
	  m_pAppPSX->SetControlProperty(_T("Backspace"), UI::PROPERTYSTATE, v);
  }
  return STATE_NULL;
}

/////////////////////////////////////////////
// PSButton8 - Go Back
/////////////////////////////////////////////
SMStateBase  *SMSmmKeyInItemCodeBase::PSButton8(void)
{
  CKeyValueParmList parmList;
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_SMM_OPERATION, KEY_SMM_OPERATION_CANCEL);
  _bstr_t bstrParmList((const _TCHAR *)parmList);
  SecMgr.SecurityMaintenance(lSecMgrCookie, bstrParmList);  

  ps.PSHideSMMNumericKeypad();
  PSClearInput();
  RETURNSTATE(SmmEditAdd)
}

