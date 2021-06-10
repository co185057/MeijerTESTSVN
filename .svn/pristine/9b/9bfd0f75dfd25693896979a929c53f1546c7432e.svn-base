// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/util/PSXError.cpp 3     10/21/08 4:08p Sm133076 $
/****************************************************************************/
/*                                                                          */
/* Module Name:               PSXError                                         */
/*                                                                          */
/* Module Title:              Exception Object                              */
/*                                                                          */
/****************************************************************************/
// $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/util/PSXError.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 6     8/09/05 12:41p Dm180015
 * 
 * 6     8/09/05 12:08p Dm180015
 * 
 * 5     8/09/05 11:31a Dm180015
 * 
 * 3     3/24/05 7:30p Dm185016
 * UNICODE
 * 
 * 2     2/28/05 3:33p Dm185016
 * lint
//* 
//* 1     1/26/05 2:32p Dm185016
//* Moved to new Repository.
// 
// 3     1/18/05 4:01p Dm185016
// UNICODE support
// 
// 2     11/02/04 3:22p Dm185016
// More updates
// 
// 1     10/14/04 1:52p Dm185016
//lint --e{534}

/****************************************************************************/
/* Include Files                                                            */
/****************************************************************************/

#include "StdAfx.h"
#include "PSXError.h"

CPSXErrorInfo::CPSXErrorInfo (  )
    : m_nErrorRC(S_OK)
{}


CPSXErrorInfo::CPSXErrorInfo ( PSXRC rc 
                             , const _TCHAR *pszText)
    : m_nErrorRC((PSX_ERROR_BASE | (HRESULT)rc))
{
    Init( rc, lookupError(rc), pszText );
}


CPSXErrorInfo::CPSXErrorInfo ( const CPSXErrorInfo& errorInformation )
    : utils::IErrorInfo(errorInformation)
{
    m_nErrorRC      = errorInformation.m_nErrorRC;
    m_szErrorText   = errorInformation.m_szErrorText;
}


void CPSXErrorInfo::Init ( PSXRC rc 
                         , const _TCHAR *pszErrorText
                         , const _TCHAR *pszAPICall)
{
    m_szErrorText.clear();
    m_szErrorText   += _T("API:  ");
	m_szErrorText   += pszAPICall;
    m_szErrorText   += _T(".  Error=");
	m_szErrorText   += pszErrorText;
    m_szErrorText   += _T(".  rc=");
    m_szErrorText   += IString(rc).d2x();
}


const _TCHAR *CPSXErrorInfo::text ( ) const 
{ 
    return (const _TCHAR *)m_szErrorText; 
}


HRESULT CPSXErrorInfo::errorId ( ) const
{ 
    return m_nErrorRC; 
}


utils::IErrorInfo* CPSXErrorInfo::copy( ) const 
{ 
    return new CPSXErrorInfo(*this); 
}


const _TCHAR *CPSXErrorInfo::lookupError( PSXRC rc ) const
{
    return RCToString( rc );
}


CPSXErrorInfo::~CPSXErrorInfo ( ) { }


IEXCLASSIMPLEMENT(CPSXError,IException);
