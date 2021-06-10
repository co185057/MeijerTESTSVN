// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/XMLDocumentLoader.cpp 1     6/20/08 1:14p Sa250050 $
/*
 * @(#)XMLDocumentLoader.cpp    $Revision: 1 $ $Date: 6/20/08 1:14p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/XMLDocumentLoader.cpp $
 * 
 * 1     6/20/08 1:14p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 9     5/03/05 4:12p Dm185016
 * Cleaned up tracing
 * 
 * 8     4/21/05 7:28p Dm185016
 * Cleaned up tracing
 * 
 * 6     3/24/05 7:39p Dm185016
 * UNICODE
 * 
 * 5     2/28/05 3:24p Dm185016
 * lint
 * 
 * 4     2/07/05 7:52p Dm185016
 * 
 * 3     2/07/05 7:19p Dm185016
 * TAR xxxxxx - Need to reset the XPath expression before use.  First
 * usage had junk left over from creation time.
 * 
 * 2     2/04/05 10:41a Dm185016
 * 
 * 1     1/26/05 2:34p Dm185016
 * Moved to new Repository.
// 
// 5     1/18/05 4:08p Dm185016
// UNICODE support
// 
// 4     10/05/04 8:33a Dm185016
// Trace object no longer shared.
// 
// 2     8/04/04 3:23p Dm185016
// Fixed to use conforming macros for parm names.
// 
// 1     7/13/04 3:17p Dm185016
*/

#include "StdAfx.h"
#include "XMLDocumentLoader.h"
#include "SAXErrorHandlerImpl.h"
#include "SAXLexicalHandlerImpl.h"
#include "XMLParser.h"


ILoggable &CXMLDocumentLoader::xpathLoggable() 
{ 
    if (m_pLoggable == NULL)
        m_pLoggable = &Trace::getLoggable(GetTraceHandle(), eDCContentHandler);

    return *m_pLoggable; 
}


CXMLDocumentLoader::CXMLDocumentLoader(HTRACE hTrace)
    : m_pPreviousHandler(NULL)
    , m_pLocator(NULL)
    , m_pLoggable(NULL)
{
    CTOR_BEGIN(hTrace)

    IMODTRACE_DEVELOP_EX(eDCXml, eDCXml);

    CTOR_END()
}


CXMLDocumentLoader::~CXMLDocumentLoader(void)
{
    IMODTRACE_DEVELOP_EX(eDCXml, eDCXml);

    try
    {
        ReleaseHandlers();

        CContentHandler::getHandlers().clear();
    }
    catch (...)
        { ITRACE_DOT_ERROR(); }

    m_pPreviousHandler  = NULL;
    m_pLocator          = NULL;
    m_pLoggable         = NULL;
}


void CXMLDocumentLoader::ReadFile(const TCHAR *pFn)
{
    if (loggable().IsDebug()) ITRACE_DEVELOP(_T(__FUNCTION__) _T(" Reading:  ") 
                                           + IString(pFn));

    // Create the XML parser.
    CXMLParser parser(GetTraceHandle());

    // Create the Lexical handler for the parser
    parser.SetLexicalHandler(GetLexicalHandler());

    // Create the error handler for the parser
    parser.SetErrorHandler(GetErrorHandler());

    parser.SetContentHandler(this);

    // Parse the document
	HRESULT hr = parser.ParseURL(pFn);
    if (hr != S_OK)
    {
		const CXMLErrorInfo &errorInfo = parser.GetErrorInfo();

        ITRACE_DEVELOP((const _TCHAR *)errorInfo);

        throw E_FAIL;
    }
}


HRESULT STDMETHODCALLTYPE CXMLDocumentLoader::putDocumentLocator( 
            /* [in] */ ISAXLocator __RPC_FAR *pLocator
            )
{
    //IMODTRACE_DEVELOP();

    m_pLocator = pLocator;

	locatorStack.push_front(m_pLocator);

    return S_OK;
}


//DEFINE_START_DOCUMENT(CXMLDocumentLoader)
HRESULT STDMETHODCALLTYPE CXMLDocumentLoader::Reset()
{
//  IMODTRACE_DEVELOP();

    xpath = _T("");

    m_pPreviousHandler = NULL;
    m_pContent = NULL;

    return S_OK;
}


//DEFINE_START_DOCUMENT(CXMLDocumentLoader)
DEFINE_START_DOCUMENT(CXMLDocumentLoader)
{
    return S_OK;
}


//DEFINE_END_DOCUMENT(CXMLDocumentLoader)
DEFINE_END_DOCUMENT(CXMLDocumentLoader)
{
    //IMODTRACE_DEVELOP();

	locatorStack.pop_front();
	m_pLocator = locatorStack.front();

    if (!IsError())
    {
        CContentHandler &previous = PreviousHandler();
        m_pContent = previous.Content();
    }

    return S_OK;
}


DEFINE_START_ELEMENT(CXMLDocumentLoader)
{
//  IMODTRACE_DEVELOP();

    StringType szElement = GET_LOCAL_NAME();
	
    // Build the XPath expression representing this node and save it on the stack
    xpath+=_T("/");
    xpath+=szElement;
    if (xpathLoggable().IsTracing())
    {
        StringType szDebug = _T("xpath:  ") + xpath;
        ITRACE_DEVELOP(szDebug.c_str());
    }

    // Obtain the registered handler for this XPath expression( /name1/name2 ).  If it cannot be
    // found, the retry using the more generic //name2
    CContentHandler *pHandler = getHandler(xpath.c_str());
    if (pHandler == NULL)
    {
        XPath tempXpath = _T("//") + szElement;
        if (xpathLoggable().IsTracing()) 
            ITRACE_DEVELOP(_T("Retry using:  ") + IString(tempXpath.c_str()));
        pHandler = getHandler(tempXpath.c_str());

        if (pHandler != NULL)
            xpath = tempXpath;
    }

    // If we still cannot find it, then try the regular expressions
    if (pHandler == NULL)
    {
        pHandler = getHandlerUsingRegex(xpath.c_str());
    }

    // If we still cannot find it, then substitute the null handler to prevent
    // problems
    if (pHandler == NULL)
    {
        LogError(_T("Handler is NULL for element:  ") + IString(szElement.c_str()) + _T(".  xpath:  ") + IString(xpath.c_str()));

        if (IsFailOnMissingHandler())
            return XML_UNDEFINED_TAG;

        pHandler = &nullHandler;
    }

    // Save the XPath expression representing this node
    xpathExprs.push_front(xpath);

    // Save the handler for this node
    handlerStack.push_front(pHandler);

    return CurrentHandler().startElement (
        CH_NAMESPACE_URI,
        CH_NAMESPACE_URI_LEN,
        CH_LOCAL_NAME,
        CH_LOCAL_NAME_LEN,
        CH_QUALIFIED_NAME,
        CH_QUALIFIED_NAME_LEN,
        CH_PATTRIBUTES );
}


DEFINE_END_ELEMENT(CXMLDocumentLoader)
{
//  IMODTRACE_DEVELOP();

    // Restore the XPath to the previous value saved on the stack
    xpathExprs.pop_front();
    if (!xpathExprs.empty())
        xpath = xpathExprs.front();
    else
        xpath = _T("");

    HRESULT hresult = 0;
    if (!StackEmpty())
        hresult = CurrentHandler().endElement (
                CH_NAMESPACE_URI,
                CH_NAMESPACE_URI_LEN,
                CH_LOCAL_NAME,
                CH_LOCAL_NAME_LEN,
                CH_QUALIFIED_NAME,
                CH_QUALIFIED_NAME_LEN );

    // Remove this handler from the stack since we are finished with it for now.
    m_pPreviousHandler = handlerStack.front();
    handlerStack.pop_front();

    return hresult;
}


HRESULT STDMETHODCALLTYPE CXMLDocumentLoader::characters(
            /* [in] */ wchar_t __RPC_FAR *CH_CHARACTERS,
            /* [in] */ int CH_CHARACTERS_LEN)
{
//  IMODTRACE_DEVELOP();

    StringType szValue = GET_CHARACTERS();

    IString chars(szValue.c_str());
    chars = chars.trim();
    if (chars.size() != 0)
    {
        if (xpathLoggable().IsTracing())
            ITRACE_DEVELOP("characters:  \"" + chars + "\"");
    }

    IString finalChars = substituteEnvVars(chars);

    return CurrentHandler().characters(
        const_cast<wchar_t *>((const wchar_t *)finalChars),
        (int)finalChars.size() );
}


IString CXMLDocumentLoader::substituteEnvVars(const IString &sInput)
{
    IString sOutput = sInput;

    return sOutput;
}


void CXMLDocumentLoader::LogError(const _TCHAR *pszText)
{
	StringType szError(pszText);
    
    szError += _T(".  ");
	szError += GetErrorLocationText();

    ITRACE_ERROR(szError.c_str());
}


const _TCHAR *CXMLDocumentLoader::GetErrorLocationText()
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
            CW2T temp(pszDocumentName);
			m_szErrorLocationText += (const _TCHAR *)temp;
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


SAXErrorHandlerImpl *CXMLDocumentLoader::GetErrorHandler()
{
    return new SAXErrorHandlerImpl;
}


SAXLexicalHandlerImpl *CXMLDocumentLoader::GetLexicalHandler()
{
    return new SAXLexicalHandlerImpl;
}
