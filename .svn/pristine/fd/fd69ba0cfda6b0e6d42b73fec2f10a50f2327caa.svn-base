//////////////////////////////////////////////////////////////////////
//
// BaseMHDisplay.cpp: implementation of the CBaseMHDisplay class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseMHDisplay.h"
#include "TraceSystem.h"
#include "CustomerMsgMap.h"
#include "CustomerCurrentDisplay.h"

#define T_ID    _T("BASEMHDISPLAY")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBaseMHDisplay::CBaseMHDisplay()
{
    SetMonitorAll(FALSE);
}

CBaseMHDisplay::~CBaseMHDisplay()
{
}

/*************************************************************************
* ProcessMessage - The base ProcessMessage for display messages passes the 
*                  parameter to the Msg Map Handler class
*
* Parameters:
*  psMessage - PSMessageElement
*
* Returns: TBSTATE
*************************************************************************/
TBSTATE CBaseMHDisplay::ProcessMessage(CTBMsgObj *pMsg)
{
   return CCustomerMsgMap::instance()->ProcessMessage();
}


/*************************************************************************
* Get2x20 - Fastlane calls this for display in assist mode. Core limitations
*           restrict this to a true 2x20. Base TB class also restricts this.
*
* Parameters:
*  szDisplay - LPCSTR to be populated with 40 CHAR array
*
* Returns: TBSTATE
*************************************************************************/
TBRC CBaseMHDisplay::Get2x20(LPCTSTR szDisplay)
{
    tbtraceIO(_T("Get2x20()"));
    TBRC rc = TB_SUCCESS;

    CBaseCurrentDisplay *pCurrDisplay = CCustomerCurrentDisplay::instance();
    ASSERT(pCurrDisplay != NULL);  // ensure this is not NULL
    if (pCurrDisplay != NULL)
    {
        CString LineOfSpaces = _T("                    ");
        CString cs2x20;
        CString cs2x20UpperLine = pCurrDisplay->Get2x20UpperLine();
        CString cs2x20LowerLine = pCurrDisplay->Get2x20LowerLine();
        
        //ensure each line has at least 20 characters
        if (cs2x20UpperLine.GetLength() < 20)
            cs2x20UpperLine += LineOfSpaces;  // append space to ensure length of 20
        
        if (cs2x20LowerLine.GetLength() < 20)
            cs2x20LowerLine += LineOfSpaces;  // append space to ensure length of 20
        
        // Now, only get the leftmost 20 character of upper and lower
        cs2x20 = cs2x20UpperLine.Left(20) + cs2x20LowerLine.Left(20);
        
        _tcscpy((TCHAR *)szDisplay, cs2x20);
    }
    else
        rc = TB_E_UNEXPECTED;
    
    return rc;
}

/*************************************************************************
* SetMonitorAll - accessor for member variable, called when Fastlane 
*                 enters and exits assist mode
*
* Parameters: 
*  bVal - BOOL value, TRUE = entering assist mode, FALSE = exiting
*
* Returns: void
*************************************************************************/
void CBaseMHDisplay::SetMonitorAll(BOOL bVal)
{
    m_bMonitorAll = bVal;
}

/*************************************************************************
* GetMonitorAll - accessor for member variable
*
* Parameters: none
*
* Returns: member variable m_bMonitorAll
*************************************************************************/
BOOL CBaseMHDisplay::GetMonitorAll(void)
{
    return m_bMonitorAll;
}
