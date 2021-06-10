//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMProduceFavorites.cpp
//
// TITLE: Class implementation for produce favorites state
//
// TIME:  08/8/00
//
// AUTHOR:    Dale Ehrahrdt
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // common includes

#include "SMState.h"                // base state
#include "SMProduceFavorites.h"           // this state

// +SSCOI-44479
#include "SMPLAEnterWeight.h"          //  enter wgt state 
#include "SMEnterQuantity.h"			
#include "SMKeyInCode.h"			
// -SSCOI-44479

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("ProduceFavorites")


//DLLEXPORT char* PRS_CMKEYBOARD="wCMKeyboard"; // Std KeyBoard
											                   

IMPLEMENT_DYNCREATE(SMProduceFavorites, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////
SMProduceFavorites::SMProduceFavorites()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMProduceFavorites::Initialize(void)
{
    return SMProduceFavoritesBase::Initialize();
}

void SMProduceFavorites::UnInitialize(void)
{
    SMProduceFavoritesBase::UnInitialize();
}

SMStateBase * SMProduceFavorites::OnWtIncreaseNotAllowed(void)
{
    return SMProduceFavoritesBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMProduceFavorites::OnSkipBaggingAllowed(void)
{
// +SSCOI-44479
	g_bNoBaggingRequiredForTheCurrentItem = false;
	if(g_bAllowLateBagging)
	{
		g_bAllowLateBagging = false;
	}
	if (fInMultiSelectPickList && (ps.GetPicklistItemsSold() > 0) && !g_bOnItemOK && !io.d.fVoid && !bKeyInCodeButtonPressed)
	{
		if (VerifyQtyItem(csItemSent))     
		{
			io.Reset();
			
			// Saved for dynamic picklist purposes //
			SMStateBase::ms_bIsCurrentItemFromPickList = true;
			
			io.csOrigCode = csItemSent;	   // item code keyed
			io.csItemCode = csItemSent;	   // assumed to be the same
			io.d.bPickListItem = true;//SR679
			SASendSCOTInput( UPC_MANUAL, csItemSent);
			
            SetNextGenUICurrentItem(io.csOrigCode);
			ps.SetFoundPickListItemsFromSearch(false);
			
			// +SSCOI-44479
			return TBItemSale( FALSE, 0, false, true ); 
			//RETURNSTATE( EnterQuantity ) 	
			// -SSCOI-44479
		}
		
		
		ps.SetFoundPickListItemsFromSearch(false);
		return TBItemSale( FALSE, 0, false, true );			  //Sell item
	}
	else if (bKeyInCodeButtonPressed)
	{
		bKeyInCodeButtonPressed = false;
        RETURNSTATE(KeyInCode)
	}
	else
	{
		return STATE_NULL;
	}
    //return SMProduceFavoritesBase::OnSkipBaggingAllowed();
// -SSCOI-44479
}

SMStateBase * SMProduceFavorites::OnBackToLGW(void)
{
    return SMProduceFavoritesBase::OnBackToLGW();
}

SMStateBase * SMProduceFavorites::PSButton1(void)
{
    return SMProduceFavoritesBase::PSButton1();
}

SMStateBase * SMProduceFavorites::PSButton2(void)
{
    return SMProduceFavoritesBase::PSButton2();
}

SMStateBase * SMProduceFavorites::PSButton3(void)
{
    return SMProduceFavoritesBase::PSButton3();
}

SMStateBase * SMProduceFavorites::PSButton4(void)
{
    return SMProduceFavoritesBase::PSButton4();
}

SMStateBase * SMProduceFavorites::PSListLookup(CString csItemInfo)
{
	if(!co.GetfOperationsAllowMultiPick())
    {
        m_csCategory.Empty();
    }
// +SSCOI-44479
	//g_bMultiPickSkipBaggingAllowed = false;// TAR 398289
	//071904 TAR 273325 BMediano - BEGIN
	//Retrieves information for the selected item.
	//The format is in ItemCode/CategoryId@@Description@@IsSubCategory@@InCategories
	//see PSProceduresBase::DisplaySelectedItem(..) on the formatting.
	csItemSent = csItemInfo.Left( csItemInfo.Find(STR_SEPARATOR));
    
	int nLen = csItemInfo.GetLength()-(csItemSent.GetLength()-_tcsclen(STR_SEPARATOR));
	CString csDescWithRemainder = csItemInfo.Mid(csItemSent.GetLength()+_tcsclen(STR_SEPARATOR),nLen );
	int nIndex = csDescWithRemainder.Find(STR_SEPARATOR);
	CString csDesc, csSubCatWithRemainder, csSubCategory, csInCategory; 
	if (nIndex != -1)
	{
		csDesc = csDescWithRemainder.Left(nIndex);
		csSubCatWithRemainder= csDescWithRemainder.Mid(nIndex + 2);
		csSubCategory = csSubCatWithRemainder.Left(csSubCatWithRemainder.Find(STR_SEPARATOR));
		csInCategory = csSubCatWithRemainder.Mid(csSubCatWithRemainder.Find(STR_SEPARATOR)+_tcsclen(STR_SEPARATOR));
	}

    if (csItemSent.Find(TAB_FAV) != -1)  //bwdFav
    {
        ps.PushLevels(csInCategory, csItemSent);
        ps.DisplaySelectedItem(csItemSent, false, true);
        ps.SetLeadthruText(ps.GetScreenTitle(), _T("")); 
        return STATE_NULL;
    }
#ifndef _CPPUNIT
	else if (!csSubCategory.CompareNoCase(_T("True")))
	{
        //determine the index of the currentcategory through its main category
        long lTabIndex = ps.GetTabSelectedIndexofSubCategory(ps.GetFirstLevelCategoryID(), ps.GetSecondLevelCategoryID());
        CString csIndex;
        csIndex.Format(_T("%d"),lTabIndex + 1);

        //if tab menu are subcategories, set TabSelected correctly
        if(!ps.GetDisplayTabIsMainCategories())
        {
            ps.SetProduceLookupCategoryNumber(csIndex);
        }

		ps.PushLevels(csInCategory, csItemSent);
		
		ps.DisplaySelectedSubItem(csItemSent);

		ps.SetLeadthruText(ps.GetScreenTitle(), _T("")); 
		//ps.Message12(ps.GetInstructions(), true);

		return STATE_NULL;
	}

    //+ Personalization picklist
    CTimeSpan tTimeElapsed = CTime::GetCurrentTime() - m_EnterPicklistTime;
    long lPicklistTimeSpent = tTimeElapsed.GetTotalSeconds();
    CString csPicklistTimeSpent;
    csPicklistTimeSpent.Format(_T("%ld000"), lPicklistTimeSpent);
    CString csPicklistSoldItemMetrics = csPicklistTimeSpent + STR_SEPARATOR + csItemSent + 
       STR_SEPARATOR + csInCategory;
    g_csPicklistItemSoldInfo.Add(csPicklistSoldItemMetrics);
    trace(L7, _T("PicklistSoldItemMetrics = %s"), csPicklistSoldItemMetrics);
    g_bHavePicklist = true;
    g_bHaveSoldFromPicklist = false;
    //- Personalization picklist

	ps.SetItemDesc(csDesc);

	if( csItemSent==_T("") ) //fail in picking list
	{
		DMSayPhrase(PICKLISTINVALID);
		return STATE_NULL;
	}

	//TAR 407200 Move this code here, need to wait for replied message from SM before sending ItemSold to SM
	//SR773 only send Skip Bagging message when the previous item is not bag and not void
	if (fInMultiSelectPickList && (ps.GetPicklistItemsSold() > 0) && !g_bOnItemOK && !io.d.fVoid && !io.d.fCoupon && !g_bMultiPickSkipBaggingAllowed) 
	{
		if(g_bAllowLateBagging)
		{
			CKeyValueParmList parmList;
			CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_CLEAR_ALLOW_LATE_BAGGING);
			_bstr_t bstrParmList((const _TCHAR *)parmList);
			BSTR bstrResult;
			bstrResult = NULL;
			SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
			SysFreeString(bstrResult);
		}
		else
		{
			//SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
			CKeyValueParmList parmList;
			CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_SKIP_BAGGING);
			_bstr_t bstrParmList((const _TCHAR *)parmList);
			BSTR bstrResult;
			bstrResult = NULL;
			SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
			SysFreeString(bstrResult);
			nSAPrevSCOTInputID = 0;
		}

		return STATE_NULL;
	}
	
	g_bMultiPickSkipBaggingAllowed = false; //TAR417235

#endif // _CPPUNIT

	if (VerifyQtyItem(csItemSent))     
	{
		io.Reset();
		
		// Saved for dynamic picklist purposes //
		SMStateBase::ms_bIsCurrentItemFromPickList = true;
		
		io.csOrigCode = csItemSent;	   // item code keyed
		io.csItemCode = csItemSent;	   // assumed to be the same
		io.d.bPickListItem = true;//SR679
        if (!co.IsPicklistAssistEnable()) // dont Send SASendScot when PLA is enable, Let TBItemSale called by PLAEnterWeight do
        {
            SASendSCOTInput( UPC_MANUAL, csItemSent);
        }
        
        SetNextGenUICurrentItem(io.csOrigCode);
		
		ps.SetFoundPickListItemsFromSearch(false);
	
		// +SSCOI-44479
		return TBItemSale( FALSE, 0, false, true ); 
		//RETURNSTATE( EnterQuantity ) 	
		// -SSCOI-44479
	}


	ps.SetFoundPickListItemsFromSearch(false);

    if (co.IsPicklistAssistEnable() && ps.IsPickListItem(csItemSent))
    {
        io.Reset();
        io.csOrigCode = csItemSent;
        io.csItemCode = csItemSent;
        RETURNSTATE(PLAEnterWeight)
    }
    else
    {
        return TBItemSale( FALSE, 0, false, true ); //Sell item
    }
    //return SMProduceFavoritesBase::PSListLookup(csItemInfo);
// -SSCOI-44479
}

SMStateBase * SMProduceFavorites::PSListLookupTab(LPCTSTR lpszTabValue) 
{
    return SMProduceFavoritesBase::PSListLookupTab(lpszTabValue);
}

SMStateBase * SMProduceFavorites::PSListScroll(const long lScroll)
{
    return SMProduceFavoritesBase::PSListScroll(lScroll);
}

SMStateBase * SMProduceFavorites::DMScanner(void)
{
    return SMProduceFavoritesBase::DMScanner();
}

SMStateBase * SMProduceFavorites::PSButtonGoBack(void)
{
    return SMProduceFavoritesBase::PSButtonGoBack();
}

SMStateBase * SMProduceFavorites::TimedOut(void)
{
    return SMProduceFavoritesBase::TimedOut();
}

SMStateBase * SMProduceFavorites::SASecMgrParse( const MessageElement* me)
{
    return SMProduceFavoritesBase::SASecMgrParse(me);
}

SMStateBase * SMProduceFavorites::PSShiftKey(void)
{
    return SMProduceFavoritesBase::PSShiftKey();
}

SMStateBase * SMProduceFavorites::PSHotKey(CString & csHotKey)
{
    return SMProduceFavoritesBase::PSHotKey(csHotKey);
}

void SMProduceFavorites::SetPSContext()
{
    SMProduceFavoritesBase::SetPSContext();
}

SMStateBase  * SMProduceFavorites::PSSpaceKey(void)
{
    return SMProduceFavoritesBase::PSSpaceKey();
}

SMStateBase  * SMProduceFavorites::PSAltGrKey(void)
{
    return SMProduceFavoritesBase::PSAltGrKey();
}

SMStateBase  * SMProduceFavorites::PSBackSpaceKey(void)
{
    return SMProduceFavoritesBase::PSBackSpaceKey();
}

SMStateBase  * SMProduceFavorites::PSClearKey(void)
{
    return SMProduceFavoritesBase::PSClearKey();
}

SMStateBase  * SMProduceFavorites::PSCharKey(TCHAR c)
{
    return SMProduceFavoritesBase::PSCharKey(c);
}

SMStateBase  * SMProduceFavorites::SearchStringFromList(void)
{
    return SMProduceFavoritesBase::SearchStringFromList();
}

void SMProduceFavorites::SetAppStateToSecurity(void)
{
    SMProduceFavoritesBase::SetAppStateToSecurity();
}

void SMProduceFavorites::LoadPersonalizedPicklist(void)
{
    SMProduceFavoritesBase::LoadPersonalizedPicklist();
}

void SMProduceFavorites::UpdateDisplayTextsAndButtons(void)
{
    SMProduceFavoritesBase::UpdateDisplayTextsAndButtons();
}

bool SMProduceFavorites::GetSearchKey(CString &csSearchKey)
{
    return SMProduceFavoritesBase::GetSearchKey(csSearchKey);
}

void SMProduceFavorites::SendSearchString(const CString csSearch)
{
    SMProduceFavoritesBase::SendSearchString(csSearch);
}

bool SMProduceFavorites::CheckSubCategoryDisplay(const CString csTabValue)
{
    return SMProduceFavoritesBase::CheckSubCategoryDisplay(csTabValue);
}

void SMProduceFavorites::SetDisplayToNoTabSelected(const int nButtonCount)
{
    SMProduceFavoritesBase::SetDisplayToNoTabSelected(nButtonCount);
}

void SMProduceFavorites::SetupTabDisplayFromOtherStates(void)
{
    SMProduceFavoritesBase::SetupTabDisplayFromOtherStates();
}

void SMProduceFavorites::ResetNextGenUIPicklistDisplayLevelsData(bool bResetSearch)
{
    SMProduceFavoritesBase::ResetNextGenUIPicklistDisplayLevelsData(bResetSearch);
}

void SMProduceFavorites::SetupTabDisplayFromScanAndBag(BEST prevAnchorState, bool bIsFromQuickPick)
{
    SMProduceFavoritesBase::SetupTabDisplayFromScanAndBag(prevAnchorState, bIsFromQuickPick);
}

void SMProduceFavorites::UpdateTextsAndButtonsAfterTabSetup(bool bIsFromQuickPick, BEST prevAnchorState)
{
    SMProduceFavoritesBase::UpdateTextsAndButtonsAfterTabSetup(bIsFromQuickPick,prevAnchorState);
}
