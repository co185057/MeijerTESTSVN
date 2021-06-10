//////////////////////////////////////////////////////////////////////
//
// SolutionMsgMap.cpp: implementation of the CSolutionMsgMap class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h" 
#include "SolutionMsgMap.h"
#include "TraceSystem.h"
#include "CustomerCurrentDisplay.h"
#include "CustomerTBConnectPOS.h"
#include "CustomerCurrentItem.h"
#include "CustomerCurrentTender.h"
#include "CustomerMHOptions.h"
#include "CustomerMHItem.h"
#include "CustomerMHTender.h"
#include "CustomerTrxBroker.h"
#include "CustomerTBProperties.h"
#include "CustomerCurrentReceipt.h"
#include "CustomerCurrentCashDrawer.h"
#include "CustomerTBConnectFL.h"
#include "BaseMessages.h"
#include "CustomerCurrentSlip.h"
#include "scannerMsg.h"
#include "CustomerTBSharedDefs.h"
#include "CustomerMHMicr.h"
#include "SolutionCurrentSigCap.h"
#include "CustomerMHSlip.h"

#define T_ID    _T("CSolutionMsgMap")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSolutionMsgMap::CSolutionMsgMap()
{
}

CSolutionMsgMap::~CSolutionMsgMap()
{
}

void CSolutionMsgMap::Initialize()
{
   CBaseMsgMap::Initialize();
}

// msgAPI_EnterItem()
// is used to send a keyed or scanned item to the POS.
// it is called by the message map as a FLAPI handler for TBEnterItem.
// 
bool CSolutionMsgMap::msgAPI_EnterItem(TBSTATE &tbretState, LPARAM lparam)
{
   tbtraceIO(_T("msgAPI_EnterItem"));

   tbretState = CCustomerMHItem::instance()->HandleItemEntry(lparam);

   return true;
}

// msgAPI_EnterTender()
// is called by the message map as a FLAPI handler for TBEnterTender.
// 
bool CSolutionMsgMap::msgAPI_EnterTender(TBSTATE &tbretState, LPARAM lparam)
{
   tbtraceIO(_T("msgAPI_EnterTender"));

   tbretState = CCustomerMHTender::instance()->HandleTenderEntry(lparam);

   return true;
}

// msgAPI_Finish()
// is called by the message map as a FLAPI handler for TBFinish.
// 
//bool CSolutionMsgMap::msgAPI_Finish(TBSTATE &tbretState, LPARAM lparam)
//{
   //tbtraceIO(_T("msgAPI_Finish"));
   //CBaseMsgMap::msgAPI_Finish(tbretState, lparam);
//
//   return true;
//}

// msgAPI_CancelTransaction()
// is called by the message map as a FLAPI handler for TBCancelTransaction.
// 
bool CSolutionMsgMap::msgAPI_CancelTransaction(TBSTATE &tbretState, LPARAM lparam)
{
   tbtraceIO(_T("msgAPI_CancelTransaction"));

   return true;
}
// msgAPI_SuspendTransaction()
// is called by the message map as a FLAPI handler for TBSuspendTransaction.
// 
bool CSolutionMsgMap::msgAPI_SuspendTransaction(TBSTATE &tbretState, LPARAM lparam)
{
   tbtraceIO(_T("msgAPI_SuspendTransaction"));
    
   return true;
}

// msgAPI_SetTrainingModeIn()
// is called by the message map as a FLAPI handler for TBSetTrainingMode(1).
// 
bool CSolutionMsgMap::msgAPI_SetTrainingModeIn(TBSTATE &tbretState, LPARAM lparam)
{
	tbtraceIO(_T("msgAPI_SetTrainingModeIn"));
    CString csSending= _T("{MISC}");
    tbtrace(TM_ENTRY, _T("-msgAPI_SetTrainingModeIn() sending:%s"), csSending);
    CCustomerTBConnectPOS::instance()->SendSequence(csSending);    
	return true;
}

// msgAPI_SetTrainingModeOut()
// is called by the message map as a FLAPI handler for TBSetTrainingMode(0).
// 
bool CSolutionMsgMap::msgAPI_SetTrainingModeOut(TBSTATE &tbretState, LPARAM lparam)
{
	tbtraceIO(_T("msgAPI_SetTrainingModeOut"));
	CString csSending= _T("{MISC}");
    tbtrace(TM_ENTRY, _T("-msgAPI_SetTrainingModeOut() sending:%s"), csSending);
    CCustomerTBConnectPOS::instance()->SendSequence(csSending);   
	return true;
}
// msgAPI_TermDisconnected()
// is called by the message map as a FLAPI handler for Disconnected POS msg
//
bool CSolutionMsgMap::msgTermDisconnected(TBSTATE &tbretState, LPARAM lparam)
{
    tbtraceIO(_T("msgTermDisconnected"));
	return true;
}

bool CSolutionMsgMap::msgMessage300(TBSTATE &tbretState, LPARAM lparam)
{
    tbtraceIO(_T("msgMessage300"));
    // this msg is tender related. pass it to tender handler.
    CString csBoolValue = (LPCTSTR) lparam;
    if(csBoolValue == _T("1"))   //in itemization
    {
        tbretState = CCustomerMHTender::instance()->HandleMessage300(true);
    }else
    {
        tbretState = CCustomerMHTender::instance()->HandleMessage300();
    }
    return true;
}

bool CSolutionMsgMap::msgSendWeight(TBSTATE &tbretState, LPARAM lparam)
{
    tbtraceIO(_T("msgSendWeight"));
    // this msg is tender related. pass it to tender handler.
    CCustomerMHItem::instance()->SendWeight(_ttol((LPCTSTR)lparam));
    return true;
}


TBSTATE CSolutionMsgMap::ProcessMessage()
{
   return CBaseMsgMap::ProcessMessage();
}

bool CSolutionMsgMap::msgTenderAccepted(TBSTATE &tbretState, LPARAM lparam)
{
    tbtraceIO(_T("msgTenderAccepted"));
    // this msg is tender related. pass it to tender handler.
    CCustomerMHTender::instance()->HandleTenderAccepted();
    return true;
}

bool CSolutionMsgMap::msgDetermineIfTenderResetNeeded(TBSTATE &tbretState, LPARAM lparam)
{ 
    CCustomerTrxBroker *pTB= CCustomerTrxBroker::instance();   
   
   	CString  csCurTBState = pTB->GetCurrentInProgressStateName();
    CString csTenderSelected = CCustomerTBProperties::instance()->GetHookProperty(_T("TenderType"));

    //make sure we're in a TENDER_CREDIT, TENDER_DEBIT, or TENDER_EBT type state
    if((csCurTBState.Find(_T("TENDER_")) == 0) && (csCurTBState.Find(_T("TENDER_PAYMENT")) == -1))
    {         
        //if no tender already select then do nothing
        if(csTenderSelected.IsEmpty())
        {
            tbretState = TB_IGNORE;
            pTB->SetCurrentInProgressState(_T("TB_ENTERTENDERSTATE_IN"));            
            pTB->SetFLState(_T("SMFinish"));
            return true;
        
        }

        CString csTmp = csCurTBState;
        CString csLen = _T("TENDER_");
        csTmp.Delete(0, csLen.GetLength());

        if(csTmp == _T("GIFTCARD"))
        {
            csTmp = _T("GIFT");
        }

        if(csTmp == _T("FOODSTAMPS"))
        {
            csTmp = _T("FOOD");
        }
        //check if we are on the payment menu and a tender is selected that matches what FL sent
        if(csTenderSelected.Find(csTmp) != -1)
        {
            tbretState = TB_IGNORE;
        
        }else
        {
            pTB->SetCurrentInProgressState(_T("TB_ENTERTENDERSTATE_IN"));
            
            pTB->SetFLState(_T("SMFinish")); 
        }
    }

    return true;

}

bool CSolutionMsgMap::msgAssistModeTrxEnd( TBSTATE &tbretState, LPARAM lparam )
{
    tbtraceIO(_T("msgAssistModeTrxEnd()"));
    tbretState = TB_TRXEND;

    CCustomerTrxBroker & trxBroker = *CCustomerTrxBroker::instance();

	if ( CCustomerMHOptions::instance()->BoolOptionValue( _T("TB_FT_SUPPORT_RAPTENDER") ) &&
       ( trxBroker.InRemoteAssistMode() || trxBroker.IsCheckPaymentAtLaneAvailable() )   )
	{
		if ( CCustomerCurrentCashDrawer::instance()->Status() ==
					CCashDrawerMsg::CashDrawerStates::CD_OPEN )
		{
			long lExpect;
			CCustomerCurrentCashDrawer::instance()->GetExpectedCashDrawerState( &lExpect );
			if ( lExpect == CASHDRAWER_OPEN )
			{
				// FastLane needs to open the cash drawer at end of transaction.
				tbtrace( TM_INFO, _T("Asking RAP to open the drawer.") );
				tbretState = TB_NEWCASHDRAWERSTATE;
			}
		}

        TBSTATE tbSlip = tbretState;
		CCustomerCurrentSlip* pCurSlip = CCustomerCurrentSlip::instance();

		CString csIsRapTendering;
        csIsRapTendering = trxBroker.TBGetProperty(_T("TB_FT_SUPPORT_RAPTENDER"));

		if ( pCurSlip->GetNumRemaining() > 0 )
		{
			if ( ! pCurSlip->IsSlipPrinting() )
			{
				tbretState = TB_ENDORSEDOCUMENT;
				
			}
            else
            {
                tbtrace( TM_INFO, _T("Wait for Slip to Print") );
                tbSlip = TB_IGNORE;
            }
		}
		else
		{
			if (pCurSlip->IsInProgress())
			{
                tbSlip = TB_IGNORE;
				pCurSlip->SetEndorseWhenFinished( true );
			}
		}

        if ( tbSlip != tbretState )
        {
            if ( tbretState != TB_TRXEND )
            {
                if ( tbSlip != TB_IGNORE )
                {
    				CCustomerTBScotMsg::instance()->SendReturnStateMsg( tbSlip, NULL, NULL );
                }
            }
            else
            {
                tbretState = tbSlip;
            }
        }


		if ( (tbretState == TB_TRXEND) || (tbSlip == TB_NEWCASHDRAWERSTATE)  )
		{
            TBSTATE tbPrinter = tbretState;

			CCustomerCurrentReceipt* pCurReceipt = CCustomerCurrentReceipt::instance();

			// AssistMode Receipt and CashDrawer handling are RAP Tendering specific.
			if ( 0 != pCurReceipt->GetReceiptFileCount() )
			{
				// Handle Pending Receipts
				if ( !pCurReceipt->IsPrinting() )
				{
					tbPrinter = TB_COMPLETEPRINTANDCUT;
					pCurReceipt->SetIsPrinting( true );
				}
				else
				{
					// Was already printing, wait for it to complete
					tbtrace( TM_INFO, _T("Waiting on Receipt printing to complete."));
                    tbPrinter = TB_IGNORE;
				}
			}
			else if ( pCurReceipt->IsInProgress() )
			{
				pCurReceipt->SetPrintWhenFinished( true );
                tbPrinter = TB_IGNORE;
			}
		    else
            {
                CString csRebate = CCustomerTBProperties::instance()->GetHookProperty(_T("RebateReceiptRequested"));
                CString csGift = CCustomerTBProperties::instance()->GetHookProperty(_T("PrintGiftReceipt"));

                if ( (csRebate == _T("1")) || (csGift != _T("0")) )
                {
                    pCurReceipt->SetPrintWhenFinished( true );
                    tbPrinter = TB_IGNORE;
                } 
                else if ( CCustomerCurrentCashDrawer::instance()->Status() ==
					    CCashDrawerMsg::CashDrawerStates::CD_OPEN )
		        {
                    tbtrace( TM_INFO, _T("Waiting on Cash Drawer to Close") );
                    tbPrinter = TB_IGNORE;
		        }
            }

            if (tbPrinter != tbretState)
            {
                if ( tbretState != TB_TRXEND )
                {
                    if ( tbPrinter != TB_IGNORE )
                    {
    				    CCustomerTBScotMsg::instance()->SendReturnStateMsg( tbPrinter, NULL, NULL );
                    }
                }
                else
                {
                    tbretState = tbPrinter;
                }
            }
		}
	}

    return true;
}

bool CSolutionMsgMap::msgEBTSlipPrint( TBSTATE &tbretState, LPARAM lparam )
{
	CCustomerCurrentSlip* pCurSlip = CCustomerCurrentSlip::instance();
    
	CString csIsRapTendering;
    csIsRapTendering = CCustomerTrxBroker::instance()->TBGetProperty(_T("TB_FT_SUPPORT_RAPTENDER"));

	if ( pCurSlip->GetNumRemaining() > 0) 
	{
		if ( ! pCurSlip->IsSlipPrinting())
		{
			tbretState = TB_ENDORSEDOCUMENT;
		}
		else
		{
			tbretState = TB_IGNORE;
		}
	}
	else
	{
		if (pCurSlip->IsInProgress())
		{
			pCurSlip->SetEndorseWhenFinished( true );
			tbretState = TB_IGNORE;
		}
	}
    return true;
}
bool CSolutionMsgMap::msgAddRewardLine( TBSTATE &tbretState, LPARAM lparam )
{
	tbtraceIO(_T("msgAddRewardLine"));

	//
	// Extract the reward line from the parameter
	//
	// Get all the variables
	LPCTSTR pszMsg = (LPCTSTR)lparam;
	ASSERT(pszMsg && *pszMsg);

	PSMessageInfo pMsg = new SMessageInfo;
	ZeroMemory(pMsg, sizeof(SMessageInfo));
	
	CopyString(pMsg->szE_ReceiptText, pszMsg);
	CopyString(pMsg->szRAP_MessageText, pszMsg);
				
	pMsg->bHiliteReceiptText = false;
    pMsg->bNeedItemCancel = true;
    pMsg->bApprovalNeeded = false;
    pMsg->bGoBackScanAndBag = true;
    pMsg->bHiliteRAPText = false;

	CCustomerTBConnectFL::instance()->SetMessageInfo(pMsg);
    delete [] pMsg->szE_ReceiptText;
    delete [] pMsg->szRAP_MessageText;
    delete pMsg;
    CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_MESSAGESCREEN, NULL, NULL);   

    //note that this TRX has a reward item (eg. loyalty card...)
    CCustomerTrxBroker::instance()->SetAnyRewardItems(true);
    return true;
}

bool CSolutionMsgMap::msgPreserveAssistModeState( TBSTATE &tbretState, LPARAM lparam )
{
    tbtraceIO(_T("msgPreserveAssistModeState"));

    CString csCurr = CCustomerTrxBroker::instance()->GetCurrentInProgressStateName();

    if ( csCurr.Find(_T("ASSISTMODE_")) != 0)
    {
        tbtrace( TM_WARNING, _T("Replacing current state '%s' with '%s'"), 
            csCurr, (LPCTSTR) lparam );
        CCustomerTrxBroker::instance()->SetCurrentInProgressState( (LPCTSTR) lparam );
    }

    return true;
}

bool CSolutionMsgMap::msgVoidScannedItem( TBSTATE &tbretState, LPARAM lparam )
{
    //ensure POS at a state ready to receive the scan and sent it on
	tbtrace(TM_ENTRY, _T("+-msgVoidScannedItem()"));
    
	CCustomerCurrentItem *pCurItem= CCustomerCurrentItem::instance();
    CScannerMsg scan;
	CString csSymbology(_T(""));
	CString itemCode = pCurItem->ItemCode();

    scan.BarcodeType((CScannerMsg::BarcodeTypes)pCurItem->GetScanCodeType());
    scan.Data(pCurItem->ItemCode());

	//104, 119 code EAN13
	if( scan.BarcodeType() == 104 || scan.BarcodeType() == 119) 
	{	
		csSymbology = _T("F");
		
		if ( csSymbology.GetLength() > 0 )
		{
			// Don't re-add symbology code if it is already there!!
			// This is a work-around.  An SMState Override is also adding symbology!!!!
			CString csTest = itemCode.Left(csSymbology.GetLength());
			if ( csSymbology == csTest )
			{
				csSymbology = "";
			}
		}
		CString csScanCode = csSymbology + itemCode;
		scan.Data(csScanCode);

		tbtrace(TM_INFO, _T("Sending Scan Data to POS: BarcodeType: %d, Barcode: %s"), scan.BarcodeType(), csScanCode);//(LPCTSTR)scan.Data());
		//CCustomerTBConnectPOS::instance()->SendMsgToPOS(scan);
		//return TB_IGNORE;
	}

    CCustomerTBConnectPOS::instance()->SendMsgToPOS(scan);
    return true;

}

bool CSolutionMsgMap::msgDontReturnTBNeedAsstIfInAsst( TBSTATE &tbretState, LPARAM lparam )
{
    tbtraceIO(_T("msgDontReturnTBNeedAsstIfInAsst"));

    CString csCurr = CCustomerTrxBroker::instance()->GetCurrentInProgressStateName();

    if ( csCurr.Find(_T("ASSISTMODE_")) != -1)
    {
       tbretState = TB_IGNORE;
    }

    return true;
}

bool CSolutionMsgMap::msgRestartFastlane( TBSTATE &tbretState, LPARAM lparam )
{
    tbtraceIO(_T("msgRestartFastlane"));
    const CString REBOOT_FASTLANE(_T("c:\\scot\\bin\\reboot.exe"));
    CCustomerTrxBroker *pTB = CCustomerTrxBroker::instance();
    
    //if the POS exited because a Force Load was issue go ahead a reboot
    if(
        (CString(CCustomerTBProperties::instance()->GetHookProperty(_T("ForceLoadRequested"))) == _T("1")) ||
        (CString(pTB->GetCurrentInProgressStateName()) == _T("POS_EXIT_FASTLANE_RETDN_cOk"))
       )
    {
        Sleep(5);
        tbtrace(TM_ENTRY, _T("Attempting reboot of Fastlane due to Force Load/POS termination"));
        //issue reboot command 
        if( !LaunchApp(REBOOT_FASTLANE))   
        {
            tbtrace(TM_ENTRY, _T("Fail to issue reboot command when requested()"));
        }
    }else
    {
        tbtrace(TM_ENTRY, _T("POS has exited unexpectedly"));
        pTB->SetCurrentInProgressState(_T("POS_EXIT_FASTLANE"));
        tbretState = TB_NEEDMOREDATA;
        return (msgDataNeeded(tbretState, (LPARAM)_T("RebootLane;")));
        
    }
    return true;
}

bool CSolutionMsgMap::msgSendKeyIfNotInAssist( TBSTATE &tbretState, LPARAM lparam )
{
    CString csCurr = CCustomerTrxBroker::instance()->GetCurrentInProgressStateName();

    //if we're in assistmode don't send this key
    if (( csCurr.Find(_T("ASSISTMODE_")) == -1) || 
        ( csCurr.Find(_T("ASSISTMODE_EXIT")) != -1) || 
        ( csCurr.Find(_T("ASSISTMODE_TENDER_CASH_SENT")) != -1) ||
        ( csCurr.Find(_T("ASSISTMODE_TENDER_CASH")) != -1))
    {
        msgSendSequence(tbretState, lparam);
    }else
    {
        tbretState = TB_IGNORE;
    }

    return true;

    
}

bool CSolutionMsgMap::msgDetermineIfSeparateMediaOkToHandleNow( TBSTATE &tbretState, LPARAM lparam )
{
    CCustomerTrxBroker *pTB = CCustomerTrxBroker::instance();
    CString csFLCurrState = pTB->GetFLNextStateName();

    if((csFLCurrState != _T("SMTakeReceipt")) &&
       (csFLCurrState != _T("SMTakeCard"))    &&
       (csFLCurrState != _T("SMTakeChange"))  &&
       (csFLCurrState != _T("SMThankYou"))    &&
       (csFLCurrState != _T("SMPrintGiftReceipt")))
    {
        CTBInProgress *pTBState = CTBInProgress::instance();
        CString csTBStateType= pTBState->GetType(pTB->GetCurrentInProgressState());
        if (csTBStateType != _T("TERM_INITIALIZATION"))
        {
            pTB->SetCurrentInProgressState(_T("SEPARATE_MEDIA_CLOSE_LANE"));
        
            msgSendSequence(tbretState, lparam);
        }else
        {
            
            pTB->TBSetProperty(_T("SeparateMedia"), _T(""),true);
            pTB->TBSetProperty(_T("SeparateMediaExceptLastTrxn"), _T(""),true);
        }

    }else
    {
        //we know FL is finishing a TRXN so give it time to get back to Attract
        pTB->TriggerMsgMap();
    }
    
    return true;
}

bool CSolutionMsgMap::msgDataNeededWithCustomerScreen( TBSTATE & tbretState, LPARAM lparam )
{
	tbtraceIO(_T("msgDataNeededWithCustomerScreen"));
	
	LPCTSTR pszMsg = (LPCTSTR)lparam;
	ASSERT(pszMsg && *pszMsg);
	
	CString strMsg = pszMsg;
	CString strRapDataEntry = _T("");
	CString strCustDataEntry = _T("");
	
	int iPos = strMsg.Find(_T(';'));
	if( iPos >= 0 )
	{
		strRapDataEntry = strMsg.Left(iPos+1);
		strCustDataEntry = strMsg.Mid(iPos+1);
		// setting this property triggers the subsequence state change from CustomMessage to first go 
		// to RAP Data Needed
		CCustomerTBProperties::instance()->SetProperty(TBPROP_RAPDATANEEDED_CUSTENTRY, strCustDataEntry);
		return msgDataNeeded(tbretState, (LPARAM)(LPCTSTR)strRapDataEntry);
	}
	else
	{
		tbtrace(TM_ERROR, _T("Invalid parameter specified: '%s'"), (LPCTSTR)strMsg);
	}

	return true;
}
bool CSolutionMsgMap::msgAssistModeTenderInProgress( TBSTATE & tbretState, LPARAM lparam )
{
   tbtraceIO(_T("msgAssistModeTenderInProgress")); 
   
   bool bFlag;
   
   if(CString((LPCTSTR)lparam) == _T("1"))
      bFlag = true;
   else
       bFlag = false;

   CCustomerTrxBroker *pTB= CCustomerTrxBroker::instance();
   CString csCurr = CCustomerTrxBroker::instance()->GetCurrentInProgressStateName(); 
   CString csPosState = CCustomerTrxBroker::instance()->TBGetProperty(_T("__POSState"), true);

   //POS289891
   if( csPosState.Find(_T("AGE_VERIFY")) != -1 && csCurr.Find(_T("ASSISTMODE")) != -1)
   {
       tbtrace( TM_INFO, _T("The POS is at Age verify prompt. Ignore the cash tender.") );

       tbretState = TB_IGNORE;
       pTB->SetCurrentInProgressState(_T("ASSISTMODE_ENTER"));
	   return true;
   }
   else if(bFlag)
    {
        CString csAmt = CCustomerTrxBroker::instance()->TBGetProperty(_T("EnteredTenderAmount"), false);
        CString csSeq;
        csSeq.Format(_T("{PAYMENT}{CASH}%s{ENTER}"), csAmt);
        CCustomerTBConnectPOS::instance()->SendSequence( csSeq);       
    }
    tbtrace( TM_INFO, _T("TenderInProgress set to %d"), bFlag );
   
    CCustomerTrxBroker::instance()->SetTenderInProgress(bFlag);  
    return true;

    
}

bool CSolutionMsgMap::msgNeedMICR( TBSTATE & tbretState, LPARAM lparam )
{
	long lVal = _ttol((LPCTSTR)lparam);
	if (lVal)
	{
		CCustomerMHMicr::instance()->SetMICR( true );
	}
	else
	{
		CCustomerMHMicr::instance()->SetMICR( false );
	}
	return true;
}

bool CSolutionMsgMap::msgSayPhrase(TBSTATE & tbretState, LPARAM lparam )
{

    CCustomerTrxBroker::instance()->TBSayPhrase( (LPCTSTR)lparam);
    return true;
}
bool CSolutionMsgMap::msgSetHookProperty(TBSTATE &tbretState, LPARAM lparam)
{
    tbtrace(TM_ENTRY, _T("msgSetHookProperty:%s"), (LPCTSTR)lparam);

	LPCTSTR psz;
	CString strProperty;
	CString strValue;
	CString strType;
   
	// Get all the variables
	LPCTSTR pszMsg = (LPCTSTR)lparam;
	ASSERT(pszMsg && *pszMsg);
   
	LPTSTR pszParams = NULL;
	CopyString(pszParams, pszMsg);
   
	psz = _tcstok(pszParams, _T(";"));
	if( psz )
	{
      strProperty = psz;
      psz = _tcstok(NULL, _T(";"));
      if( psz )
      {
         strValue = psz;
         psz = _tcstok(NULL, _T(";"));
         if( psz )
            strType = psz;
      }
	}
	delete [] pszParams;

    CCustomerTrxBroker::instance()->TBSetProperty(strProperty, strValue, true);
    return true;
}

bool CSolutionMsgMap::msgAMCancelRebateReceipt(TBSTATE &tbretState, LPARAM lparam)
{
    CString csRebate = CCustomerTrxBroker::instance()->TBGetProperty(_T("RebateReceiptRequested"), true );

    if ( csRebate == _T("1") )
    {
        CCustomerTrxBroker::instance()->TBSetProperty(_T("RebateReceiptRequested"), _T("0"), true);
        return msgAssistModeTrxEnd( tbretState, lparam );
    }
    return true;
}

bool CSolutionMsgMap::msgAddWeightLine( TBSTATE &tbretState, LPARAM lparam )
{
	tbtraceIO(_T("msgAddWeightLine"));

	//
	// Extract the weight line from the parameter
	//
	// Get all the variables
    CString csData = CString((LPCTSTR) lparam);
	int firstSpace = csData.Find(_T(" "));
	int secondSpace = csData.Mid(firstSpace+1,csData.GetLength()-firstSpace-1).Find(_T(" "));
	int thirdSpace = csData.Mid(secondSpace+1+firstSpace+1,csData.GetLength()-secondSpace-1).Find(_T(" "));

	CString price = csData.Mid(0,firstSpace);
	CString weight = csData.Mid(firstSpace+1,secondSpace);
	CString units = csData.Mid(secondSpace+1+firstSpace+1,thirdSpace);

	// Not currently used
    CString quant = csData.Mid(firstSpace+1+secondSpace+1+thirdSpace+1,csData.GetLength()-thirdSpace-1);

	// Format data for decimal
	if (weight.GetLength() <= 2) 
	{
		weight.Insert(0, _T("."));
	}
	else
	{
		weight.Insert(weight.GetLength()-2, _T("."));
	}
	if (price.GetLength() <= 2) 
	{
		price.Insert(0, _T("."));
	}
	else
	{
		//price.Insert(weight.GetLength()-2, _T(".")); commented to fix TAR#315444 and added line below
		price.Insert(price.GetLength()-2, _T("."));
	}
	CString weightLine = _T("     ") + weight + (_T(" ")) + units + _T(" @ 1 ") + units + _T(" /     ") + price;

    LPCTSTR pszMsg = (LPCTSTR)weightLine;
	ASSERT(pszMsg && *pszMsg);

	PSMessageInfo pMsg = new SMessageInfo;
	ZeroMemory(pMsg, sizeof(SMessageInfo));
	
	CopyString(pMsg->szE_ReceiptText, pszMsg);
	CopyString(pMsg->szRAP_MessageText, pszMsg);
				
	pMsg->bHiliteReceiptText = false;
    //pMsg->bNeedItemCancel = true;  commented to fix TAR#315444
    pMsg->bApprovalNeeded = false;
    pMsg->bGoBackScanAndBag = true;
    pMsg->bHiliteRAPText = false;

	CCustomerTBConnectFL::instance()->SetMessageInfo(pMsg);
    delete [] pMsg->szE_ReceiptText;
    delete [] pMsg->szRAP_MessageText;
    delete pMsg;
    CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_MESSAGESCREEN, NULL, NULL);   

    return true;
}

bool CSolutionMsgMap::msgMessageScreen(TBSTATE &tbretState, LPARAM lparam )
{
   tbtraceIO(_T("msgMessageScreen"));
   
   LPCTSTR pszMsg = (LPCTSTR)lparam;
   ASSERT(pszMsg && *pszMsg);
   
   // expecting the following parameters
   // 
   // 1) Screen Text ID  --> (indirect lookup from property if enclosed in [ ]
   // 2) Leadthru Text ID --> (indirect lookup from property if enclosed in [ ]
   // 3) Audio file to play --> (name only, no path expected)
   // 4) Need cancel button --> '1' = yes  anything else = no
   // 5) Need go back to Scan And Bag --> '1' = yes anything else = no
   // 6) Screen Text --> 1 = Use actual text supplied: do not preform ID lookup; Anything else = no
   // 7) Leadthru Text --> 1 = Use actual text supplied: do not preform ID lookup; Anything else = no
   LPTSTR pszParams = NULL;
   CopyString(pszParams, pszMsg);
   
   CString strScreenText = _T(""); 
   CString strLeadThru = _T("");
   CString strAudio = _T("");
   CString strCancel = _T("");
   CString strGoBack = _T("");
   CString strScreenTextNoLookup = _T("");
   CString strLeadThruTextNoLookup = _T("");
   
   LPCTSTR psz;
   
   // Get all the variables
   psz = _tcstok(pszParams, _T(";"));
   if( psz )
   {
      strScreenText = psz;
      psz = _tcstok(NULL, _T(";"));
      if( psz )
      {
         strLeadThru = psz;
         psz = _tcstok(NULL, _T(";"));
         if( psz )
         {
            strAudio = psz;
            psz = _tcstok(NULL, _T(";"));
            if( psz )
            {
               strCancel = psz;
               strCancel.TrimLeft();
               strCancel.TrimRight();
               
               psz = _tcstok(NULL, _T(";"));
               if( psz )
               {
                  strGoBack = psz;
                  strGoBack.TrimLeft();
                  strGoBack.TrimRight();

				  psz = _tcstok(NULL, _T(";"));
				  if( psz )
				  {
				     strScreenTextNoLookup = psz;
					 strScreenTextNoLookup.TrimLeft();
					 strScreenTextNoLookup.TrimRight();

					 psz = _tcstok(NULL, _T(";"));
					 if( psz )
					 {
					    strLeadThruTextNoLookup = psz;
					    strLeadThruTextNoLookup.TrimLeft();
					    strLeadThruTextNoLookup.TrimRight();
					 }
				  }
               }
            }
         }
      }
   }

   tbtrace(TM_INFO, _T("ScreenText='%s'; LeadThru='%s'; Audio='%s'; CancelButton='%s'; BackToScanAndBag='%s'; strScreenTextNoLookup='%s'; strLeadThruTextNoLookup='%s'"),
			strScreenText, strLeadThru, strAudio, strCancel, strGoBack, strScreenTextNoLookup, strLeadThruTextNoLookup);

   if (strScreenTextNoLookup == _T("1") && strLeadThruTextNoLookup == _T("1"))
   {
	   // Not yet implemented
   }
   else if (strScreenTextNoLookup == _T("1"))
   {
	  SetupMessageScreen(strScreenText, _ttoi(strLeadThru), strAudio, strCancel == _T("1"), strGoBack == _T("1"));
   }
   else if (strLeadThruTextNoLookup == _T("1"))
   {
		// Not yet implemented
   }
   else 
   {
	   CBaseMsgMap::SetupMessageScreen(_ttoi(strScreenText), _ttoi(strLeadThru), strAudio, strCancel == _T("1"), strGoBack == _T("1"));
   }
   
   
   delete [] pszParams;
   
   return true;
}

void CSolutionMsgMap::SetupMessageScreen(CString strScreenText, int nLeadThruTextID, LPCTSTR strWaveFile, bool bNeedCancel, bool bNeedGoBack)
{
   CBaseMessages *pMessages = CBaseMessages::instance();
   CCustomerTrxBroker *pCurrTrx = CCustomerTrxBroker::instance();
   CCustomerTBConnectFL &tbConnectFL = *CCustomerTBConnectFL::instance();
   
   SMessageInfo msgMessageObj;
   memset(&msgMessageObj,0,sizeof(SMessageInfo));
   
   CString csWaveFile = strWaveFile;
   CString csScreenText = strScreenText;
   CString csLeadThruText = pMessages->GetStringValueFromID(nLeadThruTextID);
   msgMessageObj.szLeadThruText    = (LPTSTR)(LPCTSTR)csLeadThruText;
   msgMessageObj.szScreenText      = (LPTSTR)(LPCTSTR)csScreenText;
   msgMessageObj.szPlayWave        = (LPTSTR)(LPCTSTR)csWaveFile;
   msgMessageObj.bNeedItemCancel   = bNeedCancel;
   msgMessageObj.bGoBackScanAndBag = bNeedGoBack;
   tbConnectFL.SetMessageInfo(&msgMessageObj);
}

void CSolutionMsgMap::LookupHandlers()
{
   CBaseMsgMap::LookupHandlers();   // call base first

   // add custom msg handler lookups
   AddHandlerLookup(_T("msgAPI_EnterItem"),         (tMethodPointer) &CSolutionMsgMap::msgAPI_EnterItem);
   AddHandlerLookup(_T("msgAPI_EnterTender"),       (tMethodPointer) &CSolutionMsgMap::msgAPI_EnterTender);
   //AddHandlerLookup(_T("msgAPI_Finish"),            (tMethodPointer) &CSolutionMsgMap::msgAPI_Finish);
   AddHandlerLookup(_T("msgAPI_CancelTransaction"), (tMethodPointer) &CSolutionMsgMap::msgAPI_CancelTransaction);
   AddHandlerLookup(_T("msgAPI_SuspendTransaction"),(tMethodPointer) &CSolutionMsgMap::msgAPI_SuspendTransaction);
   AddHandlerLookup(_T("msgAPI_SetTrainingModeOut"),(tMethodPointer) &CSolutionMsgMap::msgAPI_SetTrainingModeOut);
   AddHandlerLookup(_T("msgAPI_SetTrainingModeIn"), (tMethodPointer) &CSolutionMsgMap::msgAPI_SetTrainingModeIn);
   AddHandlerLookup(_T("msgTermDisconnected"),		(tMethodPointer) &CSolutionMsgMap::msgTermDisconnected);
   AddHandlerLookup(_T("msgMessage300"),		    (tMethodPointer) &CSolutionMsgMap::msgMessage300);
   AddHandlerLookup(_T("msgSendWeight"),		    (tMethodPointer) &CSolutionMsgMap::msgSendWeight);
   AddHandlerLookup(_T("msgTenderAccepted"),		(tMethodPointer) &CSolutionMsgMap::msgTenderAccepted);
   AddHandlerLookup(_T("msgDetermineIfTenderResetNeeded"),		(tMethodPointer) &CSolutionMsgMap::msgDetermineIfTenderResetNeeded);
   AddHandlerLookup(_T("msgAssistModeTrxEnd"),      (tMethodPointer) &CSolutionMsgMap::msgAssistModeTrxEnd);
   AddHandlerLookup(_T("msgAddRewardLine"),        (tMethodPointer) &CSolutionMsgMap::msgAddRewardLine);
   AddHandlerLookup(_T("msgPreserveAssistModeState"), (tMethodPointer) &CSolutionMsgMap::msgPreserveAssistModeState);   
   AddHandlerLookup(_T("msgVoidScannedItem"),         (tMethodPointer) &CSolutionMsgMap::msgVoidScannedItem);   
   AddHandlerLookup(_T("msgDontReturnTBNeedAsstIfInAsst"),         (tMethodPointer) &CSolutionMsgMap::msgDontReturnTBNeedAsstIfInAsst);   
   AddHandlerLookup(_T("msgRestartFastlane"),                      (tMethodPointer) &CSolutionMsgMap::msgRestartFastlane);   
   AddHandlerLookup(_T("msgEBTSlipPrint"),                         (tMethodPointer) &CSolutionMsgMap::msgEBTSlipPrint);   
   AddHandlerLookup(_T("msgSendKeyIfNotInAssist"),                 (tMethodPointer) &CSolutionMsgMap::msgSendKeyIfNotInAssist);   
   AddHandlerLookup(_T("msgDetermineIfSeparateMediaOkToHandleNow"),(tMethodPointer) &CSolutionMsgMap::msgDetermineIfSeparateMediaOkToHandleNow);   
   AddHandlerLookup(_T("msgDataNeededWithCustomerScreen"),         (tMethodPointer) &CSolutionMsgMap::msgDataNeededWithCustomerScreen); 
   AddHandlerLookup(_T("msgAssistModeTenderInProgress"),           (tMethodPointer) &CSolutionMsgMap::msgAssistModeTenderInProgress); 
   AddHandlerLookup(_T("msgNeedMICR"),				(tMethodPointer) &CSolutionMsgMap::msgNeedMICR);
   AddHandlerLookup(_T("msgSayPhrase"),				(tMethodPointer) &CSolutionMsgMap::msgSayPhrase);
   AddHandlerLookup(_T("msgSetHookProperty"),		(tMethodPointer) &CSolutionMsgMap::msgSetHookProperty);
   AddHandlerLookup(_T("msgAMCancelRebateReceipt"), (tMethodPointer) &CSolutionMsgMap::msgAMCancelRebateReceipt);
   AddHandlerLookup(_T("msgAddWeightLine"),	(tMethodPointer) &CSolutionMsgMap::msgAddWeightLine);
   AddHandlerLookup(_T("msgMessageScreen"),	(tMethodPointer) &CSolutionMsgMap::msgMessageScreen);
}