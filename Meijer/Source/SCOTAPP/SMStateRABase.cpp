//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateRABase.cpp
//
// TITLE: Remote Approval Functions
//
//
// AUTHOR:  Ying Xie
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include "RAProcedures.h"           // RA procedures

#ifndef _CPPUNIT
#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV abstract base state
extern RAProcedures ra;             // global RA object
#include "SCOTRADll.h"
#include "SMSuspend.h"
#include "SMDropoffCoupons.h"
#include "SMSmConfirmSignature.h"
#include "SMTakeCard.h"             // Take Card state
#include "SMRequestSig.h"
#include "SMLaneClosed.h"
#include "SigReceiptDataFile.h"
#include "SMWaitForRemoteAssistance.h"
#include "SMWaitForApproval.h"      // Wait for approval
#include "MPProcedures.h"			// RFC 228102
#include "SMSecUnexpectedIncrease.h"
#include "RCMKeyValue.h"
#include "RAPKeyvalue.h"
#include "SMSmAssistMode.h"
#include "SMInProgress.h"
#include "SMFinish.h"
#include "SMDegradedContinueTrans.h"		//degraded mode tars
// AKR extern DLLEXPORT CTransBroker tb;   // global TB object
#include "SMContinueTrans.h"
#include "CmFunctions.h"  //SR93.4
#include "SMSmDataEntry.h"

DLLEXPORT extern MPProcedures mp;   // RFC 228102

#else
#include "FakeSMStateBase.h"
#endif
#include "QueuedActionGenericDelayedIntervention.h"
#include "Base64EncodeDecode.h"
#define COMP_ID ID_RA               // base state component
#define T_ID    _T("RABase")

//+dp185016 tar 280966
bool SMStateBase::m_bClearStack = false;
//-dp185016

//////////////////////////////////////////////////////////////////////////////////////////////////
// additional Initialization

#ifndef _CPPUNIT
long SMStateBase::RAInitialize(void)
{
  long rc = 0;
  trace(L6,_T("+Init"));
  m_bEncryptedBarcodeAtSignOn = false;  //SR93.4
  rc = ra.Initialize();

  trace(L6,_T("-Init %8.8x"),rc);
  return rc;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
long SMStateBase::RAUnInitialize(void)// before SCOT
{
  long rc = 0;
  trace(L6,_T("+UnInit"));

  rc = ra.UnInitialize();

  trace(L6,_T("-UnInit %d"),rc);
  return rc;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::RedoSigCap(void)
{
  // Handle this same as PSButton2 in SMSmConfirmSignature.  //TAR126560
  trace(L6,_T("+RedoSigCap"));
  trace(L6,_T("-RedoSigCap"));
  //ra.OnSigCapApproval();  // Tell RAP it's approved, redo will be automatic. ? not sure about this, comment it for now

  //TAR261301+
  CString csStateName = mp.sCurr->GetRuntimeClass()->m_lpszClassName;
  if(csStateName == _T("SMSecUnExpectedIncrease"))	// Currently at UnexpectedIncrease
  {
  	CREATE_AND_DISPENSE(SecUnExpectedIncrease)(BES_REQUESTSIG, BES_REQUESTSIG);
  }
  else
  {
    RETURNSTATE(RequestSig)
  }
  //TAR261301-
}


//////////////////////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::ApproveSigCap(void)
{
    trace(L6,_T("+ApproveSigCap"));
	   //Send signature data to TB
    if(co.fOperationsSendSignatureDataToTB)
    {
        BSTR pData= NULL;
        gpSCOTSigCap->Get5991Format(&pData);  // Get 5991 format for either RAP or S-Receipt file
        TBSendSigData(CustomerName((LPCTSTR)cDMLastTrack),(LPCTSTR) pData,(DWORD)(SysStringByteLen(pData)));
        SysFreeString(pData); // Since SMRequestSigBase::DMSigcapEnded frees the BSTR after calling TBSendSigData
                              // we need to do the same here.
    }

    trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(RA_TENDER_COMPLETE, SCOT_LANGUAGE_PRIMARY));
	ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY +
                        ps.GetPSText(RA_TENDER_COMPLETE, SCOT_LANGUAGE_PRIMARY));
	trace(L7, _T("-ra.OnNormalItem"));
  // Handle this same as PSButton1 in SMSmConfirmSignature. TAR#126560  LPM111999

	EndOfTransactionPrint(); //SR697 
    
    // +TAR 390646
    if (co.m_bOperationsPrintSignatureReceipt)
    {
        BSTR pData= NULL;
        gpSCOTSigCap->Get5991Format(&pData);
        SaveSigReceiptData(pData) ;
        SysFreeString(pData);
    }
    // -TAR 390646

	//TAR260600+
	CString csStateName = mp.sCurr->GetRuntimeClass()->m_lpszClassName;
	if(csStateName == _T("SMSecUnExpectedIncrease"))	// Currently at UnexpectedIncrease
	{
		CREATE_AND_DISPENSE(SecUnExpectedIncrease)(BES_TAKECARD, BES_TAKECARD);
	}
	else
	{
		CREATE_AND_DISPENSE(TakeCard)(true) ;
	}
    //TAR260600-
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::RAParse(MessageElement *me)
{
  SMStateBase *sUser = STATE_NULL;
  CString csStateName;
  trace(L6,_T("+RAParse"));

	/******cm150000: Removed In Merged Code******/
  //TRACE("SMStateBase::RAParse: me->raInfo.msg = %d", me->raInfo.msg);
	/******cm150000: End Remove Merged Code******/
	/******cm150000: Added In Merged Code******/
	trace(L4,_T("SMStateBase::RAParse: me->raInfo.msg = %d on thread 0x%08.8x"), me->raInfo.msg, GetCurrentThreadId());
	/******cm150000: End Merged Code***********/

  switch (me->raInfo.msg)
  {
    case WMRA_SEND_UNSOLICITED_DATA:
       {
#ifndef _CPPUNIT
		//+SR814.4
		CString csData = me->raInfo.pData;
	
		if ((csData.Find(_T("DisableSecurity")) != -1))
		{
			trace(L6, _T("RAPUnsolicitedDataReceived(%s) - Pressed Security Disable button"),csData); 
	
			ra.OnDisableSecurity();

			if(g_bIsSecurityDisableSubCheckOnly)
			{	
				co.SetSecurityDisableSubCheck(true);
				SetSecurityConfig(CONFIG_KEY_IS_SECURITYDISABLE_SUB_CHECK_ON, _T("true"));	
			}
			else
			{	
				co.SetSecurityDisable(true);
				
				UpdateStateName(ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY)); //SSCOADK-6661    

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

			//+SSCOADK-6651
			if( getAnchorState() == BES_PUTBAGONSCALE  ) {
				trace(L6, _T("Security Disabled at Put Bag On Scale proceed to Scan And Bag"));
				return TareNone();
			}
			//-SSCOADK-6651

			return STATE_NULL;
		}
		else if ((csData.Find(_T("EnableSecurity")) != -1))
		{
			trace(L6, _T("RAPUnsolicitedDataReceived(%s) - Pressed Security Disable button"),csData);
	
			ra.OnEnableSecurity();
		

			if(g_bIsSecurityDisableSubCheckOnly)
			{	
				co.SetSecurityDisableSubCheck(false);
				SetSecurityConfig(CONFIG_KEY_IS_SECURITYDISABLE_SUB_CHECK_ON, _T("false"));
			}
			else
			{
				co.SetSecurityDisable(false);

				CKeyValueParmList parmList;
				CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_EXIT_DISABLE_SECURITY_MODE);
				_bstr_t bstrParmList((const _TCHAR *)parmList);
				BSTR bstrResult = NULL;
				bstrResult = NULL;
				SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
				SysFreeString(bstrResult);
			}
   
			if(csOperator != _T("")) 
			{ rp.SetOperator(csOperator); }
			rp.ModeChange(rp.m_lAttendantEntryID, SECURITYDISABLED, EXIT);

			return STATE_NULL;
		}
		//-SR814.4
   
       SMStateBase *p = OnRAPUnsolicitedDataReceived(me->raInfo.pData);
       delete[] me->raInfo.pData;
       return p;
#endif
       }
    case WMRA_REQUEST_APPROVAL:
      {

#ifndef _CPPUNIT
        SCOTDO_APPROVAL_INFO Info;
#endif
        CString sData = me->raInfo.pData;
#ifndef _CPPUNIT
		delete[] me->raInfo.pData;
		// Get pre-approval info before we go thru the approval.
		/******cm150000: Added In Merged Code******/
		trace(L4, _T("SMStateBase::RAParse: Calling GetApprovalInfo()."));
		/******cm150000: End Merged Code***********/
		ra.GetApprovalInfo(&Info);
   		int approveSigCap = Info.bSigCapApprovalRequired;

		// call this first since it updates internal approval info
		/******cm150000: Added In Merged Code******/
		trace(L4,_T("SMStateBase::RAParse: Calling OnRemoteApproval() handle:(0x%08.8x) - operatorID:(%s) - approvalCode:(0x%08.8x) - sData:(%s) - dwcookie:(0x%08.8x)"),
        me->raInfo.handle, me->raInfo.operatorID, me->raInfo.approvalCode, sData, me->raInfo.dwCookie);
		/******cm150000: End Merged Code***********/

		ra.OnRemoteApproval(
				me->raInfo.handle,
				me->raInfo.operatorID,
				me->raInfo.password,
				me->raInfo.approvalCode,
				(LPCTSTR)sData,
				me->raInfo.dwCookie
				);

        if(!isAnyApprovalRequired())
        {
            UpdateStateName(getSCOTStateName());
        }

		//moved this portion back here after ra.OnRemoteApproval to roll back the earlier changes
		//+TAR354093 - transferred this code before ra.OnRemoteApproval to fix tar354093
        if (ra.IsRAPDataAvailable())
		{
			trace(L4, _T("SMStateBase::RAParse: Calling OnRapDataReceived() Data:(%s)"), ra.GetRAPData());
			return OnRAPDataReceived(ra.GetRAPData());
		}
		//-TAR354093
#endif

		//TAR 192098 start
		DWORD TypeOfApproval;
		TypeOfApproval = ra.GetCurrentApprovalCode();
#ifndef _CPPUNIT
		//TAR 257459 start
		if (TypeOfApproval == RA_CAT_STALE)
		{	// Approval for stale condition; ignore it
			trace(L2,_T("SMStateBase::RAParse: Ignoring stale approval from RAP"));
			return STATE_NULL;
		}
#endif
		//TAR 257459 end
		if (TypeOfApproval == RA_CAT_AGERESTRICT && lUnapprovedRestrictedItems != 0)  //393655
		{
		  CString csRAPdob = sData;   //get age entered at RAP station
		  //TAR 196908 start
		  CString csoperatorID = me->raInfo.operatorID;
		  CString cspassword = me->raInfo.password;
		  //TAR 196908 end

		  if (csRAPdob != _T("1;"))						 //check to see if dob not needed key pressed
		  {
#ifndef _CPPUNIT
			CString csFromRAPdob = csRAPdob.Right(csRAPdob.GetLength() -1);  //removeleading ";"
			if(!VerifyBirthDate(csFromRAPdob,csoperatorID,cspassword))
			{


			// Feng122299 Selling restricted item in assist mode still need approval. Since store people may not know this is a restricted item.
			// Need input customer age to sell it.
			//  if (!bAutoApprovedItem)            //TAR#125677 Don't worry about approving rest items if in assist mode

    		io.lRestCode = 1;				   // show app needed msg
		    
            // (*) SR898 ====================
			if(!co.fOperationsImmediateInterventionForRestrictedItems)
			{
				DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                               DM_TRICOLORLIGHT_ON,
                               DM_TRICOLORLIGHT_NORMALREQUEST);
			}
			else
			{
				DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                               DM_TRICOLORLIGHT_BLINK_1HZ,
                               DM_TRICOLORLIGHT_NORMALREQUEST);
			}

			// remote approval
			CString csAnchorStateName = getSCOTStateName(); //Tar 247393
			CString csDesc, csInstruction;
			csDesc = ps.GetPSText(RA_AGE_RESTRICTED_ID, SCOT_LANGUAGE_PRIMARY);

			trace(L7, _T("+ra.OnAgeRestrictedItem, Message=%s"), csInstruction);
			//TAR 196908 start
			long nCustomerAge = GetAge(csFromRAPdob);
			long nRestrictedAge = ps.FindRequiredAge(nCustomerAge);

			int nYearRequired;
			COleDateTime dtToday;
			CString csValidBirthday;

			dtToday = COleDateTime::GetCurrentTime();
			nYearRequired = dtToday.GetYear() - nRestrictedAge;

			csValidBirthday.Format(ps.GetPSText(RA_VALIDBIRTHDAYREQUIRED, SCOT_LANGUAGE_PRIMARY), ps.DateFormat(dtToday.GetMonth(), dtToday.GetDay(), nYearRequired));
			//Begin TAR 196356
			_TCHAR TempBuffer[20];
				_itot(nRestrictedAge, TempBuffer, 10);	// turn the index into a string so it can be passed via Add below
		    CString csAge(TempBuffer);
			csDesc += _T("\n") + csAge + _T(" ")
				      + ps.GetPSText(RA_AGERESTRICT_MINIMUM_AGE, SCOT_LANGUAGE_PRIMARY)
			          + _T("\n") + csValidBirthday;
			csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_LANEBTT_RESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, 
				co.IsSecurityDisableOn()==true || co.GetSecurityDisableSubCheckOn() ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), ps.GetPSText(RA_THIRDLINE_RESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY));
			// End TAR 196356
			//TAR 196908 end
			trace(L7, _T("+ra.OnAgeRestrictedItem, Message=%s"), csInstruction);
                        
                        //Removing for TAR#387926
			//TAR 371932 &tar 371918+
            //trace(L5, _T("ra.OnAgeApproval:reporting purpose only to close the OnAgeRestrictedItem called in ItemDetail"));
			//ra.OnAgeApproval();
            //TAR 371932 & tar 371918-

            ra.OnAgeRestrictedItem(csInstruction, nRestrictedAge, lUnapprovedRestrictedItems, FALSE, co.fStateDOBNotNeededButton ); //SR803
			trace(L7, _T("-ra.OnAgeRestrictedItem"));
			}
#endif
		  }
		  else
		  {
            CString csDateNotNeeded = ps.GetPSText(RA_AGE_RESTRICTED_ID, SCOT_LANGUAGE_PRIMARY);
            csDateNotNeeded +=  _T("\n") + ps.GetPSText(BTT_BIRTHDATENOTREQUIRED, SCOT_LANGUAGE_PRIMARY);
			trace(L7, _T("+ra.OnNormalItem, Message=%s"), csDateNotNeeded);
            ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csDateNotNeeded); // Display valid birthday on RAP
			trace(L7, _T("-ra.OnNormalItem"));
            long lSavedlUnapprovedRestrictedItems = lUnapprovedRestrictedItems;
			lUnapprovedRestrictedItems = 0; //TAR#350775
            m_lAgerestrictQueuedtoRAP = 0;
            lRegularYellowNeedingApproval = 0;
            fApprovedForRestrictedItems = true;
            fAgeObtained = true;
            nCustomerAge = 99;
            // update ITEM_AUTOAPPROVAL_FLAG for age-restricted items for NGUI
            ps.RestrictedItemNotAllowed(nCustomerAge, lSavedlUnapprovedRestrictedItems);
		  }
#ifndef _CPPUNIT
		  //TAR 364905 Begin
		  if (co.fOperationsForceDelayedApprovalBeforeSuspend && !IsForceDelayedApprovalBeforeSuspend())
		  {
			trace(L7, _T("+ra.ForceApprovalBeforeSuspend, ForceApprovalBeforeSuspend=false"));
			ra.ForceApprovalBeforeSuspend(false);
			trace(L7, _T("-ra.ForceApprovalBeforeSuspend"));
		  }//TAR 364905 End
#endif
		}
		//TAR 192098 end


#ifndef _CPPUNIT
        if (TypeOfApproval == RA_CAT_SECURITY)
        { // modeled after SMSmSecurityLogsBase::PSButton1
			lSAYellowNeedingApproval = 0;
			lSALoggedMsgs = 0;
			//SASendSCOTInput( STORE_MODE );   // Tar149058 Send Input to Security when go in and out store mode
			//SAClearOtherTallies();
			trace(L7, _T("+ra.OnSecurityApproval"));
			ra.OnSecurityApproval();
			trace(L7, _T("-ra.OnSecurityApproval"));

            trace(L6, _T("Trust Level intervention is approved remotely so reset bSecurityTrustLevelAlert flag."));
            bSecurityTrustLevelAlert = false;

			CKeyValueParmList parmList;

			if( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_MISMATCHED )
			{
				CKeyValue<long>::Put(parmList, KEY_OSE_EXCEPTION_ID, secEventParms.m_lExceptionId);
				CKeyValue<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_AUTHORIZATION, KEY_EXCEPTION_AUTHORIZATION_RAP);
				CKeyValue<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_OPERATION, KEY_EXCEPTION_OPERATION_CLEAR_WITH_UPDATE);
				_bstr_t bstrParmList((const _TCHAR *)parmList);
				SecMgr.OverrideException(lSecMgrCookie, bstrParmList);
				parmList.Clear();
			}

			CKeyValue<const _TCHAR*>::Put(parmList, KEY_OSE_EXCEPTION_ID, _T("*"));
			CKeyValue<const _TCHAR*>::Put(parmList, KEY_OSM_OPERATION, KEY_EXCEPTION_OPERATION_CLEAR_DELAYED);
			_bstr_t bstrTempParmList((const _TCHAR *)parmList);
			SecMgr.OverrideException(lSecMgrCookie, bstrTempParmList);

			//SASendSCOTInput(CUSTOMER_MODE);

			// (*) SR898 ====================
            DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                               DM_TRICOLORLIGHT_OFF,
                               DM_TRICOLORLIGHT_NORMALREQUEST);			//TAR125350  Security lane light
        }
#endif

		// call this second since it should now reflect latest approval info
		/******cm150000: Added In Merged Code******/
		//trace(L4, _T("SMStateBase::RAParse: Calling GetAprovalInfo() (again)."));
		/******cm150000: End Merged Code***********/
		//ra.GetApprovalInfo(&Info);
   		//int approveSigCap = Info.bSigCapApprovalRequired;

        // update corresponding core app flags to sync with ra
        if (TypeOfApproval == RA_CAT_VISUALVERIFY)
		{
          lUnapprovedVisualVerifyItems = 0;
          
            POSITION nPos = g_mapTransactionVisualVerify.GetStartPosition();
            while (nPos != NULL)
            {
                COleVariant vValue;
                CString csItemId;
                int nKey;
                bool bValue;
                g_mapTransactionVisualVerify.GetNextAssoc(nPos, nKey, bValue);

                if( !bValue )
                {
                    bValue = true;
                    g_mapTransactionVisualVerify.SetAt(nKey, bValue);
                    csItemId.Format(_T("%d"), nKey);
                    vValue = (long) bValue;
                    ps.GetPSXObject()->SetReceiptItemVariable(_T("CMSMReceipt"), csItemId, _T("ITEM_AUTOAPPROVAL_FLAG"), vValue);
                }
            }
            ps.GetPSXObject()->UpdateReceiptControls(_T("CMSMReceipt"));

#ifndef _CPPUNIT
		  //TAR 364905 Begin
		  if (co.fOperationsForceDelayedApprovalBeforeSuspend && !IsForceDelayedApprovalBeforeSuspend())
		  {
			trace(L7, _T("+ra.ForceApprovalBeforeSuspend, ForceApprovalBeforeSuspend=false"));
			ra.ForceApprovalBeforeSuspend(false);
			trace(L7, _T("-ra.ForceApprovalBeforeSuspend"));
		  }//TAR 364905 End
#endif
		}
#ifndef _CPPUNIT
		if (TypeOfApproval == RA_CAT_HOPPERSUBSTITUTION)
		{
			HandleHopperError(sData);

		}
        if (TypeOfApproval == RA_CAT_RECALLEDITEM)
          lNotForSaleItems = 0;
        //Tar 119698
        if (TypeOfApproval == RA_CAT_TIMERESTRICT)
          lTimeRestrictedItems = 0;
        if (TypeOfApproval == RA_CAT_UNKNOWNITEM)
		{
          lUnknownItems = 0;
		  lUnknownItemsKeyed = 0;// T.D 05/20/02 TAR 198931
		}
        if (Info.bPriceUnknown)
          lPriceRequiredItems = 0;
        if (TypeOfApproval == RA_CAT_QTYRESTRICT)
          lQuantityRestrictedItems = 0;
		if (TypeOfApproval == RA_CAT_EASITEMACTIVE)
		{
			fUnDeActivatedItem = false;
		}

		if (TypeOfApproval == RA_CAT_RESTRICTEDNOTALLOWED)
		{
			lRestrictedNotAllowedItems = 0;
		}

		csStateName= this->GetRuntimeClass()->m_lpszClassName;

        if (Info.bCouponApprovalRequired)
        { // modeled after SMSmCouponItemsBase::PSButton1
          lUnapprovedCoupons    = 0;
		  // Begin TAR 197558
		  fCantScanCoupon       = 0;
		  // End TAR 197558
          lCouponsInAssist      = 0;
          //Begin TAR 234970
          lTotalCouponAmount    = 0;
          lCoupons              = 0;
          //End TAR 234970

          lExceptionItemsRequiringApproval = 0;
		  /******cm150000: Added In Merged Code******/
		  trace(L4, _T("SMStateBase::RAParse: Entering STORE_MODE to clear coupon tallies."));
		  /******cm150000: End Merged Code***********/
		  //Tar 196365
		  if(nCountPossibleDuplicateCoupon >= co.nOperationsDuplicateCouponMax)
		  {
			  nCountPossibleDuplicateCoupon = 0;
              if(g_mapTransactionCoupon.GetCount())
              {
                  g_mapTransactionCoupon.RemoveAll();
              }
		  }

		  //TAR 364905 Begin
		  if (co.fOperationsForceDelayedApprovalBeforeSuspend && !IsForceDelayedApprovalBeforeSuspend())
		  {
			trace(L7, _T("+ra.ForceApprovalBeforeSuspend, ForceApprovalBeforeSuspend=false"));
			ra.ForceApprovalBeforeSuspend(false);
			trace(L7, _T("-ra.ForceApprovalBeforeSuspend"));
		  }//TAR 364905 End

		  //SASendSCOTInput( STORE_MODE );   // Tar149058 Send Input to Security when go in and out store mode
		  trace(L4, _T("SMStateBase::RAParse: Leaving STORE_MODE."));
		  /******cm150000: End Merged Code***********/
		  //SASendSCOTInput(CUSTOMER_MODE);
		  if ((!Info.bDropoffRequired) && (TypeOfApproval != RA_CAT_COLLECTION) && (!IsUsingCouponSensor()))
		  {
			  if(csStateName.Find(_T("WaitForApproval")) != -1)
				  return DepositCouponsIfNecessary();
		  }
        }

		// Tar 169190 if Approval other payment from RAP, need to reset the flag from SCOT
		if(TypeOfApproval == RA_CAT_OTHERPAYMENT)
		{
			fOtherPayment = false;
		}

		//if Approval other approval from RAP, need to reset the flag from SCOT
		if(TypeOfApproval == RA_CAT_CALLFORHELP)
		{
			if (fRequestForHelp || fCantScanCoupon || (lCouponsInAssist > 0))
			{
				//Tar 209034 & 209045 BZ: Restore tri-light only if approval is needed
				// 209371 sometimes the restore is the wrong color if the paper is low, so moving
				// this to it's own if() LPM071002 BZ
				if (isAnyApprovalRequired())
				{
					dm.restoreTriColorLight();
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
		            // If inside a transaction, turn on the green transaction light.
		            if (fInSignedOn || fInSignedOff || fStateAssist || fSAInTransaction)
					{
			            // (*) SR898 ====================
                        DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                                           DM_TRICOLORLIGHT_ON,
                                           DM_TRICOLORLIGHT_NORMALREQUEST);
					}
				} // end else
			}
			fCantScanCoupon = false;
			fRequestForHelp = false;
			lCouponsInAssist = 0;
		}

        if (TypeOfApproval == RA_CAT_SUSPEND)
        {
			bSuspendRequested = true;
			bSuspendApproved = true;
			
			if (!fInSignedOn && !fInSignedOff) //TAR448020
			{
				trace(L6, _T("SMStateBase::RAParse: No active transaction so not going to Suspend state"));
				return STATE_NULL;
			}
			
			
			/******cm150000: Added In Merged Code******/
			trace(L4, _T("SMStateBase::RAParse: Returning Suspend state."));
			/******cm150000: End Merged Code***********/

			RETURNSTATE(Suspend)
        }

        if (TypeOfApproval == RA_CAT_COLLECTION)
        {
			bDropoffRequired = (Info.bDropoffRequired != 0);
            if (Info.bDropoffApproved)
            {
                bDropoffApproved = true;
            }
        }
        else if (Info.bDropoffRequired) // this flag is also on for allow suspend hence the "else if"
        {
          bDropoffRequired = true;
          if (Info.bDropoffApproved)
          {
            bDropoffApproved = true;
          }
        }

		if (bDropoffRequired && !bDropoffApproved)
		{
			trace(L7, _T("+ra.OnReadyToCollect, Collect Coupons\n"));
			CString csSecInfo;
			csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),
			  ps.GetPSText(LTT_DROPOFFCOUPONS, SCOT_LANGUAGE_PRIMARY), //Description
			  ps.GetPSText(RA_LANEBTT_COLLECCOUPONS, SCOT_LANGUAGE_PRIMARY), //Button
			  ps.GetPSText(RA_FIRSTLINE_COLLECTCOUPONS, SCOT_LANGUAGE_PRIMARY), _T(""), _T("")); //Status line
			ra.OnReadyToCollect(csSecInfo);
			trace(L7, _T("-ra.OnReadyToCollect, Collect Coupons\n"));
		}

        if (Info.bVoidApprovalRequired)
        { // modeled after SMSmVoidedItemsBase::PSButton1
		  //Fix for TAR#125546 LPM110499
		  //  isVoidRatioApprovalRequired() location above the statement
		  //  'lUnapprovedVoidedItems = 0' is important since that
		  //  global is used in isVoidRatioApprovalRequired() and if it is
		  //  set to zero, the call doesn't work as expected.
		  if ( isAnySoldVoidedItems() )  //TAR#129440 LPM010500
		  {
			  lUnapprovedVoidedItems++;
		  }
		  isVoidRatioApprovalRequired();
          lUnapprovedVoidedItems = 0;
		  //SASendSCOTInput( STORE_MODE );   // Tar149058 Send Input to Security when go in and out store mode
		  //SASendSCOTInput(CUSTOMER_MODE);
        }


        //TAR125350 Lane light must be reset by RAP approvals
		if (!isAnyApprovalRequired(false))
        {
            // TAR 450311 - Ensure that we send RCM an update for the lane state window after this intervention is cleared.
            //              This corrects the issue where RCM is clearing the window prematurely or there is no state change
            //              to trigger an update.

            if (!ra.GetRAPDataPendingCount())//SSCOP-2691 - Do not update lane state if there is an immediate intervention
            {
                UpdateStateName(SMStateBase::csLastStateName);//SSCOP-1439
            }

			lRegularYellowNeedingApproval = 0;
            
			// (*) SR898 ====================
            DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                               DM_TRICOLORLIGHT_OFF,
                               DM_TRICOLORLIGHT_NORMALREQUEST);
                               
			// Turn off red light for any non-immediate approvals
			if (!ra.GetRAPDataPendingCount())
			{
				// (*) SR898 ====================
                DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                                   DM_TRICOLORLIGHT_OFF,
                                   DM_TRICOLORLIGHT_NORMALREQUEST);
                                   
                // turn lane light green if nothing else to clear. Ch082001
                
                // (*) SR898 ====================
				DMTurnOffTriColorLight();
                
                if (dm.GetCoinsOrBillsVeryLow() || dm.GetPrinterPaperLow())
				{
					// (*) SR898 ====================
                    DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                                       DM_TRICOLORLIGHT_BLINK_1HZ,
                                       DM_TRICOLORLIGHT_NORMALREQUEST);
				}
				else
				{

					// If inside a transaction, turn on the green transaction light.
					if (fInSignedOn || fInSignedOff || fStateAssist || fSAInTransaction)
					{
			            // (*) SR898 ====================
                        DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                                           DM_TRICOLORLIGHT_ON,
                                           DM_TRICOLORLIGHT_NORMALREQUEST);
					}
					else
					{
						DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                                           DM_TRICOLORLIGHT_OFF,
                                           DM_TRICOLORLIGHT_NORMALREQUEST);
					}
				}
			}
			//TAR213623+ CHU 09/04/02
			else
			{
				//If there is pending, we need to set the lights back to RED
                
				// (*) SR898 ====================
                if(!storeMode())
                {
                    DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                                       DM_TRICOLORLIGHT_ON,
                                       DM_TRICOLORLIGHT_NORMALREQUEST);
                }

			}//TAR213623-

        }
		//end TAR125350  Normal lane light	LPM110499
		//Begin TAR209546 Restore tri-light state if approvals are needed BZ071602
		else
		{
			if(!AreAnyRedApprovalsPending())
			{
				/*dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
								DM_TRICOLORLIGHT_OFF,
								DM_TRICOLORLIGHT_NORMALREQUEST);
				if(isAnyApprovalRequired())
					dm.restoreTriColorLight();*/

				//Fix for TAR390110 Lane Button flashed to yellow clearing security.
				if(!isAnyApprovalRequired())
				{
					// (*) SR898 ====================
                    DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                                       DM_TRICOLORLIGHT_OFF,
                                       DM_TRICOLORLIGHT_NORMALREQUEST);
				}
				else
				{
					dm.restoreTriColorLight();
				} 
				// End of TAR390110 fix
			}

			// sometimes we are seeing red from restore when it is no longer
			// accurate because we have done two restores without a backup in between
			// or processed multiple approvals at once such as when leaving SM
			// so check if any of the red type approvals are still pending
			// if not, turn off the red light
		}
		//End TAR209546
		
        if(bSecurityTrustLevelAlert && !isAnyApprovalRequired())
        {
            trace(L7, _T("SMStateBase::RAParse: Security Trust Level intervention"));

            DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                               DM_TRICOLORLIGHT_ON,
                               DM_TRICOLORLIGHT_NORMALREQUEST);
        }

		//+ tar 350775 -  this code was being transferred here
		/******cm150000: Added In Merged Code******/
		// If the lane is in store mode, we don't tinker with approvals.
		if (fSAInStoreMode)
		{
			trace(L0,_T("SMStateBase::RAParse: Ignoring approval from RAP since SCOT is in store mode!"));
			return STATE_NULL;
		}
		/******cm150000: End Merged Code***********/
		//- tar 350775

		// + Tar 360035
		if (TypeOfApproval == RA_CAT_COLLECTION || Info.bCouponApprovalRequired )
		 {
			 if (!bDropoffRequired && IsUsingCouponSensor() && !Info.bCouponApprovalRequired)
				return OnCouponSensor();
			 else if (ImmediateInterventionStack.GetCount() == 0) //SSCOP-756 / TAR448562  need to return STATE_NULL if on security screen
				return createAnchorState();
		 }
		// - Tar 360035

		// call this second since it should now reflect latest approval info
		/******cm150000: Added In Merged Code******/
		trace(L4, _T("SMStateBase::RAParse: Calling GetAprovalInfo() (again)."));
		/******cm150000: End Merged Code***********/
		ra.GetApprovalInfo(&Info);


		// Now, we need to handle the approval or redo of signature capture
        if ((TypeOfApproval == RA_CAT_SIGCAP) || approveSigCap)
        {
		   g_bgVerifySig = false;
		   if (Info.bSigCapRedoRequired)
			 return RedoSigCap();

		   return ApproveSigCap();
		}
		//Tar 122139  //If in the Device error screen, do change to other screen

		if (csStateName.Find(_T("WaitForApproval")) != -1 ||
			csStateName.Find(_T("SystemMessage")) != -1)
		{
			// TAR 237695 If we still have any immediate intervention, return to Wait For Approval State when coming from that state.
			// else return anchor state
            if (me->raInfo.dwCookie!=-1) //TAR 418787, TAR 425660
            {
			    if(ra.GetRAPDataPendingCount())
				    return ReturnToWaitForApprovalState();
			// end TAR 237695
        
			    return createAnchorState(); //Tar 169222
            }
		}

		//If current state is HelpOnWay and get achorstate is Attract set SCANANDBAG
        //Above comment no longer valid 041703 See RFC 234621
		if((csStateName.Find(_T("HelpOnWay"))) !=-1)
      //&& getAnchorState()==BES_START) //dp185016 tar 286987
        {
			//+jrparro added TAR329234
			//return createAnchorState();	 //jrparro commented
			return PSButton1();
		}   //-jrparro added TAR329234
		else
			return STATE_NULL; // tar 121634
#endif
	  }
    break;

    case WMRA_REQUEST_SUSPEND:
#ifndef _CPPUNIT
            if (!fInSignedOn && !fInSignedOff) //TAR444899
            {
                trace(L6, _T("Ignoring WMRA_REQUEST_SUSPEND because no active transaction"));
                return STATE_NULL;
            }
			if (fSAInStoreMode)
				return STATE_NULL;
	  //+tar 363650
	  if(fOtherPayment)
	  {
		  fOtherPayment = 0;
		  trace(L7, _T("+ra.OnOtherPaymentApproval"));
		  ra.OnOtherPaymentApproval();
		  trace(L7, _T("-ra.OnOtherPaymentApproval"));
	  }

	  if (ra.GetRAPDataPendingCount())
	  {
		  if(m_View == DATANEEDED_PRICEPROMPT)
		  {
			  SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
		  }
		  trace(L2, _T("Calling ra.OnNoRAPDataNeeded()"));
		  ra.OnNoRAPDataNeeded();
	  }
	  //-tar 363650
      bSuspendRequested = true;
      bSuspendApproved = false;
	  trace(L7, _T("+ra.OnRequestSuspend, SuspendReason=%s"), (LPCTSTR) me->raInfo.pData);
	  ra.OnRequestSuspend((LPCTSTR) me->raInfo.pData);
     delete[] me->raInfo.pData;  // Done with me->raInfo, we must delete pData...
	  trace(L7, _T("-ra.OnRequestSuspend"));
      //Tar 112723
      //SCOT already past Finish, so allow suspend immediatly
      if(bFinish)
      {
		trace(L7, _T("+ra.OnReadyToSuspend"));
		ra.OnReadyToSuspend();
		trace(L7, _T("-ra.OnReadyToSuspend"));
      }
	  else
	  {
		  RETURNSTATE(Finish);
	  }
#endif
    break;

	case WMRA_CANCEL_SUSPEND:
#ifndef _CPPUNIT
		/******cm150000: Added In Merged Code******/
		if (fSAInStoreMode)
			return STATE_NULL;
		/******cm150000: End Merged Code***********/
		bSuspendRequested = false;
		bSuspendApproved = false;
		trace(L7, _T("+ra.OnNormalItem"));
		ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+ps.GetPSText(BTT_CANCELSUSPEND, SCOT_LANGUAGE_PRIMARY));
		trace(L7, _T("-ra.OnNormalItem"));
		//ra.OnSuspendApproval();
		if ((!isAnyApprovalRequired(false)) && (nSellState != getAnchorState()))  // If there is no approval needed and the previous state isn't Wait For Approval, go to previous state
		{
			SCOTDO_APPROVAL_INFO Info;
			// Get pre-approval info before we go thru the approval.
			ra.GetApprovalInfo(&Info);
			Info.bAnyApprovalRequired = FALSE;
			return setAndCreateAnchorState(nSellState);
		}
		else
			return createAnchorState();
#endif
	break;


    case WMRA_VALIDATE_OPERATOR:
      {
#ifndef _CPPUNIT
        BOOL bValid;
        CString csData = me->raInfo.operatorID;
        CString csOperatorID;
		int nRawDataLength;
		nRawDataLength = csData.GetLength();

        CString csTempId(me->raInfo.operatorID);
        CString csTempPw(me->raInfo.password);
        if(co.GetfOperationsEncodeOperatorIDPW())
        {
            const CString csPrefixBase64(_T("BASE64:"));
            CBase64EncodeDecode base64;
            csTempId = base64.GetBase64EncodeWithPrefix(csPrefixBase64, csTempId);
            csTempPw = base64.GetBase64EncodeWithPrefix(csPrefixBase64, csTempPw);
        }

		if(csData[0] == _T('!'))  //Means it is Operator Scan Raw data  //FZ20020730
		{
			bValid = isBarcodeValidOperatorPassword(csData.Right(nRawDataLength - 1),&csOperatorID);
			trace(L2, _T("return type for validation is <%d>"), bValid);

		}
		else if(csData[0] == _T('&'))  	  //253241  send SCOTAPP "&"+sScanData in operator id field,
		  //and send SCOTAPP the scan type in password field.
		{
			//Current SCOTAPP only supports EAN13 and code 128 operator barcode
			//Tar 253241 requests RAP sends other scantypes operator barcode to SCOTAPP for future use so that SSF can override.
			//Currect SCOTAPP will always return false for scantypes other than EAN13 and code 128.
			//Someone can override "isBarcodeValidOperatorPassword()" to support other scantypes operator barcode.
	        CString csScanType = me->raInfo.password;
			bValid = isBarcodeValidOperatorPassword(csData.Right(nRawDataLength - 1),&csScanType);
			trace(L2, _T("Scan Data = %s, Scan Type = %s, return type for validation is <%d>"), csData.Right(nRawDataLength - 1),csScanType, bValid);
		}

		else
        {
            bValid = tb.ValidateOperator(csTempId, csTempPw);
            csOperatorID = me->raInfo.operatorID;
        }

	    CString strFmt ;

        // TAR 246246 - Should not log password entered!!!
	    strFmt =_T("tb.ValidateOperator--operatorID:%s; password:xxx") ;
	    traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt, me->raInfo.operatorID) ;

        // tell RA operator validation result
		ra.ValidateOperator(
				me->raInfo.operatorID,
				csOperatorID,
				me->raInfo.password,
				bValid,
				me->raInfo.dwCookie
				);
#endif
      }
    break;

	case WMRA_SEND_CLIENT_MACHINE_NAME:
		{
#ifndef _CPPUNIT
		co.csMachineName = me->raInfo.pData;
      delete[] me->raInfo.pData;  // Finished with me->raInfo -- so delete pData.
		// Saved data to registry for next time when SCOTAPP is restarted.
		HKEY  hKey;
		long  lRC;

		// If the entry does not exist, it will create one.
		//if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\NCR\\SCOT\\CurrentVersion\\SCOTAPP",
		//	0, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,NULL,&hKey, NULL) == ERROR_SUCCESS)
		//{
			// Write a string value to the registry.
		lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP"),
				 0, KEY_SET_VALUE, &hKey);
		lRC = ::RegSetValueEx(hKey,
               _T("ClientName"),
               0,
               REG_SZ,
               (LPBYTE)(LPCTSTR)co.csMachineName,
               co.csMachineName.GetLength() * sizeof(_TCHAR));
		//}
		RegCloseKey(hKey);
#endif
		}
	break;

	//TAR 371932 & tar 371918+
	case WMRA_SEND_INTERVENTION_APPROVED:
		{
#ifndef _CPPUNIT
          trace(L5, _T("WMRA_SEND_INTERVENTION_APPROVED is received"));
		  g_bInterventionApproval = false;
		  
		  if (m_bIsFromLaneClosed )
		  {
             m_bIsFromLaneClosed = false;
             //TAR376328+ if TB thinks the lane is still closed
             if (bLaneClosed)
			 {
			     return STATE_NULL;
			 }		
		     //TAR376328-

			 return setAndCreateAnchorState(BES_START);
		  }
#endif
		}
    break;
	//TAR 371932 & tar 371918-

    default:
      ASSERT(FALSE);
    break;
  }

  trace(L6,_T("-RAParse"));
  return sUser;
}


SMStateBase *SMStateBase::OnRAPUnsolicitedDataReceived(LPCTSTR sData)
   {
    // SSF Should always override this if RAP is configured to send unsolicited
    // messages!
	CString csData = sData;

    trace(L6,_T("+SMStateBase::OnRAPUnsolicitedDataReceived() was called : <%s>."), sData);
	// If a request for remote access comes in and we are not already
	// in remote mode

	//Update context display if in assist menu dynamically - fix FL hung issues when go to
	//VoidItem, UpdateItem, ... from basic assist menu and toggle between two lanes.
	if (co.IsInAssistMenus() && ps.RemoteMode())
	{
		if(ExtractString(sData, _T("operation")) == _T("toggle"))
		{
		   trace(L6, _T("Received toggle notification"));
		   CString csDest = ExtractString(sData, _T("destination"));
		   if ( csDest == _T("assistmode"))
		   {
			   ps.ShowFrame();
		   }
		}
	}

    // Rel 3.2 Requirement 4.5.1 - Single Mode RAP
	if (((csData.Find(_T("cRemote"))    != -1) || (csData.Find(_T("cStoreMode")) != -1) ||
		 (csData.Find(_T("cSingleAM4")) != -1) || (csData.Find(_T("cSingleAM6")) != -1) ||
		 (csData.Find(_T("cSingleAM8")) != -1)) && (!ps.RemoteMode()))
	{
		if(!ra.GetRequestEnableAssistMode() || !g_bAllowEnterRemoteAssistMode)
		{
			trace(L6, _T("SMStateBase::OnRAPUnsolicitedDataReceived() Ignoring Request for Remote AssistMode.")); //tar 217015
		}
	}
#ifndef _CPPUNIT
    else if (csData.Find(_T("cSignonScanData")) != -1)
    {
        int iFind;

        trace(L6, _T("SMStateBase::OnRAPUnsolicitedDataReceived() Processing signon scan data..."));
        // 5 Fields: cSignonScanData~RemoteConnectionName:LastData~Label~Type
        CString csData1 = csData;
        CString csData2 = _T("");
        CString csData3 = _T("");
        CString csData4 = _T("");  // SR93.4
        CString csData5 = _T("");  // SR93.4
		CString csRemoteConnectionName = _T("");
        CString csLabelData = _T(""); // SR93.4
        long lTypeData = 0;  //SR93.4

        // Get First Field (cSignonScanData~RemoteConnectionName)
        iFind=csData1.Find((_TCHAR)_T(':'));
        ASSERT (iFind != -1);

        csData2 = csData1.Mid(iFind+1);
        ASSERT (csData2.GetLength()>0);

        // Isolate the remote connection name from the first field
        csData3 = csData1.Left(iFind);
        iFind = csData3.Find((_TCHAR)DM_FS);
        ASSERT (iFind != -1);
        csRemoteConnectionName = csData3.Mid(iFind+1);

        //+SR93.4
        // Get 4th Field
		iFind = csData2.Find((_TCHAR)DM_FS);
        csData4 = csData2.Mid(iFind+1);
        iFind = csData4.Find((_TCHAR)DM_FS);
        ASSERT (iFind != -1);
        csLabelData = csData4.Left(iFind);

        // Get 5th Field
        csData5 = csData4.Mid(iFind+1);
        iFind = csData5.Find((_TCHAR)DM_FS);
        ASSERT (csData4.GetLength()>0);
        lTypeData = _ttol(csData5);

        if(dm.IsBarcodeTypeEncrypted(lTypeData))
        {
            if (!m_bEncryptedBarcodeAtSignOn)
            {
                return STATE_NULL;
            }
        }

        //-SR93.4

        if (isBarcodeValidOperatorPassword(csData2))
        {
            trace(L6, _T("-SMStateBase::OnRAPUnsolicitedDataReceived() - Valid Operator Barcode - returning STATE_NULL."));
            //+SR819
			if(TBValidateHeadCashier(csOperator, csPassword) == 1) 
			{
				trace(L6, _T("Valid Head Cashier Operator Barcode"));
				ra.SignOn(csRemoteConnectionName, csOperator, csPassword, true, true);   
            }
			else
			{
				ra.SignOn(csRemoteConnectionName, csOperator, csPassword, true, false);			
			}
			//-SR819
            return STATE_NULL;
        }
        else
        {
            trace(L6, _T("-SMStateBase::OnRAPUnsolicitedDataReceived() - Invalid Operator Barcode - returning STATE_NULL."));
            ra.SignOn(csRemoteConnectionName, csOperator, csPassword, false, false);   //SR819
            return STATE_NULL;
        }
    }
    else if (csData.Find(_T("cScanData")) != -1)
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
      if (cDMLastData) delete [] cDMLastData; //SSCOP-4165
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

	  //TAR235212+ CHU 042303
	  //If there are queued up messages from RAP and ScotApp is no longer at Remote mode,
	  //throw these message away
	  if (!ps.RemoteMode())
      {
		 trace(L0, _T("Ignore SCAN details from RAP since we are not in Remote mode."));
         trace(L6,_T("-SMStateBase::OnRAPUnsolicitedDataReceived - Not In Remote Mode -- returning STATE_NULL."));
         return STATE_NULL;

	  }
	  //TAR235212-


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

	  // Fix for TAR:208233
      // if not in tender mode and
	  // an item is still in progress
	  // and inside a trx:
      // do not allow further scans
      if ((fItemIsWaiting || fSAInItemSale) &&
		  !bEnterTenderState &&
		  (fInSignedOn || fInSignedOff) &&
		  (!dm.IsBarcodeTypeEncrypted(csDMLastBarType))
		  )
      {
		 trace(L0, _T("Ignore SCAN details:fItemIsWaiting:%d, fSAInItemSale:%d, bEnterTenderState:%d, fInSignedOn:%d, fInSignedOff:%d"),
				fItemIsWaiting, fSAInItemSale, bEnterTenderState, fInSignedOn, fInSignedOff);
         trace(L6,_T("-SMStateBase::OnRAPUnsolicitedDataReceived - Item Waiting - returning STATE_NULL."));
         return STATE_NULL;
      }

      // +SR93.4
      if (dm.IsBarcodeTypeEncrypted(csDMLastBarType))
      {
          return DMScannerEncrypted();
      }
      else
      {
          ConvertOposAsciiToBinary(csDMLastBarLabel, OPOS_BC_DECIMAL);
          ConvertOposAsciiToBinary(csDMLastBarCode, OPOS_BC_DECIMAL);
          return DMScanner();         // barcode
      }
      // -SR93.4

   }
   else if ((csData.Find(_T("CancelRAPInitiateTransaction")) != -1) && (!ps.RemoteMode()) // tar 330582
	   && g_bAllowEnterRemoteAssistMode && !fSAInTransaction)
   {

		trace(L6,_T("ra.OnNoRAPDataNeeded()"));
		ra.OnNoRAPDataNeeded();
		UpdateStateName(ps.GetPSText(LTT_ATTRACT, SCOT_LANGUAGE_PRIMARY)); //TAR408894
        return STATE_NULL;
   }
   // RFC 228102 Initiate transactions that will require hand scanning at the RAP
   else if ((csData.Find(_T("RAPInitiateTransaction")) != -1) && (!ps.RemoteMode())
	   && g_bAllowEnterRemoteAssistMode && !fSAInTransaction)
   {   // RAP cashier is asking whether the RFC logic has been turned on
	   CString csRAPInitiateTrx = co.getRapInitiateTransaction();
	   // Tar 240789 only three values allowed, N, StartTransaction, and StartTransactionAM
	   // so if not the last two, either it's N or bad value
	   // in which case we default to N
	   if((csRAPInitiateTrx != _T("StartTransactionAM")) && (csRAPInitiateTrx != _T("StartTransaction")))
	   {  // option turned off, so tell RAP to process into AM outside transaction
		  ra.OnAddOtherEvent(_T("COMMAND=RAPInitiateTransactionAssistMode;"));
		  if(csRAPInitiateTrx == _T("N"))
             trace(L4,_T("RAPInitiateTransaction option=N.  Transaction WILL NOT BE STARTED."));
		  else
             trace(L4,_T("invalid RAPInitiateTransaction option = %s"),csRAPInitiateTrx);

		  ra.OnNoRAPDataNeeded();	// TAR 356716
	   }
	   else
	   {  // option turned on, will now send screen info
		  ra.OnAddOtherEvent(_T("COMMAND=RapInitiateTransactionDataNeeded;"));

		  //tar 255330 -- merged the fix in 3.0.2 here
          //Removed dm.settricolorlight because this is not a red light condition

		  // send a DataNeeded Approval
		  cDataEntry.FromConfigData(co.csDataEntryINIFile, csRAPInitiateTrx);
		  if (csRAPInitiateTrx == _T("StartTransaction"))
				m_View = DATANEEDED_STARTTRANSACTION;
		  else
				m_View = DATANEEDED_STARTTRANSACTIONAM;
		  CString csSecInfo;
		  csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONWITH_DNTYPEINFO, SCOT_LANGUAGE_PRIMARY),
			  _T(" "), //Description
			  _T(" "), //Button
			  _T(""),_T(""),_T(""), m_View); //Status line
		  ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
          trace(L7,_T("ra.OnRAPDataNeeded, Start Transaction options"));
	   }
       return STATE_NULL;
   }
   // RFC 228102 Initiate transactions that will require hand scanning at the RAP
   else if ((csData.Find(KEY_EV_TYPE_ADD_OTHER_EVENT_RAPINITIATELANECLOSE) != -1) && !fSAInTransaction)
   {   // RAP cashier is asking whether the RFC logic has been turned on
        trace(L7,_T("RAPInitiateTransaction option=N"));
        CString csCurrentStateName;
        try
        {
            csCurrentStateName = this->GetRuntimeClass()->m_lpszClassName;
        }
        catch(...)
        {
            trace(L1, _T("this->GetRuntimeClass() threw Exception!"));
        };
        if(csCurrentStateName.Find(_T("SystemMessage")) != -1)//tar 253778
        {
            trace(L6, _T("SCOT is at a SystemMessage state. Cannot go to lane closed. Returning STATE_NULL."));
            return STATE_NULL;
        }
		trace(L6, _T("SCOT is not in the middle of a transaction and RAP initiated lane closed."));
        //+RFC 986
        if (co.fStateStartupWaitForTB)
        {
            setAnchorState(BES_START);
            TBLaneClosed(true);
            CREATE_AND_DISPENSE(InProgress)(storeMode(), helpMode(), co.getTimeOutItemTransactionResult());
        }
        else
        {
            ms_bLaneClosedFromRapFlag = true;  //TAR 450342 : PLEASE INCLUDE THIS CODE IN CASE OF FUNCTION OVERRIDE 
            RETURNSTATE(LaneClosed)
        }
        //-RFC 986
   }
#endif //_CPPUNIT
   else if ((csData.Find(_T("GenericDelayedAssistModeStatus")) != -1))
   {   
       if(fSAInItemSale || !ra.GetRequestEnableAssistMode())
       {
           ra.OnAssistModeNotAvailable();
       }
       else
       {
           ra.OnAssistModeAvailable();
       }
       return STATE_NULL;
   }
   else if ((csData.Find(_T("GenericDelayedStart")) != -1))
   {   
       return HandleRemoteGenericDelayed(csData);
   }
   else if (csData.Find(_T("operation=DataNeededResponse")) != -1)
   {
       return HandleDataRequest(csData);
   }
   trace(L4, _T("-SMStateBase::OnRAPUnsolicitedDataReceived() Message Unhandled by SMStateBase - returning STATE_NULL."));
   return STATE_NULL;
   }

#ifndef _CPPUNIT
SMStateBase *SMStateBase::OnRAPDataReceived(LPCTSTR sData)
   {
   /*// We should never receive RAP data at a state without a handler!

   trace(L6,_T("SMStateBase::OnRAPDataReceived() was called.  RAP Data was not expected in this state.  Possible SSF or RAP programming error,"));

   ASSERT(FALSE);
   ra.OnNoRAPDataNeeded();
   return STATE_NULL;*/
   int iPos;

   m_sInput = sData;
   trace(L2, _T("+SMStateBase::OnRAPDataReceived"));
   m_sButton.Empty();
   iPos = m_sInput.Find(_T(";"));

   // This data is bogus -- the data from RAP must contain a ";" to
   // separate button data from input data.
   ASSERT(iPos >= 0);

   if (iPos > 0)
      {
      m_sButton = m_sInput.Mid(0, iPos);
      m_sInput = m_sInput.Mid(iPos+1);
      }
   else if (iPos == 0)
      {
      m_sButton.Empty();
      m_sInput = m_sInput.Mid(1);
      }
   else
   {
	  trace(L2, _T("-SMStateBase::OnRAPDataReceived"));
	  bEASReEnableScanner = true;	// TAR199297 want to allow the scanner to be re-enabled by the next state LPM032002
      return createAnchorState();
   }
	/*
   // start of TAR 265143
   CString csCurrentStateName = this->GetRuntimeClass()->m_lpszClassName;
   if( csCurrentStateName.Find(_T("RAPDataNeeded")) != -1)
   {
     g_bFromRAP = true;
   }
   // end of TAR 265143  */

   g_bFromRAP = true;
   trace(L2, _T("-SMStateBase::OnRAPDataReceived"));
   //USSF START
   USSFHOOK(_T("HandleRAPData"), nextState = HandleRAPData(), return nextState);
   //return HandleRAPData();
   //USSF END
   }


SMStateBase *SMStateBase::HandleRAPData()
   {
   SMStateBase *pNextState = STATE_NULL;

   switch (m_View)
      {
	  case DATANEEDED_STARTTRAININGTRANS: // cs185088
        pNextState = HandleStartTrainingTrans();
        break;
      case DATANEEDED_CONFIRMSUSPEND:
		  pNextState = HandleConfirmSuspend();
         break;
      case DATANEEDED_PRICEPROMPT:
         pNextState = HandlePricePrompt();
         break;
      case DATANEEDED_MOREDATA:
		 //USSF START
		 USSFHOOK(_T("HandleDataNeeded"), pNextState = HandleDataNeeded(), ;);
		 //pNextState = HandleDataNeeded();
		 //USSF END
         break;
      //+SR678
	  case DATANEEDED_MAXCOUPONLIMIT:			
		pNextState = HandleMaxCouponLimit();
		break;
	  case DATANEEDED_MAXCOUPONLIMITFINALIZATION:
	    pNextState = HandleMaxCouponLimitFinalization();
		break;
	  //-SR678
	  // + TAR 352409
	  case DATANEEDED_SUSPENSION_DECLINE:
		  {
			bSuspendFailure = false;
			SAClearSecurityCondition();
			pNextState = STATE_RESUME; //sscop-144
		  }	break;	  
	  // - TAR 352409
	  //+SR879
	  case DATANEEDED_INVALIDBARCODE:
	    pNextState = HandleInvalidBarcode();
		break;
	  //-SR879
      case DATANEEDED_GO_TO_LANE: //SR831
	  case DATANEEDED_MISMATCHWEIGHT:
	  case DATANEEDED_UNEXPECTEDINCREASE:
	  case DATANEEDED_DONTBAGTHRESHOLDEXCEEDED:
      case DATANEEDED_NEWITEMENTRY:
	  case DATANEEDED_BAGITEMTIMEREXPIRED:
      case DATANEEDED_TAKECHANGETIMEREXPIRED:
      case DATANEEDED_DONEREMOVINGTHRESHOLDEXCEEDED:
      case DATANEEDED_BAGBOXNOTVALID://SR085
	  case DATANEEDED_WALKAWAYTIMEREXPIRED:
	  case DATANEEDED_APPROVEVOIDEDITEM:
	  case DATANEEDED_BAGBOXTHRESHOLDEXCEEDED:
	  case DATANEEDED_UNEXPECTEDDECREASE:
	  case DATANEEDED_DEVICEERROR:
	  case DATANEEDED_WLDBFAILURE:
	  case DATANEEDED_CLOSELANE: //awk.2.6.2 TAR192509
	  case DATANEEDED_VERIFYSIGNATURE: //awk.3.26.2 TAR199004
	  case DATANEEDED_BAGAREABACKUP:
      case DATANEEDED_THANKYOUEXPIRED: //RFC 209702
      case DATANEEDED_INSERTCOUPONTIMEREXPIRED: //RFC 219178
	  case DATANEEDED_COLLECTGIFTCARD:
	  case DATANEEDED_HOSTOFFLINE:
	  // RFQ 7477
	  case DATANEEDED_CHECKONLANE:  //RFC 356020
	  case DATANEEDED_CHECKONLANERAP:  //RFC 356020
	  // RFQ 7477
	  case DATANEEDED_MEIJERCARDVERIFICATION:		// CRD 526693
		  if(m_sButton == _T("cGoBack"))
			pNextState = STATE_NULL;
		  else
		  {
			// re-format if statement to remove similar lines into one location LPM031302
			lSARedNeedingApproval = false;   // TAR 197015 Clear alert flag.
			trace(L2, _T("bEASReEnableScanner is set to true."));
			bEASReEnableScanner = true;
			g_bApproveFromRAP = true;

			if(m_sButton == _T("cApprove&Update"))
				pNextState = OnRAPApproveAndUpdateBtn();
			else if(m_sButton == _T("cApproveNoUpdate"))
				pNextState = OnRAPApproveNoUpdateBtn();
			else if(m_sButton == _T("cLightItem"))
				pNextState = OnRAPLightItemBtn();
			else if(m_sButton == _T("cItemNotBagged"))
				pNextState = OnRAPItemNotBaggedBtn();
			else if(m_sButton == _T("cDone"))
			{
				g_bgVerifySig = false;
				SAClearSecurityCondition();			//tar 346725
				pNextState = ApproveSigCap();
			}
			else
			{
				pNextState = OnRAPClearSecurityBtn(_T("RAP"));
			}
		  }
          break;
	  case DATANEEDED_CANCELLTRANSACTION:
		  pNextState = HandleCancelTrasaction(m_sButton);
          break;
	  case DATANEEDED_UNDEACTIVATEDITEM:
		  if(m_sButton == _T("cGoBack"))
		  {
              pNextState = STATE_NULL;
		  }
		  else
		  {
              pNextState = OnRAPClearSecurityBtn(_T("RAP"));
		  }
          break;
      case DATANEEDED_STARTTRANSACTION:		// RFC 228102
      case DATANEEDED_STARTTRANSACTIONAM:		// RFC 228102
         pNextState = HandleStartTransaction();
         break;
      //+dp185016
      case DATANEEDED_OUTOFSYNC_STOP:
      case DATANEEDED_OUTOFSYNC_ASSIST:
        pNextState = STATE_NULL;
        break;
        //-dp185016
	  case DATANEEDED_MULTIPACKITEM:	   // + TAR 256154
		  {
	          trace(L6, _T("SMStateBase::HandleRAPData() - DATANEEDED_MULTIPACKITEM view "));
			  setAnchorState(BES_SCANANDBAG);
			  SAClearSecurityCondition();
			  bEASReEnableScanner = true;
			  CKeyValueParmList parmList;
			  // need to clear mismatch with no WLDB update
			  if( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_MISMATCHED )
			  {
				CKeyValue<long>::Put(parmList, KEY_OSE_EXCEPTION_ID, secEventParms.m_lExceptionId);
				CKeyValue<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_AUTHORIZATION, KEY_EXCEPTION_AUTHORIZATION_RAP);
				CKeyValue<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_OPERATION, KEY_EXCEPTION_OPERATION_CLEAR);
				_bstr_t bstrParmList((const _TCHAR *)parmList);
				SecMgr.OverrideException(lSecMgrCookie, bstrParmList);
				parmList.Clear();
			  }
		  }
          // Fall through to assist mode button check in next case
      case DATANEEDED_ENDORSEDOCUMENT:
          {
			  if (m_sButton == _T("cAssistMode"))
			  {
	 	          trace(L6, _T("SMStateBase::HandleRAPData() - pressed AssistMode button"));
				  CString csCurrentStateName = this->GetRuntimeClass()->m_lpszClassName;
				  if (csCurrentStateName.Find( _T("SMSmDataEntry") ) != -1)
				  {
		 	          trace(L6, _T("SMStateBase::HandleRAPData() - RETURNSTATE(SmAssistMode) "));
					  RETURNSTATE(SmAssistMode);
				  }
			  }

			  pNextState = STATE_NULL;

		  }
          break;
      case DATANEEDED_NOVIEW:{ //TAR301557
         trace(L2, _T("SMStateBase::HandleRAPData() m_View == DATANEEDED_NOVIEW, returning STATE_NULL"));
		 //+sscop-340
		 CString csCurrentStateName = this->GetRuntimeClass()->m_lpszClassName;
		 if (csCurrentStateName.Find( _T("SMAttract") ) != -1){
				UpdateStateName(ps.GetPSText(LTT_ATTRACT, SCOT_LANGUAGE_PRIMARY));
		 }//-
         pNextState = STATE_NULL;
		} break;
      default:
         // This is an unknown View.  All views handled by this state
         // must be present in this switch.
         pNextState = createAnchorState();
		 bEASReEnableScanner = true;	// TAR199297 just in case, want to allow the scanner to be re-enabled by the next state LPM032002
         ASSERT(FALSE);
      }
   g_bFromRAP = false; // TAR 265143
   return pNextState;
   }

SMStateBase *SMStateBase::HandleApproveVoidedItem()
{
	return STATE_NULL;
}

SMStateBase *SMStateBase::HandleConfirmSuspend()
{
	return STATE_NULL;
}

SMStateBase *SMStateBase::HandlePricePrompt()
{
	return STATE_NULL;
}

//+SR678
SMStateBase *SMStateBase::HandleMaxCouponLimit()
{
	return STATE_NULL;
}

SMStateBase *SMStateBase::HandleMaxCouponLimitFinalization()
{
	return STATE_NULL;
}
//-SR678

//+SR879
SMStateBase *SMStateBase::HandleInvalidBarcode()
{
	return STATE_NULL;
}
//-SR879
#endif //_CPPUNIT
SMStateBase *SMStateBase::HandleStartTrainingTrans() // cs185088
{
    SAClearSecurityCondition();  //move this call outside so both Yes and No button pressed still clear intervention.
    if (m_sButton == _T("cYes")) 
    {
#ifndef _CPPUNIT
        //TAR 417255 +
        TrainingModeObj.SetTransactionApproved(true);
        SMRAPDataNeeded::nlaststate = BES_SCANANDBAG;
#endif //_CPPUNIT

        if( getAnchorState() == BES_PUTBAGONSCALE  ) 
        {
            CKEYVALUEPARMLIST parmList;
            CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_CUST_HAS_BAG);
            _bstr_t bstrParmList((const _TCHAR *)parmList);
            BSTR bstrResult;
            bstrResult = NULL;
            SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
            SysFreeString(bstrResult);
        }

        if (co.GetfStateDualLanguage())
        {
            //+-SSCOADK-6208
            return TBStart(co.Getlanguage(m_customerLanguage).szCode);
        }
        else
        {
            return TBStart();
        }
    }
    else
    {
        
        SASendSCOTInput(TRANSACTION_COMPLETE);
        m_customerLanguage = SCOT_LANGUAGE_PRIMARY; //sscoadk-6835: done with training mode; setting it to PRIMARY
    
#ifndef _CPPUNIT
        ResetAllTransactionValues();
#endif //_CPPUNIT
        return setAndCreateAnchorState(BES_START);
    }
    //TAR 417255-
    
}

#ifndef _CPPUNIT
// This Method is implemented based on the methods
// SMStateBase  *SMSmAbortBase::PSButton1(void) // yes
// and
// SMStateBase  *SMSmAbortBase::PSButton2(void) // no
// of file SMSmAbortBase.CPP
SMStateBase *SMStateBase::HandleCancelTrasaction(CString csBttID)
{
	SMStateBase *pretState = STATE_NULL;
    if(m_sButton == _T("cYes"))
	{
		//TAR 200589 start

        ra.OnNoRAPDataNeeded();
		//TAR 200589 end
		if ( isAnySoldVoidedItems() )
		{
	        ClearStateStackEOT();

		    CKeyValueParmList parmList;
		    CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_TC_OPERATION_VOID_BEGIN);
		    _bstr_t bstrParmList((const _TCHAR *)parmList);
		    SecMgr.TransactionControl(lSecMgrCookie, bstrParmList);

			// end TAR 237695
	        pretState = TBCancelTransaction(true);       // wait for *VOID TRANSACTION*
		}
		else
		{
			//Tar 124345
			pretState = TBFinish();  //Tar 126024
		}
	}
	else if(m_sButton == _T("cNo"))
	{
    /*//+dp185016 tar 280966
    //the help stack of mpprocedures is cleared when next state is not a help state.
    //in the flow SMCustomerState -> SMHelpState->then did approval on rap(state in rap will
    //that be of storemode), the help stack should be cleared, and we base our next state to
    //the saved anchor state, not the help stack.
    */
    SMStateBase::m_bClearStack = true;
    /**///-dp185016
		SASendSCOTInput( TRANSACTION_CONTINUE );
        //TAR#125989	LPM111199 the global must be set to false when the error is canceled
        bApplicationErrorApprovalRequired = FALSE;
		// TAR 197203
		//Clear Approval
	    trace(L6, _T("ra.OnNoRAPDataNeeded()"));
        ra.OnNoRAPDataNeeded();
		
		//clear trilight tar 389685
		ClearSecurityViolationLight();

        SMStateBase *sReturnState = CkStateStackAndReturn();
        if (sReturnState != STATE_NULL)
		{
	        pretState = sReturnState;
		}
		else
		{
			//pretState = setAndCreateAnchorState(BES_SCANANDBAG);
			pretState = STATE_RETURN;//TAR 278404
		}
	}
	ra.RequestEnableAssistMode(true);
	return pretState;
}

#endif //_CPPUNIT
//TAR 237695 This method uses to return Wait For Approval state.
///////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::ReturnToWaitForApprovalState()
{
    // + TAR 418143
    bool bWaitForApprovalVoid = false;
    CString csStateName = mp.sCurr->GetRuntimeClass()->m_lpszClassName;
    if(csStateName == _T("WaitForApproval"))
    {
        bWaitForApprovalVoid = true;        
    }    
    CREATE_AND_DISPENSE(WaitForApproval)(false, bWaitForApprovalVoid);
    //CREATE_AND_DISPENSE(WaitForApproval)(false,true);
    // - TAR 418143
}

#ifndef _CPPUNIT
///////////////////////////////////////////////////////////////////////////////
// RFC 228102 Initiate transactions that will require hand scanning at the RAP
// this routine called when RAP cashier selects from Operator Assisted screen
// options that can remotely control transaction
///////////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::HandleStartTransaction()
   {
   trace(L6,_T("+SMStateRABase::HandleStartTransaction()"));
   dm.restoreTriColorLight();	// tar 240782
   if (m_sButton == _T("cAMOutsideTrx"))		// Assist Mode button
      {
	    ra.OnAddOtherEvent(_T("COMMAND=RapOperatorAssistMode;"));
		trace(L6,_T("ra.OnNoRAPDataNeeded()"));
		ra.OnNoRAPDataNeeded();
		CString csStateName = mp.sCurr->GetRuntimeClass()->m_lpszClassName;
		if(csStateName == _T("SMSecUnExpectedIncrease"))	// not quiet mode but security issue
		{
           return OnRAPClearSecurityBtn(_T("RAP"));
		}
      }
   else if (m_sButton == _T("cGoBack"))			// Go Back button
      {
		trace(L6,_T("ra.OnNoRAPDataNeeded()"));
		ra.OnNoRAPDataNeeded();
      }
   else if (m_sButton == _T("cStartTransaction")) // Start Transaction button
      {
        ra.OnAddOtherEvent(_T("COMMAND=StartTransaction;"));

		if(!ra.GetRequestEnableAssistMode())//tar 253585
        {
		  trace(L6, _T("ra.OnNoRAPDataNeeded() because this State cannot allow StartTransaction"));
		  ra.OnNoRAPDataNeeded();
          return STATE_NULL;
        }
		setAnchorState(BES_SCANANDBAG);

		// enable the sensormatic EAS deactivator while in a transaction
		dm.EASDeactivatorEnable( EASEnable );
		SASendSCOTInput( ITEMIZATION );
		bQuietNeedSecApproval = false; //389664

		trace(L6,_T("SMStateBase::OnRAPUnsolicitedDataReceived() Starting Transaction."));
        ra.OnAddOtherEvent(_T("COMMAND=AssistModeRefreshLaneStatus;"));
		trace(L6,_T("ra.OnNoRAPDataNeeded()"));
		ra.OnNoRAPDataNeeded();
		if (co.GetfStateDualLanguage())
		{
			m_customerLanguage = SCOT_LANGUAGE_PRIMARY;
			ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
			co.csCMCustomerMessage = ps.GetCustomerMessageDefault(); //SSCOB-1860
			CustomerMessage = co.csCMCustomerMessage;
			ps.SetTareListToCurrentLanguage();
			if(gpSCOTAuxDisp)
				gpSCOTAuxDisp->SetLanguage(SCOT_LANGUAGE_PRIMARY);
			return TBStart(co.Getlanguage(SCOT_LANGUAGE_PRIMARY).szCode);
		}
		else
		{
			return TBStart();
		}
      }
   else if (m_sButton == _T("cStartTransactionAM"))	// Start Transaction and Enter Assist Mode button
      {
        if(!ra.GetRequestEnableAssistMode())//253585
        {
          ra.OnAddOtherEvent(_T("COMMAND=StartTransaction;"));
		  trace(L6, _T("ra.OnNoRAPDataNeeded() because this State cannot allow StartTransaction"));
		  ra.OnNoRAPDataNeeded();
          return STATE_NULL;
        }
		// remote approval BASICALLY TBSTART
		// enable the sensormatic EAS deactivator while in a transaction
		dm.EASDeactivatorEnable( EASEnable );

		trace(L6,_T("SMStateBase::OnRAPUnsolicitedDataReceived() Starting Transaction and remote AM."));
        ra.OnAddOtherEvent(_T("COMMAND=AssistModeRefreshLaneStatus;"));
		if (co.GetfStateDualLanguage())
		{
            if(ps.GetLanguage() == SCOT_LANGUAGE_PRIMARY) //SSCOP-676 Migration
            {
                m_customerLanguage = SCOT_LANGUAGE_PRIMARY;
                ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
                co.csCMCustomerMessage = ps.GetCustomerMessageDefault(); //SSCOB-1860
                CustomerMessage = co.csCMCustomerMessage;
                ps.SetTareListToCurrentLanguage();
                if(gpSCOTAuxDisp)
                    gpSCOTAuxDisp->SetLanguage(SCOT_LANGUAGE_PRIMARY);
            }
		}
		// check for security issues
		if(bQuietNeedSecApproval)
		{
			trace(L6,_T("Starting transaction but not sending remote AM request to RAP because of security issue\n"));
		    SASendSCOTInput( ITEMIZATION );	//SM will fire an exception
			ra.OnAddOtherEvent(_T("COMMAND=StartTransaction;"));
			trace(L6,_T("ra.OnNoRAPDataNeeded()"));
			ra.OnNoRAPDataNeeded();
			bQuietNeedSecApproval = false; //389664
			return TBStart(co.Getlanguage(SCOT_LANGUAGE_PRIMARY).szCode);
		}
		else
		{   // SMSmAssistModeBase::Initialize will handle rest of start transaction logic
            fStartTrxInAM = true;
	        ra.OnAddOtherEvent(_T("COMMAND=StartTransactionEnterAM;"));
			trace(L6,_T("ra.OnNoRAPDataNeeded()"));
			ra.OnNoRAPDataNeeded();
			//SSCOB-257+
		    trace(L6,_T("-SMStateRABase::HandleStartTransaction()"));
            CREATE_AND_DISPENSE(InProgress)(storeMode(), helpMode(), co.getTimeOutItemTransactionResult());
			//SSCOB-257-

        }
   }
   //+ TAR 332302
   else if (m_sButton == "cAssistMode")
   {
       trace(L6,_T("SMStateBase::OnRAPUnsolicitedDataReceived() Starting remote AM."));
	   fStartTrxInAM = true; //SSCOP-338
       ra.OnNoRAPDataNeeded();
   }
   //- TAR 332302
   else
      {
      ASSERT(FALSE);
      trace(L0,_T("SMStateRABase::HandleStartTransaction() -- Button data was not valid."));
      }

   trace(L6,_T("-SMStateRABase::HandleStartTransaction()"));
   return STATE_NULL;
   }

void SMStateBase::EnableHandheldAssistMode(bool bEnable)
{
    if (bEnable && co.fStateAssistMenuFromRAPAllowed)
    {
        ra.RequestEnableHandheldAssistMode(true);
    }
    else
    {
        ra.RequestEnableHandheldAssistMode(false);
    }
    ra.RequestEnableAssistMode(ra.GetRequestEnableAssistMode());
}

void SMStateBase::HandleHopperError(CString csBttID)
{
	if(csBttID == _T("cFixLater;"))
	{
		nHopperTrxCount=0;
		lUnapprovedHopperSubstitution = 0;
		lRegularYellowNeedingApproval--;
        return ;
	}
	else if (csBttID == _T("cFixedHopper;"))
	{
		long deviceClass = DMCLASS_CASHCHANGER;
		long rc = DMCheckHealth(DMCLASS_CASHCHANGER); // issue check health
		if (rc == OPOS_SUCCESS)
		{
			nHopperTrxCount = 0;
			lUnapprovedHopperSubstitution = 0;
			lRegularYellowNeedingApproval--;
		}
		return ;
	}
	return;

}

void SMStateBase::ClearHopperSubstitution(void)
{
		nHopperTrxCount=0;
		lUnapprovedHopperSubstitution = 0;
		lRegularYellowNeedingApproval--;
		trace(L7, _T("+ra.OnHopperSubstitutionApproval"));
		ra.OnHopperSubstitutionApproval();
		trace(L7, _T("-ra.OnHopperSubstitutionApproval"));
		//bInApprovalNeeded = false;
		if (!isAnyApprovalRequired(false))
        {
			lRegularYellowNeedingApproval = 0;
            
			// (*) SR898 ====================
            DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                               DM_TRICOLORLIGHT_OFF,
                               DM_TRICOLORLIGHT_NORMALREQUEST);
                               
			// Turn off red light for any non-immediate approvals
			if (!ra.GetRAPDataPendingCount())
			{
				// (*) SR898 ====================
                DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                                   DM_TRICOLORLIGHT_OFF,
                                   DM_TRICOLORLIGHT_NORMALREQUEST);
                                   
                // turn lane light green if nothing else to clear. Ch082001
                
                // (*) SR898 ====================
				DMTurnOffTriColorLight();
                
                if (dm.GetCoinsOrBillsVeryLow() || dm.GetPrinterPaperLow())
				{
					// (*) SR898 ====================
                    DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                                       DM_TRICOLORLIGHT_BLINK_1HZ,
                                       DM_TRICOLORLIGHT_NORMALREQUEST);
				}
				else
				{
					// If inside a transaction, turn on the green transaction light.
					if (fInSignedOn || fInSignedOff || fStateAssist || fSAInTransaction)
					{
			            // (*) SR898 ====================
                        DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                                           DM_TRICOLORLIGHT_ON,
                                           DM_TRICOLORLIGHT_NORMALREQUEST);
					}
					else
					{
						DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                                           DM_TRICOLORLIGHT_OFF,
                                           DM_TRICOLORLIGHT_NORMALREQUEST);
					}
				}
			}
			//TAR213623+ CHU 09/04/02
			else
			{
				//If there is pending, we need to set the lights back to RED
                
				// (*) SR898 ====================
                DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                                   DM_TRICOLORLIGHT_ON,
                                   DM_TRICOLORLIGHT_NORMALREQUEST);

			}//TAR213623-

        }
		//end TAR125350  Normal lane light	LPM110499
		//Begin TAR209546 Restore tri-light state if approvals are needed BZ071602
		else
		{
			if(!AreAnyRedApprovalsPending())
			{
				// (*) SR898 ====================
                DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                                   DM_TRICOLORLIGHT_OFF,
                                   DM_TRICOLORLIGHT_NORMALREQUEST);
                                   
				if(isAnyApprovalRequired())
					dm.restoreTriColorLight();
			}

			// sometimes we are seeing red from restore when it is no longer
			// accurate because we have done two restores without a backup in between
			// or processed multiple approvals at once such as when leaving SM
			// so check if any of the red type approvals are still pending
			// if not, turn off the red light
		}


}

#endif

CString SMStateBase::ExtractString(CString strData, CString sStringName)
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
            bStatus = true;     //Allow final pair to be unterminated - SR815
            iFind = sString.Find(_T(";"));
            if (iFind > 0)
            {
                sString = sString.Left(iFind);

            }
        }
        if (!bStatus)
            sString.Empty();
    }
    return sString;
}


SMStateBase *SMStateBase::HandleRemoteGenericDelayed(CString csData)
{
    trace(L6,_T("HandleRemoteGenericDelayed"));

    CString csID = ExtractString(csData, _T("ID"));
    if (!csID.IsEmpty())
    {

      for (int i=0; i<m_GenericDelayedInterventions.GetSize() && !m_GenericDelayedInterventionStatus; i++)
      {
          CQueuedActionGenericDelayedIntervention *pDelayedIntervention = (CQueuedActionGenericDelayedIntervention* const) m_GenericDelayedInterventions.Peek(i);

          if (csID.Compare(pDelayedIntervention->GetActionID()) == 0)
          {
             pDelayedIntervention = (CQueuedActionGenericDelayedIntervention* const) m_GenericDelayedInterventions.Get(i);
             m_GenericDelayedInterventions.PushBack(pDelayedIntervention);
             m_GenericDelayedInterventionStatus = GENERIC_DELAYED_STARTING;
             g_bFromRAP = true;
             break;
          }
      }
    }
    
    return STATE_NULL;
    //-RFC 986
}

