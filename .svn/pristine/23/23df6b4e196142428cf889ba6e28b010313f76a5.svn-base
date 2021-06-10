// BaseCurrentKeyboard.cpp: implementation of the CBaseCurrentKeyboard class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseCurrentKeyboard.h"
#include "TraceSystem.h"

#define T_ID  _T("CBaseCurrentKeyboard")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseCurrentKeyboard::CBaseCurrentKeyboard()
{
}

CBaseCurrentKeyboard::~CBaseCurrentKeyboard()
{
    
}

/*************************************************************************
* Refresh - casts a CKeyboardMsg to a this*, get msg data into class
*
* Parameters: 
*  pMsg  - CKeyboardMsg *
*
* Returns: void
*************************************************************************/
void CBaseCurrentKeyboard::Refresh(CKeyboardMsg *pMsg)
{
    tbtraceIO(_T("Refresh()"));
	// lock if other threads can get here
	delete [] m_pExtensionBuf;	// delete old pointer
	*((CKeyboardMsg *) this) = *pMsg;  // USE DEFAULT COPY 
	m_pExtensionBuf= pMsg->GetAndForgetExtensionData();// take ownership of this pointer.
	// unlock
}
