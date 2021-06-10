#if !defined(AFX_DMOPTIONDLG_H__B0A12AA4_EC93_11D3_BBFE_925BBB93315F__INCLUDED_)
#define AFX_DMOPTIONDLG_H__B0A12AA4_EC93_11D3_BBFE_925BBB93315F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMOptionDlg.h : header file
//

#include "resource.h"		// main symbols
#include "OptionListBox.h"

class COptionCopiableStringArray : public CStringArray
{
  public:
  COptionCopiableStringArray() {}
  COptionCopiableStringArray( const COptionCopiableStringArray& src )
  {
    *this = src;
  }
  COptionCopiableStringArray& operator=( const COptionCopiableStringArray& src )
  {
    RemoveAll();
    for ( int nIndex = 0; nIndex < src.GetSize(); nIndex++ )
      Add( src[ nIndex ] );
    return *this;
  }
};

template<>
inline UINT AFXAPI HashKey(CString& sKey)
{
  UINT nHashKey = 0;
  for ( int nIndex = 0; nIndex < sKey.GetLength(); nIndex++ )
    nHashKey += sKey[ nIndex ] * sKey[ nIndex ];
  return nHashKey;
}

/////////////////////////////////////////////////////////////////////////////
// CDMOptionDlg dialog

class CDMOptionDlg : public CDialog
{
// Construction
public:
	CDMOptionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDMOptionDlg)
	enum { IDD = IDD_DMCONFIG_OPTION };
	COptionListBox	m_listOptions;
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDMOptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDMOptionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnButtonApply();
	afx_msg void OnButtonExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();
private:
   void AddDevicesToList( LPCTSTR sClassName );
   void ReadRegistry();
   CMap< CString, CString&, COptionCopiableStringArray, COptionCopiableStringArray&>  m_MapNoBatch;
   CMap< CString, CString&, COptionCopiableStringArray, COptionCopiableStringArray&>  m_MapYesBatch;
   CString  m_sWorkingDirectory;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMOPTIONDLG_H__B0A12AA4_EC93_11D3_BBFE_925BBB93315F__INCLUDED_)
