//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateSABase.CPP
//
// TITLE: SA SMStateBase functions, implement all SMState::SA* functions
//
// AUTHOR:  
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include "LaneStateData.h"
#include "DataNeededUtil.h"
#ifndef _CPPUNIT
#include "Common.h"                 // common includes
#include "SMState.h"
#include "resource.h"
#include "SMSecurityLevel2.h"
#include "SMWaitForApproval.h"      // Wait for approval state
#include "SMSmmEditAdd.h"			// SMM edit add screen return to refresh the buttons
#include "MPProcedures.h"           // MP Object
#include "SMSecUnExpectedIncrease.h"
#include "SMSecBagBoxThreshold.h"      
#include "SMSecItemRemovedThreshold.h"      
#include "SMSecNewItemEntry.h"      
#include "SMSecSkipBaggingThreshold.h"      
#include "SMBagAndEAS.h" 
#include "SMSecUnExpectedDecrease.h"
#include "SMTransportItem.h"         // TAR221089 CHU 11/25/02
#include "SMSmmEditAdd.h"		// Prev state
#include "SMSmmInvalidWtTol.h"	// Error state
#include "SMSecMisMatchWeight.h"
#include "SMSmAuthorization.h"   
#include "SMSecurityAnalysis.h" 
#include "SMFinish.h" 
#include "SMDegradedContinueTrans.h"
//USSF Start
#include "USSFProcedures.h"
#include "SMUSSFManager.h"
#include "USSFCommon.h"
//USSF End
#include "SMSecBagBoxNotValid.h"//SR085

#include "SMRAPDataNeeded.h"
#else
#include "scotdeferredoverride.h"
#include "FakeSMStateBase.h"
#endif //_CPPUNIT
#include "IQueuedAction.h"
#include "QueuedActionGenericDelayedIntervention.h"
#include "DataNeededUtil.h"

#ifndef _CPPUNIT
DLLEXPORT extern DMProcedures dm;
DLLEXPORT extern MPProcedures mp;
DLLEXPORT extern SecMgrProcedures SecMgr; // global Security Manager object


#ifdef _DEBUG
DLLEXPORT extern CWnd * pDialogWindow;			  // access to dialog controls for debug build
#endif

#define COMP_ID ID_SM			// base state component
#define T_ID    _T("SMsaBase")
#define ITEM_CODE_LENGTH 32     // item code length



#define PROCESS_EXCEPTIONS(a)                                 \
	catch(COleDispatchException * e)                              \
{                                                             \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SSINTERFACE_OLEDISPATCH_EXCEPTION, \
  _T("%s|%s|%x"),   \
	msg,                                                      \
	e->m_strDescription,                                      \
	e->m_scError);                                            \
  e->Delete();                                               \
  return a;                                                   \
}                                                             \
	catch(COleException* e)                                       \
{                                                             \
  e->GetErrorMessage( cstr, 255 );                           \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SSINTERFACE_OLE_EXCEPTION,_T("%s|%s|%x"),        \
	msg,                                                      \
	cstr,                                                     \
	e->m_sc);                                                 \
  e->Delete();                                               \
  return a;                                                   \
}                                                             \
	catch(CException* e)                                          \
{                                                             \
  e->GetErrorMessage( cstr, 255 );                           \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SSINTERFACE_C_EXCEPTION,_T("%s|%s"),                   \
	msg,                                                      \
	cstr);                                                    \
  e->Delete();                                               \
  return a;                                                   \
}                                                             \
	catch(...)                                                    \
{                                                             \
  long le = GetLastError();                                   \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SSINTERFACE_EXCEPTION,_T("%s|%d"),msg,le);            \
  return a;                                                   \
}


#endif //_CPPUNIT
// Security Agent status objects, static variable available to all states
long SMStateBase::lSALoggedMsgs = 0;    // total number of SA generated messages that have been logged
#ifndef _CPPUNIT
long SMStateBase::lSAYellowNeedingApproval = 0;	// number of SA yellow light conclusions requiring approval
bool SMStateBase::lSARedNeedingApproval = false;	// TAR 197015 Threshold limits exceeded
long rc;
#endif //_CPPUNIT
bool SMStateBase::fSAInTransaction = false;
bool SMStateBase::fSAInItemSale = false;
#ifndef _CPPUNIT
bool SMStateBase::fSAInItemize = false;
bool SMStateBase::fSAInTender	= false;
bool SMStateBase::fSAInCustMode = false;
#endif //_CPPUNIT
bool SMStateBase::fSAInStoreMode = false;
#ifndef _CPPUNIT
bool SMStateBase::fSAInTenderStart	= false;
bool SMStateBase::g_bAllowRemove = false;
#endif //_CPPUNIT
long SMStateBase::nSAPrevSCOTInputID = 0; // store previous input id sent to security

CString SMStateBase::csScaleUnits = _T("");
#ifndef _CPPUNIT
CString SMStateBase::csSMMScannedUPC = _T("");

#endif //_CPPUNIT
long SMStateBase::g_lLGW = 0;				//current "good weight"
#ifndef _CPPUNIT
long SMStateBase::g_lCTW = 0;				//current total weight (delta + LGW)
long SMStateBase::g_lLRW = 0;				//current removed total (LGW - delta)
#endif //_CPPUNIT
bool SMStateBase::bIsInFinalization = false; //in finalization
#ifndef _CPPUNIT
long SMStateBase::g_lPreLRW = 0; //previous LRW - tar 197339

long SMStateBase::g_lBeginningTransLGW = 0;
#endif //_CPPUNIT
bool SMStateBase::g_bLastImmediateInterventionCleared = false;
#ifndef _CPPUNIT
long SMStateBase::g_lBaggingReq = true;			//229344 should be true, not 1
long SMStateBase::m_lWtComparisonReq = true;	//229344 new global to track wt comparison required LPM022703
bool SMStateBase::m_bLastChanceLookupCalled = false;
#endif
long SMStateBase::g_lNumberOfWLDBEntriesThisItem = -1;
#ifndef _CPPUNIT
long SMStateBase::g_lWLDBNotApprovedThisTrans = 0;
bool SMStateBase::g_bOkToUpdate = false;
#endif
bool SMStateBase::g_bWtExpected = false;
#ifndef _CPPUNIT
bool SMStateBase::g_lRAPApprovedUpdate = false;
BEST SMStateBase::nStateBeforeSecVio = BES_STATERETURN;
bool SMStateBase::g_bGotDBLookupSuccess = false;
bool SMStateBase::g_bGotDeptInfoSuccess = false;
bool SMStateBase::g_bTBReturnedITEMSOLDforSA = false;
bool SMStateBase::g_bMatchedReceivedForTAB = false;
bool SMStateBase::g_bWtIncAtContextHelpComingFromAttract = false; //TAR237079 CHU/TP 051303
static BSTR pItemCode = NULL;
static short sNoWLDBData = -1;
long SMStateBase::g_lTABMisMatchApprWt = 0;
long SMStateBase::m_lRememberVirtualZero = 9999999;  // TAR 257709
#endif //_CPPUNIT
long SMStateBase::lSecMgrCookie = 0;
bool SMStateBase::bTransactionComplete = false;
SMStateBase::SecMgrEventParms SMStateBase::secEventParms;
SMStateBase::SecAppControlEventParms SMStateBase::secAppControlEventParms;
//PointerStack<SMStateBase::SecImmediateIntervention *> SMStateBase::ImmediateInterventionStack;  
CTypedPtrList<CPtrList,SMStateBase::SecImmediateIntervention *> SMStateBase::ImmediateInterventionStack;  

SMStateBase::SecMaintenanceRecord *SMStateBase::psecMaintenanceRecordArray= NULL;
long SMStateBase::lSMMWeightCount = 0;
long SMStateBase::g_lTolerance = 0;
bool SMStateBase::g_bIsSecureCamEnable = false;
bool SMStateBase::g_bIsSmartAssistImmediate = false;
bool SMStateBase::g_bReportTrustLevel = false;

int SMStateBase::g_nSecurityAnalysisTimer = DEFAULT_SECURITY_ANALYSIS_TIMER;
bool SMStateBase::g_bSecurityAnalysisTimerExpires = false; 
bool SMStateBase::m_bSecurityAnalysisComplete = false; 
bool SMStateBase::g_bSLServerStatus = false;
CString SMStateBase::csSMMTolerance=_T("0.20");
CString SMStateBase::csSMMSubstitutionCheck=_T("default-y");
CString SMStateBase::csSMMToleranceType=_T("default");
CString SMStateBase::csSMMWeightRequired=_T("default-y");
CString SMStateBase::csSMMErrorType=_T("");
CString SMStateBase::csSMMMinTolerance=_T("50");
CString SMStateBase::csSMMMaxTolerance=_T("500");
CString SMStateBase::csSMMOperation=_T("query");
CString SMStateBase::csSMMServerStatus=_T("online");
CString SMStateBase::csSMMObservedWeight=_T("0.00");
#ifndef _CPPUNIT
CString SMStateBase::g_csInitMissingFilename = _T("");
CString SMStateBase::g_csInitErrorFilename = _T("");
#endif //_CPPUNIT
CString SMStateBase::g_csActualWeight = _T("");
CString SMStateBase::g_csExpectedWeight = _T("");
CString SMStateBase::g_csDeltaWeight = _T("");  // SR682
CString SMStateBase::g_csTareWt1 = _T(""); // SR682
CString SMStateBase::g_csTareWt2 = _T(""); // SR682
CString SMStateBase::g_csTareWt3 = _T(""); //SR714
bool SMStateBase::g_bSubstCkPriceEmbeddedWeights = false;
bool SMStateBase::g_bApproveFromRAP = false;
bool SMStateBase::g_bIsForgiveUnexpectedDecreaseDuringTendering = true;
bool SMStateBase::g_bIsSecurityDisableSubCheckOnly = false;
bool SMStateBase::g_bIsSecurityTransactionPolicyReady = false;
#ifndef _CPPUNIT


extern bool bWriteEndOfTransactionToWLDBlog; // NN RFC 210177

#endif //_CPPUNIT
#ifndef _CPPUNIT
void SMStateBase::SecMgrPreInitialize(void)
{
    trace(L6, _T("+SMStateBase::SecMgrPreInitialize"));

    // +TAR 452531:  Moved this block of code from 
    //               SMOutOfServiceBase::InitializeAll to consolidate calls
    //               from 2 different places.
    //BEGIN: TAR123168    LPM101199  BagScale & Scanner disabled while 
    //       terminal out of service    
    // DMInitialize() enables the bagscale & scanner, so they have to be 
    // disabled here
    DMScaleDisable();                  // disable the scale
    DMScannerDisable();                // disable the scanner    
    //end 123168

    //DMScaleEnable();       // enable the scale moved to attract base TAR123268
    DMEncryptorEnable();     // enable the encryptor
    // Lane light off
    dm.RemoteRelay(DM_MISC_STATE_OFF);
    // Red light on
    
    // (*) SR898 ====================
    DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                       DM_TRICOLORLIGHT_ON,
                       DM_TRICOLORLIGHT_NORMALREQUEST);
                       
    ps.Echo(DM_LOADED);

    // load Weight Info Object
    ps.Echo(WTINFO_LOADING);
    DMSayPhrase(KEYPADVALID);
    // NewSec
    ps.Echo(MSG_SEC_MGR_LOADING);
    DMSayPhrase(KEYPADVALID);
    // -TAR 452531

    trace(L6, _T("-SMStateBase::SecMgrPreInitialize"));
}

///////////////////////////////////////////////////////////////////////////////
long SMStateBase::SecMgrInitialize(void)    // called by MP at start up
{
	_TCHAR msg[] = T_ID _T("::SecMgrInitialize");
	_TCHAR cstr[255];
	long	rc = 1;


	// if we were able to create the Security Manager control then call its methods
	// to initialize the control
    try   //tar 266236
	{
		if ( rc = SecMgr.Initialize() )
		{
			lSecMgrCookie = SecMgr.SecurityAdvise(0);
			if ( 0 == lSecMgrCookie )
			{
				ScotError(FATAL,SM_CAT_STARTUP_INIT, SCOTAPP_SEC_MGR_PROCEDURE_ADVISE_FAILED);
				rc = 0;
				return rc;
			}
			trace(L6, _T("SMStateBase::SecMgrInitialize lSecMgrCookie = %d"), lSecMgrCookie);
			BSTR bstrResult = NULL;
			HRESULT hr;
			CString csEventResult;
			InitializeParms parms;
			CKeyValueParmList parmList;
			// send the terminal number for merge to Security Manager
			int nTermNumber = TBGetTerminalNumber();
			CKeyValue<int>::Put(parmList, KEY_IZ_LANE_NUMBER, nTermNumber);
			_bstr_t bstrParmList((const _TCHAR *)parmList);
			while( (hr = SecMgr.Initialize(lSecMgrCookie, bstrParmList, &bstrResult) ) != S_OK )
			{
				CString csResult(bstrResult);
				parms.ParseKeys(csResult);
				ps.Echo(parms.m_nMsgTextIndx);

				char ucValue[18];
				itoa(hr, (char *)&ucValue, 10);
				csEventResult = "hr=";
				csEventResult += ucValue;
				csEventResult += ";";
				csEventResult += bstrResult;
       			trace(L6, _T("SMStateBase::SecMgrInitialize bstrResult = %s"), csResult);
				SysFreeString(bstrResult);
				bstrResult = NULL;
				// if an error occurred during initialization then log an error and stop
				// loading application
				if ( (hr != S_FALSE) )
				{
					// +TAR 268391
					if (g_csInitMissingFilename != _T("") )
					{
						ScotError(FATAL,SM_CAT_STARTUP_INIT, SCOTAPP_SEC_MGR_PROCEDURE_INIT_FAILED, _T("%s %s %s"), parms.m_csDeviceName, parms.m_csFailureText, g_csInitMissingFilename);
						CString csMsg;
						csMsg.Format(_T("%s is missing.  Please copy this file to Config Folder."), g_csInitMissingFilename);
						AfxMessageBox(csMsg, MB_SYSTEMMODAL);

					}
					else if (g_csInitErrorFilename != _T("") )
					{
						ScotError(FATAL,SM_CAT_STARTUP_INIT, SCOTAPP_SEC_MGR_PROCEDURE_INIT_FAILED, _T("%s %s %s"), parms.m_csDeviceName, parms.m_csFailureText, g_csInitErrorFilename);
						CString csMsg;
						csMsg.Format(_T("Error in %s file.  Please open this file to find edit probelms."), g_csInitErrorFilename);
						AfxMessageBox(csMsg, MB_SYSTEMMODAL);

					}
					// -TAR 268291
					rc = 0;
					return rc;
				}
			}
			SysFreeString(bstrResult); // cs185088 mem leak 2012.09.11
		}
	}
    PROCESS_EXCEPTIONS(rc);

	UpdateScaleUnits();      // Update the scale unit
	return rc;

}

void SMStateBase::SecMgrPostInitialize(void) 
{
    trace(L6, _T("+SMStateBase::SecMgrPostInitialize"));
    // +TAR 452531:  Moved this block of code from 
    //               SMOutOfServiceBase::InitializeAll to consolidate calls
    //               from 2 different places.
    
    // set the value of ScaleMetric option in the Security Manager
    if (co.fOperationsScaleMetric)
    {
        SetSecurityConfig( CONFIG_KEY_SS_METRIC_SCALE, _T("true"));
    }
    else
    {
    SetSecurityConfig(CONFIG_KEY_SS_METRIC_SCALE, _T("false"));
    }

    if (co.fOperationsShowItemRemovedButtonOnRemoveItemViolationScreen)
    {
    SetSecurityConfig( CONFIG_KEY_REMOVE_ITEM_VIOLATION_AUTO_ADVANCE, _T("true"));
    }
    else
    {
    SetSecurityConfig(CONFIG_KEY_REMOVE_ITEM_VIOLATION_AUTO_ADVANCE, _T("false"));
    } 

    SendSecureCamConfigToSecMgr(g_bIsSecureCamEnable);

    /*  g_bDelayedSecIntervention = GetSecurityConfig(CONFIG_KEY_DELAY_EXCEPTIONS);
    g_bSubstCkPriceEmbeddedWeights = GetSecurityConfig(CONFIG_KEY_SUBST_CK_PRICE_EMBEDDED_WT);
    g_bIsForgiveUnexpectedDecreaseDuringTendering = GetSecurityConfig(CONFIG_KEY_FORGIVE_UNEX_DEC_DURING_TENDERING);
    g_bIsSecurityDisableSubCheckOnly = GetSecurityConfig(CONFIG_KEY_SECURITY_DISABLE_SUBSTITUTION_CHECK);
    //sr714+
    if( GetSecurityConfigValue(CONFIG_KEY_TARE_WT_1) != _T("") )
    {
        g_csTareWt1 = GetSecurityConfigValue(CONFIG_KEY_TARE_WT_1); 
    }
    if( GetSecurityConfigValue(CONFIG_KEY_TARE_WT_2) != _T("") )
    {
        g_csTareWt2 = GetSecurityConfigValue(CONFIG_KEY_TARE_WT_2); 
    }
    if( GetSecurityConfigValue(CONFIG_KEY_TARE_WT_3) != _T("") )
    {
        g_csTareWt3 = GetSecurityConfigValue(CONFIG_KEY_TARE_WT_3); 
    }
    //sr714-


    trace(L6, _T("SubstCkPriceEmbeddedWeights items = %d."), g_bSubstCkPriceEmbeddedWeights);*/

    //Check for TAB is available or not
    CString csDevices = SecMgr.GetDevices();
    if (csDevices.Find(VALUE_DEVICE_TAKEAWAY_BELT) != -1)
    {
      dm.fStateTakeawayBelt = true;
	  ps.Echo(TAB_LOADED);//SSCOP-2683
      m_pAppPSX->SetTransactionVariable(_T("NextGenUITABConfigured"), VARIANT_TRUE); //SSCOADK-4984
      trace(L6, _T("!!!!!!!!!!!!!!!This is TAB system.!!!!!!!!!!!!"));	
    }
    else
    {
      m_pAppPSX->SetTransactionVariable(_T("NextGenUITABConfigured"), VARIANT_FALSE); //SSCOADK-4984
      trace(L6, _T("!!!!!!!!!!!!!!!This is non-TAB system.!!!!!!!!!!!!"));
    }

    co.InitCustomerBagAllowed(dm.fStateTakeawayBelt); // TAR 393731

    ps.Echo(MSG_SEC_MGR_LOADED); 

    trace(L6, _T("-SMStateBase::SecMgrPostInitialize"));
}

///////////////////////////////////////////////////////////////////////////////
long SMStateBase::SecMgrUnInitialize(void)  // called by MP at shut down
{
  if (!SecMgr.SecMgrICreated)
    return 0;
  // NewSec - to do...need to call UnAdvise first
  SecMgr.SecurityUnadvise(lSecMgrCookie);
  lSecMgrCookie = 0;
  return SecMgr.UnInitialize();
}
#endif //_CPPUNIT
///////////////////////////////////////////////////////////////////////////////
// Handles setting security lights when a security condition is entered.
// 
void SMStateBase::SAEnterSecurityCondition()
{
	trace(L6, _T("::SAEnterSecurityCondition - Saving tri-light color"));
	dm.saveTriColorLightStatus();

	// Turn on red security light.
    
	// (*) SR898 ====================
    DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                       DM_TRICOLORLIGHT_BLINK_1HZ,
                       DM_TRICOLORLIGHT_NORMALREQUEST);
}

///////////////////////////////////////////////////////////////////////////////
// Handles resetting security lights and notifying RAP when a security
// condition is cleared.
// 
void SMStateBase::SAClearSecurityCondition()
{
		trace(L6, _T("::SAClearSecurityCondition - RAP view = %d"), m_View);

        if ( m_View >= 0 )
        {
            //Clear RAP Approval 
            trace(L2, _T("Calling ra.OnNoRAPDataNeeded()"));
            ra.OnNoRAPDataNeeded();  
        }
		ClearSecurityViolationLight();//End of Tar 209034 & 209045
		if(dm.fStateTakeawayBelt)	// is on a take away system
		{
#ifndef _CPPUNIT
			TABTransportClearedItem();	// consolidate dup lines into one procedure LPM090502
#endif //_CPPUNIT
		}
		//218558 removed because not every case of clearing the security violation wants the scanner re-enabled
		// for instance, when the EAS has not really been deactivated and the variable 'MatchedWaitingOnEAS' is true, 
		// then automatically setting the reenable flag to true will cause problems LPM110502 
}
////////////////////////////////////////////////////////////////////////////////////

SMStateBase * SMStateBase::SASecMgrParse( const MessageElement *me)
{

    int nEvent = me->secmgrInfo.nEvt;
	trace(L6, _T("::SASecMgrParse nEvent = %d"), nEvent);

    LPTSTR szParms;
    szParms = new TCHAR [_tcslen(me->secmgrInfo.szParms) + sizeof(TCHAR)];
    _tcscpy( szParms, me->secmgrInfo.szParms );
    delete [] me->secmgrInfo.szParms;

#ifndef _CPPUNIT
	// + RFC 330050
	CString csFLState = this->GetRuntimeClass()->m_lpszClassName;		// get the state of FL	
	CString csSzParms = szParms;	
	CString csExpectedWeight = secEventParms.m_csExpectedWeight;// SSCOP-1208
	if (csSzParms.Find(_T("Application-State=")) != -1)
	{
		int nSzParmsIndex = csSzParms.Find(_T("Application-State=")) + _tcslen(_T("Application-State=")); 
		CString csSecState = csSzParms.Mid(nSzParmsIndex);
			
		if (csFLState.Find(csSecState) == -1)
			trace (L6, _T("FastLane and Security Manager State does not match  -  FL State: %s, SM State: %s"), csFLState, csSecState);
	}
	// - RFC 330050
#endif
    switch (nEvent)
    {
    case SmOnItemOK:
#ifndef _CPPUNIT
		trace(L7, _T("SASecMgrParse - Got SmOnItemOK event"));
		delete [] szParms; // mm185096  TAR 329948

        // SSCOP-1208 +
		if(!csExpectedWeight.IsEmpty())
		{
		   secEventParms.m_csExpectedWeight = EMPTY_STRING;
		}
        // SSCOP-1208 -
		//TAR 298537
		if(dm.fStateTakeawayBelt || (getAnchorState() == BES_BAGANDEAS)) // TAR 402848
		{
			g_bOnItemOK = true;
		}
        else if (g_pDelay_TBExpectedStateChange != NULL)
        {
            CString csStateName = g_pDelay_TBExpectedStateChange->GetRuntimeClass()->m_lpszClassName;
            if(csStateName.Find(_T("BagAndEAS")) != -1)
            {
                g_bOnItemOK = true;
            }
        }

		if(fInMultiSelectPickList)
		{
			g_bOnItemOK = true;
			g_lNumberOfItemsNotBagInMultiPick--;
			return STATE_NULL;
		}


		if(storeMode())
		{
				//TAr390419+
			//there is a racing condition such as OnItemOK event received at Mismatch state
			//but parsed at SmAuthorization state
			if (getAnchorState() == BES_BAGANDEAS)
			{
				setAnchorState(BES_SCANANDBAG);
			}
			//TAr390419-
			return STATE_NULL;
		}
#endif
		//442275+
		//If FL goes to SystemMessage from BagAndEAS, there could be a racing condition:
		//OnItemOk is received after FL goes to SystemMessage
		//We want FL to go to ScanAndBag after it exits SystemMessage since OnItemOK has been arrived.
		{
		CString csCurrentStateName = this->GetRuntimeClass()->m_lpszClassName;
        if(csCurrentStateName.Find(_T("SystemMessage")) != -1)
        {
            g_bOnItemOK = true;
			if (getAnchorState() == BES_BAGANDEAS)
			{
				setAnchorState(BES_SCANANDBAG);
			}
			return STATE_NULL;
		}
		}
		//442575-
#ifndef _CPPUNIT
		// + TAR 294041 - After skip bagging threshold is reached and an immediate wt change 
		// intervention has been approved - do let the threshold intervention to be cleared 
		// off. So if outstanding immediate intervention exists and we haven't cleared the
		// the late one off the stack yet, then stay in the same state
		//
		// TAR 362446 - Updated logic to cover timing issue under different states. Introduced
		// DeliverUpdate function 
		if ((ImmediateInterventionStack.GetCount() >= 1) && !g_bLastImmediateInterventionCleared)
		{
			if(!co.IsSecurityBypassOn())
			{
				if(dm.fStateTakeawayBelt)
				{
					if(getAnchorState()==BES_BAGANDEAS)//added extra condition for TAR 362906
						setAnchorState(BES_TRANSPORTITEM);
					DeliverUpdate(BES_TRANSPORTITEM);
				}
				else
				{
					if(getAnchorState()==BES_BAGANDEAS)//added extra condition for TAR 362906
						setAnchorState(BES_SCANANDBAG);
					DeliverUpdate(BES_SCANANDBAG);
				}
			}
			//SR085 - fix for TAR 314813
			if(secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_MISMATCHED)
				g_bMatchedWtWaitingOnEAS = true;
			//SR085 - fix for TAR 314813

            if ( secEventParms.m_csExceptionType != KEY_OSE_EXC_TYPE_BAG_BOX_THRESHOLD ) //SSCOADK-2240
            {
                OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR,KEY_EXCEPTION_CATEGORY_IMMEDIATE_WT_CHANGES);//439026
            }
			return STATE_NULL;
		}
		// - TAR 294041
		else
		{
//USSF START
			USSFHOOK(_T("OnMatchedWt"), nextState = OnMatchedWt(), return nextState;);
			//return OnMatchedWt();
//USSF END
		}
        break;
    case SmOnTransactionComplete:
		trace(L7, _T("SASecMgrParse - Got SmOnTransactionComplete event"));
        
		delete [] szParms; // mm185096  TAR 329948
		
        //SSCOP-1702+
        {
        CString csStateName = this->GetRuntimeClass()->m_lpszClassName;
		//TAR389664+
        if (  (getAnchorState() == BES_START)
			|| ((getAnchorState() == BES_LANGUAGESELECT) && !bIsTransactionStarted) //
            || (this->csLastNonHelpStateName == _T("SMAttract") && getAnchorState() == BES_SCANANDBAG))//SSCOP-3805
		{
			bQuietNeedSecApproval = false; //UnexpectedIncrease at Attract has been cleared by resetting LGW in SM
			trace(L6, _T("SASecMgrParse - SmOnTransactionComplete: bQuietNeedSecApproval is set to false"));
            if ( (csStateName.Find(_T("RAPDataNeeded")) != -1) && 
                 ( TrainingModeObj.IsTMOn() && !TrainingModeObj.IsTransactionApproved() && !fStartTrxInAM )
	           )    
            {
                trace(L6, _T("ra.OnNoRAPDataNeeded()"));
                ra.OnNoRAPDataNeeded(); //clear the one sent to RCM before
			    trace(L7, _T("+ra.ONRAPDataNeeded, RAP Intervention"));
	            m_View = DATANEEDED_STARTTRAININGTRANS;
	            CString csSecInfo;
	            cDataEntry.FromConfigData(co.GetcsDataEntryINIFile(), _T("StartTrainingModeTrans"));
	            csSecInfo.Format(ps.GetPSText(MSG_DATANEEDED_TRAININGMODE_TRANS, SCOT_LANGUAGE_PRIMARY));
	            ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(),m_View);
	            trace(L7, _T("-ra.ONRAPDataNeeded"));
            }
            else
            {
                UpdateStateName(ps.GetPSText(LTT_ATTRACT, SCOT_LANGUAGE_PRIMARY));
            }
            
            if(csStateName.Find(_T("SecUnExpectedIncrease")) != -1)
                return OnBackToLGW();
            else
                break;
		}
		//TAR389664-
		else
		{
			bTransactionComplete= true;
			m_bSecurityAnalysisComplete = false; 
			g_bSecurityAnalysisTimerExpires = false; 
		}
        }
        //SSCOP-1702-

		return OnTransactionComplete();
        break;
#endif
    case SmOnSecurityException:

#ifndef _CPPUNIT
		trace(L7, _T("SASecMgrParse - Got SmOnSecurityException event"));
		if (_tcslen(szParms) != 0)
		{
            secEventParms.Clear();
			secEventParms.ParseKeys(szParms);
	    }
		delete [] szParms;
#endif
        if(secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_DATANEEDED)
        {
            return OnSecMgrNeedData(secEventParms.m_csExceptionType, secEventParms.m_lItemId);
        }
#ifndef _CPPUNIT
		//if there is a delay, tell RAP
		else if ( secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_DELAYED )
		{
            g_bClearAllDelayedSecIntervention = false;
			lSALoggedMsgs++;
			TellRAPAboutDelaySecurityIntervention();

            //+SR814.5 SR815.6 SR851 Personalization and Security
            if( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_UNEX_INCREASE )
            {
                custInfo.IncrementMetricLong( _T("UnexpectedIncreaseCountDelayed"), 1); 
                trace ( L6, _T("Incremented Value of UnexpectedIncreaseCountDelayed: %d"),custInfo.GetMetricLong(_T("UnexpectedIncreaseCountDelayed")));
            }
            else if ( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_UNEX_DECREASE )
            {
                custInfo.IncrementMetricLong( _T("UnexpectedDecreaseCountDelayed"), 1); 
                trace ( L6, _T("Incremented Value of UnexpectedDecreaseCountDelayed: %d"),custInfo.GetMetricLong(_T("UnexpectedDecreaseCountDelayed")));
            }
            else if ( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_MISMATCHED )
            {
                custInfo.IncrementMetricLong( _T("WeightMismatchCountDelayed"), 1); 
                trace ( L6, _T("Incremented Value of WeightMismatchCountDelayed: %d"),custInfo.GetMetricLong(_T("WeightMismatchCountDelayed")));
            } 
            //-SR814.5 SR815.6 SR851 Personalization and Security

		}
        else if ( (secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_OUTSTANDING) 
			 || ((secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_NO_INTERVENTION_REQ) && (bIsInFinalization  || getAnchorState() == BES_BAGANDEAS) ) )  //SSCOP-1353
		{
			// if immediate intervention, then push it on the ImmediateInterventionStack stack
			SecImmediateIntervention *psecImmediateIntervention = new SecImmediateIntervention;

			psecImmediateIntervention->csSecInterventionType = secEventParms.m_csExceptionType;
			psecImmediateIntervention->lExceptionId = secEventParms.m_lExceptionId;
// 			ImmediateInterventionStack.push(psecImmediateIntervention);
			ImmediateInterventionStack.AddHead(psecImmediateIntervention);

            //+SR814.5 SR815.6 SR851 Personalization and Security
            if( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_UNEX_INCREASE )
            {
                custInfo.IncrementMetricLong( _T("UnexpectedIncreaseCountImmediate"), 1); 
                trace ( L6, _T("Incremented Value of UnexpectedIncreaseCountImmediate: %d"),custInfo.GetMetricLong(_T("UnexpectedIncreaseCountImmediate")));
            }
            else if ( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_UNEX_DECREASE )
            {
                custInfo.IncrementMetricLong( _T("UnexpectedDecreaseCountImmediate"), 1);
                trace ( L6, _T("Incremented Value of UnexpectedDecreaseCountImmediate: %d"),custInfo.GetMetricLong(_T("UnexpectedDecreaseCountImmediate")));
            }
            else if ( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_MISMATCHED )
            {
                custInfo.IncrementMetricLong( _T("WeightMismatchCountImmediate"), 1); 
                trace ( L6, _T("Incremented Value of WeightMismatchCountImmediate: %d"),custInfo.GetMetricLong(_T("WeightMismatchCountImmediate")));
            } 
            //-SR814.5 SR815.6 SR851 Personalization and Security

        }
        //+SR814.5 SR815.6 SR851 Personalization and Security
        else if ( secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_FORGIVEN )
        {           
            if( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_UNEX_INCREASE )
            {
                custInfo.IncrementMetricLong( _T("UnexpectedIncreaseCountForgiven"), 1); 
                trace ( L6, _T("Incremented Value of UnexpectedIncreaseCountForgiven: %d"),custInfo.GetMetricLong(_T("UnexpectedIncreaseCountForgiven")));
            }
            else if ( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_UNEX_DECREASE )
            {
                custInfo.IncrementMetricLong( _T("UnexpectedDecreaseCountForgiven"), 1); 
                trace ( L6, _T("Incremented Value of UnexpectedDecreaseCountForgiven: %d"),custInfo.GetMetricLong(_T("UnexpectedDecreaseCountForgiven")));
            }
            else if ( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_MISMATCHED )
            {
                custInfo.IncrementMetricLong( _T("WeightMismatchCountForgiven"), 1); 
                trace ( L6, _T("Incremented Value of WeightMismatchCountForgiven: %d"),custInfo.GetMetricLong(_T("WeightMismatchCountForgiven")));
            }           
        }
        //-SR814.5 SR815.6 SR851 Personalization and Security

		if ( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_UNEX_INCREASE )
		{
			g_csDeltaWeight = secEventParms.m_csDeltaWeight;	// SR682
			//+RFC374303
			if ( co.GetfAutoRunOn() && co.GetfAutoRunBypassBagging() )
			{
                trace(L6, _T("AutoRun and AutoRunBypassBagging are true, returning STATE_NULL"));
                OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR);
			    return STATE_NULL;

			}//-RFC374303
			else
			{
			    return OnWtIncreaseNotAllowed();
			}
		}
		else if ( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_UNEX_DECREASE )
		{
			g_csDeltaWeight = secEventParms.m_csDeltaWeight;	// SR682
			//+RFC374303
			if ( co.GetfAutoRunOn() && co.GetfAutoRunBypassBagging() )
			{
                trace(L6, _T("AutoRun and AutoRunBypassBagging are true, returning STATE_NULL"));
                OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR);
			    return STATE_NULL;
			}//-RFC374303
			else
			{
			    return OnWtDecrease();
			}
		}
		else if ( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_MISMATCHED )
		{
			// TAR 276027 - disable Skip Bagging button as soon as a mismatch event is
			// received. 
			m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), _T("BagAndEAS"), UI::PROPERTYVISIBLE, VARIANT_FALSE );

			//+MisMatch ItemExceptions for Reporting 3.1 
			trace(L6, _T("tolerance value = %d , type = %s "), secEventParms.m_lTolerance, secEventParms.m_csToleranceType);
            // Save information for later logging
            g_csActualWeight = secEventParms.m_csActualWeight;
            g_csExpectedWeight = secEventParms.m_csExpectedWeight;
			g_csDeltaWeight = secEventParms.m_csDeltaWeight;	// SR682
            g_lTolerance = secEventParms.m_lTolerance;
/*
			trace(L7, _T("+rp.ItemException"));
			CString csItemSku = io.csOrigCode;
			long lPrice = io.lExtPriceEntered;
			CString csMisMatchDesc = io.csOrgItemDescription; //Always use the orginal item description 
			rp.ItemException(csMisMatchDesc ,csItemSku,lPrice, secEventParms.m_csActualWeight, secEventParms.m_csExpectedWeight);
			trace(L7, _T("-rp.ItemException"));
			//-MisMatch ItemExceptions for Reporting 3.1 

			if (co.fOperationsLogWLDBmismatch)
			{
				LogWLDBWeightMismatch(secEventParms.m_csActualWeight, secEventParms.m_csExpectedWeight, secEventParms.m_lTolerance);
			}
*/
			//+RFC374303
			if ( co.GetfAutoRunOn() && co.GetfAutoRunBypassBagging() )
			{
                trace(L6, _T("AutoRun and AutoRunBypassBagging are true, returning STATE_NULL"));
                OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR);
				return STATE_NULL;
			}//-RFC374303
			else
			{
			    return OnWtIncreaseNotAllowed();
			}
		}
		else if ( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_SKIP_BAGGING_THRESHOLD )
		{
			CREATE_AND_DISPENSE(SecSkipBaggingThreshold)(BES_SCANANDBAG);
		}
		else if ( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_ITEM_REMOVED_THRESHOLD )
		{
			g_csDeltaWeight = secEventParms.m_csDeltaWeight;	// SR682
			CREATE_AND_DISPENSE(SecItemRemovedThreshold)(BES_SCANANDBAG);
		}
		else if ( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_BAG_BOX_THRESHOLD )
		{
			//TAR417185
			BEST PrevState = getAnchorState();
			if(bIsInFinalization && PrevState != BES_SCANANDBAG 
			|| bIsInFinalization && PrevState != BES_TRANSPORTITEM)
			{
				CREATE_AND_DISPENSE(SecBagBoxThreshold)(PrevState);
			}
			else
			{
				CREATE_AND_DISPENSE(SecBagBoxThreshold)(getNextStateForGoodItem());
			}
		}
		else if ( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_NEW_ITEM_THRESHOLD )
		{
			CREATE_AND_DISPENSE(SecNewItemEntry)(getNextStateForGoodItem(), BES_SCANANDBAG);
		}
#endif
		//SR085 +
		else if ( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_INVALID_BAG )
		{
#ifndef _CPPUNIT
			CString csCurrentStateName= this->GetRuntimeClass()->m_lpszClassName;
			if (csCurrentStateName.Find(_T("ScanAndBag")) != -1)
			{
				trace(L6, _T("Having back to back invalid bag, need to clear the intervention and stay in Scan and Bag state"));
				OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR);
				return STATE_NULL;
			}
#endif
			// +SSCOP-3327: If in finalization, then go back to where you were.
            BEST anchorState = getAnchorState();
            trace(L6,_T("SASecMgrParse -- SecBagBoxNotValid anchor[%d][%d]"), anchorState, BES_BAGANDEAS);
			if(bIsInFinalization || anchorState == BES_BAGANDEAS)
			{
				CREATE_AND_DISPENSE(SecBagBoxNotValid)(anchorState);
			}
            else
            {
                CREATE_AND_DISPENSE(SecBagBoxNotValid)(BES_SCANANDBAG);
            }
		}
		//SR085 -
        break;
#ifndef _CPPUNIT
    case SmOnSecurityExceptionCleared:
		{
			trace(L7, _T("SASecMgrParse - Got SmOnSecurityExceptionCleared event"));
			SecImmediateIntervention *psecImmediateIntervention = new SecImmediateIntervention;
			SecExceptionClearedEventParms parms(psecImmediateIntervention);

			parms.m_bClearRAPApproval = false;
			if (_tcslen(szParms) != 0)
			{
				parms.ParseRecords(szParms);
			}
			delete [] szParms;
			delete  psecImmediateIntervention;

			if(parms.m_bClearRAPApproval)
			{
				lSALoggedMsgs--;
				if(lSALoggedMsgs == 0)
				{
					trace(L7, _T("+ra.OnSecurityApproval"));
					ra.OnSecurityApproval();
					trace(L7, _T("-ra.OnSecurityApproval"));
                    
					// (*) SR898 ====================
                    DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                                       DM_TRICOLORLIGHT_OFF,
                                       DM_TRICOLORLIGHT_NORMALREQUEST);
				}
			}

			if(storeMode() || ( co.GetfAutoRunOn() && 
                                co.GetfAutoRunBypassBagging() && 
                                parms.m_csExceptionType != KEY_OSE_EXC_TYPE_SKIP_BAGGING_THRESHOLD ) )			{
				return STATE_NULL;
			}
            
            if(parms.m_csDisposition == KEY_OSE_DISPOSITION_DATANEEDED)
            {
                trace(L6, _T("SmOnSecurityExceptionCleared, Disposition= %s"),KEY_OSE_DISPOSITION_DATANEEDED);
                return STATE_NULL;
            }

			//TAR 271768 Check for the app immediate intervention stack.  If this clear message cause by the customer correcting security violation
			// and still have any threshold intervention in the stack, go to that threshold state.
			if ((ImmediateInterventionStack.GetCount() >= 1) && !g_bLastImmediateInterventionCleared)
			{
				SecImmediateIntervention *pTempSecImmediateIntervention;
				// pop the last exception
				pTempSecImmediateIntervention = ImmediateInterventionStack.GetAt( 
					ImmediateInterventionStack.GetHeadPosition() );
				//TAR 277066 Check to see if there is any outstanding intervention left, don't change state
				if (( pTempSecImmediateIntervention->csSecInterventionType == KEY_OSE_EXC_TYPE_MISMATCHED ) ||
					( pTempSecImmediateIntervention->csSecInterventionType == KEY_OSE_EXC_TYPE_UNEX_INCREASE ) ||
					( pTempSecImmediateIntervention->csSecInterventionType == KEY_OSE_EXC_TYPE_UNEX_DECREASE ) )
				{
					OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR, KEY_EXCEPTION_CATEGORY_IMMEDIATE_WT_CHANGES);       
					return STATE_NULL;
				}

				//TAR379716 / 380954 (Fix merged from 4.0E5).
				//There is a very small window that FL is at Threshold intervention state, while RAP is approving the
				//threshold intervention, a weight intervention occured at FL, so FL goes to UnexpectedIncrease, then 
				//FL receives threshold intervention cleared when FL is at UnexpectedIncrease
				CString csCurrentStateName= this->GetRuntimeClass()->m_lpszClassName;
				if ( ( (csCurrentStateName.Find(_T("SecUnexpectedDecrease")) != -1) ||
                       (csCurrentStateName.Find(_T("SecUnExpectedIncrease")) != -1) ||
					   (csCurrentStateName.Find(_T("SecMisMatchWeight")) != -1) 
                     ) &&
					 ( ( parms.m_csExceptionType == KEY_OSE_EXC_TYPE_SKIP_BAGGING_THRESHOLD ) ||
					   ( parms.m_csExceptionType == KEY_OSE_EXC_TYPE_ITEM_REMOVED_THRESHOLD ) ||
					   ( parms.m_csExceptionType == KEY_OSE_EXC_TYPE_BAG_BOX_THRESHOLD ) ||
					   ( parms.m_csExceptionType == KEY_OSE_EXC_TYPE_NEW_ITEM_THRESHOLD)
					 )
                   )
				{
					trace(L2, _T("Calling ra.OnNoRAPDataNeeded()"));
					ra.OnNoRAPDataNeeded();  
					return STATE_NULL;
				}


				SAClearSecurityCondition();
				if ( pTempSecImmediateIntervention->csSecInterventionType == KEY_OSE_EXC_TYPE_SKIP_BAGGING_THRESHOLD )
				{
					CREATE_AND_DISPENSE(SecSkipBaggingThreshold)(BES_SCANANDBAG);
				}
				else if ( pTempSecImmediateIntervention->csSecInterventionType == KEY_OSE_EXC_TYPE_ITEM_REMOVED_THRESHOLD )
				{
					CREATE_AND_DISPENSE(SecItemRemovedThreshold)(BES_SCANANDBAG);
				}
				else if ( pTempSecImmediateIntervention->csSecInterventionType == KEY_OSE_EXC_TYPE_BAG_BOX_THRESHOLD )
				{
					secAppControlEventParms.m_csTare = _T(""); // TAR 294023
					//TAR417185
					BEST PrevState = getAnchorState();
					if(bIsInFinalization && PrevState != BES_SCANANDBAG 
					|| bIsInFinalization && PrevState != BES_TRANSPORTITEM)
					{
						CREATE_AND_DISPENSE(SecBagBoxThreshold)(PrevState);
					}
					else
					{
						CREATE_AND_DISPENSE(SecBagBoxThreshold)(getNextStateForGoodItem());
					}
				}
				else if ( pTempSecImmediateIntervention->csSecInterventionType == KEY_OSE_EXC_TYPE_NEW_ITEM_THRESHOLD )
				{
					CREATE_AND_DISPENSE(SecNewItemEntry)(getNextStateForGoodItem(), BES_SCANANDBAG);
				}
			}
			//TAR 271768

		   	trace(L6, _T("SmOnSecurityExceptionCleared g_bLastImmediateInterventionCleared= %d"),g_bLastImmediateInterventionCleared);
			// if last immediate intervention is cleared and there are delayed interventions
			// still need approval - the bring them up automatically(PM wanted this)
			if (g_bLastImmediateInterventionCleared )
			{
				g_bLastImmediateInterventionCleared = false;
			}
			g_bApproveFromRAP = false;		//Reset this flag no matter what

			CString csStateName= this->GetRuntimeClass()->m_lpszClassName;

			/*if( ((KEY_OSE_EXC_TYPE_UNEX_DECREASE == parms.m_csExceptionType) &&
			   (csStateName.Find(_T("SecItemRemovedThreshold")) != -1) ) ||
			( (KEY_OSE_EXC_TYPE_UNEX_INCREASE == parms.m_csExceptionType) &&
			   (csStateName.Find(_T("SecBagBoxThreshold")) != -1) ) )*/
			
			//TAR377492
			if ((KEY_OSE_EXC_TYPE_UNEX_DECREASE == parms.m_csExceptionType) &&
			   (csStateName.Find(_T("SecItemRemovedThreshold")) != -1) )
			{
				//412670: ScanAndBag-->PickUpItems(after timed out) --> ItemRemoveThreshold;
				//By logging in at lane/or clearing at RAP, SM clears Threshold exception first then
				//clears all wt decrease exceptions. At this point, Threshold exception and 
				//all wt decrease exceptions should be cleared, FL should go to ScanAndBag
				return createState(BES_SCANANDBAG); 
				//return STATE_NULL;
			}
			/* SR085 - This TAR fix removed for SR85. New fix in being put in place
			// +TAR 314813
			if( ((KEY_OSE_EXC_TYPE_MISMATCHED == parms.m_csExceptionType) &&
			   (csStateName.Find(_T("SMBagAndEAS")) != -1) ) )
			{
				return setAndCreateAnchorState(BES_SCANANDBAG);
			}

			// -TAR 314813*/ 

			// +TAR 369810 merged from E5
            if (( parms.m_csExceptionType == KEY_OSE_EXC_TYPE_ITEM_REMOVED_THRESHOLD ) ||   
                ( parms.m_csExceptionType == KEY_OSE_EXC_TYPE_BAG_BOX_THRESHOLD ) ||
                ( parms.m_csExceptionType == KEY_OSE_EXC_TYPE_SKIP_BAGGING_THRESHOLD ))
            {
               BEST PrevState = getNextStateForGoodItem();
               if( parms.m_csExceptionType == KEY_OSE_EXC_TYPE_ITEM_REMOVED_THRESHOLD )
               {
                  PrevState = BES_SCANANDBAG;
               }
               if(PrevState == BES_STATERETURN)
		           return STATE_RETURN;
               if(parms.m_csExceptionType != KEY_OSE_EXC_TYPE_SKIP_BAGGING_THRESHOLD)
               {
                    return setAndCreateAnchorState(PrevState); 
               }
               else
               {
                   if(g_bOnItemOK)
                   {
                       trace(L6, _T("Exception - SkipBaggingThreshold -- g_bOnItemOK true"));
                       return setAndCreateAnchorState(PrevState);
                   }
                   else
                   {
                       trace(L6, _T("Exception - SkipBaggingThreshold -- Wait for ItemOK"));
                       return STATE_NULL;
                   }
               }
            }
            // -TAR 369810
			//USSF Start
			USSFHOOK(_T("OnBackToLGW"), nextState = OnBackToLGW(), return nextState;);
			//return OnBackToLGW();
			//USSF End
		}
        break;
    case SmOnSecurityMaintenance:

		{
			trace(L7, _T("SASecMgrParse - Got SmOnSecurityMaintenance event"));
			SecMaintenanceEventParms parms(&psecMaintenanceRecordArray);
			if (_tcslen(szParms) != 0)
			{
				parms.ParseRecords(szParms);
			}
			delete [] szParms;
			if ( csSMMServerStatus == KEY_OSM_SERVER_STATUS_OFFLINE )
			{
				SASMMSecOffline();
				bSMMFailure = true;
				RETURNSTATE(SmmEditAdd);
			}
			else
			{
				bSMMFailure = false;
			}

			CalcObtainAndTraceSMMWtTol();
            // +TAR 450492 - If an invalid tolerance is entered, no need to reset and repopulate entries list.
            if ((csSMMOperation == KEY_SMM_OPERATION_CHANGE_WT_TOLERANCE) && 
                (csSMMErrorType == KEY_OSM_ERROR_TYPE_INVALID_TOLERANCE ))
            {
                trace(L7,_T("Invalid Weight Tolerance entered. Bypass repopulating the SMM entries list."));
            }
            else
            {
                SASMMDisplayTolInfo();
                SASMMFormatWLDBInfo();
                
                // TAR 452007
                // -TAR 450492
                COleVariant v;
                
                if ( 0 != lSMMWeightCount )
                {
                    v = (long) UI::Normal;
                    
                }
                else
                {
                    v = (long) UI::Disabled;
                    
                }
                COleVariant v1;
                v1 = ps.ConvertToEscapeSequence(ps.GetPSText(BTT_SMM_DELWT),_T("\\n"), _T("\n"));
                m_pAppPSX->SetConfigProperty(_T("SMButton3"), _T("SmmEditAdd"), UI::PROPERTYTEXTFORMAT, v1);
                m_pAppPSX->SetConfigProperty(_T("SMButton3"), _T("SmmEditAdd"), UI::PROPERTYSTATE, v);
            }
			// + TAR 268390 & 268392
			if (csSMMOperation == KEY_SMM_OPERATION_ADD_WEIGHT)
			{
				// + TAR 268387
				CString csFormatedWeight;
				csFormatedWeight.Format(_T("%s %s"), csSMMObservedWeight, csScaleUnits );
				ps.Echo(1, csFormatedWeight );
			    // - TAR 268387
				// After we get the event that weight was added, then set hold weight to true
                //+SSCOADK-1986
				/*CKeyValueParmList parmList;
				CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_HOLD);
				_bstr_t bstrParmList((const _TCHAR *)parmList);
				BSTR bstrResult;
				bstrResult = NULL;
				SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
				SysFreeString(bstrResult);
				Sleep (100);
				ps.Echo(1, _T(""));*/
                //-SSCOADK-1986
				RETURNSTATE(SmmEditAdd)
			}
			// - TAR 268390 & 268392
			if (csSMMOperation == KEY_SMM_OPERATION_CHANGE_WT_TOLERANCE)
			{
				if ( csSMMErrorType == KEY_OSM_ERROR_TYPE_INVALID_TOLERANCE )
				{
					csSMMErrorType = _T("");
					RETURNSTATE(SmmInvalidWtTol);
				}
				else
				{
					ps.PSHideSMMNumericKeypad();
					PSClearInput();
					RETURNSTATE(SmmEditAdd)
				}
			}
			if ( 0 != lSMMWeightCount )
			{
				delete [] psecMaintenanceRecordArray;
			}
			lSMMWeightCount = 0;
		}

        break;
    case SmOnDeviceFailure:
		{
			trace(L7, _T("SASecMgrParse - Got SmOnDeviceFailure event"));
			SecurityDeviceFailureEventParms secDeviceFailureEventParms;

			if (_tcslen(szParms) != 0)
			{
				secDeviceFailureEventParms.ParseKeys(szParms);
			}
			delete [] szParms;
			if ( secDeviceFailureEventParms.m_csDeviceName == VALUE_DEVICE_SS )
			{
				return OnSecurityScaleFailure(secDeviceFailureEventParms.m_lErrorCode );
			}
			else if ( secDeviceFailureEventParms.m_csDeviceName == VALUE_DEVICE_DBMGRCLIENT )
			{
				return OnDBMgrFailure(secDeviceFailureEventParms.m_lErrorCode, secDeviceFailureEventParms.m_lExtendedErrorCode );
			}
			else if ( secDeviceFailureEventParms.m_csDeviceName == VALUE_DEVICE_TAKEAWAY_BELT )
			{
				if ( secDeviceFailureEventParms.m_lExtendedErrorCode == 0 )
				{
					g_bSafetyCoverClosed = true;
					trace(L6, _T("DM Takeaway Belt detected safety cover closed"));
					trace(L6,_T("-Parse"));
					return DMTakeawaySafetyCoverOpen(false);
				}
				if ( secDeviceFailureEventParms.m_lExtendedErrorCode == 1 )
				{
					g_bSafetyCoverClosed = false;
					trace(L6, _T("DM Takeaway Belt detected safety cover opened"));
					trace(L6,_T("-Parse"));
					return DMTakeawaySafetyCoverOpen(true);
				}
			}

		}
	    break;

    case SmOnInitializationComplete:
		trace(L7, _T("SASecMgrParse - Got SmOnInitializationComplete event"));
		delete [] szParms; // mm185096  TAR 329948
        break;
	case SmOnAppControl:
		trace(L7, _T("SASecMgrParse - Got SmOnAppControl event"));

		if (_tcslen(szParms) != 0)
		{
			secAppControlEventParms.ParseKeys(szParms);
	    }
		delete [] szParms;
		{ 
			// + RFC 330050 
			CString csApplicationState = secAppControlEventParms.m_csApplicationState;									
			
			if (  (secAppControlEventParms.m_csOperation == KEY_OAC_OPERATION_ITEM_REMOVED) 
			   || (secAppControlEventParms.m_csOperation == KEY_OAC_OPERATION_SKIP_BAGGING)
			   )			  
			{
				if (  (csFLState.Find(csApplicationState) != -1 ) )
				{
					trace (L6, _T("SMOnAppControl- PsButton1 clicked. The current state matches the state in Security Manager -   FastLane State: %s Security Manager State = %s "), csFLState, csApplicationState);
					return PSButton1();
				}
				else
				{
					trace (L6, _T("SMOnAppControl - PSButton1 not clicked.  State in Security Manager: %s   Current State in Lane: %s   Operation: %s "), csApplicationState, csFLState, secAppControlEventParms.m_csOperation);
				}
				
			}
			// - RFC 330050 
		}

		//SR085 +
		//Handling the newly added operation KEY_OAC_OPERATION_CUST_BAG
		if (secAppControlEventParms.m_csOperation == KEY_OAC_OPERATION_CUST_BAG)
		{
			if (secAppControlEventParms.m_csTare == KEY_OAC_TARE_VALID_CUST_BAG)
			{
				return TareOnlyAtAttract();
			}
			else if (secAppControlEventParms.m_csTare == KEY_OAC_TARE_NONE)
			{
				return TareNone();
			}
		}
		//SR085 -

		if (secAppControlEventParms.m_csOperation == KEY_OAC_OPERATION_BAG_BOX)
		{
			if (secAppControlEventParms.m_csTare == KEY_OAC_TARE_WEIGHTED_ITEM)
			{
				return TareWeightedItem();
			}
			else if (secAppControlEventParms.m_csTare == KEY_OAC_TARE_TARE_ONLY)
			{
				return TareOnly();
			}
			else if (secAppControlEventParms.m_csTare == KEY_OAC_TARE_WLDB_ITEM )
			{
				return TareWLDBItem();
			}
			else if (secAppControlEventParms.m_csTare == KEY_OAC_TARE_NONE )
			{
				return TareNone();
			}
		}
		
		//+SR778
		if(secAppControlEventParms.m_csOperation == KEY_AC_SKIP_BAG_BTTN_ENABLE 
			&& co.getTimeOutEnableSkipBaggingButtonTimer() > 0 && co.GetfOperationsAllowSkipBaggingBtn())
		{	
			
			StartTimerForSkipBaggingBtn();
			return STATE_NULL;
		
		}
		//-SR778

        //handle only bad trust level from stoplift if SecureCam is enabled and ReportTrustLevel is enabled.
        if(secAppControlEventParms.m_csOperation == KEY_AC_BAD_TRUST_LEVEL && g_bReportTrustLevel && g_bIsSecureCamEnable)
        {
            trace(L6, _T("KEY_AC_BAD_TRUST_LEVEL is received."));

            return GenerateTrustLevelDataNeeded();
        }

		if(secAppControlEventParms.m_csOperation == KEY_AC_TRANS_ANALYSIS_COMPLETE)
		{
			trace(L6, _T("Stoplift Transaction Analysis Complete."));
			m_bSecurityAnalysisComplete  = true;
			CString csCurrentStateName;
			csCurrentStateName = this->GetRuntimeClass()->m_lpszClassName;
			if ( csCurrentStateName.Find(_T("SecurityAnalysis")) != -1 ) 
			{
				trace(L6, _T("We are still at SecurityAnalysis state."));
				RETURNSTATE(Finish);
			}
            return STATE_NULL;
		}
        else if(secAppControlEventParms.m_csOperation == KEY_AC_SL_OFFLINE)
        {
            trace(L6, _T("Stoplift Server Offline."));
            g_bSLServerStatus  = false;
            return STATE_NULL;
        }
        else if(secAppControlEventParms.m_csOperation == KEY_AC_SL_ONLINE)
        {
            trace(L6, _T("Stoplift Server Online."));
            g_bSLServerStatus  = true;
            return STATE_NULL;
        }

        break;
#endif
    case SmItemPickFinish://SR679
#ifndef _CPPUNIT
		trace(L7, _T("SASecMgrParse - Got SmItemPickFinish event"));
		delete [] szParms;
        fInMultiSelectPickList = false; //429915
#endif
		if(!co.IsInAssistMenus())
		{
			if (ps.GetPicklistItemsSold() > 0)
			{
#ifndef _CPPUNIT
				//TAR 418258
				if(!io.d.fCoupon)
				{
					ps.SetPicklistItemsSold(0);
				}
				ps.SetPicklistItemsIncludeVoidItems(0);
#endif
				if((g_lNumberOfItemsNotBagInMultiPick <= 0) || co.IsSecurityDisableOn())
				{
					return setAndCreateAnchorState(BES_SCANANDBAG);
				}
#ifndef _CPPUNIT
				else
				{
					//SSCOP-586
					if(io.fVoid)
					{
						m_pAppPSX->ClearReceipt(_T("CMSmallReceipt"));
					}
					g_lNumberOfItemsNotBagInMultiPick = 0;
					if(g_bAllowLateBagging || g_bNoBaggingRequiredForTheCurrentItem)//SR078
					{
						g_bNoBaggingRequiredForTheCurrentItem = false;
						return setAndCreateAnchorState(BES_SCANANDBAG);
					}
					else
					{
						RETURNSTATE(BagAndEAS);
					}
				}
#endif
			}
#ifndef _CPPUNIT
			else
			{
				g_lNumberOfItemsNotBagInMultiPick = 0;
				ps.SetPicklistItemsSold(0);
				ps.SetPicklistItemsIncludeVoidItems(0);
				return setAndCreateAnchorState(BES_SCANANDBAG);
			}
#endif
		}
#ifndef _CPPUNIT
		else
		{
			g_lNumberOfItemsNotBagInMultiPick = 0;
		 	ps.SetPicklistItemsSold(0);
			ps.SetPicklistItemsIncludeVoidItems(0);
			trace(L6,_T("+SMStateSABase::SASecMgrParse() In Assist Menu do nothing when exiting out Multi Pick Mode"));
			return STATE_NULL;
		}
        break;
#endif
	case SmOnSecurityControl:
#ifndef _CPPUNIT
		trace(L6, _T("SASecMgrParse - Got SmOnSecurityControl event"));
		SecurityControlEventParms secControlEventParms;
		if (_tcslen(szParms) != 0)
		{
			secControlEventParms.ParseKeys(szParms);
	    }
		delete [] szParms;

        if(secControlEventParms.m_csOperation == KEY_OSC_FORCE_DISABLE_SECURECAM)
        {
            trace(L6, _T("Forcing g_bIsSecureCamEnable to false"));
            g_bIsSecureCamEnable = false;
            return STATE_NULL;
        }
        //TAR 407200
        else if(secControlEventParms.m_csOperation == KEY_OSC_OPERATION_SKIP_BAGGING_ALLOWED)
		{
			return OnSkipBaggingAllowed();
		}


		else if(secControlEventParms.m_csOperation == KEY_OSC_OPERATION_UNEX_INCREASE)
		{
			//389664+
			if (((getAnchorState() == BES_START) || ((getAnchorState() == BES_LANGUAGESELECT) && !bIsTransactionStarted)) && !storeMode() && !dm.fStateTakeawayBelt)  //409489 //SSCOP-1405 & SSCOP-143/SSCOP-1287 //SSCOADK-328
			{
                if (  !co.GetfAutoRunOn() || (co.GetfAutoRunOn() && !co.GetfAutoRunBypassBagging()) )  //SSCOB-610 //SSCOP-3552
                {
				    bQuietNeedSecApproval = true; 
				    trace(L6, _T("SmOnSecurityControl: bQuietNeedSecApproval is set to true"));
                }			
			}
			//389664-

			if(storeMode()) 
			{
				CString csCurrentStateName;
				csCurrentStateName = this->GetRuntimeClass()->m_lpszClassName;
				if ( csCurrentStateName.Find(_T("SmmEditAdd")) != -1 ) 
				{
					// + TAR  407230 
					 bQuietNeedSecApproval = false;  		
					 trace(L6, _T("SmOnSecurityControl: bQuietNeedSecApproval is set to false"));	 
					// - TAR  407230

					if ( secControlEventParms.m_csWeightObserved.IsEmpty() )
					{
						ps.Echo(1, _T("") );
					}
					else
					{
						CString csFormatedWeight;
						csFormatedWeight.Format(_T("%s %s"), secControlEventParms.m_csWeightObserved, csScaleUnits );
						ps.Echo(1, csFormatedWeight );
					}
				}
			}
			else
			{

				CString csInstruction;
				csInstruction.Format(ps.GetPSText(RA_WEIGHTINCREASEINFO, SCOT_LANGUAGE_PRIMARY), 
									 _T(""), 
									 _T(""), 
									 ps.GetPSText(RA_WTINCREASE, SCOT_LANGUAGE_PRIMARY), 
									 secControlEventParms.m_csWeightObserved, 
									 csScaleUnits, 
									 _T(""));
				trace(L7, _T("+ra.OnNormalItem, Message = %s"), csInstruction);
				ra.OnNormalItem(csInstruction);
				trace(L7, _T("-ra.OnNormalItem"));
			}
		}
		else if(secControlEventParms.m_csOperation == KEY_OSC_OPERATION_ENABLE_SCANNER)
		{
			g_bOnScannerEnable = true;
			return DMTakeawayReadyForItem();
		}
		else if(secControlEventParms.m_csOperation == KEY_SC_OPERATION_ASSIST_MODE_STOP)
		{
			return DMTakeawayAMExitDone();
		}
		else if(secControlEventParms.m_csOperation == KEY_OSC_OPERATION_BAG_AREA_BACKUP)
		{
            trace(L6,_T("DM Takeaway Belt detected bag area backup"));
			return DMTakeawayBagAreaBackup(true);
		}
		else if(secControlEventParms.m_csOperation == KEY_OSC_OPERATION_BAG_AREA_CLEAR)
		{
            trace(L6,_T("DM Takeaway Belt detected bag area clear"));
            return DMTakeawayBagAreaBackup(false);
		}
		else if(secControlEventParms.m_csOperation == KEY_OSC_OPERATION_ASSIST_MODE_STOP_COMPLETE )
		{
			trace(L6, _T("DM Takeaway Belt assist mode exit done"));
            return DMTakeawayAMExitDone();
		}
		//410299+
		else if(secControlEventParms.m_csOperation == KEY_OSC_OPERATION_GOTO_SCANANDBAG ) 
		{
			trace(L6, _T("KEY_OSC_OPERATION_GOTO_SCANANDBAG is received at SMStateBase"));
            return STATE_NULL;
		}
        //410299-
        //Process Transaction Policy message
		else if(secControlEventParms.m_csOperation == KEY_OSC_OPERATION_TRXPOLICY_READY)
		{
            //Assign Scotopt options
            g_bDelayedSecIntervention = GetSecurityConfig(CONFIG_KEY_DELAY_EXCEPTIONS);
            g_bSubstCkPriceEmbeddedWeights = GetSecurityConfig(CONFIG_KEY_SUBST_CK_PRICE_EMBEDDED_WT);
	        trace(L6, _T("SubstCkPriceEmbeddedWeights items = %d."), g_bSubstCkPriceEmbeddedWeights);
	        g_bIsForgiveUnexpectedDecreaseDuringTendering = GetSecurityConfig(CONFIG_KEY_FORGIVE_UNEX_DEC_DURING_TENDERING);
			g_bIsSecurityDisableSubCheckOnly = GetSecurityConfig(CONFIG_KEY_SECURITY_DISABLE_SUBSTITUTION_CHECK);

            if( GetSecurityConfigValue(CONFIG_KEY_TARE_WT_1) != _T("") )
	        {
		        g_csTareWt1 = GetSecurityConfigValue(CONFIG_KEY_TARE_WT_1); 
	        }
	        if( GetSecurityConfigValue(CONFIG_KEY_TARE_WT_2) != _T("") )
	        {
		        g_csTareWt2 = GetSecurityConfigValue(CONFIG_KEY_TARE_WT_2); 
	        }
	        if( GetSecurityConfigValue(CONFIG_KEY_TARE_WT_3) != _T("") )
	        {
		        g_csTareWt3 = GetSecurityConfigValue(CONFIG_KEY_TARE_WT_3); 
	        }
            if(fLoyaltyCardTrx || g_bIsSecurityTransactionPolicyReady)
            {
       	        trace(L6, _T("Loyalty Card or already have a policy"));
                return STATE_NULL;
            }

			if(fSAInTransaction)	//SSCOADK-2090
			{
				g_bIsSecurityTransactionPolicyReady = true; 
			}

			CString csCurrentStateName = this->GetRuntimeClass()->m_lpszClassName; // SSCOP-852
	        // if item is scanned from attract, wait to go to proper sale screen
			//SSCOB-567
			// SSCOP-852 - check also if currentstate is on SMCmDataEntry.
			// don't go to ScanAndBag if currentstate is on SMCmDataEntry
	        if (fItemIsWaiting 
				&& !isAnySoldVoidedItems() 
				&& !(co.IsDegradedModeOn() && dm.GetDegradedModeType() != DEGRADEDMODE_OFF)
				&& csCurrentStateName.Find(_T("SMCmDataEntry")) == -1 /*SSCOP-852*/ ) 
	        {
	          setAnchorState(BES_SCANANDBAG);
              if(fInSignedOn)
              {
                  if( co.IsDegradedModeOn() && dm.GetDegradedModeType() != DEGRADEDMODE_OFF )
                  {
                      RETURNSTATE(DegradedContinueTrans)
                  }
                  
#endif //CPPUNIT
                  if (m_bScanOnAttractAtQuietMode && !fStateAssist)
                  {
                      if ((g_bIsSecureCamEnable && g_bSLServerStatus))
                      {
                          m_bScanOnAttractAtQuietMode = false;
                          bQuietNeedSecApproval = false;
                      }
                      else
                          return setAndCreateAnchorState(BES_SCANANDBAG); //wait here for SM to fire exception
                  }
#ifndef _CPPUNIT

                  SMStateBase* retState = TBItemSale(false,0,true);
                  // go to inprogress
                  return retState;
              }
			  else
			  {
				  // TB not ready yet on async TBStart.
 			      trace(L6, _T("Waiting for READY from TB (async TBStart)"));
		          return STATE_NULL;
			  }
	        }
            else if(!fInSignedOn)
            {
                return STATE_NULL;
            }
            else
            {
                if(!co.fOperationsCustomerBagAllowed ||  (getAnchorState() != BES_PUTBAGONSCALE && g_savedAnchorStateFromAttract != BES_PUTBAGONSCALE )  ) //416582
                {
			        //+degraded mode tars
			        if( co.IsDegradedModeOn() &&
				        dm.GetDegradedModeType() != DEGRADEDMODE_OFF /*&&
				        getAnchorState() == BES_CMDATAENTRY TAR442912*/)
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
                      if(!co.IsSecurityBypassOn() && !(co.IsDegradedModeOn() && //SSCOADK-3166
		                (dm.GetDegradedModeType() != DEGRADEDMODE_OFF)))
                      {
                            CKeyValueParmList parmList;
                            CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_CUST_HAS_BAG);
                            _bstr_t bstrParmList((const _TCHAR *)parmList);
                            BSTR bstrResult;
                            bstrResult = NULL;
                            SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
                            SysFreeString(bstrResult);
 
                            g_savedAnchorStateFromAttract = BES_START;
		                    return setAndCreateAnchorState(BES_PUTBAGONSCALE);
                      }
                      else
                      {
                          return setAndCreateAnchorState(BES_SCANANDBAG); // first state is S&B
                      }
                }
            }
        }
		else if(secControlEventParms.m_csOperation == KEY_OSC_OPERATION_ALLOWLATEBAGGING_GOTO_SCANANDBAG ) 
		{
			trace(L6, _T("KEY_OSC_OPERATION_ALLOWLATEBAGGING_GOTO_SCANANDBAG is received at SMStateBase"));
			g_bAllowLateBagging = true;
            return setAndCreateAnchorState(BES_SCANANDBAG);
		}
		else if(secControlEventParms.m_csOperation == KEY_OSC_OPERATION_LATEBAGGINGNOTALLOWED ) 
		{
			trace(L6, _T("KEY_OSC_OPERATION_LATEBAGGINGNOTALLOWED is received at SMStateBase"));
			g_bAllowLateBagging = false;
            if(!co.IsBagScaleDisabled())
            {
                ShowDoNotBagOnEReceipt();
            }
            if(fItemIsWaiting)
			{
			  setAnchorState(BES_SCANANDBAG);
	          SMStateBase* retState = TBItemSale(false,0,true);
	          // go to inprogress
	          return retState;
			}
			else
			{
			  return STATE_NULL;
			}
		}
		else if(secControlEventParms.m_csOperation == KEY_OSC_OPERATION_GOTO_RECALLINGITEMS ) 
		{
			trace(L6, _T("KEY_OSC_OPERATION_GOTO_RECALLINGITEMS is received at SMStateBase"));
            return STATE_NULL;
		}


        break;

#endif
    }
	return STATE_NULL;
}

////////////////////////////////////////////////////////////////////////////////////
// called by Core App to tell Security which input to process next
////////////////////////////////////////////////////////////////////////////////////
long SMStateBase::SASendSCOTInput(	long lInputID,
		    						LPCTSTR strUPCNumber,
    								long lWeight,
 				    				long lPrice,
									long lQuantity,
									long lDepartment,
									bool bIsPriceEmbedded  )		
{
    
  nSAPrevSCOTInputID = lInputID;
  CKEYVALUEPARMLIST parmList;
  // +TAR 373654
  trace(L5,_T("SASendSCOTInput lInputID: %d"), lInputID);
  trace(L5,_T("fStateAssist=%d, coIsSecurityBypassOn()=%d, dm.fStateTakeawayBelt=%d"), fStateAssist, co.IsSecurityBypassOn(), dm.fStateTakeawayBelt);
 // -TAR 373654

  switch (lInputID)
  {
#ifndef _CPPUNIT
    case ITEMIZATION:
	{
      if(!fSAInTransaction)
      {
          fSAInTransaction = true;
	      CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_TC_OPERATION_BEGIN);
	      CKeyValue<int>::Put(parmList, KEY_TC_IDLE_TIME, nTimeAttractIdleMsec);
          CString csTerminal;
	      csTerminal.Format(_T("%2li"), TBGetTerminalNumber());
          CKeyValue<const _TCHAR*>::Put(parmList, KEY_TC_TERMINAL_NUMBER, csTerminal);
          //CString csTransactionID = _T("");
          //csTransactionID.Format(_T("%s"), rp.GetTransactionNumber());

          CKeyValue<const _TCHAR*>::Put(parmList, KEY_TC_TRANSACTION_ID, rp.m_csTransactionID);
          if(co.IsPicklistAssistEnable())
          {
              CKeyValue<const _TCHAR*>::Put(parmList, KEY_PICKLISTASSIST, KEY_PLA_ENABLE);
          }

	      _bstr_t bstrParmList((const _TCHAR *)parmList);
	      SecMgr.TransactionControl(lSecMgrCookie, bstrParmList);
          nTimeAttractIdleMsec = 0;
    //	  appBusy.SetBusyStatus(co.getTimeOutBusyLeaseTimer()); //bwd - take delay out of sec flow
	      m_bSecurityAnalysisComplete = false; 
		  g_bSecurityAnalysisTimerExpires =false;

          CLaneStateData laneStateData(m_pAppPSX);
          laneStateData.SendLaneStateData(_T("ScanAndBag"), _T(""), false, false);
          //laneStateData.SendNextGenUIData(_T("NextGenUIApplicationState"), (CString) laneStateData.INTRANSACTION);

      }

	}
      break;
	case MAINTENANCE_MODE:						// Security Maintenance Mode
		bSecMaintMode = true;
		break;
	case TRANSACTION_COMPLETE: // state
		{
        fSAInItemSale = false;//SSCOP-3383 fail-safe resetting of flag
		fSAInTransaction = false;
        //+SR814.5 SR815.6 SR851 Personalization and Security
        CString csTerminal;
        csTerminal.Format(_T("%2li"), TBGetTerminalNumber());
        CKeyValue<const _TCHAR*>::Put(parmList, KEY_TC_TERMINAL_NUMBER, csTerminal);
        //-SR814.5 SR815.6 SR851 Personalization and Security

		CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_TC_OPERATION_END);
        if(co.IsPicklistAssistEnable())
        {
            CKeyValue<const _TCHAR*>::Put(parmList, KEY_PICKLISTASSIST, KEY_PLA_ENABLE);
        }
		_bstr_t bstrParmList((const _TCHAR *)parmList);
	    SecMgr.TransactionControl(lSecMgrCookie, bstrParmList);
		}
		break;
	case TENDER_START:			// state
	{
	  CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_TC_OPERATION_FINALIZE);
	  _bstr_t bstrParmList((const _TCHAR *)parmList);
	  SecMgr.TransactionControl(lSecMgrCookie, bstrParmList);
	}
		break;
#endif //_CPPUNIT
	case TRANSACTION_CONTINUE:			// state
	{
        if (g_bInRecallMode)
        {
            CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_ENTER_RECALLMODE);
            _bstr_t bstrParmList((const _TCHAR *)parmList);
            BSTR bstrResult;
            bstrResult = NULL;
            SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
            SysFreeString(bstrResult);
        }
        else
        {
            CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_TC_OPERATION_CONTINUE);
            _bstr_t bstrParmList((const _TCHAR *)parmList);
            SecMgr.TransactionControl(lSecMgrCookie, bstrParmList);
        }
	  //m_bSecurityAnalysisComplete = false;
	  //g_bSecurityAnalysisTimerExpires = false;
	}
		break;
#ifndef _CPPUNIT
	case UPC_SCANNED:			// ok
	case UPC_MANUAL:			// ok
		{
		if(!(fStateAssist || co.IsSecurityBypassOn()) || dm.fStateTakeawayBelt)
		{
			g_bAllowLateBagging = false;
			fSAInItemSale = true;	// fix no response for items like quantity needed
			// +TAR 321239/320705  If it is InStoreBarCode, we need to replace  
			if (io.b_gHasCustomCodeBeenFound)//RFC239992 Use custom code regardless of Price Embedded 
			{
				csItemSent = io.csItemCodeVariablePartZeroed;
			}// -TAR 321239/320705
            // +TAR 373654
            else if(io.cScanType > 0 && !io.csItemCode.IsEmpty() && io.d.nCrateableScreenAnswer != CRATEABLE_SCREEN_NOT_ENTERED) //TAR 373654 / 396440
            {
               csItemSent = io.csItemCode;
               trace(L5,_T("Setting csItemSent to io.csItemCode"));
            }
            // -TAR 373654

			CKeyValue<const _TCHAR*>::Put(parmList, KEY_UPC, AdjustBarCode());
			if(UPC_SCANNED == lInputID)
			{
				CKeyValue<const _TCHAR*>::Put(parmList, KEY_UPC_ENTRY, KEY_UPC_ENTRY_SCAN);
			}
			else if(io.d.bPickListItem  || SMStateBase::ms_bIsCurrentItemFromPickList) //400230
			{
				if (fInMultiSelectPickList)
				{
					g_lNumberOfItemsNotBagInMultiPick++;
				}
				CKeyValue<const _TCHAR*>::Put(parmList, KEY_UPC_ENTRY, KEY_UPC_ENTRY_PICK);
			}
			else
			{
                if (fInMultiSelectPickList)
				{
					g_lNumberOfItemsNotBagInMultiPick++;
				}
				CKeyValue<const _TCHAR*>::Put(parmList, KEY_UPC_ENTRY, KEY_UPC_ENTRY_KEYED);
			}
			_bstr_t bstrParmList((const _TCHAR *)parmList); 
			SecMgr.ItemSold(lSecMgrCookie, bstrParmList);
		}
		}
		break;
	case ITEM_SOLD_SCAN:
	case ITEM_SOLD_MANUAL:
		{
		fSAInItemSale = false;//TAR 196952
		if(!(fStateAssist || co.IsSecurityBypassOn()) || dm.fStateTakeawayBelt  )
		{
			// +TAR 321239/320705  If it is InStoreBarCode, we need to replace  
			if (io.b_gHasCustomCodeBeenFound)//RFC239992 Use custom code regardless of Price Embedded 
			{
				csItemSent = io.csItemCodeVariablePartZeroed;
			}// -TAR 321239/320705
            // +TAR 373654
            else if(io.cScanType > 0 && !io.csItemCode.IsEmpty() && io.d.nCrateableScreenAnswer != CRATEABLE_SCREEN_NOT_ENTERED) //TAR 373654 / 396440
            {
               csItemSent = io.csItemCode;
               trace(L5,_T("Setting csItemSent to io.csItemCode"));
            }
            // -TAR 373654

            CKeyValue<const _TCHAR*>::Put(parmList, KEY_UPC, AdjustBarCode());
            CKeyValue<int>::Put(parmList, KEY_ITEM_QUANTITY, lQuantity);
            CKeyValue<const _TCHAR*>::Put(parmList, KEY_ITEM_ID, io.d.csItemID);
            CKeyValue<const _TCHAR*>::Put(parmList, KEY_ITEM_DETAILS_FLAGS, KEY_ITEM_DETAILS_SELLABLE);
            CKeyValue<const _TCHAR*>::Put(parmList, KEY_ITEM_DETAILS_DEPT_ID, g_csSavedDeptInfo); // TAR 294001
            //checking for tolerance, flags from TB
            if( (io.d.lRequireSecBagging != TB_LET_SECURITY_DECIDE) || 
                (io.d.lRequireSecSubstChk != TB_LET_SECURITY_DECIDE) )
            {
                // now, change lRequireSecBagging and lRequireSecSubstChk to 
                // Smart Scale 'accepted' values of 1 (true) & 0 (false)
                if(io.d.lRequireSecBagging == TB_OVERRIDE_SECURITY_WITH_NO) // Means to use the TB overridden value of NO
                {
                    io.d.lRequireSecBagging = 0;
                    CKeyValue<const _TCHAR*>::Put(parmList, KEY_ITEM_DETAILS_FLAGS, _T("no-bagging-required"));
                }
                else if(io.d.lRequireSecBagging == TB_OVERRIDE_SECURITY_WITH_YES)   // Means to use the TB overridden value of YES
                {
                    io.d.lRequireSecBagging = 1;
                    CKeyValue<const _TCHAR*>::Put(parmList, KEY_ITEM_DETAILS_FLAGS, KEY_ITEM_DETAILS_BAGGING);
                }
                else if(io.d.lRequireSecBagging == TB_OVERRIDE_SECURITY_BYPASSBAGGINGPROMPT)    // Means to use the TB overridden value of YES
                {
                    io.d.lRequireSecBagging = 5;
                    CKeyValue<const _TCHAR*>::Put(parmList, KEY_ITEM_DETAILS_FLAGS, KEY_ITEM_DETAILS_BYPASSBAGGINGPROMPT);
                }
                
                else // make it valid - failsafe CMal010603 TAR223769
                    io.d.lRequireSecBagging = -1;

                if(io.d.lRequireSecSubstChk == TB_OVERRIDE_SECURITY_WITH_NO)    // Means to use the TB overridden value of NO
                {
                    io.d.lRequireSecSubstChk = 0;
                    CKeyValue<const _TCHAR*>::Put(parmList, KEY_ITEM_DETAILS_FLAGS, _T("no-substitution-check-required"));
                }
                else if(io.d.lRequireSecSubstChk == TB_OVERRIDE_SECURITY_WITH_YES)  // Means to use the TB overridden value of YES
                {
                    io.d.lRequireSecSubstChk = 1;
                    CKeyValue<const _TCHAR*>::Put(parmList, KEY_ITEM_DETAILS_FLAGS, KEY_ITEM_DETAILS_SUBSTITUTION_CHECK);
                }
                else // make it valid - failsafe CMal010603 TAR223769
                    io.d.lRequireSecSubstChk = -1;
                    
                    
            }

            //SR814.4
            if(co.GetSecurityDisableSubCheckOn())
            {
                io.d.lRequireSecSubstChk = 0;
                CKeyValue<const _TCHAR*>::Put(parmList, KEY_ITEM_DETAILS_FLAGS, _T("no-substitution-check-required"));  
            }

            if (io.b_gIsThisItemPriceEmbedded)
            {
                CKeyValue<const _TCHAR*>::Put(parmList, KEY_ITEM_DETAILS_FLAGS, KEY_ITEM_DETAILS_PRICE_EMBEDDED);
                if (io.d.lUnitPrice == 0 ) //tar400229
                    CKeyValue<const _TCHAR*>::Put(parmList, KEY_ITEM_DETAILS_FLAGS, KEY_ITEM_DETAILS_ZEROUNITPRICE);
                if (io.l_gPriceEmbeddedWt > 0) 
                    CKeyValue<int>::Put(parmList, KEY_ITEM_DETAILS_PRICE_EMBEDDED_WT, io.l_gPriceEmbeddedWt);
            }
            
            if(io.l_gPriceEmbeddedWt > 0)
            {
                CKeyValue<int>::Put(parmList, KEY_ITEM_DETAILS_GROSS_WEIGHT, io.l_gPriceEmbeddedWt);
            }
            else if(io.lTotalWeightEntered > 0)
            {
                CKeyValue<int>::Put(parmList, KEY_ITEM_DETAILS_GROSS_WEIGHT, io.lTotalWeightEntered);
            }

            if(io.d.bZeroWeightItem)
            {
                CKeyValue<const _TCHAR*>::Put(parmList, KEY_ITEM_DETAILS_FLAGS, KEY_ITEM_DETAILS_ZERO_WT);
            }
            CKeyValue<const _TCHAR*>::Put(parmList, KEY_ITEM_DETAILS_DESCRIPTION, io.d.csDescription);

            CString csRequestID;
            csRequestID.Format(_T("%d"), g_lPLAResponseTimerID);
            CKeyValue<const _TCHAR*>::Put(parmList, KEY_REQUEST_ID, csRequestID);
            
            if(co.IsPicklistAssistEnable())
            {
                CKeyValue<const _TCHAR*>::Put(parmList, KEY_MESSAGE_TYPE, KEY_PLA_ITEMSOLD);
                CKeyValue<const _TCHAR*>::Put(parmList, KEY_PICKLISTASSIST, KEY_PLA_ENABLE);
            }

            _bstr_t bstrParmList((const _TCHAR *)parmList); 
            SecMgr.ItemDetails(lSecMgrCookie, bstrParmList);
            StartNewPLAResponseTimer();
        }
        }
        break;
    case CUSTOMER_MODE:
        //RFC 263404
        if (!fSAInTransaction)
        {
            appBusy.ClearBusyStatus();
        }
        //RFC 263404
        fSAInCustMode = true;
        fSAInStoreMode = false;
		break;
	case STORE_MODE:
        appBusy.SetBusyStatus(co.getTimeOutBusyLeaseTimer());  //RFC 263404
        fSAInCustMode = false;
        fSAInStoreMode = true;
		bEASReEnableScanner = true;
		g_bWtExpected = false;

		//security violation event will not fired when get out of storemdoe if has the following line 
		//ss.SetHoldWeight(false);   
		break;
	case ITEM_VOID_SCAN:
	case ITEM_VOID_MANUAL:
	case COUPON_VOID_SCAN:
	case COUPON_VOID_MANUAL:
		{
		g_lNumberOfWLDBEntriesThisItem = -1;//TAR 196952
		
		g_bWtExpected = false;

		bEASReEnableScanner = true;
		fSAInItemSale = false; // Fix coupon scanning from RAP issue (TAR: 209401)
		// +TAR 321239/320705  If it is InStoreBarCode, we need to replace  
		if (io.b_gHasCustomCodeBeenFound)//RFC239992 Use custom code regardless of Price Embedded 
		{
			csItemSent = io.csItemCodeVariablePartZeroed;
		}// -TAR 321239/320705
        // +TAR 269208
		// Tell security manager void item details
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_UPC, AdjustBarCode());
		CKeyValue<int>::Put(parmList, KEY_ITEM_QUANTITY, lQuantity);
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_ITEM_ID, io.d.csItemID);

		_bstr_t bstrParmList((const _TCHAR *)parmList); 
		SecMgr.ItemDetails(lSecMgrCookie, bstrParmList);
	    // -TAR 269208
		}
		break;
	case CANCEL_ITEM:
    case COUPON_REFUSED:
        {
        //sscop-3321+
        if(fInMultiSelectPickList)
        {   
            g_bOnItemOK = true;
            g_lNumberOfItemsNotBagInMultiPick = 0;
        }
        //sscop-3321-
        g_lNumberOfWLDBEntriesThisItem = -1;//TAR 196952
        
        g_bWtExpected = false;

        bEASReEnableScanner = true;

        if (fSAInItemSale)
        {
            SendTBCommand(TBCMD_CMDSTRING, CString(_T("SASendScotInput_ItemCancelled")));
        }
        fSAInItemSale = false; // Fix coupon scanning from RAP issue (TAR: 209401)
        // +TAR 321239/320705  If it is InStoreBarCode, we need to replace  
        if (io.b_gHasCustomCodeBeenFound)//RFC239992 Use custom code regardless of Price Embedded 
        {
            csItemSent = io.csItemCodeVariablePartZeroed;
        }// -TAR 321239/320705
        g_csPLAItemCode = _T("");
        CKeyValue<const _TCHAR*>::Put(parmList, KEY_UPC, AdjustBarCode());
        //TAR291629: For some TB such as GiantEagle, their lQuantity is returned as 0 which is not 
        //accepted by SM
        CKeyValue<int>::Put(parmList, KEY_ITEM_QUANTITY, lQuantity == 0 ? 1 : lQuantity);
        CKeyValue<const _TCHAR*>::Put(parmList, KEY_ITEM_ID, io.d.csItemID);
        CKeyValue<const _TCHAR*>::Put(parmList, KEY_ITEM_DETAILS_FLAGS, KEY_ITEM_DETAILS_NOT_SELLABLE);
        CKeyValue<const _TCHAR*>::Put(parmList, KEY_ITEM_DETAILS_DESCRIPTION, io.d.csDescription);

        _bstr_t bstrParmList((const _TCHAR *)parmList); 
        SecMgr.ItemDetails(lSecMgrCookie, bstrParmList);
        }
        break;
	case PRODUCT_TAG_DEACTIVATED:
		bEASReEnableScanner = true;
		break;
	case BALANCE_PAID:			
		{
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_TC_OPERATION_BALANCE_PAID);
		_bstr_t bstrParmList((const _TCHAR *)parmList);
		SecMgr.TransactionControl(lSecMgrCookie, bstrParmList);
		}
	  	break;
#endif //_CPPUNIT
  }
  return 1;
}
#ifndef _CPPUNIT
/////////////////////////////////////////////
// trim off the line feed
/////////////////////////////////////////////
CString FindLeadingNewlines(CString& string)
{
  bool cont = true;
  int i = 0;
  CString ret = _T("");

  cont = !string.IsEmpty();

  while(cont)
  {
    if (string[i] == _T('\n'))
      i++;
    else if (string[i] == _T('\\'))
    {
      if (string[i+1] == _T('n'))
        i+=2;
      else
        cont = false;
    }
    else
      cont = false;
  }

  if (i > 0)
  {
    ret = string.Left(i);
  }

  return ret;
}

//////////////////////////////////////////////////////////////////////////////
// override in some states if needed
//////////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::SAEventOccurred(const long)
{
	return STATE_NULL;
}


//////////////////////////////////////////////////////////////////////////////////////
//	This function replace DMScannerEnable called before by each State
//////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SAWLDBScannerEnable(void)
{
	int nISIPTimeOutValue = GetISIPTimeOut();
	if (!nISIPTimeOutValue && bEASReEnableScanner)
	{
		DMScannerEnable();	
	}
	else
	{
		CString csInProgress = _T("Is product sale in progress = ");
		CString csTagDeactivated = _T("Is EAS tag deactivated yet = ");
		trace(L2, _T("SAWLDBScannerEnable = DMScanner NOT Re-enabled, Re-Enable requested by screen/state change = 1, %s, %d, %s, %d"), 
			  csInProgress, nISIPTimeOutValue,
			  csTagDeactivated, bEASReEnableScanner);  //Added EAS tracing informations LPM092800
	}
	fScannerRequested = TRUE;	
}

//////////////////////////////////////////////////////////////////////////////////////
//	This function replace DMScannerDisable called before by each State
//////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SAWLDBScannerDisable(void)
{
	fScannerRequested = FALSE;
	// we disable the scanner in any case
	DMScannerDisable();
}


///////////////////////////////////
//	Security Maintenance Mode	//
//////////////////////////////////


#endif //_CPPUNIT
///////////////////////////////////////////////////////////////////////////
//  SMM Display the Tolerance data to the screen                
///////////////////////////////////////////////////////////////////////////
void SMStateBase::SASMMDisplayTolInfo()
{
#ifndef _CPPUNIT
    CString csMultiBarCode;
    CString csSingleLabel;
    int iReturn;
	bSMMFailure = false;
	iReturn = ps.ResetSMMItemList();
    ASSERT (iReturn == 0);  // Make lint happy.  Hardcoded return value

	SASMMFormatProductInfo();
    
	// blank lines and column titles
    if (m_bLastMntScanDataWasMultiLabel)
    {
        for (int i = 0; i < m_MntMLBarcodeArray.GetSize() ; i++)
        {
            csSingleLabel = m_MntMLBarcodeArray.GetAt(i);
            if (i % 2)
            {
                // If not the first time add a plus
                csMultiBarCode = csMultiBarCode + _T(" + {") + csSingleLabel + _T("}");
                // If this isn't the last one
                if (i !=  m_MntMLBarcodeArray.GetSize() - 1) 
                {
                    csMultiBarCode += _T(" + ");
                    iReturn = ps.AddSMMItemList(csMultiBarCode);
                    ASSERT (iReturn == 0);  // Make lint happy.  Hardcoded return value
                }
            }
            else
            {
                csMultiBarCode = _T("{") + csSingleLabel + _T("}");
            }
        }
        iReturn = ps.AddSMMItemList(csMultiBarCode);
        ASSERT(iReturn == 0);  // Make lint happy.  Hardcoded return value

    }
    else
    {
	    iReturn = ps.AddSMMItemList(_T("                                           "));
        ASSERT(iReturn == 0);  // Make lint happy.  Hardcoded return value
    }

	iReturn = ps.AddSMMItemList(ps.GetPSText(MSG_SMM_SEPARATOR));
    ASSERT(iReturn == 0);  // Make lint happy.  Hardcoded return value
	iReturn = ps.AddSMMItemList(ps.GetPSText(MSG_SMM_TOLCAPTION));
    ASSERT(iReturn == 0);  // Make lint happy.  Hardcoded return value
	iReturn = ps.AddSMMItemList(_T("                                           "));
    ASSERT(iReturn == 0);  // Make lint happy.  Hardcoded return value
	iReturn = ps.AddSMMItemList(ps.GetPSText(MSG_SMM_TOLCOLUMNS));
    ASSERT(iReturn == 0);  // Make lint happy.  Hardcoded return value

	SASMMFormatToleranceInfo();

	// more blank lines and column titles
	iReturn = ps.AddSMMItemList(ps.GetPSText(MSG_SMM_SEPARATOR));
    ASSERT(iReturn == 0);  // Make lint happy.  Hardcoded return value
	iReturn = ps.AddSMMItemList(ps.GetPSText(MSG_SMM_WLDBCAPTION));
    ASSERT(iReturn == 0);  // Make lint happy.  Hardcoded return value
	iReturn = ps.AddSMMItemList(_T("                                           "));
    ASSERT(iReturn == 0);  // Make lint happy.  Hardcoded return value
	iReturn = ps.AddSMMItemList(ps.GetPSText(MSG_SMM_WLDBCOLUMNS));
    ASSERT(iReturn == 0);  // Make lint happy.  Hardcoded return value
	iReturn = ps.SMShowSMMItemList();
    ASSERT(iReturn == 0);  // Make lint happy.  Hardcoded return value
#endif //_CPPUNIT
}

#ifndef _CPPUNIT

///////////////////////////////////////////////////////////////////////////
//  SMM default method for when not is AddWtToleranceBase
///////////////////////////////////////////////////////////////////////////
SMStateBase * SMStateBase::SASMMBagScaleEvent(void)
{
  return STATE_NULL;
}

#endif //_CPPUNIT
///////////////////////////////////////////////////////////////////////////
//  SMM Security went off line during Security Maintenance Mode  
///////////////////////////////////////////////////////////////////////////
SMStateBase * SMStateBase::SASMMSecOffline()
{
#ifndef _CPPUNIT
  if (bSMMFailure) //228239 Don't handle 2 messages in a row.
  { 
	  trace (L6, _T("+SASMMSecOffline: already offline. No need to handle.") );
	  return STATE_NULL; 
  }

  trace(L7, _T("+rp.m_bOffline"));
  rp.m_bOffline = true;
  trace(L7, _T("+rp.m_bOffline<%d>"),rp.m_bOffline); 
  ps.ResetSMMItemList();
#endif //_CPPUNIT
  RETURNSTATE(SmmEditAdd)
}
#ifndef _CPPUNIT

///////////////////////////////////////////////////////////////////////////
//  SMM Format and display the 1st line displayed with the UPC number and product type
///////////////////////////////////////////////////////////////////////////
void SMStateBase::SASMMFormatProductInfo()
{
  // label for product type differs based on price per weight unit
  int nTotalListWidth = SMM_LISTWIDTH;
  CString csUPCLabel, csProdLabel, csUPCInfo;
  if (bIsSMMitemTolPPWU)
	{csProdLabel = ps.GetPSText(MSG_SMM_PPWULABEL);}
  else
	{csProdLabel = ps.GetPSText(MSG_SMM_REGULARLABEL);}

  csUPCLabel = ps.GetPSText(MSG_SMM_UPCLABEL);

  int nLen1 = csUPCLabel.GetLength() + 1;
  int nLen2 = csSMMScannedUPC.GetLength();
  int nLen3 = csProdLabel.GetLength();
  int nRemainderLen = nLen1 + nLen2; 
  int nblnkLen = (nTotalListWidth - nRemainderLen);
  csUPCInfo.Format(_T("%-*s%-s%*s"), nLen1, csUPCLabel, csSMMScannedUPC, nblnkLen, csProdLabel);
  ps.AddSMMItemList(csUPCInfo);
}

///////////////////////////////////////////////////////////////////////////
//  SMM Format and display the 3 tolerance columns
///////////////////////////////////////////////////////////////////////////
void SMStateBase::SASMMFormatToleranceInfo()
{
	// add the word 'Default' to the Y or N if it matches the global value
	//+TAR 423665
	CString csWtExp, csWtSubCk,/* csWtTol,*/ cstempGlobal, cstempLocal, csDefault;
	//-TAR 423665
	int nTotalListWidth = SMM_LISTWIDTH;
	csDefault = ps.GetPSText(MSG_SMM_DEFAULTLBL);
	int nLen1, nLen2;

	if ( csSMMWeightRequired == _T("default-y") )
	{
		cstempGlobal = ps.GetPSText(MSG_SMM_Y);
		csWtExp.Format(_T("%s-%s"), csDefault, cstempGlobal);
		nLen1 = csWtExp.GetLength() + 1;
	}
	else if ( csSMMWeightRequired == _T("default-n") )
	{
		cstempGlobal = ps.GetPSText(MSG_SMM_N);
		csWtExp.Format(_T("%s-%s"), csDefault, cstempGlobal);
		nLen1 = csWtExp.GetLength() + 1;
	}
	else if ( csSMMWeightRequired == _T("y") )
	{
		cstempLocal = ps.GetPSText(MSG_SMM_Y);
		int nSpace1 = (nTotalListWidth/8);  
		csWtExp = cstempLocal;
		nLen1 = csWtExp.GetLength() + nSpace1;
	}
	else if ( csSMMWeightRequired == _T("n") )
	{
		cstempLocal = ps.GetPSText(MSG_SMM_N);
		int nSpace1 = (nTotalListWidth/8);  
		csWtExp = cstempLocal;
		nLen1 = csWtExp.GetLength() + nSpace1;
	}
	else if ( csSMMWeightRequired == _T("o") )
	{
		cstempLocal = ps.GetPSText(MSG_SMM_O);
		int nSpace1 = 2;  //SSCOP-1039
		csWtExp = cstempLocal;
		nLen1 = csWtExp.GetLength() + nSpace1;
	}


	if ( csSMMSubstitutionCheck == _T("default-y") )
	{
		cstempGlobal = ps.GetPSText(MSG_SMM_Y);
		csWtSubCk.Format(_T("%s-%s"), csDefault, cstempGlobal);
		nLen2 = csWtSubCk.GetLength();
	}
	else if ( csSMMSubstitutionCheck == _T("default-n") )
	{
		cstempGlobal = ps.GetPSText(MSG_SMM_N);
		csWtSubCk.Format(_T("%s-%s"), csDefault, cstempGlobal);
		nLen2 = csWtSubCk.GetLength();
	}
	else if ( csSMMSubstitutionCheck == _T("y") )
	{
		cstempLocal = ps.GetPSText(MSG_SMM_Y);
		int nSpace1 = (nTotalListWidth/8) - 4;
		csWtSubCk = cstempLocal;
		nLen2 = csWtSubCk.GetLength() + nSpace1;
	}

	else if ( csSMMSubstitutionCheck == _T("n") )
	{
		cstempLocal = ps.GetPSText(MSG_SMM_N);
		int nSpace1 = (nTotalListWidth/8) - 4;
		csWtSubCk = cstempLocal;
		nLen2 = csWtSubCk.GetLength() + nSpace1;
	}
	// format the wt tolerance column to a string
	//+TAR 423665
	//csWtTol = SASMMConvertTol();
	//-TAR423665

	// determine the width of each column
	int nSpace2 = ( ((nTotalListWidth/2) + (nLen2/2)) - (nLen1 + nLen2) );  //take the midway point, add 1/2 of the length 
	int nWidth2 = nLen2 + nSpace2;
	int nWidth3 = nTotalListWidth - (nWidth2 + nLen1);
		
	// write all that out
	CString csAllWLDBTolerance;
	//+TAR 423665
	//csAllWLDBTolerance.Format(_T("%*s%*s%*s"), nLen1, csWtExp, nWidth2, csWtSubCk, nWidth3, csWtTol);
	csAllWLDBTolerance.Format(_T("%*s%*s"), nLen1, csWtExp, nWidth2, csWtSubCk);
	//-TAR 423665
	ps.AddSMMItemList(csAllWLDBTolerance);
}
#endif // _CPPUNIT
///////////////////////////////////////////////////////////////////////////
void SMStateBase::SASMMFormatWLDBInfo(void)
{
#ifndef _CPPUNIT
	// if no WLDB entry
	if (0 == lSMMWeightCount )
	{
		ps.AddSMMItemList(ps.GetPSText(MSG_SMM_NOENTRIES));
	}
	else
	{
		//changes for TAR373193 so columns line up better if multiple rows are displayed
        //unneeded code left in for now
//		int nTotalListWidth = SMM_LISTWIDTH;
		for (int i=0; i < lSMMWeightCount; i++ )
		{
			CString csFormatedWeight;
			csFormatedWeight.Format(_T("%s %s"), psecMaintenanceRecordArray[i].csWeight, csScaleUnits );
//			int nLen1 = csFormatedWeight.GetLength();
//			int nLen2 = psecMaintenanceRecordArray[i].csApproved.GetLength();
//			int nLen3 = 3;	// Tally 
//			int nLen4 = psecMaintenanceRecordArray[i].csDate.GetLength();
//			int nSpace2 = ( (nTotalListWidth - (nLen1 + nLen2)) / 3 );
//			int nWidth2 = ((nLen2/2) + nSpace2) - 2;
//			int nWidth3 = ((nLen3/2) + nSpace2) - 1;
//			int nWidth4 = nTotalListWidth - (nLen1 + nWidth2 + nWidth3);
			//stick strings together		%[flags] [width] [.precision] [{h | l | I64 | L}]type
			CString csWLDBEntryLine, /*csTally,*/ csApproved;
//			csTally.Format(_T("%i"), psecMaintenanceRecordArray[i].lTally );
			if (psecMaintenanceRecordArray[i].csApproved == _T("true"))	
			{
				csApproved = ps.GetPSText(MSG_SMM_Y);
			}
			else		
			{
				csApproved = ps.GetPSText(MSG_SMM_N);
			}
//			csWLDBEntryLine.Format(_T("%s%s%*s%*s%*s"), _T(" "), csFormatedWeight, nWidth2, csApproved, nWidth3, csTally, nWidth4, psecMaintenanceRecordArray[i].csDate);
			//+TAR423665
            //  csWLDBEntryLine.Format(_T(" %-15s%-7s%6d%14s"), csFormatedWeight, csApproved, psecMaintenanceRecordArray[i].lTally, psecMaintenanceRecordArray[i].csDate);
			csSMMToleranceType = psecMaintenanceRecordArray[i].csToleranceType;
			CString csTolType = SASMMConvertTol();
			//CString csTolType=SASMMConvertTol(),csTolValue;
			//csTolValue.Format(_T("%s %s"),csSMMTolerance, csScaleUnits);
			CString csTolValue;
		        csTolValue.Format(_T("%s %s"),psecMaintenanceRecordArray[i].csTolerance, csScaleUnits);
			if (co.csPrimaryLanguageCode == _T("0804") || co.csPrimaryLanguageCode.CompareNoCase(_T("0C04")) == 0)
			{
				if (co.fOperationsScaleMetric)
				{
				    csWLDBEntryLine.Format(_T(" %-14s%-8s%-9d%-12s%s%10s"), csFormatedWeight, csApproved, psecMaintenanceRecordArray[i].lTally, csTolType, csTolValue, psecMaintenanceRecordArray[i].csDate);
				}
				else
				{
					csWLDBEntryLine.Format(_T(" %-15s%-7s%-10d%-12s%s%12s"), csFormatedWeight, csApproved, psecMaintenanceRecordArray[i].lTally, csTolType, csTolValue, psecMaintenanceRecordArray[i].csDate);
				}
			} 
			else
			{
			    csWLDBEntryLine.Format(_T(" %-15s%-9s%-11d%-12s%s%13s"), csFormatedWeight, csApproved, psecMaintenanceRecordArray[i].lTally, csTolType, csTolValue, psecMaintenanceRecordArray[i].csDate);
			}			
			//-TAR423665
			ps.AddSMMItemList(csWLDBEntryLine);
		}
	}
#endif // _CPPUNIT
}

#ifndef _CPPUNIT

///////////////////////////////////////////////////////////////////////////
//  SMM Convert the tolerance to a viewable value
///////////////////////////////////////////////////////////////////////////
CString SMStateBase::SASMMConvertTol()
{

	CString csTempWtTol,csTempTxt;
	/*423665
	if (csSMMToleranceType == KEY_OSM_TOLERANCE_TYPE_DEFAULT )
	{
		csTempTxt = ps.GetPSText(MSG_ABBREV_DEF);
	}
	else if (csSMMToleranceType == KEY_OSM_TOLERANCE_TYPE_STD_DEV )
	{
		csTempTxt = ps.GetPSText(MSG_ABBREV_STD);
							
	}
	else if (csSMMToleranceType == KEY_OSM_TOLERANCE_TYPE_FIXED )
	{
		csTempTxt = ps.GetPSText(MSG_ABBREV_EXC);

	}
	*/
    
	//423665+
	if (_tcstoul(csSMMToleranceType,NULL,10) == WTINFO_DEFAULT )
	{
		csTempTxt = ps.GetPSText(MSG_ABBREV_DEF);
	}
	else if ( _tcstoul(csSMMToleranceType,NULL,10) == WTINFO_STDDEV )
	{
		csTempTxt = ps.GetPSText(MSG_ABBREV_STD);
							
	}
	else if ( _tcstoul(csSMMToleranceType,NULL,10) == WTINFO_INTERPOLATED_TOLERANCE )
	{
		csTempTxt = ps.GetPSText(MSG_ABBREV_INTERPOLATED);
	}
	else if ( _tcstoul(csSMMToleranceType,NULL,10) == WTINFO_EXCEPTIONITEM )
	{
		csTempTxt = ps.GetPSText(MSG_ABBREV_EXC);
	}
	else if ( _tcstoul(csSMMToleranceType,NULL,10) == WTINFO_EXCEPTIONDEPT )
	{
		csTempTxt = ps.GetPSText(MSG_ABBREV_EXCEPTIONDEPT);
	}
	else if ( _tcstoul(csSMMToleranceType,NULL,10) == WTINFO_MAXERROR )
	{
		csTempTxt = ps.GetPSText(MSG_ABBREV_MAXERROR);
	}
	else if ( _tcstoul(csSMMToleranceType,NULL,10) == WTINFO_EXCEPTIONTB )
	{
		csTempTxt = ps.GetPSText(MSG_ABBREV_EXCEPTIONTB);
	}
	
	csTempWtTol.Format(_T("%s"),csTempTxt);
	//TAR423665-
	return csTempWtTol;
}

///////////////////////////////////////////////////////////////////////////
//  SMM Convert the BS wt to PPWU
///////////////////////////////////////////////////////////////////////////
long SMStateBase::SASMMConvertToPPWU()
{
	_TCHAR price[20], item_code[20];
	CString cstempItemCode = csSMMScannedUPC;
	int nUPCFullLen = csSMMScannedUPC.GetLength();
	//if item code has scan type character prefix
	if ( nUPCFullLen && (!_istdigit(csSMMScannedUPC[0])))		
	{	
		// remove alpha characters from UPC, assumes prefix is never more than one character
		cstempItemCode = csSMMScannedUPC.Right(nUPCFullLen - 1);
	}
	int nItemCodeLen = cstempItemCode.GetLength();
	if (nItemCodeLen < 13)
	{
		cstempItemCode = csSMMScannedUPC.Left(nUPCFullLen - 1);
		_tcscpy (item_code, cstempItemCode); // item code without scan type or ck sum
		(void) _tcscpy(price, item_code+7);	//UPC 12
	}
	else
	{
		cstempItemCode = csSMMScannedUPC.Left(nUPCFullLen - 2);
		_tcscpy (item_code, cstempItemCode); // item code without scan type or ck sum
		(void) _tcscpy(price, item_code+6);	//EAN 13
	}

	// stick string price into long
	long lPrice = _ttol(price);
	return lPrice;
}

///////////////////////////////////////////////////////////////////////////
//  SMM Convert the bag scale wt from LB to KG
///////////////////////////////////////////////////////////////////////////
long SMStateBase::SALbsToKgs(long lWeight)
{
	long ResWT = 0;
	div_t div_result;
	double dKilograms = 0.0;
	double dPounds = 0.0;
	ResWT = lWeight;

	if( ResWT != 0 )
	{
		dPounds	= (double)ResWT;				// e.g. 8569
		dPounds	/= 1000;						// all weights should be in thousandths (put in the decimal point)	e.g. 8.569
		dKilograms = dPounds * 0.4535929;		// convert pounds to kilograms (this is 1/(2.20462)	e.g. 3.8868
		ResWT	= (long) (dKilograms * 1000);	// add the implied decimal back (now the whole number is kg in thousandths)	e.g. 3886.8		
		div_result = div(ResWT, 10);			// round to the hundredths place	e.g. 388.68		

		if( div_result.rem >= 5 )							//should we round
			div_result.quot++;								// yes  (e.g. 389)

		// convert back to thousands and put a zero in the thousandth place

		div_result.quot *= 10;					//e.g. 3890
		ResWT	= div_result.quot;
		TRACE(_T("Converting %4.3f pounds to %4.3f kilograms.\n"), dPounds, (((double)ResWT)/1000));
	}
	return ResWT;
}

/// end of SMM

///////////////////////////////////////////////////////////////////////////
//  App Security 3.0
///////////////////////////////////////////////////////////////////////////
bool SMStateBase::CouponTally(long TotalSales, long CouponTotals)
{
	bool bReturn = false;

	// Coupon logic
	// If total sale is greater then configurable total sale in SCOTOPTS (Upper$AmtForPercentToApply)
	// then use percentage logic
	// else then check for the total coupon amout if it greater or equal the configurable 
	// total coupon max (TotalCoupons$AmtMax)
	if(CouponTotals)
	{
		if(TotalSales >= co.nOperationsUpper$AmtForPercentToApply)
		{
			int Percentage = 0;
			float temp = 0;
			long lGrossSales = 0;
			// The GrossSales here means the gross sale without the total of coupons.
			lGrossSales = abs(TotalSales + CouponTotals);
			temp = (float) CouponTotals / lGrossSales;
			Percentage = (int) (temp * 100);
			if (Percentage >= co.nOperationsCouponPercentofGrossSales)  
			{
				// (*) SR898 ====================
                DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                                   DM_TRICOLORLIGHT_ON,
                                   DM_TRICOLORLIGHT_NORMALREQUEST);
                                   
				bReturn= true;
			}
		}
		else
		{
			if(CouponTotals >= co.nOperationsTotalCoupons$AmtMax)
			{
				// (*) SR898 ====================
                DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                                   DM_TRICOLORLIGHT_ON,
                                   DM_TRICOLORLIGHT_NORMALREQUEST);
                                   
				bReturn = true;
			}
		}
	}
	return bReturn;
}

///////////////////////////////////////////////////////////////////////////
//  App Security 3.0
///////////////////////////////////////////////////////////////////////////
bool SMStateBase::IsDuplicateCoupon()
{
	bool bReturn=false;
	CString csTmp;
	// Tar 228716 should be able to verify same coupon whether scanned or manually entered
	if (!io.cScanType)	// keyed-in coupon
	{
		csTmp = io.csItemCode;
	}

	else				// scanned coupon
	{
		int nItemCodeLen = io.csItemCode.GetLength();
		if ( (io.cScanType == SCAN_SDT_EAN8)) 
		{
			csTmp = io.csItemCode.Right(nItemCodeLen - 2); // remove first two scan type charactera
		}
		else		// upc and ean13
		{
			csTmp = io.csItemCode.Right(nItemCodeLen - 1); // remove first scan type character
		}
	}

    int nCouponCount;
    if (g_mapTransactionCoupon.Lookup( csTmp, nCouponCount ))
    {
        nCouponCount++;
    }
    else
    {
        nCouponCount = 1;

    }
    g_mapTransactionCoupon.SetAt(csTmp, nCouponCount);

    if (nCouponCount > nCountPossibleDuplicateCoupon)
        nCountPossibleDuplicateCoupon = nCouponCount;

	//TAR196365 Don't need to sub by 1 because if the duplicate coupon max is set to 2
	//need to scan 3 same coupon to have approval.
	if(nCountPossibleDuplicateCoupon >= co.nOperationsDuplicateCouponMax)
	{
		bReturn = true;
        
		// (*) SR898 ====================
        DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                           DM_TRICOLORLIGHT_ON,
                           DM_TRICOLORLIGHT_NORMALREQUEST);
	}

	return bReturn;
}

#endif //_CPPUNIT
///////////////////////////////////////////////////////////////////////////
//  App Security 3.0
///////////////////////////////////////////////////////////////////////////
CString SMStateBase::GetExpectedWts()
{
    CString csRet;

    csRet = secEventParms.m_csExpectedWeight;

    if(csRet.IsEmpty())
        csRet = _T("None");

    return csRet;
}
#ifndef _CPPUNIT



///////////////////////////////////////////////////////////////////////////
SMStateBase * SMStateBase::FinalizationWtIncrease(BEST nextState)
{

	CREATE_AND_DISPENSE(SecUnExpectedIncrease)(nextState, nextState);
}

///////////////////////////////////////////////////////////////////////////
void SMStateBase::FinalizationWtDecrease(void)
{
	// Finalization decrease occured, always allowed.
	trace ( L4, _T("In Finalization state, allow decreases while here."));
}

///////////////////////////////////////////////////////////////////////////
// SR695 - Used for cases when finalization weight decreases are not allowed
SMStateBase * SMStateBase::FinalizationWtDecreaseNotAllowed(BEST nextState)
{
	trace ( L4, _T("Finalization weight decrease not allowed."));
	CREATE_AND_DISPENSE(SecUnexpectedDecrease)(nextState, nextState);
}
///////////////////////////////////////////////////////////////////////////
SMStateBase * SMStateBase::VoidingWtIncrease(BEST prevState, BEST nextState)
{
	if(secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_MISMATCHED)
	{
        if (m_View == DATANEEDED_MOREDATA)
        {
            CREATE_AND_DISPENSE(SecMisMatchWeight)(BES_RAPDATANEEDED, BES_RAPDATANEEDED);
        }
        else
        {
            CREATE_AND_DISPENSE(SecMisMatchWeight)(BES_BAGANDEAS, getNextStateForGoodItem());
        }
	}

	CREATE_AND_DISPENSE(SecUnExpectedIncrease)(prevState, nextState);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase * SMStateBase::VoidingWtDecrease(void)
{
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::ThresholdViolRAPClearSecBtn(BEST PrevReturnState)
{
	trace(L2, _T("+SMStateBase::ThresholdViolRAPClearSecBtn"));
	
	DMSaySecurity( SEC_NONE, _T("blank.wav") );

    
	if(m_View == DATANEEDED_BAGBOXTHRESHOLDEXCEEDED)
	{
		nBagBoxBtnPresses = 0;
		//TAR 214691 Reset wt expected and number of entries variables
		g_bWtExpected = false;	//We should never expect wt if button is pressed
		g_lNumberOfWLDBEntriesThisItem = -1;	// reset the number of WLDB entries
	}
	else if(m_View == DATANEEDED_DONEREMOVINGTHRESHOLDEXCEEDED)
	{
		nItemRemovedBtnPresses = 0;


	}
	else if(m_View == DATANEEDED_DONTBAGTHRESHOLDEXCEEDED)
	{
		nSkipBaggingBtnPresses = 0;
		//TAR 214691 Reset wt expected and number of entries variables
		g_bWtExpected = false;	//We should never expect wt if button is pressed
		g_lNumberOfWLDBEntriesThisItem = -1;	// reset the number of WLDB entries
	}
	else if(m_View == DATANEEDED_NEWITEMENTRY)
	{

		g_lNumberOfWLDBEntriesThisItem = -1;
	}
   
	SAClearSecurityCondition();
	//218558 set scanner flag to re-enabled only in state that isn't handling EAS automatically
	trace(L2, _T("bEASReEnableScanner is set to true."));
	bEASReEnableScanner = true;

	if ((PrevReturnState == BES_START || !bIsTransactionStarted ) && co.fOperationsAllowContainerBtn)
	{
		// enable the sensormatic EAS deactivator because going into a transaction from attract
		dm.EASDeactivatorEnable( EASEnable );
		SASendSCOTInput( ITEMIZATION );
		trace(L2, _T("-SMStateBase::ThresholdViolRAPClearSecBtn -- Bag/Box"));
		return TBStart();
	}

	trace(L2, _T("-SMStateBase::ThresholdViolRAPClearSecBtn"));

	if (PrevReturnState == BES_SECBAGBOXNOTVALID )//SR085
		{return setAndCreateAnchorState(PrevReturnState); }
	else if(bIsInFinalization && PrevReturnState != BES_SCANANDBAG 
		|| bIsInFinalization && PrevReturnState != BES_TRANSPORTITEM) //TAR417185
	{
		return setAndCreateAnchorState(PrevReturnState);
	}
    //sscoadk-6206+
    else if (g_bOnItemOK == true && (ImmediateInterventionStack.GetCount() == 0) )
    {
        trace (L6, _T("Got ITEMOK and there is no intervention"));
        return setAndCreateAnchorState(PrevReturnState);
    }
    //sscoadk-6206-
	else
		{return STATE_NULL;}	//Tar 369810
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::ThresholdViolWtIncreaseNotAllowed(BEST PrevState, BEST ThreshViolationState)	 
{
	//if not from Bag&EAS, then the wt is UnExpected
	DMSaySecurity( SEC_NONE, _T("blank.wav") );
	CREATE_AND_DISPENSE(SecUnExpectedIncrease)(PrevState, ThreshViolationState);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::ThresholdViolWtDecrease(BEST PrevState, BEST ThreshViolationState)
{
	// normal case where removing is not allowed.
	DMSaySecurity( SEC_NONE, _T("blank.wav") );
	CREATE_AND_DISPENSE(SecUnexpectedDecrease)(PrevState, ThreshViolationState);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::ClearedSecViolSetReturnState(BEST Prevstate, BEST Nextstate)
{
	// PrevState is the state we were in when the sec violation occurred
	// NextState is the state we should got when the sec violation is cleared or approved
	DMSaySecurity( SEC_NONE, _T("blank.wav") );

	SAClearSecurityCondition();


	//218558 set scanner flag to re-enabled only in state that isn't handling EAS automatically
	trace(L2, _T("bEASReEnableScanner is set to true."));
	bEASReEnableScanner = true;

	//TAR221089 CHU+ 112502
	//The anchor state or previous state could be BES_START or BES_SCANANDBAG
	if(Nextstate == BES_TRANSPORTITEM)
	{
		DMTakeawayBeltNotify(tabEventStableGoodWeight);	// send good weight notification
		return setAndCreateAnchorState(BES_TRANSPORTITEM);	
	}
	//TAR221089-

	if ((getAnchorState() == BES_START) && !bQuietNeedSecApproval)//Tar 211789 - AH 08/15/02
		return setAndCreateAnchorState(BES_START);
	else if(Nextstate == BES_SECBAGBOXTHRESHOLD)
	{
		CREATE_AND_DISPENSE(SecBagBoxThreshold)(Prevstate);	// passed previous state in so could remember it at return state LPM very complicated!
	}
	else if(Nextstate == BES_SECITEMREMOVEDTHRESHOLD) 
	{
		if(ImmediateInterventionStack.GetCount() == 0)
		{
			trace(L6, _T("No intervention, go back to AnchorState. "));
			return createAnchorState();
		}
		CREATE_AND_DISPENSE(SecItemRemovedThreshold)(Prevstate);	// passed previous state in so could remember it at return state LPM very complicated!	
	}
	else if(Nextstate == BES_SECNEWITEMENTRY)			
	{
		CREATE_AND_DISPENSE(SecNewItemEntry)(Prevstate, Nextstate);	// passed previous state in so could remember it at return state LPM very complicated!	
	}
	else if(Nextstate == BES_SECSKIPBAGGINGTHRESHOLD) 
	{
		CREATE_AND_DISPENSE(SecSkipBaggingThreshold)(Prevstate);	// passed previous state in so could remember it at return state LPM very complicated!
	}
	else if(Prevstate == BES_BAGANDEAS)
		RETURNSTATE(BagAndEAS)

	// TAR 312196 start
	if (g_pDelay_TBExpectedStateChange != NULL)
	{
		SMStateBase *pNewState = g_pDelay_TBExpectedStateChange;
		CString csStateName = pNewState->GetRuntimeClass()->m_lpszClassName;
		g_pDelay_TBExpectedStateChange = NULL;
		trace(L6,_T("SMStateBase::ClearedSecViolSetReturnState returning %s (state saved in g_pDelay_TBExpectedStateChange)"),csStateName);
		return pNewState;
	}
    // TAR 312196 end
    if((Prevstate == BES_PICKINGUPITEMS) && (Nextstate == BES_SCANANDBAG))
        return STATE_NULL;
    else if(Prevstate == BES_STATERETURN)
        return STATE_RETURN;

	return createState(Prevstate);	// return to the state before the error
}


///////////////////////////////////////////////////////////////////////
CString SMStateBase::FmtWtToString(const long lWgt)// format weight to string 0.00 or 0.000
{
  CString csWork = _T("");
    
  trace(L6,_T("+FmtWtToString %d"),lWgt);
    
  if (co.fOperationsScaleMetric)
  {
	  long tempWgt = 0;
	  tempWgt = SALbsToKgs(lWgt);
      csWork.Format(_T("%5.3f"),1.0*tempWgt/1000.0);
  }
  else
  {
      csWork.Format(_T("%4.2f"),1.0*lWgt/1000.0);
  }
  trace(L6,_T("-FmtWtToString %s"),csWork);
  return csWork;
}

#endif // _CPPUNIT
/////////////////////////////////////////////////////////////////////
// Update the scale unit
void SMStateBase::UpdateScaleUnits()
{
#ifndef _CPPUNIT
	if (co.fOperationsScaleMetric)
		csScaleUnits = ps.GetPSText(SCALE_KG);
	else
		csScaleUnits =  ps.GetPSText(SCALE_LB);
#endif // _CPPUNIT
}
#ifndef _CPPUNIT
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::DetermineEASDeactivationState(void)
{
	// Inform Takeaway Belt that item was accepted. 213211 LPM082902
#ifndef NewSec
	if( dm.fStateTakeawayBelt )
	{
	  if(io.lQuantityEntered > 1)
		//TAR220160+ CHU 112702
		//With Tom's new state to handle Quantity item, we tell TAB GoodWeight instead
		//DMTakeawayBeltNotify(tabEventFlushAndFinish);
		DMTakeawayBeltNotify(tabEventStableGoodWeight);
	    //TAR221060-
	  else
		DMTakeawayBeltNotify(tabEventStableGoodWeight);
	}
#endif

	// TAR205479 reset these so next stolen item is not treated like a price embedded and allowed LPM052402
	io.b_gIsThisItemPriceEmbedded = false;	

    if (dm.fStateTakeawayBelt)
    {
		g_bMatchedWtWaitingOnEAS = false;	// the matched wt is not waiting on EAS
        m_bEASHappened = true;              //   because EAS is automatic with Takeaway
//USSF START
		USSFHOOK(_T("OnMatchedWt"), nextState = OnMatchedWt(), return nextState;);
		//return OnMatchedWt();
//USSF END
    }

    if (dm.EASDeactivatorPresent()) 
    {
		if (bEASReEnableScanner) //if the sensor was tripped or RAP cleared it then proceed
		{
//USSF START
			USSFHOOK(_T("OnMatchedWt"), nextState = OnMatchedWt(), return nextState;);
			//return OnMatchedWt();
//USSF END
		}
		else if (!dm.fStateMotionSensor) //if the sensor is not installed, then proceed
		{
			m_bEASHappened = true;				// because the motion sensor is not installed
//USSF START
			USSFHOOK(_T("OnMatchedWt"), nextState = OnMatchedWt(), return nextState;);
			//return OnMatchedWt();
//USSF END
		}
		else
		{
			g_bWtExpected = false;	//We should never expect wt if we got a matched wt.
			g_lNumberOfWLDBEntriesThisItem = -1;	// reset the number of WLDB entries
//USSF START
			USSFHOOK(_T("OnMatchedWt"), nextState = OnMatchedWt(), return nextState;);
			//return OnMatchedWt();
//USSF END
		}
	}
	// else no deactivation installed so just call matched wt
	g_bMatchedWtWaitingOnEAS = false;	// the matched wt is not waiting of an EAS event
	g_bWtExpected = false;
	g_lNumberOfWLDBEntriesThisItem = -1;
//USSF START
	USSFHOOK(_T("OnMatchedWt"), nextState = OnMatchedWt(), return nextState;);
	//return OnMatchedWt();
//USSF END
}

/////////////////////////////////////////////////////////////////////////
// NN 080702 RFC 210177 - This function writes the WLDB mismatch to log file
void SMStateBase::LogWLDBWeightMismatch(CString csActualWeight, CString csExpectedWeight, long lTolerance)
{
	trace(L2, _T("+ LogWLDBWeightMismatch"));

	CString csExpectedWts;
	CString csDelimiter = _T("; ");
    CString csOrigUPC, csDesc, csItemSku, csMisMatchDesc;
	//+TAR 423666
	CString csToleranceInLbs = _T("");
	CString csActualWeightNotConverted=secEventParms.m_csActualWeightNotConverted;
	CString csExpectedWeightNotConverted=secEventParms.m_csExpectedWeightNotConverted;
	//-TAR 423666

    if (co.fOperationsLogWLDBmismatch)
	{
	    bWriteEndOfTransactionToWLDBlog = true;
		 //tar379847
	    csOrigUPC = io.csOrigCode;
 	    TrimSymbology(csOrigUPC);
        //tar379847
        //csUPC = io.d.csItemCode;     
	    csDesc = io.csOrgItemDescription;//io.d.csDescription Always use the orginal item description even for linked item 
		csExpectedWts += _T("  ") + csOrigUPC; //tar379847
	    //csExpectedWts += _T("  ") + csUPC;          //item code
	    csExpectedWts += csDelimiter + csDesc;  //item description 

	    csExpectedWts += csDelimiter + csActualWeightNotConverted;		//Tar 423666 observed wt in lbs
	    csExpectedWts += csDelimiter + csExpectedWeightNotConverted;   //Tar 423666 expected wt in lbs
   	    csToleranceInLbs.Format(_T("%4.3f"),lTolerance/1000.0);		//Tar 423666 tolerance converted into lbs
		
		csExpectedWts += csDelimiter + csToleranceInLbs;     		//Tar 423666 tolerance in lbs
	    strace(L0, _T("%s"), csExpectedWts); //tar 285197 -- Item description could contain format specifier characters

        trace(L2, _T("Weight Mismatch occured: %s"), csExpectedWts); 
    }

    trace(L7, _T("+rp.ItemException"));
	//csItemSku = io.csOrigCode;
	//csItemSku = io.d.csItemCode;//TAR 355861 - changed csItemSku so that what gets written into <ComputerName>.log matches the reporting log //TAR 394696
	long lPrice = io.lExtPriceEntered;
	csMisMatchDesc = io.csOrgItemDescription; //Always use the orginal item description 
	trace(L2, _T("csOrigUPC of the item that has a link: %s"),csOrigUPC); //394696 to monitor value of csOrigUPC
	rp.ItemException(csMisMatchDesc ,csOrigUPC, lPrice, csActualWeight, csExpectedWeight); //394696 modified to display the PLU of the item that has a link on the report mismatch
	trace(L7, _T("-rp.ItemException"));

    trace(L2, _T("- LogWLDBWeightMismatch"));
} //RFC 210177 end

#endif //_CPPUNIT
//////////////////////////////////////////////////////////
void SMStateBase::ClearSecurityViolationLight(void)
{
#ifndef _CPPUNIT
		// tar 211746 don't turn off red if there are other red interventions waiting
		// because turning off red can let any pending virtual yellow be turned on which
		// means both red and yellow will be on if we immediately turned red back on
		if(!AreAnyRedApprovalsPending())
		{
			// Turn off red security light.
            
			// (*) SR898 ====================
            DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                               DM_TRICOLORLIGHT_OFF,
                               DM_TRICOLORLIGHT_NORMALREQUEST);

            // (*) SR898 ====================
			DMTurnOffTriColorLight();
		}

		//Tar 209034 & 209045 BZ: Restore tri-light only if approval is needed 
		// 209371 sometimes the restore is the wrong color if the paper is low, so moving 
		// this to it's own if() LPM071002 BZ
		if (isAnyApprovalRequired())
		{
			if(AreAnyRedApprovalsPending())
			{
				// (*) SR898 ====================
                DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                                   DM_TRICOLORLIGHT_BLINK_1HZ,
                                   DM_TRICOLORLIGHT_NORMALREQUEST);
			}
			else
			{  
				//tar 329387
                
				// (*) SR898 ====================
                DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                                   DM_TRICOLORLIGHT_ON,
                                   DM_TRICOLORLIGHT_NORMALREQUEST);
			}
		}
		else if (dm.GetCoinsOrBillsVeryLow() || dm.GetPrinterPaperLow())
		{
			// (*) SR898 ====================
            DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                               DM_TRICOLORLIGHT_BLINK_1HZ,
                               DM_TRICOLORLIGHT_NORMALREQUEST);
		}
		else
		{
			// If inside the transaction, turn on the green light
			if(fInSignedOn || fInSignedOff || fStateAssist || fSAInTransaction)
            {
				// (*) SR898 ====================
                DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                                   DM_TRICOLORLIGHT_ON,
                                   DM_TRICOLORLIGHT_NORMALREQUEST);
            }
		}
#endif //_CPPUNIT
}
#ifndef _CPPUNIT

//////////////////////////////////////////////////////////
CString SMStateBase::AdjustBarCode()                    // TAR 236796
{
	//RFC 212888 begin: nItemLabelLen is the length of the barcode without scan type (moved here during Std Dev LPM120202)
	CString csUPCNumber(csItemSent);
	//nItemCodeLen is the length of the barcode plus a pre-appended scan type, such as "A", "E", "B3" etc.
	int nItemCodeLen = csUPCNumber.GetLength();	
	int nItemLabelLen = csDMLastBarLabel.GetLength(); 

	// check to see if any alpha characters are at beginning of upc (put there by PLU server)
	if ( nItemCodeLen && (!_istdigit(csUPCNumber[0])) && (io.cScanType != SCAN_SDT_Code39))  //added Code39 condition
	{
		//BZ RFC 212888 08/26/02 begin: added for code128 start 	
		//Raw barcode data csItemSent has a pre-appended scan type. For example, an UPC-A barcodes would start 
		//with "A", while 128-C barcodes would start with "B3". Therefore, we need to remove the "3" from the raw 
		//128-C barcodes in order to have the barcodes being processed correctly. 
		if (nItemCodeLen && (io.cScanType == 110) && (csUPCNumber.Left(2) == _T("B3")))
		{
			csUPCNumber = csUPCNumber.Left(1) + csUPCNumber.Right(nItemCodeLen - 2);
		}


		//TAR 380552
		//Barcodes would start with "]e0". Therefore, we need to remove the "0" from the raw
		if (nItemCodeLen && (io.cScanType == SCAN_SDT_GS1DATABAR || io.cScanType == SCAN_SDT_GS1DATABAR_E) && 
			(csUPCNumber.Left(3) == _T("]e0")))
		{
			csUPCNumber = csUPCNumber.Left(2) + csUPCNumber.Right(nItemCodeLen - 3);
		}
		

		nItemCodeLen -= 1;	

		// -----------------------------------------------------------------
		// Chops off any leading non-digit that the scanner has added
		// as Security wants the ORIGINAL barcode, not the one that the 
		// scanner has added lead characters to. 
		// -----------------------------------------------------------------
		// we look through the ENTIRE string for non-digits.  Smart?
		// -----------------------------------------------------------------
	
		LPTSTR data;
		if (nItemCodeLen > co.nMaxBarcodesLength)	 //Avoid exception
            data = new _TCHAR[nItemCodeLen+2];	     //In case this UPC is of 128-C		//TAR 380552
            //data = new _TCHAR[nItemLabelLen+2];	     //In case this UPC is of 128-C
		else
			data = new _TCHAR[co.nMaxBarcodesLength+2]; //In case this UPC is of type 128-C
	
		_tcscpy(data, csUPCNumber);	

		// could be this, except dont know if only 1st char.  As maybe 2nd is char
		// if (!isdigit(data[0]) data[0] = " "; 				
		for(int i = 0; i < nItemCodeLen; i++)
		{
			if(!_istdigit(data[i]))
			{
				data[i] = _T(' ');
			}
		}
		csUPCNumber = data;
		csUPCNumber.TrimLeft();

		delete [] data;	
	}

	// +229344 make sure the price embedded UPC is used during the lookup and during the update LPM022703
	if (io.b_gIsThisItemPriceEmbedded   && !io.b_gHasCustomCodeBeenFound)//226745 enable learning price embedded LPM022003  //RFC239992
	{
		csUPCNumber = io.csItemCodeWOPrice;
	}
	else if (io.b_gHasSupplementalCodeBeenTruncated)	// 226745 enable learning custom codes
	{
		csUPCNumber = io.csItemCodeSupplementalCodeTruncated;
	}//-229344

	if(io.cScanType == SCAN_SDT_GS1DATABAR || io.cScanType == SCAN_SDT_GS1DATABAR_E)   //tar 247406  //RFC 412256
	{
		csUPCNumber = csUPCNumber.Mid(0,16);       //Only save 01 + GTIN, remove other item information for RSS Expanded barcode
		trace (L6, _T("UPC used in WLDB is %s ."), csUPCNumber );
	}

	// csItemSent = csUPCNumber; // TAR 236796
	pItemCode = _bstr_t(csUPCNumber);
	return csUPCNumber;          // TAR 236796
}
#endif //_CPPUNIT
//////////////////////////////////////////////////////////
void SMStateBase::CalcObtainAndTraceSMMWtTol(bool bReplaceItemException)
{
#ifndef _CPPUNIT
	if (csSMMToleranceType == KEY_OSM_TOLERANCE_TYPE_DEFAULT )
	{
		trace (L4, _T("SMM: The tolerance used was the Default Minimum from SecurityConfig."));
	}
	else if (csSMMToleranceType == KEY_OSM_TOLERANCE_TYPE_STD_DEV )
	{
		trace (L4, _T("SMM: The tolerance used was Standard Deviation calculated."));
	}
	else if (csSMMToleranceType == KEY_OSM_TOLERANCE_TYPE_FIXED )
	{
		trace (L4, _T("SMM: The tolerance used was from the Item Exception data."));
	}
#endif //_CPPUNIT
}
#ifndef _CPPUNIT

// TAR 257709 - moved the decrease function from SMAttractBase 
// to here so it can be called from other states
void SMStateBase::AttractWtDecrease(void)
{
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("Attract") );
}
// End TAR 257709
////////////////////////////////////////////////////////////////////////////////////////////
#endif // _CPPUNIT
void SMStateBase::TellRAPAboutDelaySecurityIntervention()
{
#ifndef _CPPUNIT
	trace(L6, _T("+TellRAPAboutDelaySecurityIntervention"));

	int nSizeofSecDelayIntervention = SecMgr.GetOutstandingInterventionCount();
	SecDelayIntervention *psecDelayInterventionArray = new SecDelayIntervention[nSizeofSecDelayIntervention];
	SecMgrEventParms parms(psecDelayInterventionArray);

	CString csInterventionList = SecMgr.GetOutstandingInterventionList();
	if(!csInterventionList.IsEmpty())
	{
		parms.ParseRecords(csInterventionList);
		CString csInstruction, csDesc;

		for(int i = 0; i < nSizeofSecDelayIntervention; i++)
		{
			if(psecDelayInterventionArray[i].csSecInterventionType == KEY_OSE_EXC_TYPE_UNEX_INCREASE)
			{
			  csDesc.Format(ps.GetPSText(MSG_NEWSEC_UNEX_INCREASE, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTINCREASE, SCOT_LANGUAGE_PRIMARY), psecDelayInterventionArray[i].csDeltaWeight, csScaleUnits);
			}
			else if(psecDelayInterventionArray[i].csSecInterventionType == KEY_OSE_EXC_TYPE_UNEX_DECREASE)
			{
			  csDesc.Format(ps.GetPSText(MSG_NEWSEC_UNEX_DECREASE, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTDECREASE, SCOT_LANGUAGE_PRIMARY), psecDelayInterventionArray[i].csDeltaWeight, csScaleUnits);
			}
			else if(psecDelayInterventionArray[i].csSecInterventionType == KEY_OSE_EXC_TYPE_MISMATCHED)
			{
			  csDesc.Format(ps.GetPSText(MSG_NEWSEC_MISMATCH, SCOT_LANGUAGE_PRIMARY), psecDelayInterventionArray[i].csItemDescription, ps.GetPSText(RA_WTMISMATCH, SCOT_LANGUAGE_PRIMARY), psecDelayInterventionArray[i].csExpectedWeight, csScaleUnits, psecDelayInterventionArray[i].csActualWeight, csScaleUnits);
			}
			else if(psecDelayInterventionArray[i].csSecInterventionType == KEY_OSE_EXC_TYPE_SKIP_BAGGING_THRESHOLD)
			{
			  csDesc = ps.GetPSText(RA_SKIPBAGGINGTHRESHOLD, SCOT_LANGUAGE_PRIMARY);
			}
			else if(psecDelayInterventionArray[i].csSecInterventionType == KEY_OSE_EXC_TYPE_ITEM_REMOVED_THRESHOLD)
			{
			  csDesc = ps.GetPSText(RA_REMOVEITMESTHRESHOLD, SCOT_LANGUAGE_PRIMARY);
			}
			else if(psecDelayInterventionArray[i].csSecInterventionType == KEY_OSE_EXC_TYPE_BAG_BOX_THRESHOLD)
			{
			  csDesc = ps.GetPSText(RA_BAGBOXTHRESHOLD, SCOT_LANGUAGE_PRIMARY);
			}
			else if(psecDelayInterventionArray[i].csSecInterventionType == KEY_OSE_EXC_TYPE_NEW_ITEM_THRESHOLD)
			{
			  csDesc = ps.GetPSText(RA_NEWWEIGHTITEM, SCOT_LANGUAGE_PRIMARY);
			}
			else if(psecDelayInterventionArray[i].csSecInterventionType == KEY_OSE_EXC_TYPE_INVALID_BAG)	//TAR388990
			{
				csDesc = ps.GetPSText(RA_PUTBAGONSCALE, SCOT_LANGUAGE_PRIMARY);
			}
		}

		if(psecDelayInterventionArray[nSizeofSecDelayIntervention - 1].csSecInterventionType == KEY_OSE_EXC_TYPE_UNEX_INCREASE)
		{
			csInstruction.Format(ps.GetPSText(RA_WEIGHTINCREASEINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTINCREASE, SCOT_LANGUAGE_PRIMARY), psecDelayInterventionArray[nSizeofSecDelayIntervention - 1].csDeltaWeight, csScaleUnits, _T(""));
		}
		else if(psecDelayInterventionArray[nSizeofSecDelayIntervention - 1].csSecInterventionType == KEY_OSE_EXC_TYPE_UNEX_DECREASE)
		{
			csInstruction.Format(ps.GetPSText(RA_WEIGHTDECREASEINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTDECREASE, SCOT_LANGUAGE_PRIMARY), psecDelayInterventionArray[nSizeofSecDelayIntervention - 1].csDeltaWeight, csScaleUnits, _T(""));
		}
		else if(psecDelayInterventionArray[nSizeofSecDelayIntervention - 1].csSecInterventionType == KEY_OSE_EXC_TYPE_MISMATCHED)
		{
			csInstruction.Format(ps.GetPSText(RA_WEIGHTINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTMISMATCH, SCOT_LANGUAGE_PRIMARY), psecDelayInterventionArray[nSizeofSecDelayIntervention - 1].csExpectedWeight, csScaleUnits, psecDelayInterventionArray[nSizeofSecDelayIntervention - 1].csActualWeight, csScaleUnits);
		}
		else if(psecDelayInterventionArray[nSizeofSecDelayIntervention - 1].csSecInterventionType == KEY_OSE_EXC_TYPE_SKIP_BAGGING_THRESHOLD)
		{
			if (dm.fStateTakeawayBelt)
				csInstruction.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_WAITFORASSISTANCE, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_SKIPBELTTHRESHOLD, SCOT_LANGUAGE_PRIMARY));
			else
				csInstruction.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_WAITFORASSISTANCE, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_SKIPBAGGINGTHRESHOLD, SCOT_LANGUAGE_PRIMARY));
		}
		else if(psecDelayInterventionArray[nSizeofSecDelayIntervention - 1].csSecInterventionType == KEY_OSE_EXC_TYPE_ITEM_REMOVED_THRESHOLD)
		{
			csInstruction.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_WAITFORASSISTANCE, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_REMOVEITMESTHRESHOLD, SCOT_LANGUAGE_PRIMARY));
		}
		else if(psecDelayInterventionArray[nSizeofSecDelayIntervention - 1].csSecInterventionType == KEY_OSE_EXC_TYPE_BAG_BOX_THRESHOLD)
		{
			csInstruction.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_WAITFORASSISTANCE, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_BAGBOXTHRESHOLD, SCOT_LANGUAGE_PRIMARY));
		}
		else if(psecDelayInterventionArray[nSizeofSecDelayIntervention - 1].csSecInterventionType == KEY_OSE_EXC_TYPE_NEW_ITEM_THRESHOLD)
		{
			CString csWeight;
			csWeight.Format(_T("%s %s"), secEventParms.m_csDeltaWeight, csScaleUnits);//TAR 200109 abs value
			csInstruction.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_WAITFORASSISTANCE, SCOT_LANGUAGE_PRIMARY), csWeight, ps.GetPSText(RA_NEWWEIGHTITEM, SCOT_LANGUAGE_PRIMARY));
		}
		else if(psecDelayInterventionArray[nSizeofSecDelayIntervention - 1].csSecInterventionType == KEY_OSE_EXC_TYPE_INVALID_BAG)	//TAR388990
		{
			csInstruction.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_WAITFORASSISTANCE, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""));
		}

		// (*) SR898 ====================
        DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                           DM_TRICOLORLIGHT_ON,
                           DM_TRICOLORLIGHT_NORMALREQUEST);

		dm.saveTriColorLightStatus();
		_TCHAR TempBuffer[20];
		_itot(nSizeofSecDelayIntervention, TempBuffer, 10);
		CString csTemp(TempBuffer);
		ra.OnNormalItem(_T("[DelayCount]") + csTemp);
		ra.OnSecurityEvent(csInstruction, 990, 13);
	}
	else
	{
		trace(L6, _T("TellRAPAboutDelaySecurityIntervention, Intervention List is empty"));
	}
	delete [] psecDelayInterventionArray;
	trace(L6, _T("-TellRAPAboutDelaySecurityIntervention"));
#endif // _CPPUNIT
}
#ifndef  _CPPUNIT
BEGIN_KV_HANDLER_MAP(SMStateBase::InitializeParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::InitializeParms, KEY_DEVICE_NAME, DeviceName)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::InitializeParms, KEY_MSG_TEXT_INDEX, MsgTextIndex)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::InitializeParms, KEY_INITIALIZE_FAILURE_TEXT, FailureText)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::InitializeParms, KEY_ERROR_FILE_NAME, ErrorFilename)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::InitializeParms, KEY_ERROR_CODE, ErrorCode)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::InitializeParms, KEY_MISSING_FILE_NAME, MissingFilename)
	DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::InitializeParms, KEY_EV_APPLICATION_STATE, ApplicationState)	// RFC 330050 

END_KV_HANDLER_MAP()

////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::InitializeParms::DeviceName(CKeyValueParmList::iterator &keys)
{
    m_csDeviceName = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::InitializeParms::MsgTextIndex(CKeyValueParmList::iterator &keys)
{
    m_nMsgTextIndx = CKeyValue<int>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::InitializeParms::FailureText(CKeyValueParmList::iterator &keys)
{
    m_csFailureText = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::InitializeParms::ErrorFilename(CKeyValueParmList::iterator &keys)
{
    m_csErrorFilename = CKeyValue<const _TCHAR *>::Get(keys, NULL);
	g_csInitErrorFilename  = m_csErrorFilename;
	g_csInitMissingFilename = _T("");

}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::InitializeParms::ErrorCode(CKeyValueParmList::iterator &keys)
{
    m_lErrorCode = CKeyValue<long>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::InitializeParms::MissingFilename(CKeyValueParmList::iterator &keys)
{
    m_csMissingFilename = CKeyValue<const _TCHAR *>::Get(keys, NULL);
	g_csInitMissingFilename = m_csMissingFilename;
	g_csInitErrorFilename = _T("");
}
// + RFC 330050
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::InitializeParms::ApplicationState(CKeyValueParmList::iterator &keys)
{
    m_csApplicationState = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
// - RFC 330050
void SMStateBase::InitializeParms::TraceThis(const _TCHAR *pPairs) throw()
{
	
	trace(L6, _T("%s"), CString (pPairs) );
}

BEGIN_KV_HANDLER_MAP(SMStateBase::SecMgrEventParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMgrEventParms, KEY_OSE_ITEM_ID, ItemId)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMgrEventParms, KEY_OSE_EXCEPTION_ID, ExceptionId)
	DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMgrEventParms, KEY_OSE_DISPOSITION, Disposition)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMgrEventParms, KEY_OSE_WEIGHT_DELTA, DeltaWeight)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMgrEventParms, KEY_OSE_WEIGHT_ACTUAL, ActualWeight)
	DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMgrEventParms, KEY_OSE_WEIGHT_ACTUAL_NOTCONVERTED, ActualWeightNotConverted) //TAR 423666
	DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMgrEventParms, KEY_OSE_WEIGHT_EXPECTED_NOTCONVERTED, ExpectedWeightNotConverted) //TAR 423666
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMgrEventParms, KEY_OSE_WEIGHT_EXPECTED, ExpectedWeight)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMgrEventParms, KEY_OSE_TOLERANCE, Tolerance)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMgrEventParms, KEY_OSE_TOLERANCE_TYPE, ToleranceType)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMgrEventParms, KEY_OSE_EXC_TYPE, ExceptionType)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMgrEventParms, KEY_OSE_EXCEPTION_MSG_INDEX, ExceptionMsgIndex)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMgrEventParms, KEY_OSE_TRACE_DESCRIPTION, TraceDescription)
	DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMgrEventParms, KEY_OSE_ITEM_DESCRIPTION, ItemDescription)
	DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMgrEventParms, KEY_OSE_THRESHOLD_MAX, ThresholdMax)
	DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMgrEventParms, KEY_OSE_WEIGHT_2X, Weight2x)
	DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMgrEventParms, KEY_OSE_MULTI_PACK_ITEM, MultiPackItem) // migrating TAR 256154
	DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMgrEventParms, KEY_EV_APPLICATION_STATE, ApplicationState)	// RFC 330050 
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMgrEventParms, KEY_OSE_SOURCE_PATH, SourcePath)


END_KV_HANDLER_MAP()

BEGIN_KV_HANDLER_CLASS_CLEAR(SMStateBase::SecMgrEventParms)
    CLEAR_KV_CSTRING(csDeltaWeight)
    CLEAR_KV_CSTRING(csActualWeight)
    CLEAR_KV_CSTRING(csActualWeightNotConverted)
    CLEAR_KV_CSTRING(csExpectedWeightNotConverted)
    CLEAR_KV_CSTRING(csExpectedWeight)
    CLEAR_KV_CSTRING(csToleranceType)
    CLEAR_KV_CSTRING(csDisposition)
    CLEAR_KV_CSTRING(csExceptionType)
    CLEAR_KV_CSTRING(csTraceDescription)
    CLEAR_KV_CSTRING(csItemDescription)
    CLEAR_KV_CSTRING(csApplicationState)
    CLEAR_KV_CSTRING(csSourcePath)
    m_lItemId = 0;
    m_lExceptionId = 0;
    m_lTolerance = 0;
    m_nExceptionMsgIndex = 0;
    nSecInterventionIndex = 0;
    m_nThresholdMax = 0;
    m_bWeight2x = false;
    m_bMultiPackItem = false;
END_KV_HANDLER_CLASS_CLEAR()

////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMgrEventParms::Copy(SecDataNeededIntervention secDataNeededIntervention)
{

    m_csExceptionType = secDataNeededIntervention.csSecInterventionType;
    m_csDeltaWeight = secDataNeededIntervention.csDeltaWeight;
    m_csActualWeight = secDataNeededIntervention.csActualWeight;
    m_csExpectedWeight = secDataNeededIntervention.csExpectedWeight;
    m_csItemDescription = secDataNeededIntervention.csItemDescription;
    m_lExceptionId = secDataNeededIntervention.lExceptionId;
    m_csSourcePath = secDataNeededIntervention.csSourcePath;
    m_csDisposition = secDataNeededIntervention.csDisposition;
}

////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMgrEventParms::ItemId(CKeyValueParmList::iterator &keys)
{
    m_lItemId = CKeyValue<long>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMgrEventParms::ExceptionId(CKeyValueParmList::iterator &keys)
{
    m_lExceptionId= CKeyValue<long>::Get(keys, NULL);

}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMgrEventParms::DeltaWeight(CKeyValueParmList::iterator &keys)
{
    m_csDeltaWeight = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMgrEventParms::ActualWeight(CKeyValueParmList::iterator &keys)
{
    m_csActualWeight = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
//+TAR 423666
void SMStateBase::SecMgrEventParms::ActualWeightNotConverted(CKeyValueParmList::iterator &keys)
{
	m_csActualWeightNotConverted = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}

void SMStateBase::SecMgrEventParms::ExpectedWeightNotConverted(CKeyValueParmList::iterator &keys)
{
	m_csExpectedWeightNotConverted = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
//-TAR 423666


////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMgrEventParms::ExpectedWeight(CKeyValueParmList::iterator &keys)
{
    m_csExpectedWeight = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMgrEventParms::Tolerance(CKeyValueParmList::iterator &keys)
{
    m_lTolerance = CKeyValue<long>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMgrEventParms::ToleranceType(CKeyValueParmList::iterator &keys)
{
    m_csToleranceType = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMgrEventParms::Disposition(CKeyValueParmList::iterator &keys)
{
    m_csDisposition = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMgrEventParms::ExceptionType(CKeyValueParmList::iterator &keys)
{
    m_csExceptionType = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMgrEventParms::ExceptionMsgIndex(CKeyValueParmList::iterator &keys)
{
    m_nExceptionMsgIndex= CKeyValue<int>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMgrEventParms::TraceDescription(CKeyValueParmList::iterator &keys)
{
    m_csTraceDescription = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMgrEventParms::ItemDescription(CKeyValueParmList::iterator &keys)
{
    m_csItemDescription = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
void SMStateBase::SecMgrEventParms::ThresholdMax(CKeyValueParmList::iterator &keys)
{
    m_nThresholdMax = CKeyValue<int>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMgrEventParms::Weight2x(CKeyValueParmList::iterator &keys)
{
    m_bWeight2x = CKeyValue<bool>::Get(keys, NULL);
}
// + migrating TAR 256154
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMgrEventParms::MultiPackItem(CKeyValueParmList::iterator &keys)
{
    m_bMultiPackItem = CKeyValue<bool>::Get(keys, NULL);
}
// + RFC 330050
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMgrEventParms::ApplicationState(CKeyValueParmList::iterator &keys)
{
    m_csApplicationState = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
// - RFC 330050
// - migrating TAR 256154
void SMStateBase::SecMgrEventParms::TraceThis(const _TCHAR *pPairs) throw()
{
	
	trace(L6, _T("%s"), CString (pPairs) );
}

void SMStateBase::SecMgrEventParms::SourcePath(CKeyValueParmList::iterator &keys)
{
    m_csSourcePath = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}

HRESULT SMStateBase::SecMgrEventParms::ProcessRecord()
{
    if (m_secDelayIntervention != NULL)
    {
        m_secDelayIntervention[nSecInterventionIndex].csSecInterventionType = m_csExceptionType;
        m_secDelayIntervention[nSecInterventionIndex].csDeltaWeight = m_csDeltaWeight;
        m_secDelayIntervention[nSecInterventionIndex].csActualWeight = m_csActualWeight;
        m_secDelayIntervention[nSecInterventionIndex].csExpectedWeight = m_csExpectedWeight;
        m_secDelayIntervention[nSecInterventionIndex].csItemDescription = m_csItemDescription;
    }
    if (m_secDataNeededIntervention != NULL)
    {
        m_secDataNeededIntervention[nSecInterventionIndex].csSecInterventionType = m_csExceptionType;
        m_secDataNeededIntervention[nSecInterventionIndex].csDeltaWeight = m_csDeltaWeight;
        m_secDataNeededIntervention[nSecInterventionIndex].csActualWeight = m_csActualWeight;
        m_secDataNeededIntervention[nSecInterventionIndex].csExpectedWeight = m_csExpectedWeight;
        m_secDataNeededIntervention[nSecInterventionIndex].csItemDescription = m_csItemDescription;
        m_secDataNeededIntervention[nSecInterventionIndex].lExceptionId = m_lExceptionId;
        m_secDataNeededIntervention[nSecInterventionIndex].csSourcePath = m_csSourcePath;
        m_secDataNeededIntervention[nSecInterventionIndex].csDisposition = m_csDisposition;
    }
    nSecInterventionIndex++;

    return S_OK;
}

BEGIN_KV_HANDLER_MAP(SMStateBase::SecMaintenanceEventParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMaintenanceEventParms, KEY_OSM_OPERATION, Operation)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMaintenanceEventParms, KEY_OSM_UPC, UPC)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMaintenanceEventParms, KEY_OSM_WEIGHT_COUNT, WeightCount)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMaintenanceEventParms, KEY_OSM_WEIGHT, Weight)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMaintenanceEventParms, KEY_OSM_TALLY, Tally)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMaintenanceEventParms, KEY_OSM_APPROVED, Approved)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMaintenanceEventParms, KEY_OSM_DATE, Date)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMaintenanceEventParms, KEY_OSM_TOLERANCE_TYPE, ToleranceType)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMaintenanceEventParms, KEY_OSM_TOLERANCE, Tolerance)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMaintenanceEventParms, KEY_OSM_SUBSTITUTION_CHECK, SubstitutionCheck)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMaintenanceEventParms, KEY_OSM_WT_REQUIRED, WeightRequired)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMaintenanceEventParms, KEY_OSM_METRIC, Metric)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMaintenanceEventParms, KEY_OSM_ERROR_TYPE, ErrorType)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMaintenanceEventParms, KEY_OSM_MIN_TOLERANCE, MinTolerance)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMaintenanceEventParms, KEY_OSM_MAX_TOLERANCE, MaxTolerance)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMaintenanceEventParms, KEY_OSM_SERVER_STATUS, ServerStatus)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMaintenanceEventParms, KEY_OSM_OBSERVED_WEIGHT, ObservedWeight)
	DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecMaintenanceEventParms, KEY_EV_APPLICATION_STATE, ApplicationState) // RFC 330050

END_KV_HANDLER_MAP()
////////////////////////////////////////////////////////////////////////////////////////////
HRESULT SMStateBase::SecMaintenanceEventParms::ProcessRecord()
{
	if ( (0 != m_lWeightCount) && (m_lWeightCount > nSecMaintenanceRecordIndex) )
	{
		psecMaintenanceRecordArray[nSecMaintenanceRecordIndex].lWeightCount = m_lWeightCount;
		psecMaintenanceRecordArray[nSecMaintenanceRecordIndex].csOperation = m_csOperation;
		psecMaintenanceRecordArray[nSecMaintenanceRecordIndex].csUPC = m_csUPC;

		psecMaintenanceRecordArray[nSecMaintenanceRecordIndex].csWeight = m_csWeight;
		psecMaintenanceRecordArray[nSecMaintenanceRecordIndex].lTally = m_lTally;
		psecMaintenanceRecordArray[nSecMaintenanceRecordIndex].csApproved = m_csApproved;
		CTime ctTime(m_lDate);
		CString csDate = ctTime.Format(_T("%x"));

		psecMaintenanceRecordArray[nSecMaintenanceRecordIndex].csDate = csDate;
		psecMaintenanceRecordArray[nSecMaintenanceRecordIndex].csToleranceType = m_csToleranceType;
		psecMaintenanceRecordArray[nSecMaintenanceRecordIndex].csTolerance = m_csTolerance;
		psecMaintenanceRecordArray[nSecMaintenanceRecordIndex].csSubstitutionCheck = m_csSubstitutionCheck;
		psecMaintenanceRecordArray[nSecMaintenanceRecordIndex].csWeightRequired = m_csWeightRequired;
		psecMaintenanceRecordArray[nSecMaintenanceRecordIndex].csMetric = m_csMetric;
		psecMaintenanceRecordArray[nSecMaintenanceRecordIndex].csErrorType = m_csErrorType;
		psecMaintenanceRecordArray[nSecMaintenanceRecordIndex].csMinTolerance = m_csMinTolerance;
		psecMaintenanceRecordArray[nSecMaintenanceRecordIndex].csMaxTolerance = m_csMaxTolerance;
		psecMaintenanceRecordArray[nSecMaintenanceRecordIndex].csServerStatus = m_csServerStatus;
		psecMaintenanceRecordArray[nSecMaintenanceRecordIndex].csObservedWeight = m_csObservedWeight;
		nSecMaintenanceRecordIndex++; 
	}
	//+SSCOP-1039
	else
	{
		csSMMWeightRequired = _T("default-y");
		csSMMSubstitutionCheck = _T("default-y");
	}
	//-SSCOP-1039

    return S_OK;
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMaintenanceEventParms::Operation(CKeyValueParmList::iterator &keys)
{
    m_csOperation = CKeyValue<const _TCHAR *>::Get(keys, NULL);
	csSMMOperation = m_csOperation;
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMaintenanceEventParms::UPC(CKeyValueParmList::iterator &keys)
{
    m_csUPC = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMaintenanceEventParms::WeightCount(CKeyValueParmList::iterator &keys)
{
    m_lWeightCount = CKeyValue<long>::Get(keys, NULL);
	lSMMWeightCount = m_lWeightCount;
	if ( 0 != m_lWeightCount )
	{
		// It is guaranteed that the count will come first in the 1st record, after this we
		// can allocate the memory needed
		psecMaintenanceRecordArray = new SecMaintenanceRecord[m_lWeightCount];
	}
	
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMaintenanceEventParms::Weight(CKeyValueParmList::iterator &keys)
{
	m_csWeight = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMaintenanceEventParms::Tally(CKeyValueParmList::iterator &keys)
{
	m_lTally = CKeyValue<long>::Get(keys, NULL);

}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMaintenanceEventParms::Approved(CKeyValueParmList::iterator &keys)
{
	m_csApproved = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMaintenanceEventParms::Date(CKeyValueParmList::iterator &keys)
{
    m_lDate = CKeyValue<long>::Get(keys, NULL);

}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMaintenanceEventParms::ToleranceType(CKeyValueParmList::iterator &keys)
{
    m_csToleranceType = CKeyValue<const _TCHAR *>::Get(keys, NULL);
	csSMMToleranceType = m_csToleranceType;
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMaintenanceEventParms::Tolerance(CKeyValueParmList::iterator &keys)
{
    m_csTolerance = CKeyValue<const _TCHAR *>::Get(keys, NULL);
	csSMMTolerance = m_csTolerance;
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMaintenanceEventParms::SubstitutionCheck(CKeyValueParmList::iterator &keys)
{
    m_csSubstitutionCheck = CKeyValue<const _TCHAR *>::Get(keys, NULL);
	csSMMSubstitutionCheck = m_csSubstitutionCheck;
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMaintenanceEventParms::WeightRequired(CKeyValueParmList::iterator &keys)
{
    m_csWeightRequired = CKeyValue<const _TCHAR *>::Get(keys, NULL);
	csSMMWeightRequired = m_csWeightRequired;
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMaintenanceEventParms::Metric(CKeyValueParmList::iterator &keys)
{
    m_csMetric = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMaintenanceEventParms::ErrorType(CKeyValueParmList::iterator &keys)
{
    m_csErrorType = CKeyValue<const _TCHAR *>::Get(keys, NULL);
	csSMMErrorType = m_csErrorType;

}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMaintenanceEventParms::MinTolerance(CKeyValueParmList::iterator &keys)
{
    m_csMinTolerance = CKeyValue<const _TCHAR *>::Get(keys, NULL);
	csSMMMinTolerance = m_csMinTolerance;
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMaintenanceEventParms::MaxTolerance(CKeyValueParmList::iterator &keys)
{
    m_csMaxTolerance = CKeyValue<const _TCHAR *>::Get(keys, NULL);
	csSMMMaxTolerance = m_csMaxTolerance;
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMaintenanceEventParms::ServerStatus(CKeyValueParmList::iterator &keys)
{
    m_csServerStatus = CKeyValue<const _TCHAR *>::Get(keys, NULL);
	csSMMServerStatus = m_csServerStatus;
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMaintenanceEventParms::ObservedWeight(CKeyValueParmList::iterator &keys)
{
    m_csObservedWeight = CKeyValue<const _TCHAR *>::Get(keys, NULL);
	csSMMObservedWeight = m_csObservedWeight;
}

// + RFC 330050
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecMaintenanceEventParms::ApplicationState(CKeyValueParmList::iterator &keys)
{
    m_csApplicationState = CKeyValue<const _TCHAR *>::Get(keys, NULL);	
}
// - RFC 330050
void SMStateBase::SecMaintenanceEventParms::TraceThis(const _TCHAR *pPairs) throw()
{

	trace(L6, _T("%s"), CString (pPairs) );
}
BEGIN_KV_HANDLER_MAP(SMStateBase::SecAppControlEventParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecAppControlEventParms, KEY_OAC_OPERATION, Operation)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecAppControlEventParms, KEY_OAC_TARE, Tare)
	DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecAppControlEventParms, KEY_EV_APPLICATION_STATE, ApplicationState)	// RFC 330050
END_KV_HANDLER_MAP()
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecAppControlEventParms::Operation(CKeyValueParmList::iterator &keys)
{
    m_csOperation = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecAppControlEventParms::Tare(CKeyValueParmList::iterator &keys)
{
    m_csTare = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}

// + RFC 330050
///////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecAppControlEventParms::ApplicationState(CKeyValueParmList::iterator &keys)
{
    m_csApplicationState = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
// - RFC 330050
void SMStateBase::SecAppControlEventParms::TraceThis(const _TCHAR *pPairs) throw()
{

	trace(L6, _T("%s"), CString (pPairs) );
}
BEGIN_KV_HANDLER_MAP(SMStateBase::SecExceptionClearedEventParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecExceptionClearedEventParms, KEY_OSE_EXCEPTION_ID, ExceptionId)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecExceptionClearedEventParms, KEY_OSE_EXC_TYPE, ExceptionType)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecExceptionClearedEventParms, KEY_OSE_CLEAR_RAP_APPROVAL, ClearRAPApproval)
	DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecExceptionClearedEventParms, KEY_EV_APPLICATION_STATE, ApplicationState) // RFC 330050
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecExceptionClearedEventParms, KEY_OSE_DISPOSITION, Disposition)
END_KV_HANDLER_MAP()

////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecExceptionClearedEventParms::ExceptionId(CKeyValueParmList::iterator &keys)
{
    m_lExceptionId= CKeyValue<long>::Get(keys, NULL);

}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecExceptionClearedEventParms::Disposition(CKeyValueParmList::iterator &keys)
{
    m_csDisposition = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecExceptionClearedEventParms::ExceptionType(CKeyValueParmList::iterator &keys)
{
    m_csExceptionType = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
HRESULT SMStateBase::SecExceptionClearedEventParms::ProcessRecord()
{
	for ( POSITION p = ImmediateInterventionStack.GetHeadPosition(); p != NULL;
    			ImmediateInterventionStack.GetNext( p ) )
	{
		SecImmediateIntervention *psecImmediateIntervention =
			ImmediateInterventionStack.GetAt( p );
		ASSERT( psecImmediateIntervention != NULL );
		if ( psecImmediateIntervention->lExceptionId == m_lExceptionId )
		{
			ImmediateInterventionStack.RemoveAt( p );
			delete psecImmediateIntervention;
			break;
		}
	}
#if 0
    if (! ImmediateInterventionStack.isEmpty() )
    {
		SecImmediateIntervention *psecImmediateIntervention;
		psecImmediateIntervention = ImmediateInterventionStack.pop();
	}
#endif

    return S_OK;
}

void SMStateBase::SecExceptionClearedEventParms::ClearRAPApproval(CKeyValueParmList::iterator &keys)
{
    m_bClearRAPApproval = CKeyValue<bool>::Get(keys, NULL);
}

// + RFC 330050
///////////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecExceptionClearedEventParms::ApplicationState(CKeyValueParmList::iterator &keys)
{
    m_csApplicationState = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
// - RFC 330050


void SMStateBase::SecExceptionClearedEventParms::TraceThis(const _TCHAR *pPairs) throw()
{

	trace(L6, _T("%s"), CString (pPairs) );
}

BEGIN_KV_HANDLER_MAP(SMStateBase::SecurityControlParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecurityControlParms, KEY_SC_RELOAD_OPTIONS_STATUS_TEXT, ReloadOptionsStatusText)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecurityControlParms, KEY_SC_DEVICE_NAME, DeviceName)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecurityControlParms, KEY_SC_DEVICE_STATUS, DeviceStatus)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecurityControlParms, KEY_SC_PARAM, ParamKeyValue)
	DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecurityControlParms, KEY_EV_APPLICATION_STATE, ApplicationState)	// RFC 330050 

END_KV_HANDLER_MAP()

////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecurityControlParms::ReloadOptionsStatusText(CKeyValueParmList::iterator &keys)
{
    m_csReloadOptionsStatusText = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecurityControlParms::DeviceName(CKeyValueParmList::iterator &keys)
{
    m_csDeviceName = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecurityControlParms::DeviceStatus(CKeyValueParmList::iterator &keys)
{
    m_csDeviceStatus = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}


////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecurityControlParms::ParamKeyValue(CKeyValueParmList::iterator &keys)
{
    m_csParamKeyValue = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}

// + RFC 330050
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecurityControlParms::ApplicationState(CKeyValueParmList::iterator &keys)
{
    m_csApplicationState = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
// - RFC 330050
void SMStateBase::SecurityControlParms::TraceThis(const _TCHAR *pPairs) throw()
{

	trace(L6, _T("%s"), CString (pPairs) );
}

BEGIN_KV_HANDLER_MAP(SMStateBase::SecurityDeviceFailureEventParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecurityDeviceFailureEventParms, KEY_ODF_DEVICE_NAME, DeviceName)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecurityDeviceFailureEventParms, KEY_ODF_ERROR_CODE, ErrorCode)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecurityDeviceFailureEventParms, KEY_ODF_EXT_ERROR_CODE, ExtendedErrorCode)
	DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecurityDeviceFailureEventParms, KEY_EV_APPLICATION_STATE, ApplicationState) // RFC 330050

END_KV_HANDLER_MAP()

////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecurityDeviceFailureEventParms::DeviceName(CKeyValueParmList::iterator &keys)
{
    m_csDeviceName = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecurityDeviceFailureEventParms::ErrorCode(CKeyValueParmList::iterator &keys)
{
    m_lErrorCode = CKeyValue<long>::Get(keys, NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecurityDeviceFailureEventParms::ExtendedErrorCode(CKeyValueParmList::iterator &keys)
{
    m_lExtendedErrorCode = CKeyValue<long>::Get(keys, NULL);
}

// + RFC 330050
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecurityDeviceFailureEventParms::ApplicationState(CKeyValueParmList::iterator &keys)
{
   m_csApplicationState = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
// - RFC 330050
void SMStateBase::SecurityDeviceFailureEventParms::TraceThis(const _TCHAR *pPairs) throw()
{

	trace(L6, _T("%s"), CString (pPairs) );
}
BEGIN_KV_HANDLER_MAP(SMStateBase::SecurityControlEventParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecurityControlEventParms, KEY_OSC_OPERATION, Operation)
    DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecurityControlEventParms, KEY_OSC_WEIGHT_OBSERVED, WeightObserved)
	DEFINE_KV_HANDLER_MAP_ENTRY(SMStateBase::SecurityControlEventParms, KEY_EV_APPLICATION_STATE, ApplicationState) // RFC 330050

END_KV_HANDLER_MAP()

////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecurityControlEventParms::Operation(CKeyValueParmList::iterator &keys)
{
    m_csOperation = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}


////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecurityControlEventParms::WeightObserved(CKeyValueParmList::iterator &keys)
{
    m_csWeightObserved = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}

// + RFC 330050
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SecurityControlEventParms::ApplicationState(CKeyValueParmList::iterator &keys)
{
    m_csApplicationState = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
// - RFC 330050
void SMStateBase::SecurityControlEventParms::TraceThis(const _TCHAR *pPairs) throw()
{

	trace(L6, _T("%s"), CString (pPairs) );
}


#endif //_CPPUNIT
CString SMStateBase::BuildNeedMoreDataViewString(CString csView, CString csDescription, CString csLaneStateOneText, CString csLaneStateTwoText, CString csLaneStateThreeText, CString csResourcePath)
{
    trace(L6,_T("+SMStateBase::BuildNeedMoreDataViewString() csView = %s, csDescription = %s, csLaneStateOneText = %s, csLaneStateTwoText = %s, csLaneStateThreeText = %s, csResourcePath = %s"), csView, csDescription, csLaneStateOneText,csLaneStateTwoText,csLaneStateThreeText,csResourcePath);
    CString csViewString = _T("");
    SCOTDATAENTRY cDataEntry;

    if(csView == KEY_OSE_EXC_TYPE_MISMATCHED)
    {
        cDataEntry.FromConfigData(co.GetcsDataEntryINIFile(), _T("SMDataEntryStopLiftMismatch"), false);

    }
    else if(csView == KEY_OSE_EXC_TYPE_UNEX_INCREASE)
    {
        if (secEventParms.m_csDeltaWeight.IsEmpty())
            cDataEntry.FromConfigData(co.GetcsDataEntryINIFile(), _T("SMDataEntryStopLiftUnsolicited"), false);
        else
            cDataEntry.FromConfigData(co.GetcsDataEntryINIFile(), _T("SMDataEntryStopLiftUnexpectedIncrease"), false);
    }
    else if(csView == _T("StopLiftTheftAlert"))
    {
        cDataEntry.FromConfigData(co.GetcsDataEntryINIFile(), _T("SMDataEntryStopLiftTheftAlert"), false);
    }

    _TCHAR buf[10];
    cDataEntry.SetLaneStateOneText(csLaneStateOneText);
    cDataEntry.SetLaneStateTwoText(csLaneStateTwoText);
    cDataEntry.SetLaneStateThreeText(csLaneStateThreeText);
    cDataEntry.SetVideoPath(csResourcePath);
    cDataEntry.InsertString(_T("EXCEPTIONID"),_itot(secEventParms.m_lExceptionId,buf,10));
    csViewString = cDataEntry.ToString();

    trace(L6,_T("-SMStateBase::BuildNeedMoreDataViewString() csViewString = %s"), csViewString);

    return csViewString;
}


SMStateBase *SMStateBase::OnSecMgrNeedData(CString exceptionType, long lItemID)
{
    
    trace (L6, _T("+SMStateSABase::OnSecMgrNeedData()"));

    CString csStateName = this->GetRuntimeClass()->m_lpszClassName;
    bool bNotMakeSecInterventionImmediateOnFinalize = 
        (!(csStateName.Find(_T("SMFinish")) != -1 || csStateName.Find(_T("SMThankYou")) != -1)
            && m_bSecurityAnalysisComplete
            && bIsInFinalization
        );

    if ((g_bIsSmartAssistImmediate
        && (csStateName.Find(_T("SMInProgress")) != -1
            || csStateName.Find(_T("SMCmDataEntry")) != -1
            || csStateName.Find(_T("SMCustomMessage")) != -1
            || csStateName.Find(_T("SMPutBagOnScale")) != -1 
            )
        )
        || bNotMakeSecInterventionImmediateOnFinalize
        )
    {
        trace (L6, _T("SMStateSABase::OnSecMgrNeedData() - suppressing SecMgr DataNeeded"));
        return STATE_NULL;
    }
    else
    {
        CString sViewString, sLaneStateOneText, sLaneStateTwoText, sLaneStateThreeText, sInstructionText, sSection, sID, sItemID, sGDAsyncID;
        if(exceptionType == KEY_OSE_EXC_TYPE_MISMATCHED)
        {
            sLaneStateOneText = ps.GetPSText(RA_WTMISMATCH, SCOT_LANGUAGE_PRIMARY);
            sLaneStateTwoText.Format(ps.GetPSText(MSG_EXPECTED_WEIGHT, SCOT_LANGUAGE_PRIMARY), GetExpectedWts(), csScaleUnits);
            sLaneStateThreeText.Format(ps.GetPSText(MSG_OBSERVED_WEIGHT, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits);
            sInstructionText = ps.GetPSText(MSG_SC_MISMATCH_INSTRUCTION, SCOT_LANGUAGE_PRIMARY);
            sSection = _T("SMDataEntryStopLiftMismatch");
        }
        else if(exceptionType == KEY_OSE_EXC_TYPE_UNEX_INCREASE)
        {
            if (secEventParms.m_csDeltaWeight.IsEmpty())
            {
                sLaneStateOneText = ps.GetPSText(RA_SC_LINE1, SCOT_LANGUAGE_PRIMARY);
                sLaneStateTwoText = ps.GetPSText(RA_SC_LINE2, SCOT_LANGUAGE_PRIMARY);
                sLaneStateThreeText = ps.GetPSText(RA_SC_LINE3, SCOT_LANGUAGE_PRIMARY);
                sInstructionText = ps.GetPSText(MSG_SC_UNSOLICITED_INSTRUCTION, SCOT_LANGUAGE_PRIMARY);
                sSection = _T("SMDataEntryStopLiftUnsolicited");
            }
            else
            {
                sLaneStateOneText = ps.GetPSText(RA_WTINCREASE, SCOT_LANGUAGE_PRIMARY);
                sLaneStateTwoText.Format(ps.GetPSText(MSG_OBSERVED_WEIGHT, SCOT_LANGUAGE_PRIMARY), secEventParms.m_csDeltaWeight, csScaleUnits);
                sLaneStateThreeText = _T("");
                sInstructionText = ps.GetPSText(MSG_SC_UNEXPECTEDINCREASE_INSTRUCTION, SCOT_LANGUAGE_PRIMARY);
                sSection = _T("SMDataEntryStopLiftUnexpectedIncrease");
            }
        }
        sViewString = BuildNeedMoreDataViewString(secEventParms.m_csExceptionType, _T(""), sLaneStateOneText, sLaneStateTwoText,
            sLaneStateThreeText, secEventParms.m_csSourcePath);
        SCOTDATAENTRY cDataEntry;
        cDataEntry.FromString(sViewString);
        _TCHAR buf[10];
        sID = _itot(secEventParms.m_lExceptionId,buf,10);
        sGDAsyncID = _T("SM")+sID;
        cDataEntry.InsertString(_T("ID"), sGDAsyncID);
        cDataEntry.InsertString(_T("EXCEPTIONID"), sID);
        sViewString = cDataEntry.ToString();
        
        sItemID = _itot(lItemID, buf, 10);
        if (!g_bIsSmartAssistImmediate && !bIsInFinalization)
        {
            CDataNeededUtil dataNeededUtil(sInstructionText, sViewString, _T(""), sSection);
            
            bool bIsItemVoided = false;
            if (ps.IsThisItemVoided(sItemID, bIsItemVoided))
            {
                if (bIsItemVoided)
                {
                    trace( L6, _T("SMStateSABase::OnSecMgrNeedData() ItemID:%s is already voided"), sItemID );
                    SendSmartAssistExceptionClear( _T("LANE"), sID);
                    return STATE_NULL;
                }
            }
            SetItemGDAsyncIDReceiptVariable(_T("CMSMReceipt"), sItemID, sGDAsyncID);
            return OnGenericDelayedIntervention(dataNeededUtil);
        }
        else
            return OnNeedData(sInstructionText, sViewString, co.GetcsDataEntryINIFile(), sSection); 
    }
    
    trace (L6, _T("-SMStateSABase::OnSecMgrNeedData()"));
}

void SMStateBase::SendSmartAssistExceptionClear(CString csAuthorization, CString csExceptionID)
{
    CKEYVALUEPARMLIST parmList;
    trace(L6, _T("+SMStateBase::SendSmartAssistExceptionClear Authorization= %s Operation = %s"), csAuthorization, KEY_EXCEPTION_OPERATION_CLEAR);
    CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_CATEGORY, KEY_EXCEPTION_CATEGORY_CLEAR_DATA_NEEDED );
    CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_AUTHORIZATION, csAuthorization);
    CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_OPERATION, KEY_EXCEPTION_OPERATION_CLEAR);
    CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_ID, csExceptionID);
    _bstr_t bstrParmList2((const _TCHAR *)parmList);
    SecMgr.OverrideException(lSecMgrCookie, bstrParmList2);
}

bool SMStateBase::HandleDataNeededSecureCam(CString &csData)
{
    trace (L6, _T("+SMStateSABase::HandleDataNeededSecureCam()"));
    if (!g_bIsSmartAssistImmediate
        && m_GenericDelayedInterventionStatus == GENERIC_DELAYED_NOT_PROCESSING
        && !csData.IsEmpty())
    {
        SendSmartAssistExceptionClear( _T("RAP"), ExtractString(csData, _T("ID")).Mid(2));

        CQueuedActionGenericDelayedIntervention *pDelayedIntervention = (CQueuedActionGenericDelayedIntervention*) m_GenericDelayedInterventions.Get(ExtractString(csData, _T("ID")));
        if (pDelayedIntervention)
        {
            ra.OnGenericDelayedApproval(pDelayedIntervention->GetActionID(), pDelayedIntervention->GetReportingID(), true);
        }
        SAClearSecurityCondition();
        g_csDataNeededHandler.Empty();
        return true;
    }
    if (ps.RemoteMode())
    {
        OverrideSecurityException(_T("RAP"), KEY_EXCEPTION_OPERATION_CLEAR);
    }
    else
    {
        OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR);
    }

    trace (L6, _T("g_csSecureCamInterventionType = %s"), g_csSecureCamInterventionType);

    if(g_csSecureCamInterventionType == _T("SMART ASSIST TRUST LEVEL"))
    {

        SCOTDO_INFO securityInfo;
        ra.GetSecurityInfo(&securityInfo);

        if(securityInfo.bApprovalRequired)
        {
            trace(L7, _T("+ra.OnSecurityApproval"));
            ra.OnSecurityApproval();
            trace(L7, _T("-ra.OnSecurityApproval"));
        }

        bSecurityTrustLevelAlert = false;
    }

    SAClearSecurityCondition();

    trace (L6, _T("-SMStateSABase::HandleDataNeededSecureCam()"));
    return true;
}

//(+)SSCOADK-544
void SMStateBase::SetAttendantModeStateInSM(bool bAttendantModeState)
{
    trace (L6, _T("+SMStateSABase::SetAttendantModeStateInSM()"));

    CKEYVALUEPARMLIST parmList;

    if(bAttendantModeState)
    {
        CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_ENTER_ATTENDANT_MODE);
    }
    else
    {
        CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_EXIT_ATTENDANT_MODE);
    }

    _bstr_t bstrParmList((const _TCHAR *)parmList);
    BSTR bstrResult;
    bstrResult = NULL;
    SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
    SysFreeString(bstrResult);

    trace (L6, _T("-SMStateSABase::SetAttendantModeStateInSM()"));
}
//(-)SSCOADK-544

#ifndef _CPPUNIT
bool SMStateBase::AnySMDataNeeded()
{
    bool bReturn = false;
    trace(L6, _T("+SMStateBase::AnySMDataNeeded"));

    int nSizeofSecIntervention = SecMgr.GetOutstandingInterventionCount();

    trace(L6, _T("SMStateBase::AnySMDataNeeded nSizeofSecIntervention = %d"),nSizeofSecIntervention);
    if ( 0 != nSizeofSecIntervention)
    {

        SecDataNeededIntervention *psecDataNeededInterventionArray = new SecDataNeededIntervention[nSizeofSecIntervention];
        SecMgrEventParms parms(psecDataNeededInterventionArray);

        CString csInterventionList = SecMgr.GetOutstandingInterventionList();
        if(!csInterventionList.IsEmpty())
        {
            parms.ParseRecords(csInterventionList);

            for(int i = 0; i < nSizeofSecIntervention; i++)
            {
                if(psecDataNeededInterventionArray[i].csDisposition == KEY_OSE_DISPOSITION_DATANEEDED)
                {
                    bReturn = true;
                    break;
                }

            }
        }

        delete [] psecDataNeededInterventionArray;
    }
    trace(L6, _T("-SMStateBase::AnySMDataNeeded %d"), bReturn);

    return bReturn;
}


SMStateBase *SMStateBase::ProcessSMDataNeeded()
{
    trace(L6, _T("+SMStateBase::ProcessSMDataNeeded"));
    int nSizeofSecIntervention = SecMgr.GetOutstandingInterventionCount();
    SecDataNeededIntervention *psecDataNeededInterventionArray = new SecDataNeededIntervention[nSizeofSecIntervention];
    SecMgrEventParms parms(psecDataNeededInterventionArray);

    CString csInterventionList = SecMgr.GetOutstandingInterventionList();
    if(!csInterventionList.IsEmpty())
    {
        parms.ParseRecords(csInterventionList);

        for(int i = 0; i < nSizeofSecIntervention; i++)
        {
            if(psecDataNeededInterventionArray[i].csDisposition == KEY_OSE_DISPOSITION_DATANEEDED)
            {
                secEventParms.Clear();
                secEventParms.Copy(psecDataNeededInterventionArray[i]);
                delete [] psecDataNeededInterventionArray;
                return OnSecMgrNeedData(secEventParms.m_csExceptionType, secEventParms.m_lItemId);
            }

        }
    }

    delete [] psecDataNeededInterventionArray;
    trace(L6, _T("-SMStateBase::ProcessSMDataNeeded"));
    return STATE_NULL;
}
#endif //_CPPUNIT

void SMStateBase::SendSecureCamConfigToSecMgr(bool bIsSecureCamEnable)
{
    trace (L6, _T("+SMStateSABase::SendSecureCamConfigToSecMgr()"));

    CKEYVALUEPARMLIST parmList;

    if(bIsSecureCamEnable)
    {
        CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_ENABLE_SECURECAM);
    }
    else
    {
        CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_DISABLE_SECURECAM);
    }

#ifndef _CPPUNIT
    CKEYVALUE<int>::Put(parmList, KEY_AC_ANALYSIS_COMPLETE_TIMER, (g_nSecurityAnalysisTimer * 1000)); //in milliseconds
#endif //_CPPUNIT
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    BSTR bstrResult;
    bstrResult = NULL;
    SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
    SysFreeString(bstrResult);

    trace (L6, _T("-SMStateSABase::SendSecureCamConfigToSecMgr()"));
}
