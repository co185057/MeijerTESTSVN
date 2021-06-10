// BaseCurrentFiscalPrinter.cpp: implementation of the CBaseCurrentFiscalPrinter class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseCurrentFiscalPrinter.h"
#include "TraceSystem.h"

#define T_ID  _T("CBaseCurrentMSR")


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseCurrentFiscalPrinter::CBaseCurrentFiscalPrinter()
{

}

CBaseCurrentFiscalPrinter::~CBaseCurrentFiscalPrinter()
{

}

/*************************************************************************
* Refresh - casts a CFiscalPrinterMsg to a this*, get msg data into class
*
* Parameters: 
*  pMsg  - CFiscalPrinterMsg *
*
* Returns: void
*************************************************************************/
void CBaseCurrentFiscalPrinter::Refresh(CFiscalPrinterMsg *pMsg)
{
    tbtraceIO(_T("Refresh()"));
	// lock if other threads can get here
	delete [] m_pExtensionBuf;	// delete old pointer
	*((CFiscalPrinterMsg *) this) = *pMsg;  // USE DEFAULT COPY 
	m_pExtensionBuf= pMsg->GetAndForgetExtensionData();// take ownership of this pointer.
	// unlock
}
