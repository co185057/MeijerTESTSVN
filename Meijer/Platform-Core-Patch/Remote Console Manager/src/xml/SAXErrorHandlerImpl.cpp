// SAXErrorHandler.cpp: implementation of the SAXErrorHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SAXErrorHandlerImpl.h"
#include <stdio.h>
#include "XMLError.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SAXErrorHandlerImpl::SAXErrorHandlerImpl()
	: m_pErrorInfo(NULL)
{

}

SAXErrorHandlerImpl::~SAXErrorHandlerImpl()
{
	delete m_pErrorInfo;
}

HRESULT STDMETHODCALLTYPE SAXErrorHandlerImpl::error( 
            /* [in] */ ISAXLocator __RPC_FAR *pLocator,
            /* [in] */ unsigned short * pwchErrorMessage,
			/* [in] */ HRESULT errCode)
{
    handleError(ERROR_PREFIX, pLocator, pwchErrorMessage, errCode);

	return S_OK;
}
        
HRESULT STDMETHODCALLTYPE SAXErrorHandlerImpl::fatalError( 
            /* [in] */ ISAXLocator __RPC_FAR *pLocator,
            /* [in] */ unsigned short * pwchErrorMessage,
			/* [in] */ HRESULT errCode)
{
    handleError(FATAL_ERROR_PREFIX, pLocator, pwchErrorMessage, errCode);

	return S_OK;
}
        
HRESULT STDMETHODCALLTYPE SAXErrorHandlerImpl::ignorableWarning( 
            /* [in] */ ISAXLocator __RPC_FAR *pLocator,
            /* [in] */ unsigned short * pwchErrorMessage,
			/* [in] */ HRESULT errCode)
{
    handleError(WARNING_PREFIX, pLocator, pwchErrorMessage, errCode);

    return S_OK;
}

void SAXErrorHandlerImpl::handleError(
        /* [in] */ const _TCHAR *szErrorType,
        /* [in] */ ISAXLocator __RPC_FAR *pLocator,
        /* [in] */ unsigned short *pwchErrorMessage,
		/* [in] */ HRESULT errCode)
{
    wchar_t *pwFileName;
    int nColNumber;
    int nLineNumber;

    if (pLocator != NULL)
    {
        ISAXLocator &locator = *pLocator;

        locator.getSystemId(&pwFileName);
        locator.getColumnNumber(&nColNumber);
        locator.getLineNumber(&nLineNumber);

		CW2T pFileName( pwFileName );

        m_pErrorInfo = GetErrorInfo(errCode, nLineNumber, nColNumber, pFileName, szErrorType);

        CW2T errmsg( (wchar_t *)pwchErrorMessage );
        ITRACE_ERROR((const _TCHAR *)errmsg);
    }
}


CXMLErrorInfo *SAXErrorHandlerImpl::GetErrorInfo ( HRESULT hr, 
							        int nLine, 
							        int nCol, 
							        const _TCHAR *pszFile, 
							        const _TCHAR *pszType )
{
    return new CXMLErrorInfo(hr, nLine, nCol, pszFile, pszType);
}


long __stdcall SAXErrorHandlerImpl::QueryInterface(const struct _GUID &,void ** )
{
	// hack-hack-hack!
	return 0;
}

unsigned long __stdcall SAXErrorHandlerImpl::AddRef()
{
	// hack-hack-hack!
	return 0;
}

unsigned long __stdcall SAXErrorHandlerImpl::Release()
{
	// hack-hack-hack!
	return 0;
}