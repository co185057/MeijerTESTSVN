// StringManager.cpp: implementation of the StringManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PSStringManager.h"
#include <assert.h>
#include "LibraryHelper.h"

#define COMP_ID ID_PS
#define T_ID _T("StringMgr")
#ifndef _CPPUNIT
#include "MethodTrace.h"
#endif

#define STRINGWRAPPER_DEFAULTFILENAME _T("%APP_DRIVE%\\scot\\bin\\SSCOUIStringsCppWrapper.dll")
/**
 * Function pointers to access exported DLL functions.
 */
typedef LPTSTR (*GETSTRING)(LPCTSTR stringName, const int languageId);
typedef LPTSTR (*GETSTRINGS)(const int languageId, LPCTSTR configPath);
typedef void (*FREESTRING)(LPTSTR str);

// List of exported functions that will be called.
LPCSTR g_szFuncs[] =
{
    "SSCOUIStringsCppWrapper_GetString",
    "SSCOUIStringsCppWrapper_GetStrings",
    "SSCOUIStringsCppWrapper_FreeString",
    NULL                   // Last entry should be NULL to signal end of list.
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPSStringManager::CPSStringManager() : m_hdll(NULL), 
             m_fpMap(NULL), m_pLibraryHelper(new CLibraryHelper())
{
    Initialize();
}

CPSStringManager::CPSStringManager(CLibraryHelper &helper) : 
            m_hdll(NULL), m_fpMap(NULL),  m_pLibraryHelper(&helper)
{
    Initialize();
}

CPSStringManager::~CPSStringManager()
{
    UnInitialize();
    delete m_pLibraryHelper;
    m_pLibraryHelper = NULL;
}

bool CPSStringManager::Initialize(void)
{
    TRACE_METHOD(_T("CPSStringManager::Initialize"));

    // Try to load the DLL.
    trace(L4, _T("Attempting to load UI String DLL"));
    CString csDll = GetUIStringDLLPath();
    m_hdll = m_pLibraryHelper->LoadLibrary(csDll);

    if(! m_hdll)
    {
        trace(L4, _T("** FAILED TO LOAD UI STRING DLL **"));
        return false;
    }
    trace(L4, _T("Load succeeded."));
 
    // Try to init function pointers.
    for(int i=0; g_szFuncs[i]; i++)
    {
        FARPROC fp = m_pLibraryHelper->GetProcAddress(m_hdll, g_szFuncs[i]);
        if(fp == NULL)
        {
            CString csFunc(g_szFuncs[i]);
            trace(L4, _T("ERROR:  Failed to get function pointer for [%s]"),
                  (LPCTSTR)csFunc);
        }
        else
        {
            m_fpMap[g_szFuncs[i]] = fp;
        }
    }
        
    return true;
}

CString CPSStringManager::GetUIStringDLLPath(void)
{
    _TCHAR  szRegValue[_MAX_PATH];
    CString csUIStringDLL;
    HKEY     hKey;
    DWORD dwDataSize=_MAX_PATH;
    LONG  lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP"),
             0, KEY_QUERY_VALUE, &hKey);

    if(lRC == ERROR_SUCCESS)
    {
        lRC = RegQueryValueEx(hKey, _T("UIStringPathName"), NULL, NULL,
                  (LPBYTE)szRegValue, &dwDataSize);
       
        if (lRC == ERROR_SUCCESS && _tcscmp(szRegValue, _T("")) != 0)
        {
            trace(L6,_T("Loading DLL from registry [%s]"), szRegValue);
        }
        else
        {
            _tcscpy(szRegValue, STRINGWRAPPER_DEFAULTFILENAME);
            trace(L6,_T("Loading default DLL [%s]"), szRegValue);
        }

        ::RegCloseKey(hKey);
    }

    GET_PARTITION_PATH(szRegValue, csUIStringDLL.GetBuffer(_MAX_PATH));
    csUIStringDLL.ReleaseBuffer();

    return csUIStringDLL;
}

void CPSStringManager::UnInitialize(void)
{
    m_pLibraryHelper->FreeLibrary(m_hdll);
    m_hdll = NULL;

    m_fpMap.clear();
    m_StringList.clear();

}


LPTSTR CPSStringManager::GetString(LPCTSTR stringName, const int languageId)
{
    TRACE_METHOD(_T("CPSStringManager::GetString")); 

    if(!m_hdll)
    {
        trace(L7, _T("-CPSStringManager::GetString -> SSCOUIStringsCppWrapper DLL NOT LOADED"));
        return NULL;
    }

    GETSTRING fp = (GETSTRING)m_fpMap["SSCOUIStringsCppWrapper_GetString"];
    if (fp == NULL)
    {
        return NULL;
    }

    LPTSTR str = (fp)(stringName, languageId);      // Call method.

    return str;
}

LPTSTR CPSStringManager::GetStrings(const int languageId)
{
    TRACE_METHOD(_T("CPSStringManager::GetStrings")); 

    if(!m_hdll)
    {
        trace(L7, _T("-CPSStringManager::GetStrings -> SSCOUIStringsCppWrapper DLL NOT LOADED"));
        return NULL;
    }

    GETSTRINGS fp = (GETSTRINGS)m_fpMap["SSCOUIStringsCppWrapper_GetStrings"];
    if (fp == NULL)
    {
        return NULL;
    }

    LPTSTR str = (fp)(languageId, co.csConfigFilePath);      // Call method.

    return str;
}

void CPSStringManager::FreeString(LPTSTR str)
{
    TRACE_METHOD(_T("CPSStringManager::FreeString")); 

    if(!m_hdll)
    {
        trace(L7, _T("-CPSStringManager::FreeString -> SSCOUIStringsCppWrapper DLL NOT LOADED"));
        return;
    }

    FREESTRING fp = (FREESTRING)m_fpMap["SSCOUIStringsCppWrapper_FreeString"];
    if(fp == NULL)
    {
        return;
    }

    (fp)(str);      // Call method.
}

void CPSStringManager::LoadStringToMap(const int nLangCode)
{
    LPTSTR strLang = GetStrings(nLangCode);
    CString csStringList(strLang);
    
    if(!csStringList.IsEmpty())
    {
        int nPos;
        int nIndex = csStringList.Find('~');
        CString csLine, csKey, csValue;

        map<CString, CString> mStrings;
        while(nIndex != -1)
        {
            csLine = csStringList.Left(nIndex);
            csStringList = csStringList.Mid(nIndex + 1);
    
            nPos = csLine.Find('='); 
            if(nPos != -1) 
            {
                csKey = csLine.Left(nPos);
                csValue = csLine.Mid(nPos + 1);
                mStrings[csKey] = csValue;
            }
            nIndex = csStringList.Find('~');
        }
        m_StringList.push_back(mStrings);   
        trace(L7,_T("LoadStringToMap:: lang[%d] Loaded [%d] strings"), nLangCode, mStrings.size());
    }
    else
    {
        trace(L4, _T("LoadStringToMap - List is EMPTY for language [%d]"), nLangCode);
    }
    FreeString(strLang);
}

CString CPSStringManager::GetStringFromMap(const CString csStrToSearch, const int nLangID)
{
    CString csUIString;
    map<CString, CString> *pMap = &m_StringList[nLangID];
	if(pMap != NULL)
	{
		map<CString,CString>::const_iterator it = pMap->find(csStrToSearch);
		if(it != pMap->end())
		{
			csUIString = it->second;
            trace(L7,_T("GetStringFromMap -- String[%s] found!"), csStrToSearch);
		}
		else
		{
			trace(L6,_T("GetStringFromMap -- String[%s] not found"), csStrToSearch);
		}
	}
	else
	{
		trace(L7,_T("GetStringFromMap -- Language strings NOT FOUND in map."));
	}
    
    return csUIString;
}

CString CPSStringManager::GetUIString(const CString csStringID, const int nLangID)
{
    CString csUIString;
    if(m_StringList.empty())
    {
        trace(L4,_T("GetUIString[%s] - String list is EMPTY!"));
    }
    else if(nLangID >= SCOT_LANGUAGE_PRIMARY && nLangID < co.GetNumberOfLanguagesSupported())
    {
        csUIString = GetStringFromMap(csStringID,nLangID);
    }
    else
    {
        trace(L6,_T("GetUIString[%s] - Language not supported!"), csStringID);
    }
    return csUIString;
}

void CPSStringManager::LoadAndSaveLanguageStringToMap(const int nLangID, const int nLangCode)
{
    if(SCOT_LANGUAGE_PRIMARY == nLangID)
    {
        m_StringList.clear();
    }
    m_Languages[nLangID] = nLangCode;
    LoadStringToMap(nLangCode);
}

