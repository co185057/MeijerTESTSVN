//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMVoidItem.cpp
//
// TITLE: Class impliementation for void item state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMVoidItem.h"             // MGV this
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("VoidItem")

IMPLEMENT_DYNCREATE(SMVoidItem, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMVoidItem::SMVoidItem()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMVoidItem::Initialize(void)
{
    //making sure POS and FL are synchronize
	
    // POS38686 to make it synchronize, we need to check the initial voidable state first from the POS
    CString strPropName = _T("IsItemNotVoidable");
    CString strPropValue;
    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strPropName, &strPropValue);
	
    if( strPropValue == _T("1"))
	{		
		m_bPrevCancelButtonStatus = false;
	}
	else
	{
        m_bPrevCancelButtonStatus = true;
	} 
    // e POS38686
	
	
	SMStateBase *pState = SMVoidItemBase::Initialize();
	SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(_T("CMReceiptVoid")));
	nTimeOut = 1;
	
	//disallow voiding of item through scanning.
	DMScannerDisable();
	
   return (pState);
}

void SMVoidItem::UnInitialize(void)
{
    SMVoidItemBase::UnInitialize();
}

SMStateBase * SMVoidItem::OnWtIncreaseNotAllowed(void)
{
    return SMVoidItemBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMVoidItem::OnWtDecrease(void)
{
    return SMVoidItemBase::OnWtDecrease();
}

bool SMVoidItem::PSAssistMenuLegalForState()
{
    return SMVoidItemBase::PSAssistMenuLegalForState();
}

SMStateBase * SMVoidItem::OnBackToLGW(void)
{
    return SMVoidItemBase::OnBackToLGW();
}

SMStateBase * SMVoidItem::PSButton1(void)
{
    return SMVoidItemBase::PSButton1();
}

SMStateBase * SMVoidItem::PSButton2(void)
{
    return SMVoidItemBase::PSButton2();
}

SMStateBase * SMVoidItem::PSButton3(void)
{
    return SMVoidItemBase::PSButton3();
}

SMStateBase * SMVoidItem::DMScanner(void)
{
    return SMVoidItemBase::DMScanner();
}

SMStateBase * SMVoidItem::PSCancelKey(void)
{
    return SMVoidItemBase::PSCancelKey();
}

SMStateBase * SMVoidItem::PSReceiptUp(void)
{
    SMStateBase *retState = SMVoidItemBase::PSReceiptUp();
	SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(_T("CMReceiptVoid")));
	nTimeOut = 1;
	return retState;
}

SMStateBase * SMVoidItem::PSReceiptDown(void)
{
	// TAR 342889 start
	CString csItemEntryId;
	COleVariant vValue;
	vValue.Clear();
	vValue.boolVal = VARIANT_FALSE;
	
	// NextGen //
	if (nPSLastWParam == 1111 && nPSLastParm > 0)
	{
		CString csItemId;
		CString* pTemp =(CString *) nPSLastParm;
		if(pTemp != NULL)
		{
			csItemId= *pTemp;
			delete pTemp;
			ps.GetPSXObject()->SetSelectedReceiptItem(m_sCurrentReceipt, csItemId);
			return STATE_NULL;
		}
	}
	
	while (VARIANT_FALSE == vValue.boolVal)
	{
		ps.ScrollDown(m_sCurrentReceipt); // Pre-342889 code
		vValue.Clear();
		CString csTemp = csItemEntryId;
		long rc = ps.GetPSXObject()->GetSelectedReceiptItem(m_sCurrentReceipt, csItemEntryId);
		rc = ps.GetPSXObject()->GetReceiptItemVariable(m_sCurrentReceiptSrc, csItemEntryId, _T("ITEM_SELECTABLE"), vValue);
		if (csTemp == csItemEntryId)
		{
			vValue.boolVal = VARIANT_TRUE; // Force exit from loop
		}
	}
	// TAR 342889 end
	ps.CMButton(_T("CMReceiptScrollDown"), BTT_DOWN, ps.CheckForScrolling(m_sCurrentReceiptSrc, m_sCurrentReceipt, _T("Down")));
	ps.CMButton(_T("CMReceiptScrollUp"), BTT_UP, ps.CheckForScrolling(m_sCurrentReceiptSrc, m_sCurrentReceipt, _T("Up")));    
	  
    //SMStateBase *retState = SMVoidItemBase::PSReceiptDown();
	SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(_T("CMReceiptVoid")));
	nTimeOut = 1;
	return STATE_NULL;
}

SMStateBase * SMVoidItem::PSButtonGoBack(void)
{
    return SMVoidItemBase::PSButtonGoBack();
}

bool SMVoidItem::CheckForScrollUp(long lRcptPos)
{
    return SMVoidItemBase::CheckForScrollUp(lRcptPos);
}

bool SMVoidItem::CheckForScrollDown(long lRcptPos)
{
    return SMVoidItemBase::CheckForScrollDown(lRcptPos);
}

SMStateBase  *SMVoidItem::TimedOut(void)
{
	bool bCancelButtonStatus = true;
	static int nCounts = 0;
	
	// +TAR 358224
	
	PSTEXT Btn1 = PS_NOTEXT;
	
	if (co.IsInAssistMenus())
	{
		Btn1 = BTT_VOIDITEM;
	}
	else
	{
		Btn1 = BTT_CANCELSELECT;
	}
	
	// -TAR 358224
	
	CString strPropName = _T("IsItemNotVoidable");
    CString strPropValue;
    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strPropName, &strPropValue);
    if( strPropValue == _T("1"))
	{
		ps.CMButton(_T("CMButton1Lg"),Btn1, false);
		bCancelButtonStatus = false;
	}
	else
	{
        ps.CMButton(_T("CMButton1Lg"),Btn1, true);
		bCancelButtonStatus = true;
	}
    if(m_bPrevCancelButtonStatus != bCancelButtonStatus || nCounts > 10)
	{
        nTimeOut = 0;
		nCounts = 0;
		m_bPrevCancelButtonStatus = bCancelButtonStatus;
	}
	else nCounts++;
	
	return STATE_NULL;
}
