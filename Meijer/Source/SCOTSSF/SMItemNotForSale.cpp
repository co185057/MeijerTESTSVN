//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMItemNotForSale.cpp
//
// TITLE: Class implimentation for unknown price State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMItemNotForSale.h"       // MGV this state

#include "SMInProgress.h"
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("ItemNotForSale")

IMPLEMENT_DYNCREATE(SMItemNotForSale, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////////////
SMItemNotForSale::SMItemNotForSale()
{
  IMPLEMENT_TIMESTAMP
}

// Needed to change the text string.  Unfortunately, other places use the same string so you couldn't
// just change the message file.  DEM 9/18/2007
SMStateBase * SMItemNotForSale::Initialize(void)
{
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ItemNotForSale") );
	
	// +TAR 407200
	if (fInMultiSelectPickList)
	{
		g_bMultiPickSkipBaggingAllowed = true;
	}
	// -TAR 407200
	
	DMSayPhrase(RECALLEDITEM);
	
	// remote approval
	CString csDesc, csInstruction;
	csDesc = ps.GetPSText(RA_LANEBTT_TIMERESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY); // tar 205236
	//TAR 235519
	csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(LTT_RECALLED, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_RECALLED, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_LANEBTT_TIMERESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY));
	trace(L7, _T("+ra.OnRecalledItem, Message=%s"), csInstruction);
	ra.OnRecalledItem(csInstruction);
	trace(L7, _T("-ra.OnRecalledItem"));
	if (co.IsImmedInterventionOn())
	{
		return PSButton1();
	}
	
	setAnchorState(BES_ITEMNOTFORSALE);
	
	dm.saveTriColorLightStatus();
	
	dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
		DM_TRICOLORLIGHT_BLINK_1HZ,
		DM_TRICOLORLIGHT_NORMALREQUEST);
	
	//POS11024 Mobile Shopper display item description and itemcode on Recalled Item screen
	CString csItemDesc = GetHookProperty(_T("tb_char"));  
	CString csItemCode = GetHookProperty(_T("item_code"));  
	CString csMessage = ps.GetPSText(MSG_RECALLED);
	csMessage += _T("\n\n") + csItemDesc + _T("  ") + csItemCode;
	
	ps.SetCMFrame(_T("ItemNotForSale"));
	ps.CMButtonFlash(_T("CMButton1Med"),true);   
	
	ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	ps.ShowCMTBText(csTBMessage2Scot);	//thp
	ps.Message12(csMessage, true);
	ps.ShowCMFrame();
	//For resumes with recalled items, set this flag to true  
	if( GetHookProperty(_T("IsResumeTrxInProgress")) == _T("1") )
	{
		fSAInItemSale = true;
	}   
  return STATE_NULL;   
}

void SMItemNotForSale::UnInitialize(void)
{
    SMItemNotForSaleBase::UnInitialize();
}

bool SMItemNotForSale::PSAssistMenuLegalForState()
{
    return SMItemNotForSaleBase::PSAssistMenuLegalForState();
}

//POS11024:Override base button to handle OK button click for recalled items
//Return Inprogress if we are in txn resume or flag value for sale is true
SMStateBase * SMItemNotForSale::PSButton1(void)
{
    SendTBCommand(TBCMD_SENDKEYEDDATA, CString(_T("{F2}")));
    if (fSAInItemSale || GetHookProperty(_T("IsResumeTrxInProgress")) == _T("1") )
    {
		RETURNSTATE(InProgress);
    }
    else
    {
		return SMItemNotForSaleBase::PSButton1();  
    } 
}

SMStateBase * SMItemNotForSale::PSEnterKey(void)
{
    return SMItemNotForSaleBase::PSEnterKey();
}

SMStateBase * SMItemNotForSale::TimedOut(void)
{
    return SMItemNotForSaleBase::TimedOut();
}
