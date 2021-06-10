//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMWaitForApprovalBase.CPP
//
// TITLE: Class implementation for Wait for Approval state
//
// TAR#102019: LPM120999 - AllowCoupons has 3 options now.  I reformated this entire file! It was very hard to read
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"                 // MS MFC always needed first
//               // MS MFC template header

#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSmAbort.h"              // Store mode abort state
#include "SMSmAuthorization.h"      // Store mode auths state
#include "DMProcedures.h"
#include "SMSmConfirmSignature.h"
#include "SMSmAbort.h"				// Tar 194454 RF 020702
#include "SMSmAuthorization.h"		// Tar 194454 RF 020702
#include "SMUnDeActivatedItemApproval.h"  // TAR 202135
#include "SMWaitForApproval.h"      //tar 256141
#include "DataNeededUtil.h"
#include "QueuedActionGenericDelayedIntervention.h"
//USSF START
#include "USSFCommon.h"
//USSF END
DLLEXPORT extern DMProcedures dm;   // Global DM object
#else
#include "TestMacros.h"
#include "FakeDMProcedures.h"
#include "FakePSProcedures.h"
#include "FakeQueuedActionGenericDelayedIntervention.h"
#define PSProceduresBase CFakePSProcedures
#define CQueuedActionGenericDelayedIntervention CFakeQueuedActionGenericDelayedIntervention
#endif
#include "SMWaitForApproval.h"  // This state





#define COMP_ID ID_SM               // Base state component
#define T_ID _T("WaitForApprovalBase")

IMPLEMENT_DYNCREATE(SMWaitForApprovalBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(WaitForApproval)

DEFINE_TIMESTAMP

#define MAX_APPROVALS_TO_SHOW_ONSCREEN 5

//extern SMStateBase* pgState2Push;	//LPM082301 TAR176732 Fix for timing issue
#ifndef _CPPUNIT
////////////////////////////////////////////////////
SMWaitForApprovalBase::SMWaitForApprovalBase(const bool fTV,
                                             const bool fVT,
						  					 const bool bVerifySig)
    :fVoidTrx(fVT)
{
  IMPLEMENT_TIMESTAMP
  fTrxVerif = fTV;
  bMoveSAMsgDown = true;
  m_bVerifySig = bVerifySig ;

  //TAR195100+ CHU022502 for creating new WaitForApproval state
  g_bgTrxVerif	= fTV;
  g_bgVoidTrx	= fVT;
  g_bgVerifySig	= bVerifySig;
}

#endif //_CPPUNIT
////////////////////////////////////////
SMWaitForApprovalBase::SMWaitForApprovalBase()
{
  IMPLEMENT_TIMESTAMP
}

#ifndef _CPPUNIT
////////////////////////////////////////
SMStateBase  *SMWaitForApprovalBase::Deliver(const bool fTV,
                                             const bool fVT,
                                             const bool bVerifySig)
{
  fTrxVerif = fTV;
  fVoidTrx = fVT;
  bMoveSAMsgDown = true;
  m_bVerifySig = bVerifySig ;

  //TAR195100+ CHU022502 for creating new WaitForApproval state
  g_bgTrxVerif	= fTV;
  g_bgVoidTrx	= fVT;
  g_bgVerifySig	= bVerifySig;

  return this;
}


/////////////////////////////////////////
bool SMWaitForApprovalBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{

    // There should be two or three parameters, all bools passed as word
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMWaitForApprovalBase::DeliverDescriptorList() - Parameter Count of 2"));
        Deliver((bool)DLParamWORD(dlParameterList),    // fTV
                (bool)DLParamWORD(dlParameterList));   // fVT
        return true;
    }
    else if (dlParameterList.GetDescriptorCount() == 3)
    {
        trace(L6, _T("+SMWaitForApprovalBase::DeliverDescriptorList() - Parameter Count of 3"));
        Deliver((bool)DLParamWORD(dlParameterList),    // fTV
                (bool)DLParamWORD(dlParameterList),    // fVT
                (bool)DLParamWORD(dlParameterList));   // bVerifySig
        return true;
    }
    else
    {
        trace(L2, _T("+SMWaitForApprovalBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}
/////////////////////////////////////////
SMStateBase  *SMWaitForApprovalBase::TimedOut(void)
{
  if (!(bSuspendRequested && !bSuspendApproved))  //sscoadk-5501
  {  
      if (nSaidCount<10)
      {
        DMSayPhrase(APPROVALNEEDED);
        nSaidCount++;
      }
  }

  return STATE_NULL;
}

///////////////////////////////////////////
SMStateBase  *SMWaitForApprovalBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("WaitForApproval") );	// RFC 330050
  if(!(isAnyApprovalRequired() || (lSAYellowNeedingApproval))
	  && !g_bgVoidTrx && !g_bgTrxVerif && !g_bgVerifySig && !g_bgCheckCashChanger) //tar 243851
  {
  	return createAnchorState();
  }
  if (!(bSuspendRequested && !bSuspendApproved))  //sscoadk-5501
  {
      //not showing the context of waitforApproval so there is no need to sayphrase
      DMSayPhrase(APPROVALNEEDED);
      nSaidCount = 1;
  }

  if (m_bVerifySig)
  {
    // Don't allow assist mode if we're here verifying a signature.
    ra.RequestEnableAssistMode(false);
  }
  if (bIsInFinalization && g_bAllowRemove) //tar 223051
  {
		bInApprovalNeeded = true; //TAR 279092
		g_bAllowRemove = false;
  }

  bFinish = true;   // to tell rap to go to suspend right away
  trace(L7, _T("+ra.OnSetTransactionState, Wait For Approval State"));
  ra.OnSetTransactionState(RA_STATE_WAIT_FOR_APPROVAL);
  trace(L7, _T("-ra.OnSetTransactionState"));

  nTimeOut = co.getTimeOutHelp();   // ring alarm at timeout

  dm.saveTriColorLightStatus();

  // Turn on red security light.
  
  // (*) SR898 ====================
  DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                     DM_TRICOLORLIGHT_BLINK_1HZ,
                     DM_TRICOLORLIGHT_NORMALREQUEST);
                     
  //tar 180380 starts
  m_bNeedToDisplayAuxScreen = false;
  DisplayAuxScreen(AUX_SCREEN_DEFAULT, true);
  //tar 180380 ends

  return STATE(OperatorPasswordState)::Initialize();
}
#endif //_CPPUNIT

/////////////////////////////////////////////////////////
void SMWaitForApprovalBase::showScreen(bool passwordVerified)
{
    if(!m_bVerifySig)
    {
         DMMSREnable();                 // enable the card reader, set track, turn on MEI
    }

  saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
  int nNumApprovals = 0;

  // This state does not need to reshow if the password
  // was verified, since we'll be leaving now
  if (passwordVerified)
  {
	// TAR#128476 if sig verification is required don't eject the card
	//            until after login LPM122199
	if (m_bVerifySig && waitingForCardRemoval)  //TAR260141
	{
        // must Enable() device before calling Eject() so that the Enable() while in Eject()
        // is ignored. Otherwise, the Enable() is processed too late and captures the card.  LPM100202 215897
        DMMSREnable();
		DMMSREject();
	}
    return;
  }

  CString csWork;

#ifndef _CPPUNIT

     if (inputMode == PROMPTING_FOR_OPERATOR)
     {
        saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
		STATE(OperatorPasswordState)::ShowEnterID();  //SR93 Biometrics support
		ps.SetCMLeadthruText( TXT_ENTER_ID );
		ps.AllowInput( co.GetnOperationsOperatorIDMaxDigit(), false, false );
		ps.ShowCMFrame( false );
		return;
     }
     else
     {  // Display Wait for Approval....
        // NewUI
	    ps.SetCMFrame(_T("WaitForApproval"));
        ps.SetPSText(PS_WORK1,ps.GetPSText(BTT_STORELOGIN,SCOT_LANGUAGE_PRIMARY));
        ps.SetButtonText(_T("CMButton1StoreLogIn"),PS_WORK1);
	}


  // End NewUI

  bool bHasEllipsis = false;
  //sscoadk-321+
  if (co.GetfOperationsAllowSaleofRestrictedItems() || ( co.IsAttendantModeOn() && co.GetfOperationsAllowSaleofRestrictedItemsInAssist() ))
  {
	  csWork = ps.GetPSText(MSG_WAITFORAPP);
  }
  else if (io.d.fRestricted) //sscop-1669
  {
    csWork = ps.GetPSText(LTT_HELPONWAY);
	csWork += _T("\n\n");
  }
  else
  {
	csWork = ps.GetPSText(MSG_WAITFORAPP);
  }
  //sscoadk-321-
  int iMaxApprovals = MAX_APPROVALS_TO_SHOW_ONSCREEN;
  if (co.csPrimaryLanguageCode == _T("0408"))
  {   //Greek handling of messages
      iMaxApprovals--; //deduct since the first message has 2 lines
  }

  if (fVoidTrx)
  {
    if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)  //nNumApprovals should be 1 here, but
    {                                                       //just in case people add conditions above this
		bHasEllipsis = true;
        csWork += _T("\n...");                                  //in the code, this will correctly
    }                                                       //shorten onscreen text if necessary
    else if (nNumApprovals < iMaxApprovals)
    {
        csWork += _T("\n* ");
        csWork += ps.GetPSText(TXT_CANCELALL);
    }

	ra.RequestEnableAssistMode(false);
	// send a DataNeeded Approval to RAP for Cancel Transaction intervention.
    cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("CancelTransactionGeneric"));
    m_View = DATANEEDED_CANCELLTRANSACTION;
    CString csSecInfo;
    csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),_T(" "), ps.GetPSText(RA_CANCELTRANSACTION, SCOT_LANGUAGE_PRIMARY), _T(""), 
		co.IsSecurityDisableOn()==true || co.GetSecurityDisableSubCheckOn() ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), _T(""), _T(""));
	trace(L6, _T("ra.OnRAPDataNeeded()"));
    ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
	//TAR371918 testing only
	//trace(L6, _T("ra.OnNoRAPDataNeeded()"));
    //ra.OnNoRAPDataNeeded();
 	bApplicationErrorApprovalRequired = TRUE;
  }
  else
  {
    if ((bApplicationErrorApprovalRequired) && !(m_bVerifySig))
	{
      bApplicationErrorApprovalRequired = FALSE;
	  trace(L7, _T("+ra.OnApplicationError"));
	  ra.OnApproveApplicationError();
	  trace(L7, _T("-ra.OnApplicationError"));
	}
  }

 /* //Tar 115692{
  if (bDropoffRequired && !bDropoffApproved)
  {
        CString csWork;

        csWork = ps.GetPSText(RA_TRANS_DROPOFF_ID, SCOT_LANGUAGE_PRIMARY);
        ra.OnReadyToCollect(csWork);
  }
  //}Tar 115692*/

  if (m_bVerifySig)
  {
    if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
    {
		bHasEllipsis = true;
        csWork += _T("\n...");
    }
    else if (nNumApprovals < iMaxApprovals)
    {
        csWork += _T("\n* ");
        csWork += ps.GetPSText(TXT_SIGNATURE_APPROVAL_NEEDED);
    }
    if (bSuspendRequested)
	{
	  if (!bSuspendApproved)
	  {
        if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
        {
			bHasEllipsis = true;
            csWork += _T("\n...");
        }
        else if (nNumApprovals < iMaxApprovals)
        {
            csWork += _T("\n* ");
            csWork += ps.GetPSText(RA_TRANS_SUSPEND_ID);
        }

		// cause RAP to go to Allow Suspend
		trace(L7, _T("+ra.OnReadyToSuspend"));
		ra.OnReadyToSuspend();
		trace(L7, _T("-ra.OnReadyToSuspend"));
	  }
	}
  }
  else if (fOtherPayment)   // if the Other Payment is selected, go to wait for approval and show the message
  {
    // TAR 228518 - Moved the following block from SMFinishBase - to let RAP know that
    // after a security violation (on WaitForApproval of other payment) - There is still a
    // need for other payment approval
    CString csAnchorStateName = getSCOTStateName();
    CString csDesc, csInstruction;
    csDesc = ps.GetPSText(RA_OTHER_PAYMENT_ID, SCOT_LANGUAGE_PRIMARY);
    csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_LANEBTT_OTHERPAYMENT, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, _T(""), _T(""));
    trace(L7, _T("+ra.OnOtherPaymentEvent, Message=%s"), csInstruction);
    ra.OnOtherPaymentEvent(csInstruction, false) ;
    trace(L7, _T("-ra.OnOtherPaymentEvent"));
    // TAR 228518
    if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
    {
		bHasEllipsis = true;
        csWork += _T("\n...");
    }
    else if (nNumApprovals < iMaxApprovals)
    {
        csWork += _T("\n* ");
        csWork += ps.GetPSText(TXT_OTHERPAYMENT);
    }
 	// when SCOT entering this screen, RAP automatically enters ReadyToSuspend status
	/*		 if (bSuspendRequested)
	{
	  if (!bSuspendApproved)
	  {
		csWork += "\n* ";
		csWork += ps.GetPSText(RA_TRANS_SUSPEND_ID);

		// cause RAP to go to Allow Suspend
		ra.OnReadyToSuspend();
	  }
	}*/
  }
  else
  {
    if (fTrxVerif)
    {
      if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
      {
		  bHasEllipsis = true;
          csWork += _T("\n...");
      }
      else if (nNumApprovals < iMaxApprovals)
      {
        csWork += _T("\n* ");
        csWork += ps.GetPSText(TXT_TRXVERIFICATION);
      }
    }

	if(lRestrictedNotAllowedItems)
	{
	  CString csAnchorStateName = getSCOTStateName();
	  CString csDesc, csInstruction;
	  csDesc = ps.GetPSText(RA_THIRDLINE_RESTRICTEDNOTALLOWED, SCOT_LANGUAGE_PRIMARY);
	  csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_LANEBTT_TIMERESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, _T(""), ps.GetPSText(RA_THIRDLINE_RESTRICTEDNOTALLOWED, SCOT_LANGUAGE_PRIMARY));
	  trace(L7, _T("+ra.OnRestrictedNotAllowed, Message=%s"), csInstruction);
	  ra.OnRestrictedNotAllowed(csInstruction);
	  trace(L7, _T("-ra.OnRestrictedNotAllowed"));
	}


    if (lCouponsInAssist)		//TAR#102019
    {
	  //TAR209311 Update RAP window with SCOT state for coupons only allowed in assist LPM070902
	  CString csAnchorStateName = getSCOTStateName();
	  CString csDesc, csInstruction;
	  csDesc = ps.GetPSText(TXT_COUPONSONLYINASSIST, SCOT_LANGUAGE_PRIMARY);
	  csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(BTT_HELP, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, _T(""), ps.GetPSText(BTT_CPNCANT, SCOT_LANGUAGE_PRIMARY));
	  trace(L7, _T("+ra.OnCallForHelp, Message=%s"), csInstruction);
	  ra.OnCallForHelp(csInstruction);
	  trace(L7, _T("-ra.OnCallForHelp"));
      if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
      {
		  bHasEllipsis = true;
          csWork += _T("\n...");
      }
      else if (nNumApprovals < iMaxApprovals)
      {
          csWork += _T("\n* ");
          csWork += ps.GetPSText(TXT_COUPONS);
      }
    }

    if (isRestrictedItemApprovalRequired())
    {
	  if (co.csPrimaryLanguageCode == _T("0419"))
	  {   //Russian message for restricted items consumes 2 lines
		  ++nNumApprovals; 
	  }
      if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
      {
		  bHasEllipsis = true;
          csWork += _T("\n...");
      }
      else if (nNumApprovals < iMaxApprovals)
      {
          csWork += _T("\n* ");
          //sscoadk-321+
		  if (co.GetfOperationsAllowSaleofRestrictedItems() || ( co.IsAttendantModeOn() && co.GetfOperationsAllowSaleofRestrictedItemsInAssist() ))
		  {
            csWork += ps.GetPSText(TXT_RESTRICTEDITEMS);
		  }
		  else
		  {
			csWork += ps.GetPSText(LTT_UNSELLABLEITEM);
		  }
		  //sscoadk-321-
      }
    }

/*    if (isVoidRatioApprovalRequired())
    {
      if (++nNumApprovals == iMaxApprovals)
      {
          csWork += "\n...";
      }
      else if (nNumApprovals < iMaxApprovals)
      {
          csWork += "\n* ";
          csWork += ps.GetPSText(TXT_CANCELEDITEMS);
      }
    }*/

    if (isCouponRatioApprovalRequired())
    {
      if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
      {
		  bHasEllipsis = true;
          csWork += _T("\n...");
      }
      else if (nNumApprovals < iMaxApprovals)
      {
          csWork += _T("\n* ");
          csWork += ps.GetPSText(TXT_COUPONS);
      }
    }

	//Security Log button is gone, this should be gone also
    if (isSecurityApprovalRequired())
    {
	  if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
      {
		  bHasEllipsis = true;
          csWork += "\n...";
      }
      else if (nNumApprovals < iMaxApprovals)
      {
          csWork += "\n* ";
          csWork += ps.GetPSText(BTT_SALOGS);
      }
	  /*CString csAnchorStateName = getSCOTStateName();
	  CString csDesc, csInstruction;
	  csDesc = ps.GetPSText(BTT_SALOGS, SCOT_LANGUAGE_PRIMARY);
	  csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, _T(""), csAnchorStateName, _T(""), ps.GetPSText(BTT_SALOGS, SCOT_LANGUAGE_PRIMARY));
	  trace(L7, _T("+ra.OnNormalItem, Message=%s"), csInstruction);
	  ra.OnNormalItem(csInstruction);
	  trace(L7, _T("-ra.OnNormalItem"));*/
    }

    if (lNotForSaleItems)
    {
      if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
      {
		  bHasEllipsis = true;
          csWork += _T("\n...");
      }
      else if (nNumApprovals < iMaxApprovals)
      {
          csWork += _T("\n* ");
          csWork += ps.GetPSText(TXT_RECALLEDITEMS);
      }
    }

    if (lTimeRestrictedItems)
    {
	  if (co.csPrimaryLanguageCode == _T("0419"))
	  {   //Russian message for time restricted items consumes 2 lines
		  ++nNumApprovals; 
	  }
      if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
      {
		  bHasEllipsis = true;
          csWork += _T("\n...");
      }
      else if (nNumApprovals < iMaxApprovals)
      {
          csWork += (_T("\n* ")+ps.GetPSText(TXT_TIME_RESTRICTED_ITEM));
      }
    }

//USSF START
    if (lUnapprovedUSSFItems)
    {
      if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
      {
		  bHasEllipsis = true;
          csWork += _T("\n...");
      }
      else if (nNumApprovals < iMaxApprovals)
      {
		  CString csTempstr;
		  USSFutil.GetTransactionVariable(_T("USSF_CUSTOM_APPROVAL"), csTempstr);
		  if (csTempstr.GetLength() > 0)
		  {
			//warning!! need to get the line length of the message
			//nNumApprovals = nNumApprovals + linelength - 1;
			//then if (nNumApprovals >= iMaxApprovals) then
			//add ellipsis and set the flag
			//Or create a USSF_CUSTOM_APPROVAL_LINE_LENGTH transaction variable
			csWork += _T("\n* ");
			csWork += csTempstr;
		  }
		  else
		  {
			  nNumApprovals--; //restore approval count
		  }
	  }
	}
//USSF END

    if (lUnapprovedVisualVerifyItems)
    {
      if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
      {
		  bHasEllipsis = true;
          csWork += _T("\n...");
      }
      else if (nNumApprovals < iMaxApprovals)
      {
//USSF START
		  CString csTempstr;
		  USSFutil.GetTransactionVariable(_T("USSF_VISUAL_APPROVAL"), csTempstr);

		  if (csTempstr.GetLength() > 0)
		  {
			//warning!! check on comments above
			//nNumApprovals = nNumApprovals + linelength; (since visual msg will still be shown)
			csWork += _T("\n* ");
			csWork += csTempstr;

			if (lUnapprovedVisualVerifyItems > 1)
			{
				csWork += _T("\n* ");
				csWork += ps.GetPSText(TXT_VISUALITEMS);
			}
		  }
		  else
		  {
			csWork += _T("\n* ");
			csWork += ps.GetPSText(TXT_VISUALITEMS);
		  }
//USSF END
      }
    }

	if (lUnapprovedHopperSubstitution)
    {
      if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
      {
		  bHasEllipsis = true;
          csWork += _T("\n...");
      }
      else if (nNumApprovals < iMaxApprovals)
      {
          csWork += _T("\n* ");
          csWork += ps.GetPSText(BUTTON_RA_HOPPER_ERROR);
      }
    }

    if (lUnknownItems)
    {
      if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
      {
		  bHasEllipsis = true;
          csWork += _T("\n...");
      }
      else if (nNumApprovals < iMaxApprovals)
      {
          csWork += _T("\n* ");
          csWork += ps.GetPSText(TXT_UNKNOWNITEMS);
      }
    }
//	+TAR 352409 commented
	/*
	if (bSuspendFailure)
	{
      if (++nNumApprovals == iMaxApprovals)
      {
          csWork += _T("\n...");
      }
      else if (nNumApprovals < iMaxApprovals)
      {
          csWork += _T("\n* ");
          csWork += ps.GetPSText(TXT_SUSPEND_INCOMPLETED);
      }
	}
	*/
//	-TAR 352409 commented
    if (lPriceRequiredItems)
    {
	  if (co.csPrimaryLanguageCode == _T("0419"))
	  {   //Russian message for price needed items consumes 2 lines
		  ++nNumApprovals; 
	  }
      if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
      {
		  bHasEllipsis = true;
          csWork += _T("\n...");
      }
      else if (nNumApprovals < iMaxApprovals)
      {
          csWork += _T("\n* ");
          csWork += ps.GetPSText(TXT_PRICENEEDEDITEMS);
      }
    }

    if (lRestrictedNotAllowedItems)
    {
	  if (co.csPrimaryLanguageCode == _T("0419"))
	  {   //Russian message for restricted items not allowed consumes 2 lines
		  ++nNumApprovals; 
	  }
      if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
      {
		  bHasEllipsis = true;
          csWork += _T("\n...");
      }
      else if (nNumApprovals < iMaxApprovals)
      {
          csWork += _T("\n* ");
          csWork += ps.GetPSText(TXT_RESTRICTEDNOTALLOWEDITEMS);
      }
    }

    if (lQuantityRestrictedItems)
    {
	  if (co.csPrimaryLanguageCode == _T("0419"))
	  {   //Russian message for restricted items not allowed consumes 2 lines
		  ++nNumApprovals; 
	  }
      if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
      {
		  bHasEllipsis = true;
          csWork += _T("\n...");
      }
      else if (nNumApprovals < iMaxApprovals)
      {
          csWork += _T("\n* ");
          csWork += ps.GetPSText(TXT_QUANTITYRESTRICTEDITEMS);
      }
    }

    if (fCantScanCoupon)
    {
      if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
      {
		  bHasEllipsis = true;
          csWork += _T("\n...");
      }
      else if (nNumApprovals < iMaxApprovals)
      {
          csWork += _T("\n* ");
          csWork += ps.GetPSText(TXT_CANTSCANCOUPON);
      }
    }

	if (fUnDeActivatedItem)
    {
	  if (co.csPrimaryLanguageCode == _T("0408"))
	  {   //Greek message for EAS Active item consumes 2 lines
		  ++nNumApprovals; 
	  }
      if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
      {
		  bHasEllipsis = true;
          csWork += _T("\n...");
      }
      else if (nNumApprovals < iMaxApprovals)
      {
          csWork += _T("\n* ");
          csWork += ps.GetPSText(RA_THIRDLINE_UNDEACTIVATED);
      }
    }

    if (fRequestForHelp)
    {
      if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
      {
		  bHasEllipsis = true;
          csWork += _T("\n...");
      }
      else if (nNumApprovals < iMaxApprovals)
      {
          csWork += _T("\n* ");
          csWork += ps.GetPSText(TXT_REQUESTFORHELP);
      }
    }

    if (bDropoffRequired && !bDropoffApproved)
    {
      if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
      {
		  bHasEllipsis = true;
          csWork += _T("\n...");
      }
      else if (nNumApprovals < iMaxApprovals)
      {
          csWork += _T("\n* ");
          csWork += ps.GetPSText(TXT_DROPOFFCOUPONS);
      }

	  CString coup;
	  float amt = (float) lTotalCouponAmount;
	  amt /= 100;
	  coup.Format(ps.GetPSText(RA_COUPON_AMOUNT, SCOT_LANGUAGE_PRIMARY), lCoupons, amt);

	  //awk.8.6.2 start TAR
	  /*Adding support for DropOff coupon to RAP 3.0, delayed intervention functionality
	  added for TAR211029 */
	  trace(L7, _T("+ra.OnReadyToCollect, Collect Coupons\n"));
	  CString csSecInfo;
	  csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),
		  ps.GetPSText(LTT_DROPOFFCOUPONS, SCOT_LANGUAGE_PRIMARY), //Description
		  ps.GetPSText(RA_LANEBTT_COLLECCOUPONS, SCOT_LANGUAGE_PRIMARY), //Button
		  ps.GetPSText(RA_FIRSTLINE_COLLECTCOUPONS, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""), _T("")); //Status line
	  ra.OnReadyToCollect(csSecInfo);
	  trace(L7, _T("-ra.OnReadyToCollect, Collect Coupons\n"));
	  //awk.8.6.2 end TAR211029
    }
//  +TAR 352409 Commented
/*    if (bSuspendRequested)
	{
	  if (!bSuspendApproved)
	  {
        if (++nNumApprovals == iMaxApprovals)
        {
            csWork += _T("\n...");
        }
        else if (nNumApprovals < iMaxApprovals)
        {
            csWork += _T("\n* ");
            csWork += ps.GetPSText(RA_TRANS_SUSPEND_ID);
        }
		if (!lUnapprovedVisualVerifyItems)	//TAR344019 - RAP will not go through Allow Suspend if there is unapproved visual verify item.
		{
			// cause RAP to go to Allow Suspend
			trace(L7, _T("+ra.OnReadyToSuspend"));
			ra.OnReadyToSuspend();
			trace(L7, _T("-ra.OnReadyToSuspend"));
		}
	  }
	}
*/
//  -TAR 352409 Commented
#endif
// +TAR 352409
	if (bSuspendRequested)
	{
	  //if (!bSuspendApproved)  //TAR 352409 commented
	  if (!bSuspendApproved && !bSuspendFailure)	// TAR 352409 newly added
	  {
#ifndef _CPPUNIT
        if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
        {
			bHasEllipsis = true;
            csWork += _T("\n...");
        }
        else if (nNumApprovals < iMaxApprovals)
        {
            csWork += _T("\n* ");
            csWork += ps.GetPSText(RA_TRANS_SUSPEND_ID);
        }
		if (!lUnapprovedVisualVerifyItems)	//TAR344019 - RAP will not go through Allow Suspend if there is unapproved visual verify item.
		{
			// cause RAP to go to Allow Suspend
			trace(L7, _T("+ra.OnReadyToSuspend"));
			ra.OnReadyToSuspend();
			trace(L7, _T("-ra.OnReadyToSuspend"));
		}
#endif

	  }

	  else if(bSuspendApproved && bSuspendFailure)
	  {
		if (bSuspendFailure)
		{
            bSuspendRequested = false;
#ifndef _CPPUNIT
			if (++nNumApprovals >= iMaxApprovals && bHasEllipsis == false)
			{
				bHasEllipsis = true;
				csWork += _T("\n...");
			}
			else if (nNumApprovals < iMaxApprovals)
			{
				csWork += _T("\n* ");
				csWork += ps.GetPSText(TXT_SUSPEND_INCOMPLETED);
				//TAR 352409 new code added to generate RAP Intervention event if Suspension is Declined
				cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("ApproveSuspensionDecline"));
		    	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ApproveSuspensionDecline") );
				m_View = DATANEEDED_SUSPENSION_DECLINE;
				CString csDesc, csSecInfo;
				csDesc.Format(ps.GetPSText(RA_SUSPEND_DECLINED,SCOT_LANGUAGE_PRIMARY) + _T(" %s"), io.d.csDescription);
				csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_LNBTN_SUSPENSIONDECLINE_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_LNWND_SUSPENSIONDECLINE, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""));
				ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
			}
#endif
		}

	 }
// -TAR 352409
   }
#ifndef _CPPUNIT
}
  // start tar 256141
  if (g_bgCheckCashChanger)
  {
     if (dm.GetCoinsVeryLow())
     {
	  csWork.Empty();
	  csWork += _T("*");
	  csWork += ps.GetPSText(LTT_NCR_ECOINDISP_ALARM_ERROR);
	  csWork += _T("\n\n");
	  // TAR 283924 start
	  if(m_nCoinDispenserWithLowSensor == 1)
	  {
	    csWork += ps.GetPSText(MSG_REFILLCOINWITHLOWSENSOR);  // Display 1st sentence only, if Telequip dispenser does not have a Low Sensor
	  }
	  else
	  {
	    csWork += ps.GetPSText(MSG_NCR_ECOINDISP_ALARM_ERROR);  // Display both sentences, if Telequip dispenser does not have a Low Sensor
	  }
	  // TAR 283924 end

	  CString csInfo;
	  csInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),
						ps.GetPSText(RA_MEDIA_LOW_ID, SCOT_LANGUAGE_PRIMARY),
						ps.GetPSText(MSG_MEDIA_LOW, SCOT_LANGUAGE_PRIMARY),
						_T(""), _T(""), _T(""));
	  ra.OnNormalItem(csInfo);

        ra.OnNormalItem(ps.GetPSText(RA_LOWCOINS));
     }
     // TAR 279497 start
     else if (dm.GetBillsVeryLow())
     {
        csWork.Empty();
        csWork += _T("*");
        csWork += ps.GetPSText(LTT_BILLDISPENSERERROR);
        csWork += _T("\n\n");
        csWork += DMCheckCashCounts(false);

        CString csInfo;
        csInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),
                     ps.GetPSText(RA_MEDIA_LOW_ID, SCOT_LANGUAGE_PRIMARY),
                     ps.GetPSText(MSG_MEDIA_LOW, SCOT_LANGUAGE_PRIMARY),
                     _T(""), _T(""), _T(""));
        ra.OnNormalItem(csInfo);

        ra.OnNormalItem(ps.GetPSText(RA_LOWBILLS));
     }
     // TAR 279497 end
  }
#endif
  if(isGenericDelayedInterventionApprovalRequired())
  {
      long lDelayedInterventionQueueSize = m_GenericDelayedInterventions.GetSize();
      for (int i=0; i<lDelayedInterventionQueueSize; i++)
      {
          CQueuedActionGenericDelayedIntervention *pDelayedIntervention = (CQueuedActionGenericDelayedIntervention* const) m_GenericDelayedInterventions.Peek(i);

          if (!csWork.IsEmpty())
          {
              csWork += _T("\n");
          }
          csWork += _T("*");
          csWork += pDelayedIntervention->GetInterventionInstruction();
      }
  }
  //end tar 256141
 if (bSuspendRequested && !bSuspendApproved)  //sscoadk-5501
 { 
       trace(L6, _T("SMWaitForApprovalBase::showScreen, no need to show WaitForApproval, waiting RCM to transfer to Suspend screen"));
 }
 else
 {

  SAWLDBScannerDisable();			// Disable the scanner
  ps.Message12(csWork, true);
  ps.ShowCMTBText(csTBMessage2Scot);
  ps.ShowCMFrame(false);
 }
#ifndef _CPPUNIT
  
  //sscoadk-321+
  if ( (!co.GetfOperationsAllowSaleofRestrictedItems() || ( (co.IsInAssistMenus() || fStateAssist || co.IsAttendantModeOn()) && !co.GetfOperationsAllowSaleofRestrictedItemsInAssist())) && lUnapprovedRestrictedItems )
  {
	ra.RequestEnableAssistMode(false);
  }
  //sscoadk-321-
  

  DisplayAuxScreen(AUX_SCREEN_USE_TOUCHSCREEN, true);	// TAR 228153
#endif
}

#ifndef _CPPUNIT

//////////////////////////////////////////////////////
SMStateBase * SMWaitForApprovalBase::stateAfterPasswordVerify()
{
  if (fVoidTrx)
    RETURNSTATE(SmAbort)
  else if (m_bVerifySig)
  	RETURNSTATE(SmConfirmSignature)

  RETURNSTATE(SmAuthorization)
}

/////////////////////////////////////////////
SMStateBase  *SMWaitForApprovalBase::PSButtonHelp(void)
{
  return STATE_NULL;
}

/////////////////////////////////////////////
SMStateBase  *SMWaitForApprovalBase::PSButton1(void)
{
//BP 2/9/00 following code should actually call promptForOperator to handle bad pin and
//language transition
//inputMode = PROMPTING_FOR_OPERATOR;
//showScreen(false);
  promptForOperator();
  return STATE_NULL;
}

/////////////////////////////////////////////
SMStateBase  *SMWaitForApprovalBase::PSCancelKey(void) /// override where needed
{
  DMSayPhrase(KEYPADINVALID);
  return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMWaitForApprovalBase::OnBackToLGW(void)
{
	// TAB re-enable scanner and clear Wait For Approval State
	TABReEnableScannerAndClearWaitForApprovalState();

	//TAR 362499 begin
	if (!isAnyApprovalRequired() )
	{
	    SMStateBase *sReturnState = CkStateStackAndReturn();
	    if (sReturnState != STATE_NULL)
	    {
	        return sReturnState;
	    }
	    else
	    {
	        return createAnchorState();
	    }
	}
	// TAR 362499 end

	return STATE_NULL;
}

//////////////////////////////////////////////////////////////
SMStateBase *SMWaitForApprovalBase::OnWtIncreaseNotAllowed() //tar 197857
{
	if(bIsInFinalization)	// TAR 405492 
	{		
		return FinalizationWtIncrease(BES_FINISH);	
	}
	else 
	{
        if (fVoidTrx && (secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_FORGIVEN ))
        {
            return STATE_NULL;
        }
		return VoidingWtIncrease(BES_WAITFORAPPROVAL, BES_WAITFORAPPROVAL);
	}
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMWaitForApprovalBase::OnWtDecrease() //tar 197857
{
	//TAR 214099 starts: Allow removing item while waiting for signature approval
	if (m_bVerifySig) //merged tar 223592 - fixed by Beiwei
		return STATE_NULL;
	else
		return ThresholdViolWtDecrease(BES_WAITFORAPPROVAL, BES_WAITFORAPPROVAL);
	//TAR 214099 ends
}


//TAR198810+: Added its own UnIntialize to set the light right
//Before it called SMOperatorPasswordStateBase::Uninit
//////////////////////////////////////
void SMWaitForApprovalBase::UnInitialize(void)
{
    ra.RequestEnableAssistMode(true);   // Reenable assist mode
	if (bIsInFinalization) //tar 194770/223051/223230
		g_bAllowRemove = true;

	if(g_bgVerifySig)
	{
		SMOperatorPasswordStateBase::UnInitialize();
		return;
	}
	// tar 211746 don't turn off red if there are other red interventions waiting
	// because turning off red can let any pending virtual yellow be turned on which
	// means both red and yellow will be on if we immediately turned red back on
	// also will help prevent extra pages that can occur when turning it off and then on
    if(!AreAnyRedApprovalsPending())
	{
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
	if (isAnyApprovalRequired() || bSecurityTrustLevelAlert)
	{
		dm.restoreTriColorLight();
		// sometimes we are seeing red from restore when it is no longer
		// accurate because we have done two restores without a backup in between
		// or processed multiple approvals at once such as when leaving SM
		// so check if any of the red type approvals are still pending
		// if not, turn off the red light
		if(!AreAnyRedApprovalsPending())
		{
			// (*) SR898 ====================
            DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                               DM_TRICOLORLIGHT_OFF,
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
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("WaitForApproval") );	// RFC 330050
	SMOperatorPasswordStateBase::UnInitialize();
}
//TAR198810-

///////////////////////////////////////////////////////////////////////////
SMStateBase* SMWaitForApprovalBase::OnUnDeActivatedTheftTag(void)
{
	if (fUnDeActivatedItem)
	{
		//Update RAP receipt with EAS item refernce line message
        // TAR 205318 - No need to display message since its already
        // been sent to RAP once.  DEM
		//CString sDescription = ps.GetPSText(RA_RECEIPT_MSG_EASITEM_REFERENCE, SCOT_LANGUAGE_PRIMARY);
		//ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + sDescription);
        return STATE_NULL;
	}
	else
	{
		CString sDescription = ps.GetPSText(RA_RECEIPT_MSG_EASITEM_REFERENCE, SCOT_LANGUAGE_PRIMARY);
		ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + sDescription);
		CREATE_AND_DISPENSE(UnDeActivatedItemApproval)(getAnchorState(), getAnchorState());
	}
}

/**
 * Method:      OnCouponSensor()
 * Description: Override the implementation of SMStateBase::OnCouponSensor()
 * Parameters:  None
 * Returns:     Anchor State -> application is using a coupon sensor  
 *				STATE_NULL   -> not dealing with coupons
 * Notes:       Added for TAR 386740 - Assistance Needed for Drop off Coupons on lane didn't cleared.
 */
SMStateBase  *SMWaitForApprovalBase::OnCouponSensor(void)
{
	trace(L6, _T("SMWaitForApproval::OnCouponSensor called, IsUsingCouponSensor=%d"), IsUsingCouponSensor());
	if(IsUsingCouponSensor())
	{
		return createAnchorState();
	}
	else
	{
		return STATE_NULL;
	}
}

SMStateBase *SMWaitForApprovalBase::SASecMgrParse( const MessageElement *me)
{

    int nEvent = me->secmgrInfo.nEvt;
    trace(L6, _T("SMWaitForApprovalBase::SASecMgrParse nEvent = %d"), nEvent);
    
    LPTSTR szParms;
    szParms = new TCHAR [_tcslen(me->secmgrInfo.szParms) + sizeof(TCHAR)];
    _tcscpy( szParms, me->secmgrInfo.szParms );
    

    switch (nEvent)
    {
    case SmOnSecurityExceptionCleared:
        {
            SecImmediateIntervention *psecImmediateIntervention = new SecImmediateIntervention;
            SecExceptionClearedEventParms parms(psecImmediateIntervention);
            parms.m_bClearRAPApproval = false;
            if (_tcslen(szParms) != 0)
            {
                parms.ParseRecords(szParms);
            }
            delete [] szParms;
            delete  psecImmediateIntervention;

            if(parms.m_csDisposition == KEY_OSE_DISPOSITION_DATANEEDED)
            {
                delete [] me->secmgrInfo.szParms;
                if ( m_GenericDelayedInterventionStatus == GENERIC_DELAYED_PROCESSING)
                    return OnGenericDelayedComplete();

                if ( (getAnchorState() == BES_SECURITYANALYSIS) && IsSecurityAnalysisComplete())
                    return setAndCreateAnchorState(BES_FINISH);

                return createAnchorState();
            }

            if (fVoidTrx && (secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_FORGIVEN ))
            {
                delete [] me->secmgrInfo.szParms;
                return STATE_NULL;
            }
        }
    }
    return STATE(OperatorPasswordState)::SASecMgrParse(me);
}
#endif //_CPPUNIT