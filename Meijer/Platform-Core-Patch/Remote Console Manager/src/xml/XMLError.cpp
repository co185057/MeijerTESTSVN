// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/XMLError.cpp 1     6/20/08 1:14p Sa250050 $
/****************************************************************************/
/*                                                                          */
/* Module Name:               XMLError                                         */
/*                                                                          */
/* Module Title:              Exception Object                              */
/*                                                                          */
/****************************************************************************/
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/XMLError.cpp $
 * 
 * 1     6/20/08 1:14p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 6     4/12/05 2:48p Dm185016
 * 
 * 5     4/12/05 11:46a Dm185016
 * Fixed missing error code for include file not found
 * 
 * 4     2/28/05 3:24p Dm185016
 * lint
* 
* 3     2/17/05 5:58p Dm185016
* Fixed problem displaying XML errors.
* 
* 2     2/04/05 10:41a Dm185016
* 
* 1     1/26/05 2:34p Dm185016
* Moved to new Repository.
* 
* 9     1/18/05 4:08p Dm185016
* UNICODE support
* 
* 8     10/05/04 8:34a Dm185016
* Made all xxxErrorInfo objects derive from same base class CErrorInfo.
* 
* 6     6/30/04 3:54p Dm185016
* Lint.
* 
* 5     5/18/04 7:16p Dm185016
* Missing file error
* 
* 4     4/20/04 4:46p Dm185016
* Made into a base class for the expanded error reporting mechanism.
* 
* 3     2/10/04 11:55a Dm185016
* Added error codes for problems in XML
* 
* 2     1/27/04 1:48p Dm185016
* Code cleanup.
* 
* 1     1/16/04 9:35a Dm185016
* New error reporting mechanism via CXMLErrorInfo object
*/

/****************************************************************************/
/* Include Files                                                            */
/****************************************************************************/

#include "StdAfx.h"
#include "XMLError.h"

static const _TCHAR *error0x800C0006 = _T("File not found");
static const _TCHAR *error0xC00CE503 = _T("Invalid comment format");
static const _TCHAR *error0xC00CE513 = _T("Invalid entity format");
static const _TCHAR *error0xC00CE56D = _T("End tag does not match any open tag element");
static const _TCHAR *error0xC00CEE23 = _T("Unterminated comment.  Missing \"-->\"");
static const _TCHAR *error0xC00CEE3B = _T("End tag encountered");
static const _TCHAR *error0xE06D7363 = _T("Unable to locate document");
static const _TCHAR *errorXML_UNDEFINED_TAG = _T("Undefined tag encountered");


CXMLErrorInfo::CXMLErrorInfo ( HRESULT hr, 
							  int nLine, 
							  int nCol, 
							  const _TCHAR *pszFile, 
							  const _TCHAR *pszType )
	: m_nErrorHRESULT(hr)
	, m_nErrorLine(nLine)
	, m_nErrorColumn(nCol)
{
    Init( lookupError(hr), hr, nLine, nCol, pszFile, pszType );
}


CXMLErrorInfo::CXMLErrorInfo ( const CXMLErrorInfo& errorInformation )
    : IErrorInfo(errorInformation)
{
    m_nErrorHRESULT     = errorInformation.m_nErrorHRESULT;
    m_nErrorLine        = errorInformation.m_nErrorLine;
    m_nErrorColumn      = errorInformation.m_nErrorColumn;
    m_szErrorFile       = errorInformation.m_szErrorFile;
    m_szErrorType       = errorInformation.m_szErrorType;
    m_szErrorText       = errorInformation.m_szErrorText;
}


void CXMLErrorInfo::Init ( const _TCHAR *pszErrorText, 
                           HRESULT hr, 
						   int nLine, 
						   int nCol, 
						   const _TCHAR *pszFile, 
						   const _TCHAR *pszType )
{
    m_nErrorLine    = nLine;
    m_nErrorColumn  = nCol;
    m_nErrorHRESULT = hr;
    m_szErrorFile   = (pszFile == NULL) ? _T("") : pszFile;
    m_szErrorType   = (pszType == NULL) ? _T("") : pszType;

	m_szErrorText   = (pszErrorText == NULL)
                            ? (LPCTSTR)Trace::formatLastError(_T(""), hr)
                            : pszErrorText;
}


const IString & CXMLErrorInfo::GetErrorCombinedText ( ) const
{
    if (m_szErrorCombinedText.empty())
    {
        m_szErrorCombinedText = m_szErrorType.c_str();
        if (!m_szErrorFile.empty())
        {
	        m_szErrorCombinedText += _T("  Document:  ");
	        m_szErrorCombinedText += IString(m_szErrorFile.c_str());
        }
        if (m_nErrorLine != -1)
        {
	        m_szErrorCombinedText += _T("  Line:  ");
	        m_szErrorCombinedText += IString(m_nErrorLine);
        }
        if (m_nErrorColumn != -1)
        {
	        m_szErrorCombinedText += _T("  Column:  ");
	        m_szErrorCombinedText += IString(m_nErrorColumn);
        }
        m_szErrorCombinedText += _T("  HRESULT:  ");
	    m_szErrorCombinedText += IString(m_nErrorHRESULT).d2x();
	    m_szErrorCombinedText += _T("  Description:  ");

        m_szErrorCombinedText +=  m_szErrorText.c_str();
    }

    return m_szErrorCombinedText;
}


CXMLErrorInfo::CXMLErrorInfo ( ) 
	: m_nErrorHRESULT(E_FAIL)
	, m_nErrorLine(-1)
	, m_nErrorColumn(-1)
{ }


CXMLErrorInfo::~CXMLErrorInfo ( ) { }


//nolint --e{650}
const _TCHAR *CXMLErrorInfo::lookupError(HRESULT hr)
{
    const _TCHAR *retValue;

    switch((unsigned)hr)
    {
        case 0x800C0006 :           return error0x800C0006;
        case 0xc00ce503 :           return error0xC00CE503;
        case 0xc00ce513 :           return error0xC00CE513;
        case 0xc00ce56D :           return error0xC00CE56D;
        case 0xc00cee23 :           return error0xC00CEE23;
        case 0xc00cee3b :           return error0xC00CEE3B;
        case 0xe06d7363 :           return error0xE06D7363;
        case XML_UNDEFINED_TAG :    return errorXML_UNDEFINED_TAG;
        default :
        {
            IString error = Trace::formatLastError(_T(""), hr);
            
            if (error.length() != 0)
            {
                m_sLookupText = error;
                retValue = m_sLookupText.c_str();
            }
            else
                retValue = _T("Unknown return code.");
        }
    }

    return retValue;
}