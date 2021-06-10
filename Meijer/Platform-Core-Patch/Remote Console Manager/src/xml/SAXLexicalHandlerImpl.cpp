// ******************************************************************
//
//	testSax: example of MSXML SAX2/COM use and base classes for handlers
//  (C) Microsoft Corp., 2000
//
// ******************************************************************
//
// SAXLexicalHandlerImpl.cpp: implementation of the SAXLexicalHandlerImpl class.
//
//////////////////////////////////////////////////////////////////////

//lint --e{715}     Do not care about unreferenced parameters
//lint -esym(534, SAXLexicalHandlerImpl::AddRef)

#include "stdafx.h"
#include "SAXLexicalHandlerImpl.h"
#include "XMLUtilities.h"

// Do not care about unreferenced formal parameters!
#pragma warning( disable : 4100 )

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SAXLexicalHandlerImpl::SAXLexicalHandlerImpl()
    : m_ulRefs(0)
{

}


HRESULT STDMETHODCALLTYPE SAXLexicalHandlerImpl::startDTD (
    /*[in]*/ unsigned short * pwchName,
    /*[in]*/ int cchName,
    /*[in]*/ unsigned short * pwchPublicId,
    /*[in]*/ int cchPublicId,
    /*[in]*/ unsigned short * pwchSystemId,
    /*[in]*/ int cchSystemId )
{
    IMODTRACE_DEVELOP();

	return S_OK;
}


HRESULT STDMETHODCALLTYPE SAXLexicalHandlerImpl::endDTD ( )
{
    IMODTRACE_DEVELOP();

	return S_OK;
}


HRESULT STDMETHODCALLTYPE SAXLexicalHandlerImpl::startEntity (
    /*[in]*/ unsigned short * pwchName,
    /*[in]*/ int cchName )
{
	return S_OK;
}


HRESULT STDMETHODCALLTYPE SAXLexicalHandlerImpl::endEntity (
    /*[in]*/ unsigned short * pwchName,
    /*[in]*/ int cchName )
{
	return S_OK;
}


HRESULT STDMETHODCALLTYPE SAXLexicalHandlerImpl::startCDATA ( )
{
    IMODTRACE_DEVELOP();

	return S_OK;
}


HRESULT STDMETHODCALLTYPE SAXLexicalHandlerImpl::endCDATA ( )
{
    IMODTRACE_DEVELOP();

	return S_OK;
}


HRESULT STDMETHODCALLTYPE SAXLexicalHandlerImpl::comment (
    /*[in]*/ unsigned short * pwchChars,
    /*[in]*/ int cchChars )
{
    StringType szElement = XMLUtilities::getString(pwchChars, cchChars);

    ITRACE_DEVELOP(_T("Comment:  \"") + IString(szElement.c_str()) + _T("\"."));

	return S_OK;
}


long __stdcall SAXLexicalHandlerImpl::QueryInterface(REFIID riid,void **ppv )
{
	if ((riid == IID_IUnknown)
        || (riid == __uuidof(ISAXLexicalHandler)))
    {
        *ppv = this;
        AddRef();
        return S_OK;
    }

	return E_NOINTERFACE;
}

unsigned long __stdcall SAXLexicalHandlerImpl::AddRef()
{
	return ++m_ulRefs;
}

unsigned long __stdcall SAXLexicalHandlerImpl::Release()
{
	if (--m_ulRefs == 0)
    {
    }
	return m_ulRefs;
}