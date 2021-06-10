// FindDialog.cpp : implementation file
//

#include "stdafx.h"
#include "dftrans.h"
#include "FindDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//
// This whole thing will be confusing until I rename everything.
// For now, Dynaframe Name means "SCOT State Name",
//          Dynaframe Num. means "TB State Name", and
//          Trans.    Num. means "TB State ID#".
//
// I will clean it up later.

/////////////////////////////////////////////////////////////////////////////
// CFindDialog dialog


CFindDialog::CFindDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFindDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindDialog)
	m_FindType = -1;
	m_DynaName = _T("");
	m_FindDirection = -1;
	m_DynaId = _T("");
	m_DynaTransNum = _T("");
	//}}AFX_DATA_INIT
}


void CFindDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindDialog)
	DDX_Control(pDX, IDC_DYNATNUMCOMBO, m_DynaTransNumComboBox);
	DDX_Control(pDX, IDC_DYNANAMECOMBO, m_DynaNameComboBox);
	DDX_Control(pDX, IDC_DYNAIDCOMBO, m_DynaIdComboBox);
	DDX_Radio(pDX, IDC_DYNANAME, m_FindType);
	DDX_CBString(pDX, IDC_DYNANAMECOMBO, m_DynaName);
	DDX_Radio(pDX, IDC_RADIO_UP, m_FindDirection);
	DDX_CBString(pDX, IDC_DYNAIDCOMBO, m_DynaId);
	DDX_CBString(pDX, IDC_DYNATNUMCOMBO, m_DynaTransNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindDialog, CDialog)
	//{{AFX_MSG_MAP(CFindDialog)
	ON_BN_CLICKED(IDC_DYNANAME, OnDynaname)
	ON_BN_CLICKED(IDC_DYNAID, OnDynaid)
	ON_BN_CLICKED(IDC_DYNATRANSNUM, OnDynatransnum)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindDialog message handlers

void CFindDialog::SetDynaframeID(CString& ID)
{
  m_DynaId = ID;
}

void CFindDialog::SetDynaframeTransNum(CString& transNum)
{
  m_DynaTransNum = transNum;
}

void CFindDialog::SetDynaframeName(CString& name)
{
  m_DynaName = name;
}

BOOL CFindDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

#if 0
  CWnd *editWindow = m_DynaIdComboBox.GetWindow(GW_CHILD);
  if (editWindow != NULL) 
    editWindow->ModifyStyle(0, ES_NUMBER);
#endif

  CWnd *editWindow = m_DynaTransNumComboBox.GetWindow(GW_CHILD);
  if (editWindow != NULL) 
    editWindow->ModifyStyle(0, ES_NUMBER);

  if (m_FindType == 0)
  {
    OnDynaname();
  }
  else if (m_FindType == 1)
  {
    OnDynaid();
  }
  else if (m_FindType == 2)
  {
    OnDynatransnum();
  }

  AddUnique(m_DynaName,     m_DynaNameComboBox,     m_NameList);
  AddUnique(m_DynaId,       m_DynaIdComboBox,       m_IdList);
  AddUnique(m_DynaTransNum, m_DynaTransNumComboBox, m_TransNumList);

  POSITION pos = m_NameList.GetHeadPosition();

  while(pos)
  {
    m_DynaNameComboBox.AddString(m_NameList.GetNext(pos));
  }

  pos = m_IdList.GetHeadPosition();

  while(pos)
  {
    m_DynaIdComboBox.AddString(m_IdList.GetNext(pos));
  }

  pos = m_TransNumList.GetHeadPosition();

  while(pos)
  {
    m_DynaTransNumComboBox.AddString(m_TransNumList.GetNext(pos));
  }

  return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFindDialog::OnOK() 
{
  UpdateData(TRUE);

  AddUnique(m_DynaName,     m_DynaNameComboBox,     m_NameList);
  AddUnique(m_DynaId,       m_DynaIdComboBox,       m_IdList);
  AddUnique(m_DynaTransNum, m_DynaTransNumComboBox, m_TransNumList);

	CDialog::OnOK();
}

void CFindDialog::OnDynaname() 
{
  m_DynaIdComboBox.EnableWindow(FALSE);
  m_DynaTransNumComboBox.EnableWindow(FALSE);
  m_DynaNameComboBox.EnableWindow(TRUE);
  m_DynaNameComboBox.SetFocus();
}

void CFindDialog::OnDynaid() 
{
  m_DynaIdComboBox.EnableWindow(TRUE);
  m_DynaTransNumComboBox.EnableWindow(FALSE);
  m_DynaNameComboBox.EnableWindow(FALSE);
  m_DynaIdComboBox.SetFocus();
}

void CFindDialog::OnDynatransnum() 
{
  m_DynaIdComboBox.EnableWindow(FALSE);
  m_DynaTransNumComboBox.EnableWindow(TRUE);
  m_DynaNameComboBox.EnableWindow(FALSE);
  m_DynaTransNumComboBox.SetFocus();
}

void CFindDialog::AddUnique(CString & string, 
                            CComboBox & comboBox,
                            CList<CString, CString&>& list)
{
  if ((string != "") && (!FindInList(string, list)))
  {
    list.AddTail(string);
  }
}

BOOL CFindDialog::FindInList(CString & string, CList<CString, CString&>& list)
{
  POSITION pos = list.GetHeadPosition();
  BOOL bFound = FALSE;

  while(pos && !bFound)
  {
    if ((list.GetNext(pos)).Compare(string) == 0)
    {
      bFound = TRUE;
    }
  }
  return bFound;
}
