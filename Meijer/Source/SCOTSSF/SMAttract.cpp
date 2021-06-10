//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMAttract.CPP
//
// TITLE: Class implementation for Attract State
//
// $Header: /Fastlane/4.0 Patch B/Core/Source/SCOTSSF/SMAttract.cpp 1     1/26/05 1:35p Bd123405 $
//
// AUTHOR:  Feng Zhang
//
// CHANGE HISTORY:
//
// CRD 564271 Name: CJ Estrada Date: Mar 9, 2021
// CRD 537481 Name: CJ Estrada Date: Mar 5, 2021
// CRD 557225 Name: CJ Estrada Date: Mar 4, 2021
// CRD 554916 Name: CJ Estrada Date: Feb 9, 2021
// CRD 525299 Name: CJ Estrada Date: Nov 17, 2020
// POS244229 Work Request:73594 Name:Aparna Tunuguntla Date: June 23, 2016
// POS97087 Work Request:68941 Name:D.Veerasekhara Date:June 03,2016
// POS240201 Work Request:68942 Name:Robert Susanto Date: January 7, 2016
// POS189696 Work Request:68237 Name:Saiprasad Srihasam Date: April 14, 2015
// POS143695 Work Request:66559 Name: Kranthi Kumari Cherugondi   Date:February 10, 2015
// POS138637 Work Request:66559 Name:Kranthi Kumari Cherugondi Date:March 3, 2015
// POS138636 Work Request:66559 Name:Saiprasad Srihasam    Date:December 04, 2014
// POS145373 Work Request:66559 Name: Robert Susanto   Date:November 20, 2014
// POS78133 Work Request:58657 Name: Matt Condra   Date:October 7, 2013
// POS48277/71132 Work Request:58657 Name: Robert Susanto   Date:September 13, 2013
// POS43685 Work Request:52635 Name:Robert Susanto Date:October 5, 2012
// POS37599 WORK REQUEST:50864 Name: Robert Susanto Date: June 27, 2012
// POS34070 WORK REQUEST:19114 Name: Robert Susanto Date: May 2, 2012
// POS33581 WORK REQUEST:19114 Name: Robert Susanto Date: April 25, 2012
// POS29719 WORK REQUEST:19114 Name: Matt Condra    Date: April 20, 2012
// POS24632 WORK REQUEST:19114 Name: Robert Susanto Date: March 22, 2012
// POS29349 WORK REQUEST:19114 Name: Matt Condra    Date: March 8, 2012
// POS29826 WORK REQUEST:16656 Name: Robert Susanto Date: February 21, 2012
// POS26005 WORK REQUEST:16656 Name: Robert Susanto Date: February 1, 2012
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"                // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes


#include "SMAttract.h"
#include "TransbrokerSSF.h"
#include "SMInprogress.h"
#include "SMScanReceipt.h"
#include "SMSecUnexpectedIncrease.h"
#include "SMState.h"
#include "SMLaneClosed.h"
#include "SMScaleBroken.h"
#include "LaneStateData.h" // SSCOI-45354 / CRD128616 uncomment this
#include "SMShowExternalUrl.h"	//CRD 525312

#include "SMCustomMessage.h"

#define COMP_ID ID_SM               // base state component
#define T_ID     _T("Attract")
#include "NCRScan.h"

//UploadTicket Port +
#include "MobileConstantsSSF.h"  
#include "MPProcedures.h"
DLLIMPORT extern MPProcedures mp;
//UploadTicket Port -

DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMAttract, CObject)// MFC Runtime class/object information

//////////////////////
// Constructor
//////////////////////
SMAttract::SMAttract()
{
	m_bIsFromLaneClosed = false;
	m_nLangSelectionPressed = 0;

  IMPLEMENT_TIMESTAMP
		// We have started the application successfully if we get here, so we can
		// reset the counter used to restart SCOTIS during error conditions.
		// Nothing we can do with the return code.
}

#ifndef _TRADESHOWDEMO
SMStateBase * SMAttract::Initialize(void)
{
	SMState::nVoidCounts = 0;        //VSTS 131568
	SMState::m_bCustomVoidItemMessage = false;  //VSTS 131568
	// Start CRD126015
	
	// Tell POS transaction mode is SSCO
	// At this point there is no need to Tell POS the mode is anything else other then SSCO or SnD.
	// SSCO=0, SnD=1, FLMS=2 and SnD+FLMS=3 
	SendTBCommand(TBCMD_NOTIFY_TRANSACTIONMODE, 0);
	
	// Only need to tell POS that transaction is a SnD transaction for now.
	// if TransactionMode = SnD set m_csSNDFLConfig true and don't use  ACSOPT.dat option ShopAndDropEnabled any more
	//    
	// if TransactionMode = SnD & FLMs using code below and set m_csSNDFLConfig true when where SnD button pressed is defined.
	//
	// Need to add SnD trans to code.

	// start RFQ 9554
	// CString strSNDOptionName = _T("ShopAndDropEnabled");
	m_csTransactionMode = _T("");	//crd126015
	CString csTransactionModeOptionName = _T("TransactionMode");
    SendTBCommand(TBCMD_GET_OPTIONS, &csTransactionModeOptionName, &m_csTransactionMode);
	
	COleVariant var;
	
	m_bShopAndDropEnabled = false;
	m_pAppPSX->SetTransactionVariable(_T("ShopAndDropEnabled"), VARIANT_FALSE); 
	SetHookProperty(_T("IsSND"),_T("0")); //CRD 157372
	// if ((m_csSNDFLConfig == _T("Y")) || (m_csSNDFLConfig == _T("y")))
	if (m_csTransactionMode == TRANSMODE_SND) // or if SnD pressed if FLMS+SnD screen display **** Need to ADD ****
	{
		//end CRD126015
		SetHookProperty(_T("IsSND"),_T("1")); //CRD 157372
		m_bSNDAllowItemMenu = false;
		m_bShopAndDropEnabled = true; 
		m_bSNDExitedAM = false;			
		m_bSNDSettingAge = false;
		m_bSNDxCancelTrans = false;		// CRD 125377
		m_bSNDBackToFnP = false;
	
		SetHookProperty(_T("SuspendingCurrentTransaction"), _T("0"));

		var = _T("0");
		m_pAppPSX->SetTransactionVariable(_T("WasOnTransactionMode3"), var);

		var = TRANSMODE_SND;
		m_pAppPSX->SetTransactionVariable(_T("TransactionMode"), var);
		m_pAppPSX->SetTransactionVariable(_T("ShopAndDropEnabledYesNoButtons"), VARIANT_FALSE); 

		CString strSNDName = _T("SDNRestrictedAgeTimer");
		CString m_csSNDConfig;
		SendTBCommand(TBCMD_GET_OPTIONS, &strSNDName, &m_csSNDConfig);
		lSDNRestrictedAgeTimerAmt = _ttol(m_csSNDConfig);
		
		// Tell POS transaction mode is SND
		SendTBCommand(TBCMD_NOTIFY_TRANSACTIONMODE, 1); // end CRD126015
	}
	else if(m_csTransactionMode == TRANSMODE_MEIJERGO) // CRD125378
	{
		var = _T("0");
		m_pAppPSX->SetTransactionVariable(_T("WasOnTransactionMode3"), var);

		var = TRANSMODE_MEIJERGO;
		m_pAppPSX->SetTransactionVariable(_T("TransactionMode"), var);

		SendTBCommand(TBCMD_NOTIFY_TRANSACTIONMODE, 0);
	}
	else if(m_csTransactionMode == TRANSMODE_MEIJERGO_SND) // CRD125378
	{
		var = _T("1");
		m_pAppPSX->SetTransactionVariable(_T("WasOnTransactionMode3"), var);
		var = TRANSMODE_MEIJERGO_SND;
		m_pAppPSX->SetTransactionVariable(_T("TransactionMode"), var);
		
		SendTBCommand(TBCMD_NOTIFY_TRANSACTIONMODE, 0);
	}
	else	//default 0 = SSCO only
	{
		var = TRANSMODE_SCO;
		m_pAppPSX->SetTransactionVariable(_T("TransactionMode"), var);
		SendTBCommand(TBCMD_NOTIFY_TRANSACTIONMODE, 0); // CRD126015
	}
	//trace(L6,_T("m_bShopAndDropEnabled %d m_csSNDFLConfig %s"),m_bShopAndDropEnabled, m_csSNDFLConfig);
	// end CRD 125377
	// End CRD126015

	// Start CRD 525299 564271 - tell NGUI if AllowApplyMeijerCard option is enabled/disabled
	LoadApplyMeijerCardOptions(false);
	// End CRD 525299 564271

    // +POS138637
    SMState::lWICOptionloaded = 0;
	if ((SMState::lWICOptionloaded == 0))
    {
		//Start CRD 345698	
        //CString strSectionName = _T("TEXAS_WIC");
        //CString strEntryName = _T("TexasWICOn"); 
        CString strTexasWIC;
        //SendTBCommand(TBCMD_GET_APPINI_ENTRY_VAL, (const TCHAR *)strSectionName, (const TCHAR *)strEntryName, &strTexasWIC);
		strTexasWIC = GetHookProperty(_T("IsTEXAS_WIC_ON"));
		//End CRD 345698

        trace(L6, _T("Texas WIC On status %s"), strTexasWIC);
        if (strTexasWIC == _T("0"))
		{
			SMState::lWICOptionloaded = 1;
		}
        else
		{
			SMState::lWICOptionloaded = 2;
		}
	}
    // +POS138637

//SSCOI-47468
if (co.IsAttendantModeOn() && fRAPIsInStoreMode && !storeMode() && !co.IsInAssistMenus() && !ps.RemoteMode())
{
	fRAPIsInStoreMode = false;

	trace(L6, _T("+ra.OnExitStoreMode in SMAttract::Initialize"));
	ra.OnExitStoreMode();
	trace(L6, _T("-ra.OnExitStoreMode"));
}
//SSCOI-47468

CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMAttract::Initialize"));
TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("Attract"));

//+POS97087
if (true == SMState::m_bEODEnable)
{
	fInSignedOff = false;
	SMState::m_bEODEnable = false;
}//+POS97087

SMState::m_bWICEBTSNBEnable = true;    // POS138636

									   //SOTF 7695
SendTBCommand(TBCMD_ASSISTMENUSET, false);

//+SOTF 5572
CString csMediaReport = rp.GetGurrencyList();

if (!csMediaReport.IsEmpty())
{
	SendTBCommand(TBCMD_ROBOT_CASH_FLOW, csMediaReport);
}
//-SOTF 5572

//(+) POS244229
if ((Is5XHardware() || !_tcsicmp(co.csHWType, _T("SCOT6"))) && GetHookProperty(_T("PropWasTenderAccepted")) != _T("0"))
{
	trace(L6, _T("Cash Monitoring Counts"));
	CashMonitoringCounts(1); //1 - in transaction

	SetHookProperty(_T("PropWasTenderAccepted"), _T("0"));
}
//(-) POS244229

// POS29719: Notify sales app that previous transaction has finalized so that
// the sales app can clean up in cases where the previous transaction was a
// non-item transaction containing an invalid mPerks number.
SendTBCommand(TBCMD_NOTIFY_EOT, true);

// POS29349: Initially, let sales app know that customer has not brought their own shopping bag.
SendTBCommand(TBCMD_NOTIFY_BROUGHT_BAG, false);

m_bAssistModeReturnAfterSystemMessage = false;
bTransactionComplete = false;
SetTBProperty(_T("IsThereItemNeedToDisplayed"), _T("0")); // TAR 398763

														  // +TAR 395064
if (dm.fStateTakeawayBelt)
{
	trace(L6, _T("Setting TABstateBeforeTransport = %d in Attract"), BES_EMPTY_STATE);
	TABstateBeforeTransport = BES_EMPTY_STATE;
}
// -TAR 395064    

// POS29826 - Check the var "fSAInTransaction" if the customer has pressed the WIC EBT button to start the WIC Transaction. - rrs
// The POS has a problem where it sends the invalid WIC MODE status after voiding the WIC Transaction that caused FL to send the PSButton1
// automatcially on the Attract screen for the next trx.  
//It successfully validated the WIC card, so start the transaction now.

//POS240201 - use the bitmap wicmode - rrs
if (fSAInTransaction && GetHookProperty(_T("PropTransMode")).Find(_T("wicmode")) != -1)
{
	return PSButton1();
}

CString csScaleThreshold = GetTBProperty(_T("ScaleThreshold"));
int nScaleThreshold = 0;
if (csScaleThreshold != _T(""))
nScaleThreshold = _ttoi(csScaleThreshold);

//TAR349048 
//for any reason, if remote devices are still enabled, we need to disable them now
// ps.RemoteMode() can return false even if devices are still remoted, that's why
// to be sure, we need to check with the device manager if the printer & cash drawer are remote.
if (ps.RemoteMode() ||
	dm.IsDeviceRemote(DMCLASS_RECEIPT, 0) ||
	dm.IsDeviceRemote(DMCLASS_CASHDRAWER, 0))
{
	// TAR 249091 - need to hide these controls, otherwise, other lanes may see them for a short time remotely.
	m_pAppPSX->SetConfigProperty(_T("ButtonToggle"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
	for (int i = 0; i < 8; i++)
	{
		CString csTemp = EMPTY_STRING;
		csTemp.Format(_T("ButtonLaneStatus%d"), i);
		m_pAppPSX->SetConfigProperty(csTemp, csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
	}
	trace(L6, _T("SMAttract::Initialize() Disconnecting remoted devices."));
	dm.EndRemote();
}
//TAR349048-
bool bIsItFromLaneClosed = (GetTBProperty(_T("NewFLState")) == _T("SMLaneClosed"));

//TAR345750
SetTBProperty(_T("NewFLState"), _T("SMAttract"));

//TAR348914+
//TB changed the FL state to LaneClosed but the base has a default return anchorestate to Attract,
//so we are going to change it back here to LaneClosed.
if (GetTBProperty(_T("SavedLastPosState")) == _T("SIGNIN_MENU") && bIsItFromLaneClosed)
{
	RETURNSTATE(LaneClosed);
}
//TAR348914-

m_bIsFirstTimedOut = true;
//TAR335785+
if (g_pDelay_WaitForApprovalStateChange != NULL && m_bMJWtIncreaseAtSmCmDataEntry)
{
	delete g_pDelay_WaitForApprovalStateChange;
	g_pDelay_WaitForApprovalStateChange = NULL;
	m_bMJWtIncreaseAtSmCmDataEntry = false;
	trace(L6, _T("Go to state saved in g_pDelay_WaitForApprovalStateChange"));
	CREATE_AND_DISPENSE(SecUnExpectedIncrease)(BES_START, BES_START);
}
//TAR335785-

//mini & super mini paystation
SMStateBase  *pRetState = STATE_NULL;
m_csFLConfig = GetTBProperty(_T("FastLaneConfiguration"));
SetHookProperty(_T("PropFromRTS"), _T("1"));
SetTBProperty(_T("PropFromAttract"), _T("1"));
m_lLastSuspendTotal = 0;
m_bMJPlayedWelcome = false;
m_bMJPlayedUsePinpad = false;
if (m_lTBCouponCount)
{
	CString csLogCouponEntries;
	csLogCouponEntries.Format(_T("CouponCount=\"%d\" CouponAmount=\"%s\" MediaType=\"Coupon\" "),
		m_lTBCouponCount,
		ps.FmtDollar(m_lTBCouponAmount, (enum PSProceduresBase::SCREEN_POSITION) 0, ps.REPORTINGFORMAT));
	rp.TBCreateReportEntry(_T("MCoupon"), csLogCouponEntries);
	trace(L6, _T(" Log Coupons %s"), csLogCouponEntries);
}
m_lTBCouponCount = 0;
m_lTBCouponAmount = 0;


m_bResendFLMSitem = false; // RFQ 9545 need to resend FLMS item

if (m_nLangSelectionPressed == 0)
{
	pRetState = SMAttractBase::Initialize();
}
else
{
	pRetState = STATE_NULL;
}

//TAR355534
if (lDMScaleWeight<0 || lDMScaleWeight>nScaleThreshold)
{
	ResetAllTransactionValues();	    // ensure all counters. totals, flags are reset
	delete getRealStatePointer(pRetState);

	// POS43685 - set the base flag so that upon return from scalebroken state, the customer language selected can be displayed - rrs
	//m_bSecViolationFromAttract = true;

	RETURNSTATE(ScaleBroken)
}

COleVariant v = GetTBProperty(_T("CurrentBuildNumber"));
m_pAppPSX->SetTransactionVariable(_T("CMVersionText"), v);

// POS145373 - Displaying additional information ACSVersion and Term no on Attract
CString csACSVersion(_T(""));
csACSVersion.Format(_T("v%s"), GetHookProperty(_T("ACSVersion")));
v = csACSVersion;
m_pAppPSX->SetTransactionVariable(_T("CMACSVersionText"), v);

CString csTerminalNumber(_T(""));
csTerminalNumber.Format(_T("Tm: %d"), TBGetTerminalNumber());
v = csTerminalNumber;
m_pAppPSX->SetTransactionVariable(_T("CMTerminalNumberText"), v);
// e - POS145373 - Displaying additional information ACSVersion and Term no on Attract

//Refresh the screen display
RefreshUI();

m_bIsFromScanReceipt = false;
if (m_csFLConfig == _T("SuperMini"))
{
	ps.Button(_T("CmdBtn_PriceInquiry"), PS_NOTEXT, false); //hide price inquiry button
	ps.Button(_T("CMButton7"), PS_NOTEXT, false); //finish and pay
}

SetDMCashTakenInSecState(false);
SetTimeoutInTakeChangeState(false);
SetChangeDispensedState(false);

//SOTF 7745
if (co.IsShowVolumeControl())
{
	m_pAppPSX->SetConfigProperty(_T("ButtonVolumeControl"), _T(""), UI::PROPERTYVISIBLE, VARIANT_FALSE);
	m_pAppPSX->SetConfigProperty(_T("CmdBtn_ButtonVolumeControl"), _T(""), UI::PROPERTYVISIBLE, VARIANT_TRUE);
}
else
{
	m_pAppPSX->SetConfigProperty(_T("ButtonVolumeControl"), _T(""), UI::PROPERTYVISIBLE, VARIANT_FALSE);
	m_pAppPSX->SetConfigProperty(_T("CmdBtn_ButtonVolumeControl"), _T(""), UI::PROPERTYVISIBLE, VARIANT_FALSE);

}
//sotf

//POS34070 - Reenable/redisplay the mPerks button at Attract and ScanAndBag screen if the mPerks function is turned on at POS - rrs
if (_T("1") == GetHookProperty(_T("EnableDigitalCoupons")))
{
	// Enable incase it was disabled
	m_pAppPSX->SetConfigProperty(_T("CMButton4"), _T("Attract"), UI::PROPERTYVISIBLE, VARIANT_TRUE);

	//POS33288 Use mPerks Button Video contrl to display the shimmering mperks effect - rrs
	
	m_pAppPSX->SetConfigProperty(_T("CMButton4"), _T("Attract"), UI::PROPERTYSTATE, (long)UI::Normal);
	m_pAppPSX->SetConfigProperty(_T("mPerksButtonVideo"), _T("Attract"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
	
	// e POS33288		
}
else
{
	m_pAppPSX->SetConfigProperty(_T("CMButton4"), _T("Attract"), UI::PROPERTYSTATE, (long)UI::Disabled);
	m_pAppPSX->SetConfigProperty(_T("mPerksButtonVideo"), _T("Attract"), UI::PROPERTYVISIBLE, VARIANT_FALSE);

	m_pAppPSX->SetConfigProperty(_T("CMButton4"), _T("Attract"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
	//POS33288 Use mPerks Button Video contrl to display the shimmering mperks effect - rrs
	m_pAppPSX->SetConfigProperty(_T("mPerksButtonVideo"), _T("Attract"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
	// e POS33288

	m_pAppPSX->SetConfigProperty(_T("CMButton7"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
	m_pAppPSX->SetConfigProperty(_T("mPerksButtonVideo"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
}
//e POS34070 - Reenable/redisplay the mPerks button at Attract and ScanAndBag screen if the mPerks function is turned on at POS - rrs

		//+CRD128616
		if(g_bMobileAppEnabled)
		{
			CLaneStateData laneStateData(m_pAppPSX);
			laneStateData.SendNextGenUIData(_T("NextGenUIMobileEnabled"), true );
			recallList.Reset();

			trace(L6, _T("Mobile shopper Enabled"));			
			m_pAppPSX->SetTransactionVariable(_T("EnableMobileShopper"), _T("1")); //CRD 171129
		}
		//-CRD128616

		trace(L6, _T("transaction mode is %s"), m_csTransactionMode);
		if(m_csTransactionMode == TRANSMODE_MEIJERGO || m_csTransactionMode == TRANSMODE_MEIJERGO_SND)
		{
			//Start CRD 413782
			if(SMState::SS90SupportMeijerCom.IsEmpty())
			{
				CString strOptionName = _T("SS90SupportMeijerCom");
				SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &SMState::SS90SupportMeijerCom);
				trace(L6, _T("SS90SupportMeijerCom: %s"), SMState::SS90SupportMeijerCom);
			}

			if(SMState::SS90SupportMeijerCom.Find(_T("Y")) != -1)
			{
				trace(L6, _T("SS90 configured to support Meijer.com. Scanner should be enabled."));
			}
			else
			{
				trace(L6, _T("Transaction Mode is either 2 or 3 and SS90 is not configured to support Meijer.com. Scanner should be disabled"));
				SAWLDBScannerDisable();
			}
			//End CRD 413782
		}
		csFLMSType = FLMSTYPE_NORMAL; //CRD 145555
		//Start CRD 142210
		CString csProperty;

		csProperty = _T("EASPopUpDesc");
		CString csDescForPopUp = ps.GetPSText(SSF_TXT_EAS_ITEM_DESC, ps.m_languageUsed);
		SendTBCommand(TBCMD_SET_TBPROPERTY, &csProperty, &csDescForPopUp);
					   
		csProperty = _T("EASInterventionTitle");
		csDescForPopUp = ps.GetPSText(SSF_TXT_EAS_INTERVENTION, ps.m_languageUsed);
		SendTBCommand(TBCMD_SET_TBPROPERTY, &csProperty, &csDescForPopUp);
		//End CRD 142210

		//Start CRD 223409
		csProperty = _T("MobileAssistanceInterventionTitle");
		csDescForPopUp = ps.GetPSText(SSF_TXT_MOBILE_ASSISTANCE_INTERVENTION, ps.m_languageUsed);
		SendTBCommand(TBCMD_SET_TBPROPERTY, &csProperty, &csDescForPopUp);
		//End CRD 223409

		// Start CRD 554916
		csProperty = _T("AccountLookUpInterventionTitle");
		csDescForPopUp = ps.GetPSText(SSF_TXT_ACCOUNT_LOOKUP_FAILED_INTERVENTION, ps.m_languageUsed);
		SendTBCommand(TBCMD_SET_TBPROPERTY, &csProperty, &csDescForPopUp);
		// End CRD 554916
		// Start CRD 557225
		csProperty = _T("GetAccountDetailsFailedInterventionTitle");
		csDescForPopUp = ps.GetPSText(SSF_TXT_GET_ACCOUNT_DETAILS_FAILED_INTERVENTION, ps.m_languageUsed);
		SendTBCommand(TBCMD_SET_TBPROPERTY, &csProperty, &csDescForPopUp);
		// End CRD 557225

		//CRD 299891 
		mobileCart.SetAbortingAudit(false);

		return pRetState;
}
#else
#include "..\TradeShowDemo\SMAttractTSDemo.cpp"
#endif // _TRADESHOWDEMO

void SMAttract::UnInitialize(void)
{	
	//(+) VSTS171129
	CLaneStateData laneStateData(m_pAppPSX);
	laneStateData.SendNextGenUIData(_T("NextGenUIMobileEnabled"), false );
	//(-) VSTS171129

    m_nLangSelectionPressed = 0;
	SMAttractBase::UnInitialize();
}

SMStateBase * SMAttract::SAEndOfTransaction(void)
{
    return SMAttractBase::SAEndOfTransaction();
}

SMStateBase * SMAttract::OnWtDecrease(void)
{
    return SMAttractBase::OnWtDecrease();
}

SMStateBase * SMAttract::OnWtIncreaseNotAllowed(void)
{
    return SMAttractBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMAttract::RAParse(MessageElement *me)
{
    return SMAttractBase::RAParse(me);
}

SMStateBase *SMAttract::OnLookupComplete (long byteCount)
{
    return SMAttractBase::OnLookupComplete ( byteCount);
}

bool SMAttract::LoyaltyCheck(CString csLookupType, CString csLookupData)
{
    return SMAttractBase::LoyaltyCheck(csLookupType, csLookupData);
}

void SMAttract::HandleLoyaltyCheckTimedOut()
{
    SMAttractBase::HandleLoyaltyCheckTimedOut();
}

SMStateBase * SMAttract::TimedOut(void)
{
    if (m_csFLConfig == _T("SuperMini") && !m_bIsFirstTimedOut)
		DMSayPhrase(TOUCHSTART); //scan your yellow payslip
	
	m_bIsFirstTimedOut = false;
    SendTBCommand(TBCMD_CHECK_FORCE_LOAD, NULL);
    return SMAttractBase::TimedOut();
}

SMStateBase * SMAttract::PSButton1(void)
{
    m_bIsNewTrxSelected = true;
	
	// Start CRD 125377
	if (m_bShopAndDropEnabled)
	  return PSButton4();
	// End CRD 125377
	//POS48277 - MobilePayStation
	if( m_csFLConfig == _T("MobilePayStation") )
	{
		return PSButton4();
	}
	//e POS48277 - MobilePayStation
  return transition(START_BUTTON, m_customerLanguage);  
}

SMStateBase * SMAttract::PSButton2(void)
{
    return SMAttractBase::PSButton2();
}

SMStateBase * SMAttract::PSButton3(void)
{
    SMStateBase  *ptr = transition(START_BUTTON, m_customerLanguage);
	
    SetHookProperty(_T("PropFromRTS"), _T("0"));
    // +SOTF5842
    if( EXTRACT_CLASSNAME(ptr).Find(_T("ScanAndBag")) == -1 )
    {
        SendTBCommand(TBCMD_SET_TB_STATE,_T("WIC_START_PENDING"));
        return ptr;
    }
    else
    {
        delete getRealStatePointer(ptr);
		//(+) POS189696
        if(SMState::lWICOptionloaded == 2)
        {
            SendTBCommand(TBCMD_SET_TB_STATE,_T("SMART_WIC_START"));
        }
		//(-) POS189696
        else
        {
            SendTBCommand(TBCMD_SET_TB_STATE,_T("WIC_START"));
        }
		CREATE_AND_DISPENSE(InProgress)(storeMode(), helpMode(), co.getTimeOutTender());  
		
    }
    // -SOTF5842
}

//POS24632 - mPerks button at Attract
SMStateBase * SMAttract::PSButton4(void)
{
    SMStateBase  *ptr = transition(START_BUTTON, m_customerLanguage);
	
    //POS37599 disable RAP AM
    ra.RequestEnableAssistMode(false);
    //e POS37599
	
    SetHookProperty(_T("PropFromRTS"), _T("0"));
    // +SOTF5842
    if( EXTRACT_CLASSNAME(ptr).Find(_T("ScanAndBag")) == -1 )
    {
        SendTBCommand(TBCMD_SET_TB_STATE,_T("TB_DIGITAL_COUPON_PENDING"));
        return ptr;
    }
    else
    {
        SendTBCommand(TBCMD_SET_TB_STATE,_T("TB_DIGITAL_COUPON"));
        delete getRealStatePointer(ptr);
        CREATE_AND_DISPENSE(InProgress)(storeMode(), helpMode(), co.getTimeOutTender());  
    }
    // -SOTF5842
}

SMStateBase * SMAttract::PSButton5(void)
{
    return SMAttractBase::PSButton5();
}

SMStateBase * SMAttract::PSButton6(void)
{
    return SMAttractBase::PSButton6();
}

SMStateBase * SMAttract::PSButton7(void)
{
    m_bNeedScanReceipt = true;
	m_bIsNewTrxSelected = false;
	return transition(START_BUTTON, m_customerLanguage); 
}

SMStateBase * SMAttract::PSButton8(void)
{
    return OnLanguageSelection();
 //   return SMAttractBase::PSButton8();
}

SMStateBase * SMAttract::DMScannerEncrypted(void)
{
	if ( m_csTransactionMode == TRANSMODE_MEIJERGO || m_csTransactionMode == TRANSMODE_MEIJERGO_SND )
		return DMScanner();
	else
		return SMState::DMScannerEncrypted();
}


SMStateBase * SMAttract::DMScanner(void)
{
    CString csBarcodeType;
	CString csBarcode = csDMLastBarCode;
	TrimSymbology(csBarcode);
	trace(L6, _T("SMScanReceipt::DMScanner(), csBarcode: %s"), csBarcode);

	if ( (m_csTransactionMode == TRANSMODE_MEIJERGO || m_csTransactionMode == TRANSMODE_MEIJERGO_SND) && SMState::SS90SupportMeijerCom == _T("Y") )
	{
		if(!IsShiptBarcode(csDMLastBarLabel))
		{
			CustomMessageObj.Reset();
			CustomMessageObj.csLeadThruText = ps.GetPSText(SSF_TXT_MEIJERGO_SALE_NOTALLOWED_TITLE);
			CustomMessageObj.csScreenText = ps.GetPSText(SSF_TXT_MEIJERGO_SALE_NOTEALLWED_MESSAGE);    		
			RETURNSTATE(CustomMessage)
		}
	}
	
	if ( csBarcode.GetLength() == 12 )
	{
		csBarcodeType = csBarcode.Left(2);
	}
	else if( csBarcode.GetLength() == 13 )
	{
		csBarcodeType = csBarcode.Left(3);
	}
	else if ( csBarcode.GetLength() == 14 )
	{
		csBarcodeType = csBarcode.Left(4);
		if ( csBarcodeType != _T("0047") )
		{
			csBarcodeType = csBarcode.Left(5);
		}
	}
    //S.Marks Meijer  has code that manipulates the barcode after it has been scanned
	//I moved this code to a function because Security Maint. needs to modify the barcode 
	//the exact same way.
	MeijerModifyScannedBarcode();
	
    trace(L6, _T("SMScanReceipt::DMScanner(), csBarcodePrefix: %s, length: %d"), csBarcodeType, csBarcode.GetLength());
	
	if ( (csBarcodeType == _T("47") || csBarcodeType == _T("047") ||
		csBarcodeType == _T("0047") || csBarcodeType == _T("00047")) &&
		(m_csFLConfig == _T("Mini") || m_csFLConfig == _T("SuperMini")) ) //scanned suspended receipt
	{
		m_bIsNewTrxSelected = false;
		//SASendSCOTInput( ITEMIZATION );	SSCOI-48161
		csItemSent = csDMLastBarCode;
		fItemIsWaiting = true;           
		SASendSCOTInput( UPC_SCANNED, csDMLastBarLabel );	
	}
	else //scanned item
	{
		if (m_csFLConfig == _T("SuperMini"))
		{
			//SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
			DMScannerEnable();
			return STATE_NULL;
		}
		m_bIsNewTrxSelected = true;
		
		//SASendSCOTInput( ITEMIZATION );	SSCOI-48161
		csItemSent = csDMLastBarCode;
		fItemIsWaiting = true;            
		SASendSCOTInput( UPC_SCANNED, csDMLastBarLabel );
		
		if ( bQuietNeedSecApproval )  //TAR234536
		{
			bQuietNeedSecApproval = false;
			m_bScanOnAttractAtQuietMode = true;
		}
		
		if (co.fStatePaintScanAndBagOnInitialScan)
		{
			PaintScanAndBagScreen();  // TAR 262131 Provide immediate feedback
			//ps.EnableUserInput(false);
		}
	}
	
    // POS26005 - don't play welcome audio when the item is scanned from Attract screen.
    // These are the 2 flag that SSF and scotapp uses to control the welcome audio
	// start CRD 125377 need to play wave 	
	if (!m_bShopAndDropEnabled)
	{
		m_bMJPlayedWelcome = true;
		bAlreadyPlayedWelcome = true;
    }
	// end CRD 125377 need to play wave 	
    // e POS26005
	
	return transition(SCAN_ITEM, m_customerLanguage);
} 

SMStateBase * SMAttract::DMCardReader(void)
{
    return SMAttractBase::DMCardReader();
}

SMStateBase * SMAttract::DMScale(void)
{
    return SMAttractBase::DMScale();
}

SMStateBase * SMAttract::PSButtonHelp(void)
{
    return SMAttractBase::PSButtonHelp();
}

SMStateBase * SMAttract::DMTakeawayBagAreaBackup(bool isBackedUp)
{
    return SMAttractBase::DMTakeawayBagAreaBackup(isBackedUp);
}
void SMAttract::PaintScanAndBagScreen()
{
    SMAttractBase::PaintScanAndBagScreen();

	// SSCOI-47255 Print Gift Receipt translation on background is used 
	// previous language when the program in the scan&bag context.
	// Solution: Refresh button 5 to clear the previous language in the 
	// buffer.All the if conditions in SMScanAndBag::Intitialize() are
	// copied here to make sure the program will work consistently.

	CString strSectionName = _T("WICEBT");
	CString strEntryName = _T("WICEBTEnableCM");
	CString strWICEBTCMFeature = _T("");

	SendTBCommand(TBCMD_GET_APPINI_ENTRY_VAL, (const TCHAR *)strSectionName, 
		(const TCHAR *)strEntryName, &strWICEBTCMFeature);

	if( GetHookProperty(_T("PropTransMode")).Find(_T("wicmode")) != -1)
	{
		if ( (GetHookProperty(_T("WICFreeItemPhaseActive")) != _T("1")) )
		{
			if( strWICEBTCMFeature == _T("1") )
			{
				ps.SetButtonText(_T("CMButton5"), SSF_BTT_APPLYPROMOTIONALITEMS);
			}
			else
			{
				ps.SetButtonText(_T("CMButton5"), SSF_BTT_PRINTGIFTRECEIPT);
			}
		}
	}
	else
	{
		ps.SetButtonText(_T("CMButton5"), SSF_BTT_PRINTGIFTRECEIPT);
	}
}

void SMAttract::HandleAttendantModeTimedOut()
{
    SMAttractBase::HandleAttendantModeTimedOut();
}

void SMAttract::DisplayHopperSubstitution(void)
{
    SMAttractBase::DisplayHopperSubstitution();
}

void SMAttract::HandleBackToPrimaryLanguageTimedOut()
{
    SMAttractBase::HandleBackToPrimaryLanguageTimedOut();
}

void SMAttract::ReloadThemes()
{
    SMAttractBase::ReloadThemes();
}

void SMAttract::CheckCashDiscrepancyStatus(void)
{
    SMAttractBase::CheckCashDiscrepancyStatus();
}

#ifndef _TRADESHOWDEMO
//SOTF7745
SMStateBase * SMAttract::PSCmdButton(CString csContext, CString csControl, CString csCommand)
{
	// SSCOI-45354
	// This is a migration issue. Removing code below after corrected the migration error.
	//CLaneStateData laneStateData(m_pAppPSX);
	//laneStateData.SendNextGenUIData(_T("RemoteProxyUserInputCompleted"), true);
	// -SSCOI-45354

    if(csControl == _T("CmdBtn_ButtonVolumeControl"))
    {
		//SSCO base volume control button
		return  SMAttractBase::PSButton7();
		
    }
    
    // +SOTF 8885
    if(csControl == _T("CmdBtn_PriceInquiry")) 
    {
        SendTBCommand(TBCMD_SENDKEYEDDATA, CString(_T("{PRICE}")));
        // Price inquiry is a customer mode screen and should respect current language
		CREATE_AND_DISPENSE(InProgress)(0,
			helpMode(),
			co.getTimeOutTender());
        
    }
    // - SOTF 8885
	trace(L6, _T("SMAttract::PSCmdButton(), csControl: %s"), csControl);

	// (+) CRD 125378 Meijer
	COleVariant v;

	if(csControl == _T("CmdBtn_MeijerSND"))
    {
		// Tell POS transaction mode is SND
		// At this point there is no need to Tell POS the mode is anything else other then SSCO or SnD.
		// SSCO=0, SnD=1, FLMS=2 and SnD+FLMS=3 

		SetHookProperty(_T("IsSND"),_T("1")); //CRD 157372
		m_bSNDAllowItemMenu = false;
		m_bShopAndDropEnabled = true; 
		m_bSNDExitedAM = false;			
		m_bSNDSettingAge = false;
		m_bSNDxCancelTrans = false;		// CRD 125377
		m_bSNDBackToFnP = false;

		SAWLDBScannerEnable();	//CRD 135458;
		
		SetHookProperty(_T("SuspendingCurrentTransaction"), _T("0"));
		

		m_pAppPSX->SetTransactionVariable(_T("ShopAndDropEnabledYesNoButtons"), VARIANT_FALSE); 
		
		CString strSNDName = _T("SDNRestrictedAgeTimer");
		CString m_csSNDConfig;
		SendTBCommand(TBCMD_GET_OPTIONS, &strSNDName, &m_csSNDConfig);
		lSDNRestrictedAgeTimerAmt = _ttol(m_csSNDConfig);


		
		SendTBCommand(TBCMD_NOTIFY_TRANSACTIONMODE, 1); // CRD126015
		m_csTransactionMode = TRANSMODE_SND;
		v = m_csTransactionMode;
		m_pAppPSX->SetTransactionVariable(_T("TransactionMode"), v);
		m_pAppPSX->SetTransactionVariable(_T("WasOnTransactionMode3"), v);

		return STATE_NULL;
		
    }
	if(csControl == _T("CmdBtn_MeijerGo"))
    {
		// Tell POS transaction mode is MeijerGo
		// At this point there is no need to Tell POS the mode is anything else other then SSCO or SnD.
		// SSCO=0, SnD=1, FLMS=2 and SnD+FLMS=3 
		
		SendTBCommand(TBCMD_NOTIFY_TRANSACTIONMODE, 0); // CRD126015

		v = _T("1");
		m_pAppPSX->SetTransactionVariable(_T("WasOnTransactionMode3"), v);

		m_csTransactionMode = TRANSMODE_MEIJERGO;
		v = m_csTransactionMode;
		m_pAppPSX->SetTransactionVariable(_T("TransactionMode"), v);

		// CRD133235
		SAWLDBScannerDisable();

		
		return STATE_NULL;
		
    }
	// Start CRD 537481, 525299 Handler for Apply Meijer Card button press,
	if(csControl == _T("CmdBtn_ApplyMeijerCard"))
	{
		bApplyMeijerStartedFromAttract = true;
		return transition(START_BUTTON, m_customerLanguage); 
	}
	// End CRD 537481, 525299
	
	// CRD 125378 Meijer
  
	//Debug code - do not check in
    if (csControl.Find(_T("CmdBtn_FLErrMsg")) == 0)
        return SMState::PSCmdButton(csContext, csControl, csCommand);
    else
		//-Debug code
        return SMAttractBase::PSCmdButton(csContext, csControl, csCommand);
    
}
//sotf

#endif //_TRADESHOWDEMO
SMStateBase * SMAttract::transition(TRANSITION_EVENT event, SCOT_LANGUAGE_TYPE languageUsed)
{	
	//This logic below will add the TLOG with Override ID=46 at the end of every transaction.
	if( co.IsAttendantModeOn() )
	{
		SendTBCommand(TBCMD_TLOG_OVERRIDE,
			co.GetAttendantID(),
			DATANEEDED_STARTTRANSACTIONAM,
			1,
			0,
			_ttol(rp.GetTerminalNumber()),
			0);//SSCOI-47169 POS304531
	}
	return SMAttractBase::transition(event, languageUsed);
}

void SMAttract::DMTurnOffTriColorLight( void )
{
    SMAttractBase::DMTurnOffTriColorLight();
}

SMStateBase * SMAttract::ReloadOptions( void )
{
    return SMAttractBase::ReloadOptions();
}

//////////////////////////////////////
// Language selection
//////////////////////////////////////
SMStateBase  *SMAttract::OnLanguageSelection(void)
{
	if (co.m_iNumberOfLanguagesSupported > 2)
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("Attract"));

		return setAndCreateAnchorState(BES_LANGUAGESELECT);
	}
	else if (co.m_iNumberOfLanguagesSupported == 2)
	{
		if (m_customerLanguage == SCOT_LANGUAGE_PRIMARY)
		{
			m_nLangSelectionPressed = 1;
			m_customerLanguage = SCOT_LANGUAGE_SECONDARY;
		}
		else
		{
			m_nLangSelectionPressed = 2;
			m_customerLanguage = SCOT_LANGUAGE_PRIMARY;
		}

		ps.SetLanguage(m_customerLanguage);
		co.csCMCustomerMessage = ps.GetPSText(co.nCMCustomerMessage);
		CustomerMessage = co.csCMCustomerMessage;

		/*if(gpSCOTAuxDisp)
		{
		gpSCOTAuxDisp->SetLanguage(m_customerLanguage);
		}*/
		ps.SetTareListToCurrentLanguage();
		//		ps.AssignTextToCtlsInCmCtxs(NONEPRIMARYCONTEXT);
		delete getRealStatePointer(Initialize()); // refresh the screen

		return STATE_NULL;
	}
	else
	{
		ASSERT(FALSE); // shouldn't enter here
		return STATE_NULL;
	}
}

void SMAttract::RefreshWICButton(void)
{
	if ((GetHookProperty(_T("IsWIC_EBT_ON")) == _T("1")) &&
		(GetTBProperty(_T("EnableWICButton")) == _T("Y")) && _tcsicmp(co.csHWType, _T("SS90"))) //Don't enable WIF for SS90
	{
		ps.SetButtonText(_T("CMButton3"), SSF_BTT_WIC_CARD);
		m_pAppPSX->SetTransactionVariable(_T("WICFeatureEnable"), VARIANT_TRUE); // SSCOI-46989
		trace(L6, _T("SMAttract::RefreshUI(), WIC Feature enabled in POS and in FastLane; showing WIC button."));
	}
	else
	{
		ps.SetButtonText(_T("CMButton3"), PS_NOTEXT);
		m_pAppPSX->SetTransactionVariable(_T("WICFeatureEnable"), VARIANT_FALSE); // SSCOI-46989
		trace(L6, _T("SMAttract::RefreshUI(), WIC Feature disabled either in POS or FL."));
	}
}

void SMAttract::RefreshUI()
{
	//POS48277 - Mobile PayStation - rrs
	/*CString strSectionName = _T("MOBILESHOPPER");
	CString strEntryName = _T("PayStationTmIDs");
	SendTBCommand(TBCMD_GET_APPINI_ENTRY_VAL, (const TCHAR *)strSectionName, (const TCHAR *)strEntryName, &strWICEBTCMFeature);*/

	// POS78133: Initialize FastLane configuration property used in ApplicationModel.xml to determine the mobile MOB_SHOP_ORDER_NOT_FOUND POS state.
	SetHookProperty(_T("FastLaneConfiguration_Prop"), m_csFLConfig);

	if (m_csFLConfig == _T("MobilePayStation"))
	{
		ps.SetCMFrame(_T("AttractPayStation"));
		DMScannerDisable();
	}
	else
	{
		ps.SetCMFrame(_T("Attract"));
	}
	// e POS48277   


	//SMarks changed this out because CMButton2LG is now
	//the Customer Own Bag btn that is control through a scotopts
	//CMButton8Global is now the other language (2ndary)btn.
	ps.SetButtonText(_T("CMButton8Global"), PS_NOTEXT);

	if (m_csFLConfig == _T("Mini")) // deprecated -- will be removed soon
	{
		ps.Button(_T("CMButton1Lg"), SSF_BTT_NEWTRANSACTION, true);
		ps.Button(_T("CMButton7"), BTT_FINISH, true);
		ps.Message12(ps.GetPSText(SSF_LTT_SELECTOPTION_BEGIN), false);
	}
	else if (m_csFLConfig == _T("SuperMini"))
	{
		ps.SetButtonText(_T("CMButton1Lg"), PS_NOTEXT);
		ps.Button(_T("CMButton7"), BTT_FINISH, true);
		ps.CMButtonFlash(_T("CMButton7"), true);
		ps.Message12(ps.GetPSText(SSF_LTT_COMPLETETRX_FINISHPAY), false);
	}
	// POS48277 PayStation
	else if (m_csFLConfig == _T("MobilePayStation"))
	{
		ps.SetButtonText(_T("CMButton1Lg"), 9356);
		ps.SetButtonText(_T("CMButton7"), PS_NOTEXT);
		ps.SetButtonText(_T("CmdBtn_PriceInquiry"), SSF_BTT_PRICE_INQUIRY);
		ps.Message12(ps.GetPSText(9355), false);

		trace(L6, _T("MobilePayStation Attract screen"));
	} // e POS48277
	else /* standard */
	{
		ps.SetButtonText(_T("CMButton1Lg"), BTT_START);
		ps.SetButtonText(_T("CMButton7"), PS_NOTEXT);
		ps.SetButtonText(_T("CmdBtn_PriceInquiry"), SSF_BTT_PRICE_INQUIRY);
		ps.Message12(ps.GetPSText(MSG_ATTRACT_NO_LIMIT), false);
	}

	//translate the "I brought my own bag" button when language is pressed
	if ((co.fOperationsCustomerBagAllowed) && (!dm.fStateTakeawayBelt))
	{
		ps.SetButtonText(_T("CMButton2Lg"), BTT_OWNBAG);
	}
	else
	{
		ps.SetButtonText(_T("CMButton2Lg"), PS_NOTEXT);
	}

	RefreshWICButton();

	// POS33581 don't flash the START and Own Bag button rrs 
	ps.CMButtonFlash(_T("CMButton1Lg"), false);
	if (co.fOperationsCustomerBagAllowed)
	{
		ps.CMButtonFlash(_T("CMButton2Lg"), false);
	}
	// e POS33581 don't flash the START and Own Bag button rrs 

	m_pAppPSX->SetConfigProperty(_T("CMScreenText"), _T("Attract"), UI::PROPERTYVISIBLE, VARIANT_FALSE);

	if(!_tcsicmp(co.csHWType, _T("SS90"))) //Disable own bag and WIC for SS90
	{
		m_pAppPSX->SetConfigProperty(_T("CMButton2Lg"), _T("Attract"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
		m_pAppPSX->SetConfigProperty(_T("CMButton3"), _T("Attract"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
	}

	// Rel 4.0 Req. 6.5 Change Language During Transaction-->
	if (co.fStateDualLanguage)
	{
		CString csButtonText;

		//set button text
		if (co.m_iNumberOfLanguagesSupported > 2)
		{
			ps.SetPSText(PS_WORK1, ps.GetPSText(BTT_LANGUAGE, SCOT_LANGUAGE_PRIMARY));
		}
		else if (co.m_iNumberOfLanguagesSupported == 2)
		{
			int i, iButtonID;

			if (m_customerLanguage == SCOT_LANGUAGE_PRIMARY)
			{
				i = SCOT_LANGUAGE_SECONDARY;
			}
			else if (m_customerLanguage == SCOT_LANGUAGE_SECONDARY)
			{
				i = SCOT_LANGUAGE_PRIMARY;
			}
			else
			{
				ASSERT(FALSE); // shouldn't enter here
			}
			//Get the language name text for button
			csButtonText = co.language[i].csName;
			if (_istdigit(*csButtonText))
			{
				iButtonID = _ttoi(co.language[i].csName);
				csButtonText = ps.GetPSText(iButtonID, (SCOT_LANGUAGE_TYPE)i);
			}
			ps.SetPSText(PS_WORK1, csButtonText);
		}
		else
		{
			ASSERT(FALSE); // shouldn't enter here because at least 2 languages should be supported
		}

		ps.SetButtonText(_T("CMButton8"), PS_WORK1);
		//make visible the language button
		m_pAppPSX->SetConfigProperty(_T("CMButton8"), _T("Attract"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
	}

	ps.SetButtonText(_T("ButtonHelp"), BTT_HELP);

	//POS48277 PayStation
	if (m_csFLConfig == _T("MobilePayStation"))
	{
		ps.SetLeadthruText(9354);
	}
	else
	{
		ps.SetLeadthruText(LTT_ATTRACT);
	}
	//e POS48277 PayStation

	ps.ShowCMFrame();
}

//TAR335656+
SMStateBase  *SMAttract::TBParse(MessageElement* me)
{
	trace(L6, _T("SMAttract::TBParse"));
	SMStateBase* returnState = SMState::TBParse(me);


	if (m_TBLastState == TB_TRXEND)
	{
		trace(L3, _T("SMAttract::TBParse - m_TBLastState = %d"), m_TBLastState);
		SASendSCOTInput(ITEMIZATION);
	}
	else if( m_TBLastState == TB_POS_NEED_RESTART ) //+CRD185924
	{   
		trace (L6, _T("Re-using TBCMD_CHECK_FORCE_LOAD to trigger to assistmode for POS quick restart."));				
		SendTBCommand(TBCMD_CHECK_FORCE_LOAD, NULL);   
	}
	

	return returnState;
}
//TAR335656-

//+SOTF6796 TAR421859
SMStateBase *SMAttract::OnTransactionComplete()
{
	SMStateBase * pRet = SMStateBase::OnTransactionComplete();

	if (getStateName(pRet).Find(_T("SMThankYou")) != -1)
	{
		trace(L6, _T("OnTransactionComplete() received while in Attract.  Ignoring request to go back to Thank You."));
		delete getRealStatePointer(pRet);
		pRet = STATE_NULL;
	}

	return pRet;
}
//-SOTF6796 TAR421859

// Price Inquiry
SMStateBase  *SMAttract::PSButtonGoBack(void)
{
	// +SOTF 8885
	//SendTBCommand(TBCMD_SENDKEYEDDATA, CString(_T("{PRICE}")));
	// Price inquiry is a customer mode screen and should respect current language
	//CREATE_AND_DISPENSE(InProgress)(0,
	//                        	helpMode(),
	//	                        co.getTimeOutTender());

	// +CRD132828
	trace(L6,_T("MeijerGo Back button is pressed"));
	COleVariant v;

	//reset SND variables
	m_bSNDmPerkEntered = false;			// CRD 125377
	m_bSNDTMDEntered = false;			// CRD 125377
	m_bSNDAgeEntered = false;			// CRD 125377
	m_bSNDAllowItemMenu = false;		// CRD 125377
	m_bSNDClearUnknownItemC = false;	// CRD 125377
	m_bSNDmPerksCancel = false;			// CRD 125377
	m_bShopAndDropEnabled = false;		// CRD 125377
	SetHookProperty(_T("IsSND"),_T("0")); //CRD 157372

	// CRD133235
	SAWLDBScannerDisable();

	SendTBCommand(TBCMD_NOTIFY_TRANSACTIONMODE, 3); // CRD126015
	
	v = _T("3");
	m_pAppPSX->SetTransactionVariable(_T("TransactionMode"), v);
	// -CRD132828

    return STATE_NULL;
	
    // -SOTF 8885
}

SMStateBase *SMAttract::CUParse(MessageElement *me)
{
    return SMAttractBase::CUParse(me);
}

//UploadTicket Port + 
SMStateBase *SMAttract::OnMobileTransactionStart(CString csTransactionInfo)
{
// start CDR126194
    SMStateBase *retState = STATE_NULL;

	//(+) VSTS170002
	trace(L6, _T("Making m_bMobileTransactionStarted as true"));
	SMState::m_bMobileTransactionStarted = true; 
	 //(-) VSTS170002

	
	//+POS128616
	if (csTransactionInfo.Find(MOBILE_RESCAN_REQUIRED) == 0)
	{
		CLaneStateData laneStateData(m_pAppPSX);
		laneStateData.SendNextGenUIData(_T("NextGenUIMobileEnabled"), false );
	}
	//-POS128616
    if (csTransactionInfo.Find(MOBILE_START) == 0)
    {
		//Start CRD 150652
		//SSCOIA-39267 - do not set IsAuditonSSCO Transaction Variable to true when starting the recalling items because this transaction is only intended for Audit
		// which will be reset at SMSmMobileAudit::Unitialize - this hides the totals and does not expand the cart control in NGUI
		//COleVariant var;
		//var = _T("1");
		//m_pAppPSX->SetTransactionVariable(_T("IsAuditOnSSCO"), var);
		//End CRD 150652

		retState = GenerateRAPDataNeeded(_T(" "), _T(""), ms_MobileAuditDataEntryINI, ms_MobileAuditDataEntryINISection);

        bool bAlreadyInRecall = g_bInRecallMode;
        if (!g_bInRecallMode && recallList.GetNumRecallItems() > 0)
        {
            mp.LogTransition((CString)_T("SMStartMobileReplay"), nTBLastRealState); 
            SetTransactionStatus(szTSMobileReplayStarted);
            mobileCart.SendTransactionStatus(GetTransactionStatus());
            g_bInRecallMode = true;
            g_bEnteringRecallMode = true;
            PaintScanAndBagScreen();
            ps.SetCMFrame(_T("MobileProcessing"));
            ps.SetCMLeadthruText(TTL_LOADING_TRANSACTION);
            ps.Message12(MSG_LOADING_TRANSACTION, true);
            ps.ShowCMTBText(csTBMessage2Scot);  
            ps.CMButton(_T("ButtonHelp"),BTT_HELP,false);
            ps.ShowFrame();
        }
        if ( !bAlreadyInRecall && g_bInRecallMode && recallList.ParseNextItem(false))
        {
            
            bool bLookupStarted = false;
            CString csItem = recallList.GetScanDataLabel();
            if (m_csLoyaltyCheckType.IsEmpty())
                bLookupStarted = LoyaltyCheck(_T("Scan"), csItem);
            if(bLookupStarted)
                return STATE_NULL;
        }
        
        CString csButtonText;
        csButtonText = ps.GetPSText(MSG_LANGUAGENAME);
        rp.SetLanguageName(csButtonText);
        if ( bQuietNeedSecApproval )  //TAR234536
        {
            bQuietNeedSecApproval = false;
            m_bScanOnAttractAtQuietMode = true;
        }
        return transition(SCAN_ITEM, m_customerLanguage);
    }
    else if (csTransactionInfo.Find(MOBILE_RESCAN_REQUIRED) == 0)
    {		
		//Start CRD 150652
		COleVariant var;
		var = _T("1");
		m_pAppPSX->SetTransactionVariable(_T("IsAuditOnSSCO"), var);
		//End CRD 150652
		
        retState = GenerateRAPDataNeeded(_T(" "), _T(""), ms_MobileAuditDataEntryINI, ms_MobileAuditDataEntryINISection);
    }
    else
    {
        trace(L6,_T("Ignoring Message that is not an Item list"));
        return STATE_NULL;  //Ignore messages that do not match format;
    }
    return retState;
}
//UploadTicket Port -

