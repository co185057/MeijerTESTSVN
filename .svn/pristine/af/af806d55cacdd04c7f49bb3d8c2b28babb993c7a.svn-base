//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmVoid.cpp
//
// TITLE: Class implimentation for Confirm Void State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMConfirmVoid.h"          // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("ConfirmVoid")

IMPLEMENT_DYNCREATE(SMConfirmVoid, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

#ifdef _TRADESHOWDEMO
#include "..\TradeShowDemo\SMConfirmVoidTSDemo.cpp"
#endif //_TRADESHOWDEMO

//////////////////////////////////////////
SMConfirmVoid::SMConfirmVoid()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMConfirmVoid::Initialize(void)
{
    return SMConfirmVoidBase::Initialize();
}

void SMConfirmVoid::UnInitialize(void)
{
    SMConfirmVoidBase::UnInitialize();
}

SMStateBase * SMConfirmVoid::OnWtIncreaseNotAllowed(void)
{
    return SMConfirmVoidBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMConfirmVoid::OnWtDecrease(void)
{
    return SMConfirmVoidBase::OnWtDecrease();
}

bool SMConfirmVoid::PSAssistMenuLegalForState()
{
    return SMConfirmVoidBase::PSAssistMenuLegalForState();
}
#ifndef _TRADESHOWDEMO
SMStateBase * SMConfirmVoid::PSButton1(void) // Yes, Void is ok
{
    // clear security if we're voiding a Message line (TMD & Guest card)
    if( -1 != ps.GetSelection(_T("CMReceiptVoid")).Find(_T("MSG")) )
    {
        CKeyValueParmList parmList;
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_IV_OPERATION, KEY_IV_OPERATION_END);
		_bstr_t bstrParmList((const _TCHAR *)parmList); 
		SecMgr.VoidItem(lSecMgrCookie, bstrParmList);
    }
     // (+) VSTS 131568
	trace(L6, _T("fstateAssist <%d>"), fStateAssist); 

	trace(L5,_T("Confirm void assistMenus=%d"),co.IsInAssistMenus());

	if(!co.IsInAssistMenus())
	{	
	trace(L6, _T("confirm void voiding")); 
	 SMState::nVoidCounts++;

	if(SMState::nVoidCounts<4)
	{
	  SMState::bVoidApprovalDelayed = true;
	}
	else
	{
		SMState::bVoidApprovalDelayed = false;
	}
	}

	SMStateBase  *pRetState = SMConfirmVoidBase::PSButton1();
	return pRetState;
  //  return SMConfirmVoidBase::PSButton1();

  // (-) VSTS 131568
}
#endif _TRADESHOWDEMO

SMStateBase * SMConfirmVoid::PSButton2(void)
{
    return SMConfirmVoidBase::PSButton2();
}
