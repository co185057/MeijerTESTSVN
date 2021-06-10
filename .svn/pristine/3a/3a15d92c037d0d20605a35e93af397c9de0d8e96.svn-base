#include "stdafx.h"
#include "stdlib.h"
#include "BaseMHOptions.h"
#include "TraceSystem.h"
#include "BaseCurrentoptions.h"
#include "TBUtilities.h"
#include "BaseObjectManager.h"


#define T_ID _T("BaseMHOptions")


CBaseMHOptions::CBaseMHOptions()
{
    InitializeCriticalSection(&m_CritSectOptions);
}

CBaseMHOptions::~CBaseMHOptions()
{
    DeleteCriticalSection(&m_CritSectOptions);
}

/*************************************************************************
* Initialize - reads in the base and customer level option files, sets default
*       values for options that need values.
*
* Parameters: none
*
* Returns: TBRC
*************************************************************************/
TBRC CBaseMHOptions::Initialize()
{
    TBRC rc = TB_SUCCESS;
    
    LoadStandardOptions();
    LoadCustomerOptions();
    
    EnterCriticalSection(&m_CritSectOptions);
    // TraceWriter default settings
    m_cmDefaults.SetAt(_T("TBTraceSize"), CString(_T("500000")));
    m_cmDefaults.SetAt(_T("TBTraceOn"), CString(_T("Y")));
    m_cmDefaults.SetAt(_T("TBTraceFileName"), CString(_T("c:\\scot\\bin\\TBTrace.log")));
    m_cmDefaults.SetAt(_T("TBTraceMask"), CString(_T("7")));
    //
    m_cmDefaults.SetAt(_T("CashDrawerAtRAP"), CString(_T("Y")));
    
    m_cmDefaults.SetAt(_T("CurrentLevel"), CString(_T("3")));
    m_cmDefaults.SetAt(_T("FocusLevel"), CString(_T("0")));
    m_cmDefaults.SetAt(_T("Cashback1"), CString(_T("Y")));
    
    m_cmDefaults.SetAt(_T("TBUsesPipeServer"), CString(_T("Y")));
    m_cmDefaults.SetAt(_T("PreScotStartDependsOnPostScotStop"), CString(_T("Y")));
    m_cmDefaults.SetAt(_T("ISO639PrimaryLanguageCode"), CString(_T("EN")));
    
    m_cmDefaults.SetAt(_T("TB_FT_TENDER"), CString(_T("Y")));
    m_cmDefaults.SetAt(_T("TB_FT_ITEMSALE"), CString(_T("Y")));
    m_cmDefaults.SetAt(_T("TB_FT_SUSPEND"), CString(_T("Y")));
    m_cmDefaults.SetAt(_T("TB_FT_DEBITACCOUNTTYPE"), CString(_T("Y")));
    m_cmDefaults.SetAt(_T("TB_FT_EXTENDEDDETAILS"), CString(_T("Y")));
    m_cmDefaults.SetAt(_T("TB_FT_TRAININGMODE"), CString(_T("Y")));
    m_cmDefaults.SetAt(_T("TB_FT_QUICKITEMENTRY"), CString(_T("N")));
    m_cmDefaults.SetAt(_T("TB_FT_ENDORSEMENT"), CString(_T("Y")));
    m_cmDefaults.SetAt(_T("TB_FT_SECURITYLEVEL"), CString(_T("Y")));
    m_cmDefaults.SetAt(_T("TB_FT_MICR"), CString(_T("Y")));
    m_cmDefaults.SetAt(_T("TB_FT_CASHDRAWER"), CString(_T("Y")));
    m_cmDefaults.SetAt(_T("TB_FT_AMSTATUS"), CString(_T("Y")));
    m_cmDefaults.SetAt(_T("TB_FT_SUPPORT_PIM"), CString(_T("Y")));
    
    LeaveCriticalSection(&m_CritSectOptions);
    
    return rc;
}

/*************************************************************************
* GetTerminalNumber - Return POS client terminal number. FastLane assumes 
*                       positive terminal numbers only. Permits negative
*                       numbers to be used to return error status.
*
* Parameters: none
*
* Returns: long - terminal number
*************************************************************************/
long CBaseMHOptions::GetTerminalNumber()
{
    CBaseCurrentOptions* pOptions = CCustomerCurrentOptions::instance();
    return pOptions->GetTerminalNumber();
}

/*************************************************************************
* GetVersion - Return the version number and information of the TB.
*                Version string conforms to the below format:
*                  "FastLane TB Ver xx.xx.xx for Solution Name"
*                This should be set using the CBaseCurrentOptions::SetVersionString
*                function. Base TB does not currently set this.
*
* Parameters: 
*  LPTSTR szVersionString - Buffer to contain the version string
*  long nBufferLength - Buffer length
*
* Returns: long - Length of the actual version string, including the NULL 
*                 character. If this is bigger than nBufferLength then the 
*                 buffer passed is not long enough to copy the entire version 
*                 string, but nBufferLength characters are still copied into 
*                 the buffer.
*************************************************************************/
long CBaseMHOptions::GetVersion(LPTSTR szVersionString, long nBufferLength)
{
    CBaseCurrentOptions* pOptions = CCustomerCurrentOptions::instance();
    CopyString(szVersionString, pOptions->GetVersionString());
    nBufferLength = pOptions->GetVersionStringBufferLength();
    return pOptions->GetVersionNumber();
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
bool CBaseMHOptions::IsAllowed(TBFEATURE nFeature)
{
    bool rc = false; // if not known about, don't tell FL we support it
    switch (nFeature)
    {
    case TB_FT_TENDER:
        rc = BoolOptionValue(_T("TB_FT_TENDER"));
        break;
    case TB_FT_ITEMSALE:
        rc = BoolOptionValue(_T("TB_FT_ITEMSALE"));
        break;
    case TB_FT_SUSPEND:
        rc = BoolOptionValue(_T("TB_FT_SUSPEND"));
        break;
    case TB_FT_DEBITACCOUNTTYPE:
        rc = BoolOptionValue(_T("TB_FT_DEBITACCOUNTTYPE"));
        break;
    case TB_FT_EXTENDEDDETAILS:
        rc = BoolOptionValue(_T("TB_FT_EXTENDEDDETAILS"));
        break;
    case TB_FT_TRAININGMODE:
        rc = BoolOptionValue(_T("TB_FT_TRAININGMODE"));
        break;
    case TB_FT_QUICKITEMENTRY:
        rc = BoolOptionValue(_T("TB_FT_QUICKITEMENTRY"));
        break;
    case TB_FT_ENDORSEMENT:
        rc = BoolOptionValue(_T("TB_FT_ENDORSEMENT"));
        break;
    case TB_FT_SECURITYLEVEL:
        rc = BoolOptionValue(_T("TB_FT_SECURITYLEVEL"));
        break;
    case TB_FT_MICR:
        rc = BoolOptionValue(_T("TB_FT_MICR"));
        break;
    case TB_FT_CASHDRAWER:
        rc = BoolOptionValue(_T("TB_FT_CASHDRAWER"));
        break;
    case TB_FT_AMSTATUS:
        rc = BoolOptionValue(_T("TB_FT_AMSTATUS"));
        break;
    case TB_FT_SUPPORT_PIM:
        rc = BoolOptionValue(_T("TB_FT_SUPPORT_PIM"));
        break;
    default:
        tbtrace(TM_WARNING, _T("Unknown TBFEATURE passed - TB returns not allowed"));
        break;
    }
    
    tbtrace(TM_ENTRY, _T("+-IsAllowed(%d) returns %d"), nFeature, rc);
    return rc;
}

/*************************************************************************
* LoadStandardOptions - loads base options
*
* Parameters: none
*
* Returns: void
*************************************************************************/
void CBaseMHOptions::LoadStandardOptions()
{
    tbtraceIO(_T("LoadStandardOptions"));
    CreateOptionMap(GetOptionFileName(false));
}

/*************************************************************************
* LoadCustomerOptions - loads customer options
*
* Parameters: none
*
* Returns: void
*************************************************************************/
void CBaseMHOptions::LoadCustomerOptions()
{
    tbtraceIO(_T("LoadCustomerOptions"));
    CreateOptionMap(GetOptionFileName(true));
}

/*************************************************************************
* GetOptionFileName - reads in the base or customer (depending on parameter)
*       option file name from the registry
*
* Parameters: 
*  bool bCustFile - return the customer file? False = base file.
*
* Returns: CString - full path info of option file name
*************************************************************************/
CString CBaseMHOptions::GetOptionFileName(bool bCustFile)
{
    tbtraceIO(_T("GetOptionFileName"));
    // Read terminal number from registry
    LONG     lRC;
    HKEY     hKey;
    int nTerminalNumber;
    //LPCSTR   szFilePath;
    DWORD dwDataSize;
    TCHAR custfilename[20]; // allow longer filename
    TCHAR* CustOptionFile=_T("CustomerOptionFile");
    TCHAR  basefilename[20]; 
    TCHAR* BaseOptionFile=_T("BaseOptionFile");
    CString csOptionFileName;

	// Get Handle to Base TB Registry Key - Base Options stored here
    if (lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTTB"), 
        0, KEY_QUERY_VALUE, &hKey))
	{
        TBASSERT(0);
        return _T("");
	}
    
    nTerminalNumber = DetermineTerminalNumber(hKey);
    if(nTerminalNumber < 0)
    {
        return _T("");
    }
    
    if(bCustFile)
    {
        dwDataSize= sizeof(custfilename);
        lRC = RegQueryValueEx(hKey, CustOptionFile, NULL, NULL, 
            (LPBYTE)custfilename, &dwDataSize);
        if(lRC)
        {
            TBASSERT(0);
            RegCloseKey(hKey);
            return _T("");
        }
        csOptionFileName.Format(_T("%s\\%s.%03d"),_T("C:\\SCOT\\CONFIG"),
            custfilename, nTerminalNumber);   
    }
    else
    {
        dwDataSize= sizeof(basefilename);
        lRC = RegQueryValueEx(hKey, BaseOptionFile, NULL, NULL, 
            (LPBYTE)basefilename, &dwDataSize);
        if(lRC)
        {
            TBASSERT(0);
            RegCloseKey(hKey);
            return _T("");
        }
        csOptionFileName.Format(_T("%s\\%s.%03d"),_T("C:\\SCOT\\CONFIG"),
            basefilename, nTerminalNumber);
    }
    
    ::RegCloseKey(hKey);
    
    return csOptionFileName;
}  

/*************************************************************************
* SetOptionValue - can be used to set the value of an option that was read
*       in from the option file. When you override it through this function,
*       the value in the option file will also be changed.
*
* Parameters: 
*  CString& strSectionName - name of section in option file
*  CString& strOptName - option name
*  CString & strOptValue - option value
*
* Returns: bool - true=success; false=failure
*************************************************************************/
bool CBaseMHOptions::SetOptionValue(const CString& strSectionName, const CString& strOptName, const CString & strOptValue)
{
    tbtrace(TM_ENTRY, _T("+SetOptionValue(%s, %s, %s)"), strSectionName, strOptName, strOptValue);
    bool rc = false;

    ASSERT(strSectionName.GetLength() && strOptName.GetLength());
    
    CString strCustFile = GetOptionFileName(true);
    
    // update the new value in the file
    if( WritePrivateProfileString(strSectionName, strOptName, strOptValue, strCustFile) )
    {
        // set the new value in the cached map
        EnterCriticalSection(&m_CritSectOptions);
        m_cmOptionMap.SetAt(strOptName, strOptValue);
        LeaveCriticalSection(&m_CritSectOptions);
        rc = true;
    }
    
    tbtrace(TM_ENTRY, _T("-SetOptionValue() returning %d"), rc);
    return rc;
}

/*************************************************************************
* CreateOptionMap - reads in the .dat, .000, and .xxx option files to create
*       the option map.
*
* Parameters: 
*  CString& szFileName = the filename with the .xxx (terminal num) extension
*
* Returns: void
*************************************************************************/
void CBaseMHOptions::CreateOptionMap(const CString& szFileName)
{
    tbtraceIO(_T("CreateOptionMap"));
    TCHAR szSKeys[4048];
    
    for(int i = 0; i < 3; i++)
    {
        CString tmpFile;
        switch(i)
        {
        case 0:  tmpFile = szFileName.Left(szFileName.GetLength()-4) + _T(".dat"); break;
        case 1:   tmpFile = szFileName.Left(szFileName.GetLength()-4) + _T(".000"); break;
        case 2:   tmpFile = szFileName;
        }
        int lRC = GetPrivateProfileSectionNames(szSKeys, sizeof(szSKeys)/sizeof(TCHAR), tmpFile);
        if(lRC <= 0)
        {
            continue;
        }
        
        TCHAR *pSKey = szSKeys;
        CString szSWorkingKey;
        
        while(*pSKey)
        {   
            szSWorkingKey = pSKey;
            pSKey = pSKey + szSWorkingKey.GetLength() + 1;
            
            TCHAR szKeys[4048];
            GetPrivateProfileString(szSWorkingKey, NULL, NULL, szKeys, 
                sizeof(szKeys)/sizeof(TCHAR), tmpFile);
            
            TCHAR *pKey = szKeys;    
            CString szWorkingKey;
            
            while(*pKey)
            {
                szWorkingKey = pKey;
                pKey = pKey + szWorkingKey.GetLength() + 1;
                
                if(szWorkingKey == _T("SMMWtMaxTol"))
                    int i = 1;
                
                CString szOptVal;
                TCHAR tmp[4096];
                GetPrivateProfileString(szSWorkingKey, szWorkingKey, _T("NotFound"), 
                    tmp, sizeof(tmp)/sizeof(TCHAR), tmpFile);
                
                szOptVal = tmp;
                if(szOptVal == _T("NotFound"))
                {
                    pSKey = NULL;
                }
                EnterCriticalSection(&m_CritSectOptions);
                m_cmOptionMap.SetAt(szWorkingKey, szOptVal); // CMapStringtoString add key and value to the Map
                LeaveCriticalSection(&m_CritSectOptions);
            }
        } 
    }
}

/*************************************************************************
* DetermineTerminalNumber - Read in Terminal number from registry
*
* Parameters: 
*  HKEY hKey - handle to reg key that TB reads from
*
* Returns: int - terminal number
*************************************************************************/
int CBaseMHOptions::DetermineTerminalNumber(HKEY hKey)
{
    tbtraceIO(_T("DetermineTerminalNumber"));
    LONG     lRC;
    CBaseCurrentOptions* pOptions = CCustomerCurrentOptions::instance();
    
    
    DWORD dwDataSize;
    TCHAR* TerminalNumber=_T("TerminalNumber");
    TCHAR szTerminalNumber[4]=_T("001");    // 3-digit number as file suffix
    
    
    dwDataSize= sizeof(szTerminalNumber);
    lRC = RegQueryValueEx(hKey, TerminalNumber, NULL, NULL, 
        (LPBYTE)szTerminalNumber, &dwDataSize);
    
    if(lRC)
    {
        TBASSERT(0);
        RegCloseKey(hKey);
        return -1;
    }
    
    TCHAR *ptmp;
    pOptions->SetTerminalNumber(_tcstol(szTerminalNumber, &ptmp, 10));
    return pOptions->GetTerminalNumber();
}

/*************************************************************************
* OptionValue - retrieve value of option passed as parameter
*
* Parameters: 
*  CString& strOptName - option to lookup in option map
*
* Returns: CString - value of option
*************************************************************************/
CString CBaseMHOptions::OptionValue(const CString& strOptName)
{
    CString csRetVal(_T(""));
    EnterCriticalSection(&m_CritSectOptions);
    if (!m_cmOptionMap.Lookup(strOptName, csRetVal))
        m_cmDefaults.Lookup(strOptName, csRetVal);
    LeaveCriticalSection(&m_CritSectOptions);
    return csRetVal;
}

/*************************************************************************
* BoolOptionValue - just calls BoolVal, which will return a bool value 
*       for the option passed in as the parameter
*
* Parameters: 
*  CString& strOptName - option to lookup in option map
*
* Returns: bool - value of option
*************************************************************************/
bool CBaseMHOptions::BoolOptionValue(const CString& strOptName)
{
    return BoolVal(OptionValue(strOptName));
}

/*************************************************************************
* BoolOptionValue - just calls BoolVal, which will return a bool value 
*       for the option passed in as the parameter
*
* Parameters: 
*  CString& strOptName - option to lookup in option map
*
* Returns: bool - value of option
*************************************************************************/
int CBaseMHOptions::IntOptionValue(const CString& strOptName)
{
    return IntVal(OptionValue(strOptName));
}


/*************************************************************************
* BoolVal - returns a bool value for the option passed in as the parameter
*
* Parameters: 
*  CString strVal - option to lookup in option map
*
* Returns: bool - value of option
*************************************************************************/
bool CBaseMHOptions::BoolVal(CString strVal)
{
    strVal.MakeUpper();
    return ((strVal == _T("Y")) || (strVal == _T("T")) || (strVal == _T("1")));
}

/*************************************************************************
* IntVal - returns an int value for the option passed in as the parameter
*
* Parameters: 
*  CString strVal - option to lookup in option map
*
* Returns: int - value of option
*************************************************************************/
int CBaseMHOptions::IntVal(CString strVal)
{
    TCHAR *ptmp;
    return _tcstol((LPCTSTR)strVal, &ptmp, 10);
}


