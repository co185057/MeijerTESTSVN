//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSwipeCardAsItem.cpp
//
// TITLE: Class implementation Swipe phone card state
//
// AUTHOR:    Stephen J. Chasko
//
// Copyright (c) NCR Corp. 2003
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"
#include "SMSwipeCardAsItem.h"
#include "TransBrokerSSF.h"
#include "SMInprogress.h"
#include "SMScanAndBag.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SwipePhoneCard")


IMPLEMENT_DYNCREATE(SMSwipeCardAsItem, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP



SMSwipeCardAsItem::SMSwipeCardAsItem()
{
    IMPLEMENT_TIMESTAMP
    
}


SMStateBase *SMSwipeCardAsItem::Initialize(void)
{
   trace(L7,_T("+SMSwipeCardAsItem::Initialize"));     

   //turn off security temporarily
   CString csCurContext = _T("SwipeCardAsItem");  // Tar 238987
	
   TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, csCurContext );
	
   SMStateBase *sReturnState = CkStateStackAndReturn();

   
   if (sReturnState != STATE_NULL)
		  return sReturnState;   //back to security state

   CString strPropName = _T("IsVoidingPhoneGift");
   CString strPropValue;
   SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strPropName, &strPropValue);
   //the wave file is not used and has been overrided to play
   //the appropriate file 
   if( strPropValue == _T("1"))
   {
		DMSayPhrase(THANKYOUGIFTCARD); //SwipeCardAgain
   }
   else
   {
		DMSayPhrase(SELECTPAYMENTOPTION); //ForActivationSwipeCardonSignaturePad
   }

   ps.ShowAmountDueAmountPaid(lBalanceDue, lTotalPaid + nDMCashInserted);
   ps.SetCMFrame(_T("SwipeCardAsItem"));    
   ps.ShowCMFrame();

   trace(L7,_T("-SMSwipeCardAsItem::Initialize"));
   return STATE_NULL;
}




SMStateBase  *SMSwipeCardAsItem::PSButtonGoBack(void)
{

    ps.DisableButtons();
    //set TB back to Itemization and get ready to go back to scan and bag
    SendTBCommand(TBCMD_SET_TB_STATE,_T("SWIPED_PHONE_GIFT_EXIT"));    
   

    //cancel the current item in sale
    SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
    
    return setAndCreateAnchorState(BES_SCANANDBAG);

}

void SMSwipeCardAsItem::UnInitialize(void)
{
    
    CString csCurContext = _T("Attract");
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, csCurContext );

}




