//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMVoidNoMatchBase.cpp
//
// TITLE: Class implementation for Void Item Not Match State
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMVoidNoMatch.h"      // Header file
#include "SMVoidItem.h"             // Prev state
#include "SMSmAssistMenu.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("VoidNoMatchBase")
//Tar 121035
//DLLEXPORT extern PSInterfaceErcpt CMReceipt;

IMPLEMENT_DYNCREATE(SMVoidNoMatchBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(VoidNoMatch)


DEFINE_TIMESTAMP

//////////////////////////////
SMVoidNoMatchBase::SMVoidNoMatchBase()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase *SMVoidNoMatchBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("VoidNoMatch") );

  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_CANCELNOMATCH, SCOT_LANGUAGE_PRIMARY));
  DMSayPhrase(CANNOTFINDSCANNEDITEM);

// NewUI
  ps.SetCMFrame(_T("VoidNoMatch"));
  ps.CMButtonFlash(_T("CMButton1Med"),true);
// NewUI

  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  ps.Message12(MSG_CANTCANCEL, true);
  ps.ShowCMFrame();

  return STATE_NULL;
}

SMStateBase *SMVoidNoMatchBase::PSButton1(void) // continue
{
  RETURNSTATE(VoidItem)
}

SMStateBase *SMVoidNoMatchBase::PSEnterKey(void)
{
  DMSayPhrase(KEYPADVALID);
  return PSButton1();
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMVoidNoMatchBase::OnWtIncreaseNotAllowed()	 
{	
	return VoidingWtIncrease(BES_VOIDNOMATCH, BES_VOIDNOMATCH);  //TAR348266
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMVoidNoMatchBase::OnWtDecrease()
{

	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
void SMVoidNoMatchBase::UnInitialize()
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("VoidNoMatch") );
  // + TAR 295822
  CKeyValueParmList parmList;
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_IV_OPERATION, KEY_IV_OPERATION_END);
  _bstr_t bstrParmList((const _TCHAR *)parmList); 
  SecMgr.VoidItem(lSecMgrCookie, bstrParmList);
  // - TAR 295822

}
