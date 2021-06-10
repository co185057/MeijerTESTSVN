//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAbort.CPP
//
// TITLE: Class implementation for store mode abort state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSmAbort.h"              // MGV this state
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmAbort")

IMPLEMENT_DYNCREATE(SMSmAbort, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////
SMSmAbort::SMSmAbort()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmAbort::Initialize(void)
{
	//CRD 237108
	//SSCOI-44496 - SMButton1 was erroneously disabled somewhere in the codes;
	//we need to test if there are items in the transaction. If there are, <Yes> button should be enabled
	SMStateBase  *pRetState = SMSmAbortBase::Initialize(); 
	ps.ButtonEnable(_T("SMButton1"), true);

    return pRetState; //Commenting this part for testing SSCOI-45231
	//return SMSmAbortBase::Initialize(); 
}

void SMSmAbort::UnInitialize(void)
{
    SMSmAbortBase::UnInitialize();
}

SMStateBase * SMSmAbort::OnWtIncreaseNotAllowed(void)
{
    return SMSmAbortBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMSmAbort::OnWtDecrease(void)
{
    return SMSmAbortBase::OnWtDecrease();
}

SMStateBase * SMSmAbort::PSButton1(void)
{
	bGenericDelayedFinishSent = true;	//CRD 232267
    return SMSmAbortBase::PSButton1();
}

SMStateBase * SMSmAbort::PSButton2(void)
{
    //sotf 6226
    if(m_lUnApprovedRestrictedItemsDuringCancel)
    {
        lUnapprovedRestrictedItems = m_lUnApprovedRestrictedItemsDuringCancel;
        m_lUnApprovedRestrictedItemsDuringCancel = 0;
    }
    //-sotf

	//Start CRD 366323
	NotifyExitStoreMode();
	//End CRD 366323

    return SMSmAbortBase::PSButton2();
}

SMStateBase * SMSmAbort::PSEnterKey(void)
{
    return SMSmAbortBase::PSEnterKey();
}

SMStateBase * SMSmAbort::PSCancelKey(void)
{
    return SMSmAbortBase::PSCancelKey();
}

SMStateBase * SMSmAbort::PSReceiptUp(void)
{
    return SMSmAbortBase::PSReceiptUp();
}

SMStateBase * SMSmAbort::PSReceiptDown(void)
{
    return SMSmAbortBase::PSReceiptDown();
}
