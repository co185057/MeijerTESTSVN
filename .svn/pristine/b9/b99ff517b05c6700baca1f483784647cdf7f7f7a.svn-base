#pragma once
/*******************************************************************************
* FILE NAME: xstring.h                                                         *
*                                                                              *
* DESCRIPTION:                                                                 *
*   Declaration of the class(es):                                              *
*     IString - general-purpose string class                                   *
*                                                                              *
*******************************************************************************/

//lint -esym(534, IStringBuffer::append)

// The following is required since this file is shared by both the SecurityManager
// and SCOTAPP.  It will appear in multiple directories.  #pragma once does not
// protect from this.
#ifndef UTILS_XSTRING_H_
#define UTILS_XSTRING_H_

#ifdef _UNICODE
    #define THE_BUFFER operator const wchar_t *()
    #define MEMCPY wmemcpy
#else
    #define THE_BUFFER myString
    #define MEMCPY memcpy
#endif

#include <iostream>
#include <comutil.h>
#include "Utils.h"

    /*----------------------------------------------------------------------------*/
    /* Align classes on four byte boundary.                                       */
    /*----------------------------------------------------------------------------*/
//  #pragma pack(4)

//  extern "C"
//    {
//    #include <limits.h>
//    }


// the iterator
template <typename _Tp>
class _token_iterator 
    : public std::iterator<std::forward_iterator_tag, const _Tp *>
{

public:

	friend class IString;

    typedef std::pair<const _Tp *, const _Tp *> _pair_t;


    ~_token_iterator()
    {
        // If the translation table is null, then neither of these pointers is valid.
        // m_xChars==NULL implies than m_pszSource != NULL, but has already been freed
        // via the begin iterator.
        //AfxIsMemoryBlock(m_pszSource, 1);
        if (m_xChars != NULL)
        {
            delete []m_pszSource;
            delete []m_xChars;
        }
    }
    

    inline _pair_t &operator->()
    {
        UpdatePair();
        return m_KVPair;
    }
    

    inline _pair_t &operator*()
    {
        UpdatePair();
        return m_KVPair;
    }
    

    inline _token_iterator<_Tp> & operator ++(int)
    {
		m_pszToken = token();

        return *this;
    }


    // required to test if iteration complete
    inline bool operator!=(const _token_iterator<_Tp> & rhs) const
    {
        if (m_pszSource != rhs.m_pszSource)
            return true;
        if (m_pszEnd != rhs.m_pszEnd)
            return true;

        return (m_pszToken != rhs.m_pszToken); 
    }

protected:            
    

    inline void UpdatePair()
    {
        m_KVPair.first	= m_pszSource;
        m_KVPair.second = m_pszToken;
    }


	inline const _Tp * token()
	{
        _ASSERT(m_pszSource!=NULL);
        _ASSERT(m_xChars!=NULL);

        //if (*m_pszNext == 0)
        //{
        //    m_pszNext = m_pszEnd;
        //    return NULL;
        //}

        m_pszToken = m_pszNext;
        while ((*m_pszNext > 255) || ((*m_pszNext != 0) && (m_xChars[0xFF & (unsigned int)*m_pszNext] == 0)))
            m_pszNext++;

        *m_pszNext = 0;

        m_pszNext++;

        return m_pszToken;
    }


	inline void Reset(const _Tp *pszSource, const _Tp *pszDelims)
	{
        m_xChars = new _Tp[256];
        memset(m_xChars, 0, 256);
        if (pszDelims != NULL)
        {
            for(int i=0; pszDelims[i]!=0; i++)
            {
                _Tp c = pszDelims[i];
                _ASSERT(i<256);
                m_xChars[c] = c;
            }
        }

        m_pszStart		= pszSource;
		m_pszDelims		= pszDelims;

		if (pszSource != NULL)
		{
			m_pszSource		= new _Tp[_tcslen(m_pszStart)+1];
			_tcscpy(m_pszSource, m_pszStart);
    
            m_pszEnd        = m_pszSource+_tcslen(m_pszStart)+1;

            m_pszNext		= m_pszSource;

			m_pszToken		= token();
		}
		else 
		{
			m_pszSource		= NULL;
			m_pszToken		= NULL;
		}
	}
    
private:

    _token_iterator(const _token_iterator &);
    
    // Constructor
    _token_iterator(const _Tp * pszStart, const _Tp * pszDelims)
        : m_xChars(NULL)
        , m_pszStart(NULL)
        , m_pszSource(NULL)
        , m_pszNext(NULL)
        , m_pszEnd(NULL)
        , m_pszDelims(NULL)
    {
		Reset(pszStart, pszDelims);
    }
    
    // Constructor
    _token_iterator(_token_iterator *pIter)
        : m_xChars(NULL)
        , m_pszStart(pIter->m_pszStart)
        , m_pszSource(pIter->m_pszSource)
        , m_pszNext(NULL)
        , m_pszEnd(NULL)
        , m_pszDelims(NULL)
    {
        _ASSERTE(pIter->m_pszStart != NULL);

        size_t nLength = 0;
        if (pIter->m_pszStart != NULL) 
            nLength  = _tcslen(pIter->m_pszStart);

        m_pszToken      = pIter->m_pszSource+nLength;
        m_pszNext       = const_cast<_Tp *>(m_pszToken);
        m_pszEnd        = m_pszNext+1;
    }
    
    _pair_t m_KVPair;
    const _Tp * m_pszStart;
    const _Tp * m_pszDelims;
    const _Tp * m_pszToken;
    _Tp * m_pszNext;
	_Tp * m_pszSource;
	_Tp * m_pszEnd;

    _Tp* m_xChars;

};

//lint --e{ 38 }    Lint got confused
class UTILS_API IString
{
/*******************************************************************************
* Objects of the IString class are arrays of characters.                  *
*                                                                              *
* IStrings provide the following function beyond that available from the       *
* standard C char* arrays and the string.h library functions:                  *
*   o  No restrictions on string contents.  In other words, strings can        *
*      contain null characters.                                                *
*   o  Automatic conversion from and to numeric types                          *
*   o  Automatic deletion of string buffer when the IString is destroyed       *
*   o  Full support for the following:                                         *
*         - All comparison operators                                           *
*         - All bitwise operators                                              *
*         - Concatenation using the more natural "+" operator.                 *
*   o  String data testing, such as for characters, digits, and hexadecimal    *
*      digits                                                                  *
*   o  A full complement of the following:                                     *
*         - String manipulation functions, such as center, left and right      *
*           justification, stripping of leading and trailing characters,       *
*           substring deletion, and insertion of strings                       *
*         - Corresponding string manipulation functions that return a new       *
*           IString rather than modifying the receiver                         *
*         - String searching functions, such as index of string and last       *
*           index of string.                                                   *
*   o  Word manipulation, such as index of word and search for word phrase     *
*   o  Support for mixed strings that contain both single-byte character set   *
*      (SBCS) and double-byte character set (DBCS) characters.                 *
*                                                                              *
*      When a program using IStrings is run on OS/2J, the IString objects      *
*      support DBCS characters within the string contents.  The various        *
*      IString searching functions will not accidentally match an SBCS         *
*      character with the second byte of a DBCS character that happens to      *
*      have the same value.  Also, IString functions that modify IStrings,     *
*      such as subString, remove, and translate, will never separate two       *
*      bytes of a DBCS character.  If one of the two bytes of a DBCS           *
*      character is removed, the remaining byte will be replaced with the      *
*      appropriate pad character (if the function performing the change has    *
*      one), or a blank.                                                       *
*                                                                              *
*      Care must be taken with IStrings that contain DBCS data to ensure that  *
*      the contents are not altered in such a way as to corrupt the data.      *
*      For example, the statement:                                             *
*                                                                              *
*           aString[ n ] = 'x';                                                *
*                                                                              *
*      would be in error if the nth character of the IString was the first     *
*      or second byte of a DBCS character.                                     *
*******************************************************************************/
public:

	friend class IStringBuffer;

    typedef _token_iterator<_TCHAR> token_iterator;

	token_iterator &tokens(const _TCHAR *);
	//token_iterator &tokens(const char *);
	token_iterator &last_token();

/*------------------------- Constructors/Destructor ----------------------------
| You can construct instances of this class in the following ways:             |
|    - Construct a null string.                                                |
|    - Construct a string with the ASCII representation of a given numeric     |
|      value, supporting all flavors of int and double.                        |
|    - Construct a string with a copy of the given character data, supporting  |
|      ASCIIZ strings, characters, and IStrings.  The character data passed    |
|      will be converted to its ASCII representation.                          |
|    - Construct a string with contents that consist of copies of up to three  |
|      buffers of arbitrary data (void*).  Optionally, only the length need    |
|      be provided, in which case the IString contents are initialized to a    |
|      given pad character.  The default is a blank.                           |
------------------------------------------------------------------------------*/
    IString ( ) throw();
    IString ( char * ) throw();
    IString ( const char * ) throw();
    IString ( char *, size_t ) throw();
    IString ( const char *, size_t ) throw();
    IString ( const IString & ) throw();

    IString ( char ) throw();
    IString ( int ) throw();
    IString ( unsigned ) throw();
    IString ( long ) throw();
    IString ( unsigned long ) throw();
    IString ( short ) throw();
    IString ( wchar_t ) throw();
    IString ( double ) throw();
    IString (float floatVal) throw();
    IString (bool) throw();
	IString ( __int64 ) throw();
	IString ( unsigned __int64 ) throw();
    IString (const wchar_t *, size_t) throw();
    IString (wchar_t *) throw();
    IString (const wchar_t *) throw();
    IString (const _variant_t &) throw();
//  IString (BSTR);

//-----------------------------------------------------------------------------
// This method removed because of problems with the heap
//          IString (const std::string &);

    ~IString ( );

    IString &clear();
    
    IString & operator = ( const IString &s );
    
    /*---------------------------------------*/
    // Conversion methods
    IString &d2x();
    IString &c2x();

    IString substring(size_t offset) const;

/*------------------------- Queries ------------------------------------------*/
    inline size_t length            ( ) const throw() { return uStringLength; }
    inline size_t  lengthOf ( const char * ) const throw() { return uStringLength; };
    inline size_t  size ( ) const throw() { return uStringLength; };
    inline bool  empty ( ) const throw() { return uStringLength == 0; };

    inline const char * data ( ) const throw() { return myString; }

    IString  asString ( ) const;
    int  asInt ( ) const;
    unsigned long  asUnsigned ( ) const;
    double  asDouble ( ) const;
    bool  asBool ( ) const;
    operator const char * ( ) const throw();
    operator char * ( ) const throw();
    operator const wchar_t * ( ) const;
    operator wchar_t * ( ) const;


/*------------------------- Pattern Matching ---------------------------------*/
    bool
    //isLike            ( const IString &aPattern,
    //                    char           zeroOrMore = '*',
    //                    char           anyChar    = '?' ) const,
    //isLike            ( const char    *pPattern,
    //                    char           zeroOrMore = '*',
    //                    char           anyChar    = '?' ) const,

    isAbbreviationFor ( const IString &fullString,
                        size_t       minAbbrevLength = 0 ) const;
    bool
    isAbbreviationFor ( const char    *pFullString,
                        size_t       minAbbrevLength = 0 ) const;

    //includes          ( const IString     &aString ) const,
    //includes          ( const char        *pString ) const,
    //includes          ( char               aChar   ) const,
    //includes          ( const IStringTest &aTest   ) const;

    bool
    startsWith (const char *pString) const;
    bool
    startsWith (const wchar_t *pString) const;

    /*------------------------- Manipulation Operators -----------------------------
    | The following functions allow the string's contents to be manipulated.       |
    | All are overloaded so that standard C "strings" can be used efficiently,     |
    | without constructing an equivalent String first.                             |
    |   operator =  - Replaces the contents of the string.                         |
    |   operator ~  - Returns bitwise negation (one's complement).                 |
    |   operator +  - Concatenates two strings.                                    |
    |   operator += - Concatenates and replaces.                                   |
    |   operator &  - Performs bitwise AND.                                        |
    |   operator &= - Performs bitwise AND and replaces the receiver.              |
    |   operator |  - Performs bitwise OR.                                         |
    |   operator |= - Performs bitwise OR and replaces the receiver.               |
    |   operator ^  - Performs bitwise XOR.                                        |
    |   operator ^= - Performs bitwise XOR and replaces the receiver.              |
    ------------------------------------------------------------------------------*/
    IString
        operator ~  (                        ) const;
    IString
        operator +  ( const IString &aString ) const;
    IString
        operator +  ( const char    *pString ) const;
    IString
        &operator += ( const IString &aString );
    IString
        &operator += ( const char    *pString );
    IString
        &operator += ( const wchar_t *pwString );
    IString
        operator &  ( const IString &aString ) const;
    IString
        operator &  ( const char    *pString ) const;
    IString
        &operator &= ( const IString &aString );
    IString
        &operator &= ( const char    *pString );
    IString
        operator |  ( const IString &aString ) const;
    IString
        operator |  ( const char    *pString ) const;
    IString
        &operator |= ( const IString &aString );
    IString
        &operator |= ( const char    *pString );
    IString
        operator ^  ( const IString &aString ) const;
    IString
        operator ^  ( const char    *pString ) const;
    IString
        &operator ^= ( const IString &aString );
    IString
        &operator ^= ( const char    *pString );

    bool
        operator < ( const IString &aString ) const;
    bool
        operator < ( const char    *pString ) const;
    bool
        operator > ( const IString &aString ) const;
    bool
        operator > ( const char    *pString ) const;
    bool
        operator == ( const IString &aString ) const;
    bool
        operator == ( const char    *pString ) const;

//			const char& operator [] (unsigned index) const;
//			char &operator [] (unsigned index);

    IString
        &trim(char trimCharacter=0);

    IString

 		&leftJustify       ( unsigned length,
                      		char     padCharacter = ' ' );
    IString
        &lowerCase         ( );
    IString
 		&rightJustify      ( unsigned length,
                      		char     padCharacter = ' ' );
    IString
        &upperCase         ( );

    
    IString &format(LPCTSTR sFormatString,  // prntf fmt string
                    ...);                   // variable args


    inline IString & translate ( const IString &inputChars,
                                            const IString &outputChars,
                                            char           padCharacter = ' ')
        {
        return this->translate( inputChars.data(),
                                inputChars.length(),
                                outputChars.data(),
                                outputChars.length(),
                                padCharacter );
        }
    inline IString & translate ( const IString &inputChars,
                                            const char    *pOutputChars,
                                            char           padCharacter  = ' ')
        {
        return this->translate( inputChars.data(),
                                inputChars.length(),
                                pOutputChars,
                                (unsigned int)strlen( pOutputChars ),
                                padCharacter );
        }
    inline IString & translate ( const char    *pInputChars,
                                            const IString &outputChars,
                                            char           padCharacter = ' ' )
        {
        return this->translate( pInputChars,
                                lengthOf( pInputChars ),
                                outputChars.data(),
                                outputChars.length(),
                                padCharacter );
        }
    inline IString & translate ( const char *pInputChars,
                                            const char *pOutputChars,
                                            char        padCharacter)
        {
        return this->translate( pInputChars,
                                lengthOf( pInputChars ),
                                pOutputChars,
                                (unsigned int)strlen( pOutputChars ),
                                padCharacter );
        }
    inline IString translate ( const IString &aString,
                                const IString &inputChars,
                                const IString &outputChars,
                                char           padCharacter = ' '  ) const
        {
        return IString(aString).translate( inputChars, outputChars, padCharacter);
        }
    inline IString translate ( const IString &aString,
                                const IString &inputChars,
                                const char    *pOutputChars,
                                char           padCharacter = ' '  ) const
        {
        return IString(aString).translate( inputChars, pOutputChars, padCharacter);
        }
    inline IString translate ( const IString &aString,
                                const char    *pInputChars,
                                const IString &outputChars,
                                char           padCharacter = ' ' ) const
        {
        return IString(aString).translate( pInputChars, outputChars, padCharacter );
        }
    inline IString translate ( const IString &aString,
                                const char    *pInputChars,
                                const char    *pOutputChars,
                                char           padCharacter = ' ' ) const
        {
        return IString(aString).translate( pInputChars, pOutputChars, padCharacter );
        }

    friend IString
        UTILS_API operator +  ( const char *pString,
                    const IString &aString );

    friend IString
        UTILS_API operator +  ( const wchar_t *pString,
                    const IString &aString );

    //-------------------------------- Stream I/O ----------------------------------
    //The following functions provide for reading and writing IStrings from and
    //to standard C++ streams:
    //  operator << - Puts an IString's contents to an output stream.
    //  operator >> - Puts the next whitespace-delimited word from an input
    //                stream into an IString.
    //  lineFrom    - Static function that returns the next line from the
    //                argument input stream.  This function accepts an optional
    //                line delimiter, which defaults to \n; the resulting IString
    //                contains the characters up to the next occurrence of the
    //                delimiter.  The delimiter character is skipped.
    //----------------------------------------------------------------------------*/
//         friend ostream & 
//          operator << ( ostream       &aStream,
//                         const IString &aString );
//         friend ostream 
//          & operator << ( ostream       &aStream,
//                         const IString &aString );

//         friend istream
//          & operator >> ( istream &aStream,
//                         IString &aString );

//         static IString
//           lineFrom ( istream &aStream,
//                      char     delim = '\n' );

protected :

    void init( const char *, size_t ) throw();


    void init( const wchar_t *, size_t ) throw();


    inline void init( short sValue ) throw()
    {
        char ucValue[33];
        _itoa(sValue, ucValue, 10);
        init(ucValue, strlen(ucValue));
    }


    inline void init( long ulValue ) throw()
    {
        char ucValue[33];
        _ltoa(ulValue, ucValue, 10);
        init(ucValue, strlen(ucValue));
    }


    inline void init( unsigned long ulValue ) throw()
    {
        char ucValue[33];
        _ultoa(ulValue, ucValue, 10);
        init(ucValue, strlen(ucValue));
    }


#if !defined(_SCOT) && !defined(_SCOTSSF_)
    inline void init( _bstr_t bstrValue ) throw()
    {
        CW2T szValue( (const wchar_t *)bstrValue );
        init(szValue, _tcslen(szValue));
    }
#endif


    const char *contents() const throw()
        {return myString;};
    bool isLike( const _TCHAR *pPattern,
                    size_t    patternLen,
                    _TCHAR        zeroOrMore,
                    _TCHAR        anyChar ) const;

    bool isAbbrevFor( const char *pFullString,
                        size_t    fullLen,
                        size_t    minLen ) const;
    IString &translate( const char *pInputChars,
                size_t    inputLen,
                const char *pOutputChars,
                size_t     outputLen,
                char     padCharacter );

    bool isTrimCharacter(char pString, char cTrimChar) const throw();

    size_t getStrlen(const char *cString) const throw();

    size_t getWStrlen(const wchar_t *cString) const throw();

private :

    char * myString;
    mutable wchar_t * myWString;
    size_t uStringLength;

	token_iterator *m_pTokens;
	token_iterator *m_pEndToken;
}; // class IString


class UTILS_API IStringBuffer
{
public:
/*------------------------- Constructors/Destructor ----------------------------
| You can construct instances of this class in the following ways:             |
|    - Construct a null string.                                                |
|    - Construct a string with the ASCII representation of a given numeric     |
|      value, supporting all flavors of int and double.                        |
|    - Construct a string with a copy of the given character data, supporting  |
|      ASCIIZ strings, characters, and IStrings.  The character data passed    |
|      will be converted to its ASCII representation.                          |
|    - Construct a string with contents that consist of copies of up to three  |
|      buffers of arbitrary data (void*).  Optionally, only the length need    |
|      be provided, in which case the IString contents are initialized to a    |
|      given pad character.  The default is a blank.                           |
------------------------------------------------------------------------------*/
    IStringBuffer ( unsigned long ulBfrLen = 80 ) throw();
    IStringBuffer (const IStringBuffer &);
    ~IStringBuffer ( );
    
//  IStringBuffer & operator = ( const IStringBuffer &s );

//  IString substring(size_t offset) const;

/*------------------------- Queries ------------------------------------------*/
    inline size_t  size ( ) const throw() { return m_ulStringLength; };
    inline size_t  capacity ( ) const throw() { return m_ulBfrLength; };

    IStringBuffer &clear() throw();

    operator const char * ( ) const throw();
#if !defined(_SCOT) && !defined(_SCOTSSF_)
    operator const wchar_t * ( ) const throw(...);
#endif


    /*------------------------- Manipulation Operators -----------------------------
    | The following functions allow the string's contents to be manipulated.       |
    | All are overloaded so that standard C "strings" can be used efficiently,     |
    | without constructing an equivalent String first.                             |
    |   operator =  - Replaces the contents of the string.                         |
    |   operator +  - Concatenates two strings.                                    |
    |   operator += - Concatenates and replaces.                                   |
    ------------------------------------------------------------------------------*/
    
    IStringBuffer & operator = ( const IStringBuffer &s );
    IStringBuffer & operator = ( const char * );
    IStringBuffer & operator = ( const wchar_t * );

    //IString
    //    operator +  ( const IString &aString ) const;
    //IStringBuffer
    //    operator +  ( const char    *pString ) const;
    IStringBuffer
        &operator += ( const IStringBuffer &aString );
    IStringBuffer
        &operator += ( const IString &aString );
    IStringBuffer
        &operator += ( const char *pString );
    IStringBuffer
        &operator += ( const wchar_t *pString );
    IStringBuffer
        &operator += ( char cChar );
    IStringBuffer
        &operator += ( wchar_t cChar );
    IStringBuffer
        &operator += ( long cChar );
    IStringBuffer
        &operator += ( unsigned long cChar );
    //bool
    //    operator == ( const IString &aString ) const;
    //bool
    //    operator == ( const _TCHAR    *pString ) const;

	//const char& operator [] (unsigned index) const;
	//char &operator [] (unsigned index);

    //friend IString
    //    UTILS_API operator +  ( const char *pString,
    //                const IString &aString );

    //friend IString
    //    UTILS_API operator +  ( const wchar_t *pString,
    //                const IString &aString );

    //-------------------------------- Stream I/O ----------------------------------
    //The following functions provide for reading and writing IStrings from and
    //to standard C++ streams:
    //  operator << - Puts an IString's contents to an output stream.
    //  operator >> - Puts the next whitespace-delimited word from an input
    //                stream into an IString.
    //  lineFrom    - Static function that returns the next line from the
    //                argument input stream.  This function accepts an optional
    //                line delimiter, which defaults to \n; the resulting IString
    //                contains the characters up to the next occurrence of the
    //                delimiter.  The delimiter character is skipped.
    //----------------------------------------------------------------------------*/
//         friend ostream & 
//          operator << ( ostream       &aStream,
//                         const IString &aString );
//         friend ostream 
//          & operator << ( ostream       &aStream,
//                         const IString &aString );

//         friend istream
//          & operator >> ( istream &aStream,
//                         IString &aString );

//         static IString
//           lineFrom ( istream &aStream,
//                      char     delim = '\n' );

protected :

    void ensureBuffer(size_t, bool bBoth = false);

    IStringBuffer &append(const char *, size_t);

    IStringBuffer &append(const wchar_t *, size_t);

private :

    char * myString;
    mutable wchar_t * myWString;
    size_t m_ulStringLength;
    size_t m_ulBfrLength;

}; // class IStringBuffer

#endif