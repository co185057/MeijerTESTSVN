// SolutionMHOptions.cpp: implementation of the CSolutionMHOptions class.
//
//////////////////////////////////////////////////////////////////////

// CHANGE HISTORY:
//
// POS110838 Work Request:65347 Name: Robert Susanto Date: August 12, 2014
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SolutionMHOptions.h"
#include "TraceSystem.h"
#include "CustomerCurrentOptions.h"

#define T_ID _T("CSolutionMHOptions")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSolutionMHOptions::CSolutionMHOptions()
{
    
}

CSolutionMHOptions::~CSolutionMHOptions()
{

}

/*************************************************************************
* DetermineTerminalNumber - Read in Terminal number from registry
*
* Parameters: 
*  HKEY hKey - handle to base reg key; ignored for ACS Solution
*
* Returns: int - terminal number
*************************************************************************/
int CSolutionMHOptions::DetermineTerminalNumber(HKEY hKey)
{
	int rc = -1;
    tbtraceIO(_T("DetermineTerminalNumber"));
    LONG     lRC;
    HKEY     hACSKey;
	CCustomerCurrentOptions* pOptions = CCustomerCurrentOptions::instance();

	// If we don't already have the terminal number read in, we need to go read it from registry
	if( (rc = pOptions->GetTerminalNumber()) <= 0)
	{
		if (lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\Advanced Checkout Solution\\CurrentVersion"), 
			0, KEY_QUERY_VALUE, &hACSKey))
		{
			TBASSERT(0);
			return -1; // -1 means error
		}

        // POS110838 - use the 'long' and the BYTE to retrieve the DWORD registry value
		DWORD dwDataSize= sizeof(long);
		TCHAR* TerminalNumber=_T("NodeID");
		long lTerminalID = 0l;		
        
		//lRC = RegQueryValueEx(hACSKey, TerminalNumber, NULL, NULL, 
		//	(LPBYTE)&dTerminalNum, &dwDataSize);
        lRC = RegQueryValueEx(hACSKey, TerminalNumber, NULL, NULL, 
			(BYTE *)&lTerminalID, &dwDataSize);
		
		tbtrace(TM_INFO, _T("Terminal Number Reg Result (%d), Value(%d)"), lRC, lTerminalID);
		if(lRC)
		{
			TBASSERT(0);
			RegCloseKey(hKey);
			return -1; // -1 means error
		}

		TCHAR *ptmp;
        pOptions->SetTerminalNumber((int)lTerminalID); //_tcstol(szTerminalNumber, &ptmp, 10));
        tbtrace(TM_INFO, _T("Terminal Number = %d"), pOptions->GetTerminalNumber());
        // e - POS110838 - use the 'long' and the BYTE to retrieve the DWORD registry value
	}

	// Option should be set by now. Return saved off value
    return pOptions->GetTerminalNumber();
}


/*************************************************************************
* AppIniOptionValue - retrieve value of option passed as parameter from 
*               the APPINI file and the specified section
*
* Parameters: 
*  CString& strSection - section in the ini file where option resides
*  CString& strOptName - option to lookup in the ini file
*
* Returns: CString - value of option
*************************************************************************/
CString CSolutionMHOptions::AppIniOptionValue(const CString & strSection, const CString& strOptName)
{
    TCHAR szRetVal[_MAX_PATH] = { 0 };

    if( strSection.GetLength() && strOptName.GetLength() )
    {
        CString sAppIni = OptionValue(_T("AppIniPath"));
        if( !sAppIni.GetLength() )
        {
            sAppIni = _T("C:\\ACS\\DATA\\APPINI.070");
        }
        GetPrivateProfileString(strSection, strOptName, _T(""), szRetVal, _MAX_PATH, sAppIni);
    }
    
    return szRetVal;
}


/*************************************************************************
* AppIniBoolOptionValue - just calls BoolVal, which will return a bool 
*       value for the APPINI option passed in as the parameter
*
* Parameters: 
*  CString& strSection - section in the ini file where option resides
*  CString& strOptName - option to lookup in the ini file
*
* Returns: bool - value of option
*************************************************************************/
bool CSolutionMHOptions::AppIniBoolOptionValue(const CString & strSection, const CString& strOptName)
{
    return BoolVal(AppIniOptionValue(strSection,strOptName));
}

/*************************************************************************
* AppIniIntOptionValue - just calls IntVal, which will return an int value 
*       for the APPINI option passed in as the parameter
*
* Parameters: 
*  CString& strSection - section in the ini file where option resides
*  CString& strOptName - option to lookup in the ini file
*
* Returns: bool - value of option
*************************************************************************/
int CSolutionMHOptions::AppIniIntOptionValue(const CString & strSection, const CString& strOptName)
{
    return IntVal(AppIniOptionValue(strSection,strOptName));
}


/*************************************************************************
* IsAllowed - The application can use this function to query TB to see 
*       whether or not a certain feature is allowable in the current state.
*
* Parameters: 
*  TBFEATURE nFeature - feature (TBFEATURE enum)
*
* Returns: BOOL - TRUE:permitted; FALSE:not permitted
*************************************************************************/
bool CSolutionMHOptions::IsAllowed(TBFEATURE nFeature)
{
    bool rc = false; // if not known about, don't tell FL we support it
    switch (nFeature)
    {

		case TB_FT_SIGNONRESULTS:
			rc = BoolOptionValue(_T("TB_FT_SIGNONRESULTS"));
			break;
		default:
			rc = CBaseMHOptions::IsAllowed(nFeature);
			tbtrace(TM_WARNING, _T("[%d] Unknown TBFEATURE passed - TB returns not allowed [%d]"), nFeature, rc);
			break;		

	}

	tbtrace(TM_ENTRY, _T("+-IsAllowed(%d) returns %d"), nFeature, rc);
    return rc;
}