//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAbortBase.CPP
//
// TITLE: Class implementation for store mode abort state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#pragma warning( push )
#pragma warning( disable : 4786 )
#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSmAbort.h"              // MGV this state
#include "SMTakeCard.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmAbortBase")

IMPLEMENT_DYNCREATE(SMSmAbortBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmAbort)


DEFINE_TIMESTAMP

//////////////////////
SMSmAbortBase::SMSmAbortBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////
SMStateBase  *SMSmAbortBase::Initialize(void)
{
  //+ TAR 384891 and TAR 386017
  if(!fInMultiSelectPickList)
  {
     TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmAuthorization") ); // TAR 299102
     CKeyValueParmList parmList;
     CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_TC_OPERATION_VOID_BEGIN);
     _bstr_t bstrParmList((const _TCHAR *)parmList);
     SecMgr.TransactionControl(lSecMgrCookie, bstrParmList);
  }
  //-TAR 384891 and TAR 386017
  
  //Tar 338068, approve has to be done when user press Yes or No button
  //move to PSButton1 and PSbutton2
  //ra.OnNoRAPDataNeeded(); //tar 245573 (Need to clear the Cancel Tranx Intervention on RAP)

  //set language to primary language for operator 
  ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
  if(gpSCOTAuxDisp)
  gpSCOTAuxDisp->SetLanguage(SCOT_LANGUAGE_PRIMARY);
  // NewUI
  ps.SetFrame(_T("SmAbort"));
  ps.Echo(PS_BLANK);
  // End NewUI
  ps.ShowSMTotal(true);
  ps.ShowSMTBText(csTBMessage2Scot);	//thp
  ps.SMShowItemReceipt(true);

  //Tar344136
  ps.ButtonEnable(_T("SMButton2"), true);

  ps.ShowFrame();

  return STATE_NULL;                // stay in SmAbortBase state
}

//////////////////////////////////
SMStateBase  *SMSmAbortBase::PSButton1(void) // yes
{
	//Tar 338068
  trace(L6, _T("ra.OnNoRAPDataNeeded()"));
  ra.OnNoRAPDataNeeded();

  //+Tar 384891 and TAR 386017
  if(fInMultiSelectPickList)
  {
      //Need to cancel the item currently in the middle of wt check up
      SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
	  if(ps.GetPicklistItemsSold() > 0)
	  {
	    ps.SetPicklistItemsSold(0);
	    ps.SetPicklistItemsIncludeVoidItems(0);
	  }
      //Put SM into Void Mode
      TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmAuthorization") ); // TAR 299102
      CKeyValueParmList parmList;
      CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_TC_OPERATION_VOID_BEGIN);
      _bstr_t bstrParmList((const _TCHAR *)parmList);
      SecMgr.TransactionControl(lSecMgrCookie, bstrParmList);
  }
  //-Tar 384891 and TAR 386017  
 
  SynchronizeStoreloginWithLaunchPad(_T("operation=set-signed-off")); // TAR 301706
  if ( isAnySoldVoidedItems() )
  {
	ClearStateStackEOT();
	return TBCancelTransaction(true);       // wait for *VOID TRANSACTION*
  }	
  // TAR 299102 - if we end up here then we need to issue the end here.
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAuthorization") ); 
  //Tar 124345
  return TBFinish();  //Tar 126024
    //TBFinish();
    //CREATE_AND_DISPENSE(TakeCard)(true);    
    // just restart
}

//////////////////////////////////
SMStateBase  *SMSmAbortBase::PSButton2(void) // no
{
  //+Tar 384891 and TAR 386017 
  if(!fInMultiSelectPickList)
  {
      TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAuthorization") ); // TAR 299102
      SASendSCOTInput( TRANSACTION_CONTINUE );
  }
  //-Tar 384891 and TAR 386017 
  //TAR#125989	LPM111199 the global must be set to false when the error is canceled
  bApplicationErrorApprovalRequired = FALSE;
  // TAR 197203
  trace(L6, _T("ra.OnNoRAPDataNeeded()"));
  ra.OnNoRAPDataNeeded();	
  
  SMStateBase *sReturnState = CkStateStackAndReturn();
  if (sReturnState != STATE_NULL)
	  return sReturnState;

  if(getAnchorState() == BES_VOIDITEM)
  {
	  return setAndCreateAnchorState(BES_SCANANDBAG);
  }

  //TAR 259372 - return to revious state
  return STATE_RETURN;

 
}

///////////////////////////////////
SMStateBase  *SMSmAbortBase::PSEnterKey(void)// enter key pressed, void transaction
{
  DMSayPhrase(KEYPADVALID);
  return PSButton1();
}

////////////////////////////////////
SMStateBase  *SMSmAbortBase::PSCancelKey(void) // Cancel
{
  DMSayPhrase(KEYPADVALID);
  return PSButton2();
}

void SMSmAbortBase::UnInitialize()
{
  ps.SetLanguage(m_customerLanguage);
  if(gpSCOTAuxDisp)
  gpSCOTAuxDisp->SetLanguage(m_customerLanguage);
}
////////////////////////////////////////
// NewUI
SMStateBase  *SMSmAbortBase::PSReceiptUp(void)
{
  ps.ScrollUp(_T("SMReceipt"));
  return STATE_NULL;

}
SMStateBase  *SMSmAbortBase::PSReceiptDown(void)
{
  ps.ScrollDown(_T("SMReceipt"));
  return STATE_NULL;

}
// End NewUI
#pragma warning( pop )
