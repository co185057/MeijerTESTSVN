#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/XMLParser.h 1     6/20/08 1:14p Sa250050 $
/*
 * @(#)XMLParser.h      $Revision: 1 $  $Date: 6/20/08 1:14p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/XMLParser.h $
 * 
 * 1     6/20/08 1:14p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 6     6/24/05 1:42p Dm185016
 * Added method to return the error handler.
 * 
 * 4     5/03/05 4:12p Dm185016
 * Cleaned up tracing
 * 
 * 2     2/28/05 3:24p Dm185016
 * lint
 * 
 * 1     1/26/05 2:34p Dm185016
 * Moved to new Repository.
* 
* 10    1/18/05 4:08p Dm185016
* UNICODE support
* 
* 9     10/14/04 1:54p Dm185016
* Added environment variable support
* 
* 6     6/30/04 3:54p Dm185016
* Lint.
* Added Lexical handler for processing comments.
* 
* 5     4/20/04 4:47p Dm185016
* Expanded the error reporting mechanism
* 
* 4     2/10/04 11:56a Dm185016
* Cleaned up error handling
*/

#include "XMLError.h"
#include "utils.h"
#include "ContentHandlerOwner.h"
#include <map>

class SAXContentHandlerImpl;
class SAXErrorHandlerImpl;
class SAXLexicalHandlerImpl;
class CXMLContent;

typedef std::map <StringType, StringType, StringComparator > CEnvVariables;

using namespace std;

class UTILS_API CXMLParser
    : public CBaseObject
    , public IContentHandlerOwner
{
public:

    CXMLParser(HTRACE);
    virtual ~CXMLParser(void);


    /*
    * Parses the XML document residing in a file of the given name.
    * @param xmlFile The name of the file containing the XML document.
    * @return Returns S_OK if the parse completed with no errors.  Anything
    *         else represents an error detected during the parsing.
    */
    HRESULT ParseURL(const _TCHAR *pXmlFile);


    /*
    * Parses the XML document residing in a file of the given name.
    * @param xmlFile The name of the file containing the XML document.
    * @return Returns S_OK if the parse completed with no errors.  Anything
    *         else represents an error detected during the parsing.
    */
    HRESULT ParseURL(const CString &xmlFile);


    /*
    * Parses the XML document given in the argument.
    * @param szDocument The XML document to parse.
    * @return Returns S_OK if the parse completed with no errors.  Anything
    *         else represents an error detected during the parsing.
    */
    HRESULT Parse(const wchar_t *szDocument);


    /*
    * Parses the XML document given in the argument.
    * @param szDocument The XML document to parse.
    * @return Returns S_OK if the parse completed with no errors.  Anything
    *         else represents an error detected during the parsing.
    */
    HRESULT Parse(const char *szDocument);


    /*
    * Sets the ContentHandler to the given object.
    * @param pCH A pointer to the handler to use.
    * @return Returns S_OK.
    */
    HRESULT SetContentHandler(SAXContentHandlerImpl *pCH);


    /*
    * Sets the ErrorHandler to the given object.
    * @param pEH A pointer to the handler to use.
    * @return Returns S_OK.
    */
    HRESULT SetErrorHandler(SAXErrorHandlerImpl *pEH);


    /*
    * Returns the ErrorHandler assigned to this parser.
    */
    SAXErrorHandlerImpl *GetErrorHandler();


    /*
    * Sets the LexicalHandler to the given object.
    * @param pEH A pointer to the handler to use.
    * @return Returns S_OK.
    */
    HRESULT SetLexicalHandler(SAXLexicalHandlerImpl *pEH);


    HRESULT AddEnvVariable(const _TCHAR *pszVarName, const _TCHAR *pszValue);
    HRESULT AddEnvVariable(const StringType &pszVarName, const StringType &pszValue);
    const _TCHAR * GetEnvVariable(const StringType &sVarName);


	const CXMLErrorInfo &GetErrorInfo() const;

protected:


private:

    ILoggable &loggable();

    void CheckError(HRESULT hr);

    CXMLParser();
    CXMLParser(const CXMLParser &);               // Hide the Copy construcor
    CXMLParser &operator =(const CXMLParser &);   // Hide the assignment operator

    MSXML2::ISAXXMLReader* pSAXRdr;
    SAXContentHandlerImpl* pCHMaster;
    SAXErrorHandlerImpl* m_pErrorHandler;
    SAXLexicalHandlerImpl* m_pLexicalHandler;

    CEnvVariables   m_mapVariables;

	CXMLErrorInfo *m_pError;

    ILoggable *m_pLoggable;
};
