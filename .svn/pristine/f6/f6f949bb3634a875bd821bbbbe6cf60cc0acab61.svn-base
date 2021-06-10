//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTimeRestrictedItem.cpp
//
// TITLE: Class implimentation for TimeRestrictedItem State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMTimeRestrictedItem.h"       // MGV this state
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("TimeRestrictedItem")

IMPLEMENT_DYNCREATE(SMTimeRestrictedItem, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////////////
SMTimeRestrictedItem::SMTimeRestrictedItem()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////////////////////////
//  POS11024 Mobile Shopper add item description to display
////////////////////////////////////////////////////////////
SMStateBase * SMTimeRestrictedItem::Initialize(void)
{
    SMStateBase *pRetState = SMTimeRestrictedItemBase::Initialize();
	
	//Start CRD 257100
	CString csPOSState(_T(""));
    SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);
    if( csPOSState== _T("RESTRICTED_ITEM_MOBILE") )
	{
		CString csItemDesc = GetHookProperty(_T("OG_Guidance_Prop"));  
		CString csOrgMessage = ps.GetPSText(MSG_TIMERESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY);
		CString csMessage =_T("");
		csMessage.Format(csOrgMessage, csItemDesc);
		
		ps.Message12(csMessage, true);
		ps.ShowCMFrame();
	}	
	//End 257100

   return pRetState;
}

void SMTimeRestrictedItem::UnInitialize(void)
{
    SMTimeRestrictedItemBase::UnInitialize();
}

bool SMTimeRestrictedItem::PSAssistMenuLegalForState()
{
    return SMTimeRestrictedItemBase::PSAssistMenuLegalForState();
}

SMStateBase * SMTimeRestrictedItem::PSButton1(void)
{
    CString csPOSState(_T(""));
    SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);
    if( csPOSState== _T("RESTRICTED_ITEM_MOBILE") )
    {
		SendTBCommand(TBCMD_SENDKEYEDDATA, CString(_T("{F2}")));    
    }   

    return SMTimeRestrictedItemBase::PSButton1();
}

SMStateBase * SMTimeRestrictedItem::PSEnterKey(void)
{
    return SMTimeRestrictedItemBase::PSEnterKey();
}

SMStateBase *SMTimeRestrictedItem::TimedOut(void)
{
    return SMTimeRestrictedItemBase::TimedOut();
}