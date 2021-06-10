// BaseMHItem.cpp: implementation of the CBaseMHItem class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseMHItem.h"
#include "TraceSystem.h"
#include "TBUtilities.h"
#include "BaseCurrentItem.h"
#include "BaseObjectManager.h"
#include "CustomerTrxBroker.h"
#include "CustomerTBProperties.h"

#define T_ID  _T("CBaseMHItem")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBaseMHItem::CBaseMHItem()
{
    ZeroMemory(&m_RewardLineDetails, sizeof(SRewardLineDetails));
}

CBaseMHItem::~CBaseMHItem()
{
    ResetRewardLineDetails();
}


/*************************************************************************
* EnterItem - Called by FL to sell or void an item. Base level traces parameter
*       values. It is up to Customer level to implement functionality
*
* Parameters: 
*  LPCTSTR szItemCode - item code as keyed in or extracted from barcode if 
*                       items is scanned
*  BOOL fVoid - TRUE:void item, FALSE:sell item
*  BOOL fOverride - TRUE: use override if needed; FALSE: no override
*  long nQuantity - Quantity of the items to be entered into the transaction. 
*                   For non-quantity-required items, use the value 0.
*  long nWeight - Weight associated with each item
*  long nTareCode - Tare code associated with each item. TB calculates the 
*                   weight associated with this tare code automatically.
*  long lPrice - Price of each item.
*  BOOL fQuantityConfirmed - Used if nQuantity is above confirmation limit.
*  PSEnterItemDetails psMoreDetails - structure containing lUnitPrice, scan type
*                                     and multi-label barcode data
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHItem::EnterItem(LPCTSTR szItemCode, const BOOL fVoid, const BOOL fOverride, 
                           const long nQuantity, const long nWeight, const long nTareCode,
                           const long lPrice, const BOOL fQuantityConfirmed, 
                           PSEnterItemDetails psMoreDetails)
{
    /*<%= SetItemInfo() %>*/
    /*<%ASIS
    SendReturnStatusMsg( TB_ITEMSOLD );
    %>*/

    tbtraceIO(_T("EnterItem"));
    if (psMoreDetails)
    {
       tbtrace(TM_INFO, _T("ItemCode[%s],Void[%d],Override[%d],Qty[%d],Weight[%d],TareCode[%d],Price[%d],QtyConfirmed[%d],EntryId[%s]"),
          szItemCode, fVoid, fOverride, nQuantity, nWeight, nTareCode, lPrice, fQuantityConfirmed, psMoreDetails->szItemID);
    }
    else
    {
       tbtrace(TM_INFO, _T("ItemCode[%s],Void[%d],Override[%d],Qty[%d],Weight[%d],TareCode[%d],Price[%d],QuantityConfirmed[%d]"),
           szItemCode, fVoid, fOverride, nQuantity, nWeight, nTareCode, lPrice, fQuantityConfirmed);
    }
    if (szItemCode==NULL || (szItemCode[0] == '\0'))
    {
        tbtrace( TM_WARNING, _T("EnterItem() called without szItemCode"));
        return TB_E_INVALIDPARAM;
    }
        
    CCustomerTrxBroker &Trx= *CCustomerTrxBroker::instance();
    BOOL bAdjustedVoidFlag = fVoid;
    //if we are in assistmode then change state to an assistmode
    //substate

    if(Trx.InAssistMode())
    {
        CString csInprogressState = Trx.GetCurrentInProgressStateName();
        //if assistmode is voiding then set the void flag so it doesn't sell it
        if( csInprogressState == _T("ASSISTMODE_VOID"))
        {
            bAdjustedVoidFlag = true;
        }
        
        Trx.SetCurrentInProgressState(_T("ASSISTMODE_ENTERITEM_FLAPI"));
        
    }else
    {

        Trx.SetCurrentInProgressState(_T("TB_ENTERITEM_FLAPI"));
    }
    SaveToCurrentItem(szItemCode, bAdjustedVoidFlag, fOverride, 
                           nQuantity, nWeight, nTareCode,
                           lPrice, fQuantityConfirmed, 
                           psMoreDetails);


 
    
 
    Trx.TriggerMsgMap();
   
    
    return TB_SUCCESS;
}


void CBaseMHItem::SaveToCurrentItem(LPCTSTR szItemCode, const BOOL fVoid, const BOOL fOverride, 
                           const long nQuantity, const long nWeight, const long nTareCode,
                           const long lPrice, const BOOL fQuantityConfirmed, 
                           PSEnterItemDetails psMoreDetails)
{
    //save off the void flag and the prices.  TB will get a message back from the 
    //POS but the price will be zero.  Will need to return a valid price back to 
    //FL
    CCustomerCurrentItem *pCurrentItem = CCustomerCurrentItem::instance();
    CCustomerTBProperties *pProp = CCustomerTBProperties::instance();

    pCurrentItem->Reset();
    if(fVoid)
    {
        if(psMoreDetails)
        {            
            pCurrentItem->UnitPrice(psMoreDetails->lUnitPrice);
            pCurrentItem->ExtendedPrice(psMoreDetails->lExtPrice);
        }
    }else
    {
        pCurrentItem->UnitPrice(lPrice);
        pCurrentItem->ExtendedPrice(lPrice);
    }
    pProp->SetProperty(TBPROP_ITEMPRICE, (LPCTSTR) pCurrentItem->ExtendedPrice());

    pCurrentItem->ItemCode(szItemCode);
    pProp->SetProperty(TBPROP_ITEMCODE, (LPCTSTR) pCurrentItem->ItemCode());

    pCurrentItem->IsVoided((fVoid != 0 ));
    pCurrentItem->SetOverride((fOverride != 0));

    pCurrentItem->Quantity(nQuantity);
    pProp->SetProperty(TBPROP_QTY, (LPCTSTR) pCurrentItem->Quantity());

    pCurrentItem->SetQuantityConfirmed((fQuantityConfirmed != 0));
    pCurrentItem->UnknownItem(false); // found item by default

    pCurrentItem->Weight(nWeight);
    pProp->SetProperty(TBPROP_WEIGHT, (LPCTSTR) pCurrentItem->Weight());

	 // KSo TARE support
    pCurrentItem->TareCode(nTareCode);
	 CString strTareCode;
	 strTareCode.Format(_T("%d"), nTareCode);
    pProp->SetProperty(TBPROP_ITEMTARE, (LPCTSTR) strTareCode);


    if (psMoreDetails)
    {
        pCurrentItem->SetScanCodeType(psMoreDetails->lItemScanCodeType);
        if((psMoreDetails->szItemID != NULL) && (psMoreDetails->szItemID[0] != '\0'))
        {
            pCurrentItem->EntryID(_ttoi(psMoreDetails->szItemID));
            pProp->SetProperty(TBPROP_ENTRYID, (LPCTSTR) psMoreDetails->szItemID);
        }
    }else 
    {
        pCurrentItem->SetScanCodeType(0);
    }
}

/*************************************************************************
* EnterDOB - Inform host application the Customer’s Date of Birth if needed
*       Base level traces parameter values. It is up to Customer level to 
*       implement functionality
*
* Parameters: 
*  int nYear - valid year of customer DOB
*  int nMonth - valid month of customer DOB
*  int nDay - valid day of customer DOB
*  int nCustomerAge - calculated customer age
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHItem::EnterDOB(int nYear, int nMonth, int nDay, int nCustomerAge)
{
    tbtrace(TM_INFO, _T("EnterDOB - Year[%d], Month[%d], Day[%d], CustAge[%d]"), 
        nYear, nMonth, nDay, nCustomerAge);
    return TB_SUCCESS;
}


/*************************************************************************
* GetItemDetails - FL requests TB to populate a structure with pertinent
*       item details. This will be called with FL receives TB_ITEMSOLD, or 
*       any other item sale related state event (TB_NEEDQUANTITY, TB_NEEDWEIGHT, etc)
*       This function will not reset the item details anymore - the exact 
*       timing of that is customer specific!
*
* Parameters: 
*  PSItemDetails psItemDetails - populate this struct with the item details
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHItem::GetItemDetails(PSItemDetails psDetails)
{
    tbtraceIO(_T("GetItemDetails()"));
    CBaseCurrentItem* pCurrItem = CCustomerCurrentItem::instance();

    psDetails->lDescriptionLength =
		 CopyString(psDetails->szDescription, pCurrItem->ItemDescription());

    psDetails->lErrDescriptionLength = 
		CopyString(psDetails->szErrDescription, pCurrItem->ErrorDescription());

    CopyString(psDetails->szItemCode, pCurrItem->ItemCode());

    psDetails->lExtPrice = pCurrItem->ExtendedPriceLong();
    psDetails->lUnitPrice = pCurrItem->UnitPriceLong();
    psDetails->lQuantity = pCurrItem->QuantityLong();
    psDetails->lWeight = pCurrItem->WeightLong();
    psDetails->lDealQuantity = pCurrItem->GetDealQuantity();
    psDetails->lItemQuantityLimit = pCurrItem->GetItemQuantityLimit();
    psDetails->fCoupon = pCurrItem->IsCoupon();
    psDetails->fNeedQuantity = pCurrItem->NeedQuantity();
    psDetails->fNeedWeight = pCurrItem->NeedWeight();
    psDetails->fNeedPrice = pCurrItem->NeedPrice();
    psDetails->fNotFound = pCurrItem->UnknownItem();
    psDetails->fNotForSale = pCurrItem->NotForSale();
    psDetails->fRestricted = pCurrItem->IsRestricted();
    psDetails->nRestrictedAge = pCurrItem->RestrictedAge();
    psDetails->nTareCode = pCurrItem->TareCode();
    psDetails->fDetails = pCurrItem->GetDetails();
    psDetails->fOverride = pCurrItem->GetOverride();
    psDetails->fVoid = pCurrItem->IsVoided();
    psDetails->fQuantityConfirmed = pCurrItem->GetQuantityConfirmed();
    psDetails->fQuantityLimitExceeded = pCurrItem->GetQuantityLimitExceeded();
    psDetails->fVisualVerify = pCurrItem->VisualVerify();
    psDetails->lScanCodeType = pCurrItem->GetScanCodeType();
    psDetails->bLinkedItem = pCurrItem->LinkedItem();
    psDetails->lDepartment = pCurrItem->Department();
    psDetails->lEntryID = pCurrItem->EntryID();
    psDetails->lRequireSecBagging = pCurrItem->GetRequireSecBagging();
    psDetails->lRequireSecSubstChk = pCurrItem->GetRequireSecSubstChk();
    psDetails->bSecurityTag = pCurrItem->SecurityTag();

    CString csEntryId;
    csEntryId.Format(_T("%d"), pCurrItem->EntryID());
    CopyString(psDetails->szItemID, csEntryId);

    try {
        tbtrace(TM_INFO, _T("Desc[%s], ItemCode[%s], EntryID[%d], szItemID[%s], ExtPrice[%d], UnitPrice[%d], Qty[%d], Weight[%d]"),
            psDetails->szDescription, psDetails->szItemCode, psDetails->lEntryID, psDetails->szItemID,
            psDetails->lExtPrice,psDetails->lUnitPrice,psDetails->lQuantity,psDetails->lWeight);
        tbtrace(TM_DEBUG, _T("lDealQuantity[%d], lItemQuantityLimit[%d], fCoupon[%d], fNeedQuantity[%d]"),
            psDetails->lDealQuantity,psDetails->lItemQuantityLimit,psDetails->fCoupon,psDetails->fNeedQuantity);
        tbtrace(TM_DEBUG, _T("fNeedWeight[%d], fNeedPrice[%d], fNotFound[%d], fNotForSale[%d]"),
            psDetails->fNeedWeight,psDetails->fNeedPrice,psDetails->fNotFound,psDetails->fNotForSale);
        tbtrace(TM_DEBUG, _T("fRestricted[%d], nRestrictedAge[%d], nTareCode[%d], fDetails[%d]"),
            psDetails->fRestricted,psDetails->nRestrictedAge,psDetails->nTareCode,psDetails->fDetails);
        tbtrace(TM_DEBUG, _T("fOverride[%d], fVoid[%d], fQuantityConfirmed[%d], fQuantityLimitExceeded[%d]"),
            psDetails->fOverride,psDetails->fVoid,psDetails->fQuantityConfirmed,psDetails->fQuantityLimitExceeded);
        tbtrace(TM_DEBUG, _T("fVisualVerify[%d], lScanCodeType[%d], bLinkedItem[%d]"),
            psDetails->fVisualVerify,psDetails->lScanCodeType,psDetails->bLinkedItem);
        tbtrace(TM_DEBUG, _T("szErrDescription[%s], lErrDescriptionLength[%d]"),
            psDetails->szErrDescription,psDetails->lErrDescriptionLength);
        tbtrace(TM_DEBUG, _T("lDepartment[%d],  lRequireSecBagging[%d], lRequireSecSubstChk[%d], bSecurityTag[%d], bPickListItem[%d]"),
            psDetails->lDepartment,psDetails->lRequireSecBagging,psDetails->lRequireSecSubstChk,
            psDetails->bSecurityTag,psDetails->bPickListItem);
    }
    catch(...){tbtrace(TM_ERROR, _T("Error - logging item details threw exception!"));}

    return TB_SUCCESS;
}


/*************************************************************************
* GetItemExtendedDetails - Get extended detailed information on the last 
*       item in the transaction. After confirming that the TB supports 
*       this function, with TBIsAllowed, the application should call 
*       this API when it receives TB_ITEMSOLD or any other item sale 
*       related state event from TB.
*       This function will not reset the item details anymore - the exact 
*       timing of that is customer specific!
*
* Parameters: 
*  PSItemExtendedDetails psItemExtendedDetails - populate this struct with the details
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHItem::GetItemExtendedDetails(PSItemExtendedDetails psDetails)
{
    tbtraceIO(_T("GetItemExtendedDetails()"));
    CBaseCurrentItem* pCurrItem = CCustomerCurrentItem::instance();

    // Extended details
    CopyString(psDetails->szCategory, pCurrItem->GetCategory());
    psDetails->ExceptionCode =  pCurrItem->GetExceptionCode();
    psDetails->dwExceptionFlags = pCurrItem->GetExceptionFlags();
    psDetails->dwTBOptionFlags = pCurrItem->GetTBOptionFlags();
    CopyString(psDetails->szExceptionWav, pCurrItem->GetExceptionWav());
    
    try {
        tbtrace(TM_DEBUG, _T("szCategory[%s], ExceptionCode[%d], dwExceptionFlags[%d]"),
            psDetails->szCategory, psDetails->ExceptionCode, psDetails->dwExceptionFlags);
        tbtrace(TM_DEBUG, _T("dwTBOptionFlags[%d], szExceptionWac[%s]"),
            psDetails->dwTBOptionFlags, psDetails->szExceptionWav);
    }
    catch(...){tbtrace(TM_ERROR, _T("Error - logging item details threw exception!"));}

    return TB_SUCCESS;
}


/*************************************************************************
* GetRewardLineDetails - When TB returns TB_REWARDLINE, FL calls this
*       function to collect the details of that reward line.
*
* Parameters: 
*  PSRewardLineDetails psRewardLineDetails - the reward line details struct
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHItem::GetRewardLineDetails(PSRewardLineDetails psRewardLineDetails)
{
    tbtraceIO (_T("GetRewardLineDetails()"));

    // NOTE: MAKE SURE to set szDescription to NULL first
    // because the buffer is ALWAYS dirty
    psRewardLineDetails->szDescription=NULL;
    CopyString(psRewardLineDetails->szDescription, m_RewardLineDetails.szDescription);
    psRewardLineDetails->lValue = m_RewardLineDetails.lValue;
    psRewardLineDetails->bSuppressValue = m_RewardLineDetails.bSuppressValue;
    psRewardLineDetails->lEntryID = m_RewardLineDetails.lEntryID;
    psRewardLineDetails->lEntryIDForReference = m_RewardLineDetails.lEntryIDForReference;
    psRewardLineDetails->bUpdateRewardTotal = m_RewardLineDetails.bUpdateRewardTotal;
    psRewardLineDetails->lRewardPoints = m_RewardLineDetails.lRewardPoints;
    psRewardLineDetails->lEmphasis = m_RewardLineDetails.lEmphasis;
    psRewardLineDetails->nOperatationType = m_RewardLineDetails.nOperatationType;
    psRewardLineDetails->nPosType = m_RewardLineDetails.nPosType;

    try {
        tbtrace(TM_INFO, _T("szDescription[%s], lValue[%d], bSuppressValue[%d]"),
            psRewardLineDetails->szDescription,psRewardLineDetails->lValue, psRewardLineDetails->bSuppressValue);
        tbtrace(TM_INFO, _T("lEntryID[%d], lEntryIDForReference[%d], bUpdateRewardTotal[%d], bUpdateRewardTotal[%d]"),
            psRewardLineDetails->lEntryID, psRewardLineDetails->lEntryIDForReference, 
            psRewardLineDetails->bUpdateRewardTotal, psRewardLineDetails->bUpdateRewardTotal);
        tbtrace(TM_INFO, _T("lRewardPoints[%d], lEmphasis[%d], nOperatationType[%d], nPosType[%d]"),
            psRewardLineDetails->lRewardPoints, psRewardLineDetails->lEmphasis, psRewardLineDetails->nOperatationType, psRewardLineDetails->nPosType);
    }
    catch(...){tbtrace(TM_ERROR, _T("Error - logging reward line details threw exception!"));}

    return TB_SUCCESS;
}


/*************************************************************************
* ProcessMessage - Process the message posted by TB. TB analyzes the message 
*       and returns the state of the host application, based on the presentation 
*       data in the message. This function looks at the device id, and then casts
*       the msg element to an item or reward line details msg.
*
* Parameters: 
*  PSMessageElement psMessage - Pointer to the TB message
*
* Returns: TBSTATE - TB_ITEMSOLD, TB_REWARDLINE, or TB_IGNORE if an unknown device id
*************************************************************************/
TBSTATE CBaseMHItem::ProcessMessage(CTBMsgObj *pMsg)
{
    tbtraceIO(_T("ProcessMessage()"));
    TBSTATE rc = TB_IGNORE;
    
    ASSERT(pMsg);
    switch (pMsg->GetDeviceId()) 
    {
    case TB_DEV_ITEM_DETAILS:
        {
            rc= TB_ITEMSOLD;
            CItemDetailsMsg *pItemMsg= dynamic_cast<CItemDetailsMsg *> (pMsg);
            ASSERT(pItemMsg);
            if (pItemMsg) {
                CBaseCurrentItem* pCurrItem = CCustomerCurrentItem::instance();
                pCurrItem->Refresh(pItemMsg);
            }
        }
        break;
    case TB_DEV_REWARD_DETAILS:
        {
            rc= TB_REWARDLINE;
            CRewardLineDetailsMsg *pRMsg= dynamic_cast<CRewardLineDetailsMsg *> (pMsg);
            ASSERT(pRMsg);
            if (pRMsg) {
                SetRewardLineDetails(pRMsg);
            }
            break;
        }
    default:
        ASSERT(0);
    }
    return rc;
}

/*************************************************************************
* SetRewardLineDetails - copies a RewardLineDetailsMsg into this classes 
*       member variable to be passed to FL when requested
*
* Parameters:
*  CRewardLineDetailsMsg * pRewardDetails - the struct to copy
*
* Returns: void
*************************************************************************/
void CBaseMHItem::SetRewardLineDetails(CRewardLineDetailsMsg * pRewardDetails)
{
    tbtraceIO(_T("SetRewardLineDetails()"));
    
    CopyString(m_RewardLineDetails.szDescription, (LPCTSTR) pRewardDetails->RewardDescription());
    m_RewardLineDetails.lValue = pRewardDetails->RewardValue();
    m_RewardLineDetails.bSuppressValue = ! pRewardDetails->ShowRewardPoints();
    m_RewardLineDetails.lEntryID = pRewardDetails->EntryID();
    m_RewardLineDetails.lEntryIDForReference = pRewardDetails->AssociatedEntryID();
    m_RewardLineDetails.bUpdateRewardTotal = pRewardDetails->UpdateRewardTotal();
    m_RewardLineDetails.lRewardPoints = pRewardDetails->RewardPoints();
    
    //m_RewardLineDetails.lEmphasis = pRewardDetails->Emphasis; // future usage

    m_RewardLineDetails.nOperatationType = (pRewardDetails->IsVoided() ? REWARDOP_DELETE :
    REWARDOP_INSERT);
    
    m_RewardLineDetails.nPosType = (tRewardLinePosType) pRewardDetails->RewardLocation();
}

/*************************************************************************
* ResetRewardLineDetails - resets all members of the reward line details struct.
*
* Parameters: none
*
* Returns: void
*************************************************************************/
void CBaseMHItem::ResetRewardLineDetails()
{
    tbtrace(TM_ENTRY, _T("+-ResetRewardLineDetails()"));
    
    if( m_RewardLineDetails.szDescription )
    {
        delete m_RewardLineDetails.szDescription;
        m_RewardLineDetails.szDescription = NULL;
    }
    m_RewardLineDetails.lValue = 0;
    m_RewardLineDetails.bSuppressValue = false;
    m_RewardLineDetails.lEntryID = 0;
    m_RewardLineDetails.lEntryIDForReference = 0;
    m_RewardLineDetails.bUpdateRewardTotal = false;
    m_RewardLineDetails.lRewardPoints = 0;
    m_RewardLineDetails.lEmphasis = 0;
    m_RewardLineDetails.nOperatationType = REWARDOP_NONE;
    m_RewardLineDetails.nPosType = POS_NONE;
    
}

/*************************************************************************
* SendWeight - Inform host application of the weight currently on scale.
*       This is only called while in assist mode when FL detects weight on 
*       the scale and the last return was TB_NEEDWEIGHT. This will need to
*       be implemented at the customer specific level.
*
* Parameters: 
*  long lWeight - the weight on the scale
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHItem::SendWeight(long lWeight)
{
    tbtrace( TM_INFO, _T("+-SendWeight(%d) "), lWeight);
    return TB_SUCCESS;
}

/*************************************************************************
* SendQuantity - Inform host application of the quantity entered by the 
*		customer. The value comes in through the EnterItem API. Base logic
*       will be to key in the quantity as key strokes, and press <Enter>.
*
* Parameters: 
*  long lQuantity - the item quantity keyed in
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHItem::SendQuantity(long lQuantity)
{
    tbtrace( TM_ENTRY, _T("+-SendQuantity(%d) "), lQuantity);
    return TB_SUCCESS;
}

/*************************************************************************
* HandleItemEntry - 
*
* Parameters: 
*  LPARAM - lparam passed from the MessageMap
*
* Returns: TBSTATE
*************************************************************************/
TBSTATE CBaseMHItem::HandleItemEntry(LPARAM lparam)
{
	tbtraceIO(_T("HandleItemEntry"));
	TBSTATE rc = TB_IGNORE;
	
	// All the information about the item being sold is in the CurrentItem class:
	CCustomerCurrentItem *pCurrentItem= CCustomerCurrentItem::instance();
	//are we voiding an item?
	if(pCurrentItem->IsVoided())
	{
		// void the POS item
		rc = HandleItemVoid(lparam);
	}
	//do we have a qty that must be entered
	else if(pCurrentItem->QuantityLong() > 0)
	{
		//send items qty to POS
		rc = HandleItemQty(lparam);
	}
	//do we have a weight that must be entered
	else if(pCurrentItem->WeightLong() > 0)
	{
		rc = HandleItemWeight(lparam);
	}
	else if(pCurrentItem->GetScanCodeType()) 
	{
		// item is scanned. Send as scan
		rc = HandleItemScan(lparam);
	}
	else
	{
		// item is not scanned - send as keyed if necessary
		rc = HandleItemKeyed(lparam);
	}
	
	return rc;
}

/*************************************************************************
* HandleItemVoid - 
*
* Parameters: 
*  LPARAM - lparam passed from the MessageMap
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBSTATE CBaseMHItem::HandleItemVoid(LPARAM lparam)
{
	tbtrace(TM_ENTRY, _T("+-HandleItemVoid()"));	
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
TBSTATE CBaseMHItem::HandleItemQty(LPARAM lparam)
{
	//ensure POS at a state ready to receive qty and send it on
	tbtrace(TM_ENTRY, _T("+-HandleItemQty()"));	
	return TB_IGNORE;
}

/*************************************************************************
* HandleItemWeight - 
*
* Parameters: 
*  LPARAM - lparam passed from the MessageMap
*
* Returns: TBSTATE
*************************************************************************/
TBSTATE CBaseMHItem::HandleItemWeight(LPARAM lparam)
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
TBSTATE CBaseMHItem::HandleItemScan(LPARAM lparam)
{
	//ensure POS at a state ready to receive the scan and sent it on
	tbtrace(TM_ENTRY, _T("+-HandleItemScan()"));	
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
TBSTATE CBaseMHItem::HandleItemKeyed(LPARAM lparam)
{
	//ensure POS at a state ready to receive the keyed data and sent it on
	tbtrace(TM_ENTRY, _T("+-HandleItemKeyed()"));
	return TB_IGNORE;
}
