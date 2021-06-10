// SolutionMHItem.cpp: implementation of the CSolutionMHItem class.
//
//////////////////////////////////////////////////////////////////////
/***********************************************************************
/* Change History                                            
/*
/* POS83168 Work Request:61232 Name:Matt Condra Date:February 11, 2014
/* POS19072 Work Request:16656 Name:Matt Condra Date:November 23, 2011  			
/************************************************************************/
#include "stdafx.h"
#include "SolutionMHItem.h"
#include "CustomerTBConnectPos.h"
#include "customerCurrentItem.h"
#include "scannerMsg.h"
#include "TraceSystem.h"
#include "CustomerMHOptions.h"
#include "scaleMsg.h"
#include "CustomerTrxBroker.h"
#include "CustomerMsgMap.h"
#define T_ID _T("CSolutionMHItem")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSolutionMHItem::CSolutionMHItem()
{
    SetSpecialHandlingItemList(_T("CouponTypes"), m_csCouponTypeList);
	SetSpecialHandlingItemList(_T("MonetaryRewards"), m_csMonetaryRewardList);
	SetSpecialHandlingItemList(_T("PointRewards"), m_csPointRewardList);
	m_nNumberOfItemsSold = 0;
    m_csPrevItemCode.Empty();
}

CSolutionMHItem::~CSolutionMHItem()
{
}


/*************************************************************************
* HandleItemVoid - 
*
* Parameters: 
*  LPARAM - lparam passed from the MessageMap
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBSTATE CSolutionMHItem::HandleItemVoid(LPARAM lparam)
{
	tbtrace(TM_ENTRY, _T("+-HandleItemVoid()"));
    CCustomerCurrentItem *pCurrItem = CCustomerCurrentItem::instance();
	CString csEntryID;
	csEntryID.Format(_T("%u"), pCurrItem->EntryID());
    
    //check if the item was scanned and does not have an associated entry id
    if(
        (pCurrItem->GetScanCodeType()) &&
        ((csEntryID.IsEmpty()) || (csEntryID == _T("0")))
      )
   
    {
        tbtrace(TM_ENTRY, _T("+-HandleItemVoid() entry ID is empty void by scanning"));

        CCustomerTrxBroker *pCurTrx = CCustomerTrxBroker::instance();
        if(pCurTrx->InAssistMode())
        {
            return HandleItemScan(NULL);
        }

        
        pCurTrx->SetCurrentInProgressState(_T("VOID_ITEM_SCAN"));
        pCurTrx->TriggerMsgMap();
        return TB_IGNORE;
        
    }
    //check if the item has a valid entry ID
    if((csEntryID == _T("0"))||(csEntryID.IsEmpty()))
    {
        
        tbtrace(TM_ENTRY, _T("+-HandleItemVoid() Item was not scanned and has no entry ID-- cannot void"));
        return TB_ITEMUNKNOWN;
        
    }

    tbtrace(TM_ENTRY, _T("+-HandleItemVoid() set selected item:%s"), csEntryID);
	CCustomerTBConnectPOS::instance()->SetSelectedItem(csEntryID);
    CString csSending = CCustomerMHOptions::instance()->OptionValue(_T("VoidCurrentItem"));
    
    tbtrace(TM_ENTRY, _T("+-HandleItemVoid() sending:%s"), csSending);
    CCustomerTBConnectPOS::instance()->SendSequence(csSending);
	return TB_IGNORE;
}

/*************************************************************************
* HandleItemQty - 
*
* Parameters: 
*  LPARAM - lparam passed from the MessageMap
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBSTATE CSolutionMHItem::HandleItemQty(LPARAM lparam)
{
	//ensure POS at a state ready to receive qty and send it on
	CCustomerCurrentItem *pCurItem= CCustomerCurrentItem::instance();
    CString csSending= pCurItem->ItemCode() + _T("{ENTER}");
    tbtrace(TM_ENTRY, _T("+-HandleItemQty() sending:%s"), csSending);
//	tbtrace(TM_ENTRY, _T("+-HandleItemQty()"));	
    CCustomerTBConnectPOS::instance()->SendSequence(csSending);
	return TB_IGNORE;
}

/*************************************************************************
* HandleItemWeight - 
*
* Parameters: 
*  LPARAM - lparam passed from the MessageMap
*
* Returns: TBSTATE - 
*************************************************************************/
TBSTATE CSolutionMHItem::HandleItemWeight(LPARAM lparam)
{
	//ensure POS at a state ready to receive the weight and sent it on
	tbtrace(TM_ENTRY, _T("+-HandleItemWeight()"));	
	return TB_IGNORE;
}

/*************************************************************************
* HandleItemScan - 
*
* Parameters: 
*  LPARAM - lparam passed from the MessageMap
*
* Returns: TBSTATE
*************************************************************************/
TBSTATE CSolutionMHItem::HandleItemScan(LPARAM lparam)
{
	//ensure POS at a state ready to receive the scan and sent it on
	tbtraceIO(_T("HandleItemScan()"));
    
	CCustomerCurrentItem *pCurItem= CCustomerCurrentItem::instance();
    CScannerMsg scan;
    scan.BarcodeType((CScannerMsg::BarcodeTypes)pCurItem->GetScanCodeType());
    scan.Data(pCurItem->ItemCode());


	tbtrace(TM_INFO, _T("Sending Scan Data to POS: BarcodeType: %d, Barcode: %s"), scan.BarcodeType(), (LPCTSTR)scan.Data());
    CCustomerTBConnectPOS::instance()->SendMsgToPOS(scan);
	return TB_IGNORE;
}

/*************************************************************************
* HandleItemKeyed - 
*
* Parameters: 
*  LPARAM - lparam passed from the MessageMap
*
* Returns: TBSTATE
*************************************************************************/
TBSTATE CSolutionMHItem::HandleItemKeyed(LPARAM lparam)
{
	//ensure POS at a state ready to receive the keyed data and sent it on
	CCustomerCurrentItem *pCurItem= CCustomerCurrentItem::instance();
    CString csSending= pCurItem->ItemCode() + _T("{ENTER}");
    tbtrace(TM_ENTRY, _T("+-HandleItemKeyed() sending:%s"), csSending);
    CCustomerTBConnectPOS::instance()->SendSequence(csSending);

	return TB_IGNORE;
}

/*************************************************************************
* ProcessMessage - Process the message posted by TB. TB analyzes the message 
*       and returns the state of the host application, based on the presentation 
*       data in the message. This will either be item details, or reward details.
*		The item details conform to the item control as defined in TBControls and
*		in a binary message. The Reward message is still in XML.
*
* Parameters: 
*  CTBMsgObj *pMsg - Pointer to the TB message
*
* Returns: TBSTATE - the current state of the host application
*************************************************************************/
TBSTATE CSolutionMHItem::ProcessMessage(CTBMsgObj *pMsg)
{ 
    CCustomerCurrentItem* pCurrItem = CCustomerCurrentItem::instance();
	TBSTATE rc = CBaseMHItem::ProcessMessage(pMsg);
	CCustomerTrxBroker *pCurTrx = CCustomerTrxBroker::instance(); // POS19072
	
	if (TB_ITEMSOLD == rc)
	{
		m_nNumberOfItemsSold++;

		
		if (IsItemACoupon(pMsg->UserLong()))
		{
			pCurrItem->IsCoupon(true);
		}
		
		if (pCurrItem->QuantityLong() == 0)
		{
			pCurrItem->Quantity((long)1);
		}

        if ( pCurrItem->WeightLong() )
        {
            // Must zero scale for ACS after selling item.
            this->SendWeight( 0 );
        }

        
        //check and see if we have a valid itemcode. 
        //if not use the dept number if possible
        CString csItemCode = pCurrItem->ItemCode();
        if(csItemCode.IsEmpty())
        {
            int iDeptCode = pCurrItem->Department();
            CString csDeptCode;

            if(iDeptCode > 0)
            {
                 LPTSTR pstr = csDeptCode.GetBufferSetLength(255);
                _ltot((long)iDeptCode,pstr ,10);

                csDeptCode.TrimLeft();
                csDeptCode.TrimRight();

                if(AfxIsValidString(csDeptCode))
                {
                    pCurrItem->ItemCode(csDeptCode);
                }else
                {
                    pCurrItem->ItemCode(_T("1"));
                }
            }else
            {

                pCurrItem->ItemCode(_T("1"));
            }
        }

        if(csItemCode == _T("VOID"))
        {
            pCurrItem->ItemCode((LPCTSTR)m_csPrevItemCode);
            pCurrItem->IsVoided(true);

            if (pCurrItem->LinkedItem() != true )
            {
                //play the set items asside wave file
                CCustomerMHOptions* pOpts = CCustomerMHOptions::instance();
                if ( (pOpts->OptionValue(_T("AskCustomerToSetAsideCancelItems")) == _T("1")) &&
                     (pCurTrx->TBGetProperty(_T("CustomerHasWICFreeItems"), true) == _T("0")) // POS19072: Do not use audio during automatic voiding in WIC Free Item phase.
                   )
                {
                    CCustomerTrxBroker::instance()->TBSayPhrase( _T("SetItemAsideContinueWithPurchases.wma"));
                }
            }
        }   

	}

    return rc;
}

/*************************************************************************
* ProcessRewardMessage - Process the message posted by TB. TB analyzes the message 
*       and returns the state of the host application, based on the data in 
*		the message. This creates a CRewardLineDetailsMsg and passes up to
*		BaseMHItem which already has handling for this msg type.
*
* Parameters: 
*  CTBGenMsg *pMsg - Pointer to the message
*
* Returns: TBSTATE - the current state of the host application
*************************************************************************/
TBSTATE CSolutionMHItem::ProcessRewardMessage(CTBGenMsg *pMsg)
{
	TBSTATE rc = TB_REWARDLINE;
	CRewardLineDetailsMsg sRewardDetails;

	// For all parts of the DetailsMsg we need to know which type of msg this is
	_bstr_t szTmpVal = pMsg->GetValue2(_T("ContextType"));
	int nContextNum = _ttoi(szTmpVal);
	if ((nContextNum < 1) || (nContextNum > 3))
	{
		tbtrace(TM_ERROR, _T("Unexpected Reward Context Number [%d]"), nContextNum);
		rc = TB_IGNORE;
	}
	else // Valid Context number - so build the RewardDetailsMsg
	{
		// Get the Rewards Description first
		sRewardDetails.RewardDescription(ExtractRewardDesc(pMsg, nContextNum));

		// Compute the Value of the Reward
		if (!ComputeRewardValueOrPoints(pMsg, nContextNum, &sRewardDetails))
		{
			rc = TB_IGNORE;
		}
		else  // still a vliad reward, continue processing
		{
			// Save off the EntryID of this reward
			szTmpVal = pMsg->GetValue2(_T("RewardID"));
			if (szTmpVal.length() != 0)
				sRewardDetails.EntryID(_ttoi(szTmpVal) + 1000000); // adding 1000000 to keep unique numbers
			else
			{
				tbtrace(TM_WARNING, _T("RewardID not populated - possibly CMR.dll not correct version!"));
				sRewardDetails.EntryID(0);
			}

			//save off related EntryID - also determines position of reward
			szTmpVal = pMsg->GetValue2(_T("EntryID"));
			if (szTmpVal.length() != 0)
			{
				sRewardDetails.AssociatedEntryID(_ttoi(szTmpVal));
				sRewardDetails.RewardLocation((CRewardLineDetailsMsg::tRewardLinePosType)BELOW_REF_ENTRYID);
			}
			else
			{
				tbtrace(TM_WARNING, _T("EntryID not populated - possibly CMR.dll not correct version!"));
				sRewardDetails.AssociatedEntryID(0);
				sRewardDetails.RewardLocation((CRewardLineDetailsMsg::tRewardLinePosType)BOTTOM_OF_RECEIPT);
			}

			// determine is this is a new reward or a voided reward
			sRewardDetails.IsVoided(IsRewardVoided(pMsg, nContextNum));

			// now that we have a completed RewardDetails msg, pass it up to base
			SetRewardLineDetails(&sRewardDetails);
		}
	}

	return rc;
}

#define VOID_ON_DESC 167
bool CSolutionMHItem::IsRewardVoided(CTBGenMsg *pMsg, int nContextNum)
{
	bool bVoidedReward = false;


	_bstr_t szTmpVal = pMsg->GetValue2(_T("VoidIndicator1"));
	int nVoidIndicator = _ttoi(szTmpVal);
	bVoidedReward = (nVoidIndicator == VOID_ON_DESC);
	if (nContextNum > 1)
	{
		szTmpVal = pMsg->GetValue2(_T("VoidIndicator2"));
		nVoidIndicator = _ttoi(szTmpVal);
		bVoidedReward  = bVoidedReward || (nVoidIndicator == VOID_ON_DESC);
	}
	if (nContextNum > 2)
	{
		szTmpVal = pMsg->GetValue2(_T("VoidIndicator3"));
		nVoidIndicator  = _ttoi(szTmpVal);
		bVoidedReward  = bVoidedReward || (nVoidIndicator == VOID_ON_DESC);
	}
	
	return bVoidedReward;
}

bool CSolutionMHItem::ComputeRewardValueOrPoints(CTBGenMsg *pMsg, int nContextNum, 
												 CRewardLineDetailsMsg  *pRewardDetails)
{
	bool rc = true;
	_bstr_t szValue;        
	_bstr_t szType;

    try
    {
        szValue = pMsg->GetValue2(_T("ExtendedRewardValue"));    
        szType = pMsg->GetValue2(_T("RewardType"));

    }catch(...)
    {
        tbtrace(TM_ERROR, _T("Cannot read  ExtendedRewardValue or RewardType- CMR.dll not correct version!"));
        return false;
		
    }

	int nRewardType = _ttoi(szType);
	int nTotalValue = 0;

	if (0 == szValue.length()) //field not populated - not using correct cmr.dll build
	{
		tbtrace(TM_ERROR, _T("ExtendedRewardValue not populated - CMR.dll not correct version!"));
		rc = false; //unrecoverable error, just get out of here
	}
	else 
	{
		nTotalValue = _ttoi(szValue); // use this value

		if (0 == szType.length())
		{
			tbtrace(TM_ERROR, _T("RewardType not populated - CMR.dll not correct version!"));
			rc = false; //unrecoverable error, just get out of here
		}
		else
		{
			pRewardDetails->UpdateRewardTotal(true);
			if (IsMonetaryReward(nRewardType))
			{
				pRewardDetails->RewardValue(nTotalValue);
				pRewardDetails->ShowRewardPoints(true);
			}
			else if (IsPointReward(nRewardType))
			{
				pRewardDetails->RewardPoints(nTotalValue);
				pRewardDetails->ShowRewardPoints(false);
			}
			else
			{
				tbtrace(TM_ERROR, _T("Unknown RewardType [%d]"), nRewardType);
				rc = false;
			}
		}
	}
	return rc;
}

CString CSolutionMHItem::ExtractRewardDesc(CTBGenMsg *pMsg, int nContextNum)
{
	_bstr_t szDesc = pMsg->GetValue2(_T("RewardDesc1"));
	CString csRewardDesc = (TCHAR *)szDesc;
	if (nContextNum > 1) // this could be 2 or 3 line context
	{
		szDesc = pMsg->GetValue2(_T("RewardDesc2"));
		csRewardDesc += (TCHAR *)szDesc;
	}
	if (nContextNum > 2) // this is 3 line context
	{
		szDesc = pMsg->GetValue2(_T("RewardDesc3"));
		csRewardDesc += (TCHAR *)szDesc;
	}

	return csRewardDesc;
}

bool CSolutionMHItem::IsItemACoupon(const int nItemType)
{
    CString csTest;
	csTest.Format(_T(";%d;"), nItemType);

    if(m_csCouponTypeList.Find(csTest) != -1)
    {
        return true;
    }
	else
    {
        return false;
    }
}

bool CSolutionMHItem::IsMonetaryReward(const int nRewardType)
{
    CString csTest;
	csTest.Format(_T(";%d;"), nRewardType);

    if(m_csMonetaryRewardList.Find(csTest) != -1)
    {
        return true;
    }
	else
    {
        return false;
    }
}

bool CSolutionMHItem::IsPointReward(const int nRewardType)
{
    CString csTest;
	csTest.Format(_T(";%d;"), nRewardType);

    if(m_csPointRewardList.Find(csTest) != -1)
    {
        return true;
    }
	else
    {
        return false;
    }
}

//ensure that the semicolon seperated string beoth starts and ends with semicolons
void CSolutionMHItem::SetSpecialHandlingItemList(CString strSpecialItem, CString &csVar)
{
    csVar = CCustomerMHOptions::instance()->OptionValue(strSpecialItem);
    if (csVar.GetLength() > 0)
    {
        if(csVar[0] != _T(';'))
        {
            csVar = _T(';') + csVar;
        }

        if(csVar[csVar.GetLength() - 1] != _T(';'))
        {
            csVar += _T(';');
        }
    }
}

TBRC CSolutionMHItem::SendWeight(long lWeight)
{
    tbtraceIO (_T("SendWeight"));
    tbtrace( TM_INFO, _T("SendWeight(%d)"), lWeight);
    CScaleMsg scale;
    scale.Weight(lWeight);
    CCustomerTBConnectPOS::instance()->SendMsgToPOS(scale);
    return TB_SUCCESS;
}

TBRC CSolutionMHItem::EnterItem(LPCTSTR szItemCode, const BOOL fVoid, const BOOL fOverride, 
                           const long nQuantity, const long nWeight, const long nTareCode,
                           const long lPrice, const BOOL fQuantityConfirmed, 
                           PSEnterItemDetails psMoreDetails)
{
    m_csPrevItemCode = szItemCode;

    // Need to save the weight in a hook property.
    // ACS Solution - ACS changes the weight by a factor of 10 and 
    // introduces a rounding error!
    // We know the weight because we (FL) weighed it - use our value!
    CCustomerTrxBroker *pCurTrx = CCustomerTrxBroker::instance();
    CString csWeight;
    csWeight.Format(_T("%d"), nWeight);
    pCurTrx -> TBSetProperty(_T("ItemWeight"), csWeight, true);

    if (CString(pCurTrx->GetCurrentInProgressStateName()) == _T("ITEMIZATION"))
    {
        // POS83168: This item is going to be sent to the POS sales app, so make sure
        // FastLane waits for the sales app to be ready for scan input before it
        // enables the scanner for the next item entry.
        pCurTrx->TBSetProperty(_T("WaitForPosScannerReady"), _T("1"), true);
        tbtrace(TM_INFO, _T("WaitForPosScannerReady=1"));
    }

    return (CBaseMHItem::EnterItem(szItemCode, fVoid, fOverride, 
                           nQuantity, nWeight, nTareCode,
                           lPrice, fQuantityConfirmed, 
                           psMoreDetails));
    return TB_SUCCESS;
}

TBRC CSolutionMHItem::EnterDOB(int nYear, int nMonth, int nDay, int nCustomerAge)
{
    CString csEnteredDOB;   
    TCHAR buf[100];
    CString csYear = _ltot(nYear, buf, 10);

    csEnteredDOB.Format(_T("%02d%02d%s"),nMonth,nDay,csYear.Right(2));

    CCustomerTrxBroker *pCurTrx = CCustomerTrxBroker::instance();
    pCurTrx->TBSetProperty(_T("EnteredDateOfBirth"), csEnteredDOB, true);

    pCurTrx->SetCurrentInProgressState(_T("ITEMIZATION_AGE"));
    pCurTrx->TriggerMsgMap();

    return TB_SUCCESS;

}





