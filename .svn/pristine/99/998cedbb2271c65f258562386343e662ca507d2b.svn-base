// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/Configuration.cpp 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)Configuration.cpp    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/Configuration.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     3/22/05 4:06p Dm185016
 * UNICODE
 * 
 * 2     2/28/05 3:29p Dm185016
 * lint
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
// 
// 14    1/18/05 4:00p Dm185016
// UNICODE support
// 
// 13    10/04/04 7:00p Dm185016
// ConfigManager object is no longer shared.
// 
// 11    5/18/04 3:42p Dm185016
// lint
// 
// 10    5/12/04 5:07p Tk124825
// TAR 267879
// 
// 9     5/04/04 4:27p Dm185016
// Added code to allow merge of XML documents
// 
// 8     4/28/04 12:49p Tk124825
// Added SetValue method to ConfigurationManager class
// 
// 7     4/20/04 4:45p Dm185016
// Clear configuration value map before reloading
// 
// 6     4/12/04 3:32p Dm185016
// Split SetURL into SetURL and ReloadOptions
// 
// 5     3/25/04 3:23p Dm185016
// Fixed debug
// 
// 4     3/19/04 3:30p Dm185016
// Memory leak fix
// 
// 3     3/01/04 10:20a Dm185016
// Added IsValid() method for error reporting
// 
// 2     2/18/04 9:15a Dm185016
// Added template accessor functions
// 
// 1     2/10/04 11:51a Dm185016
// New object to support XML configuration files.
*/

#include "StdAfx.h"
#include "Configuration.h"
#include "XMLParser.h"


CConfigManager::CConfigManager(HTRACE hTrace)
    : m_bValid(false)
    , m_eLoadOptions(eInvalid)
{
    CBaseObject::SetTraceObject(hTrace);
}


CConfigManager::~CConfigManager()
{
}


HRESULT CConfigManager::SetURL(const _TCHAR *szURL, ELoadOptions eLoadOptions)
{
    HRESULT hr = S_OK;

    m_szURL  = szURL;

    hr = Reload(eLoadOptions);

    return hr;
}


HRESULT CConfigManager::Reload(ELoadOptions eLoadOptions)
{
    HRESULT hr = S_OK;

	m_eLoadOptions = eLoadOptions;

	switch (eLoadOptions)
	{
		case eReplace :
			m_mapValues.clear();
			break;
		case eDoNotReplace :
		case eMerge :
		case eInvalid :
		default :
			break;
	}

    ITRACE_PRODUCTION(_T("Configuration Manager loading:  ") + IString(m_szURL.c_str()));

    CXMLParser parser(GetTraceHandle());

    hr = parser.SetContentHandler(this);

    if (hr == S_OK)
        hr = parser.ParseURL(m_szURL.c_str());
    
    if (hr != S_OK)
    {
        const CXMLErrorInfo &errorInfo = parser.GetErrorInfo();

        ITRACE_ERROR(IString(errorInfo));

        GetErrorLocationText0();        //lint !e534
    }

    _ASSERTE( _CrtCheckMemory() );

    m_bValid = (hr == S_OK);

    _ASSERTE(m_bValid);                 //lint !e717

    m_pLocator = (ISAXLocator __RPC_FAR *)NULL;

    return hr;
}


bool CConfigManager::IsValid() const { return m_bValid; }


HRESULT STDMETHODCALLTYPE CConfigManager::putDocumentLocator( 
            /* [in] */ ISAXLocator __RPC_FAR *pLocator
            )
{
    m_pLocator = pLocator;

    return S_OK;
}


//DEFINE_START_DOCUMENT(CConfigManager)
HRESULT STDMETHODCALLTYPE CConfigManager::startDocument()
{
    xpath.clear();

    return S_OK;
}


HRESULT __stdcall CConfigManager::startElement (
    /*[in]*/ unsigned short *					/* pwchNamespaceUri */,
    /*[in]*/ int								/* cchNamespaceUri */,
    /*[in]*/ unsigned short * pwchLocalName,
    /*[in]*/ int cchLocalName,
    /*[in]*/ unsigned short *					/* pwchQName */,
    /*[in]*/ int								/* cchQName */,
    /*[in]*/ struct MSXML2::ISAXAttributes *	/* pAttributes */)
{
    StringType szElement = XMLUtilities::getString(pwchLocalName, cchLocalName);

    m_stackValues.push_front(m_szCurrentValue);

    m_szCurrentValue.clear();
    
    // Build the XPath expression representing this node and save it on the stack
    // Skip the highest level tag name.  We are assuming that that tag does not matter
    // in the key names for configuration parameters.
    if (xpath.empty())
    {
        xpath=_T("/");
    }
    else
    {
        xpath+=_T("/");
        xpath+=szElement;
    }

    // Save the XPath expression representing this node
    m_stackXpathExprs.push_front(xpath);

    return S_OK;
}


HRESULT __stdcall CConfigManager::endElement (
    /*[in]*/ unsigned short *   /* pwchNamespaceUri */,
    /*[in]*/ int                /* cchNamespaceUri */,
    /*[in]*/ unsigned short *   /* pwchLocalName */,
    /*[in]*/ int                /* cchLocalName */,
    /*[in]*/ unsigned short *   /* pwchQName */,
    /*[in]*/ int                /* cchQName */ )
{
    HRESULT hresult = S_OK;

    IString szCurrentValue(m_szCurrentValue.c_str()); 
    m_szCurrentValue = szCurrentValue.trim(); 

    if (loggable().IsDebug())
    {
        StringType szDebug = xpath;
        szDebug += _T(":  ");
        szDebug += m_szCurrentValue;
        ITRACE_DEVELOP(szDebug.c_str());
    }

    if (m_eLoadOptions == eMerge)
        m_mapValues.erase(xpath);

    m_mapValues.insert(make_pair(xpath, m_szCurrentValue));

    // Restore the value to the previous value saved on the stack
    if (!m_stackValues.empty())
        m_szCurrentValue = m_stackValues.front();
    else
        m_szCurrentValue = _T("");
    m_stackValues.pop_front();

    // Restore the XPath to the previous value saved on the stack
    m_stackXpathExprs.pop_front();
    if (!m_stackXpathExprs.empty())
        xpath = m_stackXpathExprs.front();
    else
        xpath = _T("");

    return hresult;
}


HRESULT STDMETHODCALLTYPE CConfigManager::characters(
            /* [in] */ wchar_t __RPC_FAR *pwchChars,
            /* [in] */ int cchChars)
{
    m_szCurrentValue += XMLUtilities::getString(pwchChars, cchChars);

    return S_OK;
}


void CConfigManager::LogError(const _TCHAR *pszText) const
{
    StringType szError(pszText);
    
    szError += _T(".  ");
    szError += GetErrorLocationText();

    ITRACE_ERROR(szError.c_str());
}


const _TCHAR *CConfigManager::GetErrorLocationText() const
{
    return m_szErrorLocationText.c_str();
}


//lint -e{64, 119}	Lint gets confused about ISAXLocator
const _TCHAR *CConfigManager::GetErrorLocationText0()
{
    if (m_pLocator != NULL)
    {
        int nLineNo;
        int nColNo;
        wchar_t *pszDocumentName;

        m_szErrorLocationText = _T("Document:  ");
        HRESULT hr = getDocumentLocator()->getPublicId( &pszDocumentName );
        if (hr == S_OK)
        {
            CW2T sDocumentName(pszDocumentName);
            m_szErrorLocationText += sDocumentName;
        }
        else
            m_szErrorLocationText += _T("<Unknown>");

        m_szErrorLocationText += _T("  Line:  ");
        hr = getDocumentLocator()->getLineNumber(&nLineNo);
        if (hr == S_OK)
            m_szErrorLocationText += IString(nLineNo);
        else
            m_szErrorLocationText += _T("<Unknown>");

        m_szErrorLocationText += _T("  Column:  ");
        hr = getDocumentLocator()->getColumnNumber(&nColNo);
        if (hr == S_OK)
            m_szErrorLocationText += IString(nColNo);
        else
            m_szErrorLocationText += _T("<Unknown>");
    }
    else
    {
        m_szErrorLocationText += _T("No location information is available");
    }

    return m_szErrorLocationText.c_str();
}


HRESULT CConfigManager::GetString(const _TCHAR *szValueName, _TCHAR ** ppszRetValue)
{
    *ppszRetValue = NULL;

    HRESULT hr = E_FAIL;

    CConfigMap::iterator iter = m_mapValues.find(szValueName);
    if (iter != m_mapValues.end())
    {
        *ppszRetValue = const_cast<_TCHAR *>(iter->second.c_str());
		// +TAR 267879
		if (*ppszRetValue[0] == 0 )
			hr = E_FAIL;
		else
			hr = S_OK;
		// -TAR 267879
    }

    return hr;
}

HRESULT CConfigManager::SetValue(const _TCHAR *szValueName, const _TCHAR * szValue)
{
    
    HRESULT hr = E_FAIL;

    CConfigMap::iterator iter = m_mapValues.find(szValueName);
    if (iter != m_mapValues.end())
    {
		m_mapValues.erase(iter);
    }

    hr = S_OK;

	m_mapValues.insert(make_pair(szValueName, szValue));

    return hr;
}

void CConfigManager::ReleaseInstance()
{
    CSingleton<CConfigManager>::Release();
}

