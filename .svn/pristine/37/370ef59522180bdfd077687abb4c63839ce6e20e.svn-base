//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMThankYouBase.cpp
//
// TITLE: Class implementation for Thank You State
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMThankYou.h"         // This state
#include "DMProcedures.h"
#include "SMSecUnExpectedIncrease.h"
#include "MPProcedures.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("ThankYouBase")


DLLEXPORT extern CSCOTDataEntry cDataEntry;
DLLEXPORT extern HWND hw;           
DLLEXPORT extern MPProcedures mp;

IMPLEMENT_DYNCREATE(SMThankYouBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(ThankYou)

DEFINE_TIMESTAMP

bool SMThankYouBase::ms_bEscalationTimeOutFired = false;

//////////////////////////////////////////
SMThankYouBase::SMThankYouBase()
    : fThankYouAudioPlayed(false),
      m_bSSFReminder1(false),
      m_bNormalExit(false)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////
SMStateBase  *SMThankYouBase::Initialize(void)
{
  COleVariant v;
  HKEY  hKey;
  //+SR678
  if (bIsCouponExceedLimitFinalization && (!CouponTally(lTotalSale, lTotalCouponAmount)))
  {
	bIsCouponExceedLimitFinalization = false;
	return CouponMaxInterventionFinal();
  }
  //-SR678
  if((lItems>0) && (!TrainingModeObj.IsTMOn()))		// TAR 409543 
  {
	m_pAppPSX->GetTransactionVariable( _T("TransactionCount"), v );
	v = (long)(v.lVal + 1);
	m_pAppPSX->SetTransactionVariable( _T("TransactionCount"), v );
	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\TransactionCount"), 
       0, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,NULL,&hKey, NULL) == ERROR_SUCCESS)
	{
	  RegSetValueEx(hKey, _T("TransactionCount"), 0L, REG_DWORD, (LPBYTE)&v.lVal, sizeof(DWORD));
	  RegCloseKey(hKey); 
	}
  }

  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_THANKYOU, SCOT_LANGUAGE_PRIMARY));
  TBEnterTenderState(false);   //Tar 138312
  setAnchorState(BES_THANKYOU);
  // CANNOT enter Remote Assist Mode at this state
  //ra.OnSetTransactionState(RA_STATE_ASSISTNOTOK);
  ra.RequestEnableAssistMode(false);   //TAR 213249

  if (custInfo.GetPreference(_T("LangSelectionMethod")) == _T("None"))
  { //Restore access to preference value sent by server
     custInfo.SetPreference(_T("LangSelectionMethod"), custInfo.GetPreference(_T("LangSelectionMethod"), true));
  }
  custInfo.UpdatePreferences();

  // THP - Removed this  function to SMAttractBase.cpp  due to Security Tag Screen feature
  // If calling ResetAllTransactionValues() here, it will reset all the global variables and receipt list.
  // ResetAllTransactionValues();
  // ensure all counters. totals, flags are reset
  
  /* TAR390688
  //TAR381316+
  //if g_bInterventionApproval is true, that means this is second time FL comes to ThankYou and g_bInterventionApproval
  //hasn't been cleared it, so there is still an outstanding dataneeded intervention. We don't want to call
  //ra.OnTransactionEnd() because it will make RCM clear the intervention. 
  if (!g_bInterventionApproval)  
  {
     trace(L7, _T("+ra.OnTransactionEnd"));
     ra.OnTransactionEnd();
     trace(L7, _T("-ra.OnTransactionEnd"));
  }
  //TAR381316-
  */

  SASendSCOTInput(TRANSACTION_COMPLETE );

  //Tar 122712 reset
  bFinish = false;
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ThankYou") );

  trace(L7, _T("+ra.OnSetTransactionState, Inactive State"));
  ra.OnSetTransactionState(RA_STATE_INACTIVE);
  trace(L7, _T("-ra.OnSetTransactionState"));

  g_lNumberOfWLDBEntriesThisItem = -1;
  // Tar 219857 RF 111302 if events are fired then do not reset the timeout, process it
  bDoNotResetTimeOutOnDMEvent = true;  // Don't let DMEvents affect timers
// NewUI
  ps.SetCMFrame(_T("ThankYou"));

  CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMThankYouBase::Initialize"));

  if(ps.ScanReceiptForSecurityTagItems() && (!fInSuspendTrans) && (!fInVoidTrans))
  {
	  ps.SetCMLeadthruText(TTL_SECURITYTAG);
  }
  else
  {
	  ps.SetCMLeadthruText(LTT_THANKYOU);
  }
// NewUI


  //+RFC #282631
	//Launch AutoPushDiagFiles.bat
	if (m_bTBNeedAssistModeGetDiag)
	{
		//Get AutoPushDiagFiles.bat path
		LONG     lRC;
		HKEY     hKey;
		_TCHAR  szFilePath[_MAX_PATH+1];
		DWORD dwDataSize=_MAX_PATH;

		lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\Load Controller"),
				 0, KEY_QUERY_VALUE, &hKey);
		ASSERT(lRC==ERROR_SUCCESS);

		lRC = RegQueryValueEx(hKey, _T("OutOfSyncDiagFile"), NULL, NULL, (LPBYTE)szFilePath, &dwDataSize);

		ASSERT(lRC==ERROR_SUCCESS ) ;
		m_bTBNeedAssistModeGetDiag = false;
    Out_of_sync_Trace_Object.CloseTraceFile(); //dp185016 tar 286968
    //+dp185016 tar 287258
    trace(L6, _T("Get Registry Key Value of OutOfSyncDiagFile : %d"), lRC);

    if( ERROR_SUCCESS == lRC )
    {
      //+additional checking since 4.0 build 43 base, patch A build 49 still registers the 
      //autopushdiagfiles.bat in registry, even if it is not part of that patch.
  		_TCHAR  szDiagPath[_MAX_PATH+1];
  		lRC = RegQueryValueEx(hKey, _T("GetDiagFiles"), NULL, NULL, (LPBYTE)szDiagPath, &dwDataSize);
      
      // we check if GetDiagFiles key has autopushdiagfiles.bat, then autopush getdiag feature
      // installed, else, we use the getdiagfiles.exe
      trace(L6, _T("Value of GetDiagFiles key is : %s"), szDiagPath);

      if( _tcsstr( szDiagPath, _T("AutoPushDiagFiles" ) ) == NULL )
	  {
        _tcscpy( szFilePath, OUTOFSYNCTEMPORARYPARAMETER );
			ExpandPartitionStrings(szFilePath, szFilePath);
	  }
      //-
    }
    else
	{
      _tcscpy( szFilePath, OUTOFSYNCTEMPORARYPARAMETER );
		ExpandPartitionStrings(szFilePath, szFilePath);
	}
    //-dp185016
		bool rc = LaunchProgram(szFilePath, 0, false);
		RegCloseKey(hKey);
	}
	//-RFC #282631

  nTimeOut = TIMEOUT_SECURITY_WAIT_AT_THANKYOU;

  CString csMsg12;
  if(ps.ScanReceiptForSecurityTagItems() && (!fInSuspendTrans) && (!fInVoidTrans))
  {
      csMsg12 = ps.GetPSText(MSG_SECURITYTAG);
  }
  if(m_bSSFReminder1)
  {
      csMsg12 = csMsg12 + _T("\n");
      csMsg12 = csMsg12 + ps.GetPSText(MSG_THANKYOUFIGTCARD);
  }
  if (csMsg12.IsEmpty())
  {
      csMsg12 = ps.GetPSText(MSG_THANKYOUFORUSINGSCOT);
  }
  if (!csMsg12.IsEmpty())
    ps.Message12Modal(csMsg12);

  // only want to stay on the screen for a short time if the weight is already correct
  // Check to see if weight is already correct.

  if (bTransactionComplete)
  {
      // Weight is correct, so say thank you immediately and then set a timeout
      // to remain on the Thank You screen for a minimum time.
      sayThankYou();
	  
	  //+TAR 358816
	  if (fInVoidTrans || (lVoidedItems > 0 && lItems <= 0))
	  {
		 ps.Message12(MSG_SETALLASIDE,true);  
	  }
	  else if(fInSuspendTrans)
	  {
		  ps.Message12(MSG_TRANSUSPENDED,true);
	  }
	  //-TAR 358816

      if (dm.fStateTakeawayBelt)
      {
          // For Takeaway Belt, use the configured minimum time.
          nTimeOut = co.getTimeOutThankYouTimerBelt();
      }
      else
      {
          if(m_bSSFReminder1)
          {
              nTimeOut = 3; // add 2 seconds delay to remind customer to return gift card for store-collection or a similar Solution specific reminder
          }
          else
          {
              // If not Takeaway Belt, stay on the screen for 1 second.
	        nTimeOut = 1;
          }
      }
  }
  else	// else, the weight is not correct yet, so run the normal timer
  {
	  //+RFC 276161
	  // TAR276161 - ji/ - Need to change "Take your items" message on cancel all
	  if ( fInVoidTrans || fInSuspendTrans || (lVoidedItems > 0 && lItems <= 0))
	  {
		  displaySuspendVoid();  
	  }else
	  //-RFC 276161
	  {
		  if( !bTransactionComplete && !fInVoidTrans && !fInSuspendTrans && !(lVoidedItems>0 && lItems<=0) )
//added for TAR 358816 // 397234
			  ps.Message12(MSG_PLEASE_TAKE_YOUR_ITEMS, true);
	  }
	  nTimeOut = co.getTimeOutInitialTakeYourItemAudio();

      
      if(co.getTimeOutTakeYourItemEscalation() > 0 && !ms_bEscalationTimeOutFired)
      {
          SetTimer(hw, RAP_ESCALATION, co.getTimeOutTakeYourItemEscalation(), &EscalationTimedOut);
      }
      else
      {
          SetTimer(hw, RAP_ESCALATION, 1, &EscalationTimedOut);
      }  
  }
  // Display the Auxilary thank-you screen
  DisplayAuxScreen(AUX_SCREEN_THANK_YOU, true);

  ps.CMButton(_T("ButtonHelp"), BTT_HELP,false);

  ps.ShowCMFrame(false);

  if(!fInSignedOn && !fInSignedOff)
	  return STATE_NULL;
  else
	  return TBFinish();
}

//////////////////////////////////
SMStateBase  *SMThankYouBase::TimedOut(void)
{
  // Transaction over, start Attract clean
  //rfc209702 dm.turnOffTriColorLight();

  if (bTransactionComplete)
  {
      m_bNormalExit = true;
	  //TAR 379317+
	  if (!g_bInterventionApproval )
	  {
		  bTransactionComplete = false;
	  }
	  //TAR 379317+
	  setAnchorState(BES_START);
      sayThankYou();

	  if (dm.IsCashDrawerAvailable())	// Cashdrawer/ExitGate is available check. awk.1
	  {								// if it is available wait until security give go ahead then open.
		  dm.CashDrawerOpen();	//Cashdrawer/ExitGate open
	  }

      ms_bEscalationTimeOutFired = false;

      bDoNotResetTimeOutOnDMEvent = false; // TAR 219857 RF 111302 - leaving ThankYou, back to normal
	  if(!fInSignedOn && !fInSignedOff)
      {
          fSAInTransaction = false;
		  ClearStateStackEOT();
		  //TAR 371932 & tar 371918+
		  if (g_bInterventionApproval)
		  {   
			  trace (L5, _T("Stay at ThankYou until WMRA_SEND_INTERVENTION_APPROVED received"));
			  return STATE_NULL;
		  }
		  else
		  {
			 //TAR390688+
            trace(L7, _T("+ra.OnTransactionEnd"));
	        ra.OnTransactionEnd();
	        trace(L7, _T("-ra.OnTransactionEnd"));  
			//TAR390688-
			return createAnchorState();
		  }
		  //TAR 371932 & tar 371918-

      }
	  else
		  return TBFinish();
  }
  else	  //else give reminder messages to remove items from smartscale
  {
	  //+RFC 276161	
	  if ( fInVoidTrans || fInSuspendTrans)
	  {
		  displaySuspendVoid();
	  }else
	  //-RFC 276161	
	  {
		 ps.Message12(MSG_PLEASE_TAKE_YOUR_ITEMS, true);
		 DMSaySecurity( SEC_NONE, ps.GetSoundName(SEC_PLSTAKEYOURITEMS));
	  }

      // Even if we already played the Thank You audio once, we'll need it
      // again after we get back out of this condition.
      fThankYouAudioPlayed = false;
      nTimeOut = co.getTimeOutRecurringTakeYourItemAudio();
	  return STATE_NULL;
  }
}


///////////////////////////////////////////////////////////////////////////
SMStateBase *SMThankYouBase::OnWtDecrease()
{
	FinalizationWtDecrease();
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMThankYouBase::OnWtIncreaseNotAllowed()
{
	if (co.IsSecurityBypassOn()	)
	{
		OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR);
		return STATE_NULL;
	}

	return FinalizationWtIncrease(getAnchorState());
}

//////////////////////////////////////////////////////////////////////
// Internal helper functions
//////////////////////////////////////////////////////////////////////

void SMThankYouBase::sayThankYou()
{
    if (!fThankYouAudioPlayed)
    {
        if (ps.ScanReceiptForSecurityTagItems() && !fInSuspendTrans && !fInVoidTrans)
            DMSaySecurity( SEC_NONE, ps.GetSoundName(THANKYOUSECURITYTAGREMINDER),
                           ps.GetVolEventHandle());     // TAR 351654 - Add notification event.
        else
		{
			// TAR276161 - ji/ - Need to change "Take your items" message on cancel all
			//removed the if else statement for TAR 358816
			/* - if (lVoidedItems > 0 && lItems <= 0)
			{
				displaySuspendVoid();
			}
			else
			{
				DMSaySecurity( SEC_NONE, ps.GetSoundName(THANKYOUFORUSING));
			}*/

            // TAR 351654 - Add notification event.
            DMSaySecurity( SEC_NONE, ps.GetSoundName(THANKYOUFORUSING), ps.GetVolEventHandle());
		}
    }
    fThankYouAudioPlayed = true;
}

void SMThankYouBase::UnInitialize()
{
	KillTimer(hw,RAP_ESCALATION);     //RFC209702
	// Transaction over, start Attract clean
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ThankYou") );

    // TAR 268001 - If an event such as a system message because of the receipt printer
    // takes us out, don't clear everything, we'll be back
    if (m_bNormalExit)
    {
        // TAR 249233 Don't call SAClearSecurityCondition because it makes the takeaway belt 
        // takeaway belt jump
        ClearSecurityViolationLight();//End of Tar 209034 & 209045
		if (fRequestForHelp)
		{
			fRequestForHelp = false;
			trace(L7, _T("+ra.OnCallForHelpApproval"));
			ra.OnCallForHelpApproval();
			trace(L7, _T("-ra.OnCallForHelpApproval"));
		}

	    //Clear RAP Approval 
	    trace(L2, _T("Calling ra.OnNoRAPDataNeeded()"));
	    ra.OnNoRAPDataNeeded();  
	    
        dm.TriColorLightEndTransaction();
        dm.turnOffTriColorLight();
        // Do not enable remote assist mode here because we could be going to help or system 
        // message screen where remote assist mode is of no help.
        // We enable it first thing at SMAttractBase::Initialize.
	    //tar 254390 ra.RequestEnableAssistMode(true);   //TAR213249
	    bQuietNeedSecApproval = false; //Tar 224576
    }
}

//merged TAR232645 - fixed by Chen
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMThankYouBase::OnBackToLGW(void)
{
	trace(L2, _T("SMThankYouBase::OnBackToLGW: no need to process BackToLGW."));

	//If there is WaitForApproval state waiting, clear it
	if (g_pDelay_WaitForApprovalStateChange != NULL)
	{
		CString csStateName = g_pDelay_WaitForApprovalStateChange->GetRuntimeClass()->m_lpszClassName;
		delete g_pDelay_WaitForApprovalStateChange;
		g_pDelay_WaitForApprovalStateChange = NULL;
		trace(L6,_T("OnBackToLGW - Delete the state %s saved in g_pDelay_WaitForApprovalStateChange"),
			     csStateName);
	}

	return TimedOut();
}

//RFC209702+
///////////////////////////////////////
VOID CALLBACK SMThankYouBase::EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	mp.sCurr->Escalation2RAP();
}

void SMThankYouBase::Escalation2RAP()
{ 
    trace(L2, _T("+SMThankYouBase::Escalation2RAP Escalate=%d"), co.getStateEscalateTakeYourItemsTimeout());
    KillTimer(hw,RAP_ESCALATION);           
    ms_bEscalationTimeOutFired = true;
    if( (!bTransactionComplete && co.getStateEscalateTakeYourItemsTimeout()) &&
	    !( co.fAutoRunOn && co.fAutoRunBypassBagging ) ) 	//+rfc374303
    {
	    
	    dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
             DM_TRICOLORLIGHT_BLINK_1HZ,
             DM_TRICOLORLIGHT_NORMALREQUEST);
	    cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("ItemsForgotten"));
		m_View = DATANEEDED_THANKYOUEXPIRED;
		CString csSecInfo;

	    csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), _T(" "), ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(MSG_ITEMSREMAINAT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(MSG_ITEMSREMAINAT2, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_TIMEREXPIRED, SCOT_LANGUAGE_PRIMARY));

        ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
		g_bInterventionApproval = true; //TAR 371932 & tar 371918
       
        ps.CMButton(_T("ButtonHelp"), BTT_HELP,true);//rfc209702 allow access to system message screen so they can clear violation from lane
    }
    else
    {
        nTimeOut = 1;

		CKeyValueParmList parmList;
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_TC_OPERATION_EXPIRED);
		_bstr_t bstrParmList((const _TCHAR *)parmList);
		SecMgr.TransactionControl(lSecMgrCookie, bstrParmList);
        trace(L6, _T("g_lBeginningTransLGW is now reset to %d"),g_lBeginningTransLGW);
    }
    trace(L2, _T("-SMThankYouBase::Escalation2RAP"));
}
//RFC209702-

SMStateBase *SMThankYouBase::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
	trace(L2, _T("+SMThankYouBase::OnRAPClearSecurityBtn"));
    
        // TAR 249233 Don't call SAClearSecurityCondition because it makes the takeaway belt 
    // takeaway belt jump
    ClearSecurityViolationLight();//End of Tar 209034 & 209045

	//Clear RAP Approval 
	trace(L2, _T("Calling ra.OnNoRAPDataNeeded()"));
	ra.OnNoRAPDataNeeded();  

    trace(L6, _T("g_lBeginningTransLGW is now reset to %d"),g_lBeginningTransLGW);
    // NewSec
	CKeyValueParmList parmList;
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_TC_OPERATION_EXPIRED);
	_bstr_t bstrParmList((const _TCHAR *)parmList);
	SecMgr.TransactionControl(lSecMgrCookie, bstrParmList);

	trace(L2, _T("-SMThankYouBase::OnRAPClearSecurityBtn"));
    return TimedOut();
}


////////////////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMThankYouBase::OnTransactionComplete()
{
	//TAR 371932 & tar 371918+
	if (g_bInterventionApproval)
	{
		g_bInterventionApproval = false; //TAR402332
		//Clear RAP Approval so it tells report about interventionend
		trace(L2, _T("Calling ra.OnNoRAPDataNeeded()"));
		ra.OnNoRAPDataNeeded();  

	}
	//TAR 371932 & tar 371918-

	return TimedOut();
}

//+RFC 276161
void SMThankYouBase::displaySuspendVoid()
{
	// TAR276161 - ji/ - Need to change "Take your items" message on cancel all
	if ( fInVoidTrans || (lVoidedItems > 0 && lItems <= 0))
	{
		ps.Message12( MSG_SETALLASIDE,true );
		DMSayPhrase( VOIDTRANS_SETALLITEMSASIDE );
	}else
	{
		ps.Message12( MSG_TRANSUSPENDED,true );
		DMSayPhrase( SUSPENDTRANS_SETALLITEMSASIDE );
	}
		
	
		  
}
//-RFC 276161

//TAR 371932 & tar 371918+
SMStateBase *SMThankYouBase::RAParse(MessageElement *me)
{
	trace(L5,_T("SMThankYouBase::RAParse"));
 	trace(L4,_T("SMThankYouBase::RAParse: me->raInfo.msg = %d on thread 0x%08.8x"), me->raInfo.msg, GetCurrentThreadId());
	 
    if (me->raInfo.msg == WMRA_SEND_INTERVENTION_APPROVED)
	{
		g_bInterventionApproval = false;
		//TAR379317+
		if (!bTransactionComplete)
		{
			//there is still weight on bagscale, wait on security
			return STATE_NULL;
		}
		else
		{
			return TimedOut();
		}
		//TAR379317-

	}
    
    #ifdef _BUILDLIB_
		return SMStateBase::RAParse(me);
	#else
		return SMState::RAParse(me);
	#endif
    return SMStateBase::RAParse(me);


}


//SMStateBase *SMThankYouBase::stateAfterPasswordVerify()
//{
//	trace(L2, _T("SMThankYouBase::stateAfterPasswordVerify"));
//	binterventionApproval = false;
//	//Clear RAP Approval so it tells report about interventionend
//	trace(L2, _T("Calling ra.OnNoRAPDataNeeded()"));
//	ra.OnNoRAPDataNeeded();  
//
//	return SMSystemMessageOpPassBase::stateAfterPasswordVerify();
//
//}
//TAR 371932 & tar 371918-

