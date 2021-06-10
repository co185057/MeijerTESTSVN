// ColorEdit.cpp : implementation file
//

#include "stdafx.h"
#include "dftrans.h"
#include "ColorEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorEdit

CColorEdit::CColorEdit()
{
}

CColorEdit::~CColorEdit()
{
}


BEGIN_MESSAGE_MAP(CColorEdit, CEdit)
	//{{AFX_MSG_MAP(CColorEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorEdit message handlers
CBrush CColorEdit::m_brush(RGB(0,0,0));

BOOL CColorEdit::OnChildNotify(UINT message, WPARAM wParam,
                            LPARAM lParam, LRESULT* pLResult)
{
  // If "message" is not the message you're after, do default processing:

  if (message != WM_CTLCOLOREDIT)
  {
     return CEdit::OnChildNotify(message,wParam,lParam,pLResult);
  }

  // Set the text foreground to red:
  HDC hdcChild = (HDC)wParam;
  SetTextColor(hdcChild, m_CurrentColor);
  m_brush.CreateSolidBrush(m_CurrentColor);

  // Send what would have been the return value of OnCtlColor() - the
  // brush handle - back in pLResult:
  *pLResult = (LRESULT)(m_brush.GetSafeHandle());

// Return TRUE to indicate that the message was handled:
  return TRUE;
} 

void CColorEdit::SetCurrentColor(COLORREF &color)
{
  m_CurrentColor = color;
}
