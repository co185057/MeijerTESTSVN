//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMInProgressBase.cpp
//
// Description: The InProgressBase state is used when the transition from a
//        state to the next state might take some time.  InProgressBase is a
//        "chameleon" state with respect to Customer Mode, Store Mode,
//        and Help Mode - it becomes the mode of the creating state and
//        therefore does not inherit from the above classes.
//
//
// Author:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////
 
#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header


#ifndef _CPPUNIT
#include "Common.h"                 // MGV common includes

#include "SMInProgress.h"           // MGV this state
#include "SMSystemMessage1.h" 
#include "SMScanAndBag.h"			// 410299	
#include "SMFinish.h"               // 410299
#include "SMRapDataNeeded.h"        // SSCOADK-493
#include "SMCmDataEntry.h"          // SSCOADK-493
#include "PSConfigData.h"           // SSCOADK-493
#include "LaneStateData.h"
#include "SMRecallingItems.h"


#define COMP_ID ID_SM               // base state component
#define T_ID _T("InProgressBase")
const long NCR_DO_NOT_INSERT = 1007;
#else
#include "SMInProgressBase.h"           // MGV this state
#include "RAEvents.h"


#endif //CPPUNIT

IMPLEMENT_DYNCREATE(SMInProgressBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(InProgress)

DEFINE_TIMESTAMP


bool  myStoreMode = false ; //SSCOADK-493

#ifndef _CPPUNIT
////////////////////////////////////////////////
SMInProgressBase::SMInProgressBase(const bool storeMode,
                                    const bool helpMode,
                                    const int timeout)
                                    : fStoreMode(storeMode),
                                    fHelpMode(helpMode)
{
  IMPLEMENT_TIMESTAMP
		nTimeOut = timeout;
}
#endif //CPPUNIT
////////////////////////////////////////
SMInProgressBase::SMInProgressBase()
{
  IMPLEMENT_TIMESTAMP
#ifndef _CPPUNIT
  fAMNeedEnable = false;  // save the state of Assistmode button at RAP
#endif //CPPUNIT
}

#ifndef _CPPUNIT
////////////////////////////////////////
SMStateBase  *SMInProgressBase::Deliver(const bool storeMode,
                           const bool helpMode,
                           const int timeout)
{
  fStoreMode = storeMode;
  fHelpMode = helpMode;
  nTimeOut = timeout;
  fAMNeedEnable = false;  // TAR246532
  myStoreMode = storeMode; //SSCOADK-493
  return this;
}

/////////////////////////////////////////
bool SMInProgressBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    
    // There should be three parameters, two bools passed as word and an int
    if (dlParameterList.GetDescriptorCount() == 3)
    {
        trace(L6, _T("+SMInProgressBase::DeliverDescriptorList() - Parameter Count of 3"));
        Deliver((bool)DLParamWORD(dlParameterList),    // storeMode
                (bool)DLParamWORD(dlParameterList),    // helpMode
                (int)DLParamDWORD(dlParameterList));   // timeout
        return true;
    }
    else
    {
        trace(L2, _T("+SMInProgressBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}

SMInProgressBase::~SMInProgressBase()
{
  CLaneStateData laneStateData(m_pAppPSX);
  laneStateData.SendNextGenUIData(_T("RemoteProxyUserInputCompleted"), true);

}

#endif //CPPUNIT
//////////////////////////////////////
SMStateBase  *SMInProgressBase::Initialize(void)
{
    trace(L6,_T("+SMInProgressBase::Initialize(), g_bInRecallMode = %d"), g_bInRecallMode);
    if(g_bInRecallMode && fSAInItemSale)
        RETURNSTATE(RecallingItems)

	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("InProgress") );	// RFC 330050
	m_bNeedToDisplayAuxScreen = false; // DON'T display aux screens in this state!
	//+TAR 350775 
	CString csPreviousStateName = this->csLastNonHelpStateName;
	csPreviousStateName.MakeLower();
	if ( fSAInStoreMode && ( csPreviousStateName.Find( _T("smsmdataentry") ) > -1 ) && \
		fItemIsWaiting )
	{
        fRAPIsInStoreMode = false;
		ra.OnExitStoreMode();
	}
	//-TAR 350775 
	// +TAR246532
	if (ra.GetRequestEnableAssistMode() && !ps.RemoteMode())  
	{
		// not in remote mode, currently AM is enabled to access it at RAP
		// disable access to AM at RAP while in this state and enable it on exit

        fAMNeedEnable = true; // save the state of Assistmode button at RAP and restore it on exit of this state
		ra.RequestEnableAssistMode(false);	 
	}
	// -TAR246532
    bDoNotResetTimeOutOnDMEvent = true; //sscoadk-6114
	return STATE_NULL;
}

#ifndef _CPPUNIT
////////////////////////////////////
SMStateBase  *SMInProgressBase::TimedOut(void) // timeout
{
  trace(L6, _T(" !!! Timedout in SMInProgress while waiting to hear from TB !!!, potential out-of-synch situation betweeen TB and SCOT"));

  CLaneStateData laneStateData(m_pAppPSX);
  laneStateData.SendNextGenUIData(_T("RemoteProxyUserInputCompleted"), true);

  // RFC 262343 - start
  if (fTenderPending) //If we time out while in a tender
  {
      ScotError(RETURN, SM_CAT_INTERNAL_ERROR, SCOTAPP_TENDER_TIMEOUT);
      fTenderPending = false;       
      if (cDMLastTrack)
      {
          delete [] cDMLastTrack; //SSCOP-4165
          cDMLastTrack = NULL;
      }
  }
  else
  {
      ScotError(RETURN, SM_CAT_INTERNAL_ERROR, SCOTAPP_TB_OUT_OF_SYNC);
  }
  // RFC 262343 - end

  // +207606 LPM062002  Scanner not re-enabled if the TB doesn't ever reply 
  trace(L2, _T("::TimedOut - bEASReEnableScanner is set to true."));
  bEASReEnableScanner = true;
  // -207606
  //TAR251537+
  if ( getAnchorState() == BES_CARDPROCESSING ) 
  {
	  if (bIsInFinalization )
	  {
		trace(L2, _T("::TimedOut - Changing Anchor State to Finish."));
		//(+)SSCOB-19503
        if(nTenderType != TB_TENDER_CASH)
        {
            trace(L2, _T("::TimedOut - Resetting nTenderType [%d] to [%d]"),nTenderType,TB_TENDER_NONE);
             nTenderType = TB_TENDER_NONE;
        }
		//(-)SSCOB-19503
            
		return setAndCreateAnchorState(BES_FINISH);
	  }
	  else
	  {
		trace(L2, _T("::TimedOut - Changing Anchor State to ScanAndBag."));
		return setAndCreateAnchorState(BES_SCANANDBAG);
	  }
  }
  //TAR252537-
  
  //+TAR391424+
  //we will go to HandleOutOfSyncAssist if item is in progress of selling and TB
  //doesn't return anything until InProgress timedout.
  if ( (io.csItemCode.GetLength()>0) && (getAnchorState() != BES_CARDPROCESSING) ) 
  {
	SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
  	SMStateBase *returnState = STATE(State)::HandleOutOfSyncAssist();
	m_bTBNeedAssistModeGetDiag = false; //if this is the case, no diag file needed
	return returnState;
  }
  //-TAR391424-  
  
  
  return createAnchorState();
}

//////////////////////////////////
SMStateBase  *SMInProgressBase::DMScanner(void) // scanned an item
{
  trace(L3, _T("Probable race condition, scanned code: %s, Previous input sent to Sec: %d, fItemIsWaiting: %d"), csDMLastBarCode, nSAPrevSCOTInputID, fItemIsWaiting); 
  if (!fItemIsWaiting)
  {
	csItemSent = csDMLastBarCode;
	fItemIsWaiting = true;            // to know if the item was sold from the attract screen or not.  sell item after signon in S&B
	if (nSAPrevSCOTInputID != UPC_SCANNED) 
	{
	  trace(L3, _T("Sending Security event ID: 1 to fix race condition."));		// 
	  SASendSCOTInput( UPC_SCANNED, csDMLastBarLabel); //security needs to know we scanned an item quickly.  //TAR#121479
	}
  }
  return STATE_NULL;
}

////////////////////////////////////////
// trap this message just so base doesn't give an error msg,
// nothing else to do here
SMStateBase  *SMInProgressBase::DMCardReader(void)
{
  return STATE_NULL;
}


///////////////////////////////////////////////////////////////////////////
SMStateBase *SMInProgressBase::OnBackToLGW(void)
{
	// TAB re-enable scanner and clear Wait For Approval State
	TABReEnableScannerAndClearWaitForApprovalState();
	return STATE_NULL;
}

#endif
//TAR199913+
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMInProgressBase::OnWtIncreaseNotAllowed()  
{
    if (bIsInFinalization )   //We are at finialization stage
        return FinalizationWtIncrease(BES_STATERETURN); 
    else if (bQuietNeedSecApproval || m_bScanOnAttractAtQuietMode)
        return STATE_NULL;
    else
        return STATE(State)::OnWtIncreaseNotAllowed();
}
#ifndef _CPPUNIT

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMInProgressBase::OnWtDecrease()
{

	if (bIsInFinalization)   //We are at finialization stage
	{
		if(!fInSuspendTrans && !g_bIsForgiveUnexpectedDecreaseDuringTendering && !(lBalanceDue < 0.01))  //sscoadk-5038
			return FinalizationWtDecreaseNotAllowed(BES_STATERETURN);
		else
        {   trace(L6, _T("SMInProgressBase::OnWtDecrease:Ignore weight decrease exceptions"));
			return STATE_NULL;
        }
	}
	else
		return STATE(State)::OnWtDecrease();
}

//TAR199913-

//////////////////////////////////////////////////////////////////////////
void SMInProgressBase::UnInitialize()
{
    bDoNotResetTimeOutOnDMEvent = false; //sscoadk-6114
	if (fAMNeedEnable)  
	{
        fAMNeedEnable = false; 
		ra.RequestEnableAssistMode(true); // save the state of Assistmode button at RAP
	}
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("InProgress") );	// RFC 330050
}

bool SMInProgressBase::PSRemoteLegalForState() 
{
	return true;
}

#endif //CPPUNIT	

//TAR222852 CHU 052903
//If the bill/coin is accepted in this state, we will reject it
////////////////////////////////
SMStateBase *SMInProgressBase::DMAcceptor(void)
{
  long rc;
 //+SSCOADK-836
  //if already paid enough cash, don't reject any extra money until TakeChange or later
  if(m_nDMCashPaid+lTotalPaid >= lBalanceDue || co.fLocaleLeaveCashAcceptorsEnabledDuringTransaction)
  {
     trace(L6,_T("SMInProgressBase::DMAcceptor() (nDMCashInserted=%d) (m_nDMCashPaid=%d) (lTotalPaid=%d) (lBalanceDue=%d)"),nDMCashInserted,m_nDMCashPaid,lTotalPaid,lBalanceDue);
	 return STATE_NULL;
  }//-SSCOADK-836
  trace(L6,_T("SMInProgressBase::DMAcceptor; can't accept coin/bill in this state."));
  long amountDispensed = 0; //SSCOADK-836
  rc = DMCashAcceptorReject();  // Coin Acceptor Modifications
  if (rc < 0)			//  most likely out of change
  {
	return DMCashChangerError(nDMLastBillEvent);
  }
  else if (nDMLastBillEvent > 0)
  {

    amountDispensed = nDMLastBillEvent;
    trace(L6, _T("Reject bill amount: %d"), nDMLastBillEvent);
    nDMCashInserted -= nDMLastBillEvent;
    nDMLastBillEvent = 0;
	
  }

  //+SSCOADK-836
  if(nDMLastCoinEvent)
  {
      rc = DMCoinAcceptorReject(); 
      if (rc <= 0)		
      {
	    return DMCashChangerError(nDMLastCoinEvent);
      }
      else 
      {
          trace(L6, _T("Reject coin amount: %d"), rc);
          amountDispensed = rc;
          nDMCashInserted -= rc;
          if(nDMLastCoinEvent == rc)
		  {
			  nDMLastCoinEvent = 0;
		  }
		  else
		  {
			  nDMLastCoinEvent -= rc;
              
              CString csTemp = EMPTY_STRING;
              csTemp.Format(ps.GetPSText(MSG_RA_CHANGEDISPENSED, SCOT_LANGUAGE_PRIMARY), ps.FmtDollar(amountDispensed));
              amountDispensed = 0;
              trace(L7, _T("+ra.OnNormalItem, Message=%s"), csTemp);
              ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+ csTemp);
              trace(L7, _T("-ra.OnNormalItem"));

			  return DMCashChangerError(nDMLastCoinEvent);
		  }
      }  
  //-SSCOADK-836
  } 
  #ifndef _CPPUNIT
  if (!Is5XHardware() && !Is6XHardware())  // Tar 403042
  {
	  //SR672
	  bool fCheckHealth = false;
	  CString sModelName=dm.GetDeviceModelName(DMCLASS_CASHCHANGER, 0);
	  CString csWork = DMGetDeviceErrorHTML(DMCLASS_CASHCHANGER, sModelName, CString(_T("")), NCR_DO_NOT_INSERT, -99, 0, NULL, &fCheckHealth, 0, co.csPrimaryLanguageCode );
	  
	  //+RFC 330995
	  //CString csWork = _T("DeviceErrorCashChanger.xml:DM_CASHCHANGER_STATUS_DISPENSE");
	  CREATE_AND_DISPENSE(SystemMessage1)(csWork,	// no char *message	
						PS_NOTEXT,	// canned message
						PS_NOTEXT,  // no prompt
						0,          // no prompt value
						-1,         // no device
						false, true);     // no DM check health needed 

	  //-RFC 330995
  }

  return STATE_NULL;    
}
#endif //CPPUNIT
#ifndef _CPPUNIT
//+ TAR 391316
SMStateBase *SMInProgressBase::PSButton1(void)     
{
	trace(L6, _T("SMInProgressBase::PSButton1 - ignore PSButton1 button press"));
	return STATE_NULL; 
}
//- TAR 391316


// TAR 246429
SMStateBase *SMInProgressBase::PSButtonHelp()
{
    trace(L6,_T("SMInProgressBase::PSButtonHelp - ignore help button press"));
	return STATE_NULL;    
}
// TAR 246429

//405090+
SMStateBase *SMInProgressBase::DMParse(const MessageElement *me)
{
    trace(L6, _T("SMInProgressBase::DMParse"));
        
    if( (me->dmInfo.Msg == DM_DATA) &&
        (me->dmInfo.Class == DMCLASS_MSR) )
    {
	    trace(L6,_T("SMInProgressBase::DMParse; can't accept any swipe in this state."));
		return STATE_NULL;

	}

    return STATE(State)::DMParse(me);

}
//405090-

//410299+
SMStateBase *SMInProgressBase::SASecMgrParse( const MessageElement *me)
{

    int nEvent = me->secmgrInfo.nEvt;
	trace(L6, _T("SMInProgressBase::SASecMgrParse nEvent = %d"), nEvent);
    LPTSTR szParms;
    szParms = new TCHAR [_tcslen(me->secmgrInfo.szParms) + sizeof(TCHAR)];
    _tcscpy( szParms, me->secmgrInfo.szParms );
    
	SecurityControlEventParms secControlEventParms;
	if (_tcslen(szParms) != 0)
	{
		secControlEventParms.ParseKeys(szParms);
	}
	delete [] szParms;

    if ( (nEvent == SmOnSecurityControl) && 
		 (secControlEventParms.m_csOperation == KEY_OSC_OPERATION_GOTO_SCANANDBAG ) &&
		 ( (nTBLastRealState == TB_LOYALTYCARD )
		   || (TB_OVERRIDE_SECURITY_WITH_NOWEIGHT == io.d.lRequireSecBagging )
		   || (TB_OVERRIDE_SECURITY_WITH_YES != io.d.lRequireSecBagging && io.d.lExtPrice < 0 )
		   || (io.nLoyaltyCardScanned != 0)  //tar458319
		   ) )//tar413251
    {
		trace(L6, _T("KEY_OSC_OPERATION_GOTO_SCANANDBAG is received"));
        delete [] me->secmgrInfo.szParms;
		if (getAnchorState() == BES_FINISH && !co.IsInAssistMenus())  //SSCOADK-7224
		{
			RETURNSTATE(Finish)
		}
		RETURNSTATE(ScanAndBag)
    
	} 

    return STATE(State)::SASecMgrParse(me);
#endif //CPPUNIT
}

#ifndef _CPPUNIT
SMStateBase *SMInProgressBase::TBParse(MessageElement* mg)// same parse for all
{
    return STATE(State)::TBParse(mg);
}
#endif //CPPUNIT

#ifndef _CPPUNIT
//SSCOADK-493+
SMStateBase *SMInProgressBase::OnTBNeedData(void)
{
    trace (L6, _T("+SMInProgressBase::OnTBNeedData()"));
    CString sDescription, sView, sINI, sViewString, sSection, csStateName;
    //RFQ 2506 - rework OnTBNeedData logic for correct display of new RAPDataNeeded text overrides
    CString sRDNLaneMessage, sRDNLaneLeadthru, sRDNValues;  //Strings for Rap Data Needed customer display
    CString sViewCustomer;
    CSCOTDataEntry cCustomerDataEntry;
    CPSConfigData cd;
    
    
    //Jira SSCOP-831 Start
    SCOT_LANGUAGE_TYPE currentLanguage;
    currentLanguage = ps.GetLanguage();
    ps.SetLanguage(m_customerLanguage);
    //Jira SSCOP-831 End


    bool bGotDataNeededInfo = GetDataNeededViewFromTB(sDescription, 
                                                        sView,
                                                        sINI,
                                                        sSection);


    if(!bGotDataNeededInfo)
    {
        trace(L4, _T("GetDataNeededViewFromTB Failed"));
        ps.SetLanguage(m_customerLanguage); //Jira SSCOP-831
        return STATE_NULL;
    }
    
    
    if(!sView.IsEmpty())     //String format DataNeeded parameters
    {
        sViewString = sView;
        sViewCustomer = sView;
        cd.FormatString(sViewString, true);  //Primary Language Translation
        cd.FormatString(sViewCustomer, false);  //Customer Language Translation
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
            ps.SetLanguage(m_customerLanguage); //Jira SSCOP-831
            return STATE_NULL;
        }
    }
    else if(sView == _T("") )
    {
	if (myStoreMode)  //SMSMDataEntry
        {
            trace(L4, _T("GetDataNeededView cancel, INI and View are both empty, TB wants to cancel"));
            SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
            ps.SetLanguage(m_customerLanguage);
            //setAnchorState((BEST) nlaststate);
			//return STATE_RESUME;
            return createAnchorState(); //sscoadk-1876
            
        }
        else  //SMCMDataEntry
        {
            if (fSAInItemSale) //Tar 235531 FFR: Data Entry during Item Sale 
            {
                trace(L4, _T("GetDataNeededView cancel, item sale in progress got to InProgress state"));
                //setAnchorState((BEST) nlaststate);
                return STATE_NULL;
            }
            else
            {			
                trace(L4, _T("GetDataNeededView cancel, INI and View are both empty, TB wants to cancel"));
                SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
                return createAnchorState();
            }

        }
    }

    cCustomerDataEntry.FromString(sViewCustomer);
    sRDNLaneMessage = cCustomerDataEntry.GetRDNLaneMessage();
    sRDNLaneLeadthru = cCustomerDataEntry.GetRDNLaneLeadthru();
    
    if (!cCustomerDataEntry.IsStoreLogonEnabled() && !co.IsInAssistMenus())
    {
        sViewString = cCustomerDataEntry.ToString();
	SMCmDataEntryBase::sm_pDataNeededEntry.FromString(sViewString);
        trace(L6,_T("Data Needed View configuration parameters %s"),sViewString);
        trace (L6, _T("-SMInProgressBase::OnTBNeedData() with configuration parameters %s"),sViewString);
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
    sViewString =  sViewString + sRDNValues;
    trace(L6,_T("Data Needed View configuration parameters %s"),sViewString);
    trace (L6, _T("-SMInProgressBase::OnTBNeedData()"));
    
    ps.SetLanguage(m_customerLanguage); //Jira SSCOP-831
    // no StoreLogon parameter means we default to store mode
    SMCmDataEntryBase::sm_pDataNeededEntry.FromString(sViewString);    
    CREATE_AND_DISPENSE(RAPDataNeeded)(sDescription, DATANEEDED_MOREDATA, sViewString, storeMode());    	
    
}
//SSCOADK-493-
#endif //_CPPUNIT