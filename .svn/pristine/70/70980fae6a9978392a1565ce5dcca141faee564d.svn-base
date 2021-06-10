//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateTBBase.CPP
//
// TITLE: TB SMStateBase functions, implement all SMStateBase::TB* functions
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h> 					// MS MFC always needed first

#include "Common.h" 					//	common includes
DLLEXPORT CTransBroker tb;				//	global TB object
#include "SMState.h"					//	abstract base state

#include "SMAttract.h"					//	Attract state
#include "SMCashPayment.h"				//	Cash payment state
#include "SMScanAndBag.h"				//	scan and bag state
#include "SMBagAndEAS.h"				//	bag and eas state//T.I.D
#include "SMEnterQuantity.h"			//	enter qty state
#include "SMEnterWeight.h"				//	enter wgt state
#include "SMEnterCouponValue.h" 		//	enter coupon value
#include "SMUnknownPrice.h" 			//	unknown item price state
#include "SMItemNotForSale.h"			//	item not for sale (recalled) state
#include "SMUnknownItem.h"				//	unknown item state
#include "SMConfirmQuantity.h"			//	confirm quantity
#include "SMVoidNoMatch.h"				//	void item not matched to item state
#include "SMCouponTooHigh.h"			// coupon value too high
#include "SMScaleBroken.h"				//	scale is broken
#include "SMSetAllItemsAside.h" 		//	set items aside state
#include "SMSmAborted.h"				//	store mode set items aside state
#include "SMTakeCard.h" 				//	take charge card
#include "DMProcedures.h"
#include "SMTakeChange.h"				// Coin Acceptor Modifications
#include "SMCustomMessage.h"			// Custom Message
#include "SMEnterWtForPriceEmbedded.h"	// Forced weighing of price embedded state
#include "SMExtensionState.h"
#include "SMSmAssistMenu.h"
#include "SMDegradedContinueTrans.h"	//degraded mode tars
//#include "SMDeactivateTheftTag.h" 	// T.I.D Forced deactivation of theft prevention tags	cm150000: Merge 2
//Fiscal Printer start
#include "ncrpid.h"
#include "atlbase.h"
//Fiscal Printer end
//Tar 124345
//#include "SMTakeReceipt.h"//Tar 126024
#include "SMThankYou.h"

#include "SMSignCharge.h"		   //  check card signature
#include "SMInProgress.h"		   //  in progress state
#include "SMOutOfService.h" 	 //  OutOfServ state
#include "SMCardMisRead.h"		  //  cant read the card
#include "SMCardProcessing.h"	   //  charge tender in progress
#include "SMWaitForApproval.h"	  //  wait for approval
#include "SMRestrictedNotAllowed.h" //	restricted purchase is not allowed
#include "SMCouponNoMatch.h"		//	coupon no match
#include "SMCouponNotAllowed.h" 	// NCR coupon not allowec
#include "SMTakeLoyaltyCard.h"
#include "SMDepositCoupons.h"
#include "SMSmAuthorization.h"
#include "SMLimitedEBTBenefit.h"
#include "SMRequestSig.h"
#include "SMSmSystemFunctions.h"
#include "SMItemQuantityExceeded.h"
#include "SMEnterPin.h"
#include "SMTimeRestrictedItem.h"
#include "SMSmMediaStatus.h"  //Tar 126382
#include "SMEnterCoupons.h"
#include "SMCrateableItem.h"
#include "SMRapDataNeeded.h"
#include "SMAttract.h"
#include "SMVoidApproval.h"
#include "SMSecUnexpectedIncrease.h"
#include "SMInSAProgress.h"
#include "SMInsertCoupon.h"
#include "SMCmDataEntry.h"
#include "SMTenderBalance.h"
#include "SMDepositGiftCard.h"
#include "SMCollectGiftCard.h"
#include "SMInsertGiftCard.h"
#include "PSConfigData.h"
#include "MPProcedures.h"			//TAR291610
//USSF Start
#include "USSFProcedures.h"
#include "SMUSSFManager.h"
#include "USSFCommon.h"
//USSF End 
#include "SMSmPrintReceipt.h"
#include "SMProduceFavorites.h"
#include "SMVoidItem.h"  

#include "SMRAPDataNeeded.h"
DLLEXPORT extern CStatic *h2X20;	// 2X20 display on Dialog
DLLEXPORT extern MPProcedures mp;   //TAR291610

static CString lvSMLastState = _T("");	// 071904 TAR 273325 BMediano (storing the last state)

#define COMP_ID ID_SM			   // base state component
#define T_ID	_T("SMtb")
#define RPRAP       _T("RAP")
static long rc;  // TB return value


// TB last status
TBSTATE SMStateBase::nTBLastState = TB_UNKNOWN; // last TB state for the display
TBSTATE SMStateBase::nTBLastRealState = TB_UNKNOWN; // no real SA state to init to yet
// NewUI
USHORT SMStateBase::uKeyCodeList[200] ;
CString SMStateBase::csKeyLabeList[200];
// NewUI
int SMStateBase::nTenderButtonIndex = -1;	//New Finalization	cm150000: Merge 2
int SMStateBase::nSavedTenderButtonIndex = -1;  //tar 259370
bool SMStateBase::bUpdateFoodStampTotal = true;
bool bInSuspendCall = false;
bool SMStateBase::bEndOfDay = false;
bool SMStateBase::g_bTBCaptureDeclinedCard = false;
bool SMStateBase::g_bTBNeedMoreData = false;  // TAR 238061
bool SMStateBase::m_bCouldBeScannedLoyaltyCard = false; //TAR242494
bool SMStateBase::m_fVoidedInAssist = false;  // TAR246858
extern bool bWriteEndOfTransactionToWLDBlog; //NN RFC 210177
bool SMStateBase::m_bWaitForCashDrawerClose=false;
bool SMStateBase::fTenderDeclined = false;
bool SMStateBase::m_bTBNeedAssistModeGetDiag = false; //RFC #282631
bool SMStateBase::m_bCancellingCashTender = false;  //RFC 275377 MIGRATED-BM185018
bool SMStateBase::bIsCoupon = false;	//TAR339619

//+SR697
bool SMStateBase::m_bEOTPrintPending = false;
bool SMStateBase::m_bForceReceiptPrint = false;
//-SR697

bool SMStateBase::bIsMaxCouponLimitInterventionDone = false;	//SR678

//+dp185016
int SMStateBase::m_nTBOutOfSyncError = 0;
#define OUTOFSYNCTRACEFILE _T("%DATA_DRIVE%\\scot\\logs\\OutOfSync.log")
TraceObject SMStateBase::Out_of_sync_Trace_Object( OUTOFSYNCTRACEFILE );
//-dp185016
// this is not a state member, but a transaction specific variable
// reset at the end of trx
int nGenerateEntryIDForTB = INVALID_ENTRYID; // used to set entry ID if TB does not provide one
long GenerateUniqueEntryID(long& lEntryID); // defined in this function

// start of TAR 393349
// Receipt File Handle Management
//static long rc;
CArray< HFILE, HFILE> g_ArrayOfReceiptHandles;  // storage for multiple handles
// end of TAR 393349


////////////////////////////////////
// Process the return value from Transaction Broker
void SMStateBase::ProcessTBReturn(long rc)
{
	// TAR #200002 HC 032602 Start
	CString csErrorMsg;
	extern bool bShowTBErrors;
	extern bool bShowTBInvalidUserID;
	extern bool bShowTBInvalidUserPW;
	extern bool bShowTBInvalidState;
	extern bool bShowTBFileNotFound;
	extern bool bShowTBIncompleteProcedure;
	extern bool bShowTBInvalidWindow;
	extern bool bShowTBDiskFull;
	extern bool bShowTBInvalidParam;
	extern bool bShowTBNoHostCredit;
	extern bool bShowTBNoWorkingKey;
	extern bool bShowTBNotInitialized;
	extern bool bShowTBNoHost;
	extern bool bShowTBFunctionNotSupported;
	extern bool bShowTBUnexpected;
	extern bool bShowTBItemFailed;
	extern bool bShowTBCardDataMissing;
	extern bool bShowTBOther;
	// TAR #200002 HC 032602 End

	//Tar 122881 Don't shutdown the scotapp if there is a TB error. If TB thinks the
	// transaction can not be continued, it will post message to tell SCOT to
	// cancel transaction or shutdown scotapp
	if (rc != TB_SUCCESS && rc != TB_SUCCESS_IMMEDIATE)
	{
		extern void GetTBReturnCodeName(TBRC nTBRetCode, CString & csCodeName);
		CString csRetCode;
		GetTBReturnCodeName(rc, csRetCode);
		trace(L3, _T("!!! Got an error return code from TB call, retCodeName=%s"), csRetCode);
		// TAR #200002 HC 032602 Start
		// For testing/debugging, show a pop up message if return code from TB
		// is not TB_SUCCESS or TB_SUCCESS_IMMEDIATE if bShowTBErrors is true
		// and boolean value for TB error is also true
		if (bShowTBErrors)
		{
			csErrorMsg = _T("THIS SHOULD ONLY BE SHOWN FOR DEBUGGING/TESTING PURPOSES!\n\n");
			csErrorMsg += _T("To disable, set registry entry ");
			csErrorMsg += _T("HKLM\\SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\TBReturnCodes\\AMasterKey_ShowMsgOnNonSuccess");
			csErrorMsg += _T(" = 0.\n\nTransaction Broker returned ");
			csErrorMsg.Format(csErrorMsg + _T(" %d or %s!"), rc, csRetCode);
			csErrorMsg += _T("\nYou should look in traces.log to determine which ");
			csErrorMsg += _T("TB function was called that returned this value.");
			csErrorMsg += _T("\n\nPlease press _T('OK') before continuing.");
		}
		// TAR #200002 HC 032602 End

	}
	switch (rc)
	{
		case TB_E_INVALIDUSERID:
			ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_SM_TB_USERID);

		 // TAR #200002 HC 032602 Start
		 if (bShowTBErrors && bShowTBInvalidUserID)
		 {
			CWnd* pWndFL = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);
			::MessageBox(pWndFL->GetSafeHwnd(), csErrorMsg, _T("TB Return Code Error"),
						  MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);
		 }
		 // TAR #200002 HC 032602 End
			break;
		case TB_E_INVALIDUSERPASSWORD:
			ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_SM_TB_PASSWORD);

		 // TAR #200002 HC 032602 Start
		 if (bShowTBErrors && bShowTBInvalidUserPW)
		 {
			CWnd* pWndFL = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);
			::MessageBox(pWndFL->GetSafeHwnd(), csErrorMsg, _T("TB Return Code Error"),
						  MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);
		 }
		 // TAR #200002 HC 032602 End
			break;
		case TB_E_INVALIDSTATE:
			ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_SM_TB_STATE);

		 // TAR #200002 HC 032602 Start
		 if (bShowTBErrors && bShowTBInvalidState)
		 {
			CWnd* pWndFL = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);
			::MessageBox(pWndFL->GetSafeHwnd(), csErrorMsg, _T("TB Return Code Error"),
						  MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);
		 }
		 // TAR #200002 HC 032602 End
			break;
		case TB_E_FILENOTFOUND:
			ScotError(RETURN,SM_CAT_FILE_IO, SCOTAPP_TB_FILENOTFOUND);

		 // TAR #200002 HC 032602 Start
		 if (bShowTBErrors && bShowTBFileNotFound)
		 {
			CWnd* pWndFL = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);
			::MessageBox(pWndFL->GetSafeHwnd(), csErrorMsg, _T("TB Return Code Error"),
						  MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);
		 }
		 // TAR #200002 HC 032602 End
			break;
		case TB_E_INCOMPLETEPROCEDURE:
			ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_SM_TB_PROC);

		 // TAR #200002 HC 032602 Start
		 if (bShowTBErrors && bShowTBIncompleteProcedure)
		 {
			CWnd* pWndFL = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);
			::MessageBox(pWndFL->GetSafeHwnd(), csErrorMsg, _T("TB Return Code Error"),
						  MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);
		 }
		 // TAR #200002 HC 032602 End
			break;
		case TB_E_INVALIDWINDOW:
			ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_SM_TB_WINDOW);

		 // TAR #200002 HC 032602 Start
		 if (bShowTBErrors && bShowTBInvalidWindow)
		 {
			CWnd* pWndFL = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);
			::MessageBox(pWndFL->GetSafeHwnd(), csErrorMsg, _T("TB Return Code Error"),
						  MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);
		 }
		 // TAR #200002 HC 032602 End
			break;
		case TB_E_DISKFULL:
			ScotError(RETURN,SM_CAT_RESOURCE, SCOTAPP_SM_TB_DISK);

		 // TAR #200002 HC 032602 Start
		 if (bShowTBErrors && bShowTBDiskFull)
		 {
			CWnd* pWndFL = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);
			::MessageBox(pWndFL->GetSafeHwnd(), csErrorMsg, _T("TB Return Code Error"),
						  MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);
		 }
		 // TAR #200002 HC 032602 End
			break;
		case TB_E_INVALIDPARAM:
			ScotError(RETURN,SM_CAT_INPUT_PARAMETER, SCOTAPP_SM_TB_PARA);

		 // TAR #200002 HC 032602 Start
		 if (bShowTBErrors && bShowTBInvalidParam)
		 {
			CWnd* pWndFL = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);
			::MessageBox(pWndFL->GetSafeHwnd(), csErrorMsg, _T("TB Return Code Error"),
						  MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);
		 }
		 // TAR #200002 HC 032602 End
			break;
		case TB_E_NOHOSTCREDIT:
			ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SM_TB_CREDIT);

		 // TAR #200002 HC 032602 Start
		 if (bShowTBErrors && bShowTBNoHostCredit)
		 {
			CWnd* pWndFL = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);
			::MessageBox(pWndFL->GetSafeHwnd(), csErrorMsg, _T("TB Return Code Error"),
						  MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);
		 }
		 // TAR #200002 HC 032602 End
			break;
		case TB_E_NOWORKINGKEY:
			ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_SM_TB_WORKINGKEY);

		 // TAR #200002 HC 032602 Start
		 if (bShowTBErrors && bShowTBNoWorkingKey)
		 {
			CWnd* pWndFL = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);
			::MessageBox(pWndFL->GetSafeHwnd(), csErrorMsg, _T("TB Return Code Error"),
						  MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);
		 }
		 // TAR #200002 HC 032602 End
			break;
		case TB_E_NOTINITIALIZED:
			ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_SM_TB_NOTINIT);

		 // TAR #200002 HC 032602 Start
		 if (bShowTBErrors && bShowTBNotInitialized)
		 {
			CWnd* pWndFL = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);
			::MessageBox(pWndFL->GetSafeHwnd(), csErrorMsg, _T("TB Return Code Error"),
						  MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);
		 }
		 // TAR #200002 HC 032602 End
			break;
		case TB_E_NOHOST:
			ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_SM_TB_HOST);

		 // TAR #200002 HC 032602 Start
		 if (bShowTBErrors && bShowTBNoHost)
		 {
			CWnd* pWndFL = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);
			::MessageBox(pWndFL->GetSafeHwnd(), csErrorMsg, _T("TB Return Code Error"),
						  MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);
		 }
		 // TAR #200002 HC 032602 End
			break;
		 // TAR #200002 HC 032602 Start
	 case TB_E_ITEMFAILED:
		 ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_SM_TB_OTHER);

		 if (bShowTBErrors && bShowTBItemFailed)
		 {
			CWnd* pWndFL = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);
			::MessageBox(pWndFL->GetSafeHwnd(), csErrorMsg, _T("TB Return Code Error"),
						  MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);
		 }
		 break;
	 case TB_E_CARDDATA_MISSING:
		 ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_SM_TB_OTHER);

		 if (bShowTBErrors && bShowTBCardDataMissing)
		 {
			CWnd* pWndFL = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);
			::MessageBox(pWndFL->GetSafeHwnd(), csErrorMsg, _T("TB Return Code Error"),
						  MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);
		 }
		 break;
		 // TAR #200002 HC 032602 End

		// Unsupported functions are sort of like success, because they
		// should not cause logs of errors and they are not mandatory.
		// we will blow off the "Fatal" log here and will assume that the
		// function in question will log a warning ONCE about the problem.
		// TAR# 136406 - 03/22/2000 - ncr_barn
		case TB_E_FUNCTIONNOTSUPPORTED:
		 // TAR #200002 HC 032602 Start
		 if (bShowTBErrors && bShowTBFunctionNotSupported)
		 {
			CWnd* pWndFL = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);
			::MessageBox(pWndFL->GetSafeHwnd(), csErrorMsg, _T("TB Return Code Error"),
						  MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);
		 }
		 // TAR #200002 HC 032602 End
		case TB_SUCCESS:			//success
		case TB_SUCCESS_IMMEDIATE:	//success immediate
			break;
		case TB_E_UNEXPECTED:

		 // TAR #200002 HC 032602 Start
		 ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_SM_TB_OTHER);

		 if (bShowTBErrors && bShowTBUnexpected)
		 {
			CWnd* pWndFL = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);
			::MessageBox(pWndFL->GetSafeHwnd(), csErrorMsg, _T("TB Return Code Error"),
						  MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);
		 }
		 break;
		 // TAR #200002 HC 032602 End
		default:
			 ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_SM_TB_OTHER);

		 // TAR #200002 HC 032602 Start
		 if (bShowTBErrors && bShowTBOther)
		 {
			CWnd* pWndFL = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);
			::MessageBox(pWndFL->GetSafeHwnd(), csErrorMsg, _T("TB Return Code Error"),
						  MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);
		 }
		 // TAR #200002 HC 032602 End
	}
}


////////////////////////////////////
static void ShowError(const long rc,LPCTSTR szText)
{
  CString csWork;
  csWork.Format(_T("Problem with TB Application (%d) %s"),rc,szText);
  if (h2X20) h2X20->SetWindowText(csWork);
  ps.Echo(csWork);
}

long SMStateBase::TBRefreshState(void)
{
	rc=tb.RefreshState();
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO , _T("TBRefreshState")) ;
	ProcessTBReturn(rc);
	return rc;				  // allow re-display
}

/////////////////////////////////
SMStateBase *SMStateBase::TBIdleTimer(void)
{
	// app can use this call to do anything related to timing other than state TimedOut()
	// adjust ISIP timer
#define ONE_SECOND 1000
	if (!nISIPTimeOut)	// no ISIP timer started
		return STATE_NULL;
	int nISIPCurrentTimeOutValue = GetISIPTimeOut() - ONE_SECOND;
	if (nISIPCurrentTimeOutValue < 0)
	{
		nISIPCurrentTimeOutValue = 0;
	}
	return SetISIPTimeOut(nISIPCurrentTimeOutValue);
}


/////////////////////////////////
//RFC 262343 - start
void SMStateBase::TenderComplete(void)
{
       fTenderPending = false;
       //Log "healthy" message for Tendering
       eo.Error(INFO,SM_CAT_STARTUP_INIT, SCOTAPP_TENDER_COMPLETE, (long) (NCRSM_SNMP_ACTION | NCRSM_REMOTE_LOG_ACTION));

}
//RFC 262343 - end


/*///////////////////////////////////////////////////////////////
// Signature require or not
bool SMStateBase::TenderRequireSignature(void)
{
	if(((nTenderType != TB_TENDER_CREDIT) && (nTenderType != TB_TENDER_DEBIT) &&
		(nTenderType != TB_TENDER_DEBITCHECKING) && (nTenderType != TB_TENDER_DEBITSAVING)) ||
	   ((nTenderType == TB_TENDER_CREDIT) && !co.m_bOperationsSignatureCapture && !co.m_bOperationsVerifySignature) ||
	   (((nTenderType == TB_TENDER_DEBIT) || (nTenderType == TB_TENDER_DEBITCHECKING) || (nTenderType == TB_TENDER_DEBITSAVING)) &&
	   !co.m_bOperationsDebitNeedsSignatureCapture && !co.m_bOperationsDebitNeedsVerifySignature))
	   return true;
	else
	   return false;
}*/
///////////////////////////////////////////////////
// intercept some inputs here since they could come at any time
// from any state this common handling here ensures they
// are handled and allow the handler to be coded once instead of
// in every state

/////////////////////////////////////////////////
SMStateBase *SMStateBase::TBParse(MessageElement *me)
{
	CString csDesc, csAnchorStateName, csInstruction;
	SMStateBase *sUser = STATE_NULL;
	CString csCSN;	//TAR 212001 DE & BZ 081402
	CKeyValueParmList parmList;
	BSTR bstrResult;

	//io.d.fDetails = false;	// 410299

    if (me->nDeviceID == TB_DEV_UICOMMAND)
    {
        // tb needs FastLane to execute UI requests that are detailed in the message
        // Execute UI requests from TB first
        OnExternalUIRequest(me);
    }
	if(me->nDeviceID == TB_DEV_DISPLAYDATANEEDED)	// RFC197844 TB DataEntry screens
	{
		CString csTBState = (LPTSTR) me->pData;
		nTBLastState = (TBSTATE) (_ttol(csTBState));
		csLastDisplay = _T("");
	}
	else if(TBIsAllowed(TB_FT_QUICKITEMENTRY) && (me->nDeviceID == TB_DEV_DISPLAYPRIMARY) && (me->lParam != NULL))
	{
		CString csTBState = (LPTSTR) me->pData;
		nTBLastState = (TBSTATE) (_ttol(csTBState));
		if(nTBLastState != TB_MESSAGESCREEN && nTBLastState != TB_SENDMESSAGETOSCOT && nTBLastState != TB_ITEMUNKNOWN) //TAR 182862
		{
			// Get Release information from registry
			SItemDetails* pItemDetail = (SItemDetails*) me->lParam;
			SetItemDetails(pItemDetail, io.d);
		}
		csLastDisplay.Empty();
	}
	else
	{
		csLastDisplay = (LPTSTR)me->pData;

//USSF Start
		USSFDATA(_T("POSDISPLAY"), csLastDisplay);
//USSF End

//USSF Start
		CString csTempstr;
		csTempstr.Format(_T("%d"), me->tbInfo.nDeviceID);
		USSFDATA(_T("POSDEVICE"), csTempstr);
//USSF End

//USSF Start
		// Check for Fake Message Indicating Generic POS Action
		int iPos = csLastDisplay.Find(_T("$POSDATA$"));
		if (iPos < 0)
		{
			iPos = csLastDisplay.Find(_T("+POSDATA+"));
		}
		//if ((iPos = csLastDisplay.Find(_T("$POSDATA$")) >= 0)||
		//	(iPos = csLastDisplay.Find(_T("+POSDATA+")) >= 0))
		if (iPos >= 0)
		{
			//csLastDisplay.MakeUpper();
			// Need to set transaction variables.
			// Parse the data.
			// STATES:
			//	0	Initial 
			//	1	Building Name
			//	2	In Between Name/Value.
			//	3	Building Value
			//	8	Accept
			//	9	Error
			CString csPOSDataName, csPOSDataValue;
			int iState = 0;	// Initial.
			//int i = iPos + (sizeof _T("$POSDATA$") - 1);
			int i = iPos + 9;
			while(i < csLastDisplay.GetLength())
			{
				switch(iState)
				{
				case 0:	// Initial - building identifier.
					switch(csLastDisplay[i])
					{
					case _T(' '):
						i++;
						break;
					case _T('='):
						iState = 9;
						break;
					default:
						iState = 1;
						csPOSDataName = csLastDisplay[i];
						i++;
						break;
					}
					break;
				case 1:	// Building Name.
					switch(csLastDisplay[i])
					{
					case _T(' '):
						iState = 2;
						i++;
						break;
					case _T('='):
						iState = 3;
						i++;
						break;
					default:
						csPOSDataName += csLastDisplay[i];
						i++;
						break;
					}
					break;
				case 2:	// Between Name and Value.
					switch(csLastDisplay[i])
					{
					case _T('='):
						iState = 3;
						i++;
						break;
					case _T(' '):
						i++;
						break;
					default:
						iState = 3;
						break;
					}
					break;
				case 3:	// Building Value.
					switch(csLastDisplay[i])
					{
					case _T(';'):
						iState = 8;
						break;
					default:
						csPOSDataValue += csLastDisplay[i];
						i++;
						break;
					}
					break;
				}
			}
			// Check State.
			//csLastDisplay = _T("");
			if ((iState == 3) || (iState == 8))
			{
				if ((csPOSDataName.GetLength() > 0) &&
				   (csPOSDataValue.GetLength() > 0))
				{
					APPLDATA(csPOSDataName, csPOSDataValue);
					return STATE_NULL;
				}
				else
				{
					trace(L5, _T("$POSDATA$ NULL Name and/or Value: <%s>."), csLastDisplay);
					return STATE_NULL;
				}
			}
			else
			{
				trace(L5, _T("$POSDATA$ Parse Error: <%s>."), csLastDisplay);
				return STATE_NULL;
			}
		}
		// To be Handled as USSF Event.
		//Parse csDisplay
		iPos = csLastDisplay.Find(_T("$POSEVENT$"));
		if (iPos < 0)
		{
			iPos = csLastDisplay.Find(_T("+POSEVENT+"));
		}
		//if (int iPos = csLastDisplay.Find(_T("$POSEVENT$")) >= 0)
		if (iPos >= 0)
		{
			csLastDisplay.MakeUpper();
			// Need to set transaction variables.
			// Parse the data.
			// STATES:
			//	0	Initial
			//	1	Building Name
			//  2	Finished Building Name
			//	8	Accept
			//	9	Error
			CString csPOSEventName;
			int iState = 0;	// Initial.
			//int i = iPos + (sizeof _T("$POSEVENT$") - 1);
			//int i = iPos + 9 * sizeof TCHAR;
			int i = iPos + 10;
			while(i < csLastDisplay.GetLength())
			{
				switch(iState)
				{
				case 0:	// Initial - building identifier.
					switch(csLastDisplay[i])
					{
					case _T(' '):
						i++;
						break;
					case _T('='):
						iState = 9;
						break;
					default:
						iState = 1;
						csPOSEventName = csLastDisplay[i];
						i++;
						break;
					}
					break;
				case 1:	// Building Name.
					switch(csLastDisplay[i])
					{
					case _T(' '):
						iState = 2;
						i++;
						break;
					case _T(';'):
						iState = 8;
						break;
					default:
						csPOSEventName += csLastDisplay[i];
						i++;
						break;
					}
					break;
				case 2:	// Finished Building Name.
					switch(csLastDisplay[i])
					{
					case _T(' '):
						i++;
						break;
					case _T(';'):
						iState = 8;
						break;
					default:
						iState = 9;
						break;
					}
					break;
				}
			}
			//csLastDisplay = _T("");
			// Check State.
			if ((iState == 1) || (iState == 2) || (iState == 8))
			{
				if (csPOSEventName.GetLength() > 0)
				{
					USSFDATA(_T("POSEVENT"), csPOSEventName);
					USSFHOOK(csPOSEventName, nextState = STATE_NULL, return nextState);

				}
				else
				{
					trace(L5, _T("$POSEVENT$ NULL Event: <%s>."), csLastDisplay);
					return STATE_NULL;
				}
			}
			else
			{
				trace(L5, _T("$POSEVENT$ Parse Error: <%s>."), csLastDisplay);
				return STATE_NULL;
			}
		}
//USSF End

		if (h2X20)												// update the SCOT dialog window
		{
			if (_tcsclen(csLastDisplay))
			h2X20->SetWindowText(CleanUpMessage(csLastDisplay,true));
		}
	//TAR402091+ take out the try/catch for TBProcessMessage()
		//try 
		//{
//USSF Start
			USSFHOOK(_T("POSDisplay"), nTBLastState = TBProcessMessage(me), ;);
			//nTBLastState = TBProcessMessage(me);
//USSF End
		   csLastDisplay = (LPTSTR)me->pData;  // In case it changed
		//}
		   /*
		catch (CException *e)
		{
			_TCHAR szError[255] = {NULL};
			e->GetErrorMessage( szError, 255 );
			ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_GOT_TBEXCEPTION, _T("%s|%s"), _T("TBProcessMessage()"), szError);
			e->Delete();
		}
		catch (...)
		{
			ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_GOT_TBEXCEPTION, _T("%s|%s"),_T("TBProcessMessage()"), _T("Unknown exception"));
		}
		*/
		//TAR402091-
	}
	// TAR 123183 - Call Show2x20 always if in Assist mode - changes for Albertsons.
	if ((me->nDeviceID == TB_DEV_DISPLAYIMMEDIATE) || (fStateAssist))
	{
		//TAR339619
		if (bIsCoupon)
			TBShow2x20(ps.GetPSText(MSG_COUPONNOTACCEPTED));
		else
			TBShow2x20();
	}
	if ((co.fUserExitAllEnabled) &&   // if exits enabled and
		(co.fUserExit01Enabled))		//	this exit is enabled
	{
		sUser = UserExit01();
		if (sUser != STATE_NULL)
		return sUser;
	}

	// remember the last "real" SA state, such as TB_CLOSED, etc. for later

	if (nTBLastState != TB_UNKNOWN && nTBLastState != TB_IGNORE && nTBLastState != TB_TENDERVOIDED) //RFC 275377 MIGRATED-BM185018
	{
		nTBLastRealState = nTBLastState;
		// want to know which state is it?
		CString csTBStateName;
		extern void GetTBReturnStateName(TBRC nTBState, CString & csStateName); // defined in CTransBroker.cpp
		GetTBReturnStateName(nTBLastRealState, csTBStateName);
		trace(L3, _T("!!!! TB State id=%d, name:%s"), nTBLastRealState, csTBStateName);
	}

	//bool bCompletePrintAndCut = false;
	bCompletePrintAndCut = false; // TAR_325292
	bool bSaveReceipt = true;

	switch (nTBLastState)
	{
	case TB_IGNORE:
	  {
		return STATE_NULL;
	  }
   case TB_NEEDMICR:
	  {
	  trace(L7, _T("SMStateBase::TBParse - 0x%08.8x <%s>"), nTBLastState, _T("TB_NEEDMICR"));
	  return OnNeedMICR();
	  break;
	  }
   case TB_NEWSECURITYLEVEL:
	  {
	  trace(L7, _T("SMStateBase::TBParse - 0x%08.8x <%s>"), nTBLastState, _T("TB_NEWSECURITYLEVEL"));
	  return OnNewSecurityLevel();
	  break;
	  }
   case TB_NEWCASHDRAWERSTATE:
	  {
	  trace(L7, _T("SMStateBase::TBParse - 0x%08.8x <%s>"), nTBLastState, _T("TB_NEWCASHDRAWERSTATE"));
	  return OnNewCashDrawerState();
	  break;
	  }
   case TB_ENDORSEDOCUMENT:
	  {
	  trace(L7, _T("SMStateBase::TBParse - 0x%08.8x <%s>"), nTBLastState, _T("TB_ENDORSEDOCUMENT"));
	  return OnEndorseDocument();
	  break;
	  }
   case TB_ASSISTMODESTATUSCHANGED:
	  {
	  trace(L7, _T("SMStateBase::TBParse - 0x%08.8x <%s>"), nTBLastState, _T("TB_ASSISTMODESTATUSCHANGED"));
	  return OnAssistModeStatusChanged(); //tar 243863
	  }

	case TB_REPRINTRECEIPT:
		bSaveReceipt = false;
	case TB_COMPLETEPRINTANDCUT:   //Synchronous printing
		bCompletePrintAndCut = true; // fall to TB_COMPLETEPRINT case now, no duplicate code
		// don't break, let it fall to the TB_COMPLETEPRINT case
	case TB_COMPLETEPRINT:	 //Synchronous printing
	{
		CString csLReceiptTmpFile = LRECEIPT_TMP_FILE;
		CString csLReceiptFile = LRECEIPT_FILE;
		GET_PARTITION_PATH(LRECEIPT_FILE, csLReceiptFile.GetBuffer(_MAX_PATH));
	    GET_PARTITION_PATH(LRECEIPT_TMP_FILE, csLReceiptTmpFile.GetBuffer(_MAX_PATH));
		csLReceiptFile.ReleaseBuffer();
		csLReceiptTmpFile.ReleaseBuffer();
		if (FileExist(csLReceiptTmpFile))
		{
			RemoveFile(csLReceiptFile);
			CFile::Rename(csLReceiptTmpFile, csLReceiptFile);
		}

		HFILE hFile = TBGetReceiptData();
		if (hFile!=NULL)
		{
            if (NULL != hReceiptFile)
            {
                trace(L6, _T("ASYNCHRONOUS PRINT ISSUED, POSSIBLE RECEIPT FILE HANDLE LEAK"));
            }
			hReceiptFile = hFile;

			CString csSlipFile = SLIP_FILE;
			GET_PARTITION_PATH(SLIP_FILE, csSlipFile.GetBuffer(_MAX_PATH));
			csSlipFile.ReleaseBuffer();

			RemoveFile(csSlipFile); //for Print Last Receipt module

			PrintReceipt(hFile, bSaveReceipt); // print receipt

			//+TAR415732
			if (bCompletePrintAndCut) 
			{
				CString csCutSequence = "\x1B|50fP";
				SaveLastReceiptData(csLReceiptTmpFile, csCutSequence);
			}
			//-TAR415732

			if ( nTrxReceiptLength && (bCompletePrintAndCut || !bSaveReceipt) )
			{
				bCompletePrintAndCut = false;
				CutReceipt();		// cut receipt
				if (fStateAssist)
				{
					nTrxReceiptLength = 0; // don't show take receipt
				}
			}
			//TBReleaseReceiptData(hFile); // For synchronous printing, need make sure
			// printing is finished. This release call is removed to
			// SMStateDM. After success print, then release receipt file
		}

		hFile = TBGetJournalData();
		if (hFile!=NULL)
		{
			hJournalFile = hFile;
			PrintJournal(hFile);	// print Journal
			TBReleaseJournalData(hFile);  // For journal, don't need wait for print success
		}
	}
	if (!bSaveReceipt && getAnchorState() == BES_SMPRINTRECEIPT)
	{
		RETURNSTATE(SmPrintReceipt);
	}
	return STATE_NULL;

   case TB_REPRINTRECEIPTFAILED:
	  bReprintReceiptFailed = true;
	  RETURNSTATE(SmPrintReceipt);

  case	TB_OPTIONLOADING:
		if (fInSignedOn || fInSignedOff || fStateAssist || fSAInTransaction)
		{
			// You are in the middle of a transaction, initialize all at the end of it
			fDoOptions = true;
			//ps.Echo(CleanUpMessage(szLastDisplay,true));
			return STATE_NULL;
		}
		else
		{
			// outside of a transaction, no active transaction in progress
			// Sleep(100);
			m_bReloadingOptions = true; //tar 352145
			SMStateBase* pStartState = ReloadOptions();
			m_bReloadingOptions = false;
			if (pStartState)
			{
			   return pStartState;
			}
			else
			{
				ASSERT(0);
				return STATE_RESTART;
			}
		}
		break;
  case TB_CLOSED:				   // at ****** CLOSED ******
		if(!fInSignedOff)	//Pham, Thanhhieu T
			return STATE_NULL;
		ps.Echo(PS_BLANK);			  // clear messages
		fInSignedOn = false;			// must be signed off
		fInSignedOff = false;
		if (fStateAssist)
		{
			fStateAssist = false;
			trace(L0, _T("Finishing trx while in Assistmode....."));
			//TAR236935 CHU 052003
			//If We are leaving assist mode becuase EOT in AssistMode
			//We need to tell TAB to leave AssistMode
			if (dm.fStateTakeawayBelt)
			{
				parmList.Clear();
   				CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_ASSIST_MODE_STOP);
				_bstr_t bstrParmList((const _TCHAR *)parmList);
				bstrResult = NULL;
				SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
			}
		}

//		TAR#125992 - don't set anchor state based on TB_CLOSED. If we get an error (such
//		as coin jam) after TB_CLOSED, we want to return to the system message, NOT Attract.
//		setAnchorState(BES_START);

		//NN RFC 210177
		if (bWriteEndOfTransactionToWLDBlog)
		   strace(L0, _T("End of transaction"));
		bWriteEndOfTransactionToWLDBlog = false;  //set the flag to false
		//RFC 210177 end

	  //Begin RAP Dispense Change
	  if(lChangeDue && !co.fTenderDispenseChangeAtLaneForRemoteTendering && ps.RemoteMode() ) //Remote AssistMode Tendering
	  {
		   dm.CashDrawerOpen();  //Pop up cash drawer to let cashier giving change to customers.
		   m_bWaitForCashDrawerClose = true;
		   //reporting
		   rp.RAPDispenseChange(lChangeDue);
		   lChangeDue = 0;    //Do not dispense Change at Lane
		   nTimeOut = 5;
           trace(L6, _T("Cash Drawer is still open at remote assitmode, wait until it is closed"));

			return STATE_NULL;
		}
	  //End RAP Dispense Change
	  else
	  {
		  return TBEndOfTransaction();

	  }

  case TB_CLOSED_NOPRINTANDNODISPENSE:
		if(!fInSignedOff)	//Pham, Thanhhieu T
			return STATE_NULL;

		ps.Echo(PS_BLANK);			  // clear messages
		fInSignedOn = false;			// must be signed off
		fInSignedOff = false;
		return setAndCreateAnchorState(BES_START);

  case TB_READY:				// at SELECT PROCEDUURE
		// we may get TB_ENTERITEM while waiting
		// for sign on or for an item sale, eg in progress...
		// in any case, if inProgress() then exit in progress
		// by returning createAnchorState()
		if (fInSignedOn || fInSignedOff)
		{
			//Tar 124345
			return STATE_NULL;

			// we may have just entered a PREFERRED CUSTOMER card
			// and any subsequent items are likely EM auto linked
			// coupon items so flag them as such, no harm done here
			//CString csWork = LINK_ITEM;
			//io.csOrigCode = csWork + io.csOrigCode;
			//io.csItemCode = csWork + io.csItemCode;
		}

		fInSignedOn  = true;		 // must be signed on by now
		fInSignedOff = false;
		if (TrainingModeObj.IsTMOn())
		{
		   //T.D 04/30/02 TAR 202117
		   // TAR 248037 - Need to remove the ***, there is no room on RAP
	       CString csRAPReceiptLine = ps.GetPSText(MSG_TRAINING_MODE_LINE,SCOT_LANGUAGE_PRIMARY);
           int i = csRAPReceiptLine.FindOneOf(_T("***"));
	       csRAPReceiptLine = csRAPReceiptLine.Right(csRAPReceiptLine.GetLength() - (i+3));
           csRAPReceiptLine.TrimLeft();
           i = csRAPReceiptLine.FindOneOf(_T("***"));
	       csRAPReceiptLine = csRAPReceiptLine.Left(csRAPReceiptLine.GetLength() - (csRAPReceiptLine.GetLength()-i+1));
           csRAPReceiptLine.TrimRight();
	       csRAPReceiptLine = RAP_RECEIPT_LINE_SUMMARY + csRAPReceiptLine;

	       trace(L7, _T("+ra.OnNormalItem, Message=%s"), csRAPReceiptLine);
	       ra.OnNormalItem(csRAPReceiptLine);
	       trace(L7, _T("-ra.OnNormalItem"));
		}

		// if in inprogress of a possible loyalty card, leave the state in its current state
		if (fInLoyaltyCardProcessing)
		{
			return STATE_NULL;
		}

		if(fLoyaltyCardAwaitVerification)
		{
			fLoyaltyCardAwaitVerification = false;

		    TBLOYALTYCARDTYPE nLCProcessState = tb.VerifyLoyaltyCard(cDMLastTrack,nDMLastTrackLen);
            //TAR 214441
            CString strFmt ;
	        strFmt =_T("tb.VerifyLoyaltyCard--cDMLastTrack:%s; nDMLastTrackLen:%d") ;
	        traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt, cDMLastTrack, nDMLastTrackLen) ;

            switch (nLCProcessState)
            {
                case LC_CARDDATA_MISSING:
                    {
                        // card data is not complete w.r.t TB
                        // handle it is a card-misread
                        if (co.fTenderIsMotorizedMSR)
                        {
                            CREATE_AND_DISPENSE(CardMisRead)(MSG_MISREAD_MCRW, LTT_CARDMISREAD);
                        }
                        else
                        {
                            CREATE_AND_DISPENSE(CardMisRead)(MSG_MISREAD_SLIDE, LTT_CARDMISREAD);
                        }
                    }
                break;
                case LC_INVALID:
                    {
                        // inserted  card is not a loyalty card
                        // let current state determine what to do
						// +Fix TAR246479
						// fInLoyaltyCardProcessing = true; //TAR235030 CHU 042103
						trace(L0, _T("TB: NOT A LOYALTY CARD"));
						fInLoyaltyCardProcessing =false;
						// -Fix TAR246479
                    }
				break;
                case LC_VALID:
                    {
			            fInLoyaltyCardProcessing = true;
                        RETURNSTATE(CardProcessing)
                    }
                break;
                case LC_INSERTED_BEFORE:
                    {
						CREATE_AND_DISPENSE(TakeLoyaltyCard)(LC_INSERTED_BEFORE);
                    }
                break;
            }

		}

		if(fStateAssist && fStartTrxInAM)
		{	// RFC 228102 Initiate transactions that will require hand scanning at the RAP
            trace(L6, _T("Start Transaction in AM, TB_READY received -- update keyboard"));
			TBLoadAssistModeKeyboard();
	        fStartTrxInAM = false;
		}

		//tar 211789 - AH and DE 08/15/02
		//go to SecUnExpectedIncrease screen if quiet mode
		if (bQuietNeedSecApproval && !fStateAssist)
		{
			 CREATE_AND_DISPENSE(SecUnExpectedIncrease)(BES_SCANANDBAG, BES_SCANANDBAG);
		}

		// if item is scanned from attract, wait to go to proper sale screen
		if (fItemIsWaiting)
		{
			//+degraded mode tars
			if( co.IsDegradedModeOn() &&
				dm.GetDegradedModeType() != DEGRADEDMODE_OFF &&
				getAnchorState() == BES_CMDATAENTRY )
			{
				RETURNSTATE(DegradedContinueTrans)
			}
			//-degraded mode tars
			
			// this might cause problems with new InSAProgress LPM
			setAnchorState(BES_SCANANDBAG);
			SMStateBase* retState = TBItemSale(false,0,true);

			// go to inprogress
			return retState;
		}

		//TAR 212001 DE & BZ 081402 begin
		try
		{
			csCSN = this->GetRuntimeClass()->m_lpszClassName;
		}
		catch(...){};
		if(csCSN.Find(_T("SecUnExpectedIncrease")) != -1)
			return STATE_NULL;
		//TAR 212001 end

		//395551+
		if(!co.fOperationsCustomerBagAllowed ||  (getAnchorState() != BES_PUTBAGONSCALE && g_savedAnchorStateFromAttract != BES_PUTBAGONSCALE )  ) //416582
		{
			//+degraded mode tars
			if( co.IsDegradedModeOn() &&
				dm.GetDegradedModeType() != DEGRADEDMODE_OFF &&
				getAnchorState() == BES_CMDATAENTRY )
			{
				RETURNSTATE(DegradedContinueTrans)
			}
			//-degraded mode tars
			return setAndCreateAnchorState(BES_SCANANDBAG); // first state is S&B
		}
		else
		{
			//+degraded mode tars
			if( co.IsDegradedModeOn() && dm.GetDegradedModeType() != DEGRADEDMODE_OFF )
			{
				RETURNSTATE(DegradedContinueTrans)
			}
			//-degraded mode tars
			 
			return setAndCreateAnchorState(BES_PUTBAGONSCALE);
		}
		//395551-

  case TB_NEEDWEIGHT:
		// even though there is no item sale in progress, TB can send TB_NEEDWEIGHT
		// in assist mode to get current stable weight
		// If Takeaway Belt, stop the belts on entry to this screen and TAB waits 2 seconds before requesting scale status. LPM Chu 204277
	    if(dm.fStateTakeawayBelt)
		{
			parmList.Clear();
   			CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_SUSPEND_ITEM);
			_bstr_t bstrParmList((const _TCHAR *)parmList);
			bstrResult = NULL;
			SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
		}

		if (!fSAInItemSale && !fStateAssist && !co.IsInAssistMenus()) //RFQ 986
		{
			trace(L0, _T("Ignoring TB_NEEDWEIGHT because there is no item sale in progres"));
			return STATE_NULL;
		}
		fInSignedOn = true; 		 // must be signed on by now
		fInSignedOff = false;
		if (fSAInItemSale || co.IsInAssistMenus()) //RFQ 986
		{
			if(!TBIsAllowed(TB_FT_QUICKITEMENTRY))
				TBGetItemDetails(io.d);
			if (!io.fVoid)
			{
				//If restriced item is not allowed to sell, show the message.
				if (restrictedItemNotAllowed())
				{
					PSTEXT msgNum = restrictedItemMessage();
					if (msgNum)
					{
					  // It does no good to do this from Assist Mode - no store
					  // mode state should do it, for that matter.
					  if (storeMode() && !co.IsInAssistMenus())
						return STATE_NULL;

					  //Tar 123351
					  SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
					  lRestrictedNotAllowedItems++;
					  CREATE_AND_DISPENSE(RestrictedNotAllowed)(msgNum);
					}
				}
			}
			// If there is no scale, treat as unknown item.
			if (!dm.IsScaleEnable())
			{
				SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
				io.fNotForSale = true;
				lUnknownItems++;
				RETURNSTATE(UnknownItem)
			}
			if (lDMScaleWeight<0)
			{
			  if (!fStateAssist)
			  {
				if(!fInMultiSelectPickList) //407200
				{
					setAnchorState(BES_SCANANDBAG);
				}
				RETURNSTATE(ScaleBroken)
			  }
			  else
			  {
				  return STATE_NULL;
			  }
			}
		}
		if ((fStateAssist) && (SMStateBase::lDMScaleWeight > 0))
		{
			return DMScale();
		}
		if (!fStateAssist)
		{
			RETURNSTATE(EnterWeight)	   //SA #4
		}
		else
		{
		  return STATE_NULL;
		}
		break;

  case TB_NEEDWEIGHT_BIZERBA:

		if(!fSAInItemSale && !fStateAssist && !co.IsInAssistMenus()) //RFQ 986
		{
			trace(L0, _T("Ignoring TB_NEEDWEIGHT_BERZERBA because there is no item sale in progres"));
			return STATE_NULL;
		}

		fInSignedOn = true; 		 // must be signed on by now
		fInSignedOff = false;

		if (!fStateAssist)
		{
			io.bBizerbaScale = true;
			RETURNSTATE(EnterWeight)	//SA #4
		}
		else
		{
		  return STATE_NULL;
		}


	  break;

   case TB_CRATEABLEITEM:
		if (fSAInItemSale || co.IsInAssistMenus()) //RFQ 986
		{
			if(!TBIsAllowed(TB_FT_QUICKITEMENTRY))
				TBGetItemDetails(io.d);
			if (!io.fVoid)
			{
				//If restriced item is not allowed to sell, show the message.
				if (restrictedItemNotAllowed())
				{
					PSTEXT msgNum = restrictedItemMessage();
					if (msgNum)
					{
					  // It does no good to do this from Assist Mode - no store
					  // mode state should do it, for that matter.
					  if (storeMode() && !co.IsInAssistMenus())
						return STATE_NULL;

					  SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
					  lRestrictedNotAllowedItems++;
					  CREATE_AND_DISPENSE(RestrictedNotAllowed)(msgNum);
					}
				}
			}
		}
		if (!fStateAssist)
		{
			RETURNSTATE(CrateableItem)
		}
		else
		{
		  return STATE_NULL;
		}
		break;

  case TB_NEEDQUANTITY:
	    //TAR339619
		if (fStateAssist && co.csTenderAllowCoupons == _T("N") )
			bIsCoupon = false;

		if (!fSAInItemSale && !co.IsInAssistMenus()) //RFQ 986
		{
			trace(L0, _T("Ignoring TB_NEEDQUANTITY because there is no item sale in progres"));
			return STATE_NULL;
		}
		fInSignedOn = true; 		 // must be signed on by now
		fInSignedOff = false;
		if(!TBIsAllowed(TB_FT_QUICKITEMENTRY))
			TBGetItemDetails(io.d);
		if (!io.fVoid)
		{
			if (restrictedItemNotAllowed())
			{
				PSTEXT msgNum = restrictedItemMessage();
				if (msgNum)
				{
				  // It does no good to do this from Assist Mode - no store
				  // mode state should do it, for that matter.
				  if (storeMode() && !co.IsInAssistMenus())
					return STATE_NULL;
				  //Tar 123351
				  SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
				  lRestrictedNotAllowedItems++;
				  CREATE_AND_DISPENSE(RestrictedNotAllowed)(msgNum);
				}
			}
		}

		RETURNSTATE(EnterQuantity)

  case TB_NEEDPRICE:
		{
		if (!fSAInItemSale && !co.IsInAssistMenus()) //RFQ 986
		{
			trace(L0, _T("Ignoring TB_NEEDPRICE because there is no item sale in progres"));
			return STATE_NULL;
		}
		fInSignedOn = true; 		 // must be signed on by now
		fInSignedOff = false;
		if(!TBIsAllowed(TB_FT_QUICKITEMENTRY))
			TBGetItemDetails(io.d);

		if (!io.fVoid)
		{
			if (restrictedItemNotAllowed())
			{
				  PSTEXT msgNum = restrictedItemMessage();
				if (msgNum)
				{
				  // It does no good to do this from Assist Mode - no store
				  // mode state should do it, for that matter.
				  if (storeMode() && !co.IsInAssistMenus())
					return STATE_NULL;

				  //Tar 123351
				  SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
				  lRestrictedNotAllowedItems++;
				  CREATE_AND_DISPENSE(RestrictedNotAllowed)(msgNum);
				}
			}
		}

		if (lVoidPrc && io.fVoid)
		{
		  io.lPriceEntered = lVoidPrc;
		  return TBEnterItem();
		}
		if (io.d.fCoupon)
		{
			// If Takeaway Belt, stop the belts on entry to this screen and TAB waits 2 seconds before requesting scale status. LPM Chu 204277
			if(dm.fStateTakeawayBelt)
			{
				parmList.Clear();
   				CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_SUSPEND_ITEM);
				_bstr_t bstrParmList((const _TCHAR *)parmList);
				bstrResult = NULL;
				SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
			}
			//tar 206895 - AH 06/12/02
			if (co.csTenderAllowCoupons == _T("N") || (co.csTenderAllowCoupons == _T("O") && !fStateAssist && !co.IsInAssistMenus())) //tar 220882
			{
				RETURNSTATE(CouponNotAllowed)
			}
			else
			{
				RETURNSTATE(EnterCouponValue)
			}
		}

		if(fStateAssist)  //Tar 195180
		{
			trace(L0, _T("Cant change state since it it in Assistmode"));
			return STATE_NULL;
		}

		//TAR 249417
		CString sDescription, csTemp;
		csTemp = ps.GetPSText(MSG_NEED_PRICE, SCOT_LANGUAGE_PRIMARY) + _T(" ");
        //TAR 234316
		sDescription.Format(csTemp + io.d.csDescription);  // Tar 231913
		CREATE_AND_DISPENSE(RAPDataNeeded)(sDescription,DATANEEDED_PRICEPROMPT);
	}

  case TB_NEEDMOREDATA:
	  {
		trace(L7, _T("SMStateBase::TBParse - 0x%08.8x <%s>"), nTBLastState, _T("TB_NEEDMOREDATA"));
		g_bTBNeedMoreData = true;  // TAR 238061
//USSF START
		USSFHOOK(_T("TBXNeedMoreData"), nextState = OnTBNeedData(), return nextState;);
		//return OnTBNeedData();
//USSF END
	  }

  case TB_TOTAL:
	 {
//USSF START
		USSFHOOK(_T("TBXTotal"), nextState = TBGetTotalDetails();UpdateTotal(), return nextState;);
		//SMStateBase *rc = TBGetTotalDetails();
		//Tar 133857
		//UpdateTotal();
		//return rc;
//USSF END
	 }
	 break;

  case TB_SENDMESSAGETOSCOT:
	 {
		csTBMessage2Scot =	(LPCTSTR) (me->lParam); 	//csLastDisplay;
		if (storeMode())
			ps.ShowSMTBText(csTBMessage2Scot);
		else
			ps.ShowCMTBText(csTBMessage2Scot);
		trace(L7, _T("+ra.OnNormalItem, Message=%s"), csTBMessage2Scot);
		ra.OnNormalItem(_T("[Alert]") + ps.GetPSText(RA_POSMESSAGE, SCOT_LANGUAGE_PRIMARY) + _T(" ") + csTBMessage2Scot);
		trace(L7, _T("-ra.OnNormalItem"));
		return STATE_NULL;
	 }
	 break;


  case TB_ITEMSOLD:
	{
    //Begin Lane 3.1 Reporting
	//+TAR410994
	//int nTransNumber = TBGetCurrentTransactionNumber();
	//trace(L7, _T("+rp.SetTransactionNumber"));
	//CString csTransactionNumber;
	//csTransactionNumber.Format(_T("%d"), nTransNumber);
	//rp.SetTransactionNumber(csTransactionNumber);
	//trace(L7, _T("-rp.SetTransactionNumber"));
	//-TAR410994

    //+SOTF 6755
    if (!rp.IsTransactionInProgress()) // TAR 444337
	{
		trace(L7, _T("+rp.TransactionStart"));
		rp.RPBusinessDate();
		rp.TransactionStart(rp.m_csBusinessDate);
		trace(L7, _T("-rp.TransactionStart"));
	}
    //+SOTF 6755
	//End FastLane 3.1 Reporting

	if(TBIsAllowed(TB_FT_EXTENDEDDETAILS))
	{
		//get extended detail information on item
		TBGetItemExtendedDetails(io.xd);
		//do not process this type of exception item
		if(io.xd.ExceptionCode==XDONOTPROCESS)
		{
			SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
			lUnknownItems++;
            if (co.IsInAssistMenus())
            {
                SMSmAssistMenu::SetAssistMessage(LTT_UNKNOWNITEM, UNKNOWNITEM);
                RETURNSTATE(SmAssistMenu)
            }
            else
            {
                RETURNSTATE(UnknownItem)
            }
        }
	}

	fInSignedOn = true; 		 // must be signed on
	fInSignedOff = false;
	if(!TBIsAllowed(TB_FT_QUICKITEMENTRY))
		TBGetItemDetails(io.d);

	if(io.xd.ExceptionCode==XVISUALVERIFY)
	{
		  io.d.fVisualVerify=true;
		  /* // +TAR 298364
		  if (io.d.lExtPrice < 0 )
		  {
			SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
		  }// -TAR 298364 */
	}

	bool bIsLinkedItem = io.d.bLinkedItem;
	if(!bIsLinkedItem) // tar 192865
	{
		io.lQuantityEntered = io.d.lQtySold;
		io.lTotalWeightEntered = io.lWeightEntered;    //gross weight (including tare) (TAR 120968)
		io.lWeightEntered = io.d.lWgtSold;			   //net weight
	}
	io.fVoid = io.d.fVoid;
	io.lUnitPriceEntered = io.d.lUnitPrice;
	io.lExtPriceEntered = io.d.lExtPrice;		   // Tar 137876 add extended price
	io.csItemCode = io.d.csItemCode;

	//TAR339619
	if (fStateAssist && co.csTenderAllowCoupons == _T("N") )
	{
		csItemSent = io.csItemCode;
		if (io.IsCoupon())
		{
			bIsCoupon = true;
			RETURNSTATE(CouponNotAllowed)
		}
	}

	// if item has been sold without a call to TBItemSale in assistmode
	if ((fStateAssist || co.IsInAssistMenus()) && !fSAInItemSale) //RFQ 986
	{
		if (io.csOrigCode == _T("")) io.csOrigCode = io.csItemCode;
		if (!io.cScanType)
		{
			if (!bIsLinkedItem)
			{
				io.csOrigCode = io.csItemCode; // TAR : 127368
			}
			
			if (!isAnySoldVoidedItems()) //TAR389058: only tell SM KEY_TC_OPERATION_BEGIN if no item sold
			{
				SASendSCOTInput( ITEMIZATION );
			}
			if (io.csItemCode.GetLength() && !bIsLinkedItem)
          	trace(L5, _T("ITEM SOLD calling SASendSCOTInput with UPC_MANUAL"));
			SASendSCOTInput( UPC_MANUAL, io.csItemCode );
		}
	 }
	 TBGetTotalDetails();
	 int nTmpISIPTime = co.getTimeOutItemSaleInProgress(); // is option set to non-zero?

	 if (nTmpISIPTime && !bIsLinkedItem && !GetISIPTimeOut())
	 {
		 // Item Sale In Progress (ISIP) timer should start now
		 SetISIPTimeOut(nTmpISIPTime * 1000L);	// start timer
	 }

	// Report that a PickList item as been sold //
	if( ms_bIsCurrentItemFromPickList )
	{
        //TAR 264642 - csItemCode changed by POS, need to log orig
		rp.PickListItemSold( io.csOrigCode );
	}
	//////////////////////////////////////////////

	 SMStateBase *rc = TBItemDetails();
	 io.csItemCode=_T("");
     itemDetailOrig.Reset();
     itemDetailNew.Reset();

	 // I dont' think this should be here!	LPM030402
	 if (io.d.lRequireSecBagging == TB_OVERRIDE_SECURITY_WITH_NO)
	 {
		 trace(L2, _T("::TBParse;TB_ITEMSOLD - bEASReEnableScanner is set to true."));
		 bEASReEnableScanner = true;
	 }
#ifndef NewSec
	 if ( (io.fVoid) || (io.d.fCoupon) )// || (io.d.bLinkedItem) ) //LPM linked is set too soon causing problems - ignore this here.  TAR196032
		ss.SetHoldWeight(false);
	 else
		g_bTBReturnedITEMSOLDforSA = true;
#endif
	 rp.LogItemDetails();
	 return rc;
	}
  break;
  case TB_ITEMSALECOMPLETED:
	  {
		  // end ISIP timer
		  SetISIPTimeOut(0);	// end timer, uses pStateAfterISIPTimeOut to return
		  return STATE_NULL;
	  }
  break;
  case TB_TENDERDECLINED:
  case TB_TENDERDECLINEDKEEPCARD:
	// card number 7717771234567801 will emulate a TB_TENDERDECLINEDKEEPCARD LPM110902
    TenderComplete();  //RFC 262343
    fInSignedOn = true; 		 // must be signed on
	fInSignedOff = false;
	fTenderDeclined = true;
	TBGetTenderDetails();

	if(nTenderType == TB_TENDER_VOUCHERPAYMENT)	// RFC 213717 coupon/voucher tender
	{
		return createAnchorState();
	}
	//Set the tender type to TB_TENDER_CASH if pay partial in cash
	// else set to TB_TENDER_NONE
	if (nDMCashInserted || nTenderType == TB_TENDER_CASH)
	{
		nTenderType = TB_TENDER_CASH;
	}
	else
	{
		nTenderType = TB_TENDER_NONE;
	}

	if (fStateAssist)
		return STATE_NULL;

	if (co.fTenderIsMotorizedMSR)
	{
		if (nTBLastState == TB_TENDERDECLINEDKEEPCARD) {	g_bTBCaptureDeclinedCard = true;	}	// if capture card, set flag to true
		CREATE_AND_DISPENSE(CardMisRead)(MSG_CARDDECLINEDSELECTPAYMENT, TTL_CARD_DECLINED);
	}
	else
	{
		CREATE_AND_DISPENSE(CardMisRead)(MSG_CARDDECLINEDSELECTPAYMENT, TTL_CARD_DECLINED);
	}
  case TB_TENDERCANCELLED:
	//TAR376815+
	if  ( (short(lTotalPaid)==(nDMCashInDrawer+nDMCashInserted)) && isAnySoldVoidedItems() ) 
	{
		trace(L7, _T("+ra.OnCanSuspendNow, CanSuspend=true"));
		ra.OnCanSuspendNow(true) ;  // If it is not partially paid with other(credit, ATM or EBT payment)
		trace(L7, _T("-ra.OnCanSuspendNow"));
	}
	//TAR376815-
    TenderComplete();  //RFC 262343
	fInSignedOn = true; 		 // must be signed on
	fInSignedOff = false;
	fTenderDeclined = true; 	// TAR 202818
	bBadCard = true; // mm185096 TAR 330983
	DMSayPhrase(TENDERCANCELLED);
	//Set the tender type to TB_TENDER_CASH if pay partial in cash
	// else set to TB_TENDER_NONE
	if (nDMCashInserted || nTenderType == TB_TENDER_CASH)
	{
		nTenderType = TB_TENDER_CASH;
	}
	else
	{
		nTenderType = TB_TENDER_NONE;
	}
	if (fStateAssist)
		return STATE_NULL;
	else
	{
		// If Takeaway Belt, stop the belts on entry to this screen and TAB waits 2 seconds before requesting scale status. LPM Chu 204277
	    if(dm.fStateTakeawayBelt)
		{
			parmList.Clear();
   			CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_SUSPEND_ITEM);
			_bstr_t bstrParmList((const _TCHAR *)parmList);
			bstrResult = NULL;
			SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
		}
		return setAndCreateAnchorState(BES_FINISH);
	}
  //+RFC 275377 - Void Tender - MIGRATED BM185018
   case TB_TENDERVOIDED:
	{
		  return HandleVoidedTender();
	}
	//-RFC 275377
  case TB_TRXVOIDED:				// at *VOID TRANSACTION*
	{
	  fInSignedOn = true;			 // must be signed on
	  fInSignedOff = false;
	  fInVoidTrans = true;

	  ResetApprovalValues(); //awk.2.5.2 TAR191836 Clear a few of the 5 billion Globals.//tar 253281

	  if(fStateAssist)               // TAR246858
		  m_fVoidedInAssist = true;
//Fiscal Printer start
//if we get to this point at least one item has been voided
	  if (! (lVoidedItems>0))
		lVoidedItems++;
//Fiscal Printer end
	  lItems = 0;		// Set the # of items in transaction and the
	  m_lItemsScanned = 0;	//TAR 202845 04/30/02 Set the # of items scanned in transaction
	  lTotalSale = 0;	// total cost to 0, because transaction has been canceled.
	  bDropoffRequired = false;

	  trace(L7, _T("+rp.m_nDisposition"));
	  rp.m_nDisposition = RP_DISPOSITION_CANCEL;
	  trace(L7, _T("-rp.m_nDisposition<CANCEL>"));
	  traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, _T("Void of Transaction"));
	  // the transaction has completed.
	  // there may be still printing going on, receipt bottom msg etc,,,
	  // wait for TB_TRXEND

	  // Tar 124345
	  return STATE_NULL;
	  //CREATE_AND_DISPENSE(TakeCard)(true);//Tar 126024
	}

  case TB_TENDERACCEPTED:
	{
		//ASSERT(fInSignedOn);
		//ASSERT(!fInSignedOff);
        TenderComplete();  //RFC 262343
		fInSignedOn  = true;		 // must be signed on by now
		fInSignedOff = false;
		fTenderDeclined = false;

		SMStateBase* pState = TBBalanceDetails();

		if(nTenderType == TB_TENDER_VOUCHERPAYMENT)	// RFC 213717, TAR 409226
		{
			lCoupons++;
		}
		if(nTenderType == TB_TENDER_GIFTCARD)
		{
			if(fSAInItemSale)
			{
				SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
			}
			else
			{
				bEASReEnableScanner = true;
			}
			//Get remaining balance on the gift card
			long lGiftCardBalance = 0;
			TBRC rc = TBGetGiftCardBalance(&lGiftCardBalance);

			if(rc!=TB_SUCCESS)
			{
				ProcessTBReturn(rc);   //Trace TB return code
			}

			m_pSavedStateInTendAccepted = pState;

			if (co.fOperationsDisplayGiftCardBalance)
			{
				RETURNSTATE(TenderBalance)
			}
			else
			{
				DMMSREject();  //tar 269534	Eject inserted Gift Card
				if (lGiftCardBalance <= 0				&&
						 co.nOperationsMethodOfCollectionForEmptyGiftCard == 1)
				{
					if (IsUsingCouponSensor())
						RETURNSTATE(InsertGiftCard)
					else
						RETURNSTATE(DepositGiftCard)
				}
				else if (lGiftCardBalance <= 0				&&
						 co.nOperationsMethodOfCollectionForEmptyGiftCard == 2)
				{
					RETURNSTATE(CollectGiftCard)
				}
				else
				{
					return pState;
				}
			}
		}
		else
		{
			return pState;
		}
	  }
  case TB_TENDERFOODSTAMPINSUFFICIENT:
    TenderComplete();  //RFC 262343
	fTenderDeclined = false;
	return TBGetAvailableFoodStampAmount();

  case TB_TENDERCASHBENEFITINSUFFICIENT:
    TenderComplete();  //RFC 262343
	fTenderDeclined = false;
	return TBGetAvailableBenefitAmount();

  case TB_MESSAGESCREEN:   // TAR 127247
	{
	   return TBGetMessageInfo();
	}
  case TB_CREATE_REPORT_ENTRY:
	{
	   TBCreateReportEntry();
	   return(STATE_NULL);
	}
  case TB_GETBUSINESSDATE:
	  {
	   CString csLastBusinessDate;
	   // Store the current business date before calling TB to get
	   // the business date
	   csLastBusinessDate = rp.m_csBusinessDate;
       rp.GetCurrentTBBusinessDate();
	   // If EOD option is set to TB control
       if (co.nOperationsEOD == OPTION_TB_CONTROL)
	   {
		   // if business date has changed then run EOD
		   if (rp.m_csBusinessDate != csLastBusinessDate)
		   {
				// if in the middle of transaction - just set a flag to
			    // run once current transaction is completed
				if ( fInSignedOn || fInSignedOff || fSAInTransaction )
				{
					fDoEOD = true;
				}
				else
				{
					return PerformEOD();
				}
			}
	   }
       return(STATE_NULL);
	  }

  case TB_FOODSTAMPELIGIBLETOTAL:
		TRACE(_T("%s(%d) :TBParse(TB_FOODSTAMPELIGIBLETOTAL)\n"), __FILE__, __LINE__);
		return TBParseFoodStampTotal();

  case TB_TRXEND:
	  {
		fInSignedOn = false;			 // must be signed on
		fInSignedOff = true;
		traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, _T("EOT"));
		trace(L7, _T("+rp.SetItemCount"));
		rp.SetItemCount( GetDisplayedItemCount() ); // RFC 371075
		trace(L7, _T("-rp.SetItemCount<%d>"), GetDisplayedItemCount() ); // RFC 371075
		//+TAR394157
		/*trace(L7, _T("+rp.SetTransactionAmount"));
		rp.SetTransactionAmount(lTotalSale);
		trace(L7, _T("-rp.SetTransactionAmount<%d>"),lTotalSale);*/
		//-TAR394157
		// TAR 332650 Start
		trace(L7, _T("+ra.OnCanSuspendNow, CanSuspend=false"));
		ra.OnCanSuspendNow(false) ;  // disable suspend
		trace(L7, _T("-ra.OnCanSuspendNow"));
		// TAR 332650 End
		ResetApprovalValues(); //TAR 317577

		CString csStateName = this->GetRuntimeClass()->m_lpszClassName;
		if ((csStateName.Find(_T("TenderBalance"), 1) >= 0)		||
			(csStateName.Find(_T("DepositGiftCard"), 1) >= 0)	||
			(csStateName.Find(_T("InsertGiftCard"), 1) >= 0)	||
			(csStateName.Find(_T("CollectGiftCard"), 1) >= 0)	)
		{
			m_bTrxEnded = true;
			return STATE_NULL;
		}
		else
		{
//USSF START
			USSFHOOK(_T("TBXFinish"), nextState = TBFinish(), return nextState;);
			//return TBFinish();
//USSF END
		}

	  }
  case TB_TRXEND_PRINTANDDISPENSE:
		fInSignedOn = false;			 // must be signed on
		fInSignedOff = true;
		traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, _T("EOT"));
		trace(L7, _T("+rp.SetItemCount"));
		rp.SetItemCount( GetDisplayedItemCount() ); // RFC 371075
		trace(L7, _T("-rp.SetItemCount<%d>"), GetDisplayedItemCount() ); // RFC 371075


		//TBFinish();

	  //RAP Dispense Change
	  if(lChangeDue && !co.fTenderDispenseChangeAtLaneForRemoteTendering && ps.RemoteMode() && fStateAssist  ) //Remote AssistMode Tendering
	  {
		   dm.CashDrawerOpen();  //Pop up cash drawer to let cashier giving change to customers.
		   m_bWaitForCashDrawerClose = true;
		   //reporting
		   rp.RAPDispenseChange(lChangeDue);
		   lChangeDue = 0;    //Do not dispense Change at Lane
		   nTimeOut = 1;
           trace(L6, _T("Cash Drawer is still open at remote assitmode, wait until it is closed"));

			return STATE_NULL;
		}
	  //End RAP Dispense Change
	  else
		   return TBEndOfTransaction();

  case TB_VOIDITEMBEFORECOUPON:
		csDesc = ps.GetPSText(RA_ATTEMPTED_VOID_OF_ITEM_WITH_COUPON,SCOT_LANGUAGE_PRIMARY);
		trace(L7, _T("+ra.OnNormalItem, Message=%s"), csDesc);
		ra.OnNormalItem(RAP_RECEIPT_LINE_HIGHLIGHT + csDesc);
		trace(L7, _T("-ra.OnNormalItem"));
		//DMSayPhrase(SECURITYMED);
		//ps.DelayMessage12(MSG_CANCELITEMCPN);
		SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
        if (co.IsInAssistMenus())
        {
            SMSmAssistMenu::SetAssistMessage(MSG_CANCELITEMCPN_T2, SECURITYMED);   //TAR 388762
			RETURNSTATE(SmAssistMenu)
        }
		// return createAnchorState();
		else
			return setAndCreateAnchorState(BES_SCANANDBAG);  // first state is S&B,

  case TB_ITEMQTYEXCEEDED:
		if(!TBIsAllowed(TB_FT_QUICKITEMENTRY))
			TBGetItemDetails(io.d);


		if (io.fVoid)


		{
			return TBEnterItem(true);
		}
		else
		{
			if (restrictedItemNotAllowed())
			{
				PSTEXT msgNum = restrictedItemMessage();
				if (msgNum)
				{
				  // It does no good to do this from Assist Mode - no store
				  // mode state should do it, for that matter.
				  if (storeMode() && !co.IsInAssistMenus())
					return STATE_NULL;

				  lRestrictedNotAllowedItems++;
				  // Tar 123351
				  SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
				  CREATE_AND_DISPENSE(RestrictedNotAllowed)(msgNum);
				}
			}
		}
		if (!bgAlreadySubmittedWithOverride)
		{
			RETURNSTATE(ConfirmQuantity)	// ask for confirmation
		}
		else
		{
			SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
			bgAlreadySubmittedWithOverride = false;
			return setAndCreateAnchorState(BES_SCANANDBAG);
		}

  case TB_TRXLIMITEXCEEDED:
	// return new SMWaitForApproval(false,true);  // ask for intevention
	  SMState::bTrxLimitExceed = true;	// used for message display in S&B screen
	SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
	  return setAndCreateAnchorState(BES_SCANANDBAG);  // first state is S&B,

  case TB_ITEM_QUANTITY_RESTRICTED:

	//Tar 123351
	SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
	if (co.IsInAssistMenus())
    {
        SMSmAssistMenu::SetAssistMessage(TXT_QUANTITYRESTRICTEDITEMS, QUANTITYLIMITREACHED);

        RETURNSTATE(SmAssistMenu)
    }
    else
    {
		RETURNSTATE(ItemQuantityExceeded)
	}

  case TB_COUPONNOTMATCH:

	// Inform SA that the coupon was refused
	//Tar 124112	SASendSCOTInput( COUPON_REFUSED );
	SASendSCOTInput( COUPON_REFUSED, io.csItemCode, 0, 0, 1);
	RETURNSTATE(CouponNoMatch) // prompt value

  case TB_ITEMRECALLED:
	{

	  //Tar 123351
	  SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
      lNotForSaleItems++;
      if (co.IsInAssistMenus())
      {
          SMSmAssistMenu::SetAssistMessage(LTT_UNSELLABLEITEM, SETITEMASIDECONTINUEWITHPURCHASES);
          RETURNSTATE(SmAssistMenu)
      }
      else
      {
           RETURNSTATE(ItemNotForSale)
      }
	}

  case TB_TIMERESTRICTEDITEM:
	{

	  //Tar 123351
	  SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
	  lTimeRestrictedItems++;
      if (co.IsInAssistMenus())
      {
          SMSmAssistMenu::SetAssistMessage(LTT_RESTRICTEDITEM, RESTRICTEDITEM);
          RETURNSTATE(SmAssistMenu)
      }
      else
      {
          RETURNSTATE(TimeRestrictedItem)
      }
	}

  case TB_VOIDNOTMATCH:
		// Tar 120878
		// TBGetItemDetails(io.d);
		SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
		RETURNSTATE(VoidNoMatch)

  case TB_ITEMUNKNOWN:
		//Tar 123351
		//tar 396627+
	    //if (!fInMultiSelectPickList || ps.GetPicklistItemsSold()==0)
		//{
			SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
		//}

		if(fInMultiSelectPickList)
		{
			g_lNumberOfItemsNotBagInMultiPick--;
		}
		//tar 396627-
		lUnknownItems++;
		//Unknown ItemExceptions for Reporting 3.1
		trace(L7, _T("+rp.UnknownItemException"));
		rp.UnknownItemException(io.csItemCode);
		trace(L7, _T("-rp.UnknownItemException<%s>"), io.csItemCode);
        if (co.IsInAssistMenus())
        {
            SMSmAssistMenu::SetAssistMessage(LTT_UNKNOWNITEM, UNKNOWNITEM);
            RETURNSTATE(SmAssistMenu)
        }
        else
        {
            RETURNSTATE(UnknownItem)
        }

  case TB_COUPONEXCEEDED:
		if(!TBIsAllowed(TB_FT_QUICKITEMENTRY))
			TBGetItemDetails(io.d);
		// submit the item to TB with override flag on only once
		if (!bgAlreadySubmittedWithOverride)
		{
			return TBEnterItem(true);  //Override
		}
		else
		{
			SASendSCOTInput( COUPON_REFUSED, io.csItemCode, 0, 0, 1);
			// flag 'bgAlreadySubmittedWithOverride' is reset by CouponTooHigh ok button handler
			RETURNSTATE(CouponTooHigh)
		}
  case TB_LOYALTYCARD:
		{
//USSF Start
			if (co.fOperationsEnableUSSF)
				USSFEVENT(_T("ParseLoyaltyStart"), if (nextState != STATE_NULL){return nextState;}, return nextState; );
//USSF End
			if (!fInSignedOn)
			{
				fInSignedOn = true; 	 // must be signed on by now
				fInSignedOff = false;
			}
			CString csWork = LINK_ITEM;
			io.csOrigCode = csWork + io.csOrigCode;
			io.csItemCode = csWork + io.csItemCode;

			LPTSTR pLoyaltyCardNumber = NULL;
			CString csLoyaltyCardInfo;
			TBRC retCode = tb.GetLoyaltyCardNumber(&pLoyaltyCardNumber);

			if(retCode!=TB_SUCCESS)
			{
				//Trace TB return code
				ProcessTBReturn(retCode);
			}

			if (pLoyaltyCardNumber && _tcsclen(pLoyaltyCardNumber))
			{
				CString csLoyaltyCardNumber = pLoyaltyCardNumber;
                csLoyaltyCardInfo.Format(ps.GetPSText(MSG_RA_LOYALTYCARDINFO, SCOT_LANGUAGE_PRIMARY), csLoyaltyCardNumber);

                CKeyValueParmList parmList;
                CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_LOYALTY_LOOKUP);
                CKeyValue<const _TCHAR*>::Put(parmList, KEY_LOYALTY_ID, csLoyaltyCardNumber);
                custInfo.Lookup((const _TCHAR *)parmList);
            }
			else
			{
				csLoyaltyCardInfo = ps.GetPSText(MSG_RA_LOYALTYCARDINFO2, SCOT_LANGUAGE_PRIMARY);
			}

			trace(L7,_T("+ra.OnNormalItem, Message = %s"), csLoyaltyCardInfo);
			ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csLoyaltyCardInfo);
			trace(L7,_T("-ra.OnNormalItem"));

//			if (    ( fInLoyaltyCardProcessing && cDMLastTrack )   //Loyalty swipped card
//	 		    || co.fOperationsPartialIntegratedPINPad  ) //TAR235030 CHu 042103 for PIP: swapped loyalty card
            if(!m_bCouldBeScannedLoyaltyCard)  //TAR242494
			{
				fInLoyaltyCardProcessing = false;
				// loyalty code is from card, tell user to take it back
				// once taken, go to scan and bag
				if (fLoyaltyCardTrx)
				{
					// 2nd time loyalty card inserted
					CREATE_AND_DISPENSE (TakeLoyaltyCard)(LC_INSERTED_BEFORE);
				}
				else
				{
					// first time loyalty card inserted
					// If Takeaway Belt, stop the belts on entry to this screen and TAB waits 2 seconds before requesting scale status. LPM Chu 204277
					if(dm.fStateTakeawayBelt)
					{
						parmList.Clear();
   						CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_SUSPEND_ITEM);
						_bstr_t bstrParmList((const _TCHAR *)parmList);
						bstrResult = NULL;
						SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
					}
					fLoyaltyCardTrx = true;
					TBGetTotalDetails();   //Tar 219951
//USSF Start
					if (co.fOperationsEnableUSSF)
						USSFEVENT(_T("ParseLoyaltyEnd"), if (nextState != STATE_NULL){return nextState;}, return nextState; );
//USSF End
					CREATE_AND_DISPENSE(TakeLoyaltyCard)(LC_VALID);
				}
			}
			else	// no track data, must have been a scan
			{
				// tell security to treat this as an item cancel to enable scanner
				// we need to send this input to Security until it supports loyalty code
				if(!TBIsAllowed(TB_FT_QUICKITEMENTRY))
					TBGetItemDetails(io.d);
				io.csItemCode = io.d.csItemCode;
				SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
				io.nLoyaltyCardScanned = 1;
				TBGetTotalDetails();//Tar 124992
				fLoyaltyCardTrx = true;
				io.bLoyaltyCard = true;
				io.d.fDetails = true;
//USSF Start
				if (co.fOperationsEnableUSSF)
				 USSFEVENT(_T("ParseLoyaltyEnd"), if (nextState != STATE_NULL){return nextState;}, return nextState; );
//USSF End
				if (getAnchorState() == BES_ENTERCOUPONS)
					return createAnchorState();
				//Scan loyalty card at select payment type screen should not return to S&B	Tar 200015 BZ 03/25/02 +
				if (getAnchorState() == BES_FINISH)
				{
					//return createAnchorState(); //Tar 200015 -
					return STATE_NULL;  //410299
				}
				//410299+
				if(co.IsInAssistMenus() || fStateAssist)
				{
					// by default goto S&B
					RETURNSTATE(ScanAndBag)
				}
				else
				{
					return STATE_NULL; //go back to Inprogress and let SM decide when to go to ScanAndBag
				}
				//410299--
			}
		}
		break;
  case TB_LOYALTYINVALID:
//		if (   ( fInLoyaltyCardProcessing && cDMLastTrack) //Loyalty swipped card
//			|| co.fOperationsPartialIntegratedPINPad  ) //TAR235030 CHu 042103 for PIP: swapped loyalty card
        if(!m_bCouldBeScannedLoyaltyCard)  //TAR242494
		{
			fInLoyaltyCardProcessing = false;

			// loyalty code is from card, tell user to take it back
			// once taken, go to scan and bag
			CREATE_AND_DISPENSE(TakeLoyaltyCard)(LC_INVALID);
		}
		else
		{
			// loyalty code is from bar code and it is an invalid code, so treat it as unknown item code
			SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
            lUnknownItems++;
            if (co.IsInAssistMenus())
            {
                SMSmAssistMenu::SetAssistMessage(LTT_UNKNOWNITEM, UNKNOWNITEM);
                RETURNSTATE(SmAssistMenu)
            }
            else
            {
                //Tar 123351
				//SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
                RETURNSTATE(UnknownItem)
            }
		}
	  break;
  case TB_CMOS_DOSOPEN:
  case TB_CMOS_DOSCLOSE:
  case TB_CMOS_DOSREAD:
  case TB_CMOS_DOSWRITE:
  case TB_CMOS_DOSSEEK:
  case TB_CMOS_PURGE:
		// Add all other TM_CMOS_ states here also as more become available
		return HTParse(me, nTBLastState);
  case TB_PRINTIMMEDIATE:
  //case TB_NEEDOVERRIDE:
  //case TB_NEEDTARE:
  case TB_SECURED:
		return STATE_NULL;

  case TB_PRINTIMMEDIATECUT:
//Fiscal Printer start
		if(! co.fOperationsTBControlsPrinter)
//Fiscal Printer end
		DMReceiptCut();
		return STATE_NULL;

  case TB_VOIDLIMITEXCEEDED:
		//Tar 131585
		dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
							DM_TRICOLORLIGHT_ON,
							DM_TRICOLORLIGHT_NORMALREQUEST);
		if(!TBIsAllowed(TB_FT_QUICKITEMENTRY))
			TBGetItemDetails(io.d);
		// return TBEnterItem(true);
		// let us not get into a loop
		// where SCOT submits item with override and TB returns the same state
		if (!bgAlreadySubmittedWithOverride)
		{
			return TBEnterItem(true);  //Override
		}
		else
		{
			bgAlreadySubmittedWithOverride = false;
			// don't know what to do, but let us wait for approval and show the latest message received from TB
			CString csWork = CleanUpMessage(csLastDisplay,true);
			return createLevel2Message( csWork, 	// char *message
										PS_NOTEXT,	// no canned message
										PS_NOTEXT,	// no prompt
										0,			// no prompt value
										-1, 		// no device
										false		// no DM check health needed
									   );
		}

  //Begin_Suspend
  case TB_TRXSUSPENDED:
  {
	fInSignedOn = true; 		 // must be signed on
	fInSignedOff = false;
	bInSuspendCall = false;
	fInSuspendTrans = true;
	ResetApprovalValues();

	//TAR 209544 Turn off red light when trx is suspended.
	dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
						DM_TRICOLORLIGHT_OFF,
						DM_TRICOLORLIGHT_NORMALREQUEST);
	dm.turnOffTriColorLight();

	trace(L7, _T("+rp.SetItemCount"));
	trace(L7, _T("+rp.m_nDisposition"));
	rp.SetItemCount( GetDisplayedItemCount() ); // RFC 371075
	rp.m_nDisposition = RP_DISPOSITION_SUSPEND;
	trace(L7, _T("-rp.SetItemCount<%d>"), GetDisplayedItemCount() ); // RFC 371075
	trace(L7, _T("-rp.m_nDisposition = RP_DISPOSITION_SUSPEND"));

	// If Takeaway Belt, stop the belts on entry to this screen and TAB waits 2 seconds before requesting scale status. LPM Chu 204277
	if(dm.fStateTakeawayBelt)
	{
		parmList.Clear();
   		CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_SUSPEND_ITEM);
		_bstr_t bstrParmList((const _TCHAR *)parmList);
		bstrResult = NULL;
		SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
	}
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, _T("Suspension of Transaction"));
		// the transaction has suspended.
		// there may be still printing going on, receipt bottom msg etc,,,
    // wait for TB_TRXEND
    // Tar 124345
    // if there is Other Payment pending approval, the approval should be cleared
    if (fOtherPayment)
    {  // let ra know that Other Payment has be cleared.
        trace(L7, _T("+ra.OnOtherPaymentApproval"));
        ra.OnOtherPaymentApproval() ;
        trace(L7, _T("-ra.OnOtherPaymentApproval"));
        //fOtherPayment = false ;    save this flag for outstanding approval printing
    }
    return STATE_NULL;
   // CREATE_AND_DISPENSE(TakeCard)(true);	//Tar 126024
  }
  case TB_TRXSUSPENSIONUNCOMPLETED:
	{ //Tar 126382
		bInSuspendCall = false;
		bSuspendApproved = true;		//TAR 352409
		bSuspendFailure = true;
		trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(RA_SUSPEND_DECLINED,SCOT_LANGUAGE_PRIMARY));
    	ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY +
						ps.GetPSText(RA_SUSPEND_DECLINED,SCOT_LANGUAGE_PRIMARY));
		trace(L7, _T("-ra.OnNormalItem"));
		if(CString(_T("SMSmAuthorization")) == this->csLastNonHelpStateName ||
           CString(_T("SMSmConfirmSuspendedTransaction")) == this->csLastNonHelpStateName)
        {
            fCancelSuspend = true;
            RETURNSTATE(SmAuthorization)
        }
        else
        {
            return setAndCreateAnchorState(BES_FINISH);
        }
	}


  case TB_INVALIDPIN:
	  {
		nPINTries++;
		if(nPINTries < 3)
		{
		  CREATE_AND_DISPENSE(EnterPin)(nTenderType, fCashBack); //try again
		}
		else
		{// 3 wrong PIN attempts.. must do something different
		  if (co.fTenderIsMotorizedMSR)
		  {
			CREATE_AND_DISPENSE(CardMisRead)(MSG_INSERT_DIFF_CARD, TTL_WRONG_PIN);
		  }
		  else
		  {
			CREATE_AND_DISPENSE(CardMisRead)(MSG_SLIDE_DIFF_CARD, TTL_WRONG_PIN);
		  }
		}
	  }

  case TB_EPSEODOK:
	  {
		 extern HFILE hEPSPrintFile;
         if (NULL != hReceiptFile)
         {
             trace(L6, _T("ASYNCHRONOUS PRINT ISSUED, POSSIBLE RECEIPT FILE HANDLE LEAK"));
         }
		 hReceiptFile = hEPSPrintFile;
		 hEPSPrintFile = NULL;
		 if (hReceiptFile)
		 {
			long nOldTrxReceiptLength = nTrxReceiptLength;
			long nEODReceiptDataLen = PrintReceipt(hReceiptFile); // print receipt
			if (nEODReceiptDataLen)
			{
				CutReceipt();		// cut EOD receipt

				nTrxReceiptLength = nOldTrxReceiptLength;
			}
		 }
	  }
  case TB_EPSEODNOK:
	  RETURNSTATE(SmSystemFunctions)
	  break;
  case TB_ENTEREDTRAINING:
	  {
		// enter training mode and return SCOT to SmMediaStatus screen
		if (!TrainingModeObj.IsTMOn()) // training mode should be off by this time
		{
		        //+TAR 391863
			CString csLReceiptFile = LRECEIPT_FILE;
			CString csLReceiptTmpFile = LRECEIPT_TMP_FILE;
	                GET_PARTITION_PATH(LRECEIPT_FILE, csLReceiptFile.GetBuffer(_MAX_PATH));
			GET_PARTITION_PATH(LRECEIPT_TMP_FILE, csLReceiptTmpFile.GetBuffer(_MAX_PATH));
	                csLReceiptFile.ReleaseBuffer();
			csLReceiptTmpFile.ReleaseBuffer();

			//Create TmpLastReceiptData.dat file with a data from LastReceiptData.dat file
			::CopyFile(csLReceiptFile, csLReceiptTmpFile, NULL);
			//-TAR 391863
			
			TrainingModeObj.TurnOn();
			//Return to current store mode menu state instead of SmSystemFunctions
            return STATE(StoreModeMenu)::CreateStoreModeMenu();
		}
		trace(L3, _T("!!!! TB ERROR:  SCOT got a TB state change request at an unexpected context, Fix will be in TB !!!!"));
		return STATE_NULL;
	  }
	  break;
  case TB_LEFTTRAINING:
	  {
		// leave training mode and return SCOT to SmMediaStatus screen
		if (TrainingModeObj.IsTMOn()) // training mode should be on by this time
		{
		        //+TAR 391863
			CString csLReceiptTmpFile = LRECEIPT_TMP_FILE;
	                GET_PARTITION_PATH(LRECEIPT_TMP_FILE, csLReceiptTmpFile.GetBuffer(_MAX_PATH));
	                csLReceiptTmpFile.ReleaseBuffer();

			//If TmpLastReceiptData.dat file exist
		        //Delete the temp file to preserve the last receipt with no training mode text
		        if(FileExist(csLReceiptTmpFile))
			{
		            RemoveFile(csLReceiptTmpFile);
			}
			//-TAR 391863

			TrainingModeObj.TurnOff();
			//Return to current store mode menu state instead of SmSystemFunctions
            return STATE(StoreModeMenu)::CreateStoreModeMenu();
		}
		trace(L3, _T("!!!! TB ERROR:  SCOT got a TB state change request at an unexpected context, Fix will be in TB !!!!"));
		return STATE_NULL;
	  }
	  break;
  case TB_VISUAL_VERIFY: //go to System Message screen
	  {
	  if (fStateAssist)
	  {
		  trace(L6, _T("!!!WARNING: SCOT received TB_VISUAL_VERIFY inside assistmode: \"%s\""), csLastDisplay);
		  // during assistmode, no need to display TB unknown state messages since it is in assistmode display
		  return STATE_NULL;
	  }
	  CString csWork = CleanUpMessage(csLastDisplay,true);

	  // Should goto System Message 4 which has a continue button
	  return createLevel1Message(	csWork, 	// char *message
									PS_NOTEXT,	// no canned message
									PS_NOTEXT,	// no prompt
									0,			// no prompt value
									-1, 		// no device
									false); 	// no DM check health needed
	  }
	  break;
  case TB_CUSTOMERMESSAGE:
	  {
		  LPTSTR pCustomerMessage = NULL;
		  LPTSTR pCustomerWaveFile = NULL;
		  tb.GetCustomerMessage(&pCustomerMessage, &pCustomerWaveFile);
		  if ((pCustomerMessage) && _tcsclen(pCustomerMessage))
		  {
			  ps.ShowCustomerRewardMessage(pCustomerMessage);
			  CustomerMessage = pCustomerMessage;
			  try
			  {
				  delete pCustomerMessage;
			  }
			  catch (...)
			  {
				trace(L6, _T("Received exception while deleting pCustomerMessage..."));
			  };
		  }
		  if ((pCustomerWaveFile) && _tcsclen(pCustomerWaveFile))
		  {
			  CString csWaveFile = pCustomerWaveFile;
			  if (csWaveFile.Find(_T("\\")) == -1)
			  {
				  // no path specified, use SCOT multilingual path location rules
				  DMSayPhrase(pCustomerWaveFile);
			  }
			  else
			  {
				  TRACE(_T("!!!!SCOTPriceSound: Playing this wave file: %s !!!!!\n"), pCustomerWaveFile);
				  // DMSayPhrase adds path and this wave file will already contains path
				  BOOL bRet = SCOTPriceSoundSayMessage(pCustomerWaveFile);
				  if(!bRet)
				  {
					ScotError( INFO,SM_CAT_INTERNAL_ERROR, SCOTAPP_DMPROCEDURES_SAYPHRASE);
				  }
			  }
			  try
			  {
				  delete pCustomerWaveFile;
			  }
			  catch (...)
			  {
				trace(L6, _T("Received exception while deleting pCustomerWaveFile..."));
			  };
		  }
		  return STATE_NULL;
	  }
	  break;
  case TB_REWARDLINE:
	  {
		  SRewardLineDetails RewardItemDetails;
		  tb.GetRewardLineDetails(&RewardItemDetails);
		  CString csDesc;
		  // get description
		  if (RewardItemDetails.szDescription)
		  {
			  csDesc = RewardItemDetails.szDescription;
			  try
			  {
				  delete RewardItemDetails.szDescription;
			  }
			  catch(...)
			  {
				  trace(L4, _T("Exception while deleting RewardItemDetails.szDescription"));
			  }
		  }
		  long lRewardID = RewardItemDetails.lEntryID;
		  long lValue = RewardItemDetails.lValue;
		  if (RewardItemDetails.bSuppressValue)
		  {
			  lValue = LONG_MIN;
		  }
		  long lEmphasis = RewardItemDetails.lEmphasis;
		  // execute requested reward operation
		  if (RewardItemDetails.nOperatationType == REWARDOP_DELETE)
		  {
			  // DeleteRewardItem
			  // FindIndexFromRewardID and delete
			  ps.DeleteRewardItem(lRewardID);
		  }
		  else if (RewardItemDetails.nOperatationType == REWARDOP_INSERT)
		  {
			  long nIndexReferred = -1;
			  // insert reward item
			  switch (RewardItemDetails.nPosType)
			  {
				//*** NewUI ***//
			    case BOTTOM_OF_RECEIPT:
				case ABOVE_REF_ENTRYID:
				case BELOW_REF_ENTRYID:
					ps.AddRewardItem(_T("CMSMReceipt"), csDesc, lRewardID, lValue, RewardItemDetails.nPosType, RewardItemDetails.lEntryIDForReference);
					//TAR 397432
					ps.AddRewardItem(_T("PickListReceipt"), csDesc, lRewardID, lValue, RewardItemDetails.nPosType, RewardItemDetails.lEntryIDForReference);
					ps.AddRewardItem(_T("CMSmallReceipt"), csDesc, lRewardID, lValue, RewardItemDetails.nPosType, RewardItemDetails.lEntryIDForReference);
					break;
				//*** end of NewUI ***//
				case POS_NONE:
				default:
					trace(L4, _T("TB requests invalid reward line insertion, Desc:%s"), csDesc);
					break;
			  }
		  }
		  if (RewardItemDetails.bUpdateRewardTotal)
		  {
			  // update reward total
			  ps.ShowTotalRewardPoints(RewardItemDetails.lRewardPoints);
		  }
		  csDesc.Empty();
		  return STATE_NULL;
	  }
	  break; // of tb_rewardline
//Fiscal Printer start
  case TB_PRINTER_VERIFY:
	  {
		dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
							DM_TRICOLORLIGHT_BLINK_1HZ,
							DM_TRICOLORLIGHT_NORMALREQUEST);
		long rc=0;
		LPTSTR pTBPrintErrormsg = NULL;
		CString csWork;
		rc = tb.GetPrinterError(&pTBPrintErrormsg);
		if (rc != TB_E_FUNCTIONNOTSUPPORTED && pTBPrintErrormsg != NULL)
		{
			csWork = pTBPrintErrormsg;
			delete [] pTBPrintErrormsg;
			SMStateBase* sExit =
			  createLevel1Message(csWork,	  // char *message
								 PS_NOTEXT,  // no canned message
								 PS_NOTEXT,  // no prompt
								 0, 		 // no prompt value
								 PIDCLASS_FISCALPRTR,
								 false);	 // no DM check health needed

		//Notify rap
		trace(L7, _T("+ra.OnRAPDataNeeded, ErrorMessage=%s, DeviceError=%d"), csWork, PIDCLASS_FISCALPRTR);

		// save the previous view
		m_PrevView = m_View;

		cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("DeviceFailure"));
		m_View = DATANEEDED_DEVICEERROR;
		CString csSecInfo;
		csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),csWork,ps.GetPSText(BUTTON_RA_DEVICE_ERROR, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""), _T(""), _T(""));
		ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);

		trace(L7, _T("-ra.OnRAPDataNeeded"));
		return sExit;
	  }
	}
	break;
//Fiscal Printer end
  case TB_ASSISTMODETABFLUSH:
	if(dm.fStateTakeawayBelt)
	{
		parmList.Clear();
   		CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_FLUSH_ITEM);
		_bstr_t bstrParmList((const _TCHAR *)parmList);
		bstrResult = NULL;
		SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
	}
	break;
  case TB_HOSTOFFLINE:
	  return HostOffLine();
	break;
  case TB_HOSTONLINE:
	  return HostOnLine();
	break;
  case TB_CHANGESTATE:
    RETURNSTATE(ExtensionState)
    break;
  //+dp185016
  case TB_OUTOFSYNC_ASSISTMODE:
      return HandleOutOfSyncAssist();
  case TB_OUTOFSYNC_STOP:
      return HandleOutOfSyncStop();
  //-dp185016
  //+RFC 986
  case TB_LANECLOSED:
        fPutSCOTinLC = true;
        bLaneClosed = true;
		ps.Echo(PS_BLANK);			  // clear messages
		fInSignedOn = false;			// must be signed off
		fInSignedOff = false;
		return setAndCreateAnchorState(BES_START);
  case TB_LANEOPEN:
		ps.Echo(PS_BLANK);			  // clear messages
		fPutSCOTinLC = false;	      // TAR 377496
        bLaneClosed = false;
		fInSignedOn = false;			// must be signed off
		fInSignedOff = false;
		dm.RemoteRelay( DM_MISC_STATE_ON );
		return setAndCreateAnchorState(BES_START);
  //-RFC 986
  }  // end switch case
	// not a known state..
  if ((co.fUserExitAllEnabled) &&	// if exits enabled and
	(co.fUserExit02Enabled))		//	this exit is enabled
  {
	// maybe the user exit can use this message
	sUser = UserExit02();
	if (sUser != STATE_NULL)
	  return sUser;
  }

  if (fStateAssist)
	return STATE_NULL;			  // pd980325 no error msgs in bypass mode

  CString csWork = CleanUpMessage(csLastDisplay,true);

  return createLevel2Message(csWork,	 // char *message
							 PS_NOTEXT,  // no canned message
							 PS_NOTEXT,  // no prompt
							 0, 	  // no prompt value
							 -1,		 // no device
							 false);	 // no DM check health needed
}  // of TBParse
// TAR 374450 This function was moved from being an inline function so that it would continue to
// function properly after ProcessMessage began being accessed indirectly from the CTransBroker module.
// If inline, calls from SSF (which overrides this function and then calls the base) end up accessing through
// a non initialized set of static variables and the call fails. 
TBSTATE SMStateBase::TBProcessMessage(MessageElement* me)
{
	return tb.ProcessMessage(me);
}

///////////////////////////////
SMStateBase *SMStateBase::TBItemDetails(void)
{
  SMStateBase *sUser = STATE_NULL;
  CString csFormat;
  CString csItemLine;
  bool bAutoApprovedItem = false;


  //TAR342185+
  //TB could send 2nd DataNeeded with empty view during an item sale, then sends ItemSold to FL. This empty view
  //will tell SM that the item sale is finished which puts FL and SM out of sync
  trace(L5, _T("TBItemDetails: nTBLastState=%d, fSAInItemsale=%d, io.d.LinkedItem=%d"),nTBLastState, fSAInItemSale, io.d.bLinkedItem);
  trace(L5, _T("TBItemDetails: io.cScanType=%d"),io.cScanType);

  if( ( nTBLastState == TB_ITEMSOLD) && !fSAInItemSale && !io.d.bLinkedItem && (io.d.lRequireSecBagging != TB_OVERRIDE_SECURITY_WITH_NOWEIGHT) ) 
  {
		// if there is a scanned UPC
		if(io.cScanType!=0)
		{
            trace(L5, _T("TBItemDetails: SASendSCOTInput UPC_SCANNED"));
			csItemSent = io.d.csItemCode;
			SASendSCOTInput( UPC_SCANNED, io.d.csItemCode );
		}
		else	// UPC keyed in,
		{
            trace(L5, _T("TBItemDetails: SASendSCOTInput UPC_MANUAL"));
			csItemSent = io.d.csItemCode;
			SASendSCOTInput( UPC_MANUAL, io.d.csItemCode );
		}
  }
  //TAR342185-

  if (fStateAssist || co.IsInAssistMenus())
  {
     // Logic reworked for TAR 257368 to support PIM notification to RAP
     bool found = false;
     CString csDesc,csCode, csItemEntryId;
     long lQty,lWgt,lPrc,lUPrc, lTcd, lBarcodeType;

     // TAR 272593 start - support lookup by item ID or entry ID
     if (INVALID_ENTRYID == io.d.lEntryID)
     {
        csItemEntryId = io.d.csItemID;
     }
     else
     {
     // TAR 272593 end
		csItemEntryId.Format(_T("%d"), io.d.lEntryID);
	 }
     if (!io.d.bLinkedItem &&   //linked items don't send a valid entry ID
         (ps.GetItem(csDesc,
                    csCode,
                    lQty,
                    lWgt,
                    lPrc,
                    lUPrc,
                    lTcd,
                    lBarcodeType,
                    _T("CMSMReceipt"),
                    _T("CMReceiptVoid"),
                    csItemEntryId)      > 0))// get item info for specified entry ID
     {
        if (csCode.GetLength())
        {
           if (!_istdigit(csCode[0]))
              csCode.SetAt(0, _T(' '));
           csCode.TrimLeft();
        }
        // TAR 272593 start
        CString csItemCodeVal = io.csItemCode;
        if (csItemCodeVal.GetLength())
        {
           if (!_istdigit(csItemCodeVal[0]))
              csItemCodeVal.SetAt(0, _T(' '));
           csItemCodeVal.TrimLeft();
        }
        if (csCode == csItemCodeVal)
        // TAR 272593 end
        {
           found = true;
        }
     }

     if (io.fVoid)
     {
        if (!found)
        {
           int nCountList = ps.ScanReceiptToVoid(_T("CMSMReceipt"), csItemEntryId);
           for (int i=nCountList-1; i>=0; i--)
           {
              ps.ScanReceiptToVoid(_T("CMSMReceipt"), csItemEntryId, i);
              if (ps.GetItem(csDesc,csCode,lQty,lWgt,lPrc,lUPrc,lTcd, lBarcodeType, _T("CMSMReceipt"), _T("CMReceiptVoid"), csItemEntryId)>0)// get selected item info
              {
                 if (csCode.GetLength())
                 {
                    if (!_istdigit(csCode[0]))
                       csCode.SetAt(0, _T(' '));
                    csCode.TrimLeft();
                 }
                 if (csCode == io.csItemCode)
                 {
                    found = true;
                    ps.SetSelection(_T("CMReceiptVoid"), csItemEntryId);
                    break;
                 }
              }
           }
        }
        ps.GetCurrentItemAutoApprovalFlag(bAutoApprovedItem);  //TAR#125677 find out whether or not
     }
     // TAR 257368 addition start
     else if (found)  // Post-item modification occurred
     {
        CString csPIM;

        // Create explanatory line for PIM
        csFormat = ps.GetPSText(RA_PIM_START_ID, SCOT_LANGUAGE_PRIMARY);
        if (lQty != io.lQuantityEntered)
        {
           csPIM.Format(csFormat, ps.GetPSText(RA_PIM_QTY_TXT, SCOT_LANGUAGE_PRIMARY));
           csFormat = csPIM + ps.GetPSText(RA_PIM_CONTINUED_ID, SCOT_LANGUAGE_PRIMARY);
           trace(L7, _T("PIM of '%s' (item %s, entry ID %d) - quantity change"),
                     csDesc, csCode, io.d.lEntryID);
        }
        if (lWgt != io.lWeightEntered)
        {
           csPIM.Format(csFormat, ps.GetPSText(RA_PIM_WT_TXT, SCOT_LANGUAGE_PRIMARY));
           csFormat = csPIM + ps.GetPSText(RA_PIM_CONTINUED_ID, SCOT_LANGUAGE_PRIMARY);
           trace(L7, _T("PIM of '%s' (item %s, entry ID %d) - weight change"),
                     csDesc, csCode, io.d.lEntryID);
        }
        if (lUPrc != io.lUnitPriceEntered)
        {
           csPIM.Format(csFormat, ps.GetPSText(RA_PIM_PRICE_TXT, SCOT_LANGUAGE_PRIMARY));
           trace(L7, _T("PIM of '%s' (item %s, entry ID %d) - unit price change"),
                     csDesc, csCode, io.d.lEntryID);
        }

        if (lPrc != io.d.lExtPrice)
        {
           if (csPIM.IsEmpty())
           {
              csPIM.Format(csFormat, ps.GetPSText(RA_PIM_EXTPRICE_TXT, SCOT_LANGUAGE_PRIMARY));
              trace(L6, _T("PIM of '%s' (item %s, entry ID %d) - extended price change"),
                        csDesc, csCode, io.d.lEntryID);
           }
           lItems -= lQty;  // Adjust number of items; new qty added later

           if (!co.IsInAssistMenus())
           {
               // Indicate PIM has occurred by equivalent of void
               csFormat = ps.GetPSText(RA_VOIDED_ID, SCOT_LANGUAGE_PRIMARY);
               csItemLine.Format(csFormat, csDesc, ps.FmtDollar(0 - lPrc, ps.SP_ERECEIPTCOL2));
               trace(L7, _T("+ra.OnVoidedItem, Message=%s"), csItemLine);
               ra.OnVoidedItem(csItemLine, lPrc, lTotalSale-lTaxDue);
               trace(L7, _T("-ra.OnVoidedItem"));

               // Output explanatory line
               trace(L7, _T("+ra.OnNormalItem, Message=%s"), csPIM);
               ra.OnNormalItem(RAP_RECEIPT_LINE_HIGHLIGHT + csPIM); // Display PIM reason on RAP
               trace(L7, _T("-ra.OnNormalItem"));
           }
        }
        else
        {
           trace(L2, _T("PIM of '%s' (item %s, entry ID %d) - unknown reason"),
                     csDesc, csCode, io.d.lEntryID);
        }

     } // TAR 257368 addition end
  }

  ps.AddItem();   //TAR#125677 used to be after security messages, but needed to be first
				  //so following code can assume current item on receipt is the item
				  //we are getting the details for.
  ps.ShowCMTotal(true ,false); //tar 270191

  //+SR678
    if (io.d.fCoupon && (-io.d.lUnitPrice > co.nOperationsMaxCouponLimit) && (co.nOperationsMaxCouponLimit >= 0)
	  && (!CouponTally(lTotalSale, lTotalCouponAmount)) && !bIsMaxCouponLimitInterventionDone
	  && !co.IsInAssistMenus() && !fStateAssist)
	{
		bIsMaxCouponLimitInterventionDone = true;
		if(bIsInFinalization)
		{
			bIsCouponExceedLimitFinalization = true;
		}
		else
		{
			return CouponMaxIntervention();
		}
	}
	//-SR678
      
  // count voids, set auth needed
  if (io.fVoid)
  {
	lVoidQty = 0;
	lVoidWgt = 0;
	lVoidPrc = 0;
	lVoidUPrc = 0;
	lVoidTcd = 0;
	lVoidBarcodeType = -1;

	if (nTBLastState==TB_ITEMSOLD)
	{
		lVoidedItems++;
		// the extended price is -ve for a voided item so to
		// add it, subtract it
		if (!fStateAssist)	  //TAR125677 Don't worry about approving voids if in assist mode
		{
			lItemSalesAmountVoided -= io.d.lExtPrice;	// Mgg Item only no coupons
		}
	}

	// remote approval
	// voided item
	/* TAR 297521
	csFormat = ps.GetPSText(RA_VOIDED_ID, SCOT_LANGUAGE_PRIMARY);
	csItemLine.Format(csFormat, io.d.csDescription, ps.FmtDollar(io.d.lExtPrice, ps.SP_ERECEIPTCOL2));
	trace(L7, _T("+ra.OnVoidedItem, csItemLine=%s, lExtPrice=%d, lTotalSale-lTaxDue=%d"), csItemLine, io.d.lExtPrice, lTotalSale-lTaxDue);
	ra.OnVoidedItem(csItemLine, io.d.lExtPrice, lTotalSale-lTaxDue);
	trace(L7, _T("-ra.OnVoidedItem"));*/

	// if this is a multiple coupon void the item code may have been
	// trashed by the first item, reset it here
	if (*io.csItemCode==VOID_ITEM)
	  io.csItemCode = io.csOrigCode;
  }
  else
  {
	  //TAR#125677 store whether or not item was sold in assist mode (and therefore can be
	  //automatically approved). Restricted Items are not autmatically approved in assist
	  //mode, because the cashier still needs to enter the age, and might not recognize the
	  //item as restricted.
	  bAutoApprovedItem = fStateAssist && !io.d.fRestricted;
	  ps.SetCurrentItemAutoApprovalFlag(bAutoApprovedItem);  //saved in case item is voided
  }

  // If this is a visual item
  if (io.d.fVisualVerify)
  {
	if (io.fVoid) // voiding a visual item
	{
	  if (lVisualVerifyItems)
		lVisualVerifyItems--;
	  if (lUnapprovedVisualVerifyItems && !bAutoApprovedItem) //TAR#125677 Only worry about approving vv items if not in assist mode
	  {
		lUnapprovedVisualVerifyItems--;
		if (lUnapprovedVisualVerifyItems == 0)
		{
			trace(L7, _T("+ra.OnVisualValidationApproval"));
			ra.OnVisualValidationApproval();
			trace(L7, _T("-ra.OnVisualValidationApproval"));
		}
	  }
	  if (lRegularYellowNeedingApproval && !bAutoApprovedItem) //TAR#125677 Only worry about approving vv items if not in assist mode
		lRegularYellowNeedingApproval--;

	  if (lRegularYellowNeedingApproval == 0)
	  {
		dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
							DM_TRICOLORLIGHT_OFF,
							DM_TRICOLORLIGHT_NORMALREQUEST);
	  }
	  io.lVisualCode = 2;			 // no warning needed for void
	}
	else // selling a visual item
	{
		csFormat = ps.GetPSText(RA_NORMAL_ITEM_FMT_ID, SCOT_LANGUAGE_PRIMARY);
		csItemLine.Format(csFormat, io.d.csDescription, ps.FmtDollar(io.d.lExtPrice, ps.SP_ERECEIPTCOL2));
		trace(L7, _T("+ra.OnNormalItem, Message=%s"), csItemLine);
		ra.OnNormalItem(csItemLine);
		trace(L7, _T("-ra.OnNormalItem"));

		lVisualVerifyItems++;


		if (!bAutoApprovedItem || co.IsInAssistMenus() )  //TAR#125677 Only worry about approving vv items if not in assist mode
		{
			io.lVisualCode = 1;
			lUnapprovedVisualVerifyItems++;
			lRegularYellowNeedingApproval++;
			dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
								DM_TRICOLORLIGHT_ON,
								DM_TRICOLORLIGHT_NORMALREQUEST);

			CString csDesc;
			// csErrDescription may hold an alternate VV description.
			if (io.d.csErrDescription.IsEmpty())
			   csDesc = ps.GetPSText(RA_VISUAL_VALIDATE_ID, SCOT_LANGUAGE_PRIMARY);
			else
			   csDesc = io.d.csErrDescription + io.d.csDescription;

			CString csAnchorStateName = getSCOTStateName();//Tar 247393
			CString csInstruction;
			csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_LANEBTT_VISUALVALIDATIONITEM, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, co.IsSecurityDisableOn()==true ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), ps.GetPSText(RA_THIRDLINE_VISUALVALIDATIONITEM, SCOT_LANGUAGE_PRIMARY));
			trace(L7, _T("+ra.OnVisualValidationItem, Message=%s"), csInstruction);
			ra.OnVisualValidationItem(csInstruction, FALSE);
			trace(L7, _T("-ra.OnVisualValidationItem"));
		}
		else
		{
			io.lVisualCode = 2;  //no warning, automatically approved
		}
	}
  }

  // if this is a restricted item
  if (io.d.fRestricted)
  {

	if (io.fVoid) // voiding a restricted item
	{
	  //+ TAR209317 AutoVoided of restricted items (lRestCode == 3) not increment
	  // the following flags so don't need to decrement them here is being autovoided.
	  // They are decremented when birthdate is verified .
	  // AutoVoided also doesn't tell RAP in the first place so don't remove from RAP queue
	  if (io.lRestCode != 3)
	  {
		  if (lRestrictedItems)
			lRestrictedItems--;
		  if (lUnapprovedRestrictedItems && !bAutoApprovedItem)
			lUnapprovedRestrictedItems--;
		  if (lRegularYellowNeedingApproval && !bAutoApprovedItem)
			lRegularYellowNeedingApproval--;

		  //TAR 211080 LPM 080202 begin: if two restricted items but only one voided,
		  m_lAgerestrictQueuedtoRAP--;
		  if (m_lAgerestrictQueuedtoRAP == 0)
		  {
			// tell RAP to remove the age approval needed
			trace(L7, _T("+ra.OnAgeApproval"));
			ra.OnAgeApproval();
			trace(L7, _T("-ra.OnAgeApproval"));
		  }
		  //tar 211080 end
	  }
	  //- TAR209317 LPM071002

	  if (lRegularYellowNeedingApproval == 0)
	  {
		dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
							DM_TRICOLORLIGHT_OFF,
							DM_TRICOLORLIGHT_NORMALREQUEST);
	  }

	  io.lRestCode = 2;
	  if (restrictedItemNotAllowed())
	  {
		io.lRestCode = 4;	 // void of not allowed rest'd item
	  }
	}
	else	// selling a restricted item
	{
	  if (restrictedItemNotAllowed() && ( co.fOperationsAutoVoidUnderageRestrict))	 //Feng122299
	  {

		io.lRestCode = 3;					  // must void this item
	  }
	  // see if this restricted type has already been approved
	  else if ( io.d.nRestrictedAge <= nCustomerAge )
	  {
		lRestrictedItems++;


		io.lRestCode = 2;				  // ok, no msg, no phrases, no lights
		// remote approval
		// After restricted item is approved, treat it as normal item FZ080299
		// If customer's age is greater than the restricted age,
		// we have to set AutoApprovalFlag to true.
		bAutoApprovedItem = true;
		ps.SetCurrentItemAutoApprovalFlag(bAutoApprovedItem);  //saved in case item is voided

		csFormat = ps.GetPSText(RA_NORMAL_ITEM_FMT_ID, SCOT_LANGUAGE_PRIMARY);
		csItemLine.Format(csFormat, io.d.csDescription, ps.FmtDollar(io.d.lExtPrice, ps.SP_ERECEIPTCOL2));
		trace(L7, _T("+ra.OnNormalItem, Message=%s"), csItemLine);
		ra.OnNormalItem(csItemLine);
		trace(L7, _T("-ra.OnNormalItem"));
	  }
	  else
	  {
		  //If we already have customer age and AutoVoidUnderageRestrict flag is yes,
		  //must void this item
		  if(nCustomerAge && co.fOperationsAutoVoidUnderageRestrict)
			io.lRestCode = 3;
		  else
		  {
			lRestrictedItems++;

			// Feng122299 Selling restricted item in assist mode still need approval. Since store people may not know this is a restricted item.
			// Need input customer age to sell it.
			//	if (!bAutoApprovedItem) 		   //TAR#125677 Don't worry about approving rest items if in assist mode

			io.lRestCode = 1;				   // show app needed msg
			lUnapprovedRestrictedItems++;
			lRegularYellowNeedingApproval++;
			dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
								DM_TRICOLORLIGHT_ON,
								DM_TRICOLORLIGHT_NORMALREQUEST);

			// remote approval
			csFormat = ps.GetPSText(RA_NORMAL_ITEM_FMT_ID, SCOT_LANGUAGE_PRIMARY);
			csItemLine.Format(csFormat, io.d.csDescription, ps.FmtDollar(io.d.lExtPrice, ps.SP_ERECEIPTCOL2));
			trace(L7, _T("+ra.OnNormalItem, Message=%s"), csItemLine);
			ra.OnNormalItem(csItemLine);
			trace(L7, _T("-ra.OnNormalItem"));
			int nYearRequired;
			COleDateTime dtToday;
			CString csValidBirthday;
			//CString csAgeRequired;
			dtToday = COleDateTime::GetCurrentTime();
			nYearRequired = dtToday.GetYear() - io.d.nRestrictedAge;
			//csAgeRequired.Format(ps.GetPSText(RA_AGEREQUIRED), io.d.nRestrictedAge);
			csValidBirthday.Format(ps.GetPSText(RA_VALIDBIRTHDAYREQUIRED, SCOT_LANGUAGE_PRIMARY), ps.DateFormat(dtToday.GetMonth(), dtToday.GetDay(), nYearRequired));
			//ra.OnNormalItem("[highlight]" + csAgeRequired);	// Display age required on RAP

			CString csAnchorStateName = _T("");//Tar 211737
			CString csDesc, csInstruction;
			csDesc = ps.GetPSText(RA_AGE_RESTRICTED_ID, SCOT_LANGUAGE_PRIMARY);
			//Begin TAR 196356
			_TCHAR TempBuffer[20];
				_itot(io.d.nRestrictedAge, TempBuffer, 10); // turn the index into a string so it can be passed via Add below
			CString csAge(TempBuffer);
			csDesc += _T("\n") + csAge + _T(" ")
					  + ps.GetPSText(RA_AGERESTRICT_MINIMUM_AGE, SCOT_LANGUAGE_PRIMARY)
					  + _T("\n") + csValidBirthday;
			//End TAR 196356
			csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_LANEBTT_RESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, co.IsSecurityDisableOn()==true ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), ps.GetPSText(RA_THIRDLINE_RESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY));
			trace(L7, _T("+ra.OnAgeRestrictedItem, Message=%s"), csInstruction);
			ra.OnAgeRestrictedItem(csInstruction, io.d.nRestrictedAge, FALSE, co.fStateDOBNotNeededButton);
			trace(L7, _T("-ra.OnAgeRestrictedItem"));
			//TAR 196910 start
			m_lAgerestrictQueuedtoRAP++;
			//TAR 196910 end
		  }
	  }
	}
  }


  // count coupons, set auth needed
  if (io.d.fCoupon && (io.xd.ExceptionCode!=XAPPROVALNONE))
  {
	if (io.fVoid)
	{
	  lVoidedCoupons++;

	 // RFC 371075
	 if( !co.fOperationsIncludeCouponsInItemCount )
		{ms_lExcludedItems--;}

	  if (lCoupons) lCoupons--;
	  io.lCoupCode = 2; 		 // no warning needed for void
	  CString csTmp;

	  // Tar 228716 csPrevCoupon no longer contains scan prefix


	  // so need to adjust csItemCode before comparison
	  int nItemCodeLen = io.csItemCode.GetLength();
	  if ( nItemCodeLen && (!_istdigit(io.csItemCode[0])))
	  {
		// item code is with scan type prefix
		// remove alpha characters from UPC, assumes prefix is never more than one character
	 	csTmp = io.csItemCode.Right(nItemCodeLen - 1);
	  }
	  if((--nItemCodeLen > 0) && (!_istdigit(io.csItemCode[1]))) //tar 237560	// possible ean8 with double prefix
	  {
	 	csTmp = io.csItemCode.Right(nItemCodeLen - 1);
	  }

	  if ((csPrevCoupon == csTmp) && (nCountPossibleDuplicateCoupon > 0))
		nCountPossibleDuplicateCoupon--;

	  if (!bAutoApprovedItem)	 //TAR#125677 Only worry about approving coupon voids if not in assist mode
	  {
		// the extended price is +ve for a voided coupon so to
		// just add it
		lCouponAmountVoided += io.d.lExtPrice;
		// A voided coupon has a positive extended price, so simply
		// subtract it from the totals.
		if (lTotalCouponAmount) lTotalCouponAmount -= io.d.lExtPrice;

		if (lUnapprovedCoupons) lUnapprovedCoupons--;

		if(io.xd.ExceptionCode==XAPPROVALALWAYS)
			lExceptionItemsRequiringApproval--;

		// tar 228747 is voided coupon results in not needing
		// approval any longer, call approval
		if (!isCouponRatioApprovalRequired())
		{
			// remote approval
			trace(L7, _T("+ra.OnCouponApproval"));
			ra.OnCouponApproval();
			trace(L7, _T("-ra.OnCouponApproval"));
		}
	  }
	}
	else // not voided, sold
	{
	  lCoupons++;
      trace(L5, _T("TBItemDetails: lCoupons=%d"), lCoupons);
      trace(L5, _T("TBItemDetails: SASendSCOTInput CANCEL_ITEM"));

	  // RFC 371075
	  if( !co.fOperationsIncludeCouponsInItemCount )
		{ms_lExcludedItems++;}


	  SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
	  csFormat = ps.GetPSText(RA_NORMAL_ITEM_FMT_ID, SCOT_LANGUAGE_PRIMARY);
	  csItemLine.Format(csFormat, io.d.csDescription, ps.FmtDollar(io.d.lExtPrice, ps.SP_ERECEIPTCOL2));

	  if (!bAutoApprovedItem) //TAR#125677 Only worry about approving coupons if not in assist mode
	  {
		lUnapprovedCoupons++;
		lTotalCouponAmount += -(io.d.lExtPrice);
		io.lCoupCode = 1;
		//If the item is coupon and in customer mode, check for coupon tally and duplication.
		//If the coupon needs approval aways, ask for approval

//USSF Start
		CString csTempstr;
		csTempstr.Format(_T("%d"), io.d.lExtPrice);
		if (co.fOperationsEnableUSSF)
		{
		USSFDATA(_T("CouponValue"), csTempstr)
		USSFEVENT(_T("CheckCoupon"), ;, ; )
		}
//USSF End

		if(io.xd.ExceptionCode==XAPPROVALALWAYS)
		{
			lExceptionItemsRequiringApproval++;
			dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
								DM_TRICOLORLIGHT_ON,
								DM_TRICOLORLIGHT_NORMALREQUEST);
			CString csAnchorStateName = getSCOTStateName();//Tar 247393
			CString csDesc, csInstruction;
			// Begin TAR 197558
			// Give description as Coupon Threshold exceeded.
			// Begin TAR 198466
			// This should have never been commented out by CH123556
			// Redoing the fix of TAR 197558
			csDesc = ps.GetPSText(TXT_COUPON_THRESHOLD_EXCEEDED, SCOT_LANGUAGE_PRIMARY);
			// End TAR 198466
			// end TAR 197558
			csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(TXT_COUPON, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, co.IsSecurityDisableOn()==true ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), ps.GetPSText(RA_THIRDLINE_QUANTITYRESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY));
			trace(L7, _T("+ra.OnCouponItem, csInstruction=%s, lExtPrice=%d"), csInstruction, io.d.lExtPrice);
			ra.OnCouponItem(csInstruction, io.d.lExtPrice, true);
			trace(L7, _T("-ra.OnCouponItem"));
		}
		else
		{
			if(CouponTally(lTotalSale, lTotalCouponAmount) || IsDuplicateCoupon())
			{
				CString csAnchorStateName = getSCOTStateName();//Tar 247393
				CString csDesc, csInstruction;
				// Begin TAR 197558 Give description as Coupon Threshold exceeded.
				// Begin TAR 198466 This should have never been commented out by CH123556
				// Redoing the fix of TAR 197558
				csDesc = ps.GetPSText(TXT_COUPON_THRESHOLD_EXCEEDED, SCOT_LANGUAGE_PRIMARY);
				// End TAR 198466 // end TAR 197558
				csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(TXT_COUPON, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, co.IsSecurityDisableOn()==true ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), ps.GetPSText(RA_THIRDLINE_QUANTITYRESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY));
				trace(L7, _T("+ra.OnCouponItem, csInstruction=%s, lExtPrice=%d"), csInstruction, io.d.lExtPrice);
				ra.OnCouponItem(csInstruction, io.d.lExtPrice, true);
				trace(L7, _T("-ra.OnCouponItem"));
			}
		}

	  }
	  else
	  {
		io.lCoupCode = 2;  //no msg in assist mode
		trace(L7, _T("+ra.OnNormalItem, Message=%d"), csItemLine);
		ra.OnNormalItem(csItemLine);
		trace(L7, _T("-ra.OnNormalItem"));
	  }
	}
  }

  long lCnt= io.d.lQtySold;
  if (!lCnt) lCnt = 1;

  if (io.fVoid)
  {
	  lItems -= lCnt;	// one less item
  }

  else
  {
	  lItems += lCnt;			// count of items or coupons etc
	  trace(L7, _T("+rp.ItemsID"));
	  rp.ID(RP_IDTYPE_ITEMS,RP_IDOPERATION_INCREMENT);
	  trace(L7, _T("-rp.ID"));

  }

  if(co.fStateDisplayNumberOfItemsOnRAP)
  {
  	  CString csNumItems = EMPTY_STRING;
	  csNumItems.Format(_T("%d"), GetDisplayedItemCount());	// RFC 371075
	  trace(L7, _T("+ra.OnNormalItem, Sending the number of items to RAP"));
	  ra.OnNormalItem(_T("[NumItems]") + csNumItems);
	  trace(L7, _T("-ra.OnNormalItem, NumItems = %s"), csNumItems);
  }

  if (!io.d.bLinkedItem)
	  m_lItemsScanned++;		//TAR 202845

  //*********************************************************************************************//
  //* PRICE EMBEDDED - FOR 3.0 APP/SECURITY MERGE *
  //;Should price embedded BagScale weight be checked to see if correct (expected) weight, Y/N.
  //;  This can be ScannerScale weight or PLU "Weight Field".
  //;  TB supplied will only work if ForceWeighingOFPriceEmbedded=N.
  //SubstCkPriceEmbeddedWeights

  //;Should Price Embedded items be weighed on the ScannerScale, Y/N.
  //;  If Y, TB supplied "Weight Field" will not be used.
  //;  Yes will only work if SubstCkPriceEmbeddedWeights=Y
  //ForceWeighingOFPriceEmbedded
  //*********************************************************************************************//

  //+226745 re-write Price Embedded Substitution Check (Wt Comparison) LPM022003
  //If Subst Ck is required on price embedded and TB->lWeight > 0, App supplies wt to SmartScale instead of WLDB.
  //If Subst Ck is required on price embedded and want to use the ScannerScale wt, must set ForceWeighingOFPriceEmbedded=Yes
  //To stop this from happening, set TB->lWeight = 0 (App uses io.d.lWgtSold).

  //
  //PLU supplied TB->lWeight > 0 will only work if ForceWeighingOFPriceEmbedded=N.
  //
  //Examples:
  //SubstCkPriceEmbeddedWeights=Y, lUnitPrice > 0, lWeight == 0, will use calculated wt from unit price and will NOT update the WLDB
  //SubstCkPriceEmbeddedWeights=Y, lUnitPrice == 0, lWeight == 0. will use WLDB entries and WILL update the WLDB
  //SubstCkPriceEmbeddedWeights=Y, lUnitPrice > 0, lWeight > 0, will use calculated wt from unit price and will NOT update the WLDB
  //SubstCkPriceEmbeddedWeights=Y, lUnitPrice == 0, lWeight > 0. will use supplied weight and will NOT update the WLDB
  //Can still use the scanner scale weight with SubstCkPriceEmbeddedWeights=Y and will NOT update the WLDB
  //SubstCkPriceEmbeddedWeights=N will not compare either lUnitPrice calc or TB supplied lWeight.
  //
  if (io.b_gIsThisItemPriceEmbedded && !io.fVoid)	// if this is a price embedded (and not a void)
  {
	trace(L6, _T("+Price Embedded item. Compare Weights of Price Embedded items = %d."), g_bSubstCkPriceEmbeddedWeights);
	if (g_bSubstCkPriceEmbeddedWeights) // if the securityscale wt should be compared to something
	{
	  if (!co.fStateForceWeighingOFPriceEmbedded)
	  {
		// if Not using scanner scale weight and a TB supplied weight exists, not using WLDB entries
		if (io.d.lWgtSold > 0)
		{

			// if a unit price exists, then calculate the Price Embedded expected wt
			if (io.lUnitPriceEntered > 0)
			{
				//possible loss of data compile warning on line below is intentional and OK : DE200005
				io.l_gPriceEmbeddedWt = (long)(((double)io.lExtPriceEntered / io.lUnitPriceEntered) * 1000.0);
				trace(L2, _T(" TB passed Unit Price information, lUnitPrice = %d, using calculated weight for Wt Comparison (ExtendedPrice divided by UnitPrice)."), io.lUnitPriceEntered);
			}
			// else use the TB supplied weight
			else
			{
				io.l_gPriceEmbeddedWt = io.d.lWgtSold;
				trace(L6, _T(" TB passed weight information, lWeight = %d, using this weight for Wt Comparison."), io.lTotalWeightEntered);

			}
		}
		else if ( 0 == io.d.lWgtSold )
		{
			trace(L6, _T(" TB passed no weight information. Will Update the WLDB with this entry."));
			// if a unit price exists, then calculate the Price Embedded expected wt
			if (io.lUnitPriceEntered > 0)
			{
				//possible loss of data compile warning on line below is intentional and OK : DE200005
				io.l_gPriceEmbeddedWt = (long)(((double)io.lExtPriceEntered / io.lUnitPriceEntered) * 1000.0);
				trace(L2, _T(" TB also passed Unit Price information, lUnitPrice = %d, using calculated weight for Wt Comparison (ExtendedPrice divided by UnitPrice). Will only learn this item if SS fires OnMatchedWt."), io.lUnitPriceEntered);
			}
			else
			{
				io.l_gPriceEmbeddedWt = 0;
				io.lTotalWeightEntered = 0;
				trace (L6, _T(" Using WLDB entries for Wt Comparison. Will learn this item if no WLDB entries exist."));
			}
		}
	  }
	  else
	  {
		trace(L6, _T(" SCOTOption ForceWeighingOFPriceEmbedded = %d. Using scanner scale data for Wt Comparison."), co.fStateForceWeighingOFPriceEmbedded);
	  }
	}
	// end of price embedded special cases
	trace(L6, _T("-Price Embedded item."));
  }//-226745	LPM022003

  if (io.lRestCode<3)
  {
	long lAmt = io.d.lExtPrice;
	//if (io.fVoided && lAmt<0) lAmt = 0-lAmt;	// suppress the 'Savings' phrase on voided items
	if (!io.fVoid)
	{
	  // Tar 120586
	  if(io.d.fCoupon && !io.d.lExtPrice)	// For zero price coupon, play "Coupon Accepted"
	  {
		DMSayPhrase(COUPONACCEPTED);
	  }
	  else if(io.xd.ExceptionCode==XAPPROVALNONE  && (io.xd.dwExceptionFlags & TBXF_SUPPRESSPRICEDISPLAY) )
	  {
		//suppress price display and sound
		if(!io.xd.csExceptionWav.IsEmpty())
		  DMSayPhrase(io.xd.csExceptionWav);
		else
		  DMSayPhrase(ITEMPROCESSED);
	  }
	  else
	  {
		DMSayAmount(lAmt);
	  }
	}
  }

	// update security agent

	if (io.lRestCode<3 && (!io.d.bLinkedItem ||							// if not going to undo this item
		(*io.csItemCode!=LINK_ITEM)))			// and this is not a linked item (no weight)
	{
		//	If void item(s)
		if (io.fVoid)
		{
			//	if item scanned else manual item
			if (io.cScanType!=0)
			{ // if a coupon
				if (io.d.fCoupon)
				{
					// BeginCodeLocaleUPC
					//if (io.csItemCode.GetLength())
					if (!io.d.bLinkedItem)	  // tar 125389
                    {
                        trace(L5, _T("TBItemDetails: SASendSCOTInput COUPON_VOID_SCAN"));
						SASendSCOTInput( COUPON_VOID_SCAN, io.csItemCode, 0, io.d.lExtPrice, io.lQuantityEntered);
                    }
					// EndCodeLocaleUPC
				}
				else
				{
					// normal item
					// BeginCodeLocaleUPC
					// if (io.csItemCode.GetLength())
					if (!io.d.bLinkedItem)	  // tar 125389
                    {
                        trace(L5, _T("TBItemDetails: SASendSCOTInput ITEM_VOID_SCAN"));
						SASendSCOTInput( ITEM_VOID_SCAN, io.csItemCode, 0, io.d.lExtPrice, io.lQuantityEntered);
                    }
					// EndCodeLocaleUPC
				}
			}
			else
			{	// if a coupon
				if (io.d.fCoupon)
				{
					// BeginCodeLocaleUPC
					// if (io.csItemCode.GetLength())
					if (!io.d.bLinkedItem)	  // tar 125389
					{
                        trace(L5, _T("TBItemDetails: SASendSCOTInput COUPON_VOID_MANUAL"));
						SASendSCOTInput( COUPON_VOID_MANUAL, io.csItemCode,
								io.lWeightEntered, io.d.lExtPrice, io.lQuantityEntered);
					}
					// EndCodeLocaleUPC
				}
				else
				{ // if a normal item
					// BeginCodeLocaleUPC
					//if( io.csItemCode.GetLength())   //Tar 119730 // if csItemcode=="", means it is linked item or
					if (!io.d.bLinkedItem)	  // tar 125389
					{
						//if ( !fInMultiSelectPickList )
						//{
                            trace(L5, _T("TBItemDetails: SASendSCOTInput ITEM_VOID_MANUAL"));
							// electronical coupon, don't send message to security
							SASendSCOTInput(ITEM_VOID_MANUAL,
											io.csItemCode,
											( io.xd.ExceptionCode == XAPPROVALNONE ) ? 0 : io.lWeightEntered,  // No weight for this exception type
											io.d.lExtPrice,
											io.lQuantityEntered);
						//}
					}
				}
			}
		}
		else
		{
			//	sale item
			//	if item has a scanned type is will not be zero
			if (io.cScanType!=0)
			{ // if a coupon
				if (io.d.fCoupon)
				{
					// BeginCodeLocaleUPC
					// if (io.csItemCode.GetLength())
					if (!io.d.bLinkedItem)	  // tar 125389
					{
						//T.I.D
						//if ((IsDuplicateCoupon(io.d.csItemCode)==true) &&(CouponTally(GrossSales,CouponTotal)==true))
						//{
						//RETURNSTATE(CouponNotAllowed);


					//	SASendSCOTInput( COUPON_REDEEMED_SCAN, io.csItemCode,		//cm150000: Merged 2
					//			0, io.d.lExtPrice, io.lQuantityEntered );						// cm150000: Merged 2
					}
					// EndCodeLocaleUPC
				}
				else
				{ // normal item
					// + TAR 313046
					if( (io.d.lRequireSecBagging == TB_OVERRIDE_SECURITY_WITH_NOWEIGHT )
						|| ( (io.d.lExtPrice < 0 ) 
						&& (TB_OVERRIDE_SECURITY_WITH_YES != io.d.lRequireSecBagging) )) //413251
					{
						// TAR 409946 - Do not send cancel if item is sold under the Multi Item Picklist.
						if(!fInMultiSelectPickList)
						{
							trace(L5, _T("TBItemDetails: SASendSCOTInput CANCEL_ITEM"));
							SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
						}
					}
					// - TAR 313046
					else
					{
						if (!io.d.bLinkedItem)	  // tar 125389
						{
							if(!io.bBizerbaScale)
							{
                                trace(L5, _T("TBItemDetails: SASendSCOTInput ITEM_SOLD_SCAN"));

								SASendSCOTInput(	ITEM_SOLD_SCAN,
												io.csItemCode,
												0,
												io.d.lExtPrice,
												io.lQuantityEntered,
												io.d.lDepartment,
												io.b_gIsThisItemPriceEmbedded	);
							}
							else
							{
                                trace(L5, _T("TBItemDetails: SASendSCOTInput ITEM_SOLD_SCAN"));

								SASendSCOTInput(	ITEM_SOLD_SCAN,
												io.csItemCode,
												io.lWeightEntered,
												io.d.lExtPrice,
												io.lQuantityEntered,
												io.d.lDepartment,
												io.b_gIsThisItemPriceEmbedded	);
							}

						}
					}
				}
			}
			else		//else no scan type so must be manually entered item
			{	// if a coupon
				if (io.d.fCoupon)
				{
					if (!io.d.bLinkedItem)	  // tar 125389
					{
				//			SASendSCOTInput( COUPON_REDEEMED_MANUAL, io.csItemCode,
				//					io.lWeightEntered, io.d.lExtPrice, io.lQuantityEntered);
					}

				}
				//RFC255148+
				else if( (io.d.lRequireSecBagging == TB_OVERRIDE_SECURITY_WITH_NOWEIGHT )
						|| ( (io.d.lExtPrice < 0 ) 
						&& ( TB_OVERRIDE_SECURITY_WITH_YES != io.d.lRequireSecBagging) )) //413251

				{
					// TAR 409946 - Do not send cancel if item is sold under the Multi Item Picklist.
					if(!fInMultiSelectPickList)
					{
						trace(L5, _T("TBItemDetails: SASendSCOTInput CANCEL_ITEM"));
						SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);	// + TAR 313046
					}
				}
				//RFC255148-
				else
				{
					/*if (fInMultiSelectPickList)//SR679
					{
						RETURNSTATE(ProduceFavorites);
					}*/

					// if a normal item
					//Tar 119730 // if csItemcode=="", means it is linked item or
					// electronical coupon, don't send message to security
					// if (io.csItemCode.GetLength())
					if (!io.d.bLinkedItem)	  // tar 125389
					{
						if(!io.bBizerbaScale)
						{
                            trace(L5, _T("TBItemDetails: SASendSCOTInput ITEM_SOLD_MANUAL"));
							SASendSCOTInput(	ITEM_SOLD_MANUAL,
												io.csItemCode,	//TAR123822  LPM101399 SMStateSABase now determines if it is price embedded
												( io.xd.ExceptionCode == XAPPROVALNONE ) ? 0 : io.lTotalWeightEntered,	// No weight for this exception type
												io.d.lExtPrice,
												io.lQuantityEntered,
												io.d.lDepartment,
												io.b_gIsThisItemPriceEmbedded	);
						}

						else
						{
                            trace(L5, _T("TBItemDetails: SASendSCOTInput ITEM_SOLD_MANUAL"));
							SASendSCOTInput(	ITEM_SOLD_MANUAL,
												io.csItemCode,	//TAR123822  LPM101399 SMStateSABase now determines if it is price embedded
												io.lWeightEntered,
												io.d.lExtPrice,
												io.lQuantityEntered,
												io.d.lDepartment,
												io.b_gIsThisItemPriceEmbedded	);
						}
					}
				}
			}
		}
	}

	if (fInMultiSelectPickList)//SR773
	{
		RETURNSTATE(ProduceFavorites);
	}

  // remote approval
  if (!io.d.fRestricted && !io.d.fCoupon && !io.fVoid && !io.d.fVisualVerify)
  {
	 // normal item
	  // if item is exception of type XAPPROVALNONE and TBXF_SUPPRESSPRICEDISPLAY
	  // display exception category description and do not show price
		CString csUnitInfo;
	  if ((io.xd.ExceptionCode == XAPPROVALNONE) &&
		 (io.xd.dwExceptionFlags & TBXF_SUPPRESSPRICEDISPLAY))
	  {
		  csItemLine = io.xd.csCategory;
	 }
	 else
	 {
	   csFormat = ps.GetPSText(RA_NORMAL_ITEM_FMT_ID, SCOT_LANGUAGE_PRIMARY);
	   csItemLine.Format(csFormat, io.d.csDescription, ps.FmtDollar(io.d.lExtPrice, ps.SP_ERECEIPTCOL2));

	/******cm150000: Removed In Merged Code******/
	//CString csWeightInfo = ps.FmtWeightInfo();
	//if (csWeightInfo.GetLength() > 1)
	//{
	//	CString temp = csWeightInfo;
	//	temp.TrimLeft();
	//	temp = temp.SpanIncluding("0123456789.");
	//	if (temp.SpanExcluding("123456789") != temp)
	//	{
	//		csItemLine += CString("\\n") + csWeightInfo;
	//		csItemLine.Format(csFormat, io.d.csDescription + CString(" ") + csWeightInfo, ps.FmtDollar(io.d.lExtPrice, ps.SP_ERECEIPTCOL2));
	//	}
	//}
	/******cm150000: End Remove Merge Code******/

	/******cm150000: Added In Merged Code******/
		if (io.d.lWgtSold)
			csUnitInfo = ps.FmtWeightInfo();
		else if (io.d.lQtySold > 1)  //Tar 236946
			csUnitInfo = ps.FmtQuantityInfo();
	/******cm150000: End Merged Code***********/
	 }
	trace(L7, _T("+ra.OnNormalItem, Message=%s"), csItemLine);
	ra.OnNormalItem(csItemLine);
	trace(L7, _T("-ra.OnNormalItem"));

	/******cm150000: Added In Merged Code******/
	if (!csUnitInfo.IsEmpty())
		{
		ra.OnNormalItem(csUnitInfo);
		}
	/******cm150000: End Merged Code***********/
  }

  if ( ((io.d.fRestricted==1) && (io.lRestCode != 2)) && (!io.d.bLinkedItem) && (!io.fVoid) || ((io.d.fVisualVerify==1) && (!io.fVoid)) )
  {
    if (!co.IsImmedInterventionOn())  //Don't say it if intervention will be immediately handled
    {
        DMSayPhrase(APPROVALNEEDED);
    }
  }
  else if (io.lCoupCode==1 &&
		  (io.xd.ExceptionCode!=XAPPROVALTHRESHOLDS && io.xd.ExceptionCode!=XAPPROVALALWAYS) //tar 164159
		  && (!IsUsingCouponSensor())) //not using coupon sensor
	DMSayPhrase(SETCOUPONSASIDE);

  // save last item, if it was voided mark it un-voidable
  if (io.fVoid && !io.d.bLinkedItem)
  {
	if (io.lRestCode==4 || io.lRestCode==0 || io.lRestCode == 2 )
	  DMSayPhrase(ITEMCANCELED);  //Tar 129588
	else
	  DMSayPhrase(ITEMVOIDED); //Voided Items
		CString csWork = VOID_ITEM;
	csItemSent = csWork + csItemSent;
  }

  //ts.Total(); 					// force a re-total

  nTBLastState = TB_ITEMSOLD;	// this was an item entry, set that state

  // item has been sold and all tallies and screens update
  // if this was a restricted item that should not have
  // been sold, void it to reverse the tallies
  if (io.lRestCode==3 && !io.fVoid)
	{
		CKeyValueParmList parmList;
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_IV_OPERATION, KEY_IV_OPERATION_BEGIN);
		_bstr_t bstrParmList((const _TCHAR *)parmList);
		SecMgr.VoidItem(lSecMgrCookie, bstrParmList);

		io.fVoid = true;
		return TBEnterItem();	  // void this rest'd item
	}

  //go to Assistance Needed screen and send voided item info to rap
  if (io.fVoid && !fStateAssist && !io.d.bLinkedItem && !co.IsInAssistMenus())
  {
	  bIsFromVoidApproval = true;

	  g_lNumberOfWLDBEntriesThisItem = -1;
	  if (fInMultiSelectPickList)
	  {
		  RETURNSTATE(VoidItem);
	  }
	  
	  CREATE_AND_DISPENSE(VoidApproval)(BES_STATERETURN, BES_SCANANDBAG); //TAR240141-
  }  
	
  if (io.lRestCode==4)
  {
	// It does no good to do this from Assist Mode - no store
	// mode state should do it, for that matter.
	if (storeMode() && !co.IsInAssistMenus())
	  return STATE_NULL;

	lRestrictedNotAllowedItems++;

	// if (io.csItemCode.GetLength())
	if (!io.d.bLinkedItem)	  // tar 125389
    {
       trace(L5, _T("TBItemDetails: SASendSCOTInput CANCEL_ITEM"));
	   SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
    }

	CREATE_AND_DISPENSE(RestrictedNotAllowed)(restrictedItemMessage());
  }

  // linked items may follow this item without our
  // recording a new item code, convert this item code
  // it indicate the next item may be a LINK_ITEM so
  // ps.AddItem will still work next time through
  CString csWork = LINK_ITEM;
  io.csOrigCode = csWork + io.csOrigCode;
  io.csItemCode = csWork + io.csItemCode;

  // changed for 3.0 LPM0122302 	 T.I.D
  if (	(!io.fVoid) &&								//not voided item
		(!io.d.bLinkedItem) &&						//not linked item
		(!io.d.fCoupon) &&							//not coupon
		(TB_OVERRIDE_SECURITY_WITH_NOWEIGHT != io.d.lRequireSecBagging ) && // RFC255148, no weight expected
		(io.d.lExtPrice >= 0 ) ||					//positive price item
		( (io.d.lExtPrice < 0 ) && (TB_OVERRIDE_SECURITY_WITH_YES == io.d.lRequireSecBagging) )	 )	// 212891 TB needs to be able to goto BagAndEAS if sell negative priced items & TB override bagging required. LPM021203
  {
	 RETURNSTATE(BagAndEAS);			
  }
  else if ( (io.d.lExtPrice < 0 ) &&
			(TB_LET_SECURITY_DECIDE == io.d.lRequireSecBagging) )		// 212891 TB does not require the negative priced item to be bagged, need to enable the scanner. LPM021203
  {
	 bEASReEnableScanner = true;	// going back to ScanAndBag, must ok re-enable here
  }
  //RFC255148+
  else if (TB_OVERRIDE_SECURITY_WITH_NOWEIGHT == io.d.lRequireSecBagging )    //TB indicated no Weight Item
  {
      if(!fStateAssist && dm.fStateTakeawayBelt)	//TAR 229218 Missing from the merge (3.0.2 to 3.1)
			DMTakeawayBeltReset();		// since tell TAB about scan sooner , have to reset if cancelled
		g_lNumberOfWLDBEntriesThisItem = -1;//TAR 196952
		bEASReEnableScanner = true;
		g_bWtExpected = false;

//409479+ No need to tell SM to resume. 
        /* 
		CKeyValueParmList parmList;
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_RESUME);
		_bstr_t bstrParmList((const _TCHAR *)parmList);
		BSTR bstrResult;
		bstrResult = NULL;
		SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
		SysFreeString(bstrResult);
        */
 //409469-

		fSAInItemSale = false; // Fix coupon scanning from RAP issue (TAR: 209401)
           

  }
  //RFC255148-
  else if (io.d.bZeroWeightItem)    //TB indicated Zero Weight Item
  {
	 bEASReEnableScanner = true;	// going back to ScanAndBag, must ok re-enable here
  }

  if (io.d.bLinkedItem ) //TAR 233881 & 233884
  {
	 return STATE_NULL;
  }

  // Return to coupons if appropriate, all other cases go to S&B
  if (io.d.fCoupon && bEnterTenderState)
  {
	  if (IsUsingCouponSensor() && (!fStateAssist) && (!co.IsInAssistMenus()))
	  {
		  CREATE_AND_DISPENSE(InsertCoupon)(getAnchorState(), BES_ENTERCOUPONS);
	  }
	  else
		  RETURNSTATE(EnterCoupons)
  }

  //TAR220913+ CHU 12/06/02
  //Added another condition. If the anchorState is Finish, we want to stay at Finish
  //not going to ScanAndBag. For example, we could be at Finish, then go to AssistMode to void
  //an item which should back to Finish not ScanAndBag
  // TAR 164166 -if ((getAnchorState() == BES_ENTERCOUPONS) && io.d.fCoupon)
  //if (getAnchorState() == BES_ENTERCOUPONS)
  if (getAnchorState() == BES_ENTERCOUPONS || getAnchorState() == BES_FINISH)
  //TAR220913-
	 return createAnchorState();

  //if coupon sensor is configured, go to Insert Coupon screen
  if (io.d.fCoupon
		&& (!io.d.fVoid)
		&& (!fStateAssist)
        && (!co.IsInAssistMenus())
		&& IsUsingCouponSensor())
  {
	 CREATE_AND_DISPENSE(InsertCoupon)(getAnchorState(), BES_SCANANDBAG);
  }
  //TAR291610+: We found this problem in GiantEagle
  //If TB returns 2nd ItemSold for a discount of an item and the item is still on the belt
  //We want to go back to Transport state and let TAB to tell us to change to ScanAndBag
  CString csStateName = mp.sCurr->GetRuntimeClass()->m_lpszClassName;
  if (dm.fStateTakeawayBelt && ( csStateName == _T("SMTransportItem") ))
  {
	  return STATE_NULL; //go back to Transport state
  }
  //TAR291610-

  //410299+
  if ( !co.IsInAssistMenus() && !fStateAssist )
  {
	if (TB_OVERRIDE_SECURITY_WITH_NOWEIGHT == io.d.lRequireSecBagging )
	{
	  trace(L6, _T("Return STATE_NULL because of TB_OVERRIDE_SECURITY_WITH_NOWEIGHT"));
      return STATE_NULL; //go back to Inprogress and let SM decide when to go to ScanAndBag
	}
	//413251+
	if ((TB_OVERRIDE_SECURITY_WITH_YES != io.d.lRequireSecBagging ) && (io.d.lExtPrice < 0 ))
	{
	  trace(L6, _T("Return STATE_NULL because of TB_OVERRIDE_SECURITY_WITH_NO and io.d.lExtPrice < 0"));
      return STATE_NULL; //go back to Inprogress and let SM decide when to go to ScanAndBag
	}
	//413251-
  }
  //410299-

  return setAndCreateAnchorState(BES_SCANANDBAG);
}


/////////////////////////////////
void SMStateBase::TBGetItemDetails(ItemDetails &iDetails)  //Get Item Details, transfer to item object.
{
	trace(L2, _T("::TBGetItemDetails - bEASReEnableScanner is set to false."));
	bEASReEnableScanner = false;
	SItemDetails   dItem;
	memset(&dItem, 0, sizeof(SItemDetails));
	dItem.lEntryID = INVALID_ENTRYID;
	// set up the defaults so that any TB doesn't have to always set these to default
	dItem.lRequireSecBagging = TB_LET_SECURITY_DECIDE;
	dItem.lRequireSecSubstChk = TB_LET_SECURITY_DECIDE;

	rc= tb.GetItemDetails(&dItem);
	CString strFmt ;

	strFmt	= _T("TBGetItemDetails--ItemDetail: lExtPrice:%d; lUnitPrice:%d; lQuantity:%d; lWeight:%d; lDealQuantity:%d; fCoupon:%d; ") ;
	strFmt += _T("fVoid:%d; fNeedQuantity:%d; fNeedWeight:%d; fRestricted:%d; fVisualVerify:%d; nRestrictedAge:%d; szDescription:%s; ") ;
	strFmt += _T("szItemCode:%s; fNeedPrice:%d; fNotFound:%d; fNotForSale:%d; lScanCodeType:%d; bLinkedItem:%d lDepartment:%d, entryid:%d; ") ;
	strFmt += _T("lRequireSecBagging:%d; lRequireSecSubstChk:%d; bSecurityTag:%d; bZeroWeightItem:%d, bPickListItem:%d, szItemID:%s");

	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt, dItem.lExtPrice,
															dItem.lUnitPrice,
															dItem.lQuantity,
															dItem.lWeight,
															dItem.lDealQuantity,
															dItem.fCoupon,
															dItem.fVoid,
															dItem.fNeedQuantity,
															dItem.fNeedWeight,
															dItem.fRestricted,
															dItem.fVisualVerify,
															dItem.nRestrictedAge,
															dItem.szDescription,
															dItem.szItemCode,
															dItem.fNeedPrice,
															dItem.fNotFound,
															dItem.fNotForSale,
															dItem.lScanCodeType,
															dItem.bLinkedItem,
															dItem.lDepartment,
															dItem.lEntryID,
															dItem.lRequireSecBagging,
															dItem.lRequireSecSubstChk,
															dItem.bSecurityTag,
															dItem.bZeroWeightItem,
															dItem.bPickListItem,
															dItem.szItemID);

	// extract the total taxes and the balance still due
	ProcessTBReturn(rc);

	iDetails.lExtPrice		 = dItem.lExtPrice;
	iDetails.lUnitPrice 	= dItem.lUnitPrice;
	iDetails.lQtySold		  = dItem.lQuantity;
	iDetails.lWgtSold		  = dItem.lWeight;
	iDetails.lDealQty		  = dItem.lDealQuantity;
	iDetails.fCoupon		   = dItem.fCoupon;
	iDetails.fVoid			   = (dItem.fVoid? true: false);
	iDetails.fQuantityRequired = dItem.fNeedQuantity;
	iDetails.fWeightRequired   = dItem.fNeedWeight;
	iDetails.fRestricted	   = dItem.fRestricted;
	iDetails.fVisualVerify	   = dItem.fVisualVerify;
	iDetails.nRestrictedAge  = dItem.nRestrictedAge;
	iDetails.csDescription	 = (LPTSTR)dItem.szDescription;
	iDetails.csItemCode 	   = (LPCTSTR)dItem.szItemCode;
   iDetails.csErrDescription = (LPCTSTR)dItem.szErrDescription;
	iDetails.fPriceRequired = dItem.fNeedPrice;
	iDetails.fNotFound	 = dItem.fNotFound;
	iDetails.fNotForSale = dItem.fNotForSale;
	io.lTareCode		 = dItem.nTareCode; // Matt from ACS reported this bug
	iDetails.bLinkedItem = dItem.bLinkedItem;
	if(!iDetails.bLinkedItem)
	{

		io.cScanType				= dItem.lScanCodeType;
		// If department is non zero, then use the the value in lDepartment field
		// otherwise if department is zero, then use the value in szExtendedDeptInfo
		if (0 != dItem.lDepartment ) // old way
		{
			g_csSavedDeptInfo.Format(_T("%d"), dItem.lDepartment);
		}
		else // new way
		{
			g_csSavedDeptInfo		= dItem.szExtendedDeptInfo;	// SR 141 - RFC 283384
		}
		io.csOrgItemDescription     = dItem.szDescription;		// Need to store orginal desc for every linked item
	}

	iDetails.lDepartment = dItem.lDepartment;
	iDetails.lRequireSecBagging = dItem.lRequireSecBagging;
	iDetails.lRequireSecSubstChk = dItem.lRequireSecSubstChk;
	iDetails.bSecurityTag = dItem.bSecurityTag;
	iDetails.bZeroWeightItem = dItem.bZeroWeightItem;
	iDetails.bPickListItem = dItem.bPickListItem;

	// set item identifier used to refer receipt line items by TB later for PIM or for reward lines
	iDetails.csItemID = dItem.szItemID;
	// if new string version of item id is not set, generate unique entry id as needed
    if ( iDetails.csItemID.IsEmpty())
	{
		long nOldID = dItem.lEntryID;
		GenerateUniqueEntryID(dItem.lEntryID); // call this before using lEntryID in dItem
		if (nOldID != dItem.lEntryID)
		{
			trace(L0, _T("TB set lEntryID to an invalid id(%d), SCOTApp corrected it and set it to new value(%d)"), nOldID, dItem.lEntryID);

		}
	}
	iDetails.lEntryID	 = dItem.lEntryID;

	if (!iDetails.csItemID.IsEmpty())
	{
		// szItemID is new field introduced to replace numeric limitation on item entry ID
		// if TB sets both values, as per the ISpec, ignore lEntryID and use new field, szItemID
		iDetails.lEntryID = INVALID_ENTRYID;
	}
	else
	{
		// new field, szItemID is not set
		// use lEntryID for further use  inside application where needed
		iDetails.csItemID.Format(_T("%d"), iDetails.lEntryID);
	}
	trace(L0, _T("Adjusted Items Entry ID:%s"), iDetails.csItemID);

	CString csItemCode;
	if (!iDetails.csItemCode.IsEmpty())
	{
		csItemCode = iDetails.csItemCode;
	}
	if (!csItemCode.IsEmpty())
	{
	  // non-empty item code, check for proper item scan type
	  if (_istdigit(csItemCode[0]))
	  {
		// ASSERT(0 == io.cScanType);
		// io.cScanType = 0;
		// it is not necessary by SCOT Application to force TB to return original item code with
		// scan type(OPOS) as prefix. It is upto TB to decide whether to save original item code with app.
	  }
	  if (io.cScanType < 0)
	  {
		io.cScanType = 0;
	  }
	}
	else
	{
		// item code is empty, must be a linked item
		ASSERT(iDetails.bLinkedItem);	// this must be set to true by TB, if it doesn't
		iDetails.bLinkedItem = true;
	}
	if (dItem.szItemID) 
		try{ 	delete [] dItem.szItemID;} catch(...){trace(L6,_T("delete exception:dItem.szItemID"));} 
	if (dItem.szDescription)
		try{	delete [] dItem.szDescription;} catch(...){trace(L6,_T("delete exception:dItem.szDescription"));}
	if (dItem.szErrDescription)
		try{	delete [] dItem.szErrDescription;} catch(...){trace(L6,_T("delete exception:dItem.szErrDescription"));}
	if (dItem.szItemCode)
		try{	delete [] dItem.szItemCode;} catch(...){trace(L6,_T("delete exception:dItem.szItemCode"));}

	iDetails.fDetails = true;

	return;
}

/////////////////////////////////
void SMStateBase::SetItemDetails(SItemDetails* dItem, ItemDetails &iDetails)  //Get Item Details, transfer to item object.
{
//	dItem->lEntryID = INVALID_ENTRYID;

	trace(L2, _T("::SetItemDetails - bEASReEnableScanner is set to false."));

	//Tar 199108 BZ 03/29/02  This assignment will possibly overwrite its value which
	//could have been previously set when PRODUCT_TAG_DEACTIVATED event was caught.
	//bEASReEnableScanner = false;

	CString strFmt ;
	CString csItemID;
	if (tb.IsAllowed(TB_FT_SUPPORT_PIM))
	{
		try
		{
			csItemID = dItem->szItemID;
			if (dItem->szItemID && csItemID.GetLength())
			{
				delete [] dItem->szItemID;
				dItem->szItemID = NULL;      // TAR 389631
			}
		}catch(...)
		{
			csItemID.Empty();
			trace(L0, _T("Failed to read szItemID field from SItemDetails passed by TB while handling quick item entry"));
		}
	}
	strFmt	= _T("SetItemDetails--ItemDetail: lExtPrice:%d; lUnitPrice:%d; lQuantity:%d; lWeight:%d; lDealQuantity:%d; fCoupon:%d; ") ;
	strFmt += _T("fVoid:%d; fNeedQuantity:%d; fNeedWeight:%d; fRestricted:%d; fVisualVerify:%d; nRestrictedAge:%d; szDescription:%s; ") ;
	strFmt += _T("szItemCode:%s; fNeedPrice:%d; fNotFound:%d; fNotForSale:%d; lScanCodeType:%d; bLinkedItem:%d lDepartment:%d, entryid:%d; ") ;
	strFmt += _T("lRequireSecBagging:%d; lRequireSecSubstChk:%d; bSecurityTag:%d; bPickListItem:%d, bZeroWeightItem:%d, szItemID:%s");

	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt, dItem->lExtPrice,
															dItem->lUnitPrice,
															dItem->lQuantity,
															dItem->lWeight,
															dItem->lDealQuantity,
															dItem->fCoupon,
															dItem->fVoid,
															dItem->fNeedQuantity,
															dItem->fNeedWeight,
															dItem->fRestricted,
															dItem->fVisualVerify,
															dItem->nRestrictedAge,
															dItem->szDescription,
															dItem->szItemCode,
															dItem->fNeedPrice,
															dItem->fNotFound,
															dItem->fNotForSale,
															dItem->lScanCodeType,
															dItem->bLinkedItem,
															dItem->lDepartment,
															dItem->lEntryID,
															dItem->lRequireSecBagging,
															dItem->lRequireSecSubstChk,
															dItem->bSecurityTag,
															dItem->bZeroWeightItem,
															dItem->bPickListItem,
															csItemID);


	iDetails.lExtPrice		 = dItem->lExtPrice;
	iDetails.lUnitPrice 	= dItem->lUnitPrice;
	iDetails.lQtySold		  = dItem->lQuantity;
	iDetails.lWgtSold		  = dItem->lWeight;
	iDetails.lDealQty		  = dItem->lDealQuantity;
	iDetails.fCoupon		   = dItem->fCoupon;
	iDetails.fVoid			   = (dItem->fVoid? true: false);
	iDetails.fQuantityRequired = dItem->fNeedQuantity;
	iDetails.fWeightRequired   = dItem->fNeedWeight;
	iDetails.fRestricted	   = dItem->fRestricted;
	iDetails.fVisualVerify	   = dItem->fVisualVerify;
	iDetails.nRestrictedAge  = dItem->nRestrictedAge;
	iDetails.csDescription	 = (LPTSTR)dItem->szDescription;
	iDetails.csItemCode 	   = (LPCTSTR)dItem->szItemCode;
	iDetails.fPriceRequired = dItem->fNeedPrice;
	iDetails.fNotFound	 = dItem->fNotFound;
	iDetails.fNotForSale = dItem->fNotForSale;
	iDetails.bLinkedItem = dItem->bLinkedItem;
	if(!iDetails.bLinkedItem)
	{
		io.cScanType			= dItem->lScanCodeType;
		// If department is non zero, then use the the value in lDepartment field
		// otherwise if department is zero, then use the value in szExtendedDeptInfo
		if (0 != dItem->lDepartment ) // old way
		{
			g_csSavedDeptInfo.Format(_T("%d"), dItem->lDepartment);
		}
		else // new way
		{
			g_csSavedDeptInfo		= dItem->szExtendedDeptInfo;	// SR 141 - RFC 283384
		}
		//071904: TAR 273325 BMediano - BEGIN
		// Modified to set the item description to its selected LANGUAGE(e.g. SPANISH )
		if( ( this->csLastNonHelpStateName == "SMProduceFavorites" ) ||
			( this->csLastNonHelpStateName == "SMSelectTare" ) )
		{
			if( ps.GetLanguage() == SCOT_LANGUAGE_PRIMARY )
			{
				io.csOrgItemDescription = dItem->szDescription;
			}
			else
			{
				if( lvSMLastState == "SMKeyInCode" )
				{
					io.csOrgItemDescription = dItem->szDescription;
					lvSMLastState = _T("");
				}
				else
				{
					io.csOrgItemDescription = ps.GetItemDesc();  // see SMProduceFavoritesBase::PSListLookup()
				}
			}
		}
		else
		{
			io.csOrgItemDescription = dItem->szDescription;
			lvSMLastState = this->csLastNonHelpStateName;   // stores the last state
		}
		//071904: TAR 273325 BMediano - END
        //io.csOrgItemDescription = dItem->szDescription;		// Need to store orginal desc for every linked item
	}

	io.lTareCode		 = dItem->nTareCode; // Matt from ACS reported this bug
	iDetails.lDepartment = dItem->lDepartment;
	iDetails.lRequireSecBagging = dItem->lRequireSecBagging;
	iDetails.lRequireSecSubstChk = dItem->lRequireSecSubstChk;
	iDetails.bSecurityTag = dItem->bSecurityTag;
	iDetails.bZeroWeightItem = dItem->bZeroWeightItem;
	iDetails.bPickListItem = dItem->bPickListItem;

	// set item identifier used to refer receipt line items by TB later for PIM or for reward lines
	iDetails.csItemID = csItemID;
	// if new string version of item id is not set, generate unique entry id as needed
    if (iDetails.csItemID.IsEmpty())
	{
		long nOldID = dItem->lEntryID;
		GenerateUniqueEntryID(dItem->lEntryID); // call this before using lEntryID in dItem
		if (nOldID != dItem->lEntryID)
		{
			trace(L0, _T("TB set lEntryID to an invalid id(%d), SCOTApp corrected it and set it to new value(%d)"), nOldID, dItem->lEntryID);
		}
	}
	iDetails.lEntryID	 = dItem->lEntryID;

	if (!iDetails.csItemID.IsEmpty())
	{
		// szItemID is new field introduced to replace numeric limitation on item entry ID
		// if TB sets both values, as per the ISpec, ignore lEntryID and use new field, szItemID
		iDetails.lEntryID = INVALID_ENTRYID;
	}
	else
	{
		// new field, szItemID is not set
		// use lEntryID for further use  inside application where needed
		iDetails.csItemID.Format(_T("%d"), iDetails.lEntryID);
	}
	CString csItemCode;
	if (!iDetails.csItemCode.IsEmpty())
	{
		csItemCode = iDetails.csItemCode;
	}
	if (!csItemCode.IsEmpty())
	{
	  // non-empty item code, check for proper item scan type
	  if (_istdigit(csItemCode[0]))
	  {
		// ASSERT(0 == io.cScanType);
		// io.cScanType = 0;
		// it is not necessary by SCOT Application to force TB to return original item code with
		// scan type(OPOS) as prefix. It is upto TB to decide whether to save original item code with app.
	  }
	  if (io.cScanType < 0)
	  {
		io.cScanType = 0;
	  }
	}
	else
	{
		// item code is empty, must be a linked item
		ASSERT(iDetails.bLinkedItem);	// this must be set to true by TB, if it doesn't
		iDetails.bLinkedItem = true;
	}
	if (dItem->szItemID)
		try{	delete [] dItem->szItemID;} catch(...){trace(L6,_T("delete exception:dItem->szItemID"));}
	if (dItem->szDescription)
		try{	delete [] dItem->szDescription;} catch(...){trace(L6,_T("delete exception:dItem->szDescription"));}
	if (dItem->szErrDescription)
		try{	delete [] dItem->szErrDescription;} catch(...){trace(L6,_T("delete exception:dItem->szErrDescription"));}
	if (dItem->szItemCode)
		try{	delete [] dItem->szItemCode;} catch(...){trace(L6,_T("delete exception:dItem->szItemCode"));}

	iDetails.fDetails = true;

	return;
}

///////////////////////////////
void SMStateBase::UpdateTotal(void)
{
	ps.RefreshCMTotal();	   // Update total if it has changed.
	ps.ShowCMRewardInfoBox(lTrxTotalSavings, lTrxTotalRewardPoints);
}

///////////////////////////////
void SMStateBase::TBGetItemExtendedDetails(ItemExtendedDetails &iXDetails)	//Get Item Extended Details, transfer to item object.
{

	SItemExtendedDetails   dXItem;
	memset(&dXItem, 0, sizeof(SItemExtendedDetails));

	rc = tb.GetItemExtendedDetails(&dXItem);

	CString strFmt ;

	strFmt	= _T("TBGetItemExtendedDetails--ItemXDetail: szCategory: %s; ExceptionCode: %d; dwExceptionFlags: %X; dwTBOptionFlags: %X; szExceptionWav: %s");

	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt,	dXItem.szCategory,
															dXItem.ExceptionCode,
															dXItem.dwExceptionFlags,
															dXItem.dwTBOptionFlags,
															dXItem.szExceptionWav);

	ProcessTBReturn(rc);

  static bool bFirstTime = true;
	if(rc==TB_SUCCESS)
  {
	  iXDetails.csCategory = (LPTSTR)dXItem.szCategory;
	  iXDetails.ExceptionCode = dXItem.ExceptionCode;
	  iXDetails.dwExceptionFlags = dXItem.dwExceptionFlags;
	  iXDetails.dwTBOptionFlags = dXItem.dwTBOptionFlags;
		iXDetails.csExceptionWav = dXItem.szExceptionWav;
	}
  else if (rc == TB_E_FUNCTIONNOTSUPPORTED && bFirstTime)
  {
	// TAR 136406 - Don't log a fatal message, and not every time!
	bFirstTime = false;
		ScotError(RETURN,
		  SM_CAT_3rdPARTY_INTERFACE,
		  SCOTAPP_SM_TB_FUNCTION_NOT_SUPPORTED,
		  _T("GetItemExtendedDetails"));
  }
  //iXDetails members enter initialized so it is not necessary to call
	//reset if rc != TB_SUCCESS
	if(dXItem.szCategory)
		try{	delete [] dXItem.szCategory;} catch(...){trace(L6,_T("delete exception:dXItem.szCategory"));}
	if(dXItem.szExceptionWav)
		try{	delete [] dXItem.szExceptionWav;} catch(...){trace(L6,_T("delete exception:dXItem.szExceptionWav"));}
}
///////////////////////////////
SMStateBase *SMStateBase::TBGetTotalDetails(void)
{
	SMStateBase *sUser = STATE_NULL;

	STotalDetails dTotal;
	memset(&dTotal, 0, sizeof(STotalDetails));


	rc= tb.GetTotalDetails(&dTotal);
	ProcessTBReturn(rc);
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO ,
		_T("TBGetTotalDetails--TotalDetail: lTaxDue:%d; lTotal:%d; lDiscount:%d; szErrDescription:%s; lFoodStampDue:%d"),
								dTotal.lTaxDue, dTotal.lTotal, dTotal.lDiscount, dTotal.szErrDescription, dTotal.lFoodStampDue) ;

	// extract the total taxes and the balance still due

	lTaxDue = dTotal.lTaxDue;

	lBalanceDue = dTotal.lTotal - lTotalPaid;
	lTotalSale = dTotal.lTotal;
	lTrxTotalSavings = dTotal.lDiscount;
	lTrxTotalRewardPoints = dTotal.lRewardPoints;

    trace(L7, _T("+rp.SetTransactionAmount"));
	rp.SetTransactionAmount(lTotalSale);
	trace(L7, _T("-rp.SetTransactionAmount<%d>"),lTotalSale);

	lFoodStampsDue = dTotal.lFoodStampDue;
	if (dTotal.szErrDescription)
		try{	delete [] dTotal.szErrDescription;} catch(...){trace(L6,_T("delete exception:dTotal.szErrDescription"));}

	// show the taxes anf total sale
	ps.Tax(lTaxDue);				  // update the taxes
	ps.Total(lTotalSale);			 // update current total

	//tar 350863 - begin
   _TCHAR sTotalTemp[_MAX_PATH];
   _ltot( lTotalSale, sTotalTemp, 10 );
   CString csTotal(sTotalTemp);
  //tar 350863 - end

	trace(L7, _T("+ra.OnNormalItem, Sending Total to RAP"));
	if (!io.nLoyaltyCardScanned)
	{
		ra.OnNormalItem(_T("[Total]") + csTotal);
	}
	trace(L7, _T("-ra.OnNormalItem, Total = %s"), csTotal);
	ps.UpdateAmountDueAmountPaid(lBalanceDue - nDMCashInserted, lTotalPaid + nDMCashInserted );

	CString csCurrentStateName;
	try
	{
		csCurrentStateName = this->GetRuntimeClass()->m_lpszClassName;
	}
	catch(...){};

		if (   ((lBalanceDue - nDMCashInserted) <= 0)
		&& bEnterTenderState
		&& (csCurrentStateName.Find(_T("InProgress")) == -1)
		&& (csCurrentStateName.Find(_T("VoidApproval")) == -1)  //TAR251283
		&& (csCurrentStateName.Find(_T("InsertCoupon")) == -1 ) //TAR322450
		&& (csCurrentStateName.Find(_T("DepositCoupon")) == -1 ) //LP185019 added fix TAR339622
		&& (csCurrentStateName.Find(_T("SMRAPDataNeeded")) == -1 ) // TAR 359471
	   )
	{
		// not safe to call directly TBEnterTender since Finish checks coupon deposit and other pre-tender requirements of SCOT
		// TAR: 155764
		return setAndCreateAnchorState(BES_FINISH);
	}

/*	TAR#125858 Removing the following code because it causes extra money to be returned to
	the user - VERY BAD!!! It was supposed to return cash entered if TB never ever
	came back with TENDERACCEPTED OR TENDERDECLINED, but that should mean the TB has stopped
	responding, which means you wouldn't get here anyway.
	// if an error occurred on the last ENTERTENDER for CASH
	// a TS_UKSTE or TS_TENDER_DECLINED will be caught in IdentifyState
	// and cuse the nDMCashTendered to be set > 0
	if (nDMCashTendered>0)
	{
		// oh oh, the last cash submitted was rejected
		// we have to return the cash to the customer
		lChangeDue += nDMCashTendered;
		nDMCashTendered = 0;
		CREATE_AND_DISPENSE(TakeChange)(false);  // Coin Acceptor Modifications, TAR 107249
	}
*/
  return STATE_NULL;
}

///////////////////////////////
SMStateBase *SMStateBase::TBBalanceDetails(void)
{
	SMStateBase *sUser = STATE_NULL;

	/* tar 260141 - removed this line of code
	//if a card was inserted set flag to remove it LPM082702	212937
	if (cDMLastTrack && co.fTenderIsMotorizedMSR)
		waitingForCardRemoval = true;
	*/
	STenderDetails dTender;
	memset(&dTender, 0, sizeof(STenderDetails));

	rc = tb.GetTenderDetails(&dTender);
	ProcessTBReturn(rc);
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO ,
		_T("TBBalanceDetails--TenderDetail: szDescription:%s; lTender:%d; lBalance:%d; lChange:%d; szErrDescription:%s; nTenderType:%d;"),
		dTender.szDescription, dTender.lTender, dTender.lBalance, dTender.lChange, dTender.szErrDescription, dTender.nTenderType) ;

	// read the current tender amount and balance due
	// from the display line, accumulate the total tender

	lLastTender = dTender.lTender;	// can be -ve if tender was voided
	lTotalPaid += dTender.lTender;
	lBalanceDue = dTender.lBalance;
	lChangeDue = dTender.lChange;

	// SR695 - Send Tender Balance Paid message to Security Manager if balance
	// fully paid.
	if ((lBalanceDue < 0.01) && bIsInFinalization && !dm.fStateTakeawayBelt)
		SASendSCOTInput(BALANCE_PAID);

	if (fStateAssist && dTender.nTenderType)
	{
		// tendered in AssistMode
		AdjustTenderButtonIndex(dTender.nTenderType);
        trace(L7, _T("+rp.m_bRapAssisted"));
        rp.m_bRapAssisted = true;
        rp.SetTenderTerminalNumber(RPRAP);
        trace(L7, _T("-rp.m_bRapAssisted<%d>"),rp.m_bRapAssisted );
	}
    else
    {
        rp.SetTenderTerminalNumber(rp.GetTerminalNumber());
        trace(L7, _T("TenderTerminalNumber<%s>"),rp.GetTerminalNumber() );
    }
	if((dTender.nTenderType>0) && (dTender.nTenderType != nTenderType))
	{
	   nTenderType = dTender.nTenderType; //PIP
	   if (nTenderType == TB_TENDER_VOUCHERPAYMENT)
	   {
		   setAnchorState( BES_VOUCHERPAYMENT );
	   }
	}

	//PIP
	TBPrcessPIPTenderType(nTenderType);

	// Save data to log for System Reports
	// TAR 211178 - Record tender data when we're not in training mode.
	if (co.fOperationsLogDataForSystemReports)
	//Begin FastLane 3.1 Reporting
    {
	   trace(L7, _T("+rp.TenderStart"));
	   rp.TenderStart(nTenderType);
	   trace(L7, _T("-rp.TenderStart<%d>"), nTenderType);
	   trace(L7, _T("+rp.TenderEnd"));
	   rp.TenderEnd(lLastTender);
	   trace(L7, _T("-rp.TenderEnd<%d>"),lLastTender );
    }
	//End FastLane 3.1 Reporting



	// Tar 126061 if cash acceptor receive some cash after tendering for some reason,
	//	need return these cash to customer
	if( nDMCashInserted)
	{
	  lTotalPaid += nDMCashInserted;
	  lChangeDue += nDMCashInserted;
	  nDMCashInserted = 0; //TAR295144
	}



	if (dTender.szDescription)
		try{	delete [] dTender.szDescription;} catch(...){trace(L6,_T("delete exception:dTender.szDescription"));}
	if (dTender.szErrDescription)
		try{	delete [] dTender.szErrDescription;} catch(...){trace(L6,_T("delete exception:dTender.szErrDescription"));}

	if ((nTenderType == TB_TENDER_CREDIT ||
		nTenderType == TB_TENDER_DEBIT ||
		nTenderType == TB_TENDER_LOYALTY ||
		nTenderType == TB_TENDER_DEBITSAVING ||
		nTenderType == TB_TENDER_GIFTCARD ||
		nTenderType == TB_TENDER_EBT) && (lBalanceDue < -1))
	{
		trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(RA_ENDCARDTENDER_BALANCEDUE, SCOT_LANGUAGE_PRIMARY));
		ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY +
						ps.GetPSText(RA_ENDCARDTENDER_BALANCEDUE, SCOT_LANGUAGE_PRIMARY));
		trace(L7, _T("-ra.OnNormalItem"));
	}
/*	else
	{
		if (lBalanceDue < 0.01)
			ra.OnNormalItem("[highlight]" +
							ps.GetPSText(RA_TENDER_COMPLETE, SCOT_LANGUAGE_PRIMARY));
	}
*/
	if (nTenderType == TB_TENDER_FOODSTAMPS)
		lFoodStampsDue = lFoodStampsDue - lLastTender;

	nDMCashInDrawer += nDMCashTendered;
	nDMCashTendered = 0;

	ResetAllTenderValues();

	// if (lLastTender<0)				// if a void of last cash payment
	//{
	// ts.Total();					  // refresh the total
	// }
	if (fStateAssist || bInSuspendCall)
	{
		//trace(L0, "Ignoring TB_TENDERACCEPTED since SCOT doesnt make any TBEnterTender calls");
		return STATE_NULL;
	}

	if(lBalanceDue>0)
	{
		if(nTenderType == TB_TENDER_FOODSTAMPS ||
		   nTenderType == TB_TENDER_EBT)			//TAR#124396 Treat food stamps & cash benefit
		{											//as the same for purposes of Allow partial cancel with EBT option
			bPartialTenderWithFoodStamps = true;
			setAnchorState(BES_FINISH);  //
			if (co.fTenderIsMotorizedMSR)
			{
				CREATE_AND_DISPENSE(TakeCard)(false);
			}
			else
			{
				//TAR213763+ CHU/Dale: it should return back to FinishAndPay
				//Also before we return we need to reset cDMLastTrack since it still has
				//EBT data in there
				if (cDMLastTrack)
				{ // if there is, delete it
					delete cDMLastTrack;
					cDMLastTrack = NULL;
					nDMLastTrackLen = 0;
					csDMEncryptedPin = EMPTY_STRING;
				}
				return setAndCreateAnchorState(BES_FINISH);
				//TAR213763-
			}
		}
		else if(nTenderType == TB_TENDER_VOUCHERPAYMENT)	// RFC 213717 coupon/voucher tender
		{
			if(IsUsingCouponSensor()) //TAR366924
            {
                CREATE_AND_DISPENSE(InsertCoupon)(getAnchorState(), BES_FINISH);
            }
            else
			  return createAnchorState();
		}
		else
		{
			//TAR206672+ 092302 CHU: it should reset cDMLastTrack
			//It could be after paying with gift card first, still have balance left.
			//So we want to go back to FinishAndPay and start over.
			if (cDMLastTrack)
			{ // if there is, delete it
				delete cDMLastTrack;
				cDMLastTrack = NULL;
				nDMLastTrackLen = 0;
				csDMEncryptedPin = EMPTY_STRING;
			}
                        //+TAR 285955
			if(io.cScanType !=0) // if the last item was scanned
			{
			  return setAndCreateAnchorState(BES_FINISH);
			}
                        //-TAR 285955

            // RFC 265361
            if (co.fTenderIsMotorizedMSR)
            {
                setAnchorState(BES_FINISH);
                CREATE_AND_DISPENSE(TakeCard)(false);
            }
            // end RFC 265361

			//TAR206672-
			return setAndCreateAnchorState(BES_FINISH);  // usually set already except from assist mode
		}
	}
	else
	{
		return STATE_NULL;
	}
}

//////////////////////////////////////////
SMStateBase* SMStateBase::TBGetTenderDetails(void)
{
	STenderDetails dTender;
	memset(&dTender, 0, sizeof(STenderDetails));

	rc = tb.GetTenderDetails(&dTender);
	ProcessTBReturn(rc);
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO ,
		_T("TBGetTenderDetails--TenderDetail: szDescription:%s; lTender:%d; lBalance:%d; lChange:%d; szErrDescription:%s"),
		dTender.szDescription, dTender.lTender, dTender.lBalance, dTender.lChange, dTender.szErrDescription) ;

	if (dTender.lChange < 0)
	{
		// this will never happen unless TB freaks out
		trace(L0, _T("wrong lChange value:%d returned by TB, negative dispense is not possible"), dTender.lChange);
		dTender.lChange = 0;
	}
	if(!fTenderDeclined)   //If tenderdeclined, do not need get these data
	{
	  lLastTender = dTender.lTender;
	  lBalanceDue = dTender.lBalance;
	  lChangeDue = dTender.lChange;

	  if(dTender.nTenderType>0)
		  nTenderType = dTender.nTenderType; //PIP
	}


	//PIP
	TBPrcessPIPTenderType(nTenderType);

	trace(L7, _T("+ra.OnNormalItem"));
	// add display tender for rap
	if (nTenderType == TB_TENDER_CREDIT)
		ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+ps.GetPSText(RA_CARD_DECLINED,SCOT_LANGUAGE_PRIMARY));
	else if (nTenderType == TB_TENDER_DEBIT)

		ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+ps.GetPSText(RA_CARD_DECLINED,SCOT_LANGUAGE_PRIMARY));
	else if (nTenderType == TB_TENDER_LOYALTY)
		ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+ps.GetPSText(RA_CARD_DECLINED,SCOT_LANGUAGE_PRIMARY));
	else if (nTenderType == TB_TENDER_DEBITSAVING)
		ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+ps.GetPSText(RA_CARD_DECLINED,SCOT_LANGUAGE_PRIMARY));
	else if (nTenderType == TB_TENDER_DEBITCHECKING)
		ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+ps.GetPSText(RA_CARD_DECLINED,SCOT_LANGUAGE_PRIMARY));
	else if (nTenderType == TB_TENDER_EBT)
		ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+ps.GetPSText(RA_EBT_DECLINED,SCOT_LANGUAGE_PRIMARY));
	else if (nTenderType == TB_TENDER_FOODSTAMPS)
		ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+ps.GetPSText(RA_EBT_DECLINED,SCOT_LANGUAGE_PRIMARY));

	csErrDescription = CString(dTender.szErrDescription, dTender.lErrDescriptionLength);
	lErrDescriptionLength = dTender.lErrDescriptionLength;


  if (lErrDescriptionLength > 0)
  {
	CString csRADeclineMsg;
	csRADeclineMsg = CString(RAP_RECEIPT_LINE_SUMMARY) + _T("... ") + csErrDescription;
	ra.OnNormalItem(csRADeclineMsg);
  }
  trace(L7, _T("-ra.OnNormalItem"));

	if (dTender.lDescriptionLength)
		try{	delete [] dTender.szDescription;} catch(...){trace(L6,_T("delete exception:dTender.szDescription"));}
	if (dTender.lErrDescriptionLength)
		try{	delete [] dTender.szErrDescription;} catch(...){trace(L6,_T("delete exception:dTender.szErrDescription"));}
	return STATE_NULL;
}

/////////////////////////////////////////////////////////////////////////////
void SMStateBase::TBPrcessPIPTenderType(TBTENDERTYPE& nTenderType)
{
	switch (nTenderType)
	{
	case TB_TENDER_PIP_CREDIT:
		nTenderType = TB_TENDER_CREDIT;
		break;
	case TB_TENDER_PIP_DEBIT:
		nTenderType = TB_TENDER_DEBIT;
		break;
	case TB_TENDER_PIP_DEBIT_CASHBACK:
		nTenderType = TB_TENDER_DEBIT;
		break;
	case TB_TENDER_PIP_LOYALTY:
		nTenderType = TB_TENDER_LOYALTY;
		break;
	case TB_TENDER_PIP_DEBITSAVING:
		nTenderType = TB_TENDER_DEBITSAVING;
		break;
	case TB_TENDER_PIP_DEBITCHECKING:
		nTenderType = TB_TENDER_DEBITCHECKING;
		break;
	case TB_TENDER_PIP_EBT:
		nTenderType = TB_TENDER_EBT;
		break;
	case TB_TENDER_PIP_FOODSTAMPS:
		nTenderType = TB_TENDER_FOODSTAMPS;
		break;
	case TB_TENDER_PIP_GIFTCARD:
		nTenderType = TB_TENDER_GIFTCARD;
		break;
	}
	return;
}

/*
///////////////////////////////
SMStateBase *SMStateBase::TBChangeDetails(void)
{
  SMStateBase *sUser = STATE_NULL;

  TBGetTenderDetails();

  //lTotalPaid += lLastTender;
  lBalanceDue = 0-lChangeDue;

//	nDMCashInDrawer += nDMCashTendered;
//	nDMCashTendered = 0;

	ResetAllTenderValues();

  // the transaction has completed.
  // there may be still printing going on, receipt bottom msg etc,,,
	// wait for TB_TRXEND
  return STATE_NULL;
}
*/
///////////////////////////////
SMStateBase *SMStateBase::TBEnterItem(const bool fOverride, int lUpdateFields)// sell item currently in the ItemObject
{
	//+TAR410994
	trace(L7, _T("+rp.TransactionStart"));
    rp.RPBusinessDate();
	rp.TransactionStart(rp.m_csBusinessDate);
	trace(L7, _T("-rp.TransactionStart"));
	//-TAR410994
	SEnterItemDetails sMoreItemDetails;
	sMoreItemDetails.lUnitPrice  = io.lUnitPriceEntered;
	sMoreItemDetails.lExtPrice = io.lExtPriceEntered;  //Tar 137876 add extended price
	sMoreItemDetails.lItemScanCodeType =  io.cScanType;
    sMoreItemDetails.szOriginalCode = (LPCTSTR) io.csOrigCode;
	sMoreItemDetails.nCrateableScreenAnswer = io.d.nCrateableScreenAnswer;
	sMoreItemDetails.bPickListItem = io.d.bPickListItem;
	if (lUpdateFields)
		sMoreItemDetails.szItemID = io.d.csItemID;
	else
		sMoreItemDetails.szItemID = NULL;
    //Only fill the following details if item was scanned
    sMoreItemDetails.nMultiLabelBarcodes = io.cScanType ? m_nNumberOfPartsReturnedSoFar : 0;
    sMoreItemDetails.psMLBCDetails = io.cScanType ? m_psMLBarCodeDetails : NULL;
	sMoreItemDetails.lUpdateFields = lUpdateFields;
    /////////////////////////////////////////////////////
    m_bCouldBeScannedLoyaltyCard = true;

    if (io.fVoid && io.d.csItemID.IsEmpty() )
	{
		//TAR246355+ If it is a scanned void item and it is in Assist Mode,
		//We find the item from receipt first, then calling TBEnterItem with entry id of the
		//receipt
		if(fStateAssist && io.cScanType != 0)
		{
			 bool found = false;
			 CString csDesc,csCode;
			 long lQty,lWgt,lPrc,lUPrc, lTcd, lBarcodeType;

			 CString csDMLastBarLabelWithoutCheckDigit;
			 int nItemCodeLen = csDMLastBarLabel.GetLength();
			 csDMLastBarLabelWithoutCheckDigit = csDMLastBarLabel.Left(nItemCodeLen - 1); //remove the check digit

			 if (ps.GetItem(csDesc,csCode,lQty,lWgt,lPrc,lUPrc,lTcd, lBarcodeType, _T("CMSMReceipt"), _T("CMReceiptVoid"))>0)// get selected item info
			 {
				if ((csCode == csDMLastBarCode) || (csCode == csDMLastBarLabel) ||
					(csCode == csDMLastBarLabelWithoutCheckDigit) )
				{
					found = true;
					// TAR 277147 start
					CString csSelItemEntryId = _T("");
					csSelItemEntryId = ps.GetSelection(_T("CMReceiptVoid"));
					io.d.csItemID = csSelItemEntryId;
					trace(L2, _T("Void item ID for current item: %s"), csSelItemEntryId);
					// TAR 277147 end
			    }
			 }

			 if (!found)
			 {
				 CString csItemEntryId;
				 int nCountList = ps.ScanReceiptToVoid(_T("CMSMReceipt"), csItemEntryId);
				 for (int i=nCountList-1; i>=0; i--)
				 {
					ps.ScanReceiptToVoid(_T("CMSMReceipt"), csItemEntryId, i);
					if (ps.GetItem(csDesc,csCode,lQty,lWgt,lPrc,lUPrc,lTcd, lBarcodeType, _T("CMSMReceipt"), _T("CMReceiptVoid"), csItemEntryId)>0)// get selected item info
					{
						if ((csCode == csDMLastBarCode) || (csCode == csDMLastBarLabel) ||
							(csCode == csDMLastBarLabelWithoutCheckDigit) )  //TAR239865
						{
						  found = true;
						  ps.SetSelection(_T("CMReceiptVoid"), csItemEntryId);
						  trace(L0, _T("Void item ID: %s"), csItemEntryId);
						  io.d.csItemID = csItemEntryId;
						  sMoreItemDetails.szOriginalCode = (LPCTSTR) io.csOrigCode;
						  break;
						}
					}
				 }
			 }
			 if(!found)
			 {
				trace(L0, _T("Void item ID(Scanned in Assist): not found in the receipt"));
				sMoreItemDetails.szItemID = NULL;
			 }
		} //TAR246355-
		else
		{
			// find item id and assign to io.d.csItemID
			sMoreItemDetails.szItemID = NULL;
			CString csItemID;
			// find the void item's szItemID  
			if (fInMultiSelectPickList)
			{
				FindItemID(csItemID, _T("PickListReceiptVoid"), _T("PickListReceipt"), io.lExtPriceEntered, io.lUnitPriceEntered, io.csOrigCode);
			}
			else
				FindItemID(csItemID, _T("CMReceiptVoid"), _T("CMSMReceipt"), io.lExtPriceEntered, io.lUnitPriceEntered, io.csOrigCode);
			if (!csItemID.IsEmpty())
			{
				trace(L0, _T("Void item ID: %s"), csItemID);
				io.d.csItemID = csItemID;
			}
			else
			{
				trace(L0, _T("Void item ID: not found"));
			}
		}
	}
	if (io.fVoid)
	{
		sMoreItemDetails.szItemID = io.d.csItemID;
	}
	if (fOverride && fSAInItemSale)
	{
		bgAlreadySubmittedWithOverride = true;
	}
	else
	{
		bgAlreadySubmittedWithOverride = false;
	}
     if (io.csItemCode.IsEmpty())
     {
         //In case information has come through quick item entry - RFQ 986
         io.csItemCode = io.d.csItemCode;
     }
	 rc = tb.EnterItem(io.csItemCode,
						&sMoreItemDetails,
						io.fVoid,
						fOverride,
						io.lQuantityEntered,
						io.lWeightEntered,
						io.lTareCode,
						io.lPriceEntered,
						io.d.fQuantityConfirmed);
	// process the return code of EnterItem TB API
	if (lDMScaleWeight>0)
		fDMScaleZeroed = false;

	ps.Echo(PS_BLANK);
	fItemIsWaiting = false;
	//io.b_gIsThisItemPriceEmbedded = false;	// I don't know why this was set to false but it causes the price embedded to not work! TAR196093

    if (rc == TB_E_ITEMFAILED)	// TAR 127247
	{
	   // Issue item cancel to disable pending security since this
	   // is an item failed return code even if flag is not set.
	   return TBGetMessageInfo(true);
	}

	ProcessTBReturn(rc);
	CString strFmt ;
	strFmt =_T("TBEnterItem--ItemDetail: io.csItemCode:%s; io.fVoid:%d; io.lQuantityEntered:%d; io.lWeightEntered:%d; io.lTareCode:%d; ") ;
	strFmt+=_T("io.lPriceEntered:%d; io.d.fQuantityConfirmed:%d; sMoreItemDetails.lUnitPrice:%d; sMoreItemDetails.lExtPrice:%d; sMoreItemDetails.lItemScanCodeType:%d; ") ;
	strFmt+=_T("sMoreItemDetails.szOriginalCode:%s; szItemID:%s; fOverride:%d") ;

	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt,
													io.csItemCode,
													io.fVoid,
													io.lQuantityEntered,
													io.lWeightEntered,
													io.lTareCode,
													io.lPriceEntered,
													io.d.fQuantityConfirmed,
													sMoreItemDetails.lUnitPrice,
													sMoreItemDetails.lExtPrice,
													sMoreItemDetails.lItemScanCodeType,
        											sMoreItemDetails.szOriginalCode,
    											    sMoreItemDetails.szItemID,
        											fOverride) ;
    ResetMLBCValues(); //reset Multi-Label Barcode variable if necessary

	// Tell InProgress what type of state we are and what its timeout should be
	CREATE_AND_DISPENSE(InProgress)(storeMode(), helpMode(),
									co.getTimeOutItemTransactionResult());
}

//////////////////////////////////////////////////////////////
// try selling an item, set control variables
// note: the tare is in the range 0-10, 0-no tare, 1-10 for Smkt tares 0-9
SMStateBase *SMStateBase::TBItemSale(const bool fVoid,
									 const long lTare,
									 const bool fScannedItem,
									 const bool bIsPickListItem,
									 const long lUpdateFields)
{
//USSF START
	if (fItemOverride == false)
	{
		io.Reset(); 		// clear previous item info object
	}
	else
	{
		fItemOverride = false;
	}
//USSF END

	SMStateBase::ms_bIsCurrentItemFromPickList = bIsPickListItem;

	// save current item info supplied by user activity
	if(bIsPickListItem)
		io.d.bPickListItem = true;

	if (fScannedItem)
	{
		io.csOrigCode = csItemSent;    // item code scanned
		if (io.csOrigCode.IsEmpty())
		{
			// an item has been scanned but bar code is null!!
			ASSERT(0);
			io.csOrigCode = csDMLastBarCode;	   // item code scanned
		}
		io.csItemCode = csDMLastBarLabel;	// remove leading type
		io.cScanType = csDMLastBarType; 	// get scan type A,E,2...
	}
	else
	{
		io.csOrigCode = csItemSent;    // item code keyed
		io.csItemCode = csItemSent;    // assumed to be the same
	}

	io.fVoid = fVoid;
	io.lTareCode = lTare;			 // tare value -1,0,1,2...9,10,
	io.b_gIsThisItemPriceEmbedded = io.IsEmbeddedPrice(io.csItemCode);	//set up global for forced weighing of price embedded LPM
    io.lUpdateFields = lUpdateFields;
	trace(L6, _T("This item Price Embedded flag = %d."), io.b_gIsThisItemPriceEmbedded);

	if (fVoid)
	{
		io.lQuantityEntered = lVoidQty; // saved void qty
		io.lUnitPriceEntered = lVoidUPrc; // saved void unit price
		io.lWeightEntered	= lVoidWgt; // saved void weight
		io.lTareCode		= lVoidTcd; // tare value -1,0,1,2...9,10,
		io.lExtPriceEntered = lVoidPrc; //Tar 137876 add extended price
		if (!fScannedItem)
		{
			io.cScanType =	lVoidBarcodeType;
			int nItemCodeLen = io.csItemCode.GetLength();
			if ( (io.cScanType != SCAN_SDT_Code39) && (nItemCodeLen) && (!_istdigit(io.csItemCode[0])))  // added != SCAN_SDT_Code39
			{
				// item was scanned when it was sold and now it is selected
				// restore item code
				// This is where we stript the prefix from the original item code
				//TAR234917+ CHU 041803
				if (io.csItemCode.Left(2) == _T("B3"))
					//We need to take out 2 digits for UPC/EAN128
					//For example, the orig item code could be B398061490000404439997419
					io.csItemCode = io.csItemCode.Right(nItemCodeLen - 2); // remove second scan type character
				else if (io.csItemCode.Left(3) == _T("]e0"))  //RFC 412256
					io.csItemCode = io.csItemCode.Right(nItemCodeLen - 3); 
				else
                //TAR234917-
				{
					CString csTmp = io.csItemCode.Right(nItemCodeLen - 1); // remove first scan type character
					io.csItemCode = csTmp;
				}
			}

		}
	}
	if (lUpdateFields != ITEMUPDATE_NONE)
	{
		io.lQuantityEntered = itemDetailNew.lQtySold; // saved void qty
		io.lUnitPriceEntered = itemDetailNew.lUnitPrice; // saved void unit price
		io.lWeightEntered	= itemDetailNew.lWgtSold; // saved void weight
		io.lTareCode		= lVoidTcd; // tare value -1,0,1,2...9,10,
		io.lExtPriceEntered = itemDetailNew.lExtPrice; //Tar 137876 add extended price
		io.d.csItemID = itemDetailNew.csItemID;
        io.d.nCrateableScreenAnswer = itemDetailNew.nCrateableScreenAnswer;
		if (!fScannedItem)
		{
			io.cScanType =	lVoidBarcodeType;
			int nItemCodeLen = io.csItemCode.GetLength();
			if ( (io.cScanType != SCAN_SDT_Code39) && (nItemCodeLen) && (!_istdigit(io.csItemCode[0])))  // added != SCAN_SDT_Code39
			{
				// item was scanned when it was sold and now it is selected
				// restore item code
				// This is where we stript the prefix from the original item code
				//TAR234917+ CHU 041803
				if (io.csItemCode.Left(2) == _T("B3"))
					//We need to take out 2 digits for UPC/EAN128
					//For example, the orig item code could be B398061490000404439997419
					io.csItemCode = io.csItemCode.Right(nItemCodeLen - 2); // remove second scan type character
				else if (io.csItemCode.Left(3) == _T("]e0"))  //RFC 412256
					io.csItemCode = io.csItemCode.Right(nItemCodeLen - 3); // remove second scan type character
				else
                //TAR234917-
				{
					CString csTmp = io.csItemCode.Right(nItemCodeLen - 1); // remove first scan type character
					io.csItemCode = csTmp;
				}
			}

		}
	}

	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, _T("TB item sale <%s>, %d, %d"),csItemSent,fVoid,lTare);

	//begin TAR#102019 allow coupons has 3 optiont
	// if item is a coupon, not in assist and not voided
	if ( (io.IsCoupon() && !fStateAssist && !co.IsInAssistMenus() && !fVoid) )
	{
	  //CouponNotAllowed screen displays for both situations
	  if  ( (co.csTenderAllowCoupons == _T("O")) || (co.csTenderAllowCoupons == _T("N")) )
		RETURNSTATE(CouponNotAllowed)
	}	//end 102019  LPM120799

	//TAR339619
	if (( io.IsCoupon() && fStateAssist && (co.csTenderAllowCoupons == _T("N")) ))
	{
		TBShow2x20(ps.GetPSText(MSG_COUPONNOTACCEPTED));
		RETURNSTATE(CouponNotAllowed)
	}

	SMStateBase *sUser = STATE_NULL;
	if ((co.fUserExitAllEnabled) &&   // if exits enabled and
		(co.fUserExit16Enabled))		//	this exit is enabled
	{
		sUser = UserExit16(fVoid, lTare, csItemSent);

		if (sUser != STATE_NULL)
		   return sUser;
	}

	if( (!io.fVoid) && (!io.IsCoupon()) && (!io.d.bLinkedItem) )
	{
		// if there is a scanned UPC
		if(io.cScanType!=0)
		{
			//if UPC_SCANNED already sent to Security, then don't send it again
			if (nSAPrevSCOTInputID != UPC_SCANNED)	// TAR:121479 and TAR:148633  we already told SA so skip this step
			{
				// then inform Security Agent item is to be sold
				SASendSCOTInput( UPC_SCANNED, io.csItemCode );
			}
		}
		else	// UPC keyed in,
		{
			// TAR:121479 and TAR:148633  we already told SA so skip this step
			if (nSAPrevSCOTInputID != UPC_MANUAL)
			{
				// then inform Security Agent item is to be sold
				SASendSCOTInput( UPC_MANUAL, io.csItemCode );
			}
		}
	}

	//TAR 275049
	//if(io.IsCoupon()) //Tar 277820 - This flag should be set to true for all items
	{
		fSAInItemSale = true;
	}
	//TAR 275049

	if (io.b_gIsThisItemPriceEmbedded && !io.fVoid)	// if this is a price embedded (and not a void)
	{
	  trace(L6, _T("Price Embedded item. Compare Weights of Price Embedded items = %d."), g_bSubstCkPriceEmbeddedWeights);
	  if (g_bSubstCkPriceEmbeddedWeights && co.fStateForceWeighingOFPriceEmbedded) // if wt should be compared to scanner scale data
	  {
		  //TAB, we need to stop belts before we get into this state
		  if(dm.fStateTakeawayBelt)
		  {
				CKeyValueParmList parmList;
				BSTR bstrResult;
				CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_SUSPEND_ITEM);
				_bstr_t bstrParmList((const _TCHAR *)parmList);
				bstrResult = NULL;
				SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
		  }
	  }
	  if (g_bSubstCkPriceEmbeddedWeights && co.fStateForceWeighingOFPriceEmbedded) // if wt should be compared to scanner scale data
	  {
		  RETURNSTATE(EnterWtForPriceEmbedded);
	  }
//	  }
	}

	nSellState = getAnchorState();		  // save current sell entry state
	return TBEnterItem(false, lUpdateFields);
}

/////////////////////////////////////////////
// try tendering with cash
SMStateBase *SMStateBase::TBEnterTender(const TBTENDERTYPE nType,
																const long lAmt,
																const bool fOverride)
{
	fBeyondFinishAndPayState = true;
	//TAR239136 CHU 060403 we do this in SMFinish
	//lEOT_LGW = g_lLGW;		// when tender is initialized, save off the last good wt

	//Tar 112723
	bFinish = false;

	nTenderType = nType;
	long lTender = lAmt;
	bool fVoid = false;
	LPTSTR cTempLTD = NULL;

	if (lTender<0)
	{
		fVoid = true;
		lTender = 0-lTender;
	}

	CString csStateName = this->GetRuntimeClass()->m_lpszClassName;

	// if card used first and then cash inserted, can't send card data to TB
	// along with cash tender, only remember the latest tender type as TB can
	// only handle one tender type at a time.  TAR206158 LPM KRao 053102
	// will need trackdata to know if should eject card later 212937 LPM082702
	if(nTenderType == TB_TENDER_CASH)
	{
		rc = tb.EnterTender(
			nType,						// 0 - cash, 1 - credit, 2 - debit
			lTender,					// always a +ve amount
			cTempLTD,					// send in null track data is tendering with cash
			csDMEncryptedPin.GetLength(),
			csDMEncryptedPin,
			fOverride,
			fVoid);
	}
	else if(nTenderType == TB_TENDER_VOUCHERPAYMENT ||  // RFC 213717
			(nTenderType == TB_TENDER_GIFTCARD && csStateName.Find(_T("ScanCard"), 1) >= 0))	// Scanned gift card tender from ScanCard
	{
		rc = tb.EnterTender(
			nType,						// coupon/voucher tender
			lTender,					// always a +ve amount
			csItemSent,					// send in scanned coupon data
			csDMEncryptedPin.GetLength(),
			csDMEncryptedPin,
			fOverride,
			fVoid);
	}
	else
	{
		rc = tb.EnterTender(
			nType,						// 0 - cash, 1 - credit, 2 - debit
			lTender,					// always a +ve amount
			cDMLastTrack,
			csDMEncryptedPin.GetLength(),
			csDMEncryptedPin,
			fOverride,
			fVoid);
	}

	CString strFmt ;
	strFmt =_T("TBEnterTender--nType:%d; lTender:%d; nPinLengthInput:%d; csDMEncryptedPin:%s; fOverride:%d; fVoid:%d") ;
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt,
													nType,
													lTender,
													csDMEncryptedPin.GetLength(),
													csDMEncryptedPin,
													fOverride,
													fVoid);

	if (rc == TB_E_CARDDATA_MISSING)
	{
		//TAR193038
		bBadCard = true;

		// card data is not complete w.r.t TB
		if (co.fTenderIsMotorizedMSR)
		{
			CREATE_AND_DISPENSE(CardMisRead)(MSG_CARDDECLINEDSELECTPAYMENT, LTT_CARDMISREAD);
		}
		else
		{
			CREATE_AND_DISPENSE(CardMisRead)(MSG_CARDDECLINEDSELECTPAYMENT, LTT_CARDMISREAD);
		}
	}
	ProcessTBReturn(rc);
	// now offically tendering, tell SA
	//SASendSCOTInput( TENDER_START );
	// tender has been issued, show progress to customer
//	SASendSCOTInput(TRANSACTION_COMPLETE );
	// RFC 213717 new coupon/voucher tender is positive value
	if ((nType>TB_TENDER_CASH) && (nType != TB_TENDER_VOUCHERPAYMENT))  // credit or debit
	{
		RETURNSTATE(CardProcessing)
	}

	// For cash transactions, at this point the amount paid for the transaction
	// has been sent to supermarket (but not necessarily accepted by supermarket.
	// If it is rejected by supermarket we have to return the cash to the customer.
	// the lDMCashTendered has the amount of the cash just submitted
	// we do not reset this until we get a TS_BALANCE or TS_CHANGE

	// cash tender
	m_nDMCashPaid += nDMCashInserted;		// TAR 312035

	nDMCashTendered = nDMCashInserted;
	nDMCashInserted = 0;

	if (lAmt != 0 && !ps.RemoteMode())
	  {
		ps.Message12(LTT_CASHPROCESSING, true);
	  }

	// Tell InProgress what type of state we are and what its timeout should be
	CREATE_AND_DISPENSE(InProgress)(storeMode(),
		helpMode(),
		co.getTimeOutTender());
}





/////////////////////////////////////////////////////////////////////////////
USHORT SMStateBase::TBGetKeyCode(const int nKey) // 1.2.3,,,
{
	return uKeyCodeList[nKey];
}

/////////////////////////////////////////////////////////////////////////////
long SMStateBase::TBGetKeyLabel(const int nKey,CString &csLabel)
{
   csLabel = csKeyLabeList[nKey];
   return 0;
}

//////////////////////////////////////////////////////////
CString SMStateBase::TBLookupCode(const USHORT uKey)
{
	if (uKey<KEY_LOOKUP1) return _T("");
	if (uKey>KEY_LOOKUP40) return _T("");
	return SmktLkUp[uKey-KEY_LOOKUP1];
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
USHORT *SMStateBase::TBEmulatorKeys(void)
{
  static USHORT nKeys[200] =
		{	0,
				1,	 2,   3,  4,   5,
				6,	 7,   8,  9,  10,
				11,  12, 13, 14,  15,
				16,  17, 18, 19,  20,
				21,  22, 23, 24,  25,
				26,  27, 28, 29,  20,
				31,  32, 33, 34,  35,
				36,  37, 38, 39,  40,
				41,  42, 43, 44,  45,
				46,  47, 48, 49,  50,
				51,  52, 53, 54,  55,
				56,  57, 58, 59,  60,
				61,  62, 63, 64,  65,
				66,  67, 68, 69,  70,
				71,  72, 73, 74,  75,
				76,  77, 78, 79,  80,
				81,  82, 83, 84,  85,
				86,  87, 88, 89,  90,
				91,  92, 93, 94,  95,
				96,  97, 98, 99, 100
		};							// 100 is used for the rest of the keys
		return nKeys;
}

//load assist mode keyboard codes and layouts from the data file
void SMStateBase::TBLoadAssistModeKeyboard(void)
{
	// this function should be overridden by customer in SCOTSSF
	// Here is just a sample to show how to load.
	memcpy(uKeyCodeList,TBEmulatorKeys(),sizeof(USHORT)*200);
	for(int i = 1; (i < 200); ++i)
	{
		csKeyLabeList[i].Format(_T("key: %d"), i);
	}
	return;
/*
	_TCHAR szLayout[15];

	//if (fEmulator)
	//	{
		_tcscpy(szLayout,"SCOTKEYB"); // no TS/NT, no BATPRMEM.DAT file
		memcpy(uKeyCodeList,TBEmulatorKeys(),50);
		return;
	}

	//	CString csFile1;
	CString csFile2;

	// The IBM 50 key keyboard layout for Assist Mode is in 4690 controller file
	// ADX_SPGM:ADXCSCKF.DAT
	// We need to read the layout for this terminal.
	// the BATPRMEM file is locked by TSNT, copy it then read it
//	csFile1 = SCOTDRIVE ":\\TSNT\\BATPRMEM.DAT";
	csFile2 = SCOTDRIVE ":" SCOTPATH "\\BATPRMEM.DAT";
//	CopyFile(csFile1,csFile2,false);

	// The 8 character layout name we need to find is in the C:\TSNT\BATPRMEM.DAT
	// file at offset 0x0442 (1090).

	try
	{
		CFile cfIn(csFile2,CFile::modeRead | CFile::typeBinary);
		cfIn.Seek(1090,CFile::begin);
		cfIn.Read(szLayout,8);
		cfIn.Close();
	}
	catch(CFileException *e)
	{
		eo.FileError(RETURN,"TS/NT Configuration",*e);
		e->Delete();
		return;
	}

	szLayout[8] = 0;  // terminate the string
	trace(L0,"IBM 50 keyboard name <%s>",szLayout);

	// get the 4690 keyboard config file name ADX_SPGM:ADXCSCKF.DAT
//	csFile1 = "ADX_SPGM:ADXCSCKF.DAT";
	csFile2 = SCOTDRIVE ":" SCOTPATH "\\ADXCSCKF.DAT";
//	CopyFileFrom4690(csFile1,csFile2,false);

	// The file has a 381 byte header followed by a random number of
	// 380 byte keyboard layouts.
	// Each layout starts with a 0x01 byte, followed by key codes and
	// has the name of the layout at offset 367.
	// The key codes are not in a pattern and are mapped randomly as follows:
	//
	//		Offsets in the File in hex
	//
	//	Left 15    Center		  Right 20
	//
	//	11	12	13	  01  02	  0f  10  1f  20  S1
	//	21	14	15	04	05	06	16	17	2d	2e	S2
	//	22	23	36	07	08	09	28	19	2b	2c
	//	24	25	27	0a	0b	0c	1a	3c	3d	3e
	//	33	34	35	  0d	0e	38	39	3a	3b
	//
	//	  PS IDC_KEYxx Key Number in decimal
	//
	//	Left 15    Center		  Right 20
	//
	//	 1	 2	 3	  16  17	  29  30  31  32  50
	//	 4	 5	 6	18	19	20	33	34	35	36	51
	//	 7	 8	 9	21	22	23	37	38	39	40
	//	10	11	12	24	25	26	41	42	43	44
	//	13	14	15	  27	28	45	46	47	48


	memset(u50Key,0,50); // clear all keys

	try
	{
		_TCHAR cRec[380];
		CFile cfIn(csFile2,CFile::modeRead | CFile::typeBinary);
		cfIn.Seek(381,CFile::begin);
		while (cfIn.Read(cRec,380)==380)
		{
			LPTSTR p = cRec+367;
			if (_tcscmp(p,szLayout)==0)
			{
				trace(L6,"Found 50-keyboard layout <%s>",p);
				Uchar uMap[] = { // 48 programmable key offsets
					0x11,0x12,0x13,
						0x21,0x14,0x15,
						0x22,0x23,0x36,
						0x24,0x25,0x27,
						0x33,0x34,0x35,
						0x01,0x02,
						0x04,0x05,0x06,
						0x07,0x08,0x09,
						0x0a,0x0b,0x0c,
						0x0d,0x0e,
						0x0f,0x10,0x1f,0x20,
						0x16,0x17,0x2d,0x2e,
						0x28,0x19,0x2b,0x2c,
						0x1a,0x3c,0x3d,0x3e,
						0x38,0x39,0x3a,0x3b

				};		  // S1 and S2 are not programmable

				for (int i=0;i<48;i++)
				{
					u50Key[i] = cRec[uMap[i]];
				}
				u50Key[48] = 0x01; // S1 key code
				u50Key[49] = 0x02; // S2 key code
				break;
			}
		}
		cfIn.Close();
	}
	catch(CFileException *e)
	{
		eo.FileError(RETURN,"Keyboard layout",*e);
		e->Delete();
		return;
	}

	if (u50Key[49])   // if a keyboard was found
	{
		// read the layoutname.DAT file line by line
		// each line is in the form "xxx=desc' or 'xx=desc' or 'x=desc'

		try
		{
			CString csWork;
			CStdioFile csfIn(szLayout,CFile::modeRead);

			while (csfIn.ReadString(csWork))
			{
				// check for an = sign
				int i = csWork.Find("=");
				if (i>=1 && i<=3)
				{
					// convert the number
					int j = _ttoi(csWork.Left(i));
					// find key j on the u50Key array
					for (int k=0; k<50; k++)
					{
						if (u50Key[k]==j) // found key in array
						{
							cs50Key[k] = csWork.Mid(i+1);	// save key label
						}
					}
				}
			}
			csfIn.Close();
		}
		catch(CFileException *e)
		{
			eo.FileError(INFO,"Keyboard layout",*e);
			e->Delete();
			return;
		}

	}
	return;
*/

}

//////////////////////////////
SMStateBase *SMStateBase::TBCancelTransaction(const bool fAllowOverride)
{
  // ensure we are not in the middle of a prompt by pressing
  // clear twice before issuing void total
  //long rc = ts.Clear();

  //if (rc>=0)
  //  rc = ts.Clear();

  //TAR420881: Only tell SM KEY_TC_OPERATION_VOID_END if current state is not SMAbort
   CString csTempStateName = this->GetRuntimeClass()->m_lpszClassName;
   if( csTempStateName.Find(_T("Abort")) == -1 )
   {
  //if (rc>=0)
	// Tell security that transaction is voided/canceled
	  CKeyValueParmList parmList;
	  CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_TC_OPERATION_VOID_END);
	  _bstr_t bstrParmList((const _TCHAR *)parmList);
	  SecMgr.TransactionControl(lSecMgrCookie, bstrParmList);
   }

    rc = tb.CancelTransaction(fAllowOverride);
	CString strFmt ;
	strFmt =_T("TBCancelTransaction--fAllowOverride:%d") ;
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt, fAllowOverride) ;

  ProcessTBReturn(rc);

  // From Koteswara: You should not do ResetAllTransactionValues here since we loose many other values
  //which are required in screens that are there before thank you.
  // for example:	//SMTakeChange, and SMTakeReceipt
  // IsAnyVoidSOldItems() goes wrong if we do reset here.


   //if (rc<0)
   // ShowError(rc,"VoidTrans");

  //fInVoidTrans = true;

  // Tell InProgress what type of state we are and what its timeout should be

 //TAR 290233 start

  if(fAllowOverride && (TB_E_INVALIDSTATE ==rc))
	{
		return createAnchorState();

	}

  //TAR 290233 end


  CREATE_AND_DISPENSE(InProgress)(storeMode(),
		helpMode(),
		co.getTimeOutSignoff());
}

//Begin_Suspend
//////////////////////////////////////////////
SMStateBase *SMStateBase::TBSuspendTransaction(const bool fOverride)
{
	bInSuspendCall = true;
//Fiscal Printer start
	CComBSTR csPrintApprovalList = GetApprovalNeededString();
	rc=tb.SuspendTransactionEX(fOverride, (LPTSTR)csPrintApprovalList.Copy());
//Fiscal Printer end
//using new TB API	rc= tb.SuspendTransaction(fOverride);
	ProcessTBReturn(rc);
	CString strFmt ;
	strFmt =_T("TBSuspendTransaction--fOverride:%d") ;
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt, fOverride) ;

	// Tell InProgress what type of state we are and what its timeout should be
	CREATE_AND_DISPENSE(InProgress)(storeMode(),
		helpMode(),
		co.getTimeOutSignoff());
}
//End_Suspend

///////////////////////////////
long SMStateBase::TBInitialize(HWND hwnd, LPCTSTR szFilePath)
{
	// start of TAR 393349
	g_ArrayOfReceiptHandles.RemoveAll();  // ensure empty when starting
	trace(L6,_T("TAR 393349 (Initializing g_ArrayOfReceiptHandles)"));
	// end of TAR 393349

	rc = tb.Initialize(hwnd, szFilePath);
	ProcessTBReturn(rc);
	CString strFmt ;
	strFmt =_T("TBInitialize--hwnd:%x; szFilePath:%s") ;
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt, hwnd, szFilePath) ;
	return rc;
}

///////////////////////////////
long SMStateBase::TBLoadOptions(void)
{
	rc = tb.LoadOptions();
	ProcessTBReturn(rc);
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, _T("TBLoadOptions")) ;

//USSF Start
	if (co.fOperationsEnableUSSF)
		USSFEVENT(_T("TBLoadOptions"), ;, ; );
//USSF End  

	return rc;
}

TBRC SMStateBase::TBLogErrorToServer(LPCTSTR szErrorText, const int nSeverity,
						   const int nComponentID, LPCTSTR szComponentID,
						   LPCTSTR szModuleID, const int nModuleLineNumber)
{
	rc = tb.LogErrorToServer(szErrorText, nSeverity, nComponentID, szComponentID, szModuleID, nModuleLineNumber);
	ProcessTBReturn(rc);
	CString strFmt ;
	strFmt =_T("TBLogErrorToServer--szErrorText:%s; nSeverity:%d; nComponentID:%d; szComponentID:%s; szModuleID:%s; nModuleLineNumber:%n") ;
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt, szErrorText, nSeverity, nComponentID, szComponentID, szModuleID, nModuleLineNumber) ;

	return rc;
}

/////////////////////////////////////////////
long SMStateBase::TBGetTerminalNumber(void)
{
	rc= tb.GetTerminalNumber();
	if (rc<0)
		ProcessTBReturn(rc);
	CString strFmt ;
	strFmt =_T("TBGetTerminalNumber--Terminal#:%d") ;
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt,rc) ;

	return rc;
}

/////////////////////////////////////////////
long SMStateBase::TBGetCurrentTransactionNumber(void)
{
	rc= tb.GetCurrentTransactionNumber();
	if (rc<0)
		ProcessTBReturn(rc);
	CString strFmt ;
	strFmt =_T("TBGetCurrentTransactionNumber--Transaction Number#:%d") ;
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt,rc) ;

	return rc;
}
//////////////////////////////////////////////
// TBGetCurrentBusinessDate(CString &csBusinessDate)
////////////////////////////////////////////////
long SMStateBase::TBGetCurrentBusinessDate(CString &csBusinessDate)
{
    // call Tb function through wrapper
    rc= tb.GetCurrentBusinessDate(csBusinessDate);
	if (rc<0)
		ProcessTBReturn(rc);
    CString strFmt ;
	strFmt =_T("TBGetCurrentBusinessDate--BusinessDate#:%d %s") ;
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt,rc, csBusinessDate) ;
	return rc;
}
//////////////////////////////////////////////
// TBSetBusinessDate(LPCTSTR szSetBusinessDate)
////////////////////////////////////////////////
long SMStateBase::TBSetBusinessDate(LPCTSTR szSetBusinessDate)
{
	// call Tb function through wrapper
    rc = tb.SetBusinessDate(szSetBusinessDate);
	if (rc<0)
		ProcessTBReturn(rc);
	// log and process return code if needed
	CString strFmt(_T("TBSetBusinessDate: %s, rc=%d")) ;
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt, szSetBusinessDate, rc) ;
    return rc;

}
/////////////////////////////////
long SMStateBase::TBUnInitialize(void)
{
	rc = tb.Uninitialize();

	if (rc == TB_E_NOTINITIALIZED)
	{
		ScotError(RETURN,SM_CAT_STOP_ABORT, SCOTAPP_SM_TB_NOTINIT);
	}
	else
	  ProcessTBReturn(rc);
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, _T("TBUnInitialize")) ;
	return rc;
}

//////////////////////////////////////////////
BOOL SMStateBase::TBValidateOperator(LPCTSTR szOper, LPCTSTR szPass)
{
	BOOL brc = tb.ValidateOperator(szOper,szPass);
	CString strFmt ;

	if(co.csUserExitString5 == "OmitCustSensitiveDataInTraces")
	{
	  strFmt =_T("TBValidateOperator; ReturnCd:%d") ;
	  traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt,brc) ;
	}
	else
	{
      // TAR 246246 - We should not log operator password.
	  strFmt =_T("TBValidateOperator--OperatorID:%s; Password:xxx; ReturnCd:%d") ;
	  traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt,szOper,brc) ;
	}

	trace(L7, _T("+rp.SetOperator"));
	CString csOperatorNumber;
	csOperatorNumber.Format(_T("%s"), szOper);
	rp.SetOperator(csOperatorNumber);
	trace(L7, _T("-rp.SetOperator<%s>"),csOperatorNumber);
	return brc;
}


//////////////////////////////////////////////
//Called when starting a transaction
//parameter szLanguageCode is the ISO 639 code for the language
//selected by the customer for the transaction.  Null if just testing the TB or
//dual language is not set up
//08/17/01 segments of code also used in  SMContextHelpBase::PSButtonHelp1(void)
//if modified, please update.
SMStateBase *SMStateBase::TBStart(LPCTSTR szLanguageCode)
{
  trace(L7, _T("+ra.OnTransactionStart"));
  ra.OnTransactionStart();
  trace(L7, _T("-ra.OnTransactionStart"));

  // Generate Intervention if in Training Mode
  if ( TrainingModeObj.IsTMOn() && !TrainingModeObj.IsTransactionApproved() && !fStartTrxInAM )
	  return GenerateTrainModeIntervention();


  trace(L7, _T("+ra.OnSetTransactionState, Itemization State"));
  ra.OnSetTransactionState(RA_STATE_ITEMIZATION);
  trace(L7, _T("-ra.OnSetTransactionState"));

  //tar 350863 - begin
  _TCHAR sTotalTemp[_MAX_PATH];
  _ltot( lTotalSale, sTotalTemp, 10 );
  CString csTotal(sTotalTemp);
  //tar 350863 - end

  trace(L7, _T("+ra.OnNormalItem, Sending Total to RAP"));
  ra.OnNormalItem(_T("[Total]") + csTotal);
  trace(L7, _T("-ra.OnNormalItem, Total = %s"), csTotal);

  if(co.fStateDisplayNumberOfItemsOnRAP)
  {
	  trace(L7, _T("+ra.OnAddOtherEvent COMMAND=EnableNumItems;"));
	  ra.OnAddOtherEvent(_T("COMMAND=EnableNumItems;"));
	  trace(L7, _T("-ra.OnAddOtherEvent"));
	  CString csNumItems = EMPTY_STRING;
	  csNumItems.Format(_T("%d"), GetDisplayedItemCount());	// RFC 371075
	  trace(L7, _T("+ra.OnNormalItem, Sending the number of items to RAP"));
	  ra.OnNormalItem(_T("[NumItems]") + csNumItems);
	  trace(L7, _T("-ra.OnNormalItem, NumItems = %s"), csNumItems);
  }
  else
  {
	  trace(L7, _T("+ra.OnAddOtherEvent COMMAND=DisableNumItems;"));
	  ra.OnAddOtherEvent(_T("COMMAND=DisableNumItems;"));
	  trace(L7, _T("-ra.OnAddOtherEvent"));
  }

  if(fRequestForHelp) // tar 242625
  {
	  //TAR 371932 & tar 371918+
	  trace(L5, _T("+ra.OnCallForHelpApproval"));
      ra.OnCallForHelpApproval();
      trace(L5, _T("-ra.OnCallForHelpApproval"));
      //TAR 371932 & tar 371918-

      CString csAnchorStateName = getSCOTStateName();//Tar 247393
      CString csDesc, csInstruction;
      csDesc = ps.GetPSText(RA_CALL_FOR_HELP_ID, SCOT_LANGUAGE_PRIMARY);
      csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(BTT_HELP, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, co.IsSecurityDisableOn()==true ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), ps.GetPSText(RA_THIRDLINE_CALLFORHELP, SCOT_LANGUAGE_PRIMARY));
      trace(L7, _T("+ra.OnCallForHelp, Message=%s"), csInstruction);
      ra.OnCallForHelp(csInstruction);
      trace(L7, _T("-ra.OnCallForHelp"));
  }

  nAdvanceUnexpectedIncreaseCnt = 1; // 3.2 patch B - RFC 257450
  nAdvanceMismatchCnt = 1; // 3.2 patch B - RFC 257450
  bIsTransactionStarted = true; //TAR219278 CHU 0110602: used in SmSMmediastatus

  //TAR214210+ CHU 091102
  //Right before we call TB.start, set anchor state to ScanAndBag otherwise we
  //could put ScotApp and TB out of sync since the anchor state is Attract. If there is
  //a security violation during inProgress state, after processing security violation,
  //ScotApp goes back to Attract. It should go back to ScanAndBag
  //395551+
  if(getAnchorState() != BES_PUTBAGONSCALE)
  {
	  setAnchorState(BES_SCANANDBAG);
  }
  //395551-

  //TAR214210-
  rc = tb.Start(szLanguageCode);  // use language code as szOverrideCode parameter
  ProcessTBReturn(rc);	//Process TB return value

  CString strFmt ;
  strFmt =_T("TBStart--szLanguageCode:%s") ;
  traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt,szLanguageCode) ;
  // +Fix TAR246479
  trace(L3, _T("rc = %d, fInLoyaltyCardProcessing =%d, fStartTrxInAM = %d"), rc, fInLoyaltyCardProcessing, fStartTrxInAM);
  // -Fix TAR246479
  if(rc == TB_SUCCESS_IMMEDIATE && !fInLoyaltyCardProcessing && !fStartTrxInAM)
  {
	trace(L3, _T("tb.start returned TB_SUCCESS_IMMEDIATE (Don't wait for TB_READY)"));
    nTBLastRealState = TB_READY; //TAR 266327
	fInSignedOn  = true;		 // must be signed on by now
	fInSignedOff = false;
	if (TrainingModeObj.IsTMOn())
	{
	  //T.D 04/30/02 TAR 202117
      // TAR 248037 - Need to remove the ***, there is no room on RAP
	  CString csRAPReceiptLine = ps.GetPSText(MSG_TRAINING_MODE_LINE,SCOT_LANGUAGE_PRIMARY);
      int i = csRAPReceiptLine.FindOneOf(_T("***"));
	  csRAPReceiptLine = csRAPReceiptLine.Right(csRAPReceiptLine.GetLength() - (i+3));
      csRAPReceiptLine.TrimLeft();
      i = csRAPReceiptLine.FindOneOf(_T("***"));
	  csRAPReceiptLine = csRAPReceiptLine.Left(csRAPReceiptLine.GetLength() - (csRAPReceiptLine.GetLength()-i+1));
      csRAPReceiptLine.TrimRight();
	  csRAPReceiptLine = RAP_RECEIPT_LINE_SUMMARY + csRAPReceiptLine;

	  trace(L7, _T("+ra.OnNormalItem, Message=%s"), csRAPReceiptLine);
	  ra.OnNormalItem(csRAPReceiptLine);
	  trace(L7, _T("-ra.OnNormalItem"));
	}
	//tar 211789 - AH and DE 08/15/02

	//+degraded mode tars
    if ( co.IsDegradedModeOn() && 
		(dm.GetDegradedModeType() != DEGRADEDMODE_OFF) )
    {
		RETURNSTATE(DegradedContinueTrans)	
    }
	//-degraded mode tars
	
	//go to SecUnExpectedIncrease screen if quiet mode
	if (bQuietNeedSecApproval && !fStateAssist)
	{
		 CREATE_AND_DISPENSE(SecUnExpectedIncrease)(BES_SCANANDBAG, BES_SCANANDBAG);
	}
	
	// if item is scanned from attract, wait to go to proper sale screen
	if (fItemIsWaiting)
	{
	  setAnchorState(BES_SCANANDBAG);
	  SMStateBase* retState = TBItemSale(false,0,true);
	  // go to inprogress
	  return retState;
	}
	//TAR338648+
	else if (!fSAInTransaction)
	{
		SASendSCOTInput( ITEMIZATION ); //tell SM to start a transaction
	}
	//TAR338648-

	//395551+
	return createAnchorState();
	//395551-
  }
  //TB_SUCCESS


  // Tell InProgress what type of state we are and what its timeout should be
  CREATE_AND_DISPENSE(InProgress)(storeMode(),
									  helpMode(),
									  co.getTimeOutSignon());
}


//////////////////////////////////////////////
SMStateBase *SMStateBase::TBFinish(void)
{
	fInSignedOn = false;
	fInSignedOff = true;
	m_pSavedStateInTendAccepted = STATE_NULL;
	// moved it to here since remote cash tender does not call TBEnterTender()

	//tar 209537
	if (!InRemoteDeviceMode())
		ra.RequestEnableAssistMode(false);	 //TAR 213249
		//ra.OnSetTransactionState(RA_STATE_ASSISTNOTOK);
	//tar 209537
	rc=tb.Finish();
	ProcessTBReturn(rc);
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, _T("TBFinish")) ;
	// Tell InProgress what type of state we are and what its timeout should be
	//Tar 124345
	CREATE_AND_DISPENSE(InProgress)(storeMode(), helpMode(), co.getTimeOutSignoff());
  //return STATE_NULL;	//Tar 126024
}



/*
///////////////////////////////////////////////

SMStateBase *SMStateBase::TBEnterDOB(int nYear, int nMonth, int nDay)
{
	rc = tb.EnterDOB(nYear, nMonth, nDay);
		ProcessTBReturn(rc);
	CREATE_AND_DISPENSE(InProgress)(storeMode(),
		helpMode(),
		co.getTimeOutSignoff());
}
*/

//////////////////////////////////////////////
SMStateBase * SMStateBase::TBEndOfTransaction(void)
{
  CString csSlipFile = SLIP_FILE;
  CString csLReceiptFile = LRECEIPT_FILE;

  GET_PARTITION_PATH(SLIP_FILE, csSlipFile.GetBuffer(_MAX_PATH));
  GET_PARTITION_PATH(LRECEIPT_FILE, csLReceiptFile.GetBuffer(_MAX_PATH));

  csSlipFile.ReleaseBuffer();
  csLReceiptFile.ReleaseBuffer();

  //TAR 277732 If there is any delay security, tell SM to clear all
  if (isSecurityApprovalRequired())
  {
	  lSAYellowNeedingApproval = 0;

	//  SAClearOtherTallies();

	  // remote approval
	  trace(L7, _T("+ra.OnSecurityApproval"));
	  ra.OnSecurityApproval();
	  trace(L7, _T("-ra.OnSecurityApproval"));

	  lSALoggedMsgs  = 0;
	  CKeyValueParmList parmList;
	  CKeyValue<const _TCHAR*>::Put(parmList, KEY_OSE_EXCEPTION_ID, _T("*"));
	  CKeyValue<const _TCHAR*>::Put(parmList, KEY_OSM_OPERATION, KEY_EXCEPTION_OPERATION_CLEAR);
	  _bstr_t bstrParmList((const _TCHAR *)parmList);
	  SecMgr.OverrideException(lSecMgrCookie, bstrParmList);
	  ps.ResetDelaySecurityList();
	  g_bClearAllDelayedSecIntervention = true;
  }
  if ( !fInVoidTrans ) //tar 412495
  {
	//TAR247289+
	//Don't print the receipt yet. Wait until coupon is inserted
	if (getAnchorState() == BES_INSERTCOUPON || getAnchorState() == BES_VOUCHERPAYMENT) //TAR366924
	{
		if(IsUsingCouponSensor()) //TAR 379078
   		{
	 		//coupon not inserted yet
			CREATE_AND_DISPENSE(InsertCoupon)(getAnchorState(), BES_FINISH);
		}
		else
		{
			RETURNSTATE(DepositCoupons)		//added fix on TAR379078
		}
	}
	//TAR247289-
  }

  traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, _T("End of Transaction"));

  //for Print Last Receipt module
  RemoveFile(csSlipFile);  //remove slip file if it exists

	HFILE hFile;
  //bTrxClosed = true;	// 124345

	// either cash or credit (no sig) or debit (no sig)
 // if(TenderRequireSignature())

    if(nSavedTenderButtonIndex != -1) //TAR259370
    {
        nTenderButtonIndex = nSavedTenderButtonIndex;
    }

	SaveReceiptData();  //Query TB for slips it might want to log

	if(
	   (nTenderButtonIndex == -1) ||
	   (!GetTenderButton().bCaptureSignature && !GetTenderButton().bVerifySignature && (co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nVerifySignatureOverAmount == -1 || co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nVerifySignatureOverAmount  > lLastTender ))||
	   (nTenderType == TB_TENDER_CASH) || (nTenderType == TB_TENDER_CASH_RAP)  || //Tar 205538
	   (nTenderType == TB_TENDER_NONE)
	  )
	{
		if (lBalanceDue < 0.01)
		{
			if(lItems > 0) //if there are items sold
			{
				trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(RA_TENDER_COMPLETE, SCOT_LANGUAGE_PRIMARY));
				ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY +
					ps.GetPSText(RA_TENDER_COMPLETE, SCOT_LANGUAGE_PRIMARY));
				trace(L7, _T("-ra.OnNormalItem"));
			}
			else	//Don’t send TENDER_COMPLETE to RAP if transaction is cancelled
				trace(L6, _T("Don’t send TENDER_COMPLETE to RAP for cancelled transaction."));
		}
		
 	        EndOfTransactionPrint(); //SR697 - Delay receipt printing until finalization complete
	}
	else  // if the signature capture or verify signature are configured
	{										// do paper signature.
		if (GetTenderButton().bCaptureSignature)  // if the signature capture is  configured
		{										// do  signature capturing.
			RETURNSTATE(RequestSig) 		 //TAR#132072 -- Proceed SigCap after tendering
		}
		else		// if the sig needs to be verified, slip receipt will be printing for user to sign on
		{											// it is also a fix for TAR 120453
			hFile = TBGetSlipData();
			if (hFile!=NULL)
			{
				if ( !co.fOperationsPrintReceiptOnCancelledTransaction && ( fInVoidTrans || (lVoidedItems > 0 && lItems <= 0))  )
				{
					trace(L6, _T("SMStateBase::TBEndOfTransaction() - Don’t save receipt data  for cancelled transaction."));
				}
				else
				{
					//for Print Last Receipt module
					SaveLastReceiptData(hFile, csSlipFile);	//save slip receipt data
					RemoveFile(csLReceiptFile);
					//end of for Print Last Receipt module
				}

				long nOldTrxReceiptLength = nTrxReceiptLength;	// save this since slip printing is going to change this
				long nSlipDataLen = PrintReceipt(hFile);		// print signature slip file
				TBReleaseSlipData(hFile);	// the file can not be deleted, real receipt will print later
				if (nSlipDataLen)
				{
					CutReceipt();		// cut receipt
					nTrxReceiptLength = nOldTrxReceiptLength;
				}
			}
			DMMSREject();		// Eject Card
			if (!ps.RemoteMode())  //Not remote tender
				RETURNSTATE(SignCharge) 		//TAR#132072 -- Proceed SigCap after tendering
			else
			{
				if (lBalanceDue < 0.01)
				{
					if(lItems > 0) //if there are items sold
					{
						trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(RA_TENDER_COMPLETE, SCOT_LANGUAGE_PRIMARY));
						ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY +
								ps.GetPSText(RA_TENDER_COMPLETE, SCOT_LANGUAGE_PRIMARY));
						trace(L7, _T("-ra.OnNormalItem"));
					}
					else	//Don’t send TENDER_COMPLETE to RAP if transaction is cancelled
						trace(L6, _T("Don’t send TENDER_COMPLETE to RAP for cancelled transaction."));

				}
				
				EndOfTransactionPrint(); //SR697 - Delay receipt printing until finalization complete
			}
		}
	}


	// this looks odd but if there is no card inserted
	// the TakeCard will skip its presentation anyway
	// if no tender, eg. coupon over balance due and cash returned
	// but a card was inserted, then we must still eject the card
	if (fInVoidTrans)
	{
	  if ( storeMode() && !m_fVoidedInAssist)   // TAR246858
		  RETURNSTATE(SmAborted)
 	  m_fVoidedInAssist=false;                  // TAR246858
	}

	//Tar 124345
 // if (!bReadyForTakeReceipt)	//	not ready to go to take receipt screen,  in
	 //return STATE_NULL;	  // Take card or take change screen

	//TAR247289+
	if (getAnchorState() == BES_INSERTCOUPON )
	{
		//coupon not inserted yet
		RETURNSTATE(InsertCoupon)
	}
	//TAR247289-

	CREATE_AND_DISPENSE(TakeCard)(true);
  //if (isAnySoldVoidedItems())
  //  RETURNSTATE(TakeReceipt); 		  // from the StoryBoard end of transaction

  //RETURNSTATE(ThankYou);
}
//+SR697 gc185038
bool SMStateBase::AllowImmediateReceiptPrint()
{
	trace(L6,_T("+AllowImmediateReceiptPrint"));

	bool bStartPrintProcess = false; // flag to start the printing process
	CString csCurrentStateName = EXTRACT_CLASSNAME(this);

	if(csCurrentStateName == co.csOperationsEOTReceiptPrintingState ||
		co.csOperationsEOTReceiptPrintingState == _T("") ||
		co.csOperationsEOTReceiptPrintingState == _T("Any") ||
		(m_bForceReceiptPrint))
	{
		trace(L6,_T("Start Print Process"));
		bStartPrintProcess = true;
	}
	else
	{
		trace(L6, _T("Hold printout until after possible security interventions"));
	}
    
	trace(L6,_T("EOTReceiptPrintingState =%s, Current State=%s, Start Print Process=%d"),
		(LPCTSTR) co.csOperationsEOTReceiptPrintingState,(LPCTSTR) csCurrentStateName, 
		bStartPrintProcess);
	trace(L6,_T("-AllowImmediateReceiptPrint"));

	return bStartPrintProcess;
}

void SMStateBase::EndOfTransactionPrint()
{
	trace(L6,_T("+EndOfTransactionPrint"));

	if(AllowImmediateReceiptPrint())
	{
		m_bEOTPrintPending = false;
		m_bForceReceiptPrint = false;
		
		HFILE hFile;

		hFile = TBGetReceiptData();
		if (hFile!=NULL)
		{
            if (NULL != hReceiptFile)
            {
                trace(L6, _T("ASYNCHRONOUS PRINT ISSUED, POSSIBLE RECEIPT FILE HANDLE LEAK"));
            }
			hReceiptFile = hFile;
			PrintReceipt(hFile);	// print receipt
			if (nTrxReceiptLength)
			{
				if(co.fOperationsAddPaperCutToTransReceipt)		
				{
					trace(L6, _T("Adding Papercut to receipt"));
					CutReceipt();		// cut receipt
				}
			}
		}

		// Journal Printer
		hFile = TBGetJournalData();   // print the journal last
		if (hFile!=NULL)
		{
			hJournalFile = hFile;
			PrintJournal(hFile);
			TBReleaseJournalData(hFile);
		}

		// +Removed for TAR 390646
        // SaveSigReceiptData should not be always called whenever printing a receipt. Not all transactions have signatures.
		//if (co.m_bOperationsPrintSignatureReceipt)
		//{
		//	BSTR pData= NULL;
		//	gpSCOTSigCap->Get5991Format(&pData);  
		//	SaveSigReceiptData(pData) ;	
		//	SysFreeString(pData);
		//}
		// -TAR 390646	
	}
	else
	{
		m_bEOTPrintPending = true;
	}

	trace(L6,_T("-EndOfTransactionPrint"));
}

void SMStateBase::ProcessPendingActions() 
{
	trace(L6,_T("+ProcessPendingActions"));
	if(m_bEOTPrintPending)
	{
		EndOfTransactionPrint();
	}
	trace(L6,_T("-ProcessPendingActions"));
}
//-SR697 gc185038

//////////////////////////////////////////////
HFILE SMStateBase::TBGetSlipData(void)
{
//Fiscal Printer start
if (co.fOperationsTBControlsPrinter)
	return NULL;
//Fiscal Printer end
	HFILE hFile = tb.GetSlipData();

	//tar 260690
	if (hFile == -1)
		hFile = NULL;

	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, _T("TBGetSlipData--hFile: %x"), hFile) ;
	long nFileLength = 0;
	if (hFile)
	{
		try
		{
#if _MFC_VER < 0x0700
          nFileLength = (long)CFile( hFile ).GetLength();
#else
          nFileLength = (long)CFile( (HANDLE)hFile ).GetLength();
#endif
		}
		catch(CFileException *e)
		{
			_TCHAR szText[100];
			e->GetErrorMessage(szText,100);
			ScotError(INFO,SM_CAT_FILE_IO, SCOTAPP_TB_PRINTERFILE,_T("%s"),szText);
			e->Delete(); //TAR247967

		} // of catch
		trace(L6, _T("TBGetSlipData() returns %d handle and its length:%d"),hFile,nFileLength);
	}
	if (hFile && !nFileLength)
	{
		// valid handle but of zero length
		// SCOT still needs TB to release this fake handle
		trace(L6, _T("!!!! POSSIBLE TB Slip error: TBGetSlipData() returns %d handle with zero length !!!!"),hFile);
		TBReleaseSlipData(hFile);
		return NULL;
	}
	else if ( hFile && !co.fOperationsPrintReceiptOnCancelledTransaction && ( fInVoidTrans || (lVoidedItems > 0 && lItems <= 0))  )
	{
		trace(L6, _T("SMStateBase::TBGetSlipData() - Release slip data  for cancelled transaction."));
		TBReleaseSlipData(hFile);
		return NULL;
	}
	return hFile ;
}
long SMStateBase::TBGetGiftCardBalance(long *plBalance)
{
    // call Tb function through wrapper
    rc= tb.GetGiftCardBalance(plBalance);
    if (rc<0)
	ProcessTBReturn(rc);

	return rc;
}

long SMStateBase::TBLogReceipt(LPTSTR* lpFileName,BSTR* bstrSignatureString, LPTSTR* lpReceiptType)
{
    // call Tb function through wrapper
    rc= tb.LogReceipt(lpFileName, bstrSignatureString,lpReceiptType);
    if (rc<0)
	ProcessTBReturn(rc);

	return rc;
}

long SMStateBase::TBPreprocessSignatureData(LPCTSTR lpFileName,BSTR bstrSignatureData, LPCTSTR lpReceiptType, BOOL* bProcessData)
{
    // call Tb function through wrapper
    rc= tb.PreprocessSignatureData(lpFileName, bstrSignatureData, lpReceiptType,bProcessData );
    if (rc<0)
	ProcessTBReturn(rc);

	return rc;
}

long SMStateBase::TBGetNextState(LPCTSTR lpLastStateName,
                                 LPCTSTR lpNextStateName,
                                 bool *pbOverride,
                                 LPTSTR *lpOverrideStateName,
                                 CDescriptorList &dlParameterList)
{
    rc = tb.GetNextState(lpLastStateName, lpNextStateName, pbOverride, lpOverrideStateName, dlParameterList);
    if (rc < 0)
        ProcessTBReturn(rc);
    return rc;
}

bool SMStateBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return true;
}

TBRC SMStateBase::SendTBCommand(  long lCommandID, ...)
{
    va_list pArgs;
    va_start(pArgs, lCommandID);
    TBRC retCode = tb.SendTBCommand(lCommandID, pArgs);
    va_end(pArgs);
    return retCode;
}

//////////////////////////////////////////////
HFILE SMStateBase::TBGetReceiptData(void)
{
//Fiscal Printer start
if (co.fOperationsTBControlsPrinter)
	return NULL;
//Fiscal Printer end
	HFILE hReceiptFile = tb.GetReceiptData();

	//tar 260690
	if (hReceiptFile == -1)
		hReceiptFile = NULL;

	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, _T("TBGetReceiptData--hReceiptFile:0x%x"), hReceiptFile) ;
	if (isAnySoldVoidedItems() && !hReceiptFile)
	{
		// if it is a null handle but trx has sold or voided items
		ScotError(INFO,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_NULL_RECEIPT, _T("%d|%d"), hReceiptFile, 0);
	}


	long nFileLength = 0;
	if (hReceiptFile)
	{
		try
		{
#if _MFC_VER < 0x0700
            nFileLength = (long)CFile( hReceiptFile ).GetLength();
#else
            nFileLength = (long)CFile( (HANDLE)hReceiptFile ).GetLength();
#endif
		}
		catch(CFileException *e)
		{
			_TCHAR szText[100];
			e->GetErrorMessage(szText,100);
			ScotError(INFO,SM_CAT_FILE_IO, SCOTAPP_TB_PRINTERFILE,_T("%s"),szText);
			e->Delete(); //TAR247967

		} // of catch
		trace(L6, _T("TBGetReceiptData() returns %d handle and its length:%d"),hReceiptFile,nFileLength);
	}
	if (hReceiptFile && !nFileLength)
	{

		// valid handle but of zero length

		// SCOT still needs TB to release this fake handle
		trace(L6, _T("!!!! POSSIBLE TB Receipt error: TBGetReceiptData() returns %d handle with zero length !!!!"),hReceiptFile);
		TBReleaseReceiptData(hReceiptFile);
		ScotError(INFO,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_NULL_RECEIPT, _T("%d|%d"), hReceiptFile, 0);
		return NULL;
	}
	// let the caller of this function find about ...
	// it it is an invalid file handle
	//for Print Last Receipt module
	else if( hReceiptFile && !co.fOperationsPrintReceiptOnCancelledTransaction && ( fInVoidTrans || (lVoidedItems > 0 && lItems <= 0)) )
	{
		trace(L6, _T("SMStateBase::TBGetReceiptDatat() - Release receipt data  for cancelled transaction."));
        
        //TAR 425084
        //With G2 a new mechanism for handling the receipt handle has been created
        //The release of the receipt will fail if the handle is not present in
        //the array.  Add the handle to the array so that it can be deleted.
        g_ArrayOfReceiptHandles.Add( hReceiptFile);
        //tar
		
        TBReleaseReceiptData(hReceiptFile);
		return NULL;
	}

	if ( !co.fOperationsPrintReceiptOnCancelledTransaction && ( fInVoidTrans || (lVoidedItems > 0 && lItems <= 0))  )
	{
		trace(L6, _T("SMStateBase::TBGetReceiptDatat() - Don’t save receipt data  for cancelled transaction."));
	}
	else
	{
		CString csLReceiptTmpFile = LRECEIPT_TMP_FILE;
		GET_PARTITION_PATH(LRECEIPT_TMP_FILE, csLReceiptTmpFile.GetBuffer(_MAX_PATH));
		csLReceiptTmpFile.ReleaseBuffer();

		SaveLastReceiptData(hReceiptFile, csLReceiptTmpFile);  //save receipt data
	}

	// start of TAR 393349
	if( hReceiptFile != NULL)
	{
    int nIndex = g_ArrayOfReceiptHandles.Add( hReceiptFile);
    trace(L6,_T("TAR 393349 (Adding HFILE (%08x) to Array)"), hReceiptFile);
	}
	// end of TAR 393349

	
	return hReceiptFile;
}

HFILE SMStateBase::TBGetJournalData(void)
{
//Fiscal Printer start
if (co.fOperationsTBControlsPrinter)
	return NULL;
//Fiscal Printer end
	HFILE hJournalFile = tb.GetJournalData();

	//tar 260690
	if (hJournalFile == -1)
		hJournalFile = NULL;

	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, _T("TBGetJournalData--hJournalFile: %x"), hJournalFile) ;
	if (isAnySoldVoidedItems() && !hJournalFile)
	{
		// if it is a null handle but trx has sold or voided items
		trace(L6, _T("No Journal data"));
	}
	long nFileLength = 0;
	if (hJournalFile)
	{
		try
		{
#if _MFC_VER < 0x0700
            nFileLength = (long)CFile( hJournalFile).GetLength();
#else
            nFileLength = (long)CFile((HANDLE)hJournalFile).GetLength();
#endif
		}
		catch(CFileException *e)
		{
			_TCHAR szText[100];
			e->GetErrorMessage(szText,100);
			ScotError(INFO,SM_CAT_FILE_IO, SCOTAPP_TB_PRINTERFILE,_T("%s"),szText);
			e->Delete(); //TAR247967

		} // of catch
		trace(L6, _T("TBGetJournalData() returns %d handle and its length:%d"),hJournalFile,nFileLength);
	}
	if (hJournalFile && !nFileLength)
	{
		// valid handle but of zero length
		// SCOT still needs TB to release this fake handle
		trace(L6, _T("!!!! POSSIBLE TB journal data error: TBGetJournalData() returns %d handle with zero length !!!!"),hJournalFile);
		TBReleaseJournalData(hJournalFile);
		ScotError(INFO,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_NULL_JOURNALDATA, _T("%d|%d"), hJournalFile, 0);
		return NULL;
	}
	return hJournalFile ;
}

//////////////////////////////////////////////
CString SMStateBase::TBGetDisplay(void)
{
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, _T("TBGetDisplay--szLastDisplay: %s"), csLastDisplay) ;

	return csLastDisplay;

}

//////////////////////////////////////////////
void SMStateBase::TBReleaseReceiptData(HFILE hFile)
{
//Fiscal Printer start
if (! co.fOperationsTBControlsPrinter)
 {
//Fiscal Printer end
       // start of TAR 393349
    TBRC rc = TB_SUCCESS;

    try
    {
      CFile cFile( hFile);
      if( 0 == cFile.SeekToEnd())
      {
        // - If the receipt file is of zero length, FastLane Base has imediately
        //   called TBReleaseReceiptData from SMStateBase::TBGetReceiptData.
        // - Therefore automatically delete it, free its handle, and clean up
        //   any outstanding TB flags.
        tb.ReleaseReceiptData( hFile);
        trace(L6,_T("TAR 393349 (If zero in length, free receipt file and handle(%08x))"), hFile);
      }
      else
      {
        // - If the receipt file is of non-zero length,
        //   search the g_ArrayOfReceiptHandles array
        //   for it and other older handles to clean up.
        bool bMatchFound = false;
        int x;
        for (x = 0; x < g_ArrayOfReceiptHandles.GetSize(); x++)
        {
          if (g_ArrayOfReceiptHandles.GetAt(x) == hFile)
          {
            bMatchFound = true;
            break;
          }
        }

        if (bMatchFound)
        {
          int counter = 0;
          while (counter <= x)
          {
            HFILE myHFILE = g_ArrayOfReceiptHandles.GetAt(0);
            trace(L6,_T("TAR 393349 tb address [%08x]"), &tb);

            rc = tb.ReleaseReceiptData(myHFILE);

            g_ArrayOfReceiptHandles.RemoveAt(0);
            trace(L6,_T("TAR 393349 (Removing HFILE (%08x) from Array)"), myHFILE);
            counter ++;
          }
        }
      }
    }
    catch (...)
    {
      trace(L6,_T("TAR 393349 (Skipping unnamed receipt file - presumably it has already be deleted and the handle (%08x) freed)"), hFile);
    }
    // end of TAR 393349

  if (rc==TB_E_INVALIDPARAM)
  {
	ScotError(RETURN,SM_CAT_INPUT_PARAMETER, SCOTAPP_SM_TB_PARA);
  }
  else
	ProcessTBReturn(rc);
//Fiscal Printer start
 }
//Fiscal Printer end
}



//////////////////////////////////////////////
void SMStateBase::TBReleaseSlipData(HFILE hFile)
{
//Fiscal Printer start
if (! co.fOperationsTBControlsPrinter)
 {
//Fiscal Printer end
	rc= tb.ReleaseSlipData(hFile);
	if (rc==TB_E_INVALIDPARAM)
	{
		ScotError(RETURN,SM_CAT_INPUT_PARAMETER, SCOTAPP_SM_TB_PARA);

	}
	else
		ProcessTBReturn(rc);

	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, _T("TBReleaseSlipData--hFile %x"), hFile) ;
//Fiscal Printer start
 }
//Fiscal Printer end
}

///////////////////////////////////////////////
void SMStateBase::TBReleaseJournalData(HFILE hFile)
{
//Fiscal Printer start
if (! co.fOperationsTBControlsPrinter)
 {
//Fiscal Printer end
	rc=tb.ReleaseJournalData(hFile);
	if (rc==TB_E_INVALIDPARAM)
	{
	ScotError(RETURN,SM_CAT_INPUT_PARAMETER, SCOTAPP_SM_TB_PARA);
	}
	else
		ProcessTBReturn(rc);
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, _T("TBReleaseJournalData--hFile %x"), hFile) ;
//Fiscal Printer start
 }
//Fiscal Printer end
}
/////////////////////////////////////////////
CString SMStateBase::TBName(void)
{
	CString cVersionString;
	rc= tb.GetVersion(cVersionString);
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, _T("TBName--cVersionString %s"), cVersionString) ;
	return cVersionString;
}


//////////////////////////////////////////////
TBRC SMStateBase::TBGetPINWorkingKey(CString &csAct,
								CString &csKey,
								TBTENDERTYPE nTyp,
				TBPINBLOCKFORMAT& nPinBlockFormat,
				TBPINPADSYSTEM& nPinpadSystem,
				TBPINENCODING& nPinBlockEncoding)
{
	SPINWorkingKey sPinKey;
	TBRC rc;
	memset(&sPinKey, 0, sizeof(SPINWorkingKey));

	sPinKey.pchMSRData		  = cDMLastTrack;
	sPinKey.nMSRDataLength	  = nDMLastTrackLen;
	sPinKey.nAccountType	  = nTyp;
	sPinKey.nPinBlockFormat   = nPinBlockFormat;
	sPinKey.nPinpadSystem	  = nPinpadSystem;
	sPinKey.nPinBlockEncoding = nPinBlockEncoding;

	try
	{
		rc = tb.GetPINWorkingKey(&sPinKey);
	}
	catch (...)
	{
		trace(L3, _T("Got an exception in TBGetPINWorkingKey()"));
	}
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_ERROR | TRACE_LEVEL_WARNING , _T("TBGetPINWorkingKey -- RC:%d"), rc) ;
	// ProcessTBReturn(rc); // koteswara, for testing
	if (rc != TB_SUCCESS)
		return rc;
	csAct = sPinKey.szAccountNumber;
	csKey = sPinKey.szWorkingKey;

	// TAR# 127873 - support changing PIN block format from ANSI to Diebold
	nPinBlockFormat 	= sPinKey.nPinBlockFormat;
	nPinpadSystem		= sPinKey.nPinpadSystem;
	nPinBlockEncoding	= sPinKey.nPinBlockEncoding;

	if(sPinKey.szWorkingKey)
		try{	delete [] sPinKey.szWorkingKey;} catch(...){trace(L6,_T("delete exception:sPinKey.szWorkingKey"));}
	if(sPinKey.szAccountNumber)
		try{	delete [] sPinKey.szAccountNumber;} catch(...){trace(L6,_T("delete exception:sPinKey.szAccountNumber"));}

	return rc;
}

//////////////////////////////////////////////
bool SMStateBase::TBPromptForDebitAccountType(void)
{
	// no different debit card type
	return false;
}

///////////////////////////////
//Gets the Available Balance on Food Stamp Card that can
//be used to pay for Food Stamp Items
SMStateBase *SMStateBase::TBGetAvailableFoodStampAmount(void)
{
	SMStateBase *sUser = STATE_NULL;

	SEBTAvailBalance dAvailBal;
	memset(&dAvailBal, 0, sizeof(SEBTAvailBalance));


	rc= tb.GetAvailableFoodStampAmount(&dAvailBal);
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO , _T("TBGetAvailableFoodStampAmount -- lAvailBal:%d"), dAvailBal.lAvailBal) ;
	ProcessTBReturn(rc);

	// extract the Available Balance
	lEBTAvailableBalance = dAvailBal.lAvailBal;


	CREATE_AND_DISPENSE(LimitedEBTBenefit)(TB_TENDER_FOODSTAMPS, lEBTAvailableBalance);
//	return STATE_NULL;
}


///////////////////////////////
//Gets the Available Balance on Benefit Card that can
//be used to pay for Items
SMStateBase *SMStateBase::TBGetAvailableBenefitAmount(void)
{
	SMStateBase *sUser = STATE_NULL;

	SEBTAvailBalance dAvailBal;
	memset(&dAvailBal, 0, sizeof(SEBTAvailBalance));


	rc= tb.GetAvailableBenefitAmount(&dAvailBal);
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO , _T("TBGetAvailableBenefitAmount -- lAvailBal:%d"), dAvailBal.lAvailBal) ;
	ProcessTBReturn(rc);

	// extract the Available Balance
	lEBTAvailableBalance = dAvailBal.lAvailBal;

	//return STATE_NULL;
	CREATE_AND_DISPENSE(LimitedEBTBenefit)(TB_TENDER_EBT, lEBTAvailableBalance);
}

///////////////////////////////
SMStateBase *SMStateBase::TBRequestFoodStampEligibleTotal(void)
{
	SMStateBase *sUser = STATE_NULL;

	rc= tb.RequestFoodStampEligibleTotal();
	ProcessTBReturn(rc);

	return STATE_NULL;
}

///////////////////////////////
SMStateBase *SMStateBase::TBParseFoodStampTotal(void)
{
	SMStateBase *sUser = STATE_NULL;

	STotalDetails dTotal;
	memset(&dTotal, 0, sizeof(STotalDetails));


	rc= tb.ParseFoodStampTotal(&dTotal);
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO ,    \
		_T("TBParseFoodStampTotal--TotalDetails TaxDue: %d; Total: %d; Discount: %d; ErrDesc: %s; FoodStampDue: %d"),  \
		dTotal.lTaxDue, dTotal.lTotal, dTotal.lDiscount, dTotal.szErrDescription, dTotal.lFoodStampDue) ;

	ProcessTBReturn(rc);

	// extract just the Food Stamp total
	lFoodStampsDue = dTotal.lFoodStampDue;

     // +TAR457302
    if (g_bTBNeedMoreData)
    {
        return STATE_NULL;
    }
    //-TAR457302

	return createAnchorState();
}

// Tar 125278
// Enter or Leave lane Closed state
void SMStateBase::TBLaneClosed(const bool bEnter)
{
	if (bLaneClosed != bEnter)
	{
		rc = tb.LaneClosed(bEnter);
		ProcessTBReturn(rc);
		traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO , _T("TBLaneClosed -- EnterClose:%d"), bEnter) ;
		if (rc == TB_SUCCESS && !co.fStateStartupWaitForTB)
		{
			bLaneClosed = bEnter;
		}
	}
	return;
}

// Enter or Leave lane Closed state
void SMStateBase::TBEnterTenderState(const bool bEnter)
{

	if (bEnterTenderState != bEnter)

	{
		rc = tb.EnterTenderState(bEnter);
		ProcessTBReturn(rc);
		traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO , _T("TBEnterTenderState -- bEnter:%d"), bEnter) ;

		bEnterTenderState = bEnter;
	}
	return;
}

void SMStateBase::TBSendSigData(LPCTSTR CustomerName, LPCTSTR cSigData, DWORD nLength)
{
	rc = tb.SendSigData(CustomerName, cSigData, nLength);
	ProcessTBReturn(rc);
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO , _T("TBSendSigData-- CustomerName:%s, nLength:%d, Return Code: %d "), CustomerName, nLength, rc) ;
	return;
}

SMStateBase* SMStateBase::TBGetMessageInfo(const bool disableSA)  // TAR 127247
{
	SMessageInfo sTempMessageInfo;
	memset(&sTempMessageInfo, 0, sizeof(SMessageInfo));

	rc = tb.GetMessageInfo(&sTempMessageInfo);
	ProcessTBReturn(rc);

	CString strFmt ;
	strFmt =_T("szLeadThruText= %s; szScreenText=%s; szPlayWave=%s;") ;
	strFmt+=_T(" szE_ReceiptText=%s; bHiliteReceiptText=%d; bNeedItemCancel=%d, bApprovalNeeded=%d;") ;
	strFmt+=_T(" bGoBackScanAndBag=%d; szRAP_MessageText=%s; bHiliteRAPText=%d") ;

    traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt,
			 sTempMessageInfo.szLeadThruText,
			 sTempMessageInfo.szScreenText,
			 sTempMessageInfo.szPlayWave,
			 sTempMessageInfo.szE_ReceiptText,
			 sTempMessageInfo.bHiliteReceiptText,
			 sTempMessageInfo.bNeedItemCancel,
			 sTempMessageInfo.bApprovalNeeded,
			 sTempMessageInfo.bGoBackScanAndBag,
			 sTempMessageInfo.szRAP_MessageText,
			 sTempMessageInfo.bHiliteRAPText);

	// save data to constant object.
	CustomMessageObj.csScreenText		= sTempMessageInfo.szScreenText;
	CustomMessageObj.csPlayWave 		= sTempMessageInfo.szPlayWave;
	CustomMessageObj.csLeadThruText 	= sTempMessageInfo.szLeadThruText;
	CustomMessageObj.csE_ReceiptText	= sTempMessageInfo.szE_ReceiptText;
	CustomMessageObj.bHiliteReceiptText = sTempMessageInfo.bHiliteReceiptText;
	CustomMessageObj.bApprovalNeeded	= sTempMessageInfo.bApprovalNeeded;
	CustomMessageObj.bNeedItemCancel	= sTempMessageInfo.bNeedItemCancel;
	CustomMessageObj.bGoBackScanAndBag	  = sTempMessageInfo.bGoBackScanAndBag;
	CustomMessageObj.csRAP_MessageText	= sTempMessageInfo.szRAP_MessageText;
	CustomMessageObj.bHiliteRAPText     = sTempMessageInfo.bHiliteRAPText;

	// Delete temporary data.
	if (sTempMessageInfo.szScreenText)
		try{	delete [] sTempMessageInfo.szScreenText;}
		catch(...){trace(L6,_T("delete exception:sTempMessageInfo.szScreenText"));}
	if (sTempMessageInfo.szPlayWave)
		try{	delete [] sTempMessageInfo.szPlayWave;}
		catch(...){trace(L6,_T("delete exception:sTempMessageInfo.szPlayWave"));}
	if (sTempMessageInfo.szLeadThruText)
		try{	delete [] sTempMessageInfo.szLeadThruText;}
		catch(...){trace(L6,_T("delete exception:sTempMessageInfo.szLeadThruText"));}
	if (sTempMessageInfo.szE_ReceiptText)
		try{	delete [] sTempMessageInfo.szE_ReceiptText;}
		catch(...){trace(L6,_T("delete exception:sTempMessageInfo.szE_ReceiptText"));}
	if (sTempMessageInfo.szRAP_MessageText)
		try{	delete [] sTempMessageInfo.szRAP_MessageText;}
		catch(...){trace(L6,_T("delete exception:sTempMessageInfo.szRAP_MessageText"));}

	// Disable pending security if needed.
	if(CustomMessageObj.bNeedItemCancel || disableSA)
	{
		SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
		// Do delayed approval if needed.
		if(CustomMessageObj.bApprovalNeeded)
		{
            if (!co.IsInAssistMenus())
            {
                lUnknownItems++;
                //TAR 287049+
                lRegularYellowNeedingApproval++;
                dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                    DM_TRICOLORLIGHT_ON,
                    DM_TRICOLORLIGHT_NORMALREQUEST);
                CString csAnchorStateName = getSCOTStateName();
                CString csDesc, csInstruction;
                csDesc = ps.GetPSText(RA_SCANNED_UNKNOWN_ID, SCOT_LANGUAGE_PRIMARY);
                csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_LANEBTT_RECALLEDITEM, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, co.IsSecurityDisableOn()==true ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), ps.GetPSText(RA_THIRDLINE_RECALLEDITEM, SCOT_LANGUAGE_PRIMARY));
                trace(L7, _T("+ra.OnUnknownItem, Message=%s"), csInstruction);
                ra.OnUnknownItem(csInstruction, FALSE);
                trace(L7, _T("-ra.OnUnknownItem"));
                //TAR 287049-
            }
            else
            {
                SMSmAssistMenu::SetAssistMessage(LTT_UNKNOWNITEM,UNKNOWNITEM);
            }
		}

	}

    CPSConfigData cd;    //RFQ 2605 - add multilanguage capability to CustomMessage functionality
    //*** NewUI  ***//
	// Add text to receipt if needed.
	if(CustomMessageObj.csE_ReceiptText.GetLength())
	{
      cd.FormatString(CustomMessageObj.csE_ReceiptText, true);   //RFQ 2605 - Receipt text translations should be primary language
	  ps.AddMessageToReceipt(_T("CMSMReceipt"), CustomMessageObj.csE_ReceiptText );
	  ps.AddMessageToReceipt(_T("CMSmallReceipt"), CustomMessageObj.csE_ReceiptText);
	}
    //*** end of NewUI ***//

	if(CustomMessageObj.csRAP_MessageText.GetLength())
	{
       cd.FormatString(CustomMessageObj.csRAP_MessageText, true); //RFQ 2605 - RAP messages should be primary language
       CString csRAPReceiptLine;
       if(CustomMessageObj.bHiliteRAPText)
	   {
	       csRAPReceiptLine = RAP_RECEIPT_LINE_SUMMARY + CustomMessageObj.csRAP_MessageText;
	       trace(L7, _T("+ra.OnNormalItem, Message=%s"), csRAPReceiptLine);
		   ra.OnNormalItem(csRAPReceiptLine);
	       trace(L7, _T("-ra.OnNormalItem"));
	   }
       else
	   {
	       csRAPReceiptLine = CustomMessageObj.csRAP_MessageText;
	       trace(L7, _T("+ra.OnNormalItem, Message=%s"), csRAPReceiptLine);
		   ra.OnNormalItem(csRAPReceiptLine);
	       trace(L7, _T("-ra.OnNormalItem"));
	   }
	}

	// If there exists a screen text or leadthru text then create "Custom Message"
	// screen to display it.
	if(CustomMessageObj.csScreenText.GetLength() ||
	   CustomMessageObj.csLeadThruText.GetLength())
	{

       cd.FormatString(CustomMessageObj.csScreenText, false);  //RFQ 2605 - Customer screen messages should be customer language
       cd.FormatString(CustomMessageObj.csLeadThruText, false);  //RFQ 2605 - Customer message translation
	   RETURNSTATE(CustomMessage)
	}
	else
	{
	   if(CustomMessageObj.csPlayWave.GetLength())
	   {
		 DMSayPhrase(CustomMessageObj.csPlayWave);
	   }
	   bool bReturnToScanAndBag = CustomMessageObj.bGoBackScanAndBag; //tar 265812
	   CustomMessageObj.Reset();
	   if(bReturnToScanAndBag)   //tar 265812
	   {
		  //+TAR 322945
		   if(getAnchorState() == BES_START)
		   {
			   return TBStart();
		   }
		   //-TAR 322945
		 RETURNSTATE(ScanAndBag);
	   }
	   // If getting here from TB_E_ITEMFAILED, then go back to previous state.
	   // Otherwise, stay where you are since this is probably from
	   // TB_MESSAGESCREEN state
	   if (disableSA)
		 return STATE_RETURN;
	   else
		 return STATE_NULL;
	 }
}
//////////////////////////////////////////////
//	 TBCreateReportEntry(void)
//
////////////////////////////////////////////////

void SMStateBase::TBCreateReportEntry()
{
	trace(L7, _T("+SMStateBase::TBCreateReportEntry"));
	SCreateReportEntry sTempCreateReportEntry;
	memset(&sTempCreateReportEntry, 0, sizeof(SCreateReportEntry));

	rc = tb.CreateReportEntry(&sTempCreateReportEntry);
	ProcessTBReturn(rc);

	rp.TBCreateReportEntry(sTempCreateReportEntry.szRecordType, sTempCreateReportEntry.szValue);

	trace(L7, _T("-SMStateBase::TBCreateReportEntry"));
	// Delete temporary data.
	if (sTempCreateReportEntry.szRecordType)
		try{	delete [] sTempCreateReportEntry.szRecordType;}
		catch(...){trace(L6,_T("delete exception:sTempCreateReportEntry.szRecordType"));}
	if (sTempCreateReportEntry.szValue)
		try{	delete [] sTempCreateReportEntry.szValue;}
		catch(...){trace(L6,_T("delete exception:sTempCreateReportEntry.szValue"));}
}
//////////////////////////////////////////////
// TBSetTrainingModeState(const bool bTMState)
//	 bTMState = true to request TB to enter  training mode status, wait for TB_ENTEREDTRAINING state from TB
//	 bTMState = false to request TB to leave training mode status, wait for TB_LEFTTRAINING state from TB
////////////////////////////////////////////////
SMStateBase *SMStateBase::TBSetTrainingModeState(const bool bTMState)
{
	// call Tb function through wrapper
	TBRC rc = tb.SetTrainingModeState(bTMState);

	// log and process return code if needed
	ProcessTBReturn(rc);
	CString strFmt(_T("TBSetTrainingModeState: state: %d, rc=%d")) ;
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt, bTMState, rc) ;

	// handle the response for the tb call
	CREATE_AND_DISPENSE(InProgress)(storeMode(), helpMode(), co.getTimeOutSignoff());
}
/////////////////////////////////////////////////////////////////
// Process existing messages in the application's message queue.
// When the queue is empty, do clean up and return.
/////////////////////////////////////////////////////////////////
void SMStateBase::DoEvents()
{
	MSG msg;
	while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
	{
		if (!AfxGetThread()->PumpMessage())
			return;
	}
}

void SMStateBase::TBEPSEndOfDay(LPVOID lp)
{
	rc = tb.EPSEndOfDay(lp);
	ProcessTBReturn(rc);
	bEndOfDay = true;
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO , _T("TBEPSEndOfDay()"));
	return;
}

void SMStateBase::TBEPSStartOfDay(LPVOID lp)
{
	rc = tb.EPSStartOfDay(lp);
	ProcessTBReturn(rc);
	bEndOfDay = false;
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO , _T("TBEPSStartOfDay()"));
	return;
}

void SMStateBase::TBNeedStartOfDay(TBSTARTOFDAYSTATE &eState)
{
	rc = tb.NeedStartOfDay(eState);
	ProcessTBReturn(rc);
	bEndOfDay = false;
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO , _T("TBNeedStartOfDay()"));
	return;
}

//awk.2.5.2 TAR191836 Reset the globals. ResetApprovalValues() new Method.
void SMStateBase::ResetApprovalValues(const bool fUpdateSA)
{
	//TAR197104 begin
		if (isVisualItemApprovalRequired())
		{
			trace(L7, _T("+ra.OnVisualValidationApproval"));
			ra.OnVisualValidationApproval();
			trace(L7, _T("-ra.OnVisualValidationApproval"));
		}

		if (isCouponRatioApprovalRequired(fUpdateSA))
		{
			trace(L7, _T("+ra.OnCouponApproval"));
			ra.OnCouponApproval();
			trace(L7, _T("-ra.OnCouponApproval"));
		}

		if (isRestrictedItemApprovalRequired()) //awk.4.21.3, TAR232223
		{
			trace(L7, _T("+ra.OnAgeApproval"));
			ra.OnAgeApproval();
			trace(L7, _T("-ra.OnAgeApproval"));
		}

		if ((lNotForSaleItems>0)			||
			(lUnknownItems>0)				||
			(lPriceRequiredItems>0) 		||
			(lRestrictedNotAllowedItems>0)	||
			(lTimeRestrictedItems>0)		||
			(bSuspendFailure)				||
			(lCouponsInAssist>0)			||		//TAR#102019
			(lQuantityRestrictedItems>0)	||
			(fOtherPayment) 				||		// for outstanding approvals printing
			(lUnapprovedVoidedItems>0))
		{

			if (lQuantityRestrictedItems != 0)
			{
//				while ((lQuantityRestrictedItems--) > 0)
//				{
				trace(L7, _T("+ra.OnQuantityRestrictedItemApproval"));
				ra.OnQuantityRestrictedItemApproval();
				trace(L7, _T("-ra.OnQuantityRestrictedItemApproval"));
//				}
			}
			if (lNotForSaleItems != 0)
			{
//				while ((lNotForSaleItems--) > 0)
//				{
				trace(L7, _T("+ra.OnRecalledItemApproval"));
				ra.OnRecalledItemApproval();
				trace(L7, _T("-ra.OnRecalledItemApproval"));
//				}
			}
			if (lUnknownItems != 0)
			{
//				while ((lUnknownItems--) > 0)
//				{
				trace(L7, _T("+ra.OnUnknownItemApproval"));
				ra.OnUnknownItemApproval();
				trace(L7, _T("-ra.OnUnknownItemApproval"));
//				}
			}
		}
		//TAR 197104 end
	//awk.2.5.2 TAR191836 Reseting the globals.
   // Restricted Item
   lRestrictedItems = 0;
   lUnapprovedRestrictedItems = 0;
   // Unknown Items
   lUnknownItems = 0;
   lUnknownItemsKeyed = 0;
   // No Sale Items
	if(lRestrictedNotAllowedItems)
	{
		lRestrictedNotAllowedItems = 0;
        trace(L7, _T("+ra.OnRestrictedNotAllowedApproval"));
        ra.OnRestrictedNotAllowedApproval();
        trace(L7, _T("-ra.OnRestrictedNotAllowedApproval"));
	}
   // Quantity Restricted Items
   lQuantityRestrictedItems = 0;
   // Visual Verify
   lVisualVerifyItems = 0;
   lUnapprovedVisualVerifyItems = 0;
   // Beats me.
   lRegularYellowNeedingApproval = 0;
}


SMStateBase *SMStateBase::OnEndorseDocument(void)
   {
    if(nTenderButtonIndex != -1) //TAR259370
    {
        if(   GetTenderButton().bCaptureSignature
           || GetTenderButton().bVerifySignature
           || (
                  co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nVerifySignatureOverAmount != -1
               && co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nVerifySignatureOverAmount <= lLastTender
              )
          )
          nSavedTenderButtonIndex = nTenderButtonIndex;
    }

	fInEndorsementState = ENDORSE_WAITING; // let remote assist mode handle this state
	io.d.csErrDescription = ps.GetPSText(MSG_ENDORSEMENTNEEDED);
	// return to RAPDataNeeded
	CString data = ps.GetPSText(MSG_ENDORSEMENTNEEDED, SCOT_LANGUAGE_PRIMARY);  // TAR 228468
	CREATE_AND_DISPENSE(RAPDataNeeded)(data, DATANEEDED_ENDORSEDOCUMENT);
   }

SMStateBase *SMStateBase::OnNewCashDrawerState(void)
   {
   return STATE_NULL;
   }

SMStateBase *SMStateBase::OnNeedMICR(void)
   {
   return STATE_NULL;
   }

SMStateBase *SMStateBase::OnNewSecurityLevel(void)
   {
   return STATE_NULL;
   }


void SMStateBase::TBEndorsementCompleted(bool bSuppressSubsequent)
{
	tb.EndorsementCompleted(bSuppressSubsequent);
}

SMStateBase *SMStateBase::OnAssistModeStatusChanged(void)
   {
   trace (L7, _T("+-SMStateBase::OnAssistModeStatusChanged() : assist mode may/may not exit status change."));
   return STATE_NULL;
   }

SMStateBase *SMStateBase::OnTBNeedData(void)
{
    trace (L6, _T("+SMStateTBBase::OnTBNeedData()"));
    CString sDescription, sView, sINI, sViewString, sSection, csStateName;
    //RFQ 2506 - rework OnTBNeedData logic for correct display of new RAPDataNeeded text overrides
    CString sRDNLaneMessage, sRDNLaneLeadthru, sRDNValues;  //Strings for Rap Data Needed customer display
    CString sViewCustomer;
    CSCOTDataEntry cCustomerDataEntry;
    CPSConfigData cd;
    
    sDescription.Empty();
    sView.Empty();
    
    sINI.Empty();
    sSection.Empty();
    
    SDataNeededView ViewInfo;
    ViewInfo.szDescription = NULL;
    ViewInfo.szView = NULL;
    ViewInfo.szIniFile = NULL;
    ViewInfo.szSection = NULL;
    
    
    
    rc = tb.GetDataNeededView(&ViewInfo);
    ProcessTBReturn(rc);
    if(rc!=TB_SUCCESS)
    {
        trace(L4, _T("Exception with GetDataNeededView, return code=%d"), rc);
        return STATE_NULL;
    }
    
    if(ViewInfo.szDescription)
    {
        sDescription = ViewInfo.szDescription;
        try{ delete ViewInfo.szDescription;}
        catch(...){trace(L4,_T("Exception while deleting ViewInfo.szDescription"));}
    }
    
    if(ViewInfo.szView)     //String format DataNeeded parameters
    {
        sView = ViewInfo.szView;
        sViewString = sView;
        sViewCustomer = sView;
        cd.FormatString(sViewString, true);  //Primary Language Translation
        cd.FormatString(sViewCustomer, false);  //Customer Language Translation
        
        try{ delete ViewInfo.szView;}
        catch(...){trace(L4, _T("Exception while deleting ViewInfo.szView"));}
    }
    if(ViewInfo.szIniFile)
    {
        sINI = ViewInfo.szIniFile;
        try{ delete ViewInfo.szIniFile;}
        catch(...){trace(L4, _T("Exception while deleting ViewInfo.szIniFile"));}
    }
    if(ViewInfo.szSection)
    {
        sSection = ViewInfo.szSection;
        try{ delete ViewInfo.szSection;}
        catch(...){trace(L4, _T("Exception while deleting ViewInfo.szSection"));}
    }
    if(sINI != _T(""))
    {
        if(sSection != _T(""))
        {   //Dataneeded parameters to be retrieved from INI file
            if (cDataEntry.FromConfigData(sINI, sSection))
            {
                cCustomerDataEntry.FromConfigData(sINI, sSection, false);
            }
            else
            {
                trace(L4, _T("Configuration Error: No Section data for INIfile=%s, Section=%s"),sINI, sSection);
            }

            // Adding sView/sViewCustomer after the cDataEntry settings gives 
            // precedence to settings in the INI file. 
            sViewString = cDataEntry.ToString() + sViewString;
            sViewCustomer = cCustomerDataEntry.ToString() + sViewCustomer;
        }
        else
        {
            trace(L4, _T("GetDataNeededView error, if INI specified, section is required"));
            return STATE_NULL;
        }
    }
    else if(sView == _T("") )
    {
        trace(L4, _T("GetDataNeededView error, INI and View are both empty, one of them is required"));
        // TAR 276329 start
        if (g_bClearOccurred)
        {
            SAClearSecurityCondition();
            return createAnchorState();
        }
        // TAR 276329 end
        return STATE_NULL;
    }

    cCustomerDataEntry.FromString(sViewCustomer);
    sRDNLaneMessage = cCustomerDataEntry.GetRDNLaneMessage();
    sRDNLaneLeadthru = cCustomerDataEntry.GetRDNLaneLeadthru();
    
    if (!cCustomerDataEntry.IsStoreLogonEnabled() && !co.IsInAssistMenus())
    {
        sViewString = cCustomerDataEntry.ToString();
        trace(L6,_T("Data Needed View configuration parameters %s"),sViewString);
        trace (L6, _T("-SMStateTBBase::OnTBNeedData() with configuration parameters %s"),sViewString);
        CREATE_AND_DISPENSE(CmDataEntry)(sViewString);
    }
    
    //Build customer-language messages for RapDataNeeded screen.
    sRDNValues = _T("");
    if (sRDNLaneLeadthru != _T(""))
    {
        sRDNValues += _T("RDNLANELEADTHRU=") + sRDNLaneLeadthru + _T(";");
    }
    if (sRDNLaneMessage != _T(""))
    {
        sRDNValues += _T("RDNLANEMESSAGE=") + sRDNLaneMessage + _T(";");
    }
    
    // RFC 265805 end
    sViewString = sRDNValues + sViewString;
    trace(L6,_T("Data Needed View configuration parameters %s"),sViewString);
    trace (L6, _T("-SMStateTBBase::OnTBNeedData()"));
    
    // no StoreLogon parameter means we default to store mode
    
    CREATE_AND_DISPENSE(RAPDataNeeded)(sDescription, DATANEEDED_MOREDATA, sViewString, storeMode());
    
}

// caller passes lEntryID to set it to uniquely generated id in this function
long GenerateUniqueEntryID(long& lEntryID)
{
	CString csExistingID;
	ps.FindRctIndexFromRewardID(lEntryID, csExistingID);
	if (!csExistingID.IsEmpty())
	{
		// id exists, so create a new one
		// reset if it is the first app generated ID
		if (nGenerateEntryIDForTB == INVALID_ENTRYID)
		{
			nGenerateEntryIDForTB = 1000;
		}
		// create a dynamic, unique id	until it does not exist in e-receipt
		CString csFindUniqueID;
		ps.FindRctIndexFromRewardID(nGenerateEntryIDForTB, csFindUniqueID);
		while (!csFindUniqueID.IsEmpty())
		{
			ps.FindRctIndexFromRewardID(++nGenerateEntryIDForTB, csFindUniqueID);
		}
		// assign it to lEntryID
		lEntryID = nGenerateEntryIDForTB;
		// increment dynamic id
		++nGenerateEntryIDForTB;
	}
	else
	{
		// id does not exist,use it
	}

	return 0;
}
void SMStateBase::TBSyncTBorPOS()
{
  trace(L6, _T("+SMStateBase::TBSyncTBorPOS()") );
  TBRC rc = tb.SyncTBorPOS();
  ProcessTBReturn( rc );
  trace(L6, _T("-SMStateBase::TBSyncTBorPOS() %d"), rc );
}

SMStateBase *SMStateBase::HandleOutOfSyncStop()
{
  trace(L6, _T("+SMStateBase::HandleOutOfSyncStop(), TB posted TB_OUTOFSYNC_STOP.") );
  Out_of_sync_Trace_Object.Trace( L6, _T("SMStateBase::HandleOutOfSyncStop(), TB posted TB_OUTOFSYNC_STOP.") );
  eo.Error(RETURN,SM_CAT_LAN_WAN_COMM, SCOTAPP_TB_OUT_OF_SYNC_STOP);
  SMStateBase::m_nTBOutOfSyncError = 1;

  fStateAssist = false; // set these variables so that we can proceed to custommessage from Assist Mode state  fStateFreeze = false;
  fStateFreeze = false;
  //+ reporting to rap
  CString csSecInfo;
  CString csWork = ps.GetPSText( MSG_POS_TB_OUTOFSYNC_STOP );
  ra.OnNormalItem(_T("[highlight]") + csWork);

  // save the previous view
  m_PrevView = m_View;

  // send a DataNeeded Approval
  cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("OutOfSyncStop"));
  m_View = DATANEEDED_OUTOFSYNC_STOP;
  csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),csWork,ps.GetPSText(MSG_DATANEEDED_OUTOFSYNC_STOP, SCOT_LANGUAGE_PRIMARY), "", "", "", "");
  ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
  trace(L7, _T("-ra.OnRAPDataNeeded"));
  //- reporting to rap

  trace(L6, _T("-SMStateBase::HandleOutOfSyncStop()") );

  RETURNSTATE(CustomMessage);
}

SMStateBase *SMStateBase::HandleOutOfSyncAssist()
{
  trace(L6, _T("+SMStateBase::HandleOutOfSyncAssist(), TB posted TB_OUTOFSYNC_ASSISTMODE. Going to Assist") );
  Out_of_sync_Trace_Object.Trace( L6,   _T("SMStateBase::HandleOutOfSyncAssist(), TB posted TB_OUTOFSYNC_ASSISTMODE. Going to Assist") );
  eo.Error(RETURN,SM_CAT_LAN_WAN_COMM, SCOTAPP_TB_OUT_OF_SYNC_ASSISTMODE);

  SMStateBase::m_nTBOutOfSyncError = 2;
  SMStateBase::m_bTBNeedAssistModeGetDiag = true;

  //+ tell rap
  CString csSecInfo;
  CString csWork = ps.GetPSText( MSG_POS_TB_OUTOFSYNC_NEEDASSISTMODE );
  ra.OnNormalItem(_T("[highlight]") + csWork);
  // save the previous view
  m_PrevView = m_View;

  // send a DataNeeded Approval
  cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("OutOfSyncNeedAssist"));
  m_View = DATANEEDED_OUTOFSYNC_ASSIST;

  csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),csWork,ps.GetPSText(MSG_DATANEEDED_OUTOFSYNC_ASSIST, SCOT_LANGUAGE_PRIMARY), "", "", "", "");

  ra.RequestEnableAssistMode(true);

  ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);

  trace(L7, _T("-ra.OnRAPDataNeeded"));
  //- tell rap

  trace(L6, _T("-SMStateBase::HandleOutOfSyncAssist()") );

  return createLevel1Message(
    ps.GetPSText( MSG_POS_TB_OUTOFSYNC_NEEDASSISTMODE ),
    PS_NOTEXT,
    PS_NOTEXT,
    0,
    -1,
    false);
}

//+RFC 275377 MIGRATED BM185018

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    Function Name :  HandleVoidedTender
//    Parameter        :  void
//    Return Value   :  SMStateBase*
//    Description      :  The function is added for handling Voided Tender
//                               RFC 275377 - Support for Voiding a Tender
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SMStateBase * SMStateBase::HandleVoidedTender(void)
{
	SMStateBase *sUser = STATE_NULL;
	trace(L6, _T("+SMStateBase::HandleVoidedTender"));

	fStateAssist = false;
	if(fSAInCustMode && !fSAInStoreMode)
	{
		trace(L6, _T("SMStateBase::HandleVoidedTender on Customer Mode"));
		fTenderDeclined = true;
		m_bCancellingCashTender=true;
		fSAInCustMode=false;
		fSAInStoreMode=true;
		m_pAppPSX->ClearReceipt(_T("CMSmallReceipt"));
		CREATE_AND_DISPENSE(VoidApproval)(BES_ASSISTEDTENDER, BES_STOREAUTHORIZATION);
	}

	if (nTBLastRealState != TB_TENDERACCEPTED ) nTBLastRealState = nTBLastState;

	STenderDetails dTender;
	memset(&dTender, 0, sizeof(STenderDetails));

	TBRC rc = tb.GetTenderDetails(&dTender);
	ProcessTBReturn(rc);

	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO , _T("VoidedTender - TBBalanceDetails--TenderDetail: szDescription:%s; \
                   lTender:%d; lBalance:%d; lChange:%d; szErrDescription:%s; nTenderType:%d;"), dTender.szDescription, dTender.lTender, \
                   dTender.lBalance, dTender.lChange, dTender.szErrDescription, dTender.nTenderType) ;

	//+TAR289157 BM185018 - temporarily storing the voided amount.
    long lamountVoided = dTender.lTender;

	if(dTender.lTender < 0)     dTender.lTender = -dTender.lTender;
	if ( lTotalPaid <  dTender.lTender )
	{
		trace(L6, _T("Warning : Cash Back Amount is greater than actual amount tendered !   lLastTender = %d      dTender.lTender = %d "),lLastTender,dTender.lTender);
	}
	else dTender.lTender =  lTotalPaid;

	nTenderType = dTender.nTenderType;

	//+TAR289157 BM185018
	//This code fixes the incorrect amount being voided
	dTender.lBalance += lamountVoided;
	lLastTender = dTender.lTender;
	lTotalPaid -= lamountVoided;
	dTender.lTender =lTotalPaid;		//the remaining tender
	nDMCashTendered = 0;
	nDMCashInDrawer -= lamountVoided;
	lChangeDue = lamountVoided;
	lBalanceDue = dTender.lBalance;
	//-TAR289157 BM185018

	m_bCancellingCashTender = true;

	// Save data to log for System Reports
	if (co.fOperationsLogDataForSystemReports)
	{
		// Begin FastLane 3.1 Reporting
		trace(L7, _T("+rp.TenderStart"));
		rp.TenderStart(nTenderType);
		trace(L7, _T("-rp.TenderStart<%d>"), nTenderType);
		trace(L7, _T("+rp.TenderEnd"));
		rp.TenderEnd(-lLastTender);
		trace(L7, _T("-rp.TenderEnd<%d>"),-lLastTender );
	   // End FastLane 3.1 Reporting
	}


	if (dTender.szDescription)
		try{	delete [] dTender.szDescription;} catch(...){trace(L6,_T("delete exception:dTender.szDescription"));}
	if (dTender.szErrDescription)
		try{	delete [] dTender.szErrDescription;} catch(...){trace(L6,_T("delete exception:dTender.szErrDescription"));}

	trace(L6, _T("Voiding Tender ( Tender Type = %d )"),nTenderType);

	UpdateStateName(ps.GetPSText(LTT_CANCELCONFIRM, SCOT_LANGUAGE_PRIMARY));
//	setAnchorState(BES_CONFIRMVOID);

	if((co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bCashBackIfVoided) && !ps.RemoteMode())
	{
		fTenderDeclined = true;
		m_bCancellingCashTender=true;
		CREATE_AND_DISPENSE(TakeChange)(false);	// Coin Acceptor Modifications
	}
	else
	{
		m_bCancellingCashTender=false;	// CashBackIfVoided is set to N
	}

	ResetAllTenderValues();
	trace(L6, _T("-SMStateBase::HandleVoidedTender"));
	return STATE_NULL;
}
//-RFC275377 MIGRATED BM185018
//+dp185016 support glory device
BOOL SMStateBase::TBValidateHeadCashier(LPCTSTR szCashierID, LPCTSTR szCashierPW)
{
  trace(L6, _T("+SMStateBase::TBValidateHeadCashier()") );

	BOOL brc = tb.ValidateHeadCashier(szCashierID, szCashierPW);


  trace(L6, _T("-SMStateBase::TBValidateHeadCashier() %d"), brc );

	return brc;
}
//-dp185016

//+307389
SMStateBase *SMStateBase::getRealStatePointer(SMStateBase *pState)
{
   switch ((int)pState) {
      case NULL:
      case (int)STATE_NULL:
      case (int)STATE_STOP:
      case (int)STATE_RETURN:
      case (int)STATE_RESUME:
      case (int)STATE_RESTART:
      case (int)STATE_ENDED:
      case (int)STATE_ERROR:
         return NULL;
      default:
         return pState;
   }
   return NULL;
}
//-307389
