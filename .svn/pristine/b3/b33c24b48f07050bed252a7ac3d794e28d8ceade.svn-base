//
// FILE:  PSItemList.h
//
// TITLE: Class declaration to sort the product data file
//
//
//
// AUTHOR:   Feng Zhang
// DATE:     1998.7.14
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// CHANGES: Start
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __PSITEMLIST
#define __PSITEMLIST

#include <vector>
using namespace std;
typedef CMap<CString, LPCTSTR, bool, bool> CMapStringToBool;//Japan RFC#1 (RFQ 893)

//This class include the ItemName and ItemCode
#ifndef _CPPUNIT
#include "DllDefine.h"
#else
#include "TestMacros.h"
#endif // _CPPUNIT
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 

CItemInfo : public CObject
{
public:
	CItemInfo( const CString& csDesc, 
		       const CString& csCode, 
			   const CString& csGraphic, 
			   bool bFavorite, 
			   bool bSubCategory, 
			   const CString& csCategories, 
			   const CString& csSubCategories = _T(""), 
			   const CString& csReadingName = _T(""), 
			   bool bQuickPickItem = false,
			   bool bIsItem = true,
			   const CString& csAlphabetic = _T(""));

	CString GetItemName() { return m_csItemName; };
	CString GetItemCode() { return m_csItemCode; };
	CString GetItemGraphic() { return m_csItemGraphic; };
	bool GetIsFavorite() { return m_bItemIsFavorite; };
	CString GetCategories() { return m_csCategories; };
	CString GetSubCategories() { return m_csSubCategories; };
	CString GetReadingName() { return m_csReadingName; };
	bool GetIsSubCategory() { return m_bIsSubCategory; };
	bool GetQuickPickItem() { return m_bQuickPickItem; };
	bool GetIsItem() { return m_bIsItem; };
	CString GetAlphabetic() { return m_csAlphabetic; };
 
	CString m_csItemName;
	CString m_csItemCode;
	CString m_csItemGraphic; 
	bool m_bItemIsFavorite;
	CString m_csCategories;
	CString m_csSubCategories;
	CString m_csReadingName;
	bool m_bIsSubCategory;
	bool m_bQuickPickItem;
	bool m_bIsItem;
	CString m_csAlphabetic;
};



#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
CCategoryInfo : public CObject
{
public:
	CCategoryInfo( const CString& csId, 
		           const CString& csCatDesc, 
				   const CString& csCategories,
		           const CString& csAlphabetic = _T(""), 
		           const CString& csGraphic = _T(""), 
				   const CString& csScreenTitle = _T(""), 
				   const CString& csInstructions = _T(""),
                   const CString& csInCategories = _T(""),
				   bool bQuickPickItem = false,
                   bool bSubcatItem = false);

	CString GetCategoryID() { return m_csId; };
	CString GetCatDescription() { return m_csCatDesc; };
	CString GetCategories() { return m_csCategories; };
	CString GetCatAlphabetic() { return m_csAlphabetic; };
	CString GetCatGraphic() { return m_csGraphic; };
	CString GetScreenTitle() { return m_csScreenTitle; };
	CString GetInstructions() { return m_csInstructions; };
	CString GetInCategories() { return m_csInCategories; };
	bool GetQuickPickItem() { return m_bQuickPickItem; };
    bool GetSubcatItem() { return m_bSubcatItem;}

	CString m_csId;
	CString m_csCatDesc;
	CString m_csCategories;
    CString m_csAlphabetic;
	CString m_csGraphic;
	CString m_csScreenTitle;
	CString m_csInstructions;
	CString m_csInCategories;
	bool    m_bQuickPickItem;
    bool    m_bSubcatItem;
};

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
CPickListItemInfo : public CObject
{
public:
    CPickListItemInfo();
    CPickListItemInfo(CPickListItemInfo&);
    CPickListItemInfo( const CString& csItemCode,
                   //const CString& csDepartment = _T(""),
                   bool bQuantityNeeded = false,
                   //bool bQuickPickItem = false,
                   //bool bPopular = false,
                   //bool bVisible = false,
                   //bool bDeleteThisItem = false,
                   bool bZeroWeightItem = false//,
                   //bool bTaxable = false,
                   //bool bExtra = false
                   );
    CPickListItemInfo &operator=(CPickListItemInfo&);

    CString GetItemCode() { return m_csItemCode; };
    //CString GetDepartment() { return m_csDepartment; };
    bool IsQuantityNeeded() { return m_bQuantityNeeded; };
    //bool IsQuickPickItem() { return m_bQuickPickItem; };
    //bool IsPopular() { return m_bPopular; };
    //bool IsVisible() { return m_bVisible; };
    //bool IsDeleteThisItem() { return m_bDeleteThisItem; };
    bool IsZeroWeightItem() { return m_bZeroWeightItem; };
    //bool IsTaxable() { return m_bTaxable; };
    //bool IsExtra() { return m_bExtra; };

    CString m_csItemCode;
    //CString m_csDepartment;
    bool    m_bQuantityNeeded;
    //bool    m_bQuickPickItem;
    //bool    m_bPopular;
    //bool    m_bVisible;
   // bool    m_bDeleteThisItem;
    bool    m_bZeroWeightItem;
    //bool    m_bTaxable;
    //bool    m_bExtra;
};


typedef CMap<CString, LPCTSTR, CPickListItemInfo, CPickListItemInfo> CMapStringToPickListItemInfo;

//This is the Item List

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 

CSItemList : public CObList
{
public:
	CSItemList( const CString& csFileName );
	CSItemList(); 
	~CSItemList();
	
	// methods

    int BuildList(const MSXML2::IXMLDOMDocumentPtr &spDoc,
                  const CString& csLangCode, 
                  CMapStringToPickListItemInfo* pmPickListItemInfo = NULL,
                  CStringArray* pcsPickQtyList = NULL, 
                  CMapStringToBool* pmZeroWeighItems = NULL,
                  bool bSortList = true);
    void InsertInOrder( const CString& csDesc, 
                        const CString& csCode, 
                        const CString& csGraphic, 
                        bool  bFavorite, 
                        bool  bIsSubCategory, 
                        const CString& csCategories, 
                        const CString& csSubCategories, 
                        const CString& csReadingName = _T(""), 
                        bool  bQuickPickItem = false,
                        bool  bIsItem = true, 
                        const CString& csAlphabetic = _T(""),
                        const bool bSorted = true);
    void AddToTail( const CString& csDesc, 
                   const CString& csCode, 
                   const CString& csGraphic, 
                   bool  bFavorite, 
                   bool  bIsSubCategory, 
                   const CString& csCategories, 
                   const CString& csSubCategories, 
                   const CString& csReadingName = _T(""), 
                   bool  bQuickPickItem = false,
                   bool  bIsItem = true, 
                   const CString& csAlphabetic = _T(""));

	void EmptyList(); 
    vector<CString>  GetInvisiblePicklistItemList(void) const;

	//member variables
	CString FileName;
    vector<CString> invisiblePicklistItemList;
};

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
CSCategoryList : public CObList
{
public:
    CSCategoryList(); 
    ~CSCategoryList();
    
    // methods
    void InsertInOrder( const CString& csId, 
                        const CString& csDesc, 
                        const CString& csCategories,
                        const CString& csAlphabetic = _T(""), 
                        const CString& csGraphic = _T(""), 
                        const CString& csScreenTitle = _T(""), 
                        const CString& csInstructions = _T(""), 
                        bool bQuickPickItem = false ,
                        const bool bSortedList = true,
                        const bool bSubcatItem = false);
    void AddToTail( const CString& csId, 
                        const CString& csDesc, 
                        const CString& csCategories,
                        const CString& csAlphabetic = _T(""), 
                        const CString& csGraphic = _T(""), 
                        const CString& csScreenTitle = _T(""), 
                        const CString& csInstructions = _T(""), 
                        bool bQuickPickItem = false,
                        const bool bSubcatItem = false);
    int BuildCategoryList(const MSXML2::IXMLDOMDocumentPtr &spDoc, 
                          const CString& csLangCode,
                          bool bSortedList = true);
    void EmptyCategoryList(); 	
};

#endif //_PSITEMLIST
