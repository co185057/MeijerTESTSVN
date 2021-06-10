//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSm//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmConfirmSuspendedTransaction.CPP
//
// TITLE: Class implementation for store mode ConfirmSuspendedTransaction state
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"
#include "SMSmConfirmSuspendedTransaction.h"

#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmConfirmSuspendedTransaction")

IMPLEMENT_DYNCREATE(SMSmConfirmSuspendedTransaction, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////
SMSmConfirmSuspendedTransaction::SMSmConfirmSuspendedTransaction()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmConfirmSuspendedTransaction::Initialize(void)
{
    return SMSmConfirmSuspendedTransactionBase::Initialize();
}

void SMSmConfirmSuspendedTransaction::UnInitialize(void)
{
    SMSmConfirmSuspendedTransactionBase::UnInitialize();
}

SMStateBase * SMSmConfirmSuspendedTransaction::PSButton1(void)
{
    m_lLastSuspendTotal = lTotalPaid + lTotalSale;
    if( nDMCashInserted > 0 )   // already paid cash
    {
        TBEnterTender(TB_TENDER_CASH, -1*nDMCashInserted);  // void these cash payment  
        SMState::HandleVoidedTender();
    }
    //return SMSmConfirmSuspendedTransactionBase::PSButton1();
    bSuspendFailure = false;
    SynchronizeStoreloginWithLaunchPad(_T("operation=set-signed-off")); // TAR 301706
    if( lCoupons && co.fOperationsVoidCouponBeforeSuspend)
    {
        VoidAllCoupons();   
    }
    
    if (!ps.RemoteMode())
    {
        //POS61842 this logic would need to be in here instead on the initialize so that it will be in synch - rrs
        trace(L7, _T("+ra.OnRequestSuspend"));
        ra.OnRequestSuspend(ps.GetPSText(RA_SUSPENDEDATLANE));
        trace(L7, _T("-ra.OnRequestSuspend"));
        trace(L7, _T("+ra.OnReadyToSuspend"));
        ra.OnReadyToSuspend();
        trace(L7, _T("-ra.OnReadyToSuspend"));
        //e POS61842
		
		
        trace(L7, _T("+ra.OnSuspendApproval"));
        ra.OnSuspendApproval();
        trace(L7, _T("-ra.OnSuspendApproval"));
    }
    
    ClearStateStackEOT();
    
    return TBSuspendTransaction(); 
}

SMStateBase * SMSmConfirmSuspendedTransaction::PSButton2(void)
{
    return SMSmConfirmSuspendedTransactionBase::PSButton2();
}

SMStateBase * SMSmConfirmSuspendedTransaction::PSReceiptUp(void)
{
    return SMSmConfirmSuspendedTransactionBase::PSReceiptUp();
}

SMStateBase * SMSmConfirmSuspendedTransaction::PSReceiptDown(void)
{
    return SMSmConfirmSuspendedTransactionBase::PSReceiptDown();
}
