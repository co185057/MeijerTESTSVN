//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMUnknownItem.cpp
//
// TITLE: Class implimentation for unknown Item State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes
 
#include "SMState.h"                // MGV base state
#include "SMUnknownItem.h"          // MGV this state

#include "transbrokerssf.h"
#include "SMInProgress.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("UnknownItem")

IMPLEMENT_DYNCREATE(SMUnknownItem, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMUnknownItem::SMUnknownItem()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMUnknownItem::Initialize(void)
{
	//	start CRD 125377
	trace(L6, _T("m_bShopAndDropEnabled=%d m_bSNDAllowItemMenu=%d lUnknownItemsKeyed=%d lUnknownItems=%d"), m_bShopAndDropEnabled, m_bSNDAllowItemMenu, lUnknownItemsKeyed, lUnknownItems);
	long lSavelUnknownItems = lUnknownItems;
	if (m_bShopAndDropEnabled)
	{
		m_bSNDAllowItemMenu = true;		// CRD 125377
		lUnknownItems = 0;
		trace(L6, _T("m_bSNDAllowItemMenu=%d"), m_bSNDAllowItemMenu);
	}
	//  end CRD 125377
    dm.saveTriColorLightStatus();    
    /*POS11024: Mobile Shopper - Start*/
    SMStateBase *pRetState = SMUnknownItemBase::Initialize();
	trace(L6, _T("lUnknownItemsKeyed=%d lUnknownItems=%d"), lUnknownItemsKeyed, lUnknownItems);
	if (m_bShopAndDropEnabled)
	{
		lUnknownItems = lSavelUnknownItems;
	}
	//  end CRD 125377
    //Append itemcode to Instruction Text area returned by base
    CString csCurContext = ps.GetCurrentContext();
    COleVariant v;
    m_pAppPSX->GetConfigProperty(_T("InstructionTextArea"), csCurContext, UI::PROPERTYTEXTFORMAT, v);
    
	if ( m_csTransactionMode == TRANSMODE_SND) // CRD127035
	{
		ps.SetCMLeadthruText(_T("$ScotApp_9510$"));
		ps.Message12(_T("$ScotApp_9511$"), true);
		DMSayPhrase(_T("SNDItemnotallowed"));
	}
	else
	{
    CString csItemCode = GetHookProperty(_T("item_code"));  
	
	//Start - fix for Item Unknown messages screen not showing item code
	if(csItemCode == _T(""))
	{
		CString csOperatorGuidance = GetHookProperty(_T("OG_Guidance_Prop"));
		int nPos = csOperatorGuidance.Find(_T(":"));
		csItemCode = csOperatorGuidance.Mid(nPos+1, csOperatorGuidance.GetLength());
		csItemCode.TrimLeft();
		csItemCode.TrimRight();
	}
	//End

    CString csMessage = v.bstrVal;
    csMessage += _T("\n\n") + csItemCode;
    ps.Message12(csMessage, true);
	}
    ps.ShowCMFrame();
    return pRetState;    
    /*POS11024: Mobile Shopper - End*/
}

void SMUnknownItem::UnInitialize(void)
{
    SMUnknownItemBase::UnInitialize();
}

bool SMUnknownItem::PSAssistMenuLegalForState()
{
    return SMUnknownItemBase::PSAssistMenuLegalForState();
}

SMStateBase * SMUnknownItem::TimedOut(void)
{
    return SMUnknownItemBase::TimedOut();
}

SMStateBase * SMUnknownItem::PSButtonHelp(void)
{
    return SMUnknownItemBase::PSButtonHelp();
}

SMStateBase * SMUnknownItem::PSButton1(void)
{
    /*POS11024: Mobile Shopper - Start*/
    CString csPOSState(_T(""));
    SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);
    if( csPOSState== _T("MOB_ITEM_UNKNOWN") )
    {
		SendTBCommand(TBCMD_SENDKEYEDDATA, CString(_T("{F2}")));    
    }   
	else if(csPOSState == _T("ITEM_UNKNOWN_MOBILE"))
	{
		SendTBCommand(TBCMD_SENDKEYEDDATA, CString(_T("{F1}")));    
	}
    
    CString strOptionName(_T(""));
    CString strIsResume = _T("0");
    strOptionName = _T("IsResumeTrxInProgress");	 
	SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strOptionName, &strIsResume);
	
	if( strIsResume == _T("1") && !g_bInRecallMode)
    {
		RETURNSTATE(InProgress);    //resume in progress
    }
    else
    {
		return SMUnknownItemBase::PSButton1();    
    }
    /*POS11024: Mobile Shopper - End*/  
}

SMStateBase * SMUnknownItem::PSEnterKey(void)
{
    return SMUnknownItemBase::PSEnterKey();
}
