// GotoDialog.cpp : implementation file
//

#include "stdafx.h"
#include "dftrans.h"
#include "GotoDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GotoDialog dialog


CGotoDialog::CGotoDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CGotoDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGotoDialog)
	m_GotoIndex = _T("");
	//}}AFX_DATA_INIT
}


void CGotoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGotoDialog)
	DDX_Text(pDX, IDC_GOTONDX, m_GotoIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGotoDialog, CDialog)
	//{{AFX_MSG_MAP(CGotoDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GotoDialog message handlers
