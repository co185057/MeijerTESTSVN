// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/XMLParser.cpp 1     6/20/08 1:14p Sa250050 $
/*
 * @(#)XMLParser.cpp      $Revision: 1 $  $Date: 6/20/08 1:14p $
 *
 * Copyright 2004, NCR Corporation. All rights reserved.
 */
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/XMLParser.cpp $
 * 
 * 1     6/20/08 1:14p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 9     6/24/05 1:42p Dm185016
 * Added method to return the error handler.
 * 
 * 7     5/03/05 4:12p Dm185016
 * Cleaned up tracing
 * 
 * 5     3/24/05 7:40p Dm185016
 * UNICODE
 * 
 * 4     2/28/05 3:24p Dm185016
 * lint
 * 
 * 3     2/24/05 2:33p Tk124825
 * TAR 292704
 * 
 * 2     1/27/05 2:39p Dm185016
 * XMLParser now maintains its own copy of the error information.
 * 
 * 1     1/26/05 2:34p Dm185016
 * Moved to new Repository.
// 
// 12    1/18/05 4:08p Dm185016
// UNICODE support
// 
// 11    10/14/04 1:54p Dm185016
// Added environment variable support
// 
// 8     6/30/04 3:54p Dm185016
// Lint.
// Added Lexical handler for processing comments.
// 
// 7     4/20/04 4:47p Dm185016
// Expanded the error reporting mechanism
// 
// 6     4/12/04 3:33p Dm185016
// Disabled warning
// 
// 5     3/19/04 3:34p Dm185016
// SAX reader now created by PROGID rather than __uuid
// 
// 4     2/10/04 11:56a Dm185016
// Cleaned up error handling
*/

//lint -esym(534, IUnknown::Release)

#include "stdafx.h"
#include "XMLParser.h"
#include "xerror.h"
#include "SAXContentHandlerImpl.h"
#include "SAXErrorHandlerImpl.h"
#include "SAXLexicalHandlerImpl.h"
#include "Loggable.h"

#pragma warning( push )
#pragma warning( disable : 4311 )

CXMLParser::CXMLParser(HTRACE hTrace)
    : pSAXRdr(NULL)
    , pCHMaster(NULL)
	, m_pErrorHandler(NULL)
	, m_pLexicalHandler(NULL)
    , m_pError(NULL)
    , m_pLoggable(NULL)
{
    //  Create an instance of SAXReader

    CTOR_BEGIN(hTrace)

    LPOLESTR lpolestr;
    HRESULT hr = ::StringFromCLSID(__uuidof(MSXML2::SAXXMLReader40), &lpolestr);
    VERIFY(hr==S_OK);
    CW2T suuid(lpolestr);
    if (loggable().IsDebug()) ITRACE_DEVELOP(_T("Original uuid:  ") + IString((const _TCHAR *)suuid));
    ::CoTaskMemFree(lpolestr);

    CLSID clsidSAXXMLReader40;
    std::wstring wszSAXXMLReader40 = L"Msxml2.SAXXMLReader.4.0";
    hr = ::CLSIDFromProgID(wszSAXXMLReader40.c_str(), &clsidSAXXMLReader40);
    VERIFY(hr==S_OK);

    hr = ::StringFromCLSID(clsidSAXXMLReader40, &lpolestr);
    VERIFY(hr==S_OK);
    CW2T sStringFromCLSID(lpolestr);
    if (loggable().IsDebug()) ITRACE_DEVELOP(_T("From String uuid:  ") + IString((const _TCHAR *)sStringFromCLSID));
    ::CoTaskMemFree(lpolestr);

    hr = ::StringFromIID(__uuidof(MSXML2::ISAXXMLReader), &lpolestr);
    VERIFY(hr==S_OK);
    CW2T sStringFromIID(lpolestr);
    if (loggable().IsDebug()) ITRACE_DEVELOP(_T("Original Interface uuid:  ") + IString((const _TCHAR *)sStringFromIID));
    ::CoTaskMemFree(lpolestr);

    hr = ::CoCreateInstance(clsidSAXXMLReader40, NULL, CLSCTX_ALL,
                                  __uuidof(MSXML2::ISAXXMLReader), (void **)&pSAXRdr);
//  hr = CoCreateInstance(__uuidof(MSXML2::SAXXMLReader40), NULL, CLSCTX_ALL,
//                                __uuidof(MSXML2::ISAXXMLReader), (void **)&pSAXRdr);

    if (loggable().IsDebug()) ITRACE_DEVELOP(_T("pSAXRdr=") + IString((long)pSAXRdr).d2x());
    if (hr != S_OK)
    {
        ISystemErrorInfo error(hr, _T("CoCreateInstance"));

        ITRACE_ERROR((const _TCHAR *)error);

        ITHROWSYSTEMERROR(hr, CoCreateInstance, IException::unrecoverable); 
    }

    CTOR_END()
}


HRESULT CXMLParser::SetContentHandler(SAXContentHandlerImpl *pHandler)
{
    pCHMaster       = pHandler;

    HRESULT hr = pSAXRdr->putContentHandler(pCHMaster);

    return hr;
}


HRESULT CXMLParser::SetErrorHandler(SAXErrorHandlerImpl *pHandler)
{
    m_pErrorHandler   = pHandler;

    HRESULT hr = pSAXRdr->putErrorHandler(m_pErrorHandler);

    return hr;
}


SAXErrorHandlerImpl *CXMLParser::GetErrorHandler()
{
    return m_pErrorHandler;
}


//lint --e{1776}
HRESULT CXMLParser::SetLexicalHandler(SAXLexicalHandlerImpl *pHandler)
{
    _variant_t varParm = (IUnknown *)pHandler;

    HRESULT hr = pSAXRdr->putProperty(L"http://xml.org/sax/properties/lexical-handler", varParm);

    return hr;
}



CXMLParser::~CXMLParser(void)
{
    IMODTRACE_DEVELOP_EX(eDCXml, eDCXml);
    if (loggable().IsDebug()) ITRACE_DEVELOP(_T("pSAXRdr=") + IString((long)pSAXRdr).d2x());

    pSAXRdr->Release();

    delete m_pErrorHandler;
    delete m_pLexicalHandler;
    delete m_pError;

    pCHMaster = NULL;
}


#pragma warning( pop )


void CXMLParser::CheckError(HRESULT hr)
{
    _ASSERTE(m_pErrorHandler!=NULL);
    if (hr != S_OK)
    {
        CXMLErrorInfo *pError = const_cast<CXMLErrorInfo *>(m_pErrorHandler->GetErrorInfo());
        if (pError == NULL)
        {
            m_pError = new CXMLErrorInfo(hr);
        }
        else
        {
            m_pError = dynamic_cast<CXMLErrorInfo *>(pError->copy());
        }
    }
}


/*
* Parses the XML document residing in a file of the given name.
* @param xmlFile The name of the file containing the XML document.
* @return Returns S_OK if the parse completed with no errors.  Anything
*         else represents an error detected during the parsing.
*/
HRESULT CXMLParser::ParseURL(const CString &xmlFile)
{
    return ParseURL((const _TCHAR *)xmlFile);
}


HRESULT CXMLParser::ParseURL(const _TCHAR *pXmlFile)
{
    HRESULT hr = S_OK;
    _ASSERTE(pCHMaster!=NULL);

    if (m_pErrorHandler == NULL)
        hr = SetErrorHandler( new SAXErrorHandlerImpl );

    if (hr == S_OK)
    {
        CT2W sWXMLFile(pXmlFile);

        hr = pSAXRdr->parseURL(sWXMLFile);
    }

    CheckError(hr);

    return hr;
}


HRESULT CXMLParser::Parse(const wchar_t *szDocument)
{
    _ASSERTE(pCHMaster!=NULL);

    CComVariant document(szDocument);
    HRESULT hr = pSAXRdr->parse(document);

    CheckError(hr);

    return hr;
}


HRESULT CXMLParser::Parse(const char *szDocument)
{
    _ASSERTE(pCHMaster!=NULL);

    CComVariant document(szDocument);
    HRESULT hr = pSAXRdr->parse(document);

    CheckError(hr);

    return hr;
}


const CXMLErrorInfo &CXMLParser::GetErrorInfo() const
{ 
    _ASSERTE(m_pError!=NULL);
	return *m_pError; 
}


HRESULT CXMLParser::AddEnvVariable(const _TCHAR *pszVarName
                                 , const _TCHAR *pszValue)
{
    m_mapVariables.insert(std::make_pair(pszVarName, pszValue));

    return S_OK;
}


HRESULT CXMLParser::AddEnvVariable(const StringType &sVarName
                                 , const StringType &sValue)
{
    m_mapVariables.insert(std::make_pair(sVarName, sValue));

    return S_OK;
}


const _TCHAR * CXMLParser::GetEnvVariable(const StringType &sVarName)
{
    CEnvVariables::iterator iter = m_mapVariables.find(sVarName);
    if (iter != m_mapVariables.end())
    {
        const StringType &value = iter->second;
        return value.c_str();
    }

    return _T("");
}


ILoggable &CXMLParser::loggable()
{ 
    if (m_pLoggable == NULL)
        m_pLoggable = &Trace::getLoggable(GetTraceHandle(), eDCXml);

    return *m_pLoggable; 
}
