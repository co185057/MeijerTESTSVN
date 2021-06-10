/*
-------------------------------------------------------------------------------

 Module Name     : PSItemList.cpp
 Type            : PSItemList class implementation file

 Language        : MSVC++
 Operating System: Win32
 Author/Location : SCOT Team RSG-Atlanta

 Copyright (C) NCR Corporation 1999

-------------------------------------------------------------------------------
 Date    | Changes Made                                      | Version | By
-------------------------------------------------------------------------------
 ?         Original                                            01.00.00  FZhang
 990201    Support sorting based on code page.                           KSo
-------------------------------------------------------------------------------
*/

#include "stdafx.h"
#include <locale.h>

#include "PSItemList.h"
#include "CommonServices.h"
#include "common.h"

#define COMP_ID   ID_PS
#define T_ID      _T("PSil")


/*<<<Constructors>>>-----------------------------------------------------------

-----------------------------------------------------------------------------*/
CItemInfo::CItemInfo( const CString& csDesc, 
					  const CString& csCode, 
					  const CString& csGraphic, 
					  bool  bFavorite, 
					  bool  bSubCategory, 
					  const CString& csCategories, 
					  const CString& csSubCategories, 
					  const CString& csReadingName, 
					  bool  bQuickPickItem,
					  bool  bIsItem,
					  const CString& csAlphabetic) :
	m_csItemName( csDesc ),
	m_csItemCode( csCode ),
	m_csItemGraphic( csGraphic ),
	m_bItemIsFavorite( bFavorite ),
	m_csCategories( csCategories ),
	m_csReadingName( csReadingName ),
	m_bIsSubCategory( bSubCategory ),
	m_csSubCategories( csSubCategories ),
	m_bQuickPickItem( bQuickPickItem ),
	m_bIsItem ( bIsItem ),
	m_csAlphabetic ( csAlphabetic )
{												 
} 

CCategoryInfo::CCategoryInfo( const CString& csId, 
							  const CString& csCatDesc, 
							  const CString& csCategories,
							  const CString& csAlphabetic, 
							  const CString& csGraphic, 
							  const CString& csScreenTitle, 
							  const CString& csInstructions, 
							  bool bQuickPickItem ) :
	m_csId( csId ),
	m_csCatDesc( csCatDesc ),
	m_csCategories( csCategories ),
    m_csAlphabetic( csAlphabetic ),
	m_csGraphic( csGraphic ),
	m_csScreenTitle( csScreenTitle ),
	m_csInstructions( csInstructions ),
	m_bQuickPickItem( bQuickPickItem )
{												 
}

/*<<<Constructor/Destructor>>>-------------------------------------------------

-----------------------------------------------------------------------------*/
CSItemList::CSItemList( const CString& csFileName ) :
	FileName( csFileName )
{
}

CSItemList::CSItemList() :
	FileName( _T("") )
{	
}

CSItemList::~CSItemList()
{
	EmptyList();
}

void CSItemList::EmptyList() 
{
	POSITION pos = GetHeadPosition();
	CItemInfo* pItem = NULL;
	while( pos != NULL )
	{
		CItemInfo* pItem = (CItemInfo*)GetNext( pos );
		delete pItem;
	}

	RemoveAll();
}
/*<<<InsertInOrder>>>----------------------------------------------------------

 Insert an item to the list in sorting order.

 Entry:  csDesc      Item description
         csCode      Item code
 Return:   none

-----------------------------------------------------------------------------*/
void CSItemList::InsertInOrder( const CString& csDesc, 
							    const CString& csCode, 
								const CString& csGraphic, 
								bool bFavorite, 
								bool bIsSubCategory, 
								const CString& csCategories, 
								const CString& csSubCategories, 
								const CString& csReadingName, 
								bool bQuickPickItem,
								bool bIsItem,
								const CString& csAlphabetic) 
{
	CItemInfo *pCurrentItem = new CItemInfo( csDesc, csCode, csGraphic, bFavorite, bIsSubCategory, csCategories, csSubCategories, csReadingName, bQuickPickItem, bIsItem, csAlphabetic );
	POSITION pos = GetHeadPosition();
	if( pos == NULL )
	{
		// add first item into the list
		InsertBefore( pos, pCurrentItem );
	}
	else
	{
		CItemInfo *pItem = NULL;
		POSITION CurrentPos = NULL;		
		while( pos != NULL )
		{
			CurrentPos = pos;
			pItem = (CItemInfo*)GetNext( pos );   // pos is the  next item position. pItem is the current iteminfo
			if( csDesc == pItem->m_csItemName && csCode == pItem->m_csItemCode )
			{
				// skip duplicates //
				delete pCurrentItem;
				pCurrentItem = NULL;
				break;
			}
			
			int rc = CompareString( LOCALE_USER_DEFAULT, NORM_IGNORECASE | SORT_STRINGSORT | NORM_IGNOREKANATYPE | NORM_IGNORENONSPACE, csReadingName.GetLength() ? csReadingName : csDesc, -1, pItem->m_csReadingName.GetLength() ? pItem->m_csReadingName : pItem->m_csItemName, -1 ) - 2;
			if( rc < 0 )
			{
				InsertBefore( CurrentPos, pCurrentItem ); //Insert before the current item
				break;
			}
			if( pos == NULL )
			{   
				// at end of list
				InsertAfter( CurrentPos, pCurrentItem ); //Insert after the current item
				break;
			}
		}
	}
}

/*<<<BuildList>>>--------------------------------------------------------------

 Build the item list.

 Entry:  spDoc - DOM pointer
		 csLangCode - Language code
 Return:   0 

-----------------------------------------------------------------------------*/
int CSItemList::BuildList(const MSXML2::IXMLDOMDocumentPtr &spDoc, const CString& csLangCode, CStringArray* pcsPickQtyList, CMapStringToBool* pmZeroWeighItems /*= NULL*/)
{
	static const TCHAR XML_AUTO[] = _T("Auto");
    
    trace(L6,_T("+CSItemList::BuildList, csLangCode: %s"), csLangCode);
	
    // Get Horizontal resolution //
    DWORD nXScreenSize = GetSystemMetrics( SM_CXSCREEN );
	// FastLane only supports 1024x768 and 800x600 resolution //
    if( 1024 != nXScreenSize )
    {
        // Force the use of 800x600 produce images for any other resolution //
        nXScreenSize = 800;
    }    
    CString csScreenSize;
	csScreenSize.Format( _T("%d"), nXScreenSize );

	CString csDefGraphics = co.csStateDefaultProduceBMPName;
	int nIndex = csDefGraphics.Find(_T("."));
	CString csGraphicName = csDefGraphics.Left(nIndex-4);
	CString csGraphicExt = csDefGraphics.Right(csDefGraphics.GetLength() - nIndex);
 			
	MSXML2::IXMLDOMNodeListPtr spNodeList, spSubNodeList, spPictureNodeList;
	MSXML2::IXMLDOMNodePtr spNode, spSubNode, spItem, spSubItem;
	
	//Process <DefaultLanguage>
	CString csDefLangCode(_T(""));
	spNode = spDoc->selectSingleNode(XML_DEFAULT_LANGUAGE);
	if (spNode != NULL)
	{
		csDefLangCode = LPCTSTR(spNode->text);
	}

	spNodeList = spDoc->selectNodes(XML_ITEM);

	while( ( spNode = spNodeList->nextNode() ) )
	{
		CString csGraphics(_T("")), csDesc(_T("")), csDefDesc(_T("")), csCode(_T("")), csReadingName(_T("")), csQuantity(_T("")), csFavorite(_T("")), csVisible(_T("")), csLCID(_T("")), csIsSubCategory(_T(""));
		bool bFavorite = false; 
		bool bIsZeroWeightItem = false; //Japan RFC#1 (RFQ 893)
		CString csIsZeroWeightItem(_T("")); //Japan RFC#1 (RFQ 893)
		CString csCategories(_T(""));
		CString csSubCategories(_T(""));
		CString csQuickPickItem(_T(""));
		bool bSubCategory = false;
		bool bQuickPickItem = false;
		
		//Process <IsVisible>
		spItem = spNode->selectSingleNode( XML_ISVISIBLE );
		if (spItem != NULL)
		{
			csVisible = LPCTSTR(spItem->text);
		}
		if (csVisible == XML_TRUE)
		{
			//Process UPC
			spItem = spNode->attributes->getNamedItem( XML_UPC );
			if( spItem != NULL && spItem->text.length() )
			{
				csCode = LPCTSTR(spItem->text);
			}

			if (csCode.GetLength())
			{
				//process <Picture>
				CString csExp;
				csExp.Format(_T("%s[@%s= '%s']"), XML_PICTURE, XML_RESOLUTION, csScreenSize);
				spPictureNodeList = spNode->selectNodes((LPCTSTR)csExp);

				CString csId;
				int iId;
				csGraphics = csGraphicName + csLangCode + csGraphicExt;
				CString csGraphicList[2] = {csGraphics, csGraphics};
				bool bNewFormat = false;

				while (spSubNode = spPictureNodeList->nextNode())
				{
					spSubItem = spSubNode->attributes->getNamedItem(XML_PICTUREID);
					
					if (spSubItem !=NULL)
					{
						bNewFormat = true;
						csId = LPCTSTR(spSubItem->text);				
						iId = _ttoi(csId);

						csGraphics = (LPCTSTR)spSubNode->Gettext();
						if ( (iId>0) && (iId<=2) && csGraphics.GetLength() )
						{
							csGraphicList[iId-1] = csGraphics;
						}
					}					
				}
				if (bNewFormat)
				{
					//PLE added supports for two graphics, but CoreApp is only using "ID=1" with this release
					csGraphics = csGraphicList[0];
				}
				else //to be backward compatibile
				{
					spSubNode = spNode->selectSingleNode( (LPCTSTR)csExp );
					if (spSubNode != NULL)
					{
						csGraphics = (LPCTSTR)spSubNode->Gettext();
					}
					if (!csGraphics.GetLength())
					{
						csGraphics = csGraphicName + csLangCode + csGraphicExt;
					}
				}

				//Process <IsQuantity>
				spItem = spNode->selectSingleNode( XML_ISQUANTITY );
				if (spItem != NULL)
				{
					csQuantity = LPCTSTR(spItem->text);
				}
				if (csQuantity == XML_TRUE)
				{
					if (pcsPickQtyList != NULL && csCode.GetLength())
						pcsPickQtyList->Add(csCode);
				}
				
				//Process <IsPopular>
				spItem = spNode->selectSingleNode( XML_ISPOPULAR );
				if (spItem != NULL)
				{
					csFavorite = LPCTSTR(spItem->text);
				}
				if( csFavorite == XML_TRUE || XML_AUTO == csFavorite )
				{
					bFavorite = true;
				}

				//Process <IsQuickPickItem>
				spItem = spNode->selectSingleNode( XML_ISQUICKPICKITEM );
				if (spItem != NULL)
				{
					csQuickPickItem = LPCTSTR(spItem->text);
				}
				if( csQuickPickItem == XML_TRUE )
				{
					bQuickPickItem = true;
				}

				//Japan RFC#1 (RFQ 893)
				spItem = spNode->selectSingleNode( XML_ISZEROWEIGHT );
				if (spItem != NULL)
				{
					csIsZeroWeightItem = LPCTSTR(spItem->text);
				}
				if (csIsZeroWeightItem == XML_TRUE )
				{
					bIsZeroWeightItem = true;
					if ( pmZeroWeighItems != NULL )
					{
						pmZeroWeighItems->SetAt( csCode, bIsZeroWeightItem );
					}
				}
				//Process <Language>
				spSubNodeList = spNode->selectNodes(XML_LANGUAGE);
				while (spSubNode = spSubNodeList->nextNode())
				{
					spSubItem = spSubNode->attributes->getNamedItem(XML_LCID);
					csLCID = LPCTSTR(spSubItem->text);
					if (csLCID.CompareNoCase(csLangCode) == 0)
					{	
						spSubItem = spSubNode->selectSingleNode(XML_CATEGORIES);
						if (spSubItem != NULL)
						{
							csCategories = LPCTSTR(spSubItem->text);
						}

						spSubItem = spSubNode->selectSingleNode(XML_SUBCATEGORIES);
						if (spSubItem != NULL)
						{
							csSubCategories = LPCTSTR(spSubItem->text);
						}

						spSubItem = spSubNode->selectSingleNode(XML_SORTINGSTRING);
						if (spSubItem != NULL)
						{
							csReadingName = LPCTSTR(spSubItem->text);
						}

						spSubItem = spSubNode->selectSingleNode(XML_DESCRIPTION);
						if (spSubItem != NULL && spSubItem->text.length())
						{
							csDesc = LPCTSTR(spSubItem->text);
							break;
						}
					}
					if (csLCID.CompareNoCase(csDefLangCode) == 0)
					{
						spSubItem = spSubNode->selectSingleNode(XML_DESCRIPTION);
						if (spSubItem != NULL && spSubItem->text.length())
						{
							csDefDesc = LPCTSTR(spSubItem->text);
						}
					}

				}
				if (!csDesc.GetLength())
				{
					csDesc = csDefDesc;
				}

				InsertInOrder( csDesc, csCode, csGraphics, bFavorite, bSubCategory, csCategories, csSubCategories, csReadingName, bQuickPickItem );
			}
		}
	}

	trace(L6,_T("-CSItemList::BuildList"));

	return 0;
}

CSCategoryList::CSCategoryList()
{
}


CSCategoryList::~CSCategoryList()
{
	EmptyCategoryList();
}

/*<<<BuildCategoryList>>>--------------------------------------------------------------

 Build the category list.

 Entry:  spDoc - DOM pointer
		 csLangCode - Language code
 Return:   0       

-----------------------------------------------------------------------------*/
int CSCategoryList::BuildCategoryList(const MSXML2::IXMLDOMDocumentPtr &spDoc, const CString& csLangCode)
{
	trace(L6,_T("+CSItemList::BuildCategoryList, csLangCode: %s"), csLangCode);

	MSXML2::IXMLDOMNodeListPtr spNodeList, spSubNodeList, spPictureNodeList;
	MSXML2::IXMLDOMNodePtr spNode, spSubNode, spItem, spSubItem;

	//Process <DefaultLanguage>
	CString csDefLangCode(_T(""));
	spNode = spDoc->selectSingleNode(XML_DEFAULT_LANGUAGE);
	if (spNode != NULL)
	{
		csDefLangCode = LPCTSTR(spNode->text);
	}

	// Get Horizontal resolution //
    DWORD nXScreenSize = GetSystemMetrics( SM_CXSCREEN );
	// FastLane only supports 1024x768 and 800x600 resolution //
    if( 1024 != nXScreenSize )
    {
        // Force the use of 800x600 produce images for any other resolution //
        nXScreenSize = 800;
    }    
    CString csScreenSize;
	csScreenSize.Format( _T("%d"), nXScreenSize );

	CString csDefGraphics = co.csStateDefaultProduceBMPName;
	int nIndex = csDefGraphics.Find(_T("."));
	CString csGraphicName = csDefGraphics.Left(nIndex-4);
	CString csGraphicExt = csDefGraphics.Right(csDefGraphics.GetLength() - nIndex);

	spNodeList = spDoc->selectNodes( XML_CATEGORY );

	int x = spNodeList->Getlength();

	while( ( spNode = spNodeList->nextNode() ) )
	{
		spItem = spNode->selectSingleNode( XML_ISVISIBLE );
		if( spItem != NULL && spItem->text != _bstr_t( XML_TRUE ) )
		{
			// Category should not be shown //
			continue;
		}
	
        CString csCategoryId, csCategoryDesc, csCategories, csDefCategoryDesc, csLCID, csAlphabetic;
		CString csScreenTitle(_T(""));
		CString csInstructions(_T(""));
		CString csGraphic(_T(""));
		CString csQuickPickItem(_T(""));
		bool bQuickPickItem = false;
        
        spItem = spNode->selectSingleNode( XML_ALPHABETIC );
		if( spItem != NULL && spItem->text.length() )
		{
			// Category is alphabetic //
		    csAlphabetic = (LPCTSTR)spItem->text;
		}

		//Process <IsQuickPickItem>
		spItem = spNode->selectSingleNode( XML_ISQUICKPICKITEM );
		if (spItem != NULL)
		{
			csQuickPickItem = LPCTSTR(spItem->text);
		}
		if( csQuickPickItem == XML_TRUE )
		{
			bQuickPickItem = true;
		}

		//process <Picture>
		CString csExp;
		csExp.Format(_T("%s[@%s= '%s']"), XML_PICTURE, XML_RESOLUTION, csScreenSize);
		spPictureNodeList = spNode->selectNodes((LPCTSTR)csExp);

		CString csId;
		int iId = 0;
		CString csGraphicList[2] = {_T(""), _T("")};
		bool bNewFormat = false;

		while (spSubNode = spPictureNodeList->nextNode())
		{
			spSubItem = spSubNode->attributes->getNamedItem(XML_PICTUREID);
			
			if (spSubItem !=NULL)
			{
				bNewFormat = true;
				csId = LPCTSTR(spSubItem->text);				
				iId = _ttoi(csId);

				csGraphic = (LPCTSTR)spSubNode->Gettext();
				if ( (iId>0) && (iId<=2) && csGraphic.GetLength() )
				{
					csGraphicList[iId-1] = csGraphic;
				}
			}					
		}
		if (bNewFormat)
		{
			//PLE added supports for two graphics, but CoreApp is only using "ID=1" with this release
			csGraphic = csGraphicList[0];
		}
		else //to be backward compatibile
		{
			spSubNode = spNode->selectSingleNode( (LPCTSTR)csExp );
			if (spSubNode != NULL)
			{
				csGraphic = (LPCTSTR)spSubNode->Gettext();
			}
		}
	
		spItem = spNode->attributes->getNamedItem( XML_ID );
		if( spItem != NULL && spItem->text.length() )
		{
			csCategoryId = (LPCTSTR)spItem->text;
		}

		spSubNodeList = spNode->selectNodes( XML_LANGUAGE );
		while (spSubNode = spSubNodeList->nextNode())
		{
			spSubItem = spSubNode->attributes->getNamedItem(XML_LCID);
			
			csLCID = (LPCTSTR)spSubItem->text;
			if (csLCID.CompareNoCase(csLangCode) == 0)
			{
				spSubItem = spSubNode->selectSingleNode(XML_DESCRIPTION);
				if (spSubItem != NULL && spSubItem->text.length())
				{
					csCategoryDesc = (LPCTSTR)spSubItem->text;
				}

				spSubItem = spSubNode->selectSingleNode(XML_INCATEGORIES);
				if (spSubItem != NULL)
				{
					csCategories = LPCTSTR(spSubItem->text);
				}

				spSubItem = spSubNode->selectSingleNode(XML_SCREENTITLE);
				if (spSubItem != NULL)
				{
						csScreenTitle = (LPCTSTR)spSubItem->text;
				}

				spSubItem = spSubNode->selectSingleNode(XML_INSTRUCTIONS);
				if (spSubItem != NULL)
				{
						csInstructions = (LPCTSTR)spSubItem->text;
				}

			}
			if (csLCID.CompareNoCase(csDefLangCode) == 0)
			{
				spSubItem = spSubNode->selectSingleNode(XML_DESCRIPTION);
				if (spSubItem != NULL && spSubItem->text.length())
				{
					csDefCategoryDesc = (LPCTSTR)spSubItem->text;
				}
			}

		}
		if (!csCategoryDesc.GetLength())
		{
			csCategoryDesc = csDefCategoryDesc;
		}

		if (csCategoryId.GetLength()) 
		{
			InsertInOrder( csCategoryId, csCategoryDesc, csCategories, csAlphabetic, csGraphic, csScreenTitle, csInstructions, bQuickPickItem );
		}
	}

	trace(L6,_T("-CSItemList::BuildCategoryList"));

	return 0;
}


/*<<<EmptyCategoryList>>>----------------------------------------------------------

 Remove all elements in the list.

 Entry:   none
 Return:  none

-----------------------------------------------------------------------------*/
void CSCategoryList::EmptyCategoryList()
{
	POSITION pos = GetHeadPosition();
	CCategoryInfo* pItem = NULL;
	while( pos != NULL )
	{
		CCategoryInfo* pItem = (CCategoryInfo*)GetNext( pos );
		delete pItem;
	}

	RemoveAll();
}

/*<<<InsertInOrder>>>----------------------------------------------------------

 Insert a category to the list in sorting order.

 Entry:  csId - Category Id
         csDesc - Category description
         csAlphabetic - Category is Alphabetic A1A2#
 Return:   none

-----------------------------------------------------------------------------*/
void CSCategoryList::InsertInOrder( const CString& csId, const CString& csDesc, const CString& csCategories, const CString& csAlphabetic, const CString& csGraphic, const CString& csScreenTitle, const CString& csInstructions, bool bQuickPickItem ) 
{
	CCategoryInfo *pCurrentItem = new CCategoryInfo( csId, csDesc, csCategories, csAlphabetic, csGraphic, csScreenTitle, csInstructions, bQuickPickItem );
	POSITION pos = GetHeadPosition();
	if( pos == NULL )
	{
		//add first item into the list
		InsertBefore( pos, pCurrentItem );
	}
	else
	{
		CCategoryInfo *pItem = NULL;
		POSITION CurrentPos = NULL;		
		while( pos != NULL )
		{
			CurrentPos = pos;
			pItem = (CCategoryInfo*)GetNext( pos );   // pos is the  next item position. pItem is the current category info
			if( csId == pItem->m_csId )
			{
				//skip duplicates 
				delete pCurrentItem;
				pCurrentItem = NULL;
				break;
			}
			
		    if (csId.Compare(pItem->m_csId) < 0)
			{
				InsertBefore( CurrentPos, pCurrentItem ); //Insert before the current item
				break;
			}
			if( pos == NULL )
			{   
				//at end of list
				InsertAfter( CurrentPos, pCurrentItem ); //Insert after the current item
				break;
			}
		}
	}
}



