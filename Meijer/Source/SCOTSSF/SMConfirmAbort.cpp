//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmAbort.cpp
//
// TITLE: Class implimentation for trans abort State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "Common.h"                 // MGV common includes

#include "SMConfirmAbort.h"
#include "SMWaitForApproval.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("ConfirmAbort")

IMPLEMENT_DYNCREATE(SMConfirmAbort, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////
SMConfirmAbort::SMConfirmAbort()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMConfirmAbort::Initialize(void)
{
    return SMConfirmAbortBase::Initialize();
}

SMStateBase * SMConfirmAbort::OnWtIncreaseNotAllowed(void)
{
    return SMConfirmAbortBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMConfirmAbort::OnWtDecrease(void)
{
    return SMConfirmAbortBase::OnWtDecrease();
}

void SMConfirmAbort::UnInitialize(void)
{
    SMConfirmAbortBase::UnInitialize();
}

SMStateBase * SMConfirmAbort::TimedOut(void)
{
    return SMConfirmAbortBase::TimedOut();
}

SMStateBase * SMConfirmAbort::PSButton1(void)
{
    // Base calls SMVoidTransApprovalBase which is not representative of a cancelled transaction
    // rather it implements item void approval for some strange reason.
    //return SMConfirmAbortBase::PSButton1();//TBCancelTransaction(false); 
    
    ps.DisableButtons();
    // if an item was in the middle of being sold, like at the qty needed screen then it needs to be canceled.  will not hurt anything if it isn't needed
    SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
    
    //sotf 6226
    m_lUnApprovedRestrictedItemsDuringCancel = lUnapprovedRestrictedItems;
    lUnapprovedRestrictedItems = 0;
    //sotf 
	
    if ( isAnySoldVoidedItems() )
    {
        CREATE_AND_DISPENSE(WaitForApproval)(false,true);
    }
    return setAndCreateAnchorState(BES_FINISH);	 
}

SMStateBase * SMConfirmAbort::PSButton2(void)
{
    return SMConfirmAbortBase::PSButton2();
}

SMStateBase * SMConfirmAbort::PSButtonHelp(void)
{
    return SMConfirmAbortBase::PSButtonHelp();
}
