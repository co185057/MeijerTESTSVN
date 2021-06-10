/*
 * @(#)LibraryImpl.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
 *
 * Copyright 2004 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/LibraryImpl.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 4     3/24/05 7:27p Dm185016
 * UNICODE
 * 
 * 3     3/22/05 5:02p Dm185016
 * UNICODE
 * 
 * 2     2/10/05 5:50p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 1     1/26/05 2:29p Dm185016
 * Moved to new Repository.
// 
// 5     1/18/05 3:58p Dm185016
// UNICODE support
// 
// 3     4/20/04 4:44p Dm185016
// No longer requires the 'D' in the XML file.  Debug builds will
// automatically insert the D into the name just prior to the extension.
// 
// 2     3/19/04 3:28p Dm185016
// Memory leak fix
// 
// 1     1/27/04 2:06p Dm185016
// New support for DLL libraries of actions
*/
#include "StdAfx.h"
#include "LibraryImpl.h"
#include "LibraryHandler.h"
#include "xerror.h"

using namespace STATE_MACHINE_NAMESPACE;


CLibraryImpl::CLibraryImpl(int nId)
    : nId(nId)
    , m_hModule(0)
    , m_pCreateFxn(NULL)
{}


CLibraryImpl::~CLibraryImpl(void)
{
    if (m_hModule)
    {
        if (!::FreeLibrary(m_hModule))
        {
    	    ISystemErrorInfo error(_T("FreeLibrary"));
            ITRACE_ERROR((const _TCHAR *)error);
        }
    }
}


//DEFINE_STRING_PROPERTY(CLibraryImpl, Name);
HRESULT CLibraryImpl::SaveName(const StringType &szValue)
{
    Name.Convert(szValue);

    HRESULT hr = S_OK;

    StringType sName = Name.Value();
    StringType sSuffix;
#ifdef _UNICODE
    sSuffix += _T("U");
#endif
#ifdef _DEBUG
    sSuffix += _T("D");
#endif
    if (!sSuffix.empty())
    {
        size_t index = sName.find_last_of('.');
        if (index != StringType::npos)
            sName = sName.insert(index, sSuffix);
    }
    ITRACE_DEVELOP(_T("Loading library:  ") + IString(sName.c_str()));
    m_hModule = ::LoadLibrary(sName.c_str());
    if (!m_hModule)
    {
    	ISystemErrorInfo error(_T("LoadLibrary"));
        ITRACE_ERROR((const _TCHAR *)error);

        hr = error.errorId();
    }

    return hr;
}

DEFINE_STRING_PROPERTY(CLibraryImpl, Comment);

int CLibraryImpl::GetId() const { return nId; }


const _TCHAR *CLibraryImpl::GetComment() { return Comment.Value().c_str(); }


const _TCHAR *CLibraryImpl::GetName() { return Name.Value().c_str(); }


FARPROC CLibraryImpl::FindFunction(LPCTSTR pcsFxnName)
{

    FARPROC pFxn = NULL;

    CT2A aFxnName(pcsFxnName);

    pFxn = ::GetProcAddress(m_hModule, (LPCSTR)aFxnName);
    if (!pFxn)
    {
    	ISystemErrorInfo error(_T("GetProcAddress"));
        ITRACE_ERROR((const _TCHAR *)error);
    }

    return pFxn;
}


FARPROC CLibraryImpl::GetCreateFunction(LPCTSTR pcsFxnName)
{
    if (m_pCreateFxn == NULL)
        m_pCreateFxn = FindFunction(pcsFxnName);

    return m_pCreateFxn;
}


FARPROC CLibraryImpl::GetReleaseFunction(LPCTSTR pcsFxnName)
{
    if (m_pReleaseFxn == NULL)
        m_pReleaseFxn = FindFunction(pcsFxnName);

    return m_pReleaseFxn;
}


StringType CLibraryImpl::GetXMLOutput(CXMLBuilder *pXMLBuilder)
{
    if (pXMLBuilder == NULL)
        ((CXMLBuilder *)CLibraryHandler::instance())->GetXMLOutput(this);
    else
        pXMLBuilder->GetXMLOutput(this);

    return (pXMLBuilder == NULL ? CLibraryHandler::instance() : pXMLBuilder)->GetXMLOutput();
}
