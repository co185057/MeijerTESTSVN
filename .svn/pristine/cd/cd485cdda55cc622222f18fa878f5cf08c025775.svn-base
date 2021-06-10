// BaseCurrentKeylock.cpp: implementation of the CBaseCurrentKeylock class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseCurrentKeylock.h"
#include "TraceSystem.h"

#define T_ID  _T("CBaseCurrentKeylock")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseCurrentKeylock::CBaseCurrentKeylock()
{
    m_lCurrentSecurityLevel = 0;
    m_lExpectedSecurityLevel = 0;    
}

CBaseCurrentKeylock::~CBaseCurrentKeylock()
{ 
}


/*************************************************************************
* Refresh - casts a CKeylockMsg to a this*, get msg data into class
*
* Parameters: 
*  pMsg  - CKeylockMsg *
*
* Returns: void
*************************************************************************/
void CBaseCurrentKeylock::Refresh(CKeylockMsg *pMsg)
{
    tbtraceIO(_T("Refresh()"));
	// lock if other threads can get here
	delete [] m_pExtensionBuf;	// delete old pointer
	*((CKeylockMsg *) this) = *pMsg;  // USE DEFAULT COPY 
	m_pExtensionBuf= pMsg->GetAndForgetExtensionData();// take ownership of this pointer.
	// unlock
}

/*************************************************************************
* getCurrentSecurityLevel - accessor for member variable
*
* Parameters: none
*
* Returns: member variable value
*************************************************************************/
long CBaseCurrentKeylock::getCurrentSecurityLevel()
{
    return m_lCurrentSecurityLevel;
}

/*************************************************************************
* setCurrentSecurityLevel - accessor for member variable
*
* Parameters:
*  long lVal
*
* Returns: void
*************************************************************************/
void CBaseCurrentKeylock::setCurrentSecurityLevel(long lVal)
{
    m_lCurrentSecurityLevel = lVal;
}

/*************************************************************************
* getExpectedSecurityLevel - accessor for member variable
*
* Parameters: none
*
* Returns: member variable value
*************************************************************************/
long CBaseCurrentKeylock::getExpectedSecurityLevel()
{
    return m_lExpectedSecurityLevel;
}

/*************************************************************************
* setExpectedSecurityLevel - accessor for member variable
*
* Parameters:
*  long lVal
*
* Returns: void
*************************************************************************/
void CBaseCurrentKeylock::setExpectedSecurityLevel(long lVal)
{
    m_lExpectedSecurityLevel = lVal;
}
