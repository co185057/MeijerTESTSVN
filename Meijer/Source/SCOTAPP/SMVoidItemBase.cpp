//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMVoidItemBase.cpp
//
// TITLE: Class implementation for void item state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include "SMVoidItem.h"  
//               // MS MFC template header

#ifndef _CPPUNIT
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state       
#include "SMConfirmVoid.h"          // Confirm void state
#include "SMConfirmAbort.h"         // Confirm abort state
#include "PSIntList.h"
#include "SMVoidNoMatch.h"
#include "SMSmAssistMenu.h"
#include "SMSmRestrictedItems.h"
//USSF Start
#include "SMUSSFManager.h"
#include "USSFCommon.h"
//USSF End
#include "SMProduceFavorites.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("VoidItemBase")

#endif //_CPPUNIT 

IMPLEMENT_DYNCREATE(SMVoidItemBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(VoidItem)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMVoidItemBase::SMVoidItemBase()
: csCurContext(EMPTY_STRING), m_sCurrentReceipt(EMPTY_STRING), m_sCurrentReceiptSrc(EMPTY_STRING), m_bIsFromMultiSelectPickList(false)
{
   #ifndef _CPPUNIT
   IMPLEMENT_TIMESTAMP
   saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
   #endif 

}
#ifndef _CPPUNIT
////////////////////////////////////
SMStateBase *SMVoidItemBase::Initialize(void)
{
  CString csCurrentContext;

   COleVariant v;
   PSTEXT Btn1 = PS_NOTEXT; // TAR 342889 added use of Btn1
   PSTEXT Btn2 = PS_NOTEXT;

   TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("VoidItem") );	// RFC 330050
   SMStateBase *sReturnState = CkStateStackAndReturn();
   if (sReturnState != STATE_NULL)
      return sReturnState;
   //Update RAP window with SCOT state
   UpdateStateName(ps.GetPSText(LTT_CANCELITEMS, SCOT_LANGUAGE_PRIMARY));
   //+SSCOADK-6701
   // We want the lane to return to the original anchor state if the attendant goes back to approval
   // screen during remote approval, not the SmVoidItem state.
   if (!bForceApprovalBeforeSuspend)
   {
      setAnchorState(BES_VOIDITEM);
   }
   //-SSCOADK-6701
   //TAR228754 Set this flag to be true to indicate we're in cancel item screens. 
   bIsFromVoidApproval = true;

   if (fInMultiSelectPickList )
   {	
		m_sCurrentReceiptSrc = _T("PickListReceipt");
		m_sCurrentReceipt = _T("PickListReceiptVoid");
   }
   else
   {
	    m_sCurrentReceiptSrc =  _T("CMSMReceipt") ;
	    m_sCurrentReceipt = _T("CMReceiptVoid") ;
   }

   if (co.fCMRewardInfo && !co.IsInAssistMenus())
   {
	   if (fInMultiSelectPickList )
			csCurrentContext = _T("VoidPickListItemWithReward");
	   else
			csCurrentContext = _T("VoidItemWithReward"); 
   }
   else
   {
	   if (fInMultiSelectPickList )
			csCurrentContext = _T("VoidPickListItem");
	   else
			csCurrentContext = _T("VoidItem");
   }

   ps.SetCMFrame(csCurrentContext); 
   CString csItemEntryId;

   if (co.IsInAssistMenus())
   {
       Btn1 = BTT_VOIDITEM;
	   Btn2 = (co.m_bOperationsAllowCancelAll? BTT_VOIDALL:PS_NOTEXT);
       csItemEntryId = ps.GetSelection(_T("SMReceipt"));
       if (csItemEntryId.IsEmpty())
       {
           csItemEntryId = ps.GetSelection(m_sCurrentReceipt);
       }
   }
   else
   {
       ps.SetCMMascotImage(BMP_MASCOT_CANCELITEM);
       ps.SetCMLeadthruText(LTT_CANCELITEMS);
       Btn1 = BTT_CANCELSELECT;
	 
	   if ( !fInMultiSelectPickList )
			Btn2 = (co.m_bOperationsAllowCancelAll? BTT_CANCELALL:PS_NOTEXT); //TAR 102011
	
	   csItemEntryId = ps.GetSelection(m_sCurrentReceipt);
	   
   } 
   // TAR 342889 start - see if any selectable items
   CString csTemp;
   int iNumItems = ps.ScanReceiptToVoid(m_sCurrentReceiptSrc, csTemp, 0);
  
   if (0 == iNumItems)
   {
     csItemEntryId = _T(""); // No selectable items
   }
   else
   {
     v.Clear();
     long rc = ps.GetPSXObject()->GetReceiptItemVariable(m_sCurrentReceiptSrc, csItemEntryId, _T("ITEM_SELECTABLE"), v);
     if (VARIANT_FALSE == v.boolVal)
     {
       PSReceiptUp(); // Current item not selectable; scroll up
       rc = ps.GetPSXObject()->GetSelectedReceiptItem(m_sCurrentReceipt, csItemEntryId);
       rc = ps.GetPSXObject()->GetReceiptItemVariable(m_sCurrentReceiptSrc, csItemEntryId, _T("ITEM_SELECTABLE"), v);
       if (VARIANT_FALSE == v.boolVal)
       {
         PSReceiptDown(); // First item not selectable; scroll down
         rc = ps.GetPSXObject()->GetSelectedReceiptItem(m_sCurrentReceipt, csItemEntryId);
       }
     }
     v.Clear();
   }
   // TAR 342889 end
   

//USSF Start
   USSFDATA(_T("CURRENTITEM"), csItemEntryId);
//USSF End

   if ( !fInMultiSelectPickList )
	   ps.SetButtonText(_T("CMButton2"), Btn2);
   ps.SetCMGobackButtonVisible(true);
   
   //SSCOADK-4184+
   /*if(co.IsInAssistMenus() && !fInMultiSelectPickList && lUnapprovedRestrictedItems > 0 )
   {
        ps.ButtonState(_T("ButtonGoBack"), false, false);
   }
   else*/
   //SSCOADK-4184-
   {
        ps.ButtonState(_T("ButtonGoBack"), true, true);
   }

   DMSayPhrase(CANCELITEMS);
   ps.ShowCMTotal(true, false);
   ps.ShowCMTBText(csTBMessage2Scot);	//thp
 
   if (csItemEntryId.GetLength())
   {
	 ps.SetSelection(m_sCurrentReceipt, csItemEntryId);
	 //+ tar414311
 	 if( fInMultiSelectPickList )
	 {
 		v = ps.GetPSText(MSG_SELECTITEMONRECEIPT);
	 }
	 else
	 {
		 v = ps.ConvertToEscapeSequence(ps.GetPSText(MSG_CANCELSELECT), _T("\\n"), _T("\n"));
	 }
	 //- tar414311
	 m_pAppPSX->SetConfigProperty(_T("ScanBagTextArea"), csCurrentContext, UI::PROPERTYTEXTFORMAT, v);
	 ps.CMButton(_T("CMReceiptScrollDown"), BTT_DOWN, ps.CheckForScrolling(m_sCurrentReceiptSrc, m_sCurrentReceipt, _T("Down")));
     ps.CMButton(_T("CMReceiptScrollUp"), BTT_UP, ps.CheckForScrolling(m_sCurrentReceiptSrc, m_sCurrentReceipt, _T("Up")));

        //! Check to see if we are in assismode so we can disable the scanner
    if (!ps.RemoteMode())
    {  //Don't enable local scanner for remote mode
		if (!bEASReEnableScanner)
        {
            trace(L2, _T("::Initialize - bEASReEnableScanner is set to true explicitly"));
            bEASReEnableScanner = true;
        }		

        SAWLDBScannerEnable();
    }
    else
    {
        SAWLDBScannerDisable();
    }	 
   }
   else
   {  
	 // only choice is to cancel all items because these are
	 // non-plu items.
	 if ( fInMultiSelectPickList )
		 v = ps.GetPSText(MSG_SELECT_GOBACK);
	 else
		 v = ps.ConvertToEscapeSequence(ps.GetPSText(MSG_CANCELALLITEMS), _T("\\n"), _T("\n"));
	 m_pAppPSX->SetConfigProperty(_T("ScanBagTextArea"), csCurrentContext, UI::PROPERTYTEXTFORMAT, v);
	 ps.CMButton(_T("CMReceiptScrollDown"), BTT_DOWN, false); 
	 ps.CMButton(_T("CMReceiptScrollUp"), BTT_UP, false); 
	 SAWLDBScannerDisable();	  // disable the scanner
   }
   if (fInMultiSelectPickList)
   {
  	SAWLDBScannerDisable(); // Disable the scanner if in  multiPicklist//
   }

   ps.ShowCMFrame();
//*** end of NewUI ***//
   DisplayAuxScreen(AUX_SCREEN_USE_TOUCHSCREEN, true);	// TAR 226994

   lVoidQty = 0;
   lVoidWgt = 0;
   lVoidTcd = 0;
   
  // + TAR 406041	
   lVoidPrc = 0;					  
   lVoidUPrc = 0;					      
   lVoidBarcodeType = 0;
  // - TAR 406041	

   return STATE_NULL;							  // stay in attract state
}
#endif //_CPPUNIT

#ifndef _CPPUNIT
///////////////////////////////////
void SMVoidItemBase::UnInitialize(void)
{
  // BeginCodeWLDBSA11
  SAWLDBScannerDisable();		  // disable the scanner
  ps.HideButtons();
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("VoidItem") );	// RFC 330050
  // EndCodeWLDBSA11
}
#endif //_CPPUNIT

#ifndef _CPPUNIT
///////////////////////////////////
SMStateBase *SMVoidItemBase::PSButton1(void)// void selected item
{
//*** NewUI ***//
  CString csDesc,csCode;
  long lQty,lWgt,lPrc,lUPrc,lTcd, lBarcodeType;
  if (ps.GetItem(csDesc,csCode,lQty,lWgt,lPrc,lUPrc,lTcd,lBarcodeType,m_sCurrentReceiptSrc, m_sCurrentReceipt)>0)// get selected item info MGG3
  {
    csItemSent = csCode;					  // save the item code
    csDesc     = csDesc;					  // save the item desc
    lVoidQty   = lQty;						  // save the qty
    lVoidWgt   = lWgt;						  // save the weight
    lVoidTcd   = lTcd;						  // save the tare for later void
    lVoidPrc	 = lPrc;					  // save the price for later void
	lVoidUPrc  = lUPrc;					      // save the unit price for later void
	lVoidBarcodeType = lBarcodeType;
  }
//*** end of NewUI ***//
  m_bIsVoidByScanning = false;
   //sscop-3345+
  /*
  if ( fInMultiSelectPickList )
  {
	  //Cancel the wt check for previous item.
	CKeyValueParmList parmList;
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_SKIP_BAGGING);
	_bstr_t bstrParmList((const _TCHAR *)parmList);
	BSTR bstrResult;
	bstrResult = NULL;
	SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
	SysFreeString(bstrResult);
	nSAPrevSCOTInputID = 0;
	g_lNumberOfItemsNotBagInMultiPick--;

	return TBItemSale(true,lVoidTcd,m_bIsVoidByScanning,true);
  }
 */
  //sscop-3345-
  RETURNSTATE(ConfirmVoid);
}
#endif //_CPPUNIT
#ifndef _CPPUNIT

///////////////////////////////////
SMStateBase *SMVoidItemBase::PSButton2(void) // Cancel all items Dimple Button
{
  RETURNSTATE(ConfirmAbort)
}

#endif //_CPPUNIT
#ifndef _CPPUNIT
///////////////////////////////////
SMStateBase *SMVoidItemBase::PSButton3(void) // backup dimple button
{
  //TAR228754 Reset this flag to indicate that we get out Cancel item screens
  bIsFromVoidApproval = false;
  //TAR228754

  return createAnchorState();               // return to S&B or EnterCoupons
}
#endif //_CPPUNIT
#ifndef _CPPUNIT

///////////////////////////////////
SMStateBase *SMVoidItemBase::PSButtonGoBack(void) 
{
  //TAR228754 Reset this flag to indicate that we get out Cancel item screens
  bIsFromVoidApproval = false;
  //TAR228754
  // TK - TAR 199037

  if ( fInMultiSelectPickList )
  {
	  RETURNSTATE(ProduceFavorites);
  }
  else if(co.IsInAssistMenus())
  {
      if (isRestrictedItemApprovalRequired())
      {
          //+SSCOADK-6701
          // We want the lane to return to the ScanAndBag screen if returning to remote approval.
          if (ps.RemoteMode())
          {
              setAnchorState(BES_SCANANDBAG);
          }
          //-SSCOADK-6701
          RETURNSTATE(SmRestrictedItems)
      }
      else 
          RETURNSTATE(SmAssistMenu)
  }
  else
  {
	  // TAR 391552 (New Fix) +
	  setAnchorState(BES_SCANANDBAG);
	  SMStateBase *sReturnState = CkStateStackAndReturn();
	  if (sReturnState != STATE_NULL)
	  {
		  return sReturnState;
	  }
	  else
	  {
          SASendSCOTInput( TRANSACTION_CONTINUE );
		  return setAndCreateAnchorState(BES_SCANANDBAG);
	  }
	  // TAR 391552 (New Fix) -
	// Go back to Scan & Bag
	//return setAndCreateAnchorState(BES_SCANANDBAG);  
  }
}
#endif //_CPPUNIT
#ifndef _CPPUNIT

///////////////////////////////////
SMStateBase *SMVoidItemBase::DMScanner(void)// scanned item
{
/*
When an item is scanned, scanned barcode with and without label(or scan type)
are saved in csDMLastBarCode and csDMLastBarLabel respectively.

TB _getts szItemCode(copy of csDMLastBarLabel) and szOriginalCode(copy of csDMLastBarCode)
in TBEnterItem() call.

TB gives item sold details in which item code is sent by TB. TB can send App either the 
original item code or szItemcode without barcode type.

When the same item is scanned, application looks at the current item or most recent items for a match.
Application ignores whether the item is scanned or manually entered when it does matching.
If it finds a match, extracts the matching item details from the receipt list and sends them to TB for 
an item void in this screen.

TB can use quantity or weight information passed or can prompt for those details from App.
As of now, UITS doesn't specify whether app should pass item sold details for a quantity or
weight item since those items can be sold with multiple quatities or weights in the same 
transaction.

If UITS specified not to send saved details, one can try by commenting the below if (found) block. 
*/
  
//*** NewUI ***//
  bool found = false;
  CString csDesc,csCode;
  long lQty,lWgt,lPrc,lUPrc, lTcd, lBarcodeType;
 
  if (fInMultiSelectPickList )
 	{	
		csItemSent = csDMLastBarCode;
		ps.SetFoundPickListItemsFromSearch(false);
		return TBItemSale(false,0,true);
	}

  if (ps.GetItem(csDesc,csCode,lQty,lWgt,lPrc,lUPrc,lTcd, lBarcodeType, _T("CMSMReceipt"), _T("CMReceiptVoid"))>0)// get selected item info
  {
    //SSCOADK-1080 Removed condition  (csDMLastBarCode.Find(csCode) != -1)
	if ((csCode == csDMLastBarCode) || (csCode == csDMLastBarLabel)) //RFC 412256
    {
      found = true;
    }
  }
  
  if (!found)
  {
	CString csItemEntryId;
	int nCountList = ps.ScanReceiptToVoid(_T("CMSMReceipt"), csItemEntryId);
	for (int i=nCountList-1; i>=0; i--)
	{
		ps.ScanReceiptToVoid(_T("CMSMReceipt"), csItemEntryId, i);
		if (ps.GetItem(csDesc,csCode,lQty,lWgt,lPrc,lUPrc,lTcd, lBarcodeType, _T("CMSMReceipt"), _T("CMReceiptVoid"), csItemEntryId)>0)// get selected item info
		{
			if ((csCode == csDMLastBarCode) || (csCode == csDMLastBarLabel))
			{
			  found = true;
			  ps.SetSelection(_T("CMReceiptVoid"), csItemEntryId);
			  break;
			}
		}
	}
  }
  //*** end of NewUI ***//
  m_bIsVoidByScanning = true;
  // TAR208643 always send TB the Voided Item's information LPM070802
  csItemSent = csDMLastBarCode;
  if (found) // save selected item info for TB
  {
    csDesc     = csDesc;		  // save the item desc
    lVoidQty   = lQty;			  // save the qty
    lVoidWgt   = lWgt;			  // save the weight
    lVoidTcd   = lTcd;			  // save the tare for later void
    lVoidPrc	 = lPrc;		  // save the price for later void
	lVoidUPrc  = lUPrc;			  // save the unit price for later void
	lVoidBarcodeType = lBarcodeType;
	RETURNSTATE(ConfirmVoid)  //*** New UI ***//
  }

  if (m_GenericDelayedInterventionStatus == GENERIC_DELAYED_COMPLETING_APPROVALS)
  {
      m_GenericDelayedInterventionStatus = GENERIC_DELAYED_COMPLETE;
  }

  // TAR 305086:  ian-F143<voided a non scanned item>
  // TB should return TB_VOIDNOTMATCH if the scanned item is not found on the
  // receipt.  This would result in a transition to SMVoidNoMatch.
  // This does not occur with the TB emulator unless item 02310001277 (Cat Food) is 
  // scanned.

  return TBItemSale(true,0,true); 
}
#endif //_CPPUNIT

#ifndef _CPPUNIT
////////////////////////////////////////
SMStateBase  *SMVoidItemBase::PSCancelKey(void)/// override where needed
{
  DMSayPhrase(KEYPADVALID);
  return PSButtonGoBack();               // usually same as button 1
}
#endif //_CPPUNIT

//#ifndef _CPPUNIT

/////////////////////////////////////////////
SMStateBase *SMVoidItemBase::PSReceiptDown(void)
{
  // TAR 342889 start
  CString csItemEntryId;
  COleVariant vValue;
  vValue.Clear();
  vValue.boolVal = VARIANT_FALSE;

  // NextGen //
  if (0 != nPSLastParm)
  {
	  CString csItemId;
	  csItemId.Format(_T("%d"), nPSLastParm);
	  ps.GetPSXObject()->SetSelectedReceiptItem(m_sCurrentReceipt, csItemId);
	  return STATE_NULL;
  }
 
  while (VARIANT_FALSE == vValue.boolVal)
  {
	ps.ScrollDown(m_sCurrentReceipt); // Pre-342889 code
	vValue.Clear();
	CString csTemp = csItemEntryId;
	long rc = ps.GetPSXObject()->GetSelectedReceiptItem(m_sCurrentReceipt, csItemEntryId);
	rc = ps.GetPSXObject()->GetReceiptItemVariable(m_sCurrentReceiptSrc, csItemEntryId, _T("ITEM_SELECTABLE"), vValue);
	if (csTemp == csItemEntryId)
	{
	  vValue.boolVal = VARIANT_TRUE; // Force exit from loop
	}
  }
  // TAR 342889 end
  ps.CMButton(_T("CMReceiptScrollDown"), BTT_DOWN, ps.CheckForScrolling(m_sCurrentReceiptSrc, m_sCurrentReceipt, _T("Down")));
  ps.CMButton(_T("CMReceiptScrollUp"), BTT_UP, ps.CheckForScrolling(m_sCurrentReceiptSrc, m_sCurrentReceipt, _T("Up")));    

  return STATE_NULL;
}
//#endif //_CPPUNIT
#ifndef _CPPUNIT

/////////////////////////////////////////////
SMStateBase *SMVoidItemBase::PSReceiptUp(void)
{
  // TAR 342889 start
  CString csItemEntryId;
  COleVariant vValue;
  vValue.Clear();
  vValue.boolVal = VARIANT_FALSE;
  while (VARIANT_FALSE == vValue.boolVal)
  {
	ps.ScrollUp(m_sCurrentReceipt); // Pre-342889 code
	vValue.Clear();
	CString csTemp = csItemEntryId;
	long rc = ps.GetPSXObject()->GetSelectedReceiptItem(m_sCurrentReceipt, csItemEntryId);
	rc = ps.GetPSXObject()->GetReceiptItemVariable(m_sCurrentReceiptSrc, csItemEntryId, _T("ITEM_SELECTABLE"), vValue);
	if (csTemp == csItemEntryId)
	{
	  vValue.boolVal = VARIANT_TRUE; // Force exit from loop
	}
  }
  // TAR 342889 end
  ps.CMButton(_T("CMReceiptScrollDown"), BTT_DOWN, ps.CheckForScrolling(m_sCurrentReceiptSrc, m_sCurrentReceipt, _T("Down")));
  ps.CMButton(_T("CMReceiptScrollUp"), BTT_UP, ps.CheckForScrolling(m_sCurrentReceiptSrc, m_sCurrentReceipt, _T("Up")));    

  return STATE_NULL;
}
#endif //_CPPUNIT
#ifndef _CPPUNIT


//////////////////////////////////////////////////
// CheckForScrollUp 
// returns true if you can scroll up from the given position in the receipt (lRcptPos),
// false otherwise.
//////////////////////////////////////////////////
bool SMVoidItemBase::CheckForScrollUp(long lRcptPos)
{
//*** Don't need this function with NewUI ***//

  /*CString csDesc,csCode,csCat;
  long lQuantity,lWeight,lPrice,lUPrice, lTareCode, lBarcodeType;
  bool up = false;
  while (lRcptPos > 1) //loop to see if there are any valid items above current one
  {
    if ((ps.GetCMReceipt())->GetAt(--lRcptPos, csDesc, csCode, lQuantity, lWeight, lPrice, lUPrice, lTareCode, lBarcodeType, csCat) == -1)
      break;
    if (csCode[0] != INFO_ITEM && 
        csCode[0] != VOID_ITEM &&
        csCode[0] != LINK_ITEM) //item is not voided, linked, or info (therefore it is selectable)
    {
      up = true;
      break;
    }
  }
  if (!up || lRcptPos == 0)
      return false;
  else return true;
*/
  return false;
}

#endif //_CPPUNIT

#ifndef _CPPUNIT
//////////////////////////////////////////////////
// CheckForScrollDown
// returns true if you can scroll down from the given position in the receipt (lRcptPos),
// false otherwise.
//////////////////////////////////////////////////
bool SMVoidItemBase::CheckForScrollDown(long lRcptPos)
{
//*** Don't need this function with NewUI ***//
/*
  CString csDesc,csCode,csCat;
  long lQuantity,lWeight,lPrice,lUPrice,lTareCode,lBarcodeType;
  bool down = false;
  while (lRcptPos < (ps.GetCMReceipt())->GetListCount()) //loop to see if there are any valid items below current one
  {
    if ((ps.GetCMReceipt())->GetAt(++lRcptPos, csDesc, csCode, lQuantity, lWeight, lPrice, lUPrice, lTareCode, lBarcodeType, csCat) == -1)
      break;
	if (!csCode.IsEmpty() &&
			csCode[0] != INFO_ITEM &&
			csCode[0] != INFO_ITEM && 
			csCode[0] != VOID_ITEM &&
			csCode[0] != LINK_ITEM) //item is not voided, linked, or info (therefore it is selectable)
    {
      down = true;
      break;
    }
  }
  if (!down || lRcptPos == (ps.GetCMReceipt())->GetListCount() + 1)
      return false;
  else return true;
  */
  return false;
}
#endif //_CPPUNIT

#ifndef _CPPUNIT
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMVoidItemBase::OnWtIncreaseNotAllowed()	 
{
	return VoidingWtIncrease(BES_VOIDITEM, BES_VOIDITEM);
}
#endif //_CPPUNIT

#ifndef _CPPUNIT
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMVoidItemBase::OnWtDecrease()
{
	// TAR 391552 (New Fix) +
	SMStateBase::g_pDelay_WaitForApprovalStateChange = SMStateBase::createState(BES_SECUNEXPECTEDDECREASE);
	trace(L6,_T("We are at SMVoidItemBase::OnWtDecrease(), Saving next state BES_SECUNEXPECTEDDECREASE in g_pDelay_WaitForApprovalStateChange."));
	// TAR 391552 (New Fix) -
	//return ThresholdViolWtDecrease(BES_VOIDITEM, BES_VOIDITEM); //tar 391552
	return VoidingWtDecrease(); //TAR436424 / SSCOP-631
}
#endif //_CPPUNIT

SMStateBase *SMVoidItemBase::OnBackToLGW(void)
{
	trace(L2, _T("SMVoidItemBase::OnBackToLGW: clear g_pDelay_WaitForApprovalStateChange if needed."));

	if (g_pDelay_WaitForApprovalStateChange != NULL)
	{
		CString csStateName = g_pDelay_WaitForApprovalStateChange->GetRuntimeClass()->m_lpszClassName;
		delete g_pDelay_WaitForApprovalStateChange;
		g_pDelay_WaitForApprovalStateChange = NULL;
		trace(L6,_T("OnBackToLGW - Delete the state %s saved in g_pDelay_WaitForApprovalStateChange"),
			     csStateName);
	}
    
    return STATE_NULL;
}
