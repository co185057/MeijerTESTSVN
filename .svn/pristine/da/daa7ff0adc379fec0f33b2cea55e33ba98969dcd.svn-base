// dftransView.h : interface of the CDftransView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DFTRANSVIEW_H__8E5F76A1_A8AC_11D1_90AC_00609761BB2E__INCLUDED_)
#define AFX_DFTRANSVIEW_H__8E5F76A1_A8AC_11D1_90AC_00609761BB2E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "dftransDoc.h"
#include "FindDialog.h"	// Added by ClassView


#define DF_COL_INDEX         0
#define DF_COL_TIME          1
#define DF_COL_STATE         2
#define DF_COL_TB_STATE      3
#define DF_COL_TB_ID         4
#define DF_COL_TERMNUM       5
#define DF_COL_STORENUM      6

#define DF_FIND_BY_DESC      0
#define DF_FIND_BY_ID        1
#define DF_FIND_BY_TRANSNUM  2
#define DF_FIND_BY_INDEX     3

#define DF_FIND_DIR_UP       0
#define DF_FIND_DIR_DOWN     1

#define DF_NUM_SEV_BMPS      5

class CDfPrintInfo
{
public:
	BOOL IsFirstTime();
	int m_PageHeight;
	void InitDC(CDC* pDC);
	int m_DataLinesPerPage;
	BOOL ContinuePrinting(CPrintInfo* pInfo);
	void Cleanup();
	void CleanupDC(CDC* pDC);
	void PrepareDC(CDC* pDC);
	void PrepareHeaderDC(CDC* pDC);
	void Initialize(CDC* pDC, CPrintInfo* pInfo, CListCtrl& clc);
	int m_MaxLinesPerPage;
	int m_topBottomMargin;
	int m_lineHeight;
  BOOL m_bPrintSelectionOnly;
  UINT m_nNumPages;
  UINT m_nStartPage;
  UINT m_nEndPage;

  CFont* m_pDfFont;
  CFont* m_pDfHeaderFont;
  CFont* m_pOldFont;

	CDfPrintInfo();
  int GetYPos(int yPos);
  int GetFooterPos();
private:
	BOOL m_bFirstTime;
};


class CDftransView : public CListView
{
protected: // create from serialization only
	CDftransView();
	DECLARE_DYNCREATE(CDftransView)

// Attributes
public:
	CDftransDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDftransView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL
  virtual afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);

// Implementation
public:
	void ResetNextSelPrintIndex();
	void SetLastSelPrintIndex(int ndx);
	int GetNextSelPrintIndex(int curPage);
	const CString& GetPrintLine(int ndx);
	void FreeList();
	void OnShowStats();
	void UpdateSelectedItem();
	virtual ~CDftransView();
  void OnFileRefresh();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

private:
	CImageList m_SeverityImageList;
	int m_LastTopVisibleIndex;
	int m_LastBottomVisibleIndex;
	int m_MinPrintWarnItems;
	CString GetItemText( int nItem, int nSubItem );
	void SelectItem(int index, BOOL bSelected);
	void UnselectAllItems();
	CFindDialog m_FindDlg;
	int ComparePartialNoCase(CString key, CString data);
	int CompareExact(CString& key, CString& data);
	int m_FindStartIndex;
	void FindNextDynaframe(int findType, int findDirection, CString& findData, int nFindIndex = -1);
	CString m_FindData;
	int m_FindDirection;
	int m_FindType;
  CDynaTrans *selTransPtr;
  LV_ITEM selItem;
  int selItemIndex;

	CListCtrl& clc;

  CArray <CDynaTrans*, CDynaTrans*> *m_pDataList;
  CDftransDoc* m_pDoc;

  CDfPrintInfo m_dfPrintInfo;

// Generated message map functions
protected:
	unsigned int m_nMaxTBStateLength;
  CString m_csLongestTBState;
	//{{AFX_MSG(CDftransView)
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditFind();
	afx_msg void OnEditFindnext();
	afx_msg void OnUpdateEditFindnext(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
	afx_msg void OnEditFindprev();
	afx_msg void OnUpdateEditFindprev(CCmdUI* pCmdUI);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPopupFindPrev();
	afx_msg void OnUpdatePopupFindPrev(CCmdUI* pCmdUI);
	afx_msg void OnPopupFindNext();
	afx_msg void OnUpdatePopupFindNext(CCmdUI* pCmdUI);
	afx_msg void OnPopupStats();
	afx_msg void OnUpdatePopupStats(CCmdUI* pCmdUI);
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg void OnEditGotoindex();
	afx_msg void OnUpdateEditGotoindex(CCmdUI* pCmdUI);
	afx_msg void OnViewRaw();
	afx_msg void OnUpdateViewRaw(CCmdUI* pCmdUI);
	afx_msg void OnPopupFindNextTB();
	afx_msg void OnPopupFindPrevTB();
	afx_msg void OnUpdatePopupFindPrevTB(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePopupFindNextTB(CCmdUI* pCmdUI);
	afx_msg void OnPopupFindnexttypeApplicationdeath();
	afx_msg void OnPopupFindnexttypeApplicationrestart();
	afx_msg void OnPopupFindnexttypeTransaction();
	afx_msg void OnPopupFindprevtypeApplicationdeath();
	afx_msg void OnPopupFindprevtypeApplicationrestart();
	afx_msg void OnPopupFindprevtypeTransactionend();
	afx_msg void OnUpdatePopupFindnexttypeApplicationdeath(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePopupFindnexttypeApplicationrestart(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePopupFindnexttypeTransaction(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePopupFindprevtypeApplicationdeath(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePopupFindprevtypeApplicationrestart(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePopupFindprevtypeTransactionend(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in dftransView.cpp
inline CDftransDoc* CDftransView::GetDocument()
   { return (CDftransDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DFTRANSVIEW_H__8E5F76A1_A8AC_11D1_90AC_00609761BB2E__INCLUDED_)
