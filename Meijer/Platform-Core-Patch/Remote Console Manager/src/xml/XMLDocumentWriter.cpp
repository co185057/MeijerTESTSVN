// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/XMLDocumentWriter.cpp 1     6/20/08 1:14p Sa250050 $
/*
 * @(#)XMLDocumentWriter.cpp    $Revision: 1 $ $Date: 6/20/08 1:14p $
 *
 * Copyright 2004, NCR Corporation.
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with msi.
 */
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/XMLDocumentWriter.cpp $
 * 
 * 1     6/20/08 1:14p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:34p Dm185016
 * Moved to new Repository.
// 
// 1     7/20/04 3:43p Dm185016
*/ 
#include "StdAfx.h"
#include "XMLDocumentWriter.h"
#include "verify.h"

//lint --e{534}
//lint --e{666}
wchar_t *CXMLDocumentWriter::pBuffer = NULL;
size_t CXMLDocumentWriter::nBufferLength = 0;

CXMLDocumentWriter::CXMLDocumentWriter(void)
    : attrcount(0)
    , nullAttrString(L"")
{
    IMODTRACE_DEVELOP();

    EVAL_HR(
        CoCreateInstance(
            __uuidof(MXXMLWriter), NULL, CLSCTX_ALL,
            __uuidof(IMXWriter), (void **)&pWriter));
  
    //VARIANT_BOOL bOmitXMLDeclaration = VARIANT_TRUE;

    //HRESULT hr = pWriter->put_omitXMLDeclaration(bOmitXMLDeclaration);
    //if (S_OK != hr)
    //{
    //    ITRACE_DEVELOP("put_omitXMLDeclaration failed");
    //}
    VARIANT_BOOL bDisableOutputEscaping = VARIANT_TRUE;

    HRESULT hr = pWriter->put_disableOutputEscaping(bDisableOutputEscaping);
    if (S_OK != hr)
    {
        ITRACE_DEVELOP("put_disableOutputEscaping failed");
    }
    VARIANT_BOOL bindent = VARIANT_TRUE;

    hr = pWriter->put_indent(bindent);
    if (S_OK != hr)
    {
        ITRACE_DEVELOP("put_indent failed");
    }
    
    _bstr_t encoding = _T("utf-8");
    hr = pWriter->put_encoding(encoding);
    if (S_OK != hr)
    {
        ITRACE_DEVELOP("put_encoding failed");
    }


    EVAL_HR(
        CoCreateInstance(
            __uuidof(SAXAttributes), NULL, CLSCTX_ALL,
            __uuidof(IMXAttributes), (void **)&pMXAttrs));
}


//lint --e{534}
CXMLDocumentWriter::~CXMLDocumentWriter(void)
{
	try
	{
		pWriter->Release();
		pMXAttrs->Release();

		pWriter = NULL;
		pMXAttrs = NULL;

        delete [] pBuffer;
	}
	catch (...)
		{ ITRACE_ERROR("Exception caught"); }
}


wchar_t *CXMLDocumentWriter::GetString(const std::string &theString) const
{
	size_t nLength = theString.size();

	if (nLength > nBufferLength)
	{
		delete [] pBuffer;

		nBufferLength = nLength+1;
		pBuffer = new wchar_t[nBufferLength];
	}

	mbstowcs(pBuffer, theString.c_str(), nBufferLength);

	return pBuffer;
}


wchar_t *CXMLDocumentWriter::GetString(const std::wstring &theString) const
	{ return const_cast<wchar_t *>(theString.c_str()); }


StringType CXMLDocumentWriter::GetXMLOutput() const
{
	StringType retValue = _T("");
    CComVariant cvOutput;
    HRESULT hr = pWriter->get_output(&cvOutput);
	if (hr == S_OK)
	{
		BSTR xmlOutput = cvOutput.bstrVal;

		size_t length = wcslen(xmlOutput);
		_TCHAR *pszXml  = new _TCHAR[length+1];
        if (length > 0)
		    wcstombs(pszXml, xmlOutput, length);
        pszXml[length] = 0;

		::SysFreeString(xmlOutput);

		retValue = pszXml;

		delete [] pszXml;
	}

    return retValue;
}


void CXMLDocumentWriter::AddAdornments() {}


void CXMLDocumentWriter::StartDocument()
{
//  IMODTRACE_DEVELOP();

    VERIFY_AND_LOG(pWriter->put_output(CComVariant(L"")));

    CComQIPtr<ISAXContentHandler> pContentHandler(pWriter);

    pContentHandler->startDocument();
}


void CXMLDocumentWriter::EndDocument()
{
    CComQIPtr<ISAXContentHandler> pContentHandler(pWriter);

    HRESULT hr;

    hr = pContentHandler->endDocument();

 //   CComVariant cvOutput(L"");
 //   hr = pWriter->get_output(&cvOutput);
	//VERIFY(hr==S_OK);

 //   m_szXMLOutput = cvOutput.bstrVal;
}

static wchar_t nullstring[] = L"";
static int nulllength = (int)lstrlenW(nullstring);
void CXMLDocumentWriter::StartElement(const StringType &tagname)
{
//  IMODTRACE_DEVELOP();

    CComQIPtr<ISAXContentHandler> pContentHandler(pWriter);

//  ITRACE_DEVELOP("tagname:  " + IString(tagname.c_str()));
//  ITRACE_DEVELOP("Attribute count:  " + IString(attrcount));
    if (attrcount != 0)
    {
        CComQIPtr<ISAXAttributes> pSAXAttrs(pMXAttrs);

        pContentHandler->startElement(nullstring, nulllength,
                          nullstring, nulllength,
                          GetString(tagname), (int)tagname.size(),
                          pSAXAttrs);

        ClearAttributes();
    }
    else
    {
        pContentHandler->startElement(nullstring, nulllength,
                            nullstring, nulllength,
                            GetString(tagname), (int)tagname.size(),
                            NULL);
    }
}


void CXMLDocumentWriter::EndElement(const StringType &tagname)
{
//  IMODTRACE_DEVELOP();

//  ITRACE_DEVELOP(IString("tagname=") + tagname.c_str());

    CComQIPtr<ISAXContentHandler> pContentHandler(pWriter);

    pContentHandler->endElement(nullstring, nulllength,
                        nullstring, nulllength,
                        GetString(tagname), (int)tagname.size());
}


void CXMLDocumentWriter::ClearAttributes()
{
//  IMODTRACE_DEVELOP();

    pMXAttrs->clear();
    attrcount = 0;
}


void CXMLDocumentWriter::AddAttribute(const StringType &attrname, const StringType &attrvalue)
{
    HRESULT hr = pMXAttrs->addAttribute(
                        (BSTR)nullAttrString, 
                        (BSTR)nullAttrString,
                        (BSTR)CComBSTR(GetString(attrname)),
                        (BSTR)nullAttrString,
                        (BSTR)CComBSTR(GetString(attrvalue)));
    VERIFY(hr==S_OK);
    attrcount++;
}


void CXMLDocumentWriter::AddAttribute(const StringType &attrname, const _TCHAR *attrvalue)
{
    HRESULT hr = pMXAttrs->addAttribute(
                        (BSTR)nullAttrString, 
                        (BSTR)nullAttrString,
                        (BSTR)CComBSTR(GetString(attrname)),
                        (BSTR)nullAttrString,
                        (BSTR)CComBSTR(GetString(attrvalue)));
	VERIFY(hr==S_OK);
    attrcount++;
}


void CXMLDocumentWriter::AddAttribute(const StringType &attrname, short sValue)
{
    wchar_t buffer[20];
    _itow((int)sValue, buffer, 10);

    HRESULT hr = pMXAttrs->addAttribute(
                        (BSTR)nullAttrString, 
                        (BSTR)nullAttrString,
                        (BSTR)CComBSTR(GetString(attrname)),
                        (BSTR)nullAttrString,
                        (BSTR)CComBSTR(buffer));
	VERIFY(hr==S_OK);
    attrcount++;
}


void CXMLDocumentWriter::AddAttribute(const StringType &attrname, long lValue)
{
    wchar_t buffer[33];
    _ltow(lValue, buffer, 10);

    HRESULT hr = pMXAttrs->addAttribute(
                        (BSTR)nullAttrString, 
                        (BSTR)nullAttrString,
                        (BSTR)CComBSTR(GetString(attrname)),
                        (BSTR)nullAttrString,
                        (BSTR)CComBSTR(buffer));
	VERIFY(hr==S_OK);
    attrcount++;
}


void CXMLDocumentWriter::AddAttribute(const StringType &attrname, int iValue)
{
    wchar_t buffer[20];
    _itow(iValue, buffer, 10);

    HRESULT hr = pMXAttrs->addAttribute(
                        (BSTR)nullAttrString, 
                        (BSTR)nullAttrString,
                        (BSTR)CComBSTR(GetString(attrname)),
                        (BSTR)nullAttrString,
                        (BSTR)CComBSTR(buffer));
	VERIFY(hr==S_OK);
    attrcount++;
}


void CXMLDocumentWriter::AddAttribute(const StringType &attrname, unsigned long ulValue)
{
    wchar_t buffer[38];
    _ultow(ulValue, buffer, 10);

    HRESULT hr = pMXAttrs->addAttribute(
                        (BSTR)nullAttrString, 
                        (BSTR)nullAttrString,
                        (BSTR)CComBSTR(GetString(attrname)),
                        (BSTR)nullAttrString,
                        (BSTR)CComBSTR(buffer));
	VERIFY(hr==S_OK);
    attrcount++;
}


void CXMLDocumentWriter::AddAttribute(const StringType &attrname, bool bValue)
{
    HRESULT hr = pMXAttrs->addAttribute(
                        (BSTR)nullAttrString, 
                        (BSTR)nullAttrString,
                        (BSTR)CComBSTR(GetString(attrname)),
                        (BSTR)nullAttrString,
                        (BSTR)CComBSTR((bValue ? _T("true") : _T("false"))));
	VERIFY(hr==S_OK);
    attrcount++;
}


void CXMLDocumentWriter::AddAttribute(const StringType &attrname, float fValue)
{
    char buffer[50];
    _gcvt( (double)fValue, 7, buffer );

#ifdef _UNICODE
    CA2W szBuffer(buffer);
#define ATTR_BUFFER szBuffer
#else
#define ATTR_BUFFER buffer
#endif
    HRESULT hr = pMXAttrs->addAttribute(
                        (BSTR)nullAttrString, 
                        (BSTR)nullAttrString,
                        (BSTR)CComBSTR(GetString(attrname)),
                        (BSTR)nullAttrString,
                        (BSTR)CComBSTR(ATTR_BUFFER));
	VERIFY(hr==S_OK);
    attrcount++;
}


void CXMLDocumentWriter::Characters(const StringType &value)
{
//  IMODTRACE_DEVELOP();

    CComQIPtr<ISAXContentHandler> pContentHandler(pWriter);

    pContentHandler->characters(GetString(value), (int)value.size());
}


void CXMLDocumentWriter::AddElement(const StringType &tagname, const StringType &tagvalue)
{
    StartElement(tagname);
    Characters(tagvalue);
    EndElement(tagname);
}


void CXMLDocumentWriter::AddElement(const StringType &tagname, bool bValue)
{
    StartElement(tagname);
    Characters(bValue ? _T("1") : _T("0"));
    EndElement(tagname);
}


void CXMLDocumentWriter::AddElement(const StringType &tagname, int iValue)
{
    _TCHAR buffer[20];
    _itot(iValue, buffer, 10);

    StartElement(tagname);
    Characters(buffer);
    EndElement(tagname);
}

void CXMLDocumentWriter::AddElement(const StringType &tagname, long lValue)
{
    _TCHAR buffer[33];
    _ltot(lValue, buffer, 10);

    StartElement(tagname);
    Characters(buffer);
    EndElement(tagname);
}


void CXMLDocumentWriter::AddElement(const StringType &tagname, unsigned long ulValue)
{
    _TCHAR buffer[38];
    _ultot(ulValue, buffer, 10);

    StartElement(tagname);
    Characters(buffer);
    EndElement(tagname);
}


void CXMLDocumentWriter::AddElement(const StringType &tagname, short sValue)
{
    _TCHAR buffer[20];
    _itot((int)sValue, buffer, 10);

    StartElement(tagname);
    Characters(buffer);
    EndElement(tagname);
}


void CXMLDocumentWriter::AddElement(const StringType &tagname, const _TCHAR *szValue)
{
    StartElement(tagname);
    Characters(StringType(szValue));
    EndElement(tagname);
}


void CXMLDocumentWriter::AddElement(const StringType &tagname, float fValue)
{
    char buffer[50];
    _gcvt( (double)fValue, 7, buffer );

#ifdef _UNICODE
    CA2W szBuffer(buffer);
#define BUFFER szBuffer
#else
#define BUFFER buffer
#endif

    StartElement(tagname);
    Characters((_TCHAR *)BUFFER);
    EndElement(tagname);
}
