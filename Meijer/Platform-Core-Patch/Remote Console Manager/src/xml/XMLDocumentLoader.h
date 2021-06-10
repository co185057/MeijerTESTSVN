#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/XMLDocumentLoader.h 1     6/20/08 1:14p Sa250050 $
/*
 * @(#)XMLDocumentLoader.h    $Revision: 1 $ $Date: 6/20/08 1:14p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/XMLDocumentLoader.h $
 * 
 * 1     6/20/08 1:14p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     2/28/05 3:24p Dm185016
 * lint
 * 
 * 2     2/04/05 10:41a Dm185016
 * 
 * 1     1/26/05 2:34p Dm185016
 * Moved to new Repository.
* 
* 4     10/05/04 8:33a Dm185016
* Trace object no longer shared.
* 
* 2     8/04/04 3:23p Dm185016
* Fixed to use conforming macros for parm names.
* 
* 1     7/13/04 3:17p Dm185016
*/

//lint --e{1516}

#include "ContentHandler.h"
#include "property.h"
#include <list>
#include "utils.h"

#pragma warning( push )
#pragma warning( disable : 4251 )

using namespace std;

class SAXErrorHandlerImpl;
class SAXLexicalHandlerImpl;

typedef list <StringType> CXPathStack;
typedef list <CContentHandler *> CHandlerStack;
typedef list <ISAXLocator *> CLocatorStack;

class UTILS_API CXMLDocumentLoader
    : public CContentHandler
{
public:
    CXMLDocumentLoader(HTRACE hTrace);
    ~CXMLDocumentLoader(void);
	
	HRESULT STDMETHODCALLTYPE Reset();

    virtual void registerHandlers(void)=0;
    
    virtual HRESULT STDMETHODCALLTYPE putDocumentLocator( 
        /* [in] */ ISAXLocator __RPC_FAR *pLocator);

    virtual HRESULT STDMETHODCALLTYPE startDocument( void);
    
    virtual HRESULT STDMETHODCALLTYPE endDocument( void);

    virtual HRESULT STDMETHODCALLTYPE startElement (
        /*[in]*/ unsigned short * CH_NAMESPACE_URI,
        /*[in]*/ int CH_NAMESPACE_URI_LEN,
        /*[in]*/ unsigned short * CH_LOCAL_NAME,
        /*[in]*/ int CH_LOCAL_NAME_LEN,
        /*[in]*/ unsigned short * CH_QUALIFIED_NAME,
        /*[in]*/ int CH_QUALIFIED_NAME_LEN,
        /*[in]*/ struct MSXML2::ISAXAttributes * CH_PATTRIBUTES );

    virtual HRESULT STDMETHODCALLTYPE endElement (
        /*[in]*/ unsigned short * CH_NAMESPACE_URI,
        /*[in]*/ int CH_NAMESPACE_URI_LEN,
        /*[in]*/ unsigned short * CH_LOCAL_NAME,
        /*[in]*/ int CH_LOCAL_NAME_LEN,
        /*[in]*/ unsigned short * CH_QUALIFIED_NAME,
        /*[in]*/ int CH_QUALIFIED_NAME_LEN );

    virtual HRESULT STDMETHODCALLTYPE characters(
        /* [in] */ wchar_t __RPC_FAR *CH_CHARACTERS,
        /* [in] */ int CH_CHARACTERS_LEN);

    virtual ISAXLocator __RPC_FAR * getDocumentLocator() { return m_pLocator; }

    void LogError(const _TCHAR *pszText);

	const _TCHAR *GetErrorLocationText();
    
    void ReadFile(const TCHAR *pFn);

protected:

    inline CContentHandler &CurrentHandler() { return *handlerStack.front(); }
    inline CContentHandler &PreviousHandler() { return *m_pPreviousHandler;	}
    inline bool StackEmpty() const { return handlerStack.empty(); }
    inline bool IsError() const { return m_pPreviousHandler == NULL; }

    virtual SAXErrorHandlerImpl *GetErrorHandler();
    virtual SAXLexicalHandlerImpl *GetLexicalHandler();

    virtual bool IsFailOnMissingHandler() { return true; }

    ILoggable &xpathLoggable();

    virtual IString substituteEnvVars(const IString &);

private:

    CXMLDocumentLoader();
    CXMLDocumentLoader(const CXMLDocumentLoader &);               // Hide the Copy construcor
    CXMLDocumentLoader &operator =(const CXMLDocumentLoader &);   // Hide the assignment operator

    CXPathStack xpathExprs;
    CHandlerStack handlerStack;
	CLocatorStack locatorStack;

    CContentHandler *m_pPreviousHandler;

    ISAXLocator *m_pLocator;

	StringType m_szErrorLocationText;
    
    ILoggable *m_pLoggable;
};

#pragma warning( pop )
