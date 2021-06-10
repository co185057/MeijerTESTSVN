// BaseCurrentOptions.cpp: implementation of the CBaseCurrentOptions class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseCurrentOptions.h"
#include "TraceSystem.h"

#define T_ID  _T("CBaseCurrentOptions")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseCurrentOptions::CBaseCurrentOptions()
{
    m_nTerminalNum = 0;
    m_nVersionNum = 0;
    m_szVersionString = _T("");
    m_lVerBufLen = 0;
}

CBaseCurrentOptions::~CBaseCurrentOptions()
{
}

/*************************************************************************
* GetTerminalNumber, SetTerminalNumber - accessors for member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
long CBaseCurrentOptions::GetTerminalNumber()
{
   return m_nTerminalNum;
}

void CBaseCurrentOptions::SetTerminalNumber(long lVal)
{
   m_nTerminalNum = lVal;
}

/*************************************************************************
* GetVersionNumber, SetVersionNumber - accessors for member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
long CBaseCurrentOptions::GetVersionNumber()
{
   return m_nVersionNum;
}

void CBaseCurrentOptions::SetVersionNumber(long lVal)
{
    m_nVersionNum = lVal;
}

/*************************************************************************
* GetVersionString, GetVersionString - accessors for member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
LPTSTR CBaseCurrentOptions::GetVersionString()
{
   return m_szVersionString.GetBuffer(0);
}

void CBaseCurrentOptions::SetVersionString(CString szVal)
{
   m_szVersionString = szVal;
   SetVersionBufferLength(m_szVersionString.GetLength());
}

/*************************************************************************
* GetVersionStringBufferLength, SetVersionStringBufferLength - accessors for member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
long CBaseCurrentOptions::GetVersionStringBufferLength()
{
   return m_lVerBufLen;
}

void CBaseCurrentOptions::SetVersionBufferLength(long lVal)
{
   m_lVerBufLen = lVal;
}
