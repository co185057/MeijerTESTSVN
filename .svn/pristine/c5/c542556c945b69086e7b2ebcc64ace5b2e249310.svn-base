// CustomerMHTransaction.cpp: implementation of the CCustomerMHTransaction class.
//
// POS312424 Work Request:79417 Name:Veerasekhara D Date:May 01, 2017
// POS295030 Work Request:76936 Name:Veerasekhara,D Date:October 20, 2016
// POS45465 Work Request:52635 Name:Robert Susanto Date:November 1, 2012
// POS11024 Work Request:16632 Name:Sandeep Shenoy Date:October 5, 2011
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CustomerMHTransaction.h"
#include "CustomerTrxBroker.h"
#include "CustomerCurrentCashDrawer.h"
#include "traceSystem.h"
#include "CustomerMHOptions.h"
#include "CustomerCurrentReceipt.h"
#include "CustomerCurrentTotals.h"
#include "CustomerMHCashDrawer.h"
#include "CustomerCurrentSlip.h"
#include "customerTBProperties.h"
#include "CustomerMHItem.h"
#include "CustomerCurrentTransaction.h"

#define T_ID    _T("CCustomerMHTransaction")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomerMHTransaction::CCustomerMHTransaction()
{

}

CCustomerMHTransaction::~CCustomerMHTransaction()
{

}

TBRC CCustomerMHTransaction::Start(LPCTSTR szOverrideCode)
{
    CCustomerTrxBroker *pTrx = CCustomerTrxBroker::instance();
    pTrx->TBSetProperty(_T("MainReceipt"), _T("0"));
    pTrx->TBSetProperty(_T("LastEntryID"), _T("00000"));
    pTrx->TBSetProperty(_T("PropIsFromPayment"), _T("0"), true);
    pTrx->TBSetProperty(_T("PrintGiftReceipt"), _T("0"), true);
    pTrx->TBSetProperty(_T("RebateReceiptRequested"), _T("0"), true);
    pTrx->TBSetProperty(_T("NSC4Mnemonic"), _T(""), true);
    pTrx->TBSetProperty(_T("NSC4Account"), _T(""), true);
    pTrx->TBSetProperty(_T("TMDDiscountPercent"), _T(""), true);
    pTrx->TBSetProperty(_T("TMDMemberName"), _T(""), true);
    pTrx->TBSetProperty(_T("TMDMemberNumber"), _T(""), true);
    pTrx->TBSetProperty(_T("TrxChangeDue"), _T("0"), true);
    pTrx->TBSetProperty(_T("ReleaseReceipt"), _T("0"), false);
	pTrx->TBSetProperty(_T("DispenseAnyChangeAtLane"), _T("0"), false);
    pTrx->TBSetProperty(_T("NoSaleInProgress"), _T("0"), false);
	pTrx->TBSetProperty(_T("IsFromMPerksPrompt"), _T("0"), false);
	pTrx->TBSetProperty(_T("PreviousEntryID"), _T("0"), false); /* POS312424*/

    //POS45465 - rrs
    CCustomerTrxBroker::instance()->TBSetProperty(_T("WICBalanceExceeded"), _T("N/A"));
    //e - POS45465 - rrs

	pTrx->TBSetProperty(TBPROP_NEWFLSTATE, _T(""));

    CCustomerCurrentTotals::instance()->ResetTotalDetails();
	CCustomerMHCashDrawer::instance()->SetCloseImmediate( true ); //TAR339265
    CCustomerCurrentCashDrawer::instance()->setExpectedCashDrawerState( CASHDRAWER_CLOSED );

	CCustomerMHOptions* pOpts = CCustomerMHOptions::instance();
	CString csIgnoreItems = pOpts->OptionValue(CString(_T("AllowACSToIgnoreItemsOnResume")));
	CString csFLConfig = pOpts->OptionValue(CString(_T("FastLaneConfiguration")));

	if(csIgnoreItems == _T("N") || csFLConfig == _T("Standard"))
		CCustomerTrxBroker::instance()->TBSetProperty(_T("IsACSIgnoreItemsOnResume"), _T("0"), true);
	else
		CCustomerTrxBroker::instance()->TBSetProperty(_T("IsACSIgnoreItemsOnResume"), _T("1"), true);

    // Nothing should be printing as we start a transaction.
    CCustomerCurrentReceipt::instance()->SetIsPrinting( false );

    //SOTF 8362  remove all receipts if any are hanging around
    CCustomerCurrentReceipt::instance()->PurgeAllReceipts(true);

    // Verify if there are any slips remaining and purge them
    CCustomerCurrentSlip & slip = *CCustomerCurrentSlip::instance();
    if( slip.GetNumRemaining() > 0 )
    {
        tbtrace(TM_WARNING, _T("There are %d slips while starting a new transaction.  Purging..."));
    }
    slip.PurgeAllSlips();
	
	CCustomerTrxBroker::instance()->SetTenderInProgress(false); //SOTF 6524

    CCustomerMHItem::instance()->ResetDelayedAgeRequiredTLOGData();
    return CSolutionMHTransaction::Start(szOverrideCode);
}

TBRC CCustomerMHTransaction::Finish(void)
{
	CCustomerTrxBroker &Trx= *CCustomerTrxBroker::instance();
	CString csChange = Trx.TBGetProperty(_T("TrxChangeDue"), true);
	int nChange = _ttoi(csChange);
	CString csOptDelayPrint = CCustomerMHOptions::instance()->OptionValue(_T("DelayPrinting"));
	
	// TAR 388293: Reset IsCardTenderSelected since transaction has already ended.
	Trx.TBSetProperty(_T("IsCardTenderSelected"), _T("0"), false);

	if( csOptDelayPrint == _T("Y") && nChange >= 100)
	{
		Trx.TBSetProperty(_T("TrxChangeDue"), _T("0"), true);
		Trx.TBSetProperty(_T("ReleaseReceipt"), _T("1"), false);
	}
    
    // +SOTF6521
    if( !CCustomerCurrentTransaction::instance()->CountTotalRebateReceipts() &&
        Trx.TBGetProperty(_T("RebateReceiptRequested"), true) == _T("1") )
    {
        tbtrace(TM_INFO, _T("Reset RebateReceiptRequested=0 - there are no pending rebate receipts in trx."));
        Trx.TBSetProperty(_T("RebateReceiptRequested"), _T("0"), true);
    }
    // -SOTF6521
    /* POS11024: Mobile Shopper-Clear VoidPrepaidCards and PropEASDeact at end of trx */
    //EAS items would display in Full AM in next trx, so I'm clearing 2 properties here
    Trx.TBSetProperty(_T("VoidPrepaidCards"), _T("0"), true);
    Trx.TBSetProperty(_T("PropEASDeact"), _T("0"), true);
    
	return CSolutionMHTransaction::Finish();
}

TBRC CCustomerMHTransaction::SuspendTransaction(const BOOL fOverride)
{
	CCustomerTrxBroker::instance()->TBSetProperty(_T("PrintGiftReceipt"), _T("0"), true);
	CCustomerTrxBroker::instance()->TBSetProperty(_T("RebateReceiptRequested"), _T("0"), true);
	return CSolutionMHTransaction::SuspendTransaction(fOverride);
}