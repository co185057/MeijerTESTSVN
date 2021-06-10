//////////////////////////////////////////////////////////////////////
//
// CustomerMsgMap.cpp: implementation of the CCustomerMsgMap class. 
//
//
// CHANGE HISTORY:
//
// CRD 557225 Name: CJ Estrada Date: Mar 4, 2021
// CRD 554916 Name: CJ Estrada Date: Feb 9, 2021
// POS320221 WorkRequest:76936 Name:Alex Bronola Date:April 3, 2017
// POS320065 WorkRequest:76936 Name:Alex Bronola Date:April 3, 2017
// POS292753 WorkRequest:79417 Name:Alex Bronola Date:March 29, 2017
// POS315640 WorkRequest:79417 Name:Alex Bronola Date:March 17, 2017
// POS296302 WorkRequest:76936 Name:Alex Bronola Date:Jan 10, 2017
// POS297533 WorkRequest:76936 Name:Alex Bronola Date:Dec 15, 2016
// POS297520 WorkRequest:76936 Name:Robert Susanto Date:October 28, 2016
// POS253532 WorkRequest:68941 Name:Saiprasad Srihasam Date:March 18, 2016
// POS108460 Work Request:65347 Name:Matt Condra    Date:August 21, 2014
// POS82276 Work Request:59313 Name:Robert Susanto    Date:January 15, 2014
// POS77912 Work Request:58657 Name:Robert Susanto    Date:October 31, 2013
// POS73665 Work Request:58657 Name:Matt Condra    Date:September 24, 2013
// POS48277/71132 Work Request:58657 Name: Robert Susanto   Date:September 13, 2013
// POS74992 Work Request:58657 Name:Robert Susanto Date:August 29, 2013
// POS68773/58095 Work Request:52635 Name:Robert Susanto Date:June 28, 2013
// POS39476/54205 Work Request:52635 Name:Robert Susanto Date:January 8, 2013
// POS39476/52217 Work Request:52635 Name:Robert Susanto Date:January 8, 2013
// POS45465 Work Request:52635 Name:Robert Susanto Date:November 1, 2012
// POS41125 Work Request:50864 Name:Robert Susanto Date:August 17, 2012
// POS36319 Work Request:50864 Name:Robert Susanto Date:June 20, 2012
// POS30981/POS31050 Work Request:50864 Name:Robert Susanto Date:April 24, 2012
// POS11024 Work Request:16632 Name:Sandeep Shenoy Date:October 20, 2011
// POS11024 Work Request:16632 Name:Sandeep Shenoy Date:October 12, 2011
// POS10771 WORK REQUEST:16632 Name: Robert Susanto Date: August 17, 2011

// POS11024 Work Request:16632 Name:Sandy Reedy Date:July 20, 2011
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomerMsgMap.h"
#include "TraceSystem.h"
#include "CustomerCurrentDisplay.h"
#include "CustomerTBConnectPOS.h"
#include "CustomerCurrentItem.h"
#include "CustomerCurrentTender.h"
#include "CustomerTBProperties.h"
#include "CustomerTBSharedDefs.h"
#include "CustomerTBConnectFL.h"
#include "CustomerTrxBroker.h"
#include "CustomerMHItem.h"
#include "CustomerCurrentReceipt.h"
#include "CustomerCurrentSlip.h"
#include <tlhelp32.h>
#include <stdio.h>
#include <wchar.h>
#include "CustomerMHCashDrawer.h"
#include "BaseMessages.h"
#include "CustomerMHOptions.h"
#include "tchar.h"

// SOTF 8723 - RRS
#include "CustomerCurrentCashDrawer.h"
// E SOTF 8723 - RRS

// pos10771 - aug17
#include "CustomerCurrentTransaction.h"
// e pos10771

// pos 11273 - rrs
#include "scannerMsg.h"
// e pos 11273

#include "CustomerCurrentReceipt.h" //POS74992

#define T_ID    _T("CCustomerMsgMap")

CString CCustomerMsgMap::csEASItemDescription=_T("");
/***********************************************************************
/*  Change History                                            
/*
/*  POS:11273 Work Request:11273 Name:Robert Susanto Date:February 22 2011  
/*
/*  POS:11009 Work Request:11009 Name:Robert Susanto Date:February 9 2011
/*
/*  POS:10771 Work Request:10855 Name:Robert Susanto Date:February 3 2011
/*
/*
/************************************************************************/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCustomerMsgMap::CCustomerMsgMap():
m_csCurrPOSState(_T("")),
m_csPrevPOSState(_T(""))
{
    HRESULT hr = m_xmlPOSDisplay.CreateInstance(__uuidof( MSXML2::DOMDocument40) );
    if(FAILED(hr))
    {       
		m_xmlPOSDisplay = NULL;
        
    }

    
}

CCustomerMsgMap::~CCustomerMsgMap()
{
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void CCustomerMsgMap::LookupHandlers()
{
   CSolutionMsgMap::LookupHandlers();   // call base first

   // add custom msg handler lookups
    AddHandlerLookup(_T("msgPriceDiscrepancy"),       (tMethodPointer) &CCustomerMsgMap::msgPriceDiscrepancy);
    AddHandlerLookup(_T("msgFSChange"),               (tMethodPointer) &CCustomerMsgMap::msgFSChange);
    AddHandlerLookup(_T("msgPriceEmbed"),             (tMethodPointer) &CCustomerMsgMap::msgPriceEmbed);
    AddHandlerLookup(_T("msgCMTenderingAmount"),      (tMethodPointer) &CCustomerMsgMap::msgCMTenderingAmount);
    AddHandlerLookup(_T("msgFormatPriceInquiry"),     (tMethodPointer) &CCustomerMsgMap::msgFormatPriceInquiry);
    AddHandlerLookup(_T("msgEnterTenderedAmount"),    (tMethodPointer) &CCustomerMsgMap::msgEnterTenderedAmount);
    AddHandlerLookup(_T("msgOnRebateItem"),           (tMethodPointer) &CCustomerMsgMap::msgOnRebateItem);  
    AddHandlerLookup(_T("msgRestartFastlane"),        (tMethodPointer) &CCustomerMsgMap::msgRestartFastlane);   
	AddHandlerLookup(_T("msgScannedItemInSMFinish"),  (tMethodPointer) &CCustomerMsgMap::msgScannedItemInSMFinish);   
	AddHandlerLookup(_T("msgCashDrawerSetCloseImmediate"),  (tMethodPointer) &CCustomerMsgMap::msgCashDrawerSetCloseImmediate);
    AddHandlerLookup(_T("msgAssistModeExit"),         (tMethodPointer) &CCustomerMsgMap::msgAssistModeExit);
    AddHandlerLookup(_T("msgOnGiftReceipt"),          (tMethodPointer) &CCustomerMsgMap::msgOnGiftReceipt);
    AddHandlerLookup(_T("msgSendSequenceIfRemote"),   (tMethodPointer) &CCustomerMsgMap::msgSendSequenceIfRemote);
    AddHandlerLookup(_T("msgVoidRewardLine"),         (tMethodPointer) &CCustomerMsgMap::msgVoidRewardLine);
    AddHandlerLookup(_T("msgSendSequenceWithWaits"),  (tMethodPointer) &CCustomerMsgMap::msgSendSequenceWithWaits);
    AddHandlerLookup(_T("msgDecideIfTenderAccepted"), (tMethodPointer) &CCustomerMsgMap::msgDecideIfTenderAccepted);
    AddHandlerLookup(_T("msgResetToLastPOSState"),    (tMethodPointer) &CCustomerMsgMap::msgResetToLastPOSState);
    AddHandlerLookup(_T("msgReturnCodeHandler"),      (tMethodPointer) &CCustomerMsgMap::msgReturnCodeHandler);
	AddHandlerLookup(_T("msgDataNeeded"),             (tMethodPointer) &CCustomerMsgMap::msgDataNeeded);
	AddHandlerLookup(_T("msgHandleWicBalanceExceeded"),(tMethodPointer) &CCustomerMsgMap::msgHandleWicBalanceExceeded);
    AddHandlerLookup(_T("msgDetectAndHandleNewRestrictedItem"),         (tMethodPointer) &CCustomerMsgMap::msgDetectAndHandleNewRestrictedItem);
    AddHandlerLookup(_T("msgCheckIfCouponToBeCollectedAndDisplay"),     (tMethodPointer) &CCustomerMsgMap::msgCheckIfCouponToBeCollectedAndDisplay);
	AddHandlerLookup(_T("msgImmediateOrDelayedIntervention"),           (tMethodPointer) &CCustomerMsgMap::msgImmediateOrDelayedIntervention);
    AddHandlerLookup(_T("msgFormatPromoNumber"),                        (tMethodPointer) &CCustomerMsgMap::msgFormatPromoNumber);   // SOTF6400
	AddHandlerLookup(_T("msgDynamicCustomerDigitalCouponMessageScreen"),(tMethodPointer) &CCustomerMsgMap::msgDynamicCustomerDigitalCouponMessageScreen);   // SOTF6400
    AddHandlerLookup(_T("msgGiftPhonePriceEntrySetup"),                 (tMethodPointer) &CCustomerMsgMap::msgGiftPhonePriceEntrySetup);   // SOTF7042
    AddHandlerLookup(_T("msgResendDOB"),                                (tMethodPointer) &CCustomerMsgMap::msgResendDOB);   // SOTF7261
    AddHandlerLookup(_T("msgMEPAvailable"),                             (tMethodPointer) &CCustomerMsgMap::msgMEPAvailable);   // SOTF7695
    AddHandlerLookup(_T("msgGiftPhonePriceRetry"),                      (tMethodPointer) &CCustomerMsgMap::msgGiftPhonePriceRetry);   // SOTF7701

    AddHandlerLookup(_T("msgCheckedIfDataIsScannedSequence"),           (tMethodPointer) &CCustomerMsgMap::msgCheckedIfDataIsScannedSequence);  // pos 11273
    AddHandlerLookup(_T("msgBagAloneScreen"),                           (tMethodPointer) &CCustomerMsgMap::msgBagAloneScreen);  //POS11024 Mobile Shopper

    AddHandlerLookup(_T("msgCheckForMediaCollection"),     (tMethodPointer) &CCustomerMsgMap::msgCheckForMediaCollection);    //POS36319 collect bottle and coin slip

    AddHandlerLookup(_T("msgBuildGreenDotDataNeeded"),     (tMethodPointer) &CCustomerMsgMap::msgBuildGreenDotDataNeeded);    //POS68773

    AddHandlerLookup(_T("msgPurgeAllReceipts"),     (tMethodPointer) &CCustomerMsgMap::msgPurgeAllReceipts);    //POS74922
	AddHandlerLookup(_T("msgMessageScreen"),	(tMethodPointer) &CCustomerMsgMap::msgMessageScreen);
	AddHandlerLookup(_T("msgEASGenericDelayed"),	(tMethodPointer) &CCustomerMsgMap::msgEASGenericDelayed);
	AddHandlerLookup(_T("msgMEPMobileAvailable"),	(tMethodPointer) &CCustomerMsgMap::msgMEPMobileAvailable);	//CRD 231416
//	AddHandlerLookup(_T("msgMobileGenericDelayed"),	(tMethodPointer) &CCustomerMsgMap::msgMobileGenericDelayed);	//CRD 223409
	AddHandlerLookup(_T("msgSendFLNotifyCancelAlert"),	(tMethodPointer) &CCustomerMsgMap::msgSendFLNotifyCancelAlert);
	AddHandlerLookup(_T("msgAccountLookupFailedIntervention"), (tMethodPointer) &CCustomerMsgMap::msgAccountLookupFailedIntervention);	// CRD 554916
}

TBSTATE CCustomerMsgMap::ProcessMessage()
{
     tbtraceIO( _T("CCustomerMsgMap::ProcessMessage"));

     CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();
     pCurTrx->TBSetProperty(_T("PropIsItNewPosState"), _T("1"));
	 
	 return CSolutionMsgMap::ProcessMessage();
}

bool CCustomerMsgMap::msgMessageScreen(TBSTATE &tbretState, LPARAM lparam )
{
   tbtraceIO(_T("msgMessageScreen"));
   
   //ScotApp goes to SMFinish multiple times and so when the match below occurs, it
   //calls this method multiple times also. Below is preventing this to happened.
   //POSState=[HIGHVALUE_PROMO_CHOICE], CurTBState=[TB_ENTERTENDERSTATE_IN]
   //POS289070
	CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();
    if(pCurTrx->TBGetProperty(_T("PropIsItNewPosState")) != _T("1") )
	{
		tbtrace(TM_INFO, _T("Ignoring duplicate call for msgMessageScreen."));
		tbretState = TB_IGNORE;
		return true;
	}
	pCurTrx->TBSetProperty(_T("PropIsItNewPosState"), _T("0")); 
    

   return CSolutionMsgMap::msgMessageScreen(tbretState, lparam );
}

TBSTATE CCustomerMsgMap::dispatchHelper(const CString &csMessage)
{

	//don't save the current screen if it is error_context_clear.
	//error_context_clear is not good screen to go back to if we
	//have to reset to last good screen
	if(m_csCurrPOSState != _T("ERROR_CONTEXT_CLEAR"))
	{
		m_csPrevPOSState = m_csCurrPOSState;
	}

	m_csCurrPOSState = csMessage; 

	
	return CSolutionMsgMap::dispatchHelper(csMessage);

}

void CCustomerMsgMap::resetToLastPOSState(void)
{
	if(!m_csPrevPOSState.IsEmpty())
	{
		tbtrace(TM_ENTRY, _T("resetToLastPOSState() setting POSstate = %s"), m_csPrevPOSState);
		CSolutionMsgMap::dispatchHelper(m_csPrevPOSState);
	}

}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool CCustomerMsgMap::msgFormatPriceInquiry( TBSTATE &tbretState, LPARAM lparam )
{
    tbtraceIO( _T("CCustomerMsgMap::msgFormatPriceInquiry"));

	CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();
	CString csPrice = pCurTrx->TBGetProperty(_T("PriceInq_Price"),true);
	

	tbtrace(TM_INFO, _T("Price Inquiry = Price = %s"), csPrice);

    long lPrice =_ttol((LPCTSTR) csPrice);

	tbtrace(TM_INFO, _T("Price Inquiry = long Price = %d"), lPrice);
	float fPrice = lPrice / 100.00;

	csPrice.Format(_T("%0.2f"), fPrice);

    pCurTrx->TBSetProperty(_T("PriceInq_Price"),csPrice,true);

	pCurTrx->TriggerMsgMap();

	return true;
}

bool CCustomerMsgMap::msgPriceDiscrepancy( TBSTATE &tbretState, LPARAM lparam )
{
   
	CString csPriceDiscrepancy = _T("");
	CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();
	CString csOrigPrice = pCurTrx->TBGetProperty(_T("Orig_price_display"),true);

	
	CString temp2 = _T("");
	temp2.Format(_T("%.2lf"),( wcstod(csOrigPrice,NULL))/ 100);
	csPriceDiscrepancy = _T("Original Price                 ") + temp2;
	
	TBSTATE tbstate;
	msgAddRewardLine(tbstate,(LPARAM)(LPCTSTR)csPriceDiscrepancy);
    
    return true;
}

bool CCustomerMsgMap::msgFSChange( TBSTATE &tbretState, LPARAM lparam )
{
   
	CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();
	CString csFSChange = pCurTrx->TBGetProperty(_T("FS_Change_Prop"),true);
	CString csFSCProp = pCurTrx->TBGetProperty(_T("PropFSChange"),true);
	if( csFSCProp == _T("0") )
	{
		CString csTemp = _T("");
		csTemp.Format(_T("%.2lf"),( wcstod(csFSChange,NULL))/ 100.0);
		csFSChange = _T("FS CHANGE                      ") + csTemp;
		TBSTATE tbstate;
		msgAddRewardLine(tbstate,(LPARAM)(LPCTSTR)csFSChange);
		pCurTrx->TBSetProperty(_T("PropFSChange"), _T("1"),true);

	}
    
    return true;
}


bool CCustomerMsgMap::msgPriceEmbed( TBSTATE &tbretState, LPARAM lparam )
{
   
	CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();

	PS_UI_CMD psLocalUICmd = new S_UI_CMD;
	memset(psLocalUICmd, 0, sizeof(S_UI_CMD)); // to be safe
	psLocalUICmd->InitUnionField();
	psLocalUICmd->eStructType = UI_STR_RECEIPT;  // set the command type
	
	//Set the entry ID
	CString csEntryID;
	csEntryID = pCurTrx->TBGetProperty(_T("PropEntryId"),true);
	psLocalUICmd->Rcpt.SetItemID(csEntryID);
	
	//Set the receipt name
	psLocalUICmd->Rcpt.SetRcptName(_T("CMSMReceipt"));
	
	//Set UI command details
	psLocalUICmd->Rcpt.eItemCmd = UI_ITEM_CMD_REMOVEITEM;

    CCustomerTBScotMsg::instance()->SendScotMsg(TB_DEV_UICOMMAND, 0, NULL,(LPARAM) psLocalUICmd);
    
	
    return true;
}

bool CCustomerMsgMap::msgCMTenderingAmount(TBSTATE &tbretState, LPARAM lparam )
{
	CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();
	CString csTenderAmount = pCurTrx->TBGetProperty(TBPROP_ENTERTENDERAMOUNT,false);
	CString csTransactionAmount = pCurTrx->TBGetProperty(_T("CM_Tendering_Amt"), true);
	CString csTenderType =  pCurTrx->TBGetProperty(_T("CM_TenderType"), true);
	CString csIsCashBack =  pCurTrx->TBGetProperty(_T("CashBackValue"), false);

	bool bCorrectTender(false);
	if ( (csTenderType.Find(_T("MEIJER CREDIT"))!= -1 ||
	      csTenderType.Find(_T("CREDIT"))!= -1 || csTenderType != _T("CASH")) && (csIsCashBack == _T("0")))
	{
	    bCorrectTender = true;
	}
	long lTenderAmount = _ttol(csTenderAmount);
	long lTransactionAmount = _ttol(csTransactionAmount);
	int iTest = pCurTrx->InAssistMode();

    tbtrace(TM_INFO, _T("msgCMTenderingAmount - CashBack flag(%s)"), csIsCashBack);
    tbtrace(TM_INFO, _T("msgCMTenderingAmount - tender amount(%d)"), lTenderAmount);
    tbtrace(TM_INFO, _T("msgCMTenderingAmount - transaction amount (%d)"), lTransactionAmount);
    tbtrace(TM_INFO, _T("msgCMTenderingAmount - in assistmode (%d)"), iTest);
    tbtrace(TM_INFO, _T("msgCMTenderingAmount - tendertype text(%s)"), csTenderType);
    tbtrace(TM_INFO, _T("msgCMTenderingAmount - valid tendertype(%d)"), bCorrectTender);

	if ((lTransactionAmount> 0) &&
	     (lTransactionAmount < lTenderAmount) &&
	     (bCorrectTender) &&
	     (!pCurTrx->InAssistMode()))
	{
	    pCurTrx->TBSetProperty(TBPROP_ENTERTENDERAMOUNT, csTransactionAmount);
	    pCurTrx->TBSetProperty(_T("CM_Tendering_Amt"), _T("0"), true);
	    pCurTrx->TBSetProperty(_T("CM_TenderType"), _T("CM_TEST"), true);
	    tbtrace(TM_INFO, _T("msgCMTenderingAmount - Changed Tendering Amount new amount(%d)"), lTransactionAmount);
	}
	else if(lTransactionAmount < 0 && lTenderAmount > 0
		&& (csIsCashBack == _T("0") || csIsCashBack == _T(""))) //POS315640/320065
	{
		//The base TBEnterTender reverse the negative to positive value but since the POS
		//is expecting a 0 tender for the negative total transaction, let's reset the tendered amount here to 0.
		tbtrace(TM_INFO, _T("Reset the tendered amount to zero."));
        pCurTrx->TBSetProperty(TBPROP_ENTERTENDERAMOUNT, _T("0"), false); 
	}
	return true;
}

bool CCustomerMsgMap::msgEnterTenderedAmount(TBSTATE &tbretState, LPARAM lparam )
{
    CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();
	CString csTenderType =  pCurTrx->TBGetProperty(_T("CM_TenderType"), true);
	CString csIsCashBack =  pCurTrx->TBGetProperty(_T("CashBackValue"), false);
	
	TBSTATE tbstate;
    if( csIsCashBack == _T("0") && csTenderType != _T("CASH"))
	{
		//CLEAR is necessary here to refresh the payment amount value in the case of
		//card tendering with promotion
		//msgSendSequence(tbstate,(LPARAM)(LPCTSTR)CString(_T("{CLEAR}")));
		//we need press F1 here for no cash back but we need to do it at tbmsgmap just to delay a little bit
		pCurTrx->SetCurrentInProgressState(_T("PAYMENT_AMOUNT_NO_CASHBACK"));
		pCurTrx->TriggerMsgMap();
	}
    else
	{	
		CString csTenderAmount = pCurTrx->TBGetProperty(TBPROP_ENTERTENDERAMOUNT,false);
		csTenderAmount += _T("{ENTER}");
		msgSendSequence(tbstate,(LPARAM)(LPCTSTR)csTenderAmount);
	}
	return true;
}

bool CCustomerMsgMap::msgOnRebateItem(TBSTATE &tbretState, LPARAM lparam)
{
    tbtraceIO(_T("msgOnRebateItem"));
    
    CString csBoolValue = (LPCTSTR) lparam;
	bool lparamBoolValue = csBoolValue == _T("1");

	CCustomerMHItem::instance()->HandleRebateItemAttributeChange(lparamBoolValue);
	if( lparamBoolValue)
		CCustomerTrxBroker::instance()->TBSetProperty(_T("RebateReceiptRequested"), _T("1"), true);

    return true;
}

bool CCustomerMsgMap::msgRestartFastlane( TBSTATE &tbretState, LPARAM lparam )
{
    tbtraceIO(_T("msgRestartFastlane"));
    const CString REBOOT_FASTLANE(_T("c:\\scot\\bin\\initsys.exe"));
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
        //before we do anything let's see if we are here because of a dr.watson    
        if(HasDrWatsonErrorOccurred())
        {
            tbtrace(TM_ENTRY, _T("Possible Dr. Watson has occurred.  Allowing FL normal exeception handling to run"));
            return true;
        }
        tbtrace(TM_ENTRY, _T("POS has exited unexpectedly"));
        pTB->SetCurrentInProgressState(_T("POS_EXIT_FASTLANE"));
        tbretState = TB_NEEDMOREDATA;
        return (msgDataNeeded(tbretState, (LPARAM)_T("RebootLane;")));
        
    }
    return true;
}
//+TAR345688 Keep FL in SelectPayment screen when Loyalty Card is scanned 
bool CCustomerMsgMap::msgScannedItemInSMFinish( TBSTATE &tbretState, LPARAM lparam )
{
	tbtraceIO(_T("msgScannedItemInSMFinish"));

	CCustomerCurrentItem *pCurItem = CCustomerCurrentItem::instance();
	CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();

	CString itemCode = pCurItem->ItemCode();
	
	CString csNSC4 = itemCode.Left(3);
	CString csNSC4_1 = itemCode.Left(4);
	
	if(csNSC4 == _T("469") || csNSC4 == _T("465") ||
	   csNSC4_1 == _T("0469") || csNSC4_1 == _T("0465"))
	{
		pCurTrx->SetCurrentInProgressState(_T("TB_LOYALTY_IN_FINISH_FLAPI"));			
	}

	msgSendSequence(tbretState,lparam);

	
	return true;
}
//-

bool CCustomerMsgMap::HasDrWatsonErrorOccurred(void)
{
   long lMatches = 0; 
   
   CArray<CString, CString> csProcessNames;

   //sleep for one second to make sure the dr watson has 
   //already started.
   Sleep(1000);
   

   if(GetProcessList(csProcessNames))
   {

      long lEnd = csProcessNames.GetSize();

      for(long iter = 0; iter < lEnd; iter++)
      {

        if( !_tcsicmp(csProcessNames[iter], _T("drwtsn32.exe")) )
        {
           return true;
        }

        if(( !_tcsicmp(csProcessNames[iter], _T("cmd.exe")) ) || 
           ( !_tcsicmp(csProcessNames[iter], _T("popup.exe")) ) ||
           ( !_tcsicmp(csProcessNames[iter], _T("getdiagfiles.exe")) )||
           ( !_tcsicmp(csProcessNames[iter], _T("getdiagfilesu.exe")) ))
        {
            lMatches++;
        }

        
        if(lMatches >= 3)
        {
            return true;
        }
      }
   }
   return false;
}


bool CCustomerMsgMap::GetProcessList(CArray<CString, CString> &csProcessNames)
{
  HANDLE hProcessSnap;
  PROCESSENTRY32 pe32;


  // Take a snapshot of all processes in the system.
  hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
  if( hProcessSnap == INVALID_HANDLE_VALUE )
  {
    tbtrace(TM_ENTRY,  _T("GetProcessList failure at CreateToolhelp32Snapshot (of processes)") );
    return( false );
  }

  // Set the size of the structure before using it.
  pe32.dwSize = sizeof( PROCESSENTRY32 );

  // Retrieve information about the first process,
  // and exit if unsuccessful
  if( !Process32First( hProcessSnap, &pe32 ) )
  {
    tbtrace(TM_ENTRY,  _T("GetProcessList failure at Process32First" ));  // Show cause of failure
    CloseHandle( hProcessSnap );     // Must clean up the snapshot object!
    return( false );
  }

  // Now walk the snapshot of processes, and
  // display information about each process in turn
  do
  {
    

    csProcessNames.Add(pe32.szExeFile);

  } while( Process32Next( hProcessSnap, &pe32 ) );

  CloseHandle( hProcessSnap );
  return( true );
}

/*
bool CCustomerMsgMap::msgAMCancelRebateReceipt(TBSTATE &tbretState, LPARAM lparam)
{
    CCustomerTBProperties & props = *CCustomerTBProperties::instance();
    CCustomerCurrentReceipt & rcpt = *CCustomerCurrentReceipt::instance();

    if( props.GetHookProperty(_T("RebateReceiptRequested")) == _T("1") && rcpt.IsPrinting() )
    {
        // printing was stalled to ensure the rebate receipts were spooled, go ahead and clear it
        rcpt.SetIsPrinting(false);
    }
    return CSolutionMsgMap::msgAMCancelRebateReceipt(tbretState, lparam);
}
*/
bool CCustomerMsgMap::msgAssistModeTrxEnd( TBSTATE &tbretState, LPARAM lparam )
{
	bool bIsEndorsementPending = false;
    bool bIsTaxSlip = false;
	CCustomerCurrentSlip* pCurSlip = CCustomerCurrentSlip::instance();

    // pos10771 - aug17
    CCustomerTrxBroker &trx = *CCustomerTrxBroker::instance();
    // e pos10771 - aug17 

	if ( pCurSlip->GetNumRemaining() > 0 )
	{
		if (pCurSlip->IsSlipPrinting())
		{
			bIsEndorsementPending = true;
            bIsTaxSlip = (pCurSlip->GetLastSlipType() == pCurSlip->SlipTypeTaxExempt);

            // pos 11009 Move this logic to the bottom as the issue in pos:10771 only can happen in the taxexempt slip - rrs
            // POS 10771 - rrs            
            /*CString csIsFromLaneEndorsement = CCustomerTrxBroker::instance()->TBGetProperty(_T("IsFromEndorsement"), false);
            long lPrev = CCustomerCurrentTransaction::instance()->GetPreviousInProgressState();
            CString csPrev = CCustomerCurrentTransaction::instance()->GetCurrentInProgressStateName(lPrev);
            tbtrace(TM_INFO, _T("PreviousInProgressState (%s)"), csPrev);
            if( ( csIsFromLaneEndorsement.IsEmpty() || csIsFromLaneEndorsement == _T("0") ) && csPrev == _T("ASSISTMODE_ENTER") ) //!_tcscmp(trx.GetCurrentInProgressStateName(),_T("ASSISTMODE_TRXEND")) )
            {
                CCustomerTrxBroker & trx = *CCustomerTrxBroker::instance();

                trx.TBSetProperty(_T("IsFromEndorsement"), _T("1"));
                trx.SetCurrentInProgressState(_T("TENDER_PAYMENT_ENDORSEMENT"));
                trx.TriggerMsgMap();
            }*/
            // E - POS 10771 - rrs            
            // E - pos 11009 Move this logic to the bottom as the issue in pos:10771 only can happen in the taxexempt slip - rrs           
		}
        //pos10771-aug17
        else
        {
            // If the attendant just came to RAP AM and the POS is still at POS_TRXEND, then we need to ignore and don't send the
            // TB_TRXEND state to the base as it will create the OOS
            long lPrev = CCustomerCurrentTransaction::instance()->GetPreviousInProgressState();
            CString csPrev = CCustomerCurrentTransaction::instance()->GetCurrentInProgressStateName(lPrev);
            tbtrace(TM_INFO, _T("Previous InProgressState (%s)"), csPrev);
            if( csPrev == _T("ASSISTMODE_ENTER") )
            {
                trx.SetCurrentInProgressState(_T("ASSISTMODE_ENTER"));
                tbtrace(TM_INFO, _T("Blocked TB_TRXEND return during msgAssistModeTrxEnd since there is endorsement data waiting to be printed."));        
                tbretState = TB_IGNORE;
                return true;
            }
        }
        //e - pos10771-aug17

        // POS 10771 - rrs
        /*tbtrace(TM_INFO, _T("Blocked TB_TRXEND return since there is endorsement data waiting to be printed."));        
        tbretState = TB_IGNORE;*/
        // E POS 10771
	}

	bool ret = CSolutionMsgMap::msgAssistModeTrxEnd(tbretState, lparam);
	
	//TAR356233 and TAR355066 - open the cash drawer immediately if it is not a check
	CString csTenderType = CCustomerTrxBroker::instance()->TBGetProperty(_T("TestTenderType"),true);
    LPCTSTR szChecks[] = { _T("CHECK"), _T("TRAVELER") };
    bool bIsACheck = (csTenderType.Find(szChecks[0]) != -1) || (csTenderType.Find(szChecks[1]) != -1);

	if (bIsEndorsementPending && (tbretState == TB_NEWCASHDRAWERSTATE) && (bIsACheck || bIsTaxSlip) )
	{
		tbtrace( TM_INFO, _T("Cash Drawer is not opening, endorsement is not complete yet!") );
		tbretState = TB_IGNORE;
	}
	else if(tbretState == TB_NEWCASHDRAWERSTATE)
    {
        // SOTF 8723 - RRS
        CString csECheckTender = trx.TBGetProperty(_T("IsECheckTender"),false);
        CString csChange = trx.TBGetProperty(_T("TrxChangeDue"), true);
	    int nChange = _ttoi(csChange);

        tbtrace( TM_INFO, _T("Try to open Cash Drawer, ECheck:(%s), Change Due:(%d)"), csECheckTender,nChange );
        if( csECheckTender == _T("1") && nChange == 0)
        {
            tbtrace( TM_INFO, _T("No need to open Cash Drawer, its echeck") );
            CCustomerCurrentCashDrawer::instance()->setExpectedCashDrawerState( CASHDRAWER_CLOSED );
            CCustomerCurrentCashDrawer::instance()->SetCashDrawerState( CASHDRAWER_CLOSED );
            //CCustomerTrxBroker::instance()->TBSetProperty(_T("IsECheckTender"),_T("0"),false);
            tbretState = TB_IGNORE;
        }
        // E - SOTF 8723 - RRS
        else
        {
            tbtrace( TM_INFO, _T("It's Ok to open the cash drawer now.") );
        }
    }
    // POS 11009 move the logic to fix pos 10771 - rrs
    else if (bIsEndorsementPending && bIsTaxSlip )
    {
        // POS 10771 - rrs            
        //CString csIsFromLaneEndorsement = CCustomerTrxBroker::instance()->TBGetProperty(_T("IsFromEndorsement"), false);
        //long lPrev = CCustomerCurrentTransaction::instance()->GetPreviousInProgressState();
        //CString csPrev = CCustomerCurrentTransaction::instance()->GetCurrentInProgressStateName(lPrev);
        //tbtrace(TM_INFO, _T("IsFromLaneEndorsement (%s)"), csIsFromLaneEndorsement);
        //CCustomerTrxBroker & trx = *CCustomerTrxBroker::instance();
        if( !_tcscmp(trx.GetCurrentInProgressStateName(),_T("ASSISTMODE_TRXEND")) )
        {
            trx.TBSetProperty(_T("IsFromEndorsement"), _T("1"));
            trx.SetCurrentInProgressState(_T("TENDER_PAYMENT_ENDORSEMENT"));
            trx.TriggerMsgMap();
        }
        // E - POS 10771 - rrs

        // POS 10771 - rrs
        tbtrace(TM_INFO, _T("Blocked TB_TRXEND return since there is endorsement data waiting to be printed."));        
        tbretState = TB_IGNORE;
        // E POS 10771
    }
    // E POS 11009 move the logic to fix pos 10771 - rrs


	return ret;
}
//TAR359317
bool  CCustomerMsgMap::msgCashDrawerSetCloseImmediate( TBSTATE &tbretState, LPARAM lparam )
{
	tbtraceIO(_T("msgCashDrawerSetCloseImmediate"));
	CString csBoolValue = (LPCTSTR) lparam;
	if(csBoolValue == _T("1"))   
	{
		CCustomerMHCashDrawer::instance()->SetCloseImmediate(true);
	}
	else
	{
		CCustomerMHCashDrawer::instance()->SetCloseImmediate(false);
	}

	return true;
}


bool  CCustomerMsgMap::msgAssistModeExit( TBSTATE &tbretState, LPARAM lparam )
{
	tbtraceIO(_T("msgAssistModeExit"));

    CCustomerTBProperties & props = *CCustomerTBProperties::instance();

    // POS31050 dont send the ITEM key during the Mobile Assistance streamline intervention
    // Do not send the Item menu key press if we're exiting assistmode 
    // as a result of a Cancel Transaction due to a partial payment.
    tbtrace(TM_INFO, _T("POSState=%s; PropIsOperGuid=%s"),m_csCurrPOSState,props.GetHookProperty(_T("PropIsOperGuid"))); // POS73665
    if( (props.GetHookProperty(_T("PropMobShopOrderPrompt")) == _T("0")) && (m_csCurrPOSState != _T("ALL_TENDERS_VOIDED")) && (m_csCurrPOSState != _T("CUSTOMER_TENDER_SELECTED")) &&  // POS73665: Changed TAR393716 or-condition (||) to an and-condition (&&) which makes better sense.
        (m_csCurrPOSState != _T("PAYMENTMENU_DK1")) && // POS108460
		(m_csCurrPOSState != _T("GIFTCARD_TRANS")) && (m_csCurrPOSState.Find(_T("WAITING_ON_PINPAD")) == -1) && (m_csCurrPOSState != _T("SIGNIN_MENU")) // POS297520
      ) 
    {
        tbtrace(TM_INFO, _T("Sending {ITEM}")); // POS73665
        CString csKeyPress= _T("{ITEM}");
        CCustomerTBConnectPOS::instance()->SendSequence(csKeyPress);         
    }

    return true;
}

//************************************************************************ 
// Function:    msgOnGiftReceipt
// 
// Purpose:     Keep count of gift receipts to print
// 
// Parameters:	1	- gift receipt requested
//			0	- print gift receipts at trx end			
//************************************************************************ 
bool  CCustomerMsgMap::msgOnGiftReceipt( TBSTATE &tbretState, LPARAM lparam )
{
	tbtraceIO(_T("msgOnGiftReceipt"));
	CCustomerTrxBroker* pTB = CCustomerTrxBroker::instance();
	CString csPropValue = pTB->TBGetProperty(_T("PrintGiftReceipt"), true);
	int nGiftReceipts = _ttoi(csPropValue);

	CString csBoolValue = (LPCTSTR) lparam;
	if(csBoolValue == _T("0"))   
	{
		CString csSendKey = _T("{TRANS}{F6}");
		while(nGiftReceipts > 0)
		{
			nGiftReceipts--;
			msgSendSequence(tbretState,(LPARAM)(LPCTSTR)csSendKey);
		}
		pTB->TBSetProperty(_T("PrintGiftReceipt"), _T("0"), true);
		csSendKey = _T("{ITEM}");
		msgSendSequence(tbretState,(LPARAM)(LPCTSTR)csSendKey);
	}
	else
	{
		nGiftReceipts++;
		csPropValue.Format(_T("%d"), nGiftReceipts);
		pTB->TBSetProperty(_T("PrintGiftReceipt"), csPropValue, true);
	}
	return true;
}

bool  CCustomerMsgMap::msgSendSequenceIfRemote( TBSTATE &tbretState, LPARAM lparam )
{
	tbtraceIO(_T("msgSendSequenceIfRemote"));

    if( CCustomerTrxBroker::instance()->InRemoteAssistMode() )
    {
        tbtrace(TM_INFO, _T("msgSendSequence:%s"), (LPCTSTR)lparam);
        CCustomerTBConnectPOS::instance()->SendSequence(CString((LPCTSTR) lparam));
    }
    else
    {
        tbtrace(TM_INFO, _T("Not in remote assist mode, ignoring button press %s"), (LPCTSTR)lparam);
    }

    return true;
}

bool CCustomerMsgMap::msgVoidRewardLine( TBSTATE &tbretState, LPARAM lparam )
{
	tbtraceIO(_T("msgVoidRewardLine"));

	//
	// Extract the id of the message from the parameter
	//
	// Get all the variables
	LPCTSTR pszMsg = (LPCTSTR)lparam;
	ASSERT(pszMsg && *pszMsg);

	PSMessageInfo pMsg = new SMessageInfo;
	ZeroMemory(pMsg, sizeof(SMessageInfo));
	
    // Build string MSGx, where x represents the entry ID of the MSG to be voided
    CString csMsg;
    csMsg.Format(_T("%s%s"), _T("VOIDMSG"), pszMsg);

	CopyString(pMsg->szE_ReceiptText, csMsg);
	CopyString(pMsg->szRAP_MessageText, csMsg);
				
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
    return true;
}

bool CCustomerMsgMap::msgSendSequenceWithWaits( TBSTATE &tbretState, LPARAM lparam )
{
    tbtraceIO(_T("msgSendSequenceWithWaits"));
    
    LPCTSTR pszMsg = (LPCTSTR)lparam;
    ASSERT(pszMsg && *pszMsg);
    
    // expecting the following parameters
    // -----------------------------------
    // 1) milliseconds to wait before SendSequence set (max 5000 ms)
    // 2) SendSequence command string
    // 3) milliseconds to wait after sendSequence set (max 5000 ms)
    LPTSTR pszParams = NULL;
    CString strWaitBefore = _T(""); 
    CString strSendSequence = _T("");
    CString strWaitAfter = _T("");
    DWORD dwWaitBefore = 0;
    DWORD dwWaitAfter = 0;
    LPCTSTR psz;

    CopyString(pszParams, pszMsg);
    
    // Get all the variables
    psz = _tcstok(pszParams, _T(";"));
    if( psz )
    {
        strWaitBefore = psz;
        if( strWaitBefore.GetLength() )
        {
            strWaitBefore.TrimLeft();
            strWaitBefore.TrimRight();
            dwWaitBefore = (DWORD)min(_ttol(strWaitBefore), 5000);
        }
        psz = _tcstok(NULL, _T(";"));
        if( psz )
        {
            strSendSequence = psz;
            psz = _tcstok(NULL, _T(";"));
            if( psz )
            {
                strWaitAfter = psz;
                if( strWaitAfter.GetLength() )
                {
                    strWaitAfter.TrimLeft();
                    strWaitAfter.TrimRight();
                    dwWaitAfter = (DWORD)min(_ttol(strWaitAfter), 5000);
                }
            }
        }
    }
    
    delete [] pszParams;

    if( dwWaitBefore )
    {
        tbtrace(TM_INFO, _T("Waiting %u ms before sending inputs..."), dwWaitBefore);
        Sleep(dwWaitBefore);
    }
    bool bRet = msgSendSequence(tbretState, (LPARAM)(LPCTSTR)strSendSequence);
    if( dwWaitAfter )
    {
        tbtrace(TM_INFO, _T("Waiting %u ms after sending inputs..."), dwWaitAfter);
        Sleep(dwWaitAfter);
    }

    return bRet;
}

bool CCustomerMsgMap::msgAddRewardLine( TBSTATE &tbretState, LPARAM lparam )
{
	tbtraceIO(_T("msgAddRewardLine"));

	//Commented the code below as it is already taken care of by the 
	//changes in CBaseMsgMap::dispatchMsgHandler that prevents from multiple handling of cSTATE_ANY
	//+POS291204
/*	CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();
    if(pCurTrx->TBGetProperty(_T("PropIsItNewPosState")) != _T("1") )
	{
		tbtrace(TM_INFO, _T("Ignoring duplicate call for rewardline."));
		return true;
	}
	pCurTrx->TBSetProperty(_T("PropIsItNewPosState"), _T("0")); */
    //-POS291204
    
    LPCTSTR pszMsg = (LPCTSTR)lparam;
    ASSERT(pszMsg && *pszMsg);
    
    // Solution level msgAddRewardline with second parameter
    // used to specify the bGoBackScanAndBag flag
    // it will continue to default to TRUE, unless 0 is specified
    LPTSTR pszParams = NULL;
    CString csRewardLine = _T(""); 
    CString csGoBack = _T("");
    bool bGoBack = true;
    LPCTSTR psz;

    CopyString(pszParams, pszMsg);
    
    // Get all the variables
    psz = _tcstok(pszParams, _T(";"));
    if( psz )
    {
        csRewardLine = psz;
        psz = _tcstok(NULL, _T(";"));
        if( psz )
        {
            csGoBack = psz;
            if( csGoBack == _T("0") )
            {
                bGoBack = false;
                tbtrace(TM_INFO, _T("Reward line requests NOT to go to S&B after it's added."));
            }
        }
    }
        
    delete [] pszParams;

	PSMessageInfo pMsg = new SMessageInfo;
	ZeroMemory(pMsg, sizeof(SMessageInfo));

	if(csRewardLine.Find(_T("Rate=")) != -1)//This must be a reward line for Team Member Discount
	{
		CCustomerTBProperties & props = *CCustomerTBProperties::instance();
		if(props.GetHookProperty(_T("TMDDiscountPercent")) == _T("")) //TMD discount is blank
		{
			//TMD discount is blank, strip off Rate=%% from reward line
			csRewardLine.Replace(_T("Rate=%%"), _T(""));
			tbtrace( TM_INFO, _T("TMD discount is blank. Reward line after modifying = %s"),csRewardLine);
		}
	}
	
	CopyString(pMsg->szE_ReceiptText, (LPCTSTR)csRewardLine);
	CopyString(pMsg->szRAP_MessageText, (LPCTSTR)csRewardLine);
				
	pMsg->bHiliteReceiptText = false;
    pMsg->bNeedItemCancel = true;
    pMsg->bApprovalNeeded = false;
    pMsg->bGoBackScanAndBag = bGoBack;
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


bool CCustomerMsgMap::msgDecideIfTenderAccepted( TBSTATE &tbretState, LPARAM lparam )
{
    CCustomerCurrentTender* pCurr = CCustomerCurrentTender::instance();
    
    CString csTenderDesc(pCurr->TenderDescription());
    if((csTenderDesc.IsEmpty()) && (pCurr->TenderAmountDbl() == 0))
    {
        tbtrace(TM_INFO, _T("msgDecideIfTenderAccepted() Tender is empty doing no work"));
        tbretState = TB_IGNORE;

        return true;
            
    }

    return msgTenderAccepted(tbretState, lparam);
}

bool CCustomerMsgMap::msgResetToLastPOSState(TBSTATE &tbretState, LPARAM lparam)
{
	tbtraceIO(_T("msgResetToLastPOSState"));

    resetToLastPOSState();

    return true;
}

bool CCustomerMsgMap::msgReturnCodeHandler(TBSTATE &tbretState, LPARAM lparam)
{
	tbtraceIO(_T("msgReturnCodeHandler"));
    CCustomerTrxBroker & trx = *CCustomerTrxBroker::instance();

    long rc = trx.GetTBSTATEFromName((LPCTSTR) lparam);

    switch(rc)
    {
    case TB_TRXEND:
        {
            if( CCustomerCurrentSlip::instance()->GetNumRemaining() > 0 )
            {
                tbtrace(TM_INFO, _T("Blocked TB_TRXEND return since there is endorsement data waiting to be printed."));
                if( !_tcscmp(trx.GetCurrentInProgressStateName(),_T("TENDER_PAYMENT")) )
                {
                    trx.TBSetProperty(_T("IsFromEndorsement"), _T("1"));
                    trx.SetCurrentInProgressState(_T("TENDER_PAYMENT_ENDORSEMENT"));
                    trx.TriggerMsgMap();
                }
                tbretState = TB_IGNORE;
            }
            else
            {
                tbretState = TB_TRXEND;
            }
        }
        break;
    };
    return true;
}

bool CCustomerMsgMap::msgDataNeeded(TBSTATE &tbretState, LPARAM lparam)
{
	CString csCurr = CCustomerTrxBroker::instance()->GetCurrentInProgressStateName();
	
	//+ POS253532
    CString csProperty = _T("IsResumeTrxInProgress");
	CString strPropValue;
    strPropValue = CCustomerTrxBroker::instance()->TBGetProperty(csProperty, true);
    tbtrace( TM_INFO,_T("strPropValue = %S"),strPropValue);
	//- POS253532

    if ( csCurr.Find(_T("ASSISTMODE_")) != -1)
    {
        tbtrace( TM_WARNING, _T("CCustomerMsgMap::msgDataNeeded-- TB_NEEDMOREDATA not allowed in assist mode'"));

		tbretState = TB_IGNORE;

		return true;     
	}
	//+POS296302 - aab
	CString csIsBagging = CCustomerTrxBroker::instance()->TBGetProperty(_T("IsInBagAndEAS"));

    if(m_csCurrPOSState == _T("DO_YOU_WANT_MPERKS") && csIsBagging == _T("1")) 
    {
		tbtrace( TM_INFO, _T("CCustomerMsgMap::msgDataNeeded-- DO_YOU_WANT_MPERKS display is hold for now until after bagging."));
		
		tbretState = TB_IGNORE;
		CCustomerTrxBroker::instance()->SetCurrentInProgressState(_T("ITEMIZATION")); //set it back
		
		return true;
	}
    //-POS296302 - aab
    
	//+ POS253532
    tbtrace(TM_INFO,_T("CurrentTBprogressstate = %s, strPropValue = %s"),CCustomerTrxBroker::instance()->GetCurrentInProgressStateName(),strPropValue);
    CString cstr = CCustomerTrxBroker::instance()->GetCurrentInProgressStateName();

    if ((cstr == _T("TB_DIGITAL_COUPON_PIN_ENTRY")) && (strPropValue == _T("1")))
    {
        tbtrace( TM_INFO,_T("IsFromResumeTrx is set to true"));
        CCustomerTrxBroker::instance()->TBSetProperty(_T("IsFromResumeTrx"), _T("1"), true); 
    }
	//- POS253532
	
    //POS82276
    if(m_csCurrPOSState == _T("PRICEINQUIRY_MENU")) 
    {
        // set the TB to not print any leftover receipt at all.
        CCustomerCurrentReceipt::instance()->SetPrintWhenFinished(false);        
    }
    // e POS82276

    //POS48277 Mobile PayStation
    CCustomerTBProperties *pTBProperties = CCustomerTBProperties::instance();
    if( pTBProperties->GetProperty(_T("FastLaneConfiguration")) == _T("MobilePayStation") ) 
    {
        if(m_csCurrPOSState == _T("MOB_SHOP_ORDER_PENDING_MSG")) 
        {
             tbtrace( TM_INFO, _T("CCustomerMsgMap::msgDataNeeded-- TB_NEEDMOREDATA for MobileOrderprompt is bypassed'"));
             tbretState = TB_IGNORE;

             CCustomerTrxBroker::instance()->SetCurrentInProgressState(_T("ITEMIZATION"));  //POS77912

             CString csKeyPress= _T("{F1}");
             CCustomerTBConnectPOS::instance()->SendSequence(csKeyPress);  
             
             return true;
        }
    }
    // e POS48277 Mobile PayStation


	return CSolutionMsgMap::msgDataNeeded(tbretState,  lparam);
}

bool CCustomerMsgMap::msgHandleWicBalanceExceeded(TBSTATE &tbretState, LPARAM lparam)
{
    tbtraceIO(_T("msgHandleWicBalanceExceeded"));
    CCustomerTBProperties & props = *CCustomerTBProperties::instance();
    CBaseMessages & msgs = *CBaseMessages::instance();

    // Two new possible messages for WIC CVV:
    // 1251=WIC balance exceeded. %a due.
    // 1252=NON WIC Balance Due is %a
    CString csDue = _T("");
    LPCTSTR pszStart = _T("exceeded.");
    LPCTSTR pszEnd = _T("due.");
    CString csProp = props.GetHookProperty(_T("OG_Guidance_Prop"));
    CString csNum = props.GetHookProperty(_T("OG_MsgNum_Prop"));
    if( !csNum.Compare(_T("1251")) )
    {
        pszStart = _T("exceeded.");    
        pszEnd = _T("due.");
        
        int iStart = csProp.Find(pszStart);
        int iEnd = csProp.Find(pszEnd);
        if( iStart != -1 && iEnd != -1 )
        {
            iStart = iStart + _tcslen(pszStart);
            if( iEnd > iStart )
            {
                csDue = csProp.Mid(iStart, (iEnd - iStart));
                csDue.TrimLeft();
                csDue.TrimRight();
            }
        }
    }
    else if( !csNum.Compare(_T("1252")) )
    {
        pszStart = _T("Due is ");
        int iStart = csProp.Find(pszStart);
        if( iStart != -1 )
        {
            iStart = iStart + _tcslen(pszStart);
            if( csProp.GetLength() > iStart )
            {
                csDue = csProp.Mid(iStart);
                csDue.TrimLeft();
                csDue.TrimRight();
            }
        }
    }


    if( csDue.IsEmpty() )
    {
        csDue = _T("N/A");
    }

    //POS45465 - rrs
    CCustomerTrxBroker::instance()->TBSetProperty(_T("WICBalanceExceeded"), csDue);
    //e - POS45465 - rrs


    CString csMsg = msgs.GetStringValueFromID(9239);
    csMsg.Format(csMsg, csDue);

    CString csDataNeededCommand;
    csDataNeededCommand.Format(_T("TOPCAPTION=%s;DETAILEDINSTRUCTION=%s;SUMMARYINSTRUCTION=%s;SCANNERINPUTENABLED=0;LSWND1=%s;LSWND2=%s;LANEBUTTONTEXT=%s;BUTTON0=%s;BUTTON1=%s;STORELOGON=%s;"),
                                   msgs.GetStringValueFromID(9238),                 /*TOPCAPTION*/
                                   _T(" "),                                         /*DETAILEDINSTRUCTION*/
                                   csMsg,                                           /*SUMMARYINSTRUCTION*/
                                   _T(" "),                                         /*LSWND1*/                                  
                                   _T(" "),                                         /*LSWND2*/
                                   msgs.GetStringValueFromID(9238),                 /*LANEBUTTONTEXT*/
                                   // _T("cYES,") + msgs.GetStringValueFromID(62),     /*BUTTON0*/
                                   _T("$ScotApp_62$"),     /*BUTTON0*/ // SSCOI-45374
                                   //_T("cNO,") + msgs.GetStringValueFromID(63),      /*BUTTON1*/
                                   _T("$ScotApp_63$"),      /*BUTTON1*/ // SSCOI-45374
                                   _T("0"));                                        /*STORELOGON*/
                                    
    SDataNeededView MyDataNeeded={0};
    
    MyDataNeeded.szView = csDataNeededCommand.GetBuffer(0);
    CCustomerTBConnectFL::instance()->SetDataNeededView(&MyDataNeeded);

    tbretState = TB_NEEDMOREDATA;
    return true;
}

bool CCustomerMsgMap::msgDetectAndHandleNewRestrictedItem(TBSTATE &tbretState, LPARAM lparam)
{
    tbtraceIO(_T("msgDetectAndHandleNewRestrictedItem"));

    CCustomerTBProperties & props = *CCustomerTBProperties::instance();
    CCustomerTrxBroker & trx = *CCustomerTrxBroker::instance();

    if( props.GetHookProperty(_T("IsResumeTrxInProgress")) == _T("1") )
    {
        //trx.SetCurrentInProgressState(_T("ITEMIZATION"));
        tbretState = TB_NEEDIMMEDATEAGECHECK;
    }
    return true;
}

bool CCustomerMsgMap::msgCheckIfCouponToBeCollectedAndDisplay(TBSTATE &tbretState, LPARAM lparam)
{
    tbtraceIO(_T("msgCheckIfCouponToBeCollectedAndDisplay"));

	CString csLastItem = CCustomerMHItem::instance()->m_csPrevItemCode;
	CCustomerMHOptions *pOptions = CCustomerMHOptions::instance(); 
	CCustomerTBProperties *pTBprop = CCustomerTBProperties::instance();
	

	CString csNSC4Acct = pTBprop->GetHookProperty(_T("NSC4Account"));
	CString csNSC4Mnemonic = pTBprop->GetHookProperty(_T("NSC4Mnemonic"));
	CString csRewardLine = csNSC4Mnemonic + _T(" -- ") + csNSC4Acct;
	CString csRewardLineNoScanAndBag = csRewardLine + _T(";0");


    //POS36319 --
	if( (csNSC4Mnemonic == pOptions->OptionValue(_T("NSC4Mnemonic"))) )
	{
        if( (pOptions->BoolOptionValue(_T("CollectCMCoupons"))))
        {

            //POS41125 - don't go back to scan and bag only if need assistance to collect coupon
            msgAddRewardLine(tbretState, (LPARAM)(LPCTSTR)csRewardLineNoScanAndBag);

            msgDataNeededWithCustomerScreen(tbretState, (LPARAM)(LPCTSTR)_T("GetCoupon;9146"));
		    CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_NEEDMOREDATA, NULL, NULL);
        }
        else
        {
            //POS41125 - go back to scan and bag if the system is set to insert the nsc4 type coupon and in the assist menu
            CCustomerTrxBroker *pCurTrx = CCustomerTrxBroker::instance();
            if( pCurTrx->InAssistMenu() || pCurTrx->InAssistMode() )
            {
		        msgAddRewardLine(tbretState, (LPARAM)(LPCTSTR)csRewardLine);
            }
            else
            {
                msgAddRewardLine(tbretState, (LPARAM)(LPCTSTR)csRewardLineNoScanAndBag);
            }
            // e POS41125

            // POS36319 - Display the Insert coupon for the NSC4 promo media only
            CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_INSERT_MEDIA_ITEM, NULL, NULL);	
        
        }
		//CCustomerTrxBroker::instance()->SetCurrentInProgressState(_T("ITEMIZATION_GET_COUPON_IP"));
		
		
		return true;
	}else
	{
        //go back to scan and bag
		msgAddRewardLine(tbretState, (LPARAM)(LPCTSTR)csRewardLine);        
	}
    //e POS36319
	return true;
}

//POS36319
bool CCustomerMsgMap::msgCheckForMediaCollection(TBSTATE &tbretState, LPARAM lparam)
{
    tbtraceIO(_T("msgCheckForMediaCollection"));

    CCustomerMHOptions *pOptions = CCustomerMHOptions::instance(); 

	if( pOptions->BoolOptionValue(_T("CollectCMCoupons")) )
	{
		msgDataNeededWithCustomerScreen(tbretState, (LPARAM)(LPCTSTR)_T("NegativeMedia;9070"));
		CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_NEEDMOREDATA, NULL, NULL);		
		
		return true;
	}
    else
	{        
        tbtrace(TM_ENTRY, _T("Display InsertCoupon screen"));

		if(m_csCurrPOSState == _T("NEGATIVE_MEDIA_AT_PAYMENT_COLLECTION")) 
		{
			tbtrace(TM_INFO, _T("Manually changing POSState to ITEMMENU_DK1"));
			dispatchHelper(_T("ITEMMENU_DK1")); //SSCOI-44258 - change the POSState back to ITEMMENU_DK1 to prevent from sending repeated TB_INSERT_MEDIA_ITEM
		}

        CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_INSERT_MEDIA_ITEM, NULL, NULL);	
	}
	return true;
}
//e POS36319

bool CCustomerMsgMap::msgImmediateOrDelayedIntervention(TBSTATE &tbretState, LPARAM lparam)
{
	bool bDelayedIntervention = CCustomerMHItem::instance()->GetACSDelayedAgeRestricted();

	if(bDelayedIntervention)
	{
        CCustomerTrxBroker *pCurTrx = CCustomerTrxBroker::instance();
		tbretState = TB_IGNORE;
		
        CString csResumeStarted = CCustomerTBProperties::instance()->GetHookProperty(_T("IsResumeStarted"));
		
        CString csAgeRequired = CCustomerTBProperties::instance()->GetHookProperty(_T("RestrictedAge_Prop"));

        if((csResumeStarted != _T("1")) && 
           (CCustomerMHOptions::instance()->BoolOptionValue(_T("PromptCustomerForLegalAge"))) &&
           (_ttol(csAgeRequired) > CCustomerMHItem::instance()->GetHighestAgeRequired()))
        {
            pCurTrx->SetCurrentInProgressState(_T("PROMPT_CUSTOMER_LEGAL_AGE"));
            pCurTrx->TriggerMsgMap();

        }else
        {
            tbtrace(TM_ENTRY, _T("msgImmediateOrDelayedIntervention() sending DOB not needed to the POS"));
        
            pCurTrx->SetCurrentInProgressState(_T("ITEMIZATION"));
			
			CString csDOBEntry = CCustomerTBProperties::instance()->GetHookProperty(_T("DOBEntry")); // SSCOI-45255
		    CString csSendKey = _T("{F3}");

			// SSCOI-45255
			if ( csDOBEntry == _T("1"))
				csSendKey = _T("{F4}");
		    
		    return msgSendSequence(tbretState,(LPARAM)(LPCTSTR)csSendKey);
        }
	
		
	}

	return true;

}

// +SOTF6400 - Digital Couponing
bool CCustomerMsgMap::msgFormatPromoNumber (TBSTATE &tbretState, LPARAM lparam)
{
	CString csCustomerNumber ((LPCTSTR)lparam);
	CString csFormattedPhone (csCustomerNumber.Right (10)); // Remove any long distance "1" prefix

	CCustomerTBProperties::instance()->SetProperty(_T("DigitalCouponCustomerNumber"), csFormattedPhone);

	return true;
}

bool CCustomerMsgMap::msgDynamicCustomerDigitalCouponMessageScreen (TBSTATE &tbretState, LPARAM lparam)
{
	// Catch in testing if APMSG no longer is formatted as expected.
	// Original messages are:
	// 1404 1 Valid phone number xxxx%a already entered.
	CString csMsg ("Error in APMSG"); 

	CString csParam ((LPCTSTR) lparam);
	int iIndexBegin = csParam.Find (_T("xxxx"), 0); // lparam should contained a masked number
	if (iIndexBegin >= 0)
	{
		int iIndexEnd = csParam.Find (_T(" "), iIndexBegin);
		if ((iIndexEnd < 0) || (iIndexEnd == iIndexBegin))
			iIndexEnd = csParam.GetLength ();

		csMsg.Format (CBaseMessages::instance()->GetStringValueFromID (9255), csParam.Mid (iIndexBegin, iIndexEnd - iIndexBegin));
	}

    CString strAudio = _T("blank.wav");
    SetupMessageScreen(csMsg, 9161, strAudio, true, true);
	
	tbretState = TB_MESSAGESCREEN;

    return true;
}
// -SOTF6400


/*
Description:
   Parse a currency string into an unsigned LONG containing the number of smallest 
   currency units described by the string value. 
   
Comments:
   Fractional values are converted to whole numbers, so that "1,234.56" becomes 123456. 
   Input values that would be too large or too small to fit into a LONG will trigger a 
   return of LONG_MAX or LONG_MIN. Other errors will trigger a return of LONG_MAX.

   Currently, the separator character and the decimal character are both treated the 
   same as white space (i.e., ignored). This may change in the future, if necessary.

   The currency symbol must be removed from the input string. There is no capability 
   for interpreting, ignoring, or otherwise handling currency symbols.

Return:
   LONG value of currency amount in smallest unit (e.g., cents for USD), 
   or LONG_MAX or LONG_MIN on error
*/
LONG ParseCurrencyString(LPCTSTR src, TCHAR sepChar, TCHAR decChar)
{
   if (!src)
   {
      /* ERROR: NULL input string */
      return LONG_MAX;
   }

   if (sepChar == decChar)
   {
      /* ERROR: separator character same as decimal character */
      return LONG_MAX;
   }

   TCHAR posSign = _T('+');
   TCHAR negSign = _T('-');

   size_t srcLen = _tcslen(src);

   LONG retVal = LONG_MAX;
   LPCTSTR p = src;

   bool isNum = false;

   typedef std::vector<TCHAR> CharBuf;
   CharBuf buf(srcLen + 1);
   CharBuf::size_type bufIndex = 0;

   /* Scan for sign before number, decimal, or separator */
   while (*p && !isNum && bufIndex < srcLen)
   {
      if (*p != sepChar && *p != decChar)
      {
         /* Place digits or sign characters in the buffer */
         if (_istdigit(*p) || *p == posSign || *p == negSign)
         {
            buf[bufIndex] = *p;
            ++bufIndex;
            /* Found a digit, so proceed to next loop. */
            isNum = true;
         }
         /* Ignore white space, all other characters will halt parsing. */
         else if (!_istspace(*p))
         {
            /* Found a non-space, non-digit character. Proceed to next loop, leaving 
            isNum set to false. */
            break;
         }
      }

      ++p;
   }

   /* Once a number, decimal, or separator has been found, signs are treated as non-space. */
   while (*p && isNum && bufIndex < srcLen)
   {
      if (*p != sepChar && *p != decChar)
      {
         /* Place digits in the buffer */
         if (_istdigit(*p))
         {
            buf[bufIndex] = *p;
            ++bufIndex;
         }
         /* Ignore white space, all other characters will halt parsing. */
         else if (!_istspace(*p))
         {
            /* Stop this loop */
            isNum = false;
         }
      }

      ++p;
   }

   TCHAR const* convertBuf = &buf[0];

   retVal = _tcstol(convertBuf, 0, 10);
   return retVal;
}


//SOTF 7701
bool CCustomerMsgMap::msgGiftPhonePriceRetry(TBSTATE &tbretState, LPARAM lparam)
{
    CCustomerTBProperties *pTBProperties = CCustomerTBProperties::instance();
    CBaseMessages *pBasemsgs = CBaseMessages::instance();


    tbretState = TB_NEEDMOREDATA;
    if(m_xmlPOSDisplay)
    {
        CString csTemp = pTBProperties->GetProperty(_T("POSDisplay") );
        tbtrace(TM_DEBUG, _T("msgGiftPhonePriceEntrySetup() XML data %s"), csTemp );


        m_xmlPOSDisplay->async = VARIANT_FALSE;
        if( m_xmlPOSDisplay->loadXML( (_bstr_t) pTBProperties->GetProperty(_T("POSDisplay") )) != VARIANT_TRUE )
	    {
		    return  (msgDataNeeded(tbretState, (LPARAM)_T("GiftPhonePriceRetry;")));
		  
	    }
    }

    //Grab the second leadthru text to see if there are any restrictions
    //like limits or balances to display
    CString csPath  = _T("message/fields/field[@name = \"LTT2\"]");
    MSXML2::IXMLDOMNodePtr  xmlTestnode = m_xmlPOSDisplay->selectSingleNode(_bstr_t(csPath));
    CString csCardLimits;

	if (xmlTestnode != NULL)
	{

		_variant_t varNodeValue = xmlTestnode->Gettext(); 
        csCardLimits = varNodeValue.bstrVal;
        csCardLimits.TrimLeft();
        csCardLimits.TrimRight();
        csCardLimits.MakeUpper();
		
	}

    double fpMin = 0;
    double fpMax = 0;

    if(csCardLimits.Find(_T("RANGE")) != -1)
    {
        int rangeStart = csCardLimits.Find(_T(":")) + 1;
        CString range = csCardLimits.Mid(rangeStart);
        int rangeSep = range.Find(_T("-"));

        CString minAmountString = range.Mid(0, rangeSep);
        CString maxAmountString = range.Mid(rangeSep + 1);

        LONG minAmountCents = ParseCurrencyString(minAmountString, _T(','), _T('.'));
        LONG maxAmountCents = ParseCurrencyString(maxAmountString, _T(','), _T('.'));

        fpMin = minAmountCents / 100.0;
        fpMax = maxAmountCents / 100.0;
    }
    else
    {
        return  (msgDataNeeded(tbretState, (LPARAM)_T("GiftPhonePriceRetry;")));
    }

    CString csDataNeededFormatString;
    csDataNeededFormatString = _T("TOPCAPTION=%s;SUMMARYINSTRUCTIONS=%s;INPUTMASK=9999v99;SCANNERINPUTENABLED=0;LSWND1=%s;LSWND2=%s;LANEBUTTONTEXT=%s;STORELOGON=%s;");

    CString csSummaryInstructions;
    csSummaryInstructions.Format(_T("%s\\n%s $%.2f %s $%.2f"),pBasemsgs->GetStringValueFromID(9267), pBasemsgs->GetStringValueFromID(9269), fpMin, pBasemsgs->GetStringValueFromID(9270), fpMax);

    CString csDataNeededCommand;
    csDataNeededCommand.Format(    csDataNeededFormatString,                                     
                                   _T("$ScotApp_9038$"),                                     /*TOPCAPTION*/                                   
                                   csSummaryInstructions,                           /*SUMMARYINSTRUCTIONS*/                                
                                   _T(" "),                                         /*LSWND1*/                                  
                                   _T(" "),                                         /*LSWND2*/
                                   _T("GiftPhonePrice"),                            /*LANEBUTTONTEXT*/
                                   _T("0")                                          /*STORELOGON*/
                                   );                                      
                                    
    SDataNeededView MyDataNeeded={0};
    
    MyDataNeeded.szView = csDataNeededCommand.GetBuffer(0);
    CCustomerTBConnectFL::instance()->SetDataNeededView(&MyDataNeeded);

    return true;
}
//sotf

//SOFT 7042 reloadable mall card
bool CCustomerMsgMap::msgGiftPhonePriceEntrySetup(TBSTATE &tbretState, LPARAM lparam)
{
    
    CCustomerTBProperties *pTBProperties = CCustomerTBProperties::instance();
    CBaseMessages & msgs = *CBaseMessages::instance();
    bool bSelectableAmountList = false;
    bool bPOSDisplayLoadFailed = false;

    tbretState = TB_NEEDMOREDATA;

    if(m_xmlPOSDisplay)
    {
        CString csTemp = pTBProperties->GetProperty(_T("POSDisplay") );
        tbtrace(TM_DEBUG, _T("msgGiftPhonePriceEntrySetup() XML data %s"), csTemp );


        m_xmlPOSDisplay->async = VARIANT_FALSE;
        if( m_xmlPOSDisplay->loadXML( (_bstr_t) pTBProperties->GetProperty(_T("POSDisplay") )) != VARIANT_TRUE )
	    {
            bPOSDisplayLoadFailed = true;
		    		  
	    }

    }

    CString csButtonLabel;
    CString csDataNeededFormatString;
    csDataNeededFormatString = _T("TOPCAPTION=%s;DETAILEDINSTRUCTION=%s;SUMMARYINSTRUCTION=%s;VIEWTYPE=Alpha;CONTEXTTODISPLAY=CmDataEntry5Vertical;SCANNERINPUTENABLED=0;LSWND1=%s;LSWND2=%s;LANEBUTTONTEXT=%s;STORELOGON=%s;BUTTON7=cGoBack,%s");

    CString csPath;
    MSXML2::IXMLDOMNodePtr xmlTestnode;    
    CString csTypeOfPreAuth;
    CString csCardLimits;

    //Grab the second title to see if this is a reload or new activation
    csPath  = _T("message/fields/field[@name = \"Title2\"]"); 
    xmlTestnode = m_xmlPOSDisplay->selectSingleNode(_bstr_t(csPath));

	if (xmlTestnode != NULL)
	{

		_variant_t varNodeValue = xmlTestnode->Gettext(); 
        csTypeOfPreAuth = varNodeValue.bstrVal;
        csTypeOfPreAuth.TrimLeft();
        csTypeOfPreAuth.TrimRight();
        csTypeOfPreAuth.MakeUpper();
		
	}
    
    //Grab the second leadthru text to see if there are any restrictions
    //like limits or balances to display
    csPath  = _T("message/fields/field[@name = \"LTT2\"]");
    xmlTestnode = m_xmlPOSDisplay->selectSingleNode(_bstr_t(csPath));

	if (xmlTestnode != NULL)
	{

		_variant_t varNodeValue = xmlTestnode->Gettext(); 
        csCardLimits = varNodeValue.bstrVal;
        csCardLimits.TrimLeft();
        csCardLimits.TrimRight();
        csCardLimits.MakeUpper();
		
	}

    //loop through the xml screen data to get all the denominations on the POS' buttons
    //and add them to the DataNeeded command string to send to FL
    for(int i = 1; i<= 7; i++)
    {
        CString csBtnText = _T("");
        
        csButtonLabel.Format(_T("DK%d_Label"), i);
        
	    csPath.Format(_T("message/fields/field[@name = \"%s\"]"), csButtonLabel); 
	    _bstr_t test = _bstr_t(csPath);
        
        
        MSXML2::IXMLDOMNodePtr xmlTestnode = m_xmlPOSDisplay->selectSingleNode(_bstr_t(csPath));

	    if (xmlTestnode != NULL)
	    {

		    _variant_t varNodeValue = xmlTestnode->Gettext(); 
            csBtnText = varNodeValue.bstrVal;
		    
	    }
               
        
        csBtnText.TrimLeft();
        csBtnText.TrimRight();

        if(!csBtnText.IsEmpty())
        {   
            CString csTemp = _T("");
            bSelectableAmountList = true;
            csTemp.Format(_T(";BUTTON%d=cButton%d,$%s"), i,i, csBtnText);
            csDataNeededFormatString += csTemp;
        }
        
    }

    csDataNeededFormatString += _T(";");
    
    tbtrace(TM_ENTRY, _T("msgGiftPhonePriceEntrySetup() Dyanamic Selectable Amount list is %s"), (bSelectableAmountList ? _T("TRUE"): _T("FALSE")) );


    if(!bSelectableAmountList)
    {
        return (msgDataNeeded(tbretState, (LPARAM)_T("GiftPhonePrice;")));
    }
    
    CString csTopCaptionID = _T("%9259"); // SSCOI-45374
    CString csSummaryInstructions = msgs.GetStringValueFromID(9260);

    if(csTypeOfPreAuth == _T("ACTIVATION"))
    {
        csTopCaptionID = _T("$ScotApp_9261$"); // SSCOI-45374
    }
    
    if(csTypeOfPreAuth == _T("RELOAD"))
    {
        csTopCaptionID = _T("$ScotApp_9262$"); // SSCOI-45374
    }
    
    /* +SOTF8442 */
    double fpLimit = 0;
    double fpBalance = 0;
    double fpMin = 0;
    double fpMax = 0;
    /* -SOTF8442 */

    if(csCardLimits.Find(_T("BALANCE")) != -1)
    {
        LONG balanceCents = ParseCurrencyString(csCardLimits.Mid(csCardLimits.ReverseFind(_T(':')) + 1), _T(','), _T('.'));
        fpBalance = balanceCents / 100.0;
    }
    
    if(csCardLimits.Find(_T("LIMIT")) != -1) 
    {
        LONG balanceCents = ParseCurrencyString(csCardLimits.Mid(csCardLimits.Find(_T(':')) + 1), _T(','), _T('.'));
        fpLimit = balanceCents / 100.0;

        // fpLimit = _tcstod(csCardLimits.Mid(csCardLimits.Find(_T(':')) + 1), &tcTemp); //, csCardLimits.ReverseFind(_T("BALANCE")) - (csCardLimits.Find(_T(":")) + 1)));
        
    }

    if (csCardLimits.Find(_T("RANGE")) != -1)
    {
        int rangeStart = csCardLimits.Find(_T(":")) + 1;
        CString range = csCardLimits.Mid(rangeStart);
        int rangeSep = range.Find(_T("-"));

        CString minAmountString = range.Mid(0, rangeSep);
        CString maxAmountString = range.Mid(rangeSep + 1);

        LONG minAmountCents = ParseCurrencyString(minAmountString, _T(','), _T('.'));
        LONG maxAmountCents = ParseCurrencyString(maxAmountString, _T(','), _T('.'));

        fpMin = minAmountCents / 100.0;
        fpMax = maxAmountCents / 100.0;
    }

    fpLimit = (fpLimit > fpBalance) ? (fpLimit - fpBalance) : fpLimit;
    fpMax = (fpMax > fpBalance) ? (fpMax - fpBalance) : fpMax;

    CString csBalance = _T("");
    if(fpBalance > 0)
    {
        csBalance.Format(_T("%s%s $%.2f"), _T("\n"),  msgs.GetStringValueFromID(9263), fpBalance);
    }

    CString csLimit = _T("");
    if(fpLimit > 0)
    {
        csLimit.Format(_T("%s%s $%.2f"), _T("\n"), msgs.GetStringValueFromID(9264), fpLimit);

    }

    CString csRange = _T("");
    if((fpMin > 0) && (fpMax > 0))
    {
        csRange.Format(_T("%s%s $%.2f - $%.2f"), _T("\n"), msgs.GetStringValueFromID(9265), fpMin, fpMax);
    }

    csSummaryInstructions += csBalance + csLimit + csRange;

    CString csDataNeededCommand;
    csDataNeededCommand.Format(    csDataNeededFormatString,                                     
                                   csTopCaptionID,                                     /*TOPCAPTION*/
                                   _T(" "),                                         /*DETAILEDINSTRUCTION*/ 
                                   csSummaryInstructions,                                     /*SUMMARYINSTRUCTIONS*/
                                   _T(" "),                                         /*LSWND1*/                                  
                                   _T(" "),                                         /*LSWND2*/
                                   _T("GiftPhonePrice"),                            /*LANEBUTTONTEXT*/
                                   _T("0"),                                         /*STORELOGON*/
                                   _T("$ScotApp_67$"));                                   /*BUTTON7*/  // SSCOI-45374
                                    
    SDataNeededView MyDataNeeded={0};
    
    MyDataNeeded.szView = csDataNeededCommand.GetBuffer(0);
    CCustomerTBConnectFL::instance()->SetDataNeededView(&MyDataNeeded);

    return true;


}

//-sotf

//SOTF 7261
bool CCustomerMsgMap::msgResendDOB(TBSTATE &tbretState, LPARAM lparam)
{
    CCustomerMHItem *pMhItem = CCustomerMHItem::instance();

    CString csCustDOB = pMhItem->GetCustomerDOB();

    if(!csCustDOB.IsEmpty())
    {
        CString csKeySeq = csCustDOB + _T("{ENTER}");

        //A DOB of 000000 means the operator selected date not needed
        if(csCustDOB == _T("000000"))
        {
             csKeySeq = _T("{F1}");
        }

        return msgSendSequence(tbretState,(LPARAM)(LPCTSTR)csKeySeq);
    }

    CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_NEEDASSISTMODE, NULL, NULL);   
    return true;

}
//sotf

// SOTF 7695
bool CCustomerMsgMap::msgMEPAvailable(TBSTATE &tbretState, LPARAM lparam)
{

    tbretState = TB_IGNORE;
    if(!CCustomerTrxBroker::instance()->InAssistMenu())
    {
        CString csCmd;
        CBaseMessages & msgs = *CBaseMessages::instance();

		CString warrantyItem = CCustomerTBProperties::instance()->GetHookProperty(_T("Item_descriptor_Prop")); //CRD 264675 use item_descriptor_prop from item_entry_display rather item_descriptor_last which is from last_item_display
		warrantyItem.TrimLeft();
		warrantyItem.TrimRight();
		if(warrantyItem.IsEmpty())
		{
			warrantyItem = CCustomerTBProperties::instance()->GetHookProperty(_T("Item_descriptor_last"));
			warrantyItem.TrimLeft();
			warrantyItem.TrimRight();
		}
		
		CString promptFirstPart;
		promptFirstPart.Format(msgs.GetStringValueFromID(9131), warrantyItem);
		
		
		//POS39476/52217 - use 1 LF - RRS
		csCmd.Format(_T("%s%s\\n %s %s;"), 
			_T("MEPAvailable;;SUMMARYINSTRUCTION="),
			promptFirstPart,
			CCustomerTBProperties::instance()->GetHookProperty(_T("OG_Guidance_Prop")), //MEP item description
			msgs.GetStringValueFromID(9268)); //MEP prompt second part
       
        tbretState = TB_NEEDMOREDATA; 
        return CCustomerMsgMap::msgDataNeeded(tbretState, (LPARAM)(LPCTSTR)csCmd);

    }

    tbtrace(TM_ENTRY, _T("msgMEPAvailable() In Assist Menu.  Not requesting DataNeeded"));
    return true;

}
//sotf 

// pos 11273 - rrs
// The scanner data would be received in this format:B3B4KG0DAOIA78ZV~B4KG0DAOIA78ZV~110
// TB will get the item code from first section and the symbology from the third section
// Then it will construct the scannermsg to be sent to the POS through PipeServer and ACSIO
bool CCustomerMsgMap::msgCheckedIfDataIsScannedSequence(TBSTATE &tbretState, LPARAM lparam)
{
    tbtrace(TM_ENTRY, _T("msgCheckedIfDataIsScannedSequence:%s"), (LPCTSTR)lparam);
    CString csData = (LPCTSTR) lparam;    

    
    int iFind = csData.Find(_T("~"));
    if(  iFind != -1 )
    {
        CScannerMsg scan;
	    CString csSymbology(_T(""));
	    
        CString csItemCode = csData.Left(iFind);
        csData = csData.Mid(iFind+1);
        iFind = csData.Find(_T("~"));

        if( iFind != -1 )
        {
            csData = csData.Mid(iFind+1);
            iFind = csData.Find(_T("{"));
            if( iFind != -1 )
            {
                csData = csData.Left(iFind);
            }
            
            scan.BarcodeType((CScannerMsg::BarcodeTypes)(_ttol(csData)));
        }
        else
        {
            scan.BarcodeType((CScannerMsg::BarcodeTypes)0);
        }

		//Start CRD 328873 - need to append barcode prefix before sending to POS
		if(!iswalpha(csItemCode.GetAt(0)) && csItemCode.GetAt(0) != ']' && csItemCode.GetAt(0) != '@') //check first char
		{
			CCustomerMHItem *pMhItem = CCustomerMHItem::instance();
			csItemCode = pMhItem->ModifyBarcode(csItemCode, _ttol(csData));
		}
		//End CRD 328873

        scan.Data(csItemCode);

        CCustomerTBConnectPOS::instance()->SendMsgToPOS(scan);
        
        tbtrace(TM_ENTRY, _T("BarcodeType(%s), ItemCode Sent (%s)"), csData, csItemCode);
    }
    else
    {
        CCustomerTBConnectPOS::instance()->SendSequence(CString((LPCTSTR) lparam));
    }
    return true;
}

//POS11024 Mobile Shopper START
bool CCustomerMsgMap::msgBagAloneScreen( TBSTATE &tbretState, LPARAM lparam )
{
   tbtraceIO(_T("msgBagAloneScreen"));  // based on SolutionsMsgMap::msgMessageScreen

   // get item desc from tb_char field of acs_tb_interface
   CCustomerTBProperties & props = *CCustomerTBProperties::instance();
   CCustomerTrxBroker & trx = *CCustomerTrxBroker::instance();
   CString csItemDesc = props.GetHookProperty(_T("tb_char"));

   // message parameters for display
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

   // display screen
   CBaseMessages *pMessages = CBaseMessages::instance();
   CCustomerTrxBroker *pCurrTrx = CCustomerTrxBroker::instance();
   CCustomerTBConnectFL &tbConnectFL = *CCustomerTBConnectFL::instance();
   
   SMessageInfo msgMessageObj;
   memset(&msgMessageObj,0,sizeof(SMessageInfo));
   
   CString csWaveFile = strAudio;
   
   //POS39476/54205 - use one LF - rrs
   // prepend item description to screen message
   //CString csScreenText = csItemDesc + ("\n");
   CString csScreenText = _T("");	//CRD 235561
   csScreenText = csScreenText + pMessages->GetStringValueFromID(_ttoi(strScreenText));

   CString csLeadThruText = pMessages->GetStringValueFromID(_ttoi(strLeadThru));
   msgMessageObj.szLeadThruText    = (LPTSTR)(LPCTSTR)csLeadThruText;
   msgMessageObj.szScreenText      = (LPTSTR)(LPCTSTR)csScreenText;
   msgMessageObj.szPlayWave        = (LPTSTR)(LPCTSTR)csWaveFile;
   //POS11024:This flag was originally set to true. Causes an issue when Bag Alone
   //items are followed by price required items. Causes a hang. So I'm setting to false
   msgMessageObj.bNeedItemCancel   = false;

   //For resumes with Bag Alone, don't go back to Scan and Bag
   if( props.GetHookProperty(_T("IsResumeTrxInProgress")) == _T("1") )
   {
      msgMessageObj.bGoBackScanAndBag = false;      
      pCurrTrx->SetCurrentInProgressState(_T("ITEMIZATION"));
   }
   else
   {
      msgMessageObj.bGoBackScanAndBag = true;
   }
   tbConnectFL.SetMessageInfo(&msgMessageObj);

   delete [] pszParams;
   
   return true;
}
//POS11024 Mobile Shopper END

//POS63877
bool CCustomerMsgMap::msgBuildGreenDotDataNeeded( TBSTATE &tbretState, LPARAM lparam )
{
    tbtraceIO(_T("msgBuildGreenDotDataNeeded"));  

    CCustomerCurrentItem* pCurrItem = CCustomerCurrentItem::instance();
    CString csItem(pCurrItem->ItemCode());
    CString csItemToRemoveText;
    CString csDesc;

    CBaseMessages *pMessages = CBaseMessages::instance();
    csItemToRemoveText.Format(_T("%s\n\n%s%s"),pMessages->GetStringValueFromID(9195), pMessages->GetStringValueFromID(9196), csItem);
    
    csDesc = pMessages->GetStringValueFromID(9197);

    CString csDataNeededCommand;
    csDataNeededCommand.Format(_T("TOPCAPTION=%s;DETAILEDINSTRUCTION=%s;SUMMARYINSTRUCTION=%s;SCANNERINPUTENABLED=0;LSWND1=%s;LSWND2=%s;LANEBUTTONTEXT=%s;BUTTON0=cReturnGreenDotItem,%s;BUTTON1=cAssistModeGreenDotItem,%s;"),
                                   csDesc,                                /*TOPCAPTION*/
                                   csItemToRemoveText,                    /*DETAILEDINSTRUCTION*/
                                   _T(" "),                               /*SUMMARYINSTRUCTION*/
                                   pMessages->GetStringValueFromID(9197), /*LSWND1*/                                  
                                   csItem,                                /*LSWND2*/
                                   csDesc,                                /*LANEBUTTONTEXT */
                                   //pMessages->GetStringValueFromID(58),   /*BUTTON0*/
                                   _T("$ScotApp_58$"),   /*BUTTON0*/  // SSCOI-45374
                                   //pMessages->GetStringValueFromID(269)); /*BUTTON1*/
                                   _T("$ScotApp_269$")); /*BUTTON1*/  // SSCOI-45374
    
                                    
    SDataNeededView MyDataNeeded={0};

    
    MyDataNeeded.szDescription= csDesc.GetBuffer(0);
    MyDataNeeded.szView = csDataNeededCommand.GetBuffer(0);
    CCustomerTBConnectFL::instance()->SetDataNeededView( &MyDataNeeded);

    //set up the string to display to the customer while on the RAP DATANeeded screen
    CCustomerTBProperties::instance()->SetProperty(TBPROP_RAPDATANEEDED_CUSTENTRY, _T("30"));

    return true;
}

//POS74922 - clean up the receipts
bool CCustomerMsgMap::msgPurgeAllReceipts( TBSTATE &tbretState, LPARAM lparam )
{
    tbtraceIO(_T("msgPurgeAllReceipts"));  

    CCustomerCurrentReceipt::instance()->PurgeAllReceipts(true);

    return true;
}
//Start CRD 185620
bool CCustomerMsgMap::msgEASGenericDelayed(TBSTATE &tbretState, LPARAM lparam)
{
	CBaseMessages *pMessages = CBaseMessages::instance();
	CCustomerTBProperties *pTBProperties = CCustomerTBProperties::instance();
	
	CString csEASItem, csDesc, csInterventionInstruction;
	csEASItemDescription = pTBProperties->GetProperty(_T("EASItem"));
	csDesc = pTBProperties->GetProperty(_T("EASPopUpDesc"));
	csInterventionInstruction = pTBProperties->GetProperty(_T("EASInterventionTitle"));

	tbtrace(TM_INFO, _T("TBCMD_HASEASITEM - csDesc %s"), csDesc);

	csEASItem.Format(_T("%s %s"), csEASItemDescription, csDesc);

	
	CString csDataNeededCommand;
	csDataNeededCommand.Format(_T("MESSAGESCREENTITLE=%s;MESSAGESCREENTEXT=%s;MESSAGESCREENITEMNEEDCANCEL=0;MESSAGESCREENGOBACKSCANANDBAG=0;LaneButtonText=%s;DefaultCmdString=%s;TopCaption=%s;SummaryInstruction=%s;StoreLogon=1;HideTotal=1;ID=TB002;InterventionInstruction=%s;ReportCategoryName=%s;HandlingContext=SMDataEntryGoToLane"),
				 _T("$ScotApp_9512$"),						/*MESSAGESCREENTITLE*/
				 csEASItem,                                /*MESSAGESCREENTEXT*/
				 csInterventionInstruction,                    /*LaneButtonText*/
				 _T("CmdBtn_HandleEASIntervention"),		/*DefaultCmdString*/
				 _T("$ScotApp_9512$"),                               /*TopCaption*/
				 _T("$ScotApp_9514$"),				/*SumaryInstruction*/  
				 csInterventionInstruction,                                /*InterventionInstruction */
				 _T("EAS Item"));				/*ReportCategoryName*/
			 
	SDataNeededView MyDataNeeded={0};
			 
	tbtrace(TM_INFO, _T("TBCMD_HASEASITEM - %s, %s"), csEASItem, csDataNeededCommand);
			
	MyDataNeeded.szDescription= csEASItem.GetBuffer(0);
	MyDataNeeded.szView = csDataNeededCommand.GetBuffer(0);
	CCustomerTBConnectFL::instance()->SetDataNeededView( &MyDataNeeded);
			 
	//set up the string to display to the customer while on the RAP DATANeeded screen
	pTBProperties->SetProperty(TBPROP_RAPDATANEEDED_CUSTENTRY, _T("30"));
			 
	CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_GENERIC_DELAYED, NULL, NULL);
	

	return true;
}
//End CRD 185620

//Start CRD 231416
bool CCustomerMsgMap::msgMEPMobileAvailable( TBSTATE &tbretState, LPARAM lparam )
{
   tbtraceIO(_T("msgMEPMobileAvailable"));  // based on SolutionsMsgMap::msgMessageScreen

   tbretState = TB_IGNORE;
   if(!CCustomerTrxBroker::instance()->InAssistMenu())
   {
	   CString csCmd;
	   CBaseMessages & msgs = *CBaseMessages::instance();
	   
	   CString warrantyItem = CCustomerTBProperties::instance()->GetHookProperty(_T("Item_descriptor_Prop")); //CRD 264675 use item_descriptor_prop from item_entry_display rather item_descriptor_last which is from last_item_display
	   warrantyItem.TrimLeft();
	   warrantyItem.TrimRight();
	   if(warrantyItem.IsEmpty())
	   {
		   warrantyItem = CCustomerTBProperties::instance()->GetHookProperty(_T("Item_descriptor_last"));
		   warrantyItem.TrimLeft();
		   warrantyItem.TrimRight();
	   }

	   CString promptFirstPart;
	   promptFirstPart.Format(msgs.GetStringValueFromID(9131), warrantyItem);

	   
	   //POS39476/52217 - use 1 LF - RRS
	   csCmd.Format(_T("%s%s\\n %s %s;"), 
		   _T("SuspendResumeMEP;;SUMMARYINSTRUCTION="),
		   promptFirstPart,
		   CCustomerTBProperties::instance()->GetHookProperty(_T("OG_Guidance_Prop")), //MEP item description
		   msgs.GetStringValueFromID(9268)); //MEP prompt second part
       
	   tbretState = TB_NEEDMOREDATA; 
	   return CCustomerMsgMap::msgDataNeeded(tbretState, (LPARAM)(LPCTSTR)csCmd);
	   
   }
   
   tbtrace(TM_ENTRY, _T("msgMEPAvailable() In Assist Menu.  Not requesting DataNeeded"));
    return true;
}
//End CRD 231416
//Start CRD 231416
bool CCustomerMsgMap::msgSendFLNotifyCancelAlert( TBSTATE &tbretState, LPARAM lparam )
{
	tbtraceIO(_T("msgSendFLNotifyCancelAlert"));  // based on SolutionsMsgMap::msgMessageScreen
	if(CCustomerTBConnectPOS::instance()->SendAuxMsgNotifyCancelAlert() > 0)
		return true;
	else
		return false;
}

// Start CRD 554916
bool CCustomerMsgMap::msgAccountLookupFailedIntervention(TBSTATE &tbretState, LPARAM lparam)
{
	CBaseMessages *pMessages = CBaseMessages::instance();
	CCustomerTBProperties *pTBProperties = CCustomerTBProperties::instance();
	CString csTitle, csReportCategoryName, csInterventionInstruction;

	CString csStr= (LPCTSTR)lparam;
	if(csStr.Find(_T("AccountLookUpFailed")) != -1)
	{
		csTitle = _T("$ScotApp_9545$");
		csReportCategoryName = szGDAccountLookUpFailed;
		csInterventionInstruction = pTBProperties->GetProperty(_T("AccountLookUpInterventionTitle"));
	}
	// Start CRD 557225
	else if(csStr.Find(_T("GetAccountDetailsFailed")) != -1)
	{
		csTitle = _T("$ScotApp_9548$");
		csReportCategoryName = szGDGetAccountDetailsFailed;
		csInterventionInstruction = pTBProperties->GetProperty(_T("GetAccountDetailsFailedInterventionTitle"));
	}
	// End CRD 557225

	CString csDataNeededCommand;
	csDataNeededCommand.Format(_T("MESSAGESCREENTITLE=%s;MESSAGESCREENTEXT=%s;MESSAGESCREENITEMNEEDCANCEL=0;MESSAGESCREENGOBACKSCANANDBAG=0;LaneButtonText=%s;DefaultCmdString=%s;TopCaption=%s;SummaryInstruction=%s;StoreLogon=1;HideTotal=1;ID=TB003;InterventionInstruction=%s;ReportCategoryName=%s;HandlingContext=SMDataEntryGoToLane"),
		csTitle,						/*MESSAGESCREENTITLE*/
		csTitle,                                /*MESSAGESCREENTEXT*/
		csInterventionInstruction,                    /*LaneButtonText*/
		_T("CmdBtn_HandleAccountLookupFailed"),		/*DefaultCmdString*/
		csTitle,                               /*TopCaption*/
		_T("$ScotApp_9514$"),				/*SumaryInstruction*/  
		csInterventionInstruction,                                /*InterventionInstruction */
		csReportCategoryName);				/*ReportCategoryName*/
			 
	SDataNeededView MyDataNeeded={0};
			 	
	MyDataNeeded.szDescription= csDataNeededCommand.GetBuffer(0);
	MyDataNeeded.szView = csDataNeededCommand.GetBuffer(0);
	CCustomerTBConnectFL::instance()->SetDataNeededView( &MyDataNeeded);
			 
	//set up the string to display to the customer while on the RAP DATANeeded screen
	pTBProperties->SetProperty(TBPROP_RAPDATANEEDED_CUSTENTRY, _T("30"));
			 
	CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_GENERIC_DELAYED, NULL, NULL);
	
	
	return true;
}
// End CRD 554916