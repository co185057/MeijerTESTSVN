//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAuthorizationBase.CPP
//
// TITLE: Class implementation for store mode Authorization state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"
#include "SMSmAuthorization.h"
#include "SMSmRestrictedItems.h"
#include "SMSmVoidedItems.h"
#include "SMSmCouponItems.h"
#include "SMSmVisualItems.h"
#include "SMDepositCoupons.h"
#include "SMSmSecurityLogs.h"
#include "SMSmMediaStatus.h"
#include "SMSmAssistMode.h"
#include "SMSmAssistMenu.h"
#include "SMSmSystemFunctions.h"
#include "SMWaitForApproval.h"
#include "SMLaneClosed.h"
#include "SMSmConfirmSuspendedTransaction.h"
#include "SMSmReportsMenu.h"
#include "SMSystemMessage1.h"
#include "DMProcedures.h"
#include "RemoteMessageAPI.h"		// External library ...\platform\RemoteMessage
#include "SMDegradedContinueTrans.h"			//degraded mode tars
//USSF START
#include "SMUSSFManager.h"
#include "USSFCommon.h"
//USSF END
#include "SMInProgress.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmAuthorizationBase")

IMPLEMENT_DYNCREATE(SMSmAuthorizationBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmAuthorization)
bool bReturn2Shopping = false;		// Tar 213624 RJF 090602
bool bSetSignedOn = false;			// TAR295879

DEFINE_TIMESTAMP

//////////////////////////////////////
SMSmAuthorizationBase::SMSmAuthorizationBase()
: sReturnState(STATE_NULL), csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////////
SMStateBase  *SMSmAuthorizationBase::Initialize(void)
{
	// TAR 397386 - Check if we are in Multi-item picklist before sending state to Security Manager.
	if(!fInMultiSelectPickList)
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmAuthorization") );
	}
   bReturn2Shopping = false;			// Tar 213624 RJF 090602 
   nStoreModeMenuState = BES_STOREAUTHORIZATION;//Current Storemode menu state

  //Tar 218158
  //Keep these lines first to avoid getting out of sync with
  //RAP.
  if (!ps.RemoteMode())
  {
     ra.RequestEnableAssistMode(false);

	 if ( !bStoreModeGoBack )
	 {
		 // TAR295879
		 // Synchronize the Store login with LaunchPad so that if the user turns the keyswitch to
		 // Normal (supervisor mode), the signon status will match scotapp.
 		 LPCTSTR szTarget = _T("LaunchPadNet.exe");
		 if ( 0 != ::FindWindow( NULL, szTarget ))
		 {
			LPCTSTR szRights = _T("1");		// Currently the rights are only boolean 0 and 1
 			CString strMessage = _T("operation=set-signed-on;operator=") + csOperator + _T(";rights=") + szRights;
 			LPCTSTR szMessage = LPCTSTR(strMessage);
			SynchronizeStoreloginWithLaunchPad(strMessage); // TAR 301706
		 }
	 }

	 if ( getAnchorState() == BES_THANKYOU )//rfc209702
	 {
		 TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAuthorization") );
         if ( bTransactionComplete )
		 {
   		    //TAR 371932 & tar 371918+
			if (g_bInterventionApproval)
			{
				//Clear RAP Approval so it tells report about interventionend
				trace(L2, _T("Calling ra.OnNoRAPDataNeeded()"));
				ra.OnNoRAPDataNeeded();  

			}
			//TAR 371932 & tar 371918-

             return createAnchorState();
		 }
         else
		 {
			CKeyValueParmList parmList;
			CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_TC_OPERATION_EXPIRED);
			_bstr_t bstrParmList((const _TCHAR *)parmList);
			SecMgr.TransactionControl(lSecMgrCookie, bstrParmList);
			//TAR 371932 & tar 371918+
	        if (g_bInterventionApproval)
			{
		        //Clear RAP Approval so it tells report about interventionend
		        trace(L2, _T("Calling ra.OnNoRAPDataNeeded()"));
		        ra.OnNoRAPDataNeeded();  

			}
			//TAR 371932 & tar 371918-
             CREATE_AND_DISPENSE(SystemMessage1)(	NULL,           // no char *message
									            MSG_WEIGHTONSCALEAFTERTRANX,
									            PS_NOTEXT,		// no prompt
									            0,				// no prompt value
									            -1,				// no device
									            false);			// no DM check health needed
		 }
	 }
  }
  else
  {
	  trace(L6, _T("RAP entering remote store mode"));
  }
  //Tar 218158
  // Instead of entering store mode and waiting for intervention button to be pushed
  // we will automatically check if there are unapproved interventions and
  // if so, go directly to those states
  // Handle any action required interventions first
  // When each is done, logic will return to this function

  if((!bStoreModeGoBack) || (bForceApprovalBeforeSuspend))//TAR 364905
  {
//USSF Start
	  if (co.fOperationsEnableUSSF)
		USSFEVENT(_T("USSFApproval"), if (nextState != STATE_NULL){return nextState;}, ; );
//USSF End

      if (isRestrictedItemApprovalRequired())
          RETURNSTATE(SmRestrictedItems)

      if (isCouponRatioApprovalRequired())
          RETURNSTATE(SmCouponItems)

      if (isVisualItemApprovalRequired())
          RETURNSTATE(SmVisualItems)

      if (isSecurityApprovalRequired() && !g_bClearAllDelayedSecIntervention)
          RETURNSTATE(SmSecurityLogs)

  }
  //TAR 364905 Begin. After approval restricted item on lane, it will be back to this state
  if (co.fOperationsForceDelayedApprovalBeforeSuspend && !IsForceDelayedApprovalBeforeSuspend()&& bStoreModeApprovals)
  {
		trace(L7, _T("+ra.ForceApprovalBeforeSuspend, ForceApprovalBeforeSuspend=false"));
		ra.ForceApprovalBeforeSuspend(false);
		trace(L7, _T("-ra.ForceApprovalBeforeSuspend"));
		bForceApprovalBeforeSuspend = false;
  }//TAR 364905 End

    // streamline store-mode screens such that delayed interventions are more quickly and easily handled
    // determine if we can can automatically return to shopping mode w/o button 8 being selected
    // age/coupon/vv need to have been approved and no other approvals pending
    if(!isAnyApprovalRequired()  && bStoreModeApprovals)
	{
        bFinish = false;        // TAR 221307
        return PSButton8();		// return to shopping
	}

	//return to Attract if is from LaneClosed state
	if (m_bIsFromLaneClosed || m_bIsAttendantModeChanged)	//tar371932 & 371918
	{
		//tar371932 & 371918+
        if(m_bIsAttendantModeChanged)
            m_bIsAttendantModeChanged=false;
		//tar371932 & 371918-

		return PSButton8();		// return to shopping
	}


  //Update RAP window with SCOT state
  //TAR 196366 Empty string is for store mode
  UpdateStateName(_T(""));
  if (dm.fStateTakeawayBelt)	// do not run belt in store mode  LPM091802 215283
  {
	csItemSent = EMPTY_STRING;       // forget last item code we TRIED to sell
  }

  InitLanguage();

  InitDevices();

  if ( !co.csOperationsHeadCashierContext.IsEmpty() && g_bValidHeadCashier )
  {
      csCurContext = co.csOperationsHeadCashierContext;      
  }
  else
  {
      csCurContext = _T("SmAuthorization");  // Tar 238987
  }

    InitUI();


  return STATE_NULL;							  // stay in SmAuthorizationBase state
}

void SMSmAuthorizationBase::InitUI(void)
{
  // New UI
    ps.SetFrame(csCurContext);
    //Use custom data to set title in xml context, so an alternate title can be set 
    //ps.SetTitle(TTL_STOREAUTH, PS_BLANK); 

	ps.Echo(PS_BLANK);

    InitLeadThruText();
    // TAR 236945 - Needed for Remote, this api will check option whether to display or not.
    ps.Weight(lDMScaleWeight);

	ps.ShowTrainingModeOnScreen(TrainingModeObj.IsTMOn());  //Tar 228689, While lane in training, RAP store mode did not show in training

    InitButtons();
    InitReceiptArea();
}

void SMSmAuthorizationBase::InitDevices(void)
{
    if (!ps.RemoteMode())
    {
        dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                            DM_TRICOLORLIGHT_OFF,
                            DM_TRICOLORLIGHT_NORMALREQUEST);
        dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                            DM_TRICOLORLIGHT_OFF,
                            DM_TRICOLORLIGHT_NORMALREQUEST);

		//Tar 418821
		if (dm.GetCoinsOrBillsVeryLow() || dm.GetPrinterPaperLow())
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


		//+dp185016 System Specs : Turn off lane light in store mode if outside transaction
		if (!(fInSignedOn || fInSignedOff || fSAInTransaction))
			dm.RemoteRelay(DM_MISC_STATE_OFF);
		//-dp185016
    }
}

void SMSmAuthorizationBase::InitLanguage(void)
{
    ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
    if(gpSCOTAuxDisp)
    {
        gpSCOTAuxDisp->SetLanguage(SCOT_LANGUAGE_PRIMARY);
    }
}

void SMSmAuthorizationBase::InitButtons(void)
{
    ShowStoreModeMenuButtons();  
}

void SMSmAuthorizationBase::InitLeadThruText(void)
{
  // New UI
    if(bSuspendFailure)
    {
        ps.SetLeadthruText(TXT_SUSPEND_INCOMPLETED, PS_BLANK);
    }
    else
    {
        ps.SetLeadthruText(TXT_SMM_EDITADD_INSTR, PS_BLANK);
    }
}

void SMSmAuthorizationBase::InitReceiptArea(void)
{
  ShowSMInfo();
  ps.ShowSMTotal(true);
  ps.ShowSMTBText(csTBMessage2Scot);	//thp
  ps.SMShowItemReceipt(true);

  // when RAP is remote connecting for store mode,
  // do NOT send approval notifications to RAP
  if (!ps.RemoteMode())
  {
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
	//  +TAR 352409
		if(bSuspendFailure)
		{
			trace(L7, _T("+ra.OnNoRAPDataNeeded"));
			bSuspendFailure = false;
		    ra.OnNoRAPDataNeeded();
			trace(L7, _T("-ra.OnNoRAPDataNeeded"));
		}
	//  -TAR 352409
  }
// Commented out to save values for outstanding Approval Printing moved to PSButton8
  /*
  lNotForSaleItems = 0;
  lTimeRestrictedItems = 0;
  lUnknownItems = 0;
  lUnknownItemsKeyed = 0;
  lPriceRequiredItems = 0;
  lRestrictedNotAllowedItems = 0;
  lQuantityRestrictedItems = 0;
  fRequestForHelp = false;
  fCantScanCoupon = false;
  bDropoffApproved = true;
  bSuspendFailure  = false;
  bSuspendApproved = true;
  lCouponsInAssist = 0;			//TAR#102019
  */

  // 206331 LPM lane light not blinking after going into media status even if printer is low
  if (dm.GetPrinterPaperLow())
  {
	dm.setTriColorLight(DM_TRICOLORLIGHT_GREEN,
		                DM_TRICOLORLIGHT_BLINK_1HZ,
			            DM_TRICOLORLIGHT_NORMALREQUEST);
  }

  // TAR 248418 Need to tell RAP to have full screen.
  ra.OnAddOtherEvent(_T("COMMAND=AssistModeSizeToFullScreen;"));
  if(!ps.RemoteMode() || co.IsInAssistMenus())
  {
	  ps.ShowFrame();
  }
}

///////////////////////////////////////////
void SMSmAuthorizationBase::UnInitialize(void)
{
  // only do on exit if Return to Shopping button selected and there is no security state saved in the stack
  // If there is a saved security state, by resetting LGW makes the state invalid CHU 041503

  if(bReturn2Shopping)
  {

	ra.RequestEnableAssistMode(true); //Tar218158

	// TAR295879
	// Synchronize the Store login with LaunchPad so that if the user turns the keyswitch to
	 // Normal (supervisor mode), the signon status will match scotapp.
	// Normal (supervisor mode), the signon status will match scotapp.
    trace(L6, _T("bReturn2Shopping %d"),bReturn2Shopping );
    SynchronizeStoreloginWithLaunchPad(_T("operation=set-signed-off")); // TAR 301706

	if ( sReturnState == STATE_NULL )
	{
		// Tar 213624 timing issue, putting something on smartscale and
        // hitting ReturnToShopping button events close together  RJF090402
	}
  }
  nStoreModeMenuState = BES_STATERETURN;  //This is the parent of all store menus, reset value.
}

///////////////////////////////////////////
// Button 1 Close Lane
///////////////////////////////////////////
SMStateBase  *SMSmAuthorizationBase::PSButton1(void)
{
    return OnCloseLaneButton();
}

///////////////////////////////////////////
// Button 2 Suspend
///////////////////////////////////////////
SMStateBase  *SMSmAuthorizationBase::PSButton2(void) //
{
    return OnSuspendButton();
}


///////////////////////////////////////////
// Button 3 - Reports Menu
///////////////////////////////////////////
SMStateBase  *SMSmAuthorizationBase::PSButton3(void)
{
    return OnReportingButton();
}

///////////////////////////////////////////
// Button 4 - System Functions
///////////////////////////////////////////
SMStateBase  *SMSmAuthorizationBase::PSButton4(void)
{
    return OnSystemFunctionsButton();
}

///////////////////////////////////////////
// Button 5 - Media Status
///////////////////////////////////////////
SMStateBase  *SMSmAuthorizationBase::PSButton5(void)
{
    return OnMediaStatusButton();
}

///////////////////////////////////////////
// Button 6 - Assist mode
///////////////////////////////////////////
SMStateBase  *SMSmAuthorizationBase::PSButton6(void)
{
    return OnAssistModeButton();
}

///////////////////////////////////////////
// Button 7 - Blank
///////////////////////////////////////////

///////////////////////////////////////////
// Button 8 - Return to Shopping
///////////////////////////////////////////
SMStateBase  *SMSmAuthorizationBase::PSButton8(void)
{
    return OnGoBackButton();
}

/////////////////////////////////////////////
// OnGoBackButton
//
// Purpose: Handle Return to Shopping
// Parameters:
//   None
/////////////////////////////////////////////
SMStateBase  *SMSmAuthorizationBase::OnGoBackButton(void)
{

	// TAR 397386 - Check if we are in Multi-item picklist before sending state to Security Manager.
	if(!fInMultiSelectPickList)
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAuthorization") );
	}

  if(!(!isAnyApprovalRequired()  && bStoreModeApprovals))
  {
    bFinish = false; // TAR393586
	co.SetInAssistMenus(false);
	// TAR 397386 - Check if we are in Multi-item picklist before sending state to Security Manager.
	if(!fInMultiSelectPickList)
	{
		if (dm.fStateTakeawayBelt && ps.RemoteMode())
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmRemoteAssistMode") );
		else
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAssistMode") );
	}
  	SendTBCommand(TBCMD_ASSISTMENUSET, false);
  }

  //TAR390419+
  //security intervention cleaning: 
  if (SecMgr.GetOutstandingInterventionCount() >= 1) 
  {
	OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR);
  }
  //TAR390419-

	//414687 If there is WaitForApproval state waiting, clear it
	if (g_pDelay_WaitForApprovalStateChange != NULL)
	{
		CString csStateName = g_pDelay_WaitForApprovalStateChange->GetRuntimeClass()->m_lpszClassName;
		delete g_pDelay_WaitForApprovalStateChange;
		g_pDelay_WaitForApprovalStateChange = NULL;
		trace(L6,_T("SMSmAuthorizationBase::OnGoBackButton - Delete the state %s saved in g_pDelay_WaitForApprovalStateChange"),
			     csStateName);
	}

  if (!ps.RemoteMode() || co.IsInAssistMenus())
  {
    //+dp185016 System Specs : Turn off lane light in store mode if outside transaction
    //          turn it back on here
    dm.RemoteRelay( DM_MISC_STATE_ON );
    //-dp185016

	fOtherPayment = false ; //Tar #158619	reset here so we can return to shopping if needed
	lNotForSaleItems = 0;
	lTimeRestrictedItems = 0;
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
	bReturn2Shopping = true;			// Tar 213624 RJF 090602
	bStoreModeApprovals = false;	// 3.1 req 4.4.5 streamline storemode screens
	bStoreModeGoBack = false;		// track use of Go Back(now Store Authorization) for action required approval
	g_bClearAllDelayedSecIntervention = false;

	if (lUnapprovedHopperSubstitution > 0)
	{
		nHopperTrxCount=0;
		lUnapprovedHopperSubstitution = 0;
		lRegularYellowNeedingApproval--;
		trace(L7, _T("+ra.OnHopperSubstitutionApproval"));
		ra.OnHopperSubstitutionApproval();
		trace(L7, _T("-ra.OnHopperSubstitutionApproval"));
	}


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
	// End TAR 197558

	bDropoffApproved = true;
	bSuspendFailure  = false;
	bSuspendApproved = true;

	//Reseting LGW only if SmartScale fires a weight event (in SMStateSABase.cpp)
	//That way app can remember if an error occurred before entering Store mode and process that error
	//ss.SetLastGoodWeight(0x8000000);		// TAR196221 what ever bag scale event happens in store mode should reset the smart scale LPM022002
  }	// end of if (!ps.RemoteMode())

  ps.SetLanguage(m_customerLanguage);
  if(gpSCOTAuxDisp)
	gpSCOTAuxDisp->SetLanguage(m_customerLanguage);

  BEST best;
  if(fCancelSuspend)
  {
	  fCancelSuspend = false;
	  setAnchorState(nSellState);
  }
  else
	  best = getAnchorState();

  //TAR# 195639 Money accepted while system was entering assist mode should also be checked for.
  //If cash payment is greater than amount due, system should go to Finish state upon return.
  if (nDMCashInserted && ((lBalanceDue - nDMCashInserted) <= 0))
  {
   	  setAnchorState(BES_FINISH);
  }

  if (!ps.RemoteMode()|| co.IsInAssistMenus())
  {
	if (isAnyApprovalRequired())

	{
		dm.restoreTriColorLight();
		if (best == BES_FINISH) // gotta approve
		{
			if (lSAYellowNeedingApproval)
			{
				dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                          DM_TRICOLORLIGHT_ON,
                          DM_TRICOLORLIGHT_NORMALREQUEST);
			}
			trace(L7, _T("+ra.OnCancelApplicationError"));
			ra.OnCancelApplicationError();	// go back to wait for approval
			m_bIsFromLaneClosed = false; //TAR 279371
			trace(L7, _T("-ra.OnCancelApplicationError"));
			CREATE_AND_DISPENSE(WaitForApproval)(false,false);
		}
		else if (best == BES_SCANANDBAG) // went in here from pressing help, can wait to approve later
		{

			//if (lSAYellowNeedingApproval)
			// the flag lSAYellowNeedingApproval is not set propoerly
			// So using the explicit tests if we need a yellow light before we go to shopping.
			// When we enter the Store Authorization screen, the initialize make the light green
			// as most approvals are just approved by returning to shopping.
			// Only these 3 (excluding void as that is always handled as an immediate Intervention
			// so light turns back to Red) if still not approved need a yellow light.
			if (isRestrictedItemApprovalRequired() || isVisualItemApprovalRequired()
			   || isCouponRatioApprovalRequired() || isSecurityApprovalRequired())
			{
			     // Tar 209546 if turning yellow, make sure red is off
                 dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                                DM_TRICOLORLIGHT_OFF,
                                DM_TRICOLORLIGHT_NORMALREQUEST);
				 dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                                DM_TRICOLORLIGHT_ON,
                                DM_TRICOLORLIGHT_NORMALREQUEST);
			}
			m_bIsFromLaneClosed = false; //TAR 279371
			return createAnchorState();
		}
		else if (best == BES_START)		//TAR#122340		Kotes
		{
			if (lSAYellowNeedingApproval)
			{
				trace(L7, _T("+ra.OnCancelApplicationError"));
				ra.OnCancelApplicationError();		// go back to wait for approval
				m_bIsFromLaneClosed = false; //TAR 279371
				trace(L7, _T("-ra.OnCancelApplicationError"));
				CREATE_AND_DISPENSE(WaitForApproval)(false,false);
			}
		}

	}
	else if (dm.GetPrinterPaperLow())
	{
		dm.setTriColorLight(DM_TRICOLORLIGHT_GREEN,
			                DM_TRICOLORLIGHT_BLINK_1HZ,
				            DM_TRICOLORLIGHT_NORMALREQUEST);
	}
	else //Tar 180598: turn lane light green if nothing else to clear. Ch082001
	{
		dm.turnOffTriColorLight();
		dm.setTriColorLight(DM_TRICOLORLIGHT_GREEN,
                         DM_TRICOLORLIGHT_ON,
                         DM_TRICOLORLIGHT_NORMALREQUEST);
	}

	//Tar #158619 only suspend gets us out of loop if we wait to reset here ->  fOtherPayment = false;
	// don't show Deposit Coupons screen if coupon sensor is configured
	if (lCoupons && !IsUsingCouponSensor())
	{
		if (!bDropoffRequired && (best != BES_SCANANDBAG))
			m_bIsFromLaneClosed = false; //TAR 279371
			return DepositCouponsIfNecessary();
	}
  }
  else
  {
    bSuspendFailure = false;  //if in remote mode, need to reset suspend failure value
	trace(L6, _T("RAP exiting remote store mode"));
  }

  m_bIsFromLaneClosed = false;
  sReturnState = CkStateStackAndReturn();  //TAR213624 CHU 041503
  if (sReturnState != STATE_NULL)
  {
	  return sReturnState;
  }

    //TAR 371932 & tar 371918+
  if (m_bIsFromLaneClosed == true)
  {
      trace(L5, _T("wait here until app receives WMRA_SEND_INTERVENTION_APPROVED"));
	  return STATE_NULL; //wait here until app receives WMRA_SEND_INTERVENTION_APPROVED
     //m_bIsFromLaneClosed = false; //set this later
  }
  //TAR 371932 & tar 371918-

  if (best == BES_START)	// if came from attract then return there instead of to the thank you screen
	  return setAndCreateAnchorState(BES_START);

  if (getAnchorState()==BES_INSERTCOUPON)
  {
	  if (bEnterTenderState)
		  return setAndCreateAnchorState(BES_ENTERCOUPONS);
	  else
		  return setAndCreateAnchorState(BES_SCANANDBAG);
  }
  //+degraded mode tars
  if( co.IsDegradedModeOn() 
      && dm.GetDegradedModeType() != DEGRADEDMODE_OFF 
      && getAnchorState() == BES_DEGRADEDCONTINUETRANS )
  {
    RETURNSTATE(DegradedContinueTrans)
  }
  else if(getAnchorState() == BES_DEGRADEDCONTINUETRANS)
  {
    setAnchorState(BES_SCANANDBAG);   //tar 417499
  }
  //-degraded mode tars
  
  // otherwise, back to caller...
  return createAnchorState();
}

////////////////////////////////////////
// NewUI
SMStateBase  *SMSmAuthorizationBase::PSReceiptUp(void)
{
  //The scrolling of the receipt is done by action (defined in xml)
   return STATE_NULL;
}

SMStateBase  *SMSmAuthorizationBase::PSReceiptDown(void)
{
  //The scrolling of the receipt is done by action (defined in xml)
   return STATE_NULL;
}
// End NewUI

/////////////////////////////////////
// ShowCloseLaneButton 
// 
// Purpose: Display the command button - CmdBtn_CloseLane 
//          or the fixed button - SMButon1
// Parameters:
//   None
/////////////////////////////////////
void SMSmAuthorizationBase::ShowCloseLaneButton()
{
    STATE(StoreModeMenu)::ShowCloseLaneButton();
    //Override the display of the command button - CmdBtn_CloseLane 
    //to still be able to use the fixed button - SMButon1
    if (co.IsInAssistMenus())
    {
        ps.ButtonState(_T("SMButton1"), false, false);
    }
    else
    {
        ps.Button(_T("SMButton1"), BTT_CLOSELANE, (ps.SalesReceiptEmpty() && !bIsTransactionStarted));
    }
}

/////////////////////////////////////
// ShowSuspendButton 
// 
// Purpose: Display the command button - CmdBtn_Suspend 
//          or the fixed button - SMButon2
// Parameters:
//   None
/////////////////////////////////////
void SMSmAuthorizationBase::ShowSuspendButton()
{
    STATE(StoreModeMenu)::ShowSuspendButton();
    //Override the display of the command button - CmdBtn_Suspend 
    //to still be able to use the fixed button - SMButon2
    if (co.IsInAssistMenus())
    {
        ps.ButtonState(_T("SMButton2"), false, false);
        return;
    }
    ps.SetButtonText(_T("SMButton2"), co.fOperationsSuspend? BTT_SUSPEND : PS_NOTEXT); // if suspend is not configured, do not display button
	// +TAR 299534 - added tracing to see why form attract suspend button was enabled
	bool bStatus = isAnySoldVoidedItems();
    trace(L6, _T("SMSmAuthorizationBase::ShowSuspendButton isAnySoldVoidedItems =%d fInSignedOn= %d lTotalPaid= %d nDMCashInDrawer = %d"), bStatus, fInSignedOn, lTotalPaid, nDMCashInDrawer);
	// - TAR 299534
    //+RFC267820
    if (    (!isAnySoldVoidedItems() && co.fOperationsSuspend) //Nothing Sold
         || !fInSignedOn //TB not in tranx
         || ( (short(lTotalPaid) != nDMCashInDrawer) && !co.fOperationsAllowSuspendIfPartiallyTendered) )  //Tranx partially tendered, but config option not set to allow suspend if tranx is partially tendered
    {
        ps.Button(_T("SMButton2"),BTT_SUSPEND,false); // disable suspend button
    }//-RFC267820
    // RFC 263291.  Ask the TB if the Suspend button is allowed, only if the app already has it enabled
    else if (!TBIsAllowed(TB_FT_SUSPEND))
    {
        ps.Button(_T("SMButton2"),BTT_SUSPEND,false);			// disable suspend button
    }

}

/////////////////////////////////////
// ShowReportingButton 
// 
// Purpose: Display the command button - CmdBtn_Reporting 
//          or the fixed button - SMButon3
// Parameters:
//   None
/////////////////////////////////////
void SMSmAuthorizationBase::ShowReportingButton()
{
    STATE(StoreModeMenu)::ShowReportingButton();
    //Override the display of the command button - CmdBtn_Reporting 
    //to still be able to use the fixed button - SMButon3
    if (co.IsInAssistMenus())
    {
        ps.ButtonState(_T("SMButton3"), false, false);
        return;
    }

    ps.SetButtonText(_T("SMButton3"), BTT_REPORTING); //Reports Menu
	//403469: either AssistMode or AssistMenuFromStoreModeAllowed is configured
    if ( ( co.fStateAssistMode || co.fStateAssistMenuFromStoreModeAllowed ) && fInSignedOn && co.fOperationsTBControlsPrinter)
    //if (co.fStateAssistMode && fInSignedOn && co.fOperationsTBControlsPrinter)
        ps.Button(_T("SMButton3"), BTT_REPORTING, false);
}

/////////////////////////////////////
// ShowSystemFunctionsButton 
// 
// Purpose: Display the command button - CmdBtn_SystemFunctions 
//          or the fixed button - SMButon4
// Parameters:
//   None
/////////////////////////////////////
void SMSmAuthorizationBase::ShowSystemFunctionsButton()
{
    STATE(StoreModeMenu)::ShowSystemFunctionsButton();
    //Override the display of the command button - CmdBtn_SystemFunctions 
    //to still be able to use the fixed button - SMButon4
    if (co.IsInAssistMenus())
    {
        ps.ButtonState(_T("SMButton4"), false, false);
        return;
    }

    ps.Button(_T("SMButton4"), BTT_SYSTEMFUNCTIONS, !ps.RemoteMode()); //System Functions Screen
}

/////////////////////////////////////
// ShowMediaStatusButton 
// 
// Purpose: Display the command button - CmdBtn_MediaStatus 
//          or the fixed button - SMButon5
// Parameters:
//   None
/////////////////////////////////////
void SMSmAuthorizationBase::ShowMediaStatusButton()
{
    STATE(StoreModeMenu)::ShowMediaStatusButton();
    //Override the display of the command button - CmdBtn_MediaStatus 
    //to still be able to use the fixed button - SMButon5
    if (co.IsInAssistMenus())
    {
        ps.ButtonState(_T("SMButton5"), false, false);
        return;
    }

    ps.Button(_T("SMButton5"), BTT_LANESTATUS, !ps.RemoteMode()); //Media Status Screen
}

/////////////////////////////////////
// ShowAssistModeButton 
// 
// Purpose: Display the command button - CmdBtn_AssistMode 
//          or the fixed button - SMButon6
// Parameters:
//   None
/////////////////////////////////////
void SMSmAuthorizationBase::ShowAssistModeButton()
{
    STATE(StoreModeMenu)::ShowAssistModeButton();
    //Override the display of the command button - CmdBtn_AssistMode 
    //to still be able to use the fixed button - SMButon6
    if (co.IsInAssistMenus())
    {
        ps.ButtonState(_T("SMButton6"), false, false);
        return;
    }
    ps.Button(_T("SMButton6"), BTT_ASSISTMODE, ( co.fStateAssistMode || co.fStateAssistMenuFromStoreModeAllowed ) && fInSignedOn && !ps.RemoteMode());

}

/////////////////////////////////////
// ShowGoBackButton 
// 
// Purpose: Display the command button - CmdBtn_GoBack 
//          or the fixed button - SMButon8
// Parameters:
//   None
/////////////////////////////////////
void SMSmAuthorizationBase::ShowGoBackButton()
{
    STATE(StoreModeMenu)::ShowGoBackButton();
    //Override the display of the command button - CmdBtn_GoBack 
    //to still be able to use the fixed button - SMButon8
    if (co.IsInAssistMenus())
    {
        ps.Button(_T("SMButton8"), BTT_GOBACK, true);
        return;
    }
   
    ps.Button(_T("CmdBtn_GoBack"), BTT_GOTOSHOP, true);
    ps.Button(_T("SMButton8"),BTT_GOTOSHOP,true);
}

/////////////////////////////////////
// PSSwitchContext 
// 
// Purpose: Handle Switch Context event from PSX
// Parameters:
//   bRemote - whether the switch occurred on the remote display
//   csNewContext - the name of the new context
/////////////////////////////////////
SMStateBase *SMSmAuthorizationBase::PSSwitchContext(BOOL bRemote, CString csNewContext)
{
    trace(L6, _T("+SMSmAuthorizationBase::PSSwitchContext(%d, %s)"),bRemote, csNewContext);
    // Set Frame if context change is in the active context
    if( bRemote == ps.RemoteMode())
    {
        if (csNewContext.Find(_T("RAPSingle")) != 0) //Don't switch for RAP toggle
        {
            //Flexible store mode menus may have generated a switch context
            // from PSX.  Refresh text and button handling 
            if ((csCurContext != csNewContext) &&
                ( csNewContext != _T("Attract")) &&  	//TAR 413060 && TAR 415721
                ( csNewContext != _T("BiometricProcessing")))
            {
                csCurContext = csNewContext;
                ps.SetFrame(csCurContext);          
                InitLeadThruText();            
                InitButtons();
                ShowSMInfo();
                
                //TAR 415721 DO NOT tell PSX to switch contexts again - causes loop
                //if(!ps.RemoteMode() || co.IsInAssistMenus())
                //{
                //    ps.ShowFrame();  
                //}
            }
        }
    }
    
    trace(L6, _T("-SMSmAuthorizationBase::PSSwitchContext()"),bRemote, csNewContext);

    return STATE_NULL;
}

void SMSmAuthorizationBase::ShowSMInfo()
{
      // display items that were set aside
      CString csWork = _T("");
      if (lNotForSaleItems)
      {
        csWork += _T("\n* ");
        csWork += ps.GetPSText(TXT_RECALLEDITEMS);
      }
      if(lTimeRestrictedItems)
        csWork += _T("\n* ") + ps.GetPSText(TXT_TIME_RESTRICTED_ITEM);

      if (lUnknownItems)
      {
        csWork += _T("\n* ");
        csWork += ps.GetPSText(TXT_UNKNOWNITEMS);
      }
      if (fOtherPayment)
      {
        csWork += _T("\n* ");
        csWork += ps.GetPSText(TXT_OTHERPAYMENT);
      }
      if (lUnapprovedVisualVerifyItems)
      {
        csWork += _T("\n* ");
        csWork += ps.GetPSText(TXT_VISUALITEMS);
        CStringArray csArray;
        ps.ScanReceiptForExceptionCategorys(csArray);
		    int iSize = csArray.GetSize();
		    if(iSize)
        {
			    int index = 0;
          while(index < iSize)
          {
			      csWork +=_T("\n  ");  // indent list of Exception Category Descriptions under Visual Verify
			      csWork += csArray.GetAt(index);
				    index++;
			    }
		    }
      }
      if (lPriceRequiredItems)
      {
        csWork += _T("\n* ");
        csWork += ps.GetPSText(TXT_PRICENEEDEDITEMS);
      }
      if (lRestrictedNotAllowedItems)
      {
        csWork += _T("\n* ");
        csWork += ps.GetPSText(TXT_RESTRICTEDNOTALLOWEDITEMS);
      }
      if (lQuantityRestrictedItems)
      {
        csWork += _T("\n* ");
        csWork += ps.GetPSText(TXT_QUANTITYRESTRICTEDITEMS);
      }
      if (fCantScanCoupon)
      {
        csWork += _T("\n* ");
        csWork += ps.GetPSText(TXT_CANTSCANCOUPON);
      }
      if (fRequestForHelp)
      {
        csWork += _T("\n* ");
        csWork += ps.GetPSText(TXT_REQUESTFORHELP);
      }
      if (bDropoffRequired && !bDropoffApproved)
      {
        csWork += _T("\n* ");
        csWork += ps.GetPSText(TXT_DROPOFFCOUPONS);
      }
      if (bSuspendRequested && !bSuspendApproved)
      {
        csWork += _T("\n* ");
        csWork += ps.GetPSText(RA_TRANS_SUSPEND_ID);
      }
      if (lCouponsInAssist)		//TAR#102019
      {
	    csWork += _T("\n* ");
	    csWork += ps.GetPSText(TXT_COUPONSONLYINASSIST);
      }

      if (fUnDeActivatedItem)
      {
	    csWork += _T("\n* ");
	    csWork += ps.GetPSText(RA_THIRDLINE_UNDEACTIVATED);
      }

      // don't show gray pop-up box when RAP goes into store mode
      if (csWork!=_T("") && (!ps.RemoteMode() || co.IsInAssistMenus()))
      {
 	    CString csInfo;
	    csInfo = ps.GetPSText(MSG_SETASIDEAPPROVALS1);
	    csInfo += _T("\n");
	    csInfo += csWork;
	    csInfo += _T("\n\n");
	    csInfo += ps.GetPSText(MSG_SETASIDEAPPROVALS2);
	    ps.SMInfo(csInfo);
      }
      else
      {
	      m_pAppPSX->SetConfigProperty( _T("SMInfoBoxField"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );
	      m_pAppPSX->SetConfigProperty( _T("SMInfoBox"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );
      }
}