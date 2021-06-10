// CustomerMHItem.cpp: implementation of the CCustomerMHItem class.
//
//////////////////////////////////////////////////////////////////////
/***********************************************************************
/* Change History                                            
/*
/*  POS329313 Work Request:79417 Name: Alexander Bronola  Date:August 24, 2017
/*  POS277295 Work Request:73595 Name:Alexander Bronola/Veerasekhara D 	Date:August 12, 2016
/*  POS248467 Work Request:68942 Name:Robert Susanto Date:April 14, 2016
/*  POS242535 Work Request:68942 Name:Saiprasad Srihasam    Date:April 05, 2016
/*  POS203228 Work Request:68237 Name:Robert Susanto    Date:June 5, 2015
/*  POS149568 Work Request:66559 Name:Saiprasad Srihasam    Date:December 18, 2014
/*  POS114261 Work Request:65347 Name:Saiprasad Srihasam Date:September 3, 2014
/*  POS94866 Work Request:61233 Name: Saiprasad Srihasam Date:July 25, 2014
/*  POS94866 Work Request:61233 Name: Saiprasad Srihasam Date:July 21, 2014
/*  POS48277/71132 Work Request:58657 Name: Robert Susanto   Date:September 13, 2013
/*  POS63296  Work Request:56285 Name:Robert Susanto Date:June 5, 2013
/*  POS25385  Work Request:16656 Name:Matt Condra    Date:Dec 9, 2011
/*  POS:20334 Work Request:16656 Name:Matt Condra Date:October 3 2011 			
/*  POS:15676 Work Request:16656 Name:Matt Condra Date:September 20 2011  			
/*  POS:15745 Work Request:16632 Name:Robert Susanto Date:August 11 2011  			
/*  POS:14776 Work Request:15196 Name:Robert Susanto Date:June 10 2011
/*  POS:12625 Work Request:15196 Name:Sandeep Shenoy Date:May 2 2011
/*  POS:11013 Work Request:11013 Name:Robert Susanto Date:February 10 2011
/*  POS:10851 Work Request:10859 Name:Robert Susanto Date:February 4 2011
/************************************************************************/
#include "stdafx.h"
#include "CustomerMHItem.h"
#include "TraceSystem.h"
#include "CustomerCurrentItem.h"
#include "CustomerTBScotMsg.h"
#include "CustomerTrxBroker.h"
#include "SolutionTBConnectPOS.h"
#include "CustomerCurrentTransaction.h"
#include "scannerMsg.h"
#include "CustomerTBSharedDefs.h"
#include "CustomerMHOptions.h"
#include "CustomerTBProperties.h"
#include "BaseMessages.h"
#include "CustomerTBConnectFL.h"
#include "TBUtilities.h"

// SOTF 8380 O2S - RRS
#include "CustomerMsgMap.h"
// E - SOTF 8380 O2S - RRS

#define T_ID _T("CCustomerMHItem")

#undef super
#define super CSolutionMHItem


long CCustomerMHItem::m_nRewardLineEntryID = 100000;
long CCustomerMHItem::m_lEntryIDtemp = 0;
bool CCustomerMHItem::isFLMSTransferring = false;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCustomerMHItem::CCustomerMHItem()
{
	m_lFLRewardID = 0;

    //sotf 6226
    ResetDelayedAgeRequiredTLOGData();	
    
	SetSpecialHandlingItemList(_T("GreenDotItems"), m_csGreenDotBarcodes);
	
	//build list of Green Dot barcodes
	if(!m_csGreenDotBarcodes.IsEmpty())
	{
		m_VecGreenDotBarcodesList.clear();
		long lDelimiter =0;
		long lNextDelimiter =0;

		lDelimiter = m_csGreenDotBarcodes.Find(_T(";"));
		do{

			lNextDelimiter = m_csGreenDotBarcodes.Find(_T(";"), (lDelimiter +1 ));
			if ((lNextDelimiter != -1) && (lNextDelimiter >lDelimiter ))
			{
			


				m_VecGreenDotBarcodesList.push_back(m_csGreenDotBarcodes.Mid((lDelimiter + 1), (lNextDelimiter - lDelimiter - 1)));
			}

			lDelimiter = lNextDelimiter;

		}while(lDelimiter != -1);

	}

	SetSpecialHandlingItemList(_T("PicklistLightweightItems"), m_csPiclistLightWeightItems);

	//build list of Green Dot barcodes
	if(!m_csPiclistLightWeightItems.IsEmpty())
	{
		m_VecPicklistLightWeightList.clear();
		long lDelimiter =0;
		long lNextDelimiter =0;

		lDelimiter = m_csPiclistLightWeightItems.Find(_T(";"));
		do{

			lNextDelimiter = m_csPiclistLightWeightItems.Find(_T(";"), (lDelimiter +1 ));
			if ((lNextDelimiter != -1) && (lNextDelimiter >lDelimiter ))
			{
			


				m_VecPicklistLightWeightList.push_back(m_csPiclistLightWeightItems.Mid((lDelimiter + 1), (lNextDelimiter - lDelimiter - 1)));
			}

			lDelimiter = lNextDelimiter;

		}while(lDelimiter != -1);

	}


    
}

CCustomerMHItem::~CCustomerMHItem()
{
}


/*************************************************************************
* GetItemDetails - FL requests TB to populate a structure with pertinent
*       item details. This will be called with FL receives TB_ITEMSOLD, or 
*       any other item sale related state event (TB_NEEDQUANTITY, TB_NEEDWEIGHT, etc)
*       Base level will populate the structure. Customer level must reset
*       details when appropriate
*
* Parameters: 
*  PSItemDetails psItemDetails - populate this struct with the item details
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CCustomerMHItem::GetItemDetails(PSItemDetails psDetails)
{
	CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();
    CCustomerTBProperties & props = *CCustomerTBProperties::instance();
    
	TBRC rc = super::GetItemDetails(psDetails);
    
	CString strTemp = psDetails->szItemCode;
		
	if(strTemp.Compare(_T("VOID")) == 0)
	{
	    psDetails->fVoid = true;
	}    

	//CRD 232268
	tbtrace(TM_INFO, _T("::ItemDetails EntryID - %d"), psDetails->lEntryID);
	m_lEntryIDtemp =  psDetails->lEntryID;
   
    //SOTF 6226
    //moved this logic to the SSF because it is now more complicated
    //with delayed age turned on.  Basically, there are some conditions
    //where the fRestricted flag needs to remain on even though the 
    //item was sold in assistmode.  The case where there are other 
    //delayed age items sold by the guest
	//CString  csCurTBState = pCurTrx->GetCurrentInProgressStateName();

	//if (csCurTBState.Find(_T("ASSISTMODE")) != -1)
	//{
	//	psDetails->fRestricted = false;
	//}
    //-sotf

    psDetails->lWeight *= 10;	// Factor of 10!

	CString csDealQuantity = props.GetHookProperty(_T("DealQuantity"));
	psDetails -> lDealQuantity = _ttol(csDealQuantity); 

    if( strTemp.IsEmpty() && props.GetHookProperty(_T("IsResumeTrxInProgress")) == _T("1") )
    {
        // We're resuming and an intervention has been requested, populate
        // the itemcode with a dummy value to prevent assertion and scotapp to 
        // assume the item is a linked item (default handling)
        CopyString(psDetails->szItemCode, _T("RESUMEITEM"));
    }

    if(IsItemPicklistLightWeightItem(psDetails->szItemCode))
    {
		    tbtrace(TM_INFO, _T("Ice Bag sold!!! Setting substituion check to TB_OVERRIDE_SECURITY_WITH_NO!!!"));
          psDetails->lRequireSecSubstChk = TB_OVERRIDE_SECURITY_WITH_NO;

    }

    // if item details are always repopulated then it is safe to reset them here.
    // keep in mind the this will be called with NEEDQUANTITY and NEEDWEIGHT
    // are returned, so this may be an intermediate request for details. An
    // additional request will come once the item is sold.
    CCustomerCurrentItem::instance()->ResetItemDetails(); // if appropriate
	pCurTrx->TBSetProperty(_T("PropIsProcessingItem"), _T("0"), true);

    return rc;
}

/*************************************************************************
* GetItemExtendedDetails - Get extended detailed information on the last 
*       item in the transaction. After confirming that the TB supports 
*       this function, with TBIsAllowed, the application should call 
*       this API when it receives TB_ITEMSOLD or any other item sale 
*       related state event from TB.
*       Base level will populate the structure. Customer level must reset
*       details when appropriate
*
* Parameters: 
*  PSItemExtendedDetails psItemExtendedDetails - populate this struct with the details
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CCustomerMHItem::GetItemExtendedDetails(PSItemExtendedDetails psDetails)
{
    TBRC rc = super::GetItemExtendedDetails(psDetails);

    // if item details are always repopulated then it is safe to reset them here.
    // keep in mind the this will be called with NEEDQUANTITY and NEEDWEIGHT
    // are returned, so this may be an intermediate request for details. An
    // additional request will come once the item is sold.
    CCustomerCurrentItem::instance()->ResetItemExtendedDetails(); // if appropriate

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
TBSTATE CCustomerMHItem::HandleItemVoid(LPARAM lparam)
{
    // it's possible that we are attempting to void
    // a TMD or Guest Card here.  This would be the case
    // if the stored entry ID is MSGx.
    CCustomerCurrentItem & item = *CCustomerCurrentItem::instance();
    CCustomerTBProperties & props = *CCustomerTBProperties::instance(); //POS94866
        props.SetHookProperty(_T("IsFromMEPAvailable"), _T("1"));  //POS94866

    return super::HandleItemVoid(lparam);
}

/*************************************************************************
* HandleItemQty - 
*
* Parameters: 
*  LPARAM - lparam passed from the MessageMap
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBSTATE CCustomerMHItem::HandleItemQty(LPARAM lparam)
{
    return super::HandleItemQty(lparam);
}

/*************************************************************************
* HandleItemWeight - 
*
* Parameters: 
*  LPARAM - lparam passed from the MessageMap
*
* Returns: TBSTATE - 
*************************************************************************/
TBSTATE CCustomerMHItem::HandleItemWeight(LPARAM lparam)
{
    return super::HandleItemWeight(lparam);
}

/*************************************************************************
* HandleItemScan - 
*
* Parameters: 
*  LPARAM - lparam passed from the MessageMap
*
* Returns: TBSTATE
*************************************************************************/
TBSTATE CCustomerMHItem::HandleItemScan(LPARAM lparam)
{
	CCustomerCurrentItem *pCurItem= CCustomerCurrentItem::instance();
	CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();
    CCustomerTBProperties &props = *CCustomerTBProperties::instance();
	CString csFirstAlpha;

	//Drivers License Sscanning detection and manipulation
	if(props.GetProperty(_T("IsDLScanning_Prop")) == _T("1") &&
		props.GetProperty(_T("UseThisAlphaTable")) == _T("Y"))
	{
		CString csIsSecondSetAlpha(_T("0"));
		if(props.GetHookProperty(_T("PropDynaFrameID")) == _T("372"))
			csIsSecondSetAlpha = _T("1");
			
		props.SetProperty(_T("IsDLScanning_Prop"),_T("0"));
      
		CString itemCode = pCurItem->ItemCode();
		if(!iswalpha(itemCode.GetAt(0))) //check first char
		{
			tbtrace(TM_INFO, _T("Scanner data is not a Drivers License...returning TB_IGNORE"));
			return TB_IGNORE;
		}

		csFirstAlpha = itemCode.Left(1);
		tbtrace(TM_INFO, _T("First DL alpha char(%s)"), csFirstAlpha);

		//get the equivalent dynakey from acsopts.dat
		csFirstAlpha = props.GetProperty(csFirstAlpha + csIsSecondSetAlpha);
		//store it into a property to be used in tbmsgmap.xml
		props.SetProperty(_T("DL_First_Alpha"),csFirstAlpha);

		if(iswalpha(itemCode.GetAt(1)))
		{ //it means Ohio DL. two alpha at the beginning
			CString csSecondAlpha = _T(" ");
			csSecondAlpha.SetAt(0,itemCode.GetAt(1));
			tbtrace(TM_INFO, _T("Second DL alpha char(%s)"), csSecondAlpha);

			//get the equivalent dynakey from acsopts.dat
			csSecondAlpha = props.GetProperty(csSecondAlpha + csIsSecondSetAlpha);
			
            //store it into a property to be used in tbmsgmap.xml
			props.SetProperty(_T("DL_Second_Alpha"),csSecondAlpha);
			props.SetProperty(_T("DL_Numeric_Data"),itemCode.Mid(2));

			pCurTrx->SetCurrentInProgressState(_T("OHIO_DL_SCANNING_STATE"));

			tbtrace(TM_INFO, _T("Second DL alpha Key(%s)"), props.GetProperty(_T("DL_Second_Alpha")));
		}
		else //regular DL with only one alpha
		{
            props.SetProperty(_T("DL_Numeric_Data"),itemCode.Mid(1));
			pCurTrx->SetCurrentInProgressState(_T("DL_SCANNING_STATE"));
		}
		
		tbtrace(TM_INFO, _T("First DL alpha Key(%s)"), props.GetProperty(_T("DL_First_Alpha")));
		tbtrace(TM_INFO, _T("DL numeric data(%s)"), props.GetProperty(_T("DL_Numeric_Data")));
		
		pCurTrx->TriggerMsgMap();
		return TB_IGNORE;
	} //end of Drivers License
	

        //this is for one pass card. add 0 at the begginning because the leading zero drop off
	//along the way in sending to POS. Add zero also to all barcode with numberic at the beginning
	CScannerMsg scanmsg;
	scanmsg.BarcodeType((CScannerMsg::BarcodeTypes)pCurItem->GetScanCodeType());

/*  SMarks commented out this code because its is no longer needed
	if(pCurItem->ItemCode().GetLength() == 30 || 
        ( (scanmsg.BarcodeType() != 106) && (pCurItem->ItemCode().Left(1).FindOneOf(_T("123456789")) != -1))) 

	{
		CString csItemCode = _T("0") + pCurItem->ItemCode();
		pCurItem->ItemCode(csItemCode);
			tbtrace(TM_INFO, _T("TB decided a 0 needs to be added to front of barcode"));
	}
*/

	CScannerMsg scan;
	scan.BarcodeType((CScannerMsg::BarcodeTypes)pCurItem->GetScanCodeType());
	CString modifiedBarcode = ModifyBarcode(pCurItem->ItemCode(), pCurItem->GetScanCodeType());  //CRD 328873

	scan.Data(modifiedBarcode);

	tbtrace(TM_INFO, _T("Sending Scan Data to POS: BarcodeType: %d, modified Barcode: %s"), scan.BarcodeType(), modifiedBarcode);//(LPCTSTR)scan.Data());
	CCustomerTBConnectPOS::instance()->SendMsgToPOS(scan); 
	return TB_IGNORE;
	//}

	//return super::HandleItemScan(lparam);
}


/*************************************************************************
* HandleItemEntry - 
*
* Parameters: 
*  LPARAM - lparam passed from the MessageMap
*
* Returns: TBSTATE
*************************************************************************/
TBSTATE CCustomerMHItem::HandleItemEntry(LPARAM lparam)
{
    CCustomerCurrentItem *pCurItem= CCustomerCurrentItem::instance();
	CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();
    CCustomerTBProperties & props = *CCustomerTBProperties::instance();

	pCurTrx->TBSetProperty(_T("IsResumeTrxInProgress"), _T("0"),true); 
	pCurTrx->TBSetProperty(_T("TMDOrGuestCardScanned"), _T("0"),true);
    // TAR360799
    pCurTrx->TBSetProperty(_T("RewardLineDisplay"), _T("0"), false);

    props.SetHookProperty(_T("IsFromMEPAvailable"), _T("1"));  //POS94866

	CString itemCode = pCurItem->ItemCode();	

	//special OH DL barcode example
    //	itemCode = _T("POHCOLUMBUS^MOUSE$MICKEY$$^1970 W BROAD ST^6360232012544690=14011942010110432231102  D A             140070BLKBRO");

	//POS290418 - P means scanned from RAP and OH is for Ohio DL
	if(itemCode.Left(3) == _T("POH"))
	{
        itemCode = itemCode.Mid(1, itemCode.GetLength() - 1);
        pCurItem->ItemCode(itemCode);
		pCurItem->SetScanCodeType(201); //PDF417
	}

	CScannerMsg scan;
	scan.BarcodeType((CScannerMsg::BarcodeTypes)pCurItem->GetScanCodeType());
	tbtrace(TM_INFO, _T("BarcodeType: %d"),  scan.BarcodeType());

    /* 
	if( scan.BarcodeType() == 132 || 
        scan.BarcodeType() == 502   ) //RSS barcode
	{
		itemCode = itemCode.Mid(4); //strip off the 0100
		tbtrace(TM_INFO, _T("Itemcode: %s"),  itemCode);
		pCurItem->ItemCode(itemCode);
		pCurItem->SetScanCodeType(0);
	}
    */

	//this is for ME319081 - Background Host Inquiry Messages for TMD and Community Rewards
	//go back to scan and bag to allow scanning of additional item while Background host inquire is
	//in progress. this is to prevent fastlane from hunging also when pos is performing Background host inquire 
	
    // POS20334 - Don't force to go to S&B screen while POS is still processing the NSC4 card.  Let the POS
    // finished processing the NSC4 then customer could continue with scanning another item. - rrs

	// Code has been modified to support NSC4 based Once and Done coupons as well 
    /*if( IsItemWithNoItemDetailsExpected(itemCode) ) // SOTF5803
    {        
        CCustomerTBScotMsg::instance()->SendReturnStateMsg(TB_CANCEL_ITEM, NULL, NULL);        
        CCustomerTBScotMsg::instance()->SendReturnStateMsg(TB_NEED_SCANANDBAG, NULL, NULL);
    }*/
    
	//Convert Pharmacy Item scan code to Keyed in Item
	if(itemCode.Left(1) == _T("P") &&
		props.GetProperty(_T("UseThisAlphaTable")) == _T("Y")) //if this flag is disable meaning sending of alphanumeric is OK now, no need to convert to keyincode
	{
                //hook recognize only small p so, convert P to p here
		itemCode = itemCode.Mid(1, itemCode.GetLength() - 1);
                itemCode = _T("p") + itemCode;

                //we need to make sure that big P will arrive at POS so, turn the caps on here
		bool capsOn = (bool)(GetKeyState(VK_CAPITAL) & 0x1);
		if(!capsOn)
		{
		   keybd_event(VK_CAPITAL, 0, 0, 0);
		   Sleep(300);
		   keybd_event(VK_CAPITAL, 0, KEYEVENTF_KEYUP, 0);
		}

		pCurItem->ItemCode(itemCode);
		pCurItem->SetScanCodeType(0);
	}

	return  super::HandleItemEntry(lparam);
}

/*************************************************************************
* HandleItemKeyed - 
*
* Parameters: 
*  LPARAM - lparam passed from the MessageMap
*
* Returns: TBSTATE
*************************************************************************/
TBSTATE CCustomerMHItem::HandleItemKeyed(LPARAM lparam)
{
    return super::HandleItemKeyed(lparam);
}


BOOL CCustomerMHItem::DetectIsBogusItem(void)
{
	CCustomerCurrentItem* pCurrItem = CCustomerCurrentItem::instance();

	// If the item details is empty from the POS then ignore the packet
	 // This is causes One Free processing and BagEAS to display in incorrectl places.
	return ((pCurrItem->ItemDescription()  ==_T("")) &&
	    (pCurrItem->ItemCode() ==_T("1"))		&&   // set some place. The details passed has ""
		(pCurrItem->ExtendedPrice() ==_T(""))	&&
		(pCurrItem->UnitPrice() ==_T(""))		&&
		(pCurrItem->Quantity() ==_T("1"))		&&
		(pCurrItem->Weight()== _T(""))			&&   // set some place. The details passed has ""
		(pCurrItem->IsCoupon() == false)		&&
		(pCurrItem->NeedQuantity() == false)	&&
		(pCurrItem->NeedWeight() == false)		&&
		(pCurrItem->NeedPrice() == false)		&&
		(pCurrItem->UnknownItem() == false)		&&
		(pCurrItem->NotForSale() == false)		&&
		(pCurrItem->IsRestricted() == false)	&&
		(pCurrItem->RestrictedAge() == 0)		&&
		(pCurrItem->TareCode() == 0)			&&
		(pCurrItem->IsVoided() == false)		&&
		(pCurrItem->VisualVerify() == false)	&&
		(pCurrItem->LinkedItem() == false)		&&
		(pCurrItem->ErrorDescription() == _T(""))	&&
		(pCurrItem->Department() == 0)			&&
		(pCurrItem->EntryID() == 0)				);
}

void CCustomerMHItem::HandleRebateItemAttributeChange(BOOL bAddRebateReceiptLine)
{
	tbtraceIO(_T("HandleRebateItemAttributeChange()"));

	CCustomerCurrentItem* pCurrItem = CCustomerCurrentItem::instance();
	CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();

	PS_UI_CMD psLocalUICmd = new S_UI_CMD;
	ZeroMemory(psLocalUICmd, sizeof(S_UI_CMD));
    
    psLocalUICmd->InitUnionField();
	// set the command type
    psLocalUICmd->eStructType = UI_STR_RECEIPT;
	psLocalUICmd->Rcpt.SetRcptName(_T("CMSMReceipt"));

	if( bAddRebateReceiptLine )
	{
		tbtrace(TM_INFO, _T("Turning Rebate Item Flag ON for EntryID '%d'"), pCurrItem->EntryID());
		CString strEntryId;
		psLocalUICmd->Rcpt.eItemCmd = UI_ITEM_CMD_INSERT;
		strEntryId.Format(_T("%d"), pCurrItem->EntryID());
		CString strRebateEntryId = strEntryId + _T("LNKRebateItem");

		psLocalUICmd->Rcpt.SetRefItemID(strEntryId);
		psLocalUICmd->Rcpt.SetItemID(strRebateEntryId);

		PS_UI_VAR pFetchProperty = new S_UI_VAR;
		ZeroMemory(pFetchProperty, sizeof(S_UI_VAR));
        pFetchProperty->InitStruct();
        pFetchProperty->SetVarName(_T("ITEM_MSG"));
        pFetchProperty->eAccessCmd = UI_ACCESS_SET;
        pFetchProperty->vVarValue = COleVariant(_T("  Rebate item")).Detach();
        psLocalUICmd->Rcpt.pUiVars[0] = pFetchProperty;
	}
	else
	{
		tbtrace(TM_INFO, _T("Turning Rebate Item Flag OFF for EntryID '%d'"), pCurrItem->EntryID());
		CString strEntryId;
		strEntryId.Format(_T("%dLNKRebateItem"), pCurrItem->EntryID());
		psLocalUICmd->Rcpt.eItemCmd = UI_ITEM_CMD_REMOVEITEM;
		psLocalUICmd->Rcpt.SetItemID(strEntryId);
	}

    CCustomerTBScotMsg::instance()->SendScotMsg(TB_DEV_UICOMMAND, 0, NULL,(LPARAM) psLocalUICmd);
}


TBSTATE CCustomerMHItem::ProcessMessage(CTBMsgObj *pMsg)
{
	CCustomerCurrentTransaction  * pCurTrans = CCustomerCurrentTransaction::instance();
	CCustomerCurrentItem* pCurrItem = CCustomerCurrentItem::instance();
	CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();
    CCustomerTBProperties & props = *CCustomerTBProperties::instance();

	CString  csCurTBState = pCurTrx->GetCurrentInProgressStateName();

	CString csIsRebatedItem = pMsg->UserString();
	if (csIsRebatedItem == _T("") || csIsRebatedItem.GetLength() == 0)
    {
        csIsRebatedItem = _T("0");
    }
	tbtrace(TM_INFO, _T("csIsRebatedItem = %s"), csIsRebatedItem);

    CItemDetailsMsg *pItemMsg= dynamic_cast<CItemDetailsMsg *> (pMsg);
    ASSERT(pItemMsg);
    
    // Treat ITEM DISCOUNT type items as linked items
    bool bIsItemDiscountItem = (pItemMsg->ItemDescription().Find(_T("ITEM DISCOUNT")) != -1);
    bool bIsVoidIndicator = (props.GetHookProperty(_T("PropVoidIndicator")) == _T("167"));

    // get item code value before solution level call.
    CString csItemCode;
    if( bIsItemDiscountItem && bIsVoidIndicator )
    {
        pItemMsg->ItemCode(_T("VOID"));
        csItemCode = _T("VOID");
    }
    else
    {
        csItemCode = pItemMsg->ItemCode(); 
    }

    // Verify if item is truly a linked item 
    if( pItemMsg->LinkedItem() && (csItemCode != _T("VOID")) )
    {
        //POS14776 - Don't use the compare, instead use the Find function as the POS could also put the checkdigit at the
        // end of the LinkedItemCode 
        //the linked item code would have to match the current item, otherwise, discard linked item.
        if( pItemMsg->LinkedItemCode().Find(pItemMsg->ItemCode()) == -1 ) //   Compare(pItemMsg->ItemCode()) )
        {
            pItemMsg->LinkedItem(false);
        }
    }
    else if( bIsItemDiscountItem )
    {
        pItemMsg->LinkedItem(true);
    }

    TBSTATE rc = CSolutionMHItem::ProcessMessage(pMsg);

    //SOTF 6226
    //if we are doing age restricted items as immediate
    //remove the age so that FL does not do it as a delay
    if(m_bACSDelayedAgeRestricted == false) 
    {
        pCurrItem->IsRestricted(false);
        pCurrItem->RestrictedAge(0);

    }else
    {
        //if we're dealing with an age restricted item
        //save off the max age required
        long lTmpAge = pCurrItem->RestrictedAge();
        if((pCurrItem->IsRestricted()) || (lTmpAge))
        {
            pCurrItem->IsRestricted(true);            
            if(lTmpAge > m_lHighestAgeRequired)
            {
                m_lHighestAgeRequired = lTmpAge;

                //POS63296 - if another restricted item is scanned requiring older age, then save the information - rrs
                if(m_lUPCAge)
                {
                    m_lUPCAge = lTmpAge;
                    m_csUPCAgeRequired = pCurrItem->ItemCode();
                }
                //e - POS63296
                
            }

            //save off the item code of the very first
            //age rest. item for tlogging
            if(m_csUPCAgeRequired.IsEmpty())
            {
                m_csUPCAgeRequired = pCurrItem->ItemCode();
            }

            if(!m_lUPCAge)
            {
               m_lUPCAge = lTmpAge;
            }

        }
    }
    //-SOTF
    
    // SOTF 8380 - Order 2 store - RRS
    if( IsItemOrder2StoreBarcodes(pCurrItem->ItemCode(), pCurrItem->GetScanCodeType()) )
    {
        CBaseMessages *pMessages = CBaseMessages::instance();
        
        CString csItemDesc = pMessages->GetStringValueFromID(9275);
        csItemDesc += pCurrItem->ItemDescription();

        pCurrItem->ItemDescription(csItemDesc);
        pCurrItem->SetRequireSecBagging(TB_OVERRIDE_SECURITY_WITH_NOWEIGHT);
        pCurrItem->SetRequireSecSubstChk(TB_OVERRIDE_SECURITY_WITH_NOWEIGHT);


        if( pCurrItem->UserString() == _T("V") )
        {
            pCurrItem->IsVoided(true);
        }

        tbtrace(TM_INFO, _T("Order To Store Description (%s), ID (%d)"), pCurrItem->ItemDescription(), pCurrItem->EntryID() );

        if( props.GetHookProperty(_T("IsResumeTrxInProgress")) == _T("1") )
        {
            pCurTrx->IsItemScrolled(false);
		    
            tbtrace(TM_INFO, _T("Order To Store Box ID retrieved from suspended TRX") );
            return TB_ITEMSOLD;
        }
    }
    // E - SOTF 8380 - Order 2 store - RRS

    //POS48277 - MobilePayStation    
    if( props.GetProperty(_T("FastLaneConfiguration")) == _T("MobilePayStation") )
    {
        pCurrItem->SetRequireSecBagging(TB_OVERRIDE_SECURITY_WITH_NOWEIGHT);
        pCurrItem->SetRequireSecSubstChk(TB_OVERRIDE_SECURITY_WITH_NOWEIGHT);
        tbtrace(TM_INFO, _T("NO Weight for MobilePayStation!!!"));
    }
    //e POS48277 - MobilePayStation

    // Solution level will assign the previous item's ID for a void
    // this works fine in Customer mode which uses EnterItem, it won't
    // work in assist mode.
    if( csItemCode == _T("VOID") && (pCurrItem->ItemCode() != csItemCode) )
    {
        tbtrace(TM_WARNING, _T("Changed back ItemCode='VOID', Solution level had reassigned it to prev item '%s'"), pCurrItem->ItemCode());
        pCurrItem->ItemCode(_T("VOID"));
    }

	if(props.GetHookProperty(_T("IsResumeTrxInProgress")) == _T("1") &&
		csIsRebatedItem != _T("0"))
	{
		props.SetHookProperty(_T("RebateReceiptRequested"), _T("1"));
		HandleRebateItemAttributeChange(true);
	} 
	 

	 static UINT nPrevEntryID = 0;
	 
	 if( DetectIsBogusItem() )
	 {
		tbtrace(TM_INFO, _T("Ignoring item details since there are no details (Empty details)"));
		return TB_IGNORE;
	 }
	
     // Various security overrides for certain items
     if( 
         (props.GetHookProperty(_T("PropItemType")) == _T("9")) ||		// negative item
         (csCurTBState == _T("ITEMIZATION_PROCESSING_PHONE_GIFT")) ||	// TAR 372395
         (csCurTBState == _T("ITEMIZATION_SWIPE_NEEDED")) ||	// TAR 425230
         (pCurrItem->Department() == 420) ||                            // Promo Coupon department
         (pCurrItem->Department() == 428) ||                            // Meijer Extended Protection item (MEP) or other Warranty Item
         (props.GetHookProperty(_T("IsResumeTrxInProgress")) == _T("1")) ||
		(props.GetHookProperty(_T("IsSCOFLMS")) == _T("1"))	//CRD 177093
		 )// Resume transaction - skip security
     {	
         pCurrItem->m_nRequireSecBagging = TB_OVERRIDE_SECURITY_WITH_NOWEIGHT;
     }

	 CString strItemFlag = props.GetHookProperty(_T("ItemFlag"));

 	 if( (pCurrItem->IsVoided() != true) && 
         (  (pCurrItem->ExtendedPriceLong() < 0 && pCurrItem->ItemDescription().Find(_T("Photo")) != -1) ||
	      (pCurrItem->ExtendedPriceLong() < 0 && bIsVoidIndicator)  )
        ) /* fix for TAR#308863 */
	 {
		pCurrItem->IsVoided(true);
		CCustomerTrxBroker::instance()->TBSayPhrase( _T("SetItemAsideContinueWithPurchases.wma"));
	 }

	 if( 
         pCurTrans->DetectAndUpdateRebateItemAttributeChange() &&
		 pCurrItem->ItemDescription().Find(_T("Coupon")) == -1 && 
         props.GetHookProperty(_T("IsResumeTrxInProgress")) != _T("1")
       )
	 {
		 tbtrace(TM_INFO,
             _T("Changing Rebate Receipt Item attribute to %d"), pCurTrans->IsCurrentItemWithRebateReceipt());

		 // the state of the item's rebate flag has been changed
		 // we need to PIM this item and set the new flag accordingly
		 // 
		 HandleRebateItemAttributeChange(pCurTrans->IsCurrentItemWithRebateReceipt());

		 // now it could be that the rebate status changed as a result of the item being voided.
		 // we probably want to let the rest of the routine proceed, otherwise return
		 if( !pCurrItem->IsVoided() )
		 {
			return TB_IGNORE;
		 }
	 }
	 else if( pCurrItem->ItemDescription().Find(_T("Vendor")) != -1 && strItemFlag == _T("D"))
	 {
		 //no need to return TB_ITEMSOLD again if the customer just press the Void Only "Extra Coupon" button
		 return TB_IGNORE;
	 }
	//tar 355146 - voiding linked item
	 else if( pCurrItem->IsVoided() && pCurrItem->LinkedItem() && csItemCode != _T("VOID") && 
         (props.GetHookProperty(_T("Item_descriptor_last")) != props.GetHookProperty(_T("Item_descriptor_Prop"))) )
	 {
        CCustomerTrxBroker::instance()->TBSayPhrase( _T("blank.wav"));
	 }
	 else
	 {
		 CString strCurState = pCurTrx -> GetCurrentInProgressStateName();
		 if( strCurState.Find(_T("REBATEITEM")) != -1 )
		 {
			 // although the rebate item state of items has not changed - we're still handling 
			 // a rebate item change therefore the itemdetails should not be processed
			return TB_IGNORE;
		 }
	 }
	 
     
	 // If we are suspending, POS may send item details, but don't tell FL!
     //SOTF 6890
     //7915 allow resume items to be sent if we are in assist mode.  Otherwise
     //there will be no items on the e-receipt

/*	 CString csCurStateName = pCurTrx -> GetCurrentInProgressStateName();
	 if (( ( (csCurStateName.Find(_T("SUSPEND")) != -1) ||
	          ( (props.GetHookProperty(_T("IsResumeStarted")) == _T("1")) &&
	            (props.GetHookProperty(_T("IsResumeComplete")) == _T("0"))    // POS25385: Better definition of resumed items.
	          )
	        )
	      ) &&
	      (csCurStateName.Find(_T("ASSISTMODE")) == -1 )
		 )
	 {		
		return TB_IGNORE;
	 }*/
	 //}
	 /*POS12625-If POS sends a coupon context and POS is in Tender process, ignore at FL*/	 
	if( pCurrItem->IsCoupon() == true &&
		      csCurTBState == _T("TENDER_SELECTED") 			  
			)			  
	 {
		return TB_IGNORE;
	 }	 
	 else
	 {
		props.SetHookProperty(_T("IsFromMEPAvailable"), _T("0"));   //POS94866
		pCurTrx->IsItemScrolled(false);
		nPrevEntryID = pCurrItem->EntryID();
		return TB_ITEMSOLD;
	 }	
}

/*POS277295 */
TBSTATE CCustomerMHItem::ProcessMPerksRewardLine(CTBGenMsg *pMsg)
{
	tbtraceIO( _T("CCustomerMHItem::ProcessMPerksRewardLine"));
	
    CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();
	CCustomerCurrentTransaction* pCurrTrx = CCustomerCurrentTransaction::instance();
	
    tbtrace(TM_INFO, _T("Got an 800 acs_tb_interface"));

	
    _bstr_t szMnemonic = pMsg->GetValue2(_T("RewardDesc1"));
	_bstr_t szAccntNo = pMsg->GetValue2(_T("RewardDesc2"));
	_bstr_t szTb_long = pMsg->GetValue2(_T("RewardID"));

	CString csMnemonic = (TCHAR *)szMnemonic;
	CString csAccntNo = (TCHAR *)szAccntNo;
	CString csTb_long = (TCHAR *)szTb_long;
	
	tbtrace(TM_INFO, _T("Account no=%s, mnemonic=%s, tb_long=%s."), csAccntNo, csMnemonic, csTb_long);
	
	CString csEntryID = csTb_long + _T("mPerksLine");
	
	PS_UI_CMD psLocalUICmd = new S_UI_CMD;
	memset(psLocalUICmd, 0, sizeof(S_UI_CMD)); // to be safe
	psLocalUICmd->InitUnionField();
	
	psLocalUICmd->eStructType = UI_STR_RECEIPT;
	
	psLocalUICmd->Rcpt.SetItemID(csEntryID);
	
	// Set the receipt name
	psLocalUICmd->Rcpt.SetRcptName(_T("CMSMReceipt")); 
	
	if(csAccntNo == _T("") && csMnemonic == _T("")) //remove the line
	{         
		tbtrace(TM_INFO, _T("Removing mPerks reward line."));
		
		psLocalUICmd->Rcpt.eItemCmd = UI_ITEM_CMD_REMOVEMPERKS;
		CCustomerTBScotMsg::instance()->SendScotMsg(TB_DEV_UICOMMAND, 0, NULL, (LPARAM) psLocalUICmd);
		
	}
	else //add
	{   
		tbtrace(TM_INFO, _T("Adding mPerks reward line."));
		
		csAccntNo = _T(" ") + csAccntNo;
		// set UI command details
		psLocalUICmd->Rcpt.eItemCmd = UI_ITEM_CMD_INSERT;
		PS_UI_VAR pFetchProperty = new S_UI_VAR;
		pFetchProperty->InitStruct();
		pFetchProperty->SetVarName(_T("ITEM_DESCRIPTION"));
		pFetchProperty->eAccessCmd = UI_ACCESS_SET;
		pFetchProperty->vVarValue = COleVariant(csMnemonic + csAccntNo).Detach();
		psLocalUICmd->Rcpt.pUiVars[0] = pFetchProperty;
		
		CCustomerTBScotMsg::instance()->SendScotMsg(TB_DEV_UICOMMAND, 0, NULL,(LPARAM) psLocalUICmd); 
	}
	
	return TB_IGNORE;
} /*POS277295 */


TBSTATE CCustomerMHItem::ProcessRewardMessage(CTBGenMsg *pMsg)
{
	tbtraceIO( _T("CCustomerMHItem::ProcessRewardMessage"));

	/*POS277295 */
    CString csRewardType = _T("");
	//Try catch is added for POS290580
	try{
	  _bstr_t szType = pMsg->GetValue2(_T("RewardType"));
	   csRewardType = (TCHAR *)szType;
	}
	catch(...) {
        tbtrace(TM_INFO, _T("Reward Type is NULL"));
    }

 	tbtrace(TM_INFO, _T("Reward Type --> %s"), csRewardType);
	
	if(csRewardType == _T("800"))
		return ProcessMPerksRewardLine(pMsg); /*POS277295 */

	CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();
    CCustomerTBProperties & props = *CCustomerTBProperties::instance();

	_bstr_t bstrDesc = pMsg->GetValue2(_T("RewardDesc1"));
	CString csRewardDesc = (TCHAR *)bstrDesc;

//(+) POS149568
     tbtrace(TM_INFO, _T("Reward Description --> %s"), csRewardDesc);

     CString csWICDescOpt = CCustomerMHOptions::instance()->OptionValue(_T("WICTotalDesc"));

    if(csRewardDesc.Find(csWICDescOpt) >= 0)
    {
        bstrDesc = pMsg->GetValue2(_T("RewardAmt1"));
		CString csRewardValue = (TCHAR *)bstrDesc;
         csRewardValue.Remove('.');

        if(m_nRewardLineEntryID != 100000)
        {
            RemovePrevWICTotal(m_nRewardLineEntryID);
        }

        CRewardLineDetailsMsg sRewardDetails;
        sRewardDetails.RewardDescription(csRewardDesc);
        sRewardDetails.RewardValue(_ttol(csRewardValue));
        sRewardDetails.IsVoided(false);
        sRewardDetails.AssociatedEntryID(0);
        sRewardDetails.EntryID(++m_nRewardLineEntryID);
        sRewardDetails.RewardLocation((CRewardLineDetailsMsg::tRewardLinePosType)BOTTOM_OF_RECEIPT);
        SetRewardLineDetails(&sRewardDetails);
        return TB_REWARDLINE;
    }

//(-) POS149568

    // Determine which item this ACSReward applies to
    CString csEntryID(_T(""));
    _variant_t vt = pMsg->GetValue2(_T("EntryID"));
    if( vt.vt != VT_NULL )
    {
        csEntryID = (LPCTSTR)_bstr_t(vt);
    }
    
    long lEntryID = -1; // default to current
    if( csEntryID.GetLength() )
    {
        lEntryID = _ttol(csEntryID);
    }

    // Get the unique CM Reward ID
    CString csRewardID(_T(""));
    vt = pMsg->GetValue2(_T("RewardID"));
    if( vt.vt != VT_NULL )
    {
        csRewardID = (LPCTSTR)_bstr_t(vt);
    }
    
    long lRewardID = -1; // default to current
    if( csRewardID.GetLength() )
    {
        lRewardID = _ttol(csRewardID);
    }
	
	CString csIsSVLegalTender = props.GetHookProperty(_T("IsItSVCardLegalTender"));
	if (csIsSVLegalTender == _T("1"))
	{
		CString csSVCardLegalTender = props.GetProperty(_T("SVCardLegalTender"));
		bstrDesc = pMsg->GetValue2(_T("RewardAmt1"));
		CString csRewardAmt = (TCHAR *)bstrDesc;
		csRewardAmt = _T("  ") + csRewardAmt;
		csSVCardLegalTender += _T("\n");
		csSVCardLegalTender += csRewardAmt; 
		csSVCardLegalTender += _T("        ");
		csSVCardLegalTender += csRewardDesc;
		pCurTrx->TBSetProperty(_T("SVCardLegalTender"), csSVCardLegalTender, false);
		tbtrace(TM_INFO, _T("SVCardLegalTender = %s"), csSVCardLegalTender);
		
		return TB_IGNORE;
	}
	bstrDesc = pMsg->GetValue2(_T("VoidIndicator1"));
	int iVoidIndicator = _ttoi((TCHAR *)bstrDesc);

	if(csRewardDesc.Find(_T("Granted")) != -1) 
	{
	    AddReceiptCMDiscount(csRewardDesc, lRewardID, lEntryID);
	} 
	else if((iVoidIndicator != 0) && (iVoidIndicator != 166)) //!= 166 for tar re Santa Bucks Voided Discount
	{
        if( -1 != lRewardID )
        {
            RemoveReceiptCMDiscount(lRewardID, lEntryID);
        }
        else
        {
            // since we don't have a RewardID to match and remove, go ahead and add a
            // line item instead with a V in front of it -- as per ME309747
            AddReceiptCMDiscount(_T("V ") + csRewardDesc, lRewardID, lEntryID);
        }
	}
	else
	{
		bstrDesc = pMsg->GetValue2(_T("RewardAmt1"));
		CString csRewardVar = (TCHAR *)bstrDesc;
		csRewardDesc = _T("      ") + csRewardDesc + _T("                       ") + csRewardVar;

		bstrDesc = pMsg->GetValue2(_T("ContextType"));
		int nContextNum = _ttoi(bstrDesc);
		if ((nContextNum < 1) || (nContextNum > 3))
		{
			tbtrace(TM_ERROR, _T("Unexpected Reward Context Number [%d]"), nContextNum);
			return TB_IGNORE;
		}
		if (nContextNum > 1) // this could be 2 or 3 line context
		{
			bstrDesc = pMsg->GetValue2(_T("RewardDesc2"));
			csRewardVar = (TCHAR *)bstrDesc;
			csRewardDesc += _T("\r\n        ") + csRewardVar;
		}
		if (nContextNum > 2) // this is 3 line context
		{
			bstrDesc = pMsg->GetValue2(_T("RewardDesc3"));
			csRewardVar = (TCHAR *)bstrDesc;
			csRewardDesc += _T("\r\n        ") + csRewardVar;
		}

		AddReceiptCMDiscount(csRewardDesc, lRewardID, lEntryID);
	}

	return TB_IGNORE; //CSolutionMHItem::ProcessRewardMessage(pMsg);
}

void CCustomerMHItem::AddReceiptCMDiscount(CString csReward, long lRewardID, long lRefID)
{
	tbtraceIO( _T("CCustomerMHItem::AddReceiptCMDiscount"));

	CCustomerCurrentItem* pCurrItem = CCustomerCurrentItem::instance();

	CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();
	CCustomerCurrentTransaction* pCurrTrx = CCustomerCurrentTransaction::instance();
	//
	PS_UI_CMD psLocalUICmd = new S_UI_CMD;
    memset(psLocalUICmd, 0, sizeof(S_UI_CMD)); // to be safe
    psLocalUICmd->InitUnionField();
        // set the command type
    psLocalUICmd->eStructType = UI_STR_RECEIPT;
        
	CString csEntryID(_T(""));
    CString csRefEntryID(_T(""));

    // If there's no entry ID then it's not tied to an item.
    if( -1 != lRefID )
    {
        csRefEntryID.Format(_T("%d"),lRefID);
    }

    // If there's no reward ID provided, generate a unique # using counter
    if( -1 != lRewardID )
    {
        csEntryID.Format(_T("%d"), lRewardID);
        csEntryID += _T("CMDiscount");
    }
    else
    {
        csEntryID.Format(_T("%d"), ++m_lFLRewardID);
        csEntryID += _T("CMDiscountFL");
    }

	pCurrTrx->InsertedDiscount(csRefEntryID);

	psLocalUICmd->Rcpt.SetRefItemID(csRefEntryID);
    psLocalUICmd->Rcpt.SetItemID(csEntryID);
 
    // Set the receipt name
    psLocalUICmd->Rcpt.SetRcptName(_T("CMSMReceipt"));
    // set UI command details
    psLocalUICmd->Rcpt.eItemCmd = UI_ITEM_CMD_INSERT;
    PS_UI_VAR pFetchProperty = new S_UI_VAR;
    pFetchProperty->InitStruct();
    pFetchProperty->SetVarName(_T("ITEM_DESCRIPTION"));
    pFetchProperty->eAccessCmd = UI_ACCESS_SET;
    pFetchProperty->vVarValue = COleVariant(csReward).Detach();
    psLocalUICmd->Rcpt.pUiVars[0] = pFetchProperty;

    CCustomerTBScotMsg::instance()->SendScotMsg(TB_DEV_UICOMMAND, 0, NULL,(LPARAM) psLocalUICmd);
}

void CCustomerMHItem::RemoveReceiptCMDiscount(long lRewardID, long lRefID)
{
	tbtraceIO( _T("CCustomerMHItem::RemoveReceiptCMDiscount"));

	PS_UI_CMD psLocalUICmd = new S_UI_CMD;
    memset(psLocalUICmd, 0, sizeof(S_UI_CMD)); // to be safe
	psLocalUICmd->InitUnionField();

    psLocalUICmd->eStructType = UI_STR_RECEIPT;

    // Set the entry ID 
    CString csEntryID;
    csEntryID.Format(_T("%d"), lRewardID); 
	csEntryID += _T("CMDiscount");

    // Set the reference entry ID only if provided
    CString csRefEntryID(_T(""));
    if( -1 != lRefID )
    {
        csRefEntryID.Format(_T("%d"), lRefID); 
    }

    psLocalUICmd->Rcpt.SetRefItemID(csRefEntryID);
    psLocalUICmd->Rcpt.SetItemID(csEntryID);

    // Set the receipt name
    psLocalUICmd->Rcpt.SetRcptName(_T("CMSMReceipt"));

    // set UI command details
    psLocalUICmd->Rcpt.eItemCmd = UI_ITEM_CMD_REMOVEITEM;
    CCustomerTBScotMsg::instance()->SendScotMsg(TB_DEV_UICOMMAND, 0, NULL, (LPARAM) psLocalUICmd);
}


TBRC CCustomerMHItem::EnterItem(LPCTSTR szItemCode, const BOOL fVoid, const BOOL fOverride, 
                           const long nQuantity, const long nWeight, const long nTareCode,
                           const long lPrice, const BOOL fQuantityConfirmed, 
                           PSEnterItemDetails psMoreDetails)
{

    CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();
    CCustomerTBProperties & props = *CCustomerTBProperties::instance();

// (+)POS114261 - Return core TB_ITEMUNKNOWN instead of TB_E_INVALIDPARAM so that this is handled as unknown item and will avoid hang scenario.

    if (szItemCode==NULL || (szItemCode[0] == '\0'))
    {
        tbtrace( TM_WARNING, _T("EnterItem() called without szItemCode"));
         CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_ITEMUNKNOWN, NULL, NULL);   

         return TB_SUCCESS;

    }
// (-)POS114261

    /* Start SOTF7718 */
    CString csCurrState = pCurTrx->GetCurrentInProgressStateName();

    if(csCurrState.Find(_T("NO_SALE")) != -1)
    {
       tbtrace(TM_INFO, _T("NOSALE state in progress - Cancel item scan."));

       CCustomerTBScotMsg *pTBMsg = CCustomerTBScotMsg::instance();
       pTBMsg->SendReturnStateMsg( TB_CANCEL_ITEM, NULL, NULL ); 
       return TB_SUCCESS;
    }
    /* End SOTF7718 */

    //is this an intervention item
    if((IsItemGreenDotBarcodes(szItemCode)) && 
       (!pCurTrx->InAssistMode()) &&
       (props.GetHookProperty(_T("IsResumeTrxInProgress")) != _T("1")))
    {       
        if(csCurrState != _T("ITEMIZATION_GREEN_DOT_ITEM"))
        {
            tbtrace(TM_INFO, _T("Triggering intervention for Green Dot item."));

            pCurTrx->SetCurrentInProgressState(_T("ITEMIZATION_GREEN_DOT_ITEM"));
            
            CString csItemDesc(szItemCode);

            BuildGreenDotDataNeeded(csItemDesc);

            CCustomerTBScotMsg *pTBMsg = CCustomerTBScotMsg::instance();

            pTBMsg->SendReturnStateMsg( TB_CANCEL_ITEM, NULL, NULL );     
            pTBMsg->SendReturnStateMsg( TB_NEEDMOREDATA, NULL, NULL );
        }

        return TB_SUCCESS;
    }    

    // SOTF 8380 - Order 2 store - RRS
    if( IsItemOrder2StoreBarcodes(szItemCode, psMoreDetails->lItemScanCodeType) && 
       (!pCurTrx->InAssistMode()) && !pCurTrx->InAssistMenu() )
    {

        pCurTrx->SetCurrentInProgressState(_T("ITEMIZATION_ORDER_TO_STORE_ITEM"));

        CCustomerTBScotMsg *pTBMsg = CCustomerTBScotMsg::instance();

        pTBMsg->SendReturnStateMsg( TB_CANCEL_ITEM, NULL, NULL );     
        
        TBSTATE tbretState;
        tbretState = TB_NEEDMOREDATA;

        CCustomerMsgMap::instance()->msgDataNeededWithCustomerScreen(tbretState, (LPARAM)(LPCTSTR)_T("Order2Store;9273"));
        pTBMsg->SendReturnStateMsg( TB_NEEDMOREDATA, NULL, NULL );

        tbtrace(TM_INFO, _T("Triggering intervention for Order2Store item."));

        return TB_SUCCESS;
    }
    // E - SOTF 8380 - Order 2 store - RRS

    // Verify if the current EntryID is a message entry, if so, convert it to 
    // its EntryID equivalent stripping off the MSG prefix
    // keep stack variable definition at this level to ensure it stays alive
    // during call below to super::EnterItem()
    CString strMsgEntryID;

    if( psMoreDetails && (psMoreDetails->szItemID) && (psMoreDetails->szItemID[0]) )
    {
        LPCTSTR szMsg = _T("MSG");
        strMsgEntryID = psMoreDetails->szItemID;
        int iPos = strMsgEntryID.Find(szMsg);
        if( -1 != iPos )
        {
            strMsgEntryID.Delete(iPos, _tcslen(szMsg));
            // this is extremely unconventional and weird, just trying to follow
            // the same approached used in assigning the szItemID originally in 
            // SMStateBase::TBEnterItem()
            //          . . .
            //          sMoreItemDetails.szItemID = io.d.csItemID;
            psMoreDetails->szItemID = strMsgEntryID;
        }
    }

    //pos15745
    // Must zero scale before selling item.
    //this->SendWeight( 0 ); // POS248467 rrs
    // e pos15745

    return super::EnterItem(szItemCode, fVoid, fOverride, 
                           nQuantity, nWeight, nTareCode,
                           lPrice, fQuantityConfirmed, 
                           psMoreDetails);
}


bool CCustomerMHItem::IsItemGreenDotBarcodes(LPCTSTR str)
{
	CString csTest(str);
	
	long lSize = m_VecGreenDotBarcodesList.size();
	CString csTemp;

	for(long i=0; i<lSize; i++)
	{
		csTemp = m_VecGreenDotBarcodesList[i];
		if( csTest.Find(csTemp) != -1)
		{
			return true;
		}

        
    }
	return false;
}

void CCustomerMHItem::BuildGreenDotDataNeeded(LPCTSTR strItem)
{
    CString csItem(strItem);
    CString csItemToRemoveText;
    CString csDesc;

    CBaseMessages *pMessages = CBaseMessages::instance();
    csItemToRemoveText.Format(_T("%s\n\n%s%s"),pMessages->GetStringValueFromID(9195), pMessages->GetStringValueFromID(9196), strItem);
    
    csDesc = pMessages->GetStringValueFromID(9197);

    CString csDataNeededCommand;
    csDataNeededCommand.Format(_T("TOPCAPTION=%s;DETAILEDINSTRUCTION=%s;SUMMARYINSTRUCTION=%s;SCANNERINPUTENABLED=0;LSWND1=%s;LSWND2=%s;LANEBUTTONTEXT=%s;BUTTON0=cReturnGreenDotItem,%s;BUTTON1=cAssistModeGreenDotItem,%s;"),
                                   csDesc,                                /*TOPCAPTION*/
                                   csItemToRemoveText,                    /*DETAILEDINSTRUCTION*/
                                   _T(" "),                               /*SUMMARYINSTRUCTION*/
                                   pMessages->GetStringValueFromID(9197), /*LSWND1*/                                  
                                   csItem,                                /*LSWND2*/
                                   csDesc,                                /*LANEBUTTONTEXT */
                                   pMessages->GetStringValueFromID(58),   /*BUTTON0*/
                                   pMessages->GetStringValueFromID(269)); /*BUTTON1*/
    
                                    
    SDataNeededView MyDataNeeded={0};

    
    MyDataNeeded.szDescription= csDesc.GetBuffer(0);
    MyDataNeeded.szView = csDataNeededCommand.GetBuffer(0);
    CCustomerTBConnectFL::instance()->SetDataNeededView( &MyDataNeeded);

    //set up the string to display to the customer while on the RAP DATANeeded screen
    CCustomerTBProperties::instance()->SetProperty(TBPROP_RAPDATANEEDED_CUSTENTRY, _T("30"));
}

// SOTF 8380 -Order 2 store - RRS
bool CCustomerMHItem::IsItemOrder2StoreBarcodes(LPCTSTR str, LONG lBarcodeType)
{
	CString csTest(str);	
    
	if( lBarcodeType == 110 && csTest.Find(_T("E") ) == 0)
	{
        tbtrace(TM_INFO, _T("Scanner data is Order 2 Store"));
		return true;
	}        
    else if( csTest.Find( _T("E") ) == 0 && csTest.GetLength() == 11 )
    {
        tbtrace(TM_INFO, _T("Possible Order 2 Store Item. Do More Checking on the AM or Void Flag") );
        CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();
        CCustomerCurrentItem* pCurrItem = CCustomerCurrentItem::instance();

        CCustomerTBProperties & props = *CCustomerTBProperties::instance();
        if( pCurTrx->InAssistMode() || pCurrItem->IsVoided() || props.GetHookProperty(_T("IsResumeTrxInProgress")) == _T("1"))
        {
            tbtrace(TM_INFO, _T("Scanner data is Order 2 Store"));
            return true;
        }
    }
    
	return false;
}
// E - SOTF 8380 - O2S - RRS


TBRC CCustomerMHItem::EnterDOB(int nYear, int nMonth, int nDay, int nCustomerAge)
{
    CString csEnteredDOB;   
    //+SOTF 8136/8146
    //TCHAR buf[100];    
    //CString csYear = _ltot(nYear, buf, 10);
    
    csEnteredDOB.Format(_T("%02d%02d%02d"),nMonth,nDay,(nYear % 100));
    //-sotf8136/8146

    //SOTF 6226
    m_csCustomerDOB = csEnteredDOB;

     //SOTF 7261
    if( (m_csHighestCustomerDOB.IsEmpty()) || (m_lCustomerAge < nCustomerAge))
    {
        m_csHighestCustomerDOB = m_csCustomerDOB;
        m_lHighestCustomerAge = nCustomerAge;
    } 
    //sotf

    if((!nYear) && (!nMonth) && (!nDay))
    {
        //make the year 6 digits
        m_csCustomerDOB = _T("000000");
        m_lAgeRestrictionEntry = 2;
        m_lCustomerAge = m_lHighestAgeRequired;
    }else
    {
        
        m_lAgeRestrictionEntry = 1;
        m_lCustomerAge = nCustomerAge;
    }

    CCustomerTrxBroker *pCurTrx = CCustomerTrxBroker::instance();
    pCurTrx->TBSetProperty(_T("EnteredDateOfBirth"), csEnteredDOB, true);

    //pCurTrx->SetCurrentInProgressState(_T("ITEMIZATION_AGE"));
    //pCurTrx->TriggerMsgMap();

    return TB_SUCCESS;

}

void CCustomerMHItem::ResetDelayedAgeRequiredTLOGData(void)
{
    m_csCustomerDOB.Empty();
    m_lHighestAgeRequired = 0;
    m_lAgeRequiredItemAllowed = 0;
    m_csUPCAgeRequired.Empty();
    m_lAgeRestrictionEntry = 0;
    m_lCustomerAge =0 ;
    m_lUPCAge=0;

     //SOTF 7261
    m_csHighestCustomerDOB.Empty();
    m_lHighestCustomerAge = 0;
    //sotf

	CString csProp;
	csProp = CCustomerTrxBroker::instance()->TBGetProperty(_T("ACSDelayedAgeRestricted"), true);

	if (_ttoi(csProp))
	{

		m_bACSDelayedAgeRestricted = true;
	}
	else
	{
		m_bACSDelayedAgeRestricted = false;
	}
	//-sotf
}

void CCustomerMHItem::GetAgeVerifyData(int &ageEntry, CString *csCustomerDOB, CString *csUPC, int &ageRequired, int &itemAllowed, int &nCustomerAge)
{
    ageEntry = m_lAgeRestrictionEntry;
    csCustomerDOB->Insert(0, m_csCustomerDOB);
    csUPC->Insert(0, m_csUPCAgeRequired);
    ageRequired = m_lUPCAge;
    itemAllowed = m_lAgeRequiredItemAllowed;
    nCustomerAge = m_lCustomerAge;

}

// +SOTF5803
bool CCustomerMHItem::IsItemWithNoItemDetailsExpected(CString sCode)
{
    // left trim any leading 0s
    while( sCode.GetLength() && sCode.Left(1) == _T("0") )
    {
        sCode.Delete(0,1);
    }
    
    // POS15676: Make sure the itemcode has the correct length before determining if it is a NSC4-no-detail item.
    // NSC4 items have a format of 4yzxxxxxxxxk (where y is the NSC4 format code, z is the account type, x is the
    // 8-digit account number, k is the check-digit, and a total length of 12.)
    if( sCode.GetLength() > 11 )
    {
        CString sPrefixList = CCustomerMHOptions::instance()->OptionValue(_T("NSC4NoItemDetailsReturnToScanning"));
        tbtrace(TM_INFO, _T("Item: %s; NSC4 prefixes that have no item details: %s"), sCode, sPrefixList); // POS15676
        CStringArray sList;
        if( SplitString(sPrefixList, _T(";"), &sList) )
        {
            for(int i = 0; i < sList.GetSize(); i++)
            {
                // starts with one of the listed prefixes
                if( sCode.Find(sList[i]) == 0 )
                {
                    return true;
                }
            }
        }
    }
    return false;
}
// -SOTF5803

//(+) POS149568
void CCustomerMHItem::RemovePrevWICTotal(long lEntryID)
{
	tbtraceIO( _T("CCustomerMHItem::RemoveWICTotal"));

	PS_UI_CMD psLocalUICmd = new S_UI_CMD;
    memset(psLocalUICmd, 0, sizeof(S_UI_CMD)); // to be safe
	psLocalUICmd->InitUnionField();

    psLocalUICmd->eStructType = UI_STR_RECEIPT;

    // Set the entry ID 
    CString csEntryID;
    csEntryID.Format(_T("%d"), lEntryID); 

    psLocalUICmd->Rcpt.SetItemID(csEntryID);

    // Set the receipt name
    psLocalUICmd->Rcpt.SetRcptName(_T("CMSMReceipt"));

    // set UI command details
    psLocalUICmd->Rcpt.eItemCmd = UI_ITEM_CMD_REMOVEITEM;
    CCustomerTBScotMsg::instance()->SendScotMsg(TB_DEV_UICOMMAND, 0, NULL, (LPARAM) psLocalUICmd);
}
//(-) POS149568


bool CCustomerMHItem::IsItemPicklistLightWeightItem(LPCTSTR str)
{

	CString csTest(str);
	
	long lSize = m_VecPicklistLightWeightList.size();
	CString csTemp;

	for(long i=0; i<lSize; i++)
	{
		csTemp = m_VecPicklistLightWeightList[i];
		if( csTest.Find(csTemp) != -1)
		{
			return true;
		}

        
    }
	return false;
}

//Start CRD 223409
bool CCustomerMHItem::msgMobileGenericDelayed()
{
	CBaseMessages *pMessages = CBaseMessages::instance();
	CCustomerTBProperties *pTBProperties = CCustomerTBProperties::instance();
	
	CString csInterventionInstruction = pTBProperties->GetProperty(_T("MobileAssistanceInterventionTitle"));
	
	CString csDataNeededCommand;
	csDataNeededCommand.Format(_T("MESSAGESCREENTITLE=%s;MESSAGESCREENTEXT=%s;MESSAGESCREENITEMNEEDCANCEL=0;MESSAGESCREENGOBACKSCANANDBAG=0;LaneButtonText=%s;DefaultCmdString=%s;TopCaption=%s;SummaryInstruction=%s;StoreLogon=1;HideTotal=1;ID=TB001;InterventionInstruction=%s;ReportCategoryName=%s;CONTEXTTODISPLAY=SmDataEntrySmall;HandlingContext=SMDataEntryGoToLaneOrRap"),
		_T("$ScotApp_9516$"),						/*MESSAGESCREENTITLE*/
		_T("$ScotApp_9516$"),                                /*MESSAGESCREENTEXT*/
		csInterventionInstruction,                    /*LaneButtonText*/
		_T("CmdBtn_HandleMobileGenericDelayedIntervention"),		/*DefaultCmdString*/
		_T("$ScotApp_9516$"),                               /*TopCaption*/
		_T("$ScotApp_9514$"),				/*SumaryInstruction*/  
		csInterventionInstruction,                                /*InterventionInstruction */
		_T("Mobile Assistance Needed"));				/*ReportCategoryName*/
			 
	SDataNeededView MyDataNeeded={0};
			 
	
	MyDataNeeded.szDescription= _T("");
	MyDataNeeded.szView = csDataNeededCommand.GetBuffer(0);
	CCustomerTBConnectFL::instance()->SetDataNeededView( &MyDataNeeded);
			 
	//set up the string to display to the customer while on the RAP DATANeeded screen
	pTBProperties->SetProperty(TBPROP_RAPDATANEEDED_CUSTENTRY, _T("30"));
			 
	CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_GENERIC_DELAYED, NULL, NULL);
	
	return true;
}
//End CRD 223409

bool CCustomerMHItem::msgEASItem()
{
	CCustomerTBProperties & props = *CCustomerTBProperties::instance();
	CString csEASEntryID = props.GetHookProperty(_T("tb_long"));

	if(isFLMSTransferring)
	{
		tbtrace(TM_INFO, _T("is an EAS item and mobile - EntryID - %s = %d"), csEASEntryID, m_lEntryIDtemp);
		if(csEASEntryID == _T(""))	//in case entry id is blank from the POS. get it from item details
			csEASEntryID.Format(_T("%d"), m_lEntryIDtemp);

		CSolutionTBConnectPOS::csEASEntryIDList.Add(csEASEntryID);
	}
	else		//if EAS item is sold in a regular mode, send TB_GENERIC_DELAYED intervention
	{
		
		props.SetHookProperty(_T("IsAnEASItem"), _T("1"));
		tbtrace(TM_INFO, _T("is an EAS item and not mobile - EntryID - %s"), csEASEntryID);
		
		CString csInterventionInstruction;
		csInterventionInstruction = props.GetProperty(_T("EASInterventionTitle"));
		
		CString csDataNeededCommand;
		csDataNeededCommand.Format(_T("MESSAGESCREENTITLE=%s;MESSAGESCREENTEXT=%s;MESSAGESCREENITEMNEEDCANCEL=0;MESSAGESCREENGOBACKSCANANDBAG=0;LaneButtonText=%s;DefaultCmdString=%s;TopCaption=%s;SummaryInstruction=%s;StoreLogon=1;HideTotal=1;ID=TB002;InterventionInstruction=%s;ReportCategoryName=%s;HandlingContext=SMDataEntryGoToLane"),
			_T(""),						/*MESSAGESCREENTITLE*/
			_T("$ScotApp_9512$"),                                /*MESSAGESCREENTEXT*/
			csInterventionInstruction,                    /*LaneButtonText*/
			_T("CmdBtn_HandleEASIntervention"),		/*DefaultCmdString*/
			_T("$ScotApp_9512$"),                               /*TopCaption*/
			_T("$ScotApp_9514$"),				/*SumaryInstruction*/  
			csInterventionInstruction,                                /*InterventionInstruction */
			_T("EAS Item"));				/*ReportCategoryName*/
		
		SDataNeededView MyDataNeeded={0};
		
		tbtrace(TM_INFO, _T("msgEASItem() - %s"), csDataNeededCommand);
		
		//MyDataNeeded.szDescription= csEASItem.GetBuffer(0);
		MyDataNeeded.szView = csDataNeededCommand.GetBuffer(0);
		CCustomerTBConnectFL::instance()->SetDataNeededView( &MyDataNeeded);
		
		//set up the string to display to the customer while on the RAP DATANeeded screen
		props.SetProperty(TBPROP_RAPDATANEEDED_CUSTENTRY, _T("30"));
		
		CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_GENERIC_DELAYED, NULL, NULL);		
	}
	
	return true;
}
//End CRD 185620
//Start CRD 328873 - function to append barcode prefix, code originally from HandleItemScan() function
CString CCustomerMHItem::ModifyBarcode(CString csItemCode, long lScanCodeType)
{
	tbtrace(TM_INFO, _T("ModifyBarcode(%s; %d)"), csItemCode, lScanCodeType);

	CCustomerCurrentItem *pCurItem= CCustomerCurrentItem::instance();
	CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();
    CCustomerTBProperties &props = *CCustomerTBProperties::instance();

	//the following if condition is the one sending the whole barcode data to POS including the symbologies
	//this is applicable to all scanner data supposedly but to minimize the impact for the existing implementation,
	//it is just applied for now to all barcode with alpha char at the bigenning. This should be applied to solution level later
	CString itemCode = csItemCode;		

	//if(!iswalpha(itemCode.GetAt(0))) //check first char
	//{
	CString csSymbology(_T(""));
	CScannerMsg scan;
	//scan.BarcodeType((CScannerMsg::BarcodeTypes)csScanCodeType));

	tbtrace(TM_INFO, _T("scan barcode type %d"), lScanCodeType);

	if( lScanCodeType == 108) //108 code 39
		csSymbology = _T("B1");
	else if( lScanCodeType== 110) //110 code 128
		csSymbology = _T("B3");
	else if( lScanCodeType == 104 || lScanCodeType == 119) //104, 119 code EAN13
		csSymbology = _T("F");
	else if( lScanCodeType == 103) //103 code EAN8
		csSymbology = _T("FF");
	else if( lScanCodeType == 101 || lScanCodeType == 111) //101, 111 code UPC-A
		csSymbology = _T("A");
	else if( lScanCodeType == 102 || lScanCodeType == 112) //102, 112 code UPC-E
		csSymbology = _T("E");
	else if ( lScanCodeType == 106)   //106: I_2/5 barcode type: photo package item
	{
		csSymbology = _T("B2");
	}
    else if( lScanCodeType == 131 || lScanCodeType == 132 ) //131, 132 RSS-14 or RSS-Expanded (a.k.a Databar-14 or Databar-Expanded)
    {
		csSymbology = _T("]e0");
    }
    else if( lScanCodeType == 201 )   //POS203228 - rrs
	{
        //sample barcode for Michigan DL
        // itemCode =  _T("@\n\x001E\rANSI 636032030002DL00410205ZM02730027DLDCA\nDCB\nDCD\nDBA02012016\nDCSSAMPLE\nDCTJANICE\nDBD07012012\nDBB02011977\nDBC2\nDAY\nDAU\nDAG123 NORTH STATE ST.\nDAILANSING\nDAJMI\nDAK489181234  \rDAQS 100 100 100 100\nDCF\nDCG\nDCH\nDAH\nDCKS100100100100197702011102\n\rZMZMARev 01-21-2011\nZMB03\n\r");

		//sample barcode for Kentucky DL
		//itemCode = _T(" @\n\x001E\rANSI 636046050002DL00410528ZK05690059DLDAQS00000101XY1XY1XY1XY1XY1X\nDCSSAMPLE XY1XY1XY1XY1XY1XY1XY1XY1XY1XY1XY1\nDDEU\nDACMATEOXY1XY1XY1XY1XY1XY1XY1XY1XY1XY1XY1XY\nDDFU\nDADMIDDLENAME XY1XY1XY1XY1XY1XY1XY1XY1XY1XY\nDDGU\nDCAABCDY1\nDCB12345FIJKXY1\nDCDHTPNX\nDBD07122010\nDBB12121992\nDBA12122014\nDBC1\nDAU073 IN\nDAYBRO\nDAG100 MAIN STREET XY1XY1XY1XY1XY1XY1X\nDAIFRANKFORT XY1XY1XY1X\nDAJKY\nDAK406011234XY\nDCFS00000001DLYYDDD011XY1XY1\nDCGUSA\nDCUSUFIX\nDAHAPT. 1 XY1XY1XY1XY1XY1XY1XY1XY1XY1X\nDCKS00000101121219921XY1XY1X\nDDAN\nDDBMMDDCCYY\nDDCMMDDCCYY\nDDD1\nDDH12122010\nDDJ12122013\nZKZKAORGAN DONOR\nZKBCOUNTYXY1XY1XY1XY1XY1XY1XY1XY\nZKC0900\n\r");
        
		itemCode.TrimLeft(); //bug274193 - KY barcode started with space so lets remove it

		CString csmPerksPrefix;
        csmPerksPrefix = CCustomerMHOptions::instance()->AppIniOptionValue (_T("MPERKS_FROM_BARCODE"), _T("Prefix"));
        if( pCurItem->ItemCode().Left(5).Find(csmPerksPrefix) != -1)
        {
		    CString csItemCode = _T("9") + pCurItem->ItemCode();
		    pCurItem->ItemCode(csItemCode);
            itemCode = csItemCode;
            tbtrace(TM_INFO, _T("TB decided a 9 needs to be added to front of 2D mPerks barcode"));
        }
        else if(itemCode.GetAt(0) == '@') //Task25163 DL 2D barcode - aab
        {    
			//the IL barcode has a double backlash but somehow it will become one along the way, so restore it here.
			int index = itemCode.Find(_T("\\"));
			if(index != -1)
			{
				//92 = \. Insert only if its only 1 backslash.
               if(itemCode.GetAt(index + 1) != 92) 
			      itemCode.Insert(index, _T("\\"));
			}
				
			//TFS266902 - aab
			//remove not only 1e but all non-printable control character which is from 0x00 to 0x1F or (0 to 31 dec)            
		    tbtrace(TM_INFO, _T("It's a 2D DL barcode. Removing non-printable characters."));
			for(TCHAR i=0 ; i<=31; i++)
            {
                //tbtrace(TM_INFO, _T("csHex=%04x"), i);
				
				if(i != 13 && i != 10) //preserve the carriage return and line feed.
                   itemCode.Replace(i, _T(' '));
            }
		}
		else if(itemCode.GetAt(0) == 'H') //VSTS 149725
        {    
            	csSymbology = _T("]L0");
		}
		else
			csSymbology = _T("]L2"); //VSTS-222611
	}
    //e - POS203228 - rrs

    // POS 11013 - the SMState::MeijerModifyScannedBarcode logic to append the symbology would be removed, 
    // so this logic is not needed anymore as it always append the symbology to the barcode data - rrs
    /*if ( csSymbology.GetLength() > 0 )
    {
        // Don't re-add symbology code if it is already there!!
        // This is a work-around.  An SMState Override is also adding symbology!!!!
        // POS 10851 - For Code 128 symbology (B3), make sure it's alwasy appended
        CString csTest = itemCode.Left(csSymbology.GetLength());
        if ( csSymbology == csTest && csSymbology != _T("B3") )
        {
            csSymbology = "";
        }
    }*/
    // E - POS 11013 - the SMState::MeijerModifyScannedBarcode logic to append the symbology would be removed, 
    // so this logic is not needed anymore as it always append the symbology to the barcode data - rrs
    CString csScanCode = csSymbology + itemCode;

	return csScanCode;
}
//End CRD 328873 