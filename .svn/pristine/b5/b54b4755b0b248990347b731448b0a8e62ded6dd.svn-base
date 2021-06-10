// DeviceChildDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "DeviceChildDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HBRUSH CDeviceChildDialog::m_hBrushButton = NULL;

/////////////////////////////////////////////////////////////////////////////
// CDeviceChildDialog dialog


CDeviceChildDialog::CDeviceChildDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDeviceChildDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeviceChildDialog)
	m_sClassName = _T("");
	m_nSelection = -1;
	//}}AFX_DATA_INIT
}


void CDeviceChildDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeviceChildDialog)
	DDX_Text(pDX, IDC_STATIC_NAME, m_sClassName);
	DDX_Radio(pDX, IDC_RADIO_NONE, m_nSelection);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeviceChildDialog, CDialog)
	//{{AFX_MSG_MAP(CDeviceChildDialog)
		// NOTE: the ClassWizard will add message map macros here
   ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CTLCOLORDLG, OnCtlColorDlg)
	ON_MESSAGE(WM_CTLCOLORSTATIC, OnCtlColorDlg)
	ON_MESSAGE(WM_CTLCOLOREDIT, OnCtlColorDlg)
	ON_MESSAGE(WM_CTLCOLORBTN, OnCtlColorDlg)
	ON_MESSAGE(WM_CTLCOLORLISTBOX, OnCtlColorDlg)
	ON_MESSAGE(WM_CTLCOLORDLG, OnCtlColorDlg)
	ON_MESSAGE(WM_CTLCOLORMSGBOX, OnCtlColorDlg)
	ON_MESSAGE(WM_CTLCOLORSCROLLBAR, OnCtlColorDlg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeviceChildDialog message handlers

BOOL CDeviceChildDialog::OnEraseBkgnd( CDC* pDC )
{                                                 
  return CDialog::OnEraseBkgnd( pDC );
}


LRESULT CDeviceChildDialog::OnCtlColorDlg(WPARAM wParam, LPARAM lParam)
{
  if ( m_hBrushButton == NULL )
    m_hBrushButton = (HBRUSH)GetStockObject(WHITE_BRUSH) ;
  return (LRESULT )m_hBrushButton;
}

