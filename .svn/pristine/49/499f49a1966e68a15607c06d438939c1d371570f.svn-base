//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSellBagsBase.CPP
//
// TITLE: Class implementation for Sell Bags state - Done for SR713 by Gerald C. Cabaral
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMScanAndBagBase.h"      
#include "SMFinishBase.h"
#include "SMSellBags.h"     
#include "PSConfigData.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SellBagsBase")

IMPLEMENT_DYNCREATE(SMSellBagsBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SellBags)

DEFINE_TIMESTAMP


CString SMSellBagsBase::m_csBagItemCode = _T("");
CString SMSellBagsBase::m_csBagPrice = _T("");

//////////////////////////////////////////
SMSellBagsBase::SMSellBagsBase()
{
  IMPLEMENT_TIMESTAMP
  
	 
}

SMSellBagsBase::SMSellBagsBase(LPCTSTR sItemCode, const long lQty, LPCTSTR sPrice)
{
	m_csBagItemCode = sItemCode;
	m_lQty		= lQty;
    m_csBagPrice	= sPrice;

	IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMSellBagsBase::Deliver(LPCTSTR sItemCode, const long lQty, LPCTSTR sPrice)
{
	
	m_csBagItemCode = sItemCode;
	m_lQty		 = lQty;
	m_csBagPrice = sPrice;

	return this;
}

bool SMSellBagsBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be three parameters sItemCode, lQty and sPrice
    if (dlParameterList.GetDescriptorCount() == 3)
    {
        trace(L6, _T("+SMSellBagsBase:DeliverDescriptorList() - Parameter Count of 3"));
        Deliver(DLParamCString(dlParameterList),	// sItemCode
				DLParamDWORD(dlParameterList),		// lQty
				DLParamCString(dlParameterList));	// sPrice
        return true;
    }
    else
    {
        trace(L2, _T("+SMSellBagsBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}

SMStateBase *SMSellBagsBase::Initialize()
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SellBags") );	
	
	
	DMMSRDisable();
    //DMScaleDisable();
	DMScannerDisable();
	DMCashAcceptorDisable();
	DMCoinAcceptorDisable();
	ra.RequestEnableAssistMode(false);
	setAnchorState(BES_SELLBAGS);

	io.Reset();
	io.csOrigCode = m_csBagItemCode;	   // item code keyed
	io.csItemCode = m_csBagItemCode;	   // assumed to be the same

		UpdateStateName(ps.GetPSText(LTT_BAGSALE, SCOT_LANGUAGE_PRIMARY));
		ps.SetCMFrame(_T("EnterQuantity"));  
		COleVariant v;
		m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("EnterQuantity"), UI::PROPERTYVISIBLE, VARIANT_TRUE); 
		DMSayPhrase(QUANTITYNEEDED);
		ps.Message12(MSG_BAGSALEPROMPT, false );
		ps.SetCMBackground(BMP_TYPEITEMDESC);
    	ps.SetCMLeadthruText(LTT_BAGSALE);
		ps.ShowNumericKeypad();
	
		ps.SetCMMascotImage(BMP_MASCOT_ATMKEY);

		ps.SetCMGobackButtonVisible(true);
		ps.AllowInput(MAX_DIGITS_QUANTITY,false,false);


	ps.ShowAmountDueAmountPaid(lBalanceDue - nDMCashInserted, lTotalPaid + nDMCashInserted);

	ps.ShowCMTBText(csTBMessage2Scot);	//thp

	
	
	ps.ShowCMFrame();
	return STATE_NULL; 
	
}

void SMSellBagsBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SellBags") );	
	
}

SMStateBase  *SMSellBagsBase::PSNumericKey(const long lKey)
{
  PSEchoInput(lKey);

  // Show OK key if qty > 0.
  CString csQty;
  long lLen = ps.GetInput(csQty);
  m_lQty = _ttol(csQty);

   //if (m_lQty==0)
	//ps.EnterClearKeys(true, false);  // disable Enter, enable Clear key.  

   
  return STATE_NULL;
}


SMStateBase *SMSellBagsBase::PSClearKey(void)
{
  PSClearAllInput(true);
  return STATE_NULL;
}

SMStateBase *SMSellBagsBase::PSEnterKey(void)
{
   CString csQty;
	
 

  long lLen = ps.GetInput(csQty);

  if (lLen==0)
  {
    DMSayPhrase(KEYPADINVALID);
    return PSClearKey();
  }

 

  bDoNotShowSellBags = true;

  m_lQty = _ttol(csQty);
  if (m_lQty == 0)
  { 
	trace(L6,_T("SMSellBagsBase::PSEnterKey, Quantity of Plastic Bags is %d"),m_lQty);
	return setAndCreateAnchorState(BES_FINISH);
    
  }
	
  ps.DisableButtons();

  io.lQuantityEntered = m_lQty;
  io.d.fQuantityRequired = true;
  io.d.fQuantityConfirmed = false;
	
 
  trace(L6,_T("SMSellBagsBase::PSEnterKey, Quantity of Plastic Bags is %d"),m_lQty);
  return ConfirmQtyForBags();

}

SMStateBase *SMSellBagsBase::PSButtonGoBack(void)
{		
		SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);

		return setAndCreateAnchorState(BES_SCANANDBAG);	  
}

//////////////////////////////////////////
SMStateBase  *SMSellBagsBase::PSButton1(void) // Yes, Quantity is ok
{  

	SASendSCOTInput( UPC_MANUAL, m_csBagItemCode);
	m_csBagItemCode = _T("");
	m_csBagPrice	= _T("");
	return TBEnterItem();

}

//////////////////////////////////////////
SMStateBase  *SMSellBagsBase::PSButton2(void) // no, Quantity is wrong
{
  
  bDoNotShowSellBags = false;
  return Initialize();
}

SMStateBase *SMSellBagsBase::ConfirmQtyForBags(void)
{

	UpdateStateName(ps.GetPSText(LTT_QTYCONFIRM, SCOT_LANGUAGE_PRIMARY));
   //@@@INFOPOINT

	ps.SetCMFrame(_T("ConfirmQuantity"));
	
	ps.ShowAmountDueAmountPaid(lBalanceDue - nDMCashInserted, lTotalPaid + nDMCashInserted);
		
	

	ps.ShowCMTBText(csTBMessage2Scot);	//thp
	//@@@INFOPOINT
  
	DMSayPhrase(CONFIRMQTY);
	CString csWork,sPrice;
   
		
	long lPrice=0,lTotalPrice=0;
	lPrice = _tcstol(m_csBagPrice, NULL, 10);
	lTotalPrice = lPrice * m_lQty;

	csWork.Format(ps.GetPSText(MSG_BAGPRICE),m_lQty,ps.FmtDollar(lTotalPrice, ps.SP_OTHER));
		
	
	ps.Message12(csWork, true);
	 

	ps.ShowCMFrame();
	return STATE_NULL;  
}
