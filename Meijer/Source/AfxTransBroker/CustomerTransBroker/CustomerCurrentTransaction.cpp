//////////////////////////////////////////////////////////////////////
//
// CustomerCurrentTransaction.cpp: implementation of the CCustomerCurrentTransaction class.
//
//
// Change history:
//
// POS149568 Work Request:66559 Name:Saiprasad Srihasam    Date:December 18, 2014
// POS18041 WORK REQUEST:16656 Name: Matt Condra Date: October 3, 2011
//
// POS11024 Work Request:16632 Name:Sandeep Shenoy Date:October 24, 2011
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CustomerCurrentTransaction.h"
#include "TraceSystem.h"
#include "CustomerCurrentItem.h"
#include "CustomerTrxBroker.h"
#include "CustomerCurrentTender.h"
#include "customerTBProperties.h"
#include "CustomerMHItem.h"


#define T_ID    _T("CustomerCurrentTransaction")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCustomerCurrentTransaction::CCustomerCurrentTransaction()
{
       InsertedDiscount(_T(""));
	   m_rcptItems.RemoveAll();
}

CCustomerCurrentTransaction::~CCustomerCurrentTransaction()
{
}

void CCustomerCurrentTransaction::InsertedDiscount(CString csInsertedDiscount) 
{
	if(csInsertedDiscount.IsEmpty())
		m_csInsertedDiscount = _T("");

	else m_csInsertedDiscount += (csInsertedDiscount + _T(";"));
}


void CCustomerCurrentTransaction::ResetTransactionDetails(void)
{
	CBaseCurrentTransaction::ResetTransactionDetails();
	tbtrace(TM_INFO, _T("ResetTransactionDetails() - m_SItemArray cleared; IsThereItemNeedToDisplayed not reset.")); // POS18041
	CCustomerCurrentItem::instance()->m_SItemArray.clear();
	CCustomerTrxBroker *pTrx = CCustomerTrxBroker::instance();
	// TAR 398763 // pTrx->TBSetProperty(_T("IsThereItemNeedToDisplayed"), _T("0"), false);
	pTrx->TBSetProperty(_T("IsDirtyTrxWithNoItems"), _T("0"), false);
	pTrx->TBSetProperty(_T("IsFromEndorsement"), _T("0"), false); //TAR360011 
    pTrx->TBSetProperty(_T("SuspendingCurrentTransaction"), _T("0"), true);
	pTrx->TBSetProperty(TBPROP_NEWFLSTATE, _T(""));
	pTrx->TBSetProperty (_T("DigitalCouponPromoEntryEnabled"), _T("1"), true); // SOTF6400
	pTrx->TBSetProperty (_T("DigitalCouponActive"), _T("0")); // SOTF6400
	pTrx->TBSetProperty (_T("DigitalCouponAppliedOffline"), _T("0"));

    // SOTF 8723 Reset the variable - RRS
    pTrx->TBSetProperty(_T("IsECheckTender"),_T("0"),false);
    // e SOTF 8723

    CCustomerTrxBroker::nIndex = 0;                //POS11024:Mobile Shopper

	InsertedDiscount(_T(""));
	ClearReceiptItems();
    CCustomerCurrentTender  &CurrTender = *CCustomerCurrentTender::instance();
    CurrTender.Reset();
    CCustomerMHItem::m_nRewardLineEntryID = 100000; //POS149568
}

void CCustomerCurrentTransaction::ClearReceiptItems(void)
{
	tbtraceIO(_T("ClearReceiptItems"));

	POSITION pos = NULL;
	for( pos = m_rcptItems.GetStartPosition(); pos != NULL; )
	{
		int iID;
		PRECEIPTITEMINFO prii = NULL;
		m_rcptItems.GetNextAssoc(pos, iID, prii);
		delete prii;
	}
	m_rcptItems.RemoveAll();
}


BOOL CCustomerCurrentTransaction::DetectAndUpdateRebateItemAttributeChange()
{
	tbtraceIO(_T("DetectRebateItemAttributeChange()"));
	
	CCustomerCurrentItem* pCurrItem = CCustomerCurrentItem::instance();
	CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();
	BOOL bRet = FALSE;
	
	//TAR 324463, 357673
	CString strGiftRebateInd1 = pCurTrx->TBGetProperty(_T("PropGiftRebateInd"), true);
    CString strGiftRebateInd2 = pCurTrx->TBGetProperty(_T("PropGiftRebateInd2"), true);
	CString strRebateReceiptNums = pCurTrx->TBGetProperty(_T("PropRebateReceiptNums"), true);

	// determine which property counts here, for quantity items with a qty of 1, we find that the strGiftRebateInd1
	// field is the one that is populated AND we have a value in strGiftRebateInd1...who knew?!
	CString strGiftRebateInd;
	if( strGiftRebateInd2 == _T("0") && strGiftRebateInd1 != _T("") )
	{
		// qty type item with a qty of 1 - use the strGiftRebateInd1
		strGiftRebateInd = strGiftRebateInd1;
	}
	else
	{
		strGiftRebateInd = strGiftRebateInd2 != _T("") ? strGiftRebateInd2 : strGiftRebateInd1;
	}
	if(strGiftRebateInd == _T(""))
	{	// uniformity for non-rebate items to avoid confusion (tar 357606)
		strGiftRebateInd = _T("0");
	}
	
	// reset the properties for the next time process message 
	pCurTrx->TBSetProperty(_T("PropGiftRebateInd"), _T(""), true);
	pCurTrx->TBSetProperty(_T("PropGiftRebateInd2"), _T(""), true);
	pCurTrx->TBSetProperty(_T("PropRebateReceiptNums"), _T(""), true);
	
	
	PRECEIPTITEMINFO prii = NULL;
	if( m_rcptItems.Lookup(pCurrItem->EntryID(), prii) )
	{
		// is the current rebate property the same as the property last saved for this item?
		// if it is not then the current itemdetails is notifying us that the rebate item flag 
		// has been changed.
		bRet = prii->strGiftRebateInd != strGiftRebateInd;

		// if it is the same and we're handling a quantity required rebate receipt case, 
		// it could be that the number of receipts has changed.
		if( !bRet && strGiftRebateInd2 != _T("") && strRebateReceiptNums != _T("") )
		{
			bRet = prii->strRebateReceiptNums != strRebateReceiptNums;
		}
	}

	// Before returning, ensure you've updated the attributes of the item 
	// Add a new item if not already there
	if( !prii && !m_rcptItems.Lookup(pCurrItem->EntryID(), prii) )
	{
		prii = new RECEIPTITEMINFO;
		m_rcptItems.SetAt(pCurrItem->EntryID(), prii);
	}

	prii->strGiftRebateInd = strGiftRebateInd;
	prii->strRebateReceiptNums = strRebateReceiptNums;
	
	return bRet;
}

BOOL CCustomerCurrentTransaction::IsCurrentItemWithRebateReceipt(void)
{
	BOOL bRet = FALSE;
	CCustomerCurrentItem* pCurrItem = CCustomerCurrentItem::instance();
	PRECEIPTITEMINFO prii = NULL;
	if( m_rcptItems.Lookup(pCurrItem->EntryID(), prii) )
	{
		tbtrace(TM_INFO, _T("prii->strGiftRebateInd = %s"), prii->strGiftRebateInd);
		if ( prii->strGiftRebateInd != _T("0") && prii->strGiftRebateInd.GetLength() != 0)
			bRet = true;
	}
	return bRet;
}

int CCustomerCurrentTransaction::CountItemsWithRebateReceipts(void)
{
	POSITION pos = NULL;
    int iItems = 0;
	for( pos = m_rcptItems.GetStartPosition(); pos != NULL; )
	{
		int iID;
		PRECEIPTITEMINFO prii = NULL;
		m_rcptItems.GetNextAssoc(pos, iID, prii);
        if( prii->strGiftRebateInd != _T("0") )
        {
            iItems++;
        }
	}
    tbtrace(TM_INFO, _T("CountItemsWithRebateReceipts() - Counted %d items with rebate receipts."), iItems);
    return iItems;
}



int CCustomerCurrentTransaction::CountTotalRebateReceipts(void)
{
	POSITION pos = NULL;
    int iItems = 0;
	for( pos = m_rcptItems.GetStartPosition(); pos != NULL; )
	{
		int iID;
		PRECEIPTITEMINFO prii = NULL;
		m_rcptItems.GetNextAssoc(pos, iID, prii);
        if( prii->strGiftRebateInd != _T("0") )
        {
            if( prii->strRebateReceiptNums != _T("") )
            {
                iItems += _ttol(prii->strRebateReceiptNums);
            }
            else
            {
                iItems++;
            }
        }
	}
    tbtrace(TM_INFO, _T("CountTotalRebateReceipts() - Counted %d rebate receipts."), iItems);
    return iItems;
}
