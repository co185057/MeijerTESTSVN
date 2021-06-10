//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmUpdateItemBase.cpp
//
// TITLE: Class implementation for update item state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#include "SMSmUpdateItem.h"     

#ifndef _CPPUNIT
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMSmAssistMenu.h"    
#include "PSIntList.h"
#include "SMVoidNoMatch.h"    
#endif //_CPPUNIT 

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SmUpdateItemBase")

IMPLEMENT_DYNCREATE(SMSmUpdateItemBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmUpdateItem)


DEFINE_TIMESTAMP
#ifndef _CPPUNIT
//////////////////////////////////////////
SMSmUpdateItemBase::SMSmUpdateItemBase()
: csCurContext(EMPTY_STRING)
{
	IMPLEMENT_TIMESTAMP
		saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;	
}

//////////////////////////////////////////
SMSmUpdateItemBase::SMSmUpdateItemBase(long lUpdateType)
: csCurContext(EMPTY_STRING)
{
	m_lUpdateType = lUpdateType;
	IMPLEMENT_TIMESTAMP
		saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;	
}

//////////////////////////////////////////
SMStateBase *SMSmUpdateItemBase::Deliver(long lUpdateType)
{
	m_lUpdateType = lUpdateType;
	return this;	
}

// +TAR 438704
bool SMSmUpdateItemBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    const int nCount = dlParameterList.GetDescriptorCount();
    const int nExpected = 1;  // Should have one parameter of type long.

    if(nCount == nExpected)
    {
        trace(L6, 
         _T("+SMSmUpdateItem::DeliverDescriptorList() - Parameter Count of %d"),
         nCount);
        Deliver(DLParamDWORD(dlParameterList));   
        return true;
    }

    // Error.  parameter count is incorrect.
    trace(L2, 
    _T("+SMSmUpdateItem::DeliverDescriptorList() - ERROR - Parameter Count %d"),
         nCount);
    return false;
}
// -TAR 438704

////////////////////////////////////
SMStateBase *SMSmUpdateItemBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmUpdateItem") );	// RFC 330050
    m_bCoupon = false;
    m_nCrateableScreenAnswer = CRATEABLE_ANSWER_NO;
	COleVariant v;
	SMStateBase *sReturnState = CkStateStackAndReturn();
	if (sReturnState != STATE_NULL)
		return sReturnState;

  
    CString csItemEntryId = ps.GetSelection(_T("CMReceipt"));  //SSCOADK-5375
    trace(L6, _T("csItemEntryId from CMReceipt is %s"), csItemEntryId);
	if (!csItemEntryId.GetLength())
	{
		RETURNSTATE(SmAssistMenu)
	}
	else
    {
        ps.SetSelection( _T("SMReceipt"), csItemEntryId ); //SSCOADK=5375
    }
	ps.SetFrame(_T("SmUpdateItem"));
    ps.ButtonState(_T("SMButton1"), true, true);
	
    if (m_lUpdateType == ITEMUPDATE_QTY)
		ps.SetTitle(ps.GetPSText(MSG_ASSISTFUNCTIONS), ps.GetPSText(LTT_CHANGEQUANTITY));
	else if (m_lUpdateType == ITEMUPDATE_PRC)
		ps.SetTitle(ps.GetPSText(MSG_ASSISTFUNCTIONS), ps.GetPSText(LTT_CHANGEPRICE));

	SAWLDBScannerDisable();		  // disable the scanner
	
	ps.ShowFrame();

	DisplayAuxScreen(AUX_SCREEN_USE_TOUCHSCREEN, true);	// TAR 226994
	
	return STATE_NULL;							  
}

///////////////////////////////////
void SMSmUpdateItemBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmUpdateItem") );	// RFC 330050
}

///////////////////////////////////
SMStateBase *SMSmUpdateItemBase::PSButton1(void) 
{
  if(ps.GetCurrentContext()==_T("AM_UpdateNotAllowed"))
	RETURNSTATE(SmAssistMenu)
  else
    return OnChangeItem();
}


///////////////////////////////////
SMStateBase *SMSmUpdateItemBase::PSButton8(void) 
{
	RETURNSTATE(SmAssistMenu);               
}


////////////////////////////////////////
SMStateBase  *SMSmUpdateItemBase::PSCancelKey(void)/// override where needed
{
	DMSayPhrase(KEYPADVALID);
	return PSButtonGoBack();               // usually same as button 1
}

///////////////////////////////////////////////////////////
// PSNumericKey
///////////////////////////////////////////////////////////
SMStateBase  *SMSmUpdateItemBase::PSNumericKey(const long lKey)
{
  CString csVal = _T("");
  long lLen = 0;
  long lVal = 0;
  
  if (m_lUpdateType == ITEMUPDATE_PRC)
  {
      lLen = ps.GetInput(csVal);
      if(lLen > 0)
      {
          lVal = _ttol(csVal);
      }

      if(0 == lLen) //this is the first character
      {
	     if(lKey == '0') //throw leading 0 keys away
	     {
		     DMSayPhrase(KEYPADINVALID);
		     return STATE_NULL;
	     }
      }
  }

  PSEchoInput(lKey);

  lLen = ps.GetInput(csVal);
  lVal = _ttol(csVal);

  if (lVal==0)
    ps.EnterClearKeys(true, false);  // disable Enter, enable Clear key. 
  else
	ps.EnterClearKeys(false, false);

  return STATE_NULL;
}

/////////////////////////////////////////
SMStateBase  *SMSmUpdateItemBase::PSEnterKey(void)// enter key pressed, sell this item
{
  CString csInput;
  long lLen = ps.GetInput(csInput);

  if (lLen==0)
  {
    DMSayPhrase(KEYPADINVALID);
    return PSClearKey();
  }
	
  long lAmount = _ttol(csInput);
  if (lAmount == 0)
  {
    return PSClearKey();
  }
	
  ps.DisableButtons();

  if (m_lUpdateType == ITEMUPDATE_QTY)
  {
	  itemDetailNew.lQtySold = lAmount;
  }
  else if (m_lUpdateType == ITEMUPDATE_PRC)
  {
      if (m_bCoupon)
      {
          itemDetailNew.lUnitPrice = lAmount * -1;
      }
      else
      {
          itemDetailNew.lUnitPrice = lAmount;
      }
  }
  itemDetailNew.nCrateableScreenAnswer = m_nCrateableScreenAnswer;

  //SSCOADK-901: always pass false for fScannedItem in TBItemSale
  return TBItemSale(false, 0, false, false, m_lUpdateType);
}

SMStateBase  *SMSmUpdateItemBase::PSClearKey(void)
{

  PSClearAllInput(true);
 
  return STATE_NULL;
}

SMStateBase  *SMSmUpdateItemBase::PSReceiptUp(void)
{
  ps.ScrollUp(_T("SMReceipt"));
  COleVariant v = VARIANT_FALSE;
  m_pAppPSX->SetControlProperty(_T("SMInfoBoxField"), UI::PROPERTYVISIBLE, v);
  m_pAppPSX->SetControlProperty(_T("SMInfoBox"), UI::PROPERTYVISIBLE, v);
  return STATE_NULL;
}
#endif _CPPUNIT

SMStateBase  *SMSmUpdateItemBase::PSReceiptDown(void)
{
  // NextGen //
  if (0 != nPSLastParm)
  {
	  CString csItemId;
	  csItemId.Format(_T("%d"), nPSLastParm);
	  ps.GetPSXObject()->SetSelectedReceiptItem(_T("SMReceipt"), csItemId);
	  return STATE_NULL;
  }

  ps.ScrollDown(_T("SMReceipt"));
  COleVariant v = VARIANT_FALSE;
  m_pAppPSX->SetControlProperty(_T("SMInfoBoxField"), UI::PROPERTYVISIBLE, v);
  m_pAppPSX->SetControlProperty(_T("SMInfoBox"), UI::PROPERTYVISIBLE, v);
  return STATE_NULL;
}

#ifndef _CPPUNIT

SMStateBase *SMSmUpdateItemBase::OnChangeItem(void) 
{
   	CString csDesc,csCode;
    m_bCoupon = false;
	long lQty,lWgt,lPrc,lUPrc,lTcd, lBarcodeType;
	if (ps.GetItem(csDesc,csCode,lQty,lWgt,lPrc,lUPrc,lTcd,lBarcodeType, _T("CMSMReceipt"), _T("SMReceipt"))>0)// get selected item info MGG3
	{
		csItemSent = csCode;					  // save the item code
		itemDetailNew.csItemID = itemDetailOrig.csItemID = ps.GetSelection(_T("SMReceipt"));
		itemDetailNew.lQtySold = itemDetailOrig.lQtySold = io.lQuantityEntered = lQty; // saved qty
		itemDetailNew.lUnitPrice = io.lUnitPriceEntered = lUPrc; // saved unit price 
		itemDetailNew.lWgtSold = itemDetailOrig.lWgtSold = lWgt;
		lVoidTcd		= lTcd; // tare value -1,0,1,2...9,10,
        lVoidBarcodeType = lBarcodeType;
		itemDetailNew.lExtPrice = itemDetailOrig.lExtPrice = io.lExtPriceEntered = lPrc; //Tar 137876 add extended price
		itemDetailNew.csDescription = itemDetailOrig.csDescription = csDesc;

        COleVariant vValue = VARIANT_FALSE;
        if (m_pAppPSX->GetReceiptItemVariable(_T("CMSMReceipt"), itemDetailNew.csItemID, _T("ITEM_COUPON"), vValue) == PSX_SUCCESS)
        {
            if (vValue.boolVal == VARIANT_TRUE)
                m_bCoupon = true;
        }
        if (m_pAppPSX->GetReceiptItemVariable(_T("CMSMReceipt"), itemDetailNew.csItemID, _T("ITEM_CRATEABLE_ANSWER"), vValue) == PSX_SUCCESS)
        {
            if (vValue.boolVal == VARIANT_TRUE)
                m_nCrateableScreenAnswer = CRATEABLE_ANSWER_YES;
            else
                m_nCrateableScreenAnswer = CRATEABLE_ANSWER_NO;
        }
	}

	CString strItemDesc;
	if (m_lUpdateType == ITEMUPDATE_QTY)
	{
		if(lWgt >0)
		{
		  ps.SetFrame(_T("AM_UpdateNotAllowed"));
		}
		else
		{
		  ps.SetFrame(_T("AM_KeyInQuantity"));
		  strItemDesc.Format(ps.GetPSText(MSG_CHANGEQUANTITY), csDesc);
          ps.AllowInput(MAX_DIGITS_QUANTITY,false,false);
		}
	}
	else if (m_lUpdateType == ITEMUPDATE_PRC)
	{
		ps.SetFrame(_T("AM_KeyInPrice"));
		strItemDesc.Format(ps.GetPSText(MSG_CHANGEPRICE), csDesc);
        ps.AllowInput(MAX_DIGITS_CASHAMT,false,true);
	}
	if (strItemDesc !=_T(""))
      ps.SetLeadthruText(strItemDesc, _T(""));
	ps.ShowFrame();
	return STATE_NULL;
             
}
#endif _CPPUNIT