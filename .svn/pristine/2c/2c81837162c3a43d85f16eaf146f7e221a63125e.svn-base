//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMRebateItem.cpp
//
// TITLE: Class implementation for Tagging Items for Rebate Receipt
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h> 				// MS MFC always needed first
#include <afxtempl.h>				// MS MFC template header

#include "Common.h" 				// Common includes

#include "SMState.h"				// Base state
#include "SMRebateItem.h"
#include "PSIntList.h"
#include "TransBrokerSSF.h"

#define COMP_ID ID_SM				// Base state component
#define T_ID	_T("RebateItem")

IMPLEMENT_DYNCREATE(SMRebateItem, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMRebateItem::SMRebateItem()
: csCurContext(EMPTY_STRING)
{
	IMPLEMENT_TIMESTAMP
	saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
}

////////////////////////////////////
SMStateBase *SMRebateItem::Initialize(void)
{
	CString csCurrentContext;

	COleVariant v;
	SMStateBase *sReturnState = CkStateStackAndReturn();
	if (sReturnState != STATE_NULL)
		return sReturnState;

	ps.SetCMFrame(_T("RebateItem")); 
  
	CString csItemEntryId = ps.GetSelection(_T("CMReceipt"));

	if (csItemEntryId.GetLength()) 
	{
		ps.SetSelection(_T("CMReceiptVoid"), csItemEntryId);
		ps.CMButton(_T("CMReceiptScrollDown"), BTT_DOWN, ps.CheckForScrolling(_T("CMSMReceipt"), _T("CMReceiptVoid"), _T("Down")));
		ps.CMButton(_T("CMReceiptScrollUp"),   BTT_UP,   ps.CheckForScrolling(_T("CMSMReceipt"), _T("CMReceiptVoid"), _T("Up")));
		// Making sure POS and FL are synchronize
		SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(_T("CMReceiptVoid")));
	}
	else
	{
		// Disable scrolling button if no selection is found
		ps.CMButton(_T("CMReceiptScrollDown"), BTT_DOWN, false);
		ps.CMButton(_T("CMReceiptScrollUp"),   BTT_UP,   false);
	}
	ps.SetCMGobackButtonVisible(true);
   
	DMSayPhrase(_T("PrintRebateReceipt.wma"));
	ps.ShowCMTotal(true, false);
	ps.ShowCMTBText(csTBMessage2Scot);
 
	SAWLDBScannerDisable();	  // disable the scanner
   
	ps.ShowCMFrame();
    
	delete getRealStatePointer(sReturnState);  
	return STATE_NULL;
}

///////////////////////////////////
void SMRebateItem::UnInitialize(void)
{
	SAWLDBScannerDisable();		  // disable the scanner
	ps.HideButtons();
}

/*
---------------------------------------------------------------------------------------------------
 Tag the selected item for Rebate Receipt.
---------------------------------------------------------------------------------------------------
*/
SMStateBase *SMRebateItem::PSButton1(void)
{	
	trace(L6, _T("+SMRebateItem::PSButton1 (Toggle Rebate Item)"));

	CString csItemEntryId  = ps.GetSelection(_T("CMReceiptVoid"));
	SendTBCommand(TBCMD_SETSELECTEDITEM, csItemEntryId);  

	if( csItemEntryId.GetLength() )
	{
		SendTBCommand(TBCMD_SET_TB_STATE,_T("TAG_REBATEITEM"));
	}
	return STATE_NULL;
}

SMStateBase *SMRebateItem::PSButton5(void)
{	
	trace(L6, _T("+SMRebateItem::PSButton5 (OK)"));

    int iRebateItemCount = 0;
    int iRebateRcptCount = 0;
    SendTBCommand(TBCMD_GET_REBATE_RECEIPT_COUNTS, &iRebateItemCount, &iRebateRcptCount);

    trace(L6, _T("+SMRebateItem::PSButton5() - There are %d rebate receipts to be printed. Show Rebate Receipt screen."), iRebateRcptCount);
    if( iRebateRcptCount > 0 )
    {
        // at least one item has been tagged - play audio
	    SendTBCommand(TBCMD_SET_TB_STATE,_T("REBATEITEM_MSG"));
    }
    else
    {
        // no items were tagged, return to scan and bag and don't play rebate reminder
        SendTBCommand(TBCMD_SET_TB_STATE,_T("REBATEITEM_NOMSG"));
    }
    return setAndCreateAnchorState(BES_SCANANDBAG);
}

///////////////////////////////////
SMStateBase *SMRebateItem::PSButtonGoBack(void) 
{
    trace(L6, _T("+SMRebateItem::PSButtonGoBack() -- redirecting to PSButton5 (OK)"));
	return PSButton5();
}


/////////////////////////////////////////////
SMStateBase *SMRebateItem::PSReceiptDown(void)
{ 
	ps.ScrollDown(_T("CMReceiptVoid"));
	ps.CMButton(_T("CMReceiptScrollDown"), BTT_DOWN, ps.CheckForScrolling(_T("CMSMReceipt"), _T("CMReceiptVoid"), _T("Down")));
	ps.CMButton(_T("CMReceiptScrollUp"), BTT_UP, ps.CheckForScrolling(_T("CMSMReceipt"), _T("CMReceiptVoid"), _T("Up")));    
	SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(_T("CMReceiptVoid")));  
	return STATE_NULL;
}

/////////////////////////////////////////////
SMStateBase *SMRebateItem::PSReceiptUp(void)
{
	ps.ScrollUp(_T("CMReceiptVoid"));
	ps.CMButton(_T("CMReceiptScrollDown"), BTT_DOWN, ps.CheckForScrolling(_T("CMSMReceipt"), _T("CMReceiptVoid"), _T("Down")));
	ps.CMButton(_T("CMReceiptScrollUp"), BTT_UP, ps.CheckForScrolling(_T("CMSMReceipt"), _T("CMReceiptVoid"), _T("Up")));    
	SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(_T("CMReceiptVoid")));
	return STATE_NULL;
}

/*
---------------------------------------------------------------------------------------------------
 The base implementation always puts us back to Bagging state, which we do not want.
---------------------------------------------------------------------------------------------------
*/
SMStateBase *SMRebateItem::TBItemDetails(void)
{
    //SMStateBase::TBItemDetails();
	return STATE_NULL; 
}