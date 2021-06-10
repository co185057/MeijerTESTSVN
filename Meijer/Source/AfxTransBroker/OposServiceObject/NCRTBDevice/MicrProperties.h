// MicrProperties.h: interface for the CMicrProperties class.
//
// Based on: /OPOS/Rel.2_3/Services/NCR71xx/MICR.h  revision 35  6/29/01 1:37p
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MICRPROPERTIES_H__53F6BE46_BF7E_4A90_AAAB_0200B6230BA8__INCLUDED_)
#define AFX_MICRPROPERTIES_H__53F6BE46_BF7E_4A90_AAAB_0200B6230BA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// Types used by ParseSubString.

const char TypeDigit          = 0x01;
const char TypeDigitDash      = 0x02;
const char TypeDigitDashSpace = 0x04;
const char TypeDashSpace      = 0x10;

class CMicrProperties
{
public:
    CMicrProperties();
    virtual ~CMicrProperties();
    void Initialize(CmDataCapture* cmDc, CString sRemoveNonDigits);
    CString GetPropertyString(long lPropIndex);
    long GetPropertyNumber(long lPropIndex);
    void ParseMicrData(const CString& sRawData);

private:
    CString  m_AccountNumber;
    CString  m_Amount;
    CString  m_BankNumber;
    CString  m_EPC;
    CString  m_RawData;
    CString  m_SerialNumber;
    CString  m_TransitNumber;
    long     m_CheckType;
    long     m_CountryCode;

    CmDataCapture *m_cmDc;

    // String representations of integer properties.
    CString     m_sCheckType;           // String representation of m_CheckType.
    CString     m_sCountryCode;         // String representation of m_CountryCode.

    // Internal methods
    BOOL ParseToString(             // Try to parse raw data to the format of a string.
        LPCTSTR& pExcep );              // In > Exception string.
    BOOL ParseRepeats(              // Return TRUE if valid data; FALSE if bad format.
                                        //  If TRUE, then m_nRMin and m_nRMax are set.
        LPCTSTR& pExcep );              // In/Out> Pointer to exception string.
    BOOL ParseSubString(            // Return TRUE if valid data; FALSE if bad format.
        LPCTSTR& pRaw,                  // In > Pointer to Micr data.
        char cType,                     // In > Type of characters permitted.
        CString& sString );             // Out> Parsed substring.
    void RemoveNonDigits(           // Remove any non-digit characters.
        CString& sString );             // In/Out> String.

    // Internal Data
    int         m_nRMin;                // Minimum repeat count.
    int         m_nRMax;                // Maximum repeat count.
    CString     m_sRemoveNonDigits;     // String from registry.

    CStringList m_ExcepList;            // List of Micr Parsing exceptions.
};

#endif // !defined(AFX_MICRPROPERTIES_H__53F6BE46_BF7E_4A90_AAAB_0200B6230BA8__INCLUDED_)
