// DFStatsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dftrans.h"
#include "DFStatsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDFStatsDlg dialog


CDFStatsDlg::CDFStatsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDFStatsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDFStatsDlg)
	m_AvgTime = _T("");
	m_NumTimes = 0;
	m_ID = 0;
	m_Name = _T("");
	m_MaxTime = _T("");
	m_MinTime = _T("");
	m_TotalTime = _T("");
	//}}AFX_DATA_INIT
}


void CDFStatsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDFStatsDlg)
	DDX_Text(pDX, IDC_AVGTIME, m_AvgTime);
	DDV_MaxChars(pDX, m_AvgTime, 100);
	DDX_Text(pDX, IDC_DFNUMTIMES, m_NumTimes);
	DDX_Text(pDX, IDC_DFID, m_ID);
	DDX_Text(pDX, IDC_DFNAME, m_Name);
	DDX_Text(pDX, IDC_MAXTIME, m_MaxTime);
	DDX_Text(pDX, IDC_MINTIME, m_MinTime);
	DDX_Text(pDX, IDC_TOTALTIME, m_TotalTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDFStatsDlg, CDialog)
	//{{AFX_MSG_MAP(CDFStatsDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDFStatsDlg message handlers
