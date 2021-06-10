//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectPaymentBase.cpp
//
// TITLE: Class implementation for Select Payment Amount state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSelectPayment.h"    // This state
#include "SMSelectCardType.h"       // Card type state
#include "SMTakeCard.h"             // Take card
#include "SMCardMisRead.h"          // Card read error
#include "SMConfirmDebitAmount.h"   // Confirm debit amount
#include "SMConfirmEBTAmount.h"     // Confirm EBT amount

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SelectPaymentBase")

IMPLEMENT_DYNCREATE(SMSelectPaymentBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SelectPayment)

DEFINE_TIMESTAMP

//////////////////////////////////
SMSelectPaymentBase::SMSelectPaymentBase(const TBTENDERTYPE nTyp)
: nTndrType(nTyp)
{
  if (nTndrType != TB_TENDER_EBT && nTndrType != TB_TENDER_PIP_EBT )
  {
    m_pCashBackList = co.nTenderCashBack;
    m_CashBackListNum = co.nTenderCashBackNum;
  }
  else
  {
    m_pCashBackList = co.nTenderEBTCashBack;
    m_CashBackListNum = co.nTenderEBTCashBackNum;
  }

  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSelectPaymentBase::SMSelectPaymentBase()
{
  IMPLEMENT_TIMESTAMP
  saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
}

////////////////////////////////////////
SMStateBase  *SMSelectPaymentBase::Deliver(const TBTENDERTYPE nTyp)
{
  nTndrType = nTyp;
  if (nTndrType != TB_TENDER_EBT && nTndrType != TB_TENDER_PIP_EBT)
  {
    m_pCashBackList = co.nTenderCashBack;
    m_CashBackListNum = co.nTenderCashBackNum;
  }
  else
  {
    m_pCashBackList = co.nTenderEBTCashBack;
    m_CashBackListNum = co.nTenderEBTCashBackNum;
  }
  return this;
}
//////////////////////////////////////////
bool SMSelectPaymentBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be one parameter, a dword
    if (dlParameterList.GetDescriptorCount() == 1)
    {
        trace(L6, _T("+SMSelectPaymentBase::DeliverDescriptorList() - Parameter Count of 1"));
        Deliver((TBTENDERTYPE)DLParamDWORD(dlParameterList));    // nTyp
        return true;
    }
    else
    {
        trace(L2, _T("+SMSelectPaymentBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}
//////////////////////////////////////////
SMStateBase  *SMSelectPaymentBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SelectPayment") );	// RFC 330050
  // if cash back is not configured...
  if (m_CashBackListNum == 0)
  {
    // no cash back is allowed
    trace(L6,_T("SelectPayment 2 pin <%s>, no cash back allowed"),csDMEncryptedPin);

    // the TB BALANCE/CHANGE message will change us to either
    // STCashPayment or STTakeChange or STTakeReceipt
    return TBEnterTender(nTndrType,	lBalanceDue);
  }

  UpdateStateName(ps.GetPSText(LTT_DEBITAMT, SCOT_LANGUAGE_PRIMARY));
  //tar 226722
  if (nTndrType == TB_TENDER_EBT)
  {
	ps.SetCMFrame(_T("SelectPaymentEBT"));
	ps.ShowCashBackList(lBalanceDue, m_pCashBackList, m_CashBackListNum, true);
  }
  else
  {
	ps.SetCMFrame(_T("SelectPayment"));
	ps.ShowCashBackList(lBalanceDue, m_pCashBackList, m_CashBackListNum, false);
  }
  //end of tar 226722
  
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.Message12(MSG_DEBITAMT, false);
	
  DMSayPhrase(SELECTAMT);

  DisplayAuxScreen(AUX_SCREEN_SELECT_AMOUNT, true);
  ps.ShowCMFrame();
  return STATE_NULL;                // stay in SelectPayment state
}

/////////////////////////////////////////
// Button 3 (Cancel)
SMStateBase  *SMSelectPaymentBase::PSButton3(void)
{
  CREATE_AND_DISPENSE(TakeCard)(false);
}
/////////////////////////////////////////
// 
SMStateBase  *SMSelectPaymentBase::PSButtonGoBack(void)
{
  CREATE_AND_DISPENSE(TakeCard)(false);
}

SMStateBase  *SMSelectPaymentBase::PSCancelKey(void)/// override where needed
{
  DMSayPhrase(KEYPADVALID);
  return PSButtonGoBack();
}

//
// The SCOTAuxDisp device will spoof it's cash back amounts thru
// DPS by way of this function.  The long lAmount is the actual 
// cash back amount, not just a key 0-9
//
SMStateBase  *SMSelectPaymentBase::PSNumericKey(const long lAmount)	// spoofed cashback amt from AuxDisp
{
  return HandleAmount(lAmount);
}

SMStateBase *SMSelectPaymentBase::HandleAmount(const long lAmount)
{
	// BeginCodeLocale
  // lAmt *= 100;
	long lAmt = lAmount * co.nLocalePriceCentUnit;
	// EndCodeLocale
	
  trace(L6,_T("SelectPaymentBase 2 pin <%s>, cash back %d"),csDMEncryptedPin,lAmt);

  // the TB BALANCE/CHANGE message will change us to either
  // STCashPayment or STTakeChange or STTakeReceipt

	if(!(co.fOperationsPartialIntegratedPINPad && !co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bIntegratedMSR))
	{
	//Non-PIP
	  if (nTndrType == TB_TENDER_EBT || nTndrType == TB_TENDER_PIP_EBT )   //EBT
	  {
		if(co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bConfirmTotal)
		{
			CREATE_AND_DISPENSE(ConfirmEBTAmount)(nTndrType,lBalanceDue, lAmt);
		}
		else
			return TBEnterTender(
				nTndrType,
				lBalanceDue+lAmt);
	  }

	  if (nTndrType == TB_TENDER_FOODSTAMPS || nTndrType == TB_TENDER_PIP_FOODSTAMPS )   //FS
	  {
		if(co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bConfirmTotal)
		{
			CREATE_AND_DISPENSE(ConfirmEBTAmount)(nTndrType,lFoodStampsDue, lAmt);
		}
		else
		{
			// submit food stamp total instead of balancedue to let customer
			// get change for his/her coupons
			long nSubmitTotal = lFoodStampsDue;
			if( lBalanceDue < lFoodStampsDue) // customer has already paid part of balance off  //Tar 154631
			{
				// safeway fix, lFoodStampsDue = lBalanceDue; //gets passed in as the available EBT balance
				if (!co.fTenderSubmitFSTtotalIfMorethanTrxTotal)
				{
					nSubmitTotal = lBalanceDue;
				}
			}
			return TBEnterTender(nTndrType,nSubmitTotal +lAmt);	
		}
	  }


	  //Other cards with cash back

		if(co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bConfirmTotal)
		{
			CREATE_AND_DISPENSE(ConfirmDebitAmount)(nTndrType,lBalanceDue+lAmt);
		}
		
	  // the TS BALANCE/CHANGE message will change us to either
	  // STCashPayment or STTakeChange or STTakeReceipt

	  return TBEnterTender(
		  nTndrType,
			lBalanceDue+lAmt);
	}
	else
	{
		//PIP
		if((co.bReadPIPSCOTPromptConfirmTotal && co.fOperationsPIPSCOTPromptConfirmTotal ) || (!co.bReadPIPSCOTPromptConfirmTotal && co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bConfirmTotal))  //confirm total
		 {		  
			if (nTndrType == TB_TENDER_FOODSTAMPS || nTndrType == TB_TENDER_PIP_FOODSTAMPS )
			{
				CREATE_AND_DISPENSE(ConfirmEBTAmount)(nTndrType,lFoodStampsDue,lAmt );
			}
			else if (nTndrType == TB_TENDER_EBT || nTndrType == TB_TENDER_PIP_EBT )   //EBT
			{
				CREATE_AND_DISPENSE(ConfirmEBTAmount)(nTndrType,lBalanceDue,lAmt);
			}
			else  //Other cards
			{
				CREATE_AND_DISPENSE(ConfirmDebitAmount)(nTndrType,lBalanceDue + lAmt);
			}
 		  }
		  else
		  {    // Call PIP EnterTender
			  if (nTndrType == TB_TENDER_FOODSTAMPS || nTndrType == TB_TENDER_PIP_FOODSTAMPS )	
			  {
					// submit food stamp total instead of balancedue to let customer
					// get change for his/her coupons
					long nSubmitTotal = lFoodStampsDue;
					if( lBalanceDue < lFoodStampsDue) // customer has already paid part of balance off  //Tar 154631
					{
						// safeway fix, lFoodStampsDue = lBalanceDue; //gets passed in as the available EBT balance
						if (!co.fTenderSubmitFSTtotalIfMorethanTrxTotal)
						{
							nSubmitTotal = lBalanceDue;
						}
					}
					return TBEnterTender(nTndrType,nSubmitTotal +lAmt);	
			  }
			  else
				  return TBEnterTender(nTndrType,lBalanceDue +lAmt);	
		  }

	}

}

////////////////////////////////////////////
// List or Lookup
SMStateBase  *SMSelectPaymentBase::PSListLookup(CString cSelection)// list or lookup selection
{
// NewUI
  long lAmt = 0;                    // assume invalid selection
  long lSelection = 0;
  lSelection = _ttoi(cSelection);
	
  // CANNOT enter Remote Assist Mode at this state
  //ra.OnSetTransactionState(RA_STATE_ASSISTNOTOK);
    ra.RequestEnableAssistMode(false);   //TAR 213249

  if (lSelection == 1)
  {
    lAmt = 0;
  }
  else if ((lSelection > 1) && (lSelection < (m_CashBackListNum+2)))
  {
    lAmt = m_pCashBackList[lSelection - 2];
  }

	if (lAmt==0 && lSelection>1)
	{
    DMSayPhrase(KEYPADINVALID);

	if (nTndrType == TB_TENDER_EBT)
		ps.ShowCashBackList(lBalanceDue, m_pCashBackList, m_CashBackListNum, true);
	else
		ps.ShowCashBackList(lBalanceDue, m_pCashBackList, m_CashBackListNum, false);

    return STATE_NULL;
	}
	
  return HandleAmount(lAmt);
// NewUI
}
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSelectPaymentBase::OnWtIncreaseNotAllowed()	 
{
	return FinalizationWtIncrease(BES_STATERETURN);	
}


///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSelectPaymentBase::OnWtDecrease()
{
	if(!g_bIsForgiveUnexpectedDecreaseDuringTendering)
		return FinalizationWtDecreaseNotAllowed(BES_STATERETURN);
	else
		return STATE_NULL;
}

//////////////////////////////////////////////////////////////////////////
void SMSelectPaymentBase::UnInitialize()
{
	// CAN enter Remote Assist Mode after leaving this state
	//ra.OnSetTransactionState(RA_STATE_ASSISTOK);
    ra.RequestEnableAssistMode(true);   //TAR213249
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SelectPayment") );	// RFC 330050

}
