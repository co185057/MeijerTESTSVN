//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSuspend.cpp
//
// TITLE: Class implimentation for suspend state
//
//
// AUTHOR:    Ying Xie
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSuspend.h"       // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("Suspend")

IMPLEMENT_DYNCREATE(SMSuspend, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSuspend::SMSuspend()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSuspend::Initialize(void)
{
    trace(L7, _T("+ra.OnRequestSuspend"));
    ra.OnRequestSuspend(ps.GetPSText(RA_SUSPENDEDATLANE, SCOT_LANGUAGE_PRIMARY));
    trace(L7, _T("-ra.OnRequestSuspend"));
    trace(L7, _T("+ra.OnReadyToSuspend"));
    ra.OnReadyToSuspend();
    trace(L7, _T("-ra.OnReadyToSuspend"));
    
    return SMSuspendBase::Initialize();
}

void SMSuspend::UnInitialize(void)
{
    SMSuspendBase::UnInitialize();
}

SMStateBase * SMSuspend::PSButton1(void)
{
    bSuspendFailure = false;
    
    if( lCoupons && co.fOperationsVoidCouponBeforeSuspend)   
    {
        VoidAllCoupons();   // Void all coupons before suspend transaction
    }
    
    // tell TB to suspend the transaction
    return TBSuspendTransaction(TRUE);  // force suspension
}

SMStateBase * SMSuspend::TimedOut(void)
{
    return SMSuspendBase::TimedOut();
}
