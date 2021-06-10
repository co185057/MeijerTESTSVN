#if !defined(AFX_COLOREDIT_H__CBCE1BB7_FA8C_11D3_AD78_00A0C9EDD523__INCLUDED_)
#define AFX_COLOREDIT_H__CBCE1BB7_FA8C_11D3_AD78_00A0C9EDD523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorEdit window

class CColorEdit : public CEdit
{
// Construction
public:
	CColorEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorEdit)
	//}}AFX_VIRTUAL
public:
  BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM Param, LRESULT* pLResult);
   
// Implementation
public:
	void SetCurrentColor(COLORREF& color);
	virtual ~CColorEdit();

	// Generated message map functions
protected:
	COLORREF m_CurrentColor;
	//{{AFX_MSG(CColorEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
  
  static CBrush m_brush;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOREDIT_H__CBCE1BB7_FA8C_11D3_AD78_00A0C9EDD523__INCLUDED_)
