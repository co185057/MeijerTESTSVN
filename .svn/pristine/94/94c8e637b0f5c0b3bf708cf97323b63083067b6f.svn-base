//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMState.CPP
//
// TITLE: Class implementation file for base class SMState
//
// All states ultimately inherit from SMState.
// This class is an abstract class (it has at least one
// pure virtual function and can't be instantiated.)
//
// This class provides default processing for
// all possible messages to be parsed. Message
// handlers are usually implemented in deriving state
// but if not they end up here and are usually
// ignored.
//
// The functions in SMState are implemented here and in
// SMStatePS.cpp, SMStateDM.cpp, SMStateTB.cpp and SMStateSA.cpp
//
//
// AUTHOR:  Peter Denhoed, Updated DcL-
//
// CHANGE HISTORY
//
// CRD 537490 Name: CJ Estrada Date: Mar 17, 2021
// CRD 564271 Name: CJ Estrada Date: Mar 9, 2021
// CRD 537481 Name: CJ Estrada Date: Mar 5, 2021
// CRD 557225 Name: CJ Estrada Date: Mar 4, 2021
// CRD 574057 Name: CJ Estrada Date: Feb 23, 2021
// CRD 554916 Name: CJ Estrada Date: Feb 9, 2021
// CRD 558065 Name: CJ Estrada Date: Jan 14, 2021
// CRD 541144 Name: CJ Estrada Date: Jan 7, 2021
// CRD 517533 Name: Dennis Michol Gabutin Date: November 17, 2020
// CRD 503257 Name: CJ Estrada Date: Oct 2, 2020
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"

#include "SMAttract.h"
#include "SMScanAndBag.h"
#include "transbrokerssf.h"

#include "CMInterface.h"

#include "IQueuedAction.h"
#include "QueuedActionGenericDelayedIntervention.h"

#include "FLTCPDefines.h"
#include "TCPConnectConstants.h"
#include "LaneStateData.h"
#include "SMSmGenericDelayed.h"
#include "IQueuedAction.h"
#include "DataNeededUtil.h"
#include "QueuedActionGenericDelayedIntervention.h"
#include "QueuedActionShowMessageScreen.h"
#include "SMFinish.h"
#include "MobileCartAudit.h"
#include "TCPConnectConstantsSSF.h"
#include "PSConfigData.h"
#include "SMRapDataNeeded.h"
#include "SMProcessing.h"	// CRD 554916

#include "SMRecallingItems.h" // RFQ 9545
#include "MobileCartAudit.h"  // CDR126194

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SMState")

//#ifdef _SSCOMOBILE
CMobileCart myCMobileCart;
DLLEXPORT CMobileCart &mobileCart = myCMobileCart;
CTCPWebConnect myTCPConnect;
DLLEXPORT CTCPWebConnect &tcpConnect = myTCPConnect;


bool SMState::m_bResendFLMSitem = false; // RFQ 9545 need to resend FLMS item
//#endif _SSCOMOBILE
_bstr_t SMState::m_bstrTransactionStatus = _T("");
SCOTSSFErrorObject eoSSF(_T("SCOTSSF"));

long  SMState::m_lSavedforMobileTotal = 0;
long  SMState::m_lSavedforMobileTransNumber = 0;

bool SMState::m_bNeedImmediateApproval = false;
bool SMState::m_bMJPlayedWelcome = false;
bool SMState::m_bMJPlayedUsePinpad = false;
bool SMState::m_bNeedScanReceipt = false;
bool SMState::m_bIsFromScanReceipt = false;
bool SMState::m_bIsNewTrxSelected = false;
bool SMState::m_bIsFromPaymentGoback = false;
bool SMState::m_bIsFromDisplayingResumeItem = false;
long SMState::m_lLastSuspendTotal = 0;
bool SMState::m_bMJWtIncreaseAtSmCmDataEntry= false;        //TAR335785
bool SMState::m_bIsNoSale = false;
bool SMState::m_bIsSuspendFromRap = false;
long SMState::m_lUnApprovedRestrictedItemsDuringCancel = 0;
bool SMState::m_bDataNeededAssistMode = false;              //SOTF 6583 
bool SMState::m_bGoBackAfterAgeRestrictedItem = false;      //SOTF 7384
bool SMState::m_bCouponInsertionBypassed = false;           //POS33020
long SMState::m_lInsertedCouponsTotal = 0;                  //POS33020
bool SMState::m_bActivateHHScanner = false;		// POS181195/POS181197

bool SMState::m_bNeedYellowLightForMobile = false;  //POS31054
bool SMState::m_bWICEBTSNBEnable = true;	// POS138636
long SMState::lWICOptionloaded = 0;	// POS138637
BEST SMState::nPreviousState = BES_LASTSTATE; //SSCOI-45180
bool SMState::bVoidApprovalDelayed = false;  //VSTS 131568

bool SMState::m_bEODEnable = false; //POS97087

bool SMState::m_bSecurityDisabledForMPerks = false;

int SMState::nVoidCounts = 0;   //VSTS 131568
bool SMState::m_bCustomVoidItemMessage = false; //VSTS 131568
bool SMState::mPerksActivatedInTransaction = false;

bool SMState::bRAPPrinterBitmapStatus = false;// Lane starts with assuming bitmap was loaded at RAP printer. If required, RAP alerts the lane explicitly when needed to load the bitmap
bool SMState::bSCOPrinterBitmapStatus = false;
CString SMState::csMeijerLogoFilePath = _T("");

bool SMState::m_bAgeItemSoldInAM=false; //SSCOI-47774, flag for restricted item sold in AM

bool SMState::m_bShopAndDropEnabled = false;	// CRD 125377
bool SMState::m_bSNDmPerkEntered = false;		// CRD 125377
bool SMState::m_bSNDTMDEntered = false;			// CRD 125377
bool SMState::m_bSNDAgeEntered = false;			// CRD 125377
bool SMState::m_bSNDAgeEnteredFound = false;	// CRD 125377
bool SMState::m_bSNDAllowItemMenu = false;		// CRD 125377
bool SMState::m_bSNDClearUnknownItemC = false;	// CRD 125377
bool SMState::m_bSNDmPerksCancel = false;		// CRD 125377
bool SMState::m_bSNDExitedAM = false;			// CRD 125377
bool SMState::m_bSNDSettingAge = false;			// CRD 125377
bool SMState::m_bSNDxCancelTrans = false;		// CRD 125377
bool SMState::m_bSNDBackToFnP = false;			// CRD 125377
long SMState::lSDNRestrictedAgeTimerAmt = 5000; // CRD 125377

CString SMState::m_csTransactionMode= _T("");	// CRD126015 transaction mode
bool SMState::bFinishRecall=false;	//SSCOI-55092
bool SMState::bIsItemizationAfterSkipBaggingSent=false;	//SSCOI-55023
bool SMState::m_bIsLoyaltyProcessed = false; //154609
bool SMState::bQuantityRequiredActive = false; // 140001
bool SMState::m_bTMDAccepted=false; //CRD 168992
bool SMState::m_bMobileTransactionStarted=false; //VSTS170002

bool SMState::bTMDVoided = false;
bool SMState::bHighValueActive=false;


bool SMState::bDoNotSendStatusRequest=false;	//CRD 189076
//Start CRD 185620
CMap <int, int, bool, bool> SMState::g_mapTransactionEASItem;	//CRD 142210
long SMState::lUnapprovedEASItems = 0;
bool SMState::bEASApproveFromVoid = false;
EAS_ITEMS SMState::sEASItem;
bool SMState::isPlayedAssistanceAudio = false;
//End CRD 185620
bool SMState::bFinishSendingSuspendBarcode = false;	//CRD 177093
long SMState::lUnapprovedMobileGenericDelayed = 0;	 //CRD 223409
bool SMState::bIsGenericDelayedProcessed = false;	//CRD 223409
long SMState::sProcessingDelayedIntervention = DELAYED_NONE;
bool SMState::bGenericDelayedFinishSent = false;	//CRD 232271
bool SMState::g_bIsProcessingEverseenAlert = false;	//CRD 347846

CString SMState::SS90SupportMeijerCom = _T("");
CString SMState::ShiptBarcodePrefix = _T("");
CString SMState::ShiptBarcodeSuffix = _T("");
bool SMState::bResumeCompleteWaitingForItemMenu = false;	//CRD 503257

// (+) POS244229
CMap<INT, INT &, INT, INT &> cmCashCounts_Coin;
CMap<INT, INT &, INT, INT &> cmCashCounts_Bill;
CMap<INT, INT &, INT, INT &> cmAcceptCounts_Coin;
CMap<INT, INT &, INT, INT &> cmAcceptCounts_Bill;
CArray<INT,INT> caCoinList;
CArray<INT,INT> caCashList;
// (-) POS244229

CString SMState::csApplyMeijerCardGUID = _T("");	//CRD 525312
CITIACCOUNTDETAILS SMState::sCitiAccountDetails;	//CRD 558065
bool SMState::bUnapprovedAccountLookupFailed = false;	// CRD 554916
bool SMState::validatingMeijerCardWebApp = false;	//CRD 574057
bool SMState::bApplyMeijerStartedFromAttract = false;	//CRD 537481

DLLEXPORT CMobileCartAudit mobileAudit;    // end CDR126194


#define RA_CAT_SCANNER_ACTIVATED DATANEEDED_LASTVIEW+2 //POS181201
#define INFO_BUFFER_SIZE 32767 // POS181195/POS181197


IMPLEMENT_DYNCREATE(SMState, CObject)// MFC Runtime class/object information

DLLEXPORT SMState *GetStartState(void)  //Get the start screen. The default screen is SMAttract
{
  CRuntimeClass* pStateAttract = RUNTIME_CLASS(SMAttract);
  CObject* pSMState = pStateAttract->CreateObject();
  ASSERT( pSMState->IsKindOf( RUNTIME_CLASS(SMAttract) ) );
  return (SMState*) pSMState;
}

CString SMState::ms_MobileAuditDataEntryINI = _T("");			// CDR126194
CString SMState::ms_MobileAuditDataEntryINISection = _T("");	// CDR126194

//Start CRD 145555
CString SMState::csFLMSType = _T("");
CString SMState::csFLMSAuditResult = _T("");
CString SMState::csMinimumScanRequired = _T("");
CString SMState::csFLMSAuditStartTime = _T("");
CString SMState::csFLMSAuditEndTime = _T("");
CString SMState::csFLMSOperatorID = _T("");
CString SMState::csFLMSSessionID = _T("");
CString SMState::csFLMSmPerksBarcode = _T("");
CArray <AUDIT_ITEMS*, AUDIT_ITEMS*> SMState::csFLMSAuditItemList;	//CRD 145557
//End CRD 145555

CTime SMState::StoreAssistRequestedTime = NULL;
//
//
// IMPORTANT CONSIDERATIONS FOR INTERMEDIATE AND BASE SSF STATES!
//
// 
//  You cannot add new VIRTUAL methods/functions to any
//    intermediate or base state
//
// �  You cannot add new non-static properties/data members
//    to any intermediate or base state
//
// �  You can override existing virtual methods using an
//    identical prototype
//
// �  You can add new non-virtual methods
//
// �  You can add new static properties/data members.
//
// The reason for these restrictions is that the core is pre-built
// using a set of default intermediate and base states.  At the
// time the code is built, the virtual method/function entry point
// table becomes fixed and the space for non-static properties/data
// members becomes fixed.  Any attempt to extend the virtual table
// or properties in SSF DLL will compile and will link but the
// resulting tables will not mesh with the pre-built tables in
// the core EXE.  This causes unpredictable serious errors at run
// time, usually access violations or application hangs. 


/////////////////////////////////////////
// Constructor
/////////////////////////////////////////
SMState::SMState()
: SMStateBase()
{
			 

}

#ifdef _TRADESHOWDEMO
#include "..\TradeShowDemo\SMStateTSDemo.cpp"
#endif

#ifdef _TESTSSF
const TCHAR SCOTAPPSHOPPINGCARD[] = _T("^SCOTAPP SHOPPING CARD^"); //gift card key
///////////////////////////////////////////////////////////////////////////////
// Function:    IsCardDispenserOK
// Description: Check the health of card dispenser. 
// Return Value: bool - true  - if card dispense is ok
//						false - if not ok
///////////////////////////////////////////////////////////////////////////////////
bool SMState::IsCardDispenserOK() 
{
	long lCountBin,lCountReject;
	bool bReturn = true;
	dm.GetAdjustCardDispenserBinCounts(lCountBin, lCountReject, 0);
	if (dm.GetCardDispenserEmpty() || dm.GetCardDispenserGateOpen () \
		|| lCountReject == co.getDispenserRejectBinFull() )
	{
		bReturn = false;
	}
	return bReturn;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    GetAccountNumber
// Description: Extract the account number from m_csGiftCardTrackData
// Return Value: CString - accout number
///////////////////////////////////////////////////////////////////////////////////
CString SMState::GetAccountNumber() 
{
	CString csRet;
	int nLastCaret = m_csGiftCardTrackData.Find(_T("^"));
	csRet = m_csGiftCardTrackData.Mid(1, (nLastCaret - 1) );	
	return csRet;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    IsValidShoppingCard
// Description: Validate if card inserted is a valid gift card
// Paremeter:   [out] CString& csCardData - Card Data
// Return Value: bool - true if valid gift card
///////////////////////////////////////////////////////////////////////////////////
bool SMState::IsValidShoppingCard(CString& cspCardData) 
{
	trace(L6, _T("+SMState::IsValidShoppingCard"));
	cspCardData = _T("");
	//+codes here are copied from SMStateBase::GetAccountNum
	//use LPTSTR SMStateBase::cDMLastTrack to get the track data
	//get track and validate
	short int nT1, nT2, nT3, nHeaderLen;
	short int nShortLen = sizeof(short);

    nT1 = nT2 = nT3 = 0;
	nT1 = (short ) *(  (const char *)cDMLastTrack);						//length of Track 1
	nT2 = (short ) *( ((const char *)cDMLastTrack) + nShortLen);		//length of Track 2
	nT3 = (short ) *( ((const char *)cDMLastTrack) + (nShortLen * 2));	//length of Track 3
  	nHeaderLen = nT1 + nT2 + nT3;

	LPTSTR szTrackData = new _TCHAR[nHeaderLen+1];
	memcpy(szTrackData, ((const char *)cDMLastTrack + (3*nShortLen)), nHeaderLen*sizeof(_TCHAR));
	szTrackData[nHeaderLen] = '\0';
	cspCardData = szTrackData;
	delete[] szTrackData;
	//-codes here are copied from SMStateBase::GetAccountNum
	trace(L6, _T("-SMState::IsValidShoppingCard = %s"),cspCardData );
	return (cspCardData.Find(SCOTAPPSHOPPINGCARD) > 0 ? true:false);
}
///////////////////////////////////////////////////////////////////////////////
// Function:    WriteGiftCard
// Description: Create Gift Card
// Paremeter:   [in] bNewCard - true if create new card
//							  - false if update existing card	
// Return Value: bool - true if write to card dispenser is successfull
///////////////////////////////////////////////////////////////////////////////////
bool SMState::WriteGiftCard( bool bNewCard ) 
{
	bool bReturn = false;
	CString csAccountNo, csAmount;
	//create new account number if true
	if (  bNewCard )
	{
		COleDateTime dt = COleDateTime::GetCurrentTime();
		csAccountNo.Format( _T("%s%s"), dt.Format(_T("%m%d%y%H%M%S")), _T("0000") );
	}else
	{
		csAccountNo = GetAccountNumber();
		//+extract current card amount and add the amount selected from the shopping card
		int nLastCaretLocation = m_csGiftCardTrackData.Find(_T("^"));
		nLastCaretLocation = m_csGiftCardTrackData.Find(_T("^"), (nLastCaretLocation + 1));
		m_SSFMoneyCenterGiftCardAmount += (_ttoi(m_csGiftCardTrackData.Mid((nLastCaretLocation + 1), 7))/100);
		//-extract current card amount and add the amount selected from the shopping card
	}
	//format amount;
	csAmount.Format(_T("%07d"), (m_SSFMoneyCenterGiftCardAmount * 100));
	//Track 1: Bmmddyyhhmmss0000^SCOTAPP SHOPPING CARD^aaaaaaa
	//Track 2:mmddyyhhmmss0000=aaaaaaa
	//Where mmddyy is the month day year, hhmmss is the hour minute and second 
	//and aaaaaaa is the current value of the card in dollars and cents 
	//(so $25 would have a value of 0002500 for this field).  

	CString csTrack1, csTrack2;
	csTrack1.Format(_T("B%s%s%s"), csAccountNo,  SCOTAPPSHOPPINGCARD,  csAmount); 
	csTrack2.Format(_T("%s=%s"),csAccountNo, csAmount);
	m_csGiftCardTrackData.Format(_T("%s%s"), csTrack1, csTrack2); 
	if (dm.CardDispenserWrite( DM_MSR_TRACK_1 | DM_MSR_TRACK_2, csTrack1, csTrack2, _T("")) )
	{
		bReturn = true;
	}
	return bReturn;
}
#endif _TESTSSF



BOOL SMState::SCOTDeleteFile(LPCTSTR lpSourceFileName)
{
    return SMStateBase::SCOTDeleteFile(lpSourceFileName);
}

void SMState::SaveLastReceiptData(HFILE hFile, CString csFileName)
{
    SMStateBase::SaveLastReceiptData(hFile, csFileName);
}

CString SMState::GetLastBarcode()
{
	return csDMLastBarCode;
}

CString SMState::GetLastBarLabel()
{
	return csDMLastBarLabel;
}

CString SMState::GetLastBarType()
{
	return csDMLastBarType;
}

void SMState::SaveLastReceiptData(CString csFileName, CString csData)
{
    SMStateBase::SaveLastReceiptData(csFileName, csData);
}

void SMState::PrintLastReceipt(CString csReceiptFileName)
{
    SMStateBase::PrintLastReceipt(csReceiptFileName);
}

SMStateBase * SMState::PerformEOD()
{
	m_bEODEnable = true;
    return SMStateBase::PerformEOD();
}

SMStateBase * SMState::Initialize(void)
{
    return SMStateBase::Initialize();
}

void SMState::UnInitialize(void)
{
    SMStateBase::UnInitialize();
}

SMStateBase * SMState::TimedOut(void)
{
    return SMStateBase::TimedOut();
}

bool SMState::customerMode()
{
    return SMStateBase::customerMode();
}

bool SMState::storeMode()
{
    return SMStateBase::storeMode();
}

bool SMState::helpMode()
{
    return SMStateBase::helpMode();
}

bool SMState::assistMode()
{
    return SMStateBase::assistMode();
}

bool SMState::storeModeHelp()
{
    return SMStateBase::storeModeHelp();
}

bool SMState::inProgress()
{
    return SMStateBase::inProgress();
}

SMStateBase * SMState::Parse(MessageElement *me)
{
    //nTimeIdleMsec = 0;
    nLastMsgID = me->id;
	
    if (me->id == ID_GP)
    {
        //SSCOMobile
        if (GP_TCP_RECEIVED == me->gpInfo.nEvt ||
            GP_TCP_DISCONNECTED == me->gpInfo.nEvt ||
            GP_TCP_CONNECTED == me->gpInfo.nEvt)
        {
            return ParseMobileSSF(me);  //6.0.5 ParseMobile not virtual,  fixed in later version.
        }
    }

    return SMStateBase::Parse(me);
}


SMStateBase * SMState::ParseMobileSSF(MessageElement *me)
{
    trace(L6, _T("+SMStateBase::ParseMobile"), g_bInRecallMode);
    switch (me->id)
    {
      case ID_GP:                      // general purpose notify
        nTimeIdleMsec = 0;
        if(me->gpInfo.nEvt==GP_TCP_RECEIVED)
        {          
            BYTE* pMsg = (BYTE*) me->psInfo.lParm;
            BYTE* pMsgData = &pMsg[sizeof(structMsgHeader)];
            structMsgHeader* pHeader = (structMsgHeader*) pMsg;
            _bstr_t bstrMessage = _T("");
            if (pHeader->sAppID == TCP_APPID_MOBILECART && pHeader->lAppData==TCP_MOBILECARTMSG_FAILURE_REPORT)
            {
                bstrMessage = (LPCWSTR) pMsgData;
                delete [] pMsg;
                me->psInfo.lParm = NULL; 
                return OnMobileErrorEvent((LPCTSTR) bstrMessage);
            }
            else if (pHeader->sAppID == TCP_MOBILECARTMSG_GETLASTSTATUS)
            {
                mobileCart.SendTransactionStatus(GetTransactionStatus());
            }
        }
    }// of switch
    return ParseMobile(me);
}

SMStateBase * SMState::OnMobileErrorEvent(CString csMessage)
{
    CString csViewString = _T("");
    CString csSection = csMessage;   //This may have more info in it - need info from Basab.
    CString csStateName = getStateName(this);
    
    SCOTDATAENTRY cDataEntry;
    if(csMessage.Find(_T("MobileTransferFailed")) != -1)
    {
        rp.TransactionStart(rp.m_csBusinessDate);
        cDataEntry.FromConfigData(ms_MobileAuditDataEntryINI, _T("MobileTransferFailure"), false);
    }
    else if (csMessage.Find(_T("MobileUploadFailed")) != -1)
    {
        rp.TransactionStart(rp.m_csBusinessDate);
        cDataEntry.FromConfigData(ms_MobileAuditDataEntryINI, _T("MobileUploadFailure"), false);
    }
    csViewString = cDataEntry.ToString();
    if (csStateName.Find(_T("SMAttract")) != -1)
    {
        CREATE_AND_DISPENSE(RAPDataNeeded)(cDataEntry.ExtractString(_T("InterventionInstruction")) ,DATANEEDED_MOREDATA, csViewString, storeMode());
    }


    return STATE_NULL;
}
/*
SMStateBase * SMState::GenerateExternalIntervention(CString csViewString)
{
    
}
*/
//UploadTicket Port -

SMStateBase * SMState::IdleTimer(const long lMsec)
{
    return SMStateBase::IdleTimer(lMsec);
}

CString SMState::CleanUpMessage(LPCTSTR szIn, const bool f2x20)
{
    return SMStateBase::CleanUpMessage(szIn, f2x20);
}

void SMState::GetTrxLimitString(const PSTEXT nTextID, CString& csMsgText)
{
    SMStateBase::GetTrxLimitString(nTextID, csMsgText);
}

bool SMState::restrictedItemNotAllowed()
{
    return SMStateBase::restrictedItemNotAllowed();
}

PSTEXT SMState::restrictedItemMessage()
{
    return SMStateBase::restrictedItemMessage();
}

long SMState::CopyFileFromServer(LPCTSTR szFileName, LPCTSTR szDestinationFilePath, const bool fForce)
{
    return SMStateBase::CopyFileFromServer(szFileName, szDestinationFilePath, fForce);
}

long SMState::CopyFileToServer(LPCTSTR szFileName, LPCTSTR szSourceFilePath)
{
    return SMStateBase::CopyFileToServer(szFileName, szSourceFilePath);
}

SMStateBase * SMState::createLevel1Message( LPCTSTR szMsg,
                                           PSTEXT pstMsg,
                                           PSTEXT prompt,
                                           long   value,
                                           long   device,
                                           bool   checkHealth,
                                           bool displayHTML)
{
    return SMStateBase::createLevel1Message( szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML);
}

SMStateBase * SMState::createLevel2Message( LPCTSTR szMsg,
                                           PSTEXT pstMsg,
                                           PSTEXT prompt,
                                           long   value,
                                           long   device,
                                           bool   checkHealth,
                                           bool displayHTML)
{
    return SMStateBase::createLevel2Message( szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML);
}

SMStateBase * SMState::createLevel3Message( LPCTSTR szMsg,
                                           PSTEXT pstMsg,
                                           PSTEXT prompt,
                                           long   value,
                                           long   device,
                                           bool   checkHealth, 
                                           bool displayHTML )
{
    return SMStateBase::createLevel3Message( szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML );
}

SMStateBase * SMState::DMdf_EASNoMotion()
{
    return SMStateBase::DMdf_EASNoMotion();
}

SMStateBase * SMState::PushStateFifoStack(SMStateBase *sPush)
{
    return SMStateBase::PushStateFifoStack(sPush);
}

SMStateBase * SMState::OnCouponSensor()
{
    return SMStateBase::OnCouponSensor();
}

SMStateBase * SMState::OnMotionCashOutSensor()
{
    return SMStateBase::OnMotionCashOutSensor();
}

SMStateBase * SMState::OnNoMotionCashOutSensor()
{
    return SMStateBase::OnNoMotionCashOutSensor();
}

SMStateBase * SMState::GenerateTrainModeIntervention(void)
{
    return SMStateBase::GenerateTrainModeIntervention();
}

bool SMState::InvalidCouponPrefix(CString& csItemSent)
{
    return SMStateBase::InvalidCouponPrefix(csItemSent);
}

void SMState::SynchronizeTime()
{
    SMStateBase::SynchronizeTime();
}

bool SMState::ShrinkPicklistImages()
{
    return SMStateBase::ShrinkPicklistImages();
}

CString SMState::GetWideCharToMultiByte(const CString &csData)
{
    return SMStateBase::GetWideCharToMultiByte(csData);
}

SMStateBase * SMState::ReloadOptions(void)
{
	SMStateBase* retState = STATE_NULL;			
	bool IsRequestedFromStoreClose = ps.GetCurrentContext() == _T("SmAssistMode");
	COleVariant v;
	if(IsRequestedFromStoreClose)
	{
		v = (long) UI::Disabled;
		m_pAppPSX->SetConfigProperty(_T("SMButton1"), _T("SmAssistMode"), UI::PROPERTYSTATE, v);//SmAssistMode
		ps.SetLeadthruText(TXT_PLEASEWAIT);
		ps.SetTitle(_T(""));
	}
	// start CRD 125377
	// start CRD126015
	CString strOptionName = _T("TransactionMode");
    CString m_csFLConfig;
    SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &m_csFLConfig);
  
	m_bShopAndDropEnabled = false;
	SetHookProperty(_T("IsSND"),_T("0")); //CRD 157372
	if (m_csFLConfig == _T("1"))
		m_bShopAndDropEnabled = true; 
		SetHookProperty(_T("IsSND"),_T("1")); //CRD 157372
	// end CRD126015
	
	trace(L6,_T("m_bShopAndDropEnabled %d m_csFLConfig %s"),m_bShopAndDropEnabled, m_csFLConfig);
	// end CRD 125377
    retState = SMStateBase::ReloadOptions();

	if(IsRequestedFromStoreClose)
	{
		v = (long) UI::Normal;
		m_pAppPSX->SetConfigProperty(_T("SMButton1"), _T("SmAssistMode"), UI::PROPERTYSTATE, v);	
		ps.SetLeadthruText(_T("POS signed out"));		
	}
	
	LoadEverseenOptions();	//CRD 313028
	
	return retState;
}

/*long SMState::DisplayAuxScreen(SCOT_AUX_SCREEN eScreen, bool bForceRefresh, int nArgs, ...)
{
    return SMStateBase::DisplayAuxScreen(eScreen, bForceRefresh, nArgs , ...);
}*/

SMStateBase * SMState::VerifyBirthDate(void)
{
    return SMStateBase::VerifyBirthDate();
}

BOOL SMState::VerifyBirthDate(CString csFromAnyDOB, CString csoperatorID, CString cspassword)
{
    return SMStateBase::VerifyBirthDate(csFromAnyDOB, csoperatorID, cspassword);
}

long SMState::GetAge(CString csFromAnyDOB)
{
    return SMStateBase::GetAge(csFromAnyDOB);
}

SMStateBase * SMState::SAEndOfTransaction(void)
{
    return SMStateBase::SAEndOfTransaction();
}

long SMState::ReloadSASIActions(void)
{
    return SMStateBase::ReloadSASIActions();
}

SMStateBase * SMState::CheckForDATFile(bool bReloadOption)
{
    return SMStateBase::CheckForDATFile( bReloadOption);
}

SMStateBase * SMState::OnMatchedWt(void)
{
	CString strOptionName = _T("FastLaneConfiguration");
    CString m_csFLConfig;
    SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &m_csFLConfig);
    //TAR335351
    if ( m_csFLConfig == _T("SuperMini") )
    {
        return STATE_NULL;
    }
	    return SMStateBase::OnMatchedWt();
}

SMStateBase * SMState::OnWtIncreaseNotAllowed(void)
{
    return SMStateBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMState::OnWtDecrease(void)
{
    return SMStateBase::OnWtDecrease();
}

SMStateBase * SMState::OnBackToLGW(void)
{
    return SMStateBase::OnBackToLGW();
}

SMStateBase * SMState::OnSecurityScaleFailure(int nFailureType)
{
    return SMStateBase::OnSecurityScaleFailure( nFailureType);
}

SMStateBase * SMState::OnDBMgrFailure(long lStatusCode, long lStatusType)
{
    return SMStateBase::OnDBMgrFailure( lStatusCode,  lStatusType);
}

SMStateBase * SMState::OnUnDeActivatedTheftTag(void)
{
    return SMStateBase::OnUnDeActivatedTheftTag();
}

SMStateBase * SMState::OnSkipBaggingAllowed(void)
{
    return SMStateBase::OnSkipBaggingAllowed();
}

SMStateBase * SMState::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
    return SMStateBase::OnRAPClearSecurityBtn( lpszAuthorizationType);
}

SMStateBase * SMState::OnRAPApproveAndUpdateBtn(void)
{
    return SMStateBase::OnRAPApproveAndUpdateBtn();
}

SMStateBase * SMState::OnRAPApproveNoUpdateBtn(void)
{
    return SMStateBase::OnRAPApproveNoUpdateBtn();
}

SMStateBase * SMState::OnRAPLightItemBtn(void)
{
    return SMStateBase::OnRAPLightItemBtn();
}

SMStateBase * SMState::OnRAPItemNotBaggedBtn(void)
{
    return SMStateBase::OnRAPItemNotBaggedBtn();
}

/////////////////////////////////////////////////////////
//Retrieving the Description from View for Reporting Purposes
/////////////////////////////////////////////////////////
CString SMState::GetDescFromView(int nView)
{
    trace(L7, _T("+SMState::GetDescFromView"));
    CString csView = EMPTY_STRING;
    
    if( nView == DATANEEDED_MOREDATA )
    {
        //Report Descriptions are always captured in Primary language
        SCOT_LANGUAGE_TYPE customerLanguage = ps.GetLanguage();
        ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
        
        CString csTBState(_T(""));
        SendTBCommand(TBCMD_GET_TB_STATE, &csTBState);
        
        CString csPOSState(_T(""));
        SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);
        
        if( -1 != csTBState.Find(_T("NEGATIVE_ITEM")) )
        {
            csView = ps.GetPSText(MSG_RA_CAT_NEGATIVEDEPT);
        }
        else if( -1 != csTBState.Find(_T("ITEMIZATION_GET_COUPON")) )
        {
            csView = ps.GetPSText(MSG_DATANEEDED_ENTERCOUPON);
        }
        else if( -1 != csTBState.Find(_T("COUPON_ONLY_TRX")) )
        {
            csView = _T("COUPON ONLY TRANSACTION");
        }
        else if( -1 != csPOSState.Find(_T("UNMATCH_COUPON")) )
        {
            csView = _T("COUPON NO MATCH OR LIMIT");
        }
        else if( -1 != csPOSState.Find(_T("WARRANTY")) )
        {
            csView = _T("WARRANTY ITEM");
        }
        else if( -1 != csPOSState.Find(_T("SUPERVISOR_")) )
        {
            csView = _T("SUPERVISOR APPROVAL");
        }
        else if( GetHookProperty(_T("PropIsPrcVerify")) == _T("1") )
        {
            csView = ps.GetPSText(MSG_RA_CAT_VISUALVERIFY);
        }
        else if( -1 != csTBState.Find(_T("TAKECHANGE_TIMER_EXPIRED")) )
        {
            csView = ps.GetPSText(MSG_DATANEEDED_TAKECHANGETIMEREXPIRED);
        }
        else
        {
            csView = ps.GetPSText(MSG_DATANEEDED_MOREDATA);    //_T("MOREDATA");
        }
        ps.SetLanguage(customerLanguage);
    }
    else
    {
        csView = SMStateBase::GetDescFromView(nView);
    }
    trace(L7, _T("-SMState::GetDescFromView<%s>"), csView);
    return csView;
}

CString SMState::GetCategory(DWORD m_nRpID)
{
    return SMStateBase::GetCategory( m_nRpID);
}

CString SMState::getSCOTStateName(void)
{
    return SMStateBase::getSCOTStateName();
}

void SMState::UpdateStateName(CString csStateName)
{
    SMStateBase::UpdateStateName( csStateName );
}

SMStateBase * SMState::CkStateStackAndReturn(void)
{
    return SMStateBase::CkStateStackAndReturn();
}

void SMState::ClearStateStackEOT(void)
{
    SMStateBase::ClearStateStackEOT();
}

SMStateBase * SMState::OnDataBaseInfoComplete(void)
{
    return SMStateBase::OnDataBaseInfoComplete();
}

#ifndef _TRADESHOWDEMO
SMStateBase * SMState::HandleDataNeeded(void)
{	
	//Start CRD 347846
	CString csIsEverseenAlert = GetHookProperty(_T("IsEverseenAlert"));
	if(csIsEverseenAlert == _T("1"))
	{
		//set this flag to true if a response (item scan) has been made in an Everseen alert
		//in order for SCO to know when to go or not go back to scan and bag
		//when POS returns to Item Menu
		g_bIsProcessingEverseenAlert = true;
	}
	//End CRD 347846
	//If this TB_NEEDMOREDATA is due to mPerks and the previous state is BAGANDEAS
	//we need to resume the security and go back to bagging - else go to Scan&Bag	
	if(m_sButton.Find(_T("cYES")) != -1 || m_sButton.Find(_T("cNO")) != -1
		|| m_sButton.Find(_T("MEP_YES")) != -1 || m_sButton.Find(_T("MEP_NO")) != -1)
	{		
		if(nPreviousState == BES_BAGANDEAS || nPreviousState == BES_INSERTCOUPON)
		{
			switch(nPreviousState)
			{
				case BES_BAGANDEAS:
					if(g_bOnItemOK) //SSCOI-49858
						setAnchorState(BES_SCANANDBAG);	  
					else
					{
						//resume the security
						trace(L7,_T("CMDataEntry, Setting hold weight=false"));
						CKeyValueParmList parmList;
						CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_RESUME);
						_bstr_t bstrParmList((const _TCHAR *)parmList);
						BSTR bstrResult;
						bstrResult = NULL;
						SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
						SysFreeString(bstrResult);
						//go to bagging
						setAnchorState(BES_BAGANDEAS); 
					}
					break;
				case BES_INSERTCOUPON:
					setAnchorState(BES_INSERTCOUPON);
					break;
			}			

			SMStateBase* retState = SMStateBase::HandleDataNeeded();
			delete getRealStatePointer(retState);
			
			return STATE_RETURN;	
		}		
	}
	

	return SMStateBase::HandleDataNeeded();
}
#endif //_TRADESHOWDEMO

SMStateBase * SMState::HandleMSRData(void)
{
    return SMStateBase::HandleMSRData();
}

void SMState::Escalation2RAP(void)
{
    SMStateBase::Escalation2RAP();
}

SMStateBase * SMState::PSRemoteButton1(void)
{
    return SMStateBase::PSRemoteButton1();
}

void SMState::ScanScaleAttract(void)
{
    SMStateBase::ScanScaleAttract();
}

void SMState::ScanScaleSB(void)
{
    SMStateBase::ScanScaleSB();
}

void SMState::TABTransportClearedItem(void)
{
    SMStateBase::TABTransportClearedItem();
}

void SMState::TABReEnableScannerAndClearWaitForApprovalState(void)
{
    SMStateBase::TABReEnableScannerAndClearWaitForApprovalState();
}

int SMState::GetButtonIndex(int nTenderType)
{
    return SMStateBase::GetButtonIndex( nTenderType);
}

bool SMState::IsCashDeviceError(long lDeviceClass)
{
    return SMStateBase::IsCashDeviceError( lDeviceClass);
}

bool SMState::IsCardDeviceError(long lDeviceClass)
{
    return SMStateBase::IsCardDeviceError( lDeviceClass);
}

SMStateBase * SMState::EnterDegradedMode(DEGRADEDMODES lModeType)
{
    return SMStateBase::EnterDegradedMode(lModeType);
}

SMStateBase * SMState::ExitDegradedMode()
{
    return SMStateBase::ExitDegradedMode();
}

void SMState::SetStatePopped( bool bPopped )
{
    SMStateBase::SetStatePopped(  bPopped );
}

bool SMState::GetStatePopped()
{
    return SMStateBase::GetStatePopped();
}

BEST SMState::getNextStateForGoodItem()
{
    return SMStateBase::getNextStateForGoodItem();
}

bool SMState::AreAnyRedApprovalsPending()
{
    return SMStateBase::AreAnyRedApprovalsPending();
}

SMStateBase * SMState::HostOffLine(void)
{
    return SMStateBase::HostOffLine();
}

SMStateBase * SMState::HostOnLine(void)
{
    return SMStateBase::HostOnLine();
}

SMStateBase * SMState::OnTransactionComplete(void)
{
    return SMStateBase::OnTransactionComplete();
}

void SMState::OverrideSecurityException(LPCTSTR lpszAuthorization, LPCTSTR lpszOperation)
{
    SMStateBase::OverrideSecurityException(lpszAuthorization, lpszOperation);
}

void SMState::OverrideSecurityException(LPCTSTR lpszAuthorization, LPCTSTR lpszOperation, LPCTSTR lpszCategory)
{
    SMStateBase::OverrideSecurityException(lpszAuthorization, lpszOperation, lpszCategory);
}

void SMState::TellApplicationStateToSecurity(LPCTSTR lpszOperation, LPCTSTR lpszStateName)
{
	#ifndef _CPPUNIT

	CString csStateName(lpszStateName);
    if((ps.RemoteMode()) && 
		(csStateName != _T("SmAssistMode"))&& 
		(csStateName != _T("SmRemoteAssistMode")) && 
		(csStateName != _T("SmAuthorization")))
    {
        trace(L6, _T("TellApplicationStateToSecurity() Remoted but not telling security about SystemMessage state"));
        return;
		
    }

    trace(L5, _T("+TellApplicationStateToSecurity Operation = %s StateName = %s"), lpszOperation, lpszStateName );

     CKeyValueParmList parmList;
     BSTR bstrResult;
        
    if (co.IsSecurityBypassOn() && fInEndorsementState == ENDORSE_NONE)
    {
        trace(L5, _T("Security bypass on,  ignoring state change"));
    }
   else
    {
        CKeyValue<const _TCHAR*>::Put(parmList, KEY_AC_SET_APP_STATE, lpszOperation);
        
        if (co.IsLoginBypassOn() && fInEndorsementState == ENDORSE_NONE)  // SSCOI-47725: Updated teh code to escalate the endorsement to RAP in attendant mode
        {
          CKeyValue<const _TCHAR*>::Put(parmList, _T("SmAssistMode"));
        }
        else
        {
          CKeyValue<const _TCHAR*>::Put(parmList, lpszStateName);
        }
    
        _bstr_t bstrParmList((const _TCHAR *)parmList);
        bstrResult = NULL;
        SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
        SysFreeString(bstrResult);
    }
        
    if(csStateName == _T("SmAssistMode") || csStateName == _T("SmRemoteAssistMode"))
    {
        CString csOpName(lpszOperation); // tar340275 mm185096

        //if (lpszOperation == KEY_AC_SET_APP_STATE_END) // tar340275 mm185096
        if (csOpName == KEY_AC_SET_APP_STATE_END)
        {
            if(!co.IsSecurityBypassOn() && dm.fStateTakeawayBelt)
            {
                parmList.Clear();
                CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_ASSIST_MODE_STOP);
                _bstr_t bstrParmList((const _TCHAR *)parmList);
                bstrResult = NULL;
                SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
                SysFreeString(bstrResult); // cs185088 mem new
            }

			if(m_bAssistModeReturnAfterSystemMessage)
            {
                trace(L5,_T(" TellApplicationStateToSecurity: Not Disconnecting from RAP"));
                m_bAssistModeReturnAfterSystemMessage = false;
                return;
            }

            trace(L5,_T(" TellApplicationStateToSecurity: Disconnecting from RAP"));
            CString csRemoteConnectionName = ra.GetRemoteConnectionName();
            m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), _T("operation=assist-release;"), csRemoteConnectionName );
            ps.DisconnectFromServer( TRUE );
            ra.RequestEnableAssistMode(true);			
        }
    }

    trace(L5, _T("-TellApplicationStateToSecurity"));

#endif //_CPPUNIT
    //SMStateBase::TellApplicationStateToSecurity(lpszOperation, lpszStateName);
}

long SMState::ChangeSecurityMaintenanceWtFlags(LPCTSTR lpszOperation, LPCTSTR lpszUPC,LPCTSTR lpszFlag, bool bPriceEmbedded)
{
    return SMStateBase::ChangeSecurityMaintenanceWtFlags( lpszOperation, lpszUPC,lpszFlag, bPriceEmbedded);
}

long SMState::ChangeSecurityMaintenanceTolerance(LPCTSTR lpszOperation, LPCTSTR lpszUPC, LPCTSTR lpszTolerance, bool bPriceEmbedded)
{
    return SMStateBase::ChangeSecurityMaintenanceTolerance(lpszOperation, lpszUPC, lpszTolerance, bPriceEmbedded);
}

SMStateBase * SMState::ReturnDelayedStateForGCTender()
{
    return SMStateBase::ReturnDelayedStateForGCTender();
}

SMStateBase * SMState::TareWeightedItem(void)
{
    return SMStateBase::TareWeightedItem();
}

SMStateBase * SMState::TareOnly(void)
{
    return SMStateBase::TareOnly();
}

SMStateBase * SMState::TareWLDBItem(void)
{
    return SMStateBase::TareWLDBItem();
}

SMStateBase * SMState::TareNone(void)
{
    return SMStateBase::TareNone();
}

SMStateBase * SMState::TareOnlyAtAttract(void)
{
    return SMStateBase::TareOnlyAtAttract();
}

void SMState::SetSecurityConfig(LPCTSTR lpszParamName, LPCTSTR lpszParamValue)
{
    SMStateBase::SetSecurityConfig(lpszParamName, lpszParamValue);
}

bool SMState::GetSecurityConfig(LPCTSTR lpszParamName)
{
    return SMStateBase::GetSecurityConfig(lpszParamName);
}

CString SMState::GetSecurityConfigValue(LPCTSTR lpszParamName)
{
    return SMStateBase::GetSecurityConfigValue(lpszParamName);
}

bool SMState::IsLoginBypassAllowed()
{
    return SMStateBase::IsLoginBypassAllowed();
}

void SMState::setAnchorState(BEST state)
{
    SMStateBase::setAnchorState(state);
}

SMStateBase * SMState::setAndCreateAnchorState(BEST state)
{
    return SMStateBase::setAndCreateAnchorState(state);
}

void SMState::DeliverAnchorState(SMStateBase *StatePtr)
{
    SMStateBase::DeliverAnchorState(StatePtr);
}

void SMState::DeliverUpdate(BEST state)
{
    SMStateBase::DeliverUpdate(state);
}

bool SMState::IgnoreTBMessageInSecurityState()
{
    return SMStateBase::IgnoreTBMessageInSecurityState();
}

bool SMState::IsForceDelayedApprovalBeforeSuspend()
{
    return SMStateBase::IsForceDelayedApprovalBeforeSuspend();
}

SMStateBase * SMState::CouponMaxIntervention()
{
    return SMStateBase::CouponMaxIntervention();
}

SMStateBase * SMState::CouponMaxInterventionFinal()
{
    return SMStateBase::CouponMaxInterventionFinal();
}

SMStateBase * SMState::PreTenderState(void)
{
    return SMStateBase::PreTenderState();
}

void SMState::StartTimerForSkipBaggingBtn()
{
    SMStateBase::StartTimerForSkipBaggingBtn();
}

bool SMState::IsRemoteErrorAllowed(long lDeviceClass)
{
    return SMStateBase::IsRemoteErrorAllowed(lDeviceClass);
}

void SMState::SetStateBeforeHelp(CString &csLastName, CString &csNextStateName)
{
    SMStateBase::SetStateBeforeHelp(csLastName, csNextStateName);
}

SMStateBase * SMState::ProcessTBTender(TBTENDERTYPE nTenderType)
{
    return SMStateBase::ProcessTBTender(nTenderType);
}

void SMState::DMSetTriColorLight( DMTriColorLightColor color, DMTriColorLightState state, DMTriColorLightRequestType request )
{
    SMStateBase::DMSetTriColorLight( color, state, request );
}

void SMState::DMTurnOffTriColorLight( void )
{
    SMStateBase::DMTurnOffTriColorLight();
}

void SMState::ShowDoNotBagOnEReceipt( void )
{
    SMStateBase::ShowDoNotBagOnEReceipt();
}

SMStateBase * SMState::GenerateTrustLevelDataNeeded(void)
{
    return SMStateBase::GenerateTrustLevelDataNeeded();
}

void SMState::SendPLAStartPickList()
{
    SMStateBase::SendPLAStartPickList();
}

void SMState::CutReceipt(void)
{
    trace(L6,_T("Receipt Print Cut"));
    //TAR 367557
    if( GetTBProperty(_T("ReleaseReceipt")) == _T("1") ) 
    {
        trace(L6, _T("ReleaseReceipt=1 -> skipping receipt cut."));
    }
    else
    {
        DMReceiptCut();
    }
}

void SMState::SaveSigReceiptData(BSTR pSignatureData)
{
    SMStateBase::SaveSigReceiptData( pSignatureData);
}

void SMState::SaveReceiptData(void)
{
    SMStateBase::SaveReceiptData();
}

SMStateBase * SMState::OnNeedData(CString szDescription, CString szView, CString szIniFile, CString szSection)
{
    return SMStateBase::OnNeedData(szDescription, szView, szIniFile, szSection);
}

SMStateBase * SMState::GenerateRAPDataNeeded(CString sDescription, CString sView, CString sINI, CString sSection)
{
    return SMStateBase::GenerateRAPDataNeeded(sDescription, sView, sINI, sSection);
}

SMStateBase * SMState::HandleDataRequest()
{
    return SMStateBase::HandleDataRequest();
}

SMStateBase * SMState::GPNotify(const GPEVT gpevt)
{
    return SMStateBase::GPNotify(gpevt);
}

SMStateBase * SMState::PopStateFifoStack(void)
{
    return SMStateBase::PopStateFifoStack();
}

bool SMState::isAnySoldVoidedItems(void)
{
    return SMStateBase::isAnySoldVoidedItems();
}

bool SMState::AreThereAnySoldVoidedItems(void)
{
    return SMStateBase::AreThereAnySoldVoidedItems();
}

bool SMState::isBarcodeValidOperatorPassword(const CString csItemCode, CString *pcsOperatorID)
{
    //+ TAR 396459
    if (!co.IsAttendantModeOn() && !storeMode())
	{
		csOperator = EMPTY_STRING;
		csPassword = EMPTY_STRING;
	}
    //- TAR 396459
	
    bool bEmplid = (SendTBCommand(TBCMD_IS_EMPLID_ENABLED) == TB_SUCCESS_IMMEDIATE);
    if( bEmplid )
    {
        trace (L6, _T("isBarcodeValidOperatorPassword() returns false since EMPLID is enabled."));
        return false;
    }
	
    //this function is override to fix the TAR347122
    CString cItemCode = csItemCode;
    switch(co.nOperationsOperatorLoginBarcodeType) 
    {
    case 0:
        return false;
    case 1:
        {
            int index;
            CString csTemp;
            
            index = csItemCode.Find(_T('~'));
            if (index == -1)
                break;
            //the base is not working, it should use Mid instead of Right
            csTemp = csItemCode.Mid(index+1);
            
            index = csTemp.Find(_T('~'));
            if (index == -1)
                break;
            //the base is not working, it should be csTemp.Mid not csItemCode.Right
            csTemp = csTemp.Mid(index+1);
            if (csTemp == _T("104"))
            {
                if (isalpha(csItemCode[0]))
                {
                    trace (L6, _T("csItemCode[0] = %c"), csItemCode[0]);
                    if (cItemCode[0] != _T('F'))
                    {
                        //the base doesn't have this. if the first char is alpha but not F,
                        //overwrite it with F.
                        cItemCode.SetAt(0, _T('F'));
                        trace (L6, _T("New item code = %s. Forcing 'F' at the beginning."), cItemCode);
                    }
                }
                else 
                {
                    cItemCode = _T('F') + cItemCode;
                    trace (L6, _T("csItemCode[0] is numeric. Prepend 'F' at the beginning"));
                }
            }
        }
    default:
        return SMStateBase::isBarcodeValidOperatorPassword(cItemCode, pcsOperatorID);    
    }
}

void SMState::ReadOperatorLoginBarcodeMask(CString OperatorLoginBarcodeMask)
{
    SMStateBase::ReadOperatorLoginBarcodeMask(OperatorLoginBarcodeMask);
}

void SMState::InvalidOperatorLoginBarcodeMask(CString csOperatorLoginBarcodeMask)
{
    SMStateBase::InvalidOperatorLoginBarcodeMask(csOperatorLoginBarcodeMask);
}

bool SMState::isRestrictedItemApprovalRequired(void)
{
    return SMStateBase::isRestrictedItemApprovalRequired();
}

bool SMState::isVisualItemApprovalRequired(void)
{
    return SMStateBase::isVisualItemApprovalRequired();
}

bool SMState::isSecurityApprovalRequired(void)
{
    return SMStateBase::isSecurityApprovalRequired();
}

bool SMState::isCouponRatioApprovalRequired(const bool fUpdateSA)
{
    return SMStateBase::isCouponRatioApprovalRequired(fUpdateSA);
}

bool SMState::isVoidRatioApprovalRequired(const bool fUpdateSA)
{
    return SMStateBase::isVoidRatioApprovalRequired(fUpdateSA);
}

bool SMState::isAnyApprovalRequired(const bool fUpdateSA)
{
     	//(+) VSTS 131568
	bool fReturnVal = false;
        CString csTraceInfo = EMPTY_STRING;
        CString csTemp = EMPTY_STRING;
#ifndef _CPPUNIT

//USSF START
        if (lUnapprovedUSSFItems > 0)
        {
            fReturnVal = true;
            csTraceInfo = _T("USSF Item Approval Required");
        }
//USSF END

        if (isRestrictedItemApprovalRequired())
        {
            fReturnVal = true;
            csTraceInfo = _T("Restricted Item Approval Required ");
        }

        if (isVisualItemApprovalRequired())
        {
            fReturnVal = true;
            csTraceInfo += _T("Visual Item Approval Required ");
        }

        if (lUnapprovedHopperSubstitution> 0)
        {
            fReturnVal = true;
            csTraceInfo += _T("Hopper Substitution Approval Required ");
        }


        if (isCouponRatioApprovalRequired(fUpdateSA))
        {
            fReturnVal = true;
            csTraceInfo += _T("Coupon Ratio Approval Required ");
        }

        if (isAnyNonImmediateApprovalRequired()) //SSCOP-4920
        {
            fReturnVal = true;
            csTemp.Format(_T("Approval Required For: lNotForSaleItems=%d lUnknownItems=%d lPriceRequiredItems=%d lRestrictedNotAllowedItems=%d lTimeRestrictedItems=%d bSuspendFailure=%d lCouponsInAssist=%d lQuantityRestrictedItems=%d fOtherPayment=%d lUnapprovedVoidedItems=%d"),
                              lNotForSaleItems, lUnknownItems, lPriceRequiredItems, lRestrictedNotAllowedItems,
                              lTimeRestrictedItems, bSuspendFailure, lCouponsInAssist, lQuantityRestrictedItems,
                              fOtherPayment, lUnapprovedVoidedItems);
            csTraceInfo += csTemp;
        }

        if (fCantScanCoupon || fRequestForHelp)
        {
            fReturnVal = true;
            csTemp.Format(_T("Approval Required For: fCantScanCoupon=%d; fRequestForHelp=%d"),
                              fCantScanCoupon, fRequestForHelp);
            csTraceInfo += csTemp;
        }

        if (bDropoffRequired && !bDropoffApproved)
        {
            fReturnVal = true;
            csTemp.Format(_T("Approval Required For: bDropoffRequired=%d; bDropoffApproved=%d"),
                              bDropoffRequired, bDropoffApproved);
            csTraceInfo += csTemp;
        }

        if (bSuspendRequested && !bSuspendApproved)
        {
            fReturnVal = true;
            csTemp.Format(_T("Approval Required For: bSuspendRequested=%d; bSuspendApproved=%d"),
                              bSuspendRequested, bSuspendApproved);
            csTraceInfo += csTemp;
        }

        if (fUnDeActivatedItem)
        {
            fReturnVal = true;
            csTraceInfo += _T("UnDeActivated Item Approval Required ");
        }

        // must be last of these tests
        if (isSecurityApprovalRequired())
        {
            fReturnVal = true;
            csTraceInfo += _T("Security Approval Required");
        }

        //Generic Delay intervention
        if(isGenericDelayedInterventionApprovalRequired())
        {
            fReturnVal = true;
            csTraceInfo += _T("Generic Delay Approval Required");
        }
        
		 if (SMState::bVoidApprovalDelayed)
        {
            fReturnVal = true;
            csTemp.Format(_T("Approval Required For: bVoidApprovalDelayed=%d"),
                              bVoidApprovalDelayed);
            csTraceInfo += csTemp;
        }

        trace(L6, _T("+-SMStateBase::isAnyApprovalRequired: Return Code=%d Result Info=%s"), fReturnVal, csTraceInfo);
#endif

        return fReturnVal; 
			
	//return SMStateBase::isAnyApprovalRequired(fUpdateSA);
	//(-) VSTS 131568
}

SMStateBase *SMState::OnGenericDelayedIntervention(CDataNeededUtil &dataNeededUtil)
{
	trace(L6, _T("+-SMState::OnGenericDelayedIntervention - %d "), m_GenericDelayedInterventionStatus);

	CString csDataNeeded = dataNeededUtil.GetViewString();
	
	//extract DataNeeded
	if(csDataNeeded.Find(_T("EAS")) != -1)
	{
		++lUnapprovedEASItems;
		sProcessingDelayedIntervention = DELAYED_EAS;
	}
	else if(csDataNeeded.Find(_T("Mobile Assistance Needed")) != -1)
	{
		++lUnapprovedMobileGenericDelayed;
		sProcessingDelayedIntervention = DELAYED_GENERIC;
	}
	//End CRD 223409
	else if(csDataNeeded.Find(szGDAccountLookUpFailed) != -1 || csDataNeeded.Find(szGDGetAccountDetailsFailed) != -1)	// CRD 557225
	{
		bUnapprovedAccountLookupFailed = true;
	}


    CQueuedActionGenericDelayedIntervention *pDelayedIntervention = new CQueuedActionGenericDelayedIntervention(dataNeededUtil.GetViewStringTranslated(true));
    
    if (!bIsInFinalization && !co.IsInAssistMenus() && !fStateAssist)
    {
		CQueuedActionShowMessageScreen *pAction = new CQueuedActionShowMessageScreen(dataNeededUtil.GetViewString());
		
		if(lUnapprovedEASItems || lUnapprovedMobileGenericDelayed || bUnapprovedAccountLookupFailed)	//CRD 223409, 554916
		{
			trace(L6, _T("do nothing since this is an EAS item or mobile generic delayed or account lookup failed"));
		}
		else
		{
			m_ActionQueue.Add((IQueuedAction*)pAction);
		}
        
    }
	
    if (co.IsInAssistMenus() && m_GenericDelayedInterventionStatus != GENERIC_DELAYED_PROCESSING)
    {
        SMStateBase* retState = pDelayedIntervention->HandleQueuedAction(this);
        delete pDelayedIntervention;
        return retState;
    }
    QueueDelayedIntervention(pDelayedIntervention);
	
    bool bResult = false;
    if (dataNeededUtil.GetBool(QUEUED_MESSAGESCREENITEMNEEDCANCEL, bResult) && bResult)
    {
        SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
    }
	
    if (bIsInFinalization)
    {   //trigger the intervention immediately
        CString csStateName = this->GetRuntimeClass()->m_lpszClassName;
        if (csStateName == _T("SMInProgress") ||
            csStateName == _T("SMFinish"))
        {
            SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
            RETURNSTATE(Finish);
        }
    }
	
    if (dataNeededUtil.GetBool(QUEUED_MESSAGESCREENGOBACKSCANANDBAG, bResult) && bResult)
    {
        if (fInMultiSelectPickList)
        {
            return setAndCreateAnchorState( BES_PRODUCEFAVORITES );
        }
        else
        {
            return setAndCreateAnchorState(BES_SCANANDBAG); 
        }
    }
    
    return STATE_NULL;
}
bool SMState::isProcessingGenericDelayedIntervention()
{
    CQueuedActionGenericDelayedIntervention *pDelayedIntervention = (CQueuedActionGenericDelayedIntervention* const) m_GenericDelayedInterventions.Peek(0);
    if(pDelayedIntervention != NULL) 
    {
        bool IsHandheldRAP = ra.IsRemoteConnectionHandheld(ra.GetRemoteConnectionName());
		
        if(ps.RemoteMode())
        {
            CSCOTDataEntry cDelayedInterventionDataEntry;
            cDelayedInterventionDataEntry.FromString(pDelayedIntervention->GetActionString());
			
            CString csHandlingContext = cDelayedInterventionDataEntry.ExtractString(_T("HandlingContext"));
			
            if(csHandlingContext == _T("SMDataEntryGoToLane"))
                return false;
            else if((csHandlingContext == _T("SMDataEntryGoToLaneOrRap")) && IsHandheldRAP)
                return false;
            else if((csHandlingContext == _T("SMDataEntryGoToLaneOrHHRap")) && !IsHandheldRAP)
                return false;			
        }
		return true;
    }
    return false;
}

void SMState::ResetAllTransactionValues(void)
{
	if (fRAPIsInStoreMode)
	{
		fRAPIsInStoreMode = false;

		trace(L6, _T("+ra.OnExitStoreMode in SMState::ResetAllTransactionValues"));
		ra.OnExitStoreMode();
		trace(L6, _T("-ra.OnExitStoreMode"));
	}

	SMState::bTMDVoided = false;
	SMStateBase::ResetAllTransactionValues();
	
	SMStateBase::bIsFromVoidApproval = false;
	SMState::m_bSecurityDisabledForMPerks = false;
	m_lUnApprovedRestrictedItemsDuringCancel = 0;
	m_bDataNeededAssistMode = false;

	// POS31054 - reset the yellow lane light flag and remove the red tab area on the rap
	m_bNeedYellowLightForMobile = false;

	m_pAppPSX->SetConfigProperty(_T("LaneMobileStateBackground"), _T(""), UI::PROPERTYVISIBLE, VARIANT_FALSE);
	m_pAppPSX->SetConfigProperty(_T("LaneMobileStateWindow"), _T(""), UI::PROPERTYVISIBLE, VARIANT_FALSE);
	//e POS31054

	m_bCouponInsertionBypassed = false; //POS33020
	m_lInsertedCouponsTotal = 0;        //POS33020

	m_bRecallModeWaitforItemSold = true; // RFQ 9545
	m_bIsLoyaltyProcessed = false; //154609
	
	bQuantityRequiredActive = false; // 140001
	bFinishRecall=false;	//SSCOI-55092
	m_bTMDAccepted = false; //CRD 168992
	SetTBProperty(_T("xWaitForPosScannerReady"), _T("0")); // 135446 Lane hung when transferring
	lUnapprovedEASItems = 0;	//CRD 185620
	bEASApproveFromVoid = false;	//CRD 185620
	bIsItemizationAfterSkipBaggingSent = false;	//SSCOI-55023
	//Start CRD 145555
	csFLMSType = _T("");
	csFLMSAuditResult = _T("");
	csMinimumScanRequired = _T("");
	csFLMSAuditStartTime = _T("");
	csFLMSAuditEndTime = _T("");
	csFLMSOperatorID = _T("");
	csFLMSSessionID = _T("");
	SMState::csFLMSAuditItemList.RemoveAll(); //CRD 145557
	//End CRD 145555
	m_bSNDmPerkEntered = false;			// CRD 125377
	m_bSNDTMDEntered = false;			// CRD 125377
	m_bSNDAgeEntered = false;			// CRD 125377
	m_bSNDAllowItemMenu = false;		// CRD 125377
	m_bSNDClearUnknownItemC = false;	// CRD 125377
	m_bSNDmPerksCancel = false;			// CRD 125377
	//m_csTransactionMode = TRANSMODE_SCO;	CRD 155116
	m_bIsLoyaltyProcessed = false; //154609
	bDoNotSendStatusRequest = false;
	lUnapprovedMobileGenericDelayed = 0;	//CRD 223409
	bGenericDelayedFinishSent = false;	//CRD 232271
	SMState::mPerksActivatedInTransaction = false; //reset for the next transaction
	sEASItem = EAS_ITEMS();	//reset struct
	SetHookProperty(_T("IsInStoreMode"),_T("0"));	//CRD 232267
	bFinishSendingSuspendBarcode = false;	//CRD 260234
	SetHookProperty(_T("AlcoholLimitExceeded"), _T("0"));	//CRD 291250
	SetHookProperty(_T("ShiptBarcodeScanned"), _T("0"));	//CRD 297752
	g_bIsProcessingEverseenAlert = false;	//CRD 347846
	bResumeCompleteWaitingForItemMenu = false;	//CRD 503257
	csApplyMeijerCardGUID = _T("");							//CRD 525312
	sCitiAccountDetails = CITIACCOUNTDETAILS();
	sCitiAccountDetails.bSend = false;
	bUnapprovedAccountLookupFailed = false;	// CRD 554916
	validatingMeijerCardWebApp = false;		// CRD 574057
	bApplyMeijerStartedFromAttract = false;	// CRD 537481
	SMState::StoreAssistRequestedTime = NULL;
}

void SMState::ResetAllTenderValues(void)
{
    SMStateBase::ResetAllTenderValues();
}


#ifndef _TRADESHOWDEMO
//////////////////////////////////////////////////////////////
// Print a receipt file, hFile is the handler of the receipt file
//////////////////////////////////////////////////////////////
long SMState::PrintReceipt(HFILE hFile, bool bSaveReceipt)
{
    trace(L6,_T("+DMPrintReceipt"));
    CString csReceiptData;
    DWORD  nRetValue = 0;	// return length of the receipt
    CString csLReceiptTmpFile = LRECEIPT_TMP_FILE;
    GET_PARTITION_PATH(LRECEIPT_TMP_FILE, csLReceiptTmpFile.GetBuffer(_MAX_PATH));
    csLReceiptTmpFile.ReleaseBuffer();

    //TAR 357192, 365185
    if( GetTBProperty(_T("ReleaseReceipt")) == _T("1") ) 
    {
        trace(L6, _T("ReleaseReceipt=1 - delaying receipt print."));
        return nRetValue;
    }
    
    fDMReceiptError = false;
    
    try {
        if (hFile!=NULL)
        {
            DWORD	len;
            LPTSTR pBuff;
            
#if _MFC_VER < 0x0700
            CFile	cFile( hFile );
#else
            CFile	cFile( (HANDLE)hFile );  
#endif
            
            len = (DWORD)cFile.SeekToEnd();
            cFile.SeekToBegin();
            //		  len = len/sizeof(_TCHAR);
            
            if (len)
            {
                pBuff = new _TCHAR [len+1];
                if (pBuff)
                {
                    memset(pBuff, 0, (len+1));
                    UINT c = cFile.Read(pBuff, len);
                    if (c)
                    {
                        if (pBuff[0] == 0xFEFF)
                        {
                            len = len/sizeof(_TCHAR);
                            pBuff[len] = 0;
                            CString csWholePrintData((wchar_t*) pBuff, len);
                            csReceiptData = csWholePrintData;
                            csWholePrintData = csWholePrintData.Mid(1);
                        }
                        else
                        {
                            pBuff[len] = 0;
                            CString csWholePrintData((char*) pBuff, len);
                            csReceiptData = csWholePrintData;
                        }
                        
                        nRetValue = len;
                    }
                    delete [] pBuff;
                }
            }
        }
    }
    catch(CFileException *e)
    {
        // may be an invalid file handle
        _TCHAR szError[255] = {NULL};
        e->GetErrorMessage( szError, 255 );
        //eo.Error(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_BS_RECEIPT,_T("%d|%s"),hFile, szError);
        trace(L2,_T("SMState::PrintReceipt exception: %s"),szError);
        e->Delete();
        return nRetValue;
    }
    
    if ((co.fUserExitAllEnabled) &&    // if exits enabled and
        (co.fUserExit15Enabled))        //    this exit is enabled
    {
        if (UserExit15(csReceiptData) != 1)   // if userexit return 1, means no print
        {
            // csReceiptData might have been got modified by user exit
            nRetValue = csReceiptData.GetLength();
        }
    }
    
    if (csReceiptData.IsEmpty())
    {
        // valid file handle but file is of zero length
        // though this is not an error, app wants to log this event as an info item
        //eo.Error(INFO,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_NULL_RECEIPT, _T("%d|%d"), hFile, 0);
        trace(L2,_T("PrintReceipt received zero-length file"));
        return 0;
    }
    
    if (TrainingModeObj.IsTMOn())
    {
        csReceiptData = csReceiptData + ps.GetPSText(MSG_TRAINING_MODE_LINE,SCOT_LANGUAGE_PRIMARY);
    }
    
    CString csPrintApprovalList = GetApprovalNeededString();
    if (csPrintApprovalList.GetLength()) 
    {
        csReceiptData += csPrintApprovalList;
    }
    // if there is print data, ...
    nRetValue = csReceiptData.GetLength();
    
    if (bSaveReceipt)
    {
        //for Print Last Receipt module
        if (TrainingModeObj.IsTMOn())
        {
            SaveLastReceiptData(csLReceiptTmpFile, ps.GetPSText(MSG_TRAINING_MODE_LINE,SCOT_LANGUAGE_PRIMARY)); //save last receipt data from TM
        }
        if (csPrintApprovalList.GetLength()) 
        {
            SaveLastReceiptData(csLReceiptTmpFile, csPrintApprovalList); //save approval list
        }
        //end of for Print Last Receipt module
    }
    DMReceiptLine(csReceiptData);   // Print to Receipt
    
    //tar 266257 if csReceiptData contains the "Paper Cut" Esc sequence at the end, SCOTAPP don't cut paper again.
    // Reset nRetValue to 0. The "Paper Cut" Esc sequence's format is "\x1B|#p", "\x1B|#fp" or "\x1B|#sp". # will be replaced
    // by the percentage cut desired.
    
    if(nRetValue >= 3  )  
    {
        CString csTemp = csReceiptData.Right(7);
        if(csTemp.Find(_T("\x1B|")) != -1 
            && !(csTemp.Right(1)).CompareNoCase(_T("P")))   
        {
            nRetValue = 0;   //Last command is Paper Cut
        }
    }
    
    nTrxReceiptLength += nRetValue;
    
    trace(L6,_T("-DMPrintReceipt %d"),nRetValue);
    return nRetValue;

    //return SMStateBase::PrintReceipt(hFile, bSaveReceipt);
}
#endif //_TRADESHOWDEMO

long SMState::PrintJournal(HFILE hFile)
{
    return SMStateBase::PrintJournal(hFile);
}

CString SMState::CustomerName(LPCTSTR trackData)
{
    return SMStateBase::CustomerName( trackData);
}

CString SMState::GetAccountNum(LPCTSTR trackData)
{
    return SMStateBase::GetAccountNum( trackData);
}

void SMState::UpdateDepositedCouponVars()
{
    SMStateBase::UpdateDepositedCouponVars();
}

SMStateBase * SMState::DepositCouponsIfNecessary()
{
    return SMStateBase::DepositCouponsIfNecessary();
}

SMStateBase * SMState::SetISIPTimeOut(int nMilliSec)
{
    return SMStateBase::SetISIPTimeOut(nMilliSec);
}

int SMState::GetISIPTimeOut()
{
    return SMStateBase::GetISIPTimeOut();
}

bool SMState::isAnyNonImmediateApprovalRequired(void)
{
    return SMStateBase::isAnyNonImmediateApprovalRequired();
}

CString SMState::GetApprovalNeededString(void)
{
    return SMStateBase::GetApprovalNeededString();
}

bool SMState::WriteSCOTStatusToFile(CString cStatus)
{
    return SMStateBase::WriteSCOTStatusToFile(cStatus);
}

bool SMState::OpenSCOTStatusFile(CString cFileName)
{
    return SMStateBase::OpenSCOTStatusFile(cFileName);
}

SMStateBase * SMState::DMTakeawaySendScaleStatus()
{
    return SMStateBase::DMTakeawaySendScaleStatus();
}

SMStateBase * SMState::DMTakeawayAMExitDone()
{
    return SMStateBase::DMTakeawayAMExitDone();
}

TenderButton & SMState::GetTenderButton(int iIndex)
{
    return SMStateBase::GetTenderButton(iIndex);
}

void SMState::SetTenderButton(int iIndex)
{
    SMStateBase::SetTenderButton(iIndex);
}

int SMState::AdjustTenderButtonIndex(TBTENDERTYPE nAMTender)
{
    return SMStateBase::AdjustTenderButtonIndex(nAMTender);
}

void SMState::SetNextGenUICurrentItem(CString csUPC /*= _T("")*/,
                                          CString csDescription /*= _T("")*/,
                                          bool isAgeRestricted /*= false*/,
                                          bool isVisualVerify /*= false*/,
                                          int nQuantity /*= 0*/)
{
    SMStateBase::SetNextGenUICurrentItem(csUPC, csDescription, isAgeRestricted, isVisualVerify, nQuantity);
}

void SMState::SetNextGenUIMode(void)
{
    SMStateBase::SetNextGenUIMode();
}

bool SMState::SetItemGDAsyncIDReceiptVariable(const CString& csReceipt, CString& csItemID, CString& csGDAsyncID)
{
    return SMStateBase::SetItemGDAsyncIDReceiptVariable(csReceipt, csItemID, csGDAsyncID);
}

bool SMState::isItemGenericDelayed(CString csItemID, CString &csGenDataNeededID)
{
    return SMStateBase::isItemGenericDelayed(csItemID, csGenDataNeededID);
}

void SMState::loadOptionsFromConfigObject()
{
    SMStateBase::loadOptionsFromConfigObject();
    // start CDR126194
    ms_MobileAuditDataEntryINI = cr.GetStringOption(_T("Operations"), _T("MobileAuditDataEntryINI"), _T(""), _T("Scotopts"));
    ms_MobileAuditDataEntryINISection = cr.GetStringOption(_T("Operations"), _T("MobileAuditSection"), _T("MobileTransactionAudit"), _T("Scotopts"));
    if (ms_MobileAuditDataEntryINI.IsEmpty())
    {
        ms_MobileAuditDataEntryINI = co.csDataEntryINIFile;
    }
	// end CDR126194
}

SMStateBase *SMState::OnGenericDelayedComplete(void)
{
    SMStateBase* retState = SMStateBase::OnGenericDelayedComplete();

    CString test = getStateName(this);

	//CRD 185620 - code to handle voiding item in Customer Mode
	//bIsGenericDelayedProcessed - in the event TB_GENERIC_DELAYED_COMPLETE  is received for the 2nd generic delayed (supplemental barcode or EAS tagged items), no need to process
	//the intervention again because POS is finished. Clear directly the intervention
	trace(L6, _T("m_GenericDelayedInterventionStatus = %d, lUnapprovedEASItems = %d, lUnapprovedMobileGenericDelayed = %d, bEASApproveFromVoid = %d, bIsGenericDelayedProcessed = %d"), m_GenericDelayedInterventionStatus, lUnapprovedEASItems, lUnapprovedMobileGenericDelayed, bEASApproveFromVoid, bIsGenericDelayedProcessed);
	if((!lUnapprovedEASItems && bEASApproveFromVoid) 
		|| bIsGenericDelayedProcessed)
	{
		m_csCurrentIntervention = m_GenericDelayedInterventions.GetLast();

		CString m_csLastInterventionID = _T("");
        
        if(m_csCurrentIntervention != NULL)
        {
            m_csLastInterventionID = m_csCurrentIntervention->GetActionID();
            ra.OnGenericDelayedApproval(m_csCurrentIntervention->GetActionID(), m_csCurrentIntervention->GetReportingID(), ps.RemoteMode());
            if (ps.RemoteMode())
            {
                rp.SetOperator(ra.getOperatorID(ra.GetRemoteConnectionName()));
            }
            else
            {
                rp.SetOperator(csOperator);
            }
            rp.SetInterventionEndInProgress(true); 

            m_GenericDelayedInterventionStatus = GENERIC_DELAYED_NOT_PROCESSING;
			ClearCurrentGenericDelayedIntervention();

            return setAndCreateAnchorState(BES_SCANANDBAG);
        }
	}

    if (retState == STATE_NULL &&
        !m_GenericDelayedInterventionStatus &&
        co.IsInAssistMenus())
    {
        return createAnchorState();
    }
    else
    {
        return retState;
    }
}

//#ifdef _SSCOMOBILE
void SMState::InitializeSSCOMobile()
{
    SMStateBase::InitializeSSCOMobile();
}

void SMState::UnInitializeSSCOMobile()
{
    SMStateBase::UnInitializeSSCOMobile();
}

SMStateBase* SMState::SellRecalledItem()
{

	// s RFQ 9545
	// if first recall item change to mPerks phone number entry since FLMS
	if ((lUnknownItems == 0) && (lItems == 0) && (m_bRecallModeWaitforItemSold))
	{
		m_bRecallModeWaitforItemSold = false;
        trace(L6, _T("SMState::SellRecalledItem m_bRecallModeWaitforItemSold = %d"),m_bRecallModeWaitforItemSold);
		
		m_bIsLoyaltyProcessed = true; //set isLoyaltyProcess to true as we are sending it here to pos.
		trace(L6, _T("SMState::SellRecalledItem m_bIsLoyaltyProcessed = %d"), m_bIsLoyaltyProcessed);

		// POS111525 POS111527 CRD 145555
		// Tell POS FLMS item processing is complete.
		//csFLMSmPerksBarcode = recallList.GetScanData();
		//sendFLMSTLog(0, 0);
		
		ra.RequestEnableAssistMode(false);
	
		// Tell security that we are exiting form scan& bag state
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );

			m_pAppPSX->SetConfigProperty (_T("mPerksButtonVideo"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_FALSE);	
			
			//+POS109057 means that scan data is a mPerks barcode with PIN
			if(recallList.GetScanData().GetLength() > 10 && recallList.GetScanData().Find(_T("99999")) != -1)	
			{
				trace(L6, _T("mPerks barcode received and will be sent to POS directly"));
				SetTBProperty(_T("xWaitForPosScannerReady"), _T("1"));
				SMRecallingItems::g_nTimeoutRecallingWaitForPos = 0;  // RFQ 9545
				SMRecallingItems::g_bRecallingWaitForPos = false;	   // RFQ 9545

				csDMLastBarCode = recallList.GetScanData();
				csDMLastBarLabel = recallList.GetScanDataLabel();
				csDMLastBarType = 201;	//need to have the barcode data here - SCAN_SDT_PDF217
				csItemSent = csDMLastBarCode;
				io.Reset();
				io.csOrigCode = csItemSent;    // item code keyed
				io.csItemCode = csItemSent;    // assumed to be the same
				io.lQuantityEntered = 0;
				io.cScanType = csDMLastBarType;
				io.d.fQuantityConfirmed = true;
				fItemOverride = true;
				trace(L6, _T("-SMState::SellRecalledItem"));
				return TBItemSale(false,0,true);
			}
			//-POS109057

			// Trigger the event 
			SendTBCommand(TBCMD_SET_TB_STATE,_T("TB_DIGITAL_COUPON"));
		}
	

	// May not need this...
	trace(L6, _T("SMState::SellRecalledItem setting xWaitForPosScannerReady to 1"));
	SetTBProperty(_T("xWaitForPosScannerReady"), _T("1"));
	SMRecallingItems::g_nTimeoutRecallingWaitForPos = 0;  // RFQ 9545
	SMRecallingItems::g_bRecallingWaitForPos = false;	   // RFQ 9545	
	//	SetHookProperty(_T("WaitForPosScannerReady"), _T("1"));
	// e RFQ 9545

    return SMStateBase::SellRecalledItem();
}

int SMState::GetUnsolictedDataBarcodeType(CString csData)
{
    return SMStateBase::GetUnsolictedDataBarcodeType(csData);
}

void SMState::SetInRecalledModeReceiptMessage(PSTEXTMSG nReceiptMsg, PSTEXTMSG nInstructionText)
{
     trace(L6, _T("+SMStateBase::SetInRecalledModeReceiptMessage"), g_bInRecallMode);

    CString csReceiptMsg;
    CString csMsg;
	CString csItemDesc;
	
	//CRD 293397 - get item description from POS, not from the mobile cart from Central Server  
	CString csPOSState(_T(""));
    SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);
	if(csPOSState== _T("RESTRICTED_ITEM_MOBILE"))
	{
		csItemDesc = GetHookProperty(_T("OG_Guidance_Prop"));  
	}
	else if(csPOSState == _T("ITEM_UNKNOWN"))
	{
		csItemDesc = GetHookProperty(_T("item_code"));  
		
		if(csItemDesc == _T(""))
		{
			CString csOperatorGuidance = GetHookProperty(_T("OG_Guidance_Prop"));
			int nPos = csOperatorGuidance.Find(_T(":"));
			csItemDesc = csOperatorGuidance.Mid(nPos+1, csOperatorGuidance.GetLength());
			csItemDesc.TrimLeft();
			csItemDesc.TrimRight();
		}
	}
	//End CRD 293397 
	
    CString csCurrentItem(_T("\n\n"));
    csCurrentItem += ps.GetPSText(TXT_ITEMCODE);
    csCurrentItem += csItemDesc;
    csReceiptMsg = ps.GetPSText(nReceiptMsg, SCOT_LANGUAGE_PRIMARY)+ _T(" ") + csItemDesc;
    ps.AddMessageToReceipt(_T("CMSMReceipt"), csReceiptMsg);
    
    csMsg = ps.GetPSText(nInstructionText);
    csMsg += csCurrentItem;
    ps.Message12(csMsg, true);

    trace(L6, _T("-SMStateBase::SetInRecalledModeReceiptMessage"));
}

bool SMState::RAPAssistModeAllowed()
{
    return SMStateBase::RAPAssistModeAllowed();
}
//#endif _SSCOMOBILE
void SMState::LoadMeijerBiometricOptions()
{
    trace(L6, _T("+SMState::LoadMeijerBiometricOptions()"));
    // Check if scanner sharing is enabled. For this we need to determine
    // which scanner class is being used by the LaunchPad.
    _bstr_t configFile =  co.csConfigFilePath + _T("\\") + _T("LaunchPad.xml");
    try
    {
        MSXML2::IXMLDOMDocumentPtr spDoc(__uuidof( MSXML2::DOMDocument40));
        if( VARIANT_TRUE == spDoc->load(configFile) )
        {
            LPCTSTR szOpts[] = { _T("UseBiometrics"), _T("AllowEmplIdSignOn"), _T("AllowHandKeyEMPLID") };
            
            for(int i = 0; i < sizeof(szOpts)/sizeof(szOpts[0]); i++)
            {
                //POS102622 check if the acsopts has this option already, if it is, then ignore the ones from launchpad.xml
                if( GetTBProperty(szOpts[i]) == _T("") )
                {                
                    CString strPath;
                    strPath.Format(_T("//Options/%s"), szOpts[i]);
                    MSXML2::IXMLDOMNodePtr spOption = spDoc->selectSingleNode(strPath.GetBuffer(0));
                    strPath.ReleaseBuffer();
                    CString csVal = _T("0");
                    if( spOption != NULL )
                    {
                        if( BoolValue((LPCTSTR)spOption->text) )
                        {
                            csVal = _T("1");
                        }
                    }
                    trace(L6, _T("Setting TB option %s=%s"), szOpts[i], csVal);
                    SetTBProperty(szOpts[i], csVal);
                }
                else
                {
                    trace(L6, _T("DON'T Set TB option %s"), szOpts[i]);
                }
                // e POS102622
                
				
            }
        }
    }
    catch(_com_error & cex)
    {
        trace(L6, _T("Exception occurred trying to read from the %s file: %s"), (LPCTSTR)configFile, cex.ErrorMessage());
    }
    trace(L6, _T("-SMState::LoadMeijerBiometricOptions()"));
}

bool SMState::BoolValue(CString csVal)
{
	csVal.MakeUpper();
    return ( (csVal == _T("Y")) || (csVal == _T("T")) || (csVal == _T("1")) || (csVal == _T("TRUE")) || (csVal == _T("YES")) );
}

/////////////////////////////////////////////////////
bool SMState::SCOTIsVisible()
{
    bool visible = false;
    CWnd *pWndPrev = CWnd::FindWindow(NULL, _T("NCR SCOT"));
    if (pWndPrev && pWndPrev->IsWindowVisible())
    {
        visible = true;
    }
    return visible;
}


//(+) POS181195
CString SMState::GetRAPConnectionName()
{
	// +SSCOI-45301
	// Commented below codes as Base already have a method for getting the RAP connection name;
	//long lRC;
    //CString csRAPName;
    //HKEY hKey;
	
	//if (lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP"), 
    //    0, KEY_QUERY_VALUE, &hKey))
		
	//	DWORD dwDataSize;
    //TCHAR* client=_T("ClientName");
    //TCHAR clientname[20]; // allow longer filename
    
    
	//DWORD    dwDataSize= sizeof(clientname);
    //lRC = RegQueryValueEx(hKey, client, NULL, NULL, 
    //    (LPBYTE)clientname, &dwDataSize);
	
    //if(lRC)
	//{
	//	RegCloseKey(hKey);
	//	return "";
	//}
	//csRAPName = clientname;
	//csRAPName = _T("RAP.") + csRAPName;
	return ra.GetRemoteConnectionName();
	// -SSCOI-45301
}

CString SMState::GetLaneName()
{
	
	
	
	TCHAR InfoBuf[INFO_BUFFER_SIZE];
	DWORD buffCount = INFO_BUFFER_SIZE;
	bool bVal =  GetComputerName(InfoBuf, &buffCount);
	CString csLane = InfoBuf;
	
	
    return csLane;
}

//(-) POS181195

void SMState::MeijerModifyScannedBarcode(void)
{
	//-
    // If ConvertUPCtoEAN option is set, then convert all UPC-A
    // scans to EAN-13 scans by pre-prending a "0".
    // If ConvertUPCtoEAN Option Set, do the conversion.
    CString strOptionName = _T("ConvertUPCtoEAN");
    CString strConvertUPCtoEAN;
    SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &strConvertUPCtoEAN);
	if(strConvertUPCtoEAN == _T("Y"))
    {
        CString cdTempDMLastBarCode = csDMLastBarCode;
        CString csOrigSymbology = cdTempDMLastBarCode.GetAt(0); 
        if (csOrigSymbology == _T("A"))
        {
			CString csSymbology(_T(""));
            csDMLastBarType = SCAN_SDT_EAN13;			
			


            CString cdTempDMLastBarLabel = csDMLastBarLabel;
			if(iswalpha(cdTempDMLastBarCode.GetAt(0))) //check first char
			{
				csSymbology = _T("F");
				csDMLastBarCode = csDMLastBarCode.Mid(1);
			}
			
			
            csDMLastBarLabel = _T("0") + csDMLastBarLabel;
            csDMLastBarCode = csSymbology + _T("0") + csDMLastBarCode;
			cdTempDMLastBarCode = csDMLastBarCode;
			
            
        }
    }
    
    

    // TAR #303324
#ifdef _DEBUG
    // In DEBUG ONLY, since our emulator doesn't do code-128, emulate using code-39
    // changing the symbology from B1 to B3.
    // Assume discounted item if code 39 (type 108) AND starts with "M"
    if ((csDMLastBarType == SCAN_SDT_Code39)&&
        (csDMLastBarLabel.Mid(0, 1) == _T("M")))
    {
        // Add "B3" back to the start of barcode data.
        trace(L6, _T("Adding Symbology (B3) Back to Code 39 Scan"));
        CString csScanCode = _T("B3");
        csScanCode += csDMLastBarLabel;
        csDMLastBarLabel = csScanCode;
    }
#endif
    
    // POS 11013 - the SMState::MeijerModifyScannedBarcode logic to append the symbology needs to be removed, 
    // so the logic to check symbology in CustomerMHIUtem is not needed anymore - rrs
    // Assume discounted item if code 128 (type 110) AND starts with "M"
    
    /*if ((csDMLastBarType == SCAN_SDT_Code128)&&
        (csDMLastBarLabel.Mid(0, 1) == _T("M")))
    {
        // Add "B3" back to the start of barcode data.
        trace(L6, _T("Adding Symbology (B3) Back to Code 128 Scan"));
        CString csScanCode = _T("B3");
        csScanCode += csDMLastBarLabel;
        csDMLastBarLabel = csScanCode;
    }
    
    // Assume Catalina coupon if EAN-13 (type 104) AND starts with "99"
    
    if ((csDMLastBarType == SCAN_SDT_EAN13)&&
        (csDMLastBarLabel.Mid(0, 2) == _T("99")))
    {
        // Add "F" back to the start of barcode data.
        trace(L6, _T("Adding Symbology (F) Back to EAN-13 Scan"));
        CString csScanCode = _T("F");
        csScanCode += csDMLastBarLabel;
        csDMLastBarLabel = csScanCode;
    }
    
    // Add back symbology for type 119 EAN13 + Supplement.
    
    if (csDMLastBarType == SCAN_SDT_EAN13_S)
    {
        // Add "F" back to the start of barcode data.
        trace(L6, _T("Adding Symbology (F) Back to EAN-13 Supp Scan"));
        CString csScanCode = _T("F");
        csScanCode += csDMLastBarLabel;
        csDMLastBarLabel = csScanCode;
    }*/
    // E - POS 11013 - the SMState::MeijerModifyScannedBarcode logic to append the symbology would be removed, 
    // so the logic to check symbology in CustomerMHIUtem is not needed anymore - rrs
    

    csItemSent = csDMLastBarCode;
}

//(+) POS244229 
void SMState::CashMonitoringCounts(int transactionState)
{    	
    CString csDispenserCounts = _T(""), csAcceptorCounts = _T("");

	if(!m_cmMgr.GetCMCashCounts(csDispenserCounts, csAcceptorCounts))
	{
		trace(L6, _T("Failed to retrieve Cash counts from Cash Manager. Unable to report cash counts to ACS for transaction type: %d"), transactionState);
		return;
	}
	trace(L6, _T("Cash in the dispenser = %s"), csDispenserCounts);
    trace(L6, _T("Cash in the acceptor = %s"), csAcceptorCounts); 
	csDispenserCounts += ',';
 
	CString csCashStatus(_T(""));
    bool bCashUpdate = false;
     
    int count = csDispenserCounts.GetLength();   
    int mid = csDispenserCounts.Find(_T(";")); 
    CString csCashCounts_Coin =  csDispenserCounts.Left(mid);
    CString csCashCounts_Bill =  csDispenserCounts.Mid( mid+1, count-mid );
     
    trace(L6, _T("Dispenser Coin counts = %s"), csCashCounts_Coin);
    trace(L6, _T("Dispenser cash counts = %s"), csCashCounts_Bill);
	  	
    if(CashRetrieveMap(csCashCounts_Coin,1)) 
    {
		bCashUpdate = true;
		trace(L6, _T("csCashCounts_Coin updated"));
	}
		
	if(CashRetrieveMap(csCashCounts_Bill,2)) 
    {
		bCashUpdate = true;
		trace(L6, _T("csCashCounts_Bill updated"));
	}
	
    int count_A = csAcceptorCounts.GetLength();
    int mid_A = csAcceptorCounts.Find(_T(";"));
    CString csAcceptCounts_Coin =  csAcceptorCounts.Left(mid_A);
    CString csAcceptCounts_Bill =  csAcceptorCounts.Mid( mid_A+1, count_A-mid_A );
 
    trace(L6, _T("Acceptor Coin counts = %s"), csAcceptCounts_Coin);
    trace(L6, _T("Acceptor Cash Counts = %s"), csAcceptCounts_Bill);
    
    if(CashRetrieveMap(csAcceptCounts_Coin,3)) 
    {
		bCashUpdate = true;
		trace(L6, _T("csAcceptCounts_Coin updated"));
	}
	
	if(CashRetrieveMap(csAcceptCounts_Bill,4)) 
    {
		bCashUpdate = true;
		trace(L6, _T("csAcceptCounts_Bill updated"));
	}
	 
    CString csFmt;
	
	COleDateTime dtNow;
    dtNow = COleDateTime::GetCurrentTime();
    CString csDate, csTime, csDTime;
    csDate = dtNow.Format( _T("%m/%d/%y") );	
    csTime = dtNow.Format( _T("%H:%M:%S") );
    csDTime.Format(_T("CMDateTimeRead=\"%s %s\""),csDate,csTime);
    trace(L6, _T("Date and time = %s"), csDTime);
    trace(L6, _T("IsInTransaction = %d"), transactionState);
    CString csIsInTransaction(_T(""));
	csIsInTransaction.Format(_T("<CashMonitoring IsInTransaction=\"%d\" %s>"),transactionState,csDTime);	
	csCashStatus += csIsInTransaction;

	//getting maximum possible coin counts from acsopts.dat 
    CString strOptionName = _T("CoinMDenom");
	CString csFLConfig;
	SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &csFLConfig);
    trace(L6, _T("Maximum Available Coin Counts from ACSOpts.dat csFLConfig:= %s"),csFLConfig);

	//If acsopts.dat file is not available. directly added the counts to the string
    if(csFLConfig == "")
    {
        csFLConfig = _T("1,5,10,25,50,100");
    }

    CashArray(csFLConfig,1);

    CString strcashOptionName = _T("CashMDenom");
	CString CashFLConfig;
	SendTBCommand(TBCMD_GET_OPTIONS, &strcashOptionName, &CashFLConfig);
    trace(L6, _T("Maximum Available Cash Counts from ACSOpts.dat csFLConfig: = %s"),CashFLConfig);
    
    if(CashFLConfig == "")
    {
        CashFLConfig = _T("100,200,500,1000,2000,5000,10000");
    }
	
    CashArray(CashFLConfig,2);
	
	//retriving cash counts from map using max possible coin and cash counts arrays.
    for(int i = caCashList.GetSize()-1; i>=0;i--)  
    { 
        INT m_DisPenceCashcount;
        m_DisPenceCashcount = cmCashCounts_Bill[caCashList[i]];       
        csFmt.Format(_T("<AvailableCashCount_%dB Qty=\"%d\"/>"),caCashList[i],m_DisPenceCashcount);
        csCashStatus += csFmt;        
        m_DisPenceCashcount = 0;     
    }     
	
	for(int j = caCoinList.GetSize()-1; j>=0;j--)  
    {    
        INT m_DisPenceCoincount;
        m_DisPenceCoincount = cmCashCounts_Coin[caCoinList[j]];       
        csFmt.Format(_T("<AvailableCashCount_%dC Qty=\"%d\"/>"),caCoinList[j],m_DisPenceCoincount);
        csCashStatus += csFmt;
        m_DisPenceCoincount = 0;   
    }
	
    for(int k = caCashList.GetSize()-1; k>=0;k--)  
    {
        INT m_AcceptCashcount;
        m_AcceptCashcount = cmAcceptCounts_Bill[caCashList[k]];      
        csFmt.Format(_T("<UnAvailableCashCount_%dB Qty=\"%d\"/>"),caCashList[k],m_AcceptCashcount);
        csCashStatus += csFmt;        
        m_AcceptCashcount = 0;     
    }
    
	for(int l = caCoinList.GetSize()-1; l>=0;l--)  
    {
        INT m_AcceptCoincount;
        m_AcceptCoincount = cmAcceptCounts_Coin[caCoinList[l]];      
        csFmt.Format(_T("<UnAvailableCashCount_%dC Qty=\"%d\"/>"),caCoinList[l],m_AcceptCoincount);
        csCashStatus += csFmt;
        m_AcceptCoincount = 0;   
    }
    
	CString csStatusEnd(_T(""));
    csStatusEnd.Format(_T("</CashMonitoring>"));
    csCashStatus += csStatusEnd;
		
    if(!csCashStatus.IsEmpty() && (bCashUpdate || transactionState==3))   
    {
		SetTBProperty(_T("IsItForCashMonitoring"), _T("1")); //POS281261 by aab.
        trace(L6, _T("Cash status = %s"), csCashStatus);
        SendTBCommand(TBCMD_ROBOT_CASH_FLOW, csCashStatus);
	}  
	   
    caCoinList.RemoveAll();
    caCashList.RemoveAll();           
}

bool SMState::CashRetrieveMap(CString csStrData,int nStringtype)  
{ 
    TCHAR* str = new TCHAR[csStrData.GetLength()+1]; 
    lstrcpy( str,csStrData );
    bool bCashUpdate = false;
    TCHAR * CashString = _tcstok (str,_T(","));
    while (CashString != NULL)
    {
        CString csToken(CashString);
        int count1 = csToken.GetLength();
        int mid2 = csToken.Find(_T(":"));
        CString CashDen =  csToken.Left(mid2);
        CString CashCount =  csToken.Mid( mid2+1, count1-mid2 );
        INT CMCashDen = _ttoi(CashDen);
        INT Value = _ttoi(CashCount);
        INT CMCashCount;
         
        switch(nStringtype) 
        {
            case 1:
		    {
                cmCashCounts_Coin.Lookup(CMCashDen,CMCashCount);
                if(CMCashCount != Value) 
                {
                    cmCashCounts_Coin[CMCashDen] = Value;
                    bCashUpdate = true; 
                }  
               break;
            }
            case 2:
		    {
                cmCashCounts_Bill.Lookup(CMCashDen,CMCashCount);
                if(CMCashCount != Value) 
                {
                    cmCashCounts_Bill[CMCashDen] = Value;
                    bCashUpdate = true;
                }  
               break;
            }
            case 3:
		    {
                cmAcceptCounts_Coin.Lookup(CMCashDen,CMCashCount);
                if(CMCashCount != Value) 
                {
                    cmAcceptCounts_Coin[CMCashDen] = Value;
                    bCashUpdate = true;
                }  
               break;
            }
            case 4:
		    {
                cmAcceptCounts_Bill.Lookup(CMCashDen,CMCashCount);
                if(CMCashCount != Value) 
                {
                    cmAcceptCounts_Bill[CMCashDen] = Value;
                    bCashUpdate = true;
                }  
               break;
            }
            default:
            {
               trace(L6, _T("Invalid String type"));               
            }
        }

        csToken.ReleaseBuffer();
        CashDen.ReleaseBuffer();
        CashCount.ReleaseBuffer();
        count1 = 0;
        mid2 = 0;

      CashString = _tcstok (NULL, _T(","));
    }
  return bCashUpdate;
}


void SMState::CashArray(CString CashLConfig,int csstringtype)  
{ 
  trace(L6, _T("CashLConfig = %s"),CashLConfig);
  TCHAR* Cashstr = new TCHAR[CashLConfig.GetLength()+1]; 
  lstrcpy( Cashstr,CashLConfig );
  
  TCHAR * ACashString = _tcstok (Cashstr,_T(","));
  while (ACashString != NULL)
    {
     CString csAToken(ACashString);
     INT CCashDen = _ttoi(csAToken);
      switch(csstringtype) 
      {
        case 1:
	    {
            caCoinList.Add(CCashDen);
            break;
        }
        case 2:
        {
            caCashList.Add(CCashDen);
            break;
        }
      }
      csAToken.ReleaseBuffer();
      ACashString = _tcstok (NULL, _T(","));
    }
}
//(-) POS244229

//Start CRD 145555
void SMState::sendFLMSTLog(int iStartEnd, int iPassFailCancel)	//0 = start, 1 = end ; 0 = Pass, 1 = Fail, 2 = Cancel
{
	CString csAuxMsg;
	
	if(iStartEnd == 0) //Start
	{
		csAuxMsg.Format(_T("<FLMSTrans><FLMS>%s</FLMS><Type>%s</Type><TrustLevel>%05d</TrustLevel></FLMSTrans>"), FLMS_START, csFLMSType, 0);
	}
	else if(iStartEnd == 1) //End
	{
			if(csFLMSType == FLMSTYPE_NORMAL)
			{
				csAuxMsg.Format(_T("<FLMSTrans><FLMS>%s</FLMS><Type>%s</Type><mPerksBarcode>%s</mPerksBarcode><TrustLevel>%05d</TrustLevel></FLMSTrans>"), FLMS_END, csFLMSType, csFLMSmPerksBarcode, 0);
			}
			else if (csFLMSType == FLMSTYPE_PARTIALRESCAN)
			{
				if(iPassFailCancel == 0)
					csFLMSAuditResult = FLMSAUDITRESULT_PASS;
				else if(iPassFailCancel == 1)
					csFLMSAuditResult = FLMSAUDITRESULT_FAIL;
				else if(iPassFailCancel == 2)
					csFLMSAuditResult = FLMSAUDITRESULT_CANCEL;

				//Start CRD 145557
				int nAuditListSize = SMState::csFLMSAuditItemList.GetSize();
				CString csItemsMsg = _T("");
				CString csItemsMsgList = _T("");

				int ctr = 0;
				bool bSendPart=true;
				while(bSendPart)
				{
					for(int n=0; n < 10; n++)
					{
						if(ctr < nAuditListSize)
						{
							csItemsMsg.Format(_T("<Item><UPC>%s</UPC><Price>%d</Price><Status>%s</Status><AccessMethod>%s</AccessMethod></Item>"), SMState::csFLMSAuditItemList[ctr]->UPC, SMState::csFLMSAuditItemList[ctr]->Price, SMState::csFLMSAuditItemList[ctr]->Status, SMState::csFLMSAuditItemList[ctr]->AccessMethod);
							csItemsMsgList += csItemsMsg;
							ctr++;
						}
						else // last items, exit loop and send LAST
						{
							bSendPart = false;
							break;
						}							
					}
					
					if(bSendPart) //send Part of the FLMS end, 
					{
						csAuxMsg.Format(_T("<FLMSTrans><FLMS>%s</FLMS><Type>%s</Type><mPerksBarcode>%s</mPerksBarcode><TrustLevel>%05d</TrustLevel><ServiceCheck><RecordNumber>%s</RecordNumber><Result>%s</Result><SessionID>%s</SessionID><CashierID>%s</CashierID><CSMinimumScanRequired>%s</CSMinimumScanRequired><StartTime>%s</StartTime><EndTime>%s</EndTime><Items>%s</Items></ServiceCheck></FLMSTrans>"), FLMS_END, csFLMSType, csFLMSmPerksBarcode, 0, FLMSAUDITRECORD_PART, csFLMSAuditResult, csFLMSSessionID, csFLMSOperatorID, csMinimumScanRequired, csFLMSAuditStartTime, csFLMSAuditEndTime, csItemsMsgList);
						trace(L6, _T("sendFLMSTLog - %s"), csAuxMsg);
						SendTBCommand(TBCMD_NOTIFY_FLMS, csAuxMsg);
						csItemsMsgList = _T(""); //clear the msg after sending
					}
					//continue loop
				}
				//End CRD 145557

				csAuxMsg.Format(_T("<FLMSTrans><FLMS>%s</FLMS><Type>%s</Type><mPerksBarcode>%s</mPerksBarcode><TrustLevel>%05d</TrustLevel><ServiceCheck><RecordNumber>%s</RecordNumber><Result>%s</Result><SessionID>%s</SessionID><CashierID>%s</CashierID><CSMinimumScanRequired>%s</CSMinimumScanRequired><StartTime>%s</StartTime><EndTime>%s</EndTime><Items>%s</Items></ServiceCheck></FLMSTrans>"), FLMS_END, csFLMSType, csFLMSmPerksBarcode, 0, FLMSAUDITRECORD_LAST, csFLMSAuditResult, csFLMSSessionID, csFLMSOperatorID, csMinimumScanRequired, csFLMSAuditStartTime, csFLMSAuditEndTime, csItemsMsgList);
			}
	}
	
	trace(L6, _T("sendFLMSTLog - %s"), csAuxMsg);
	SendTBCommand(TBCMD_NOTIFY_FLMS, csAuxMsg);
}
//End CRD 145555

void SMState::SetTransactionStatus(_bstr_t bstrTransactionStatus)
{
    m_bstrTransactionStatus = bstrTransactionStatus;
}

_bstr_t SMState::GetTransactionStatus()
{
    return m_bstrTransactionStatus;
}

//Start CRD 232268
void SMState::appendEASMsgToReceipt(long nEntryID)
{
	trace(L6, _T("SMState::appendEASMsgToReceipt(%d)"), nEntryID);

	CString csDesc, csItemEntryId, csMsgItemEntryId, csMsgItemEntryRapId,csReceiptSrc, csDescPrimary;
	COleVariant vValue, vOrigDesc, vOrigExtPrice;
	//cashier assistance required sub item text

	//saving of EAS item in respective nEntryID for voiding purposes
	bool bApprovedEASItem;
	if (!g_mapTransactionEASItem.Lookup( nEntryID, bApprovedEASItem))
	{
		g_mapTransactionEASItem.SetAt(nEntryID, false);
	}
	
	
	//for the CMSMReceipt
	csReceiptSrc = _T("CMSMReceipt");
	csDesc.Format(ps.GetPSText(SSF_TXT_EAS_ITEM_TAG, ps.m_languageUsed)); 
	csDesc =_T("  ") + csDesc;
	csItemEntryId.Format(_T("%d"), nEntryID);
	csMsgItemEntryId = csItemEntryId + _T("EASItem");
	
	vValue= csDesc;
	m_pAppPSX->CreateReceiptItem(csReceiptSrc, csMsgItemEntryId, csItemEntryId);        
	m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csMsgItemEntryId, _T("ITEM_MSG"), vValue);
	
	m_pAppPSX->UpdateReceiptControls(csReceiptSrc);
}

bool SMState::checkEASItemsList()
{
	int iValue;
	SendTBCommand(TBCMD_GET_EASLISTSIZE, &iValue);
				
	trace(L6, _T("SMState::checkEASItemsList() size - %d"), iValue);
	
	if(iValue > 0)
	{
		for(int index=0; index < iValue; index++)
		{
			//pass on index and get EntryID
			CString csValue;
			SendTBCommand(TBCMD_GET_EASENTRYID, &index, &csValue);
			
			//add Cashier Assistance Needed text to the entry IDs
			long lEntryId = _wtol(csValue);
			appendEASMsgToReceipt(lEntryId);
			addItemToEASREceipt(csValue);		

			CString csInterventionInstruction;
			csInterventionInstruction = GetTBProperty(_T("EASInterventionTitle"));
			
			CString csDataNeededCommand;
			csDataNeededCommand.Format(_T("MESSAGESCREENTITLE=%s;MESSAGESCREENTEXT=%s;MESSAGESCREENITEMNEEDCANCEL=0;MESSAGESCREENGOBACKSCANANDBAG=0;LaneButtonText=%s;DefaultCmdString=%s;TopCaption=%s;SummaryInstruction=%s;StoreLogon=1;HideTotal=1;ID=TB002;InterventionInstruction=%s;ReportCategoryName=%s;HandlingContext=SMDataEntryGoToLane"),
				_T(""),						/*MESSAGESCREENTITLE*/
				_T("$ScotApp_9512$"),                                /*MESSAGESCREENTEXT*/
				csInterventionInstruction,                    /*LaneButtonText*/
				_T("CmdBtn_HandleEASIntervention"),		/*DefaultCmdString*/
				_T("$ScotApp_9512$"),                               /*TopCaption*/
				_T("$ScotApp_9514$"),				/*SumaryInstruction*/  
				csInterventionInstruction,                                /*InterventionInstruction */
				_T("EAS Item"));				/*ReportCategoryName*/


			CString sDescription, sView, sINI, sViewString, sSection, csStateName;
			sDescription = _T("");
			sView = csDataNeededCommand;
			sINI = _T("");
			sSection = _T("");

			CDataNeededUtil dataNeededUtil(sDescription, sView, sINI, sSection);
			OnGenericDelayedIntervention(dataNeededUtil);
		}

		SendTBCommand(TBCMD_CLEAR_EASLIST, false);
		return true;
	}

	return false;
}

void SMState::addItemToEASREceipt(CString csEntryId)
{
	CString csDesc,csCode, csItemEntryId, csReceiptSrc, csMsgItemEntryId;
	long lQty,lWgt,lPrc,lUPrc, lTcd, lBarcodeType;
	COleVariant vValue, vOrigDesc, vOrigExtPrice;

	//get the item details first based on the entry ID from the POS
	ps.GetItem(csDesc,
		csCode,
		lQty,
		lWgt,
		lPrc,
		lUPrc,
		lTcd,
		lBarcodeType,
		_T("CMSMReceipt"),
		_T("CMReceiptVoid"),
		csEntryId);
		
	trace(L6, _T("SMState::addItemToEASREceipt - csCode - %s, csDesc - %s, csValue - %s"), csCode, csDesc, csEntryId);

	//Start CRD 212453 need to add the item to the EAS receipt
	csReceiptSrc = _T("CMSMReceiptEAS");
	csItemEntryId = csEntryId;
	
	PSXRC rc;
	
	rc = m_pAppPSX->CreateReceiptItem(csReceiptSrc, csItemEntryId);
	
	if ( PSX_SUCCESS != rc )
	{
		// kaboom!
	}
	
	//description of item

	vValue = csDesc;
	m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_DESCRIPTION"), vValue);
	vValue = csCode;
	m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_CODE"), vValue);
	vValue = lQty;
	m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_QUANTITY"), vValue, true); //true means this applies to all subitems also
	vValue = lWgt;
	m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_WEIGHT"), vValue); 
	vValue = lTcd;
	m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_TARECODE"), vValue);
	vValue = lPrc;
	m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_EXT_PRICE"), vValue);
	vValue = lUPrc;
	m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_PRICE"), vValue);
	vValue = lBarcodeType;
	vValue = sEASItem.lSecurityTag;
	m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SECURITY_TAG"), vValue);
	vValue = VARIANT_TRUE;
	m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SELECTABLE"), vValue);
	

	csDesc.Format(ps.GetPSText(SSF_TXT_EAS_ITEM_TAG, ps.m_languageUsed)); 
	csDesc =_T("  ") + csDesc;

	csMsgItemEntryId = csItemEntryId + _T("EASItem");
	
	vValue= csDesc;
	bool bValue = false;
	m_pAppPSX->CreateReceiptItem(csReceiptSrc, csMsgItemEntryId, csItemEntryId); 

	vValue= csDesc;      
	m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csMsgItemEntryId, _T("ITEM_MSG"), vValue);
	m_pAppPSX->UpdateReceiptControls(csReceiptSrc); 
	//End CRD 212453 
}

bool SMState::checkMobileAssistanceNeededItems()
{
	int iValue;
	SendTBCommand(TBCMD_GET_MOBILEASSISTANCENEEDED, &iValue);
				
	trace(L6, _T("SMState::checkMobileGenericDelayedItems() - %d"), iValue);
	if(iValue > 0)
	{
		for(int index=0; index < iValue; index++)
		{
			CString csInterventionInstruction = GetTBProperty(_T("MobileAssistanceInterventionTitle"));
			
			CString csDataNeededCommand;
			csDataNeededCommand.Format(_T("MESSAGESCREENTITLE=%s;MESSAGESCREENTEXT=%s;MESSAGESCREENITEMNEEDCANCEL=0;MESSAGESCREENGOBACKSCANANDBAG=0;LaneButtonText=%s;DefaultCmdString=%s;TopCaption=%s;SummaryInstruction=%s;StoreLogon=1;HideTotal=1;ID=TB001;InterventionInstruction=%s;ReportCategoryName=%s;CONTEXTTODISPLAY=SmDataEntrySmall;HandlingContext=SMDataEntryGoToLaneOrRap"),
				_T("$ScotApp_9516$"),						/*MESSAGESCREENTITLE*/
				_T("$ScotApp_9516$"),                                /*MESSAGESCREENTEXT*/
				csInterventionInstruction,                    /*LaneButtonText*/
				_T("CmdBtn_HandleMobileGenericDelayedIntervention"),		/*DefaultCmdString*/
				_T("$ScotApp_9516$"),                               /*TopCaption*/
				_T("$ScotApp_9514$"),				/*SumaryInstruction*/  
				csInterventionInstruction,                                /*InterventionInstruction */
			_T("Mobile Assistance Needed"));


			
			CString sDescription, sView, sINI, sViewString, sSection, csStateName;
			sDescription = _T("");
			sView = csDataNeededCommand;
			sINI = _T("");
			sSection = _T("");
			
			CDataNeededUtil dataNeededUtil(sDescription, sView, sINI, sSection);
			OnGenericDelayedIntervention(dataNeededUtil);
		}
		
		SendTBCommand(TBCMD_CLEAR_MOBILEASSISTANCENEEDED, false);
		return true;
	}
	
	return false;
}
//CRD 232268

//Start CRD 291249 - move this from Scan and Bag
// SOTF6400
bool SMState::DigitalCouponActiveWithNoItems (void)
{
	bool digitalCouponActive = false;
	CString csPPAProperty = _T("DigitalCouponActive");
	CString csPPAValue;
	SendTBCommand(TBCMD_GET_TBPROPERTY, &csPPAProperty, &csPPAValue);
	
	if(_T("1") == csPPAValue)
	{
		digitalCouponActive = true;
	}
	else	
	{
		//check if mPerks applied Offline		
		CString digitalCouponAppliedOfflineName = _T("DigitalCouponAppliedOffline");
		CString digitalCouponAppliedOfflineValue;
		SendTBCommand(TBCMD_GET_TBPROPERTY, &digitalCouponAppliedOfflineName, &digitalCouponAppliedOfflineValue);
		digitalCouponActive = _T("1") == digitalCouponAppliedOfflineValue;
	}

    /* POS61464 - The logic to check whether there is an active item on the transaction should be the same whether the MobileShopper feature is turned on/off.
        Removed the logic below that would check the 'EnableMobileShopper' option and just use the same logic - rrs
   /*POS11024 Mobile Shopper Start
   CString strSectionName = _T("MOBILESHOPPER");
   CString strEntryName = _T("EnableMobileShopper");
	CString strMobileShopperFeature = _T("");
	SendTBCommand(TBCMD_GET_APPINI_ENTRY_VAL, (const TCHAR *)strSectionName, 
					  (const TCHAR *)strEntryName, &strMobileShopperFeature);
    
   //Removed check for IsResumeComplete for resumes if mobile shopper is ON
   //Sometimes, ereceipt is not populated (lItems is zero) but balance is not
   //IsThereItemNeedToDisplayed will determine whether trx is truly empty or not
   //We do not want to cancel the trx if after mobile resume, balance is non zero   
	if( strMobileShopperFeature == _T("1") )
   {
      //If this is a resume, we may be resuming a mobile order
      //where mPerks # would have been itemized and ereceipt not yet populated         
      return ( GetTBProperty(_T("IsThereItemNeedToDisplayed")) == _T("0") && (0 == lItems) 
               && (_T("1") == csPPAValue) && (lBalanceDue <= 0) );
   }
   else
   {       
	   return ( (0 == lItems) && (_T("1") == csPPAValue));
   }
   /*POS11024 Mobile Shopper End
    POS61464 End rrs */

	//CRD 286688 - also check if there's any interventions required even if trx is empty due to supplemental barcode items.
	//(Supplemental items from mobile transfer are added after clearing the delayed interventions).
	//CRD 297752 - check if shipt barcode is scanned and there are no items in the transaction
	CString csShiptBarcodeScanned = GetHookProperty(_T("ShiptBarcodeScanned"));

	return (GetTBProperty(_T("IsThereItemNeedToDisplayed")) == _T("0")
		&& (0 == lItems)
		&& (digitalCouponActive || csShiptBarcodeScanned == _T("1"))
		&& (lBalanceDue <= 0)
		&& !isAnyApprovalRequired(false));
}

//+SOTF 7555
bool SMState::IsTrxnCompletelyClean(void)
{
    //POS34664 - first just check if the lItems var has value on it.  if it has, then don't bother to check anything else
    if( lItems != 0 )
    {
        return false;
    }
    // e pos34664 - rrs
	
    CStringArray csItemEntryIdList;
	
    // POS 10178 - Instead of monitoring the CMSmallReceipt, we should monitor the CMSMReceipt. 
    // CMSmallReceipt is cleared everytime the Remote Assist Mode is requested - RRS
    m_pAppPSX->FindReceiptItems(_T("CMSMReceipt"), csItemEntryIdList);
    int nCountList = csItemEntryIdList.GetSize();
    //there's nothing on the e-receipt (no items were scanned or entered) and there
    //are no pending interventions.  Then their is no need to press the payment
    //button on the POS.  Just end the transaction and this will keep the PIN pad 
    //at the Meijer logo
	
    //POS34664 - The CMSMReceipt might have the non-item receipt entries for the RAP intervention, so we would need to ignore those entries also - rrs
    CString csTmpEntryId(_T(""));
    bool bReceiptEmpty = true;    
	
    for (int i=0; i<nCountList; i++)
    {
		csTmpEntryId = csItemEntryIdList.GetAt(i);
		
		//ingore non item receipt entries
		if ( (csTmpEntryId.Find(_T("RCM")) == -1 ) )
		{
			bReceiptEmpty = false;
			trace(L6, _T("Real Item found in the CMSMReceipt, trx is not empty"));
			continue;
		}
    }
    // e POS34464 - rrs
	
	//POS34464 uses bReceiptEmpty variable - rrs
    if( (bReceiptEmpty) && (!isAnyApprovalRequired(false)))  
    {   
        // POS 10303 - Trx is dirty if the transmode=WIC MODE - rrs
        CString csTransMode = GetHookProperty(_T("PropTransMode"));
		
        // check if this property as for resumed transaction, there is nothing in the receipt yet - rrs
        //POS240201 use wicmode bmp - rrs
        if(  GetTBProperty(_T("IsThereItemNeedToDisplayed")) == _T("0") && (csTransMode.Find(_T("wicmode")) == -1) )
			return true;
		
    }
    return false;
}
//-SOTF 7555
//End CRD 291249

//Start CRD 313028
void SMState::LoadEverseenOptions()
{
	CString csEverseenEnabled = GetHookProperty(_T("EverseenEnabled"));
	trace(L6, _T("LoadEverseenOptions - Everseen enabled = %s"), csEverseenEnabled);
	if(csEverseenEnabled == _T("1"))
	{
		//1. Disable security if Everseen if enabled
		ra.OnDisableSecurity();
		if(g_bIsSecurityDisableSubCheckOnly)
		{	
			co.SetSecurityDisableSubCheck(true);
			SetSecurityConfig(CONFIG_KEY_IS_SECURITYDISABLE_SUB_CHECK_ON, _T("true"));	
		}
		else
		{	
			co.SetSecurityDisable(true);
			co.fStateSecurityDisableNoReset = true;
							
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
//End CRD 313028

bool SMState::IsShiptBarcode(CString barcode)
{
	if(barcode.IsEmpty())
		return false;
	if(barcode.GetLength() < 1)
		return false;

	if(SMState::ShiptBarcodePrefix.IsEmpty())
	{
		CString strOptionName = _T("ShiptBarcodePrefix");
		SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &SMState::ShiptBarcodePrefix);
		trace(L6, _T("ShiptBarcodePrefix: %s"), SMState::ShiptBarcodePrefix);
	}
	if(SMState::ShiptBarcodeSuffix.IsEmpty())
	{
		CString strOptionName = _T("ShiptBarcodeSuffix");
		SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &SMState::ShiptBarcodeSuffix);
		trace(L6, _T("ShiptBarcodeSuffix: %s"), SMState::ShiptBarcodeSuffix);
	}
	if(barcode.Mid(0,SMState::ShiptBarcodePrefix.GetLength()) == ShiptBarcodePrefix && barcode.Mid(barcode.GetLength() - SMState::ShiptBarcodeSuffix.GetLength(), SMState::ShiptBarcodeSuffix.GetLength()) == ShiptBarcodeSuffix)
		return true;
	else
		return false;
}

void SMState::NotifyExitStoreMode()
{
	if(getAnchorState() == BES_SCANANDBAG)
	{
		SendTBCommand(TBCMD_NOTIFY_EXIT_STOREMODE_TO_SCANANDBAG);
	}
	else if(getAnchorState() == BES_FINISH)
	{
		SendTBCommand(TBCMD_NOTIFY_EXIT_STOREMODE_TO_PAYMENT);
	}
}

// Start CRD 541144
void SMState::SendCitiAccountDetailsToPOS(CString csRefNumber, CString csCaseNumber, long lCreditLimit, int iStatus, int iUseApprovedCard)
{

	//	For some reason SendTBCommand is failing for many arguments, hence building the AuxMsg in SSF
	//  <AuxMsg MsgSeq="1">	
	//	<CitiApplication RefID='Ijr7utxAnkdPeqG9S2Ge47BiSt2SzwbB2JvZNJA18brGkhzMTr1qhwWtMJzeX7PY'	
	//			CaseNumber='060720ZG0K000'		
	//			CreditLimit='30000'	
	//			Status='0'
	//			Use = '1' />	
	//	</AuxMsg>
				
	//Note:
	//Status='0' -> Citi Application was completed/approved or it was declined by Citi Bank.
	//	(A decline is known because the RefID and CreditLimit values will be empty.)
	//	(A decline however will have a CaseNumber value.)
	//	Status='1' -> Citi Application canceled by customer before final approval.
	//	Use='0'     -> Do not use the approved account in the current transaction.
		//Use='1'     -> Use the approved account in the current transaction.

	trace(L6, _T("SendCitiAccountDetailsToPOS - %s, %s, %d, %d, %d"), csRefNumber, csCaseNumber, lCreditLimit, iStatus, iUseApprovedCard);

	CString csAuxMsg;
	csAuxMsg.Format(_T("<CitiApplication RefID=\"%s\" CaseNumber=\"%s\" CreditLimit=\"%d\" Status=\"%d\" Use=\"%d\"/>"), csRefNumber, csCaseNumber, lCreditLimit, iStatus, iUseApprovedCard);

	SendTBCommand(TBCMD_SEND_CITI_ACCOUNT_DETAILS, csAuxMsg);
}
// End CRD 541144

// Start CRD 557225
bool SMState::ProcessCitiApplicationDetails()
{
	bool bNeedProcessing = false;
	if(sCitiAccountDetails.bSend)
	{
		if(AreThereAnySoldVoidedItems())
		{
			SendCitiAccountDetailsToPOS(sCitiAccountDetails.csRefNumber, sCitiAccountDetails.csCaseNumber, sCitiAccountDetails.lCreditLimit, sCitiAccountDetails.iStatus, sCitiAccountDetails.iUseApprovedCard);
			sCitiAccountDetails.bSend = false;
			
			// CRD 554915 account look up is only done if Status is Complete, with Reference Number and customer has opted to use approved card
			if(sCitiAccountDetails.iStatus == 0 )
			{
				if(sCitiAccountDetails.csRefNumber != _T("") && sCitiAccountDetails.iUseApprovedCard == 1)
				{
					if(getAnchorState() == BES_SCANANDBAG)
					{
						SendTBCommand(TBCMD_SET_TB_STATE,_T("ACCOUNT_LOOKUP_PROCESS_ITEMIZATION"));
					}
					else if(getAnchorState() == BES_FINISH)
					{
						SendTBCommand(TBCMD_SET_TB_STATE,_T("ACCOUNT_LOOKUP_PROCESS_PAYMENT"));
					}
					
					bNeedProcessing = true;
				}
			}
			else if(sCitiAccountDetails.iStatus == 3)
			{
				//trigger delayed intervention
				SendTBCommand(TBCMD_SET_TB_STATE, _T("GET_ACCOUNT_DETAILS_FAILED"));
				bNeedProcessing = true;
			}
		}

		// Start CRD 537490
		if(sCitiAccountDetails.iStatus == 0 )
		{
			LoadApplyMeijerCardOptions(true);
		}
		// End CRD 537490
	}

	return bNeedProcessing;
}
// End CRD 557225

// Start CRD 564271
void SMState::LoadApplyMeijerCardOptions(bool bApplyMeijerComplete)
{
	CString csApplyMeijerCardEnabled = GetTBProperty(_T("ApplyMeijerCardEnabled"));
	CString csApplyMeijerCardOnAttract = GetTBProperty(_T("ApplyMeijerCardOnAttract"));
	CString csApplyMeijerCardOnSNB = GetTBProperty(_T("ApplyMeijerCardOnSNB"));
	CString csApplyMeijerCardOnPayment = GetTBProperty(_T("ApplyMeijerCardOnPayment"));
	CString csApplyMeijerCardRedisplay = GetTBProperty(_T("ApplyMeijerCardRedisplay"));

	trace(L6, _T("LoadApplyMeijerCardOptions - Apply Meijer Card enabled = %s, on %s, %s, %s; redisplay - %s"), csApplyMeijerCardEnabled, csApplyMeijerCardOnAttract, csApplyMeijerCardOnSNB, csApplyMeijerCardOnPayment, csApplyMeijerCardRedisplay);

	// Start CRD 537490 if complete and configured to NOT redisplay, force disable it
	// if not yet complete and configure to NOT redisplay, do nothing
	if(csApplyMeijerCardRedisplay == _T("0") && bApplyMeijerComplete)
	{
		csApplyMeijerCardEnabled = _T("0");
	}
	// End CRD 537490
	
	// Start CRD 525299 - tell NGUI if AllowApplyMeijerCard option is enabled/disabled
	m_pAppPSX->SetTransactionVariable(_T("AllowApplyMeijerCard"), csApplyMeijerCardEnabled);

	// Tell which state should ApplyMeijerCard button is displayed

	CString csAllowMeijerCardScreens = _T("");
	csAllowMeijerCardScreens.Format(_T("%s,%s,%s"), csApplyMeijerCardOnAttract, csApplyMeijerCardOnSNB, csApplyMeijerCardOnPayment);

	m_pAppPSX->SetTransactionVariable(_T("AllowApplyMeijerCardScreens"), csAllowMeijerCardScreens);
	// End CRD 525299
}
// End CRD 564271