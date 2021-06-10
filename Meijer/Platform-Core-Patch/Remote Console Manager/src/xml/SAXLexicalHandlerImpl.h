// ******************************************************************
//
//	testSax: example of MSXML SAX2/COM use and base classes for handlers
//  (C) Microsoft Corp., 2000
//
// ******************************************************************
//
// SAXLexicalHandlerImpl.h: interface for the SAXLexicalHandlerImpl class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "utils.h"

class UTILS_API SAXLexicalHandlerImpl 
    : public MSXML2::ISAXLexicalHandler  
    , public CBaseObject
{
public:
	SAXLexicalHandlerImpl();

	// This must be correctly implemented, if your handler must be a COM Object (in this example it does not)
	long __stdcall QueryInterface(const struct _GUID &,void ** );
	unsigned long __stdcall AddRef(void);
	unsigned long __stdcall Release(void);

    virtual HRESULT STDMETHODCALLTYPE startDTD (
    /*[in]*/ unsigned short * pwchName,
    /*[in]*/ int cchName,
    /*[in]*/ unsigned short * pwchPublicId,
    /*[in]*/ int cchPublicId,
    /*[in]*/ unsigned short * pwchSystemId,
    /*[in]*/ int cchSystemId );
    virtual HRESULT STDMETHODCALLTYPE endDTD ( );
    virtual HRESULT STDMETHODCALLTYPE startEntity (
    /*[in]*/ unsigned short * pwchName,
    /*[in]*/ int cchName );
    virtual HRESULT STDMETHODCALLTYPE endEntity (
    /*[in]*/ unsigned short * pwchName,
    /*[in]*/ int cchName );
    virtual HRESULT STDMETHODCALLTYPE startCDATA ( );
    virtual HRESULT STDMETHODCALLTYPE endCDATA ( );
    virtual HRESULT STDMETHODCALLTYPE comment (
    /*[in]*/ unsigned short * pwchChars,
    /*[in]*/ int cchChars );

private:

    SAXLexicalHandlerImpl(const SAXLexicalHandlerImpl &);               // Hide the Copy construcor
    SAXLexicalHandlerImpl &operator =(const SAXLexicalHandlerImpl &);   // Hide the assignment operator

    ULONG m_ulRefs;
};
