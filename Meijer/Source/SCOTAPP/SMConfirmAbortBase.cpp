//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmAbortBase.cpp
//
// TITLE: Class implementation for Transaction Abort State
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "Common.h"                 // MGV common includes

#include "SMConfirmAbort.h"
#include "SMWaitForApproval.h"
#include "SMContinueTrans.h"
#include "SMSetAllItemsAside.h"
#include "SMVoidTransApproval.h"
#include "SMProduceFavorites.h"
#include "SMSmAssistMenu.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("ConfirmAbortBase")

IMPLEMENT_DYNCREATE(SMConfirmAbortBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(ConfirmAbort)


DEFINE_TIMESTAMP

////////////////////////////////
SMConfirmAbortBase::SMConfirmAbortBase()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMConfirmAbortBase::Initialize(void)
{
    //+ TAR 384891 and TAR 386017 
	if(!fInMultiSelectPickList)
	{
	    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ConfirmAbort") );	// RFC 330050
	    CKeyValueParmList parmList;
	    CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_TC_OPERATION_VOID_BEGIN);
	    _bstr_t bstrParmList((const _TCHAR *)parmList);
	    SecMgr.TransactionControl(lSecMgrCookie, bstrParmList);
	}
	//-TAR 384891 and TAR 386017 

	//Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(LTT_CANCELALL, SCOT_LANGUAGE_PRIMARY));
	DMSayPhrase(CANCELALLITEMS);
	//TAR228754 Set this flag to be true to indicate we're in cancel item screens. 
	bIsFromVoidApproval = true;
	//TAR228754
    // NewUI
    ps.SetCMFrame(_T("ConfirmAbort"));
    ps.ShowAmountDueAmountPaid(lBalanceDue - nDMCashInserted, 
                               lTotalPaid  + nDMCashInserted);
	ps.ShowCMTBText(csTBMessage2Scot);		//thp
    ps.Message12(MSG_TRANSABORT, true);
    //TAR 407723 - no flashing of button when assist menu on RAP
	if (co.IsInAssistMenus())
	{
		ps.CMButtonFlash(_T("CMButton1Med"),false);   //  no flashing of button.
	}
	else
	{
		ps.CMButtonFlash(_T("CMButton1Med"),true);   //  Flash the button.
	}   
    // End NewUI
    ps.ShowCMFrame();
  return STATE_NULL;							  // stay in ConfirmAbortBase state
}

//////////////////////////////////////
SMStateBase  *SMConfirmAbortBase::TimedOut(void)
{
  // this may seem odd, because we just came from there
  // but SMContinueTrans is a Help state and as such
  // there can be multiple instances of it stacked.
  // Fix for TAR 153465
  if (SCOTIsMinimized())
     return STATE_NULL;
  else
     RETURNSTATE(ContinueTrans)
	  
}

///////////////////////////////////////
SMStateBase  *SMConfirmAbortBase::PSButton1(void) // Yes Dimple Button, cancel order
{
  //tar 196574
  //RETURNSTATE(SetAllItemsAside)
  //go to void approval 
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

  CREATE_AND_DISPENSE(VoidTransApproval)(BES_CONFIRMABORT, BES_TAKERECEIPT);
}

///////////////////////////////////////
SMStateBase *SMConfirmAbortBase::PSButton2(void) // cancel, no, do not abort
{
  //TAR228754 Reset this flag to indicate that we get out Cancel item screens
  bIsFromVoidApproval = false;
  //TAR228754
  bEASReEnableScanner = true; //TAR286414: reenable the scanner
  SASendSCOTInput( TRANSACTION_CONTINUE );
  if ( fInMultiSelectPickList )
  {
	  RETURNSTATE(ProduceFavorites);
  }
  else
  {
	  // TAR 391552 (New Fix) +
	  SMStateBase *sReturnState = CkStateStackAndReturn();
	  if (sReturnState != STATE_NULL)
	  {
		  trace(L7, _T("Unexpected decrease was detected on SMVoidItem!"));
		  return sReturnState;
	  }
	  else
	  {
          if (co.IsInAssistMenus() || fStateAssist)
          {
              RETURNSTATE(SmAssistMenu)
          }
          return createAnchorState();
	  }
	  // TAR 391552 (New Fix) -
	  //return createAnchorState();  // scan and bag or coupon entry
  }
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMConfirmAbortBase::OnWtDecrease() //tar 196574
{
	return VoidingWtDecrease();
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMConfirmAbortBase::OnWtIncreaseNotAllowed() //tar 196574	 
{
	return VoidingWtIncrease(BES_CONFIRMABORT, BES_CONFIRMABORT);
}
// + RFC 330050
void SMConfirmAbortBase::UnInitialize(void)
{
	if(!fInMultiSelectPickList)
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ConfirmAbort") );
	}
}
// - RFC 330050

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMConfirmAbortBase::PSButtonHelp() //TAR400959 / 426673
{
  SASendSCOTInput( TRANSACTION_CONTINUE );
  return STATE(HelpModeState)::PSButtonHelp();
}