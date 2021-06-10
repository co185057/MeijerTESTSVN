//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAssitMode.CPP
//
// TITLE: Class implementation for Assist Mode keyboard
//
//
// AUTHOR:    Peter Denhoed
//
// CHANGE HISTORY:
//
// CRD 557225 Name: CJ Estrada Date: Mar 4, 2021
// CRD 554916 Name: CJ Estrada Date: FEb 9, 2021
// CRD124746 Work Request:Sprint 10.3 Name:Aparna Tunuguntla Date: March 19,2018
// POS318903 Work Request:77937 Name: Alex Bronola Date:March 23, 2017
// POS296838 Work Request:76936 Name:Alex Bronola Date:Oct 12, 2016 
// POS293858 Work Request:76936 Name:Alex Bronola Date:Sept. 28,2016
// POS152889 Work Request:68940 Name:Robert Susanto Date:June 25,2015
// POS181195 Work Request:68237 Name:Saiprasad Srihasam Date:April 30,2015
// POS190111 Work Request:68237 Name:Saiprasad Srihasam Date: April 21, 2015
// POS181201 Work Request:68237 Name:Saiprasad Srihasam Date: March 13, 2015
// POS181195/POS181197 Work Request:68237 Name:Saiprasad Srihasam Date:March 12,2015
// POS154826 Work Request:66559 Name:Saiprasad Srihasam    Date:January 22, 2015
// POS111636 Work Request:65347 Name:Srihasam Saiprasad Date:October 10, 2014
// POS107531 Work Request:65347 Name:Robert Susanto Date:August 22, 2014
// POS16295 Work Request:61233 Name:Robert Susanto Date:May 10, 2014
// POS82918 Work Request:61232 Name:Robert Susanto    Date:March 18, 2014
// POS57465 Work Request:56285 Name:Matt Condra    Date:May 15, 2013
// POS59850 Work Request:54416 Name:Robert Susanto    Date:April 1, 2013
// POS57686 Work Request:54416 Name:Robert Susanto    Date:March 11, 2013
// POS41473 Work Request:54416 Name:Matt Condra    Date:March 6, 2013
// POS54351 Work Request:54416 Name: Robert Susanto   Date:February 27, 2013
// POS33020 Work Request:52635 Name: Matt Condra   Date:November 16, 2012
// POS39475 Work Request:52635 Name: Matt Condra   Date:November 10, 2012
// POS36514 Work Request:52635 Name:Robert Susanto Date:October 5, 2012
// POS30981/POS31054 Work Request:50864 Name:Robert Susanto Date:June 29, 2012
// POS30981/POS31050 Work Request:50864 Name:Robert Susanto Date:April 24, 2012
// POS27031 Work Request:16656 Name:Robert Susanto Date:January 12, 2012
// POS11024 Work Request:16632 Name:Sandeep Shenoy Date:October 25, 2011
// POS11024 Work Request:16632 Name: Sandeep Shenoy Date: Oct 7, 2011
// POS18905 WORK REQUEST:16656 Name: Robert Susanto Date: October 11, 2011
// POS18819 Work Request:16656 Name:Robert Susanto Date:September 16 2011  			
// POS18320 Work Request:16656 Name:Robert Susanto Date:September 14 2011  			
// POS15745 Work Request:16632 Name:Robert Susanto Date:August 11 2011  			
// POS16295 Work Request:15196 Name:Robert Susanto Date:July 20, 2011
// POS15302 Work Request:15196 Name:Robert Susanto Date:July 15, 2011
// POS15035 Work Request:15196 Name:Matt Condra Date:June 29, 2011
// POS9528  Work Request:15196 Name:Sandy Reedy Date:April 21, 2011
// POS13126 Work Request:15196 Name:Robert Susanto Date:April 20, 2011
// POS10984 Work Request:15196 Name:Matt Condra Date:April 8, 2011
// POS10185 Work Request:13752 Name:Sandy Reedy Date:March 11, 2011
// POS12127 Work Request:13752 Name:Robert Susanto Date:April 5 2011  
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "TransBrokerSSF.h"
#include "SMSmAssistMode.h"
#include "SMSmHandHeldAssistMode.h"
#include "SMSmRestrictedItems.h"
//SOTF 8172
#include "SMScanAndBag.h"

#include "LaneStateData.h"
#include "MPProcedures.h"	//CRD 289150

#include "SMSmEnhancedAssistMode.h" //POS31050 rrs

#include "SMRecallingItems.h" // RFQ 9545 mobile shopper

#include "NCRPtr.h"
DLLIMPORT extern HWND hw; //CRD124746

DLLIMPORT extern MPProcedures mp;	//CRD 289150


#define TB_DEV_TOTAL_DETAILS 10006
#define TIMEOUT_DISABLED 4

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmAssistMode")

IMPLEMENT_DYNCREATE(SMSmAssistMode, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

bool SMSmAssistMode::m_bTenderInProgress = false;
long g_lOrigTimeoutValue = 0;
static bool g_bGotVPIMInAssistMode = false; //TAR353551
static bool g_bGoingIntoHandHeld = false;
static bool g_bRefreshAfterSoftKeyDisable = false;

static DWORD g_dwLastMICREvent = 0;
CString g_csInputData = _T("");
//bool SMSmAssistMode::m_bHHScanerActive = false;  // POS181195/POS181197
CString SMSmAssistMode::m_csRAPConnectionName = ""; // POS181195/POS181197
CString SMSmAssistMode::m_csLaneName = ""; // POS181195/POS181197

bool SMSmAssistMode::isEASOperatorGuidance = false;	//Start CRD 185620
//Start CRD 289150
bool SMSmAssistMode::bShowSmRecallingItems = false;	
bool SMSmAssistMode::bShowFinishRecall = false;
bool SMSmAssistMode::bShowPromptOnRefresh = true;
bool SMSmAssistMode::bShowB2BHighValuePromo = true;
//End CRD 289150
// POS31050 - There are mass changes in this file during Mobile SSCO enhancement project to not use the hardcoded context SmAssistMode when setting some properties to the control
// Instead we need to use the csCurrentContext parameter so that the SmEnhancedAssistMode could use it also as SmEnhancedAssistMode extends this class - RRS

////////////////////////////////
SMSmAssistMode::SMSmAssistMode()
: SMSmAssistModeBase()
{
	IMPLEMENT_TIMESTAMP
		
	//Create display string for testing until display message flow complete
	m_csLastDisplay = _T("");
	m_csLastTitle1 = _T("");
	m_bTransactionEnding = false;
	ps.DisableAssistModeKey(_T("Key24"), -1);
	//disable all the softkeys until they are ready to be displayed
	DisableSoftKeys();
	m_csPrevLastDisplay = _T("");
	m_bSlipPrintSideFront = false;
	m_bSlipAlreadyFlippedAfterApproval = false;
	m_bSlipActivityIsWaiting = false;
	m_bSlipFlipInProgress = false;
	m_bUnRemotedPrinterDevices = false;
	m_bRequestingCashDrawerOpen = false;
	//SOTF 8327
	m_bPreviousDeviceError = false;
    m_csWICItemList = _T("");	//POS154826
} 

void SMSmAssistMode::EnableDisableAllKeys(bool bFlag)
{
    CString csKeyCount, csKey;
    int i, nKeyCount;
	
	
    m_pAppPSX->GetCustomDataVar(_T("AssistKeyCount"), csKeyCount, _T("AssistKeyMap"), _T(""));
	nKeyCount = _ttol(csKeyCount);
	
    for(i=1; i < nKeyCount; i++)
    {
        csKey.Format(_T("Key%d"),i);
        ps.ButtonEnable(csKey, bFlag);
    }
}

SMStateBase * SMSmAssistMode::Initialize(void)
{
    trace(L0, _T("+SMSmAssistMode::Initialize()"));
	//	start CRD 125377
	trace(L6, _T("m_bShopAndDropEnabled=%d m_bSNDAllowItemMenu=%d, co.IsInAssistMenus() = %d"), m_bShopAndDropEnabled, m_bSNDAllowItemMenu, co.IsInAssistMenus());
	if (m_bShopAndDropEnabled)
	{
		m_bSNDAllowItemMenu = true;		// CRD 125377
		trace(L6, _T("m_bSNDAllowItemMenu=%d"), m_bSNDAllowItemMenu);
	}
	//  end CRD 125377
	
	//Start CRD 214582
	if(!co.IsInAssistMenus())
		co.SetInAssistMenus(true);
	//End CRD 214582

	m_bWasPOSRestarted = false;

    CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMSmAssistMode::Initialize"));
    //SOTF 8327
    m_bPreviousDeviceError = ms_bKeepRemoteDevices;
	
	//(+) CRD124746
	StopHHScannerTimer();
	trace(L6, _T("*** Assistmode killing timer intialize"));
	//(-) CRD124746

    CString strOptionName = _T("AgeRestItemApprovalOnEneteringAssistMode");
	CString strOptionValue;
	SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &strOptionValue);
	CString csPOSState = GetTBProperty(_T("SavedLastPosState"));
		
    if ( csPOSState.Find(_T("ITEMMENU")) != -1 && strOptionValue != _T("0") && (m_lAgerestrictQueuedtoRAP > 0 || lUnapprovedRestrictedItems > 0))
	{
			if (g_bInRecallMode)
			{
				trace(L6, _T("Skipping Payment processing due to FLMS ticket in progress"));
			}
			else
			{
				trace(L6, _T("Need to approve outstanding age restricted item "));
				CString posKey = _T("{PAYMENT}");
				SendTBCommand(TBCMD_SENDKEYEDDATA, posKey);
			}
    }    

//    if(!ps.RemoteMode())
    {

// (+) POS181195/POS181197

        //CString csOperation=_T("operation=scanner-status-153.60.40.86;");
        if(ps.RemoteMode())
        {
        CString csCompName=_T("");
        if(m_csLaneName == _T(""))
        {
            csCompName = SMState::GetLaneName();
        }
        else
        {
            csCompName = m_csLaneName;
        }
        csCompName += _T(";");

        CString csOperation=_T("operation=scanner-status-");
        csOperation = csOperation + csCompName;



        if(SMSmAssistMode::m_csRAPConnectionName == "")
        {
            SMSmAssistMode::m_csRAPConnectionName = SMState::GetRAPConnectionName();
        }

        m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), csOperation, SMSmAssistMode::m_csRAPConnectionName );
        }
    }
// (-) POS181195/POS181197

    /*if(SMState::m_bActivateHHScanner) //CRD124746
    {
        SMSmAssistMode::PSButton2();
    }*/


	CString strTBPropValue = _T("SMSmAssistMode");
	CString strTBPropName = _T("NewFLState");
    SendTBCommand(TBCMD_SET_TBPROPERTY, &strTBPropName, &strTBPropValue);
	//save off the orignal timeout value
    g_lOrigTimeoutValue = nTimeOut;
	m_lWaitCount = 0;

   if (TrainingModeObj.IsTMOn())
    {
		SetTBProperty(_T("TrainingMode"), _T("Y"));
        trace(L6, _T("Training Mode"));
    }
    else
    {
		SetTBProperty(_T("TrainingMode"), _T("N"));
        trace(L6, _T("Not in Training Mode"));
    }

    //POS11024-Mobile Shopper:Moved check to send TBCMD_RESUME_ITEMS
    //to after we SendTBCommand(TBCMD_ENTER_ASSISTMODE)
	
	 m_bIsFromScanReceipt = false;
    
    if( (csPOSState.Find(_T("PRICE_REQUIRED_INVALID")) != -1) ||
        (csPOSState.Find(_T("VARIABLE_COUPON_VALUE_INVALID")) != -1) )
    {
        SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
    }

	//fix for 306685
    SetHookProperty(_T("PropIsFromPayment"), _T("0"));
    SetHookProperty(_T("IsACSIgnoreItemsOnResume"), _T("0"));

	m_bTransactionEnding = false;
	//SOTF 6263
	//m_bSlipActivityIsWaiting = false;

	//are we going to handheld assistmode
	g_bGoingIntoHandHeld = false;
    if(( ra.IsRemoteConnectionHandheld(ra.GetRemoteConnectionName())) && (ps.RemoteMode()))
    {
		EnableHandheldAssistMode(true);
		ps.SetFrame(_T("HHSmAssistMode")); 
        g_bGoingIntoHandHeld = true;
        trace(L0, _T("Hand Held Assist Mode requested"));
        RETURNSTATE( SmHandHeldAssistMode)
    }

    //TAR #302051
    //Scanner Does Not Enable After Date of Birth Entry in Assist.
    //Ensure FL scanner is always enabled upon entry to assist mode.
    //SOTF 6004/410613
	if(!ps.RemoteMode())
	{
		DMScannerEnable();
	}

    //pos18905
    if( GetHookProperty(_T("PropHasRebateCheck")) == _T("1") && nTenderType == TB_TENDER_ASSISTTENDER) 
    {
        CString csPropRebateCheck = _T("PropHasRebateCheck");
	    CString csPropRebateCheckValue = _T("0");        
    
	    SendTBCommand(TBCMD_SET_HOOKPROPERTY, &csPropRebateCheck, &csPropRebateCheckValue);        
        trace(L6, _T("Reset the PropHasRebateCheck"));
    }

    HRESULT hr;
	bool bRestart = ms_bKeepRemoteDevices;
	
    bool bRemote = ps.RemoteMode() ? true : false;
    
    //if in remote assistmode and no endorsements pending clear security
    //so that the needassist text no longer displays on the rap
    
    if(fInEndorsementState == ENDORSE_NONE)
    {
		m_bSlipActivityIsWaiting = false;
        SAClearSecurityCondition();
    }

	SendTBCommand(TBCMD_ENTER_ASSISTMODE, bRemote);
    
    

    if ( SendTBCommand(TBCMD_IS_TRXN_ENDING, bRemote))
	{
		m_bTransactionEnding = true;
	}

    hr = m_POSDisplay.CreateInstance(__uuidof( MSXML2::DOMDocument40) );
    if(FAILED(hr))
    {
		trace(L1, _T("ERROR: Failed to create XML DOM Object. IS MS XML INSTALLED??"));
        return PSButton8();
    }
	ps.AllowInput( MAX_DIGITS_ITEMCODE, false, false );
	
    //m_pAppPSX->SetControlProperty( _T("Display"), UI::PROPERTYREDRAW, VARIANT_FALSE );
    //CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMSmAssistMode::Initialize"));
	
	//+POS318903 -aab
	int nTempCashInserted = 0;	

	//SSCOIA-39604 - logic needs to apply to EAS tagged items too
	// CRD 554916 also applies to Account Lookup Fail
	if ( ((csPOSState.Find(_T("AGE_VERIFY_DK1")) != -1) || (lUnapprovedEASItems || lUnapprovedMobileGenericDelayed || bUnapprovedAccountLookupFailed) && nDMCashInserted ))	//CRD 223409
	{
        nTempCashInserted = nDMCashInserted;
        nDMCashInserted = 0; //prevent the SMSmAssistModeBase::Initialize to call TBEnterTender
	}
    //-POS318903 -aab
	
	SMStateBase *retState = SMSmAssistModeBase::Initialize();

	//+POS318903 -aab
    if(nTempCashInserted)
		nDMCashInserted = nTempCashInserted;
    //-POS318903 -aab 
	
    // +SOTF6235 
    bool bTenderInProgress = (TB_SUCCESS_IMMEDIATE == SendTBCommand(TBCMD_TENDER_IN_PROGRESS, NULL));
    if( bTenderInProgress )
    {
        ps.ButtonEnable(_T("SMButton1"), false);
    }
    // -SOTF6235

	ps.ScrollDown(_T("AssistReceipt"));

	// sotf 9269 - dont' move the cursor if it's in AM - rrs
	//SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(_T("AssistReceipt")));
// (+) POS181195/POS181197
//    ps.ButtonState(_T("SMButton2"), false, false);
// (-) POS181195/POS181197


	if ( ps.RemoteMode() )
	{
        //Currently remoted, i.e. RAP connected	            
		bool bPrinterAvailable = CheckIfPrinterAvailable(true);
		if  (bRestart  && bPrinterAvailable)
		{
			// Coming back after error, Check to see if the drawer needs to be opened.
			delete getRealStatePointer( OnNewCashDrawerState());
		}  	
        
        //POS27031 - update the RTS button based on the latest state if we coming back to RAP AM from device error
        if( bRestart )
        {            
            CString csAllowAMExit = GetHookProperty(_T("PropertyAllowAMExit"));    
	        
	        COleVariant v = (long) UI::Normal;

            if(csAllowAMExit == _T("1") && (!m_bRequestingCashDrawerOpen) )
	        { 
                m_pAppPSX->SetConfigProperty(_T("SMButton1"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
		        m_pAppPSX->SetConfigProperty(_T("SMButton1"), csCurContext, UI::PROPERTYSTATE, v);
	        }
            else if ( csAllowAMExit == _T("1")  && ( !m_bTransactionEnding && !m_bRequestingCashDrawerOpen && !m_bTenderInProgress)) // SOTF6235 
	        {
                m_pAppPSX->SetConfigProperty(_T("SMButton1"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
		        m_pAppPSX->SetConfigProperty(_T("SMButton1"), csCurContext, UI::PROPERTYSTATE, v);
	        }
	        else
	        {
		        m_pAppPSX->SetConfigProperty(_T("SMButton1"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);

                

                trace(L7, _T("Disable Return To Shopping Button!!!!"));
	        }    
        }
        // e POS27031 - RRS
  
	}
	else
	{//TAR 304732
		trace(L0,_T("Lane Assist; Tell the TB in Case Cash Drawer Opens."));
		SendTBCommand(TBCMD_CASHDRAWER_CLOSE, NULL );
	}

    // POS 13126 - if the g_bTBNeedMoreData is true then we need to reset it in here as the attendant will enter the data from AM - rs
    if(g_bTBNeedMoreData)
    {
        g_bTBNeedMoreData = false;  
    }
    // e POS 13126	    

    //POS11024:Mobile shopper
    CString csResumeInProgress = GetHookProperty(_T("IsResumeTrxInProgress"));
    if( csResumeInProgress == _T("1") )
    {
       setAnchorState(BES_FINISH);  
    }  
    
    //POS11024:Mobile shopper. Moved this block of code from above
    CString csPropName = _T("IsThereItemNeedToDisplayed");
	 CString csIsNeedToDisplay;
	 SendTBCommand(TBCMD_GET_TBPROPERTY, &csPropName, &csIsNeedToDisplay);

     if ( csIsNeedToDisplay == _T("1"))
    {      
	    SendTBCommand(TBCMD_RESUME_ITEMS, NULL);
    }

    if( TB_SUCCESS == SendTBCommand(TBCMD_TENDER_IN_PROGRESS, NULL) )
    {

        //SOTF 6657
        SendTBCommand(TBCMD_NOTIFY_ACS_REMOTE_ASSISTMODE_STATUS, (ps.RemoteMode() ? true : false));

	    TBShowDisplay();
	    //m_pAppPSX->SetControlProperty( _T("Display"), UI::PROPERTYREDRAW, VARIANT_TRUE );
	    ps.ShowFrame();
        
	    if( (dm.IsCheckPaymentAtLaneAvailable() || ps.RemoteMode() ) && 
            (fInEndorsementState == ENDORSE_INPROGRESS) && 
            (GetTBProperty(_T("IsFromEndorsement")) != _T("1")) )
	    {
		    trace (L6, _T("SMSmAssistMode::Initialize() Remoted (or local check printer) and Endorsement inprogress"));
            // NewUI
		    COleVariant v;
            v = (long) UI::Disabled; // Disable
            //m_pAppPSX->SetControlProperty(_T("SMButton1"), UI::PROPERTYSTATE, v);
            m_pAppPSX->SetConfigProperty(_T("SMButton1"), csCurContext, UI::PROPERTYSTATE, v);

            ps.ShowFrame();
	    }
        
        return retState;
    }else
    {
        m_bTenderInProgress = true;
        EnableDisableAllKeys(false);

        //don't let operator leave until the partial cash tender is complete
        COleVariant v;
        v = (long) UI::Disabled; // Disable
        //m_pAppPSX->SetControlProperty(_T("SMButton1"), UI::PROPERTYSTATE, v);
        m_pAppPSX->SetConfigProperty(_T("SMButton1"), csCurContext, UI::PROPERTYSTATE, v);

        ps.SetTitle(_T(""));
        ps.SetLeadthruText(ps.GetPSText(LTT_CASHPROCESSING)); //processing screen
        //m_pAppPSX->SetControlProperty( _T("Display"), UI::PROPERTYREDRAW, VARIANT_TRUE );
        ps.ShowFrame();

        
        delete getRealStatePointer( retState);        
        return TimedOut();
    }
}

void SMSmAssistMode::UnInitialize(void)
{

	//Start CRD 366323
	NotifyExitStoreMode();
	//End CRD 366323
	
	SMRecallingItems::g_nTimeoutRecallingWaitForPos = 9999; // RFQ 9545 
	// Start CRD 125377
	if (m_bShopAndDropEnabled)
	{
		m_bSNDSettingAge = false;
		m_bSNDExitedAM = true;
	}
	// End CRD 125377
    //sotf 6657
    SendTBCommand(TBCMD_NOTIFY_ACS_REMOTE_ASSISTMODE_STATUS, false);
	
    // POS18320 don't set it in here as it will put the wrong text for SystemMessage
	//co.SetInAssistMenus(false);
    // e pos18320
	
	//nTenderType = TB_TENDER_CASH; //TAR338001
    nTenderType = TB_TENDER_NONE;
	g_bGotVPIMInAssistMode = false;
	
	// POS15035: A need-more-data item could of been rung up while in Assist Mode
	// so clear flag so that there are no delays when customer goes to Finish & Pay.
	g_bTBNeedMoreData = false;
	
	// POS31050 don't reset the m_bDataNeededAssistMode if it's in the middle of mobile streamline intervention
    if(m_bDataNeededAssistMode && GetHookProperty(_T("PropMobShopOrderPrompt")) != _T("1") )
    {
        m_bDataNeededAssistMode = false;
    }
	
	//check the input window. if we exit if the input window is not
    //empty we'll get the error_context screen if we scan
    CString csActive;
    ps.GetInput( csActive );
    CString csClearKey = _T("{CLEAR}");
    if(!csActive.IsEmpty())
    {
        trace( L6, _T("SMSmAssistMode::UnInitialize() sending CLEAR before exit. Input window not clear") );
        SendTBCommand(TBCMD_SENDKEYEDDATA, csClearKey);
    }
	
	SetHookProperty(_T("IsResumeTrxInProgress"), _T("0"));
	
    // TAR 354798: Value set to 1 after initiating a Transaction Suspend {TRANS} {F1}, here we reset it
	SetHookProperty(_T("SuspendingCurrentTransaction"), _T("0"));
	
	CString strOptionName = _T("AllowACSToIgnoreItemsOnResume");
	CString m_csIsIgnoreItems;
	SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &m_csIsIgnoreItems);
	
	if(m_csIsIgnoreItems != _T("N"))
	{
		SetHookProperty(_T("IsACSIgnoreItemsOnResume"), _T("1"));
	}
	
	bool bTBExitAlready = false;
    if (!ms_bKeepRemoteDevices)
	{
		bTBExitAlready = true;
		if(!g_bGoingIntoHandHeld)
		{
			SendTBCommand(TBCMD_EXIT_ASSISTMODE, NULL);
			fSAInItemSale = false;		
		}
	}
    m_bSlipActivityIsWaiting = false;
    if((m_nEndorsementsPending) && (ra.GetRAPDataPendingCount()))
    {
		
        trace( L0, _T("ERROR:  Pending Endorsement not processed and exiting assistmode"));
        ra.OnNoRAPDataNeeded();
    } 
    SMSmAssistModeBase::UnInitialize();
	
    if ( bSetMonitorFalse && !bTBExitAlready )
	{
		if(!g_bGoingIntoHandHeld)
		{
			SendTBCommand(TBCMD_EXIT_ASSISTMODE, NULL);
			fSAInItemSale = false;		
		}
	}
    
	g_bGoingIntoHandHeld = false;
	
    if ( (!ms_bKeepRemoteDevices) && ( dm.IsDeviceRemote( DMCLASS_RECEIPT, 0 ) ))
    {
        trace( L0, _T("ERROR:  Device still remote after AssistMode Exit!!!!!"));
        trace (L6, _T("SMSmAssistMode::UnInitialize() Disconnecting remoted devices."));
        m_pAppPSX->SetConfigProperty(_T("ButtonToggle"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
        for (int i = 0; i < 10; i++)//TAR 355581 - changed i<8 to i<10 as now there can be at max 10 lanes
		{
			CString csTemp = EMPTY_STRING;
			csTemp.Format(_T("ButtonLaneStatus%d"), i);
			m_pAppPSX->SetConfigProperty(csTemp, csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
		}
        dm.EndRemote();
        trace (L6, _T("SMSmAssistMode::UnInitialize() Disconnecting remoted devices."));
    }       
    
    SetHookProperty(_T("WICItemList"), _T("0"));	//POS154826
}

bool SMSmAssistMode::PSRemoteLegalForState()
{
    return SMSmAssistModeBase::PSRemoteLegalForState();
}

SMStateBase * SMSmAssistMode::TimedOut(void)
{
    static nPrevTimeoutValue = 999;

	long lDrawerState;
	TBGetExpectedCashDrawerState(&lDrawerState);

	if (!ps.RemoteMode() || !dm.IsDeviceRemote(DMCLASS_CASHDRAWER, 0) && (lDrawerState & 1))  
    {
		//TAR 304732
		SendTBCommand(TBCMD_CASHDRAWER_CLOSE, NULL );

    }

    if(m_bTenderInProgress)
    {
        if(nPrevTimeoutValue == 999)
        {
            nPrevTimeoutValue = nTimeOut;
            nTimeOut =1;
        }

        if(SendTBCommand(TBCMD_TENDER_IN_PROGRESS, NULL) == 0)
        {
            //SOTF 6840
            SendTBCommand(TBCMD_NOTIFY_ACS_REMOTE_ASSISTMODE_STATUS, (ps.RemoteMode() ? true : false));

            m_bTenderInProgress = false;
            nTimeOut = nPrevTimeoutValue;
            nPrevTimeoutValue = 999;
            
            EnableDisableAllKeys(true);            
        }

		TBShowDisplay(true); // SOTF6235
        return STATE_NULL;
    }
	//if (nTimeOut == TIMEOUT_DISABLED)
	//{
	//	trace(L6,_T("+SMSmAssistMode::TimedOut reenabling keys"));
		//reenable keys,  we haven't gotten a new display
	//	TBShowDisplay();
	//	return STATE_NULL;
	//}
	//TAR356170 and 354819
	if (fWaitingOnBelt)
	{
		m_lWaitCount++;  // We won't wait forever
	}
	if(co.IsAttendantModeOn() || (m_lWaitCount > 4))
	{
		fWaitingOnBelt = false;
	}

	if (lCoupons && !IsUsingCouponSensor())
	{
		if (!bDropoffRequired && (getAnchorState() != BES_SCANANDBAG))
        {
		    fStateAssist = false;
        }
	}


    //SOTF 8172
    //ensure we return to scan and bag if there are age restricted item
    //the base makes you handle them on the exit from assistmode
    //Meijer stated they want to handle when the guest presses finish and pay
    SMStateBase  *pNextState = SMSmAssistModeBase::TimedOut();
    if ((SendTBCommand(TBCMD_ACS_DELAYED_AGE_RESTRICTION)) &&
        (isRestrictedItemApprovalRequired()) ||
		(isGenericDelayedInterventionApprovalRequired()) && //SSCOIA-39570 CRD 185620
        (getAnchorState()==BES_SCANANDBAG))
	{		
            dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
								DM_TRICOLORLIGHT_ON,
								DM_TRICOLORLIGHT_NORMALREQUEST);

	        delete getRealStatePointer( pNextState); 
            RETURNSTATE(ScanAndBag);       

    } 
    
    //POS 10185
    if (getAnchorState() == BES_ENTERWEIGHT )
    {
 	    setAnchorState(BES_SCANANDBAG);
    }

    // POS10984: Normally, if the current anchor state is BES_CARDPROCESSING, then we
    // will return to the Scan&Bag screen with no problems. However, there is an unusual
    // occurrence of a card being swiped in Assist Mode's Payment Menu with a pending
    // approval that causes the system to get out-of-sync, thinking it needs to continue
    // a previously completed card process. To fix this, we must anchor the BES_SCANANDBAG
    // state whenever the current anchor state is BES_CARDPROCESSING.
	CString csPosState = GetHookProperty(_T("__POSState"));
    if (getAnchorState() == BES_CARDPROCESSING && csPosState.Find(_T("WAITING_ON_PINPAD")) == -1)
    {
        trace (L3, _T("TimedOut() Changing anchor state from BES_CARDPROCESSING to BES_SCANANDBAG"));
        setAnchorState(BES_SCANANDBAG);
    }

    // +issue 7234
    //is SSCO going to return to the scan and bag screen
    //when we exit from assist mode.  If so, we need to check
    //where the POS is and to match sure we go to the correct
    //matching state
    //POS36514 - Would need to check the getAnchorState() rather than pNextState to make the logic works - rrs
    if(getAnchorState() == BES_SCANANDBAG || getAnchorState() == BES_CMDATAENTRY )
    {
        //is the POS at the item menu
        CString csPosState = GetHookProperty(_T("__POSState"));

        //POS36514 - occasionally when exiting from RAP AM, the POS is updating the total and POSState is not at PAYMENT menu, so we need to handle this scenario differently by checking the current dynaframe ID - rrs
        long lDynaframeID;
        _variant_t vFieldValue = GetFieldValue(_T("DF_ID"));
        lDynaframeID = vFieldValue.intVal;                

        if( (csPosState.Find(_T("PAYMENTMENU")) != -1) || (csPosState.Find(_T("CUSTOMER_TENDER_SELECTED")) != -1) || ( csPosState.Find(_T("CMTOTAL_UPDATE")) != -1 && lDynaframeID == 58 ) )
        {
			delete getRealStatePointer( pNextState);
            return setAndCreateAnchorState(BES_FINISH);
        }  
        else if( lItems == 0 && !isAnyApprovalRequired(false) && csPosState.Find(_T("MISCMENU")) != -1 && lDynaframeID == 40 )     // POS152889
        {
            trace(L6, _T("Exit to Attract after NO SALE procedures"));
            delete getRealStatePointer( pNextState);
            return setAndCreateAnchorState(BES_FINISH);            
        }   // e - POS152889 */
    }
	// -issue 7234

	//CRD 212453 AM issue where it goes to Scan and Bag instead of Finish
	trace(L6, _T("SMSmAssistMode::Timedout - lUnapprovedEASItems - %d, getAnchorState() - %d"), lUnapprovedEASItems, getAnchorState());
	if(getAnchorState() != BES_FINISH && (lUnapprovedEASItems || lUnapprovedMobileGenericDelayed || bUnapprovedAccountLookupFailed))	//CRD 223409, 554916
	{
		m_GenericDelayedInterventionStatus = GENERIC_DELAYED_COMPLETE;
		delete getRealStatePointer( pNextState);
        return setAndCreateAnchorState(BES_FINISH);
	}
	
	//CRD 291250 - in the event SCO is waiting for response from the POS
	//check if there's alcohol limit exceeded but POS went to Payment Menu, means interventions has been cleared so we can proceed to item menu
	CString csHasAlcoholLimitExceeded = GetHookProperty(_T("AlcoholLimitExceeded"));
	if(csHasAlcoholLimitExceeded == _T("1") && csPosState.Find(_T("PAYMENTMENU")))
	{
		SetHookProperty(_T("AlcoholLimitExceeded"), _T("0"));	//reset because alcohol limit exceeded intervention has been cleared
		delete getRealStatePointer( pNextState);
        return setAndCreateAnchorState(BES_FINISH);
	}
	
	//Start CRD 265301 - go to Finish and let it go to Attract gracefully
	if((GetHookProperty(_T("IsSASTimeOut")) == _T("1")))
	{
		SetHookProperty(_T("IsSASTimeOut"), _T("0"));
		delete getRealStatePointer(pNextState);
		return setAndCreateAnchorState(BES_FINISH);
	}
	//End CRD 265301
    
	return pNextState;
}

bool SMSmAssistMode::assistMode()
{
    return SMSmAssistModeBase::assistMode();
}

SMStateBase * SMSmAssistMode::DMRemoteScannerConnect(bool bConnect)
{
    return SMSmAssistModeBase::DMRemoteScannerConnect(bConnect);
}

SMStateBase * SMSmAssistMode::PSButton1(void)
{
    // SOTF 8286 - RRS
    _variant_t bAllowExit = GetFieldValue(_T("AllowAMExit"));
	CString csIsEverseenAlert = GetHookProperty(_T("IsEverseenAlert"));	//CRD 347846
    // E - SOTF 8286 - RRS
	
	if( fSAInItemSale && dm.fStateTakeawayBelt )
	{
		trace(L6, _T("Sending CANCEL_ITEM to SA before exiting AM to synch as the previous item wasn't sold(it could be DL/Coupon etc)"));
		SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);		
	}

/*    if(ps.GetRemoteModeOnLane())
    {
        
       ps.SetRemoteModeOnLane(false);
    }*/
// (+) POS181195/POS181197
    if(SMState::m_bActivateHHScanner) //CRD124746
    {
//        SMSmAssistMode::m_bHHScanerActive = false;
//        ps.Button(_T("SMButton2"),SSF_TXT_SCANNER_ACTIVE ,false);
            SMState::m_bActivateHHScanner = false;
    
        CString csRAPName;
    
        if(SMSmAssistMode::m_csRAPConnectionName == "")
        {
            SMSmAssistMode::m_csRAPConnectionName = SMState::GetRAPConnectionName();
        }
   

        CString csOperation=_T("operation=release-scanner;");
        m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), csOperation, SMSmAssistMode::m_csRAPConnectionName );
    

    }
// (-) POS181195/POS181197

    if( m_bRequestingCashDrawerOpen )
    {
        trace(L6, _T("'Return To Shopping' button click ignored - Cash drawer pop was requested."));
        return STATE_NULL;
    }
    else if( m_bTenderInProgress ) // SOTF6235
    {
        trace(L6, _T("'Return To Shopping' button click ignored - Tender in progress."));
        return STATE_NULL;
    }
    else if( !bAllowExit.boolVal && csIsEverseenAlert == _T("0") ) // SOTF 8286 - RRS; CRD 347846
    {
        trace(L6, _T("'Return To Shopping' button click ignored - ItemSold in progress."));
        return STATE_NULL;
    }                       // E - SOTF 8286 - RRS

	//CRD 347846
	//due to items that are not sold such as item not on file, recalled item, etc.
	//reset IsEverseenAlert flag when exiting to Scan and Bag
	if(csIsEverseenAlert == _T("1"))
	{
		g_bIsProcessingEverseenAlert = false;
		SetHookProperty(_T("IsEverseenAlert"), _T("0"));
	}

    SMStateBase* pRet = SMSmAssistModeBase::PSButton1();
    SAWLDBScannerDisable();

    // POS31054 - RAP Mobile lane light and make sure that we check the RemoteMode so that the exit AM can be processed correctly from Remote AM - rrs
    // POS31050 - don't call the TimedOut during the mobile resume
    CString csPosState = GetHookProperty(_T("__POSState"));    
    if( !ps.RemoteMode() && GetHookProperty(_T("PropMobShopOrderPrompt")) == _T("1") && csPosState.Find(_T("PAYMENT")) == -1 )
    {
        trace(L6, _T("Stay at RAPDataNeeded for MobileAssistance"));
        nTimeOut = 0;

        co.SetInAssistMenus(false) ;
        tb.SetMonitorAll(false);
        fStateAssist = false;

        return pRet;
    }
    // e POS31050 - don't call the TimedOut during the mobile resume

	if( GetTBProperty(_T("IsFromEndorsement")) == _T("1"))
	{
		SetTBProperty(_T("IsFromEndorsement"), _T("0"));
		SendTBCommand(TBCMD_SET_TB_STATE,_T("TENDER_PAYMENT_ENDORSEMENT_IP"));
	}

	//Start CRD 329260 - SS90 Negative Total
	CString csCurTBState(_T(""));
    SendTBCommand(TBCMD_GET_TB_STATE, &csCurTBState);
	if (csPosState.Find(_T("ITEMMENU_DK1")) && csCurTBState.Find(_T("ASSISTMODE_EXIT")) || csCurTBState.Find(_T("TB_ENTERTENDERSTATE_IN"))
		|| csCurTBState.Find(_T("TENDER_CASH")))
	{
		SASendSCOTInput( TRANSACTION_CONTINUE );
		return setAndCreateAnchorState(BES_SCANANDBAG);
	}
	//End CRD 329260

	//Start CRD 289149, 554916 - process interventions right away when going to Assist Mode	
	CString csCurPosState = GetHookProperty(_T("__POSState"));
	CString csHasAlcoholLimitExceeded = GetHookProperty(_T("AlcoholLimitExceeded"));

	if ( (csCurPosState.Find(_T("ITEMMENU")) != -1 || csCurPosState.Find(_T("VOIDMENU")) != -1) && 
		(isAnyApprovalRequired() || lUnapprovedEASItems || lUnapprovedMobileGenericDelayed || bUnapprovedAccountLookupFailed || csHasAlcoholLimitExceeded == _T("1")))
	{
		trace(L6, _T("Need to approve outstanding interventions"));
		if(isRestrictedItemApprovalRequired())
		{
			lUnapprovedRestrictedItems =0;
			fApprovedForRestrictedItems = true;
		}
	
		if (isGenericDelayedInterventionApprovalRequired())
		{
			m_csCurrentIntervention = m_GenericDelayedInterventions.GetLast();
			
			CString m_csLastInterventionID = _T("");
			
			if(m_csCurrentIntervention != NULL)
			{
				trace(L6, _T("m_csCurrentIntervention is not NULL"));
				
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
		CString posKey = _T("{PAYMENT}");
		SendTBCommand(TBCMD_SENDKEYEDDATA, posKey);
		
		fWaitingToExit = false;	//CRD 291250 we are not going to exit since we have an outstanding intervention
		nTimeOut=0;
		return pRet;
	}
	//End CRD 289149 - process interventions right away

    // Speed up AssistMode Exit by removing the base's artificial 1 second delay.
    if ( STATE_NULL == pRet )
    {
        return pRet;
    }
    else
    {
		delete getRealStatePointer( pRet); 
        return TimedOut();
    }
}

// (+) POS181195/POS181197
SMStateBase * SMSmAssistMode::PSButton2(void)
{
    // ps.Button(_T("SMButton2"),SSF_TXT_SCANNER_ACTIVE ,false);
	
	
    CString csOperation=_T("operation=request-scanner-");
	
	CString csCompName=_T("");
	if(m_csLaneName == _T(""))
	{
		csCompName = SMState::GetLaneName();
	}
	else
	{
		csCompName = m_csLaneName;
	}
	csCompName += _T(";");
	
    csOperation = csOperation + csCompName;
	
	
	
	if(SMSmAssistMode::m_csRAPConnectionName == "")
	{
		SMSmAssistMode::m_csRAPConnectionName = SMState::GetRAPConnectionName();
	}
	
	m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), csOperation, SMSmAssistMode::m_csRAPConnectionName );
	
	//SMSmAssistMode::m_bHHScanerActive = true;
	
	
    return STATE_NULL;
}
// (-) POS181195/POS181197

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Method:      SMStateBase  *SMSmAssistMode::PSAssistKey
//
// Purpose:     Send TB the assist mode key press
//
// Parameters:  const long nKey
//
// Returns:     SMStateBase
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SMStateBase  *SMSmAssistMode::PSAssistKey(const long nKey)// Assist Mode or Stop program
{
   trace(L0,_T("+Assist Mode %d"),nKey);     

    ps.GetInput(g_csInputData);
	
    CString csPOSString = TBGetKeyPOSString(nKey);
    CString csTitle = GetDisplayText(_T("Title1"));
    
	// SSCOI-45194
	// This is a migration issue. Removing code below after corrected the migration error.
	//CLaneStateData laneStateData(m_pAppPSX);
    //laneStateData.SendNextGenUIData(_T("RemoteProxyUserInputCompleted"), true);
	
    //POS18320 - rrs
    // Ignore all the dynaframe keystrokes when we are waiting to end of trx as it will cause the OOS situation
    if( fWaitingForEOT )
    {
        trace(L6, _T("Ignoring Key %s was pressed in %s screen."), csPOSString, csTitle);
        return STATE_NULL;
    }
    //e pos18320

	trace(L6, _T("    Key %s was pressed in %s screen."), csPOSString, csTitle);

	//get the current dynaframe ID
    long lDynaframeID;
    _variant_t vFieldValue = GetFieldValue(_T("DF_ID"));
    lDynaframeID = vFieldValue.intVal;


	//Start CRD 289150 - call Finish Recall Done once OK button is pressed
	if(bShowFinishRecall && csPOSString.CompareNoCase(_T("{F1}")) == 0 && !bShowSmRecallingItems)
	{
		AMFinishRecallDone();
		return STATE_NULL;
	}
	//End CRD 289150

	// Start CRD 185620
	// CRD 223409 mobile assistance - trigger to TB to send TB_GENERIC_DELAYED_COMPLETE when OK button for Operator Message 1840 is pressed
	// CRD 554916 Account Lookup Status delayed intervention
	// CRD 557225 Account Detaisl Failed delayed intervention
	_variant_t nOperGuidNum = GetFieldValue(_T("OperGuid_Num"));
	if((nOperGuidNum.intVal == OPERATORMSGNO_EASTAGS && csPOSString.CompareNoCase(_T("{F1}")) == 0 && lDynaframeID == DF_OPERATORMSG) //for EAS item
		|| (lDynaframeID == DF_MOBILEASSISTANCENEEDED && csPOSString.CompareNoCase(_T("{F1}")) == 0)
		|| (nOperGuidNum.intVal == OPERATORMSGNO_ACCOUNTLOOKUPFAILED && csPOSString.CompareNoCase(_T("{F1}")) == 0)
		|| (nOperGuidNum.intVal == OPERATORMSGNO_GETACCOUNTDETAILEDFAILED && csPOSString.CompareNoCase(_T("{F1}")) == 0))
	{
		trace(L6, _T("let's trigger TB to send TB_GENERIC_DELAYED_COMPLETE lUnapprovedEASItems - %d; lUnapprovedMobileGenericDelayed - %d; bUnapprovedAccountLookupFailed - %d"), lUnapprovedEASItems, lUnapprovedMobileGenericDelayed, bUnapprovedAccountLookupFailed);

		if(!lUnapprovedEASItems || !lUnapprovedMobileGenericDelayed || bUnapprovedAccountLookupFailed)
			bIsGenericDelayedProcessed = true;	//set this flag in the event for second intervention, SCO will clear immediately the generic delayed intervention in SMState::OnGenericDelayedComplete
		//CRD 232270
		if((nOperGuidNum.intVal == OPERATORMSGNO_EASTAGS && csPOSString.CompareNoCase(_T("{F1}")) == 0 && lDynaframeID == DF_OPERATORMSG))
			m_pAppPSX->ClearReceipt( _T("CMSMReceiptEAS") );

		SendTBCommand(TBCMD_EASCOMPLETE, false);		
	}
	else if(nOperGuidNum.intVal == OPERATORMSGNO_EASTAGS && csPOSString.CompareNoCase(_T("{F1}")) != 0 && lDynaframeID == DF_OPERATORMSG)	//CRD 213258 ignore key presses if operator msg 1840 is showed
	{
		return STATE_NULL;
	}
	//End CRD 185620
	//Start CRD 232267
	//CRD 289150 recall is still pending that needs to be dismissed, don't send {Payment} key
	else if(!bFinishRecall && (nOperGuidNum.intVal == OPERATORMSGNO_MPERKSOFFLINE && csPOSString.CompareNoCase(_T("{F1}")) == 0 && lDynaframeID == DF_OPERATORMSG))
	{
		//we need to tell POS to go to Payment Menu since it we are processing generic delayed
		bGenericDelayedFinishSent = true;
		setAnchorState(BES_FINISH);
		
		SendTBCommand(TBCMD_ENTERING_FINISH_AND_PAY, false);
        
		CREATE_AND_DISPENSE(InProgress)(0,
			helpMode(),
			co.getTimeOutTender());
		
		return STATE_NULL;
	}
	//End CRD 232267
	//Start CRD 265301 - no need to go Full Assist Mode, go back to Attract
	else if(nOperGuidNum.intVal == OPERATORMSGNO_SASTIMEOUT && csPOSString.CompareNoCase(_T("{F2}")) == 0 && lDynaframeID == DF_OPERATORMSG)
	{
		return PSButton1();	//OnReturnToShopping
	}
	//End CRD 265301
	   
    //TAR348278+
	trace(L6, _T("bSND  lDynaframeID = %d  Key %s was pressed in %s screen."), lDynaframeID, csPOSString, csTitle);
    if (     !ps.RemoteMode()
		 &&  (lDynaframeID == 58 )
		 &&  (    (csPOSString.CompareNoCase(_T("{ENTER}")) == 0) 
		       || (m_nAssistKeyVirtualKeyValue[nKey] >= VK_NUMPAD0 && m_nAssistKeyVirtualKeyValue[nKey] <= VK_NUMPAD9)
             )
       )   
    { 
		//we ignore this key press
		trace(L0,_T("+Assist Mode at lane; Payment Menu; key press %d is ignored"),nKey);
		return STATE_NULL;
	}
	//TAR348278-

    //POS37397 - clear the rainck buffer list - rrs
    if( lDynaframeID == 604 )
    {
        //clear the rainck buffer when the softkey is presses
        SetHookProperty(_T("PrepaidCardList"), _T(""));
        SetHookProperty(_T("VoidPrepaidCards"), _T("0"));
    }
    //e POS37397 - clear the rainck buffer list - rrs

    bool bSendKeysToPos = IsKeyAllowed(csPOSString, nKey);   
	//TAR341910
	if( bSendKeysToPos && (nKey > 23) ) // disabled all dynakeys after pressing anyone of it.
		DisableSoftKeys();

    //check if we are on one of the special acct screens check. If so we need to handle
    //clearing of the input field and backspaces differently
    bool bSpecialScreenHandling = IsSpecialHandlingScreen();

    if(bSendKeysToPos)
    {
		// Start CRD 125377
		if (((m_bShopAndDropEnabled) && (lDynaframeID == 13) && (nKey == 24) && (m_bSNDSettingAge)) ||
			((m_bShopAndDropEnabled) && (lDynaframeID == 1010) && (nKey == 24)) && (m_bSNDSettingAge))
		{
			m_bSNDAgeEntered = true; // set Age input to true
			m_bSNDAgeEnteredFound = true; // set Age input to true
			m_bSNDSettingAge = false;
			
			TBGetMessageInfo(false);
		}
		// End CRD 125377
        // Disable the Return to shopping button immediately; although disabled after drawer pops anyway
        // there is a small gap during which the user may press the RTS button immediately after pressing F3
        if ( (-1 != csTitle.Find(_T("MISCELLANEOUS MENU"))) &&
             (-1 != csPOSString.Find(_T("{F3}"))) )
        {
            trace(L6, _T("'Return To Shopping' button will be blocked while waiting for drawer to pop."));
            m_bRequestingCashDrawerOpen = true;            
        }

      if (nKey > 23)
      {
         if ( (GetHookProperty(_T("__POSState")).Find(_T("HIGHVALUE_PROMO_CHOICE")) != -1) && (GetHookProperty(_T("HighValuePromoChoice")) == _T("1")) )
         {
            // POS39475: Clear high value promo data because the promo is now going to be responded to.
            trace(L6, _T("HighValuePromoChoice softkey %d response"), (nKey-23));
            SetHookProperty(_T("HighValuePromoChoice"), _T("0"));

			//CRD 289150
			if(!bShowB2BHighValuePromo) 
				bShowB2BHighValuePromo = true;

			//commenting this out to fix blank screen issue for high value prompts
            //SetHookProperty(_T("tb_char"), _T(""));
            //SetHookProperty(_T("tb_ExpirationDate"), _T(""));
         }
         else if ( (GetHookProperty(_T("__POSState")).Find(_T("VENDOR_COUPON_CHOICE")) != -1) && (GetHookProperty(_T("CouponChoice")) == _T("1")) )
         {
            // POS33020: Clear coupon choice data before sending response to sales app.
            trace(L6, _T("CouponChoice softkey %d response"), (nKey-23));
            SetHookProperty(_T("CouponChoice"), _T("0"));
            SetHookProperty(_T("tb_char"), _T(""));
            SetHookProperty(_T("tb_ExpirationDate"), _T(""));
            SetHookProperty(_T("item_code"), _T(""));
            SetHookProperty(_T("tb_mnemonic"), _T(""));
            SetHookProperty(_T("tb_Item_rec_flags"), _T("0"));
         }
      }

		// TAR 359555+
		int nCompareEnter = csPOSString.CompareNoCase(_T("{ENTER}"));

		if ( (nCompareEnter == 0) && (GetHookProperty(_T("AppleSerialNumberEntry")) == _T("1")) )
		{
			// POS41473: Enter key was hit during manual entry of serial number for  Apple item.

			if (lDynaframeID != 373)
			{
				// Sales app will need to process the manually entered serial number of the Apple 
				// item by communicating with the high-order Apple system which could take some time, so 
				// disable softkeys and display a 'please wait' leadthru message for the attendant.
				DisableSoftKeys();
				ps.LeadThruText(TXT_PLEASEWAIT);
				SetHookProperty(_T("AppleSerialNumberEntry"), _T("0"));
				}
			else
			{
				trace(L0,_T("Ignore Enter key during alpha entry dynaframe of Apple serial number"));
				return STATE_NULL;
			}
		}

		if ( (lDynaframeID == 182) && ( (m_nAssistKeyVirtualKeyValue[nKey] >= VK_NUMPAD0 && 
					m_nAssistKeyVirtualKeyValue[nKey] <= VK_NUMPAD9) || 
					( nCompareEnter == 0) ) )
		{
			m_csEntryDate += csPOSString;

			if ( nCompareEnter == 0 )
			{
				SendTBCommand(TBCMD_SENDKEYEDDATA, m_csEntryDate);
				m_csEntryDate = _T("");
			}
		}
		else
		{
			SendTBCommand(TBCMD_SENDKEYEDDATA, csPOSString);
		}
		// TAR 359555-

        //POS31050 - if CLEAR key is pressed during AgeVerify screen, then reset the display
        int nCompareClear = csPOSString.CompareNoCase(_T("{CLEAR}"));

	    if ( (lDynaframeID == 182) && (nCompareClear == 0) )
	    {
            m_csEntryDate = _T("");

		    TBShowDisplay(true);
	    }
        // e - pos31050
        
        //display the numeric keys and no other work needs to be done
        if (m_nAssistKeyVirtualKeyValue[nKey] >= VK_NUMPAD0 && 
            m_nAssistKeyVirtualKeyValue[nKey] <= VK_NUMPAD9)   //numeric key
        {
                //if we don't have any of the input data from the DF_Alphanum_display_lc
                //context then go ahead and display the key pressed
                
                if ( bSpecialScreenHandling == false )
                {
                    PSEchoInput(VirtKeyToTChar(m_nAssistKeyVirtualKeyValue[nKey],0));
                }
            
        }else
        {
            //if the clear, enter, hard or softkeys were pressed go ahead and clear the input field
            if ((m_nAssistKeyVirtualKeyValue[nKey] == VK_RETURN || m_nAssistKeyVirtualKeyValue[nKey] == VK_F17) ||
                ((m_nAssistKeyVirtualKeyValue[nKey] >= VK_F1 ) && (m_nAssistKeyVirtualKeyValue[nKey] <= VK_F8)) ||
                ((nKey >= 1) && (nKey <= 12)) // one of the hard keys
               )
                
            {
                  bool bSendClear = true;                                  

                  if(bSpecialScreenHandling)
                  {
                     //the backspace softkey was pressed?  We have
                     //to handle that key specially
                     //get the text of the softkey.  Don't blindly assume that
                     //F1 is backspace
                     CString csSoftkeyPressedText = GetSoftKeyButtonText(nKey);
                     csSoftkeyPressedText.MakeUpper();
                     if(csSoftkeyPressedText.Find(_T("BACKSPACE")) != -1)
                     {

                        bSendClear = false;
                        CString csEntryDataText = GetDisplayText( _T("EntryData") );
                        //if we are on one of the special screen and the input data from the DF_Alphanum_display_lc
                        //is empty then clear the FL input window
                        if(csEntryDataText.IsEmpty())
                        {
                            bSendClear = true;
                        }
                     } 
                  }

                  if(bSendClear)
                  {
                       AMClearAllInput(); 
                  }
                  //a non numeric key pressed see if we need to clear any flags or
                  //set TB states
                  ResetPropertiesBasedOnScreenAndKeys(csPOSString, csTitle);
            }


        }

        /* Not Needed - rrs
		// sotf 9034 - resume o2s trx then scan an item - rrs
        if( csPOSString.Find( _T("{F3}") ) != -1 && csTitle.Find(_T("Signature Verify")) != -1 )
        {
            lItems = 0;		// TAR363054
		    ps.LeadThruText(TXT_PLEASEWAIT);
		    SendTBCommand(TBCMD_RESUME_ITEMS, NULL);
            trace(L6, _T("Display the Item during O2S Resumed Transaction"));
        }
		*/

    }

	//+CRD185924
	//_variant_t nOperGuidNum = GetFieldValue(_T("OperGuid_Num"));
    _variant_t bIsOperGuid = GetFieldValue(_T("IsOperGuid"));
	
	trace( L6, _T("SMSmAssistMode::PSAssistKey() - bIsOperGuid = %d, and nOperGuidNum = %d."), bIsOperGuid.boolVal, nOperGuidNum.intVal);
	
	if (bIsOperGuid.boolVal && nOperGuidNum.intVal == 1845)
	{
		trace( L6, _T("SMSmAssistMode::PSAssistKey() - csPOSString = %s"), csPOSString);
		EnableDisableAllKeys(true);
		
		if(csPOSString == _T("{F1}") || csPOSString == _T("{F2}"))
		{
			if(csPOSString == _T("{F2}"))
			{
				trace( L6, _T("SMSmAssistMode::PSAssistKey() - going to SMOutOfService2 and wait for POS to restart."));
				m_bWasPOSRestarted = true;
				fStateFreeze = false; 
				ps.LeadThruText(TXT_PLEASEWAIT);
				SendTBCommand(TBCMD_SET_TB_STATE,_T("ASSISTMODE_POS_QUICK_RESTART"));
				setAnchorState(BES_OUTOFSERVICE2);
				return STATE_NULL;
			}
			else
			{				
				trace( L6, _T("SMSmAssistMode::PSAssistKey() - Pressing 'Return to Shopping' button automatically."));
				SendTBCommand(TBCMD_SET_TB_STATE,_T("ASSISTMODE_EXIT")); //needed in order to prevent going to scanandbag.
				return SMSmAssistModeBase::PSButton1();
			}
		
		}
	} //-CRD185924
 
    

    return STATE_NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Method:      SMStateBase  *SMSmAssistMode::PSNumericKey
//
// Purpose:     Find keycode for numberic keys. Send these keycode to TB 
//
// Parameters:  const long lKey
//
// Returns:     SMStateBase
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SMStateBase  *SMSmAssistMode::PSNumericKey(const long lKey)
{
  trace(L0,_T("+SMSmAssistMode::PSNumericKey %d"),lKey);


  //find keycodes for numberic keys
  for(int i=0; i<200; i++)
  {
      CString csLabelTemp;
      TBGetKeyLabel(i, csLabelTemp);
      if(!csLabelTemp.IsEmpty() && ((lKey-48) == _ttoi(csLabelTemp) && lKey != 48)  // not '0' key
          || (lKey == 48 && csLabelTemp == "0"))   // process '0' key
      { 
        //find the matched numberic key
        TBKeyPress(TBGetKeyCode(i));   //Send the keycode to TB
        TBShow2x20();                  //Update 2x20
        return STATE_NULL;
      }
  }

  trace(L0,_T("-SMSmAssistMode::PSNumericKey Did not find matched key for %d"),lKey);

  return STATE_NULL;
}

SMStateBase * SMSmAssistMode::PSEnterKey()
{
    return SMSmAssistModeBase::PSEnterKey();
}

SMStateBase * SMSmAssistMode::PSClearKey()
{
    return SMSmAssistModeBase::PSClearKey();
}

SMStateBase * SMSmAssistMode::PSCancelKey()
{
    return SMSmAssistModeBase::PSCancelKey();
}

SMStateBase * SMSmAssistMode::DMScanner()
{
    SMStateBase *nextState = STATE_NULL;
	//check the input window. if we send a scan and it's not
    //empty we'll get the error_context screen
    CString csActive;
    ps.GetInput( csActive );
    CString csClearKey = _T("{CLEAR}");
    if(!csActive.IsEmpty())
    {
        trace( L6, _T("SMSmAssistMode::DMScanner() sending CLEAR before scan data. Input window not clear") );
        SendTBCommand(TBCMD_SENDKEYEDDATA, csClearKey);
    }
	// Is this a raincheck redemption?
    // Assume raincheck if code 139 (type 108) AND starts with "R"
	
    // POS59850 - Handle Code128 raincheck barcode. rrs
	if ((csDMLastBarType == SCAN_SDT_Code39 || csDMLastBarType == SCAN_SDT_Code128)&&
		(csDMLastBarLabel.Mid(0, 1) == _T("R")))
	{
		PSAssistKey(5);
        // POS59850 - Add 'B3' code128 prefix back to the scan data for Code128 raincheck barcode. rrs
		// Add "B1" back to the start of barcode data.
        CString csScanCode;
        if( csDMLastBarType == SCAN_SDT_Code39 )
        {
            csScanCode = _T("B1");
            csScanCode += csDMLastBarLabel;
			SendTBCommand(TBCMD_SENDSCANDATA, (long)108, csScanCode);
        }
        else
        {
            csScanCode = _T("B3");
            csScanCode += csDMLastBarLabel;
			SendTBCommand(TBCMD_SENDSCANDATA, (long)110, csScanCode); 
        }
		
		
		DMScannerEnable();
	}
	else
	{
		// Otherwise, call super.
		
		if (GetHookProperty(_T("AppleSerialNumberEntry")) == _T("1"))
		{
			// POS41473: Sales app will need to process the scanned serial number of the Apple item by
			// communicating with the high-order Apple system which could take some time, so disable
			// softkeys and display a 'please wait' leadthru message for the attendant.
			DisableSoftKeys();
			ps.LeadThruText(TXT_PLEASEWAIT);
			SetHookProperty(_T("AppleSerialNumberEntry"), _T("0"));
		}
		
		nextState = SMSmAssistModeBase::DMScanner();
	}
	PSClearAllInput();
	return nextState;
}

SMStateBase * SMSmAssistMode::DMCardReader(void)
{
    return SMSmAssistModeBase::DMCardReader();
}

SMStateBase * SMSmAssistMode::DMScale()
{
    // let the base prime the 'virtual' scale
	SMStateBase  *sExit = SMStateBase ::DMScale();
	if (sExit!=STATE_NULL)
		return sExit;
	
	long lCurrentWeight = SMStateBase::lDMScaleWeight;
	TBSTATE nCurrentTBState = SMStateBase::nTBLastRealState;
	
	if (lCurrentWeight == 0)
    {   
        //pos15745  rrs
        // Must zero scale for ACS to reset the scale
        TBSendWeight(0);
        trace(L6, _T("Reset to zero scale"));
        // e pos15745
		
		return STATE_NULL;	// shakey scale, wait for good weight
    }
	
	CString csCurPosState(_T(""));
    CString csProp = _T("__POSState");        
    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csProp, &csCurPosState);
	
	if ((nCurrentTBState == TB_NEEDWEIGHT) && (lCurrentWeight > 0) &&
		csCurPosState == _T("WEIGHT_REQUIRED"))
	{       
        //TBRC rc = tb.SendWeight(lCurrentWeight);
        TBRC rc = ::TBSendWeight(lCurrentWeight);
		ProcessTBReturn(rc);
	}
   return STATE_NULL;
}

SMStateBase * SMSmAssistMode::SAEndOfTransaction(void)
{
    return SMSmAssistModeBase::SAEndOfTransaction();
}

SMStateBase * SMSmAssistMode::DMAcceptor(void)
{
    return SMSmAssistModeBase::DMAcceptor();
}

SMStateBase * SMSmAssistMode::DMMICR(DWORD dwEvent)
{
    trace (L6, _T("+SMSmAssistMode::DMMICR(0x%08.8x)"), dwEvent);
    m_nMICREvents++;
    g_dwLastMICREvent = GetTickCount();
    switch(dwEvent)
    {
    case DM_DATA:
        ASSERT(m_bWaitingForMICRInsertion == TRUE);
        if (m_bWaitingForMICRInsertion)
        {
            CString sRawData;
            CString sAccountNumber;
            CString sTransitNumber;
            CString sBankNumber;
            CString sCheckNumber;
            long nCheckType;
            long nCountryCode;
            CString sAmount;
            CString sEPC;
            dm.MICRGetCheckData( sRawData, sAccountNumber, sAmount,
                sBankNumber, sEPC,sCheckNumber, sTransitNumber, &nCheckType, 
                &nCountryCode );
            
            trace (L6, _T("SMSmAssistMode::DMMICR() data event: sRawData=<%s> ")
                _T("sTransitNumber=<%s> sAccountNumber=<%s> sBankNumber=<%s> ")
                _T("sCheckNumber=<%s> sEPC=<%s> nCheckType=%d nCountryCode=%d"),
                sRawData, sTransitNumber, sAccountNumber, sBankNumber, 
                sCheckNumber, sEPC, nCheckType, nCountryCode);
			
            // DG30 check handling - ask TB whether to eject or not
            bool bEject = (TB_SUCCESS_IMMEDIATE == SendTBCommand(TBCMD_MICR_SHOULD_EJECT));
            dm.MICRDisable(bEject);
            m_bSlipAlreadyFlippedAfterApproval = false;
			
            m_bWaitingForMICRInsertion = FALSE;
			
            if( (GetDisplayText(_T("Title1")).Find(_T("Routing Number")) != -1) )
            {
                //Begin RFC 356020
                if (SMStateBase::nTenderType == TB_TENDER_CHECK_LANE)
                    TBEnterMICRWithType(SMStateBase::nTenderType, sRawData, sTransitNumber, sAccountNumber, sBankNumber, 
                    sCheckNumber, sEPC, nCheckType, nCountryCode);
                else
                    TBEnterMICR(sRawData, sTransitNumber, sAccountNumber, sBankNumber, 
                    sCheckNumber, sEPC, nCheckType, nCountryCode);
                //End RFC 356020
            }
            else
            {
                trace (L0, _T("SMSmAssistMode::DMMICR() MICR check data received but POS not on MICR/Routing Number screen. Discarding MICR data and not notifying TB."));
            }
            
            TBShow2x20();
			
            break;
        }
        trace (L0, _T("SMSmAssistMode::DMMICR() Unexpected MICR Data event received."));
        break;
        
    case DM_ERROR_TIMEOUT:
    case DM_ERROR_UNEXPECTED:
    case DM_ERROR_NO_HARDWARE:
    case DM_ERROR_GENERALFAILURE:
    case DM_MICR_ERR_CHECKJAM:
    case DM_MICR_ERR_LINE_ERROR:
    case DM_MICR_ERR_NOCHECK:
    case DM_MICR_ERR_NO_DATA:
    case DM_MICR_ERR_BAD_DATA:
    case DM_MICR_ERR_COVER_OPEN:
    case DM_MICR_ERR_BADSIZE:
    case DM_MICR_ERR_CHECKDIGIT:
	// +SSCOI-45608
	//SR672+
      case NCR_EMICR_LINE_ERROR:
	  case NCR_EMICR_NOCHECK:
	  case NCR_EMICR_NO_DATA:
	  case NCR_EMICR_BAD_DATA:
	  case NCR_EMICR_CHECKJAM:
	  case OPOS_E_FAILURE:
	  case OPOS_E_TIMEOUT:
	  case OPOS_E_NOHARDWARE:
	  case OPOS_E_OFFLINE:
	  //SR672-
	// -SSCOI-45608
        {
            if( m_bWaitingForMICRInsertion && 
                (GetDisplayText(_T("Title1")).Find(_T("Routing Number")) != -1) )
            {
                dm.MICRDisable();
                m_bWaitingForMICRInsertion = FALSE;
				
                //merged TAR233401 to 3.1.1 - fixed by Chen - Tell TB about this bad MICR
                TBEnterMICR(_T(""), _T(""), _T(""), _T(""),_T(""),_T(""), 0,dwEvent);
                trace (L0, _T("SMSmAssistMode::DMMICR() Fatal MICR error event received -- not retrying MICR read. Sending event to TB."));
            }
            else
            {
                dm.MICRDisable();
                m_bWaitingForMICRInsertion = FALSE;
                trace (L0, _T("SMSmAssistMode::DMMICR() Fatal MICR error event received -- TB not expecting MICR so we will not notify TB."));
            }
            TBShow2x20();
        }
        break;
    default:
        ASSERT(FALSE);
    }
    
    trace (L6, _T("-SMSmAssistMode::DMMICR()"));
    return STATE_NULL;
}

SMStateBase * SMSmAssistMode::OnNeedMICR()
{
    return SMSmAssistModeBase::OnNeedMICR();
}

SMStateBase * SMSmAssistMode::DMSlipPrinter(DWORD dwEvent)
{
    trace (L6, _T("+SMSmAssistMode::DMSlipPrinter(0x%08.8x)"), dwEvent);

    switch(dwEvent)
    {
    case PTR_SUE_SLP_PAPEROK:
        {
            trace (L6, _T("+SMSmAssistMode::DMSlipPrinter() Slip inserted."));
            if (m_nEndorsementsPending)
            {   
                DMSlipPrinterPrint(m_szSlipDocumentBack);
            }
            
        }
        break;
    case DM_PTR_REQUEST_COMPLETED:
        {
            if( m_bSlipFlipInProgress )
            {
                m_bSlipFlipInProgress = false;
                trace (L6, _T("SMSmAssistMode::DMSlipPrinter() Non-endorsement printer activity completed."));

                // An endorsement activity is waiting after a flip has completed
                // Only applicable in the processing of echecks where preflipping has occured
                // because there's no pending TB_ENDORSEDOCUMENT to be processed
                if( m_bSlipActivityIsWaiting && m_bSlipAlreadyFlippedAfterApproval )
                {
                    return OnEndorseDocument();
                }
                if( !m_szSlipDocumentDescription )
                {
                    break;
                }
            }
            
            trace (L6, _T("SMSmAssistMode::DMSlipPrinter() Endorsement <%s> completed."), m_szSlipDocumentDescription);
            // Successfully completed printing the document.
            fInEndorsementState = ENDORSE_NONE;
            
            // TAR 393280 - Always eject a tax exempt slip
            bool bIsTaxSlip = (GetTBProperty(_T("SlipDescTaxExempt")) == m_szSlipDocumentDescription);
            
            // Clear stored slip contents
            delete m_szSlipDocumentDescription;
            delete m_szSlipDocumentFront;
            delete m_szSlipDocumentBack;
            m_szSlipDocumentDescription = NULL;
            m_szSlipDocumentFront = NULL;
            m_szSlipDocumentBack = NULL;
    
            // we should only be ejecting the slip if this is the last endorsement or a tax slip
            // the endorse logic will also eject if required
            if( bIsTaxSlip || (m_nEndorsementsPending <= 1 && !m_bSlipActivityIsWaiting) )
            {
                trace(L6, _T("There are no more endorsements, ejecting slip."));
                DMSlipPrinterInsertSlip(false);
                m_bSlipPrintSideFront = false;
            }
    
            ShowCashDrawerCloseMessageIfApplicable(true);
    
            //TAR257468+
            //We need to clear RAP view if only if 
            if ( m_View == DATANEEDED_ENDORSEDOCUMENT )
            {
                ra.OnNoRAPDataNeeded();
            }
            //TAR257468-
    
            // Tell TB the slip was printed.
            TBEndorsementCompleted();
            TBShowDisplay(true, false);
            break;
        }
   
    case DM_PTR_EMPTY:
	case PTR_SUE_SLP_EMPTY:
        trace (L4, _T("SMSmAssistMode::DMSlipPrinter() slip empty."));
        break;
    case PTR_SUE_SLP_NEAREMPTY:
        trace (L4, _T("SMSmAssistMode::DMSlipPrinter() slip near empty."));
        break;
    //+TAR 435200
    case DM_PTR_ERR_EMPTY:
        if (m_nEndorsementsPending) 
        {
            // Open the throat and wait for a slip
            trace(L6, _T("Waiting for slip insertion..."));
            DMSlipPrinterInsertSlip(true);
            break;
        }
    //-TAR 435200    
    case DM_PTR_ERR_COVER_OPEN:    
    case DM_PTR_ERR_FAULT:
    case DM_PTR_ERR_REQUEST_SEQUENCE:
    case DM_PTR_ERR_HEAD_JAM:  
    case DM_PTR_ERR_KNIFE_JAM:
    case DM_PTR_ERR_PAPER_JAM:
    case DM_PTR_ERR_PAPER_NOT_LOADED_JAM:
    case DM_PTR_ERR_COMMS_ERROR:
    case DM_PTR_ERR_PRINTER_OPEN:
    case DM_PTR_ERR_REPLACE_HEAD:
    case DM_PTR_ERR_SIDEWAYS_COMMS_ERROR:
    case DM_PTR_ERR_EJECT_JAM:
    case DM_PTR_ERR_BLACKMARK_ERROR:
    case DM_PTR_ERR_TIME_OUT:
    default:
        trace (L0, _T("SMSmAssistMode::DMSlipPrinter() Error printing slip."));
        ShowCashDrawerCloseMessageIfApplicable(true);
        m_bSlipFlipInProgress = false;
        if (m_nEndorsementsPending)
        {
            trace (L0, _T("SMSmAssistMode::DMSlipPrinter() Printing endorsement data to a chit on the receipt station."));
            // Prohibit multiple chits per endorsement.
            m_nEndorsementsPending = 0;
            PrintEndorsementChit(m_szSlipDocumentBack);

            // Clear intervention, just as we do where slip print is successful above.
            if ( m_View == DATANEEDED_ENDORSEDOCUMENT )
            {
                ra.OnNoRAPDataNeeded();
            }

            TBEndorsementCompleted();
        }
        break;
    };
    trace (L6, _T("-SMSmAssistMode::DMSlipPrinter(0x%08.8x)"), dwEvent);
    return STATE_NULL;
}

SMStateBase * SMSmAssistMode::OnEndorseDocument(void)
{
    trace (L6, _T("+SMSmAssistMode::OnEndorseDocument()"));
    long nEndorsementsPending=0;

    COleVariant v;
    v = (long) UI::Disabled; // Disable
    //m_pAppPSX->SetControlProperty(_T("SMButton1"), UI::PROPERTYSTATE, v);
    m_pAppPSX->SetConfigProperty(_T("SMButton1"), csCurContext, UI::PROPERTYSTATE, v);

    ps.ShowFrame();
    
    if( GetHookProperty(_T("IsEBTTenderInsufficient")) == _T("1")) 
	{
       trace (L6, _T(" SMSmAssistMode::OnEndorseDocument() IsEBTTenderInsufficient=1"));
       do
	   {
        LPTSTR szSlipDocumentDescription = NULL;
        LPTSTR szSlipDocumentFront = NULL;
        LPTSTR szSlipDocumentBack = NULL;
	
	    TBGetDocEndorsementInfo(&nEndorsementsPending,
        &szSlipDocumentDescription,  &szSlipDocumentFront, &szSlipDocumentBack);

        if(nEndorsementsPending)
        {
            trace (L0, _T("SMSmAssistMode::OnEndorseDocument Printing endorsement data to a chit"));
            PrintEndorsementChit(szSlipDocumentBack);
            // Suppress further endorsement message from TB since we are doing these
            // in a loop and don't need to be event driven at this time.
            TBEndorsementCompleted(true);
        }
        delete szSlipDocumentDescription;
		delete szSlipDocumentFront;
    	delete szSlipDocumentBack;
	   } while (nEndorsementsPending);
      trace (L6, _T("-SMSmAssistMode::OnEndorseDocument() STATE_NULL"));
      return STATE_NULL;
	}
    
    //
    // TB_ENDORSEMENT messages can come back to back, when they do
    // we get slip data corruption. We need to handle them one at a time
    //
    if( fInEndorsementState == ENDORSE_INPROGRESS || m_bSlipFlipInProgress )
    {
        m_bSlipActivityIsWaiting = true;
        trace(L6, _T("The slip printer is busy processing a slip activity, this new one will have to wait."));
        return STATE_NULL;
    }
    else
    {
        m_bSlipActivityIsWaiting = false;
    }

    
    fInEndorsementState = ENDORSE_INPROGRESS;
    delete m_szSlipDocumentDescription;
    delete m_szSlipDocumentFront;
    delete m_szSlipDocumentBack;
    m_szSlipDocumentDescription = NULL;
    m_szSlipDocumentFront = NULL;
    m_szSlipDocumentBack = NULL;
    
    TBRC rc = ::TBGetDocEndorsementInfo(&m_nEndorsementsPending,
        &m_szSlipDocumentDescription,  &m_szSlipDocumentFront, &m_szSlipDocumentBack);
    
    if (!m_nEndorsementsPending)
    {
        fInEndorsementState = ENDORSE_NONE;
        DMSlipPrinterInsertSlip(false);
        m_bSlipPrintSideFront = false;
        m_bSlipAlreadyFlippedAfterApproval = false;
        trace (L6, _T("-SMSmAssistMode::OnEndorseDocument()"));
        return STATE_NULL;
    }
    
    if (m_szSlipDocumentDescription)
        trace (L6, _T(" SMSmAssistModeBase::OnEndorseDocument() : Description: <%s>"), m_szSlipDocumentDescription);
    if (m_szSlipDocumentFront)
        trace (L6, _T(" SMSmAssistModeBase::OnEndorseDocument() : Document front: <%s>"), m_szSlipDocumentFront);
    if (m_szSlipDocumentBack)
        trace (L6, _T(" SMSmAssistModeBase::OnEndorseDocument() : Document back: <%s>"), m_szSlipDocumentBack);
    
    if ((dm.IsDeviceRemote(DMCLASS_SLIP, 0) && dm.IsDeviceRemote(DMCLASS_RECEIPT, 0)) ||
        SMStateBase::nTenderType == TB_TENDER_CHECK_LANE ||    
        (dm.IsDeviceAvailable(DMCLASS_SLIP) && !ps.RemoteMode()))  //RFC 356020
    {
        TBShow2x20(m_szSlipDocumentDescription);
        bool bJustFlippedToBack = false;
        
        // DG30/DG32 check handling - ask TB whether we should flip or not (if we haven't already done so
        if( TB_SUCCESS_IMMEDIATE == SendTBCommand(TBCMD_SLIP_SHOULD_FLIP, m_bSlipPrintSideFront) )
        {
            dm.PrinterSlipFlip();
            m_bSlipFlipInProgress = true;
                
            // toggle the state of the current slip face
            m_bSlipPrintSideFront = !m_bSlipPrintSideFront;
                
            if( !m_bSlipPrintSideFront )
            {
                bJustFlippedToBack = true; 
            }
        }
          
        // DG30 check handling - as TB whether we should be ejecting the slip before print
        rc = SendTBCommand(TBCMD_SLIP_SHOULD_EJECT, bJustFlippedToBack);
        if( TB_SUCCESS_IMMEDIATE == rc )
        {
            trace(L6, _T("TB instructed to eject check before Print/Endorsement. Opening slip jaws..."));
            DMSlipPrinterInsertSlip(false);
            m_bSlipPrintSideFront = false;
            // The two back-to-back DMSlipPrinterInsertSlip calls sometimes fire DM_ERROR message DM_ERR_PTR_EMPTY
            // so a small Sleep has been added to space out the commands to the OPOS driver. This needs to be 
            // looked into by the platform team.
            Sleep(500);
        }

        // Open the throat and wait for a slip
        trace(L6, _T("Waiting for slip insertion..."));
        DMSlipPrinterInsertSlip(true);
      }
      else
      {
          m_bSlipActivityIsWaiting = false;
          trace (L0, _T(" SMSmAssistModeBase::OnEndorsedocument() - Application wants to print endorsement, but we are not is RemoteDeviceMode."));
          trace (L0, _T(" SMSmAssistModeBase::OnEndorseDocument() - Printing endorsement data to a chit on the receipt station."));
          // Prohibit multiple chits per endorsement.
          m_nEndorsementsPending = 0;
          PrintEndorsementChit(m_szSlipDocumentBack);
          TBEndorsementCompleted();
      }
      
      trace (L6, _T("-SMSmAssistModeBase::OnEndorseDocument()"));
      return STATE_NULL;
}

SMStateBase * SMSmAssistMode::OnNewCashDrawerState(void)
{
    long lDrawerState;
	TBGetExpectedCashDrawerState(&lDrawerState);
	
	if (dm.IsDeviceRemote(DMCLASS_CASHDRAWER, 0))
	{
		//sotf 6263
		//don't pop the cash drawer if we have a pending endorsement
		if ((lDrawerState & 1) && (fInEndorsementState == ENDORSE_NONE))
		{
			// Open Drawer here...
            CString csTitle = GetDisplayText(_T("Title1"));
            csTitle.MakeUpper();
            // NOTE:  Cash Drawer open on "MISCELLANEOUS MENU" is a NO SALE, transaction is not ending.
            if ( -1 == csTitle.Find(_T("MISCELLANEOUS MENU")) )
            {
				m_bTransactionEnding = true;
            }
			
			COleVariant v;
			v = (long) UI::Disabled;
			m_pAppPSX->SetConfigProperty(_T("SMButton1"), csCurContext, UI::PROPERTYSTATE, v);
			dm.CashDrawerOpen();
		}
		else
		{
			// Prompt for drawer close here...
		}
	}
	else if( !ps.RemoteMode() || (lDrawerState & 1))
	{
        trace(L0,_T("No Cash Drawer or not in remote assistmode"));
		//(+)POS190111.
		if(lDrawerState & 1)
		{
			m_bRequestingCashDrawerOpen = false; 
			ps.Echo(8, ps.GetPSText(9368));	     
		}
		//(-)POS190111.
		SendTBCommand(TBCMD_CASHDRAWER_CLOSE, NULL );
	}
	else
	{
		// There is no drawer available -- just tell TB what it wants to hear.
		TBSetCashDrawerState(lDrawerState);
	}
	
	return STATE_NULL;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Method:		SMStateBase  *SMSmAssistMode::PSKeystroke
//
// Purpose:		Precess keystroke for physical keyboard
//
// Parameters:	const long wParam, const long lParam
//
// Returns:		SMStateBase
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SMStateBase *SMSmAssistMode::PSKeystroke(const long wParam, const long lParam)
{
    trace (L6, _T("+SMSmAssistMode::PSKeystroke(0x%08.8x, 0x%08.8x)"), wParam, lParam);
    
    if(!lParam)  //No shift, control,or alt key pressed
    {
        for(int i = 1; i < 200; i++)
        {
            if(wParam == m_nAssistKeyVirtualKeyValue[i])   
            {
                //Find the key, send keycode to TB
                return PSAssistKey(i);
                
            }
        }
    } 

    SMStateBase* retState = SMSmAssistModeBase::PSKeystroke(wParam, lParam);
    return retState;
}

TBRC SMSmAssistMode::TBKeyPress(const USHORT uKey)
{
    return SMSmAssistModeBase::TBKeyPress(uKey);
}

SMStateBase * SMSmAssistMode::PSAlphaKey(const long lKey)
{
    return SMSmAssistModeBase::PSAlphaKey(lKey);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Method:		SMStateBase  *SMSmAssistMode::PSAlphaNumericKey
//
// Purpose:		Find keycode for numeric keys. Send these keycode to TB 
//
// Parameters:	const long lKey
//
// Returns:		SMStateBase
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SMStateBase *SMSmAssistMode::PSAlphaNumericKey(const long lKey)
{
	trace(L0,_T("+SMSmAssistMode::PSAlphaNumericKey %d"),lKey);
	return STATE_NULL;                
}

SMStateBase * SMSmAssistMode::OnAssistModeStatusChanged(void)
{
    return SMSmAssistModeBase::OnAssistModeStatusChanged();
}

SMStateBase * SMSmAssistMode::PSButtonToggle(void)
{
    return SMSmAssistModeBase::PSButtonToggle();
}

SMStateBase * SMSmAssistMode::PSButtonLaneStatus(long lLaneIndex)
{
    return SMSmAssistModeBase::PSButtonLaneStatus(lLaneIndex);
}

SMStateBase * SMSmAssistMode::OnRAPUnsolicitedDataReceived(LPCTSTR sData)
{
    trace (L6, _T("+SMSmAssistMode::OnRAPUnsolicitedDataReceived  fSAInItemSale(%d)."), fSAInItemSale);
    fSAInItemSale = false;
    trace (L6, _T("+SMSmAssistMode::OnRAPUnsolicitedDataReceived  reset fSAInItemSale to (%d)."), fSAInItemSale);
    
    const TCHAR szHeader[] = _T("cBiometData:~");
    const int iHeaderLen = _tcslen(szHeader);
    if( !_tcsncmp(sData, szHeader, iHeaderLen) )
    {
        // 1 Field: cBiometData:~[Encoded Data] 
        trace(L6, _T("SMStateBase::Received Biometric data, send it to POS."));
        SendTBCommand(TBCMD_BIOMET_DATA, &sData[iHeaderLen]);
    }

    //if we receive a toggle command for Meijer.  Ignore it because this makes no sense
    if((ExtractString(sData, _T("operation")) == _T("toggle")) && (co.nOperationsRemoteAssistModeToggleDisplay == 0))
    {
        trace(L6, _T("SMSmAssistMode::OnRAPUnsolicitedDataReceived() Received toggle command.  This is not allowed on Meijer.  Ignoring it"));
        return STATE_NULL;
        
    } 
// (+) POS181195/POS181197
    CString csData = sData;

    if (csData.Find(_T("cScanData")) != -1)
    {

      int iFind;

      trace(L6, _T("SMStateBase::OnRAPUnsolicitedDataReceived() Processing scan data..."));
      // 4 Fields: cRScan:Data:Label:Type
      // 4 Fields: cScanData:LastData|0xe1|Label|0xe1|Type
      CString csData1 = csData;
      CString csData2 = _T("");
      CString csData3 = _T("");
      CString csData4 = _T("");


      // Get First Field (cScanData)
      iFind=csData1.Find((_TCHAR)_T(':'));
      ASSERT (iFind != -1);

      csData2 = csData1.Mid(iFind+1);
      ASSERT (csData2.GetLength()>0);

	  // + Tar 360527
	   nDMLastLength = csData2.GetLength();
      //Save scan data
	   if (cDMLastData) delete[] cDMLastData;
      cDMLastData = new TCHAR [nDMLastLength+1];
      // End Merge

      // copy the new data to our buffer
      // the MsgObject graciously allocates 1 byte more and
      // tacks on a 0x00 so we can copy len + 1
      memcpy(cDMLastData, csData2, nDMLastLength * sizeof(_TCHAR));
      cDMLastData[nDMLastLength] = NULL;    // set last character as a NULL
	  // - Tar 360527

	  //Tar 206250 JSM  Merged from HD SSF

      if ((lBalanceDue == 0) && (nTenderType != TB_TENDER_NONE))
		  return STATE_NULL;
	  //Tar 206250 JSM

      // Add intercept for scan of special operators card
      // to immediately go to store mode
      // the function that checks if this bar code is a special operator
      // bar code should be in the UserExit module since this is customer
      // specific
      if (isBarcodeValidOperatorPassword(csData2))
      {
         trace(L6, _T("-SMStateBase::OnRAPUnsolicitedDataReceived() - Operator Barcode - returning STATE_NULL."));
	      return STATE_NULL;
      }

	

	  // Fix for TAR:208233
      // if not in tender mode and
	  // an item is still in progress
	  // and inside a trx:
      // do not allow further scans
      if ((fItemIsWaiting || fSAInItemSale) &&
		  !bEnterTenderState &&
		  (fInSignedOn || fInSignedOff)
		  )
      {
		 trace(L0, _T("Ignore SCAN details:fItemIsWaiting:%d, fSAInItemSale:%d, bEnterTenderState:%d, fInSignedOn:%d, fInSignedOff:%d"),
				fItemIsWaiting, fSAInItemSale, bEnterTenderState, fInSignedOn, fInSignedOff);
         trace(L6,_T("-SMStateBase::OnRAPUnsolicitedDataReceived - Item Waiting - returning STATE_NULL."));
         return STATE_NULL;
      }

      // BeginCodeLocaleUPC

      // Get Second Field (Data)
      iFind=csData2.Find((_TCHAR)DM_FS);
      ASSERT (iFind != -1);
      csDMLastBarCode = csData2.Left(iFind);

      // Get Third Field (Label)
      csData3 = csData2.Mid(iFind+1);
      ASSERT (csData3.GetLength()>0);
      iFind=csData3.Find((_TCHAR)DM_FS);
      ASSERT (iFind != -1);
      csDMLastBarLabel = csData3.Left(iFind);

      // Get Third Field
      csData4 = csData3.Mid(iFind+1);
      ASSERT (csData4.GetLength()>0);
      csDMLastBarType = _ttol(csData4);

      // EndCodeLocaleUPC
      //Need to know if Code 39 early
      io.cScanType = csDMLastBarType;
      trace(L6, _T("-SMStateBase::OnRAPUnsolicitedDataReceived() - DMScanner() called with scan data."));
      return DMScanner();         // barcode
    }
// (-) POS181195/POS181197
    return SMSmAssistModeBase::OnRAPUnsolicitedDataReceived(sData);
}

SMStateBase * SMSmAssistMode::PSRemoteButton1(void)
{
    return SMSmAssistModeBase::PSRemoteButton1();
}

SMStateBase * SMSmAssistMode::DMTakeawayAMExitDone()
{
    return SMSmAssistModeBase::DMTakeawayAMExitDone();
}

void SMSmAssistMode::TABExitingAssistMode(void)
{
    SMSmAssistModeBase::TABExitingAssistMode();
}

SMStateBase * SMSmAssistMode::HandleNewContext()
{
    return SMSmAssistModeBase::HandleNewContext();
}

SMStateBase * SMSmAssistMode::AutoRunTimeOut(void)
{
    return SMSmAssistModeBase::AutoRunTimeOut();
}

SMStateBase * SMSmAssistMode::TBEndOfTransaction(void)
{
    trace( L4, _T("+SMSmAssistMode::TBEndOfTransaction()") );
	
	// Start CRD 125377 
	if (m_bShopAndDropEnabled)
	{ 
		m_bSNDxCancelTrans = true;	// Allow Thank You exit without a wait.
	}
	// End CRD 125377
    // Ending trx in AM, assume all coupons have been collected.
    if (getAnchorState() == BES_INSERTCOUPON )
    {
		setAnchorState(BES_SCANANDBAG);
    }
	
	SMStateBase* pRet = SMSmAssistModeBase::TBEndOfTransaction();
	
    CString csChangeAtLane = GetTBProperty(_T("DispenseAnyChangeAtLane"));
	
	if( dm.IsDeviceRemote( DMCLASS_CASHDRAWER, 0 ) && ( csChangeAtLane != _T("1")) )
	{
        // Assumes that at the end of the transaction with a remote cash drawer
		// the cashier has given change.  TB controls the cash drawer pop.
        //sotf 6689
        if(lChangeDue)
        {
			rp.RAPDispenseChange(lChangeDue);
			lChangeDue = 0;
        }
		trace(L4,_T("SMSmAssistMode::TBEndOfTransaction:  Setting change due to 0"));
	}
    
	if ( ps.RemoteMode() && ( csChangeAtLane != _T("1")) )
	{
		if ( lChangeDue > 0 )
		{
			// NOTE:  SMSmAssistModeBase always returns STATE_NULL from TBEndOfTransaction
			//        if in Remote mode.
			delete getRealStatePointer( pRet); 
			pRet = IssueChange();
		}
	}
    SetTBProperty(_T("DispenseAnyChangeAtLane"));
	
	trace( L4, _T("-SMSmAssistMode::TBEndOfTransaction()") );
	
    if ( STATE_NULL != pRet )
    {
		return pRet;
    }
    else
    {
        // Speed up AssistMode Exit after end of transaction.
		delete getRealStatePointer( pRet); 
        return TimedOut();
    }
}

CString SMSmAssistMode::ExtractString(CString strData, CString sStringName)
{
    return SMSmAssistModeBase::ExtractString(strData, sStringName);
}

SMStateBase * SMSmAssistMode::PSConnectRemote( CString& strMachineName, CString& strRemoteContext )
{
    return SMSmAssistModeBase::PSConnectRemote( strMachineName, strRemoteContext );
}

void SMSmAssistMode::ResetFlagsInAM(void)
{
    SMSmAssistModeBase::ResetFlagsInAM();
}

long SMSmAssistMode::DoKeyCode(UCHAR uKey)
{
    return SMSmAssistModeBase::DoKeyCode( uKey);
}

SMStateBase * SMSmAssistMode::IssueChange(void)
{
    return SMSmAssistModeBase::IssueChange();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Method:		SMStateBase  *SMSmAssistMode::TBShowDisplay
//
// Purpose:		Retrieve latest POS screen info and display it
//
// Parameters:	N/A
//
// Returns:		N/A
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SMSmAssistMode::TBShowDisplay(bool bRefresh, bool bRepaintLastScreen)
{
   trace( L4, _T("+SMSmAssistMode::TBShowDisplay() bRefresh = %d bRepaintLastScreen = %d, g_bInRecallMode = %d, g_bIsProcessingEverseenAlert = %d"), 
	   bRefresh, 
	   bRepaintLastScreen, 
	   g_bInRecallMode, 
	   g_bIsProcessingEverseenAlert);
   
   COleVariant v;
   static COleVariant vFontAssistOperatorMessage (_T("")); // POS33020
   CString csCurPosState = GetHookProperty(_T("__POSState"));
 
   //Start CRD 289150 - if POS display is Item Menu and transfer has been completed, show Finish Recall screen
   if(csCurPosState.Find(_T("ITEMMENU_DK")) != -1 && bFinishRecall && !bShowSmRecallingItems)
   {
	   bRefresh = true;	//force to refresh
	   AMFinishRecall();
   }
   //End CRD 289150
   //Start CRD 347846
   CString csIsEverseenAlert = GetHookProperty(_T("IsEverseenAlert"));
   if((csCurPosState.Find(_T("ITEMMENU_DK")) != -1) && !g_bIsProcessingEverseenAlert && csIsEverseenAlert == _T("1") )
   {
	   trace(L6, _T("Everseen processing is done... go back to scan and bag"));
	   PSButton1();
	   return;
   }
   //End CRD 347846


	if (nTimeOut == TIMEOUT_DISABLED)
	{
		//TBLoadAssistModeKeyboard();	// KSo - This fixes the "Clear" key problem
		nTimeOut = g_lOrigTimeoutValue;
	}

    // TAR 346579: Disable the 'Return To Shopping' button if we're
    //             currently ending the transaction
    CString csTBState(_T(""));
    SendTBCommand(TBCMD_GET_TB_STATE, &csTBState);

	if ( fInVoidTrans || (-1 != csTBState.Find(_T("ASSISTMODE_TRXEND"))) )
	{
		m_bTransactionEnding = true;
	}

	bool bPrinterAvailable = CheckIfPrinterAvailable(false);

	if (SMState::bRAPPrinterBitmapStatus && bPrinterAvailable && dm.IsDeviceRemote(DMCLASS_RECEIPT, 0))
	{
		SMState::bRAPPrinterBitmapStatus = false; // reset the flag and be done regardless the call succeeds/fails. If failed, other lane remoting in future takes care of this
		if (SMSmAssistMode::m_csRAPConnectionName == "")
		{
			SMSmAssistMode::m_csRAPConnectionName = SMState::GetRAPConnectionName();
		}

		if (LoadReceiptLogo(SMState::csMeijerLogoFilePath))
		{
			//notify RAP that the bitmap loading was successful
			m_pAppPSX->GenerateEvent(_T(""), _T(""), _T("RemoteData"), _T("operation=bitmap-loaded;"), SMSmAssistMode::m_csRAPConnectionName);
			trace(L6, _T("Successfully loaded Meijer logo into the RAP printer"));
		}
		else
		{
			//notify RAP that bitmap loading has failed
			m_pAppPSX->GenerateEvent(_T(""), _T(""), _T("RemoteData"), _T("operation=bitmap-loadfailed"), SMSmAssistMode::m_csRAPConnectionName);
			trace(L6, _T("Failed to load Meijer logo into the RAP printer"));
			ps.Echo(8, _T("Meijer logo failed to load in the printer, Check the printer status and Reboot the RAP station."));
		}
	}
	else
	{
		if (SMState::bRAPPrinterBitmapStatus)
		{
			trace(L6, _T("Skip loading Meijer logo at RAP printer. The receipt printer is either unavailable or not remoted."));
			SMState::bRAPPrinterBitmapStatus = false;//reset the flag to avoid unnecessary future attempts in case the printer was unavailable
		}
	}

    v = (long) UI::Normal;
    m_pAppPSX->SetConfigProperty(_T("DeviceStatus"), csCurContext, UI::PROPERTYSTATE, v);


    m_POSDisplay->async = VARIANT_FALSE; // need to use sync mode if accessing DOM immediately after Load
    
	CString strDisplay = _T("");
	
   // TAR363054.  POS9528 backout TAR363054.
	//if ( GetTBProperty(_T("IsThereItemNeedToDisplayed")) == _T("1"))
	//{
	//	lItems = 0;
	//}

    //
    // If, after MICR the slip does not get ejected, ask TB if conditions
    // are met where we should be flipping the check.
    //        
    if( (dm.IsCheckPaymentAtLaneAvailable() || ps.RemoteMode()) && 
        !m_bWaitingForMICRInsertion &&
        !m_bSlipAlreadyFlippedAfterApproval &&
        DMSlipPrinterIsSlipInserted() )
    {
        // DG32 check handling mods - ask TB whether we should be flipping the check
        TBRC rc = SendTBCommand(TBCMD_SLIP_SHOULD_FLIP_AFTER_MICR);
        if( TB_SUCCESS_IMMEDIATE == rc )
        {
            dm.PrinterSlipFlip();
            m_bSlipFlipInProgress = true;

            // toggle the state of the current slip face
            m_bSlipAlreadyFlippedAfterApproval = true;
            m_bSlipPrintSideFront = true;
        }
    }


    //if the clear key was pressed then go back and show the last
    //screen we painted.  If a new screen came in, then this flag would be 
    //false
    if(bRepaintLastScreen == false)
    {   
	    SendTBCommand(TBCMD_GETDISPLAYXML, &strDisplay);
    
	
        //m_pAppPSX->SetControlProperty( _T("Display"), UI::PROPERTYREDRAW, VARIANT_FALSE );
	    
	    if(strDisplay == _T(""))
	    {
            //m_pAppPSX->SetControlProperty( _T("Display"), UI::PROPERTYREDRAW, VARIANT_TRUE );
		    return;
	    }

        //save the previous last display, if not on ERROR_CONTEXT_CLEAR
        // begin 399123 
		CString csCurPosState(_T(""));
        SendTBCommand(TBCMD_GET_POSSTATE, &csCurPosState); 
		// end 399123

        // pos 18819 - Reset the IsResumeStarted flag so that customer could continue scanning an item from the S&B screen
        if( csCurPosState == _T("RESUME_STARTED_NOIGNORE") )
        {
            if( GetHookProperty(_T("IsResumeStarted")) == _T("1") )
            {
                SetHookProperty(_T("IsResumeStarted"), _T("0")); 
                trace(L6, _T("RESUME_STARTED_NOIGNORE received, reset the IsResumeStarted flag"));
            }
        }
        // e pos 18819

		if(csCurPosState != _T("ERROR_CONTEXT_CLEAR"))
		{
			m_csPrevLastDisplay =  m_csLastDisplay;
			// if we have not saved off a PrevLastDisplay, then set it to the curr screen
			if(m_csPrevLastDisplay.IsEmpty())
			{
				m_csPrevLastDisplay = strDisplay;
			}
			trace (L6, _T("Setting current display to PrevLastDisplay variable."));
			
		}

        // DG30: If check handling is on and Check21 is off we may still be holding on to 
        // the slip since the MICR was not followed by an endorsement request. So if we're
        // back at payment check and eject
        if( csCurPosState == _T("PAYMENTMENU_DK1") )
        {
            if( (GetHookProperty(_T("SlipCheckHandling")) == _T("1")) &&
                (GetHookProperty(_T("SlipCheck21")) != _T("1")) &&
                (fInEndorsementState == ENDORSE_NONE) )
            {
                trace(L6, _T("Ejecting any slips being held by slip tray."));
                DMSlipPrinterInsertSlip(false);
                m_bSlipAlreadyFlippedAfterApproval = false;
            }
        } 
        

		CString csIsRefresh = GetHookProperty(_T("RefreshDisplay"));
		CString csIsVoiding = GetHookProperty(_T("IsVoiding"));
		//Start CRD 289150 - force display refresh if it's recalling items
		if(!bShowSmRecallingItems)
		{
			if((strDisplay != _T("")) && (m_csLastDisplay == strDisplay) && !bRefresh &&
				csIsRefresh == _T("0") && csIsVoiding == _T("0") && !g_bRefreshAfterSoftKeyDisable)
			{ 
				trace (L6, _T("Display is the same as previous, no forced refresh requested and not voiding -> not refreshing display."));
				return;
			}
			g_bRefreshAfterSoftKeyDisable = false;

			// Don't update the display in the middle of an endorsement
			if( ENDORSE_NONE != fInEndorsementState )
			{
				trace (L6, _T("We're processing an endorsement (fInEndorsementState=%d) -> not refreshing display."), fInEndorsementState);
				TBShow2x20(m_szSlipDocumentDescription);
				return;
			}
        
			// TAR 354798: Don't update the display if we're finalizing a suspend
			if( GetHookProperty(_T("SuspendingCurrentTransaction")) == _T("1") )
			{
				trace (L6, _T("We're finalizing a transaction suspend -> not refreshing display."));
				return;
			}
		}
		//End CRD 289150

		m_csLastDisplay = strDisplay;
		strDisplay = _T("");
    }
    else
    {
		trace(L4, _T("SMSmAssistMode::TBShowDisplay resetting to last POS screen"));
        m_csLastDisplay = m_csPrevLastDisplay;		
    }
    
    TraceDisplayMessage();
	
	//+Bug 95965
    m_csLastDisplay.Replace(_T("TRAVELER/REBATE/ETC"), _T("TRAVELER/REBATE/  ETC"));
	//-
	
	if( m_POSDisplay->loadXML( (_bstr_t) m_csLastDisplay ) != VARIANT_TRUE )
	{
		trace(L1, _T("ERROR: Load display xml failed"));
		return;
	}

    _variant_t nOperGuidNum = GetFieldValue(_T("OperGuid_Num"));
    _variant_t bIsOperGuid = GetFieldValue(_T("IsOperGuid"));

	if (bIsOperGuid.boolVal && nOperGuidNum.intVal == 1001)//PLACE CHECK IN DRAWER msg
	{
        trace (L6, _T("This is the 'Place Check In Drawer message (msgid = 1001)' -> not refreshing display."));
        ShowCashDrawerCloseMessageIfApplicable();
		return;
	}


    //POS82918 - Disabled NO SALE button even in the Remote AM when it's in the middle of trx
    if( m_bRequestingCashDrawerOpen && ps.RemoteMode() && bIsOperGuid.boolVal && nOperGuidNum.intVal == 92 )
    {
        trace(L6, _T("'Return To Shopping' button will be enabled after clearing the END TRANSACTION."));
        m_bRequestingCashDrawerOpen = false;
    }           

	//m_pAppPSX->SetControlProperty( _T("Display"), UI::PROPERTYREDRAW, VARIANT_FALSE );
    CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMSmAssistMode::TBShowDisplay"));

	_variant_t bAllowExit = GetFieldValue(_T("AllowAMExit"));
	
	
	v = (long) UI::Normal;

    if (bAllowExit.boolVal && (!m_bRequestingCashDrawerOpen))
	{ 
        m_pAppPSX->SetConfigProperty(_T("SMButton1"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
		m_pAppPSX->SetConfigProperty(_T("SMButton1"), csCurContext, UI::PROPERTYSTATE, v);
	}
    else if ( bAllowExit.boolVal  && ( !m_bTransactionEnding && !m_bRequestingCashDrawerOpen && !m_bTenderInProgress)) // SOTF6235 
	{
        m_pAppPSX->SetConfigProperty(_T("SMButton1"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
		m_pAppPSX->SetConfigProperty(_T("SMButton1"), csCurContext, UI::PROPERTYSTATE, v);
	}
	else
	{
		m_pAppPSX->SetConfigProperty(_T("SMButton1"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
	}

	//CRD 291874 - do not allow attendant to return to shopping unless Done button is pressed in the Transfer Complete screen
    //POS54351 - Remove the Return To shopping if it's in the middle for Mobile Intervention from RAP AM
    if( bShowFinishRecall || (ps.RemoteMode() && GetHookProperty(_T("PropMobShopOrderPrompt")) == _T("1") ))
    {        
        m_pAppPSX->SetConfigProperty(_T("SMButton1"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
    }
    //e POS54351


    //if we don't have any of the input data from the DF_Alphanum_display_lc
    //context then go ahead and adjust the input window.  if the data comming
    //from the POS is set SetEntryData will handle it
    //check if we are on one of the special acct screens . If so we need to handle
    //clearing of the input field and backspaces differently
    CString csText = GetDisplayText( _T("EntryData") );
    if (IsSpecialHandlingScreen())

    {       
       if ( csText.IsEmpty() )
       {
           AMClearAllInput();
       }
    }
    else
    {

        //not on a special handling screen adjust FL input window as needed
        CString csActive;
	    ps.GetInput( csActive );
        // NOTE:  AllowInput also clears the input field!!!!
        if(SendTBCommand(TBCMD_HIDEINPUT, NULL))
        {
            //hide the input
            ps.AllowInput( MAX_DIGITS_ITEMCODE, true, false );
            ps.SetInput( csActive, true );
        }
        else
        {
            //don't hide the input
            ps.AllowInput( MAX_DIGITS_ITEMCODE, false, false );
            ps.SetInput( csActive, false );
        }
    }

	v = (long) UI::Normal;
    m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollDown"), csCurContext, UI::PROPERTYSTATE, v);
    m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollUp"), csCurContext, UI::PROPERTYSTATE, v);

   // POS33020
   trace(L6, _T("Clear up everything to do with InsertCoupon; lCoupons=%d; m_lInsertedCouponsTotal=%d"), lCoupons, m_lInsertedCouponsTotal);
   m_bCouponInsertionBypassed = false;

   // POS33020: Just pass thru this one time to save the font for the typical operator message text.
   CString csFontAssistOperatorMessage(vFontAssistOperatorMessage.bstrVal);
   if (csFontAssistOperatorMessage == "")
   {
      PSXRC rc = m_pAppPSX->GetConfigProperty(_T("AssistOperatorMessage"), csCurContext, UI::PROPERTYFONT, vFontAssistOperatorMessage);
      if (PSX_SUCCESS == rc)
      {
         // Save the operator message font in case it gets changed by a POS33020 Coupon Choice screen.
         csFontAssistOperatorMessage = vFontAssistOperatorMessage.bstrVal;
         trace(L6, _T("Font for AssistOperatorMessage on %s is %s"), csCurContext, csFontAssistOperatorMessage);
      }
      else
      {
         // Failed to get operator message font so default font is FastLaneAMTitleFont which is "Lucida Sans Unicode,20,1,0,0".
         trace(L4, _T("Failed to get font for AssistOperatorMessage on %s, err=%x"), csCurContext, rc);
         csFontAssistOperatorMessage = _T("Lucida Sans Unicode,20,1,0,0");
         vFontAssistOperatorMessage = csFontAssistOperatorMessage;
         trace(L6, _T("Default font for AssistOperatorMessage on %s is %s"), csCurContext, csFontAssistOperatorMessage);
      }
   }

   long lDynaframeID;
   _variant_t vFieldValue = GetFieldValue(_T("DF_ID"));
   lDynaframeID = vFieldValue.intVal;

//Start CRD 289150
   bool bshowHighValuePromo = false;
   bool bshowVendorCoupon = false;

   //if ( (GetHookProperty(_T("__POSState")).Find(_T("HIGHVALUE_PROMO_CHOICE")) != -1) && (GetHookProperty(_T("HighValuePromoChoice")) == _T("1")) )
   if ((GetHookProperty(_T("__POSState")).Find(_T("HIGHVALUE_PROMO_CHOICE")) != -1))
   {
      // POS39475: High value mPerks promo reward must be displayed so the customer can choose to use it or not.
      CString csPromoDesc = GetHookProperty(_T("tb_char"));
      CString csPromoExpDate = GetHookProperty(_T("tb_ExpirationDate"));
      CString csPromoScreenText = ps.GetPSText(SSF_TXT_HIGHVALUE_PROMO);
      CString csMessage = csPromoScreenText + _T("\n") + csPromoDesc + _T("\n") + csPromoExpDate;
      trace(L6, _T("HighValuePromoChoice: Text:<%s>; Desc:<%s>; Date:<%s>"), csPromoScreenText, csPromoDesc, csPromoExpDate);
      ps.Echo(4, csMessage);

	  bshowHighValuePromo = true;	//CRD 289150

	  /* CRD 289150 - move to modify context at the end of this method
      // Make sure e-receipt is disabled and that the promo reward text displays.
      COleVariant v;
      v = (long) UI::Disabled;
      m_pAppPSX->SetConfigProperty(_T("AssistOperatorMessage"),  csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
      m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollDown"), csCurContext, UI::PROPERTYSTATE, v);
      m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollUp"), csCurContext, UI::PROPERTYSTATE, v);

      // Make sure font for operator message area is the typical setting.
      m_pAppPSX->SetConfigProperty(_T("AssistOperatorMessage"), csCurContext, UI::PROPERTYFONT, vFontAssistOperatorMessage);
    */
   }
   else if ( (GetHookProperty(_T("__POSState")).Find(_T("VENDOR_COUPON_CHOICE")) != -1) && (GetHookProperty(_T("CouponChoice")) == _T("1")) )
   {
      // POS33020: Coupon choice data must be displayed so the customer can choose between (mPerks/digital) E coupon and vendor (paper) coupon. 
      CString csECouponTitle = ps.GetPSText(SSF_TXT_COUPONCHOICE_MPERKS);;  
      CString csECouponDescAndCode = GetHookProperty(_T("tb_char"));  
      CString csECouponValueAndExpDate = GetHookProperty(_T("tb_ExpirationDate"));  
      CString csPaperCouponTitle = ps.GetPSText(SSF_TXT_COUPONCHOICE_VENDOR);;  
      CString csPaperCouponDescAndCode = GetHookProperty(_T("item_code"));  
      CString csPaperCouponValueAndExpDate = GetHookProperty(_T("tb_mnemonic"));  
      CString csMessage = csECouponTitle + _T("\n") + csECouponDescAndCode + _T("\n") + csECouponValueAndExpDate + _T("\n\n") +
                          csPaperCouponTitle + _T("\n") + csPaperCouponDescAndCode + _T("\n") + csPaperCouponValueAndExpDate;
      trace(L6, _T("ECouponChoice: Text:<%s>; Desc:<%s>; Date:<%s>"), csECouponTitle, csECouponDescAndCode, csECouponValueAndExpDate);
      trace(L6, _T("PaperCouponChoice: Text:<%s>; Desc:<%s>; Date:<%s>"), csPaperCouponTitle, csPaperCouponDescAndCode, csPaperCouponValueAndExpDate);
      ps.Echo(4, csMessage);

	  bshowVendorCoupon = true;
	  /* CRD 289150 - move to modify context at the end of this method
      // Make sure e-receipt is disabled and that the coupon choice text displays.
      COleVariant v;
      v = (long) UI::Disabled;
      m_pAppPSX->SetConfigProperty(_T("AssistOperatorMessage"),  csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
      m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollDown"), csCurContext, UI::PROPERTYSTATE, v);
      m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollUp"), csCurContext, UI::PROPERTYSTATE, v);

      // Make sure font for operator message area is smaller than the typical setting
      // due to the increase in information being displayed for coupon choice.
      PSXRC rc = m_pAppPSX->GetConfigFont(_T("FastLaneAMCouponChoiceFont"), v);
      if (PSX_SUCCESS == rc)
      {
         m_pAppPSX->SetConfigProperty(_T("AssistOperatorMessage"), csCurContext, UI::PROPERTYFONT, v);
      }
      else
      {
         trace(L4, _T("Failed to get FastLaneAMCouponChoiceFont font, err=%x"), rc);
      }
	  */
   }
   else
   {
	   //Start CRD 185620
	   //have to set this flag to true when Operator Guidance Number is for EAS
	   if(nOperGuidNum.intVal == OPERATORMSGNO_EASTAGS && lDynaframeID == DF_OPERATORMSG)
		   isEASOperatorGuidance = true;

      // Make sure font for operator message area is the typical setting.
      m_pAppPSX->SetConfigProperty(_T("AssistOperatorMessage"), csCurContext, UI::PROPERTYFONT, vFontAssistOperatorMessage);
      SetOperatorGuidance();
   }
   
   if ( (GetHookProperty(_T("__POSState")).Find(_T("AGE_VERIFY_DK1")) != -1) || (GetHookProperty(_T("__POSState")).Find(_T("AGE_VERIFY_SELECT_STATE")) != -1) )
   {
	   trace(L6, _T("Disabling the scanner since we don't allow to scan at this time."));
       DMScannerDisable();
   }
   else
	   DMScannerEnable();

    PrcMethodMsg();
	VoidItemMsg();
	SetEntryText();
	SetChangeDueDisplay();


	CString csTermInfo;
	csTermInfo.Format(ps.GetPSText(MSG_SCP_LANENUM), TBGetTerminalNumber());
	ps.Echo(3, csTermInfo);
	

	//tar302048 
	//add operator number to assistmode
    CString strOperatorNum = _T("OperatorNumber");
    CString strOperatorNumber;
    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strOperatorNum, &strOperatorNumber);
	csTermInfo.Format(_T("Operator # : %s"),strOperatorNumber);
	ps.Echo(5, csTermInfo);
	//tar302048 end

	ps.SetTitle(GetDisplayText(_T("Title1")), GetDisplayText(_T("Title2")));

	//Start CRD 185620
	//append Operator Guidance text to LTT so that e-receipt will be shown
	CString csOperGuidance = GetDisplayText(_T("OperGuid_Txt"));
	CString csNewTitle1 = GetDisplayText(_T("Title1"));

	//Start CRD 185620
	if(isEASOperatorGuidance)
	{		
		//append csOperGuidance to LTT
		CString csLTT;
		csLTT.Format(_T("%s %s"), csOperGuidance, GetDisplayText(_T("LTT1")));
		ps.SetLeadthruText(csLTT, GetDisplayText(_T("LTT2")));	
	}
	//End CRD 185620
	else	
		ps.SetLeadthruText(GetDisplayText(_T("LTT1")), GetDisplayText(_T("LTT2")));	

	SetDynakeys();
	//m_pAppPSX->SetControlProperty( _T("Display"), UI::PROPERTYREDRAW, VARIANT_TRUE );
	m_pAppPSX->SetTransactionVariable( _T("NextGenData"), COleVariant(_T("NextGenUIDynaKeysUpdateComplete")) );
	m_pAppPSX->SetTransactionVariable( _T("NextGenData"), COleVariant(_T("")) ); //Reset to blank to prevent multiple updates to NGUI
    
	
	if(m_csLastTitle1 != csNewTitle1)
	{   
		m_csLastTitle1 = csNewTitle1;
	}

    if ( m_bWaitingForMICRInsertion && m_csLastTitle1.Find(_T("Routing Number")) == -1 )
    {
        // turn off MICR flag if not on MICR screen.
		bool bEject = (TB_SUCCESS_IMMEDIATE == SendTBCommand(TBCMD_MICR_SHOULD_EJECT));
        dm.MICRDisable(bEject);
        m_bWaitingForMICRInsertion = FALSE;
    }
    //TAR348914+
	if ( m_csLastTitle1.Find(_T("Sign-in Menu")) != -1 )
    {
        setAnchorState(BES_LANECLOSED);
    }
	//TAR348914-

    //TAR370989+
    if ( m_csLastTitle1.Find(_T("Supervisor Override")) != -1 
         || m_csLastTitle1.Find(_T("Supervisor Password")) != -1 )
    {
        CString csOperGuidTxt = GetDisplayText(_T("OperGuid_Txt"));
 
        if( csOperGuidTxt.Find(_T("PRICE DIFFERENCE OVER LIMIT")) != -1 )
        {
         //disable Return to Shopping Mode button
		 v = (long) UI::Disabled;
         m_pAppPSX->SetConfigProperty(_T("SMButton1"), csCurContext, UI::PROPERTYSTATE, v);
        }
    }
    //TAR370989- 


	//Start CRD 289150 - set strings for title and LTT for recall items and finish recall
	if(bShowSmRecallingItems)
	{
		ps.SetTitle(ps.GetPSText(MSG_LOADING_TRANSACTION), _T(""));
		ps.SetLeadthruText(ps.GetPSText(TTL_LOADING_TRANSACTION), _T(""));
	}
	else if(bShowFinishRecall)
	{
		ps.SetTitle(ps.GetPSText(TTL_RECALL_COMPLETE), _T(""));
		ps.SetLeadthruText(ps.GetPSText(MSG_RECALL_PLEASE_SCAN_MORE_ITEMS), _T(""));
	}
	//End CRD 289150

	
    CString csVoidPrepaidCardsProp = _T("VoidPrepaidCards");
    CString csIsVoidPrepaidCards;
    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csVoidPrepaidCardsProp, &csIsVoidPrepaidCards);

    //pos 12127 rrs
    CString csTaxListReceiptProp = _T("TaxListReceipt");
    CString csTaxListReceipt;
    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csTaxListReceiptProp, &csTaxListReceipt);
    //e pos 12127 rrs

    //POS57686 rrs
    CString csRainCheckListProp = _T("RainCheckList");
    CString csRainCheckList;
    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csRainCheckListProp, &csRainCheckList);
    //e POS57686 rrs

// (+) POS154826
    CString csWICItemListProp = _T("WICItemList");
    CString csWICItemList;
    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csWICItemListProp, &csWICItemList);

        
// (-) POS154826

	PSXRC rc;

    trace(L6, _T(" VoidPrepaidCard(%s), TaxList(%s), RainCheckList(%s), isEASOperatorGuidance(%d)"), csIsVoidPrepaidCards, csTaxListReceipt, csRainCheckList, isEASOperatorGuidance);
    // If voiding prepaid cards 
    if (csIsVoidPrepaidCards == _T("1")) 
    {

        /*POS11024 Mobile Shopper: Clear Receipt before populating it*/
        rc = m_pAppPSX->ClearReceipt( _T("PrepaidCardList") );

        // Make ACSList visible here
        m_pAppPSX->SetConfigProperty( _T("PrepaidCardList"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE );
        m_pAppPSX->SetConfigProperty( _T("AssistReceipt"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );

        //POS57686 - rrs
        // Hide the TaxList receipt
        m_pAppPSX->SetConfigProperty( _T("TaxList"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );
        // Reset the taxlist receipt
        rc = m_pAppPSX->ClearReceipt( _T("TaxList") );
        //e POS57686 - rrs

        CString csPrepaidCardProp = _T("PrepaidCardList");
        CString csPrepaidCard;
        SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csPrepaidCardProp, &csPrepaidCard);

		rc = m_pAppPSX->CreateReceiptItem( _T("PrepaidCardList"), _T("item1") );
        if ( PSX_SUCCESS != rc )
        {
            // kaboom!
        }
		COleVariant vValue;
		vValue = csPrepaidCard;

        rc = m_pAppPSX->SetReceiptItemVariable( _T("PrepaidCardList"), _T("item1"), _T("ITEM_DESCRIPTION"), vValue );
        // create more
        rc = m_pAppPSX->UpdateReceiptControls( _T("PrepaidCardList") );
        
        //POS31050 - Don't clear the list -- rrs
        /*POS11024 Mobile Shopper: Clear List after we have used it*/
        //SetHookProperty(_T("PrepaidCardList"), _T(""));
    }
    // pos12127 rrs
    else if(csTaxListReceipt == _T("1"))
    {
         // Make ACSList visible here
        m_pAppPSX->SetConfigProperty( _T("TaxList"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE );
        m_pAppPSX->SetConfigProperty( _T("AssistReceipt"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );

        // disabled the scrolling up and down
        m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollUp"), csCurContext, UI::PROPERTYSTATE, (long)UI::Disabled);
        m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollDown"), csCurContext, UI::PROPERTYSTATE, (long)UI::Disabled);

        CString csTaxListItemProp = _T("TaxListItem");
        CString csTaxListItem;
        SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csTaxListItemProp, &csTaxListItem);

        if( csTaxListItem.GetLength() > 0 )
        {
		    rc = m_pAppPSX->ClearReceipt( _T("TaxList") );
            if ( PSX_SUCCESS != rc )
            {
                // kaboom!
            }
            else
            {
		        COleVariant vValue;
                bool bIsTaxListItemExist = true;
                CString csTaxListItemDesc, csReceiptID;
                int iToken=0;
                int iReceiptID=0;
                
                // parse the tax list item from the cstring buffer
                // the cstring would have the format of "Tax1 Off   |Tax2 Off    |Tax3 Off"
                while( bIsTaxListItemExist )
                {
                    iToken = csTaxListItem.Find(_T("|"));
                    if( iToken > -1 )
                    {
                        // create the tax list description
                        csTaxListItemDesc = csTaxListItem.Left(iToken);
                        csTaxListItem = csTaxListItem.Mid(iToken+1);

                        // create the receipt item entry id
                        csReceiptID.Format(_T("item%d"), iReceiptID);
                        vValue = csTaxListItemDesc;
                        iReceiptID++;

                        rc = m_pAppPSX->CreateReceiptItem( _T("TaxList"), csReceiptID );
                        rc = m_pAppPSX->SetReceiptItemVariable( _T("TaxList"), csReceiptID, _T("ITEM_DESCRIPTION"), vValue );
                    }
                    else
                    {
                        csReceiptID.Format(_T("item%d"), iReceiptID);
                        vValue = csTaxListItem;
                        
                        rc = m_pAppPSX->CreateReceiptItem( _T("TaxList"), csReceiptID );
                        rc = m_pAppPSX->SetReceiptItemVariable( _T("TaxList"), csReceiptID, _T("ITEM_DESCRIPTION"), vValue );
                        bIsTaxListItemExist = false;
                    }
                }
                // update the receipt control
                rc = m_pAppPSX->UpdateReceiptControls( _T("TaxList") );

                SetHookProperty(_T("TaxListItem"), _T(""));                
            }
        } 
        
        // highlight the selected TAX item from POS
        CString csSelectedTaxListValue = GetDisplayText(_T("TaxList_Selected"));
        if( csSelectedTaxListValue != _T("") )
        {
            CString csSelectedTaxList;
            csSelectedTaxList.Format(_T("item%s"),csSelectedTaxListValue);
            m_pAppPSX->SetSelectedReceiptItem(_T("TaxList"), csSelectedTaxList);            
        }            

    }
    // e pos12127 rrs
    //POS57686 -rs
    else if(csRainCheckList == _T("1"))
    {        
        // POS57465: Note that this raincheck section is also being used by the mPerks Lookup feature.

        // Make ACSList visible here
        m_pAppPSX->SetConfigProperty( _T("TaxList"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE );
        m_pAppPSX->SetConfigProperty( _T("AssistReceipt"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );

        CString csPrepaidCardProp = _T("PrepaidCardList");
        CString csPrepaidCard;
        SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csPrepaidCardProp, &csPrepaidCard);

        // disabled the scrolling up and down
        m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollUp"), csCurContext, UI::PROPERTYSTATE, (long)UI::Disabled);
        m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollDown"), csCurContext, UI::PROPERTYSTATE, (long)UI::Disabled);

        trace(L6, _T("RainCheckList is active"));
		if( csPrepaidCard.GetLength() > 0 )
        {
            trace(L6, _T("RainCheckList content (%s)"), csPrepaidCard);
		    rc = m_pAppPSX->ClearReceipt( _T("TaxList") );
            if ( PSX_SUCCESS != rc )
            {
                // kaboom!
            }
            else
            {
		        COleVariant vValue;
                bool bIsRainCheckItemExist = true;
                CString csRainCheckItemDesc, csReceiptID;
                int iToken=0;
                int iReceiptID=0;
                
                // parse the tax list item from the cstring buffer
                // the cstring would have the format of "BC PREMIUM BROWNIE \tTRIPLE CHUN	\tCOTTONELLE BATH TISSUE GENTLE	\tDELMONTE GREEN BEANS FRENCH NS"
                while( bIsRainCheckItemExist )
                {
                    iToken = csPrepaidCard.Find(_T("\t"));
                    if( iToken > -1 )
                    {
                        // create the tax list description
                        csRainCheckItemDesc = csPrepaidCard.Left(iToken);
                        csPrepaidCard = csPrepaidCard.Mid(iToken+1);

                        // create the receipt item entry id
                        csReceiptID.Format(_T("item%d"), iReceiptID);
                        vValue = csRainCheckItemDesc;
                        iReceiptID++;

                        rc = m_pAppPSX->CreateReceiptItem( _T("TaxList"), csReceiptID );
                        rc = m_pAppPSX->SetReceiptItemVariable( _T("TaxList"), csReceiptID, _T("ITEM_DESCRIPTION"), vValue );
                        trace(L6, _T("RainCheck Item Id=%s, Desc=%s"), csReceiptID, csRainCheckItemDesc);
                    }
                    else
                    {
                        csReceiptID.Format(_T("item%d"), iReceiptID);
                        vValue = csPrepaidCard;
                        
                        rc = m_pAppPSX->CreateReceiptItem( _T("TaxList"), csReceiptID );
                        rc = m_pAppPSX->SetReceiptItemVariable( _T("TaxList"), csReceiptID, _T("ITEM_DESCRIPTION"), vValue );
                        bIsRainCheckItemExist = false;
                        trace(L6, _T("RainCheck Item Id=%s, Desc=%s"), csReceiptID, csPrepaidCard); // POS57465
                    }
                }
                // update the receipt control
                rc = m_pAppPSX->UpdateReceiptControls( _T("TaxList") );

                SetHookProperty(_T("PrepaidCardList"), _T(""));                
            }
        } 
        
        // highlight the selected TAX item from POS
        CString csSelectedTaxListValue = GetDisplayText(_T("Raincheck_Selected"));
        trace(L6, _T("RainCheck SelectedItem Id=%s"), csSelectedTaxListValue);
        if( csSelectedTaxListValue != _T("") )
        {
            CString csSelectedTaxList;
            csSelectedTaxList.Format(_T("item%s"),csSelectedTaxListValue);
            m_pAppPSX->SetSelectedReceiptItem(_T("TaxList"), csSelectedTaxList);            
        } 
    }
    // e POS57686 

// (+) POS154826   
    else if(csCurPosState == _T("WIC_CUSTOMER_CONFIRMATION1") || csCurPosState == _T("WIC_CUSTOMER_CONFIRMATION2") || csCurPosState == _T("WIC_CONFIRM_RECEIPT_DONE"))
    {        
        // POS57465: Note that this raincheck section is also being used by the mPerks Lookup feature.

        // Make ACSList visible here
        
        m_pAppPSX->SetConfigProperty( _T("WICList"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE );
        m_pAppPSX->SetConfigProperty( _T("TaxList"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );
        m_pAppPSX->SetConfigProperty( _T("AssistReceipt"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );

        CString csPrepaidCardProp = _T("WICList");
        CString csPrepaidCard;
        SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csPrepaidCardProp, &csPrepaidCard);

        if(csPrepaidCard != _T(""))
        {
            m_csWICItemList = csPrepaidCard;            
        }

        if(csPrepaidCard == _T(""))
        {
            csPrepaidCard=m_csWICItemList;
        }



        // disabled the scrolling up and down
//        m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollUp"), csCurContext, UI::PROPERTYSTATE, (long)UI::Disabled);
  //      m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollDown"), csCurContext, UI::PROPERTYSTATE, (long)UI::Disabled);

        trace(L6, _T("WICItemList is active"));
		if( csPrepaidCard.GetLength() > 0 )
        {
            trace(L6, _T("WICItemList content (%s)"), csPrepaidCard);
		    rc = m_pAppPSX->ClearReceipt( _T("WICList") );
            if ( PSX_SUCCESS != rc )
            {
                // kaboom!
            }
            else
            {
		        COleVariant vValue;
                bool bIsRainCheckItemExist = true;
                CString csRainCheckItemDesc, csReceiptID;
                int iToken=0;
                int iReceiptID=0;
                
                // parse the tax list item from the cstring buffer
                // the cstring would have the format of "BC PREMIUM BROWNIE \tTRIPLE CHUN	\tCOTTONELLE BATH TISSUE GENTLE	\tDELMONTE GREEN BEANS FRENCH NS"
                while( bIsRainCheckItemExist )
                {
                    iToken = csPrepaidCard.Find(_T("\t"));
                    if( iToken > -1 )
                    {
                        // create the tax list description
                        csRainCheckItemDesc = csPrepaidCard.Left(iToken);
                        csPrepaidCard = csPrepaidCard.Mid(iToken+1);

                        // create the receipt item entry id
                        csReceiptID.Format(_T("%d"), iReceiptID);
                        vValue = csRainCheckItemDesc;
                        iReceiptID++;

                        rc = m_pAppPSX->CreateReceiptItem( _T("WICList"), csReceiptID );
                        rc = m_pAppPSX->SetReceiptItemVariable( _T("WICList"), csReceiptID, _T("ITEM_DESCRIPTION"), vValue );
                        trace(L6, _T("WIC Item Id=%s, Desc=%s"), csReceiptID, csRainCheckItemDesc);
                    }
                    else
                    {
                        csReceiptID.Format(_T("%d"), iReceiptID);
                        vValue = csPrepaidCard;
                        
                        rc = m_pAppPSX->CreateReceiptItem( _T("WICList"), csReceiptID );
                        rc = m_pAppPSX->SetReceiptItemVariable( _T("WICList"), csReceiptID, _T("ITEM_DESCRIPTION"), vValue );
                        bIsRainCheckItemExist = false;
                        trace(L6, _T("WIC Item Id=%s, Desc=%s"), csReceiptID, csPrepaidCard); // POS57465
                    }
                }
                // update the receipt control
                rc = m_pAppPSX->UpdateReceiptControls( _T("WICList") );

                SetHookProperty(_T("WICList"), _T(""));                
            }
        } 
        
        // highlight the selected TAX item from POS
        CString csSelectedTaxListValue = GetDisplayText(_T("Raincheck_Selected"));
        csSelectedTaxListValue=_T("0");
        trace(L6, _T("WIC SelectedItem Id=%s"), csSelectedTaxListValue);
        if( csSelectedTaxListValue != _T("") )
        {
            CString csSelectedTaxList;
            csSelectedTaxList.Format(_T("%s"),csSelectedTaxListValue);
            m_pAppPSX->SetSelectedReceiptItem(_T("WICList"), csSelectedTaxList);            
        } 
    }
// (-) POS154826   
	//Start CRD 212453
	else if(isEASOperatorGuidance)
	{
		isEASOperatorGuidance = false;
		m_pAppPSX->SetConfigProperty(_T("AssistReceiptEAS"),  csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
		m_pAppPSX->SetConfigProperty(_T("AssistReceipt"),  csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
	} 
	//End CRD 212453
	//Start CRD 289150 - moved to show promo reward text here
	else if (bshowHighValuePromo)
	{
		// Make sure e-receipt is disabled and that the promo reward text displays.
		COleVariant v;
		v = (long) UI::Disabled;
		m_pAppPSX->SetConfigProperty(_T("AssistOperatorMessage"),  csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
		m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollDown"), csCurContext, UI::PROPERTYSTATE, v);
		m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollUp"), csCurContext, UI::PROPERTYSTATE, v);
		
		// Make sure font for operator message area is the typical setting.
        m_pAppPSX->SetConfigProperty(_T("AssistOperatorMessage"), csCurContext, UI::PROPERTYFONT, vFontAssistOperatorMessage);
	}
	else if (bshowVendorCoupon)
	{
		// Make sure e-receipt is disabled and that the coupon choice text displays.
		COleVariant v;
		v = (long) UI::Disabled;
		m_pAppPSX->SetConfigProperty(_T("AssistOperatorMessage"),  csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
		m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollDown"), csCurContext, UI::PROPERTYSTATE, v);
		m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollUp"), csCurContext, UI::PROPERTYSTATE, v);
		
		// Make sure font for operator message area is smaller than the typical setting
		// due to the increase in information being displayed for coupon choice.
		PSXRC rc = m_pAppPSX->GetConfigFont(_T("FastLaneAMCouponChoiceFont"), v);
		if (PSX_SUCCESS == rc)
		{
			m_pAppPSX->SetConfigProperty(_T("AssistOperatorMessage"), csCurContext, UI::PROPERTYFONT, v);
		}
		else
		{
			trace(L4, _T("Failed to get FastLaneAMCouponChoiceFont font, err=%x"), rc);
		}
	}
	//End CRD 289150
    else
    {
        // Make PrepaidList not visible here
        m_pAppPSX->SetConfigProperty( _T("PrepaidCardList"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );
        m_pAppPSX->SetConfigProperty( _T("AssistReceipt"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE );
        m_pAppPSX->SetConfigProperty( _T("WICList"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );

		m_pAppPSX->SetConfigProperty(_T("AssistReceiptEAS"),  csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE); //CRD 212453

        // pos 12127 rrs
        // Hide the TaxList receipt
        m_pAppPSX->SetConfigProperty( _T("TaxList"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );

        // re-enable the scrolling up and down
        m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollUp"), csCurContext, UI::PROPERTYSTATE, (long)UI::Normal);
        m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollDown"), csCurContext, UI::PROPERTYSTATE, (long)UI::Normal);
        
		SetHookProperty(_T("PrepaidCardList"), _T(""));
		rc = m_pAppPSX->ClearReceipt( _T("PrepaidCardList") );

        // pos 12127 rrs
        // Reset the taxlist receipt
        rc = m_pAppPSX->ClearReceipt( _T("TaxList") );
		rc = m_pAppPSX->ClearReceipt( _T("WICList") );
	}

    
    

    ps.ShowFrame();

	trace( L4, _T("-SMSmAssistMode::TBShowDisplay()"));
}

void SMSmAssistMode::SetEntryText( )
{
	CString csText = GetDisplayText( _T("EntryData") );
	if ( !csText.IsEmpty() )
	{
		if(SendTBCommand(TBCMD_HIDEINPUT, NULL))
		{
			ps.SetInput( csText, true );
		}
		else
		{
			ps.SetInput( csText, false );
		}
	}
}

CString SMSmAssistMode::GetDisplayText(CString csFieldName)
{
	_variant_t vFieldValue = GetFieldValue(csFieldName);
	if (vFieldValue.vt == VT_NULL)
		return _T("");
	else
		return vFieldValue.bstrVal;
}
_variant_t  SMSmAssistMode::GetFieldValue(CString csFieldName)
{
	CString csPath;
	csPath.Format(_T("message/fields/field[@name = \"%s\"]"), csFieldName); 
	_bstr_t test = _bstr_t(csPath);
	
	MSXML2::IXMLDOMNodePtr testnode = m_POSDisplay->selectSingleNode(_bstr_t(csPath));
	
	_variant_t nodeValue = _T("");
    nodeValue.vt = VT_NULL;
	if (testnode != NULL)
	{
		_variant_t nodeValue=testnode->GetnodeTypedValue();
		return nodeValue;
	}
	else
	{
		return nodeValue;
	}
}

void SMSmAssistMode::SetDynakeys()
{    
    CString csLabel, csEnabled, csText, csGrayed, csIcon;
	COleVariant vReturn, v, v1, v2;
	long nButtonCount = 8; 
	long nRelativeIndex = 0; // SSCOI-45763

	COleSafeArray saTextArray, saIconArray, saStateArray;
	COleSafeArray saDataArray;

	saIconArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
	saTextArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
	saStateArray.Create(VT_I4, 1, (DWORD*)&nButtonCount);
	
	v = saIconArray;
	
	
	//save button data list before blasting buttons
    m_pAppPSX->SendCommand( _T("Key24"), UI::COMMANDGETBUTTONDATALIST, vReturn);

	if (vReturn.vt == (VT_ARRAY | VT_VARIANT))
	{
		saDataArray.Attach(vReturn);
	}


    //m_pAppPSX->SetControlProperty( _T("Display"), UI::PROPERTYREDRAW, VARIANT_FALSE );
    CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMSmAssistMode::SetDynakeys"));

	// reset the count to 0 to clear all previously set attributes
    v = (long) 0;
    m_pAppPSX->SetConfigProperty( _T("Key24"), csCurContext, UI::PROPERTYBUTTONCOUNT, v );
	m_pAppPSX->SetControlProperty(_T("Key24"), UI::PROPERTYBUTTONCOUNT, v );

    v = nButtonCount;
    //m_pAppPSX->SetControlProperty( _T("Key24"), UI::PROPERTYREDRAW, VARIANT_FALSE );

    m_pAppPSX->SetConfigProperty( _T("Key24"), csCurContext, UI::PROPERTYBUTTONCOUNT, v );
    m_pAppPSX->SetControlProperty( _T("Key24"), UI::PROPERTYBUTTONCOUNT, v );

    bool bCheckAtLane = dm.IsCheckPaymentAtLaneAvailable();
    SendTBCommand(TBCMD_SET_CHECK_AT_LANE_AVAILABLE, bCheckAtLane);


	long nKeyOffset;
	for (int nKey=1; nKey <= nButtonCount; nKey++)
	{
		nKeyOffset=nKey-1;  
		
		csLabel.Format(_T("DK%d_Label"),nKey);
		csEnabled.Format(_T("DK%d_Enabled"), nKey);
		csGrayed.Format(_T("DK%d_Grayed"), nKey);
		csIcon.Format(_T("DK%d_Icon"), nKey);
		
		csText = GetDisplayText(csLabel);  //Dynakey1
		if (csText == _T(""))
		{
			csLabel.Format(_T("DK%d_Label_Alt"), nKey);
			csText = GetDisplayText(csLabel);
			if (csText == _T("")) 
				csText = _T(" ");
		}

        // pos15302 - displaying MEP description on the AM button
        if( nKey == 7 ) 
        {
            CString csPosState = GetHookProperty(_T("__POSState"));
            if( csPosState.Find(_T("MEP_AVAILABLE")) != -1 )
            {
            CString cs2ndLine = GetHookProperty(_T("Prop_Last_DK_normal_desc_7"));
            csText = csText + _T("\n") + cs2ndLine;            
            }
        }


// (+) POS111636
        if(csText == _T("MEIJER CREDIT CARD"))
        {
            CString cs2ndLine;
            CString csPosState = GetHookProperty(_T("__POSState"));
            CString csPropDesc;
            csPropDesc.Format(_T("Prop_Last_DK_normal_desc_%d"), nKey);

                    trace( L6, _T("SetDynaKey()  - POS State=%s"), csPosState);
            if( csPosState.Find(_T("CMTOTAL_UPDATE")) != -1 || csPosState.Find(_T("VOIDMENU_DK1")) != -1 || csPosState.Find(_T("PAYMENTMENU_DK1_NOCASH")) != -1 || csPosState.Find(_T("PAYMENTMENU_DK1")) != -1)
            {

                cs2ndLine = GetHookProperty(csPropDesc);

                csText = csText + _T("\n") + cs2ndLine;            
                
            }
        }
// (-) POS111636
        // e - pos15302

		//Start CRD 289150 - i don't want to display any buttons while recalling items
		if(bShowSmRecallingItems)
			csText = _T(" ");

		//shows OK button in Finish Recall screen
		else if(bShowFinishRecall)
		{
			if(nKey == 1)
				csText = _T("OK");
			else
				csText = _T(" ");
		}
		
		//End CRD 289150

        BSTR bstrTemp;

#ifndef UNICODE
        bstrTemp = SysAllocString( T2OLE( csText ) );
#else 
		bstrTemp = SysAllocString( csText );
#endif
		saTextArray.PutElement(&nKeyOffset, bstrTemp );
        SysFreeString( bstrTemp );
		
		_variant_t vEnabled = GetFieldValue(csEnabled);
		_variant_t vGrayed = GetFieldValue(csGrayed);
		csIcon = GetDisplayText(csIcon);
		csIcon.Replace(_T("|BITMAP|:"),_T("C:\\scot\\Image\\ACSIcons\\"));
		csIcon.Replace(_T(".bmp"),_T(".ico"));
		

#ifndef UNICODE
		bstrTemp = SysAllocString( T2OLE( csIcon ) );
#else
        bstrTemp = SysAllocString( csIcon );
#endif
		saIconArray.PutElement(&nKeyOffset, bstrTemp );
        SysFreeString( bstrTemp );

		long lButtonState;
		if (vEnabled.vt != VT_NULL && vEnabled.boolVal)
			lButtonState = (long) UI::Normal; 
		else if ((vGrayed.vt != VT_NULL) && (vGrayed.intVal == 0 || vGrayed.intVal == 2))
		{
			if (csIcon.Find(_T("checkon")) != -1)
				lButtonState = (long) UI::Highlighted;
			else
				lButtonState = (long) UI::Normal;
		}
		else 
			lButtonState = (long) UI::Disabled;        
        

        //check and see if it is one of the buttons that TB always wants
        //disabled for assistmode
        if(SendTBCommand(TBCMD_ASSISTMODE_BUTTON_ALLOWED, csText) == 1)
        {
            // Keep enabled if it's the Check button and we are permitted to pay by
            // by check at the lane
            if( (csText == _T("CHECKS")) && bCheckAtLane )
            {
                trace(L6, _T("Check payment at lane is available."));
            }
            //POS107531 - only disable mPerks if it's in ITEM MENU
            else if( (csText == _T("mPerks")) )
            {
                CString csPosState = GetHookProperty(_T("__POSState"));
                trace(L6, _T("POSState(%s)"), csPosState);
                if( csPosState.Find(_T("ITEMMENU_DK1")) != -1 )
                {
                    lButtonState = (long) UI::Disabled;                
                }
            }
            else
            {
                lButtonState = (long) UI::Disabled;                
            }            
        }

		//Start CRD 289150 - make sure button is disabled while recalling items
		if(bShowSmRecallingItems)
			lButtonState = (long) UI::Disabled;  

		//enable OK button for finish recall
		else if(bShowFinishRecall)
		{
			if(nKey == 1)
				lButtonState = (long) UI::Normal;
			else
				lButtonState = (long) UI::Disabled;  
		}
			
		//End CRD 289150

		saStateArray.PutElement(&nKeyOffset, &lButtonState );
		
		// +SSCOI-45763
		v1 = (long) nRelativeIndex;
		v2 = lButtonState;
		m_pAppPSX->SendCommand( _T("Key24"), UI::COMMANDSETBUTTONSTATE, vReturn, 2, v1, v2 );
		nRelativeIndex++;	
		// -SSCOI-45763

	}

	v = saTextArray;
    m_pAppPSX->SendCommand( _T("Key24"), UI::COMMANDSETBUTTONTEXTLIST, vReturn, 1, v );	
	v = saIconArray;
    m_pAppPSX->SendCommand( _T("Key24"), UI::COMMANDSETBUTTONLOGOLIST, vReturn, 1, v );	
	v = saStateArray;
    m_pAppPSX->SendCommand( _T("Key24"), UI::COMMANDSETBUTTONSTATELIST, vReturn, 1, v );
	v = saDataArray;
    m_pAppPSX->SendCommand( _T("Key24"), UI::COMMANDSETBUTTONDATALIST, vReturn, 1, v );	

    //m_pAppPSX->SetControlProperty( _T("Display"), UI::PROPERTYREDRAW, VARIANT_TRUE );    
}

void SMSmAssistMode::SetOperatorGuidance()
{
	trace(L6, _T("SMSmAssistMode::SetOperatorGuidance()"));
	long nEndorsementsPending=0;

	LPTSTR szSlipDocumentDescription;
    LPTSTR szSlipDocumentFront;
    LPTSTR szSlipDocumentBack;
    
    szSlipDocumentDescription = NULL;
    szSlipDocumentFront = NULL;
    szSlipDocumentBack = NULL;
	
	TBGetDocEndorsementInfo(&nEndorsementsPending,
     &szSlipDocumentDescription,  &szSlipDocumentFront, &szSlipDocumentBack);
	
	CString csOperTxt = GetDisplayText(_T("OperGuid_Txt"));
	if(nEndorsementsPending && csOperTxt.Find(_T("Close")) != -1) //don't display close cash drawer while endorsement is inprogress
	{
		delete szSlipDocumentDescription;
		delete szSlipDocumentFront;
		delete szSlipDocumentBack;
		return;
	}

	delete szSlipDocumentDescription;
	delete szSlipDocumentFront;
    delete szSlipDocumentBack;

	CString csOperatorGuidance = _T("");
	bool bIsOperGuid = false;
	COleVariant vValue = GetFieldValue(_T("IsOperGuid"));

	long lDynaframeID;
    _variant_t vFieldValue = GetFieldValue(_T("DF_ID"));
    lDynaframeID = vFieldValue.intVal;

	_variant_t nOperGuidNum = GetFieldValue(_T("OperGuid_Num"));

	if (vValue.vt != VT_NULL && vValue.boolVal)
	{
		
		bIsOperGuid = true;

		csOperatorGuidance = GetDisplayText(_T("OperGuid_Txt"));
		
		//Start CRD 269544 setup message to display for Simplr Warranty
		if(nOperGuidNum.intVal == OPERATORMSGNO_MEPAVAILABLE)
		{
			CString csTemp = ps.GetPSText(SSF_TXT_MEPAVAILABLE, ps.m_languageUsed);
			CString csWarrantyItem = GetHookProperty(_T("Item_descriptor_Prop"));
			csWarrantyItem.TrimLeft();
			csWarrantyItem.TrimRight();
			csTemp.Format(_T("%s %s \\n%s %s"), csTemp, csWarrantyItem, csOperatorGuidance, ps.GetPSText(SSF_TXT_MEPPROMPT, ps.m_languageUsed));

			csOperatorGuidance = csTemp;
		}
		//End CRD 269544
		//Start CRD 295392
		else if( nOperGuidNum.intVal == OPERATORMSGNO_RESTRICTEDITEMMOBILE )
		{
			CString csLTT = ps.GetPSText(RA_THIRDLINE_TIMERESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY);
			CString csItemDesc = GetHookProperty(_T("OG_Guidance_Prop"));  
			CString csOrgMessage = ps.GetPSText(MSG_TIMERESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY);
			CString csDesc = _T("");
			csDesc.Format(csOrgMessage, csItemDesc);
			csOperatorGuidance.Format(_T("%s\n\n%s"), csLTT, csDesc);
		}
		//End CRD 295392
		
		
		if (csOperatorGuidance == _T(""))
		{
			csOperatorGuidance = GetDisplayText(_T("OperGuid_Prompt"));
		}
	}
	
	//Start CRD 185620
	//EAS - prevent from showing the operator message at the E-Receipt area so we can still see the e-receipt with the items
	//CRD 289150 - show e-receipt if it's recalling items
	if(isEASOperatorGuidance || bShowSmRecallingItems)
	{
		csOperatorGuidance = _T("");
		bIsOperGuid = false;	// this flag will keep the e-receipt shown
	}
	//End CRD 185620

	//Start CRD 235411 - check if operator msg num is still EAS however, we're done processing EAS, clear the message to fix screen flicker
	if(!isEASOperatorGuidance && nOperGuidNum.intVal == OPERATORMSGNO_EASTAGS)
		csOperatorGuidance = _T("");
	//End CRD 235411
	
    long lDrawerState;
	TBGetExpectedCashDrawerState(&lDrawerState);

	if (!ps.RemoteMode() || !dm.IsDeviceRemote(DMCLASS_CASHDRAWER, 0) && (lDrawerState & 1))  
    {
		//TAR# 306783
		SendTBCommand(TBCMD_CASHDRAWER_CLOSE, NULL );

    }
    ps.Echo(4, csOperatorGuidance);
	if (bIsOperGuid)
	{
		COleVariant v;
		v = (long) UI::Disabled;

		m_pAppPSX->SetConfigProperty(_T("AssistOperatorMessage"),  csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollDown"), csCurContext, UI::PROPERTYSTATE, v);
		m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollUp"), csCurContext, UI::PROPERTYSTATE, v);
	}
	else
	{
		m_pAppPSX->SetConfigProperty(_T("AssistOperatorMessage"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
	}
}

void SMSmAssistMode::SetChangeDueDisplay()
{
	COleVariant vValue = GetFieldValue(_T("ChangeDue"));

	CString strPropName = _T("FoodStampChangeDisplay");
    CString strFoodStampChange;
    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strPropName, &strFoodStampChange);

	if( strFoodStampChange == _T("1"))
	{
		strPropName = _T("FS_Change_Prop");
		CString csFSChange;
	    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strPropName, &csFSChange);

		CString csTemp = _T("");
		csTemp.Format(_T("$%.2lf"),( wcstod(csFSChange,NULL))/ 100.0);
		COleVariant vValue = csTemp;

		m_pAppPSX->SetTransactionVariable( _T("FSAmount"), vValue );
		
		m_pAppPSX->SetConfigProperty(
			_T("AssistFSChangeText"), 
			csCurContext, 
			UI::PROPERTYVISIBLE, 
			VARIANT_TRUE);

		m_pAppPSX->SetConfigProperty(
			_T("AssistFSChangeAmount"),
			csCurContext, 
			UI::PROPERTYVISIBLE, 
			VARIANT_TRUE);

		strPropName = _T("FoodStampChangeDisplay");
		strFoodStampChange = _T("0");
		SendTBCommand(TBCMD_SET_HOOKPROPERTY, &strPropName, &strFoodStampChange);

	}
	else
	{
		m_pAppPSX->SetConfigProperty(
			_T("AssistFSChangeText"), 
			csCurContext, 
			UI::PROPERTYVISIBLE, 
			VARIANT_FALSE);

		m_pAppPSX->SetConfigProperty(
			_T("AssistFSChangeAmount"),
			csCurContext, 
			UI::PROPERTYVISIBLE, 
			VARIANT_FALSE);
	}

	if (vValue.vt != VT_NULL && vValue.lVal)
	{
		CString csTemp = _T("");
		csTemp.Format(_T("$%.2lf"), vValue.intVal/ 100.0);

		m_pAppPSX->SetTransactionVariable( _T("ChangeAmount"), csTemp );

		m_pAppPSX->SetConfigProperty(
			_T("AssistChangeDueText"), 
			csCurContext, 
			UI::PROPERTYVISIBLE, 
			VARIANT_TRUE);

		m_pAppPSX->SetConfigProperty(
			_T("AssistChangeDueAmount"),
			csCurContext, 
			UI::PROPERTYVISIBLE, 
			VARIANT_TRUE);
	}
	else
	{
		m_pAppPSX->SetConfigProperty(
			_T("AssistChangeDueText"), 
			csCurContext, 
			UI::PROPERTYVISIBLE, 
			VARIANT_FALSE);

		m_pAppPSX->SetConfigProperty(
			_T("AssistChangeDueAmount"),
			csCurContext, 
			UI::PROPERTYVISIBLE, 
			VARIANT_FALSE);
	}
}

void SMSmAssistMode::PrcMethodMsg()
{
	CString csPrcMethod = _T("");
	COleVariant csTemp;
	bool bIsPrcMethod = false;
	static bool bIsSVCardBefore = false;
	COleVariant vValue = GetFieldValue(_T("IsPrcMethod"));
	if (vValue.vt != VT_NULL && vValue.boolVal)
	{
		bIsPrcMethod = true;

		csPrcMethod = GetDisplayText(_T("Prc_method_desc"));

		csTemp = GetFieldValue(_T("Prc_method_prc"));
	
	}
	
//	char temp[sizeof(csTemp.lVal) + 1];
//	memset(temp, '\0', sizeof(temp) );
//	sprintf(temp,"%d", csTemp.lVal );  
//	csPrcMethod = csPrcMethod + "     " + temp;
// 	ps.Echo(4, csPrcMethod);
	CString csControlDisplay;
	if (bIsPrcMethod)
	{
		//ride-in the SVCard Legal tender here
		CString strPropName = _T("IsItSVCardLegalTender");
		CString strIsSVCardLegalTender;
		SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strPropName, &strIsSVCardLegalTender);
		if (strIsSVCardLegalTender == _T("1"))
		{
			strPropName = _T("SVCardLegalTender");
			CString csSVCardLegalTender;
			SendTBCommand(TBCMD_GET_TBPROPERTY, &strPropName, &csSVCardLegalTender);
			csPrcMethod = csSVCardLegalTender;
			
			csSVCardLegalTender = _T("");
			SendTBCommand(TBCMD_SET_TBPROPERTY, &strPropName, &csSVCardLegalTender);

			csControlDisplay = _T("SVCardLegalTender");
			bIsSVCardBefore = true;
		}
		else
		{
            // POS31050 Use the FmtDollar to format the decimal in $
			/*char temp[sizeof(csTemp.lVal) + 1];
			memset(temp, '\0', sizeof(temp) );
			sprintf(temp,"%d", csTemp.lVal );  */
            CString csPrice = ps.FmtDollar( csTemp.lVal);
			csPrcMethod = csPrcMethod + "     " + csPrice;
			csControlDisplay = _T("PrcMethodMessage");
			bIsSVCardBefore = false;
		}
    	ps.Echo(4, csPrcMethod);
		
		COleVariant v = (long) UI::Disabled;
		m_pAppPSX->SetConfigProperty(csControlDisplay, csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollDown"), csCurContext, UI::PROPERTYSTATE, v);
		m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollUp"), csCurContext, UI::PROPERTYSTATE, v);
      
	} 
	else
	{
		if(bIsSVCardBefore)
		{
			csControlDisplay = _T("SVCardLegalTender");

		}else
		{
			csControlDisplay = _T("PrcMethodMessage");
		}

		m_pAppPSX->SetConfigProperty(csControlDisplay, csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
	}
}

void SMSmAssistMode::VoidItemMsg()
{
	CString csPrcMethod = _T("");
	COleVariant csTemp1, csTemp2, v;
	bool bIsPrcMethod = false;
	
	COleVariant vValue = GetFieldValue(_T("IsVoidItem"));
	if (vValue.vt != VT_NULL && vValue.boolVal)
	{
		bIsPrcMethod = true;

		csPrcMethod = GetDisplayText(_T("Void_item_desc"));
		csTemp1 = GetFieldValue(_T("Void_item_quantity"));
		csTemp2 = GetFieldValue(_T("Void_item_price"));
	
	}
	if (bIsPrcMethod)


	{
		CString temp1 = _T("");
		temp1.Format(_T("%d"),csTemp1.lVal);
		CString temp2 = _T("");
		temp2.Format(_T("%.2lf"),((double)csTemp2.lVal) / 100);
  
		CString csText1 = ps.GetPSText(SSF_TXT_VOIDATTEMPT_LINE1);
		CString csText2 = ps.GetPSText(SSF_TXT_VOIDATTEMPT_LINE2);
		CString csText3 = ps.GetPSText(SSF_TXT_VOIDATTEMPT_LINE3);
		csPrcMethod = csText1 + csPrcMethod + csText2 + temp1 + csText3 + temp2;
    	ps.Echo(4, csPrcMethod);

		v = (long) UI::Disabled;
		m_pAppPSX->SetConfigProperty(_T("VoidItemMessage"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollDown"), csCurContext, UI::PROPERTYSTATE, v);
		m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollUp"), csCurContext, UI::PROPERTYSTATE, v);

	}
	else
	{
		m_pAppPSX->SetConfigProperty(_T("VoidItemMessage"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
//Cut display message into smaller pieces and trace it
void SMSmAssistMode::TraceDisplayMessage()
{
	if (co.nTracingCurrentLevel >=7) //Only trace at level 7, but allow easy edit to lev 6
	{
		trace (L6, _T("Display message contents:"));
		CString csTemp = m_csLastDisplay;
		int nEndLine = csTemp.Find(_T("</field>"), 1000);
		while (nEndLine != -1)
		{
			trace(L6, _T("\n%s"), csTemp.Left(nEndLine + 8));
			if (csTemp.GetLength() != nEndLine + 8)
			{
				csTemp = csTemp.Mid(nEndLine + 8);
				nEndLine = csTemp.Find(_T("</field>"), 1000);
			}
			else
			{
				nEndLine = -1;
			}
		}
		if (csTemp.GetLength() > 0)
		{
			trace(L6, _T("\n%s"), csTemp);
		}
	}
}

SMStateBase *SMSmAssistMode::TBParse(MessageElement *me)
{
	//if the POS is at the trxn end state
	//then set the trxn ending variable correctly
    
	if(SendTBCommand(TBCMD_IS_TRXN_ENDING))
	{
		m_bTransactionEnding = true;
	}

	//Start CRD 289150 - display recall items if it's POS is at processing
	CString csNeedProcessing;
    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &CString(_T("NeedAMProcessing")), &csNeedProcessing);

	CString csCurPosState = GetHookProperty(_T("__POSState"));
	trace(L6, _T("SMSmAssistMode::TBParse - csPOSState = %s, csNeedProcessing = %s"),csCurPosState, csNeedProcessing);

	if ((me->nDeviceID == TB_DEV_DISPLAYPRIMARY) && (m_bTenderInProgress != true)) 
	{
		
        //if we are on a secure_exit screen then wait and let the TB sending the keys to get out
		if(csNeedProcessing ==_T("1"))
        {
			ps.SetLeadthruText(ps.GetPSText(LTT_CASHPROCESSING, SCOT_LANGUAGE_PRIMARY)); //processing screen
			//m_pAppPSX->SetControlProperty( _T("Display"), UI::PROPERTYREDRAW, VARIANT_TRUE );
			
			//clear the input window
			PSClearAllInput();
            
            // Disable POS key presses while in processing
            EnableDisableAllKeys(false);

			//don't let operator leave until the reason for processing is done
			COleVariant v;
			v = (long) UI::Disabled; // Disable
			//m_pAppPSX->SetControlProperty(_T("SMButton1"), UI::PROPERTYSTATE, v);
            m_pAppPSX->SetConfigProperty(_T("SMButton1"), csCurContext, UI::PROPERTYSTATE, v);

			m_pAppPSX->SetConfigProperty( _T("AssistReceipt"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE );

			ps.ShowFrame();
            
        }
		//Start CRD 289150 - force MEP, Price Visual Validate and High Value Promo contexts to refresh
		else if(
				((csCurPosState.Find(_T("MEP_AVAILABLE_MOBILE")) != -1 || csCurPosState.Find(_T("PRICE_VISUAL_VALIDATE")) != -1) && bShowPromptOnRefresh) 
				|| (csCurPosState.Find(_T("B2B_HIGHVALUE_PROMO_CHOICE")) != -1 && bShowB2BHighValuePromo)
			   )
		{
			//need to force show display/refresh if it's a simplr warranty prompts
			if(g_bInRecallMode)
			{
				trace(L6, _T("setting bShowSmRecallingItems to false"));
				bShowSmRecallingItems = false;
			}

			if(bShowPromptOnRefresh)
				bShowPromptOnRefresh = false;
			if(bShowB2BHighValuePromo)
				bShowB2BHighValuePromo = false;

			TBShowDisplay(true);
		}
		//CRD 289150 must be mPerks processing
        else if((csCurPosState.Find(_T("DIGITAL_COUPON_PROMO_ENTRY_ENABLED")) != -1 || csCurPosState.Find(_T("END_PROCESSING")) != -1)
			&& bFinishRecall)
		{
			trace(L6, _T("setting bShowSmRecallingItems to true"));
			bShowSmRecallingItems = true;
			bShowFinishRecall = false;
			TBShowDisplay();
		}
		//End CRD 289150
		else
        {          

			//Start CRD 289150 - a new context display in the POS that needs to be displayed, don't display recalling items screen
			if(g_bInRecallMode || bFinishRecall)
			{
				trace(L6, _T("setting bShowSmRecallingItems to false"));
				bShowSmRecallingItems = false;
			}
			//End CRD 289150
		    TBShowDisplay();
        }

        //POS54351        
        if( ps.RemoteMode() )
        {
            CString csPosState = GetHookProperty(_T("__POSState"));
            if( (csPosState.Find(_T("RESUME_READY")) != -1) ||
                (csPosState.Find(_T("PAYMENTMENU_DK1")) != -1) || 
                (csPosState.Find(_T("HIGHVALUE_PROMO_CHOICE")) != -1) 
              )
            {
                SetHookProperty(_T("VoidPrepaidCards"), _T("0"));
                SetHookProperty(_T("PropMobShopOrderPrompt"), _T("0"));            
            }   
        }
        //POS54351 
		
	}
    else if (me->nDeviceID == TB_DEV_TOTAL_DETAILS)
	{
        //in order to prevent the constant setting of the current item on the POS
        //check if the totals have changed since the last time we set the selected
        //item.  If so then change the selected item.
        long lPrevTax, lPrevTotal, lCurrTax, lCurrTotal;

        //get the previous totals
        lPrevTax = lTaxDue;
        lPrevTotal = lTotalSale;


		//fix for TAR#308070
		CString csPropEnterPressed = _T("PropEnterPressed");
		CString csEnterPressed;
		SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csPropEnterPressed, &csEnterPressed);
		
		CString csPropertySaleRestricted = _T("PropSaleRestricted");
		CString csSaleRestricted;
		SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csPropertySaleRestricted, &csSaleRestricted);

		if(csSaleRestricted == _T("1"))
		{
			if(csEnterPressed == _T("1"))
			{
				csEnterPressed = _T("0");				
				SendTBCommand(TBCMD_SET_HOOKPROPERTY, &csPropEnterPressed, &csEnterPressed);
			}
			else
			{
				csSaleRestricted = _T("0");
				SendTBCommand(TBCMD_SET_HOOKPROPERTY, &csPropertySaleRestricted, &csSaleRestricted);
				fSAInItemSale = 1;
			}
		}
		//end of fix
        

		
        SMStateBase *pState = SMState::TBParse(me);
	    
        //now get the new totals
        lCurrTax = lTaxDue;
        lCurrTotal = lTotalSale;
        
        //are they the same?
        if((lPrevTax != lCurrTax ) || (lPrevTotal != lCurrTotal))
        {
			// sotf 9269 - dont' move the cursor if it's in AM - rrs
            //SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(_T("AssistReceipt"))); //for TAR302439
        }
        return pState;
	}

	SMStateBase *pState = SMState::TBParse(me);

	switch(m_TBLastState)
	{
		//+CRD185924
		case TB_POS_RESTART_COMPLETE:
		{
		   trace (L6, _T("Existing assistmode automatically as POS just completely restarted."));
		   
		   delete getRealStatePointer( pState);
		   setAnchorState(BES_LANECLOSED);
		   
		   return PSButton1();   
	       break;
		} //-CRD185924

		//Start CRD 289150 - generic handler
		//we don't have any trigger from POS to show Transferring Items screen but only TB_ITEMSOLD
		case TB_ITEMSOLD:
		{
			bShowPromptOnRefresh = true;

			if(!bShowSmRecallingItems && g_bInRecallMode)
			{
				trace(L6, _T("setting bShowSmRecallingItems to true"));
				bShowSmRecallingItems = true;
				TBShowDisplay();
			}

			//Start CRD 347846
			if(g_bIsProcessingEverseenAlert)
				g_bIsProcessingEverseenAlert = false;
			//End CRD 347846


			break;
		}
		default:
		{
			if(g_bInRecallMode && m_TBLastState != TB_TOTAL && m_TBLastState != TB_IGNORE && m_TBLastState != TB_RESUME_COMPLETE)
			{
				trace(L6, _T("setting bShowSmRecallingItems to false"));
				bShowSmRecallingItems = false;
				
				TBShowDisplay();
			}
			break;
		}
		//End CRD 289150
			
	}
	ps.ShowSMTotal(true);
	return pState;
}

void SMSmAssistMode::AddSerialNumToReceipt(void)
{
//	TBAddMessagetoReceipt();
	TBGetMessageInfo();
	return;
}
///////////////////////////////////////////////////////////////////////////
void SMSmAssistMode::TBShow2x20(void)
{
	return;
}


///////////////////////////////////////////////////////////////////////////
void SMSmAssistMode::TBShow2x20(CString csOpDisplay)
{
	ps.SetLeadthruText(csOpDisplay,_T(""));
	return;
}

/////////////////////////////////////////////
SMStateBase *SMSmAssistMode::PSReceiptDown(void)
{ 
	// (+) POS154826
    CString csPosState = GetHookProperty(_T("__POSState"));
	
    if(csPosState == _T("WIC_CUSTOMER_CONFIRMATION1") || csPosState == _T("WIC_CUSTOMER_CONFIRMATION2") || csPosState == _T("WIC_CONFIRM_RECEIPT_DONE"))
    {
        ps.ScrollDown(_T("WICList"));
		SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(_T("WICList")));
    }
    else
    {
		// +SSCOI-46521
		CString csItemEntryId;
		COleVariant vValue;
		vValue.Clear();
		vValue.boolVal = VARIANT_FALSE;
		
		// NextGen //
		if (nPSLastWParam == 1111 && nPSLastParm > 0)
		{
			CString csItemId;
			CString* pTemp =(CString *) nPSLastParm;
			if(pTemp != NULL)
			{				
				csItemId= *pTemp;
				delete pTemp;					
				ps.GetPSXObject()->SetSelectedReceiptItem(_T("AssistReceipt"), csItemId);			
				SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(_T("AssistReceipt")));
				return STATE_NULL;
			}

		}
		// -SSCOI-46521
		//*** NewUI ***// 
		ps.ScrollDown(_T("AssistReceipt")); // SSCOI-47021
		SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(_T("AssistReceipt")));
    }
	// (-) POS154826
	
	return STATE_NULL;
}

/////////////////////////////////////////////
SMStateBase *SMSmAssistMode::PSReceiptUp(void)
{
	
	// (+) POS154826
	
    CString csPosState = GetHookProperty(_T("__POSState"));
	
    if(csPosState == _T("WIC_CUSTOMER_CONFIRMATION1") || csPosState == _T("WIC_CUSTOMER_CONFIRMATION2") || csPosState == _T("WIC_CONFIRM_RECEIPT_DONE"))
    {
        ps.ScrollUp(_T("WICList"));
		SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(_T("WICList")));
    }
    else
    {
		//*** NewUI ***//
		ps.ScrollUp(_T("AssistReceipt"));
		SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(_T("AssistReceipt")));
    }
	
	// (-) POS154826
	return STATE_NULL;
}

void SMSmAssistMode::DisableSoftKeys()
{
    ps.DisableAssistModeKey(_T("Key24"), -1);
    ps.DisableAssistModeKey(_T("Key25"), -1);
    ps.DisableAssistModeKey(_T("Key26"), -1);
    ps.DisableAssistModeKey(_T("Key27"), -1);
    ps.DisableAssistModeKey(_T("Key28"), -1);
    ps.DisableAssistModeKey(_T("Key29"), -1);
    ps.DisableAssistModeKey(_T("Key30"), -1); 
    ps.DisableAssistModeKey(_T("Key31"), -1);
	
    // TAR 359256 - To prevent having all keys disabled on next refresh if 
    //              The same display comes in only with keys enabled.
    //              This will avoid hanging when we get the ACS screen bugs
    //              where a dynakey needs to be pressed twice to work.
    g_bRefreshAfterSoftKeyDisable = true;
}

void SMSmAssistMode::DisableKeys()
{
	ps.DisableAssistModeKey(_T("Key12"), -1);
	ps.DisableAssistModeKey(_T("Key24"), -1);
}

SMStateBase *SMSmAssistMode::DMReceiptPrinter(DWORD dwEvent)
{
    trace (L6, _T("+SMSmAssistMode::DMReceiptPrinter(0x%08.8x <%s>)"), dwEvent, dm.GetEventDescription(dwEvent));
    
    SMStateBase * pRet = SMSmAssistModeBase::DMReceiptPrinter( dwEvent );
    
	//sotf 6263
    /*if( fInEndorsementState != ENDORSE_NONE )
    {
        switch(dwEvent)
        {
            // non-error events to handle
		//SOTF 6263
        //case DM_PTR_COVER_OPEN:
            
            // error printer events
        case DM_PTR_ERR_COVER_OPEN:
        case DM_PTR_ERR_EMPTY:
        case DM_PTR_ERR_FAULT:
        case DM_PTR_ERR_REQUEST_SEQUENCE:
        case DM_PTR_ERR_HEAD_JAM:  
        case DM_PTR_ERR_KNIFE_JAM:
        case DM_PTR_ERR_PAPER_JAM:
        case DM_PTR_ERR_PAPER_NOT_LOADED_JAM:
        case DM_PTR_ERR_COMMS_ERROR:
        case DM_PTR_ERR_PRINTER_OPEN:
        case DM_PTR_ERR_REPLACE_HEAD:
        case DM_PTR_ERR_SIDEWAYS_COMMS_ERROR:
        case DM_PTR_ERR_EJECT_JAM:
        case DM_PTR_ERR_BLACKMARK_ERROR:
        case DM_PTR_ERR_TIME_OUT:
            {
                trace(L0, _T("Received a receipt printer error while an endorsement was in progress."));
                trace(L0, _T("Clearing the current endorsement and notifying TB. Retry after printer error corrected."));
                fInEndorsementState = ENDORSE_NONE;
                m_nEndorsementsPending = 0;
                TBEndorsementCompleted();
            
                break;
            }
        };
    }
    end sotf 6263*/
    trace (L6, _T("-SMSmAssistMode::DMReceiptPrinter()"));
    return pRet;
}

bool SMSmAssistMode::IsKeyAllowed(CString csPOSString, long nKey)
{
    bool bSendKeys = true;
    
    if( 
        (m_bWaitingForMICRInsertion && DMSlipPrinterIsSlipInserted()) || // MICR is active
        ((GetTickCount() - g_dwLastMICREvent) < 2000)                    )// MICR event completed less than 2 seconds ago.
        
    {
        trace(L4,_T("    Ignoring all key presses while MICR is being processed."));
        bSendKeys = false;
    }
    //if we are on a Micr screen then ignore numeric keys and enter
    if ( (m_bWaitingForMICRInsertion || (GetDisplayText(_T("Title1")).Find(_T("Routing Number")) != -1)) &&
       ((csPOSString.CompareNoCase(_T("{ENTER}")) == 0) || (m_nAssistKeyVirtualKeyValue[nKey] >= VK_NUMPAD0 && m_nAssistKeyVirtualKeyValue[nKey] <= VK_NUMPAD9)))
    {
        trace(L4,_T("    Ignoring {ENTER} and numeric key while waiting for MICR"));
        bSendKeys = false;
    }

// if we are waiting for a slip insertion for endorsement, ignore the keys
    if( fInEndorsementState == ENDORSE_INPROGRESS )
    {
        trace(L4,_T("    Ignoring %s while waiting for slip insertion or endorsement is in progress."), (LPCTSTR)csPOSString);
        bSendKeys = false;
    }

    if ( nTimeOut == TIMEOUT_DISABLED )
    {
        trace(L6, _T("    Key %s was blocked in AssistMode due to timeout disabled"), csPOSString );
        bSendKeys = false;
    }

    if(csPOSString == _T("")) 
    {
        trace(L6, _T("    AssistMode Key is empty") );
        bSendKeys = false;
    }

	CString strTenderPropName = _T("TestTenderType");
	CString strTenderValue = _T("");
    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strTenderPropName, &strTenderValue);

		//TAR353551+
    //If we get VPIM message which requires to cancle the trransaction, we need to send the button press.
	//For VPIM, OperGuid_Num is 116
	_variant_t nOperGuidNum = GetFieldValue(_T("OperGuid_Num"));  //TAR353551

	if (nOperGuidNum.intVal == 116)
	{
		g_bGotVPIMInAssistMode = true;
	}
	//TAR3535551-

    // The following code is to prevent the numerous out-of-sync POS/FL situations that 
    // can easily be generated while ending a transaction in AM (be it cash, credit, check etc)
    // There is a large window between closing the cashdrawer and exiting assist mode where 
    // AM buttons can be pressed that will send the POS to states or sell ghost items.
    //
    // OG messages should override this though and button presses should be passed through
    // even if we're ending the transaction. This would prevent problems such as that with 
    // Although the simple fix is to disable all buttons during TRXEND, there can be problems
    // TAR353551 where Operator Guidance messages can come from the POS during the 
    // transaction end and at this point we need to re-enable the buttons as the POS is 
    // expecting input to handle the results.
    // -- Code Start --
    CString csTBState(_T(""));
    SendTBCommand(TBCMD_GET_TB_STATE, &csTBState);
    CString csIsOpGuid(_T(""));
    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &CString(_T("PropIsOperGuid")), &csIsOpGuid);

    if( bSendKeys 
        && (-1 != csTBState.Find(_T("ASSISTMODE_TRXEND")))
        && (csIsOpGuid == _T("0"))  )
    {
        trace(L6, _T("    Key %s was blocked in AssistMode because the transaction is ending."), csPOSString);
        bSendKeys = false;
    }
    // -- Code End --
       
	//TAR345889+
	//We should ignore button press after TB_TENDERACCEPTED(no balance left) is received, otherwise, POS and TB
	//can be out of sync

	// commented this implementation as it creates a lot of trouble. Sometimes the POS still has
	//an operator message occurred after the TB_TENDERACCEPTED and it can't be clear. The check tendering
	//also still has a lot of screens after TB_TENDERACCEPTED and seems the assistmode already hung.
	/*
	trace(L6, _T("lBalanceDue = %d;  bIsInFinalization = %d;  m_TBLastState = %d; strTenderType = %s "), 
		lBalanceDue, bIsInFinalization, m_TBLastState, strTenderValue );
	if ( bSendKeys && (lBalanceDue <=0) && bIsInFinalization && (m_TBLastState  != TB_CLOSED) && strTenderValue.Find(_T("CHECK"))== -1 )
	{
		//TAR353551+
		if (g_bGotVPIMInAssistMode && ( (csPOSString.CompareNoCase(_T("{F2}")) != -1) || (csPOSString.CompareNoCase(_T("{F1}")) != -1)) )
		{
			bSendKeys = true;
		}
		else
		{
		//TAR353551-
			trace(L6, _T("    Key %s was blocked in AssistMode due to transaction is ending"), csPOSString );
			bSendKeys = false;
		}
	} */
	//TAR345889-
        
    return bSendKeys;


}

void SMSmAssistMode::ClearApprovalFlags(void)
{

    //This to clear any approval when there is an operator message
	lNotForSaleItems = 0;
	lTimeRestrictedItems = 0;
	lUnknownItems = 0;
	lUnknownItemsKeyed = 0;
	lPriceRequiredItems = 0;
	lQuantityRestrictedItems = 0;
	fUnDeActivatedItem = false;
	SMState::bVoidApprovalDelayed = false;  //VSTS 131568
	g_bClearAllDelayedSecIntervention = false;

    //SOTF 6226
    if(!SendTBCommand(TBCMD_ACS_DELAYED_AGE_RESTRICTION))
    {
       
    
	    lUnapprovedRestrictedItems =0;
	    lRegularYellowNeedingApproval = 0;
	    bInApprovalNeeded = false; 
	    fApprovedForRestrictedItems = true;
	    fAgeObtained = true;
	    nCustomerAge = 99;
       
	    m_lAgerestrictQueuedtoRAP = 0;
    }
}

void SMSmAssistMode::ResetPropertiesBasedOnScreenAndKeys(CString csPOSString, CString csTitleOneText )
{
    //check if we are on ERROR_CONTEXT_CLEAR screen.
    CString csCurPosState(_T(""));
    SendTBCommand(TBCMD_GET_POSSTATE, &csCurPosState); 
    if (csCurPosState.Find( _T("ERROR_CONTEXT_CLEAR")) != -1)
	{
        nTimeOut = g_lOrigTimeoutValue;
        SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
        DMScannerEnable();
        TBShowDisplay(true, true);
	    SendTBCommand(TBCMD_RESET_TO_LAST_POSSTATE, NULL);
    }
    
	long lDynaframeID;
    _variant_t vFieldValue = GetFieldValue(_T("DF_ID"));
    lDynaframeID = vFieldValue.intVal;
	
	_variant_t bIsOperGuid = GetFieldValue(_T("IsOperGuid"));
	_variant_t nOperGuidNum = GetFieldValue(_T("OperGuid_Num"));
	
	//POS291954 - aab
	if (bIsOperGuid.boolVal && m_bIsFromDLScanning)
	{
        m_bIsFromDLScanning = false;
		trace(L6, _T("Cancelling item at security mgr."));
		SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
	}

	bool bIsConfirmedAgePrompt = ((nOperGuidNum.intVal == OPERATORMSGNO_INSPECTEDIDAGE  || nOperGuidNum.intVal == OPERATORMSGNO_INSPECTEDIDAGEOVER40) && bIsOperGuid.boolVal);//POS296838 //SSCOI - 73518
	trace(L6, _T("Is confirmed age prompt[%d]."), bIsConfirmedAgePrompt);
	//age not needed
    if( csPOSString == _T("{F1}") && ( lDynaframeID == DF_AGESELECTOPTION || bIsConfirmedAgePrompt)) // SSCOI-54479
	
	{
		if(nDMCashTendered) //POS289891 - just in case there was a pending cash payment prior going to assistmode
		{
			trace(L6, _T("There was a pending cash tender of %d"), nDMCashTendered);
			nDMCashInserted = nDMCashTendered;
			nDMCashTendered = 0;
		}

        ClearApprovalFlags();  //move here before clearing IsDelayedAgeRestrictedProp - POS293066

		//POS290920
		if( bIsConfirmedAgePrompt || lDynaframeID == DF_AGESELECTOPTION) //F1 of 1010 is acknowledge over 40
		{
			if(bIsConfirmedAgePrompt)
			{
                CString csOperatorGuidance = GetDisplayText(_T("OperGuid_Txt"));
                int nAgeIndex = csOperatorGuidance.Find(_T("age"));

                if(nAgeIndex != -1)
                {
                    CString csAge = csOperatorGuidance.Mid(nAgeIndex + 5); //index 5 in like "age, 17 years old.."
                    csAge = csAge.Left(2);
                    nCustomerAge = _ttol(csAge);

                    if(nCustomerAge == 0) // SSCOI - 73518
                        nCustomerAge = 99;
            
                    trace(L6, _T("Age acquired from POS, %d"), nCustomerAge);
                }
                else
                    trace(L6, _T("No age being acquired!"));	  
			}
			else if (lDynaframeID == DF_AGESELECTOPTION) // SSCOI-46726
			{
				nCustomerAge = 99;
			}
			//-POS293858
			SetHookProperty(_T("IsDelayedAgeRestrictedProp"), _T("0"));

			//Start VSTS# 330950. Need to reset restricted approval flags.
			lUnapprovedRestrictedItems =0;
			lRegularYellowNeedingApproval = 0;
			bInApprovalNeeded = false; 
			fApprovedForRestrictedItems = true;
			fAgeObtained = true;
			nCustomerAge = 99;
			//m_lAgerestrictQueuedtoRAP = 0;
			//End VSTS# 330950

		}

        //sotf 6226
        if(SendTBCommand(TBCMD_ACS_DELAYED_AGE_RESTRICTION))
        {
            CString csStrPropValue;
            csStrPropValue = GetHookProperty(_T("AgeRequiredLeadThru"));
            csStrPropValue.TrimRight();
            csStrPropValue.TrimLeft();
            int iLen = csStrPropValue.GetLength();
            if(iLen >= 3)
            {
                csStrPropValue = csStrPropValue.Mid((iLen - 3), (iLen -1));
                if(!csStrPropValue.IsEmpty())
                {
                    long lItemAge = _ttol(csStrPropValue);
                    if(lItemAge > nCustomerAge)
                    {

                        nCustomerAge = lItemAge;
                    }
                }
            }
        }
        //sotf 6226

        CString csDateNotNeeded = ps.GetPSText(RA_AGE_RESTRICTED_ID, SCOT_LANGUAGE_PRIMARY);
        csDateNotNeeded +=  _T("\n") + ps.GetPSText(BTT_BIRTHDATENOTREQUIRED, SCOT_LANGUAGE_PRIMARY);
        trace(L7, _T("+ra.OnNormalItem, Message=%s"), csDateNotNeeded);
        ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csDateNotNeeded); // Display valid birthday on RAP
        trace(L7, _T("-ra.OnNormalItem"));  

        // remote approval
        CString csDesc = ps.GetPSText(RA_AGE_RESTRICTED_ID);
		ps.RestrictedItemNotAllowed(nCustomerAge, lUnapprovedRestrictedItems); // SSCOI-46726

        //TAR 196910 start
        while (m_lAgerestrictQueuedtoRAP > 0)
		{
	      trace(L7, _T("+ra.OnAgeApproval"));
	      ra.OnAgeApproval();
	      trace(L7, _T("-ra.OnAgeApproval"));
	      m_lAgerestrictQueuedtoRAP--;
		}
        m_lAgerestrictQueuedtoRAP = 0;
   }

   //sotf 6226
   if((csPOSString == _T("{ENTER}")) && 
      (csTitleOneText == _T("Age Verify")) && 
      (SendTBCommand(TBCMD_ACS_DELAYED_AGE_RESTRICTION)))
    {
        long lCustAge = GetAge(g_csInputData);
        if(lCustAge)
        {
            nCustomerAge = lCustAge;
        }
    }
    // -sotf

   if( csTitleOneText == _T("Operator Message")) 
   {
	   //This to clear any approval when there is an operator message
	   ClearApprovalFlags();
   }

}

CString SMSmAssistMode::GetSoftKeyButtonText(long lBtnNumber)
{
    CString csRetStr(_T(" "));
    //invalid Soft key
    if((lBtnNumber < 24) || (lBtnNumber > 31))
    {
        trace(L6, _T("SMSmAssistMode::GetSoftKeyButtonText -- passed an invalid softkey!"));
        return csRetStr;
    }

    //reset the btn index to 1 through 8
    lBtnNumber += -23;  
	CString csLabel;	
	csLabel.Format(_T("DK%d_Label"),lBtnNumber);
	
	
	csRetStr = GetDisplayText(csLabel);  
	if (csRetStr == _T(""))
	{
		csLabel.Format(_T("DK%d_Label_Alt"), lBtnNumber);
		csRetStr = GetDisplayText(csLabel);
		if (csRetStr == _T("")) 
        {
            trace(L6, _T("SMSmAssistMode::GetSoftKeyButtonText -- btn%d has no text!"), (lBtnNumber + 1) );
			csRetStr = _T(" ");
        }
	}

    return csRetStr;



}

bool SMSmAssistMode::IsSpecialHandlingScreen(void)
{
    bool bRetVal = false;

    CString csTitleOneText = GetDisplayText(_T("Title1"));    
    csTitleOneText.TrimLeft();
    csTitleOneText.TrimRight();

    if( 
        (csTitleOneText == _T("Account Number")) || 
        (csTitleOneText == _T("Drivers License")) ||
        (csTitleOneText == _T("Warranty Input")) || 
        (csTitleOneText == _T("Enter state code for id."))
      )

    {
        bRetVal = true;
    }

    return bRetVal;

}

void SMSmAssistMode::AMClearAllInput(void)
{
	PSClearAllInput();
	CString csPropName = _T("EntryData_Prop");
	CString csValue = _T("");
	SendTBCommand(TBCMD_SET_HOOKPROPERTY, &csPropName, &csValue);
}

void SMSmAssistMode::ShowCashDrawerCloseMessageIfApplicable(bool bShowDisplay)
{
    trace(L6, _T("+SMSmAssistMode::ShowCashDrawerCloseMessageIfApplicable()"));
    
    long nEndorsementsPending=0;
    LPTSTR szSlipDocumentDescription = NULL;
    LPTSTR szSlipDocumentFront = NULL;
    LPTSTR szSlipDocumentBack = NULL;
    
    TBGetDocEndorsementInfo(&nEndorsementsPending,
        &szSlipDocumentDescription,  &szSlipDocumentFront, &szSlipDocumentBack);
    
    CString csPlaceCheckPrompt = GetHookProperty(_T("PlaceCheckDrawerMsg"));

    if(!nEndorsementsPending && csPlaceCheckPrompt != _T("") && ps.RemoteMode() )
    {
        trace(L6, _T("Slip printing has completed and cash drawer message available."));
        ps.SetLeadthruText(csPlaceCheckPrompt);
//        m_pAppPSX->SetControlProperty( _T("Display"), UI::PROPERTYREDRAW, VARIANT_TRUE );
        ps.ShowFrame();
    }
    else if( bShowDisplay )
    {
        TBShowDisplay( true );
    }
    
    delete szSlipDocumentDescription;
    delete szSlipDocumentFront;
    delete szSlipDocumentBack;
    trace(L6, _T("-SMSmAssistMode::ShowCashDrawerCloseMessageIfApplicable()"));
}


bool SMSmAssistMode::CheckIfPrinterAvailable(bool bRetryPrints)
{
	bool bRetValue = false;

    if ((ps.RemoteMode()) &&		
	   ((!dm.IsDeviceRemote( DMCLASS_RECEIPT, 0 )) ||
	   (!dm.IsDeviceConfigured( DMCLASS_CASHDRAWER, 0))))
    {

		trace(L4,_T("SMSmAssistMode:: CheckIfPrinterAvailable() -- Printer Not Available"));
        ps.Echo(8, ps.GetPSText(9103));

		//did we already send the prints to the lane?
		//if so don't attempt again, it takes too long
		if(!m_bUnRemotedPrinterDevices)
		{
			 // Force Printing at the lane, since the RAP printer cannot be fixed
			dm.EndRemote();
        
			trace(L0,_T("Cash Drawer is NOT remote. Tell the TB"));
			SendTBCommand(TBCMD_CASHDRAWER_FAILED, NULL );

			Sleep(1000);
			dm.BeginRemote( _T("RemoteNoPrinter") );

			m_bUnRemotedPrinterDevices = true;
		}
        
		if(bRetryPrints)
		{
			long lInEndorsementState = fInEndorsementState;
			if((fInEndorsementState != ENDORSE_NONE) || (m_szSlipDocumentBack != NULL))
			{
            
				m_bSlipActivityIsWaiting = false;
				trace (L0, _T(" SMSmAssistMode::Initialize())  Forcing chit print at lane"));
				// Prohibit multiple chits per endorsement.
				m_nEndorsementsPending = 0;
				PrintEndorsementChit(m_szSlipDocumentBack);
				TBEndorsementCompleted();
			}

			if ( hReceiptFile )
			{
				fDMReceiptError = false;
				fDMJournalError = false;

            
				PrintReceipt( hReceiptFile );
				if (nTrxReceiptLength)
				{
					CutReceipt();
				}
            
				TBReleaseReceiptData(hReceiptFile);
            
			}
		}
		
		
        
    }else
	{
		trace(L4,_T("SMSmAssistMode:: CheckIfPrinterAvailable() --No Device Errors"));
        ps.Echo(8, _T(""));

        //+SOTF 8327
        if(( ENDORSE_NONE != fInEndorsementState ) && (m_bPreviousDeviceError))
	    {
            trace(L6, _T("Waiting for slip insertion due to previous slip error..."));
            m_bPreviousDeviceError = false;
            DMSlipPrinterInsertSlip(true);
        }
        //-SOTF 8327
		m_bUnRemotedPrinterDevices = false;
		bRetValue = true;
	}
	
	return bRetValue;
}

SMStateBase* SMSmAssistMode::HandleVoidedTender(void)
{
    trace(L0, _T("SMSmAssistMode::HandleVoidedTender()"));

    SMStateBase *pNextState = SMSmAssistModeBase::HandleVoidedTender();
    
	
	CString csOptionValue(_T(""));
	SendTBCommand(TBCMD_GET_OPTIONS, &CString(_T("VoidedTenderChangeIssuedAtRap")), &csOptionValue);
	csOptionValue.MakeUpper();
   
    if((csOptionValue == _T("Y")) &&(CheckIfPrinterAvailable(false)) &&(lChangeDue && !co.fTenderDispenseChangeAtLaneForRemoteTendering && ps.RemoteMode()  )) //Remote AssistMode Tendering
    {
		delete getRealStatePointer( pNextState);
		fStateAssist = true;

		trace(L6, _T("Cash Drawer is open at remote assistmode due to voided tender"));


		//turn off the go back button while the cash drawer is open
		COleVariant v;
		v = (long) UI::Disabled;
		m_pAppPSX->SetConfigProperty(_T("SMButton1"), csCurContext, UI::PROPERTYSTATE, v);
		
		//Pop up cash drawer to let cashier giving change to customers.
		dm.CashDrawerOpen();  
		m_bRequestingCashDrawerOpen = true;

		//reporting        
		rp.RAPDispenseChange(lChangeDue);
		lChangeDue = 0;    
        
		nTimeOut = 1;

		
		return STATE_NULL;
    }else 
	{
      return pNextState;

    }

   
}

SMStateBase *SMSmAssistMode::DMCashDrawer(DWORD dwEvent)
{
    //SOTF 413984/6265
	//did we pop the cash drawer and now it is closed?
	if(DM_DRAWER_CLOSED == dwEvent) 
	{
		//if the trxn is ending and the cash drawer has been closed
		//go ahdead and report the change and clear it out
		//since Meijer has turned off the fTenderDispenseChangeAtLaneForRemoteTendering flag
		//the base will try to manage the cash drawer and decide when to open it.
		//Currently the TB will decide to open the cash drawer based on what the POS.
		//clearing out the change when the drawer has been closed prevents a second cash
		//drawer pop from the base
		if((m_bTransactionEnding) && (lChangeDue))
		{
			rp.RAPDispenseChange(lChangeDue);
			lChangeDue = 0;   
		}
		if(m_bRequestingCashDrawerOpen)
		{
			m_bRequestingCashDrawerOpen = false;
			TBShowDisplay(true, false);
		}
		
		//end 413984/6265
	}
	
	return SMStateBase::DMCashDrawer(dwEvent);
}	

/////////////////////////////////////////////
SMStateBase * SMSmAssistMode::DepositCouponsIfNecessary()
{
    return SMSmAssistModeBase::DepositCouponsIfNecessary();
}

//Start CRD 289150 - ported from Customer Mode Recalling Items state
void SMSmAssistMode::AMFinishRecall()
{
	trace(L6, _T("+SMSmAssistMode::AMFinishRecall"));
    
	bShowFinishRecall = true; //to show Finish Recall screen in AM
    g_bInRecallMode = false;
	rp.ModeChange(rp.m_lAttendantEntryID, _T("RECALLMODE"), EXIT);

	CString csItemsSold;
    csItemsSold.Format(ps.GetPSText(MSG_NUMBER_ITEMS_RECALLED, SCOT_LANGUAGE_PRIMARY), lItems);
    ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + ps.GetPSText(MSG_RECALLMODE_END, SCOT_LANGUAGE_PRIMARY) +
        _T("\t\t  ") + csItemsSold); // inform RAP about entering/leaving recall mode
    trace(L7, _T("-ra.OnNormalItem"));

	UpdateStateName(ps.GetPSText(MSG_LOADING_TRANSACTION, SCOT_LANGUAGE_PRIMARY));

	SendTBCommand(TBCMD_CMDSTRING,CString(_T("SetRecallingModeFalse")));
}

void SMSmAssistMode::AMFinishRecallDone()
{
	bFinishRecall = false;	//SSCOI-55092
	bShowFinishRecall = false;

	// Tell POS FLMS item processing is complete.
	SendTBCommand(TBCMD_FLMS_TRANSFERRING, false);

	CKEYVALUEPARMLIST parmList;
    CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_EXIT_RECALLMODE);
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    BSTR bstrResult;
    bstrResult = NULL;
    SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
    SysFreeString(bstrResult);

	mp.LogTransition((CString)_T("SMEndMobileReplay"), nTBLastRealState);

	TBShowDisplay(true);
}
//End CRD 289150