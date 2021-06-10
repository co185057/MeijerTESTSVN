//////////////////////////////////////////////////////////////////////
//
// BaseCurrentDisplay.cpp: implementation of the CBaseCurrentDisplay class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseCurrentDisplay.h"
#include "TraceSystem.h"

#define T_ID    _T("BASECurrentDISPLAY")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBaseCurrentDisplay::CBaseCurrentDisplay()
{
    Reset2x20();
}

CBaseCurrentDisplay::~CBaseCurrentDisplay()
{
}

/*************************************************************************
* Get2x20LowerLine - accessor for member variable
*
* Parameters: none
*
* Returns: CString - m_cs2x20LowerLine
*************************************************************************/
CString CBaseCurrentDisplay::Get2x20LowerLine(void)
{
    return m_cs2x20LowerLine;
}

/*************************************************************************
* Get2x20UpperLine - accessor for member variable
*
* Parameters: none
*
* Returns: CString - m_cs2x20UpperLine
*************************************************************************/
CString CBaseCurrentDisplay::Get2x20UpperLine(void)
{
    return m_cs2x20UpperLine;
}

/*************************************************************************
* Reset2x20 - reset 2x20 display member variables
*
* Parameters: none
*
* Returns: void
*************************************************************************/
void CBaseCurrentDisplay::Reset2x20()
{
    tbtraceIO(_T("Reset2x20()"));
    m_cs2x20UpperLine.Empty();
    m_cs2x20LowerLine.Empty();
}

/*************************************************************************
* Set2x20 - set 2x20 display member variables
*
* Parameters:
*  csUpperLine - CString containing upper 1x20 display
*  csLowerLine - CString containing lower 1x20 display
*
* Returns: void
*************************************************************************/
void CBaseCurrentDisplay::Set2x20(CString csUpperLine, CString csLowerLine)
{
    tbtraceIO(_T("Set2x20()"));
    
    Set2x20UpperLine(csUpperLine);
    Set2x20LowerLine(csLowerLine);
}

/*************************************************************************
* Set2x20LowerLine - set 2x20 lower display member variable
*
* Parameters:
*  csLowerLine - CString containing lower 1x20 display
*
* Returns: void
*************************************************************************/
void CBaseCurrentDisplay::Set2x20LowerLine(CString csLowerLine)
{
    tbtraceIO(_T("Set2x20LowerLine()"));
    m_cs2x20LowerLine = csLowerLine;
}

/*************************************************************************
* Set2x20UpperLine - set 2x20 upper display member variable
*
* Parameters:
*  csUpperLine - CString containing upper 1x20 display
*
* Returns: void
*************************************************************************/
void CBaseCurrentDisplay::Set2x20UpperLine(CString csUpperLine)
{
    tbtraceIO(_T("Set2x20UpperLine()"));
    m_cs2x20UpperLine = csUpperLine;
}
