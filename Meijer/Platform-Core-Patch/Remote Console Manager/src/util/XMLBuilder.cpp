#include "StdAfx.h"
#include "XMLBuilder.h"
#include "verify.h"

//lint --e{534}
//lint --e{666}

CXMLBuilder::CXMLBuilder(void)
    : attrcount(0)
    , nullAttrString(L"")
{
    EVAL_HR(
        CoCreateInstance(
            __uuidof(MXXMLWriter), NULL, CLSCTX_ALL,
            __uuidof(IMXWriter), (void **)&pWriter));
  
    VARIANT_BOOL bOmitXMLDeclaration = VARIANT_TRUE;

    HRESULT hr = pWriter->put_omitXMLDeclaration(bOmitXMLDeclaration);
    VERIFY (S_OK == hr);

    EVAL_HR(
        CoCreateInstance(
            __uuidof(SAXAttributes), NULL, CLSCTX_ALL,
            __uuidof(IMXAttributes), (void **)&pMXAttrs));
}


//lint --e{534}
CXMLBuilder::~CXMLBuilder(void)
{
    try
    {
        pWriter->Release();
        pMXAttrs->Release();

        pWriter = NULL;
        pMXAttrs = NULL;
    }
    catch (...)
        { ITRACE_ERROR("Exception caught"); }
}


wchar_t *CXMLBuilder::GetString(const std::string &theString) const
{
    size_t nLength = theString.size();

    static size_t nBufferLength = 0;
    static wchar_t *pBuffer = NULL;

    if (nLength > nBufferLength)
    {
        delete [] pBuffer;

        nBufferLength = nLength+1;
        pBuffer = new wchar_t[nBufferLength];
    }

    mbstowcs(pBuffer, theString.c_str(), nBufferLength);

    return pBuffer;
}


wchar_t *CXMLBuilder::GetString(const std::wstring &theString) const
    { return const_cast<wchar_t *>(theString.c_str()); }


StringType CXMLBuilder::GetXMLOutput()
{
    StringType retValue = _T("");
    CComVariant cvOutput;
    HRESULT hr = pWriter->get_output(&cvOutput);
    if (hr == S_OK)
    {
        BSTR xmlOutput = cvOutput.bstrVal;

        CW2T sString(xmlOutput);

        ::SysFreeString(xmlOutput);

        retValue = sString;
    }

    return retValue;
}


void CXMLBuilder::AddAdornments() {}
            

void CXMLBuilder::GetXMLOutput(CXMLContent *pContent)
{
    StartDocument();
 
    AddAdornments();

    GetXMLOutput(pContent, this);

    EndDocument();
}


void CXMLBuilder::StartDocument()
{
//  IMODTRACE_DEVELOP();

    VERIFY_AND_LOG(pWriter->put_output(CComVariant(L"")));

    CComQIPtr<ISAXContentHandler> pContentHandler(pWriter);

    pContentHandler->startDocument();
}

void CXMLBuilder::EndDocument()
{
    CComQIPtr<ISAXContentHandler> pContentHandler(pWriter);

    HRESULT hr;

    hr = pContentHandler->endDocument();

    CComVariant cvOutput(L"");
    hr = pWriter->get_output(&cvOutput);
    VERIFY(hr==S_OK);

    _bstr_t bstrValue = cvOutput.bstrVal;
    m_szXMLOutput = (const wchar_t *)bstrValue;
}

static wchar_t nullstring[] = L"";
static int nulllength = (int)lstrlenW(nullstring);
void CXMLBuilder::StartElement(const StringType &tagname)
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


void CXMLBuilder::EndElement(const StringType &tagname)
{
//  IMODTRACE_DEVELOP();

//  ITRACE_DEVELOP(IString("tagname=") + tagname.c_str());

    CComQIPtr<ISAXContentHandler> pContentHandler(pWriter);

    pContentHandler->endElement(nullstring, nulllength,
                        nullstring, nulllength,
                        GetString(tagname), (int)tagname.size());
}


void CXMLBuilder::ClearAttributes()
{
//  IMODTRACE_DEVELOP();

    pMXAttrs->clear();
    attrcount = 0;
}


void CXMLBuilder::AddAttribute(const StringType &attrname, StringType &attrvalue)
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


void CXMLBuilder::AddAttribute(const StringType &attrname, CStringProperty &attrvalue)
{
    if (attrvalue.IsValid())
    {
        HRESULT hr = pMXAttrs->addAttribute(
                            (BSTR)nullAttrString, 
                            (BSTR)nullAttrString,
                            (BSTR)CComBSTR(GetString(attrname)),
                            (BSTR)nullAttrString,
                            (BSTR)CComBSTR(GetString(attrvalue.Value())));
        VERIFY(hr==S_OK);
        attrcount++;
    }
}


void CXMLBuilder::AddAttribute(const StringType &attrname, CShortProperty &sValue)
{
    if (sValue.IsValid())
    {
        wchar_t buffer[20];
        _itow((int)sValue.Value(), buffer, 10);

        HRESULT hr = pMXAttrs->addAttribute(
                            (BSTR)nullAttrString, 
                            (BSTR)nullAttrString,
                            (BSTR)CComBSTR(GetString(attrname)),
                            (BSTR)nullAttrString,
                            (BSTR)CComBSTR(buffer));
        VERIFY(hr==S_OK);
        attrcount++;
    }
}


void CXMLBuilder::AddAttribute(const StringType &attrname, CLongProperty &lValue)
{
    if (lValue.IsValid())
    {
        wchar_t buffer[33];
        _ltow(lValue.Value(), buffer, 10);

        HRESULT hr = pMXAttrs->addAttribute(
                            (BSTR)nullAttrString, 
                            (BSTR)nullAttrString,
                            (BSTR)CComBSTR(GetString(attrname)),
                            (BSTR)nullAttrString,
                            (BSTR)CComBSTR(buffer));
        VERIFY(hr==S_OK);
        attrcount++;
    }
}


void CXMLBuilder::AddAttribute(const StringType &attrname, CIntProperty &iValue)
{
    if (iValue.IsValid())
    {
        wchar_t buffer[20];
        _itow(iValue.Value(), buffer, 10);

        HRESULT hr = pMXAttrs->addAttribute(
                            (BSTR)nullAttrString, 
                            (BSTR)nullAttrString,
                            (BSTR)CComBSTR(GetString(attrname)),
                            (BSTR)nullAttrString,
                            (BSTR)CComBSTR(buffer));
        VERIFY(hr==S_OK);
        attrcount++;
    }
}


void CXMLBuilder::AddAttribute(const StringType &attrname, CUnsignedLongProperty &ulValue)
{
    if (ulValue.IsValid())
    {
        wchar_t buffer[38];
        _ultow(ulValue.Value(), buffer, 10);

        HRESULT hr = pMXAttrs->addAttribute(
                            (BSTR)nullAttrString, 
                            (BSTR)nullAttrString,
                            (BSTR)CComBSTR(GetString(attrname)),
                            (BSTR)nullAttrString,
                            (BSTR)CComBSTR(buffer));
        VERIFY(hr==S_OK);
        attrcount++;
    }
}


void CXMLBuilder::AddAttribute(const StringType &attrname, const IEnum &eValue, const IValidatable &vValue)
{
    if (vValue.IsValid())
    {
        HRESULT hr = pMXAttrs->addAttribute(
                            (BSTR)nullAttrString, 
                            (BSTR)nullAttrString,
                            (BSTR)CComBSTR(GetString(attrname)),
                            (BSTR)nullAttrString,
                            (BSTR)CComBSTR(GetString(eValue.ToString())));
        VERIFY(hr==S_OK);
        attrcount++;
    }
}


void CXMLBuilder::Characters(const StringType &value)
{
//  IMODTRACE_DEVELOP();

    CComQIPtr<ISAXContentHandler> pContentHandler(pWriter);

    pContentHandler->characters(GetString(value), (int)value.size());
}


void CXMLBuilder::AddElement(const StringType &tagname, StringType &tagvalue)
{
    StartElement(tagname);
    Characters(tagvalue);
    EndElement(tagname);
}


void CXMLBuilder::AddElement(const StringType &tagname, CBoolProperty &bValue)
{
    if (bValue.IsValid())
    {
        StartElement(tagname);
        Characters(bValue.Value() ? _T("1") : _T("0"));
        EndElement(tagname);
    }
}


void CXMLBuilder::AddElement(const StringType &tagname, CIntProperty &iValue)
{
    if (iValue.IsValid())
    {
        _TCHAR buffer[20];
        _itot(iValue.Value(), buffer, 10);

        StartElement(tagname);
        Characters(buffer);
        EndElement(tagname);
    }
}

void CXMLBuilder::AddElement(const StringType &tagname, CLongProperty &lValue)
{
    if (lValue.IsValid())
    {
        _TCHAR buffer[33];
        _ltot(lValue.Value(), buffer, 10);

        StartElement(tagname);
        Characters(buffer);
        EndElement(tagname);
    }
}


void CXMLBuilder::AddElement(const StringType &tagname, CUnsignedLongProperty &ulValue)
{
    if (ulValue.IsValid())
    {
        _TCHAR buffer[38];
        _ultot(ulValue.Value(), buffer, 10);

        StartElement(tagname);
        Characters(buffer);
        EndElement(tagname);
    }
}


void CXMLBuilder::AddElement(const StringType &tagname, CShortProperty &sValue)
{
    if (sValue.IsValid())
    {
        _TCHAR buffer[20];
        _itot((int)sValue.Value(), buffer, 10);

        StartElement(tagname);
        Characters(buffer);
        EndElement(tagname);
    }
}


void CXMLBuilder::AddElement(const StringType &tagname, CStringProperty &szValue)
{
    if (szValue.IsValid())
    {
        StartElement(tagname);
        Characters(szValue.Value());
        EndElement(tagname);
    }
}


void CXMLBuilder::AddElement(const StringType &tagname, CFloatProperty &fValue)
{
    if (fValue.IsValid())
    {
        char buffer[50];
        _gcvt( (double)fValue.Value(), 7, buffer );

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
}


//void CXMLBuilder::AddElement(const StringType &tagname, const IEnum &eValue)
void CXMLBuilder::AddElement(const StringType &tagname, const IEnum &eValue, const IValidatable &vValue)
{
    if (vValue.IsValid())
    {
        StartElement(tagname);
        Characters(eValue.ToString());
        EndElement(tagname);
    }
}
