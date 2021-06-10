//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateTB.CPP
//
// TITLE: TB SMState functions, implement all SMState::TB* functions
//
// Change history:
//
// CRD 554916 Name: CJ Estrada Date: Feb 9, 2021
// CRD 517533 Work Request: Sprint 24.1 Name: Dennis Michol Gabutin Date: November 17, 2020
// POS320066 Work Request:77937 Name:Aparna Tunuguntla Date: April 06, 2017
// POS296865 Work Request:76936 Name:Veerasekhara,D Date:December 01, 2016
// POS296368 Work Request:76936 Name:Veerasekhara,D Date:August 3, 2015
// POS99147 Work Request:68940 Name:Kranthi Kumari Cherugondi Date:August 3, 2015
// POS207859 Work Request:68940 Name:Saiprasad Srihasam Date:July 21, 2015
// POS143695 Work Request:66559 Name: Kranthi Kumari Cherugondi   Date:February 10, 2015
// POS59963 Work Request:59313 Name:Robert Susanto Date:January 14, 2014
// POS84328/80904 Work Request:59313 Name:Robert Susanto Date:December 13, 2013
// POS59953 Work Request:54416 Name:Robert Susanto Date:April 4, 2013
// POS40287 Work Request:52635 Name:Robert Susanto Date:Nov 6, 2012
// POS30981/POS36096 Work Request:50864 Name:Robert Susanto Date:June 7, 2012
// POS30981/POS31050 Work Request:50864 Name:Robert Susanto Date:May 21, 2012
// POS309081/POS31050 Work Request:50864 Name:Robert Susanto Date:April 23, 2012
// POS18041 WORK REQUEST:16656 Name: Matt Condra Date: October 3, 2011
//////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdafx.h>                 // MS MFC always needed first
#include "common.h"
#include "SMState.h"
#include "SCOTMultiStringList.h"
#include "transbrokerssf.h"
#include "SigReceiptDataFile.h"

#include "SMSmAssistMode.h"
#include "SMSmAssistMenu.h"
#include "SMCustomMessage.h"
#include "SMScanAndBag.h"
#include "SMInProgress.h"        
#include "SMAssistedTender.h"
#include "SMVoidApproval.h"
#include "SMTakeChange.h"
#include "SMCashPayment.h"
#include "PSConfigData.h"
#include "SMSelectTare.h"
#include "SMInsertCoupon.h"
#include "SMEnterWtForPriceEmbedded.h"

#include "SMRecallingItems.h" // RFQ 9545 mobile shopper

#include "SMRestrictedNotAllowed.h"
#include "SMEnterQuantity.h"
#include "SMSmGenericDelayed.h"	//CRD 185620

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmStateTB")

#define MAX_AMKEYS	50	// change this 50 to the required number of keys on this assistmode screen

#define SCOTSSF_EVENT_LOG _T("SCOTSSF")        // EventLog Source
#define RA_OPERATOROVERRIDE 63 //CRD 330783

USHORT SMState::m_nAssistKeyVirtualKeyValue[200];
PSTEXT SMState::m_nAssistKeyTextID[200];
CString SMState::m_csKeyPOSString[200];
TBSTATE SMState::m_TBLastState = TB_IGNORE;
long SMState::m_lTBCouponCount = 0;
long SMState::m_lTBCouponAmount = 0;
bool SMState::m_bNeedAssistMode = false;
bool SMState::m_bTrxnEndedInHandHeld = false;
bool SMState::m_bAssistModeReturnAfterSystemMessage = false;
bool SMState::m_bReEnterDOB = false;
bool SMState::m_bTakeChangeTimerExpired = false;  //SSCOI-31860

bool SMState::m_bRecallModeWaitforItemSold = true; // RFQ 9545

static long rc;  // TB return value //SSCOI-47171

CNCREventLog   ssfEventLog ;   // eventlog object to log events in NT eventlog

#ifdef _TRADESHOWDEMO
#include "..\TradeShowDemo\SMStateTBTSDemo.cpp"
#endif

USHORT SMState::TBGetKeyCode(const int nKey)
{
	CString csKeyName;
	
  // TAR 139814 -- test scotssfmsg file
  HRESULT retVal = ssfEventLog.Open(SCOTSSF_EVENT_LOG);
	ASSERT(retVal == S_OK);
	if (retVal == S_FALSE)
	{
	  TRACE(_T("Error in CNCREventLog object creation"));
	  // TAR 243814 Move LogEvent function here.
	  /*ssfEventLog.LogEvent(evlClassBasic,
			   evlTypeInformation,
			   SM_CAT_CONFIGURATION,
			   SCOTSSF_SM_MSG_START);*/ // RFC 262774 - Scotssf message file will need to define catagories
	}
	ssfEventLog.Close () ;
  // 
   if (TBGetKeyLabel(nKey,csKeyName))	// if there is a label
	{
		return uKeyCodeList[nKey];
	}
  	
  return 0;
	//return SMStateBase::TBGetKeyCode(nKey);  // default is zero based
}


/////////////////////////////////////////////////////////////////////////////
long SMState::TBGetKeyLabel(const int nKey,CString &csLabel)
{
	// not used for this integration effort
	return 0;
	// return SMStateBase::TBGetKeyLabel(nKey, csLabel);  // default is zero based
}

CString SMState::TBGetKeyPOSString(const int nKey)
{
	if (nKey >=1 || nKey <200)
	{
		return m_csKeyPOSString[nKey];
	}
	else
	{
		trace(L1, _T("ERROR: TBGetPOSString returning null string on invalid key request %d"), nKey);
		return _T("");
	}
}

USHORT *SMState::TBEmulatorKeys(void)
{
	static USHORT nKeys[MAX_AMKEYS+1] = 
		{	0,
				1,   2,   3,  4,   5,
				6,   7,   8,  9,  10,
				11,  12, 13, 14,  15,
				16,  17, 18, 19,  20,
				21,  22, 23, 24,  25,
				26,  27, 28, 29,  30,
				31,  32, 33, 34,  35,
				36,  37, 38, 39,  40,
				41,  42, 43, 44,  45,
				46,  47, 48, 49,  50
		};
	return nKeys;
	// return SMStateBase::TBEmulatorKeys();  // default is zero based
}

#define ASSISTKEY_LOCAL       1
#define ASSISTKEY_REMOTE      2
#define ASSISTKEY_INSIDE_TXN  4
#define ASSISTKEY_OUTSIDE_TXN 8

void SMState::TBLoadAssistModeKeyboard(void)
{
	
	csKeyLabeList[0] = _T(""); //All references are 1-based
	m_csKeyPOSString[0] = _T("");
	
	BOOL bRemoteDeviceMode = ps.RemoteMode();

    BOOL bCheckAtLane = !bRemoteDeviceMode && dm.IsCheckPaymentAtLaneAvailable();
    SendTBCommand(TBCMD_SET_CHECK_AT_LANE_AVAILABLE, bCheckAtLane);

	CString csKeyName, csKeyCount, csMap, csCurrentControl;
	COleVariant v1, v2, vReturn;
	int nKeyCount, nRelativeIndex;

	m_pAppPSX->GetCustomDataVar(_T("AssistKeyCount"), csKeyCount, _T("AssistKeyMap"), _T(""));
	nKeyCount = _ttol(csKeyCount);
	nRelativeIndex = 0;
	for(int i = 1; i <= nKeyCount; i++)
	{
		csKeyName.Format(_T("Key%d"), i);

		//See if there is a "KeyXX" control name matching the current index
		if( PSX_SUCCESS == m_pAppPSX->GetConfigProperty( csKeyName, _T(""), UI::PROPERTYVISIBLE, v1 ))
		{
			nRelativeIndex = 0;
			csCurrentControl = csKeyName;
		}
		
		csMap=_T("");
		m_pAppPSX->GetCustomDataVar(csKeyName, csMap, _T("AssistKeyMap"), _T(""));
		int nPosition = csMap.Find(_T(","));
		m_csKeyPOSString[i] = csMap.Left(nPosition);  // Get Virtual Key value
		
		csMap = csMap.Mid(nPosition + 1);
		nPosition = csMap.Find(_T(","));
		m_nAssistKeyVirtualKeyValue[i] = _ttol(csMap.Left(nPosition));  // Get Virtual Key value

		csMap = csMap.Mid(nPosition + 1);
		nPosition = csMap.Find(_T(","));

		int nKeyMode = (PSTEXT) _ttol(csMap.Left(nPosition));   	
		int nCurrentMode = 0;
		
		nCurrentMode |= (!bRemoteDeviceMode ? ASSISTKEY_LOCAL : 0);
		nCurrentMode |= (bRemoteDeviceMode ? ASSISTKEY_REMOTE : 0);
		nCurrentMode |= ((fInSignedOn || fInSignedOff) ? ASSISTKEY_INSIDE_TXN : 0);
		nCurrentMode |= (!(fInSignedOn || fInSignedOff) ? ASSISTKEY_OUTSIDE_TXN : 0);
			
		v1 = (long) nRelativeIndex;	
		if( ((nCurrentMode & nKeyMode) != nCurrentMode) && 
            !(bCheckAtLane && (m_csKeyPOSString[i] == _T("{PAYMENT}"))) )  // Enable Payment button if Checks at Lane is enabled
		{
			v2 = (long) UI::Disabled; // disable
			m_pAppPSX->SendCommand(csCurrentControl, UI::COMMANDSETBUTTONSTATE, vReturn, 2, v1, v2 );	
		}
		else
		{
			v2 = (long) UI::Normal; // enable
			m_pAppPSX->SendCommand(csCurrentControl, UI::COMMANDSETBUTTONSTATE, vReturn, 2, v1, v2 );	
		}
		
		csMap = csMap.Mid(nPosition + 1);
		m_nAssistKeyTextID[i] = (PSTEXT) _ttol(csMap);   
		if( m_nAssistKeyTextID[i] != PS_NOTEXT)
		{
			ps.SetKeyLabel(csCurrentControl, nRelativeIndex, ps.GetPSText(m_nAssistKeyTextID[i], SCOT_LANGUAGE_PRIMARY));
		}
		nRelativeIndex ++;
	} // end of for loop
} // end of function, void SMState::TBLoadAssistModeKeyboard(void)


void SMState::DoEvents()
{
    SMStateBase::DoEvents();
}

long SMState::TBGetNextState(LPCTSTR lpLastStateName, 
                             LPCTSTR lpNextStateName, 
                             bool *pbOverride, 
                             LPTSTR *lpOverrideStateName, 
                             CDescriptorList &dlParameterList)
{
    return SMStateBase::TBGetNextState(lpLastStateName, lpNextStateName, pbOverride, lpOverrideStateName, dlParameterList);
}

bool SMState::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMStateBase::DeliverDescriptorList(dlParameterList);
}

/*TBRC SMState::SendTBCommand(  long lCommandID, ...)
{
    //va_list pArgs;
    //va_start(pArgs, lCommandID);
    //TBRC retValue = SMStateBase::SendTBCommand(lCommandID, pArgs);
    //va_end(pArgs);
    //return retValue;
    return SMStateBase::SendTBCommand(lCommandID, pArgs);
}*/

SMStateBase * SMState::TBStart(LPCTSTR szLanguageCode)
{
    return SMStateBase::TBStart(szLanguageCode);
}

SMStateBase * SMState::TBItemSale(const bool fVoid,
                                  const long lTare,
                                  const bool fScannedItem,
                                  const bool bIsPickListItem,
                                  const long lUpdateType)
{
    //SOTF 7718
    ra.RequestEnableAssistMode(false);
    
    if (fScannedItem)
    {
        //TAR345688 - To notify user that item scanned in SMFinish is being process 
        if(bIsInFinalization)
        {
            ps.LeadThruText(TXT_PLEASEWAIT);
        }

		//S.Marks Meijer  has code that manipulates the barcode after it has been scanned
		//I moved this code to a function because Security Maint. needs to modify the barcode 
		//the exact same way.
		MeijerModifyScannedBarcode();

        //POS57568 - New Raincheck barcode will use Code128 - rrs
		// Check to ensure that scan is NOT a raincheck!
		if ( ((csDMLastBarType == SCAN_SDT_Code39) || (csDMLastBarType == SCAN_SDT_Code128))  &&
			(csDMLastBarLabel.Mid(0, 1) == _T("R")))
		{
			// NOT ALLOWED IN CUSTOMER MODE!
			// Need customer message!
			DMScannerEnable();
			CString csWork = ps.GetPSText(SSF_TXT_RAINCHECK_NOT_ALLOWED);
			/*
			return createLevel1Message(csWork,     // char *message
			PS_NOTEXT,  // no canned message
			PS_NOTEXT,  // no prompt
			0,          // no prompt value
								 -1,         // no device
								 false);     // no DM check health needed
			*/
			csDMLastBarType = 0;
			csDMLastBarLabel = _T("");
			csDMLastBarCode = _T("");
        
		/*	CREATE_AND_DISPENSE(SystemMessage1)(csWork,      // char *message
				PS_NOTEXT,
				PS_NOTEXT,	// no prompt
				0,					// no value
				-1, // no device
				false);     // no DM check health after*/
            //POS57686 - use custommessage
            CustomMessageObj.Reset();

            SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
            CustomMessageObj.bGoBackScanAndBag = true;
            CustomMessageObj.csLeadThruText = _T("");
			CustomMessageObj.csScreenText = csWork;    
			RETURNSTATE(CustomMessage)
            // e POS57686

		}else
		{
                    
            
            CString strPropName = _T("CompleteScanCode");
            CString strPropValue = csItemSent;
            SendTBCommand(TBCMD_SET_TBPROPERTY, &strPropName, &strPropValue);
            
            /////////Price Embbeded Add-on - Start
            // The ff code is added to handle the price embedded code (like 21804101000) of Meijer ACS POS
            // which is not considered as embedded code by the function call io.IsEmbeddedPrice(io.csItemCode) because
            // its price check is not equal to its computed price check but it is really treated as price embedded item by the POS
            trace(L6, _T("csItemSent = (%s)"), csItemSent);
            
            CString cstempItemCode = csItemSent;
            CString cScanType = csDMLastBarType;
            // check if any alpha characters at beginning of UPC (put there by PLU server)
            int nItemCodeLen = cstempItemCode.GetLength();
            
            if ( nItemCodeLen && (!_istdigit(cstempItemCode[0])) && (cScanType == SCAN_SDT_Code128) && (cstempItemCode.Left(2) == _T("B3")))   //modified for Code128
            {
                cstempItemCode = cstempItemCode.Right(nItemCodeLen - 2); //Take the "B3" out
            }
            else if ( nItemCodeLen && (!_istdigit(cstempItemCode[0])) && (cScanType != SCAN_SDT_Code39))   //modified for Code39
            {
                // item code is with scan type prefix
                // remove alpha characters from UPC, assumes prefix is never more than one character
                cstempItemCode = cstempItemCode.Right(nItemCodeLen - 1);
            }
            
            else if (nItemCodeLen && (cScanType == SCAN_SDT_Code39) && (cstempItemCode.Left(2) == _T("B1")))
                cstempItemCode = cstempItemCode.Right(nItemCodeLen - 2);
            
            int length = _tcsclen( cstempItemCode );
            if( (length > 1) && !fVoid && 
                (( cstempItemCode.Left(2) == _T("02") && length <= 13) ||
                 ( cstempItemCode.Left(2) == _T("21") && length <= 13) ||
                 ( cstempItemCode.Left(2) == _T("20") && length == 13))   )
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
                    io.Reset(); 		// clear previous item info object
                    
                    SMStateBase::ms_bIsCurrentItemFromPickList = bIsPickListItem;
                    
                    // save current item info supplied by user activity
                    if(bIsPickListItem)
                        io.d.bPickListItem = true;
                    
                    io.csOrigCode = csItemSent;    // item code scanned
                    if (io.csOrigCode.IsEmpty())
                    {
                        // an item has been scanned but bar code is null!!
                        ASSERT(0);
                        io.csOrigCode = csDMLastBarCode;	   // item code scanned 
                    }
                    io.csItemCode = csDMLastBarLabel;	// remove leading type
                    io.cScanType = csDMLastBarType; 	// get scan type A,E,2...
                    
                    io.fVoid = fVoid;
                    io.lTareCode = lTare;			 
                    io.b_gIsThisItemPriceEmbedded = true;	
                    io.lUpdateFields = lUpdateType;		
                    SASendSCOTInput( UPC_SCANNED, csDMLastBarLabel );
                    
                    RETURNSTATE(EnterWtForPriceEmbedded);
                }
            }
            /////////Price Embbeded Add-on - End
            return SMStateBase::TBItemSale(fVoid, lTare, fScannedItem, bIsPickListItem, lUpdateType);
        }
    }
    else
    {
        // Call Super!
        return SMStateBase::TBItemSale(fVoid, lTare, fScannedItem, bIsPickListItem, lUpdateType);
	}
}

SMStateBase * SMState::TBEnterItem(const bool fOverride, int lUpdateFields)
{
    return SMStateBase::TBEnterItem(fOverride, lUpdateFields);
}

SMStateBase * SMState::TBEnterTender(const TBTENDERTYPE nType,
                                     const long lAmt,
                                     const bool fOverride)
{
    return SMStateBase::TBEnterTender(nType, lAmt, fOverride);
}

SMStateBase * SMState::TBCancelTransaction(const bool fAllowOverride)
{
    return SMStateBase::TBCancelTransaction(fAllowOverride);
}

SMStateBase * SMState::TBSuspendTransaction(const bool fOverride)
{
    return SMStateBase::TBSuspendTransaction(fOverride);
}

SMStateBase * SMState::TBIdleTimer(void)
{
    return SMStateBase::TBIdleTimer();
}

//UploadTicket Port +
SMStateBase * SMState::TBEndOfTransaction(void)
{
	trace(L6, _T("+SMStateTB::TBEndOfTransaction() - lTotalSale = %d, TransactionNumber= %d"), lTotalSale, TBGetCurrentTransactionNumber());
	trace(L6, _T("-SMStateTB::TBEndOfTransaction() - m_lSavedforMobileTotal  = %d, m_lSavedforMobileTransNumber = %d"), m_lSavedforMobileTotal , m_lSavedforMobileTransNumber);

	// provide total and transaction number and then reset
    if (g_bMobileAppEnabled)
        mobileCart.SendFinishTransaction(m_lSavedforMobileTotal, m_lSavedforMobileTransNumber);
	
	m_lSavedforMobileTotal = 0;
	m_lSavedforMobileTransNumber  = 0;

	HFILE hSlip;
	CSigReceiptDataFile m_SigDataFile;
	
	hSlip = TBGetSlipData();
	while( NULL != hSlip )
	{
		m_SigDataFile.OpenFile(TRUE) ;
		BSTR sigData;
		SendTBCommand( TBCMD_GET_BITMAP, &sigData );
		
		trace(L7, _T("+rp.Signatures"));
       	rp.Signatures(hSlip, sigData ); 
       	trace(L7, _T("-rp.Signatures<%d><NULL>"), hSlip );
		
		m_SigDataFile.CloseFile() ;
		
		if ( sigData != NULL )
			SysFreeString( sigData );
		TBReleaseSlipData( hSlip );
		hSlip = TBGetSlipData();
	}
	// +Tar 424867 - Set transaction complete when we are in attendantmode.
	if( co.IsAttendantModeOn() )
	{
		bTransactionComplete = true;
	}
	// -Tar 424867
	CString strPropName = _T("IsTransactionVoided_Prop");
    CString strPropValue;
    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strPropName, &strPropValue);
	if( strPropValue == _T("1")) //TAR320569 -RAP display tender complete during cancel trx
	{
		lBalanceDue = 1.0;
		SMStateBase *pRetState = SMStateBase::TBEndOfTransaction();
		lBalanceDue = 0.0;
		return pRetState;
	}
	
    return SMStateBase::TBEndOfTransaction();
}

SMStateBase * SMState::TBFinish(void)
{
    bool bSaveMobileAppEnabled = g_bMobileAppEnabled;
    g_bMobileAppEnabled = false;

    if (g_bMobileAppEnabled)
        mobileCart.SendFinishTransaction(m_lSavedforMobileTotal, m_lSavedforMobileTransNumber);

    SMStateBase* retState;retState = SMStateBase::TBFinish();
    g_bMobileAppEnabled = bSaveMobileAppEnabled;
    return retState;
}
//UploadTicket Port -

TBRC SMState::TBLogErrorToServer(LPCTSTR szErrorText, const int nSeverity,
                           const int nComponentID, LPCTSTR szComponentID,
                           LPCTSTR szModuleID, const int nModuleLineNumber)
{
    return SMStateBase::TBLogErrorToServer(szErrorText, nSeverity,nComponentID, szComponentID, szModuleID, nModuleLineNumber);
}

void SMState::TBEnterTenderState(const bool bEnter)
{
    SMStateBase::TBEnterTenderState(bEnter);
}

void SMState::TBSendSigData(LPCTSTR CustomerName, LPCTSTR cSigData, DWORD nLength)
{
    SMStateBase::TBSendSigData(CustomerName, cSigData, nLength);
}

SMStateBase * SMState::TBGetMessageInfo(const bool bDisableSA )
{
    SMessageInfo sTempMessageInfo;
	memset(&sTempMessageInfo, 0, sizeof(SMessageInfo));  

	// Start CRD 125377
	if ((m_bShopAndDropEnabled) && (m_bSNDAgeEnteredFound)) // display Age Message
	{
		m_bSNDAgeEnteredFound = false;

		sTempMessageInfo.szScreenText = _T("");
		sTempMessageInfo.szPlayWave = _T("");
		sTempMessageInfo.szLeadThruText = _T("");

		LPCTSTR csTest = ps.GetPSText(FTU_CANCELITEMS); // msg 7 _T("Age Verified");
		sTempMessageInfo.szE_ReceiptText= (LPTSTR) csTest; 

		sTempMessageInfo.bHiliteReceiptText= false;
		sTempMessageInfo.bApprovalNeeded = false;
		sTempMessageInfo.bNeedItemCancel = false;
		sTempMessageInfo.bGoBackScanAndBag = false;
		sTempMessageInfo.szRAP_MessageText = _T("");
		sTempMessageInfo.bHiliteRAPText = false;
	}
	else
	{
	//long rc = tb.GetMessageInfo(&sTempMessageInfo);
    long rc = ::TBGetMessageInfo(&sTempMessageInfo);
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
	}
	// End CRD 125377

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
	// start CRD 125377
	if (m_bShopAndDropEnabled)
	{
		// FIX LATER MGG 	
		if (-1 != CustomMessageObj.csE_ReceiptText.Find(_T("Team Member")))
		{
			CustomMessageObj.csE_ReceiptText = ps.GetPSText(FTU_BUYHEAVY); // msg 7 _T("Age Verified");
		}
	}
	// end CRD 125377
	if(nPreviousState == BES_BAGANDEAS && !SMState::m_bSecurityDisabledForMPerks)
	{
		CustomMessageObj.bNeedItemCancel = false;
		CustomMessageObj.bGoBackScanAndBag = false;
	}
	else
	{
		CustomMessageObj.bNeedItemCancel = sTempMessageInfo.bNeedItemCancel;
		CustomMessageObj.bGoBackScanAndBag = sTempMessageInfo.bGoBackScanAndBag;		
	}
	
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
	if(CustomMessageObj.bNeedItemCancel || bDisableSA)
	{
		// make sure we wait to process the next item.
		if(!g_bInRecallMode)
		{
			SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
		}
		else
		{
			trace(L6, _T("TMD and Recall Mode FLM wait - DON'T CLEAR SA"));
			fSAInItemSale = false;
		}


		//+SSCOI-47185
		//CString csErrText = CustomMessageObj.csScreenText;
				//if (csErrText.Find(_T(ps.GetPSText(9016))) != -1)
		//if(csErrText == ps.GetPSText(SSF_TXT_COLLECTNEGATIVEMEDIA))
		//{
			//trace(L6, _T("Error Text Message : %s"), csErrText);
			//trace(L6, _T("Do not enable scanner."));
		//}
		//else
			//SAWLDBScannerEnable();
		//-SSCOI-47185
		
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
                csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_LANEBTT_RECALLEDITEM, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, _T(""), ps.GetPSText(RA_THIRDLINE_RECALLEDITEM, SCOT_LANGUAGE_PRIMARY));
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

	  // start CRD 125377
	  if (m_bShopAndDropEnabled)
	  {
		  // FIX LATER MGG 	
		  if (-1 != CustomMessageObj.csE_ReceiptText.Find(_T("Team Member")))
		  {
			  m_bSNDTMDEntered = true;
		  }
		  
		  if (-1 != CustomMessageObj.csE_ReceiptText.Find(_T("mPerks")))
		  {
			  m_bSNDmPerkEntered = true;
		  }

		  CString strFmt =_T("m_bSNDmPerkEntered=%d; m_bSNDTMDEntered=%d;");
		  traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt, m_bSNDmPerkEntered, m_bSNDTMDEntered);

		  if ((m_bSNDmPerkEntered) && (m_bSNDTMDEntered))
		  {
			  m_pAppPSX->SetTransactionVariable(_T("ShopAndDropEnabledYesNoButtons"), VARIANT_TRUE); 
		  }

	  }
	  // end CRD 125377
	  // start RFQ 9545
	  // move below
	  /*
	  if (-1 != CustomMessageObj.csE_ReceiptText.Find(_T("Team Member")))
	  {
		  // make sure we wait to process the next item.
		  if(g_bInRecallMode)
		  {
			trace(L6, _T("TMD and Recall Mode FLM wait - STAY WHERE WE ARE"));
			return STATE_NULL;
		  }
	  }
	  */
	  //  end RFQ 9545

      // reset props
      SetHookProperty(_T("PropertyAllowMSGEntryID"));
      SetHookProperty(_T("TMDMemberEntryID"));
      SetHookProperty(_T("NSC4MemberEntryID"));
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
	
	// start RFQ 9545
	if(CustomMessageObj.csE_ReceiptText.GetLength())
	{
		if (-1 != CustomMessageObj.csE_ReceiptText.Find(_T("Team Member")))
		{
			// make sure we wait to process the next item.
			if(g_bInRecallMode)
			{
				trace(L6, _T("TMD and Recall Mode FLM wait - STAY WHERE WE ARE"));
				m_bTMDAccepted = true; //168992
				return STATE_NULL;
			}
		}
	}
	//  end RFQ 9545


	//Disable the security for this transaction if mPerks is activated
	if(CustomMessageObj.csRAP_MessageText.Find(_T("mPerks")) == 0 || CustomMessageObj.csE_ReceiptText.Find(_T("mPerks")) == 0)
	{
		CString strOptionName = _T("DisableSecurityOnMPerksActivation");
		CString strOptionValue;
		SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &strOptionValue);
		trace(L6, _T("DisableSecurityOnMPerksActivation = %s"), strOptionValue);

		SMState::mPerksActivatedInTransaction = true; //mPerks activated for this transaction. Will use this flag to maintain mPerks button state.
		m_pAppPSX->SetConfigProperty (_T("CMButton7"), _T("ScanAndBag"), UI::PROPERTYSTATE, (long)UI::Disabled);
        m_pAppPSX->SetConfigProperty (_T("mPerksButtonVideo"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
		trace(L6,_T("+SMStateTB::TBGetMessageInfo, mPerks detected in custom message. Message: %s"), CustomMessageObj.csE_ReceiptText);

		if(strOptionValue == _T("1"))
		{
			trace(L6, _T("mPerks activated. Disabling the security as DisableSecurityOnMPerksActivation is set to 1"));
			SMState::m_bSecurityDisabledForMPerks = true;
			
			//CRD 313028 do not trigger disable again if everseen is enabled
			CString csEverseenEnabled = GetHookProperty(_T("EverseenEnabled"));	//CRD 313028
			if(csEverseenEnabled == _T("0"))
			{
				ra.OnDisableSecurity();
				
				if(g_bIsSecurityDisableSubCheckOnly)
				{	
					co.SetSecurityDisableSubCheck(true);
					SetSecurityConfig(CONFIG_KEY_IS_SECURITYDISABLE_SUB_CHECK_ON, _T("true"));	
				}
				else
				{	
					co.SetSecurityDisable(true);
									
					CKeyValueParmList parmList;
					CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_ENTER_DISABLE_SECURITY_MODE);
					_bstr_t bstrParmList((const _TCHAR *)parmList);
					BSTR bstrResult = NULL;
					bstrResult = NULL;
					SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
					SysFreeString(bstrResult);	
				}
				
				if(csOperator != _T("")) 
				{ rp.SetOperator(csOperator); }
					rp.ModeChange(rp.m_lAttendantEntryID, SECURITYDISABLED, ENTER);
			}
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
	   //+TAR345688 - prevent FL from going to scan&bag 
	   //if Loyalty is scanned @ select payment screen
       // TAR360799 - corrected the Find() params to check for -1 (which implies 'not found'
	   if( (-1 != CustomMessageObj.csE_ReceiptText.Find(_T("GUEST"))) &&
		   (-1 != CustomMessageObj.csE_ReceiptText.Find(_T("CARD"))) )
	   {
		 CustomMessageObj.Reset();
		 return STATE_NULL;
	   }//-
	   bool bReturnToScanAndBag = CustomMessageObj.bGoBackScanAndBag; //tar 265812
	   CustomMessageObj.Reset();

       //POS84328/80904 - mPerks resume
       CString strOptionName = _T("IsResumeTrxInProgress");
	   CString strIsResume = _T("0");
	   SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strOptionName, &strIsResume);

       if( strIsResume == _T("1") && !g_bInRecallMode )	//CRD 237852 make sure it doesn't go to InProgress if it's in FLMS
       {
           RETURNSTATE(InProgress);    //resume in progress
       }
       // e POS84328/80904 mPerks resume

       //POS31050 - prevent the timing issue where the Mobile Shop Order Pending moved to S&B rrs
       CString csPOSState(_T(""));
       SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);       

       if( csPOSState == _T("MOB_SHOP_ORDER_PENDING_MSG") ) 
       {
           bReturnToScanAndBag = false;
           trace(L6, _T("DONT GO BACK TO S&B"));
       }
       // e POS31050

	   if(bReturnToScanAndBag)   //tar 265812
	   {
		   if (co.IsInAssistMenus()) //TAR377676
		   {
				RETURNSTATE(SmAssistMenu)
		   }
           else
		   {
	            RETURNSTATE(ScanAndBag);	 
		   } 
	   }
	   // If getting here from TB_E_ITEMFAILED, then go back to previous state.
	   // Otherwise, stay where you are since this is probably from 
	   // TB_MESSAGESCREEN state
	   if (bDisableSA)
		 return STATE_RETURN; 
	   else 
		 return STATE_NULL;
	}
}

void SMState::TBPrcessPIPTenderType(TBTENDERTYPE& nTenderType)
{
    SMStateBase::TBPrcessPIPTenderType( nTenderType);
}

void SMState::TenderComplete(void)
{
    SMStateBase::TenderComplete();
}

SMStateBase * SMState::HandleVoidedTender(void)
{
	// +SSCOI-44745
	trace(L6, _T("+SMState::HandleVoidedTender")); 
	SMStateBase *sUser = STATE_NULL; 

	
	//fStateAssist = false;                
	if(fSAInCustMode && !fSAInStoreMode)
	{
		trace(L6, _T("SMState::HandleVoidedTender on Customer Mode")); 
		fTenderDeclined = true; 	
		m_bCancellingCashTender=true;	
		fSAInCustMode=false; 
		fSAInStoreMode=true; 
		m_pAppPSX->ClearReceipt(_T("CMSmallReceipt")); 
		if(!m_bIsSuspendFromRap)
		{
			fStateAssist = false; 
			CREATE_AND_DISPENSE(VoidApproval)(BES_ASSISTEDTENDER, BES_STOREAUTHORIZATION); 
		}
	}
	
	if (nTBLastRealState != TB_TENDERACCEPTED ) nTBLastRealState = nTBLastState;  
	
	STenderDetails dTender;
	memset(&dTender, 0, sizeof(STenderDetails)); 
    
    TBRC rc = ::TBGetTenderDetails(&dTender);
	//TBRC rc = tb.GetTenderDetails(&dTender); 
	ProcessTBReturn(rc);

	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO , _T("VoidedTender - TBBalanceDetails--TenderDetail: szDescription:%s; \
                   lTender:%d; lBalance:%d; lChange:%d; szErrDescription:%s; nTenderType:%d;"), dTender.szDescription, dTender.lTender, \
                   dTender.lBalance, dTender.lChange, dTender.szErrDescription, dTender.nTenderType) ;

	if(dTender.lTender < 0)
	{
		dTender.lTender = -dTender.lTender; 
	}
	
    long lamountVoided = dTender.lTender;
		
	nTenderType = dTender.nTenderType;

	//dTender.lBalance += lamountVoided; Balance is already updated in base. fix for SSCOI-46526
	lLastTender = dTender.lTender;
	lTotalPaid -= lamountVoided;
    if(lTotalPaid < 0)
    {
        lTotalPaid = 0; 
    }
	dTender.lTender =lTotalPaid;
	nDMCashTendered = 0;
	lChangeDue = 0;
	lBalanceDue = dTender.lBalance; 
    m_bCancellingCashTender = true; 
	
	if (co.fOperationsLogDataForSystemReports)
	{
		trace(L7, _T("+rp.TenderStart"));
		rp.TenderStart(nTenderType); 
		trace(L7, _T("-rp.TenderStart<%d>"), nTenderType); 
		trace(L7, _T("+rp.TenderEnd"));
		rp.TenderEnd(-lLastTender); 
		trace(L7, _T("-rp.TenderEnd<%d>"),-lLastTender );
	}
	
	
	if (dTender.szDescription) 
		try{	delete [] dTender.szDescription;} catch(...){trace(L6,_T("delete exception:dTender.szDescription"));}	
	if (dTender.szErrDescription) 
		try{	delete [] dTender.szErrDescription;} catch(...){trace(L6,_T("delete exception:dTender.szErrDescription"));}   
    
	trace(L6, _T("Voiding Tender ( Tender Type = %d )"),nTenderType); 
	
	UpdateStateName(ps.GetPSText(LTT_CANCELCONFIRM, SCOT_LANGUAGE_PRIMARY)); 

    short int nTenderIndex = -1;
    // find associated button section in SCOTTend.dat
    // tender button index starts from 1 (1- cash ...etc.)
    for (short i = 1; (i < MAXBUTTONNUM); ++i)
    {
        TBTENDERTYPE nTmp = (TBTENDERTYPE) GetTenderButton(i).nTenderType;
        trace(L6, _T("Iteration %d got tender type:%d"), i, nTmp);
        if (nTmp == dTender.nTenderType)
        {
            nTenderIndex = i;
            trace(L0, _T("Found a match for voided tender"));
            break;
        }
        else if( (dTender.nTenderType == TB_TENDER_CHECK) && 
            (nTmp == TB_TENDER_CHECK_LANE || nTmp == TB_TENDER_CHECK_LANE_RAP) )
        {
            nTenderIndex = i;
            trace(L0, _T("Found a match for voided tender"));
            break;
        }
    }

    if( nTenderIndex > 0 )
    {
        if((co.TenderList[ps.m_languageUsed][nTenderIndex-1].bCashBackIfVoided))
        {        
            if(nTenderType == TB_TENDER_CASH)
            {
                nDMCashInDrawer -= lamountVoided;
            }
            if(nDMCashInDrawer < 0)
            {
                nDMCashInDrawer = 0;
            }
            lChangeDue = lamountVoided;
        }

        if(lChangeDue > 0)
        { 
            fStateAssist = false;                
            fTenderDeclined = true;    
            m_bCancellingCashTender=true; 
        
            CREATE_AND_DISPENSE(TakeChange)(false);
        } 
        else  
        { 
            m_bCancellingCashTender=false;
        }
    }
    else
    {
        m_bCancellingCashTender=false;
        trace( L0, _T("ERROR!!!!  Could not map TenderType %d.  NO CASH RETURNED!"), dTender.nTenderType );
    }
    

	
	ResetAllTenderValues();
    nTenderType = TB_TENDER_NONE;                   // ResetAllTenderValues doesn't clear this
	trace(L6, _T("-SMState::HandleVoidedTender")); 
	return STATE_NULL; 
	// -SSCOI-44745
    //return SMStateBase::HandleVoidedTender();
}

BOOL SMState::TBValidateHeadCashier(LPCTSTR szCashierID, LPCTSTR szCashierPW)
{
    return SMStateBase::TBValidateHeadCashier(szCashierID, szCashierPW);
}

void SMState::ProcessPendingActions()
{
    SMStateBase::ProcessPendingActions();
}

bool SMState::IsTenderStarted()
{
    return SMStateBase::IsTenderStarted();
}

void SMState::SetTenderStarted(bool bTenderStarted)
{
    SMStateBase::SetTenderStarted(bTenderStarted);
}

SMStateBase * SMState::TBTenderSelected(TBTENDERTYPE nTenderType)
{
    return SMStateBase::TBTenderSelected(nTenderType);
}

void SMState::ProcessTBReturn(long rc)
{
    SMStateBase::ProcessTBReturn(rc);
}

#ifndef _TRADESHOWDEMO
SMStateBase * SMState::TBParse(MessageElement* me)
{
    SMStateBase *pState = SMStateBase::TBParse( me);
	trace(L3, _T("*_* SMStateTB::TBParse; after calling base TBPArse: %d"), nTBLastState);
    if (!fStateAssist)
    { 
        if( m_TBLastState != TB_UNKNOWN && m_TBLastState != TB_IGNORE && m_TBLastState != TB_TENDERVOIDED)
        {
            CString csTBStateName;
		    extern void GetTBReturnStateName(TBRC nTBState, CString & csStateName); // defined in CTransBroker.cpp
		    GetTBReturnStateName(m_TBLastState, csTBStateName);

            if( csTBStateName == _T("NewTBState") )
            {
                // get the new state from TB if possible
                SendTBCommand(TBCMD_GET_TBSTATENAME, m_TBLastState, &csTBStateName);
            }
            trace(L3, _T("!!!! SMStateTB::TBParse - m_TBLastState id=%d, name:%s"), m_TBLastState, csTBStateName);
        }
        
        switch(m_TBLastState)
        {

		case TB_NEEDWEIGHT:
			
			//don't allow weight required items on SS90
			if(!_tcsicmp(co.csHWType, _T("SS90")))
			{
				SendTBCommand(TBCMD_WEIGHTITEMS_NOT_ALLOWED);
				return STATE_NULL;
			}
			break;
			//+CRD185924
		case TB_POS_RESTARTING:
			{
				/* CString csState = GetRuntimeClass()->m_lpszClassName;
                if( csState.Find(_T("SMLaneClosed")) != -1 || csState.Find(_T("SMAttract")) != -1 || 
					csState.Find(_T("SmAssistMode")) != -1 || csState.Find(_T("ScanAndBag")) != -1)
				{	*/	//commented it as we need to handle it every time it happened. 	
				    trace(L6, _T("POS is restaring...wait at SMOutOfService2 state."));
					fStateFreeze = false; 
				    delete getRealStatePointer( pState);
					SendTBCommand(TBCMD_SET_TB_STATE,_T("TERM_INIT_DISCONNECTED"));                   
				    return setAndCreateAnchorState(BES_OUTOFSERVICE2);
					break;
				//}
			} //-CRD185924
		case TB_ENABLE_ASSISTMODE:
			{
				 trace(L3, _T("TB_ENABLE_ASSISTMODE: TB instructed to enable assistmode"));
				 ra.RequestEnableAssistMode(true);
				 delete getRealStatePointer( pState);
				 return STATE_NULL;
				 break;
			}
        case TB_SENDMESSAGETOSCOT:
            {
                ra.OnNormalItem(RAP_RECEIPT_LINE_HIGHLIGHT + ps.GetPSText(RA_POSMESSAGE, SCOT_LANGUAGE_PRIMARY) + _T(" ") + csTBMessage2Scot);
                break;
            }
            
        case TB_NEEDASSISTMODE:
            {   
                fStateFreeze = false;
                if (cDMLastTrack)
                { // if there is, delete it
                    delete cDMLastTrack;
                    cDMLastTrack = NULL;
                    nDMLastTrackLen = 0;
                    csDMEncryptedPin = EMPTY_STRING;
                }                    
                
                delete getRealStatePointer( pState);  // safe deletion in case of memory leak
                
                CString csState = GetRuntimeClass()->m_lpszClassName;
                if( csState.Find(_T("SMBagAndEAS")) != -1 )
                {
                    trace(L6, _T("AM requested while we're in scan and bag. Clear the 'waiting for weight' condition before proceeding."));
                    g_lTABMisMatchApprWt = g_lCTW;
                    g_bMatchedWtWaitingOnEAS = false;  // no longer used??
                    g_bWtExpected = false;
                    bEASReEnableScanner = true;
                    g_lNumberOfWLDBEntriesThisItem = -1;
                    CString csAuth = ps.RemoteMode() ? KEY_EXCEPTION_AUTHORIZATION_RAP : KEY_EXCEPTION_AUTHORIZATION_LANE;
                    OverrideSecurityException(csAuth, KEY_EXCEPTION_OPERATION_CLEAR);
                    
                    CKeyValueParmList parmList;
                    CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_SKIP_BAGGING);
                    _bstr_t bstrParmList((const _TCHAR *)parmList);
                    BSTR bstrResult;
                    bstrResult = NULL;
                    SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
                    SysFreeString(bstrResult);
                }
                else
                {
		              SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
                }

                // POS14512 - rrs
                // Only change the state if the m_bDataNeededAssistMode is false and the current state is not in SMRapDataNeeded
                if( !m_bDataNeededAssistMode && csState.Find(_T("SMRapDataNeeded")) == -1 )
                {
                    // SOTF 6853
                    m_bDataNeededAssistMode = true;

					//check if this is for STORE_CLOSE_REQUEST from POS					
					if (GetTBProperty(_T("SavedLastPosState")) == _T("STORE_CLOSE_REQUESTED"))
					{
						//POS initiated store close request for EOD, and not for new software installation
						if (Is5XHardware() || !_tcsicmp(co.csHWType, _T("SCOT6")))
						{
							trace(L6, _T("Cash Monitoring Counts for change over - End of Day"));
							CashMonitoringCounts(3); // 3 - Change over - End of Day
						}
					}
					else
					{
						trace(L8, _T("POS is not at store close. Will not report Cash Monitoring Counts for change over - End of Day."));
					}

					//current pos state is store close, not new software

					// POS31051 - handle the Mobile Audit screen
                    //POS31050 - Display the Mobile Assistance Text
                    CString csPosState = GetHookProperty(_T("__POSState"));
                    if( (csPosState.Find(_T("MOBILE_TRANSACTION_ASSISTANCE")) != -1) )
                    {
                        CREATE_AND_DISPENSE(RAPDataNeeded)(ps.GetPSText(SSF_TXT_ENHANCEDAM_MOBILE_ASSISTANCE, SCOT_LANGUAGE_PRIMARY), DATANEEDED_ASSISTMODE, _T(""), storeMode());
                    }
                    else if( (csPosState.Find(_T("MOBILE_AUDIT")) != -1) )  // POS31050 - Display Mobile Transaction Verification needed for audit only intervention
                    {
                        CREATE_AND_DISPENSE(RAPDataNeeded)(ps.GetPSText(9309, SCOT_LANGUAGE_PRIMARY), DATANEEDED_ASSISTMODE, _T(""), storeMode());
                    }
                    else
                    {
						trace(L6, _T("TB_NEEDASSISTMODE - going to RAPDataNeeded"));
                        CREATE_AND_DISPENSE(RAPDataNeeded)(ps.GetPSText(SSF_NEEDASSIST_DATAENTRY, SCOT_LANGUAGE_PRIMARY), DATANEEDED_ASSISTMODE, _T(""), storeMode());
                    }
                    // e POS31050
                }
                else
                {
                    return STATE_NULL;
                }
                
                break;
            }
            
        case TB_NEED_SCANANDBAG:	
            { 
                delete getRealStatePointer( pState);
				BEST currentAnchorState = getAnchorState();
				
				//CRD 287055 - we need to allow to return to scan and bag if IsResumeComplete is true so it can exit the retrieving receipt loop
				CString csIsResumeCompleteName = _T("IsResumeComplete");
				CString csIsResumeCompleteValue;
				SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csIsResumeCompleteName, &csIsResumeCompleteValue);
				trace(L6, _T("TB_NEED_SCANANDBAG - IsResumeComplete = %s"), csIsResumeCompleteValue);
	
                if((currentAnchorState == BES_FINISH ||  currentAnchorState == BES_BAGANDEAS || currentAnchorState == BES_SCANANDBAG)	//TAR345688
					&&  csIsResumeCompleteValue == _T("0"))
				{	
					//if so, FL must not return to scan&Bag
					trace(L6, _T("Current anchorstate is %d "), currentAnchorState);
                    trace(L4, _T("Ignoring TB request for TB_NEED_SCANANDBAG because anchorstate is BES_FINISH/BES_BAGANDEAS Or SCO is already at BES_SCANANDBAG"));
                    return STATE_NULL;	
                }	
                
                if (fSAInItemSale)
                {
                    RETURNSTATE(InProgress);
                }
                RETURNSTATE(ScanAndBag);
                
                break;
            }
        case TB_NEEDTARE:
            {
                delete getRealStatePointer( pState);
                RETURNSTATE(SelectTare);
                break;
            }
            
            
            //fix for TAR# 306792
            //added to display "(item name) is a sale restricted item" on e-receipt of RAP
        case TB_NEEDMOREDATA:
            {
                CString csPropertySaleRestricted = _T("PropSaleRestricted");
                CString csSaleRestricted;
                SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csPropertySaleRestricted, &csSaleRestricted);
                
                
                CString csDescSaleRestricted;
                CString csProperty = _T("Item_descriptor_Prop");
                CString csValue;
                SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csProperty, &csValue);
                
                if(csSaleRestricted == _T("1"))		
                {
                    csSaleRestricted = _T("0");
                    SendTBCommand(TBCMD_SET_HOOKPROPERTY, &csPropertySaleRestricted, &csSaleRestricted);
                    csValue += _T(" is a sale restricted item");
                    csDescSaleRestricted.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csValue, ps.GetPSText(TXT_RESTRICTEDNOTALLOWEDITEMS, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""), ps.GetPSText(LTT_RESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY));					
                    ra.OnNormalItem(csDescSaleRestricted);
                }

                //POS65108/6300 
                CString csPosState = GetHookProperty(_T("__POSState"));
                if( (csPosState.Find(_T("CELL_PHONE_2_STEP")) != -1) )
                {
                    ra.RequestEnableAssistMode(true);
                    trace(L6, _T("Enable the RAP AssistMode for Cellphone"));
                }
                // e - pos6300

                //POS192618
                if( csPosState.Find(_T("DO_YOU_WANT_MPERKS")) != -1 || 
					csPosState.Find(_T("MESSAGE_300_2_INVALID_MPERKS_PIN")) != -1 ||
					csPosState.Find(_T("DIGITAL_COUPON_PIN_ENTRY")) != -1 )
                {
                    ra.OnCanSuspendNow(false);
                    trace(L6, _T("Disable the RAP Suspend Button"));
                    //(+) POS192734
                    CString csState = GetRuntimeClass()->m_lpszClassName;
                    if(csState.Find(_T("SMInsertCoupon")) != -1
						|| (csState.Find(_T("SMCmDataEntry")) != -1  && nPreviousState == BES_INSERTCOUPON) )
                    {
						trace(L6, _T("Setting m_bCouponInsertionBypassed for the pos state = %s  and SCO state = %s"), csPosState, csState);
                        m_bCouponInsertionBypassed = true;
                    }
                    //(-) POS192734
                }
                // e - POS192618

                break;
            }
            //end of fix	
            
        case TB_NEEDIMMEDATEAGECHECK:
            {
                
                //Set up restricted and do immediate verification
                CString csAgeFromPOS(_T("0"));
                SendTBCommand(TBCMD_GET_AGEREQUIRED, &csAgeFromPOS);
                io.d.nRestrictedAge = _ttol(csAgeFromPOS);
                
                lSAYellowNeedingApproval++; //won't stay in Approval without this
                lUnapprovedRestrictedItems++;
                dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                    DM_TRICOLORLIGHT_BLINK_1HZ,
                    DM_TRICOLORLIGHT_NORMALREQUEST); 
                
                // remote approval
                int nYearRequired;
                COleDateTime dtToday;
                CString csValidBirthday;
                dtToday = COleDateTime::GetCurrentTime();
                nYearRequired = dtToday.GetYear() - io.d.nRestrictedAge;
                csValidBirthday.Format(ps.GetPSText(RA_VALIDBIRTHDAYREQUIRED, SCOT_LANGUAGE_PRIMARY), ps.DateFormat(dtToday.GetMonth(), dtToday.GetDay(), nYearRequired));
                
                CString csAnchorStateName = _T("");
                CString csDesc, csInstruction;
                csDesc = ps.GetPSText(RA_AGE_RESTRICTED_ID, SCOT_LANGUAGE_PRIMARY);
                _TCHAR TempBuffer[20];
                _itot(io.d.nRestrictedAge, TempBuffer, 10); // turn the index into a string so it can be passed via Add below
                CString csAge(TempBuffer);
                csDesc += _T("\n") + csAge + _T(" ")
                    + ps.GetPSText(RA_AGERESTRICT_MINIMUM_AGE, SCOT_LANGUAGE_PRIMARY)
                    + _T("\n") + csValidBirthday;
                csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_LANEBTT_RESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, _T(""), ps.GetPSText(RA_THIRDLINE_RESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY));
                trace(L7, _T("+ra.OnAgeRestrictedItem, Message=%s"), csInstruction);
                ra.OnAgeRestrictedItem(csInstruction, io.d.nRestrictedAge, FALSE, co.fStateDOBNotNeededButton);
                trace(L7, _T("-ra.OnAgeRestrictedItem"));
                m_lAgerestrictQueuedtoRAP++;
                
                delete getRealStatePointer( pState);
                
                // can't go to WaitForApproval directly since
                // the state InProgress will cause a delay in processing
                setAnchorState(BES_SCANANDBAG);
                m_bNeedImmediateApproval = true;
                return setAndCreateAnchorState(BES_SCANANDBAG);
            }
        case TB_RESUME_STARTED:
            {
                delete getRealStatePointer( pState);

                //POS11024 Set fSAInItemSale true so price and qty required items,
                //which are typically not in resume, will work in mobile resumes
                if(!fSAInItemSale)
                {
                   fSAInItemSale = true;
                }

                //TAR336531
                CREATE_AND_DISPENSE(InProgress)(storeMode(), helpMode(),
                    co.getTimeOutItemTransactionResult());
                //RETURNSTATE(InProgress);
                break;
            }

		case TB_ITEMSOLD:
            {
                
                CString csStateName = this->GetRuntimeClass()->m_lpszClassName;
                if((csStateName == _T("SMRAPDataNeeded"))) 
                {
                    trace(L6, _T("Received item sold while on Rap Data Needed screen.  deleting new state"));
                    delete getRealStatePointer( pState);
                    SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
                    return STATE_NULL;
                }

                // POS32142 there is a chance where we get the SecMisMatch before the TB_NEEDMOREDATA
                // and the auto voided TB_ITEMSOLD will cause the base to delete the WaitForApproval state that is stored in the stack - rrs
                CString csPOSState(_T(""));
                SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);
                if( g_pDelay_TBExpectedStateChange != NULL && csPOSState == _T("INVALID_WIC_FREE_ITEM") && io.fVoid )
                {
                    trace(L6, _T("Received voided item sold while on INVALID_WIC_FREE_ITEM POSSTATE and stacked state.  deleting new state!!!"));
                    delete getRealStatePointer( pState);
                    return STATE_NULL;
                }
                // e POS32142
				//Start CRD 139954
				sEASItem.csItemEntryId = io.d.csItemID;
				//End CRD 139954

				//CRD 185620 if an item is being voided and there's an EAS item in the transaction, we have to check if the item voided is an EAS item
				// by checking the entry id that we saved at g_mapTransactionEASItem
				// if entry id exist, then we have to remove Cashier Assistance Required text
				// then check if there are remaining EAS items in the transaction, if there is, then do not clear the intervention, if there's none, clear the intervention automatically
				
				if(io.d.fVoid && lUnapprovedEASItems)
				{
					//Step 1 - determine if item voided is an EAS item
					POSITION nPos = g_mapTransactionEASItem.GetStartPosition();
					while (nPos != NULL)
					{
						COleVariant vValue;
						CString csItemId, csMsgItemEntryId;
						int nKey;
						bool bValue;

						g_mapTransactionEASItem.GetNextAssoc(nPos, nKey, bValue);

						CString csnKey;
						csnKey.Format(_T("%d"), nKey);
					
						if( csnKey == io.d.csItemID) //entry ID of the item voided is in the array hence this is an EAS item
						{
							bEASApproveFromVoid = true;
							lUnapprovedEASItems--;	//decrement while clearing up Cashier Assistance Required message
							
							g_mapTransactionEASItem.SetAt(nKey, bValue);
							csItemId.Format(_T("%d"), nKey);
							csMsgItemEntryId = csItemId + _T("EASItem");
							vValue = _T("");

							g_mapTransactionEASItem.RemoveKey(nKey);
							
							//ps.GetPSXObject()->SetReceiptItemVariable(_T("CMSMReceipt"), csMsgItemEntryId, _T("ITEM_MSG"), vValue);
							ps.GetPSXObject()->RemoveReceiptItem(_T("CMSMReceiptEAS"), io.d.csItemID);
							ps.GetPSXObject()->UpdateReceiptControls(_T("CMSMReceiptEAS"));
						
						}
						//else do nothing since it's not an EAS item
					}
					
					ps.GetPSXObject()->UpdateReceiptControls(_T("CMSMReceipt"));
					
				}
				
				
				//(+)VSTS 131568
				if(SMState::bVoidApprovalDelayed && io.fVoid)
				{
					CString csWork = ps.GetPSText(SSF_TXT_VOIDITEM);
					CustomMessageObj.csLeadThruText = _T("");
					CustomMessageObj.csScreenText = csWork;    
					RETURNSTATE(CustomMessage)
				}
				//(-) VSTS 131568
            }
            break;
			/*POS12376:Handle case when we exit training mode from POS sign in menu state*/
		   case TB_LEFTTRAINING:
			 {
				int nState = getAnchorState();
 
				if( nState == BES_OUTOFSERVICE2 )
				{
                    
				   fStateFreeze = false;
 
				   //SMarks. this may seem odd to return to authorization instead of
				   //attract but this is what the base naturally does when we exit from
				   //lane closed.  The system goes back to authorization and returns psbutton8
				   //return to shopping.  Changing the SSF override to operate more like the
				   //base.
				   m_bIsFromLaneClosed = true;
				   setAnchorState(BES_START);
				   trace(L3, _T("!!!! Left Training Mode from lane closed with Supervisor password required"));
				}          
			 }
			 break;
            
//(+) POS150388 -  Set m_bWICEBTSNBEnable to false to disable the button when we get the WIC Eligible Item Total message from POS.
             
           case TB_REWARDLINE:
               {

                      SMState::m_bWICEBTSNBEnable = false;
               }
               break;
//(-) POS150388 -  Set m_bWICEBTSNBEnable to false to disable the button when we get the WIC Eligible Item Total message from POS.
			// +CRD 140001 - Auto process of QTY when a quantity required items recevied
			case TB_NEEDQUANTITY:
			{
					//TAR339619
					if (fStateAssist && co.csTenderAllowCoupons == _T("N") )
						bIsCoupon = false;

					if (!g_bInRecallMode && !m_bTMDAccepted && !fSAInItemSale && !co.IsInAssistMenus()) //RFQ 986	//CRD 168992
					{
						trace(L0, _T("Ignoring TB_NEEDQUANTITY because there is no item sale in progres"));
						return STATE_NULL;
					}
					
					fInSignedOn = true; 		 // must be signed on by now
					fInSignedOff = false;
					
					delete getRealStatePointer( pState);
					
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
				}
				break;// -CRD 140001
        }// end of switch(m_TBLastState)
    }else
    {
        switch(m_TBLastState)
        {
		case TB_ENABLE_ASSISTMODE:
			{
				 delete getRealStatePointer( pState);
				 return STATE_NULL;

			}
        case TB_SENDMESSAGETOSCOT:
            {
                ra.OnNormalItem(RAP_RECEIPT_LINE_HIGHLIGHT + ps.GetPSText(RA_POSMESSAGE, SCOT_LANGUAGE_PRIMARY) + _T(" ") + csTBMessage2Scot);
                
                if (storeMode())
                    ps.ShowSMTBText(_T(" "));
                else
                    ps.ShowCMTBText(_T(" "));
                
                break;
            }
        case TB_NEEDCOUPONCHANGE:
            {
                if(lCoupons > 0)  // Fix for TAR#302254.
                {
                    lCoupons--;   // The Core automatically increases the number of
                }                 // coupons even if you're just changing its value.
                break;
            }
            //TAR338118+ 
        case TB_ITEMSOLD:
            {
                if ( dm.fStateTakeawayBelt && !io.fVoid && (io.cScanType == 110 ) )
                {
                    //tell SM/TAB that the keyed in item is just sold so belt will move 
                    SASendSCOTInput( UPC_MANUAL, io.csItemCode );
                    trace(L6, _T("Tell SM/TAB that one keyed in item just sold"));
                }  
				
				//Start CRD 139954
				sEASItem.csItemEntryId = io.d.csItemID;
				//End CRD 139954

				//CRD 185620 if an item is being voided and there's an EAS item in the transaction, we have to check if the item voided is an EAS item
				// by checking the entry id that we saved at g_mapTransactionEASItem
				// if entry id exist, then we have to remove Cashier Assistance Required text
				// then check if there are remaining EAS items in the transaction, if there is, then do not clear the intervention, if there's none, clear the intervention automatically
				if(io.d.fVoid && lUnapprovedEASItems)
				{
					//Step 1 - determine if item voided is an EAS item
					POSITION nPos = g_mapTransactionEASItem.GetStartPosition();
					while (nPos != NULL)
					{
						COleVariant vValue;
						CString csItemId, csMsgItemEntryId;
						int nKey;
						bool bValue;
						g_mapTransactionEASItem.GetNextAssoc(nPos, nKey, bValue);
						
						CString csnKey;
						csnKey.Format(_T("%d"), nKey);
						
						if( csnKey == io.d.csItemID) //entry ID of the item voided is in the array hence this is an EAS item
						{
							bEASApproveFromVoid = true;
							
							lUnapprovedEASItems--;	//decrement while clearing up Cashier Assistance Required message
							
							g_mapTransactionEASItem.SetAt(nKey, bValue);
							csItemId.Format(_T("%d"), nKey);
							csMsgItemEntryId = csItemId + _T("EASItem");
							vValue = _T("");
							
							g_mapTransactionEASItem.RemoveKey(nKey);
							
							
							ps.GetPSXObject()->RemoveReceiptItem(_T("CMSMReceiptEAS"), io.d.csItemID);
							ps.GetPSXObject()->UpdateReceiptControls(_T("CMSMReceiptEAS"));
							
						}
						//else do nothing since it's not an EAS item
					}
					ps.GetPSXObject()->UpdateReceiptControls(_T("CMSMReceipt"));
					
					// if no more unapproved EAS items after voiding, them complete the intervention
					if(!lUnapprovedEASItems)
					{
						bGenericDelayedFinishSent = true;	//CRD 232271
						m_GenericDelayedInterventionStatus = GENERIC_DELAYED_COMPLETE;
						SendTBCommand(TBCMD_EASCOMPLETE, false);
					}
					
				}
            }
            break;
            //TAR338118-		
        }
    }// end of if (m_bCustomerState)// end of if (m_bCustomerState)
	//Start CRD 185620
	if(m_TBLastState == TB_GENERIC_DELAYED)
	{
		//if Generic_delayed is for EAS items
		//CString csEASItemEntryID = GetTBProperty(_T("EASEntryID"));
		trace(L6, _T("SMStateTB::TB_GENERIC_DELAYED - lUnapprovedEASItems = %d, sProcessingDelayedIntervention = %d, csEASItemEntryID = %s"), lUnapprovedEASItems, sProcessingDelayedIntervention, sEASItem.csItemEntryId);
		if(lUnapprovedEASItems && sProcessingDelayedIntervention == DELAYED_EAS && !g_bInRecallMode)
		{	
			appendEASMsgToReceipt(_wtol(sEASItem.csItemEntryId));
			addItemToEASREceipt(sEASItem.csItemEntryId);
		}

		sProcessingDelayedIntervention = DELAYED_NONE;
		SetTBProperty(_T("IsAnEASItem"), _T("0"));	//reset
		SetTBProperty(_T("IsMobileGenericDelayed"), _T("0"));
		sEASItem = EAS_ITEMS();	//reset struct

	}
	
	if(m_TBLastState == TB_GENERIC_DELAYED_COMPLETE)
	{
		if(lUnapprovedEASItems)		//EAS intervention approved. remove Cashier Assistance Required messages in the items
		{
			isPlayedAssistanceAudio = false;

			POSITION nPos = g_mapTransactionEASItem.GetStartPosition();
			while (nPos != NULL)
			{
				COleVariant vValue;
				CString csItemId, csMsgItemEntryId;
				int nKey;
				bool bValue;
				g_mapTransactionEASItem.GetNextAssoc(nPos, nKey, bValue);
				
				if( !bValue )
				{
					lUnapprovedEASItems--;	//decrement while clearing up Cashier Assistance Required message
					g_mapTransactionEASItem.SetAt(nKey, bValue);
					csItemId.Format(_T("%d"), nKey);
					csMsgItemEntryId = csItemId + _T("EASItem");
					vValue = _T("");
					
					g_mapTransactionEASItem.RemoveKey(nKey);
					
				//	ps.GetPSXObject()->SetReceiptItemVariable(_T("CMSMReceipt"), csMsgItemEntryId, _T("ITEM_MSG"), vValue); CRD 212453 remove call to remove Cashier Assistance Required text
				}
			}
			ps.GetPSXObject()->UpdateReceiptControls(_T("CMSMReceipt"));
		}
		//Start CRD 223409
		if(lUnapprovedMobileGenericDelayed)
			lUnapprovedMobileGenericDelayed = 0;
		//End 223409

		bUnapprovedAccountLookupFailed = false;		// CRD 554916
	}
	//End CRD 185620    
	
	//Start CRD 177093 - make sure both customer mode and assist mode handles TB_RESUME_COMPLETE
	if(m_TBLastState == TB_RESUME_COMPLETE)	
	{
		if(g_bInRecallMode)
		{
			delete getRealStatePointer( pState);
			trace(L6, _T("Received TB_RESUME_COMPLETE outside Recalling Items state, going to Recalling State to be in sync"));
			//In the event SCO receives TB_RESUME_COMPLETE message outside Recalling Items state, tell SCO to go to Recalling Items and process FinishRecall

			//CRD 232268 need to check if we have mobile assistance neededEAS items
			checkMobileAssistanceNeededItems();
			checkEASItemsList();
			
			SMRecallingItems::csSuspendBarcode = _T(""); //clear this CString when resume has started
			bFinishRecall = true;
			fSAInItemSale = false;
			SetHookProperty(_T("IsResumeStarted"), _T("0"));    //CRD 330778 reset IsResumeStarted as soon as SAS transfer is complete

			//CRD 237852 
			//instead of going to FinishRecall() when the resume completes, remain in current state, 
			//send the End FLMS with mPerks barcode and wait for mPerks messages
			//FinishRecall();
			sendFLMSTLog(1, 0); //CRD 145555
			return STATE_NULL;
			//RETURNSTATE(RecallingItems)
		}
	}
	//end CRD 177093

    if( m_TBLastState == TB_CANCEL_ITEM )
	{
		delete getRealStatePointer( pState);
		
		trace(L3, _T("SMStateTB::TBParse - TB_CANCEL_ITEM; TB asked for item to be cancelled"));
		
		SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
        
        if (co.IsInAssistMenus())
        {

            RETURNSTATE(SmAssistMenu)
        }

		return STATE_NULL;
	}

    //POS23514:Mobile Shopper
    if( m_TBLastState == TB_RESUME_ITEMS || m_TBLastState == TB_RESUME_ACS_REWARD)
    {
        trace(L3, _T("SMStateTB::TBParse - TB_RESUME_ITEMS"));
        
        // TAR 363410 - Since resuming items doesn't go through TBItemSale,
        //              we must manually reset the item object to ensure
        //              we're not carrying over false attributes from prior item resumed.
        io.Reset();
        TBGetItemDetails(io.d);
        
        bool bIsLinkedItem = io.d.bLinkedItem;
        if(!bIsLinkedItem) 
        {
            io.lQuantityEntered = io.d.lQtySold;
            io.lTotalWeightEntered = io.lWeightEntered;    
            io.lWeightEntered = io.d.lWgtSold;			   
        }
        io.fVoid = io.d.fVoid;
        io.lUnitPriceEntered = io.d.lUnitPrice;
        io.lExtPriceEntered = io.d.lExtPrice;		   
        io.csItemCode = io.d.csItemCode;
        
        //SOTF 6822

        bool bPrevAssistMenuStatus = co.IsAttendantModeOn();

        //if we are on an age restricted item while resuming
        //do not play approval needed.  Set the in assist menu
        //flag to avoid the wave file
        if(io.d.fRestricted)
        {
            co.SetInAssistMenus(true);
        }

        //sotf

        //POS23514 Mobile Shopper: Do not call if this is an ACS Reward
        if(m_TBLastState == TB_RESUME_ITEMS)
        {
           SMStateBase *rc = TBItemDetails();
        }

        //SOTF 6822 
        
        co.SetInAssistMenus(bPrevAssistMenuStatus);
        
        //sotf

        io.csItemCode=_T("");
        itemDetailOrig.Reset();
        itemDetailNew.Reset();
        
        if(	SendTBCommand(TBCMD_RESUME_ITEMS, NULL) == 0)
        {
            m_bIsFromDisplayingResumeItem = true;
            
            SetHookProperty(_T("IsResumeComplete"), _T("1"));
            SetTBProperty(_T("IsThereItemNeedToDisplayed"), _T("0"));
            SetTBProperty(_T("IsDirtyTrxWithNoItems"), _T("0"));
            
            if( GetTBProperty(_T("ScannedBarcodeAfterResume")) == _T("") )
            {
				trace(L3, _T("SMStateTB::TBParse - TB_RESUME_ITEMS"));

                delete getRealStatePointer( pState);
                SASendSCOTInput( TRANSACTION_CONTINUE );
                bEASReEnableScanner = true;
                //POS11024-Mobile Shopper: If in assist mode, do not setanchor state to S&B
                if(!fStateAssist)
                {
                   return setAndCreateAnchorState(BES_SCANANDBAG);
                }
                else
                {
                   return STATE_NULL;
                }
            }
            else
            {
				trace(L3, _T("SMStateTB::TBParse - TB_RESUME_ITEMS"));
                // resume items e-receipt load triggered by scanned item on SMFinish 
                // process the scan
                SASendSCOTInput( TRANSACTION_CONTINUE );
                csItemSent = GetTBProperty(_T("ScannedBarcodeAfterResume"));
                SetTBProperty(_T("ScannedBarcodeAfterResume"));

				//POS40946 - reset this flag so that after scanning an item, it wont automatically move the SelectPayment
                 m_bIsFromScanReceipt = false;
                delete getRealStatePointer( pState);
                return TBItemSale(false,0,true);
            }
        }
        
    }
    if ((m_TBLastState == TB_LOG_NEGATIVE_MEDIA) && (!io.d.fCoupon))
        
    {
        CString csLogCouponEntries;
        CString csDescription = io.d.csDescription;
        long lReversedPrice = io.d.lExtPrice * (-1);
        long lQtySold = io.d.lQtySold;
        CString csID=io.d.csItemID;
        if (io.fVoid)
        {
            // Do this as a void overrides the original entry.  This will result in a single entry
            // in the reporting database with a count and quantity of 0.
            lQtySold = 0;
            lReversedPrice = 0;
        }
        // Trim the negative media to desc only; description is followed by department right aligned
        // E.g. 'Misc Credit             160'
        //      'Bottle Refund           110'
        int iDescEnd = csDescription.Find(_T("   "));
        if( iDescEnd > 0 )
        {
            csDescription = csDescription.Left(iDescEnd);
        }
        
        //+ TAR385622
        csDescription.MakeUpper();
        //- TAR385622
        
        csLogCouponEntries.Format(_T("CouponCount=\"%d\" CouponAmount=\"%s\" MediaType=\"%s\" ID=\"%s\" "), 
            lQtySold, ps.FmtDollar(lReversedPrice,(enum PSProceduresBase::SCREEN_POSITION) 0, ps.REPORTINGFORMAT),
            csDescription, csID);
        rp.TBCreateReportEntry(_T("MCoupon"), csLogCouponEntries);
        trace(L6, _T(" Log Negative Media %s"), csLogCouponEntries);        
    }

    //POS36319
    if( (m_TBLastState == TB_INSERT_MEDIA_ITEM && !io.d.fCoupon) )
    {
        //POS36319
        if( GetTBProperty(_T("CollectCMCoupons")) == _T("N") ) 
        {
            CString csTBState(_T(""));
            SendTBCommand(TBCMD_GET_TB_STATE, &csTBState);
            
            // POS40438 - don't display the InsertCoupon during AssistMenu or AssistMode or voided bottleslip/coinstar
            if ( (!io.d.fVoid) && (!fStateAssist) && (!co.IsInAssistMenus()) && IsUsingCouponSensor() )
            {
                delete getRealStatePointer( pState);
                if( getAnchorState() == BES_FINISH || csTBState.Find( _T("TENDER_GET_COUPON_IP") ) != -1 || csTBState.Find(_T("TENDER_NEGATIVE_ITEM")) != -1 )
                {
                    CREATE_AND_DISPENSE(InsertCoupon)(getAnchorState(), BES_FINISH);
                }
                else
                {
					// POS89890 - Update Security manager about Scan & Bag state exit before leaving the state.				
	                TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") ); //SSCOI-24325.				
                    CREATE_AND_DISPENSE(InsertCoupon)(getAnchorState(), BES_SCANANDBAG);
                }
            }            
        }

        //POS62368 - rrs
        CString csPosState = GetHookProperty(_T("__POSState")); 
        
        trace(L6, _T("InsertMediaItem with POSState - %s"),csPosState);
        if( co.IsInAssistMenus() && csPosState == _T("RESTRICTED_USE_COUPON_ACCEPTED") ) 
        {
            SendTBCommand(TBCMD_SENDKEYEDDATA, CString(_T("{F1}")));
            trace(L6, _T("Clear the OK button on POS"));

            RETURNSTATE(SmAssistMenu)
         }
        //e POS62368 - rrs

        delete getRealStatePointer( pState);
		return STATE_NULL;
        
    }
    // e POS36319    

    // POS83168: START
    if (m_TBLastState == TB_POS_SCANNER_READY)
    {
		/*
        CString csStateName = this->GetRuntimeClass()->m_lpszClassName;
        if((csStateName == _T("SMScanAndBag")))
        {
           // The POS sales app is ready for scanned input so enable the scanner since the Scan&Bag screen is active.
           trace(L6, _T("Enabling scanner because POS is ready for scanned input"));
           DMScannerEnable();
        }
		else
		{
			trace(L6, _T("_* not in scan and bag"));
		}

        // Indicate that the sales app is ready for scanned input.
        SetHookProperty(_T("WaitForPosScannerReady"), _T("0"));
		*/ 
		trace(L6, _T("Ready to send next FLMS"));
		if ( g_bInRecallMode)
		{
			/*
			if (g_bEnteringRecallMode)
			{
				trace(L6, _T("Ignore in Entering Recall Mode FLMS"));
			}
			else
			{
				SMRecallingItems::g_nTimeoutRecallingWaitForPos = 0;  // RFQ 9545
				SMRecallingItems::g_bRecallingWaitForPos = false;	   // RFQ 9545	

				trace(L6, _T("Recall Mode FLMS"));
				RETURNSTATE(RecallingItems)
			}
			*/

		}

    }
    // POS83168: END

    if (io.fVoid &&  m_TBLastState == TB_ITEMSOLD)
    {
        // When ever we void item, we need to make sure that FastLane Receipt and POS
        //	has same selected item. Related to fix to TAR: 301417

        // sotf 9269 - dont' move the cursor if it's in AM - rrs
        //SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(_T("AssistReceipt")));
    }
    
    else if (m_TBLastState == TB_TRXEND)
    {
        // the trxn is over if there are any unapprove coupons there is nothing we can do about it now
        // if CouponRatioApprovalRequired is turned on and we go in to assist mode at the end of the 
        // trxn we may ask for a bogus coupon ratio approval because the total will have changed.  Since
        // the trxn is over clear this flag
        SMStateBase::lUnapprovedCoupons = 0;
        
        // The transaction end in the base code sets the sale total to the current total.
        // Unfortunately, for this product, the total is the change due at this point rather
        // than the sales total.   Can't just stop taking totals at finish because the if you do
        // a partial payment and then go back and purchase another item, the total is reduced
        // by the amount of the partial payment.  I found a two part solution.  For everything except
        // suspend, the total paid plus the total sale always equals the sales amount.  If you purchased
        // $5 and tendered $20, Total paid is $20 and total sale is -$15 totalling 5.   For suspend you have 
        // to play a couple of games.  First of all, I save that calculation at the time the suspend confirm 
        // button is pressed.  Secondly, I clear that value in attract rather than here because TRXEND seems
        // to come through twice, once for the suspend and then a cancel.  This seems to work.
        if (m_lLastSuspendTotal != 0)
        {
            rp.SetTransactionAmount(m_lLastSuspendTotal);
            rp.m_nDisposition = RP_DISPOSITION_SUSPEND;
        }
        else 
        {
            
            rp.SetTransactionAmount(lTotalPaid + lTotalSale);
            
        }

// (+) POS204222

		if(SMState::m_bActivateHHScanner)
		{
            SMState::m_bActivateHHScanner = false;
    
        CString csRAPName;
    
        if(SMSmAssistMode::m_csRAPConnectionName == "")
        {
            SMSmAssistMode::m_csRAPConnectionName = SMState::GetRAPConnectionName();
        }
   

        CString csOperation=_T("operation=release-scanner;");
        m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), csOperation, SMSmAssistMode::m_csRAPConnectionName );
    

    }

// (-) POS204222
    }
    
	// Start CRD 125377 
	if ((m_bShopAndDropEnabled) && (!m_bSNDmPerksCancel) && (m_TBLastState == TB_TENDERACCEPTED))
	{
		trace(L6, _T("SND tender accepted Suspend Transaction"));
		ClearStateStackEOT();
		return TBSuspendTransaction(); 
	
	}
	// End CRD 125377

	if(m_TBLastState == TB_SENDEODCASHREPORT)
	{
		//POS initiated store close request for EOD, and not for new software installation
		if (Is5XHardware() || !_tcsicmp(co.csHWType, _T("SCOT6")))
		{
			trace(L6, _T("Cash Monitoring Counts for change over - End of Day"));
			CashMonitoringCounts(3); // 3 - Change over - End of Day
		}
	}
    return pState;
}
#endif //_TRADESHOWDEMO

SMStateBase * SMState::TBItemDetails(void)
{
    //fix for tar 353778
	if ( io.d.lQtySold < 0 )
		io.d.lQtySold = -(io.d.lQtySold);

    //In assistmode only Security resume weight message caused the LGW to be reset to beginning wt
    if ((TB_OVERRIDE_SECURITY_WITH_NOWEIGHT == io.d.lRequireSecBagging ) && (fStateAssist))
    {
        io.d.lRequireSecBagging = TB_LET_SECURITY_DECIDE;
    }

    //SOTF 6226
    //if delayed age is not turned on the do not inform the base for restricted items
    //Basically, the TB is doing the intervention immediate

    //POS31050 - The MobileEnhancement uses 'SmEnhancedAssistMode' class so we need to add the csStateName for this
    bool bDelayedAgeEnabled = SendTBCommand(TBCMD_ACS_DELAYED_AGE_RESTRICTION);
    CString csStateName = _T("");
    csStateName = this->GetRuntimeClass()->m_lpszClassName;
    bool bAgeItemSoldInAM = false;
    if( (bDelayedAgeEnabled) && ( (csStateName.Find(_T("SmAssistMode")) != -1) || csStateName.Find(_T("SmEnhancedAssistMode")) != -1 ) )
    {
        if(io.fVoid)
        {
            CString csItemEntryId;

             if (INVALID_ENTRYID == io.d.lEntryID)
             {
                csItemEntryId = io.d.csItemID;
             }
             else
             {
             
		        csItemEntryId.Format(_T("%d"), io.d.lEntryID);
	         }
            COleVariant vValue;
		    m_pAppPSX->GetReceiptItemVariable(_T("CMSMReceipt"), csItemEntryId, _T("ITEM_RESTRICTED"), vValue);
            
            if ((vValue.vt == VT_EMPTY) || (vValue.boolVal == false))
            {
                 io.d.fRestricted = false;
            }
        }else
        {

            if(io.d.fRestricted)
            {
                io.d.fRestricted = false;
                bAgeItemSoldInAM = true;
				//SSCOI-47774
                m_bAgeItemSoldInAM = true;
				nCustomerAge=99; 
            }
            
        }
    }
    //-SOTF

    //POS40287 - detect if this is just the recalculated coupon item - rrs
    if (io.d.fCoupon)
    {
        if ( !io.fVoid)
        {            
            //POS40287 - handling CM coupon re-issue - rrs
            CString csCouponID = io.d.csItemID;
            COleVariant vValue = VARIANT_FALSE;
            trace(L6, _T("Coupon ID (%s)"), csCouponID);
            if ( m_pAppPSX->GetReceiptItemVariable(_T("CMSMReceipt"), csCouponID, _T("ITEM_COUPON"), vValue) == PSX_SUCCESS )
            {
                if (vValue.boolVal == VARIANT_TRUE)
                {
                    io.d.fCoupon = false;
                    
                    // get the existing coupon price
                    vValue.Clear();
                    m_pAppPSX->GetReceiptItemVariable(_T("CMSMReceipt"), csCouponID, _T("ITEM_EXT_PRICE"), vValue);
                    vValue.ChangeType( VT_INT );
		            long lUPrc = vValue.lVal;

                    trace(L6, _T("Existing Coupon Price(%d), New Coupon Price(%d)"), lUPrc, io.d.lExtPrice );
                        
                    if( lUPrc != io.d.lExtPrice )
                    {
                        long lAmtDiff = lUPrc - io.d.lExtPrice;
                        trace(L6, _T("Coupon Amt Diff (%d)"), lAmtDiff);
                        m_lTBCouponAmount += (lAmtDiff);
                        lTotalCouponAmount += (lAmtDiff);
                    }
                }               
            }        
        }
    }
    //e POS40287 - rrs
    //+POS99147
        trace(L6, _T("+SMStateTB, handling for codes with leading 0s"));
    CString csDesc,csCode, csItemEntryId;
    long lQty,lWgt,lPrc,lUPrc, lTcd, lBarcodeType;
     if (INVALID_ENTRYID == io.d.lEntryID)
     {
        csItemEntryId = io.d.csItemID;
     }
     else
     {
        csItemEntryId.Format(_T("%d"), io.d.lEntryID);
     }    
    ps.GetItem(csDesc,csCode,lQty,lWgt,lPrc,lUPrc,lTcd,lBarcodeType,_T("CMSMReceipt"),_T("CMReceiptVoid"),csItemEntryId);
    
    trace(L6, _T("SMStateTB - io.csItemCode(%s), csCode(%s), csItemEntryId(%s)"), io.csItemCode, csCode,csItemEntryId);
    trace(L6, _T("SMStateTB - lPrc(%d), io.d.lExPrice(%d), lQty(%d)"), lPrc, io.d.lExtPrice, lQty);
    if( io.csItemCode != _T("") && csCode != _T("") && !io.fVoid)
    {  
       trace(L6, _T("+SMStateTB, updating lItems:%d"),lItems);       
       if(csCode.Find(io.csItemCode) == -1 && io.csItemCode.Find(csCode) != -1) //SSCOI-46402. && makes sure that this evaluates to true only when you have leading zero, leaving rest to base
       {            
           //the base supposed to do this but since it did not match the ItemCode due to leading 0, let just do it here.           
           lItems -= lQty;    
           trace(L6, _T("SMStateTB - lItems decremented."));    
       }
       trace(L6, _T("-SMStateTB, updating lItems:%d"),lItems);   
    } 
     trace(L6, _T("-SMStateTB, handling for codes with leading 0s"));
	
    //-POS99147
    SMStateBase *retState = SMStateBase::TBItemDetails();

    CString csPOSState(_T(""));
    SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);

    if(// csPOSState == _T("SALE_RESTRICTED") &&  //VSTS 131568
        getRealStatePointer(retState) && 
        EXTRACT_CLASSNAME(retState).Find(_T("VoidApproval")) != -1 && (SMState::nVoidCounts <=3) )
    {
        // (1) mimic void approval initialization
        CKeyValueParmList parmList1;
        CKeyValue<const _TCHAR*>::Put(parmList1, KEY_IV_OPERATION, KEY_IV_OPERATION_BEGIN);
        _bstr_t bstrParmList1((const _TCHAR *)parmList1); 
        SecMgr.VoidItem(lSecMgrCookie, bstrParmList1);
        TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ApproveVoidedItem") );
		 trace(L1, _T(" Delaying Void approval intervention for first 3 items"));
        // (2) mimic void approval
        SMStateBase::bIsFromVoidApproval = false; 
        delete getRealStatePointer(retState);
        retState = OnRAPClearSecurityBtn(_T("Lane")); 

        // (3) mimic void approval uninitialize
        CKeyValueParmList parmList2;
        CKeyValue<const _TCHAR*>::Put(parmList2, KEY_IV_OPERATION, KEY_IV_OPERATION_END);
        _bstr_t bstrParmList2((const _TCHAR *)parmList2); 
        SecMgr.VoidItem(lSecMgrCookie, bstrParmList2);
        ClearSecurityViolationLight();
        SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
        TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ApproveVoidedItem") );
    }

    //POS59953 - Dont send the CANCEL_ITEM if it's a linked item as it caused the OOS with SM - rrs
    //TAR354932+
	if ( (io.d.lExtPrice < 0 ) && (TB_LET_SECURITY_DECIDE == io.d.lRequireSecBagging) && (!io.d.bLinkedItem) )
	{
       //tell SM to cancel this item so SM doesn't wait on the weight
		SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
	}
    //TAR354932-
    
    // fixes hang in BagAndEAS issue because of security not receiving UPC_MANUAL 
    // msg in the care where a linked item is scanned, followed by a manually
    // entered item, either keyed in or selected from PLU list.
    if( io.d.bLinkedItem )
    {
        nSAPrevSCOTInputID = ITEM_SOLD_SCAN;
    }

    if (io.d.fCoupon)
    {
        if (io.fVoid)
        {
            m_lTBCouponCount--;
            m_lTBCouponAmount += (io.d.lExtPrice);;
        }
        else
        {
            m_lTBCouponCount++;
            m_lTBCouponAmount += -(io.d.lExtPrice);;
		    trace(L1, _T(" Coupon Purchased Count %d, Amount %d, This Amount %d"), m_lTBCouponCount, m_lTBCouponAmount, io.d.lExtPrice);
        }
    }

    //sotf 6226
    //add the age XX required text
    if(((io.d.fRestricted) || (bAgeItemSoldInAM))&& 
       (!io.fVoid) &&
       (bDelayedAgeEnabled))
    {
        CString csDesc, csItemEntryId, csMsgItemEntryId, csMsgItemEntryRapId,csReceiptSrc, csDescPrimary;
        csDesc.Format(ps.GetPSText(SSF_AGE_REQUIRED, ps.m_languageUsed), io.d.nRestrictedAge); 
        
        //add spaces to make the line up the "item not bagged text"
        csDesc =_T("  ") + csDesc;
        
        csReceiptSrc = _T("CMSMReceipt");
        csItemEntryId =  io.d.csItemID;
        csMsgItemEntryId = csItemEntryId + _T("AgeRequired");

        COleVariant	vValue;
        vValue= csDesc;
        m_pAppPSX->CreateReceiptItem(csReceiptSrc, csMsgItemEntryId, csItemEntryId);        
	    m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csMsgItemEntryId, _T("ITEM_MSG"), vValue);
		
        m_pAppPSX->UpdateReceiptControls(csReceiptSrc);
					
    }
    //SOTF 6226

    if( GetHookProperty(_T("IsResumeTrxInProgress")) == _T("1") )
	{							
        CString csPosState = GetHookProperty(_T("__POSState"));
        if( 
            (csPosState.Find(_T("PRICE_REQUIRED_INVALID")) != -1)       ||
            (csPosState.Find(_T("VARIABLE_COUPON_VALUE_INVALID")) != -1)||
            (csPosState.Find(_T("PRICE_REQUIRED")) != -1)
          )
        {
            fSAInItemSale = true;
        }

		delete getRealStatePointer( retState); 
		return STATE_NULL;
	}

    
    return retState;
}

SMStateBase * SMState::TBGetTotalDetails(void)
{
    SMStateBase *sUser = STATE_NULL;
	
	STotalDetails dTotal;
	memset(&dTotal, 0, sizeof(STotalDetails));
	
	
	//long rc= tb.GetTotalDetails(&dTotal);
    long rc = ::TBGetTotalDetails(&dTotal);
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
	
	lFoodStampsDue = dTotal.lFoodStampDue;
	if (dTotal.szErrDescription)
		try{	delete [] dTotal.szErrDescription;} catch(...){trace(L6,_T("delete exception:dTotal.szErrDescription"));}	
	
	// Start CRD 125377 
	if ((m_bShopAndDropEnabled) && (!m_bSNDmPerksCancel))
	{
        trace(L6, _T("Before any changes lBalanceDue=%d lItems=%d lTotalSale=%d fInSignedOn=%d fInSignedOff=%d"), lBalanceDue, lItems, lTotalSale, fInSignedOn, fInSignedOff);
		
		lBalanceDue = 5;
		lTotalSale =  5;
		
        trace(L6, _T("After changes and before call lBalanceDue=%d lItems=%d lTotalSale=%d fInSignedOn=%d fInSignedOff=%d"), lBalanceDue, lItems, lTotalSale, fInSignedOn, fInSignedOff);
		
	}
	// End CRD 125377
	// show the taxes anf total sale
	ps.Tax(lTaxDue);				  // update the taxes
	ps.Total(lTotalSale);			 // update current total


	if (lBalanceDue != 0) // ???? need to test MGG
		m_lSavedforMobileTotal = lTotalSale;

	if (m_lSavedforMobileTransNumber == 0)
		m_lSavedforMobileTransNumber  = TBGetCurrentTransactionNumber();
	
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
	
    lBalanceDue = lTotalSale;  //fix of bugs: Incorrect amount Due displayed in Payment type menu 
    ps.UpdateAmountDueAmountPaid(lBalanceDue - nDMCashInserted, lTotalPaid + nDMCashInserted );
	
    // Refresh the CanSuspendStatus - it's possible the total has changed back to 0 due 
    // to a delayed promotion removal
	ra.OnCanSuspendNow( CanSuspendTransaction() ) ; 
	
    return STATE_NULL;
}

SMStateBase * SMState::TBParseFoodStampTotal(void)
{
    return SMStateBase::TBParseFoodStampTotal();
}

void SMState::TBGetItemDetails(ItemDetails &iDetails)
{
    SMStateBase::TBGetItemDetails(iDetails);

	/*
	if (g_bInRecallMode)
	{
		m_bRecallModeWaitforItemSold = false; // RFQ 9545
		trace(L6, _T("Ready to process next item - in Recall Mode FLMS"));
	}
    */

	CString strPropName = _T("PropertyAllowMSGVoidsBTYPE");
    long lValue = (long)io.cScanType;
    char ucValue[33];
    _ltoa(lValue,ucValue,10);
    CString strValue = ucValue;
    SendTBCommand(TBCMD_SET_HOOKPROPERTY, &strPropName, &strValue);
    
    strPropName = _T("PropertyAllowMSGVoidsVOID");
    strValue = _T("0");
    if (iDetails.fVoid)
        strValue = _T("1");
    SendTBCommand(TBCMD_SET_HOOKPROPERTY, &strPropName, &strValue);
	//(+) POS320066
    CString csItemtype = GetHookProperty(_T("PropItemType"));
    trace(L6, _T("Itemtype =%s"), csItemtype);
	if(((csItemtype == _T("11")) || (csItemtype == _T("12")) || (csItemtype == _T("13")))|| (iDetails.csDescription.Find(_T("EXTRA COUPON")) != -1))
	{
       trace(L6, _T("Making fcoupon flag as false for Extra Coupon"));
	   iDetails.fCoupon = false;		
	}
	//(-) POS320066
}

void SMState::UpdateTotal(void)
{
    SMStateBase::UpdateTotal();
}

SMStateBase * SMState::TBBalanceDetails(void)
{
    int nTmpCashInserted = nDMCashInserted; //+- POS296865
    
    SMStateBase *pRetState = SMStateBase::TBBalanceDetails();

     //+ POS296865
     if( _T("1")== GetTBProperty(_T("IsWICTenderAccepted")))
     {

        if(nTmpCashInserted > 0)
        {
            trace(L6, _T("Resetting partial cash tender values after WIC Tender Accept"));
        
            lTotalPaid -= nTmpCashInserted;
	        lChangeDue -= nTmpCashInserted;
	        nDMCashInserted = nTmpCashInserted; 
        }
        SetTBProperty(_T("IsWICTenderAccepted"), _T("0"));
     }//- POS296865

	if(lBalanceDue>0)
	{
		CString strNewState = _T("");

		if ( pRetState > STATE_ERROR ) //TAR343037
			 strNewState = pRetState->GetRuntimeClass()->m_lpszClassName;
		if(strNewState == _T("SMFinish"))
		{
			delete getRealStatePointer( pRetState);  // safe deletion in case of memory leak
                    
			CString strTBPropValue;
			CString strTBPropName = _T("NewFLState");
			SendTBCommand(TBCMD_GET_TBPROPERTY, &strTBPropName, &strTBPropValue);
			//TAr346903+
			//if(strTBPropValue == _T("SMCashPayment"))
			CString csCurState;
			csCurState = this->GetRuntimeClass()->m_lpszClassName;
			//if we are already at CashPayment state, we don't want to return to the same state
			//because we want to have devices enabled all the time
			if(strTBPropValue == _T("SMCashPayment") && csCurState != _T("SMCashPayment"))
			{
				RETURNSTATE(CashPayment); //it is necessary because the cash is now processed immediately
			}
			//TAR346903-

			//let the TBMsgMap entry returns to SMFinish.
			//this is to prevent from double entry to SMFinish.
			return STATE_NULL;
		}
	}

	return pRetState;
}

SMStateBase * SMState::TBGetTenderDetails(void)
{
	//+SSCOI-47171
    /*return SMStateBase::TBGetTenderDetails();*/
	STenderDetails dTender;
	memset(&dTender, 0, sizeof(STenderDetails));

	rc = tb.GetTenderDetails(&dTender);
	ProcessTBReturn(rc);
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO ,
		_T("TBGetTenderDetails--TenderDetail: szDescription:%s; lTender:%d; lBalance:%d; lChange:%d; nTenderType:%d; szErrDescription:%s"),
		dTender.szDescription, dTender.lTender, dTender.lBalance, dTender.lChange, dTender.nTenderType, dTender.szErrDescription) ;

	if (dTender.lChange < 0)
	{
		// this will never happen unless TB freaks out
		trace(L0, _T("wrong lChange value:%d returned by TB, negative dispense is not possible"), dTender.lChange);
		dTender.lChange = 0;
	}
	if(!fTenderDeclined) //SSCOI-47171 || (dTender.nTenderType != nTenderType))   //If tenderdeclined, do not need get these data
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
	//-SSCOI-47171
}

SMStateBase * SMState::TBGetAvailableFoodStampAmount(void)
{
    return SMStateBase::TBGetAvailableFoodStampAmount();
}

SMStateBase * SMState::TBGetAvailableBenefitAmount(void)
{
    return SMStateBase::TBGetAvailableBenefitAmount();
}

SMStateBase * SMState::TBRequestFoodStampEligibleTotal(void)
{
    return SMStateBase::TBRequestFoodStampEligibleTotal();
}

void SMState::TBShow2x20(void)
{
    TBShow2x20(_T(""));
  //  SMStateBase::TBShow2x20();
}

void SMState::TBShow2x20(CString csOpDisplay)
{
    //Do nothing	 
	return;
//    SMStateBase::TBShow2x20(csOpDisplay);
}

CString SMState::TBName(void)
{
    return SMStateBase::TBName();
}

TBRC SMState::TBGetPINWorkingKey(CString &csAct,
                                    CString &csKey,
                                    TBTENDERTYPE nTyp,
                                    TBPINBLOCKFORMAT& nPinBlockFormat,
                                    TBPINPADSYSTEM& nPinpadSystem,
                                    TBPINENCODING& nPinBlockEncoding)
{
    return SMStateBase::TBGetPINWorkingKey(csAct, csKey, nTyp, nPinBlockFormat, nPinpadSystem, nPinBlockEncoding);
}

bool SMState::TBPromptForDebitAccountType(void)
{
    return SMStateBase::TBPromptForDebitAccountType();
}

long SMState::TBRefreshState(void)
{
    return SMStateBase::TBRefreshState();
}

long SMState::TBInitialize(HWND hwnd, LPCTSTR szFilePath)
{
    return SMStateBase::TBInitialize(hwnd, szFilePath);
}

long SMState::TBLoadOptions(void)
{
    return SMStateBase::TBLoadOptions();;
}

long SMState::TBUnInitialize(void)
{
    return SMStateBase::TBUnInitialize();
}

CString SMState::TBLookupCode(const USHORT uKey)
{
    return SMStateBase::TBLookupCode(uKey);
}

long SMState::TBGetTerminalNumber(void)
{
    return SMStateBase::TBGetTerminalNumber();
}

long SMState::TBGetCurrentTransactionNumber(void)
{
    return SMStateBase::TBGetCurrentTransactionNumber();
}

long SMState::TBGetCurrentBusinessDate(CString &csBusinessDate)
{
    return SMStateBase::TBGetCurrentBusinessDate(csBusinessDate);
}

BOOL SMState::TBValidateOperator(LPCTSTR szOper, LPCTSTR szPass)
{	
	BOOL bValidateOperator = SMStateBase::TBValidateOperator(szOper, szPass);

	//Start CRD 330783 send TLOG Override with Override=63 (RA_OPERATOROVERRIDE) for POS to pause Everseen State
	if(bValidateOperator)
	{
		trace(L6, _T("Send TBCMD_TLOG_OVERRIDE after valid operator"));
		SendTBCommand(TBCMD_TLOG_OVERRIDE, rp.GetOperator(), RA_OPERATOROVERRIDE, 1, 0, _ttol(rp.GetTerminalNumber()), 0);		//POS181201
	}
	//End CRD 330783

    return bValidateOperator;
}

HFILE SMState::TBGetSlipData(void)
{
    return SMStateBase::TBGetSlipData();
}

// +TAR 398763
// The Meijer Resume feature is optimized for quick resume & pay of trx, skipping 
// over e-receipt populating and lItems may have a value of 0 even if there are items sold
// This messes with the PrintReceiptOnCancelledTransaction functionality.
HFILE SMState::TBGetReceiptData(void)
{
    HFILE hFile = NULL;
	
    //SOTF 8180
    //do not add the receipt handle to the global array if this is a delayed receipt print
    //Otherwise we will have double receipts printed on the 5x machine
	
    //SOTF 8362 - don't hold the receipts if we are in assist mode
    //this will cause receipts not to print at the RAP
    //POS59963 - If we are in the SMSmMediaStatus, then we need to pass it through so that the cash management report receipt will be printed.
    CString csCurState;
	csCurState = this->GetRuntimeClass()->m_lpszClassName;
    int nFind = csCurState.Find(_T("SMSmMediaStatus"));
	
    trace(L6, _T("Current State (%s), Compare with SMSmMediaStatus(%d)"), csCurState, nFind);	       
	
    
	if(( GetTBProperty(_T("ReleaseReceipt")) == _T("1") ) && (!fStateAssist) && (nFind == -1) )
	{
		trace(L6,_T("TBGetReceiptData() delayed receipt "));
        return NULL;
    }
    
    //SOTF 8180
	
    // We've resumed a transaction and have not populated the e-receipt
    if ( GetTBProperty(_T("IsThereItemNeedToDisplayed")) == _T("1") && (lItems <= 0) )
    {
        long lItemsTmp = lItems;
        lItems = 1;
        hFile = SMStateBase::TBGetReceiptData();
        lItems = lItemsTmp;
    }
    else
    {
        hFile = SMStateBase::TBGetReceiptData();
    }
    return hFile;
}

HFILE SMState::TBGetJournalData(void)
{
    return SMStateBase::TBGetJournalData();
}

CString SMState::TBGetDisplay(void)
{
    return SMStateBase::TBGetDisplay();
}

void SMState::TBReleaseSlipData(HFILE hFile)
{
    SMStateBase::TBReleaseSlipData(hFile);
}

void SMState::TBReleaseReceiptData(HFILE hFile)
{
    SMStateBase::TBReleaseReceiptData(hFile);
}

void SMState::TBReleaseJournalData(HFILE hFile)
{
    SMStateBase::TBReleaseJournalData(hFile);
}

void SMState::TBLaneClosed(const bool bEnter)
{
    SMStateBase::TBLaneClosed(bEnter);
}

TBSTATE SMState::TBProcessMessage(MessageElement* me)
{
    TBSTATE sTB = SMStateBase::TBProcessMessage(me);
    if (sTB > TB_MAXSTATEID)
    {
        m_TBLastState = sTB;
        sTB = TB_IGNORE;
        m_bCustomerState = 1;
    }
    else
    {
        if ((sTB == TB_COMPLETEPRINTANDCUT) && (fInEndorsementState != ENDORSE_NONE))
        {
            // POS25386: Ignore receipt printing while a slip endorsement is occurring to prevent an out-of-sync condition. The
            // receipt printing could be due to a CM-driven auto print of a gift receipt for an item greater than X amount of
            // dollars. Note: Receipt printing will occur later during TB_TRXEND.
            trace(L3, _T("TBProcessMessage - Endorsement active (fInEndorsementState=%d) -> Ignore TB_COMPLETEPRINTANDCUT"), fInEndorsementState);
            sTB =  TB_IGNORE;
        }
        else if (sTB != TB_IGNORE)
        {
            trace(L3, _T("TBProcessMessage - TB State id=%d"), sTB);
        }
		
        m_bCustomerState = false;
        m_TBLastState = sTB;
    }
    return sTB;
}

SMStateBase * SMState::TBSetTrainingModeState(const bool bTMState)
{
    return SMStateBase::TBSetTrainingModeState(bTMState);
}

long SMState::TBSetBusinessDate(LPCTSTR szSetBusinessDate)
{
    return SMStateBase::TBSetBusinessDate(szSetBusinessDate);
}

void SMState::TBEPSEndOfDay(LPVOID lp)
{
    SMStateBase::TBEPSEndOfDay(lp);
}

void SMState::TBEPSStartOfDay(LPVOID lp)
{
    SMStateBase::TBEPSStartOfDay(lp);
}

void SMState::TBNeedStartOfDay(TBSTARTOFDAYSTATE &eState)
{
    SMStateBase::TBNeedStartOfDay(eState);
}

void SMState::ResetApprovalValues(const bool fUpdateSA)
{
    SMStateBase::ResetApprovalValues(fUpdateSA);
}

SMStateBase * SMState::OnEndorseDocument(void)
{
	long nEndorsementsPending = 0;
    if( GetHookProperty(_T("IsEBTTenderInsufficient")) == _T("1") ) 
	{
        do
		{
            LPTSTR szSlipDocumentDescription = NULL;
            LPTSTR szSlipDocumentFront = NULL;
            LPTSTR szSlipDocumentBack = NULL;
			
            TBGetDocEndorsementInfo(
                &nEndorsementsPending, &szSlipDocumentDescription,  
                &szSlipDocumentFront, &szSlipDocumentBack
                );
            if(nEndorsementsPending)
			{
				trace (L0, _T("SMState::OnEndorseDocument Printing endorsement data to a chit"));
				PrintEndorsementChit(szSlipDocumentBack);
				// Suppress further endorsement message from TB since we are doing these
				// in a loop and don't need to be event driven at this time.
				TBEndorsementCompleted(true);
			}
            delete szSlipDocumentDescription;
			delete szSlipDocumentFront;
			delete szSlipDocumentBack;
        } while (nEndorsementsPending);
        RETURNSTATE(InProgress);
	}

    return SMStateBase::OnEndorseDocument();
}

SMStateBase * SMState::OnNewCashDrawerState(void)
{
	trace (L6, _T("+SMState::OnNewCashDrawerState."));
    
    if( m_bIsNoSale )
    {
        CString csRemoteConnectionName = ra.GetRemoteConnectionName();
        
        trace (L6, _T("About to open the cash drawer..."));
        long lDrawerState;
        TBGetExpectedCashDrawerState(&lDrawerState);
        
        //SOTF 7368
        bool bDeviceError = false;
        bool bDeviceRemote = dm.IsDeviceRemote(DMCLASS_CASHDRAWER, 0);
        if (bDeviceRemote)
        {
            trace (L6, _T("Cash Drawer is remote. Checking State [%d]"), lDrawerState);
            if (lDrawerState & 1)
            {
                trace (L6, _T("Opening the Cash Drawer..."));
				
                //SOTF 7368
                long rc = dm.CashDrawerOpen();
				
                if(rc == 0)
                { 
                    trace(L7, _T("+ra.OnSetTransactionState, No Sale - CashDrawer State"));
                    ra.OnSetTransactionState(RA_STATE_CASHDRAWER_OPEN);
                    trace(L7, _T("-ra.OnSetTransactionState"));	
                }else
                {
                    bDeviceError = true;
                }
            }
            else
            {
                m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), _T("operation=assist-release;"), csRemoteConnectionName );
                
                trace(L7, _T("+ra.OnSetTransactionState, No Sale state end "));
                ra.OnSetTransactionState(RA_STATE_CASHDRAWER_CLOSE);
                trace(L7, _T("-ra.OnSetTransactionState"));
                
                trace (L6, _T("Cash drawer closed."));
                
                m_bIsNoSale = false;
                
                trace (L6, _T("Ending Remote devices.."));
                dm.EndRemote();
            }
        }
		
        //SOTF 7368
        if ((!bDeviceRemote) || (bDeviceError))
        {
            m_bIsNoSale = false;
            
            trace (L6, _T("Failed to remote the Cash Drawer. Unable to do No Sale."));
            
            trace(L7, _T("+ra.OnSetTransactionState, Cash Drawer Failed "));
            ra.OnSetTransactionState(RA_STATE_CASHDRAWER_CLOSE);
            trace(L7, _T("-ra.OnSetTransactionState"));
            
            CString csNoSale = _T("Cash Drawer Failed! Please check the devices and try again.");
            trace(L7, _T("+ra.OnNormalItem, Message=%s"), csNoSale);
            ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csNoSale);
            trace(L7, _T("-ra.OnNormalItem")); 
            
            SendTBCommand(TBCMD_CASHDRAWER_FAILED, NULL );
            
            dm.EndRemote();
            m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), _T("operation=assist-release;"), csRemoteConnectionName );
        }
    }
    trace (L6, _T("-SMState::OnNewCashDrawerState."));
    return SMStateBase::OnNewCashDrawerState();
}

SMStateBase * SMState::OnNeedMICR(void)
{
    return SMStateBase::OnNeedMICR();
}

SMStateBase * SMState::OnNewSecurityLevel(void)
{
    return SMStateBase::OnNewSecurityLevel();
}

SMStateBase * SMState::OnAssistModeStatusChanged(void)
{
    return SMStateBase::OnAssistModeStatusChanged();
}

SMStateBase * SMState::OnTBNeedData(void)
{
    return SMStateBase::OnTBNeedData();
}

long SMState::TBLogReceipt(LPTSTR* lpFileName,BSTR* bstrSignatureString, LPTSTR* lpReceiptType)
{
    return SMStateBase::TBLogReceipt(lpFileName, bstrSignatureString, lpReceiptType);
}

long SMState::TBPreprocessSignatureData(LPCTSTR lpFileName,BSTR bstrSignatureData, LPCTSTR lpReceiptType, BOOL* bProcessData)
{
    return SMStateBase::TBPreprocessSignatureData(lpFileName, bstrSignatureData, lpReceiptType, bProcessData);
}

long SMState::TBGetGiftCardBalance(long *plBalance)
{
    return SMStateBase::TBGetGiftCardBalance(plBalance);
}

SMStateBase * SMState::HandleOutOfSyncStop()
{
    return SMStateBase::HandleOutOfSyncStop();
}

SMStateBase * SMState::HandleOutOfSyncAssist()
{
    return SMStateBase::HandleOutOfSyncAssist();
}

void SMState::TBSyncTBorPOS()
{
    SMStateBase::TBSyncTBorPOS();
}

SMStateBase * SMState::getRealStatePointer(SMStateBase *pState)
{
    return SMStateBase::getRealStatePointer(pState);
}

bool SMState::AllowImmediateReceiptPrint()
{
    return SMStateBase::AllowImmediateReceiptPrint();
}

void SMState::EndOfTransactionPrint()
{
	trace(L6, _T("+SMStateTB::EndOfTransactionPrint"));

	if (AllowImmediateReceiptPrint())
	{
		m_bEOTPrintPending = false;
		m_bForceReceiptPrint = false;

		HFILE hFile;

		hFile = TBGetReceiptData();

		bool bDupRec = false;

		trace(L6, _T("EndOfTransactionPrint has hReceiptFile %d "), hReceiptFile);
		trace(L6, _T("EndOfTransactionPrint has hFile %d "), hFile);

		if ((hFile != NULL) && (hFile == hReceiptFile))
		{
			bDupRec = true;
			trace(L6, _T("TBEndOfTransaction:: Current printed file the same, not printing duplicate"));
		}

		if ((hFile != NULL) && (!bDupRec))
		{
			if (NULL != hReceiptFile)
			{
				trace(L6, _T("ASYNCHRONOUS PRINT ISSUED, POSSIBLE RECEIPT FILE HANDLE LEAK"));
			}
			hReceiptFile = hFile;
			PrintReceipt(hFile);	// print receipt
			if (nTrxReceiptLength)
			{
				if (co.fOperationsAddPaperCutToTransReceipt)
				{
					trace(L6, _T("Adding Papercut to receipt"));
					CutReceipt();		// cut receipt
				}
			}
		}

		// Journal Printer
		hFile = TBGetJournalData();   // print the journal last
		if (hFile != NULL)
		{
			hJournalFile = hFile;
			PrintJournal(hFile);
			TBReleaseJournalData(hFile);
		}

	}
	else
	{
		m_bEOTPrintPending = true;
	}

	trace(L6, _T("-SMStateTB::EndOfTransactionPrint"));
}

bool SMState::CanSuspendTransaction()
{
    bool bRet = true;
    if ( 
        // Nothing has been sold:
        ( !isAnySoldVoidedItems() && co.fOperationsSuspend ) || 
        // TB Is not in a transaction:
        ( !fInSignedOn ) || //TB not in tranx
        // Tranx partially tendered, but config option not set to allow suspend if tranx is partially tendered:
        ( (short(lTotalPaid) != nDMCashInDrawer) && (!co.fOperationsAllowSuspendIfPartiallyTendered) ) ||
        // TB Does not allow suspend
        ( !TBIsAllowed(TB_FT_SUSPEND) ) 
        )
    {
        bRet = false;
    }
    trace(L6, _T("SMState::CanSuspendTransaction() == %d"), bRet);
    return bRet;
}

CString SMState::GetHookProperty(CString csProp)
{
    CString csValue = _T("");
    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csProp, &csValue);
    return csValue;
}

CString SMState::GetTBProperty(CString csProp)
{
    CString csValue = _T("");
    SendTBCommand(TBCMD_GET_TBPROPERTY, &csProp, &csValue);
    return csValue;
}

void SMState::SetHookProperty(CString csProp, CString csValue)
{
    trace(L6, _T("SetHookProperty: %s = <%s>"), csProp, csValue); // POS18041
    SendTBCommand(TBCMD_SET_HOOKPROPERTY, &csProp, &csValue);
}

void SMState::SetTBProperty(CString csProp, CString csValue)
{
    trace(L6, _T("SetTBProperty: %s = <%s>"), csProp, csValue); // POS18041
    SendTBCommand(TBCMD_SET_TBPROPERTY, &csProp, &csValue);
}

void SMState::TBAddMessagetoReceipt()
{
    SMessageInfo sTempMessageInfo;
    memset(&sTempMessageInfo, 0, sizeof(SMessageInfo));  
    
    ::TBGetMessageInfo(&sTempMessageInfo);
    //tb.GetMessageInfo(&sTempMessageInfo);
    //ProcessTBReturn(rc);
    
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
    CustomMessageObj.csRAP_MessageText	= sTempMessageInfo.szRAP_MessageText;
    CustomMessageObj.bHiliteRAPText     = sTempMessageInfo.bHiliteRAPText;
	CustomMessageObj.bNeedItemCancel = sTempMessageInfo.bNeedItemCancel;
	CustomMessageObj.bGoBackScanAndBag = sTempMessageInfo.bGoBackScanAndBag;	
    
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
   	
    //*** NewUI  ***//
    // Add text to receipt if needed.
    if(CustomMessageObj.csE_ReceiptText.GetLength())
    {
        COleVariant vValue;
        CString csMsgItemEntryId, csReceiptSrc;
        
        csReceiptSrc = _T("CMSMReceipt");
        
        CString csItemEntryId= ps.GetSelection(_T("CMReceipt")); // Get the item's entry id
        
        csMsgItemEntryId	= csItemEntryId + _T("WarrantySerialNumber");
        
        m_pAppPSX->CreateReceiptItem(csReceiptSrc, csMsgItemEntryId, csItemEntryId);
        vValue= CustomMessageObj.csE_ReceiptText;
        m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csMsgItemEntryId, _T("ITEM_DESCRIPTION"), vValue);
        vValue = VARIANT_FALSE;
        m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csMsgItemEntryId, _T("ITEM_SELECTABLE"), vValue);
        m_pAppPSX->UpdateReceiptControls(csReceiptSrc);  //update receipt display
    }
    //*** end of NewUI ***//
}

bool SMState::IsCardTender(int nTenderType)
{
	return SMStateBase::IsCardTender(nTenderType);
}
