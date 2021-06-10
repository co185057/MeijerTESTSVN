//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAborted.CPP
//
// TITLE: Class implementation for store mode aborted state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSmAborted.h"            // MGV abort-ed state

#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmAborted")

IMPLEMENT_DYNCREATE(SMSmAborted, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////
SMSmAborted::SMSmAborted()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmAborted::Initialize(void)
{
	//OK button should be enabled unconditionally. This lets the customer acknowledge the message and get back to the home screen.
	//ps.ButtonEnable(_T("SMButton1"), true); //Commenting this part for testing SSCOI-45231

    return SMSmAbortedBase::Initialize();
}

void SMSmAborted::UnInitialize(void)
{
    SMSmAbortedBase::UnInitialize();
}

SMStateBase * SMSmAborted::PSButton1(void)
{
	// Start CRD 125377 
	if (m_bShopAndDropEnabled)
	{ 
		m_bSNDxCancelTrans = true;	// Allow Thank You exit without a wait.
	}
	// End CRD 125377
    return SMSmAbortedBase::PSButton1();
}

SMStateBase * SMSmAborted::PSEnterKey(void)
{
    return SMSmAbortedBase::PSEnterKey();
}
