// ******************************************************************
//
//	testSax: example of MSXML SAX2/COM use and base classes for handlers
//  (C) Microsoft Corp., 2000
//
// ******************************************************************
//
// SAXErrorHandler.h: interface for the SAXErrorHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAXERRORHANDLER_H__736F82C7_A4FF_4B7F_B862_A77B333A299D__INCLUDED_)
#define AFX_SAXERRORHANDLER_H__736F82C7_A4FF_4B7F_B862_A77B333A299D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "utils.h"

#define WARNING_PREFIX      _T("Warning:  ")
#define FATAL_ERROR_PREFIX _T("Fatal error:  ")
#define ERROR_PREFIX        _T("Error:  ")

class CXMLErrorInfo;

//lint --e{1510}
class UTILS_API SAXErrorHandlerImpl 
    : public MSXML2::ISAXErrorHandler  
    , virtual public CBaseObject
{
public:
	SAXErrorHandlerImpl();
	virtual ~SAXErrorHandlerImpl();

		// This must be correctly implemented, if your handler must be a COM Object (in this example it does not)
		long __stdcall QueryInterface(const struct _GUID &,void ** );
		unsigned long __stdcall AddRef(void);
		unsigned long __stdcall Release(void);

        virtual HRESULT STDMETHODCALLTYPE error( 
            /* [in] */ ISAXLocator __RPC_FAR *pLocator,
            /* [in] */ unsigned short * pwchErrorMessage,
			/* [in] */ HRESULT errCode);
        
        virtual HRESULT STDMETHODCALLTYPE fatalError( 
            /* [in] */ ISAXLocator __RPC_FAR *pLocator,
            /* [in] */ unsigned short * pwchErrorMessage,
			/* [in] */ HRESULT errCode);
        
        virtual HRESULT STDMETHODCALLTYPE ignorableWarning( 
            /* [in] */ ISAXLocator __RPC_FAR *pLocator,
            /* [in] */ unsigned short * pwchErrorMessage,
			/* [in] */ HRESULT errCode);

		virtual const CXMLErrorInfo *GetErrorInfo() { return m_pErrorInfo; }

protected:

    virtual void handleError(const _TCHAR *szErrorType,
            /* [in] */ ISAXLocator __RPC_FAR *pLocator,
            /* [in] */ unsigned short * pwchErrorMessage,
			/* [in] */ HRESULT errCode);

    virtual CXMLErrorInfo *GetErrorInfo ( HRESULT hr, 
							            int nLine, 
							            int nCol, 
							            const _TCHAR *pszFile, 
							            const _TCHAR *pszType );

private:

    SAXErrorHandlerImpl(const SAXErrorHandlerImpl &);               // Hide the Copy construcor
    SAXErrorHandlerImpl &operator =(const SAXErrorHandlerImpl &);   // Hide the assignment operator

	CXMLErrorInfo *m_pErrorInfo;
};

#endif // !defined(AFX_SAXERRORHANDLER_H__736F82C7_A4FF_4B7F_B862_A77B333A299D__INCLUDED_)
