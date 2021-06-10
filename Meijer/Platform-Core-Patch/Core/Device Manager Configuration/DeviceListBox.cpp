// DeviceListBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "DeviceListBox.h"
#include "DeviceChildDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeviceListBox

CDeviceListBox::CDeviceListBox()
{
}

CDeviceListBox::~CDeviceListBox()
{
}


BEGIN_MESSAGE_MAP(CDeviceListBox, CListBox)
	//{{AFX_MSG_MAP(CDeviceListBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeviceListBox message handlers


void CDeviceListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	// all items are of fixed size
	// must use LBS_OWNERDRAWVARIABLE for this to work
	lpMIS->itemHeight = 30;
}

void CDeviceListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CDeviceChildDialog*pDlg = (CDeviceChildDialog*)lpDIS->itemData; 
  if ( pDlg != NULL )
  {
    pDlg->SetWindowPos( NULL, lpDIS->rcItem.left, lpDIS->rcItem.top, 
    lpDIS->rcItem.right - lpDIS->rcItem.left, 
    lpDIS->rcItem.bottom - lpDIS->rcItem.top, SWP_NOZORDER );
    pDlg->ShowWindow( SW_SHOW ); 
  }
}

int CDeviceListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCIS)
{
	CDeviceChildDialog*pDlg1 = (CDeviceChildDialog*)lpCIS->itemData1;
	CDeviceChildDialog*pDlg2 = (CDeviceChildDialog*)lpCIS->itemData2;
	return pDlg1->m_sClassName.CompareNoCase( pDlg2->m_sClassName );
}

