// SR Personalization Digital Receipt
// Added file to handle NCRSSCODigitalReceiptGen.ocx ActiveX Control Machine generated IDispatch wrapper class
// and insert the ActiveX control dynamically.

#include "stdafx.h"
#include <fcntl.h>
#include <stdio.h>

#include "CommonNCR.h"
#include "CommonServices.h"

#define IDW_DIGRECWINDOW                101

#include "DRProcedures.h"


DRProcedures::DRProcedures()
{
	m_bCreated = false;
}

DRProcedures::~DRProcedures()
{
	UnInitialize();
}


bool DRProcedures::Initialize(CWnd* pWnd)
{
	bool rc = true;

	if( !m_bCreated )
	{
		BOOL bRC;

		if( pWnd == NULL )
		{
			pWnd = AfxGetMainWnd(); // Get the active main window CSCOTAPPDlg
		}
		if( pWnd )
		{
            // CWnd::Create() return nonzero if successful; otherwise 0
            // Create a control in the active main window CSCOTAPPDlg
			bRC = Create(NULL,
                   _T("SCOTDRProcedures"), 
                   WS_DISABLED, 
                   CRect(0,0,0,0), 
                   pWnd, 
                   IDW_DIGRECWINDOW );
		}
		if( bRC )
		{
            // Insert the ActiveX control in the active main window CSCOTAPPDlg
			if ( m_DigitalReceipt.Create(NULL, NULL, CRect(0,0,0,0), this, 102) )
			{
				m_bCreated = true;
			}
			else
			{
				rc = false;
			}
		}
        else
        {
            rc = false;
        }
	}

	return rc;
}


void DRProcedures::UnInitialize(void)
{
	if (m_bCreated)
	{
		m_DigitalReceipt.DestroyWindow();
		m_bCreated = false;    
	}
}

//properties
CString DRProcedures::GetHTMLFont()
{
	return m_DigitalReceipt.GetHTMLFont();
}
void DRProcedures::SetHTMLFont(LPCTSTR lpszNewValue)
{
	m_DigitalReceipt.SetHTMLFont(lpszNewValue);
}

CString DRProcedures::GetTopLogo()
{
	return m_DigitalReceipt.GetTopLogo();
}

void DRProcedures::SetTopLogo(LPCTSTR lpszNewValue)
{
	m_DigitalReceipt.SetTopLogo(lpszNewValue);
}

CString DRProcedures::GetBottomLogo()
{
	return m_DigitalReceipt.GetBottomLogo();
}
void DRProcedures::SetBottomLogo(LPCTSTR lpszNewValue)
{
	m_DigitalReceipt.SetBottomLogo(lpszNewValue);
}

CString DRProcedures::GetPresetBitmap1()
{
	return m_DigitalReceipt.GetPresetBitmap1();
}
void DRProcedures::SetPresetBitmap1(LPCTSTR lpszNewValue)
{
	m_DigitalReceipt.SetPresetBitmap1(lpszNewValue);
}

CString DRProcedures::GetPresetBitmap2()
{
	return m_DigitalReceipt.GetPresetBitmap2();
}
void DRProcedures::SetPresetBitmap2(LPCTSTR lpszNewValue)
{
	m_DigitalReceipt.SetPresetBitmap2(lpszNewValue);
}

CString DRProcedures::GetLastReceiptFilename()
{
	return m_DigitalReceipt.GetLastReceiptFilename();
}
void DRProcedures::SetLastReceiptFilename(LPCTSTR lpszNewValue)
{
	m_DigitalReceipt.SetLastReceiptFilename(lpszNewValue);
}

CString DRProcedures::GetFormattedReceiptFilename()
{
	return m_DigitalReceipt.GetFormattedReceiptFilename();
}
void DRProcedures::SetFormattedReceiptFilename(LPCTSTR lpszNewValue)
{
	m_DigitalReceipt.SetFormattedReceiptFilename(lpszNewValue);
}


//methods
CString DRProcedures::DigitalReceipt()
{
    return m_DigitalReceipt.GenerateDigitalReceipt();
}



