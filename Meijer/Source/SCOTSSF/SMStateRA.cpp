//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateSA.CPP
//
// TITLE: SA SMState functions, implement all SMState::SA* functions
//
//
// AUTHOR:  DcL-
//
// CRD124746 Work Request:Sprint 10.3 Name:Aparna Tunuguntla Date: March 19,2018
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include "Common.h"
#include "SMState.h"
#include "transbrokerssf.h"
#include "SMInProgress.h"
#include "SMSmAssistMode.h"
#include "SMSuspend.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmStateRA")

long SMState::RAInitialize(void)
{
    return SMStateBase::RAInitialize();
}

long SMState::RAUnInitialize(void)
{
    return SMStateBase::RAUnInitialize();
}

SMStateBase * SMState::RedoSigCap()
{
    return SMStateBase::RedoSigCap();
}

SMStateBase * SMState::ApproveSigCap()
{
    return SMStateBase::ApproveSigCap();
}

SMStateBase * SMState::RAParse(MessageElement *me)
{
    switch (me->raInfo.msg) 
    {
    case WMRA_REQUEST_APPROVAL: 
        {
            
            // if Restricted item and validate or No date needed 
            //  send to POS
            CString sData = me->raInfo.pData;
            
            DWORD TypeOfApproval;
            TypeOfApproval = ra.GetCurrentApprovalCode();
            
            // This is causing calls double remoteapproval events 
            // to be triggered when SSF RAParse completes and the 
            // base version is then executed.
            //ra.OnRemoteApproval(
            //        me->raInfo.handle,
            //        me->raInfo.operatorID, 
            //        me->raInfo.password,
            //        me->raInfo.approvalCode,
            //        (LPCTSTR)sData,
            //        me->raInfo.dwCookie
            //        );

		    if (TypeOfApproval == RA_CAT_AGERESTRICT && lUnapprovedRestrictedItems != 0)  //393655
            {
                bool bDelayedAgeRestriction = SendTBCommand(TBCMD_ACS_DELAYED_AGE_RESTRICTION);
                    
                //tar306723
                bAlreadyPlayedWelcome = true;
                
                CString csRAPdob = sData;   //get age entered at RAP station
                CString csoperatorID = me->raInfo.operatorID;
                CString cspassword = me->raInfo.password;
                if (csRAPdob == _T("1;")) //check to see if dob not needed key pressed
                {
                    // Call remote approval since we're returning and skipping the base RAParse
                    ra.OnRemoteApproval(
                            me->raInfo.handle,
                            me->raInfo.operatorID, 
                            me->raInfo.password,
                            me->raInfo.approvalCode,
                            (LPCTSTR)sData,
                            me->raInfo.dwCookie
                            );

                    lUnapprovedRestrictedItems =0;
                    lRegularYellowNeedingApproval = 0;
                    bInApprovalNeeded = false; //TAR 279092
                    fApprovedForRestrictedItems = true;
                    fAgeObtained = true;
                    nCustomerAge = 99;
                    
                    CString csDateNotNeeded = ps.GetPSText(RA_AGE_RESTRICTED_ID, SCOT_LANGUAGE_PRIMARY);
                    csDateNotNeeded +=  _T("\n") + ps.GetPSText(BTT_BIRTHDATENOTREQUIRED, SCOT_LANGUAGE_PRIMARY);
                    trace(L7, _T("+ra.OnNormalItem, Message=%s"), csDateNotNeeded);
                    ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csDateNotNeeded); // Display valid birthday on RAP
                    trace(L7, _T("-ra.OnNormalItem"));  
                    
                    while (m_lAgerestrictQueuedtoRAP > 0)
                    {
                        trace(L7, _T("+ra.OnAgeApproval"));
                        ra.OnAgeApproval();
                        trace(L7, _T("-ra.OnAgeApproval"));
                        m_lAgerestrictQueuedtoRAP--;
                    }
                    m_lAgerestrictQueuedtoRAP = 0;

                    if(bDelayedAgeRestriction)
                    { 
                        //ra.OnNoRAPDataNeeded();

                        nCustomerAge = 99;
                        TBEnterDOB(0, 0, 0,nCustomerAge);

                        SendTBCommand(TBCMD_SET_DOB_ENTERED_STATUS, 2);        
                        SendTBCommand(TBCMD_SET_AGE_RESTRICTION_ITEM_ALLOWED_STATUS, 1);
                        SendTBCommand(TBCMD_SEND_AGE_VERIFY_DATA);

                    }else
                    {

                        // send to POS no DOB needed
                        trace(L3, _T("SMStateTB::RAParse no DOB needed "));
                        CString csTest = ps.GetPSText(SSF_TXT_GUEST_NODATENEEDED, SCOT_LANGUAGE_PRIMARY); //(_T("{F1}"));
                        SendTBCommand(TBCMD_SENDKEYEDDATA, csTest);
                    }
                    SAClearSecurityCondition();



                    if( GetHookProperty(_T("IsResumeTrxInProgress")) == _T("1") )
                    {
                        setAnchorState(BES_FINISH);
					    RETURNSTATE(InProgress)
                    }
                    else
                    {
                        // +SOTF6481
                        setAnchorState(BES_SCANANDBAG);
                        RETURNSTATE(InProgress);
                        //return setAndCreateAnchorState(BES_BAGANDEAS);
                        // -SOTF6481
                    }

                }
                else if (csRAPdob == _T("cCancel;")) //check to see if Reject/Cancel button key pressed
                {
                    if(bDelayedAgeRestriction)
                    {
                        int nYearRequired;
                        COleDateTime dtToday;
			            CString csValidBirthday;
			            CString csAnchorStateName = _T("");
			            dtToday = COleDateTime::GetCurrentTime();
			            nYearRequired = dtToday.GetYear() - io.d.nRestrictedAge;
			
                        CString csDesc, csInstruction;
			            csDesc = ps.GetPSText(RA_AGE_RESTRICTED_ID, SCOT_LANGUAGE_PRIMARY);
			            
			            _TCHAR TempBuffer[20];
				            _itot(io.d.nRestrictedAge, TempBuffer, 10); // turn the index into a string so it can be passed via Add below
			            CString csAge(TempBuffer);
			            csDesc += _T("\n") + csAge + _T(" ")
					              + ps.GetPSText(RA_AGERESTRICT_MINIMUM_AGE, SCOT_LANGUAGE_PRIMARY)
					              + _T("\n") + csValidBirthday;
			            
			            csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_LANEBTT_RESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, co.IsSecurityDisableOn()==true ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), ps.GetPSText(RA_THIRDLINE_RESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY));
		
                        ra.OnAgeRestrictedItem(csInstruction, io.d.nRestrictedAge, FALSE, co.fStateDOBNotNeededButton);
                        if( GetHookProperty(_T("IsResumeTrxInProgress")) == _T("1") )
                        {
                            setAnchorState(BES_FINISH);
					        RETURNSTATE(InProgress)
                        }
                        else
                        {
                            return setAndCreateAnchorState(BES_SCANANDBAG);
                        }
                    }
                    // Call remote approval since we're returning and skipping the base RAParse
                    ra.OnRemoteApproval(
                            me->raInfo.handle,
                            me->raInfo.operatorID, 
                            me->raInfo.password,
                            me->raInfo.approvalCode,
                            (LPCTSTR)sData,
                            me->raInfo.dwCookie
                            );

                    SendTBCommand(TBCMD_SET_TB_STATE,_T("CANCEL_AGE_RESTRICTED_ITEM"));
                    
                    lUnapprovedRestrictedItems =0;
                    lRegularYellowNeedingApproval = 0;
                    bInApprovalNeeded = false; //TAR 279092
                    fApprovedForRestrictedItems = true;
                    fAgeObtained = true;
                    nCustomerAge = 99;
                    
                    CString csAgeCheck = ps.GetPSText(RA_AGE_RESTRICTED_ID, SCOT_LANGUAGE_PRIMARY);
                    csAgeCheck +=  _T("\n") + ps.GetPSText(BTT_PRODUCE /*age check canceled*/, SCOT_LANGUAGE_PRIMARY);
                    trace(L7, _T("+ra.OnNormalItem, Message=%s"), csAgeCheck);
                    ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csAgeCheck);
                    trace(L7, _T("-ra.OnNormalItem"));  
                    
                    //TAR 196910 start
                    while (m_lAgerestrictQueuedtoRAP > 0)
                    {
                        trace(L7, _T("+ra.OnAgeApproval"));
                        ra.OnAgeApproval();
                        trace(L7, _T("-ra.OnAgeApproval"));
                        m_lAgerestrictQueuedtoRAP--;
                    }
                    m_lAgerestrictQueuedtoRAP = 0;
                    //TAR 196910 end
                    
                    SAClearSecurityCondition();

                    if( GetHookProperty(_T("IsResumeTrxInProgress")) == _T("1") )
                    {
                        setAnchorState(BES_FINISH);
					    RETURNSTATE(InProgress)
                    }
                    else
                    {
                        return setAndCreateAnchorState(BES_SCANANDBAG);
                    }
                }
                else
                {
                    CString csFromRAPdob = csRAPdob.Right(csRAPdob.GetLength() -1);  //removeleading ";"
                    if(VerifyBirthDate(csFromRAPdob,csoperatorID,cspassword))
                    {
                        // Call remote approval since we're returning and skipping the base RAParse
                        ra.OnRemoteApproval(
                                me->raInfo.handle,
                                me->raInfo.operatorID, 
                                me->raInfo.password,
                                me->raInfo.approvalCode,
                                (LPCTSTR)sData,
                                me->raInfo.dwCookie
                                );
                        trace(L3, _T("SMStateTB::RAParse DOB needed %s"), csFromRAPdob);
                            
                        // send key date to POS
                        if(!bDelayedAgeRestriction)
                        {
                            CString csTest = csFromRAPdob + _T("{ENTER}");
                            SendTBCommand(TBCMD_SENDKEYEDDATA, csTest);
                        }
                        
                        // lAge1 is the minimum age
                        // lAge2 is the input age
                        long lAge1 = io.d.nRestrictedAge;
                        long lAge2 = GetAge(csFromRAPdob);
                        if ( lAge2 < lAge1)
                        {
                            // log message to RAP and on e-Receipt so all known underage
                            CString csBirthDateEnter;
                            csBirthDateEnter.Format(ps.GetPSText(SSF_TXT_GUEST_AGE, SCOT_LANGUAGE_PRIMARY), lAge1);
                            ps.AddMessageToReceipt(_T("CMSMReceipt"), csBirthDateEnter);
                            
                            if(!bDelayedAgeRestriction)
                            {

                                DMSayPhrase(SETITEMASIDECONTINUEWITHPURCHASES);
                                SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
                            
                                if( GetHookProperty(_T("IsResumeTrxInProgress")) == _T("1") )
                                {
                                    setAnchorState(BES_FINISH);
					                RETURNSTATE(InProgress)
                                }
                                else
                                {
                                    return setAndCreateAnchorState(BES_SCANANDBAG);
                                }
                            }else
                            {
                                  
                                  RETURNSTATE(SmAssistMode);

                            }
                            
                        }
                        else
                        {
                            if(bDelayedAgeRestriction)
                            {
                                lUnapprovedRestrictedItems =0;
                                lRegularYellowNeedingApproval = 0;
                                bInApprovalNeeded = false; 
                                fApprovedForRestrictedItems = true;
                                fAgeObtained = true;
                                ra.OnNoRAPDataNeeded();

                                if(m_bReEnterDOB)
                                {
                                     m_bReEnterDOB = false;
                                     SendTBCommand(TBCMD_SET_DOB_ENTERED_STATUS, 3);
                                }

                                SendTBCommand(TBCMD_SET_AGE_RESTRICTION_ITEM_ALLOWED_STATUS, 1);
                                SendTBCommand(TBCMD_SEND_AGE_VERIFY_DATA);
                            }

                            SAClearSecurityCondition();

                            if( GetHookProperty(_T("IsResumeTrxInProgress")) == _T("1") )
                            {
                                setAnchorState(BES_FINISH);
					            RETURNSTATE(InProgress)
                            }
                            else
                            {
                                // +SOTF6481
                                setAnchorState(BES_SCANANDBAG);
                                RETURNSTATE(InProgress);
                                //return setAndCreateAnchorState(BES_BAGANDEAS);
                                // -SOTF6481
                            }
                        }
                    }
                }

                
            }
            /* TAR306844 start */  
            if (TypeOfApproval == RA_CAT_SUSPEND)
            {
                // Call remote approval since we're returning and skipping the base RAParse
                ra.OnRemoteApproval(
                        me->raInfo.handle,
                        me->raInfo.operatorID, 
                        me->raInfo.password,
                        me->raInfo.approvalCode,
                        (LPCTSTR)sData,
                        me->raInfo.dwCookie
                        );

                if(m_bIsSuspendFromRap)
                    return STATE_NULL;
                bSuspendRequested = true;
                bSuspendApproved = true;
                /******cm150000: Added In Merged Code******/
                /******cm150000: End Merged Code***********/
                m_bIsSuspendFromRap = true;
                RETURNSTATE(Suspend)
                    
            }
            /* TAR306844 end*/
         }
         break;	
         
         /* TAR306844 start*/
		// +SSCOI-45707
		// Remove below code block as this is already handled in base
     // case WMRA_REQUEST_SUSPEND:
         // if (fSAInStoreMode || m_bIsSuspendFromRap)
             // return STATE_NULL;
         // bSuspendRequested = true;
         // bSuspendApproved = false;
         // trace(L4, _T("SMState::RAParse: OnRequestSuspend."));
         // trace(L7, _T("+ra.OnRequestSuspend, SuspendReason=%s"), (LPCTSTR) me->raInfo.pData);
         // ra.OnRequestSuspend((LPCTSTR) me->raInfo.pData);
         // delete[] me->raInfo.pData;  // Done with me->raInfo, we must delete pData...
         // trace(L7, _T("-ra.OnRequestSuspend"));
         // //Tar 112723 
         // //SCOT already past Finish, so allow suspend immediatly
         // if(bFinish)
         // {
             // trace(L7, _T("+ra.OnReadyToSuspend"));
             // ra.OnReadyToSuspend();
             // trace(L7, _T("-ra.OnReadyToSuspend"));
         // }
         // return createAnchorState(); 
         
         
         // break;
		 // -SSCOI-45707
         /* TAR306844 end*/
    }
    
    // see if this is scanner data
    bool bUnsolicitedScannerData = false;
    if( me->raInfo.msg == WMRA_SEND_UNSOLICITED_DATA )
    {
        CString csData = me->raInfo.pData;
        if( (-1 != csData.Find(_T(':'))) && (-1 != csData.Find(DM_FS)) )
        {
            bUnsolicitedScannerData = true;
        }
    }
    
    // 
    CString strPropName = _T("IsRestricted_Prop");
    CString strPropValue;
    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strPropName, &strPropValue);
    
    //Scanner data is rejected by the core during partial assistmode, so here's the
    //workaround to handle the scanner data for the driver's licence at Age Restricted item.    
    if( (strPropValue == _T("1")) && bUnsolicitedScannerData )
    {
        int iFind;
        
        // 4 Fields: cRScan:Data:Label:Type
        // 4 Fields: cScanData:LastData|0xe1|Label|0xe1|Type
        CString csData1 = me->raInfo.pData;
        CString csData2 = _T("");
        CString csData3 = _T("");
        CString csData4 = _T("");
        
        // Get First Field (cScanData)
        iFind=csData1.Find((_TCHAR)_T(':'));
        ASSERT (iFind != -1);
        
        csData2 = csData1.Mid(iFind+1);
        ASSERT (csData2.GetLength()>0);
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
        
        csItemSent = csDMLastBarCode;
        io.Reset();
        
        io.csOrigCode = csDMLastBarCode;
        io.csItemCode = csDMLastBarLabel;
        io.cScanType  = csDMLastBarType;
        
        return TBItemSale(false,0,true);
        
    }
    return SMStateBase::RAParse(me);
}

SMStateBase * SMState::OnRAPDataReceived(LPCTSTR sData)
{
    return SMStateBase::OnRAPDataReceived(sData);
}

SMStateBase * SMState::HandleRAPData()
{
    return SMStateBase::HandleRAPData();
}

SMStateBase * SMState::OnRAPUnsolicitedDataReceived(LPCTSTR sData)
{
    trace (L6, _T("+SMState::OnRAPUnsolicitedDataReceived : %s"),sData);
    
    CString csData = sData;
    if (csData.Find(_T("RAPInitiateNoSale")) != -1)
    {
        trace (L6, _T("OnRAPUnsolicitedDataReceived = RAPInitiateNoSale"));
        // DEM ignore if lane closed
        if (!bLaneClosed)
        {
            // pos32258 RRS
            DMScannerDisable();
            
            // POS32258 - if there is a weight violation, we need the attendant to fix the weight violation before performing the NoSale function rrs
            if (bQuietNeedSecApproval) //for quiet mode - aiqin
            {                   
                trace(L6, _T("bQuietNeedSecApproval is true, cancel the open_cash_drawer!!!!!"));        
                
                setAnchorState(BES_SCANANDBAG);

                if(!fSAInTransaction)
	                SASendSCOTInput( ITEMIZATION );

                ra.OnSetTransactionState(RA_STATE_CASHDRAWER_CLOSE);                

                return TBStart();
            }
            // e pos32258 RRS
            
            
            CString csRemoteConnectionName = ra.GetRemoteConnectionName();
            m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), _T("operation=assist-acquire;"), csRemoteConnectionName );
            
            m_bIsNoSale = true;
            
            if (!dm.IsDeviceRemote(DMCLASS_CASHDRAWER, 0))
                dm.BeginRemote();
            
            SendTBCommand(TBCMD_SET_TB_STATE,_T("NO_SALE_STATE"));
            
            /*if (dm.IsDeviceRemote(DMCLASS_CASHDRAWER, 0))
            {
            CString csRemoteConnectionName = ra.GetRemoteConnectionName();
            m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), _T("operation=assist-acquire;"), csRemoteConnectionName );
        }*/
        }
        else
        {
            
            trace (L6, _T("Lane is Closed. Unable to do No Sale."));
            
            CString csRemoteConnectionName = ra.GetRemoteConnectionName();
            trace(L7, _T("+ra.OnSetTransactionState, No Sale Failed because closed "));
            ra.OnSetTransactionState(RA_STATE_CASHDRAWER_CLOSE);
            trace(L7, _T("-ra.OnSetTransactionState"));
            
            CString csNoSale = _T("Lane is Closed! Unable to perform No Sale.");
            trace(L7, _T("+ra.OnNormalItem, Message=%s"), csNoSale);
            ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csNoSale);
            trace(L7, _T("-ra.OnNormalItem")); 
            m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), _T("operation=assist-release;"), csRemoteConnectionName );
        }
    }
    else if (csData.Find(_T("RAPConfirmSuspend")) != -1)
    {
        trace (L6, _T("OnRAPUnsolicitedDataReceived = RAPConfirmSuspend"));
        m_bIsSuspendFromRap = true;
        RETURNSTATE(Suspend)
    }
	else if (csData.Find(_T("OnRapConnect")) != -1)
	{	
		//Doing nothing here at the moment. Just a place holder for future
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
		//(+) CRD124746
	  	CString csCurrentStateName = this->GetRuntimeClass()->m_lpszClassName;
		trace(L6, _T("SMState RA csCurrentStateName= %s "),csCurrentStateName);
		trace(L6,_T("checking csCurrentStateName is Scan&Bag state (%s)"),(csCurrentStateName.Find(_T("ScanAndBag")) == -1) ? _T("TRUE") : _T("FALSE"));

		CString csIsEverseenProcessing = GetHookProperty(_T("IsEverseenAlert"));	//CRD 347135
	  if(!((csCurrentStateName.Find(_T("ScanAndBag")) != -1) || (csCurrentStateName.Find(_T("AssistMenu")) != -1) ||
				((csCurrentStateName.Find(_T("SmDataEntry")) != -1) && csIsEverseenProcessing == _T("1"))))	//CRD 347135
      {
		 trace(L0, _T("Ignore SCAN details from RAP since we are not in Remote mode."));
         trace(L6,_T("-SMStateBase::OnRAPUnsolicitedDataReceived - Not In Remote Mode -- returning STATE_NULL."));
         return STATE_NULL;

	  }
	  //(-) CRD124746
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
		  trace(L6, _T("-SMStateBase::OnRAPUnsolicitedDataReceived() csDMLastBarLabel= %s		csDMLastBarCode= %s "),csDMLastBarLabel, csDMLastBarCode);
          return DMScanner();         // barcode
      }
      // -SR93.4

   }
   //Start CRD 232271
   else if ((csData.Find(_T("GenericDelayedApprovetype")) != -1))
   {   
	   //in the event GenericDelayedApprovetype is received and SCO still hasn't sent {PAYMENT} to POS and has interrupted SMSmDataEntry::PSCmdButton, send the TB command here
	   if(!bGenericDelayedFinishSent)
	   {
		   bGenericDelayedFinishSent = true;
		   setAnchorState(BES_FINISH);
		   
		   //Start SSCOI-72966
		   CString posKey = _T("{PAYMENT}");
		   SendTBCommand(TBCMD_SENDKEYEDDATA, posKey);
		   //End SSCOI-72966
		   
		   CREATE_AND_DISPENSE(InProgress)(0,
			   helpMode(),
			   co.getTimeOutTender());
		   
			return STATE_NULL;
	   }
   }
   //End CRD 232271
    
    trace (L6, _T("-SMState::OnRAPUnsolicitedDataReceived."));
    return SMStateBase::OnRAPUnsolicitedDataReceived(sData);
}

SMStateBase * SMState::HandleApproveVoidedItem()
{
    return SMStateBase::HandleApproveVoidedItem();
}

SMStateBase * SMState::HandleConfirmSuspend()
{
    return SMStateBase::HandleConfirmSuspend();
}

SMStateBase * SMState::HandlePricePrompt()
{
    return SMStateBase::HandlePricePrompt();
}

SMStateBase * SMState::HandleCancelTrasaction(CString csBttID)
{
    if((m_sButton == _T("cNo")) && (m_lUnApprovedRestrictedItemsDuringCancel))
    {
        
        lUnapprovedRestrictedItems = m_lUnApprovedRestrictedItemsDuringCancel;
        m_lUnApprovedRestrictedItemsDuringCancel = 0;
		
    }
	
    return SMStateBase::HandleCancelTrasaction(csBttID);
}

SMStateBase * SMState::ReturnToWaitForApprovalState()
{
    return SMStateBase::ReturnToWaitForApprovalState();
}

SMStateBase * SMState::HandleStartTransaction()
{
    CString strOptionName = _T("FastLaneConfiguration");
    CString m_csFLConfig;
    SMStateBase *pNextState;
	
	
	//there's a bug in the base.  if you press the language button on the attract screen
	//then start a trxn from the RAP.  The selected language will be changed to primary
	SCOT_LANGUAGE_TYPE tmpLanguage = m_customerLanguage;
	
    SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &m_csFLConfig);
    //TAR336289
    if ( m_csFLConfig == _T("Mini") && m_sButton == _T("cStartTransaction") )
    {
        m_bIsNewTrxSelected = true;
    }
    
    dm.saveTriColorLightStatus();
    
    pNextState = SMStateBase::HandleStartTransaction();
	
	m_customerLanguage = tmpLanguage;
	return pNextState;
}

SMStateBase * SMState::HandleStartTrainingTrans()
{
    return SMStateBase::HandleStartTrainingTrans();
}
void SMState::HandleHopperError(CString csBttID)
{
    SMStateBase::HandleHopperError(csBttID);
}

void SMState::EnableHandheldAssistMode(bool bEnable)
{
    SMStateBase::EnableHandheldAssistMode(bEnable);
}

void SMState::ClearHopperSubstitution(void)
{
    SMStateBase::ClearHopperSubstitution();
}

CString SMState::ExtractString(CString strData, CString sStringName)
{
    return SMStateBase::ExtractString(strData, sStringName);
}

SMStateBase * SMState::HandleMaxCouponLimit()
{
    return SMStateBase::HandleMaxCouponLimit();
}

SMStateBase * SMState::HandleMaxCouponLimitFinalization()
{
    return SMStateBase::HandleMaxCouponLimitFinalization();
}

SMStateBase * SMState::HandleInvalidBarcode()
{
    return SMStateBase::HandleInvalidBarcode();
}

SMStateBase *SMState::HandleRemoteGenericDelayed(CString csData)
{
    return SMStateBase::HandleRemoteGenericDelayed(csData);
}



