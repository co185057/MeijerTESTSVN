// OptionListBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "OptionListBox.h"
#include "OptionChildDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionListBox

COptionListBox::COptionListBox()
{
}

COptionListBox::~COptionListBox()
{
}


BEGIN_MESSAGE_MAP(COptionListBox, CListBox)
	//{{AFX_MSG_MAP(COptionListBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionListBox message handlers

void COptionListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	// all items are of fixed size
	// must use LBS_OWNERDRAWVARIABLE for this to work
	lpMIS->itemHeight = 30;
}

void COptionListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	COptionChildDialog*pDlg = (COptionChildDialog*)lpDIS->itemData; 
  if ( pDlg != NULL )
  {
    pDlg->SetWindowPos( NULL, lpDIS->rcItem.left, lpDIS->rcItem.top, 
    lpDIS->rcItem.right - lpDIS->rcItem.left, 
    lpDIS->rcItem.bottom - lpDIS->rcItem.top, SWP_NOZORDER );
    pDlg->ShowWindow( SW_SHOW ); 
  }
}

int COptionListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCIS)
{
	COptionChildDialog*pDlg1 = (COptionChildDialog*)lpCIS->itemData1;
	COptionChildDialog*pDlg2 = (COptionChildDialog*)lpCIS->itemData2;
	return pDlg1->m_sClassName.CompareNoCase( pDlg2->m_sClassName );
}






