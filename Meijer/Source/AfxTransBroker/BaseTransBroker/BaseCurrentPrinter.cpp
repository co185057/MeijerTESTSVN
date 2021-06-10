// BaseCurrentPrinter.cpp: implementation of the CBaseCurrentPrinter class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseCurrentPrinter.h"
#include "TraceSystem.h"
#include "TBUtilities.h"

#define T_ID  _T("CBaseCurrentPrinter")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseCurrentPrinter::CBaseCurrentPrinter()
{
    m_szPrinterError = _T("");
}

CBaseCurrentPrinter::~CBaseCurrentPrinter()
{    
}

/*************************************************************************
* Refresh - casts a CPrinterMsg to a this*, get msg data into class
*
* Parameters: 
*  pMsg  - CPrinterMsg *
*
* Returns: void
*************************************************************************/
void CBaseCurrentPrinter::Refresh(CPrinterMsg *pMsg)
{
    tbtraceIO(_T("Refresh()"));
	// lock if other threads can get here
	delete [] m_pExtensionBuf;	// delete old pointer
	*((CPrinterMsg *) this) = *pMsg;  // USE DEFAULT COPY 
	m_pExtensionBuf= pMsg->GetAndForgetExtensionData();// take ownership of this pointer.
	// unlock
}

/*************************************************************************
* GetPrinterError - TBAPI, populates parameter with printer error
*
* Parameters: 
*  LPTSTR* psErrorMessage - will contain printer error
*
* Returns: TBRC
*************************************************************************/
TBRC CBaseCurrentPrinter::GetPrinterError(LPTSTR* psErrorMessage)
{
    tbtraceIO(_T("GetPrinterError()"));
    CopyString(*psErrorMessage, m_szPrinterError);
    return TB_SUCCESS;
}

/*************************************************************************
* SetPrinterError - accessor for member variable
*
* Parameters: 
*  CString szError - new printer error message
*
* Returns: void
*************************************************************************/
void CBaseCurrentPrinter::SetPrinterError(CString szError)
{
    tbtrace( TM_ENTRY, _T("+- CBaseCurrentPrinter::SetPrinterError(%s)"), szError);
    m_szPrinterError = szError;
}
