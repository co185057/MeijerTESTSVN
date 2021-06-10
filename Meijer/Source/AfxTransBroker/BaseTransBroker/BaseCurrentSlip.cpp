// BaseCurrentSlip.cpp: implementation of the CBaseCurrentSlip class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseCurrentSlip.h"
#include "TraceSystem.h"

#define T_ID  _T("CBaseCurrentSlip")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseCurrentSlip::CBaseCurrentSlip()
{
    m_lNumRemaining = 0;
    m_szDocDescription = _T("");
    m_szDocFront = _T("");
    m_szDocBack = _T("");
}

CBaseCurrentSlip::~CBaseCurrentSlip()
{
    
}

/*************************************************************************
* ResetEndorsementInfo - reset all member data 
*
* Parameters: none
*
* Returns: void
*************************************************************************/
void CBaseCurrentSlip::ResetEndorsementInfo(void)
{
    SetNumRemaining(0);
    SetDocDescription(_T(""));
    SetDocFront(_T(""));
    SetDocBack(_T(""));
}


/*************************************************************************
* GetNumRemaining, SetNumRemaining - accessors for member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
long CBaseCurrentSlip::GetNumRemaining(void)
{
    return m_lNumRemaining;
}
 
void CBaseCurrentSlip::SetNumRemaining(long lVal)
{
    m_lNumRemaining = lVal;
}


/*************************************************************************
* GetDocDescription, SetDocDescription - accessors for member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
CString CBaseCurrentSlip::GetDocDescription(void)
{
    return m_szDocDescription;
}

void CBaseCurrentSlip::SetDocDescription(CString szVal)
{
    m_szDocDescription = szVal;
}

void CBaseCurrentSlip::SetDocDescription(LPTSTR szVal)
{
    m_szDocDescription = szVal;
}

/*************************************************************************
* GetDocFront, SetDocFront - accessors for member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
CString CBaseCurrentSlip::GetDocFront(void)
{
    return m_szDocFront;
}

void CBaseCurrentSlip::SetDocFront(CString szVal)
{
    m_szDocFront = szVal;
}

void CBaseCurrentSlip::SetDocFront(LPTSTR szVal)
{
    m_szDocFront = szVal;
}

/*************************************************************************
* GetDocBack, SetDocBack - accessors for member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
CString CBaseCurrentSlip::GetDocBack(void)
{
    return m_szDocBack;
}

void CBaseCurrentSlip::SetDocBack(CString szVal)
{
    m_szDocBack = szVal;
}

void CBaseCurrentSlip::SetDocBack(LPTSTR szVal)
{
    m_szDocBack = szVal;
}
