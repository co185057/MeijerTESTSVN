//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmVoidBase.cpp
//
// TITLE: Class implementation for Confirm Void State
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#ifndef _CPPUNIT
//               // MS MFC template header
#include <afxpriv.h>                // for USES_CONVERSION
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMConfirmVoid.h"      
#include "SMVoidItem.h"             // Void item state
#include "SMSecUnExpectedIncrease.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("ConfirmVoidBase")
#else
#include "SMConfirmVoidBase.h"
#endif
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
#ifndef _CPPUNIT
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
#endif
  COleVariant vValue;
  CString csReceipt;
  if (fInMultiSelectPickList)
  {     
      csReceipt = _T("PickListReceiptVoid");
  }
  else
  {
      csReceipt = _T("CMReceiptVoid");
  }

  m_pAppPSX->GetReceiptItemVariable(_T("CMSMReceipt"), ps.GetSelection(csReceipt), _T("ITEM_DESCRIPTION"), vValue);
#ifndef _CPPUNIT
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
#endif
  return STATE_NULL;                // stay in ConfirmVoid state
}
#ifndef _CPPUNIT
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

  // TAR 391552 (New Fix) +
  if (g_pDelay_WaitForApprovalStateChange != NULL)
  {
	  CString csStateName = g_pDelay_WaitForApprovalStateChange->GetRuntimeClass()->m_lpszClassName;
	  delete g_pDelay_WaitForApprovalStateChange;
      g_pDelay_WaitForApprovalStateChange = NULL;
	  trace(L6,_T("Delete the state %s saved in g_pDelay_WaitForApprovalStateChange"),csStateName);
	  OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR, KEY_EXCEPTION_CATEGORY_IMMEDIATE_WT_CHANGES);
  }
  // TAR 391552 (New Fix) -
  
  //SSCop-3345+
  if ( fInMultiSelectPickList )
  {
 	g_lNumberOfItemsNotBagInMultiPick--;
  }
  //sscop-3345-

  if (m_GenericDelayedInterventionStatus == GENERIC_DELAYED_COMPLETING_APPROVALS)
  {
      m_GenericDelayedInterventionStatus = GENERIC_DELAYED_COMPLETE;
  }

  return TBItemSale(true,lVoidTcd,m_bIsVoidByScanning); //tar 241704
}

//////////////////////////////////////
SMStateBase  *SMConfirmVoidBase::PSButton2(void) // no, Void is wrong
{
  bEASReEnableScanner = true;	// TAR227889 CHU 021403 allow to reenable the scanner
  //+SSCOP-1456 - comment out the following code.  Because scotapp will go back to Void Item screen,
  // don't need to send Item Void End message.
  // +TAR 269208
  //CKeyValueParmList parmList;
  //CKeyValue<const _TCHAR*>::Put(parmList, KEY_IV_OPERATION, KEY_IV_OPERATION_END);
  //_bstr_t bstrParmList((const _TCHAR *)parmList); 
  //SecMgr.VoidItem(lSecMgrCookie, bstrParmList);
  // -TAR 269208
  //-SSCOP-1456

  // TAR 391552 (New Fix) +
  SMStateBase *sReturnState = CkStateStackAndReturn();
  if (sReturnState != STATE_NULL)
  {
	  trace(L7, _T("Unexpected decrease was detected on SMVoidItem!"));
	  return sReturnState;
  }
  else
  {
	  RETURNSTATE(VoidItem)
  }
  // TAR 391552 (New Fix) -

  //RETURNSTATE(VoidItem)
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMConfirmVoidBase::OnWtDecrease()
{
	//+SSCOP-1456 - Save unexpected decrease intervention
	SMStateBase::g_pDelay_WaitForApprovalStateChange = SMStateBase::createState(BES_SECUNEXPECTEDDECREASE);
	trace(L6,_T("We are at SMConfirmVoidBase::OnWtDecrease(), Saving next state BES_SECUNEXPECTEDDECREASE in g_pDelay_WaitForApprovalStateChange."));
	//-SSCOP-1456

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
#endif