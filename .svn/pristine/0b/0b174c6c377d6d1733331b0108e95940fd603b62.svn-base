//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAbortedBase.CPP
//
// TITLE: Class implementation for store mode aborted state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSmAborted.h"            // MGV abort-ed state
#include "SMTakeCard.h"             // MGV take card

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmAbortedBase")

IMPLEMENT_DYNCREATE(SMSmAbortedBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmAborted)


DEFINE_TIMESTAMP

//////////////////////////
SMSmAbortedBase::SMSmAbortedBase()
{
  IMPLEMENT_TIMESTAMP
}

/////////////////////////////////////
SMStateBase  *SMSmAbortedBase::Initialize(void)
{

  //TAR376391+
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmAuthorization") ); // TAR 299102
  CKeyValueParmList parmList;
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_TC_OPERATION_VOID_BEGIN);
  _bstr_t bstrParmList((const _TCHAR *)parmList);
  SecMgr.TransactionControl(lSecMgrCookie, bstrParmList);
  //TAR376391-
  
 /** 
  trace(L6, _T("+ra.OnTransactionEnd"));
  ra.OnTransactionEnd(); //tar 245573 (Clear all pending rap interventions)
  trace(L6, _T("-ra.OnTransactionEnd"));
*/

  //set language to primary language for operator 
  ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
  if(gpSCOTAuxDisp)
  gpSCOTAuxDisp->SetLanguage(SCOT_LANGUAGE_PRIMARY);
  // NewUI  
  ps.SetFrame(_T("SmAborted"));

  //OK button should be enabled unconditionally. This lets the customer acknowledge the message and get back to the home screen.
  ps.ButtonEnable(_T("SMButton1"), true);

  // NewUI
  ps.ShowFrame();

  return STATE_NULL;                // stay in SmAbort state
}

////////////////////////////////////
SMStateBase  *SMSmAbortedBase::PSButton1(void) // OK
{
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAuthorization") ); // TAR 299102
    SynchronizeStoreloginWithLaunchPad( _T("operation=set-signed-off")); // TAR 301706
    if (fRequestForHelp)
    {
		fRequestForHelp = false;
        trace(L7, _T("+ra.OnCallForHelpApproval"));
        ra.OnCallForHelpApproval();
        trace(L7, _T("-ra.OnCallForHelpApproval"));
  }

 //420881+ tell SM voiding  ends
CKeyValueParmList parmList;
CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_TC_OPERATION_VOID_END);
_bstr_t bstrParmList((const _TCHAR *)parmList);
SecMgr.TransactionControl(lSecMgrCookie, bstrParmList);
//420881-

  CREATE_AND_DISPENSE(TakeCard)(true);
}

/////////////////////////////////////
SMStateBase  *SMSmAbortedBase::PSEnterKey(void)// enter key pressed, sell this item
{
  DMSayPhrase(KEYPADVALID);
  return PSButton1();
}

void SMSmAbortedBase::UnInitialize()
{
//+TAR 444373 - DO NOT CALL SetLanguage() to change the language code to get the right message.  We should 
// pass in the language code to GetPSText()
//  ps.SetLanguage(m_customerLanguage);
//-TAR 444373
  if(gpSCOTAuxDisp)
  gpSCOTAuxDisp->SetLanguage(m_customerLanguage);
}
