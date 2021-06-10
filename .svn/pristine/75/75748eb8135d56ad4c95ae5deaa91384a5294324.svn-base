// BaseCurrentTender.cpp: implementation of the CBaseCurrentTender class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseCurrentTender.h"
#include "TraceSystem.h"

#define T_ID  _T("CBaseCurrentTender")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseCurrentTender::CBaseCurrentTender():
m_csCardData(_T("")),
m_csPIN(_T("")),
m_bOverride(false)
{
}

CBaseCurrentTender::~CBaseCurrentTender()
{
}

/*************************************************************************
* Refresh - casts a CTenderDetailsMsg to a this*, get msg data into class
*
* Parameters: 
*  pMsg  - CTenderDetailsMsg *
*
* Returns: void
*************************************************************************/
void CBaseCurrentTender::Refresh(CTenderDetailsMsg *pMsg)
{
    tbtraceIO(_T("Refresh()"));
    // lock if other threads can get here
    delete [] m_pExtensionBuf;	// delete old pointer
    *((CTenderDetailsMsg *) this) = *pMsg;  // USE DEFAULT COPY 
    m_pExtensionBuf= pMsg->GetAndForgetExtensionData();// take ownership of this pointer.
    // unlock
}

/*************************************************************************
* Reset - reset all member data 
*
* Parameters: none
*
* Returns: void
*************************************************************************/
void CBaseCurrentTender::Reset( void) 
{
    tbtraceIO(_T("Reset()"));
    
    CTenderDetailsMsg::Reset();
    SetCardData(_T(""));
    SetPIN(_T(""));
    SetOverride(false);
}

/*************************************************************************
* GetCardData, SetCardData - accessors for member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
CString CBaseCurrentTender::GetCardData( void)
{
    return m_csCardData;
}

void CBaseCurrentTender::SetCardData( LPCTSTR szCardData)
{
    m_csCardData= szCardData;    
}

/*************************************************************************
* GetPIN, SetPIN - accessors for member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
CString CBaseCurrentTender::GetPIN( void)
{
    return m_csPIN;
}

void CBaseCurrentTender::SetPIN( LPCTSTR szPIN)
{
    m_csPIN = szPIN;
}

/*************************************************************************
* GetOverride, SetOverride - accessors for member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
BOOL CBaseCurrentTender::GetOverride( void)
{
    return m_bOverride;
}

void CBaseCurrentTender::SetOverride( BOOL bOverride)
{
    m_bOverride = bOverride;
}


