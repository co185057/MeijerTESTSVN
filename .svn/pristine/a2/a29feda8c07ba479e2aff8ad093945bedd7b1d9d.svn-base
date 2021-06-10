// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/XString.cpp 1     6/20/08 1:13p Sa250050 $
/*******************************************************************************
* FILE NAME: xstring.cpp                                                      *
*                                                                              *
* DESCRIPTION:                                                                 *
*   Declaration of the class(es):                                              *
*     IString - general-purpose string class                                   *
*                                                                              *
*******************************************************************************/
/*
// $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/XString.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 8     5/03/05 4:11p Dm185016
 * Corrected crash if pointer null for IStringBuffer::operator += ().
 * 
 * 6     3/31/05 2:29p Dm185016
 * TAR 297138
 * 
 * 5     3/24/05 7:36p Dm185016
 * UNICODE
 * 
 * 4     2/28/05 5:23p Dm185016
 * Unicode problems
 * 
 * 3     2/28/05 3:37p Dm185016
 * lint
 * 
 * 2     2/21/05 8:01p Dm185016
 * Added new += operators accepting unsigned long and long for
 * StringBuffer
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
// 
// 16    1/18/05 4:06p Dm185016
// UNICODE support
// 
// 15    1/13/05 2:22p Dm185016
// TAR 287359.
// 
// 14    12/16/04 10:13a Dm185016
// Repaired problems with token iterator.
// 
// 13    12/14/04 11:29a Dm185016
// 
// 12    11/29/04 7:12p Dm185016
// Fixed trim() method.
// 
// 11    10/29/04 9:34a Dm185016
// Added token iterator.
// 
// 10    10/05/04 8:25a Dm185016
// Cleaned up error processing.
// 
// 8     6/30/04 3:48p Dm185016
// Lint.
// Added new IStringBuffer class.
// 
// 7     3/31/04 10:03a Dm185016
// Added format() method
// 
// 5     3/19/04 3:32p Dm185016
// Memory leak fix
// 
// 4     2/18/04 9:16a Dm185016
// Added trim() function
// 
// 3     2/10/04 11:54a Dm185016
// Fixed compile problems
// 
// 2     1/23/04 3:42p Dm185016
// New DLL Support
// Removed CTOR for std::string
// 
// 1     1/06/04 9:00a Dm185016
// First Version
*/

//lint --e{668}
//lint --e{613}
//lint -esym(1550, IString::IString)
//lint -esym(1550, IStringBuffer::IStringBuffer)
#include "StdAfx.h"

#include <new>
#include "XString.h"
#include "XError.h"

static const _TCHAR* TRUE_STRING = _T("true");
static const _TCHAR* FALSE_STRING = _T("false");


IString :: IString ( const IString &original ) throw()
    : myString(NULL)
    , myWString(NULL)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
{
    if (original.myWString == NULL)
        init((const char *)original, original.size() );
    else
        init((const wchar_t *)original, original.size() );
}


IString :: IString( ) throw()
    : myString(NULL)
    , myWString(NULL)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
{
    uStringLength = 0;
    myString = new CHAR[1];
    myString[0] = 0;
}


//lint -e{818}  Do NOT want ptr to const
IString :: IString ( char *ucString, size_t ulLength) throw()
    : myString(NULL)
    , myWString(NULL)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
//  : myString(new string(ucString, ulLength))
{
    init(ucString, ulLength);
}


IString :: IString ( const char *ucString, size_t ulLength) throw()
    : myString(NULL)
    , myWString(NULL)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
//  : myString(new string(ucString, ulLength))
{
    init(ucString, ulLength);
}


//lint -e{818}  Do NOT want ptr to const
IString :: IString ( char *ucString) throw()
    : myString(NULL)
    , myWString(NULL)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
{
    init(ucString, getStrlen(ucString));
}


IString :: IString ( const char *ucString) throw()
    : myString(NULL)
    , myWString(NULL)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
{
    init(ucString, getStrlen(ucString));
}


//IString :: IString ( const IString &string )
IString :: IString ( char _c ) throw()
    : myString(NULL)
    , myWString(NULL)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
{
    uStringLength = 1;
    myString = new CHAR[2];

    myString[0] = _c;
    myString[uStringLength] = 0;
}


IString :: IString ( int intValue) throw()
    : myString(NULL)
    , myWString(NULL)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
{
    char ucValue[18];

    _itoa(intValue, ucValue, 10);
    init(ucValue, strlen(ucValue));
}


IString :: IString ( unsigned intValue) throw()
    : myString(NULL)
    , myWString(NULL)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
{
    char ucValue[18];

    _itoa((int)intValue, ucValue, 10);
    init(ucValue, strlen(ucValue));
}


IString :: IString ( long lValue ) throw()
    : myString(NULL)
    , myWString(NULL)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
{
    char ucValue[33];

    _ltoa(lValue, ucValue, 10);
    init(ucValue, strlen(ucValue));
}


IString :: IString ( unsigned long ulValue) throw()
    : myString(NULL)
    , myWString(NULL)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
{
    char ucValue[33];

    _ultoa(ulValue, ucValue, 10);
    init(ucValue, strlen(ucValue));
}


IString :: IString ( short sValue ) throw()
    : myString(NULL)
    , myWString(NULL)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
{
    init(sValue);
}


IString :: IString ( wchar_t wc ) throw()
    : myString(NULL)
    , myWString(NULL)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
{
    wchar_t wsValue[2];
    wsValue[0] = wc;
    wsValue[1] = 0;

    init(wsValue, 1);
}



IString :: IString(float floatVal) throw()
    : myString(NULL)
    , myWString(NULL)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
{
	char ucValue[64];
    int tmpLength;
	if (EOF == (tmpLength = sprintf(ucValue, "%g", static_cast<double>(floatVal))))
    {
        uStringLength = 0;
    }
    else
    {
        uStringLength = (ULONG)tmpLength;
    }
    init(ucValue, uStringLength);
}

IString :: IString ( double doubleVal ) throw()
    : myString(NULL)
    , myWString(NULL)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
{
	char ucValue[64];
    int tmpLength;
	if (EOF == (tmpLength = sprintf(ucValue, "%g", doubleVal)))
    {
        uStringLength = 0;
    }
    else
    {
        uStringLength = (ULONG)tmpLength;
    }
    init(ucValue, uStringLength);
}


IString :: IString (bool boolVal) throw()
    : myString(NULL)
    , myWString(NULL)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
{
	const _TCHAR *pStr = (boolVal ? TRUE_STRING : FALSE_STRING);

    init(pStr, _tcslen(pStr));
}

IString :: IString (const wchar_t *uwszString, size_t theLength) throw()
    : myString(NULL)
    , myWString(NULL)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
{
	init(uwszString, theLength);
}


#ifdef _MSC_VER
//-----------------------------
// Convert __int64 to String
//-----------------------------
IString :: IString (__int64 i64value) throw()
    : myString(NULL)
    , myWString(NULL)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
{
    char ucValue[33];

    _i64toa(i64value, ucValue, 10);
    init(ucValue, strlen(ucValue));
}


//-----------------------------
// Convert unsigned __int64 to String
//-----------------------------
IString :: IString (unsigned __int64 ui64value) throw()
    : myString(NULL)
    , myWString(NULL)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
{
    char ucValue[33];

    _ui64toa(ui64value, ucValue, 10);
    init(ucValue, strlen(ucValue));
}
#endif			


//lint -e{818}
IString :: IString ( wchar_t *uwszString) throw()
    : myString(NULL)
    , myWString(NULL)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
{
    init(uwszString, getWStrlen(uwszString));
}


IString :: IString ( const wchar_t *ucwszString) throw()
    : myString(NULL)
    , myWString(NULL)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
{
    init(ucwszString, getWStrlen(ucwszString));
}


IString::IString(const _variant_t &var) throw()
    : myString(NULL)
    , myWString(NULL)
    , uStringLength(0)
	, m_pTokens(NULL)
	, m_pEndToken(NULL)
{
    switch (((VARIANT)var).vt)
    {
        case VT_UI1 :
            break;
        case VT_I2 :
            init((short)var);
            break;
        case VT_I4 :
            init((long)var);
            break;
        case VT_BOOL :
            break;
        case VT_DATE :
            break;
        case VT_BSTR :
            init((_bstr_t)var);
            break;
        default :
            break;
    }
}


//IString :: IString ( BSTR bstrString)
//    : myString(NULL)
//    , myWString(NULL)
//{
//    int nReqSize = WideCharToMultiByte(
//                        CP_THREAD_ACP,              // code page
//                        WC_NO_BEST_FIT_CHARS,       // performance and mapping flags
//                        bstrString,                 // wide-character string
//                        ::SysStringLen(bstrString), // number of chars in string
//                        0,                          // buffer for new string
//                        0,                          // size of buffer
//                        NULL,                       // default for unmappable chars
//                        NULL                        // set when default char used
//                    );
//
//    myString = new CHAR[nReqSize+1];
//    if (szString != NULL)
//    {
//        if ( 0 == WideCharToMultiByte(
//                            CP_THREAD_ACP,              // code page
//                            WC_NO_BEST_FIT_CHARS,       // performance and mapping flags
//                            bstrString,                 // wide-character string
//                            ::SysStringLen(bstrString), // number of chars in string
//                            myString,                   // buffer for new string
//                            nReqSize,                   // size of buffer
//                            NULL,                       // default for unmappable chars
//                            NULL                        // set when default char used
//                        ) )
//        {
//            ITHROWSYSTEMERROR(GetLastError(), "WideCharToMultiByte", IException::unrecoverable);
//        }
//    }
//    uStringLength = nReqSize;
//
//    myString[nReqSize] = 0;
//}

/*
IString :: IString (const std::string &str)
    : myString(NULL)
    , myWString(NULL)
{
    init(str.c_str(), str.size());
//printf("myString=\"%s\".\n", myString);
}
*/


void IString ::init( const char *szString, size_t ulLength) throw()
{
    try
    {
        delete [] myString;

        myString = new char[ulLength+1];
        if (szString != NULL)
            memcpy(myString, szString, ulLength);
        uStringLength = ulLength;

        myString[ulLength] = 0;
    }
    catch (...)
        { /* ITRACE_DOT_ERROR(); */ }
}


static const wchar_t *getWideString(
            /* [in] */ const char *pchVal,
            /* [in] */ int cchVal)
{
    cchVal = cchVal>999 ? 999 : cchVal;
    static wchar_t wval[1000];
    mbstowcs(wval, pchVal, cchVal);
    wval[cchVal] = 0;
    return wval;
}


static const char *getAsciiString(
            /* [in] */ const wchar_t __RPC_FAR *pwchVal,
            /* [in] */ int cchVal)
{
    static wchar_t wval[1000];
    cchVal = cchVal>999 ? 999 : cchVal;
    wcsncpy( wval, pwchVal, (unsigned)cchVal ); 
    wval[cchVal] = 0;

	static char val[1000];
	wcstombs(val, wval, 1000);
    return val;
}


void IString ::init( const wchar_t *wszString, size_t ulLength) throw()
{
    try
    {
        delete [] myString;
        delete [] myWString;

        myString    = new char [ulLength+1];
        myWString   = new wchar_t[ulLength+1];

        if (wszString != NULL)
        {
            const char *aString = getAsciiString(wszString, (int)ulLength);
            memcpy(myString, aString, ulLength); 
            wmemcpy(myWString, wszString, ulLength);
        }

        uStringLength = ulLength;

        myString[ulLength]  = 0;
        myWString[ulLength] = 0;
    }
    catch (...)
        { /* ITRACE_DOT_ERROR(); */ }
}


IString :: ~IString ( )
{
    delete []myString;
    delete []myWString;
	delete m_pTokens;
	delete m_pEndToken;
}


IString &IString::clear()
{
    init((const _TCHAR *)NULL, 0);

    return *this;
}


IString::token_iterator &IString::tokens(const _TCHAR *pszDelims)
{
	delete m_pTokens;
    const _TCHAR *pBuffer = THE_BUFFER;
    m_pTokens = new token_iterator(pBuffer, pszDelims);

	return *m_pTokens;
}


IString::token_iterator &IString::last_token() 
{
    if (m_pEndToken == NULL)
	    m_pEndToken = new token_iterator(m_pTokens);

	return *m_pEndToken;
}


IString & IString :: operator = ( const IString &s )
{
    if (&s != this)
    {
        delete []myString;
        delete []myWString;
        uStringLength = s.size();

        myString = new CHAR[(UINT)uStringLength+1];
        memcpy(myString, (const CHAR *)s, (UINT)uStringLength);
        myString[uStringLength] = 0;
        if (myWString != NULL)
        {
            myWString = new wchar_t[(UINT)uStringLength+1];
            wmemcpy(myWString, (const wchar_t *)s, (UINT)uStringLength);
            myWString[uStringLength] = 0;
        }
        else
            myWString = NULL;

        delete m_pTokens;
        m_pTokens = NULL;
    
        delete m_pEndToken;
        m_pEndToken = NULL;
    } /* endif */

    return *this;
}

/*---------------------------------------*/
/* methods for compatibility with CSet++ */
/*---------------------------------------*/


IString & IString :: d2x()
{
    ULONG   ulValue = strtoul(myString, 0, 10);
    char    ucValue[34];

    delete []myString;
    delete []myWString;

    _ultoa(ulValue, ucValue, 16);
    uStringLength = strlen(ucValue);

    myString    = new CHAR[(UINT)uStringLength+1];
    myWString   = NULL;

    memcpy(myString, ucValue, (UINT)uStringLength);
    myString[uStringLength] = 0;

    return *this;
}


IString &IString :: c2x()
{
    UINT        i, j;
    UCHAR       testChar;
    CHAR  * newString = new CHAR[(UINT)((uStringLength*2)+1)];

    for (i=0, j=0; i<uStringLength ; i++, j+=2 ) {

        testChar = (UCHAR)((((UCHAR)myString[i]) & ((UCHAR)'\xf0')) >> 4);
        switch (testChar) {
            case 0x0a : case 0x0b : case 0x0c : case 0x0d : case 0x0e : case 0x0f :
                newString[j] = (CHAR)(testChar + 0x37);
                break;
            default:
                newString[j] = (CHAR)(testChar + 0x30);
                break;
        } /* endswitch */

        testChar = (UCHAR)(((UCHAR)myString[i]) & '\x0f');
        switch (testChar) {
            case 0x0a : case 0x0b : case 0x0c : case 0x0d : case 0x0e : case 0x0f :
                newString[j+1] = (CHAR)(testChar + 0x37);
                break;
            default:
                newString[j+1] = (CHAR)(testChar + 0x30);
                break;
        } /* endswitch */
    } /* endfor */
    newString[j] = '\00';

    delete []myString;
    delete []myWString;

    uStringLength *= 2;

    myString = newString;
    myWString = NULL;

    return *this;
}

    
IString &IString :: format(LPCTSTR sFormatString, ...)
{	
     size_t nLength = _tcslen(sFormatString) * 2*sizeof(_TCHAR);

    _TCHAR *pBuffer   = new _TCHAR[nLength];

    va_list argptr;
    va_start( argptr, sFormatString );

    int nCharsWritten = _vsntprintf( pBuffer, nLength, sFormatString, argptr );
    while (nCharsWritten == -1)
    {
        nLength *= 2*sizeof(_TCHAR);
        delete [] pBuffer;
        pBuffer = new _TCHAR[nLength];
        nCharsWritten = _vsntprintf( pBuffer, nLength, sFormatString, argptr );
    }

    init( pBuffer, (size_t) nCharsWritten);

    delete [] pBuffer;

    return *this;
}


IString IString :: substring(size_t offset) const
{
    if (offset > uStringLength)
    {
        ITHROW(IInvalidParameter(_T("Offset larger than length")))
    }

    size_t        newLength = uStringLength - offset;
    if (newLength == 0)
        return IString();

    const _TCHAR *pBfr = THE_BUFFER;
    IString newIString(pBfr+offset, newLength);

    return newIString;
}


int IString :: asInt ( ) const
{
    return _ttoi(THE_BUFFER);
}


unsigned long  IString :: asUnsigned ( ) const
{
	_TCHAR *stopString;
	unsigned long retValue = _tcstoul(THE_BUFFER, &stopString, 10);

    switch (errno)
    {
    	case ERANGE :
		case EDOM :
			ITHROW(IInvalidParameter(_T("Input not valid unsigned long")))
        default :
        	break;
    }

    return retValue;
}


bool  IString :: asBool ( ) const
{
    const _TCHAR *pBuffer = THE_BUFFER;
	return !_tcsicmp(TRUE_STRING, pBuffer);
}


/*lint -save -e1763 */
IString :: operator char * ( ) const throw()
{
    return myString;
}

IString :: operator const char * ( ) const throw()
{
    return myString;
}
/*lint -restore */


IString :: operator const wchar_t * ( ) const
{
    if (myWString == NULL)
    {
        myWString = new(nothrow) wchar_t[uStringLength+1];

        if (myWString != NULL)
        {
            CA2W widestring(myString);

            wcscpy(myWString, widestring);
        }
    }

    return myWString;
}


IString :: operator wchar_t * ( ) const
{
    if (myWString == NULL)
    {
        myWString = new(nothrow) wchar_t[uStringLength+1];

        if (myWString != NULL)
        {
            CA2W widestring(myString);

            wcscpy(myWString, widestring);
        }
    }

    return myWString;
}


IString IString :: operator +  ( const IString &aString ) const
{
    size_t        newLength = uStringLength + aString.size();
    CHAR  * newString = new CHAR[newLength + 1];
    memcpy(newString, myString, uStringLength);
    memcpy(newString+uStringLength, (const CHAR *)aString, aString.size());
    newString[newLength] = 0;

    IString newIString(newString);

    delete []newString;

    return newIString;
}


IString IString :: operator +  ( const char *pString ) const
{
    if (pString == NULL)
    {
        return IString(*this);
    }
    else
    {
        size_t        newLength = uStringLength + getStrlen(pString);
        CHAR  * newString = new CHAR[newLength + 1];
        memcpy(newString, myString, uStringLength);
        memcpy(newString+uStringLength, pString, getStrlen(pString));
        newString[newLength] = 0;

        IString newIString(newString);

        delete []newString;
    
        return newIString;
    }
}


IString &IString :: operator += ( const IString &aString )
{

    if ( aString.size() > 0 )
    {
        size_t  nSecondLength   = aString.size();
        size_t  newLength       = uStringLength + nSecondLength;
        _TCHAR* newString       = new _TCHAR[newLength+1];

        MEMCPY(newString, THE_BUFFER, uStringLength);
        MEMCPY(newString+uStringLength, aString.THE_BUFFER, nSecondLength);

        init(newString, newLength);

        delete []newString;
    }
    return *this;
}


IString &IString :: operator += ( const char *pString )
{
    if (pString != NULL)
    {
        size_t  nSecondLength   = strlen(pString);
        size_t  newLength       = uStringLength + nSecondLength;
        char  * newAString      = new char[newLength+1];

        memcpy(newAString, this->operator const char *(), uStringLength);
        memcpy(newAString+uStringLength, pString, nSecondLength);

        init(newAString, newLength);

        delete []newAString;
    }

    return *this;
}


IString &IString :: operator += ( const wchar_t *pwString )
{
    if (pwString != NULL)
    {
        size_t        nSecondLength = wcslen(pwString);
        size_t        newLength     = uStringLength + nSecondLength;
        wchar_t  * newWString       = new wchar_t[newLength+1];

        wmemcpy(newWString, this->operator const wchar_t *(), uStringLength);
        wmemcpy(newWString+uStringLength, pwString, nSecondLength);

        init(newWString, newLength);

        delete []newWString;
    }

    return *this;
}


bool IString :: operator < ( const IString &aString ) const
{
    if ((uStringLength == 0) && (aString.uStringLength == 0))
		return false;
    if (aString.uStringLength == 0)
        return false;
    if (uStringLength == 0)
        return true;

	return (strcmp(myString, aString.myString) < 0) ? true : false;
}


bool IString :: operator < ( const char *pString ) const
{
    size_t testLength;
	if (pString == NULL)
		testLength = 0;
	else
		testLength = strlen(pString);

    if ((uStringLength == 0) && (testLength == 0))
		return false;
    if (testLength == 0)
        return false;
    if (uStringLength == 0)
        return true;

    return (strcmp(myString, pString) < 0);
}


bool IString :: operator > ( const IString &aString ) const
{
    if ((uStringLength == 0) && (aString.uStringLength == 0))
		return false;
    if (aString.uStringLength == 0)
        return true;
    if (uStringLength == 0)
        return false;

	return (strcmp(myString, aString.myString) > 0) ? true : false;
}


bool IString :: operator > ( const char *pString ) const
{
    size_t testLength;
	if (pString == NULL)
		testLength = 0;
	else
		testLength = strlen(pString);

    if ((uStringLength == 0) && (testLength == 0))
		return false;
    if (testLength == 0)
        return true;
    if (uStringLength == 0)
        return false;

    return (strcmp(myString, pString) > 0);
}


bool IString :: operator == ( const IString &aString ) const
{
    if ( aString.size() == size() )
    {
		if (uStringLength != 0)
		{
			return !memcmp(myString, aString.myString, uStringLength);
		}

		return true;
    }

    return false;
}


bool IString :: operator == ( const char *pString ) const
{
    size_t testLength;
	if (pString == NULL)
		testLength = 0;
	else
		testLength = strlen(pString);

    if ( testLength == uStringLength )
    {
		if (uStringLength != 0)
		{
			return !memcmp(myString, pString, uStringLength);
		}

		return true;
    }

    return false;
}


/*
const char& IString :: operator [] (unsigned index) const
{
	if (index < uStringLength)
		return myString[index];
	else
		ITHROW(IInvalidRequest("Index beyond end"));
}


char &IString :: operator [] (unsigned index)
{
	if (index >= uStringLength)
	{
        UINT        newLength = index+1;
        CHAR  * newString = new CHAR[newLength+1];

        memcpy(newString, myString, uStringLength);
        memset(newString+uStringLength, 0, index-uStringLength+1);
        newString[newLength] = 0;

        delete []myString;
        delete []myWString;

        myString       = newString;
        uStringLength = newLength;
	}

	return myString[index];
}
*/


/*------------------------------------------------------------------------------
| operator +                                                                   |
------------------------------------------------------------------------------*/
IString operator + ( const char *pString,
                     const IString &aString )
{
    IString myString(pString);

    return (myString + aString);
}


IString operator +  ( const wchar_t *pString,
                    const IString &aString )
{
    IString retString(pString);
    retString += (const wchar_t *)aString;

    return retString;
}


IString
    &IString :: trim(char trimCharacter)
{
    _ASSERTE(myString!=NULL);

    char *pszBegin = myString;
    for (; *pszBegin != 0 && isTrimCharacter(*pszBegin, trimCharacter); pszBegin++)
        ;

    char *pszEnd = myString + uStringLength;
    for (; pszEnd != pszBegin && isTrimCharacter(*pszEnd, trimCharacter); pszEnd--)
        ;

    if (!isTrimCharacter(*pszEnd, trimCharacter))
        pszEnd++;

    unsigned uLength = (unsigned)(pszEnd - pszBegin);
    if (pszEnd == pszBegin)
        uLength = 0;
    char * newString = new char[uLength+1];

    strncpy(newString, pszBegin, uLength);
    newString[uLength] = 0;

    delete []myString;
    delete []myWString;

    myString        = newString;
    uStringLength   = uLength;
    myWString       = NULL;

    return *this;
}


bool IString :: isTrimCharacter(char c, char cTrimChar) const throw()
{
    if (cTrimChar != 0)
        return (c == cTrimChar);

    switch (c)
    {
        case  0 : case  1 : case  2 : case  3 : case  4 : case  5 : case  6 :case  7 : case  8 : case  9 :  
        case 10 : case 11 : case 12 : case 13 : case 14 : case 15 : case 16 :case 17 : case 18 : case 19 :  
        case 20 : case 21 : case 22 : case 23 : case 24 : case 25 : case 26 :case 27 : case 28 : case 29 :  
        case 30 : case 31 : 
        case ' ' :
            return true;
        default :
            return false;
    }
}


IString &IString :: leftJustify       ( unsigned uLength,
                      				    char     padCharacter )
{
	if (uLength > uStringLength)
    {
        char * newString = new char[uLength+1];

    	unsigned long ulPadChars = uLength - uStringLength;

        strcpy(newString, myString);
        memset(newString+uStringLength, padCharacter, ulPadChars);
        newString[uLength] = 0;

        delete []myString;
        delete []myWString;

        myString       = newString;
        uStringLength  = uLength;
        myWString      = NULL;
    }
    return *this;
}


IString &IString :: lowerCase         ( )
{
    _ASSERTE(myString!=NULL);

	strlwr(myString);

    return *this;
}


IString &IString :: rightJustify       ( unsigned uLength,
                      				    char     padCharacter )
{
	if (uLength > uStringLength)
    {
        CHAR  * newString = new CHAR[uLength+1];

    	unsigned long ulPadChars = uLength - uStringLength;

        memset(newString, padCharacter, ulPadChars);
        strcpy(newString+ulPadChars, myString);
        newString[uLength] = 0;

        delete []myString;
        delete []myWString;

        myString       = newString;
        uStringLength = uLength;
    }
    return *this;
}


IString &IString :: translate         ( const char *pInputChars,
                      unsigned    inputLen,
                      const char *pOutputChars,
                      unsigned    outputLen,
                      char        padCharacter )
{
	unsigned char transChars[256];

    memset( transChars, 0xFF, 256 );
    for (size_t i = 0; i<inputLen; i++)
	{
	   	if (i>=outputLen)
    		transChars[pInputChars[i]] = (unsigned char)padCharacter;
    	else
    		transChars[pInputChars[i]] = (unsigned char)pOutputChars[i];
    }

    for (size_t j = 0; j<uStringLength; j++)
	{
    	unsigned char transChar = transChars[(int)myString[j] & 0x000000ff];
    	if (transChar != 0xFF)
    		myString[j] = transChar;
   	}

    return *this;
}

IString &IString :: upperCase         ( )
{
	strupr(myString);

    return *this;
}


bool IString :: isAbbreviationFor (
                                         const IString &aString,
                                        size_t       minAbbrevLength ) const
{
    return this->isAbbrevFor( aString.data(),
                            aString.length(),
                            minAbbrevLength );
}


bool IString :: isAbbreviationFor (
                                        const char *pString,
                                        size_t    minAbbrevLength ) const
{
    return this->isAbbrevFor( pString,
                            lengthOf( pString ),
                            minAbbrevLength );
}


bool IString :: isAbbrevFor ( const char *pFullString,
                      size_t    fullLen,
                      size_t    minLen ) const
{
	if ((fullLen == 0) || (pFullString == NULL))
    	return FALSE;
    else
    	return (strncmp(pFullString, myString, ((minLen < fullLen) ? minLen : fullLen)) == 0);
}


bool  IString :: startsWith (const wchar_t *pString) const
{
    CW2A aString(pString);
    return startsWith (aString);
}


bool  IString :: startsWith (const char *pString) const
{
    size_t chkLength = strlen(pString);

    if (chkLength > uStringLength)
        return false;

    return !strncmp(myString, pString, strlen(pString));
}    


size_t IString :: getStrlen(const char *cString) const throw()
{
	if (cString == NULL)
    	return 0;
    else
    	return strlen(cString);
}


size_t IString :: getWStrlen(const wchar_t *cString) const throw()
{
	if (cString == NULL)
    	return 0;
    else
    	return wcslen(cString);
}


/*------------------------------------------------------------------------------
| operator<<                                                                   |
|                                                                              |
| Display the IString contents on the output stream.                           |
------------------------------------------------------------------------------*/
//STD_DEF::ostream & operator << ( STD_DEF::ostream & aStream, const IString & aString )
//{
//return aStream << (CHAR *)aString;
//}


IStringBuffer :: IStringBuffer( unsigned long ulBfrLength ) throw()
    : myString(NULL)
    , myWString(NULL)
{
    m_ulBfrLength       = ulBfrLength > 0 ? ulBfrLength : 1;
    m_ulStringLength    = 0;
    myString            = new char[m_ulBfrLength];
    myString[0]         = 0;
}


IStringBuffer :: ~IStringBuffer ( )
{
    delete []myString;
    delete []myWString;
}


IStringBuffer &IStringBuffer :: clear() throw()
{
    m_ulStringLength = 0;
    myString[m_ulStringLength] = 0;

    return *this;
}


IStringBuffer :: IStringBuffer( const IStringBuffer &theOther )
    : myString(NULL)
    , myWString(NULL)
    , m_ulBfrLength(0)
{
    append(theOther.myString, theOther.size());
}


IStringBuffer & IStringBuffer :: operator = ( const IStringBuffer &s )
{
    if (&s != this)
    {
        m_ulStringLength = 0;

        append((const char *)s, s.size());
    } /* endif */

    return *this;
}


IStringBuffer & IStringBuffer :: operator = ( const char *pcsString )
{
    m_ulStringLength = 0;

    append(pcsString, strlen(pcsString));

    return *this;
}


IStringBuffer & IStringBuffer :: operator = ( const wchar_t *pwcsString )
{
    m_ulStringLength = 0;

    append(pwcsString, wcslen(pwcsString));

    return *this;
}


IStringBuffer :: operator const char * ( ) const throw()
{
    return myString;
}


IStringBuffer :: operator const wchar_t * ( ) const throw(...)
{
    if (myWString == NULL)
    {
        myWString = new wchar_t[m_ulBfrLength+1];

        CA2W widestring(myString);

        wcscpy(myWString, widestring);
    }

    return myWString;
}


IStringBuffer &IStringBuffer :: operator += ( const IStringBuffer &aStringBuffer )
{
    append(aStringBuffer.THE_BUFFER, aStringBuffer.size());

    return *this;
}


IStringBuffer &IStringBuffer :: operator += ( const IString &aString )
{
    append(aString.THE_BUFFER, aString.size());

    return *this;
}


IStringBuffer &IStringBuffer :: operator += ( const char *pString )
{
    if (pString != NULL)
    {
        size_t stLength = strlen(pString);

        append(pString, stLength);
    }

    return *this;
}


IStringBuffer &IStringBuffer :: operator += ( const wchar_t *pwString )
{
    if (pwString != NULL)
    {
        size_t stLength = wcslen(pwString);

        append(pwString, stLength);
    }

    return *this;
}


IStringBuffer &IStringBuffer :: operator += ( char cChar )
{
    char szBuffer[2];
    szBuffer[0] = cChar;
    szBuffer[1] = 0;

    append(szBuffer, 1);

    return *this;
}


IStringBuffer &IStringBuffer :: operator += ( wchar_t cChar )
{
    wchar_t szBuffer[2];
    szBuffer[0] = cChar;
    szBuffer[1] = 0;

    append(szBuffer, 1);

    return *this;
}


IStringBuffer &IStringBuffer :: operator += ( long lValue )
{
    _TCHAR ucValue[33];

    _ltot(lValue, ucValue, 10);

    append(ucValue, _tcslen(ucValue));

    return *this;
}


IStringBuffer &IStringBuffer :: operator += ( unsigned long ulValue )
{
    _TCHAR ucValue[33];

    _ultot(ulValue, ucValue, 10);

    append(ucValue, _tcslen(ucValue));

    return *this;
}
//
//
//bool IStringBuffer :: operator < ( const IStringBuffer &aString ) const
//{
//    if ((m_ulBfrLength == 0) && (aString.m_ulBfrLength == 0))
//		return false;
//    if (aString.m_ulBfrLength == 0)
//        return false;
//    if (m_ulBfrLength == 0)
//        return true;
//
//	return (strcmp(myString, aString.myString) < 0) ? true : false;
//}
//
//
//bool IStringBuffer :: operator < ( const char *pString ) const
//{
//    size_t testLength;
//	if (pString == NULL)
//		testLength = 0;
//	else
//		testLength = strlen(pString);
//
//    if ((m_ulBfrLength == 0) && (testLength == 0))
//		return false;
//    if (testLength == 0)
//        return false;
//    if (m_ulBfrLength == 0)
//        return true;
//
//    return (strcmp(myString, pString) < 0);
//}
//
//
//bool IStringBuffer :: operator > ( const IStringBuffer &aString ) const
//{
//    if ((m_ulBfrLength == 0) && (aString.m_ulBfrLength == 0))
//		return false;
//    if (aString.m_ulBfrLength == 0)
//        return true;
//    if (m_ulBfrLength == 0)
//        return false;
//
//	return (strcmp(myString, aString.myString) > 0) ? true : false;
//}
//
//
//bool IStringBuffer :: operator > ( const char *pString ) const
//{
//    size_t testLength;
//	if (pString == NULL)
//		testLength = 0;
//	else
//		testLength = strlen(pString);
//
//    if ((m_ulBfrLength == 0) && (testLength == 0))
//		return false;
//    if (testLength == 0)
//        return true;
//    if (m_ulBfrLength == 0)
//        return false;
//
//    return (strcmp(myString, pString) > 0);
//}
//
//
//bool IStringBuffer :: operator == ( const IStringBuffer &aString ) const
//{
//    if ( aString.size() == size() )
//    {
//		if (m_ulBfrLength != 0)
//		{
//			return !memcmp(myString, aString.myString, m_ulBfrLength);
//		}
//
//		return true;
//    }
//
//    return false;
//}
//
//
//bool IStringBuffer :: operator == ( const char *pString ) const
//{
//    size_t testLength;
//	if (pString == NULL)
//		testLength = 0;
//	else
//		testLength = strlen(pString);
//
//    if ( testLength == m_ulBfrLength )
//    {
//		if (m_ulBfrLength != 0)
//		{
//			return !memcmp(myString, pString, m_ulBfrLength);
//		}
//
//		return true;
//    }
//
//    return false;
//}


//const char& IStringBuffer :: operator [] (unsigned index) const
//{
//	if (index < m_ulBfrLength)
//		return myString[index];
//	else
//		ITHROW(IInvalidRequest("Index beyond end"));
//}
//
//
//char &IStringBuffer :: operator [] (unsigned index)
//{
//	if (index >= m_ulBfrLength)
//	{
//        UINT        newLength = index+1;
//        CHAR  * newString = new CHAR[newLength+1];
//
//        memcpy(newString, myString, m_ulBfrLength);
//        memset(newString+m_ulBfrLength, 0, index-m_ulBfrLength+1);
//        newString[newLength] = 0;
//
//        delete []myString;
//        delete []myWString;
//
//        myString       = newString;
//        m_ulBfrLength = newLength;
//	}
//
//	return myString[index];
//}


void IStringBuffer :: ensureBuffer(size_t stRequestedLength, bool bBoth)
{
    // Double the requested length of the buffer
    size_t stNewLength          = stRequestedLength * 2;
    if (capacity() <= stRequestedLength)
    {
        char *pszTemp               = new char[stNewLength+1];

        if (myString != NULL)
        {
            memcpy(pszTemp, myString, m_ulStringLength);
            pszTemp[m_ulStringLength]   = 0;
        }
        else
            memset(pszTemp, 0, stNewLength+1);

        delete [] myString;
        myString                    = pszTemp;

        if (bBoth || (myWString != NULL))
        {
            wchar_t *pswzTemp            = new wchar_t[stNewLength+1];

            wmemcpy(pswzTemp, this->operator const wchar_t *(), m_ulStringLength);
            pswzTemp[m_ulStringLength]   = 0;

            delete [] myWString;
            myWString                    = pswzTemp;
        }
        else
        {
            delete [] myWString;
            myWString   = NULL;
        }

        m_ulBfrLength               = stNewLength;
    }

    if (bBoth && (myWString == NULL))
    {
        size_t nLength = max(stNewLength, capacity());
        myWString = new wchar_t[nLength+1];
        myWString[0] = 0;
    }
}


IStringBuffer &IStringBuffer :: append(const char *pszBuffer, size_t stLength)
{
    if (pszBuffer == NULL)
        return *this;

    ensureBuffer(size() + stLength, true);

    if (stLength == 0)
    {
        myString[m_ulStringLength] = 0;
        myWString[m_ulStringLength] = 0;
        return *this;
    }

    memcpy(myString+m_ulStringLength, pszBuffer, stLength);
    if (myWString!=NULL)
    {
        const wchar_t * pwszString = getWideString(pszBuffer, (int)stLength);
        wmemcpy(myWString+m_ulStringLength, pwszString, stLength);
    }

    m_ulStringLength += stLength;

    myString[m_ulStringLength] = 0;
    if (myWString!=NULL)
        myWString[m_ulStringLength] = 0;

    return *this;
}


IStringBuffer &IStringBuffer :: append(const wchar_t *pwszBuffer, size_t stLength)
{
    if (pwszBuffer == NULL)
        return *this;

    ensureBuffer(size() + stLength, true);

    if (stLength == 0)
    {
        myString[m_ulStringLength] = 0;
        myWString[m_ulStringLength] = 0;
        return *this;
    }

    const char *aString = getAsciiString(pwszBuffer, (int)stLength);
    memcpy(myString+m_ulStringLength, (const char *)aString, stLength);
    wmemcpy(myWString+m_ulStringLength, pwszBuffer, stLength);

    m_ulStringLength += stLength;

    myString[m_ulStringLength] = 0;
    myWString[m_ulStringLength] = 0;

    return *this;
}
