// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/Registry.cpp 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)Registry.cpp    $Revision: 1 $ $Date: 6/20/08 1:13p $
 *
 * Copyright 2004, NCR Corporation.
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/Registry.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     3/24/05 7:31p Dm185016
 * UNICODE
 * 
 * 2     2/28/05 3:33p Dm185016
 * lint
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
// 
// 11    1/18/05 4:02p Dm185016
// UNICODE support
// 
// 10    12/21/04 10:02a Dm185016
// Fixed buffer overrun problem.
// 
// 9     10/04/04 6:57p Dm185016
// Added iterator for registry keys and values.
// 
// 7     5/24/04 6:35p Dm185016
// lint + fix for Std Dev A value read from registry wrong - low order
// byte was zeroed out
// 
// 6     5/05/04 1:09p Dm185016
// 
// 5     5/04/04 4:27p Dm185016
// Fixed memory leak.  Added UNICODE support.
// 
// 4     4/16/04 4:19p Dm185016
// Added read using specified key
// 
// 3     4/12/04 3:32p Dm185016
// Added ContainsSubKey
// 
// 2     3/25/04 3:54p Tk124825
// 
// 1     3/19/04 3:32p Dm185016
// New classes for accessing the Registry
*/
#include "StdAfx.h"
#include "Registry.h"
#include "xerror.h"


CRegistry::CRegistry(HKEY hKey) 
    : m_puRegistryData((_UREGISTRY_DATA *)new char[sizeof(_UREGISTRY_DATA)])
    , m_ulBufferCnt(0)
    , m_dwSize(0)
	, m_dwType(0)
    , m_lDWORD(0)
    , m_hKey(KEY_PARM)
    , m_piterBegin(NULL)
    , m_piterEnd(NULL)
{
}


CRegistry::CRegistry(const _TCHAR *pKeyName, HKEY hKey)
    : m_puRegistryData((_UREGISTRY_DATA *)new char[sizeof(_UREGISTRY_DATA)])
    , m_ulBufferCnt(0)
    , m_dwSize(0)
	, m_dwType(0)
    , m_lDWORD(0)
    , m_piterBegin(NULL)
    , m_piterEnd(NULL)
{
    //lint --e{774}
    HRESULT hr = ::RegOpenKeyEx( hKey, 
                            pKeyName,
							0, 
                            KEY_READ, 
                            &m_hKey	);
    _ASSERTE(hr==ERROR_SUCCESS);

    if (hr!=ERROR_SUCCESS)
    {
        throw IException(_T("RegOpenKeyEx failed to open"), hr);
    }
}


CRegistry::~CRegistry() 
{
    delete [] m_puRegistryData;

	m_puRegistryData = NULL;

    if (m_hKey == HKEY_LOCAL_MACHINE)
    {}
    else
    {
        ::RegCloseKey(m_hKey);
    }

    delete m_piterBegin;
    delete m_piterEnd;
}


CRegistry::iterator &CRegistry::keys_begin() 
{ 
    if (m_piterBegin == NULL)
        m_piterBegin = new iterator(m_hKey);

    return *m_piterBegin; 
}


CRegistry::iterator &CRegistry::keys_end() 
{ 
    if (m_piterEnd == NULL)
    {
        m_piterEnd = new iterator(m_hKey);
        m_piterEnd->SeekEnd();
    }

    return *m_piterEnd; 
}


long CRegistry::EnsureBuffer(HKEY hKey, const _TCHAR *pValueName)
{
    long lRC;

    lRC = ::RegQueryValueEx( hKey
                        , pValueName
                        , NULL
                        , &m_dwType
                        , NULL
                        , &m_dwSize	);
    switch (lRC)
    {
        case ERROR_MORE_DATA :      // lint -fallthrough
            lRC = ERROR_SUCCESS;
        //lint -fallthrough
        case ERROR_SUCCESS :
            EnsureBuffer(m_dwSize);
            break;
        default :
            EnsureBuffer();
            break;
    }

    return lRC;
}


void CRegistry::EnsureBuffer(unsigned long lLength)
{
    if ((m_puRegistryData == NULL) || (m_ulBufferCnt <= lLength))
    {
        delete [] m_puRegistryData;
        m_ulBufferCnt = max(lLength + 1, sizeof(_UREGISTRY_DATA));

        m_puRegistryData = (_UREGISTRY_DATA *)new _TCHAR[m_ulBufferCnt];

        m_puRegistryData->szData[0] = 0;           // lint -e613
    }

    _ASSERTE(m_puRegistryData!=NULL);
}


long CRegistry::ReadValue(HKEY hKey, const _TCHAR *pValueName)
{
    long lRC = EnsureBuffer(hKey, pValueName);

	if (lRC==ERROR_SUCCESS)
    {
        lRC = ::RegQueryValueEx(hKey
                            , pValueName
                            , NULL
                            , &m_dwType
                            , (LPBYTE)m_puRegistryData->szData
                            , &m_dwSize	);
    }

	if (lRC!=ERROR_SUCCESS)
    {
        EnsureBuffer();
    }

    return lRC;
}


long CRegistry::ReadValue(const _TCHAR *pKeyName, const _TCHAR *pValueName)
{
    long lRC;
	HKEY hKey;

    // open the registry entry
	lRC = ::RegOpenKeyEx(	m_hKey, 
                            pKeyName,
							0, 
                            KEY_READ, 
                            &hKey	);
	if (lRC==ERROR_SUCCESS)
    {
        lRC = EnsureBuffer(hKey, pValueName);

	    if (lRC==ERROR_SUCCESS)
        {
            lRC = ::RegQueryValueEx(	hKey
                                , pValueName
                                , NULL
                                , &m_dwType
                                , (LPBYTE)m_puRegistryData->szData
                                , &m_dwSize	);
        }
    }

	if (lRC!=ERROR_SUCCESS)
    {
        EnsureBuffer();
    }

    ::RegCloseKey( hKey );

    return lRC;
}


bool CRegistry::ContainsSubKey( const _TCHAR *pKeyName, const _TCHAR *pSubKeyName ) const
{
    long lRC;
	HKEY hKey;
	HKEY hSubKey;

    // open the registry entry
	lRC = ::RegOpenKeyEx(	HKEY_LOCAL_MACHINE, 
                            pKeyName,
							0, 
                            KEY_READ, 
                            &hKey	);
	if (lRC==ERROR_SUCCESS)
    {
	    lRC = ::RegOpenKeyEx(	hKey, 
                                pSubKeyName,
							    0, 
                                KEY_READ, 
                                &hSubKey	);
        ::RegCloseKey( hSubKey );
        ::RegCloseKey( hKey );
    }

    return lRC==ERROR_SUCCESS;
}
