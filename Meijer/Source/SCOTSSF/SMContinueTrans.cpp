//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMContinueTrans.cpp
//
// TITLE: Class implimentation for contionue transaction state
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMContinueTrans.h"        // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID _T("ContinueTrans")

IMPLEMENT_DYNCREATE(SMContinueTrans, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////
SMContinueTrans::SMContinueTrans(void)
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMContinueTrans::Initialize(void)
{
	//	start CRD 125377
	trace(L6, _T("m_bShopAndDropEnabled=%d m_bSNDAllowItemMenu=%d"), m_bShopAndDropEnabled, m_bSNDAllowItemMenu);
	if (m_bShopAndDropEnabled)
	{
		m_bSNDAllowItemMenu = true;		// CRD 125377
		trace(L6, _T("m_bSNDAllowItemMenu=%d"), m_bSNDAllowItemMenu);
	}
	//  end CRD 125377
    return SMContinueTransBase::Initialize();
}

void SMContinueTrans::UnInitialize(void)
{
    SMContinueTransBase::UnInitialize();
}

SMStateBase * SMContinueTrans::TimedOut(void)
{


    //(+) VSTS 131568
	SMState::bVoidApprovalDelayed = false;
	trace(L6,_T(" SMState::bVoidApprovalDelayed Continuous trans (%s)"), SMState::bVoidApprovalDelayed ? _T("TRUE") : _T("FALSE"));
    // (-) VSTS 131568

    return SMContinueTransBase::TimedOut();
}

SMStateBase * SMContinueTrans::PSButton1(void)
{
    return SMContinueTransBase::PSButton1();
}

SMStateBase * SMContinueTrans::PSButton2(void)
{
    return SMContinueTransBase::PSButton2();
}

SMStateBase * SMContinueTrans::DMCardReader(void)
{
    return SMContinueTransBase::DMCardReader();
}

SMStateBase  *SMContinueTrans::DMAcceptor(void)
{
	//TAR 303853.
	// In case cash/coins were on the way in just prior to timing out!
	// Just update totals - rest will happen if/when we return.
	ps.ShowAmountDueAmountPaid(lBalanceDue - nDMCashInserted, lTotalPaid + nDMCashInserted);
	return STATE_NULL;
}
