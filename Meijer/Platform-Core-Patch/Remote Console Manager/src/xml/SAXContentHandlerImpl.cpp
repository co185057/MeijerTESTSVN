// SAXContentHandlerImpl.cpp: implementation of the SAXContentHandlerImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SAXContentHandlerImpl.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


SAXContentHandlerImpl::SAXContentHandlerImpl()
    : m_pOwner(NULL)
{
}


HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::putDocumentLocator( 
            /* [in] */ ISAXLocator __RPC_FAR * /* pLocator */
            )
{
    return S_OK;
}
        
HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::startDocument()
{
    return S_OK;
}
        

        
HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::endDocument( void)
{
    return S_OK;
}
        
        
HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::startPrefixMapping( 
            /* [in] */ wchar_t __RPC_FAR * /* pwchPrefix */,
            /* [in] */ int  /* cchPrefix */,
            /* [in] */ wchar_t __RPC_FAR * /* pwchUri */,
            /* [in] */ int  /* cchUri */)
{
    return S_OK;
}
        
        
HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::endPrefixMapping( 
            /* [in] */ wchar_t __RPC_FAR * /* pwchPrefix */,
            /* [in] */ int  /* cchPrefix */)
{
    return S_OK;
}
        

        
HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::startElement( 
            /* [in] */ wchar_t __RPC_FAR * /* pwchNamespaceUri */,
            /* [in] */ int  /* cchNamespaceUri */,
            /* [in] */ wchar_t __RPC_FAR * /* pwchLocalName */,
            /* [in] */ int  /* cchLocalName */,
            /* [in] */ wchar_t __RPC_FAR * /* pwchRawName */,
            /* [in] */ int  /* cchRawName */,
            /* [in] */ ISAXAttributes __RPC_FAR * /* pAttributes */)
{
    return S_OK;
}
        
       
HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::endElement( 
            /* [in] */ wchar_t __RPC_FAR * /* pwchNamespaceUri */,
            /* [in] */ int  /* cchNamespaceUri */,
            /* [in] */ wchar_t __RPC_FAR * /* pwchLocalName */,
            /* [in] */ int  /* cchLocalName */,
            /* [in] */ wchar_t __RPC_FAR * /* pwchRawName */,
            /* [in] */ int  /* cchRawName */)
{
    return S_OK;
}
        
HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::characters( 
            /* [in] */ wchar_t __RPC_FAR * /* pwchChars */,
            /* [in] */ int  /* cchChars */)
{
    return S_OK;
}
        

HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::ignorableWhitespace( 
            /* [in] */ wchar_t __RPC_FAR * /* pwchChars */,
            /* [in] */ int  /* cchChars */)
{
    return S_OK;
}
        

HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::processingInstruction( 
            /* [in] */ wchar_t __RPC_FAR * /* pwchTarget */,
            /* [in] */ int  /* cchTarget */,
            /* [in] */ wchar_t __RPC_FAR * /* pwchData */,
            /* [in] */ int  /* cchData */)
{
    return S_OK;
}
        
        
HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::skippedEntity( 
            /* [in] */ wchar_t __RPC_FAR * /* pwchVal */,
            /* [in] */ int  /* cchVal */)
{
    return S_OK;
}


long __stdcall SAXContentHandlerImpl::QueryInterface(const struct _GUID &, void **)
{
    // hack-hack-hack!
    return 0;
}

unsigned long __stdcall SAXContentHandlerImpl::AddRef()
{
    // hack-hack-hack!
    return 0;
}

unsigned long __stdcall SAXContentHandlerImpl::Release()
{
    // hack-hack-hack!
    return 0;
}


std::wstring SAXContentHandlerImpl::getString(
            /* [in] */ const wchar_t __RPC_FAR *pwchVal,
            /* [in] */ int cchVal) const
{
    static wchar_t val[1000];
    cchVal = cchVal>999 ? 999 : cchVal;
    wcsncpy( val, pwchVal, (unsigned)cchVal ); 
    val[cchVal] = 0;
    return std::wstring(val);
}


void SAXContentHandlerImpl::SetOwner(IContentHandlerOwner *pOwner)
{
    m_pOwner = pOwner;
}


IContentHandlerOwner * SAXContentHandlerImpl::GetOwner()
{
    return m_pOwner;
}

