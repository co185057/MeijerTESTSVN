// MicrProperties.cpp: implementation of the CMicrProperties class.
//
// Based on: /OPOS/Rel.2_3/Services/NCR71xx/MICR.cpp  revision 6  9/10/02 6:36p
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MicrProperties.h"
#include "GlobalFunctions.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMicrProperties::CMicrProperties()
: m_CheckType(MICR_CT_UNKNOWN),
  m_CountryCode(MICR_CC_UNKNOWN),
  m_cmDc(NULL)
{
}

CMicrProperties::~CMicrProperties()
{
	// Nothing to do; trace object is destroyed by creator of this class
}

//
// Function:  Initialize
// Purpose:   Clear properties associated with the last MICR read.
// Inputs:    Trace object reference
// Outputs:   None
// Assumptions:  No distinctions are made by model type
// Comments:  None
//
void CMicrProperties::Initialize(CmDataCapture* cmDc, CString sRemoveNonDigits)
{
	m_cmDc = cmDc;   // Set trace object

    CDataCapEntryExit DCE( m_cmDc, _T("CMicrProperties::Initialize") );
	m_AccountNumber.Empty();
	m_Amount.Empty();
	m_BankNumber.Empty();
	m_EPC.Empty();
	m_RawData.Empty();
	m_SerialNumber.Empty();
	m_TransitNumber.Empty();
	m_CheckType = MICR_CT_UNKNOWN;
	m_CountryCode = MICR_CC_UNKNOWN;
	
    // Remember "RemoveNonDigits" variable from the registry.
    m_sRemoveNonDigits = sRemoveNonDigits;
}

//
// Function:  GetPropertyString
// Purpose:   Implementation of the OPOS SO method for returning string
//            property values.
// Input:     lPropIndex - identifies the property to be returned.
// Output:    The string value of the property.
// Assumption:  The calling method is responsible for validation of 
//              lPropIndex.  This function assumes it will be passed a
//              valid lPropIndex.
// Comments: 
//   The corresponding SetPropertyString is not needed for the MICR
//   since all of the string properties are read only.  The ParseMICRData()
//   routine is passed the RawData value, which causes an implicit set
//   of all properties.
//
CString CMicrProperties::GetPropertyString( long lPropIndex )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CMicrProperties::GetPropertyString") );
	CString sResult;
	switch( lPropIndex )
	{
	case PIDXMicr_AccountNumber:
		sResult = m_AccountNumber;
		break;
	case PIDXMicr_Amount:
		sResult = m_Amount;
		break;
	case PIDXMicr_BankNumber:
		sResult = m_BankNumber;
		break;
	case PIDXMicr_EPC:
		sResult = m_EPC;
		break;
	case PIDXMicr_RawData:
		sResult = m_RawData;
		break;
	case PIDXMicr_SerialNumber:
		sResult = m_SerialNumber;
		break;
	case PIDXMicr_TransitNumber:
		sResult = m_TransitNumber;
		break;

	default:
		break;
	}
	return sResult;
}

//
// Function:  GetPropertyNumber
// Purpose:   Get a numeric property
// Inputs:    lPropIndex - ID of the property value to be returned.
// Outputs:   long value of the property.
// Assumptions:  None
// Comments:  None
//
long CMicrProperties::GetPropertyNumber( long lPropIndex ) 
{
    CDataCapEntryExit DCE( m_cmDc, _T("CMicrProperties::GetPropertyNumber") );
	long lResult;
	switch( lPropIndex )
	{
	case PIDXMicr_CheckType:
		lResult = m_CheckType;
		break;
	case PIDXMicr_CountryCode:
		lResult = m_CountryCode;
		break;

	default:
		break;
	}
	return lResult;

}

/////////////////////////////////////////////////////////////////////////////
//
// The following functions are copied directly from OPOS.  A comment near the
// top of this file indicates the OPOS file version on which this file is
// based.
//
// NOTE:  Tally-related code was removed from ParseMicrData following copy.
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ParseMicrData :  Parse raw data into properties.

void CMicrProperties::ParseMicrData(const CString& sRawData)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CMicrProperties::ParseMicrData") );
	m_RawData = sRawData;    // Set raw data as property

    static LPCTSTR DefaultParseStrings[] =
    {
        // (1): Serial number in Auxiliary On-Us field.
        //      Account number in On-Us field from first digit to the On-Us symbol,
        //        not including spaces and dashes before the On-Us.
_T("BoS[1+]oBEtT[9]tBA[1+]Bo*"),
        // (2): Account number in On-Us field. This field starts with an optional
        //        On-Us symbol, then the account number is taken from first digit  
        //        to the required trailing On-Us symbol, not including spaces and 
        //        dashes before the trailing On-Us.
        //      Serial number in On-Us field after the On-Us symbol.
        //        Must be three or more digits.
_T("BEtT[9]tBA[1+]BoBS[3+]*"),
		// (2a): Same as above, but also allows an On-Us symbol between the
		//        transit and account numbers.
_T("BEtT[9]tBoA[1+]BoBS[3+]*"),
        // (2b): Same as #2 above, but allows for numbers after On-Us followed by
		//        a blank before the serial number.	 TAR # 205715
_T("BEtT[9]tBA[1+]BoX[2]B[1]S[3+]*"),
        // (3): Serial number in On-Us field from first digit to next non-digit;
        //        must be between three and five digits.
        //      Account number in On-Us field from first digit after the serial
        //        number to the On-Us symbol, not including spaces and dashes
        //        before the On-Us.
_T("BEtT[9]tBS[3-5]B[1+]A[1+]Bo*"),
        // (4): Account number in On-Us field from first digit to the On-Us symbol,
        //        not including spaces and dashes before the On-Us.
_T("BEtT[9]tBA[1+]Bo*"),
        // (5): Serial number in On-Us field from first digit to the On-Us symbol,
        //        must be three or more digits.
        //      Account number in second On-Us field from the first digit after the
        //        On-Us symbol that ended the serial number to the On-Us symbol, 
        //        not including spaces and dashes before the On-Us.
_T("BtT[9]tBS[3+]BoBA[1+]o*"),
        // NULL pointer to terminate the list.
        NULL
    };

    m_cmDc->DCPrintf( TRACE_ALWAYS, _T("+CMicrProperties::ParseMicrData: Parsing data.") );

    // Initialize success/fail variable.
    BOOL bParsed = FALSE;

    // Prepare for loop that tries to parse the data.
    //  - First will try the exception strings.
    //  - Next try the default parsing strings.
    CString sExcep;
    LPCTSTR pExcep;
    LPCTSTR pExcepStart;
    POSITION pos = m_ExcepList.GetHeadPosition();
    LPCTSTR* ppDfltString = DefaultParseStrings;

    // Initialize the properties that don't get changed in the loop.
    m_BankNumber.Empty();
    m_CheckType = MICR_CT_UNKNOWN;
    m_CountryCode = MICR_CC_UNKNOWN;

    // Loop for the parsing strings.
    for (;;)
    {
        // Clear the properties.
        m_EPC.Empty();
        m_TransitNumber.Empty();
        m_AccountNumber.Empty();
        m_SerialNumber.Empty();
        m_Amount.Empty();

        // Get the pointer to the next parse string.
        //  - If another exception string, use it.
        if ( pos != NULL )
        {
            sExcep = m_ExcepList.GetNext( pos );
            pExcep = sExcep;
        }
        //  - If no more exception strings, then
        //    if another default string, use it.
        else if ( *ppDfltString != NULL )
        {
            pExcep = *ppDfltString++;
        }
        //  - If no more exception or default strings, break out: Couldn't parse.
        else
        {
            break;
        }

        // Try to parse using the string.
        //   If succeeds, then break out: Parse ok.
        //   Else will loop to try the next.
        pExcepStart = pExcep;
        if ( ParseToString( pExcep ) )
        {
            bParsed = TRUE;
            break;
        }
        m_cmDc->DCPrintf( TRACE_ALWAYS, _T(" ..Failed parse to <%s> near char %d"),
            pExcepStart, pExcep - pExcepStart );
    }

    // If failed, say so and exit.
    if ( ! bParsed )
    {
        m_cmDc->DCPrintf( TRACE_ALWAYS, _T("-..: Failed all parse strings") );

        return;
    }
	// NOTE:  Tally-related code on parse failure/success has been removed

    // Bank number: Set to digits 4-8 of transit number.
    m_BankNumber = m_TransitNumber.Mid( 3, 5 );

    // Determine check type:
    //  - If secondary On-Us field: Business check.
    //  - If Transit 1-4 = "8000": Travelers Check -> Unknown.
    //  - Else personal.
    //  * Look for On-Us or Transit. If neither, then no idea what kind of check this is,
    //      so leave at "unknown". Otherwise continue into "if" body.
    int nIndex = sRawData.FindOneOf( _T("ot") );
    if ( nIndex >= 0 )
    {
        if ( sRawData.GetAt( nIndex ) == 'o' )          // If On-Us before Transit,
            m_CheckType = MICR_CT_BUSINESS;             //   then business.
        else if ( m_TransitNumber.Left( 4 ) == _T("8000") ) // If Transit starts with "8000",
            ;                                           //   then unknown (travelers).
        else                                            // Else...
            m_CheckType = MICR_CT_PERSONAL;             //   assume personal.
    }

    // Create a transit number with only digits for later use.
    CString sTransitDigits = m_TransitNumber;
    RemoveNonDigits( sTransitDigits );

    // Determine country code (need more data for Mexico):
    //  - If Transit #6 is dash: Canadian.
    //  - If Unknown check type: Unknown.
    //  - If Transit is all-digits and Transit 1-2 = US Federal Reserve District
    //      (range 00-13 or 20-33): U.S.
    //  - Else unknown.
    int nFedDist = _ttoi( m_TransitNumber.Left( 2 ) );   // (First convert left 2 Transit digits )
    if ( m_TransitNumber.GetAt(5) == '-' )              // If Transit char #6 is dash,
        m_CountryCode = MICR_CC_CANADA;                 //   then Canadian.
    else if ( m_CheckType == MICR_CC_UNKNOWN )          // If check type unknown.
        ;                                               //   then unknown.
    else if ( m_TransitNumber.FindOneOf( _T(" -") ) >= 0 )  // If non-digit (space or dash) in Transit,
        ;                                               //   then unknown.
    else if (                                           // Check US Fed district number:
              (  0 <= nFedDist && nFedDist <= 13 ) ||   //   If 0-13 or
              ( 20 <= nFedDist && nFedDist <= 33 )      //     20-33,
            )
        m_CountryCode = MICR_CC_USA;                    //   then United States.
    //else                                              // Else...
    //    ;                                             //   Leave at unknown.

    // Based on registry, remove spaces and dashes from the transit
    //   and account numbers.
    if ( m_sRemoveNonDigits.FindOneOf( _T("Tt") ) >= 0 )
        m_TransitNumber = sTransitDigits;
    if ( m_sRemoveNonDigits.FindOneOf( _T("Aa") ) >= 0 )
        RemoveNonDigits( m_AccountNumber );

    // Convert check type and country code.
    static const LookupTable aCheckType[] =
    {
        { 0,                                    _T("%d [SWBug!]") },
        { MICR_CT_PERSONAL /* 1 */,             _T("MICR_CT_PERSONAL (%d)") },
        { MICR_CT_BUSINESS /* 2 */,             _T("MICR_CT_BUSINESS (%d)") },
        { MICR_CT_UNKNOWN /* 99 */,             _T("MICR_CT_UNKNOWN (%d)") }
    };
    m_sCheckType = LookupNumber( m_CheckType, aCheckType, LTCount(aCheckType) );

    static const LookupTable aCountryCode[] =
    {
        { 0,                                    _T("%d [SWBug!]") },
        { MICR_CC_USA /* 1 */,                  _T("MICR_CC_USA (%d)") },
        { MICR_CC_CANADA /* 2 */,               _T("MICR_CC_CANADA (%d)") },
        { MICR_CC_MEXICO /* 3 */,               _T("MICR_CC_MEXICO (%d)") },
        { MICR_CC_UNKNOWN /* 99 */,             _T("MICR_CC_UNKNOWN (%d)") }
    };
    m_sCountryCode = LookupNumber( m_CountryCode, aCountryCode, LTCount(aCountryCode) );

    // Data cap results.
    m_cmDc->DCPrintf( TRACE_ALWAYS,
        _T("-..: Parsed to <%s> into:\r\n")
        _T("   EPC           <%s>\r\n")
        _T("   TransitNumber <%s>\r\n")
        _T("   BankNumber       <%s>\r\n")
        _T("   AccountNumber <%s>\r\n")
        _T("   SerialNumber  <%s>\r\n")
        _T("   Amount        <%s>\r\n")
        _T("   CheckType     %s\r\n")
        _T("   CountryCode   %s"),
        pExcepStart,
        m_EPC,
        m_TransitNumber, m_BankNumber,
        m_AccountNumber,
        m_SerialNumber,
        m_Amount,
        m_sCheckType,
        m_sCountryCode
        );
}

/////////////////////////////////////////////////////////////////////////////
// ParseToString :  Try to parse raw data into properties, given an exception string.

BOOL CMicrProperties::ParseToString(
    LPCTSTR& pExcep )                   // In > Exception string.
{
    static TCHAR caAmount[] = _T("a$[10]aB");

    LPCTSTR pRaw = m_RawData;
    LPCTSTR pTransit = NULL;
    TCHAR cExcep;
    CString sJunk;

    while ( *pExcep != '\0' )
    {
        cExcep = *pExcep;

        // *: Trailer: Series of any digits, dashes, and spaces followed by an
        //    optional amount field
        //      (= "X[0+]a$[10]aB" if amount present and "X[0+]" if not).
        if ( cExcep == '*' )
        {
            // Parse "X[0+]".
            m_nRMin = 0;
            m_nRMax = 9999;
            ParseSubString( pRaw, TypeDigitDashSpace, sJunk );
            // If next character is NUL, then done: Break out and return success.
            if ( *pRaw == '\0' )
                break;
            // Else must finish with an amount.
            pExcep = caAmount;
            continue;
        }

        // Parse the optional repeat qualifier.
        // Also advance pExcep to the character after the exception character
        //   and repeat qualifier.
        if ( ! ParseRepeats( pExcep ) )
            return FALSE;

        // Based on the exception character...
        switch ( cExcep )
        {
        // Anything other than those listed below must match exactly.
        //   Includes 'o', 'a', '-', ' ', and digits.
        default:
            {
                // If doesn't match Micr data, then fail.
                if ( cExcep != *pRaw )
                    return FALSE;
                // If match, advance the Micr data pointer and keep going.
                pRaw++;
                break;
            }

        // Transit symbol.
        case 't':
            {
                // If doesn't match Micr data, then fail.
                if ( cExcep != *pRaw )
                    return FALSE;
                // Advance the Micr data pointer past the Transit symbol.
                pRaw++;
                // Since we allow the transit portion to be specified by
                //   explicit characters (the usual case) and/or by special
                //   characters (most likely 'T') for wildcards...
                //   Remember the start of the transit field, then form the
                //   actual field at the terminating Transit symbol.
                //  * If first symbol, then just remember start of number.
                if ( pTransit == NULL )
                    pTransit = pRaw;
                //  * If second symbol, then extract the transit number from
                //    between the two symbols.
                //    (There should be at least one character.)
                else
                {
                    ASSERT( pRaw - pTransit > 1 );
                    m_TransitNumber = CString( pTransit, pRaw - pTransit - 1 );
                    pTransit = NULL;
                }
                break;
            }

        // X: Digit, space, or dash.
        case 'X':
            {
                // Parse the data and discard.
                if ( ParseSubString( pRaw, TypeDigitDashSpace, sJunk ) == FALSE )
                    return FALSE;
                break;
            }

        // E: EPC: Digit.
        case 'E':
            {
                // Parse the data and save.
                if ( ParseSubString( pRaw, TypeDigit, m_EPC ) == FALSE )
                    return FALSE;
                break;
            }

        // T: Transit number: Digit or dash.
        //      (For US checks, only digits are allowed.)
        case 'T':
            {
                // Parse the data and discard. (Will get the data later
                //   by extracting it from the Micr data.)
                if ( ParseSubString( pRaw, TypeDigitDash, sJunk ) == FALSE )
                    return FALSE;
                break;
            }

        // A: Account number: Digit, space, or dash.
        //      (Account number begins and ends with a digit.)
        case 'A':
            {
                // Parse the data and save.
                if ( ParseSubString( pRaw, TypeDigitDashSpace, m_AccountNumber ) == FALSE )
                    return FALSE;
                break;
            }

        // S: Serial/sequence number: Digit.
        case 'S':
            {
                // Parse the data and save.
                if ( ParseSubString( pRaw, TypeDigit, m_SerialNumber ) == FALSE )
                    return FALSE;
                break;
            }

        // $: Amount: Digit.
        case '$':
            {
                // Parse the data and save.
                if ( ParseSubString( pRaw, TypeDigit, m_Amount ) == FALSE )
                    return FALSE;
                break;
            }

        // B: Series of blanks (spaces) and/or dashes.
        case 'B':
            {
                // Parse the data and discard.
                if ( ParseSubString( pRaw, TypeDashSpace, sJunk ) == FALSE )
                    return FALSE;
                break;
            }
        }
    }


    if ( *pRaw != '\0' )
        return FALSE;

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// ParseRepeats :  Parse the exception character repeat count(s).

BOOL CMicrProperties::ParseRepeats(           // Return TRUE if valid data; FALSE if bad format.
                                    //  If TRUE, then m_nRMin and m_nRMax are set.
    LPCTSTR& pExcep )                   // I/O> Pointer to exception string.
                                        //   Entry: Points to exception character.
                                        //   Exit:  Points to character after repeat count.
{
    TCHAR cChar = *pExcep;

    // If no repeat count...
    if ( *++pExcep != '[' )
    {
        m_nRMin = 0;
        m_nRMax = 1;

        switch ( cChar )
        {
        // If exception character is B, then treat like [0+].
        case 'B':
            {
                //m_nRMin = 0;
                m_nRMax = 9999;
                break;
            }
        // If exception character is E, then treat like [0-1].
        case 'E':
            {
                //m_nRMin = 0;
                //m_nRMax = 1;
                break;
            }
        // Else treat like [1,1].
        default:
            {
                m_nRMin = 1;
                //m_nRMax = 1;
                break;
            }
        }
        // Return good status.
        return TRUE;
    }

    // Process repeat count...
    //  - Parse the maximum.
    m_nRMin = 0;
    while ( cChar = *++pExcep, _istdigit( cChar ) )
        m_nRMin = m_nRMin * 10 + cChar - '0';
    //  - Wrap up minimum.
    //     Format "[#]": Min = Max.
    if ( cChar == ']' )
    {
        m_nRMax = m_nRMin;
        pExcep++;
        return TRUE;
    }
    //     Format "[#+]": Min specified; no max.
    else if ( cChar == '+' && *++pExcep == ']' )
    {
        m_nRMax = 9999;
        pExcep++;
        return TRUE;
    }
    //     Format "[#-#]": Both min and max...
    else if ( cChar == '-' )
    {
        //  - Parse the maximum.
        m_nRMax = 0;
        while ( cChar = *++pExcep, _istdigit( cChar ) )
            m_nRMax = m_nRMax * 10 + cChar - '0';
        //  - Wrap up maximum.
        //     Verfify Min <= Max.
        if ( m_nRMin > m_nRMax )
            return FALSE;
        //     Verify that terminator is OK.
        if ( cChar == ']' )
        {
            pExcep++;
            return TRUE;
        }
        // else fall through.
    }
    // Bad format if none of above.
    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// ParseSubString :  Parse a substring with the type of characters specified.

// The following array has bits on to denote the category of a character:
//   (01) Bit 0 if digit.
//   (02) Bit 1 if digit or dash.
//   (04) Bit 2 if digit, space, or dash.
//   (10) Bit 4 if space or dash.
static char caType[] =
{
 // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 00
 // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 08
 // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 10
 // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 18
    0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 20:  !"#$%&'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, // 28: ()*+,-./
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, // 30: 01234567
    0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 38: 89:;<=>?
 // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 40: @ABCDEFG
 // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 48: HIJKLMNO
 // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 50: PQRSTUVW
 // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 58: XYZ[\]^_
 // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 60: `abcdefg
 // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 68: hijklmno
 // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 70: pqrstuvw
 // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 78: xyz{|}~
};
#define MatchType(c)                      /* Returns TRUE if char "c" matches "cType": */ \
    ( ( (c) >= 0x20 ) &&                  /* If character in the                       */ \
      ( (c) < 0x20+sizeof(caType) ) &&    /*   "caType" array, and                     */ \
      ( caType[(c)-0x20] & cType ) != 0 ) /*   bits of its array element match "cType" */
                                          /*  -> Match.                                */

BOOL CMicrProperties::ParseSubString(         // Return TRUE if valid data; FALSE if bad format.
    LPCTSTR& pRaw,                      // In > Pointer to Micr data.
    char cType,                         // In > Type of characters permitted.
    CString& sString )                  // Out> Parsed substring.

                                        // In > m_nRMax & m_nRMin; they are destroyed.
{
    // Parse at least the minimum number of characters, or return FALSE.
    m_nRMax -= m_nRMin;
    while ( m_nRMin )
    {
        if ( ! MatchType(*pRaw) )
            return FALSE;
        sString += *pRaw++;
        m_nRMin--;
    }
    // Allow up to max-min more characters.
    while ( m_nRMax )
    {
        if ( ! MatchType(*pRaw) )
            break;
        sString += *pRaw++;
        m_nRMax--;
    }
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// RemoveNonDigits :  Remove any non-digit characters from the string.

void CMicrProperties::RemoveNonDigits(
    CString& sString )                  // I/O> String.
{
    CString s;
    LPCTSTR p = sString;
    while ( *p != '\0' )
    {
        if ( _istdigit( *p ) )
            s += *p;
        p++;
    }
    sString = s;
}

