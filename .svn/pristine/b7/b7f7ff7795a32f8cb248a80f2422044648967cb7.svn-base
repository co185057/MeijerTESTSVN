#include <stdafx.h>
//#include <afxtempl.h>
//#include <queue>

#include "SMRecallingItems.h"
#include "MPProcedures.h"
#include "MobileConstantsSSF.h"
#include "TransbrokerSSF.h" // POS111525 POS111527

#define COMP_ID ID_SM
#define T_ID    _T("RecallingItems")

long SMRecallingItems::g_nTimeoutRecallingWaitForPos = 0;  // RFQ 9545
bool SMRecallingItems::g_bRecallingWaitForPos = false;	   // RFQ 9545	

DLLIMPORT extern MPProcedures mp;
CString SMRecallingItems::csSuspendBarcode = _T("");	//CRD 177093

IMPLEMENT_DYNCREATE(SMRecallingItems, CObject)

DEFINE_TIMESTAMP

SMRecallingItems::SMRecallingItems()
{
    IMPLEMENT_TIMESTAMP
}

// start RFQ 9545
SMStateBase * SMRecallingItems::Initialize()
{
    trace(L6, _T("SMRecallingItems::Initialize - g_bInRecallMode = %d, fSAInItemSale=%d, bFinishRecall=%d"), g_bInRecallMode, fSAInItemSale, bFinishRecall);
    trace (L6, _T("GetPreferredLanguage() = [%d]"), GetPreferredLanguage());
    trace (L6, _T("ps.GetLanguage() = [%d]"), ps.GetLanguage());
    trace (L6, _T("m_LanguageUsedPreviously = [%d]"), m_LanguageUsedPreviously);
    trace (L6, _T("m_customerLanguage = [%d]"), m_customerLanguage);
    trace (L6, _T("fStateFreeze = [%d]"), fStateFreeze);

	trace(L6, _T("g_bRecallingWaitForPos = %d g_nTimeoutRecallingWaitForPos = %d"), g_bRecallingWaitForPos, g_nTimeoutRecallingWaitForPos);

	if(ImmediateInterventionStack.GetCount()>0) // Clear the security intervention if item is placed on scale at Attract .
	{ 
		OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR, KEY_EXCEPTION_CATEGORY_IMMEDIATE_WT_CHANGES); 
		trace (L6, _T("clear all weight exceptions if any")); 
	}
	
	InitLanguageInfo();
	
	SMStateBase * retState = STATE_NULL;
	ShowRecallingScreen();
    
	//Start CRD 186197
	if(bFinishRecall)
	{
		FinishRecall();
		delete getRealStatePointer(retState);
		
		return STATE_NULL;
	}
	//End CRD 186197
	else if(g_bInRecallMode)
	{
		nTimeOut = 0;
		trace(L6, _T("In recall, set timeout to %d"), nTimeOut);
	}
	else
	{
		nTimeOut = g_nTimeoutRecallingWaitForDone;
		trace(L6, _T("Out of recall, set timeout to %d"), nTimeOut);
	}
	

	//CRD 177093 - build the suspend barcode from the CS xml - 470TTTNNNNN; where T is terminal number and N is the transaction number from the CS
	if(!bFinishSendingSuspendBarcode)
	{
		sendFLMSTLog(0, 0);	//start tlog

		bFinishSendingSuspendBarcode = true;

		CString csTerminalNumber;
		CString csTerminalNumberOptionName = _T("FLMSSuspendResumeTerminalNumber");
		SendTBCommand(TBCMD_GET_OPTIONS, &csTerminalNumberOptionName, &csTerminalNumber);

		CString csMobileTransactionID = mobileCart.GetTransactionId();
		csFLMSmPerksBarcode = mobileCart.GetCardNumber();
		
		csSuspendBarcode.Format(_T("470%s%s"), csTerminalNumber, csMobileTransactionID);
		
		trace(L6, _T("csSuspendBarcode - %s"), csSuspendBarcode);
		
		csDMLastBarCode = csSuspendBarcode;
		csDMLastBarLabel = csSuspendBarcode;
		csItemSent = csDMLastBarCode;
		csDMLastBarType = 101;	//need to have the barcode data here - SCAN_SDT_PDF217
		
		io.Reset();
		io.csOrigCode = csDMLastBarCode;    // item code keyed
		io.csItemCode = csDMLastBarCode;    // assumed to be the same
		io.lQuantityEntered = 0;
		io.cScanType = csDMLastBarType;
		io.d.fQuantityConfirmed = true;
		fItemOverride = true;
		
		
		retState = TBItemSale(false,0,true);
	}
	


	/* CRD 177093 - this will prevent SCO to go decode the barcode for selling
			- remain on this state
	if(g_bInRecallMode && !fSAInItemSale)
	{
		// start RFQ 9545 MGG fix
		if (g_nTimeoutRecallingWaitForPos > 0) // && (lUnknownItems == 0) && (lItems == 0))
		{
			g_bRecallingWaitForPos = true;	   
			nTimeOut = g_nTimeoutRecallingWaitForPos;
			return STATE_NULL;
		}
		else
		{

			m_bResendFLMSitem = false; // RFQ 9545 need to resend FLMS item

			if ( !recallList.ParseNextItem(true))  //No More Items
			{ 
				FinishRecall();
				return STATE_NULL;
				
			}
			
			// Sell the next item in the recall item queue
			retState = SellRecalledItem();
			
			// Throw the SMInProgress state away and keep us in SMRecallingItemsBase.
			// Otherwise, return the state TBItemSale() returned to handle
			// exceptions.
			if ( retState != STATE_NULL ) {
				CString csNextStateName =
					retState->GetRuntimeClass()->m_lpszClassName;
				if ( csNextStateName.Find(_T("InProgress")) != -1 ) {
					delete getRealStatePointer( retState );
					retState = STATE_NULL;
				}
			}
		}
		// end RFQ 9545 MGG fix
	}
	*/
	
	if (co.fOperationsDisplayQuickPickItems)
	{
		ps.DisplayQuickPickItem();
	}
	return retState;
   //  return SMRecallingItemsBase::Initialize();
}
// end RFQ 9545

void SMRecallingItems::ShowRecallingScreen()
{

    CString csContext = ps.GetCurrentContext();
    
    trace(L6, _T("ShowRecallingScreen %d, %s"), g_bEnteringRecallMode, csContext );
    
    if (g_bEnteringRecallMode || (csContext.Compare(_T("Processing")) != 0) )
    {
        g_bEnteringRecallMode = false;
        ps.SetCMFrame(_T("Processing"));
        ps.SetCMLeadthruText(TTL_LOADING_TRANSACTION);
        ps.Message12(MSG_LOADING_TRANSACTION, true);
        ps.ShowCMTBText(csTBMessage2Scot);  
        ps.CMButton(_T("ButtonHelp"),BTT_HELP,false);
        UpdateStateName(ps.GetPSText(MSG_LOADING_TRANSACTION, SCOT_LANGUAGE_PRIMARY));

		// start RFQ 9545 make sure amount display is disabled
		m_pAppPSX->SetConfigProperty(_T("Amount1Label"), _T("Processing"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
		m_pAppPSX->SetConfigProperty(_T("Amount2Label"), _T("Processing"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
		m_pAppPSX->SetConfigProperty(_T("Amount1"), _T("Processing"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
		m_pAppPSX->SetConfigProperty(_T("Amount2"), _T("Processing"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
		// end RFQ 9545
		
        ps.ShowFrame();
    } 
    
    else if (!g_bInRecallMode)
    {
        //jira NFL-762
        trace(L6, _T("+SMRecallingItems::ShowRecallingScreen - redisplaying done screen"));
        SMStateBase *tempState = TBGetTotalDetails();  //Woodmans item processing needs Total between item sales
        delete getRealStatePointer(tempState);
        UpdateTotal();
        ps.SetCMFrame(_T("CmDataEntry1"));
        ps.SetCMLeadthruText(TTL_RECALL_COMPLETE);
        ps.Message12(MSG_RECALL_PLEASE_SCAN_MORE_ITEMS, true);
        ps.ShowCMTBText(csTBMessage2Scot);  
        ps.Button(_T("CMButton1MidiList"), BTT_OK, true);  
    }

    SMRecallingItemsBase::ShowRecallingScreen();
}


/// FinishRecall
/**
Once the recall list is empty,  set all necessary flags to exit recalling.
The state then waits for the "Done" button to be pressed.
*/
void SMRecallingItems::FinishRecall()
{
	bFinishRecall = true; //SSCOI-55092
	bFinishSendingSuspendBarcode = false;	//CRD 177093

	csSuspendBarcode = _T(""); //clear this CString when resume has started

	// start RFQ 9545 make sure GOBACK button is disabled
	ps.SetCMFrame(_T("CmDataEntry1"));
	ps.Button(_T("ButtonGoBack"),_T(""), false);
	m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("CmDataEntry1"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
    trace(L6, _T("+-SMRecallingItems::FinishRecall, Finished recall, make sure goback button disabled"));
	// end RFQ 9545
    SMRecallingItemsBase::FinishRecall();
	if (!_tcsicmp(co.csHWType, _T("SS90"))) //CRD 515498 - additional message for shop and scan transfer complete for SS90s.
	{
		ps.SetCMLeadthruText(SSF_TXT_ADD_MORE_IMPULSE_ITEMS_MESSAGE, true);
		DMSayPhrase(SSF_WAV_SS90_IMPULSE_ITEMS); //CRD 515496 - play custom audio file.
	}
	else
	{
		ps.Message12(MSG_RECALL_PLEASE_SCAN_MORE_ITEMS, true);
	}
}

SMStateBase * SMRecallingItems::PSButton1(void)
{
	bFinishRecall = false;	//SSCOI-55092
	
	// POS111525 POS111527
	// Tell POS FLMS item processing is complete.
	SendTBCommand(TBCMD_FLMS_TRANSFERRING, false);
    return SMRecallingItemsBase::PSButton1();
    //UploadTicket Port -
}

// start RFQ 9545
SMStateBase *SMRecallingItems::TimedOut(void)
{
    trace(L6, _T("SMRecallingItems::TimedOut - g_bInRecallMode = %d, g_bWaitingForScale = %d g_bRecallingWaitForPos=%d g_bPressedDone=%d m_bResendFLMSitem=%d"), g_bInRecallMode, g_bWaitingForScale, g_bRecallingWaitForPos, g_bPressedDone, m_bResendFLMSitem);
		
	// start RFQ 9545
	if (g_bRecallingWaitForPos)
	{
	
		SMStateBase * retState = STATE_NULL;
		m_bResendFLMSitem = false; // RFQ 9545 need to resend FLMS item
		
	   /*
		g_bRecallingWaitForPos = false;	   
		m_bResendFLMSitem = false; // RFQ 9545 need to resend FLMS item

		if ( !recallList.ParseNextItem(true))  //No More Items
		{ 
			FinishRecall();
			return STATE_NULL;
            
		}
        
		// Sell the first item in the recall item queue or the next one if
		// we've just answered an immediate intervention.
		CString LastNonHelpStateName = this->csLastNonHelpStateName;
		if ( (LastNonHelpStateName == _T("SMScanAndBag") ||
			LastNonHelpStateName == _T("SMAttract") ) )//TAR427016
		{
			// retState = SellRecalledItem();
			retState = setAndCreateAnchorState(BES_SCANANDBAG);
		}
        
		// Throw the SMInProgress state away and keep us in SMRecallingItemsBase.
		// Otherwise, return the state TBItemSale() returned to handle
		// exceptions.
		if ( retState != STATE_NULL ) {
			CString csNextStateName =
				retState->GetRuntimeClass()->m_lpszClassName;
			if ( csNextStateName.Find(_T("InProgress")) != -1 ) {
				delete getRealStatePointer( retState );
				retState = STATE_NULL;
			}
		}
		*/

		retState = setAndCreateAnchorState(BES_SCANANDBAG);
		return retState;
	}
	// end RFQ 9545
	
    if ( !g_bInRecallMode ) 
    {
        if (g_bPressedDone && !g_bWaitingForScale)
        {
            //If we've given up on waiting for the customer to something on the scale after "Done",
            // send SecurityManager a "Item Expired" message to trigger processing.
            CKEYVALUEPARMLIST parmList;
            setAnchorState(BES_SCANANDBAG);
            CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_ITEM_EXPIRED);
            g_bMatchedWtWaitingOnEAS = false;
            g_bWtExpected = false;  //We should never expect wt if button is pressed on RAP
            _bstr_t bstrParmList((const _TCHAR *)parmList);
            BSTR bstrResult;
            bstrResult = NULL;
            SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
            SysFreeString(bstrResult);
			
            return STATE_NULL;
        }
        else if (!g_bPressedDone)
        {
            return PSButton1();  //Automatically press the "Done" button
        }
        else
        {
			// If done button pressed this means SSCO has not sold something and operator should check the FLMS basket. Otherwise you can do the following.
			trace(L6, _T("SMRecallingItems::TimedOut. Since we are trying to exit why not allow the exit. Recall completed. Done button pressed. POS and SSCO has processed everything as well as possible"));
			trace(L6, _T("The customer doesn't like having  to clear this at the RAP"));
			return SMRecallingItems::OnMatchedWt();
			//trace(L6, _T("SMRecallingItems::TimedOut - wait for a security event."));

            return STATE_NULL;
        }
    } 
	else 
	{
        // Oops, must be out of sync. Panic!
		if (m_bResendFLMSitem)
		{
			trace(L6, _T("SMRecallingItems::TimedOut - try resending item"));
			m_bResendFLMSitem = false; // RFQ 9545 need to resend FLMS item
			return SellRecalledItem();
		}
		trace(L6, _T("SMRecallingItems::TimedOut - out of sync. Panic"));

        return STATE(InProgress)::TimedOut();
    }
    // return SMRecallingItemsBase::TimedOut();
}
// end RFQ 9545

/**
* \brief Override to handle KEY_OSC_OPERATION_GOTO_SCANANDBAG and SmItemPickFinish message.
* 
*  SASecMgrParse() override will make Scotapp transit to Scan and Bag after SM allows it.
*  
*/
SMStateBase *SMRecallingItems::SASecMgrParse( const MessageElement *me) 
{
	int nEvent = me->secmgrInfo.nEvt;
    trace(L6, _T("SMRecallingItems::SASecMgrParse nEvent = %d"), nEvent);
    LPTSTR szParms;
    szParms = new TCHAR [_tcslen(me->secmgrInfo.szParms) + sizeof(TCHAR)];
    _tcscpy( szParms, me->secmgrInfo.szParms );
    
    CString csParms;
    csParms = szParms;
    delete [] szParms;
    
	if(nEvent == SmOnSecurityControl)
	{
        if (csParms.Find(KEY_OSC_OPERATION_GOTO_SCANANDBAG) > -1)
        {
            trace(L6, _T("KEY_OSC_OPERATION_GOTO_SCANANDBAG is received"));
            delete [] me->secmgrInfo.szParms;
			// CRD 177093  - we don't need to go to Scan and Bag for recalling items
           // if (g_bInRecallMode || g_bPressedDone)
           // {
           //     RETURNSTATE(ScanAndBag);
           // }
           // else
           // {
                return STATE_NULL;
           // }
			
        }
        else if (csParms.Find(KEY_OSE_EXC_TYPE_MISMATCHED) > -1)
        {
            return STATE_NULL;  //Ignore this for now.
        }
        
	}

    return SMRecallingItemsBase::SASecMgrParse(me);
}



/**
* \brief Override OnSkipBaggingAllowed()
* 
*  OnSkipBaggingAllowed() override will make Scotapp transit to Scan and Bag after SM allows it.
*  
*/
// start RFQ 9545
SMStateBase *SMRecallingItems::OnSkipBaggingAllowed()    
{
    trace(L6, _T("+SMRecallingItems::OnSkipBaggingAllowed - g_bInRecallMode = %d, nTimeOut set to %d"), g_bInRecallMode, nTimeOut);
    
    SMStateBase* returnState = STATE_NULL;

	// start RFQ 9545
	if (g_nTimeoutRecallingWaitForPos > 0)
	{
		g_bRecallingWaitForPos = true;	   
		nTimeOut = g_nTimeoutRecallingWaitForPos;
		return STATE_NULL;
	}
	else
	{

		m_bResendFLMSitem = false; // RFQ 9545 need to resend FLMS item

		if ( !recallList.ParseNextItem(true))  //No More Items
		{ 
			FinishRecall();
		}
		else
		{
			// Recall queue not empty. Sell another item.
			returnState = SellRecalledItem();
			
			// Throw the SMInProgress state away and keep us in SMRecallingItemsBase.
			// Otherwise, return the state TBItemSale() returned to handle
			// exceptions.
			if ( returnState != STATE_NULL ) 
			{
				CString csNextStateName =
					returnState ->GetRuntimeClass()->m_lpszClassName;
				if ( csNextStateName.Find(_T("InProgress")) != -1 ) 
				{
					delete getRealStatePointer( returnState );
					returnState = STATE_NULL;
				}
			}
		}
	}
	// end RFQ 9545
	
    trace(L6, _T("-SMRecallingItems::OnSkipBaggingAllowed - g_bInRecallMode = %d, nTimeOut set to %d"), g_bInRecallMode, nTimeOut);
    return returnState; 
    //return SMRecallingItemsBase::OnSkipBaggingAllowed();
}
// end RFQ 9545 

/**
* \brief Override OnBackToLGW.
* 
*  OnBackToLGW() will return STATE_NULL instead of transitioning to ScanAndBag.
*  
*/
SMStateBase *SMRecallingItems::OnBackToLGW(void)
{
    return SMRecallingItemsBase::OnBackToLGW();
}


SMStateBase *SMRecallingItems::OnMatchedWt()
{
    return SMRecallingItemsBase::OnMatchedWt();
}

void SMRecallingItems::UnInitialize(void)
{
    SMRecallingItemsBase::UnInitialize();
}


//Start CRD 177093
SMStateBase *SMRecallingItems::TBParse(MessageElement *me)
{
	SMStateBase *pState = SMState::TBParse( me);

	if( m_TBLastState != TB_UNKNOWN && m_TBLastState != TB_IGNORE && m_TBLastState != TB_TENDERVOIDED)
	{
		CString csTBStateName;
		extern void GetTBReturnStateName(TBRC nTBState, CString & csStateName); // defined in CTransBroker.cpp
		GetTBReturnStateName(m_TBLastState, csTBStateName);
		
		if( csTBStateName == _T("NewTBState") )
		{
			// get the new state from TB if possible
			SendTBCommand(TBCMD_GET_TBSTATENAME, m_TBLastState, &csTBStateName);
		}
		trace(L3, _T("!!!! SMStateTB::TBParse - m_TBLastState id=%d, name:%s"), m_TBLastState, csTBStateName);
     }

	switch(m_TBLastState)
	{
		//this means that resume ticket has been started
		case TB_RESUME_STARTED:
			{
				// CRD136007
				// Tell hook that recalling items currently in progress.
				SetHookProperty(_T("IsSCOFLMS"),_T("1"));
				SendTBCommand(TBCMD_FLMS_TRANSFERRING, true); //CRD 232268

				//should still remain in RecallingItems state, should not go to SMInProgress
				delete getRealStatePointer(pState);

				return STATE_NULL;
			}
			break;
		case TB_RESUME_ITEMS:
			{
				io.Reset();
				TBGetItemDetails(io.d);
				
				bool bIsLinkedItem = io.d.bLinkedItem;
				if(!bIsLinkedItem) 
				{
					io.lQuantityEntered = io.d.lQtySold;
					io.lTotalWeightEntered = io.lWeightEntered;    
					io.lWeightEntered = io.d.lWgtSold;			   
				}
				io.fVoid = io.d.fVoid;
				io.lUnitPriceEntered = io.d.lUnitPrice;
				io.lExtPriceEntered = io.d.lExtPrice;		   
				io.csItemCode = io.d.csItemCode;
				
				//SOTF 6822
				
				bool bPrevAssistMenuStatus = co.IsAttendantModeOn();
				
				//if we are on an age restricted item while resuming
				//do not play approval needed.  Set the in assist menu
				//flag to avoid the wave file
				if(io.d.fRestricted)
				{
					co.SetInAssistMenus(true);
				}
				
				//sotf
				
				//POS23514 Mobile Shopper: Do not call if this is an ACS Reward
				if(m_TBLastState == TB_RESUME_ITEMS)
				{
					SMStateBase *rc = TBItemDetails();
				}
				
				//SOTF 6822 
				
				co.SetInAssistMenus(bPrevAssistMenuStatus);
				
				//sotf
				
				io.csItemCode=_T("");
				itemDetailOrig.Reset();
				itemDetailNew.Reset();
				
				// no more items to resume
				if(	SendTBCommand(TBCMD_RESUME_ITEMS, NULL) == 0)
				{
					m_bIsFromDisplayingResumeItem = true;
					
					SetHookProperty(_T("IsResumeComplete"), _T("1"));
					SetTBProperty(_T("IsThereItemNeedToDisplayed"), _T("0"));
					SetTBProperty(_T("IsDirtyTrxWithNoItems"), _T("0"));
					
					if( GetTBProperty(_T("ScannedBarcodeAfterResume")) == _T("") )
					{						
						delete getRealStatePointer( pState);
						SASendSCOTInput( TRANSACTION_CONTINUE );
						bEASReEnableScanner = true;

						delete getRealStatePointer(pState);
						FinishRecall();
						return STATE_NULL;
					}
				
				}
			}
			break;

		case TB_NEEDMOREDATA:
			{
				//we need to check POS state for this data needed if it's retrieval failed
				CString csPosState = GetHookProperty(_T("__POSState"));

				if(csPosState == _T("RESUME_ERROR"))
				{
					//if the POS returns RESUME_ERROR for FLMS, means that web service isn't done creating the resume file yet.
					//we need to give time for the POS to finish and resend the data
					SendTBCommand(TBCMD_SENDKEYEDDATA, CString(_T("{F2}")));
					Sleep(250);

					//Then send again the barcode
					csDMLastBarCode = csSuspendBarcode;
					csDMLastBarLabel = csSuspendBarcode;
					csItemSent = csDMLastBarCode;
					csDMLastBarType = 101;	//need to have the barcode data here - SCAN_SDT_PDF217
					
					io.Reset();
					io.csOrigCode = csDMLastBarCode;    // item code keyed
					io.csItemCode = csDMLastBarCode;    // assumed to be the same
					io.lQuantityEntered = 0;
					io.cScanType = csDMLastBarType;
					io.d.fQuantityConfirmed = true;
					fItemOverride = true;
					
					pState = TBItemSale(false,0,true);
				}

			}
			break;
	}

	return pState;
}
//End CRD 177093