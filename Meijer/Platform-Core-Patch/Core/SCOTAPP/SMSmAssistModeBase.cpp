//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAssistModeBase.CPP
//
// TITLE: Class implementation for Assist Mode keyboard
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMSmAssistMode.h"

#include "SMSmAuthorization.h"

#include "SMDepositCoupons.h"
#include "SMWaitForApproval.h"
#include "SMScanAndBag.h"				//  scan and bag state
#include "SMDegradedContinueTrans.h"	//degraded mode tars

 //extern TSProcedures ts;
DLLEXPORT extern DMProcedures dm;
DLLEXPORT extern HDEVCONTEXT hDC;             // handle to the device context from PSThread()
// NewUI removed PSInterface 

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmAssistModeBase")
bool bAssistModeReturn = false;		// Tar 213624 RJF 090602

const int SMSmAssistModeBase::ASSISTMODE_RETURN_TIMEOUT = 1;
int nAutoListIndex = 0;

IMPLEMENT_DYNCREATE(SMSmAssistModeBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmAssistMode)

DEFINE_TIMESTAMP

////////////////////////////////
SMSmAssistModeBase::SMSmAssistModeBase()
: csNumData(_T("")),
	lSlashData(0),
	lTareCode(0),
	lWeightEntered(0), 
	lQuantityEntered(0),
	lUnitPriceEntered(0),
	lExtPriceEntered(0),
	fVoided(false),
	fWaitingForEOT(false),
	fWaitingToExit(false),
	fItemizing(false),
	fTendering(false),
    bSetMonitorFalse(true),
    m_bToggled(false), //TAR 272625
    m_bPartialAssistEnabled(false),
	sReturnState(STATE_NULL),
    m_szSlipDocumentDescription (NULL), m_szSlipDocumentFront(NULL), m_szSlipDocumentBack(NULL),
    m_nEndorsementsPending(0), m_bWaitingForMICRInsertion(FALSE), m_nMICREvents(0),
	fWaitingOnBelt(false), csCurContext(EMPTY_STRING), retryReceiptAttempts(0)
{
  IMPLEMENT_TIMESTAMP
}

SMSmAssistModeBase::~SMSmAssistModeBase()
{
    delete m_szSlipDocumentDescription;
    delete m_szSlipDocumentFront;
    delete m_szSlipDocumentBack;
}

////////////////////////////////////
SMStateBase  *SMSmAssistModeBase::Initialize()
{
    trace(L0, _T("+SMSmAssistModeBase::Initialize()"));
    // Reset the Assist Mode key queue to ensure we can 
    // process keystrokes going into assist mode.
    (*CPSXInterface::instance()).SetAssistKeyQueued(false);
    
	if (fInMultiSelectPickList)
	{
		if ((ps.GetPicklistItemsSold() > 0) && !g_bOnItemOK && !io.d.fVoid)
		{
			//SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
			CKeyValueParmList parmList;
			CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_SKIP_BAGGING);
			_bstr_t bstrParmList((const _TCHAR *)parmList);
			BSTR bstrResult;
			bstrResult = NULL;
			SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
			SysFreeString(bstrResult);
			nSAPrevSCOTInputID = 0;
		}

		fInMultiSelectPickList = false;

		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("MultiPicklist") );
	}

    bool bEnteredInitAfterToggle = m_bToggled; //TAR 272625
    //m_bToggled = false; //TAR 272625

	if (!co.fStateAssistMode)
	{
        trace(L0, _T("-SMSmAssistModeBase::Initialize() Going to SMAuthorization"));
        RETURNSTATE(SmAuthorization)
	}
    if(co.fAutoRunAssistModeOn)
    {
        nTimeOut = 0;
        nAutoListIndex = 0;
    }
    csCurContext = _T("SmAssistMode");  // Tar 238987
	if(!fStateAssist)
	{
		if (dm.fStateTakeawayBelt && ps.RemoteMode())
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmRemoteAssistMode") );
		else
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmAssistMode"));
	}

	//Tar 218158
	if (!ps.RemoteMode())
	   ra.RequestEnableAssistMode(false); 
	//Tar 218158

  //+dp185016
  if ( 2 == SMStateBase::m_nTBOutOfSyncError )
  {
    eo.Error(INFO,SM_CAT_LAN_WAN_COMM, SCOTAPP_TB_OUT_OF_SYNC_ASSISTMODE_RECOVER);
    SMStateBase::m_nTBOutOfSyncError = 0;

    //+tell rap to clear error
    trace(L6, _T("ra.OnNoRAPDataNeeded()"));
    ra.OnNoRAPDataNeeded();	
  //+ TAR 323659 Commented out these lines because m_View is already restored by earlier call to ra.OnNoRAPDataNeeded
		//Restoring again will cause data in m_View to be lost..
		
		// Restore the previous view
		//m_View = m_PrevView;
		//m_PrevView = DATANEEDED_NOVIEW; //tar 252987
		
		//-TAR 323659
    //-
  }
  //-dp185016
    // If Takeaway Belt, stops the belts on entry to this screen.
    //DMTakeawayBeltReset();


    m_bMultipleEndorsements = false;
	fStateAssist = true;
    bSetMonitorFalse = true; // TAR:209671
	g_bAllowRemoteBtn1 = true;		// RFC 209075 RJF 080202
    bAssistModeReturn = false;		// Tar 213624 RJF 090602
	ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
    if (fRequestForHelp || fCantScanCoupon || (lCouponsInAssist > 0))
    {
        fCantScanCoupon = false;
		fRequestForHelp = false;
		lCouponsInAssist = 0;
        trace(L7, _T("+ra.OnCallForHelpApproval"));
        ra.OnCallForHelpApproval();
        trace(L7, _T("-ra.OnCallForHelpApproval"));
    }
	if(gpSCOTAuxDisp)
    {
		gpSCOTAuxDisp->SetLanguage(SCOT_LANGUAGE_PRIMARY);
    }
    _TCHAR szBuff[100] = {0};
	_stprintf(szBuff, ps.GetPSText(MSG_SCP_LANENUM), TBGetTerminalNumber());

	// Rel 3.2 Requirement 4.5.1 - Single Mode RAP 
	// AM context could change with PS AM Feature. It's important check
	// and adjust the context name. When it is empty, this means we 
	// must be in local AM -   TAR 244640 
	bool bLocalAM = false;  // TAR 244946      
    if (g_csAssistModeStateName.IsEmpty())   
    {
        g_csAssistModeStateName  = _T("SmAssistMode");
        if (ps.RemoteMode())
        {
            CString csRet;
            if (PSX_SUCCESS == m_pAppPSX->GetCustomDataVar( _T("PartialAssistEnabled"), csRet, _T(""), _T("SmAssistModePartial")))
            {
                if (csRet == _T("True"))
                {
                    g_csAssistModeStateName = _T("SmAssistModePartial");
                    m_bPartialAssistEnabled = true;
                }
            }
        }
    }
    if( !ps.RemoteMode() )
    {
      bLocalAM = true;      // TAR 244946 
	}

    ps.SetFrame(g_csAssistModeStateName);  
	    
	
	// Rel 3.2 Requirement 4.5.1 - Single Mode RAP
    if(g_csAssistModeStateName == _T("SmAssistMode")) 
	{
	   ps.SetTitle(szBuff, ps.GetPSText(LTT_ASSISTMODE));
       if (co.IsInAssistMenus())
       {
           ps.Button(_T("SMButton1"), BTT_GOBACK, true);
       }
       else
       {
           ps.Button(_T("SMButton1"), BTT_GOTOSHOP, true);
       }
	}
    else
    {
        ps.SetTitle(_T(""), _T(""));
        if (co.IsInAssistMenus())
            ps.SetButtonText(_T("SMButton1"), BTT_GOBACK);
        else
            ps.SetButtonText(_T("SMButton1"), BTT_CLOSE);
	}
  //+dp185016 
  ps.Button(_T("SMButton2"), BTT_SYNC_POS_TB, true);
  //-dp185016

	tb.SetMonitorAll(true);
	if(fStartTrxInAM)	// RFC 228102 Initiate transactions that will require hand scanning at RAP
	{
		if(!ps.RemoteMode())	// shouldn't ever happen but log it if it does occur
		{
			trace(L6,_T("!!trying to start Transaction and enter AM in local AM -- s/w problem!\n"));
		}

		SASendSCOTInput( ITEMIZATION );
		trace(L7,_T("+ra.OnTransactionStart"));
		ra.OnTransactionStart();
		trace(L7,_T("-ra.OnTransactionStart"));
		trace(L7,_T("+ra.OnTransactionState, Itemization State"));
		ra.OnSetTransactionState(RA_STATE_ITEMIZATION);
		trace(L7,_T("-ra.OnTransactionState"));
        bIsTransactionStarted = true; //TAR219278 CHU 0110602: used in SmSMmediastatus 
		setAnchorState(BES_SCANANDBAG);

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

		static long rc;
		rc = tb.Start(co.language[SCOT_LANGUAGE_PRIMARY].szCode);  // use language code as szOverrideCode parameter
		ProcessTBReturn(rc);	//Process TB return value
        if(rc == TB_SUCCESS_IMMEDIATE)
		{
	      fInSignedOn  = true;		 // must be signed on by now
	      fInSignedOff = false;
		  SASendSCOTInput( TRANSACTION_CONTINUE );  //Tell security we are in transaction
	      if (TrainingModeObj.IsTMOn())
		  {  
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
	      fStartTrxInAM = false;
          trace(L6,_T("SMSmAssistModeBase::Started Transaction in AM"));
		}
        else
		{
          trace(L6,_T("SMSmAssistModeBase::Started Transaction in AM, TB_SUCCESS, waiting on TB_READY to update keyboard"));
		}
	}

	TBLoadAssistModeKeyboard();

	// moved this if loop from later in Initialize in order to get more done before
	// doing the ShowFrame - Tar 236311 RJF
	// Tar 213053 RAP AM neds to clear certain approvals like going into SM at lane
	if (ps.RemoteMode())
	{
		//Begin RFC 356020
		if (SMStateBase::nTenderType == TB_TENDER_CHECK_LANE_RAP)
			SMStateBase::nTenderType = TB_TENDER_CHECK;
		//End RFC 356020
		
		// remote approval
		if (lNotForSaleItems)
		{  // let ra know we approve recalled items from app
			trace(L7, _T("+ra.OnRecalledItemApproval"));
			ra.OnRecalledItemApproval();
			trace(L7, _T("-ra.OnRecalledItemApproval"));
		}

		if (lTimeRestrictedItems)
		{
			trace(L7, _T("+ra.OnTimeRestrictedItemApproval"));
			ra.OnTimeRestrictedItemApproval();
			trace(L7, _T("-ra.OnTimeRestrictedItemApproval"));
		}

		if (lQuantityRestrictedItems)
		{
			trace(L7, _T("+ra.OnQuantityRestrictedItemApproval"));
			ra.OnQuantityRestrictedItemApproval();
			trace(L7, _T("-ra.OnQuantityRestrictedItemApproval"));
		}
		if (fUnDeActivatedItem)
		{
			trace(L7, _T("+ra.OnUnDeActivatedItemApproval"));
			ra.OnUnDeActivatedItemApproval();
			trace(L7, _T("-ra.OnUnDeActivatedItemApproval"));
		}
		// We can NOT approve these here, because there is a separate screen
		// where these are approved (SMSmVisualItemsBase) so we can't approve
		// them twice, otherwise SCOTRA doesn't like it - TAR# 121732
#if 0
		// visual verify
		if (lVisualVerifyItems)
		{  // let ra know we approve recalled items from app
			trace(L7, _T("+ra.OnVisualValidationApproval"));
			ra.OnVisualValidationApproval();
			trace(L7, _T("-ra.OnVisualValidationApproval"));
		}
#endif

		if (lUnknownItems)
		{  // let ra know we approve unknown items from app
			trace(L7, _T("+ra.OnUnknownItemApproval"));
			ra.OnUnknownItemApproval();
			trace(L7, _T("-ra.OnUnknownItemApproval"));
		}
		if (lPriceRequiredItems)
		{  // let ra know we approve price required items from app
			trace(L7, _T("+ra.OnUnknownPriceApproval"));
			ra.OnUnknownPriceApproval();
			trace(L7, _T("-ra.OnUnknownPriceApproval"));
		}
  
		// if there is Other Payment pending approval, the approval should be cleared
		if (fOtherPayment)
		{  // let ra know that Other Payment has be cleared.
			trace(L7, _T("+ra.OnOtherPaymentApproval"));
			ra.OnOtherPaymentApproval() ;
			trace(L7, _T("-ra.OnOtherPaymentApproval"));
			//fOtherPayment = false ;    save this flag for outstanding approval printing
		}
	}	// Tar 213053 end

	else
	{
		//Begin RFC 356020
		if (SMStateBase::nTenderType == TB_TENDER_CHECK_LANE_RAP)
			SMStateBase::nTenderType = TB_TENDER_CHECK_LANE;
		//End RFC 356020
	}

	// TAR 234522 Disable Toggle button and lane status static when not in remote AM
	if (bLocalAM && co.nOperationsRemoteAssistModeToggleDisplay != 0)   // TAR 244946
	{
		m_pAppPSX->SetConfigProperty(_T("ButtonToggle"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);

        for (int i = 0; i < 10; i++)//TAR 355581 - changed i<8 to i<10 as now there can be at max 10 lanes
		{
			CString csTemp = EMPTY_STRING;
			csTemp.Format(_T("ButtonLaneStatus%d"), i);
            m_pAppPSX->SetConfigProperty(csTemp, csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
		}

	} // End TAR 234522
    else
    {
        // Since we are in remote mode, we need to ask RAP to send us the lane
        // status summary:
        // ra.OnAddOtherEvent(_T("COMMAND=AssistModeRefreshLaneStatus;")); //Obsolete w/RapNet
        // Also reset nTenderButtonIndex (Expect any kind of tender in remote Assist mode!)
        nTenderButtonIndex = -1;
    }

	if (ms_bKeepRemoteDevices)
	{
      trace (L6, _T("SMSmAssistModeBase::Initialize() Re-entering AssistMode State after keeping remote devices."));
      // We are coming back into AssistMode state from another remote state
      // for which we didn't unremote the devices, so there is no need to re-remote them.
      ms_bKeepRemoteDevices = false;
      //tar 228763 && 272625
      if(fInSignedOff && !bEnteredInitAfterToggle && dm.nDMLastReceiptFunction>0 && ((lBalanceDue<0) || (lBalanceDue==0 && isAnySoldVoidedItems())))
      {
         trace(L0, _T("-SMSmAssistModeBase::Initialize() Exiting and aborting Printing on RAP"));
         fWaitingToExit=true;
         dm.nDMLastReceiptFunction=0;
         nTimeOut = ASSISTMODE_RETURN_TIMEOUT;
		 return STATE_NULL;
      }
      trace(L0, _T("SMSmAssistModeBase::Initialize() Kept Remote Devices"));
	} // of if (ms_bKeepRemoteDevices)
	else
	{
		long nNewLevel = TB_KEYLOCK_N;   // Set to N key position by default.
		if (ps.RemoteMode())
		{
			SAWLDBScannerDisable();							  // disable SCOT scanner in remote mode
			TBShow2x20(ps.GetPSText(TXT_PLEASEWAIT));
			trace (L6, _T("SMSmAssistModeBase::Initialize() Remoting devices..."));

			dm.BeginRemote();
			dm.LoadPrintObjects();  //load bitmap to memory


			_TUCHAR szDispHi[25] = _T("");
			_TUCHAR szDisp[300] = _T("");
			_TUCHAR szDispLow[25] = _T("");

			tb.Get2x20((LPCTSTR)szDisp);
			memmove(szDispHi,szDisp,20*sizeof(_TCHAR));
			szDispHi[20]=0x00;
			CString hi(szDispHi);
			hi.TrimRight();
			memmove(szDispLow,&szDisp[20],20*sizeof(_TCHAR));
			szDispLow[20]=0x00;
			CString low(szDispLow);
			low.TrimRight();
			// one can do some 2x20 display msg specific processing here
			if (dm.IsDeviceRemote(DMCLASS_KEYLOCK, 0))
            {
				long lKeyPosition = dm.GetKeylockPosition();

				trace(L6,_T("+SMSmAssistModeBase::Initialize() Keylock is in the %s position."),
				lKeyPosition == DM_KEYLOCK_L ? _T("DM_KEYLOCK_L") :
				lKeyPosition == DM_KEYLOCK_N ? _T("DM_KEYLOCK_N") :
				lKeyPosition == DM_KEYLOCK_S ? _T("DM_KEYLOCK_S") :
				lKeyPosition == DM_KEYLOCK_X ? _T("DM_KEYLOCK_X") : _T("Unknown"));

				nNewLevel =       
				lKeyPosition == DM_KEYLOCK_L ? TB_KEYLOCK_L :   // Locked
				lKeyPosition == DM_KEYLOCK_N ? TB_KEYLOCK_N :   // Normal/Regular
				lKeyPosition == DM_KEYLOCK_S ? TB_KEYLOCK_S :   // Supervisor/Manager
				lKeyPosition == DM_KEYLOCK_X ? TB_KEYLOCK_X :0; // Exception (Diagnostic) 

            }
			else
            {
				trace (L0, _T("SMSmAssistModeBase::Initialize() Remote device Keylock failed"));
            }
			//For Japan and other countries/customers, it is possible to have an MSR without
			//having a keylock.
			if (dm.IsDeviceRemote(DMCLASS_MSR, 0))  // found the need of this condition while merging customer changes to Patch C
			{
				DMMSREnable();
			}
			else
            {
				trace (L0, _T("SMSmAssistModeBase::Initialize() Remote device MSR failed"));
            }
            if(co.fAutoRunAssistModeOn)
            {
                nTimeOut = 3;
            }
		}// end of if (ps.RemoteMode())  
		else
		{
			SAWLDBScannerEnable();
		} // else if (ps.RemoteMode())  

   tb.SetSecurityLevel(nNewLevel);
	} // else if (ms_bKeepRemoteDevices)
	// Reload the keyboard to reflect that we have remote devices.
	// we may be re-starting after a change error or after a paper empty DM event
	// change may need to be re-dispensed and variables still reset

	ps.ShowFrame();

	if (fWaitingForEOT)
	{
		//TAR195100+  CHU022502: Pop the states saved in the stack, and delete all saved states
		//Pop all SystemMessage states saved in statck
		CString csStateName = g_pDelay_TBExpectedStateChange->GetRuntimeClass()->m_lpszClassName;
		if (csStateName.Find(_T("TakeCard")) == -1)   //tar 317264
				ClearStateStackEOT();

		nTimeOut = ASSISTMODE_RETURN_TIMEOUT;
		ps.DisableButtons();
		return STATE_NULL;
	}
    if(!co.fAutoRunAssistModeOn)
    {
        nTimeOut = 0;
    }
	TBShow2x20(); 
	io.Reset(); 
	if (nDMCashInserted)
	{
		ASSERT(fStateAssist);
		trace (L0, _T("SMSmAssistModeBase::Initialize() - Cash was insert on the way to assist mode!"));
		SMStateBase  *retState  = TBEnterTender(TB_TENDER_CASH, nDMCashInserted);
		delete retState; // don't go to inprogress
		Sleep(1000); // allow host to respond to the tender
	}

	if( (ps.RemoteMode() || dm.IsCheckPaymentAtLaneAvailable()) 
        && (fInEndorsementState == ENDORSE_WAITING) )
	{
		trace (L6, _T("SMSmAssistModeBase::Initialize() Remoted and Endorsement waiting"));
        // NewUI
		COleVariant v;
        v = (long) UI::Disabled; // Disable
        m_pAppPSX->SetControlProperty(_T("SMButton1"), UI::PROPERTYSTATE, v);
        // End NewUI
		// fInEndorsementState = ENDORSE_INPROGRESS; this gets set in the OnEndorseDocument() call.
		dm.LoadPrintObjects();  //load bitmap to memory
		trace (L6, _T("-SMSmAssistModeBase::Initialize()"));
		return OnEndorseDocument();
	}

	//TAR222074+ CHU 12/09/02
	//The problem we had was at RAP, if we leave AM to SystemMessage with GoBack button is disabled and
	//come back to AM, GoBack button is enabled
	long lAMStatus = -1;
    tb.GetAssistModeStatus(&lAMStatus);
    if (lAMStatus == TB_AMSTATUSMAYNOTEXIT)
    {
      trace (L6, _T("SMSmAssistModeBase::Initialize() - Disabled GO BACK button."));

      // Rel 3.2 Requirement 4.5.1 - Single Mode RAP
      if(g_csAssistModeStateName == _T("SmAssistMode")) 
		ps.Button(_T("SMButton1"), BTT_GOBACK, false);
      else
	    ps.Button(_T("SMButton1"), BTT_CLOSE, false);

	  // Disable Done button on Assistance needed screen
	  g_bAllowRemoteBtn1 = false;				
    }
	//TAR222074-
	trace (L6, _T("-SMSmAssistModeBase::Initialize()"));
	return STATE_NULL;
} // of SMSmAssistModeBase::Initialize()



///////////////////////////////////
void SMSmAssistModeBase::UnInitialize()
{
  trace (L6, _T("+SMSmAssistModeBase::UnInitialize() "));
  // Synchronize the Store login with LaunchPad so that if the user turns the keyswitch to
  // Normal (supervisor mode), the signon status will match scotapp.

  // Fix 418408
  if ( ! IsStopping() )
  {
	SynchronizeStoreloginWithLaunchPad(_T("operation=set-signed-off")); // TAR 301706
  }

  m_nDMSavedDev = -1;	   // TAR 354355 - ji/40F-274 if remove cash acceptor in full AM then won't get err msg out of AM
  if (ps.RemoteMode() && (fDMReceiptError || fDMJournalError)) //HDMerge062502 Starts here- FFR
  {
    // allow state transition from Assistmode to system message
    // do not tell TB that we are leaving assistmode 
    // because once the device error is fixed, App returns back to Assistmode
    bSetMonitorFalse = false;
  } //HDMerge 062502 ends here
    if (!ms_bKeepRemoteDevices)
    {
	    bSetMonitorFalse = true; // TAR 407451 - This is to assist-release when SystemMessage on RAP was closed.
		
	    if ((m_bWaitingForMICRInsertion && dm.IsDeviceRemote(DMCLASS_MICR, 0) && dm.IsDeviceRemote(DMCLASS_RECEIPT, 0)) 
			|| m_bWaitingForMICRInsertion && !ps.RemoteMode() && dm.IsDeviceAvailable(DMCLASS_MICR))  //RFC 356020
        {
            ASSERT(FALSE);
            trace (L0, _T("SMSmAssistModeBase::UnInitialize() Called while waiting for MICR insertion.  MICR insertion canceled."));
            dm.MICRDisable();
        }
		m_bWaitingForMICRInsertion = false; 
		if(fInEndorsementState != ENDORSE_NONE)
		{
			do
			{
				delete m_szSlipDocumentDescription;
				delete m_szSlipDocumentFront;
				delete m_szSlipDocumentBack;
				
				m_szSlipDocumentDescription = NULL;
				m_szSlipDocumentFront = NULL;
				m_szSlipDocumentBack = NULL;
				tb.GetDocEndorsementInfo(&m_nEndorsementsPending,
                    &m_szSlipDocumentDescription,  &m_szSlipDocumentFront, &m_szSlipDocumentBack);

				if (m_nEndorsementsPending)
				{
					trace (L0, _T("SMSmAssistModeBase::UnInitialize() Printing endorsement data to a chit on the receipt station."));
					PrintEndorsementChit(m_szSlipDocumentBack);
					// Suppress further endorsement message from TB since we are doing these
					// in a loop and don't need to be event driven at this time.
					tb.EndorsementCompleted(true);
				}
			} while (m_nEndorsementsPending);
		}
        if (ps.RemoteMode())
        {
            trace (L6, _T("SMSmAssistModeBase::UnInitialize() Disconnecting remoted devices."));
			// TAR 249091 - need to hide these controls, otherwise, other lanes may see them for a short time remotely.
            m_pAppPSX->SetConfigProperty(_T("ButtonToggle"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
            for (int i = 0; i < 10; i++)//TAR 355581 - changed i<8 to i<10 as now there can be at max 10 lanes
			{
               CString csTemp = EMPTY_STRING;
	  	       csTemp.Format(_T("ButtonLaneStatus%d"), i);
               m_pAppPSX->SetConfigProperty(csTemp, csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
			}
            trace (L6, _T("SMSmAssistModeBase::UnInitialize() Disconnecting remoted devices."));

            dm.EndRemote();
        }
        TBShow2x20(ps.GetPSText(TXT_PLEASEWAIT));
    }
    else
    {
        if (InRemoteDeviceMode())
            trace (L6, _T("SMSmAssistModeBase::UnInitialize() Not disconnecting remoted devices.  We are planning to return to Assist Mode."));
    }
    if (bSetMonitorFalse)
    {
        tb.SetMonitorAll(false);
    }
    fStateAssist = false;
    trace(L2, _T("::UnInitialize - bEASReEnableScanner is set to true."));
    bEASReEnableScanner = true;
	// TAR 213074 - clear data needed flag if we're processing a remote tender
	if (g_bAssistModeDataNeeded)
	{
		ra.OnNoRAPDataNeeded();
		ClearSecurityViolationLight(); 
		g_bAssistModeDataNeeded = false;
	}
    //Reseting LGW only if SmartScale fires a weight event (in SMStateSABase.cpp)
    //That way app can remember if an error occurred before entering Store mode and process that error
    //ss.SetLastGoodWeight(0x8000000);		// TAR196221 what ever bag scale event happens in store mode should reset the smart scale LPM022002
    SAWLDBScannerDisable();
	if (co.fStateDualLanguage && m_customerLanguage != SCOT_LANGUAGE_PRIMARY)  
	{
		ps.SetLanguage(m_customerLanguage);
	}

	// only do on exit if Return to Shopping button selected and there is no security state saved in the stack
	// If there is a saved security state, by resetting LGW makes the state invalid CHU 041503
	if(bAssistModeReturn &&( sReturnState == STATE_NULL) )	
	{
        // Tar 213624 timing issue, putting something on smartscale 
        // and hitting ReturnToShopping button events close together,  RJF090402
#ifndef NewSec
        ss.SetLastGoodWeight(0x8000000);	
        ss.ResetTimers(SS_ALL_TIMERS);
#endif
	}

    // Rel 3.2 Requirement 4.5.1 - Single Mode RAP
    ps.Echo(_T(""),_T("")); // Must clear this 
    if (bSetMonitorFalse ) //We are going to system message screen
    {
		if (dm.fStateTakeawayBelt && ps.RemoteMode())
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmRemoteAssistMode") );
		else
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, csCurContext );
        if (!ps.RemoteMode())
        {
            TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAuthorization") );
        }
    }
    
   // Reset the Assist Mode key queueing filter since we're leaving 
   (*CPSXInterface::instance()).SetAssistKeyQueued(false);
    
   trace (L6, _T("-SMSmAssistModeBase::UnInitialize() "));
}

//////////////////////////////////
SMStateBase  *SMSmAssistModeBase::TimedOut()
{
trace(L6, _T("SMSmAssistModeBase::TimedOut"));

  if(m_bWaitForCashDrawerClose )  //RAP Dispense Change
  {
	  if(m_bIsCashDrawerClosed)
	  {
		  m_bWaitForCashDrawerClose = false;
		  nTimeOut = 0;  
		  return TBEndOfTransaction();  //Cash Drawer is closed, continue the transaction 
	  }
	  else
	  {
		  nTimeOut = 1;
		  //Cash Drawer is not closed, wait
          trace(L6, _T("Cash Drawer is still open at remote assitmode, wait until it is closed"));
		  return STATE_NULL;     
	  }
  }
 	  
  if(co.fAutoRunAssistModeOn && nAutoListIndex != -1)
  {
      return AutoRunTimeOut();
  }

  if (dm.fStateTakeawayBelt)
  {	// Tar 218330 RJF 120302
	if(fWaitingOnBelt)	// check if waiting for takeaway to send done event
		return STATE_NULL;
  }
  
  // if waiting after return key pressed...
  if (fWaitingToExit)
  {
    trace (L6, _T("+SMSmAssistModeBase::UnInitialize() "));
    
    // +TAR 386361 - migrated changes from E5
    if (co.IsInAssistMenus())
    {
        co.SetInAssistMenus(false) ;
    }
    // -TAR 386361

	tb.SetMonitorAll(false);
    //fStateAssist = false;  TAR#369813

	// need to keep these values for Outstanding Approvals Printing reset in PSButton2
	/*
		lNotForSaleItems = 0;
        lTimeRestrictedItems =0;
		lUnknownItems = 0;
		lUnknownItemsKeyed = 0;
		lPriceRequiredItems = 0;
		lRestrictedNotAllowedItems = 0;
        lQuantityRestrictedItems = 0;        */
		
		TBRefreshState();
    
		if (fInSignedOn)
		{
		  if (isAnySoldVoidedItems())
		  {
		 //   ts.Total();                  // need to get totals for e-rcpt
		  }
		}


	//TAR 410287 Need to set anchor state to Scan and Bag 
	if (getAnchorState() == BES_PUTBAGONSCALE)
	{
		setAnchorState(BES_SCANANDBAG);
	}


    //+dp185016 tar 286969
    if (getAnchorState() == BES_KEYINCODE)
      SMStateBase::setAnchorState( BES_SCANANDBAG );
		//-dp185016
		
		if (isAnyApprovalRequired())
		{
			if (lSAYellowNeedingApproval)
			{
				dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                            DM_TRICOLORLIGHT_ON,
                            DM_TRICOLORLIGHT_NORMALREQUEST);
			}

			//+TAR 288400 & 282921
			if(getAnchorState()==BES_ENTERQUANTITY) 
				setAnchorState(BES_SCANANDBAG);   // go back to ScanAndBag after approval
			//-TAR 288400 & 282921

			// back to wait for approval
			fStateAssist = false;	//TAR 389809 
			CREATE_AND_DISPENSE(WaitForApproval)(false,false);

		}

        // TAR 197015
		if (lSARedNeedingApproval)
		{
	      trace (L6, _T("-SMSmAssistModeBase::TimedOut() lSARedNeedingApproval = %d"),lSARedNeedingApproval);
			// set lane light back saved wt
	      DMSaySecurity( SEC_NONE, _T("blank.wav") );
	      dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                  DM_TRICOLORLIGHT_OFF,
                  DM_TRICOLORLIGHT_NORMALREQUEST);
	      dm.turnOffTriColorLight();

		  if (isAnyApprovalRequired())
	      {
		      dm.restoreTriColorLight();
	      }
	      else
	      {
		      // If inside the transaction, turn on the green light
		      if(fInSignedOn || fInSignedOff || fStateAssist || fSAInTransaction)
			      dm.setTriColorLight(DM_TRICOLORLIGHT_GREEN,
							       DM_TRICOLORLIGHT_ON,
							       DM_TRICOLORLIGHT_NORMALREQUEST);
	      }
		  // reset flags
          lSARedNeedingApproval = false;  //  Assist mode clears approval needed flag
		  g_bWtExpected = false;
		  g_lNumberOfWLDBEntriesThisItem = -1;	// reset this now because we're don with it
		  g_bMatchedWtWaitingOnEAS = false;
		  m_bEASHappened = false;

		  //+211695 
		  if (getAnchorState() == BES_START)
          {
             return TBStart(); // If coming from ATTRACT we need to Start TB
          }
	      else if (getAnchorState() == BES_FINISH 
               || (fInSignedOff && ((lBalanceDue<0) || (lBalanceDue==0 && isAnySoldVoidedItems())) )) //tar 272625
          {
		     return setAndCreateAnchorState(BES_FINISH);
          }
          else
          {
			 RETURNSTATE(ScanAndBag);
          }
		  //-211695 

      }
	   // END TAR 197015
		// don't show Deposit Coupons screen if coupon sensor is configured
		if (lCoupons && !IsUsingCouponSensor())
		{
			if (!bDropoffRequired && (getAnchorState() != BES_SCANANDBAG))
			{
				fStateAssist = false;
		        return DepositCouponsIfNecessary();
		    }
		}
        
		// LPM082401 TAR176732 In case a state was pushed onto the stack 
		//	while on WaitForApproval, Pop it off here so it's not popped on Attract
		if(!fStateAssist) //TAR#369813
		{
                       sReturnState = CkStateStackAndReturn();    //TAR213624 CHU 041503
		       if (sReturnState != STATE_NULL )
                           return sReturnState;
		}
                
                fStateAssist = false;



		if(fCancelSuspend)
		{
			fCancelSuspend = false;
			setAnchorState(nSellState);
		}
		// TAR 197015 START
	    if (getAnchorState() == BES_START)
		{
		   return setAndCreateAnchorState(BES_START);
		}
	    if (getAnchorState() == BES_FINISH || (fInSignedOff && ((lBalanceDue<0) || (lBalanceDue==0 && isAnySoldVoidedItems())) )) //tar 228763
		{
		   return setAndCreateAnchorState(BES_FINISH);
		}
		if (getAnchorState() == BES_ENTERCOUPONS)
		{
		   return setAndCreateAnchorState(BES_ENTERCOUPONS);
		}
		if ((getAnchorState() == BES_INSERTCOUPON) && bEnterTenderState) 
		{
		   return setAndCreateAnchorState(BES_ENTERCOUPONS);
		}
		//+degraded mode tars
		if( co.IsDegradedModeOn() && dm.GetDegradedModeType() != DEGRADEDMODE_OFF )
		{
		   if ( getAnchorState() == BES_DEGRADEDCONTINUETRANS )  //tar 418611
		   {
		      RETURNSTATE(DegradedContinueTrans)
		   }
		}
		//-degraded mode tars
		
		RETURNSTATE(ScanAndBag);

		//return createAnchorState();
		
  }

	
  if (fWaitingForEOT)
  {
      trace(L6, _T("SMSmAssistModeBase::TimedOut, dm.nDMLastReceiptFunction = %d, dm.nDMLastJournalFunction=%d"), 
      dm.nDMLastReceiptFunction, dm.nDMLastJournalFunction);
    // either we've just completed a transaction or
    // we're coming back into assist mode after getting
    // a DM event like paper empty or coin hopper jammed
		
    // we have to be very careful here to ensure all possible
    // DM events/errors have been handled before we wait for the
    // SA 161 because we wnat to be here in SMAssistMode when the
    // 161 event comes, not off on SystemMessage(0 etal.

    // wait for printing completion indication from DM
    // this variable is set whenever a write is done to the
    // SCOT printer. and is reset when DM issues an event
    // saying printing completed
    // as long as there is pending printing, wait...
    // this may cause DM errors and force a change to SystemMessage
    if ((dm.nDMLastReceiptFunction>0)  &&  (retryReceiptAttempts < 50))
    {
      retryReceiptAttempts += 1;
      return STATE_NULL;
    }
    else if(ps.RemoteMode())
    {
        return CkStateStackAndReturn();
    }
		
    // printing has completed, issue change
    // this may cause DM errors and force a change to SystemMessage
    SMStateBase * returnState;
    returnState = IssueChange();
    if (returnState != STATE_NULL)
      return returnState;
		
    // ok all done, now issue SA TRANSACTION_COMPLETE and wait for 161

		
    setAnchorState(BES_SCANANDBAG);


		
		fItemizing = false;
		fTendering = false;
		
    // wait for 161 event
    nTimeOut = TIMEOUT_SECURITY_WAIT_AT_THANKYOU;
		
    // wait for SA to issue 161 completion
    ps.SetLeadthruText(ps.GetPSText(LTT_ASSISTMODESECURITYWAIT));
		
    return STATE_NULL;
  }
	
  if (nTimeOut == TIMEOUT_SECURITY_WAIT_AT_THANKYOU)
  {
    return SAEndOfTransaction();
  }
	
  // should not timeout normally
  return STATE_NULL;                // ignore it
}

//////////////////////////////////////////
long SMSmAssistModeBase::DoKeyCode(UCHAR uKey)
{
	return 0;
}
 

///////////////////////////////////////////////////
SMStateBase  *SMSmAssistModeBase::PSAssistKey(const long nKey)// Assist Mode or Stop program
{
  trace(L0,_T("+Assist Mode %d"),nKey);
	



  TBKeyPress(TBGetKeyCode(nKey));
  TBShow2x20();
  return STATE_NULL;
}

/////////////////////////////////////////////////////
SMStateBase  *SMSmAssistModeBase::PSNumericKey(const long lKey)
{
  DoKeyCode((UCHAR)lKey);
  return STATE_NULL;
}

////////////////////////////////////
SMStateBase  *SMSmAssistModeBase::PSClearKey()
{
  return STATE_NULL;
}

////////////////////////////////////
SMStateBase  *SMSmAssistModeBase::PSEnterKey()// enter key pressed, sell this item
{
  return STATE_NULL;
}

/////////////////////////////////////
SMStateBase  *SMSmAssistModeBase::PSCancelKey()// cancel EXIT ASSIST MODE
{
    return PSButton1();
}

SMStateBase  *SMSmAssistModeBase::PSRemoteButton1(void)		// RFC 209075 RJF 07/31/02
{
	trace(L6, _T("SMSmAssistModeBase::PSRemoteButton1(), g_bAllowRemoteBtn1=%d"),g_bAllowRemoteBtn1);
	// check if it is legal to handle Done button at this time
//	if(g_bAllowRemoteBtn1)   // 'if' statement moved to PSButton1() for TAR 272492 
		return PSButton1();
//	else
//	{
//		return STATE_NULL;
//	}
}/////////////////////////////////////

/////////////////////////////////////
SMStateBase  *SMSmAssistModeBase::PSButton1()// return to shopping mode
{
	//Begin RFC 356020: Support Check payment on lane using printer 7167
	SMStateBase::nTenderType = TB_TENDER_NONE;
	//End RFC 356020

   // If we are in remote assist mode -- toggle RAP back to quad screen
   // so the operator doesn't have to wait for the whole remote cleanup operation
   // before returning to quad.
	
	// check if it is legal to handle "return to shopping mode" button at this time
	if(!g_bAllowRemoteBtn1)  // TAR 272492
	{
		return STATE_NULL;
	}

   // Rel 3.2 Requirement 4.5.1 - Single Mode RAP
   fWaitingToExit = true; 
   g_csAssistModeStateName.Empty();
   if (ps.RemoteMode())
   {
	 co.SetInAssistMenus(false);  //TAR 373896
     // Rel 3.2 Requirement 4.5.1 - Single Mode RAP
     if (g_csAssistModeStateName != _T("SmAssistMode") && !lUnapprovedRestrictedItems)  //Tar 247059
       ra.RequestEnableAssistMode(false);   
     PSButtonToggle();
   }

   // Tar 218330 RJF 120302
   if (dm.fStateTakeawayBelt && !co.IsSecurityBypassOn())
   {
      TABExitingAssistMode();
      fWaitingOnBelt = true;
   }
   else
   {
	  ps.SetTitle(ps.GetPSText(LTT_ASSISTMODERETURN), ps.GetPSText(PS_BLANK));
   }

  nTimeOut = ASSISTMODE_RETURN_TIMEOUT;
  fOtherPayment = false ; //Tar #158619	reset here so we can return to shopping if needed
  lNotForSaleItems = 0;
  lTimeRestrictedItems =0;
  lUnknownItems = 0;
  lUnknownItemsKeyed = 0;
  lPriceRequiredItems = 0;
	if(lRestrictedNotAllowedItems)
	{
		lRestrictedNotAllowedItems = 0;
        trace(L7, _T("+ra.OnRestrictedNotAllowedApproval"));
        ra.OnRestrictedNotAllowedApproval();
        trace(L7, _T("-ra.OnRestrictedNotAllowedApproval"));
	}
  lQuantityRestrictedItems = 0;
  fUnDeActivatedItem = false;
  bAssistModeReturn = true;			// Tar 213624 RJF 090602
  bStoreModeApprovals = false;
  bStoreModeGoBack = false; 
  
  // Begin TAR 197558
    if (fRequestForHelp || fCantScanCoupon || (lCouponsInAssist > 0))
    {
        fCantScanCoupon = false;
		fRequestForHelp = false;
		lCouponsInAssist = 0;
        trace(L7, _T("+ra.OnCallForHelpApproval"));
        ra.OnCallForHelpApproval();
        trace(L7, _T("-ra.OnCallForHelpApproval"));
    }
  // + TAR 410334 	
  bDropoffApproved = true;
  bSuspendFailure  = false;
  bSuspendApproved = true;
  lCouponsInAssist = 0;
  // - TAR 410334 	

  // Tar 213044 RJF 03/06/03 non-action required approvals cleared above
  // but there could still be action required approvals pending so check
  // and restore trilight
  if (isAnyApprovalRequired())
  {
      dm.restoreTriColorLight();
  }
  else
  {
	  //TAR 208746 BZ 07/12/02 begin: Turn off security light if no approval is needed
	  // If inside the transaction, turn on the green light
	  if(fInSignedOn || fInSignedOff || fStateAssist || fSAInTransaction)
	  {
		  dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
			  DM_TRICOLORLIGHT_OFF,
			  DM_TRICOLORLIGHT_NORMALREQUEST);
		  dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
			  DM_TRICOLORLIGHT_OFF,
			  DM_TRICOLORLIGHT_NORMALREQUEST);
		  dm.turnOffTriColorLight();

		  if (dm.GetPrinterPaperLow())
		  {
			  dm.setTriColorLight(DM_TRICOLORLIGHT_GREEN,
								DM_TRICOLORLIGHT_BLINK_1HZ,
								DM_TRICOLORLIGHT_NORMALREQUEST);
		  }
		  else
		  {
			dm.setTriColorLight(DM_TRICOLORLIGHT_GREEN,
						   DM_TRICOLORLIGHT_ON,
						   DM_TRICOLORLIGHT_NORMALREQUEST);
		  }
	  }
	  //TAR 208746 end  LPM071202 moved to button2 press because this is where all the flags are set to zero
  }
  // End TAR 197558
  
  //now that we're returning to the transaction, use the customer's selected language
  //ps.SetLanguage(m_customerLanguage);	//Tar 424797 - Do not set customer language here.
  if(gpSCOTAuxDisp)
  gpSCOTAuxDisp->SetLanguage(m_customerLanguage);
  // Wait for timeout to change states.
  return STATE_NULL;
}

///////////////////////////////////
SMStateBase  *SMSmAssistModeBase::PSButton2()
{
  //+dp185016 
  trace(L6, _T("+PSButton2"));
  TBSyncTBorPOS();
  trace(L6, _T("-PSButton2"));
  //-dp185016
    return STATE_NULL;
}

///////////////////////////////////
SMStateBase  *SMSmAssistModeBase::DMScanner()// scanned item
{
  if (!bIsTransactionStarted)  // TAR 241351
     return STATE_NULL;

  
  csItemSent = csDMLastBarCode;
  io.Reset();
	
  io.csOrigCode = csDMLastBarCode;
  io.csItemCode = csDMLastBarLabel;
  io.cScanType  = csDMLastBarType;

  io.lTareCode  = lTareCode;
  io.lWeightEntered = lWeightEntered;
  io.lUnitPriceEntered = lUnitPriceEntered;
  io.lExtPriceEntered = lExtPriceEntered;   //Tar 137876 add extended price
  io.lQuantityEntered = lQuantityEntered;
  io.fVoid = fVoided;
		
  if (!fItemizing)
  {
 	 //SASendSCOTInput(ITEMIZATION );	TAR372438
	 fItemizing = true;
	 fTendering = false;
  }
		
  fVoided = false;
  lTareCode = 0;
  lWeightEntered = 0;
  lQuantityEntered = 0;
  lUnitPriceEntered = 0;
  lExtPriceEntered = 0;
  lSlashData = 0;
  csNumData = _T("");
		
  // If not in remote mode, reenable scanner
  if (!ps.RemoteMode())
  {
	  trace(L2, _T("::DMScanner, if ps.RemoteMode() = false, - bEASReEnableScanner is set to true."));
	  bEASReEnableScanner = true;
	  SAWLDBScannerEnable();
  }
  return TBItemSale(false,0,true);
 
}

// something on scale
SMStateBase  *SMSmAssistModeBase::DMScale()
{
  // let the base prime the 'virtual' scale
	SMStateBase  *sExit = SMStateBase ::DMScale();
	if (sExit!=STATE_NULL)
		return sExit;

	long lCurrentWeight = SMStateBase::lDMScaleWeight;
	TBSTATE nCurrentTBState = SMStateBase::nTBLastRealState;

	if (lCurrentWeight == 0)
		return STATE_NULL;	// shakey scale, wait for good weight

	if ((nCurrentTBState == TB_NEEDWEIGHT) && (lCurrentWeight > 0))
	{
        TBRC rc = tb.SendWeight(lCurrentWeight);
   		ProcessTBReturn(rc);
	}
   return STATE_NULL;
} 


////////////////////////////////////////////
SMStateBase  *SMSmAssistModeBase::IssueChange(void)
{
  // if change due issue it
  if (lChangeDue>0)
  {
    CString csCashCountsStatusCheck;
	long rc = DMDispenseChange(lChangeDue, csCashCountsStatusCheck);
    if (rc>0) lChangeDue -= rc;
		
    // if not enough change was issued, tell user
    if (lChangeDue>0)
    {
  	  fStateAssist = false;

	  // Need to distinguish coin or bill error.   //TINA
      //FZ Error Handling
      CString csWork = GetDeviceErrorMnemonic(DMCLASS_CASHCHANGER, dm.GetDeviceName(DMCLASS_CASHCHANGER, 0), EVENTID_ANY, dm.GetResultCodeExtended(DMCLASS_CASHCHANGER, 0), nDeviceErrorContext );
      if(csWork.Find(_T("DEVICEERRORUNKNOWN")) != -1)  // Did not find the extended result code for this error
		     csWork = CashChangerErrorMessage(lChangeDue, csCashCountsStatusCheck);
	  
	  // Added for TAR 191990

	CString csTitle1, csTitle2, csMessage, csMovie;
	BMPS bitmap;
	PSTEXTWAVE nWave;
    ProcessDeviceErrorXMLFile( csWork, csTitle1, csTitle2, csMessage, bitmap, nWave, csMovie);


    //Notify RAP
    trace(L7, _T("+ra.OnRAPDataNeeded, ErrorMessage=%s"), csWork);
    // write the error description on the receipt
     if(csTitle1.IsEmpty())
	{
		ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csWork);
	}
	else
	{
		ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csTitle1);
	}

    // save the previous view
	m_PrevView = m_View;

  // send a DataNeeded Approval
  cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("DeviceFailure"));
  m_View = DATANEEDED_DEVICEERROR;
  CString csSecInfo;
 	if(csTitle1.IsEmpty())
	{
		csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),csWork,ps.GetPSText(BUTTON_RA_DEVICE_ERROR, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""), _T(""), _T(""));
	}
	else
	{
		csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),csTitle1 + _T("\n") + csTitle2 ,ps.GetPSText(BUTTON_RA_DEVICE_ERROR, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""), _T(""), _T(""));
	}
	
	ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
  trace(L7, _T("-ra.OnRAPDataNeeded"));
  // End for TAR 191990

      // when this returns, we will return to this state at Initialize()
	  return createLevel1Message(csWork,           // char *message
                                 PS_NOTEXT,        // no canned message
                                 PS_NOTEXT,        // no prompt
                                 0,                // no value
                                 DMCLASS_CASHCHANGER,// device reporting the error or -1
                                 true);            // T/F need to issue CheckHealth after operator clears problem
    }
  }
  return STATE_NULL;
}

////////////////////////////////////////////
SMStateBase  *SMSmAssistModeBase::SAEndOfTransaction()// the all important 161
{
  ResetAllTransactionValues();
	
   trace (L6, _T("+SMSmAssistModeBase::SAEndOfTransaction()"));		
  // back to selling mode for sec. agent
  SASendSCOTInput(ITEMIZATION );
	
	fItemizing = true;
	fTendering = false;
  fWaitingForEOT = false;
  return Initialize();
}

// Assist mode may be run legally from the RAP station
bool SMSmAssistModeBase::PSRemoteLegalForState()
{
	return true;
}


/////////////////////////////////////////
// DMCardReader - card inserted
/////////////////////////////////////////
SMStateBase *SMSmAssistModeBase::DMCardReader(void)
{
  DisplayAuxScreen(AUX_SCREEN_SB_PREVSWIPE, true);
  // ignore these parameters, just pass the MSR data to TB
  tb.EnterTender(TB_TENDER_NONE,0,cDMLastTrack,csDMEncryptedPin.GetLength(),csDMEncryptedPin,0,0);
  return STATE_NULL;
}

SMStateBase *SMSmAssistModeBase::DMMICR(DWORD dwEvent)
   {
   trace (L6, _T("+SMSmAssistModeBase::DMMICR(0x%08.8x)"), dwEvent);
   m_nMICREvents++;
   switch(dwEvent)
      {
      case DM_DATA:
         ASSERT(m_bWaitingForMICRInsertion == TRUE);
         if (m_bWaitingForMICRInsertion)
            {
            m_bWaitingForMICRInsertion = FALSE;
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

            trace (L6, _T("SMSmAssistModeBase::DMMICR() data event: sRawData=<%s> ")
               _T("sTransitNumber=<%s> sAccountNumber=<%s> sBankNumber=<%s> ")
               _T("sCheckNumber=<%s> sEPC=<%s> nCheckType=%d nCountryCode=%d"),
               sRawData, sTransitNumber, sAccountNumber, sBankNumber, 
               sCheckNumber, sEPC, nCheckType, nCountryCode);
			
			//Begin RFC 356020
			if (SMStateBase::nTenderType == TB_TENDER_CHECK_LANE)
				tb.EnterMICRWithType(SMStateBase::nTenderType, sRawData, sTransitNumber, sAccountNumber, sBankNumber, 
               sCheckNumber, sEPC, nCheckType, nCountryCode);
			else
				tb.EnterMICR(sRawData, sTransitNumber, sAccountNumber, sBankNumber, 
               sCheckNumber, sEPC, nCheckType, nCountryCode);
			//End RFC 356020
			
			TBShow2x20(); 
			dm.MICRDisable();
            break;
            }
         trace (L0, _T("SMSmAssistModeBase::DMMICR() Unexpected MICR Data event received."));
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
         m_bWaitingForMICRInsertion = FALSE;
         dm.MICRDisable();
		 //merged TAR233401 to 3.1.1 - fixed by Chen - Tell TB about this bad MICR
		 tb.EnterMICR(_T(""), _T(""), _T(""), _T(""),_T(""),_T(""), 0,0);  
  		 TBShow2x20(); 

         trace (L0, _T("SMSmAssistModeBase::DMMICR() Fatal MICR error event received -- not retrying MICR read."));
         break;
      default:
         ASSERT(FALSE);
      }

   trace (L6, _T("-SMSmAssistModeBase::DMMICR()"));
   return STATE_NULL;
   }

SMStateBase *SMSmAssistModeBase::OnNeedMICR()
   {
   trace (L6, _T("+SMSmAssistModeBase::OnNeedMICR()"));
   m_bWaitingForMICRInsertion = TRUE;
   LPTSTR pMICRPrompt = NULL;
   tb.GetMICRPrompt(&pMICRPrompt);
   trace(L0, _T("TBGetMICRPrompt(%s)"), pMICRPrompt ? pMICRPrompt : _T("<Empty>"));
   
   if ((dm.IsDeviceRemote(DMCLASS_MICR, 0)  && dm.IsDeviceRemote(DMCLASS_RECEIPT, 0) ) 
	   || SMStateBase::nTenderType == TB_TENDER_CHECK_LANE || (dm.IsDeviceAvailable(DMCLASS_MICR)  && !ps.RemoteMode())) //RFC 356020: Support Check payment on lane using printer 7167
      {
		  trace (L6, _T(" SMSmAssistModeBase::OnNeedMICR() - Enabling MICR insertion."));
		//      ps.Button(IDC_BUTTON1, BTT_GOBACK, false);  //tar 205586
		  if (!ps.RemoteMode() && dm.IsCheckPaymentAtLaneAvailable())
				SMStateBase::nTenderType = TB_TENDER_CHECK_LANE;
		  else if (dm.IsDeviceRemote(DMCLASS_MICR, 0)  && dm.IsDeviceRemote(DMCLASS_RECEIPT, 0) )
				SMStateBase::nTenderType = TB_TENDER_CHECK;

		  if (pMICRPrompt)
		  {
			dm.MICREnable();
 		  }
		  else
 		  {
			m_bWaitingForMICRInsertion = FALSE;
			dm.MICRDisable();
 		  }
      }
   else
      {
	   	//RFC 356020
      	trace (L0, _T(" SMSmAssistModeBase::OnNeedMICR() - Application wants MICR data, but we are not in RemoteDeviceMode or paying check on lane."));
      }
      
   trace (L6, _T("-SMSmAssistModeBase::OnNeedMICR()"));
   delete pMICRPrompt;
   return STATE_NULL;
   }

SMStateBase *SMSmAssistModeBase::OnEndorseDocument()
   {
   trace (L6, _T("+SMSmAssistModeBase::OnEndorseDocument()"));
   fInEndorsementState = ENDORSE_INPROGRESS;
   delete m_szSlipDocumentDescription;
   delete m_szSlipDocumentFront;
   delete m_szSlipDocumentBack;
   m_szSlipDocumentDescription = NULL;
   m_szSlipDocumentFront = NULL;
   m_szSlipDocumentBack = NULL;

   ::tb.GetDocEndorsementInfo(&m_nEndorsementsPending,
      &m_szSlipDocumentDescription,  &m_szSlipDocumentFront, &m_szSlipDocumentBack);
   if (!m_nEndorsementsPending)
   {
    fInEndorsementState = ENDORSE_NONE;
    DMSlipPrinterInsertSlip(FALSE);
    trace (L6, _T("-SMSmAssistModeBase::OnEndorseDocument()"));
    return STATE_NULL;
   }

   if (m_szSlipDocumentDescription)
      trace (L6, _T(" SMSmAssistModeBase::OnEndorseDocument() : Description: <%s>"), m_szSlipDocumentDescription);
   if (m_szSlipDocumentFront)
      trace (L6, _T(" SMSmAssistModeBase::OnEndorseDocument() : Document front: <%s>"), m_szSlipDocumentFront);
   if (m_szSlipDocumentBack)
      trace (L6, _T(" SMSmAssistModeBase::OnEndorseDocument() : Document back: <%s>"), m_szSlipDocumentBack);

   if (dm.IsDeviceRemote(DMCLASS_SLIP, 0) && dm.IsDeviceRemote(DMCLASS_RECEIPT, 0))
      {
      TBShow2x20(m_szSlipDocumentDescription);
      // Handle the case where the wrong document could be in the printer...
      // Multiple endorsements after check tender.
	  if ((m_nEndorsementsPending > 1) && (false == m_bMultipleEndorsements))
	  {
		  // Multiple Endorsements are being processed
		  m_bMultipleEndorsements = true;
	      if ( m_nMICREvents && DMSlipPrinterIsSlipInserted())
		  {
			  DMSlipPrinterInsertSlip(false);
		  }
	  }
	  else if (true == m_bMultipleEndorsements)
	  {
		  // This is put in due to some shenanigans the OLE driver is pulling.  We need
		  // to "double clutch" the slip printer to ensure that the last document is
		  // removed so that the second endorsement will print properly
		  DMSlipPrinterInsertSlip(false);
	  }

      // Open the throat and wait for a slip
      DMSlipPrinterInsertSlip(true);
      }
   else
      {
      trace (L0, _T(" SMSmAssistModeBase::OnEndorsedocument() - Application wants to print endorsement, but we are not is RemoteDeviceMode."));
      trace (L0, _T(" SMSmAssistModeBase::OnEndorseDocument() - Printing endorsement data to a chit on the receipt station."));
      // Prohibit multiple chits per endorsement.
      m_nEndorsementsPending = 0;
      PrintEndorsementChit(m_szSlipDocumentBack);
      tb.EndorsementCompleted();
      }

   trace (L6, _T("-SMSmAssistModeBase::OnEndorseDocument()"));
	return STATE_NULL;
   }

SMStateBase *SMSmAssistModeBase::DMSlipPrinter(DWORD dwEvent)
   {
   trace (L6, _T("+SMSmAssistModeBase::DMSlipPrinter(0x%08.8x)"), dwEvent);

   switch(dwEvent)
      {
      case DM_PTR_PAPEROK:
         trace (L6, _T("+SMSmAssistModeBase::DMSlipPrinter() Slip inserted."));
         // Document has been inserted, so print.
         if (m_nEndorsementsPending)
            DMSlipPrinterPrint(m_szSlipDocumentBack);
         break;
      case DM_PTR_REQUEST_COMPLETED :
         trace (L6, _T("SMSmAssistModeBase::DMSlipPrinter() Endorsement <%s> completed."), m_szSlipDocumentDescription);
         // Successfully completed printing the document.
         // Eject the slip.
         fInEndorsementState = ENDORSE_NONE;
         DMSlipPrinterInsertSlip(FALSE);

		 //TAR257468+
         //We need to clear RAP view if only if 
		 if ( m_View == DATANEEDED_ENDORSEDOCUMENT )
		 {
			 ra.OnNoRAPDataNeeded();
		 }
		 //TAR257468-

         // Tell TB the slip was printed.
         tb.EndorsementCompleted();
         break;
      case DM_PTR_EMPTY:
         trace (L4, _T("SMSmAssistModeBase::DMSlipPrinter() slip empty."));
         break;
      case DM_PTR_NEAREMPTY:
         trace (L4, _T("SMSmAssistModeBase::DMSlipPrinter() slip near empty."));
         break;
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
      default:
         trace (L0, _T("SMSmAssistModeBase::DMSlipPrinter() Error printing slip."));
         if (m_nEndorsementsPending)
            {
            trace (L0, _T("SMSmAssistModeBase::DMSlipPrinter() Printing endorsement data to a chit on the receipt station."));
            // Prohibit multiple chits per endorsement.
            m_nEndorsementsPending = 0;
            PrintEndorsementChit(m_szSlipDocumentBack);
            tb.EndorsementCompleted();
            }
         break;
      }
   trace (L6, _T("-SMSmAssistModeBase::DMSlipPrinter(0x%08.8x)"), dwEvent);
   return STATE_NULL;
   }

SMStateBase *SMSmAssistModeBase::PSAlphaKey(const long lKeystroke)
{
    trace (L6, _T("+SMSmAssistModeBase::PSAlphaKey(0x%08.8x)"), lKeystroke);
    SMStateBase::PSAlphaKey(lKeystroke);
    trace (L6, _T("-SMSmAssistModeBase::PSAlphaKey()"));
    return STATE_NULL;
}

SMStateBase *SMSmAssistModeBase::PSKeystroke(const long wParam, const long lParam)
{
    trace (L6, _T("+SMSmAssistModeBase::PSKeystroke(0x%08.8x, 0x%08.8x)"), wParam, lParam);
    SMStateBase* retState = SMStateBase::PSKeystroke(wParam, lParam);
    return retState;
}


SMStateBase *SMSmAssistModeBase::OnNewCashDrawerState()
   {
    long lDrawerState;
    tb.GetExpectedCashDrawerState(&lDrawerState);

   if (dm.IsDeviceRemote(DMCLASS_CASHDRAWER, 0))
      {
      if (lDrawerState & 1)
         {
         // Open Drawer here...
         dm.CashDrawerOpen();
         }
      else
         {
         // Prompt for drawer close here...
         }
      }
   else
      {
      // There is no drawer available -- just tell TB what it wants to hear.
       tb.SetCashDrawerState(lDrawerState);
      }

   return STATE_NULL;
   }

TBRC  SMSmAssistModeBase::TBKeyPress(const USHORT uKey)
{
    TBRC rc = -1; 
    if (uKey)
    {
        if (m_bWaitingForMICRInsertion)
        {
// if the key is clear, how do we know on SCOT side?
// waiting for MICR but got a clear key, leave waiting for MICR state
//            m_bWaitingForMICRInsertion = FALSE;
//            dm.MICRDisable( );
        }
        rc = tb.KeyPress(uKey);
    }
    return rc;
}

SMStateBase  *SMSmAssistModeBase::OnAssistModeStatusChanged(void)
   {
   long lAMStatus = -1;
   trace (L6, _T("+SMSmAssistModeBase::OnAssistModeStatusChanged()"));

   tb.GetAssistModeStatus(&lAMStatus);

   if (lAMStatus == TB_AMSTATUSMAYEXIT)
      {
      trace (L6, _T("SMSmAssistModeBase::OnAssistModeStatusChanged() - Enabled GO BACK button."));
      // Rel 3.2 Requirement 4.5.1 - Single Mode RAP
      if(g_csAssistModeStateName == _T("SmAssistMode")) 
	    	  //TAR410329+
	  {
		// ps.SetButtonText(_T("SMButton1"), BTT_GOBACK);
		if (co.IsInAssistMenus())
        {
		   ps.SetButtonText(_T("SMButton1"), BTT_GOBACK);
        }
        else
        {
           ps.SetButtonText(_T("SMButton1"), BTT_GOTOSHOP);
        }
	  }
      //TAR410329-
      else
      {
        if (co.IsInAssistMenus())
            ps.SetButtonText(_T("SMButton1"), BTT_GOBACK);
        else
            ps.SetButtonText(_T("SMButton1"), BTT_CLOSE);
      }
     
	  // Enable Done button on Assistance needed screen
	  g_bAllowRemoteBtn1 = true;				// RFC 209075 RJF 080202
      }
    else
    {
        trace( L6, _T("SMSmAssistModeBase::OnAssistModeStatusChanged() - Disabled SMButton1.") );
        m_pAppPSX->SetConfigProperty( _T("SMButton1"), ps.GetCurrentContext(), UI::PROPERTYSTATE, (long)UI::Disabled );
		// +TAR 277831 & 277823
        if ( (nTenderType == TB_TENDER_CASH_RAP) && (lBalanceDue == 0) ) //TAR 277989
		{
			ps.DisableAssistModeKey(_T("Key1"), -1);
			ps.DisableAssistModeKey(_T("Key16"), -1);
			ps.DisableAssistModeKey(_T("Key18"), -1);
			ps.DisableAssistModeKey(_T("Key27"), -1);
			ps.DisableAssistModeKey(_T("Key28"), -1);
			ps.DisableAssistModeKey(_T("Key29"), -1);
			ps.DisableAssistModeKey(_T("Key49"), -1);   
		}
		ra.OnAddOtherEvent(_T("COMMAND=DisableScanner;"));
		DMScannerDisable();
		// -TAR 277831 & 277823
	    // Disable Done button on Assistance needed screen
	    g_bAllowRemoteBtn1 = false;				// RFC 209075 RJF 080202
    }

    trace( L6, _T("-SMSmAssistModeBase::OnAssistModeStatusChanged() - returning STATE_NULL") );
    return STATE_NULL;
}

SMStateBase *SMSmAssistModeBase::PSButtonToggle(void)
{
    trace (L6, _T("SMSmAssistModeBase::PSButtonToggle()."));
    //   ra.OnAddOtherEvent(_T("COMMAND=AssistModeToggleToMain;"));
    CString csRemoteConnectionName = ra.GetRemoteConnectionName();
    // Rel 3.2 Requirement 4.5.1 - Single Mode RAP
    if (!fWaitingToExit)
    {
        if (m_bPartialAssistEnabled)
        {
            g_csAssistModeStateName = _T("SmAssistModePartial");
            HandleNewContext();
        }
        
        m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), _T("operation=release;"), csRemoteConnectionName );
        m_bToggled = true; //TAR 272625
    }
    else
    {
        if (!co.IsInAssistMenus())
        {
            m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), _T("operation=release;"), csRemoteConnectionName );
        }
    }
    return STATE_NULL;
}

// Rel 3.2 Requirement 4.5.1 - Single Mode RAP
SMStateBase *SMSmAssistModeBase::HandleNewContext(void) 
   {
   trace (L6, _T("SMSmAssistModeBase::HandleNewContext()."));
   ms_bKeepRemoteDevices = true;
   return this->Initialize(); 
   }

SMStateBase *SMSmAssistModeBase::PSButtonLaneStatus(long lLaneIndex)
   {
   trace (L6, _T("SMSmAssistModeBase::PSButtonLaneStatus(%d)."), lLaneIndex);
   return STATE_NULL;
   }

SMStateBase *SMSmAssistModeBase::OnRAPUnsolicitedDataReceived(LPCTSTR sData)
   {
   trace (L6, _T("+SMSmAssistModeBase::OnRAPUnsolicitedDataReceived(%s)."), sData);
/*   
   class CSimpleStringContainer : public CNamedStringContainer 
   {
   public: CSimpleStringContainer(CString s = _T("")){FromString(s);}
   public: CString ExtractString(CString s){return CNamedStringContainer::ExtractString(s);}
   protected: bool FromContents(void){return true;}
   }
   ssc(sData);
*/
   if (co.nOperationsRemoteAssistModeToggleDisplay)
   {
      if(ExtractString(sData, _T("COMMAND")) == _T("AllLaneStatusSummary"))
      {
         CString sLane, sStatus, sDisplay, sTemp, sToggleButtonText;
         sToggleButtonText = ps.GetPSText(BTT_ASSISTMODETOGGLE);
         COleVariant v;
		 v = VARIANT_TRUE;
         ps.Button(_T("ButtonToggle"), sToggleButtonText, true);
		 m_pAppPSX->SetConfigProperty(_T("ButtonToggle"), csCurContext, UI::PROPERTYVISIBLE, v);
        
         // 8 Is the number of flexbuttons in the control for lane status.
         for (int i = 0; i < 10; i++)//TAR 355581 - changed i<8 to i<10 as now there can be at max 10 lanes
            {
            sDisplay.Empty();
            sTemp.Format(_T("LANE%d"), i);
            sLane = ExtractString(sData, sTemp);
            sTemp.Format(_T("STATUS%d"), i);
            sStatus = ExtractString(sData, sTemp);

            switch(co.nOperationsRemoteAssistModeToggleDisplay)
            {
               case 1: // Lane Only -- but only if there is a status.
                  if (!sStatus.IsEmpty())
                     sDisplay = sLane;
                  break;
               case 2: // Status Only
                  sDisplay = sStatus;
                  break;
               case 3: // Lane and Status
                  if (!sLane.IsEmpty() || !sStatus.IsEmpty()) //TAR 208319
                  sDisplay.Format(_T("%s : %s"), sLane, sStatus);
                  break;
               case 0: // Nothing
               default:
                  break;
            }
            if (!sDisplay.IsEmpty())
			{
				COleVariant v;
				v =	sDisplay;
				CString csTemp = EMPTY_STRING;
				csTemp.Format(_T("ButtonLaneStatus%d"), i);
                m_pAppPSX->SetConfigProperty(csTemp, csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
                m_pAppPSX->SetConfigProperty(csTemp, csCurContext, UI::PROPERTYTEXTFORMAT, v);
			}
            
         }
         trace (L6, _T("-SMSmAssistModeBase::OnRAPUnsolicitedDataReceived() -- Updated lane status display."));
         return STATE_NULL;
      } 
   }
   else  // TAR 227293- When options are reloaded, visible controls must be reset.
   {
      trace (L6, _T("-SMSmAssistModeBase::OnRAPUnsolicitedDataReceived() -- Hide toggle and lane status display."));
  	  m_pAppPSX->SetConfigProperty(_T("ButtonToggle"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
      for (int i = 0; i < 10; i++)//TAR 355581 - changed i<8 to i<10 as now there can be at max 10 lanes
      {
         CString csTemp = EMPTY_STRING;
	  	 csTemp.Format(_T("ButtonLaneStatus%d"), i);
         m_pAppPSX->SetConfigProperty(csTemp, csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
      }
   }

   if(ExtractString(sData, _T("operation")) == _T("toggle"))
   {
       trace(L6, _T("Received toggle notification"));
       CString csDest = ExtractString(sData, _T("destination"));
       if ( csDest == _T("main"))
       {
           m_bToggled = true;
           if (m_bPartialAssistEnabled)
           {
               g_csAssistModeStateName = _T("SmAssistModePartial");
               HandleNewContext();
           }
           CString csRemoteConnectionName = ra.GetRemoteConnectionName();
           m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), _T("operation=release;"), csRemoteConnectionName );
       }
       else if ( csDest == _T("assistmode"))
       {
           if (!m_bToggled && (g_csAssistModeStateName == _T("SmAssistModePartial")))
           {  //We are sitting at partial assist screen
               g_csAssistModeStateName = _T("SmAssistMode");
           }
           m_bToggled = false;
           HandleNewContext();
       }
   }
   trace (L6, _T("-SMSmAssistModeBase::OnRAPUnsolicitedDataReceived() -- Calling base..."));
   return SMStateBase::OnRAPUnsolicitedDataReceived(sData);
   }

// Tar 218300 changed how TAB works in AM
// this function gets called when TAB sends done event
SMStateBase *SMSmAssistModeBase::DMTakeawayAMExitDone()
{
    trace(L6, _T("+DMTakeawayAMExitDone"));

    if (dm.fStateTakeawayBelt)
    {
        trace(L6, _T("Takeaway Belt present: assist mode exit done, returning to base state"));
		fWaitingOnBelt = false;
    }
    else
    {
        trace(L7, _T("Takeaway Belt not present"));
    }

    trace(L6, _T("-DMTakeawayAMExitDone"));
	return STATE_NULL;
}

/////////////////////////////////////////////////////////
// Tar 218300 This function will probably be overwritten by SSF for 
// individual AM keyboard layouts. This is setup for default IBM layout
void SMSmAssistModeBase::TABExitingAssistMode(void)
{
	  trace(L6, _T("+TABExitingAssistMode()"));
	  // Tar 218330 changed TAB functionality while in Assist Mode
	  // Send AMStop to Belt who will then send AMExitDone msg when belt has timed out(0-6 sec)
	  // The following lines will vary according to customer Assist Mode layout
	  CKeyValueParmList parmList;
	  BSTR bstrResult;
   	  CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_ASSIST_MODE_STOP);
	  _bstr_t bstrParmList((const _TCHAR *)parmList);
	  bstrResult = NULL;
	  SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
	  // Set Title2 because exiting may take a little longer
	  // indicates belt is transporting item
      ps.SetTitle(ps.GetPSText(LTT_ASSISTMODERETURN),ps.GetPSText(LTT_TRANSPORT_ITEM));
	  // Want to ensure that TB and scotapp don't get out of sync
	  // Disable psbutton1 and psbutton2 while we wait for event

      // Rel 3.2 Requirement 4.5.1 - Single Mode RAP
      if(g_csAssistModeStateName == _T("SmAssistMode")) 
	   ps.Button(_T("SMButton1"), BTT_GOTOSHOP, false);
      else
	   ps.Button(_T("SMButton1"), BTT_CLOSE, false);

      ps.DisableAssistModeKey(_T("Key1"), -1);
	  ps.DisableAssistModeKey(_T("Key16"), -1);
	  ps.DisableAssistModeKey(_T("Key18"), -1);
	  ps.DisableAssistModeKey(_T("Key27"), -1);
	  ps.DisableAssistModeKey(_T("Key28"), -1);
	  ps.DisableAssistModeKey(_T("Key29"), -1);
	  ps.DisableAssistModeKey(_T("Key49"), -1);
	  trace(L6, _T("-TABExitingAssistMode()"));
}

//For AutoRun Use Only: Used to perform automated actions while in assist mode
SMStateBase *SMSmAssistModeBase::AutoRunTimeOut(void) 
{
   int nAction = _ttoi(co.csAutoRunAssistKeyList.Mid(nAutoListIndex)); //Get Key Press

   nAutoListIndex = co.csAutoRunAssistKeyList.Find(_T(' '),nAutoListIndex+1); //Set Index to next Key Press
   if(nAutoListIndex == -1) // No more Keys to send
   {
       nTimeOut = 0; 
   }
   else
   {    
       nTimeOut = 1; // Send Key every second (Minimum TimeOut setting)
   }
   return PSAssistKey(nAction);
}

//////////////////////////////////////
SMStateBase * SMSmAssistModeBase::TBEndOfTransaction(void)
{
    SMStateBase* retState;
#ifdef _BUILDLIB_
    retState = SMStateBase::TBEndOfTransaction();
#else
    retState = SMState::TBEndOfTransaction();
#endif
    if (ps.RemoteMode())  //TAR 271178 - delay return to lane until print complete
    {
        trace(L6, _T("TBEndOfTransaction delayed assist mode exit"));
        nTimeOut = ASSISTMODE_RETURN_TIMEOUT;
        g_pDelay_TBExpectedStateChange = retState;
        retState = STATE_NULL;
        fWaitingForEOT = true;
    }
    return retState;
}
SMStateBase* SMSmAssistModeBase::DMRemoteScannerConnect(bool bConnect)
{
    trace(L6, _T("SMSmAssistModeBase::DMRemoteScannerConnect(%d)"), bConnect);
    if (bConnect)
    {
        if (!co.IsRemoteScannerConnected())
        {
            if (!co.IsAttendantModeOn())
            {
                trace(L6, _T("Entering Attendant mode via handheld scanner"));
                ps.SMButton(_T("SMButton7"), MSG_ENTER_ATTENDANT_MODE, true);
                co.SetAttendantID(csOperator);
                rp.SetOperator(csOperator);
                rp.ModeChange(rp.m_lAttendantEntryID, ATTENDANT, ENTER);
                DMTakeawayBeltNotify(tabEventAssistModeStart);
                rp.m_lAttendantEntryID += 1;
            }
            dm.ScannerRemoteAccess(true);
            co.SetRemoteScannerConnected(true);
            SAWLDBScannerEnable();
        }
        else
        {
            dm.ScannerRemoteAccess(false);
            //Save current value of Attendant mode,
            // Save orig Attendant mode setting for comparision 
            bool bAttendantMode = co.IsAttendantModeOn();
            //Cancel connection accepted in DMParse
            co.SetRemoteScannerConnected(false);
            //Check if Attendant mode is now false because it was entered using HH Scanner.
            if (!co.IsAttendantModeOn() == bAttendantMode)  
            {
                trace(L6, _T("Remote scanner disconnect exiting attendant mode"));
                ps.SMButton(_T("SMButton7"), MSG_LEAVE_ATTENDANT_MODE, true);
                rp.SetOperator(csOperator);
                co.SetAttendantID(EMPTY_STRING);
                rp.ModeChange(rp.m_lAttendantEntryID, ATTENDANT, EXIT);
                DMTakeawayBeltNotify(tabEventAssistModeStop);
                rp.m_lAttendantEntryID += 1;
            }
        }
    }
    else
    {
        dm.ScannerRemoteAccess(false);
        // Save orig Attendant mode setting for comparision 
        bool bAttendantMode = co.IsAttendantModeOn();
        //Cancel connection accepted in DMParse
        co.SetRemoteScannerConnected(false);
        //Check if Attendant mode is now false because it was entered using HH Scanner.
        if (!co.IsAttendantModeOn() == bAttendantMode)  
        {
            trace(L6, _T("Remote scanner disconnect exiting attendant mode"));
            ps.SMButton(_T("SMButton7"), MSG_LEAVE_ATTENDANT_MODE, true);
            rp.SetOperator(csOperator);
            co.SetAttendantID(EMPTY_STRING);
            rp.ModeChange(rp.m_lAttendantEntryID, ATTENDANT, EXIT);
            DMTakeawayBeltNotify(tabEventAssistModeStop);
            rp.m_lAttendantEntryID += 1;
        }
    }
    ps.ShowTrainingModeOnScreen(TrainingModeObj.IsTMOn());
    return STATE_NULL;
}

CString SMSmAssistModeBase::ExtractString(CString strData, CString sStringName)
{
    CString sString;
    bool bStatus = false;
    
    int iFind = strData.Find(sStringName);
    
    if (iFind >= 0)
    {
        sString = strData.Mid(iFind);
        iFind = sString.Find(_T("="));
        ASSERT(iFind > 0);
        if (iFind > 0)
        {
            sString = sString.Mid(iFind + 1);
            iFind = sString.Find(_T(";"));
            if (iFind > 0)
            {
                sString = sString.Left(iFind);
                bStatus = true;
            }
        }
        if (!bStatus)
            sString.Empty();
    }
    return sString;
}

SMStateBase *SMSmAssistModeBase::PSConnectRemote( CString& strMachineName, CString& strRemoteContext)	
{	
    if (strMachineName == ps.GetRemoteServerName())
    {
        ra.AcceptConnection( ps.GetRemoteServerName(), csCurContext );
    }
    return STATE_NULL;	
}
