// BaseCurrentMSR.cpp: implementation of the CBaseCurrentMSR class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseCurrentMSR.h"
#include "TraceSystem.h"

#define T_ID  _T("CBaseCurrentMSR")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseCurrentMSR::CBaseCurrentMSR()
{
}

CBaseCurrentMSR::~CBaseCurrentMSR()
{
    
}

/*************************************************************************
* Refresh - casts a CMSRMsg to a this*, get msg data into class
*
* Parameters: 
*  pMsg  - CMSRMsg *
*
* Returns: void
*************************************************************************/
void CBaseCurrentMSR::Refresh(CMSRMsg *pMsg)
{
    tbtraceIO(_T("Refresh()"));
	// lock if other threads can get here
	delete [] m_pExtensionBuf;	// delete old pointer
	*((CMSRMsg *) this) = *pMsg;  // USE DEFAULT COPY 
	m_pExtensionBuf= pMsg->GetAndForgetExtensionData();// take ownership of this pointer.
	// unlock
}
