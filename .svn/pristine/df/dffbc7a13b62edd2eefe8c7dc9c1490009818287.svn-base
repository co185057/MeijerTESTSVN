// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/ContentHandler.cpp 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)ContentHandler.cpp    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/ContentHandler.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 5     3/24/05 7:39p Dm185016
 * UNICODE
 * 
 * 4     2/28/05 3:23p Dm185016
 * lint
*/

#include "StdAfx.h"
#include "contenthandler.h"
#include "xtrace.h"
#include "xerror.h"
#include <atlrx.h>

XPath CContentHandler::xpath = _T("");
CHMap CContentHandler::handlers;
CHMap CContentHandler::regexs;

CHVector CContentHandler::m_vSingletonHandlers;

CContentHandler CContentHandler::nullHandler;


//ILoggable *CContentHandler::m_pLoggable = NULL;


CContentHandler::CContentHandler(void)
    : maxHandlers(0)
    , m_pContent(NULL)
    , m_pLoggable(NULL)
{
//  IMODTRACE_DEVELOP();
}


CContentHandler::~CContentHandler(void)
{
//  IMODTRACE_DEVELOP();
    m_pContent  = NULL;
    m_pLoggable = NULL;
}


void CContentHandler::ReleaseHandlers()
{
//  IMODTRACE_DEVELOP();

    CHVector::iterator iter = m_vSingletonHandlers.begin();
    PRELEASE_FXN pReleaseFxn;
    for (;iter != m_vSingletonHandlers.end(); iter++)
    {
        pReleaseFxn = *iter;
        (*pReleaseFxn)();
    }
}


DEFINE_START_DOCUMENT(CContentHandler)
{
    xpath = _T("");

    return S_OK;
}


CXMLContent *CContentHandler::Content()
{
    return m_pContent;
}


CContentHandler *CContentHandler::getHandler(const _TCHAR * name) const 
{
    CHMapIterator iter = handlers.find( name );
    if (iter != handlers.end())
    {
        PHANDLER pHandler = iter->second;
        return pHandler;
    }
//  else
//  {
//      ITRACE_DEVELOP("Unable to determine handler");
//  }

    return NULL;
}


CContentHandler *CContentHandler::getHandlerUsingRegex(const _TCHAR * name) const 
{
    CHMapIterator iter = regexs.begin();
	for (;iter!=regexs.end();iter++)
    {
        StringType tempXpath = iter->first;

		CAtlRegExp<> regex;
		// five match groups: scheme, authority, path, query, fragment
		REParseError status = regex.Parse( tempXpath.c_str() );
		if (REPARSE_ERROR_OK != status)
			throw IException(_T("Invalid regular expression"), E_FAIL);

		CAtlREMatchContext<> mc;

		bool bMatched = (TRUE == regex.Match(name, &mc));
		if (bMatched && loggable().IsTracing())
		{
			const CAtlREMatchContext<>::RECHAR* szStart = mc.m_Match.szStart;
			const CAtlREMatchContext<>::RECHAR* szEnd = mc.m_Match.szEnd;

			ptrdiff_t nLength = szEnd - szStart;
			IString text;
			text.format(_T("Regex Match: %.*s\n"), nLength, szStart);
			ITRACE_DEVELOP(text);
		}

		if (bMatched)
			break;
    }

	if (iter != regexs.end())
		return iter->second;

    return NULL;
}


CHMap &CContentHandler::getHandlers() 
{ 
    return handlers; 
}


bool CContentHandler::containsHandler(const _TCHAR * name) const 
{
    CHMapIterator iter = handlers.find(const_cast<_TCHAR *>(name)); 

    return iter != handlers.end();
}


bool CContentHandler::containsRegex(const _TCHAR * name) const 
{
    CHMapIterator iter = regexs.find(const_cast<_TCHAR *>(name)); 

    return iter != regexs.end();
}


StringType CContentHandler::DebugString() const
{
    IMODTRACE_DEVELOP();
    return StringType(_T("some text"));
}


ILoggable &CContentHandler::loggable() const
{ 
    if (m_pLoggable == NULL)
        m_pLoggable = &Trace::getLoggable(GetTraceHandle(), eDCContentHandler);

    return *m_pLoggable; 
}
