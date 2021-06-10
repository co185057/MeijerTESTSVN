//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmVoidBase.cpp
//
// TITLE: Class implementation for Confirm Void State
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header
#include <afxpriv.h>                // for USES_CONVERSION
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMConfirmVoid.h"      
#include "SMVoidItem.h"             // Void item state
#include "SMSecUnExpectedIncrease.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("ConfirmVoidBase")

IMPLEMENT_DYNCREATE(SMConfirmVoidBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(ConfirmVoid)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMConfirmVoidBase::SMConfirmVoidBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////
SMStateBase  *SMConfirmVoidBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ConfirmVoid") );	// RFC 330050
  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_CANCELCONFIRM, SCOT_LANGUAGE_PRIMARY));
  // +TAR 269208
  // Tell security manager we are in confirm void
  CKeyValueParmList parmList;
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_IV_OPERATION, KEY_IV_OPERATION_BEGIN);
  _bstr_t bstrParmList((const _TCHAR *)parmList); 
  SecMgr.VoidItem(lSecMgrCookie, bstrParmList);
  // -TAR 269208

  ps.SetCMFrame(_T("ConfirmVoid"));

//*** NewUI ***//
  USES_CONVERSION;
  CString csDesc;
  COleVariant vValue;
  m_pAppPSX->GetReceiptItemVariable(_T("CMSMReceipt"), ps.GetSelection(_T("CMReceiptVoid")), _T("ITEM_DESCRIPTION"), vValue);
  vValue.ChangeType( VT_BSTR );
  csDesc = OLE2T(vValue.bstrVal);

  //TAR 407723 - no flashing of button when assist menu on RAP
  if (co.IsInAssistMenus())
  {
	ps.CMButtonFlash(_T("CMButton1Med"),false);   //  no flashing of YES button.
  }
  else
  {
	ps.CMButtonFlash(_T("CMButton1Med"),true);   //  Flash the YES button.
  } 
  
//*** end of NewUI ***//

  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp

  CString csWork;
  if (co.IsInAssistMenus())
  {
      csWork.Format(ps.GetPSText(MSG_CANCELCONFIRM_AM), csDesc);
  }
  else
  {
      csWork.Format(ps.GetPSText(MSG_CANCELCONFIRM), csDesc);
  }
  ps.Message12(csWork,true);
  DMSayPhrase(CONFIRMCANCEL);
  ps.ShowCMFrame();
  return STATE_NULL;                // stay in ConfirmVoid state
}

//////////////////////////////////////
SMStateBase  *SMConfirmVoidBase::PSButton1(void) // Yes, Void is ok
{
  ps.HideButtons();
    // +TAR 297874
  if ( co.IsInAssistMenus() )
  {
	CKeyValueParmList parmList;
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_IV_OPERATION, KEY_IV_OPERATION_END);
	_bstr_t bstrParmList((const _TCHAR *)parmList); 
	SecMgr.VoidItem(lSecMgrCookie, bstrParmList);
  }
  // -TAR 297874
  return TBItemSale(true,lVoidTcd,m_bIsVoidByScanning); //tar 241704
}

//////////////////////////////////////
SMStateBase  *SMConfirmVoidBase::PSButton2(void) // no, Void is wrong
{
  bEASReEnableScanner = true;	// TAR227889 CHU 021403 allow to reenable the scanner
  // +TAR 269208
  CKeyValueParmList parmList;
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_IV_OPERATION, KEY_IV_OPERATION_END);
  _bstr_t bstrParmList((const _TCHAR *)parmList); 
  SecMgr.VoidItem(lSecMgrCookie, bstrParmList);
  // -TAR 269208
  RETURNSTATE(VoidItem)
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMConfirmVoidBase::OnWtDecrease()
{
	return VoidingWtDecrease();
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMConfirmVoidBase::OnWtIncreaseNotAllowed()	 
{
	return VoidingWtIncrease(BES_CONFIRMVOID, BES_CONFIRMVOID);
}

// + RFC 330050
void SMConfirmVoidBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ConfirmVoid") );	
}
// - RFC 330050
