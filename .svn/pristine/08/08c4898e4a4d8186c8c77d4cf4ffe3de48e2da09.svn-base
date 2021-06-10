#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/XMLError.h 1     6/20/08 1:14p Sa250050 $
/*
 * @(#)XMLError.h      $Revision: 1 $  $Date: 6/20/08 1:14p $
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

// Provides the definitions for all HRESULT values returned by the Security component
// both internally and externally.
#define XML_ERROR_BASE                  0xC0041000L

#define XML_UNDEFINED_TAG               _HRESULT_TYPEDEF_(XML_ERROR_BASE)
//#define XML_INVALID_ACTION_ID          _HRESULT_TYPEDEF_(XML_ERROR_BASE+1)

#include "utils.h"
#include "xerror.h"

#pragma warning( push )
#pragma warning( disable : 4251 )


class UTILS_API CXMLErrorInfo : public utils::IErrorInfo
{
    public:
        /*------------------------------- Constructors -------------------------------*/
          CXMLErrorInfo ( HRESULT, int nLine=-1, int nCol=-1, const _TCHAR *pszFile=NULL, const _TCHAR *pszType=NULL );
          CXMLErrorInfo ( );

        virtual
         ~CXMLErrorInfo ( );

        /*---------------------------- Error Information -----------------------------*/
        inline virtual
            operator const _TCHAR* ( ) const { return GetErrorCombinedText(); }

        inline virtual const _TCHAR
            *text ( ) const { return GetErrorCombinedText(); }

        virtual HRESULT
            errorId ( ) const{ return m_nErrorHRESULT; }

        virtual utils::IErrorInfo* 
            copy( ) const { return new CXMLErrorInfo(*this); }

        inline virtual HRESULT
            GetErrorID ( ) const{ return m_nErrorHRESULT; }

        inline virtual long
            GetErrorLine ( ) const{ return m_nErrorLine; }

        inline virtual long
            GetErrorColumn ( ) const{ return m_nErrorColumn; }

        inline virtual const _TCHAR *
			GetErrorFile ( ) const{ return m_szErrorFile.c_str(); }

        inline virtual const _TCHAR *
            GetErrorType ( ) const{ return m_szErrorType.c_str(); }

        inline const _TCHAR *
            GetErrorText ( ) const{ return m_szErrorText.c_str(); }

    protected:

        void Init ( const _TCHAR *
                  , HRESULT
				  ,	int nLine=-1
                  , int nCol=-1
                  , const _TCHAR *pszFile=NULL
                  , const _TCHAR *pszType=NULL );

        const IString &
            GetErrorCombinedText ( ) const;

        const _TCHAR *lookupError(HRESULT hr);

    private:
        CXMLErrorInfo ( const CXMLErrorInfo& );
        CXMLErrorInfo
         &operator= ( const CXMLErrorInfo& );

        mutable IString m_szErrorCombinedText;
        StringType m_szErrorText;

		HRESULT m_nErrorHRESULT;
		long m_nErrorLine;
		long m_nErrorColumn;
		StringType m_szErrorFile;
		StringType m_szErrorType;
        StringType m_sLookupText;
}; // CXMLErrorInfo

#pragma warning( pop )
