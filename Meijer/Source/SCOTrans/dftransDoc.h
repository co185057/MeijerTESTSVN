// dftransDoc.h : interface of the CDftransDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DFTRANSDOC_H__8E5F769F_A8AC_11D1_90AC_00609761BB2E__INCLUDED_)
#define AFX_DFTRANSDOC_H__8E5F769F_A8AC_11D1_90AC_00609761BB2E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "DynaTrans.h"

class CDftransDoc : public CDocument
{
protected: // create from serialization only
	CDftransDoc();
	DECLARE_DYNCREATE(CDftransDoc)

// Attributes
private:
	long m_FileLength;
   CArray <CDynaTrans*, CDynaTrans*> m_DataList;

// Operations
public:
   CArray <CDynaTrans*, CDynaTrans*> *GetTransactionList();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDftransDoc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetScreenTransName();
	void FreeDataList();
	virtual ~CDftransDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
	//{{AFX_MSG(CDftransDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DFTRANSDOC_H__8E5F769F_A8AC_11D1_90AC_00609761BB2E__INCLUDED_)
