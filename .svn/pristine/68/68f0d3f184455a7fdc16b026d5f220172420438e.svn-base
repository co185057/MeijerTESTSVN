//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMRestrictedAgeYN.CPP
//
// TITLE: Class implementation for store mode abort state
//
//
// AUTHOR:    
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//									// MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMRestrictedAgeYN.h"      // CRD 125377 
// #include "SMTextMsgYN.h"			// CRD 125377 future function 
#include "transbrokerssf.h"
#include "SMWaitForApproval.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SMRestrictedAgeYN")

IMPLEMENT_DYNCREATE(SMRestrictedAgeYN, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////
SMRestrictedAgeYN::SMRestrictedAgeYN()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMRestrictedAgeYN::Initialize(void)
{

	if(!fInMultiSelectPickList)
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmAuthorization") ); // TAR 299102
		CKeyValueParmList parmList;
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_TC_OPERATION_VOID_BEGIN);
		_bstr_t bstrParmList((const _TCHAR *)parmList);
		SecMgr.TransactionControl(lSecMgrCookie, bstrParmList);
	}

	trace(L6, _T("m_bSNDmPerkEntered = %d m_bSNDAgeEntered=%d"), m_bSNDmPerkEntered, m_bSNDAgeEntered);

/*
	if (m_bSNDAgeEntered) 
	{
		return PSButton2(); // already input
	}
*/
	// NewUI
	ps.SetFrame(_T("RestrictedAgeYN"));
	ps.Echo(PS_BLANK);
	
	if(co.IsInAssistMenus())
		return PSButton1();
	
	// End NewUI
	//ps.ShowCMTotal(true ,false);
	//ps.ShowCMTBText(csTBMessage2Scot);	//thp
	
	//Tar344136
	//ps.ButtonEnable(_T("SMButton2"), true);
	
	ps.ShowFrame();
	
  return STATE_NULL;                // stay in SmAbortBase state
}

void SMRestrictedAgeYN::UnInitialize(void)
{

//	if(gpSCOTAuxDisp)
//		gpSCOTAuxDisp->SetLanguage(m_customerLanguage);

}
/*
SMStateBase * SMRestrictedAgeYN::OnWtIncreaseNotAllowed(void)
{
	if(!fInMultiSelectPickList)
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END,_T("SmAuthorization") ); 
		SASendSCOTInput( TRANSACTION_CONTINUE );
	}
	
	bApplicationErrorApprovalRequired = FALSE;
	trace(L6, _T("ra.OnNoRAPDataNeeded()"));
	ra.OnNoRAPDataNeeded();	

	return SMState::OnWtIncreaseNotAllowed();
}

SMStateBase * SMRestrictedAgeYN::OnWtDecrease(void)
{
	if(!fInMultiSelectPickList)
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END,_T("SmAuthorization") ); 
		SASendSCOTInput( TRANSACTION_CONTINUE );
	}
	
	bApplicationErrorApprovalRequired = FALSE;
	trace(L6, _T("ra.OnNoRAPDataNeeded()"));
	ra.OnNoRAPDataNeeded();	
    return SMState::OnWtDecrease();
}
*/

// Yes Button 
SMStateBase * SMRestrictedAgeYN::PSButton1(void)
{
	/*
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
	*/

/*
		CString csTBState(_T(""));
        SendTBCommand(TBCMD_GET_TB_STATE, &csTBState);
        CString csPOSState(_T(""));
        SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);
		
		trace(L6, _T("csPOSState=%s csTBState=%s m_bSNDAllowItemMenu=%d"), csPOSState, csTBState, m_bSNDAllowItemMenu);

	SendTBCommand(TBCMD_SENDKEYEDDATA, CString(_T("{F8}{F1}")));    
*/
  /*
	Sleep(2000);
	CString csPOSState;
    SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);
	trace(L6, _T("RestrictedAgeYN csPOSState = %s"), csPOSState );
	if(csPOSState.Find(_T("AGE_VERIFY")) != -1) //POS293066
	{
		trace(L6, _T("We got here while POS is at Age Verify screen. Triggering WaitForApproval."));
		CREATE_AND_DISPENSE(WaitForApproval) (false,false, false) ; //(bVerifyTrans, bVoidTrans, bVerifySig)  
	}
*/

return TBCancelTransaction(true);
  // return STATE_NULL;
}

// No Button 
SMStateBase * SMRestrictedAgeYN::PSButton2(void)
{
/*
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
		return createAnchorState();
	}
	
	//TAR 259372 - return to revious state
  return STATE_RETURN;
*/ 

//	RETURNSTATE(TextMsgYN)

	
	if (getAnchorState() == BES_SCANANDBAG)
		m_bSNDAllowItemMenu = true;

	return createAnchorState();
}

/*
SMStateBase * SMRestrictedAgeYN::PSEnterKey(void)
{
	DMSayPhrase(KEYPADVALID);
	return PSButton1();
}

SMStateBase * SMRestrictedAgeYN::PSCancelKey(void)
{
	DMSayPhrase(KEYPADVALID);
	return PSButton2();
}

SMStateBase * SMRestrictedAgeYN::PSReceiptUp(void)
{
	ps.ScrollUp(_T("SMReceipt"));
	return STATE_NULL;
}

SMStateBase * SMRestrictedAgeYN::PSReceiptDown(void)
{
	ps.ScrollDown(_T("SMReceipt"));
	return STATE_NULL;
}
*/