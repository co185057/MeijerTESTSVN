// OptionChildDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"		// main symbols
#include "OptionChildDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HBRUSH COptionChildDialog::m_hBrushButton = NULL;

/////////////////////////////////////////////////////////////////////////////
// COptionChildDialog dialog


COptionChildDialog::COptionChildDialog(CWnd* pParent /*=NULL*/)
	: CDialog(COptionChildDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionChildDialog)
	m_sClassName = _T("");
	m_nSelection = -1;
	//}}AFX_DATA_INIT
}


void COptionChildDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionChildDialog)
	DDX_Text(pDX, IDC_STATIC_NAME, m_sClassName);
	DDX_Radio(pDX, IDC_RADIO_NO, m_nSelection);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionChildDialog, CDialog)
	//{{AFX_MSG_MAP(COptionChildDialog)
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
// COptionChildDialog message handlers

BOOL COptionChildDialog::OnEraseBkgnd( CDC* pDC )
{                                                 
  return CDialog::OnEraseBkgnd( pDC );
}

LRESULT COptionChildDialog::OnCtlColorDlg(WPARAM wParam, LPARAM lParam)
{
  if ( m_hBrushButton == NULL )
    m_hBrushButton = (HBRUSH)GetStockObject(WHITE_BRUSH) ;
  return (LRESULT )m_hBrushButton;
}

