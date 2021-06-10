// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dftrans.h"
#include "OptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog

class CMyColorDialog : public CColorDialog
{
public:
  CMyColorDialog( COLORREF clrInit = 0, DWORD dwFlags = 0, CWnd* pParentWnd = NULL );
  void SetColorPointer(COLORREF* pColor) {m_pColor = pColor;}
  virtual BOOL OnColorOK();
  COLORREF* m_pColor;
};

CMyColorDialog::CMyColorDialog(COLORREF clrInit, DWORD dwFlags, CWnd* pParentWnd) : CColorDialog(clrInit, dwFlags, pParentWnd) 
{
}

BOOL CMyColorDialog::OnColorOK()
{
  *m_pColor = GetColor();
  return FALSE;
}

COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDlg)
	m_BakFileViewer = _T("");
	m_LogFileViewer = _T("");
	m_MinPrintWarnItems = 0;
	m_csAppDeathText = _T("");
	m_csAppRestartText = _T("");
	m_csTransEndText = _T("");
	m_csLaneClosedText = _T("");
	//}}AFX_DATA_INIT
}


void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDlg)
	DDX_Text(pDX, IDC_BAK_VIEWER, m_BakFileViewer);
	DDX_Text(pDX, IDC_LOG_VIEWER, m_LogFileViewer);
	DDX_Text(pDX, IDC_PRINT_WARN, m_MinPrintWarnItems);
	DDV_MinMaxInt(pDX, m_MinPrintWarnItems, 1, 2000000000);
	DDX_Text(pDX, IDC_APP_DEATH_TEXT, m_csAppDeathText);
	DDX_Text(pDX, IDC_APP_RESTART_TEXT, m_csAppRestartText);
	DDX_Text(pDX, IDC_TRANS_END_TEXT, m_csTransEndText);
	DDX_Text(pDX, IDC_LANE_CLOSED_TEXT, m_csLaneClosedText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	//{{AFX_MSG_MAP(COptionsDlg)
	ON_BN_CLICKED(IDC_BAK_BROWSE, OnBakBrowse)
	ON_BN_CLICKED(IDC_LOG_BROWSE, OnLogBrowse)
	ON_BN_CLICKED(IDC_APP_RESTART_COLOR, OnAppRestartColor)
	ON_BN_CLICKED(IDC_APP_DEATH_COLOR, OnAppDeathColor)
	ON_BN_CLICKED(IDC_LANE_CLOSED_COLOR, OnLaneClosedColor)
	ON_BN_CLICKED(IDC_TRANS_END_COLOR, OnTransEndColor)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg message handlers

void COptionsDlg::OnBakBrowse() 
{
  OnBrowse(IDC_BAK_VIEWER, "BAK");
}

void COptionsDlg::OnLogBrowse() 
{
  OnBrowse(IDC_LOG_VIEWER, "STF");
}

void COptionsDlg::OnBrowse(int dlgItem, CString extension)
{
  CString prompt;
  prompt.Format("Select a .%s file viewer", extension);

  CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                  "Executables (*.exe;*.com;*.cmd;*.bat)|*.exe;*.com;*.cmd;*.bat||");
  dlg.m_ofn.lpstrTitle = prompt;
  if (dlg.DoModal() == IDOK)
  {
    SetDlgItemText(dlgItem, dlg.GetPathName());
  }    

}

void COptionsDlg::OnAppRestartColor() 
{
	OnTextColor(m_AppRestartColor, IDC_APP_RESTART_TEXT);
}

void COptionsDlg::OnAppDeathColor() 
{
	OnTextColor(m_AppDeathColor, IDC_APP_DEATH_TEXT);
}

void COptionsDlg::OnLaneClosedColor() 
{
	OnTextColor(m_LaneClosedColor, IDC_LANE_CLOSED_TEXT);
}

void COptionsDlg::OnTransEndColor() 
{
	OnTextColor(m_TransEndColor, IDC_TRANS_END_TEXT);
}

void COptionsDlg::OnTextColor(COLORREF &textColorRef, UINT nCtrlID)
{
  CMyColorDialog dlg(textColorRef, CC_ANYCOLOR | CC_PREVENTFULLOPEN | CC_RGBINIT);

  dlg.SetColorPointer(&textColorRef);
  if (IDOK == dlg.DoModal())
  {
    textColorRef = dlg.GetColor();
    GetDlgItem(nCtrlID)->RedrawWindow();
  }
}

HBRUSH COptionsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
  if (nCtlColor == CTLCOLOR_EDIT)
  {
    switch (pWnd->GetDlgCtrlID())
    {
      case IDC_APP_DEATH_TEXT:   pDC->SetTextColor(m_AppDeathColor);
                                 break;
      case IDC_APP_RESTART_TEXT: pDC->SetTextColor(m_AppRestartColor);
                                 break;
      case IDC_TRANS_END_TEXT:   pDC->SetTextColor(m_TransEndColor);
                                 break;
      case IDC_LANE_CLOSED_TEXT: pDC->SetTextColor(m_LaneClosedColor);
                                 break;
    }
  }

  return hbr;
}
