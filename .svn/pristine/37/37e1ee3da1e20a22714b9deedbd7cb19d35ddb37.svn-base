// LoadingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dftrans.h"
#include "LoadingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoadingDlg dialog


CLoadingDlg::CLoadingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoadingDlg)
	m_MsgTxt = _T("");
	//}}AFX_DATA_INIT
  m_Interval = 1;
}


void CLoadingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoadingDlg)
	DDX_Control(pDX, IDC_LOADPROGRESS, m_LoadProgress);
	DDX_Text(pDX, IDC_MSGTXT, m_MsgTxt);
	DDV_MaxChars(pDX, m_MsgTxt, 256);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoadingDlg, CDialog)
	//{{AFX_MSG_MAP(CLoadingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoadingDlg message handlers

BOOL CLoadingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

  m_LoadProgress.SetRange(0,100);
  m_LoadProgress.SetPos(0);
	
  CenterWindow(GetParent());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLoadingDlg::InitProgress(int numItems)
{
  if (numItems <= 100)
  {
    m_Interval = 1;
    m_LoadProgress.SetRange(0, numItems);
  }
  else
  {
    m_Interval = numItems / 100;
    m_LoadProgress.SetRange(0, 100);
  }

  m_LoadProgress.SetPos(0);
  m_LoadProgress.SetStep(1);

}

void CLoadingDlg::StepIt(int currentItem)
{
  if ((m_Interval == 1) || ((currentItem % m_Interval) == 0))
    m_LoadProgress.StepIt();
}

void CLoadingDlg::InitVariantProgress(int size)
{
  m_LoadProgress.SetRange(0, size);
  m_LoadProgress.SetPos(0);
  m_LoadProgress.SetStep(1);
}

void CLoadingDlg::OffsetPos(int currentPos)
{
  m_LoadProgress.OffsetPos(currentPos);
}
