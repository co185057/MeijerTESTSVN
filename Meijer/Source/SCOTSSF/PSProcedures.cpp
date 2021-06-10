    //////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // FILE:  PSProcedures.CPP
    //
    // TITLE:   Display Presentation Services Interface Procedures (High Level)
    //
    // This module handles all DPS calls for the SCOT application so that no other
    // component need know the details of DPS calls and all other components are
    // insulated from changes to the DPS API.
    //
    // From here we call the PSInterface routines
    //
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    #include <stdafx.h>                 // MS MFC always needed first
    //               // MS MFC template header
	  
    #include "PSProcedures.h"
#include "transbrokerssf.h"

static int nNumLinkedItems = 0; //tar 233768
								// Start CRD 125377
bool PSProcedures::bSkipFrameDisplay = false;
// end CRD 125377

#include "LaneStateData.h"

//UploadTicket Port +
#include "MPProcedures.h"
#include "DllDefine.h"
DLLIMPORT extern MPProcedures mp;
//UploadTicket Port -


// local defines
#define COMP_ID   ID_PS
#define T_ID      _T("PSProcedures")



  ////////////////////////////////////////////////
PSProcedures::PSProcedures()
{
  
}
#ifdef _TESTSSF
#define COMP_ID ID_PS				// Base state component
#define T_ID	_T("PSProcedures")
///////////////////////////////////////////////////////////////////////////////
// Function:    ShowCardAmountList
// Description: layout the UI of Shopping Card Amount button lists
///////////////////////////////////////////////////////////////////////////////////
void PSProcedures::ShowCardAmountList()
{
	//+set the CashBackList buttonlist to 11 buttons
	long nButtonCount = 11;
    COleVariant vReturn, v = (long) nButtonCount ;
    m_pPSX->SetConfigProperty( _T("CashBackList"), _T("SelectPayment"), UI::PROPERTYBUTTONCOUNT, v );
	m_pPSX->SetControlProperty(_T("CashBackList"), UI::PROPERTYBUTTONCOUNT, v );

	CString csImage, csImagePath, csTemp;
	GET_PARTITION_PATH(SCOTIMAGE, csImagePath.GetBuffer(_MAX_PATH));
    COleSafeArray  saTextArray, saImageArray, saItemCodeArray;
	//saTextArray is for the text for each button of the buttonlist
    saTextArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
	//saImageArray is for the image for each button of the button list
    saImageArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
	//saItemCodeArray is for the code for each button of the button list.
	//the code will be used by PSListLookup as a parameter.
    saItemCodeArray.Create(VT_VARIANT, 1, (DWORD*)&nButtonCount);

	// setup an array that will be used as text and code for each button of the button lists
	int nAmt[] = {5, 10, 15, 20, 25, 30, 35, 40, 50, 100, 250};

	// setup an array that will be used as image for each button of the button lists
	CStringArray aImage;
	csImage.Format(_T("%s%s"), csImagePath, _T("\\10x7_USD_five.jpg"));
	aImage.Add(csImage);
	csImage.Format(_T("%s%s"), csImagePath, _T("\\10x7_USD_ten.jpg"));
	aImage.Add(csImage);
	csImage.Format(_T("%s%s"), csImagePath, _T("\\10x7_USD_fifteen.jpg"));
	aImage.Add(csImage);
	csImage.Format(_T("%s%s"), csImagePath, _T("\\10x7_USD_twenty.jpg"));
	aImage.Add(csImage);
	csImage.Format(_T("%s%s"), csImagePath, _T("\\10x7_USD_twentyfive.jpg"));
	aImage.Add(csImage);
	csImage.Format(_T("%s%s"), csImagePath, _T("\\10x7_USD_thirty.jpg"));
	aImage.Add(csImage);
	csImage.Format(_T("%s%s"), csImagePath, _T("\\10x7_USD_thirtyfive.jpg"));
	aImage.Add(csImage);
	csImage.Format(_T("%s%s"), csImagePath, _T("\\10x7_USD_forty.jpg"));
	aImage.Add(csImage);
	csImage.Format(_T("%s%s"), csImagePath, _T("\\10x7_USD_fifty.jpg"));
	aImage.Add(csImage);
	aImage.Add(_T(""));
	aImage.Add(_T(""));
	for (int nIndex = 0;  nIndex  < 11; nIndex++)
	{
		csTemp.Format(_T("$%d\n"), nAmt[nIndex]);
		PutToSafeArray(nIndex, saTextArray, csTemp);
		PutToSafeArray(nIndex, saImageArray, aImage[nIndex]);				
		v = (long)nAmt[nIndex];
		saItemCodeArray.PutElement((long *)&nIndex, v );	
	}

	//tell psx to layout CashBacklist
	v = saTextArray;
    m_pPSX->SendCommand(_T("CashBackList"), UI::COMMANDSETBUTTONTEXTLIST, vReturn, 1, v );
    v = saImageArray;
    m_pPSX->SendCommand( _T("CashBackList"), UI::COMMANDSETBUTTONLOGOLIST, vReturn, 1, v );
    v = saItemCodeArray;
    m_pPSX->SendCommand( _T("CashBackList"), UI::COMMANDSETBUTTONDATALIST, vReturn, 1, v );
}

///////////////////////////////////////////////////////////////////////////////
// Function:    ShowGiftCardActivityList
// Description: layout the UI of Gift Card Activity lists
///////////////////////////////////////////////////////////////////////////////////
void PSProcedures::ShowGiftCardActivityList()
{
	//+set the TenderImage buttonlist to 3 buttons
	long nButtonCount = 3;
	int nIndex = 0;
	COleVariant vReturn, v = (long) nButtonCount ;
	m_pPSX->SetConfigProperty( _T("TenderImage"), _T("Finish"), UI::PROPERTYBUTTONCOUNT, v );
	m_pPSX->SetControlProperty(_T("TenderImage"), UI::PROPERTYBUTTONCOUNT, v );
	//-set the TenderImage buttonlist to 3 buttons
	CString csImage, csImagePath;
	GET_PARTITION_PATH(SCOTIMAGE, csImagePath.GetBuffer(_MAX_PATH));
	csImage.Format(_T("%s%s"), csImagePath ,_T("\\10x7_0413giftcard.jpg"));
	COleSafeArray  saTextArray, saImageArray, saItemCodeArray;
	//saTextArray is for the text for each button of the buttonlist
	saTextArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);  
	//saImageArray is for the image for each button of the button list
	saImageArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
	//saItemCodeArray is for the code for each button of the button list.
	//the code will be used by PSListLookup as a parameter.
	saItemCodeArray.Create(VT_VARIANT, 1, (DWORD*)&nButtonCount);

	//Setup Purchase New Shopping Card button
	PutToSafeArray(nIndex, saTextArray, _T("Purchase New Shopping Card"));
	PutToSafeArray(nIndex, saImageArray, csImage);
	v = (long) (nIndex+1);
	saItemCodeArray.PutElement((long *)&nIndex, v );

	//Setup Add Money Shopping Card button
	nIndex++;
	PutToSafeArray(nIndex, saTextArray, _T("Add Money Shopping Card"));
	PutToSafeArray(nIndex, saImageArray, csImage);
	v = (long) (nIndex+1);
	saItemCodeArray.PutElement((long *)&nIndex, v );

	//Setup Shopping Card Balance
	nIndex++;
	PutToSafeArray(nIndex, saTextArray, _T("Shopping Card Balance"));
	PutToSafeArray(nIndex, saImageArray, csImage);
	v = (long) (nIndex+1);
	saItemCodeArray.PutElement((long *)&nIndex, v );

	//tell psx to layout the TenderImage buttonlist
	v = saTextArray;
	m_pPSX->SendCommand(_T("TenderImage"), UI::COMMANDSETBUTTONTEXTLIST, vReturn, 1, v );
	v = saImageArray;
	m_pPSX->SendCommand( _T("TenderImage"), UI::COMMANDSETBUTTONLOGOLIST, vReturn, 1, v );
	v = saItemCodeArray;
	m_pPSX->SendCommand( _T("TenderImage"), UI::COMMANDSETBUTTONDATALIST, vReturn, 1, v );

}
///////////////////////////////////////////////////////////////////////////////
// Function:    PutToSafeArray
// Description: Put values to safe array
// Parameters : [in] int nIndex - index to safe array
//				[in] COleSafeArray& saArray - the safe array 
//				[in]  CString csString - value to put in the safe array 
		
///////////////////////////////////////////////////////////////////////////////////
void PSProcedures::PutToSafeArray(int nIndex, COleSafeArray& saArray, CString csString)
{
	USES_CONVERSION;
#ifndef UNICODE
        saArray.PutElement((long*)&nIndex, SysAllocString( T2OLE( csString ) ) );
#else
        saArray.PutElement((long*)&nIndex, SysAllocString(  csString ) );
#endif
}


#endif

void PSProcedures::SetUSSFFrame(LPCTSTR lpszContext)
{
    PSProceduresBase::SetUSSFFrame(lpszContext);
    return;
}
//************************************************************************ 
//
// Function:		DisplaySelectedItem
// Purpose:			This function assigns the text, image, and values for 
//                  ProductImage ButtonList control.
// Parameters:      [in] csFirstchars - contains the first characters of 
//                                      selected items. This value only used to 
//                                      display the ProductImage.
//										May also be Favorites or CategoryN where N is a
//										number from 1-8.
//                  [in] bSelectByItemDescription - to select item by item description or not
// Returns:	        The number of selected items
// Calls:
// Side Effects:
// Notes:           The characters in csFirstChars are defined as a range such 
//                  as AB1(would show all the produce buttons starting with the 
//                  letter 'A' and 'B') in the XML file. A numeric value from 1 to 9
//                  tab range identifies what tab it is (i.e 1st, 2nd). This number is
//                  used to display a produce item whose text doesn't start with a letter
//                  (i.e. 4x8x16 Gray Solid Cap Block ). For 3.1 implementation,
//                  these items are displayed when 1st tab is selected.
//                  For Favorites in the XML file the tab's value must be specified
//                  as Favorites.
//					For Categories, csFirstChars must be CategoryN where N is a number from 1-8.
//
//************************************************************************
int PSProcedures::DisplaySelectedItem(CString csFirstchars, bool bSelectByItemDescription, bool bFavoritesButton)
{
#ifndef _CPPUNIT // TODO: make UT for AssignTareList()
	if (m_csCurrentContext == _T("SelectTare"))
	{
		AssignTareList();
		return 0;
	}

	// if m_csCurrentContext is not *LookUpItemXXXX or *ProduceFavoritesXXXX context
	if (!((m_csCurrentContext.Find(_T("LookUpItem")) != -1) || (m_csCurrentContext.Find(_T("ProduceFavorites")) != -1)))
	{
		trace(L6, _T("PSProceduresBase::DisplaySelectedItem do not handle DisplaySelectedItem() on %s context"), m_csCurrentContext);
		return 0;
	}

#endif // _CPPUNIT
	COleVariant v;
	m_strLastProduceLookupList = csFirstchars;

	bool bSortedList = co.GetfOperationsPicklistSortedAlphabetical();
	CSItemList *pItemList = NULL;
	if (!bSortedList && bSelectByItemDescription)
	{
		pItemList = &(m_Search[m_languageUsed]);
	}
	else
	{
		pItemList = &(ItemList[m_languageUsed]);
	}

	//+ Personalization picklist
	CString csPickListPref = custInfo.GetPreference(_T("PicklistFavorites"));
	CSItemList *pPopularItemList;


	if (co.IsPicklistAssistEnable() && !m_PicklistAssistItemList[m_languageUsed].IsEmpty() && !bFavoritesButton)
	{
		pPopularItemList = &(m_PicklistAssistItemList[m_languageUsed]);
	}
	else if (csPickListPref.GetLength() && !m_PersonalizedPopularItemList[m_languageUsed].IsEmpty()
		&& (_ttoi(custInfo.GetOption(_T("PersonalizedPicklistItems"))) > 0))
	{
		pPopularItemList = &(m_PersonalizedPopularItemList[m_languageUsed]);
	}
	else
	{
		pPopularItemList = &(m_StorePopularItemList[m_languageUsed]);
	}
	//- Personalization picklist

	DWORD nButtonCount = pItemList->GetCount();
	if (!nButtonCount)
	{
		v = 0L;
		m_pPSX->SetControlProperty(s_szProduceButtonListName, UI::PROPERTYBUTTONCOUNT, v);
		//EnableProduceScrollButtons();
		return 0;
	}

	//#ifndef _CPPUNIT // TODO: extract this code to another method.
	CPSXUserInputLock userInputLock(_T("PSProceduresBase::DisplaySelectedItem"));  //TAR 371980 fix

	CStringArray csSubCatDescList;
	CStringArray csItemDescList;
	CStringArray csItemCodeTextList;
	CStringArray csImageList;
	CStringArray csItemDataList;
	CStringArray csCatDescList;
	CArray<ITEMTYPE, ITEMTYPE> nItemTypeList;

	CItemInfo *pItem = NULL;
	POSITION pos = NULL;
	nButtonCount = 0;
	CString csCombineItemInfo = _T("");
	CString csCat(_T(""));


	int nTabIndex(-1);
	CString csAlphabetic, csPCategory;
	bool bFavoriteOn = FindCategory(TAB_FAV, csAlphabetic, csPCategory, nTabIndex);
	if (bSelectByItemDescription)
	{
		//Set locale before comparing strings
#ifndef _UNICODE
		char* localeBackup = strdup(setlocale(LC_CTYPE, NULL));
#else
		wchar_t* localeBackup = wcsdup(_wsetlocale(LC_CTYPE, NULL));
#endif
		SetLocale(GetLanguage());

		csFirstchars.MakeLower();
		trace(L6, _T("Searching picklist by item description for string %s"), csFirstchars);

		m_csScreenTitle = GetPSText(MSG_SEARCHFROMLISTTITLE);
		if (!co.GetfOperationsAllowMultiPick())
		{
			m_csInstructions = GetPSText(MSG_SEARCHFROMLISTINSTRUCTION);
		}
		else
		{
			m_csInstructions = GetPSText(MSG_SEARCHFROMLISTINSTRUCTION2);
		}

		CSortedArray<CString, CString> arrItemCode;
		pos = pItemList->GetHeadPosition();

		/******************************************************/
		//check to see if Search string has special characters
		//check it here instead in the while loops below.
		bool searchStringHasSpecialChar = false;

		for (int i = 0; i < csFirstchars.GetLength(); i++)
		{
			if (csFirstchars.GetAt(i) > 192)
			{
				searchStringHasSpecialChar = true;
			}
			//trace(L6, _T("searchString[%d] =  %d , Length = %d"), i, searchString.GetAt(i), searchString.GetLength());
		}
		/******************************************************/


		while (pos != NULL)
		{
			pItem = (CItemInfo*)pItemList->GetNext(pos);

			if (pItem->m_bIsSubCategory ||
				(arrItemCode.GetSize()>0 && (arrItemCode.FindBinary(pItem->GetItemCode(), 0, arrItemCode.GetSize() - 1) == TRUE)))
				continue;

			//+RFQ 20160107-8379    include Sorting string in product search
			CString strName = pItem->GetItemName();
			strName += _T(" ") + pItem->GetReadingName();
			//-RFQ 20160107-8379

			if (strName != _T(""))
			{
				strName.MakeLower();

				/******************************************************************/
				// SSCOADK-2496 - determines if Search string as "space" char*
				//If the search string has space but does not have special char. 
				//then check if the search string is part of Item Name (strName). 
				if (csFirstchars.Find(_T(' ')) > 0 && !searchStringHasSpecialChar)
				{
					if (strName.Find(csFirstchars) > -1)
					{
						ps.GetPicklistItemInfo(pItem, csItemDescList, csSubCatDescList, csItemCodeTextList, csImageList,
							csItemDataList, csCatDescList, nItemTypeList, s_szProduceButtonListName, csCat);

						nButtonCount++;

						arrItemCode.InsertBinary(pItem->GetItemCode(), FALSE);

					}
					//continue to next element in picklist.xml
					continue;
				}
				/***************************************************************************/

				TCHAR strTemp[255];
				_tcscpy(strTemp, strName);

				TCHAR * strSeps = _T(" ");
				TCHAR * strToken = _tcstok(strTemp, strSeps);

				while (strToken != NULL)
				{
					CString csToken = strToken;

					//SSCOADK-2496
					//Only continue if search string does not have "space" char.
					if (csFirstchars.Find(_T(" ")) < 0)
					{
						if ((csToken.GetLength() < csFirstchars.GetLength()))
						{
							strToken = _tcstok(NULL, strSeps);
							continue;
						}
					}

					csToken = csToken.Left(csFirstchars.GetLength());

					bool bIsMatch = true;
					if (co.GetfOperationsAllowFuzzySearch())
					{
						for (int i = 0; i<csToken.GetLength(); i++)
						{
							CString csFuzzyChars;
							GetFuzzyChars(csFirstchars.GetAt(i), csFuzzyChars);

							//trace(L6, _T("Description is %s, Token is %s, char is %c, FuzzyChars is %s"), strName, csToken, csToken.GetAt(i), csFuzzyChars);
							if (csFuzzyChars.Find(csToken.GetAt(i))<0)
							{
								bIsMatch = false;
								break; //breaks for-loop
							}
						}
					}
					else
					{
						if (csToken.Left(csFirstchars.GetLength()).Compare(csFirstchars) != 0)
							bIsMatch = false;
					}

					if (bIsMatch)
					{
						ps.GetPicklistItemInfo(pItem, csItemDescList, csSubCatDescList, csItemCodeTextList, csImageList,
							csItemDataList, csCatDescList, nItemTypeList, s_szProduceButtonListName, csCat);

						nButtonCount++;

						arrItemCode.InsertBinary(pItem->GetItemCode(), FALSE);

						break; //breaks while loop
					}
					strToken = _tcstok(NULL, strSeps);
				}//while         
			}//if
		}//while

		arrItemCode.RemoveAll();

		//Set locale after comparing strings
#ifndef _UNICODE
		setlocale(LC_CTYPE, localeBackup);
#else
		_wsetlocale(LC_CTYPE, localeBackup);
#endif
		free(localeBackup);

	}
	else
	{
		CString csCategories(_T(""));
		CString csCategory(_T(""));
		if (csFirstchars == TAB_FAV)
		{
			SetCategoryTitles(csFirstchars);
			CString csCatAlphabetic;
			bool bFind = FindCategory(csFirstchars, csCatAlphabetic, csCategory, nTabIndex);
			pos = pPopularItemList->GetHeadPosition();
			while (pos != NULL)
			{
				pItem = (CItemInfo*)pPopularItemList->GetNext(pos);

				ps.GetPicklistItemInfo(pItem, csItemDescList, csSubCatDescList, csItemCodeTextList, csImageList,
					csItemDataList, csCatDescList, nItemTypeList, s_szProduceButtonListName, csFirstchars);
				nButtonCount++;
			}
		}
		else if (csFirstchars.Left(_tcslen(TAB_CATEGORY)) == TAB_CATEGORY)
		{
			csCategory = csFirstchars.Right(1);
			SetCategoryTitles(csCategory);
			CString csReturnCategory(_T(""));
			CString csCategoryID = _T("");
			nTabIndex = -1;
			bool bFind = FindCategory(csCategory, csReturnCategory, csCategoryID, nTabIndex);
			pos = pItemList->GetHeadPosition();
			while (pos != NULL)
			{
				pItem = (CItemInfo*)pItemList->GetNext(pos);

				csCat = _T("");
				csCategories = pItem->GetCategories();
				if (!csCategories.IsEmpty())
				{
					csCat = GetCategory(csCategories, csCategory);
				}

				if (csCat == csCategory)
				{
					ps.GetPicklistItemInfo(pItem, csItemDescList, csSubCatDescList, csItemCodeTextList, csImageList,
						csItemDataList, csCatDescList, nItemTypeList, s_szProduceButtonListName, csCat);
					nButtonCount++;
				}
			}
		}
		else
		{
			// Alphabetic category //
			CString csCatAlphabetic;
			SetCategoryTitles(csFirstchars);
			bool bFind = FindCategory(csFirstchars, csCatAlphabetic, csCategory, nTabIndex);

			int n = csFirstchars.Find(_T('#'));
			BOOL bIncludeNumeric = (-1 != n);
			if (n > -1)
			{
				csFirstchars = csFirstchars.Left(n);
			}

			TCHAR cFirst = 0, cLast = 0;
			csFirstchars.MakeUpper();
			if (csFirstchars.GetLength())
			{
				cFirst = csFirstchars.GetAt(0);
				cLast = csFirstchars.GetLength() >= 2 ? csFirstchars.GetAt(1) : csFirstchars.GetAt(0);
			}

			CString strName(_T(""));
			pos = pItemList->GetHeadPosition();

			while (pos != NULL)
			{
				pItem = (CItemInfo*)pItemList->GetNext(pos);

				//+RFQ 20160107-8379 - only return pItem->GetItemName if sorting string not used for category inclusion
				//strName = pItem->GetReadingName().GetLength() ? pItem->GetReadingName() : pItem->GetItemName();
				strName = pItem->GetItemName();
				//-RFQ 20160107-8379
				strName.MakeUpper();

				if (strName.GetLength()) //tar 259310
					if ((bIncludeNumeric && _istdigit(strName.GetAt(0))) ||
						(_istalpha(strName.GetAt(0)) && (strName.GetAt(0) >= cFirst && strName.GetAt(0) <= cLast)))
					{
						csCat = _T("");
						csCategories = pItem->GetCategories();
						if (!csCategories.IsEmpty())
						{
							csCat = GetCategory(csCategories, csCategory);
						}
						if (((pItem->GetSubCategories()).IsEmpty() || !csCat.IsEmpty()) &&  //skip items belong to subcategory and <Categories> not explicitly configured
							!(pItem->GetIsSubCategory() && csCat.IsEmpty())) //skip subcategories not belong to the category
						{
							ps.GetPicklistItemInfo(pItem, csItemDescList, csSubCatDescList, csItemCodeTextList, csImageList,
								csItemDataList, csCatDescList, nItemTypeList, s_szProduceButtonListName, csCat);
							nButtonCount++;
						}
						else
							trace(L7, _T("Skip subcategories and items belong to subcategories."));
					}
			}
		}

		if (csCategory == TAB_FAV)
		{
			m_ProduceLookupCategoryNumber = csCategory;
		}
		else
		{
			nTabIndex = nTabIndex + 1;
			m_ProduceLookupCategoryNumber.Format(_T("%d"), nTabIndex);
		}

		CString csLevels;
		CString csBaseLevel = GetCategoryName(csCategory);
		csLevels.Format(_T("TabSelected=%s;CurrentCategory=%s;"), m_ProduceLookupCategoryNumber, csBaseLevel);
		CLaneStateData laneStateData(m_pPSX);
		laneStateData.SendNextGenUIData(_T("NextGenUIPicklistDisplayLevels"), csLevels);
	}


	v = (long)nButtonCount;

	CPSXRedrawLock csRedrawLock(s_szProduceButtonListName, _T("PSProceduresBase::DisplaySelectedItem"));
	m_pPSX->SetControlProperty(s_szProduceButtonListName, UI::PROPERTYBUTTONCOUNT, v);

	ps.SetPickListButtonProperties(csItemDescList, csSubCatDescList, csItemCodeTextList,
		csImageList, csItemDataList, csCatDescList, nItemTypeList,
		s_szProduceButtonListName, nButtonCount);

	//EnableProduceScrollButtons();	

	//#endif // _CPPUNIT
	return nButtonCount;

}


SCOT_LANGUAGE_TYPE PSProcedures::GetLanguage(void)
{
    return PSProceduresBase::GetLanguage();
}

CString PSProcedures::GetPSText(const int nMsg)
{
    return PSProceduresBase::GetPSText(nMsg);
}

CString PSProcedures::GetPSText(const int nMsg, SCOT_LANGUAGE_TYPE language)
{
    return PSProceduresBase::GetPSText(nMsg, language);
}

CString PSProcedures::GetPSText(const CString csMsgID, SCOT_LANGUAGE_TYPE language)
{
    return PSProceduresBase::GetPSText(csMsgID, language);
}

long PSProcedures::LoadPicklistAssistItemList(const CString& csPLAItemList)
{
    return PSProceduresBase::LoadPicklistAssistItemList(csPLAItemList);
}

void PSProcedures::SetLanguage(SCOT_LANGUAGE_TYPE language)
{
    PSProceduresBase::SetLanguage(language);
}

void PSProcedures::SetCategoryTitles(const CString& csCategory)
{
    PSProceduresBase::SetCategoryTitles(csCategory);
}

CString PSProcedures::GetCategory(const CString& csCategories, const CString& csCategory)
{
    return PSProceduresBase::GetCategory(csCategories, csCategory);
}

bool PSProcedures::FindCategory(const CString& csCategory, CString& csRetCategory, CString& csCategoryId, int& nTabIndex )
{
    return PSProceduresBase::FindCategory( csCategory, csRetCategory, csCategoryId, nTabIndex );
}

void PSProcedures::ClearPicklistAssistItemList(void)
{
    PSProceduresBase::ClearPicklistAssistItemList();
}

bool PSProcedures::IsPicklistAssistItemListEmpty(void)
{
    return PSProceduresBase::IsPicklistAssistItemListEmpty();
}

bool PSProcedures::IsPickListItem(const CString& csItemCode)
{
    return PSProceduresBase::IsPickListItem(csItemCode);
}

bool PSProcedures::IsInXM()
{
    return PSProceduresBase::IsInXM();
}

void PSProcedures::SetInXM(bool bInXM)
{
    PSProceduresBase::SetInXM(bInXM);
}

void PSProcedures::EchoAmountPaidToReceipt(int nTenderType, long lAmountPaid)
{
	// SSCOI-46635: Commented out below code. It only add a new receipt line with the Tender Description and Amount.
	// This is already handled in ACSTB.
	//PSProceduresBase::EchoAmountPaidToReceipt(nTenderType, lAmountPaid);
}

void PSProcedures::PushLevels(const CString& csPreviousLevel, const CString& csCurrentLevel)
{
    PSProceduresBase::PushLevels(csPreviousLevel, csCurrentLevel);
}

void PSProcedures::PopLevels(CString& csPreviousLevel, CString& csCurrentLevel)
{
    PSProceduresBase::PopLevels(csPreviousLevel, csCurrentLevel);
}

void PSProcedures::DeleteAllLevels(void)
{
    PSProceduresBase::DeleteAllLevels();
}

bool PSProcedures::IsLevelEmpty(void)
{
    return PSProceduresBase::IsLevelEmpty();
}

bool PSProcedures::CanScroll(const long lScrol)
{
    return PSProceduresBase::CanScroll(lScrol);
}

CString PSProcedures::GetScreenTitle()
{
    return PSProceduresBase::GetScreenTitle();
}

CString PSProcedures::GetInstructions()
{
    return PSProceduresBase::GetInstructions();
}

void PSProcedures::DisplayNextCategoryItem(const long lScroll)
{
    PSProceduresBase::DisplayNextCategoryItem(lScroll);
}

//************************************************************************ 
//
// Function:		DisplaySelectedSubItem
// Purpose:			Display picklist items under the selected subcategory
// Parameters:		[in] csSubCategory - selected subcategory
//						  					 
// Returns:			
// Calls:			GetPicklistItemInfo 
//					SetPickListButtonProperties 
// Side Effects:
// Notes:
//
//************************************************************************
void PSProcedures::DisplaySelectedSubItem(CString csSubCategory)
{
	trace(L7, _T("+PSProceduresBase::DisplaySelectedSubItem, csSubCategory=%s"), csSubCategory);

	CSItemList *pItemList = &(ItemList[m_languageUsed]);
	
	COleVariant v, vReturn;
	DWORD nButtonCount = pItemList->GetCount();
	if( !nButtonCount )
	{
		v = 0L;
		m_pPSX->SetControlProperty( s_szProduceButtonListName, UI::PROPERTYBUTTONCOUNT, v );
		EnableProduceScrollButtons();
		return;
	}

	CPSXUserInputLock userInputLock(_T("PSProceduresBase::DisplaySelectedSubItem"));  //TAR 371980 fix ;
   CStringArray csSubCatDescList;
	CStringArray csItemDescList;
	CStringArray csItemCodeTextList;
	CStringArray csImageList;
	CStringArray csItemDataList;
	CStringArray csCatDescList;
	CArray<ITEMTYPE,ITEMTYPE> nItemTypeList;
	
	CItemInfo *pItem = NULL;
	POSITION pos = NULL;
	nButtonCount = 0;
	CString csCategories(_T(""));	
	CString strName( _T("") );

	CString csCombineItemInfo = _T("");  
	SetCategoryTitles(csSubCategory);
	
	pos = pItemList->GetHeadPosition();
	while( pos != NULL )
	{
		pItem = (CItemInfo*)pItemList->GetNext( pos );

        //+RFQ 20160107-8379 - only return pItem->GetItemName if sorting string not used for category inclusion
        //strName = pItem->GetReadingName().GetLength() ? pItem->GetReadingName() : pItem->GetItemName();
        strName = pItem->GetItemName(); 
        //-RFQ 20160107-8379
        strName.MakeUpper();

		CString csCat(_T(""));
		CString csSubCat(_T(""));
		CString csItemSubCat(_T(""));
		//Get subcategory for subcategory
		csCategories = pItem->GetCategories();
		if ( !csCategories.IsEmpty() )
		{
			csSubCat = GetCategory(csCategories, csSubCategory);
			csCat = csSubCat;
		}
		//Get subcategory for item
		csCategories = pItem->GetSubCategories();
		if ( !csCategories.IsEmpty() )
		{
			csItemSubCat = GetCategory(csCategories, csSubCategory);
		}	

		//Get items and subcategories belong to the same subcategory 
		if ( !csSubCat.IsEmpty() || !csItemSubCat.IsEmpty() )
		{
			GetPicklistItemInfo( pItem, csItemDescList, csSubCatDescList, csItemCodeTextList, csImageList, 
					             csItemDataList, csCatDescList, nItemTypeList, s_szProduceButtonListName, csCat );
			nButtonCount++;	
		}
	}
	
	v = (long)nButtonCount;

   CPSXRedrawLock csRedrawLock( s_szProduceButtonListName, _T("PSProceduresBase::DisplaySelectedSubItem"));
	m_pPSX->SetControlProperty( s_szProduceButtonListName, UI::PROPERTYBUTTONCOUNT, v );

	SetPickListButtonProperties(csItemDescList, csSubCatDescList, csItemCodeTextList, 
							 csImageList, csItemDataList, csCatDescList, nItemTypeList,
							 s_szProduceButtonListName, nButtonCount);  

	trace(L7, _T("-PSProceduresBase::DisplaySelectedSubItem"));
}

//************************************************************************ 
//
// Function:		DisplayQuickPickItem
// Purpose:			Display picklist items/categories on Scan & Bag
// Parameters:		
//						  					 
// Returns:			
// Calls:			GetPicklistItemInfo - get picklist item information
//					SetPickListButtonProperties - set picklist button properties for display
// Side Effects:
// Notes:
//
//************************************************************************
void PSProcedures::DisplayQuickPickItem()
{
	trace(L7, _T("+PSProceduresBase::DisplayQuickPickItem"));

	//CSItemList *pItemList = &(QuickPickItemList[m_languageUsed]);

	//+ Dynamic Quickpick 
    CString csPickListPref = custInfo.GetPreference(_T("PicklistFavorites"));
    CSItemList *pItemList;

    if ( (_ttoi(custInfo.GetOption(_T("PersonalizedPicklistItems"))) > 0) && 
             (_ttoi(custInfo.GetOption(_T("DynamicQuickPickItems"))) > 0) )
	{            
       if ( csPickListPref.GetLength()  && !m_PersonalizedQuickPickItemList[m_languageUsed].IsEmpty() )        
        {
            pItemList = &(m_PersonalizedQuickPickItemList[m_languageUsed]);
        }
        else
        {
            pItemList = &(QuickPickItemList[m_languageUsed]);
        }
	}
	else
	{
       pItemList = &(QuickPickItemList[m_languageUsed]);
	}

    //- Dynamic Quickpick 


	COleVariant v, vReturn;
	DWORD nButtonCount = pItemList->GetCount();
	if( !nButtonCount )
	{
		v = 0L;
		m_pPSX->SetControlProperty( s_szQuickPickButtonListName, UI::PROPERTYBUTTONCOUNT, v );
		return;
	}

   CPSXUserInputLock userInputLock(_T("PSProceduresBase::DisplayQuickPickItem"));  //TAR 371980 fix 

	CStringArray csSubCatDescList;
	CStringArray csItemDescList;
	CStringArray csItemCodeTextList;
	CStringArray csCatDescList;
	CStringArray csImageList;
	CStringArray csItemDataList;
	CArray<ITEMTYPE,ITEMTYPE> nItemTypeList;
	
	CItemInfo *pItem = NULL;
	POSITION pos = NULL;
	nButtonCount = 0;
	CString csName;
	CString csSubControl(_T(""));

	CString csCombineItemInfo; 
	
	pos = pItemList->GetHeadPosition();
	while( pos != NULL )
	{
		pItem = (CItemInfo*)pItemList->GetNext( pos );

        //+RFQ 20160107-8379 - only return pItem->GetItemName if sorting string not used for category inclusion
        //csName = pItem->GetReadingName().GetLength() ? pItem->GetReadingName() : pItem->GetItemName();
        csName = pItem->GetItemName(); 
        //-RFQ 20160107-8379

        csName.MakeUpper();

		GetPicklistItemInfo( pItem, csItemDescList, csSubCatDescList, csItemCodeTextList, csImageList, 
					                 csItemDataList, csCatDescList, nItemTypeList, s_szQuickPickButtonListName );

		nButtonCount++;	
	}
	
	v = (long)nButtonCount;

	CPSXRedrawLock csRedrawLock( s_szQuickPickButtonListName, _T("PSProceduresBase::DisplayQuickPickItem"));
   m_pPSX->SetControlProperty( s_szQuickPickButtonListName, UI::PROPERTYBUTTONCOUNT, v );

	SetPickListButtonProperties(csItemDescList, csSubCatDescList, csItemCodeTextList, 
							 csImageList, csItemDataList, csCatDescList, nItemTypeList,
							 s_szQuickPickButtonListName, nButtonCount);


	trace(L7, _T("-PSProceduresBase::DisplayQuickPickItem"));
}

CString PSProcedures::GetLastProduceLookupList()
{
    return PSProceduresBase::GetLastProduceLookupList();
}

void PSProcedures::SetLastProduceLookupList(CString sCategory)
{
    PSProceduresBase::SetLastProduceLookupList(sCategory);
}

long PSProcedures::GetPicklistItemsSold()
{
    return PSProceduresBase::GetPicklistItemsSold();
}

void PSProcedures::SetPicklistItemsSold(long lTotalPicklistItemsSold)
{
    PSProceduresBase::SetPicklistItemsSold(lTotalPicklistItemsSold);
}

long PSProcedures::GetPicklistItemsIncludeVoidItems()
{
    return PSProceduresBase::GetPicklistItemsIncludeVoidItems();
}

void PSProcedures::SetPicklistItemsIncludeVoidItems(long lTotalPicklistItemsIncludeVoidItems)
{
    PSProceduresBase::SetPicklistItemsIncludeVoidItems( lTotalPicklistItemsIncludeVoidItems);
}

void PSProcedures::GetPicklistItemInfo( CItemInfo *pItem, CStringArray& csItemDescList, 
                                    CStringArray& csSubCatDescList, 
                                    CStringArray& csItemCodeTextList, 
                                    CStringArray& csImageList, 
                                    CStringArray& csItemDataList,
                                    CStringArray& csCatDescList,
                                    CArray<ITEMTYPE, ITEMTYPE>& nItemTypeList,
                                    const CString& csControl,
                                    const CString& csCat)
{
    PSProceduresBase::GetPicklistItemInfo( pItem, csItemDescList, 
                                     csSubCatDescList, 
                                     csItemCodeTextList, 
                                     csImageList, 
                                     csItemDataList,
                                     csCatDescList,
                                     nItemTypeList,
                                     csControl,
                                     csCat);
}

void PSProcedures::SetPickListButtonProperties( CStringArray& csItemDescList, 
                                         CStringArray& csSubCatDescList, 
                                         CStringArray& csItemCodeTextList, 
                                         CStringArray& csImageList, 
                                         CStringArray& csItemDataList,
                                         CStringArray& csCatDescList,
                                         CArray<ITEMTYPE, ITEMTYPE>& nItemTypeList,
                                         const CString& csControl,
                                         int nButtonCount)
{
    PSProceduresBase::SetPickListButtonProperties(  csItemDescList, 
                                         csSubCatDescList, 
                                         csItemCodeTextList, 
                                         csImageList, 
                                         csItemDataList,
                                         csCatDescList,
                                         nItemTypeList,
                                         csControl,
                                         nButtonCount);
}

void PSProcedures::GetParentCategories(const CString& csCategories, CStringArray& csParents)
{
    PSProceduresBase::GetParentCategories(csCategories, csParents);
}

BOOL PSProcedures::DisconnectFromServer(BOOL bSendDisconnectMsg)
{
    return PSProceduresBase::DisconnectFromServer(bSendDisconnectMsg);
}

BOOL PSProcedures::ConnectToServer(int iFocus, int iDisplay)
{
    return PSProceduresBase::ConnectToServer(iFocus, iDisplay);
}

long PSProcedures::LoadProdList(const CString& csXML_List)
{
    return PSProceduresBase::LoadProdList(csXML_List);
}

void PSProcedures::SetDevErrURL(LPCTSTR lpszURL)
{
    PSProceduresBase::SetDevErrURL(lpszURL);
}

long PSProcedures::ShowTrainingModeOnScreen(bool bShowTrainingMode)
{
    return PSProceduresBase::ShowTrainingModeOnScreen(bShowTrainingMode);
}

long PSProcedures::InitializeConfigData(LPCTSTR szPlstName, LPCTSTR szTareName)
{
    return PSProceduresBase::InitializeConfigData(szPlstName, szTareName);
}

long PSProcedures::Initialize(
                              LPCTSTR szBmpsName,
                              LPCTSTR szTextName,
                              LPCTSTR szPlstName,
                              LPCTSTR szTareName,
                              const int nTare1,
                              const int nTare2,
                              const bool fMetric,
                              const bool fLivVid)
{
    return PSProceduresBase::Initialize(
        szBmpsName,
        szTextName,
        szPlstName,
        szTareName,
        nTare1,
        nTare2,
        fMetric,
        fLivVid);
}

long PSProcedures::UnInitialize(void)
{
    return PSProceduresBase::UnInitialize();
}

long PSProcedures::UnInitializeConfigData(void)
{
    return PSProceduresBase::UnInitializeConfigData();
}

long PSProcedures::SetKeyLabel(LPCTSTR sControlName, int nKey, LPCTSTR szDesc)
{
    return PSProceduresBase::SetKeyLabel(sControlName, nKey, szDesc);
}

long PSProcedures::DisableAssistModeKey( LPCTSTR sControlName, const int nKey)
{
    return PSProceduresBase::DisableAssistModeKey(sControlName, nKey);
}

long PSProcedures::Button(LPCTSTR szCtlName,LPCTSTR szContent, const bool fEnable)
{
    return PSProceduresBase::Button(szCtlName,szContent, fEnable);
}

long PSProcedures::Button(LPCTSTR szCtlName,const PSTEXT nText, const bool fEnable)
{
    return PSProceduresBase::Button(szCtlName, nText, fEnable);
}

long PSProcedures::ButtonFlash(LPCTSTR szCtlName, const bool fFlash)
{
    return PSProceduresBase::ButtonFlash(szCtlName, fFlash);
}

long PSProcedures::ButtonState(LPCTSTR szCtlName, const bool fEnable, const bool fShow)
{
    return PSProceduresBase::ButtonState(szCtlName, fEnable, fShow);
}

long PSProcedures::ButtonEnable(LPCTSTR szCtlName, const bool fEnable)
{
    return PSProceduresBase::ButtonEnable(szCtlName, fEnable);
}

void PSProcedures::HideCMWindows()
{
    PSProceduresBase::HideCMWindows();
}

void PSProcedures::HideCMButtons()
{
    PSProceduresBase::HideCMButtons();
}

void PSProcedures::SetDisplayWMFlag(bool bNeedDisplayWM)
{
    PSProceduresBase::SetDisplayWMFlag(bNeedDisplayWM);
}

void PSProcedures::SetFrame(LPCTSTR lpszContext)
{
    PSProceduresBase::SetFrame(lpszContext);
}

void PSProcedures::SetCMFrame(LPCTSTR lpszContext)
{
    PSProceduresBase::SetCMFrame( lpszContext);
}

void PSProcedures::SetCurrentFrame(LPCTSTR lpszContext)
{
    PSProceduresBase::SetCurrentFrame( lpszContext);
}

void PSProcedures::BuildControlsList()
{
    PSProceduresBase::BuildControlsList();
}

void PSProcedures::SetBackground(const BMPS nBmp)
{
    PSProceduresBase::SetBackground(nBmp);
}

long PSProcedures::SetLeadthruText(const PSTEXT nText1, const PSTEXT nText2 )
{
    return PSProceduresBase::SetLeadthruText( nText1, nText2 );
}

long PSProcedures::SetLeadthruText(LPCTSTR szText1,LPCTSTR szText2)
{
    return PSProceduresBase::SetLeadthruText(szText1,szText2 );
}

long PSProcedures::SetLeadthruText(const PSTEXT nText1,LPCTSTR szText2)
{
    return PSProceduresBase::SetLeadthruText( nText1, szText2);
}

long PSProcedures::SetLTTinto2(LPCTSTR szLTText)
{
    return PSProceduresBase::SetLTTinto2(szLTText);
}

long PSProcedures::SetLTTinto2(const PSTEXT nLTText)
{
    return PSProceduresBase::SetLTTinto2(nLTText);
}

long PSProcedures::SetTitle(LPCTSTR szText1,LPCTSTR szText2 )
{
    return PSProceduresBase::SetTitle(szText1,szText2 );
}

long PSProcedures::SetTitle(const PSTEXT nText1, const PSTEXT nText2)
{
    return PSProceduresBase::SetTitle(nText1, nText2);
}

void PSProcedures::SetButtonText(LPCTSTR szCtlName, const PSTEXT nButton)
{
    PSProceduresBase::SetButtonText(szCtlName, nButton);
}

void PSProcedures::SetAttractScreenMessage(LPCTSTR szText)
{
    PSProceduresBase::SetAttractScreenMessage(szText);
}

void PSProcedures::SetAlphaNumericMessage(LPCTSTR szText)
{
    PSProceduresBase::SetAlphaNumericMessage(szText);
}

void PSProcedures::PopulateHKSensitiveControls(CString csCtxName, HKStatus hkStatus, bool bShiftOn, bool bAltGrOn, bool bSymbolOn)
{
    PSProceduresBase::PopulateHKSensitiveControls(csCtxName, hkStatus, bShiftOn, bAltGrOn, bSymbolOn);
}

void PSProcedures::PopulateHotKeys(CString csCtxName)
{
    PSProceduresBase::PopulateHotKeys(csCtxName);
}
void PSProcedures::SetShiftAndSymbolState(const CString csContextName, const bool bSymbolOn)
{
	PSProceduresBase::SetShiftAndSymbolState(csContextName, bSymbolOn);
}
void PSProcedures::SetCMGobackButtonText(const PSTEXT nText)
{
    PSProceduresBase::SetCMGobackButtonText(nText);
}

void PSProcedures::RefreshCMTotal()
{
    PSProceduresBase::RefreshCMTotal();
}

void PSProcedures::ShowCMTotal(const bool fTotal, const bool fReTot)
{
	COleVariant v;

	CString csGMTot("0");
	PSSendTBCommand(TBCMD_GMTOTAL, &csGMTot);
	CString csGMTotal = FmtDollar(_ttol(csGMTot));

	m_pPSX->SetConfigProperty(_T("CMTotalGMAmount"), _T(""), UI::PROPERTYVISIBLE, VARIANT_TRUE);
	m_pPSX->SetConfigProperty(_T("CMTotalGMText"), _T(""), UI::PROPERTYVISIBLE, VARIANT_TRUE);

	v = csGMTotal;
	m_pPSX->SetTransactionVariable(_T("CMTotalGMAmount"), v);

	CString csTMDisc("0");
	PSSendTBCommand(TBCMD_TMDISCOUNT, &csTMDisc);
	if(SMState::bTMDVoided) //if -ve, TMD has been voided. Ignore the discount amount
		csTMDisc = _T("0");
	CString csTMDiscount = FmtDollar(_ttol(csTMDisc));

	m_pPSX->SetConfigProperty(_T("CMTotalTMDiscountAmount"), _T(""), UI::PROPERTYVISIBLE, VARIANT_TRUE);
	m_pPSX->SetConfigProperty(_T("CMTotalTMDiscountText"), _T(""), UI::PROPERTYVISIBLE, VARIANT_TRUE);

	v = csTMDiscount;
	m_pPSX->SetTransactionVariable(_T("CMTotalTMDiscountAmount"), v);

	CString strPropName = _T("IsCM_TMDPresent");
	CString strPropValue = GetHookProperty(strPropName);

	if (strPropValue != _T("1"))
		strPropName = _T("CMDiscount_Total");
	else
		strPropName = _T("CMDiscount_Total_TM");

	strPropValue = GetHookProperty(strPropName);

	strPropValue.TrimLeft();
	strPropValue.TrimRight();
	v = FmtDollar(_ttol(strPropValue));

	m_pPSX->SetTransactionVariable(_T("CMTotalCMDiscountAmount"), v);

    PSProceduresBase::ShowCMTotal(fTotal, fReTot);
}

void PSProcedures::ShowCMWeightNumber(CString expectedWt, CString observedWt, long numItems)
{
    PSProceduresBase::ShowCMWeightNumber( expectedWt,  observedWt,  numItems);
}

void PSProcedures::HideCMWeightNumber()
{
    PSProceduresBase::HideCMWeightNumber();
}

void PSProcedures::ShowCMSmallReceipt()
{
    PSProceduresBase::ShowCMSmallReceipt();
}

void PSProcedures::ShowCMTBText(LPCTSTR csMessage)
{
	CString strOptionName = _T("ShowCMTBText");
	CString strShowCMTBText;
	PSSendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &strShowCMTBText);
	if (strShowCMTBText == _T("Y"))
	{
		PSProceduresBase::ShowCMTBText(csMessage);
	}
}

void PSProcedures::ShowSMTBText(LPCTSTR csMessage)
{
	CString strOptionName = _T("ShowSMTBText");
	CString strShowSMTBText;
	PSSendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &strShowSMTBText);
	if (strShowSMTBText == _T("Y"))
	{
		PSProceduresBase::ShowSMTBText(csMessage);
	}
}

void PSProcedures::ShowAmountDueAmountPaid(const long lDue, const long lPaid)
{
    PSProceduresBase::ShowAmountDueAmountPaid( lDue, lPaid);
}

void PSProcedures::ShowChangeDueAmountPaid(const long lChange, const long lPaid, const bool bChangeDueText)
{
	PSProceduresBase::ShowChangeDueAmountPaid(lChange, lPaid, bChangeDueText);
}

void PSProcedures::ShowAmountRefundedAmountPaid(const long lRefund, const long lPaid)
{
    PSProceduresBase::ShowAmountRefundedAmountPaid(lRefund, lPaid);
}

void PSProcedures::UpdateAmountDueAmountPaid(const long lDue, const long lPaid)
{
    PSProceduresBase::UpdateAmountDueAmountPaid(lDue, lPaid);
}

void PSProcedures::ShowTotalSavings(const long lTotalTrxSavings)
{
    PSProceduresBase::ShowTotalSavings(lTotalTrxSavings);
}

void PSProcedures::ShowTotalRewardPoints(const long lTotalTrxPoints)
{
    PSProceduresBase::ShowTotalRewardPoints(lTotalTrxPoints);
}

void PSProcedures::ShowCustomerRewardMessage(LPCTSTR pCustomerMessage)
{
    PSProceduresBase::ShowCustomerRewardMessage(pCustomerMessage);
}

void PSProcedures::ShowCMRewardInfoBox(const long lTotalTrxSavings, const long lTotalTrxPoints,
                                LPCTSTR pCustomerMessage)
{
    PSProceduresBase::ShowCMRewardInfoBox(lTotalTrxSavings , lTotalTrxPoints, pCustomerMessage);
}

void PSProcedures::ShowSavingsORRewards(bool bShowSavings, long lValue)
{
    PSProceduresBase::ShowSavingsORRewards(bShowSavings, lValue);
}

void PSProcedures::ShowSMTotal(const bool fTotal)
{
    PSProceduresBase::ShowSMTotal(fTotal);

	// TAR 360340+
	if (fTotal)
	{
		COleVariant v = lTaxes;
		m_pPSX->SetTransactionVariable(_T("SMTaxAmount"), v);
	}
	// TAR 360340-

}

long PSProcedures::ShowFrame( bool bHelpState)
{
	// start CRD 125377 don't display thank you if transaction canceled
	if ((m_bPsxConfigLoaded) && (SkipFrameDisplay()))
	{
        trace( L6, _T("PSProcedures::ShowFrame - CurrentContext %s do NOT display frame"), m_csCurrentContext );
		return 0;
	}
    else
    {
		return PSProceduresBase::ShowFrame( bHelpState);
	}
	// end CRD 125377
}

void PSProcedures::SetCMMascotImage(const BMPS nBmp)
{
    PSProceduresBase::SetCMMascotImage(nBmp);
}

CString PSProcedures::GetMascotBmpName(const int nMsg)
{
    return PSProceduresBase::GetMascotBmpName(nMsg);
}

CString PSProcedures::GetSoundName(const int nMsg)
{
    return PSProceduresBase::GetSoundName(nMsg);
}

void PSProcedures::SetCMGobackButtonVisible(bool visible)
{
    PSProceduresBase::SetCMGobackButtonVisible(visible);
}

CString PSProcedures::Background(void)
{
    return PSProceduresBase::Background();
}

void PSProcedures::ScanReceiptForExceptionCategorys(CStringArray &csArray)
{
    PSProceduresBase::ScanReceiptForExceptionCategorys(csArray);
}

bool PSProcedures::ScanReceiptForSecurityTagItems(void)
{
    return PSProceduresBase::ScanReceiptForSecurityTagItems();
}

long PSProcedures::ItemsInFavoritesList(void)
{
    return PSProceduresBase::ItemsInFavoritesList();
}

void PSProcedures::ReadPImgFiles(CStringArray *pcsaCode,CStringArray *pcsaBmpName,CStringArray *pcsaOverrideDesc)
{
    PSProceduresBase::ReadPImgFiles(pcsaCode,pcsaBmpName,pcsaOverrideDesc);
}

void PSProcedures::GetNewDescriptions( const CString& strOriginalDescription,
                                      const CString& strOriginalReadingName,
                                      CStringArray& strNewDescriptsArray,
                                      CStringArray& strNewReadingNamesArray )
{
    PSProceduresBase::GetNewDescriptions( strOriginalDescription, 
        strOriginalReadingName, 
        strNewDescriptsArray, 
        strNewReadingNamesArray );
}

void PSProcedures::GetDeviceSpecificBMPFileNames(void)
{
    PSProceduresBase::GetDeviceSpecificBMPFileNames();
}

void PSProcedures::CheckIfBMPFileExists(CString *pcsFileName)
{
    PSProceduresBase::CheckIfBMPFileExists(pcsFileName);
}

bool PSProcedures::SetDynamicBackground(CString *pcsFileName)
{
    return PSProceduresBase::SetDynamicBackground(pcsFileName);
}

CString PSProcedures::FmtDollar(const long lCents, SCREEN_POSITION SPosition, FORMAT_OPTION bReporting)
{
    return PSProceduresBase::FmtDollar(lCents, SPosition, bReporting);
}

CString PSProcedures::FmtWeight(const long lWgt)
{
    return PSProceduresBase::FmtWeight(lWgt);
}

CString PSProcedures::FmtWeightInfo()
{
    return PSProceduresBase::FmtWeightInfo();
}

CString PSProcedures::FmtQuantityInfo()
{
    return PSProceduresBase::FmtQuantityInfo();
}

long PSProcedures::PromptAmount(long lAmt)
{
    return PSProceduresBase::PromptAmount(lAmt);
}

long PSProcedures::Prompt(PSTEXT nText)
{
    return PSProceduresBase::Prompt(nText);
}

long PSProcedures::Prompt(LPCTSTR szText)
{
    return PSProceduresBase::Prompt(szText);
}

long PSProcedures::Echo(PSTEXT nText)
{
    return PSProceduresBase::Echo(nText);
}

long PSProcedures::Echo(LPCTSTR szText, LPCTSTR szText2)
{
    return PSProceduresBase::Echo(szText, szText2);
}

long PSProcedures::Echo(short echoLineNumber, LPCTSTR szText)
{
    return PSProceduresBase::Echo(echoLineNumber, szText);
}

long PSProcedures::HideEcho()
{
    return PSProceduresBase::HideEcho();
}

long PSProcedures::InfoMessage(LPCTSTR szText)
{
    return PSProceduresBase::InfoMessage(szText);
}

long PSProcedures::Message12(PSTEXT nText, bool bIsModal)
{
    return PSProceduresBase::Message12(nText, bIsModal);
}

long PSProcedures::Message12(LPCTSTR szText, bool bIsModal)
{
    return PSProceduresBase::Message12(szText, bIsModal);
}

long PSProcedures::Message12(LPCTSTR szText)
{
    return PSProcedures::Message12(szText, false);
}

long PSProcedures::Message12Modal(LPCTSTR szText)
{
    return PSProceduresBase::Message12Modal(szText);
}

long PSProcedures::Message13(PSTEXT nText)
{
    return PSProceduresBase::Message13(nText);
}

long PSProcedures::Message13(LPCTSTR szText)
{
    return PSProceduresBase::Message13(szText);
}

long PSProcedures::Message23(const PSTEXT nText, bool bIsModal)
{
    return PSProceduresBase::Message23(nText, bIsModal);
}

long PSProcedures::Message23(LPCTSTR szText, bool bIsModal)
{
    return PSProceduresBase::Message23(szText, bIsModal);
}

long PSProcedures::Message24(const PSTEXT nText, bool bIsModal)
{
    return PSProceduresBase::Message24(nText, bIsModal);
}

long PSProcedures::Message24(LPCTSTR szText, bool bIsModal)
{
    return PSProcedures::Message24(szText, bIsModal);
}

long PSProcedures::Message2(const PSTEXT nText)
{
    return PSProceduresBase::Message2(nText);
}

long PSProcedures::Message2(LPCTSTR szText)
{
    return PSProceduresBase::Message2(szText);
}

long PSProcedures::Message3(const PSTEXT nText)
{
    return PSProceduresBase::Message3(nText);
}

long PSProcedures::Message3(LPCTSTR szText)
{
    return PSProceduresBase::Message3(szText);
}

long PSProcedures::Weight(const long lWeight)
{
    return PSProceduresBase::Weight(lWeight);
}

long PSProcedures::Total(const long lTot)
{
    return PSProceduresBase::Total(lTot);
}

long PSProcedures::Tax(const long lTax)
{
    return PSProceduresBase::Tax(lTax);
}

long PSProcedures::EchoInput(const _TCHAR cDigit,
                             bool bAllowAlpha,
                             bool bAsteriskMask)
{
    return PSProceduresBase::EchoInput( cDigit,
        bAllowAlpha,
        bAsteriskMask);
}

long PSProcedures::EchoDecimalInput(const _TCHAR cDigit, bool bAllowAlpha)
{
    return PSProceduresBase::EchoDecimalInput(cDigit, bAllowAlpha);
}

void PSProcedures::PSHideKeyboard()
{
    PSProceduresBase::PSHideKeyboard();
}

void PSProcedures::PSHideNumericKeypad()
{
    PSProceduresBase::PSHideNumericKeypad();
}

void PSProcedures::PSHideSMMNumericKeypad()
{
    PSProceduresBase::PSHideSMMNumericKeypad();
}

void PSProcedures::PSBackSpaceKbd()
{
    PSProceduresBase::PSBackSpaceKbd();
}

void PSProcedures::EnterClearKeys(const bool disableEnter,
                              const bool disableClear)
{
    PSProceduresBase::EnterClearKeys(disableEnter, disableClear);
}

long PSProcedures::AllowInput(const long lMax,
                              const bool fHide,
                              const bool fDollar,
                              const long lMin)
{
    return PSProceduresBase::AllowInput(lMax, fHide, fDollar, lMin);
}

long PSProcedures::ClearInput(void)
{
    return PSProceduresBase::ClearInput();
}

long PSProcedures::BackSpaceInput(void)
{
    return PSProceduresBase::BackSpaceInput();
}

long PSProcedures::ScrollUp(const CString& csReceipt)
{
    return PSProceduresBase::ScrollUp(csReceipt);
}

long PSProcedures::ScrollDown(const CString& csReceipt)
{
    return PSProceduresBase::ScrollDown(csReceipt);
}

long PSProcedures::HideSalesReceipt(void)
{
    return PSProceduresBase::HideSalesReceipt();
}

long PSProcedures::HideButtons(void)
{
    return PSProceduresBase::HideButtons();
}

long PSProcedures::DisableButtons(void)
{
    return PSProceduresBase::DisableButtons();
}

long PSProcedures::DisableSelectionList(void)
{
    return PSProceduresBase::DisableSelectionList();
}

long PSProcedures::HideReceiptButtons(void)
{
    return PSProceduresBase::HideReceiptButtons();
}

long PSProcedures::DisableReceiptButtons(void)
{
    return PSProceduresBase::DisableReceiptButtons();
}

long PSProcedures::DelayMessage12(const PSTEXT nMsg)
{
    return PSProceduresBase::DelayMessage12(nMsg);
}

bool PSProcedures::ShowDelayedMessages(void)
{
    return PSProceduresBase::ShowWideList();
}

long PSProcedures::ShowWideList(void)
{
    return PSProceduresBase::ShowWideList();
}

long PSProcedures::ShowKeyboard(void)
{
    return PSProceduresBase::ShowKeyboard();
}

long PSProcedures::ShowAlphaNumericKeyboard(void)
{
    return PSProceduresBase::ShowAlphaNumericKeyboard();
}

void PSProcedures::PSHideAlphaNumericKeypad()
{
    PSProceduresBase::PSHideAlphaNumericKeypad();
    return;
}

long PSProcedures::ShowNumericKeypad(void)
{
    return PSProceduresBase::ShowNumericKeypad();
}

long PSProcedures::ShowSMMNumericKeypad(const bool hideDecimal)
{
    return PSProceduresBase::ShowSMMNumericKeypad(hideDecimal);
}

long PSProcedures::ShowTareList(void)
{
    return PSProceduresBase::ShowTareList();
}

long PSProcedures::ShowVideoLarge(void)
{
    return PSProceduresBase::ShowVideoLarge();
}

long PSProcedures::ShowVideoSmall(void)
{
    return PSProceduresBase::ShowVideoSmall();
}

long PSProcedures::ShowVideoProduce(void)
{
    return PSProceduresBase::ShowVideoProduce();
}

long PSProcedures::HideVideoLarge(void)
{
    return PSProceduresBase::HideVideoLarge();
}

long PSProcedures::HideVideoSmall(void)
{
    return PSProceduresBase::HideVideoSmall();
}

long PSProcedures::HideVideoProduce(void)
{
    return PSProceduresBase::HideVideoProduce();
}

long PSProcedures::ShowCashBackList(const long lDue, int nBack[], int numBack, bool bTenderEbt)
{
    return PSProceduresBase::ShowCashBackList(lDue, nBack, numBack, bTenderEbt);
}

bool PSProcedures::SalesReceiptEmpty(void)
{
    return PSProceduresBase::SalesReceiptEmpty();
}

long PSProcedures::GetItem(CString &csDesc,
                           CString &csCode,
                           long    &lQuantity,
                           long    &lWeight,
                           long    &lPrice,
                           long    &lUPrice,
                           long    &lTareCode,
                           long    &lBarcodeType,
                           const   CString& csReceiptSrc,
                           const   CString& csReceipt,
                           const   CString& csItemEntryId)
{
    return PSProceduresBase::GetItem(csDesc,
        csCode,
        lQuantity,
        lWeight,
        lPrice,
        lUPrice,
        lTareCode,
        lBarcodeType,
        csReceiptSrc,
        csReceipt,
        csItemEntryId);
}

int PSProcedures::GetCashList(CString &csDesc)
{
    return PSProceduresBase::GetCashList(csDesc);
}

long PSProcedures::GetTareList(CString &csDesc)
{
    return PSProceduresBase::GetTareList(csDesc);
}

long PSProcedures::GetTareAt(const long lIndex)
{
    return PSProceduresBase::GetTareAt(lIndex);
}

int PSProcedures::GetWideList(CString &csCode)
{
    return PSProceduresBase::GetWideList(csCode);
}

long PSProcedures::GetInput(CString &csInput)
{
    return PSProceduresBase::GetInput(csInput);
}

CString PSProcedures::GetBmpName(const int nBmp)
{
    return PSProceduresBase::GetBmpName(nBmp);
}

long PSProcedures::SetPSText(const WORKMSGS nMsg,LPCTSTR szText)
{
    return PSProceduresBase::SetPSText(nMsg,szText);
}

void PSProcedures::SetRemoteLanguage(const CString &csISOLanguageCode)
{
    PSProceduresBase::SetRemoteLanguage(csISOLanguageCode);
}

void PSProcedures::SetNextGenUICustomerLanguage(const CString &csISOLanguageCode)
{
    PSProceduresBase::SetNextGenUICustomerLanguage(csISOLanguageCode);
}

void PSProcedures::SetNextGenUICustomerLanguage(SCOT_LANGUAGE_TYPE lang)
{
    PSProceduresBase::SetNextGenUICustomerLanguage(lang);
}

long PSProcedures::AddItem(void)
{
    return PSProceduresBase::AddItem();
}

long PSProcedures::AddMessageToReceipt(const CString& csReceiptSrc, CString text)
{
	LPCTSTR szVoidMsg = _T("VOIDMSG");
	CString csMsgEntryId = _T("");
	CString strPropName = _T("");
	CString strValue = _T("");
	bool bVoidMsg = false;

	// are we voiding a selectable message (e.g. TMD & GUEST cards)?
	int iPos = text.Find(szVoidMsg);
	if (-1 != iPos)
	{
		io.fVoid = true;
		bVoidMsg = true;
		csMsgEntryId = text;
		csMsgEntryId.Delete(iPos, _tcslen(szVoidMsg));
		csMsgEntryId = _T("MSG") + csMsgEntryId;
	}

	// if voiding a selectable msg you need to use the 
	//    id of the original msg
	if (io.fVoid && (bVoidMsg || (csReceiptSrc != _T("CMSmallReceipt"))))
	{
		if (!bVoidMsg)
		{
			csMsgEntryId = io.d.csItemID;
		}
	}
	else if (!io.fVoid && GetHookProperty(_T("PropertyAllowMSGEntryID")) == _T("1"))
	{
		// this message was provided with an entry ID, use it.
		strValue = GetHookProperty(_T("TMDMemberEntryID"));
		if (strValue == _T(""))
		{
			strValue = GetHookProperty(_T("NSC4MemberEntryID"));
		}
		// increment the message ID counter to its next highest value 
		long lEntryID = _ttol(strValue);
		if (lEntryID >= m_lMessageEntryId)
		{
			m_lMessageEntryId = lEntryID + 1;
		}

		csMsgEntryId = _T("MSG") + strValue;
		m_pPSX->CreateReceiptItem(csReceiptSrc, csMsgEntryId);
	}
	else
	{
		csMsgEntryId.Format(_T("%d"), m_lMessageEntryId++);
		csMsgEntryId = _T("MSG") + csMsgEntryId;
		m_pPSX->CreateReceiptItem(csReceiptSrc, csMsgEntryId);
	}

	COleVariant vValue;

	// don't set the text if we're voiding the current message
	if (!bVoidMsg)
	{
		vValue = text;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csMsgEntryId, _T("ITEM_MSG"), vValue);
	}

	if (csReceiptSrc != _T("CMSmallReceipt"))
	{
		vValue = VARIANT_FALSE;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csMsgEntryId, _T("ITEM_SELECTABLE"), vValue);
	}

	// Allow msg to be voidable if set in applicationmodel.xml
	if ((PSSendTBCommand(TBCMD_MSGVOIDABLE)) &&
		//(text.Find(_T("ACCT#")) < 0)         &&  -> TMD CARDS ARE VOIDABLE
		(!io.fVoid) &&
		(csReceiptSrc != _T("CMSmallReceipt")))
	{
		SetHookProperty(_T("PropertyAllowMSGVoid"), _T("0"));

		vValue = text;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csMsgEntryId, _T("ITEM_DESCRIPTION"), vValue);


		CString cstest = io.csItemCode;
		if (io.csItemCode.GetLength() == 0)
			cstest = csMsgEntryId;
		vValue = cstest;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csMsgEntryId, _T("ITEM_CODE"), vValue);

		strValue = GetHookProperty(_T("PropertyAllowMSGVoidsBTYPE"));
		long lValue = _ttol(strValue);
		long ltest = (long)io.cScanType;
		if (ltest == 0)
			ltest = lValue;

		vValue = ltest;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csMsgEntryId, _T("ITEM_SCAN_TYPE"), vValue);

		vValue = VARIANT_TRUE;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csMsgEntryId, _T("ITEM_SELECTABLE"), vValue);
	}

	if (io.fVoid && csReceiptSrc != _T("CMSmallReceipt"))
	{
		//check if this is TMD
		m_pPSX->GetReceiptItemVariable(csReceiptSrc, csMsgEntryId, _T("ITEM_MSG"), vValue);
		CString csMessage = OLE2T(vValue.bstrVal);
		if(csMessage.Find(_T("Team Member")) >= 0 )
		{
			SMState::bTMDVoided = true;
		}
		vValue = VARIANT_TRUE;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csMsgEntryId, _T("ITEM_VOID"), vValue);
		vValue = VARIANT_FALSE;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csMsgEntryId, _T("ITEM_SELECTABLE"), vValue);
	}


	TraceItemVariables(csReceiptSrc, csMsgEntryId);
	m_pPSX->UpdateReceiptControls(csReceiptSrc);  //update receipt display

	ps.ScrollDown(_T("AssistReceipt"));

	// SOTF 9269 - Since MessageReceipt is not selectable, then we should tell the POS to highlight the last available valid item.
	PSSendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(_T("AssistReceipt")));

	return 0;
}

long PSProcedures::ScanReceiptToVoid(const CString& csReceiptSrc, CString& csItemEntryId, int nIndex)
{
    return PSProceduresBase::ScanReceiptToVoid(csReceiptSrc, csItemEntryId, nIndex );
}

int PSProcedures::FormatMessageToReceipt(CString &text, bool bIsSmallRcpt, int nLines)
{
    return PSProceduresBase::FormatMessageToReceipt(text, bIsSmallRcpt, nLines);
}

long PSProcedures::ResetTransactionLists(void)
{
    PSSendTBCommand(TBCMD_RESET_TMDISCOUNT_GMTOTAL, NULL);
    return PSProceduresBase::ResetTransactionLists();
}

long PSProcedures::AddSecurityLog(LPCTSTR szText)
{
    return PSProceduresBase::AddSecurityLog(szText);
}

long PSProcedures::AddMediaList(CString text)
{
    return PSProceduresBase::AddMediaList( text);
}

long PSProcedures::UpdateMediaList(const long index, CString text)
{
    return PSProceduresBase::UpdateMediaList(index, text);
}

long PSProcedures::AddDetailsList(CString text)
{
    return PSProceduresBase::AddDetailsList(text);
}

long PSProcedures::ResetMediaList()
{
    return PSProceduresBase::ResetMediaList();
}

long PSProcedures::ResetDetailsList()
{
    return PSProceduresBase::ResetDetailsList();
}

long PSProcedures::ResetAMTareList()
{
    return PSProceduresBase::ResetAMTareList();
}

long PSProcedures::AddSMMItemList(CString text)
{
    return PSProceduresBase::AddSMMItemList(text);
}

long PSProcedures::ResetSMMItemList()
{
    return PSProceduresBase::ResetSMMItemList();
}

long PSProcedures::ClearSMMItemList()
{
    return PSProceduresBase::ClearSMMItemList();
}

long PSProcedures::AddDelaySecurityList(CString text)
{
    return PSProceduresBase::AddDelaySecurityList(text);
}

long PSProcedures::ResetDelaySecurityList()
{
    return PSProceduresBase::ResetDelaySecurityList();
}

long PSProcedures::ClearDelaySecurityList()
{
    return PSProceduresBase::ClearDelaySecurityList();
}

long PSProcedures::GetCoupon(long& lCouponIndex,
                             CString& csDesc, 
                             CString& csCode, 
                             long& lQty, 
                             long& lWgt, 
                             long& lPrc, 
                             long &lUPrc, 
                             long& lTcd, 
                             long& lBarcodeType)
{
    return PSProceduresBase::GetCoupon(lCouponIndex, csDesc, csCode, lQty, lWgt, lPrc, lUPrc, lTcd, lBarcodeType);
}

long PSProcedures::GetVisualItem(long& lVisualItemIndex, 
                                 CString& csDesc, 
                                 CString& csCode,
                                 long& lQty, 
                                 long& lWgt, 
                                 long& lPrc, 
                                 long& lUPrc, 
                                 long& lTcd, 
                                 long& lBarcodeType)
{
    return PSProceduresBase::GetVisualItem(lVisualItemIndex, csDesc, csCode,lQty, lWgt, lPrc, lUPrc, lTcd, lBarcodeType);
}

LPCTSTR PSProcedures::GetCurrentMessageBoxText()
{
    return PSProceduresBase::GetCurrentMessageBoxText();
}

void PSProcedures::SetTareListToCurrentLanguage(void)
{
    PSProceduresBase::SetTareListToCurrentLanguage();
}

void PSProcedures::SetCMReceiptToCurrentLanguage(void)
{
    PSProceduresBase::SetCMReceiptToCurrentLanguage();
}

long PSProcedures::AddToReport(const CString& text, const CString& csReceiptSrc)
{
    return PSProceduresBase::AddToReport(text, csReceiptSrc);
}

void PSProcedures::ResetReport(const CString& csReceipt)
{
    PSProceduresBase::ResetReport(csReceipt);
}

void PSProcedures::HideReport(const CString& csReceipt)
{
    PSProceduresBase::HideReport(csReceipt);
}

void PSProcedures::ShowReport(const CString& csReceipt)
{
    PSProceduresBase::ShowReport(csReceipt);
}

HRESULT PSProcedures::StartRemoteServer()
{
    return PSProceduresBase::StartRemoteServer();
}

CString PSProcedures::GetRemoteServerName()
{
    return PSProceduresBase::GetRemoteServerName();
}

BOOL PSProcedures::RemoteMode()
{
    return PSProceduresBase::RemoteMode();
}

CString PSProcedures::GetCurrentContext()
{
    return PSProceduresBase::GetCurrentContext();
}

long PSProcedures::SMShowItemReceipt(const bool fSetScrolling)
{
    return PSProceduresBase::SMShowItemReceipt(fSetScrolling);
}

long PSProcedures::SMShowRestrictedItemReceipt(const bool fSetScrolling)
{
    return PSProceduresBase::SMShowRestrictedItemReceipt(fSetScrolling);
}

long PSProcedures::RestrictedItemNotAllowed(const int nCustomerAge, long& lUnapprovedRestrictedItems)
{
    return PSProceduresBase::RestrictedItemNotAllowed(nCustomerAge, lUnapprovedRestrictedItems);
}

long PSProcedures::GetCurrentItemAutoApprovalFlag(bool &bAutoApprove)
{
    return PSProceduresBase::GetCurrentItemAutoApprovalFlag(bAutoApprove);
}

bool PSProcedures::IsThisItemVoided(const CString& strItemId, bool& bValue)
{
    return PSProceduresBase::IsThisItemVoided(strItemId, bValue);
}

long PSProcedures::SetCurrentItemAutoApprovalFlag(const bool bAutoApprove)
{
    return PSProceduresBase::SetCurrentItemAutoApprovalFlag(bAutoApprove);
}

long PSProcedures::SMShowVoidedItemReceipt(const bool fSetScrolling)
{
    return PSProceduresBase::SMShowVoidedItemReceipt(fSetScrolling);
}

long PSProcedures::SMShowCouponReceipt(const bool fSetScrolling)
{
    return PSProceduresBase::SMShowCouponReceipt(fSetScrolling);
}

long PSProcedures::SMShowVisualItemReceipt(const bool fSetScrolling)
{
    return PSProceduresBase::SMShowVisualItemReceipt(fSetScrolling);
}

long PSProcedures::SMShowSecurityLog(void)
{
    return PSProceduresBase::SMShowSecurityLog();
}

long PSProcedures::SMShowMediaList(void)
{
    return PSProceduresBase::SMShowMediaList();
}

long PSProcedures::SMShowSMMItemList(void)
{
    return PSProceduresBase::SMShowSMMItemList();
}

long PSProcedures::SMInfo(LPCTSTR szInfo)
{
    return PSProceduresBase::SMInfo(szInfo);
}

int PSProcedures::ageFromDateTime(COleDateTime& date)
{
    return PSProceduresBase::ageFromDateTime(date);
}

CString PSProcedures::DateFormat(void)
{
    return PSProceduresBase::DateFormat();
}

CString PSProcedures::DateFormat(int month, int day, int year)
{
    return PSProceduresBase::DateFormat(month, day, year);
}

long PSProcedures::DeleteRewardItem(long lRewardID)
{
    return PSProceduresBase::DeleteRewardItem(lRewardID);
}

long PSProcedures::AddRewardItem(const CString& csReceiptSrc, CString& csDesc, long lRewardID, 
                                 long lValue, long lPosType, long lEntryIdForReference)
{
    return PSProceduresBase::AddRewardItem(csReceiptSrc, csDesc, lRewardID, lValue, lPosType, lEntryIdForReference);
}

long PSProcedures::FindRctIndexFromRewardID(long lRewardID, CString& csItemEntryId)
{
    return PSProceduresBase::FindRctIndexFromRewardID(lRewardID, csItemEntryId);
}

long PSProcedures::FindNextRctIndexFromRewardID(long lRewardID, CString& csItemEntryId)
{
    return PSProceduresBase::FindNextRctIndexFromRewardID(lRewardID, csItemEntryId);
}

long PSProcedures::FindRctIndexForReward(long lRefEntryID, long lPosType, CString& csItemEntryId)
{
    return PSProceduresBase::FindRctIndexForReward(lRefEntryID, lPosType, csItemEntryId);
}

long PSProcedures::InsertRewardItem(long nIndex, CString& csDesc, long RewardID, long lValue, long lEmphasis)
{
    return PSProceduresBase::InsertRewardItem(nIndex, csDesc, RewardID, lValue, lEmphasis);
}

//+POS138637
long PSProcedures::LoadTenderList(void)
{
    CString csCtxName = _T("Finish");
	TRACE(_T("LoadTenderList\n"));
    COleVariant vReturn, v1, v2;
    CString csCtlName;
     
      if (SMState::lWICOptionloaded == 1)
     {  
        csCtlName = _T("TenderImage");
        v2 = VARIANT_TRUE;
        m_pPSX->SetConfigProperty(csCtlName, csCtxName, UI::PROPERTYVISIBLE, v2);
        v2=VARIANT_FALSE;
        m_pPSX->SetConfigProperty(_T("SmartWICTenderImage"), csCtxName, UI::PROPERTYVISIBLE, v2);
        m_pPSX->SetConfigProperty( _T("SmartWICImage"), _T("Finish"), UI::PROPERTYVISIBLE, v2);

     }
     if (SMState::lWICOptionloaded == 2)
     {
        csCtlName = _T("SmartWICTenderImage");
        v2 = VARIANT_TRUE;
        m_pPSX->SetConfigProperty(csCtlName, csCtxName, UI::PROPERTYVISIBLE, v2);
        m_pPSX->SetConfigProperty( _T("SmartWICImage"), _T("Finish"), UI::PROPERTYVISIBLE, v2);
        m_pPSX->SetConfigProperty( _T("CreditImage"), _T("Finish"), UI::PROPERTYPOSITION, _T("110,454,100,110"));
        m_pPSX->SetConfigProperty( _T("CashImage"), _T("Finish"), UI::PROPERTYPOSITION, _T("350,454,100,110"));
        m_pPSX->SetConfigProperty( _T("CheckImage"), _T("Finish"), UI::PROPERTYPOSITION, _T("610,454,100,110"));
        v2=VARIANT_FALSE;
        m_pPSX->SetConfigProperty(_T("TenderImage"), csCtxName, UI::PROPERTYVISIBLE, v2);
    }
     
    USES_CONVERSION;
	long nButtonCount = 0;


    // reset the count to 0 to clear all previously set attributes
    COleVariant v = (long) 0;
    m_pPSX->SetConfigProperty( csCtlName, csCtxName, UI::PROPERTYBUTTONCOUNT, v );
	m_pPSX->SetControlProperty(csCtlName, UI::PROPERTYBUTTONCOUNT, v );

    nButtonCount = MAXBUTTONNUM;
    v = nButtonCount;

    CPSXRedrawLock csRedrawLock( csCtlName, _T("PSProceduresBase::LoadTenderList"));

    m_pPSX->SetConfigProperty( csCtlName, csCtxName, UI::PROPERTYBUTTONCOUNT, v );
    m_pPSX->SetControlProperty( csCtlName, UI::PROPERTYBUTTONCOUNT, v );

	CString str = _T("");
    COleSafeArray  saTextArray, saImageArray, saItemCodeArray;
    saTextArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
    saImageArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
    saItemCodeArray.Create(VT_VARIANT, 1, (DWORD*)&nButtonCount);
    //COleVariant vReturn, v1, v2;
	TCHAR szPath[_MAX_PATH];

    for(long  i = 0; i < nButtonCount; i++ )
	{
        v1 = i;

		BOOL bDisableButtonByTB;

        bDisableButtonByTB = tb.DisableTenderButton((TBTENDERTYPE)co.TenderList[m_languageUsed][i].nTenderType);
		
		if (bDisableButtonByTB)
		{
			v2 = (long) UI::Disabled; // disable
			m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONSTATE, vReturn, 2, v1, v2 );
			
			int nTenderType = co.TenderList[m_languageUsed][i].nTenderType;
			trace(L6,_T("Tender button disabled by TB, Tender Type: %d"), nTenderType);
		}

        if (co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_USERDEFINED1)
        {
            if (SMState::lWICOptionloaded == 1)
            {
                trace(L6, _T("Texas SmartWICFinish disable condition"));
                v2 = VARIANT_FALSE;
           	    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
                m_pPSX->SetConfigProperty( _T("SmartWICImage"), _T("Finish"), UI::PROPERTYVISIBLE, v2);
            }
        }

        if (co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_COUPON) 
        {
            if (!co.fTenderShowUseCoupons)
            {
                v2 = VARIANT_FALSE;
			    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
            }
        }
        if ((co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_FOODSTAMPS) ||
            (co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_PIP_FOODSTAMPS) )
        {
            if (!co.fTenderEBTFSAllowed)
            {
                v2 = VARIANT_FALSE;
			    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
            }
            else if (SMStateBase::lFoodStampsDue == 0)
            {
			    v2 = (long) UI::Disabled; // disable
			    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONSTATE, vReturn, 2, v1, v2 );
            }
        }
        if (co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_OTHERPAYMENT)
        {
            if (!co.fTenderOtherPayment)
            {
                v2 = VARIANT_FALSE;
			    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
            }
        }
        if (co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_CASH)
        {
            if (!co.fTenderCashAllowed)
            {
                v2 = VARIANT_FALSE;
			    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );     
            }
        }
        if ((co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_CREDIT) ||
            (co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_PIP_CREDIT) )
        {
            if (!co.fTenderCreditAllowed)
            {

                v2 = VARIANT_FALSE;

			    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
            }
        }
        if 	((co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_DEBIT) ||
			 (co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_PIP_DEBIT) ) 
        {
            if (!co.fTenderDebitAllowed)
            {
                v2 = VARIANT_FALSE;
			    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
            }
        }
        if	((co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_EBT) ||
			 (co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_PIP_EBT)) 
        {
            if (!co.fTenderEBTCBAllowed)
            {
                v2 = VARIANT_FALSE;
			    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
            }
        }
        if (co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_PIP_DEBIT_CASHBACK) 
        {
            if (! co.fTenderDebitAllowed) 
            {
                v2 = VARIANT_FALSE;
			    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
            }
        }
		if ((co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_GIFTCARD) ||
			(co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_PIP_GIFTCARD))
        {
            if (co.nOperationsGiftCardRedemption == 0)
            {
                v2 = VARIANT_FALSE;
			    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
            }
        }
        if (co.TenderList[m_languageUsed][i].nTenderType == 0) 
        {
            v2 = VARIANT_FALSE;
		    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
        }

		//SR742+
		if (co.IsDegradedModeOn() )
		{
			m_lModeType = dm.GetDegradedModeType();

			switch(m_lModeType)
			{
				case DEGRADEDMODE_CARD_ONLY:
					{
						if ( (co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_CASH) || 
							  co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_EBT   ||
						   ( (co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_CREDIT  || 
						      co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_DEBIT ) && 
							  co.TenderList[ps.m_languageUsed][i].bCashBack  ) )
						{
							v2 = VARIANT_FALSE;
							m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );		
						}		

					}break;
				case DEGRADEDMODE_CASH_ONLY:
					{
						if ( co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_CREDIT || 
							 co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_DEBIT  ||
							 co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_EBT    || 
							 co.TenderList[m_languageUsed][i].nTenderType == TB_TENDER_FOODSTAMPS )
						{
							v2 = VARIANT_FALSE;
							m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );	
						}

					}break;
				default:
					{
						trace(L6, _T("PSProceduresBase: %d is invalid degraded mode type."), m_lModeType);	
					}
			}
		}
		//SR742-

		str = co.TenderList[m_languageUsed][i].csTenderText;
		// +TAR 314786
		BSTR bstrText = str.AllocSysString();
		saTextArray.PutElement(&i, bstrText );
		::SysFreeString(bstrText);
		// -TAR 314786
		CString csImagePath;
		GET_PARTITION_PATH(SCOTIMAGE, csImagePath.GetBuffer(_MAX_PATH));

	    _tmakepath(szPath,NULL,csImagePath,co.TenderList[m_languageUsed][i].csTenderBitmap, NULL);
		csImagePath.ReleaseBuffer();
		str = szPath;
		// +TAR 314786
		BSTR bstrImage = str.AllocSysString();
		saImageArray.PutElement(&i, bstrImage );
		::SysFreeString(bstrImage);
		// -TAR 314786

		v = (long) co.TenderList[m_languageUsed][i].nButtonNum;	
        saItemCodeArray.PutElement(&i, &v );  

    }
    v = saTextArray;
    m_pPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONTEXTLIST, vReturn, 1, v );
    v = saImageArray;
    m_pPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONLOGOLIST, vReturn, 1, v );

    v = saItemCodeArray;
    m_pPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONDATALIST, vReturn, 1, v );

    return 0;
}
//-POS138637

long PSProcedures::AddRewardItem2(CString& csDesc, long lRewardID, long lValue, long lEmphasis)
{
    return PSProceduresBase::AddRewardItem2(csDesc, lRewardID, lValue, lEmphasis);
}

long PSProcedures::AddMessageToReceipt2(CString text, const bool hilite)
{
    return PSProceduresBase::AddMessageToReceipt2(text, hilite);
}

PSXRC PSProcedures::SetSelection(const CString& csReceipt, const CString& csItemEntryId)
{
    return PSProceduresBase::SetSelection( csReceipt,  csItemEntryId);
}

CString PSProcedures::GetSelection(const CString& csReceipt)
{
    return PSProceduresBase::GetSelection( csReceipt);
}

bool PSProcedures::CheckForScrolling(const CString& csReceiptSrc, const CString& csReceipt, const CString& csScrollMode)
{
    return PSProceduresBase::CheckForScrolling(csReceiptSrc, csReceipt, csScrollMode);
}

long PSProcedures::GetControlHandle(HWND& hControlWnd)
{
    return PSProceduresBase::GetControlHandle( hControlWnd);
}

void PSProcedures::ShowMoreInfoOnReceipt(const CString& csReceipt, const CString& csControl)
{
    PSProceduresBase::ShowMoreInfoOnReceipt(csReceipt, csControl);
}

void PSProcedures::SetModeDisplay(bool bShowModeDisplay)
{
    PSProceduresBase::SetModeDisplay(bShowModeDisplay);
}

long PSProcedures::LoadLanguageList(void)
{
    return PSProceduresBase::LoadLanguageList();
}

void PSProcedures::CreateVolCtlThread(bool bSetInitial)
{
    PSProceduresBase::CreateVolCtlThread(bSetInitial);
}

long PSProcedures::LoadPersonalizedPopularList(const CString& csUPCList)
{
    return PSProceduresBase::LoadPersonalizedPopularList(csUPCList);
}

void PSProcedures::AddItemToList(const CString csItemSent, CStringArray& csPersonalizedPicklist)
{
    PSProceduresBase::AddItemToList(csItemSent, csPersonalizedPicklist);
}

void PSProcedures::UpdatePicklistFavoritesMetrics(CStringArray& csPicklistItemSold, CStringArray& csQuickPickItemSold)
{
    PSProceduresBase::UpdatePicklistFavoritesMetrics(csPicklistItemSold, csQuickPickItemSold);
}

void PSProcedures::UpdateFailedDisplayPersonalPicklistItems(const CString& csUPCList)
{
    PSProceduresBase::UpdateFailedDisplayPersonalPicklistItems(csUPCList);
}

void PSProcedures::UpdatePicklistSoldItemData(const CStringArray& csPicklistItemSoldInfo, CStringArray& csQuickPickItemSold)
{
    PSProceduresBase::UpdatePicklistSoldItemData(csPicklistItemSoldInfo, csQuickPickItemSold);
}

void PSProcedures::SetInputText(LPCTSTR lpszInput)
{
    PSProceduresBase::SetInputText(lpszInput);
}

long PSProcedures::LoadPersonalizedQuickPickList(const CString& csUPCList)
{
    return PSProceduresBase::LoadPersonalizedQuickPickList(csUPCList);
}

void PSProcedures::UpdateQuickPickSoldItem(const CStringArray& csQuickPickItemSoldList, CStringArray& csPersonalizedPicklistItems)
{
    PSProceduresBase::UpdateQuickPickSoldItem(csQuickPickItemSoldList, csPersonalizedPicklistItems);
}

long PSProcedures::ExternalUIRequestExec(PS_UI_CMD pUIRequest)
{
    long lRetVal = 0;
	PS_UI_CMD ptrUIRequest = pUIRequest;
	CString csReceiptSrc;
	CString csItemEntryId;
	long rc = (long) PSX_E_FAIL;
	while (ptrUIRequest)
	{
		switch (ptrUIRequest->eStructType)
		{
			case UI_STR_RECEIPT:
			{
				csReceiptSrc =  (ptrUIRequest->Rcpt.lpcRcptStructName);
				csItemEntryId = (ptrUIRequest->Rcpt.lpcItemId);
				CString csItemEntryRefId(ptrUIRequest->Rcpt.lpcRefItemId);
				if (csReceiptSrc.IsEmpty() || csItemEntryId.IsEmpty())
				{
					break;
				}
				switch (ptrUIRequest->Rcpt.eItemCmd)
				{
					case UI_ITEM_CMD_READVARS:
					{
						rc = ProcessPIMReadReceiptVars(ptrUIRequest);
						ptrUIRequest->lStatus = rc;
					}
					break;
					case UI_ITEM_CMD_UPDATE:
					{
						rc = ProcessPIMItemUpdate(ptrUIRequest);
						ptrUIRequest->lStatus = rc;
					}
					break;
					case UI_ITEM_CMD_INSERT:
					{
						rc = ProcessPIMItemInsert(ptrUIRequest);
						ptrUIRequest->lStatus = rc;
					}
					break;
					case UI_ITEM_CMD_REMOVEITEM:
					{
						rc = m_pPSX->RemoveReceiptItem(csReceiptSrc, csItemEntryId); //remove reward item
						ptrUIRequest->lStatus = rc;
					}
					break;
					case UI_ITEM_CMD_REMOVEVARS:
					{
						rc = ProcessPIMRemoveVars(ptrUIRequest);
						ptrUIRequest->lStatus = rc;
					}
					break;
					case UI_ITEM_CMD_CLEARRCPT:
					{
						rc = m_pPSX->ClearReceipt(csReceiptSrc);
						ptrUIRequest->lStatus = rc;
					}
					break;
					case UI_ITEM_CMD_CLEARRCPTITEMVARS:
					{
						rc = m_pPSX->ClearReceiptItemVariables(csReceiptSrc, csItemEntryId, true);
						ptrUIRequest->lStatus = rc;
					}
					break;
					case UI_ITEM_CMD_REMOVEMPERKS:
						{
							rc = RemovemPerksFromEReceipt(_T("CMSMReceipt"));
							rc = RemovemPerksFromEReceipt(_T("CMSmallReceipt"));
							ptrUIRequest->lStatus = rc;
						}
					break;
					case UI_ITEM_CMD_LASTINLIST:
					default:
					break;
				}
			}
			break;
			case UI_STR_FINDITEM:
			{
				rc = ProcessPIMFindItem(ptrUIRequest);
				ptrUIRequest->lStatus = rc;
			}
			break;
			case UI_STR_SELECTEDITEM:
			{
				rc = ProcessPIMSelectItem(ptrUIRequest);
				ptrUIRequest->lStatus = rc;
			}
			break;
			case UI_STR_SCROLL:
			{
				rc = ProcessPIMScroll(ptrUIRequest);
				ptrUIRequest->lStatus = rc;
			}
			break;
			case UI_STR_TRANSVAR:
			{

				ProcessPIMTransactionVars(ptrUIRequest);
				ptrUIRequest->lStatus = rc;
			}
			break;
			case UI_STR_CONTEXT:
			{
				rc = ProcessPIMContext(ptrUIRequest);
				ptrUIRequest->lStatus = rc;
			}
			break;
			case UI_STR_SEND_EVENT:
			{
				rc = ProcessPIMSendEvent(ptrUIRequest);
				ptrUIRequest->lStatus = rc;
			}
			break;
			case UI_STR_LASTINLIST:
			default:
			break;
		} // end of switch (ptrUIRequest->eStructType)

		ptrUIRequest = ptrUIRequest->pNextUICommand;
	} // end of while (ptrUIRequest)
	if (!csReceiptSrc.IsEmpty())
	{
		if (csReceiptSrc == _T("CMSmallReceipt"))
		{
			ShowMoreInfoOnReceipt(_T("CMSmallReceipt"), _T("CMSmallReceiptMoreInfo"));
		}
		else
		{
			m_pPSX->UpdateReceiptControls(csReceiptSrc);
		}
	}
    return lRetVal;

}

long PSProcedures::AddReceiptItemFromUIRequest(const CString& csReceiptSrc, CString csRewardItemId, CString csEntryIdForReference)
{
    return PSProceduresBase::AddReceiptItemFromUIRequest(csReceiptSrc, csRewardItemId, csEntryIdForReference);
}

long PSProcedures::FindReceiptItemWithMultipleCriteria(const CString& csReceiptName, S_UI_FindItem&  sUIFindCriteria)
{
    return PSProceduresBase::FindReceiptItemWithMultipleCriteria(csReceiptName, sUIFindCriteria);
}

long PSProcedures::ProcessPIMReadReceiptVars(PS_UI_CMD ptrUIRequest)
{
    return PSProceduresBase::ProcessPIMReadReceiptVars( ptrUIRequest);
}

long PSProcedures::ProcessPIMItemInsert(PS_UI_CMD ptrUIRequest)
{
	CString csReceiptSrc =  (ptrUIRequest->Rcpt.lpcRcptStructName);
	CString csItemEntryId = (ptrUIRequest->Rcpt.lpcItemId);
	CString csItemEntryRefId(ptrUIRequest->Rcpt.lpcRefItemId);
	PSXRC rc = PSX_SUCCESS;
	PSXRC finalrc = PSX_SUCCESS;
	rc = (PSXRC) AddReceiptItemFromUIRequest(csReceiptSrc, csItemEntryId, csItemEntryRefId);
	int iUpdateVarNum = 0;
	if (rc != PSX_SUCCESS)
	{
		trace(L0, _T("failed to insert line, rcptsrc:%s, entryid:%s, refentryid:%s"), csReceiptSrc, csItemEntryId, csItemEntryRefId);
		iUpdateVarNum = QUERY_MAX; // do not process any further
		finalrc = rc;
	}
	
	// set new receipt line properties
	PS_UI_VAR ptrUpdateVarCmd = NULL;
	for (bool bContinue = true ; ((bContinue) && (iUpdateVarNum < QUERY_MAX)); ++iUpdateVarNum)
	{
		ptrUpdateVarCmd = ptrUIRequest->Rcpt.pUiVars[iUpdateVarNum];
		if (ptrUpdateVarCmd)
		{
			
			rc = m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, ptrUpdateVarCmd->lpcVarName, ptrUpdateVarCmd->vVarValue);
			ptrUpdateVarCmd->lOpStatus = rc;

			CString rcptItemDescription = ptrUpdateVarCmd->vVarValue.bstrVal;
			if(rcptItemDescription.Find(_T("mPerks")) == 0 )
			{
				SMState::mPerksActivatedInTransaction = true; //mPerks activated for this transaction. Will use this flag to maintain mPerks button state.
				m_pPSX->SetConfigProperty (_T("CMButton7"), _T("ScanAndBag"), UI::PROPERTYSTATE, (long)UI::Disabled);
				m_pPSX->SetConfigProperty (_T("mPerksButtonVideo"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
				trace(L6,_T("+PSProcedures::ProcessPIMItemInsert(), mPerks detected in the receipt. Description: %s"), rcptItemDescription);
			}
			
			if (rc != PSX_SUCCESS)
			{
				finalrc = rc;
			}
		}
		else
		{
			// break from this loop
			// no more request entries for this receipt item
			bContinue = false;
		}
	}
	return finalrc;
}

long PSProcedures::ProcessPIMItemUpdate(PS_UI_CMD ptrUIRequest)
{
    return PSProceduresBase::ProcessPIMItemUpdate( ptrUIRequest);
}

long PSProcedures::ProcessPIMRemoveVars(PS_UI_CMD ptrUIRequest)
{
    return PSProceduresBase::ProcessPIMRemoveVars( ptrUIRequest);
}

long PSProcedures::ProcessPIMTransactionVars(PS_UI_CMD ptrUIRequest)
{
    return PSProceduresBase::ProcessPIMTransactionVars( ptrUIRequest);
}

long PSProcedures::ProcessPIMSelectItem(PS_UI_CMD ptrUIRequest)
{
    return PSProceduresBase::ProcessPIMSelectItem( ptrUIRequest);
}

long PSProcedures::ProcessPIMFindItem(PS_UI_CMD ptrUIRequest)
{
    return PSProceduresBase::ProcessPIMFindItem( ptrUIRequest);
}

long PSProcedures::ProcessPIMScroll(PS_UI_CMD ptrUIRequest)
{
    return PSProceduresBase::ProcessPIMScroll( ptrUIRequest);
}

long PSProcedures::ProcessPIMContext(PS_UI_CMD ptrUIRequest)
{
    return PSProceduresBase::ProcessPIMContext( ptrUIRequest);
}

long PSProcedures::ProcessPIMSendEvent(PS_UI_CMD ptrUIRequest)
{
    return PSProceduresBase::ProcessPIMSendEvent( ptrUIRequest);
}

void PSProcedures::TraceItemVariables(CString csReceiptSrc, CString csItemEntryId)
{
    PSProceduresBase::TraceItemVariables(csReceiptSrc, csItemEntryId);
}

void PSProcedures::RegisterPsxEventHandler( void * EventHandler )
{
    PSProceduresBase::RegisterPsxEventHandler( EventHandler );
}

void PSProcedures::UnRegisterPsxEventHandler( void * EventHandler )
{
    PSProceduresBase::UnRegisterPsxEventHandler( EventHandler );
}

void PSProcedures::AddPsxClickEventMapping( LPCTSTR KeyName,
                                           PSEVT evtType,
                                           long  wParam,
                                           bool  bUseWParam,
                                           long  lParam, 
                                           bool  bUseLParam,
                                           void  *pMappingFxn )
{
    PSProceduresBase::AddPsxClickEventMapping(KeyName, evtType, wParam, bUseWParam,
        lParam, bUseLParam, pMappingFxn );
}

bool PSProcedures::RemovePsxClickEventMapping( LPCTSTR KeyName )
{
    return PSProceduresBase::RemovePsxClickEventMapping( KeyName );
}

//UploadTicket Port +

long PSProcedures::AddItemToReceipt(const CString& csReceiptSrc)
{
    long rc = TB_SUCCESS;

  trace(L7,_T("+PSProcedures::AddItemToReceipt(), ReceiptName: %s"), csReceiptSrc);

  CString csItemEntryId = io.d.csItemID;

  if (io.d.lQtySold == 1)
  { // Remove PSX Receipt Item Variables - Eliminates Detail Line.
		//quantity info is treated as a subitem for small receipt
		if (csReceiptSrc == _T("CMSmallReceipt"))
		{
			CString csSubItemId = csItemEntryId + _T("QTY");
			rc=m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_DEALQTY"));
			rc=m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_SOLDQTY"));
			rc=m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_UNITPRICE")); 
			rc=m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_SUFFIXTEXT"));
			rc=m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_RESTRICTED"));
		}
		else
		{
			m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_DEALQTY"));
			m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SOLDQTY"));
			m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_UNITPRICE")); 
			m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SUFFIXTEXT"));
			m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_RESTRICTED"));
		}

	  //m_pPSX->UpdateReceiptControls(csReceiptSrc); 
  }


  CString csDesc = _T("");
  CString csSubItemDesc = _T("");
  csItemEntryId = _T("");
  CString csInfo = _T("");
  
  CStringArray csItemEntryIdList;
  COleVariant vValue, vOrigDesc, vOrigExtPrice;
  bool bFoundOldPrice = false;
  static CString csSavedItemEntryId = _T("");
 
  vOrigDesc.Clear();
  vOrigExtPrice.Clear();
 
  // csItemEntryId.Format(_T("%d"), io.d.lEntryID); //PIM
  csItemEntryId = io.d.csItemID;

  // tar381634
  if(( _T("CMSMReceipt") == csReceiptSrc))
  {
	UseCustomerLanguageForLoadString(true);	
  }
  //+TAR 325410
  SCOT_LANGUAGE_TYPE presentLanguage;
  if ((_T("CMSMReceipt") == csReceiptSrc) && SMStateBase::fStateAssist)
  {
	  presentLanguage = GetLanguage();
	  SetLanguage(SMStateBase::m_customerLanguage);

  }
  //-TAR 325410
  if (!io.d.bLinkedItem)
  {
	//create a receipt item for display
	m_pPSX->CreateReceiptItem(csReceiptSrc, csItemEntryId);
	m_csSavedItemEntryId = csItemEntryId;
  }
  else
  {
	  //create a subitem for linked item
	  //tar 233768
	  CString csNumLinkedItems;
	  csNumLinkedItems.Format(_T("%d"), nNumLinkedItems);
	  csItemEntryId = m_csSavedItemEntryId + _T("LNK") + csNumLinkedItems;

	  if (io.d.lQtySold == 1)
	  { // Remove PSX Receipt Item Variables - Eliminates Detail Line.
		//quantity info is treated as a subitem for small receipt
		m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_DEALQTY"));
		m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SOLDQTY"));
		m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_UNITPRICE")); 
		m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SUFFIXTEXT"));
		m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_RESTRICTED"));

	  //m_pPSX->UpdateReceiptControls(csReceiptSrc); 
	  }
	  m_pPSX->CreateReceiptItem(csReceiptSrc, csItemEntryId, m_csSavedItemEntryId);
  }

  if (io.csOrigCode.Left(1) == LINK_ITEM)
  {
	io.csItemCode = io.csOrigCode;
  } 

  // if item is exception of type XAPPROVALNONE and TBXF_SUPPRESSPRICEDISPLAY 
  // display exception category description and do not show price
  if ((io.xd.ExceptionCode == XAPPROVALNONE) && 
	 (io.xd.dwExceptionFlags & TBXF_SUPPRESSPRICEDISPLAY))
  {
	csDesc = io.xd.csCategory;

	vValue = csDesc;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_DESCRIPTION"), vValue);
	vValue = io.csItemCode;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_CODE"), vValue);
	vValue = (long)io.cScanType;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SCAN_TYPE"), vValue);
	vValue = VARIANT_FALSE;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SELECTABLE"), vValue);
  }
  else 
  {
    CString csSiblingId = csItemEntryId + _T("ORIGPRICE");
    //get original price data for updates
    if (!io.d.bLinkedItem && io.lUpdateFields) //update
    {
        if (m_pPSX->GetReceiptItemVariable(csReceiptSrc, csSiblingId, _T("ITEM_OLDPRICE"), vValue) != PSX_SUCCESS)
        {  //get original data upon first update.
            bFoundOldPrice = true;
            m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_DESCRIPTION"), vOrigDesc);
            m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_EXT_PRICE"), vOrigExtPrice);
        }
    }
    //description of item
    csDesc = io.d.csDescription;
	
	vValue = csDesc;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_DESCRIPTION"), vValue);
	vValue = io.csItemCode;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_CODE"), vValue);
	vValue = (long)io.lQuantityEntered;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_QUANTITY"), vValue, true); //true means this applies to all subitems also
	vValue = (long)io.lWeightEntered;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_WEIGHT"), vValue); 
	vValue = (long)io.lTareCode;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_TARECODE"), vValue);
	vValue = (long)io.d.lExtPrice;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_EXT_PRICE"), vValue);
	vValue = (long)io.d.lUnitPrice;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_PRICE"), vValue);
	vValue = (long)io.cScanType;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SCAN_TYPE"), vValue);
	CString csExcCategory = (io.xd.ExceptionCode == XVISUALVERIFY) ? io.xd.csCategory : _T("");
	vValue = csExcCategory;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_EXCEPTION_CATEGORY"), vValue);
	vValue = (long)io.d.bSecurityTag;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SECURITY_TAG"), vValue);
	vValue = VARIANT_TRUE;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SELECTABLE"), vValue);
 
    if (io.d.fCoupon)
    {
        vValue = VARIANT_TRUE;
        m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_COUPON"), vValue);
    }

    if (bFoundOldPrice)
    {  //Create original price line for updates if it does not already exist
        m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_EXT_PRICE"), vValue);
        if (vValue != vOrigExtPrice  && !(vOrigDesc.vt == VT_EMPTY || vOrigDesc.vt == VT_NULL))  //SSCOADK-930, if extended price changed
        {
            m_pPSX->CreateReceiptItem(csReceiptSrc, csSiblingId, _T(""), csItemEntryId);
            vValue = VARIANT_FALSE;
            m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSiblingId, _T("ITEM_SELECTABLE"), vValue);
            m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSiblingId, _T("ITEM_OLDDESC"), vOrigDesc);
            m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSiblingId, _T("ITEM_OLDPRICE"), vOrigExtPrice);
			//Tar341239
			ScrollDown(_T("SMReceipt"));
			ScrollDown(_T("CMReceipt"));
        }
    }
    
	if (io.d.bLinkedItem)
	{
		vValue = VARIANT_TRUE;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_LINKED"), vValue);
		vValue = VARIANT_FALSE;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SELECTABLE"), vValue);
		//tar 233768
		if (csReceiptSrc == _T("CMSmallReceipt"))
		{
			vValue = VARIANT_TRUE;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_LINKED_FMT"), vValue);
		}
	}

	// If item is restricted, by setting ITEM_RESTRICTED variable to true
	// the item will be highlighted with a color configured in XML file

	// SSCOI-47774, added m_bAgeItemSoldInAM in the condition for restricted items from assist mode
	if (io.d.fRestricted || SMState::m_bAgeItemSoldInAM)
	{
		vValue = VARIANT_TRUE;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_RESTRICTED"), vValue);
		
		//TAR239858+ CHU 063003 If it is voided restricted item, need it to -1
		if (io.fVoid)
		{
			vValue = (long)-1;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_RESTRICTED_AGE"), vValue);
            
            //SOTF 6821
            vValue = (long)-1;
			m_pPSX->SetReceiptItemVariable(_T("SMReceiptRestricted"), csItemEntryId, _T("ITEM_RESTRICTED_AGE"), vValue);
            //sotf

		}//TAR239858-
		else
		{
			vValue = (long)io.d.nRestrictedAge;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_RESTRICTED_AGE"), vValue);
		}
		// SSCOI-47774
		if (SMState::m_bAgeItemSoldInAM)
		{
			SMState::m_bAgeItemSoldInAM = false;
		}
	}
    if (io.d.fVisualVerify)
    {
        vValue = VARIANT_TRUE;
        m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_VISUALVERIFY"), vValue);
        
    }
		
    if (io.lUpdateFields & ITEMUPDATE_QTY) //update of quantity information
    {
		//weight info for small receipt is treated as a subitem
		if (csReceiptSrc == _T("CMSmallReceipt"))
		{
			CString csSubItemId = csItemEntryId + _T("WGT");
			m_pPSX->RemoveReceiptItem(csReceiptSrc, csSubItemId);
        }
        else
        {
            m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_DEALQTY"));     
            m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SOLDQTY"));
            m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_UNITPRICE")); 
            m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SUFFIXTEXT"));
        }
    }
	// add weight info to receipt
	if (io.lWeightEntered && io.d.lUnitPrice!=0)
	{
		//weight info for small receipt is treated as a subitem
		if (csReceiptSrc == _T("CMSmallReceipt"))
		{
			CString csSubItemId = csItemEntryId + _T("WGT");
			m_pPSX->CreateReceiptItem(csReceiptSrc, csSubItemId, csItemEntryId);

			if (io.d.lDealQty>1 || io.d.lDealQty<-1)
			{
				vValue = (long)io.d.lDealQty;
				m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_DEALQTY"), vValue);
			}
			
			long lWgtSold = io.d.lWgtSold;
			vValue = FmtWeight(lWgtSold);
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_SOLDWEIGHT"), vValue);
			vValue = (long)io.d.lUnitPrice;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_UNITPRICE"), vValue); 
			vValue = csScaleUnits;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_SCALEUNITS"), vValue); 

			CString csText = _T("");
			if (io.lWeightEntered<0)
			{
			  csText = GetPSText(TXT_CANCELED);
			}

			vValue = csText;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_SUFFIXTEXT"), vValue);
			
			if (io.d.fRestricted)
			{
				vValue = VARIANT_TRUE;
				m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_RESTRICTED"), vValue);
			}
            if (io.d.fVisualVerify)
            {
                vValue = VARIANT_TRUE;
                m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_VISUALVERIFY"), vValue);
            }
		}
		else
		{
			if (io.d.lDealQty>1 || io.d.lDealQty<-1)
			{
				vValue = (long)io.d.lDealQty;
				m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_DEALQTY"), vValue);
			}
			
			vValue = FmtWeight(io.d.lWgtSold);
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SOLDWEIGHT"), vValue);
			vValue = (long)io.d.lUnitPrice;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_UNITPRICE"), vValue); 
			vValue = csScaleUnits;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SCALEUNITS"), vValue); 

			CString csText = _T("");
			if (io.lWeightEntered<0)
			{
			  csText = GetPSText(TXT_CANCELED);
			}

			vValue = csText;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SUFFIXTEXT"), vValue);
		}
	}
	// add quantity info to receipt
	else if (io.d.lQtySold > 1 || io.d.fQuantityRequired || io.d.lQtySold < -1 )
	{
		//quantity info is treated as a subitem for small receipt
		if (csReceiptSrc == _T("CMSmallReceipt"))
		{
			CString csSubItemId = csItemEntryId + _T("QTY");
			m_pPSX->CreateReceiptItem(csReceiptSrc, csSubItemId, csItemEntryId);
			if (io.d.lDealQty>1 || io.d.lDealQty<-1)
			{
				vValue = (long)io.d.lDealQty;
				m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_DEALQTY"), vValue);
			}
			
			long lQtySold = io.d.lQtySold;		
			vValue = (long)lQtySold;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_SOLDQTY"), vValue);
			vValue = (long)io.d.lUnitPrice;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_UNITPRICE"), vValue); 

			CString csText = _T("");
			if (io.lQuantityEntered<1)
			  csText = GetPSText(TXT_CANCELED);

			vValue = csText;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_SUFFIXTEXT"), vValue);
			
			if (io.d.fRestricted)
			{
				vValue = VARIANT_TRUE;
				m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_RESTRICTED"), vValue);
			}
            if (io.d.fVisualVerify)
            {
                vValue = VARIANT_TRUE;
                m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_VISUALVERIFY"), vValue);
            }
		}
		else
		{
			if (io.d.lDealQty > 1 || io.d.lDealQty < -1)
			{
				vValue = (long)io.d.lDealQty;
				m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_DEALQTY"), vValue);
			}
			vValue = (long)io.d.lQtySold;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SOLDQTY"), vValue);
			vValue = (long)io.d.lUnitPrice;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_UNITPRICE"), vValue); 

			CString csText = _T("");
			if (io.lQuantityEntered<1)
			  csText = GetPSText(TXT_CANCELED);

			vValue = csText;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SUFFIXTEXT"), vValue);
		}
	}
	// add deal quantity info to receipt
	else if (io.d.lDealQty>1 || io.d.lDealQty<-1)
	{
		if (csReceiptSrc == _T("CMSmallReceipt"))
		{
			CString csSubItemId = csItemEntryId + _T("QTY");
			m_pPSX->CreateReceiptItem(csReceiptSrc, csSubItemId, csItemEntryId);
			vValue = (long)io.d.lDealQty;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_DEALQTY"), vValue);
			long lQtySold = io.d.lQtySold;		
			vValue = (long)lQtySold;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_SOLDQTY"), vValue);
			vValue = (long)io.d.lUnitPrice;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_UNITPRICE"), vValue); 

			CString csText = _T("");
			if (io.lQuantityEntered<1)
			  csText = GetPSText(TXT_CANCELED);

			vValue = csText;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_SUFFIXTEXT"), vValue);
			
			if (io.d.fRestricted)
			{
				vValue = VARIANT_TRUE;
				m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_RESTRICTED"), vValue);
			}
		}
		else
		{
			vValue = (long)io.d.lDealQty;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_DEALQTY"), vValue);
			vValue = (long)io.d.lQtySold;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SOLDQTY"), vValue);
			vValue = (long)io.d.lUnitPrice;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_UNITPRICE"), vValue); 

			CString csText = _T("");
			if (io.lQuantityEntered<1)
			  csText = GetPSText(TXT_CANCELED);

			vValue = csText;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SUFFIXTEXT"), vValue);
		}
	}
  }
  if (io.fVoid && csReceiptSrc != _T("CMSmallReceipt"))
  {
		vValue = VARIANT_TRUE;
		// m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_VOIDADDED"), vValue);
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_VOID"), vValue);
		if (io.d.lExtPrice < 0)
		{
			// Make it positive so that there are NO parentheses at all!
			io.d.lExtPrice *= -1;
		}
		vValue = (long)io.d.lExtPrice;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_EXT_PRICE"), vValue);

		// sotf 9269 - dont' move the cursor if it's in AM - rrs
		if (!SMStateBase::fStateAssist)
		{
			vValue = VARIANT_FALSE;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SELECTABLE"), vValue);
		}
		else // SOTF 9269 - Append the 'V' in front of the Item Desc for Void
		{
			csDesc = _T("V   ") + csDesc;
			vValue = csDesc;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_DESCRIPTION"), vValue);
		}
  
  }

  TraceItemVariables(csReceiptSrc, csItemEntryId);
  m_pPSX->UpdateReceiptControls(csReceiptSrc); 

  //check to see if need to show more info on CMSmallReceipt
  if (csReceiptSrc == _T("CMSmallReceipt"))
  {
	  ShowMoreInfoOnReceipt(_T("CMSmallReceipt"), _T("CMSmallReceiptMoreInfo"));
  }

  // tar381634
  if(( _T("CMSMReceipt") == csReceiptSrc))
  {
	UseCustomerLanguageForLoadString(false);

    if (!io.d.fVoid)
    {
        CString csItemTrimmed = io.csItemCode;
        SMState* pState = (SMState*) mp.sCurr;
        pState->TrimSymbologyForSSF(csItemTrimmed);
        recallList.QueueMobileItemSold(csItemEntryId, csItemTrimmed);
    }
  }

  // For Meijer, be sure at bottom of Assist Mode Receipt (Scroll Down).
  // If we are not in assist mode, NOP.
  // TAR #301522.
  //ps.ScrollDown(_T("AssistReceipt"));

  if( GetHookProperty(_T("IsResumeTrxInProgress")) != _T("1") )
  {
    //PSSendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(_T("AssistReceipt")));
  }

 //  +TAR 325410
  if(( _T("CMSMReceipt") == csReceiptSrc) && SMStateBase::fStateAssist)
  {
	// Restore the language to the stored value.
    SetLanguage( presentLanguage);
  }
  //-TAR 325410
  trace(L7,_T("-AddItemToReceipt()"));

  return 0;
} 



//************************************************************************ 
//
// Function:		VoidItemFromReceipt
// Purpose:			This function voids an item form receipt.
// Parameters:      [in] fAdd - If TRUE indicates to add a new line to receipt
//                              in the case where no exact match on this void
//                              was found
//                  [in] csReceiptSrc   - receipt data source name
// Returns:	        0  - If item was found on the receipt and voided
//                  1  - if Item was not found on the receipt to void
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProcedures::VoidItemFromReceipt(const bool fAdd, const CString& csReceiptSrc)
{
  USES_CONVERSION;
	
  CString csItemEntryId = _T("");
  COleVariant vValue;
  CString csItemCode = _T("");
  bool bFound = false;
  long lExtPrice;
  long lBarcodeType; //TAR261289
  PSXRC rc;
  
  if (!io.d.csItemID.IsEmpty())
  {
      csItemEntryId = io.d.csItemID;
  }
  else if ( csReceiptSrc == _T("PickListReceipt") )
  {
	  m_pPSX->GetSelectedReceiptItem(_T("PickListReceiptVoid"), csItemEntryId);
  }
  else
  {
	  m_pPSX->GetSelectedReceiptItem(_T("CMReceiptVoid"), csItemEntryId);
  }
  m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_CODE"), vValue);
  vValue.ChangeType( VT_BSTR );
  csItemCode = OLE2T(vValue.bstrVal);

  vValue.Clear();
  m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_EXT_PRICE"), vValue);
  vValue.ChangeType( VT_INT );
  lExtPrice = vValue.lVal;

  //TAR261289+
  vValue.Clear();
  m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SCAN_TYPE"), vValue);
  vValue.ChangeType( VT_INT );
  lBarcodeType = vValue.lVal;
  //TAR261289-
  
  static CString csSavedEntryId = _T("");
  if (!io.d.bLinkedItem)
  {
	  csSavedEntryId = csItemEntryId;
	  vValue.Clear();
	  rc = m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_VOID"), vValue);
  }
  else
  {
	  vValue.Clear();
	  rc = m_pPSX->GetReceiptItemVariable(csReceiptSrc, csSavedEntryId, _T("ITEM_VOID"), vValue);
/*	  if (rc == PSX_SUCCESS)  //TAR 290534
	  {
		  return 0;
	  } */
  }

  vValue.Clear();
  if (rc != PSX_SUCCESS)
	  rc = m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_VOIDADDED"), vValue);
		
  CString csItemCode1 = csItemCode;
  if (csItemCode1.GetLength())
  {
	  if (csItemCode1.Left(3) == _T("]e0"))  //RFC 412256
				csItemCode1 = csItemCode1.Right(csItemCode1.GetLength() - 3); 
	  else
	  {
		  if (!_istdigit(csItemCode1[0]))
		  {
			csItemCode1.SetAt(0, _T(' '));
		  }
		  csItemCode1.TrimLeft();
	  }
  }
  CString csItemCode2 = io.csItemCode;
  if (csItemCode2.GetLength())
  {
	  if (csItemCode2.Left(3) == _T("]e0"))  //RFC 412256
				csItemCode2 = csItemCode2.Right(csItemCode2.GetLength() - 3); 
	  else
	  {
		  if (!_istdigit(csItemCode2[0]))
		  {
			csItemCode2.SetAt(0, _T(' '));
		  }
		  csItemCode2.TrimLeft();
	  }
  }
  //TAR261289+
  CString csItemCode1WithOutCheckDigit = _T("");
  CString csItemCode2WithOutCheckDigit = _T("");
  
  if (    ( abs (csItemCode1.GetLength() - csItemCode2.GetLength() ) == 1  ) 
	   && ( ( lBarcodeType != 0  && io.cScanType == 0 ) || (lBarcodeType == 0  && io.cScanType != 0) )
	 )
  {
	  if (csItemCode1.GetLength() > csItemCode2.GetLength() )
	  {
		  csItemCode1WithOutCheckDigit = csItemCode1.Left(csItemCode1.GetLength()-1);
	  }
	  else
	  {
		  csItemCode2WithOutCheckDigit = csItemCode2.Left(csItemCode2.GetLength()-1);
	  }

  }

  //if ((csItemCode1 == csItemCode2) && (rc != PSX_SUCCESS) && io.d.lExtPrice == 0-lExtPrice)
  //{
  //	  bFound = true;
  //}
  //if ( (rc != PSX_SUCCESS) && (io.d.lExtPrice == 0-lExtPrice) )
  if ( (rc != PSX_SUCCESS) && (io.d.lExtPrice == 0-lExtPrice) && io.d.csDescription== !_T("ITEM DISCOUNT") )
  {
	  if (    (csItemCode1 == csItemCode2) 
		   || (csItemCode1 == csItemCode2WithOutCheckDigit )
		   || (csItemCode2 == csItemCode1WithOutCheckDigit )
		 )
	  {
		  bFound = true;
	  }
  }
  //TAR261289-
  else
  {
	  CStringArray csItemEntryList;
	  vValue = io.csItemCode;
      if (!io.d.bLinkedItem)
      {
          m_pPSX->FindReceiptItems(csReceiptSrc, csItemEntryList, _T(""), _T("ITEM_SELECTABLE"), VARIANT_TRUE);
      }
      else
      {
          m_pPSX->FindReceiptItems(csReceiptSrc, csItemEntryList, csSavedEntryId);
      }
	  int lCountList = csItemEntryList.GetSize();
	  if (lCountList)
	  {
		  for (int i=lCountList-1; i>=0; i--)
		  {
			  vValue.Clear();
			  csItemEntryId = csItemEntryList.GetAt(i);
			  m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_CODE"), vValue);
			  vValue.ChangeType(VT_BSTR);
			  csItemCode = OLE2T(vValue.bstrVal);

			  vValue.Clear();
			  m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_EXT_PRICE"), vValue);
			  vValue.ChangeType(VT_INT);
			  lExtPrice = vValue.lVal;

			  //TAR261289+
			  vValue.Clear();
			  m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SCAN_TYPE"), vValue);
			  vValue.ChangeType( VT_INT );
			  lBarcodeType = vValue.lVal;
			  //TAR261289-

			  vValue.Clear();
			  rc = m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_VOID"), vValue);
			  if (rc != PSX_SUCCESS)
				rc = m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_VOIDADDED"), vValue);

			  csItemCode1 = csItemCode;
			  if (csItemCode1.GetLength())
			  {
				  if (!_istdigit(csItemCode1[0]))
				  {
					csItemCode1.SetAt(0, _T(' '));
				  }
				  csItemCode1.TrimLeft();
			  }
			  csItemCode2 = io.csItemCode;
			  if (csItemCode2.GetLength())
			  {
				  if (!_istdigit(csItemCode2[0]))
				  {
					csItemCode2.SetAt(0, _T(' '));
				  }
				  csItemCode2.TrimLeft();


			  }
			  //if ((csItemCode1 == csItemCode2) && (rc != PSX_SUCCESS) && io.d.lExtPrice == 0-lExtPrice)
			  if ((csItemCode1 == csItemCode2) && (rc != PSX_SUCCESS) && io.d.lExtPrice == 0-lExtPrice && io.d.csDescription== !_T("ITEM DISCOUNT"))
			  {
				  bFound = true;
				  break;
			  }
			  //TAR261289+   
			  csItemCode1WithOutCheckDigit = _T("");
			  csItemCode2WithOutCheckDigit = _T("");

			  if (    ( abs (csItemCode1.GetLength() - csItemCode2.GetLength() ) == 1  ) 
            	   && ( ( lBarcodeType != 0  && io.cScanType == 0 ) || (lBarcodeType == 0  && io.cScanType != 0) )
	             )
			  {
	              if (csItemCode1.GetLength() > csItemCode2.GetLength() )
				  {
		              csItemCode1WithOutCheckDigit = csItemCode1.Left(csItemCode1.GetLength()-1);
				  }
				  else
				  {
					  csItemCode2WithOutCheckDigit = csItemCode2.Left(csItemCode2.GetLength()-1);
				  }

			  }


			  //if ((csItemCode1 == csItemCode2) && (rc != PSX_SUCCESS) && io.d.lExtPrice == 0-lExtPrice)
			  //if ( (rc != PSX_SUCCESS) && (io.d.lExtPrice == 0-lExtPrice) )
  			  if ( (rc != PSX_SUCCESS) && (io.d.lExtPrice == 0-lExtPrice) && io.d.csDescription== !_T("ITEM DISCOUNT") )
			  {
				 if (    (csItemCode1 == csItemCode2) 
					  || (csItemCode1 == csItemCode2WithOutCheckDigit )
		              || (csItemCode2 == csItemCode1WithOutCheckDigit )
		            )
				 {

			        bFound = true;
				    break;
				 }
			  }			  //TAR260736-   
              //else if (io.d.bLinkedItem && (io.d.lExtPrice == 0-lExtPrice))
              else if (io.d.bLinkedItem && (io.d.lExtPrice == 0-lExtPrice) && io.d.csDescription== !_T("ITEM DISCOUNT"))
              {
				  trace(L6, _T("rachel checking true2"));
                  bFound = true;
                  break;
              }
			  //TAR261289-
		  }
	  }
  }

  if ( csReceiptSrc == _T("CMSMReceipt"))
  {
      CString csItemTrimmed = io.csItemCode;
      SMState* pState = (SMState*) mp.sCurr;
      pState->TrimSymbologyForSSF(csItemTrimmed);
      recallList.QueueMobileItemSold(csItemEntryId, csItemTrimmed);
  }

  if (bFound)
  {
	  vValue = VARIANT_TRUE;
	  m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_VOID"), vValue, true);
	  vValue = VARIANT_FALSE;
	  m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SELECTABLE"), vValue, true);

	  //TAR239858+ CHU 063003: if it is voided and restricted, need to set age to -1
	  if (io.d.fRestricted) 
	  {
	  	vValue = (long)-1;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_RESTRICTED_AGE"), vValue);
	  }
	  //TAR239858-
      //Remove pre-update price information and skip bagging lines.
      CString csSiblingId = csItemEntryId + _T("ORIGPRICE");
      m_pPSX->RemoveReceiptItem(csReceiptSrc, csSiblingId);
      csSiblingId = csItemEntryId + _T("ShowSkipBagging");
      m_pPSX->RemoveReceiptItem(csReceiptSrc, csSiblingId);
      csSiblingId = csItemEntryId + _T("ShowSkipBaggingRap");
      m_pPSX->RemoveReceiptItem(csReceiptSrc, csSiblingId);
      TraceItemVariables(csReceiptSrc, csItemEntryId);
	  m_pPSX->UpdateReceiptControls(csReceiptSrc); 
	  return 1;
  }

  if (fAdd && !bFound)
  {
		// there was no exact match on this void
		// so we can't find a line to strike out
		// instead we add a new line as a void
		long    lSvQty   = io.d.lQtySold;
		long    lSvWgt   = io.d.lWgtSold;
		io.d.lQtySold *= -1;            // show negative quantity on receipt
		io.d.lWgtSold *= -1;            // show negative weight on receipt
		AddItemToReceipt(csReceiptSrc);
		io.d.lQtySold = lSvQty;
		io.d.lWgtSold = lSvWgt;

		//TAR 397432
		if ( csReceiptSrc == _T("PickListReceipt") && !io.d.fCoupon)
		{
			m_lTotalPicklistItemsSold = m_lTotalPicklistItemsSold + io.d.lQtySold;
		}
  }
  return 0;
}
//UploadTicket Port - 

long PSProcedures::LoadPSText(LPCTSTR szFileName,const bool fRepErr)
{
    return PSProceduresBase::LoadPSText(szFileName, fRepErr);
}

long PSProcedures::LoadTareInfo(LPCTSTR szName)
{
    return PSProceduresBase::LoadTareInfo(szName);
}

long PSProcedures::Background(const BMPS nBmp,const int nMode)
{
    return PSProceduresBase::Background(nBmp, nMode);
}
//************************************************************************
long PSProcedures::PSSendTBCommand(long lCommandID, ...)
{
    va_list pArgs;
    va_start(pArgs, lCommandID);
    TBRC retCode = TBSendTBCommand(lCommandID, pArgs);
    va_end(pArgs);
    return retCode;
}

void PSProcedures::SetInput( CString csText, bool bHide )
{
	_tcscpy(szInputDigits, csText);
	CString csHide = _T("");
	for( int i=0; i<csText.GetLength(); i++ )
	{
		csHide += _T("*");
	}
	_tcscpy(szInputAsterx, csHide);
	CString csOut;
	if ( bHide )
	{
		csOut = csHide;
	}
	else
	{
		csOut = csText;
	}
	if ( !csOut.IsEmpty() )
	{
		csOut += _T("_");
	}
	Echo( csOut );
}

CString PSProcedures::GetHookProperty(CString csProp)
{
    CString csValue = _T("");
    PSSendTBCommand(TBCMD_GET_HOOKPROPERTY, &csProp, &csValue);
    return csValue;
}

CString PSProcedures::GetTBProperty(CString csProp)
{
    CString csValue = _T("");
    PSSendTBCommand(TBCMD_GET_TBPROPERTY, &csProp, &csValue);
    return csValue;
}

void PSProcedures::SetHookProperty(CString csProp, CString csValue)
{
    PSSendTBCommand(TBCMD_SET_HOOKPROPERTY, &csProp, &csValue);
}

void PSProcedures::SetTBProperty(CString csProp, CString csValue)
{
    PSSendTBCommand(TBCMD_SET_TBPROPERTY, &csProp, &csValue);
}

// Start CRD 125377
// Need to allow for the cases I want the state to execute but not display.
// Helps with screen flicker
void PSProcedures::SetSkipFrameDisplay(bool bSkip)
{
	bSkipFrameDisplay = bSkip;
}

bool PSProcedures::SkipFrameDisplay()
{
	return bSkipFrameDisplay;
}
// end CRD 125377

CString PSProcedures::GetMSGItemReceiptIdByDesc(CString receiptSrc, CString itemDesc)
{
	CString csItemEntryId(_T(""));
	CStringArray csItemEntryIdList;
	COleVariant vValue;
	
	m_pPSX->FindReceiptItems(receiptSrc, csItemEntryIdList, _T(""), _T("ITEM_MSG"));
	int nTmpCountList = csItemEntryIdList.GetSize();
	
	if(nTmpCountList > 0)
	{
		//Replace the message to the current language message.
		for (int i=0; i<nTmpCountList; i++)
		{	
			csItemEntryId = csItemEntryIdList.GetAt(i);
			m_pPSX->GetReceiptItemVariable(receiptSrc, csItemEntryId, _T("ITEM_MSG"), vValue);
			CString msgDesc = vValue.bstrVal;
			if(msgDesc.Find(itemDesc) >= 0 )
			{
				csItemEntryId = csItemEntryIdList.GetAt(i);
				break;
			}
		}
	}
	trace(L6,_T("+PSProcedures::GetMSGItemReceiptIdByDesc, message description: %s Id in the receipt: %s"), itemDesc, csItemEntryId);
	return csItemEntryId;
}

long PSProcedures::RemovemPerksFromEReceipt(CString csReceiptSrc)
{
	CString itemEntryId = GetMSGItemReceiptIdByDesc(csReceiptSrc, _T("mPerks"));
	COleVariant vValue;
	if(!itemEntryId.IsEmpty() && !csReceiptSrc.IsEmpty())
	{
		trace(L6,_T("Removing mPerks from eReceipt..."));
		//rc = m_pPSX->RemoveReceiptItem(csReceiptSrc, itemEntryId);
		//Or strike off by marking it void
		vValue = VARIANT_TRUE;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, itemEntryId, _T("ITEM_VOID"), vValue);
		
		vValue = VARIANT_FALSE;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, itemEntryId, _T("ITEM_SELECTABLE"), vValue);
	}
	else
	{
		trace(L6,_T("Couldn't find receipt entry for mPerks!"));
	}
	return 0;
}