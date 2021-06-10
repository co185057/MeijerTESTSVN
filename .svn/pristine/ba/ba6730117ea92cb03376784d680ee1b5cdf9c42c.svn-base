// BaseCurrentMicr.cpp: implementation of the CBaseCurrentMicr class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseCurrentMicr.h"
#include "TraceSystem.h"

#define T_ID  _T("CBaseCurrentMicr")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseCurrentMicr::CBaseCurrentMicr()
{
}

CBaseCurrentMicr::~CBaseCurrentMicr()
{
    
}

/*************************************************************************
* Refresh - casts a CMicrMsg to a this*, get msg data into class
*
* Parameters: 
*  pMsg  - CMicrMsg *
*
* Returns: void
*************************************************************************/
void CBaseCurrentMicr::Refresh(CMicrMsg *pMsg)
{
    tbtraceIO(_T("Refresh()"));
	// lock if other threads can get here
	delete [] m_pExtensionBuf;	// delete old pointer
	*((CMicrMsg *) this) = *pMsg;  // USE DEFAULT COPY 
	m_pExtensionBuf= pMsg->GetAndForgetExtensionData();// take ownership of this pointer.
	// unlock
}
