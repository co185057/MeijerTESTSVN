// DMConfigDlg.h : header file
//

#if !defined(AFX_DMCONFIGDLG_H__C5532D17_2FF1_11D3_99C0_00A0C9EDD467__INCLUDED_)
#define AFX_DMCONFIGDLG_H__C5532D17_2FF1_11D3_99C0_00A0C9EDD467__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "resource.h"		// main symbols
#include "DeviceListBox.h"

class CDeviceCopiableStringArray : public CStringArray
{
  public:
  CDeviceCopiableStringArray() {}
  CDeviceCopiableStringArray( const CDeviceCopiableStringArray& src )
  {
    *this = src;
  }
  CDeviceCopiableStringArray& operator=( const CDeviceCopiableStringArray& src )
  {
    RemoveAll();
    for ( int nIndex = 0; nIndex < src.GetSize(); nIndex++ )
      Add( src[ nIndex ] );
    return *this;
  }
};


/////////////////////////////////////////////////////////////////////////////
// CDMConfigDlg dialog

template<>
inline UINT AFXAPI HashKey(CString& sKey)
{
  UINT nHashKey = 0;
  for ( int nIndex = 0; nIndex < sKey.GetLength(); nIndex++ )
    nHashKey += sKey[ nIndex ] * sKey[ nIndex ];
  return nHashKey;
}

class CDMConfigDlg : public CDialog
{
// Construction
public:
	CDMConfigDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDMConfigDlg)
	enum { IDD = IDD_DMCONFIG_DEVICE };
	CDeviceListBox	m_listDevices;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDMConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDMConfigDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnFileSave();
	afx_msg void OnFileExit();
	afx_msg void OnAboutbox();
	afx_msg void OnButtonUnselect();
	afx_msg void OnButtonApply();
	afx_msg void OnButtonSelect();
	afx_msg void OnButtonAllNone();
	afx_msg void OnOption();
	afx_msg void OnButtonExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
   void AddDevicesToList( LPCTSTR sClassName );
   void ReadRegistry();
   CMap< CString, CString&, CDeviceCopiableStringArray, CDeviceCopiableStringArray&>  m_MapNullBatch;
   CMap< CString, CString&, CDeviceCopiableStringArray, CDeviceCopiableStringArray&>  m_MapActualBatch;
   CMap< CString, CString&, CDeviceCopiableStringArray, CDeviceCopiableStringArray&>  m_MapNoneBatch;
   CMenu    m_cfgMenu;
   CString  m_sWorkingDirectory;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMCONFIGDLG_H__C5532D17_2FF1_11D3_99C0_00A0C9EDD467__INCLUDED_)
