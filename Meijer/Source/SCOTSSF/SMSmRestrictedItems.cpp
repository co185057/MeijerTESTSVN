//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmRestrictedItems.CPP
//
// TITLE: Class implementation for store mode Restricted Items state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSmRestrictedItems.h"    // MGV this state

#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmRestrictedItems")

IMPLEMENT_DYNCREATE(SMSmRestrictedItems, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmRestrictedItems::SMSmRestrictedItems()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmRestrictedItems::Initialize(void)
{
    //SOTF 6226
    if(SendTBCommand(TBCMD_ACS_DELAYED_AGE_RESTRICTION))
    {
        fStateAssist = false;        
        return SMSmRestrictedItemsBase::PSButton1();
    }
	
	SMStateBase  *pRetState = SMSmRestrictedItemsBase::Initialize();
	
	bStoreModeApprovals = false;
	fStateAssist = true;
	
	CString strPropName = _T("AgeRequiredLeadThru");
    CString strPropValue;
    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strPropName, &strPropValue);
	
	if(strPropValue.Find(_T("SCAN")) == -1 )
	{
		DMScannerDisable();
		m_pAppPSX->SetConfigProperty(_T("LeadthruText"), 
			_T("SmRestrictedItems"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
	}
	else
		DMScannerEnable();
	
	
	strPropName = _T("RestrictedAge_Prop");
    strPropValue;
    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &strPropName, &strPropValue);
	
	//int nIndex = strPropValue.Find(_T("age"));
	//CString csAge = strPropValue.Mid(nIndex + 4, 2);
	
	CString csTitle2;
	csTitle2.Format(_T("Age %s Restricted Item"), strPropValue);
	COleVariant v;
	v = csTitle2;
	
	m_pAppPSX->SetConfigProperty(_T("Title2"), _T("SmRestrictedItems"), UI::PROPERTYTEXTFORMAT, v);
	
	ps.ShowFrame();
	
	return pRetState;
}

void SMSmRestrictedItems::UnInitialize(void)
{
    SMSmRestrictedItemsBase::UnInitialize();
}

SMStateBase * SMSmRestrictedItems::TimedOut(void)
{
    return SMSmRestrictedItemsBase::TimedOut();
}

SMStateBase * SMSmRestrictedItems::PSButton1(void)
{
    fStateAssist = false;
	return SMSmRestrictedItemsBase::PSButton1();
}

SMStateBase * SMSmRestrictedItems::PSButton2(void)
{
    SMStateBase *returnState;
	fStateAssist = false;
    // send No DOB required to POS
    trace(L3, _T("SMSmRestrictedItems::PSButton2 Send No DOB Needed to POS"));
    CString csTest(_T("{F1}"));
    SendTBCommand(TBCMD_SENDKEYEDDATA, csTest);
    returnState = SMSmRestrictedItemsBase::PSButton2();
	delete getRealStatePointer(returnState);	//TAR 376302 delete this pointer to free the memory
	
    //TAR306722, preventing from going into SMSmAuthorization that would cause
    //to neglect the added bagging scale weight    
    SAClearSecurityCondition();
	return setAndCreateAnchorState(BES_BAGANDEAS);
//    return TBEnterItem();
}

SMStateBase * SMSmRestrictedItems::PSButton3(void)
{
    return SMSmRestrictedItemsBase::PSButton3();
}

SMStateBase  *SMSmRestrictedItems::PSButton8(void)
{
	fStateAssist = false;
	
	SendTBCommand(TBCMD_SET_TB_STATE,_T("CANCEL_AGE_RESTRICTED_ITEM"));
	
	lUnapprovedRestrictedItems =0;
	lRegularYellowNeedingApproval = 0;
	bInApprovalNeeded = false; //TAR 279092
	fApprovedForRestrictedItems = true;
	fAgeObtained = true;
	nCustomerAge = 99;
	
	CString csAgeCheck = ps.GetPSText(RA_AGE_RESTRICTED_ID, SCOT_LANGUAGE_PRIMARY);
	csAgeCheck +=  _T("\n") + ps.GetPSText(BTT_PRODUCE /*age check canceled*/, SCOT_LANGUAGE_PRIMARY);
	trace(L7, _T("+ra.OnNormalItem, Message=%s"), csAgeCheck);
	ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csAgeCheck);
	trace(L7, _T("-ra.OnNormalItem"));  
	
	//TAR 196910 start
	while (m_lAgerestrictQueuedtoRAP > 0)
	{
		trace(L7, _T("+ra.OnAgeApproval"));
		ra.OnAgeApproval();
		trace(L7, _T("-ra.OnAgeApproval"));
		m_lAgerestrictQueuedtoRAP--;
	}
	m_lAgerestrictQueuedtoRAP = 0;
	//TAR 196910 end
	
	SAClearSecurityCondition();
	
	return setAndCreateAnchorState(BES_SCANANDBAG);
}

SMStateBase * SMSmRestrictedItems::PSNumericKey(const long lKey)
{
    return SMSmRestrictedItemsBase::PSNumericKey(lKey);
}

SMStateBase * SMSmRestrictedItems::PSEnterKey(void)
{
    return SMSmRestrictedItemsBase::PSEnterKey();
}

SMStateBase * SMSmRestrictedItems::PSClearKey(void)
{
    return SMSmRestrictedItemsBase::PSClearKey();
}

SMStateBase * SMSmRestrictedItems::PSReceiptUp(void)
{
    return SMSmRestrictedItemsBase::PSReceiptUp();
}

SMStateBase * SMSmRestrictedItems::PSReceiptDown(void)
{
    return SMSmRestrictedItemsBase::PSReceiptDown();
}

SMStateBase * SMSmRestrictedItems::OnRAPUnsolicitedDataReceived(LPCTSTR sData)
{
    return SMSmRestrictedItemsBase::OnRAPUnsolicitedDataReceived(sData);
}

SMStateBase *SMSmRestrictedItems::DMScanner(void)
{	
	fStateAssist = false;
	csItemSent = csDMLastBarCode;
	
	lUnapprovedRestrictedItems =0;
	lRegularYellowNeedingApproval = 0;
	bInApprovalNeeded = false; //TAR 279092
	fApprovedForRestrictedItems = true;
	fAgeObtained = true;
	nCustomerAge = 99;
    
	//TAR 196910 start
	while (m_lAgerestrictQueuedtoRAP > 0)
	{
		trace(L7, _T("+ra.OnAgeApproval"));
		ra.OnAgeApproval();
		trace(L7, _T("-ra.OnAgeApproval"));
		m_lAgerestrictQueuedtoRAP--;
	}
	m_lAgerestrictQueuedtoRAP = 0;
	
	SAClearSecurityCondition(); 
	
	return TBItemSale(false,0,true);	
}
SMStateBase * SMSmRestrictedItems::OnWtDecrease()
{
    return SMSmRestrictedItemsBase::OnWtDecrease();
}

SMStateBase * SMSmRestrictedItems::OnWtIncreaseNotAllowed()
{
    return SMSmRestrictedItemsBase::OnWtIncreaseNotAllowed();
}