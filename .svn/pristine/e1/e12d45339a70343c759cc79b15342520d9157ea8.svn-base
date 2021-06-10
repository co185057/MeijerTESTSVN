//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCashPayment.cpp
//
// TITLE: Class implementation for Cash Payment State
//
//
// AUTHOR:    Peter Denhoed
//
// POS152672 Work Request:66559 Name:Robert Susanto Date:January 15, 2015
// POS45465 Work Request:52635 Name:Robert Susanto Date:November 1, 2012
// POS41957 Work Request:50865 Name:Robert Susanto Date:August 29, 2012
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"
#include "SMCashPayment.h"
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("CashPayment")

bool SMCashPayment::m_bIsHelpClick = false;

#define DMACCEPTOR_TIMEOUT 4

DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMCashPayment, CObject)// MFC Runtime class/object information

//////////////////////////////////////////
SMCashPayment::SMCashPayment()
{
	IMPLEMENT_TIMESTAMP
}

SMStateBase * SMCashPayment::Initialize(void)
{
    if( GetTBProperty(_T("NewFLState")) != _T("SMCashPayment") )
	{
		DMSayPhrase(INSERTCASH); 
		SetTBProperty(_T("NewFLState"),_T("SMCashPayment"));
	}
	
	
	nTimeOut = 1;
	
	if(!nDMCashInserted && m_bIsHelpClick)
		return PSButtonHelp();
	
	m_bIsHelpClick = false;
	
    SMStateBase *pState = SMCashPaymentBase::Initialize();
	
	nPreviousState = BES_CASHPAYMENT; //SSCOI-45180;

    //POS152672 - use different text for RAP windows as the lane has the \n sign
    UpdateStateName(ps.GetPSText(FTU_BEGIN, SCOT_LANGUAGE_PRIMARY));
	
	/*if(nDMCashInserted >= 100 && nDMLastCoinEvent == 0) //process immediately the bill
	return TBEnterTender(TB_TENDER_CASH, nDMCashInserted);
	else */
	return pState;
}

void SMCashPayment::UnInitialize(void)
{
    SMCashPaymentBase::UnInitialize();
}

SMStateBase * SMCashPayment::OnWtIncreaseNotAllowed(void)
{
    return SMCashPaymentBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMCashPayment::OnWtDecrease(void)
{
    return SMCashPaymentBase::OnWtDecrease();
}

SMStateBase * SMCashPayment::PSButton3(void)
{
    return SMCashPaymentBase::PSButton3();
}

SMStateBase * SMCashPayment::PSButton1(void)
{
    SetTBProperty(_T("NewFLState"),_T(""));
	return SMCashPaymentBase::PSButton1();
}

SMStateBase * SMCashPayment::DMAcceptor(void)
{
    // Reset time-out each time a coin/bill is inserted.
	//TAR 303853.
	nTimeIdleMsec = 0;
	
	trace(L6, _T("\nnDMCashInserted = %d, nDMLastCoinEvent = %d, balance due = %d"), nDMCashInserted, nDMLastCoinEvent, lBalanceDue );
	
	
    //POS45465 - check if the cashinserted is enough to pay the WIC balance
    CString csWICBalanceDue = GetTBProperty(_T("WICBalanceExceeded"));    
    if( csWICBalanceDue != _T("N/A") )
    {        
        long lWICBalanceDue = 0;
        
        csWICBalanceDue.Remove('.');
        lWICBalanceDue = _ttol(csWICBalanceDue);
		
        trace(L6, _T("Final WICBalanceDue = %d, CashInserted = %d"), lWICBalanceDue, nDMCashInserted);
		
        if( lWICBalanceDue > 0 )
        {
            if( nDMCashInserted >= lWICBalanceDue )
            {
                ps.SetCMBackground(BMP_CARDPROCESSING);
                ps.LeadThruText(LTT_CASHPROCESSING);
                ps.Message12(LTT_CASHPAYMENT, true);
                ps.SetCMGobackButtonVisible(false);
                ps.HideButtons();
                
                // If amount inserted is > balance due, show change due rather than amount due
                COleVariant v;
                v = ps.GetPSText(TXT_CHANGEDUE);
                m_pAppPSX->SetControlProperty(_T("Amount1Label"), UI::PROPERTYTEXT, v);
                ps.ShowChangeDueAmountPaid(nDMCashInserted-lBalanceDue, lTotalPaid+nDMCashInserted);
                //ps.Payment(lTotalPaid+nDMCashInserted,nDMCashInserted-lBalanceDue,TXT_CHANGEDUE);
				
                return TBEnterTender(TB_TENDER_CASH, nDMCashInserted);
            }            
        }        
    }
    // e - POS45465
	
	// TAR 363318 - as part of a fix for TAR 352672 the cash acceptor was disabled in SMInProgress.cpp
	// so it is disabled at this point. If there is still a balance due, it needs to be enabled here 	
	// if (nDMCashInserted<lBalanceDue)
	//if (lBalanceDue > 0)
	if (nDMCashInserted<lBalanceDue)
	{
		DMCashAcceptorEnable(co.csTenderDenominations);	// Coin Acceptor Modifications
		DMCoinAcceptorEnable();							// Coin Acceptor Modifications
		ps.ShowAmountDueAmountPaid(lBalanceDue - nDMCashInserted, lTotalPaid + nDMCashInserted);
		
		// commented for TAR352672
		/*if(nDMCashInserted >= 100 && nDMLastCoinEvent == 0) //bill should be processed immediately
		{
		m_nCashCoinAcceptorIdle = m_nProcessCoinsAfterThisTimeOut;
		return TimedOut();
	} */
		return STATE_NULL;
	}
	ps.SetCMBackground(BMP_CARDPROCESSING);
	ps.LeadThruText(LTT_CASHPROCESSING);
	ps.Message12(LTT_CASHPAYMENT, true);
	ps.SetCMGobackButtonVisible(false);
	ps.HideButtons();
	// TK - Migrating TAR 200329 & 200889
	// If amount inserted is > balance due, show change due rather than amount due
	COleVariant v;
	v = ps.GetPSText(TXT_CHANGEDUE);
	m_pAppPSX->SetControlProperty(_T("Amount1Label"), UI::PROPERTYTEXT, v);
	ps.ShowChangeDueAmountPaid(nDMCashInserted-lBalanceDue, lTotalPaid+nDMCashInserted);
	//ps.Payment(lTotalPaid+nDMCashInserted,nDMCashInserted-lBalanceDue,TXT_CHANGEDUE);
	
	
	//TAR352672
	return TBEnterTender(TB_TENDER_CASH, nDMCashInserted);
	//TAR346903+
	/*SMStateBase *pState = TBEnterTender(TB_TENDER_CASH, nDMCashInserted);
	
	  if (nDMCashTendered < lBalanceDue)
	  { 
	  delete getRealStatePointer( pState);
	  return STATE_NULL;
	  }
	return pState;*/
	//TAR346903-
}

SMStateBase * SMCashPayment::SAEventOccurred(const long lEvent)
{
    return SMCashPaymentBase::SAEventOccurred(lEvent);
}

SMStateBase * SMCashPayment::TimedOut(void)
{
	// +SSCOI-47507
    CString csTenderType;
    long lTenderSelected = SendTBCommand(TBCMD_GET_CUSTOMER_SELECTED_TENDER, &csTenderType);
    if(lTenderSelected && csTenderType != _T("CASH"))
    {
		SetTBProperty(_T("NewFLState"),_T(""));
		nTimeOut = 0;
		return setAndCreateAnchorState(BES_FINISH);
	}

	CString csPosState(_T(""));
    SendTBCommand(TBCMD_GET_POSSTATE, &csPosState);

	trace(L6, _T("PosState=%s, CashInserted=%d"), csPosState, nDMCashInserted);

	// POS2888333, 287563 and 290569 
	// sometimes the cash that was inserted got skipped due to card activity - process it now
	if(nDMCashInserted > 0 && csPosState.Find(_T("CUSTOMER_TENDER_SELECTED")) != -1 ||
		(nDMCashInserted > 0 && ( GetTBProperty(_T("IsFromMPerksPrompt")) == _T("1") && 
		csPosState.Find(_T("HIGHVALUE_PROMO_CHOICE")) == -1)))
	{
	    // process only the cash inserted within these PosState
		if( csPosState.Find(_T("CUSTOMER_TENDER_SELECTED")) != -1 || csPosState.Find(_T("PAYMENTMENU_")) != -1)				
		{
	       ps.Message12(LTT_CASHPROCESSING, true);
		   SetTBProperty(_T("IsFromMPerksPrompt"),_T("0"));
		   trace(L6, _T(" SMCashPayment::TimedOut() cash has been inserted and the card has been swiped or inserted, start cash tender first"));
		   return PSButton1();
		}
	}

	return STATE_NULL;
	// -SSCOI-47507
}

SMStateBase * SMCashPayment::PSCancelKey(void)
{
    return SMCashPaymentBase::PSCancelKey();
}

SMStateBase * SMCashPayment::PSButtonGoBack(void)
{
    return SMCashPaymentBase::PSButtonGoBack();
}

SMStateBase *SMCashPayment::PSButtonHelp()
{
	if(nDMCashInserted)
	{
		m_bIsHelpClick = true;
		return TBEnterTender(TB_TENDER_CASH, nDMCashInserted);
	}
	else
	{
		m_bIsHelpClick = false;
		return SMCashPaymentBase::PSButtonHelp();
	}
}
