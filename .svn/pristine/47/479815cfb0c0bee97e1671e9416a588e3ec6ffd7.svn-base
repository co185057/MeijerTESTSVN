#pragma once
// $Header: /Fastlane/4.2/Development/Core/Source/Include/KVParmList.h 19    10/02/08 2:43p Jj100011 $
/*
 * @(#)KeyValueParmList.h    $Revision: 19 $ $Date: 10/02/08 2:43p $
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
/*
* $Log: /Fastlane/4.2/Development/Core/Source/Include/KVParmList.h $
 * 
 * 19    10/02/08 2:43p Jj100011
 * 374157
 * 
 * 18    9/29/08 10:24a Tn185011
 * SSF linking issue
 * 
 * 17    9/10/08 5:17p Jj100011
 * SSF Linking issue
 * 
 * 16    9/04/08 4:58p Jj100011
 * Linking error with  CKeyValueParmList in SSF.
 * 
 * 1     8/21/08 2:32p Jj100011
 * scotapp source at build 305 level
 * 
 * 15    11/27/07 5:38p Lp185019
 * SR629 (TAR 334800) - Resolve CISP Issue: RCM is tracing password by
 * default
 * 
 * 14    6/28/05 5:44p Dm185016
 * TAR 304xxx - Problem with commas in DESCRIPTION=.
 * 
 * 13    5/31/05 1:26p Dm185016
 * Added encoding functionality.
 * 
 * 11    5/03/05 4:09p Dm185016
 * Added comments.
 * 
 * 10    4/25/05 7:05p Dm185016
 * TAR 299068
 * 
 * 8     4/05/05 1:53p Dm185016
 * Merged in fix for TAR 281347.
 * 
 * 7     3/31/05 2:26p Dm185016
 * TAR 297138
 * 
 * 6     3/24/05 7:30p Dm185016
 * UNICODE
 * 
 * 5     3/11/05 11:22a Dm185016
 * TAR 294050.
 * 
 * 4     3/01/05 10:15a Dm185016
 * Fix for VS6 incompatibility.
 * 
 * 3     2/28/05 3:30p Dm185016
 * lint
 * 
 * 2     2/15/05 11:42a Dm185016
 * TAR 290931.
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 19    1/18/05 4:00p Dm185016
* UNICODE support
* 
* 18    11/30/04 4:34p Dm185016
* Added check to prevent crash in the event the parameter is null
* pointer.
* 
* 17    11/29/04 7:12p Dm185016
* EscapeText made static.
* Added line feed escaping.
* 
* 16    11/13/04 1:11p Bd123405
* Change atoi definition to ttoi for unicode
* 
* 15    11/11/04 7:32p Db185023
* Merge new RAP into 3.2
* 
* 14    11/02/04 3:21p Dm185016
* Added &comma; entity
* 
* 13    10/14/04 1:51p Dm185016
* Added entities to allow embedded control characters:
* 
* ';'  ==  &semi;
* '&' == &amp;
* '=' == &eq;
* 
* 12    10/04/04 6:59p Dm185016
* 
* 10    6/10/04 2:24p Dm185016
* TAR 271089 - Added Clear() method to _kv_iterator.
* 
* 9     5/06/04 8:07a Tk124825
* changed from _SCOTSSF to _SCOTSSF_
* 
* 8     5/04/04 6:10p Tk124825
* made the necessary changes for unicode build
* 
* 7     4/13/04 11:30a Dm185016
* Fixed Unicode problems
* 
* 6     3/31/04 10:03a Dm185016
* Added empty() and Append() methods
* 
* 5     3/19/04 3:31p Dm185016
* Memory leak fix
* 
* 4     3/01/04 10:19a Dm185016
* Corrected record parsing
* 
* 3     2/24/04 3:43p Dm185016
* Integration into SCOT required some changes
* 
* 2     2/18/04 9:15a Dm185016
* Rewritten and combined with KeyValueParser to use std:: like iterators
*/

//lint --e{578, 1536}
//lint --e{1759}        By design, the postfix increment returns a reference

#include <vector>
#include <iterator>
#include <map>
#include "Utils.h"
#include "xerror.h"
#include "StringComparator.h"

#pragma warning( push )
#pragma warning( disable : 4251 )

#if ( _MFC_VER < 0x700 )
    #ifndef _tstof
        #define _tstof      _ttof
    #endif
    #ifndef _tstol
        #define _tstol      _ttol
    #endif
    #ifndef _tstoi
        #define _tstoi      _ttoi
    #endif
#endif


#if defined(_SCOT) || defined(_SCOTSSF_)
    #undef UTILS_API
	#define UTILS_API 
#endif

#if defined(_SCOTSSF_)
	#ifndef StringType
		#include <string>
	#ifdef _UNICODE
		#define StringType std::wstring
	#else
		#define StringType std::string
	#endif
#endif

#endif

#define ENTITY_LINEFEED _T("lf")
#define ENTITY_COMMA    _T("comma")
#define ENTITY_SEMI     _T("semi")
#define ENTITY_EQ       _T("eq")
#define ENTITY_AMP      _T("amp")
#define NULL_STRING     _T("")

#define TEXT_ENCODING       _T("text")
#define MASKED_ENCODING     _T("masked")
class CKeyValueParmList;
class IException;

#ifdef _SCOTSSF_
class DLLIMPORT
#elif  defined _SCOT
class DLLEXPORT 
#else
class
#endif
UTILS_NOVTABLE IOutputEncoding
{
public:
    virtual StringType ToString(const StringType &)=0;
};

#ifdef _SCOTSSF_
class DLLIMPORT
#elif  defined _SCOT
class DLLEXPORT 
#else
class
#endif
CTextEncoder : public IOutputEncoding
{
public:
    virtual StringType ToString(const StringType &sText);
};

#ifdef _SCOTSSF_
class DLLIMPORT
#elif  defined _SCOT
class DLLEXPORT 
#else
class
#endif
CMaskedEncoder : public IOutputEncoding
{
public:	
    virtual StringType ToString(const StringType &sText);
};

/// Internal Use Only class.  Not to be used by client code
#ifdef _SCOTSSF_
class DLLIMPORT
#elif  defined _SCOT
class DLLEXPORT 
#else
class
#endif
UTILS_API _kv_parser
{
    enum States
    {
        DONE = -1
    };


    enum CCharTypes 
    {
        Alpha   = 1,
        Number,
        Delimeter,
        Escape,
        ValueSeparator,
        Separator,
        RecordSeparator,
        String,
        Ampersand,
        Other,
        eof
    };

    typedef enum CCharTypes CValueTypes;

    typedef struct
    {
        CCharTypes  eValueType;
        StringType  sValue;
        StringType  sEncoding;
    } CValue;
    typedef std::vector<CValue> CValueVector;
    typedef CValueVector::iterator CValueIterator;
    typedef std::map<StringType, IOutputEncoding *, StringComparator> CEncoderMap;

public:

//  friend class iterator;

    _kv_parser(const _TCHAR *sSource,
                    _TCHAR cDelim='=',
                    _TCHAR cValueSeparator=',',
                    _TCHAR cSeparator=';',
                    _TCHAR cRecordSeparator=0x0a);

    _kv_parser(
                    _TCHAR cDelim='=',
                    _TCHAR cValueSeparator=',',
                    _TCHAR cSeparator=';',
                    _TCHAR cRecordSeparator=0x0a);

    virtual ~_kv_parser();

    void SetSource(const _TCHAR *sSource);
    void SetDelim(_TCHAR cDelim);
    void SetValueSeparator(_TCHAR cValueSeparator);
    void SetKeySeparator(_TCHAR cSeparator);
    void SetRecordSeparator(_TCHAR cRecordSeparator);

    int GetCurrentOffset() { return (int)(m_psCurrent - m_psSource); }
// Methods in this section deal with parsing a KV parm list
public:

    const _TCHAR *Key() const;
    const _TCHAR *Value(int nIndex=0) const;
    const _TCHAR *Encoding(int nIndex=0) const;
    int GetValueCount() const;
    CCharTypes GetValueType(int nIndex=0) const;

    bool NextRecord();

protected:

    void Next();

    _TCHAR * m_psCurrent;

    bool m_bNewRecordDetected;
    static CEncoderMap m_encoders;

private:

    void State1(_TCHAR c, int nCharType);
    void State2(_TCHAR c, int nCharType);
    void State3(_TCHAR c, int nCharType);
    void State4(_TCHAR c, int nCharType);
    void State5(_TCHAR c, int nCharType);
    void State6(_TCHAR c, int nCharType);
    void State7(_TCHAR c, int nCharType);
    void State8(_TCHAR c, int nCharType);
    void State9(_TCHAR c, int nCharType);
    void State10(_TCHAR c, int nCharType);
    void State11(_TCHAR c, int nCharType);
    void State12(_TCHAR c, int nCharType);
    void State13(_TCHAR c, int nCharType);
    void State14(_TCHAR c, int nCharType);

    void ClearValues() throw();
    void Init();
    void SaveKey();
    void SaveValue();
    bool SaveEntity();
    void SaveNumericEntity();
    void SaveEncoding();
    static void InitEncoders();

    unsigned  GetTranslatedValue(unsigned c) const;
    
    const _TCHAR * m_psSource;
    size_t m_nSrcLength;

    StringType m_sValue;
    StringType m_sTemp;
    StringType m_sString;
    StringType m_sEntity;
    StringType m_sEncoding;

    _TCHAR * m_psBegin;
    _TCHAR * m_psEnd;
	_TCHAR m_cDelim;
	_TCHAR m_cSeparator;
    _TCHAR m_cValueSeparator;
    _TCHAR m_cRecordSeparator;

    unsigned m_nCharType;
    StringType m_sKey;
    CCharTypes m_eCurrentValueType;
    CValueVector m_vValues;

    unsigned char m_translation[256];

    int m_nState;

    bool m_bError;

    static unsigned char defaultTranslation[256];

};


// the iterator
#ifdef _SCOTSSF_
template <typename _Tp> class DLLIMPORT
#elif defined _SCOT
template <typename _Tp> class DLLEXPORT
#else
template <typename _Tp> class 
#endif
_kv_iterator 
    : public std::iterator<std::forward_iterator_tag, _Tp>
    , public _kv_parser
{
    friend class CKeyValueParmList;

public:

    typedef std::pair<_Tp, _Tp> _pair_t;
    

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
    

    /// <summary>Postfix operator ++ is used to get the next key/value pair.  The
    /// int parameter is not used.  It is just a placeholder to force the postfix</summary>
    inline _kv_iterator<_Tp> & operator ++(int)
    {
        if (m_bNewRecordDetected)
        {
            m_pszStart = NULL;
        }
        else
        {
            Next();

            if (Key() == NULL)
                m_pszStart = NULL;
            else
                m_pszStart = m_psCurrent;
        }

        m_nValueIndex   = 0;

        return *this;
    }
    

    /// <summary>Prefix operator ++ is used to get the next value using 
    /// the same key as the previous usage</summary>
    inline _kv_iterator<_Tp> & operator ++()
    {
        m_nValueIndex++;

        return *this;
    }


    /// <summary>Operator != returns true if the two objects are not equal</summary>
    inline bool operator!=(const _kv_iterator<_Tp> & rhs) const
    {
        return (m_pszStart != rhs.m_pszStart);
    }
        

    /// <summary>Advances the iterator to the beginning of the next record.</summary>
    inline bool NextRecord()
    {
        m_pszStart = m_psCurrent;

        bool retValue = _kv_parser::NextRecord();

        if (retValue)
        {
            Next();

            if (Key() == NULL)
                m_pszStart = NULL;
            else
                m_pszStart = m_psCurrent;
        }

        return retValue;
    }

    /// <summary>Clears the parser internal variables so that 
    /// it is ready for the next record.</summary>
    inline void Clear()
    {
        m_bNewRecordDetected = false;
        m_pszStart = NULL;
    }


    /**
    * Converts the current KeyValue pair to a string using the encoding specified for this
    * KeyValue pair.  FOR DEBUG PURPOSES ONLY.
    * @return Returns the current KeyValue pair referenced by this iterator to a string using the specified 
    * encoding
    */
    inline StringType ToString()
    {
        StringType sRetValue;

        CEncoderMap::iterator iter = m_encoders.find(Encoding());
        _ASSERTE(iter != m_encoders.end());
        
        IOutputEncoding &encoding = *iter->second;

        sRetValue = Key();
        if (Encoding() != NULL)
        {
            sRetValue += _T("[");
            sRetValue += Encoding();
            sRetValue += _T("]");
        }
        sRetValue += _T("=");
        sRetValue += encoding.ToString(Value(0));
        for (int i=1; i<GetValueCount(); i++)
        {
            sRetValue += _T(",");
            sRetValue += encoding.ToString(Value(i));
        }

        return sRetValue;
    }

	//+SR629 TAR 334800
	inline StringType GetCurrentEncoding()
	{
		return m_sCurrentEncoding;
	}
	//-SR629 TAR 334800

protected:            
    

    inline void UpdatePair()
    {
        m_KVPair.first = Key();
        m_KVPair.second = Value(m_nValueIndex);
        const _TCHAR *pcszEncoding = Encoding(m_nValueIndex);
        m_sCurrentEncoding  = (pcszEncoding == NULL) ? TEXT_ENCODING : pcszEncoding;
    }
    
private:
    
    _kv_iterator();

    // Constructor
    _kv_iterator(_Tp pszStart
               , _TCHAR cDelim='='
               , _TCHAR cValueSeparator=','
               , _TCHAR cSeparator=';'
               , _TCHAR cRecordSeparator=0x0a)
        : _kv_parser(pszStart, cDelim, cValueSeparator, cSeparator, cRecordSeparator)
        , m_pszStart(pszStart)
        , m_nLength(m_pszStart == NULL ? 0 : _tcslen(m_pszStart))
        , m_nValueIndex(0)
    {
    }
    
    _pair_t m_KVPair;
    StringType m_sCurrentEncoding;
    _Tp m_pszStart;
    size_t m_nLength;

    int m_nValueIndex;
};


/// <summary>Instances of this class contain a series of Key/Value pairs</summary>
#ifdef _SCOTSSF_
class DLLIMPORT
#elif  defined _SCOT
class DLLEXPORT 
#else
class
#endif
UTILS_API CKeyValueParmList
{

public:

    friend class _kv_iterator<const _TCHAR *>;

    typedef _kv_iterator<const _TCHAR *> iterator;

//  friend class iterator;

    /// <summary>Constructor taking a character string to be used as the source for 
    /// the key/value pairs</summary>
    /// <param name="pszSource">Source string containing a series of Key/Value pairs
    /// of the form key=value</param>
    /// <param name="cDelim">Character used as the delimeter between the key and its value</param>
    /// <param name="cValueSeparator">Character used to separate individual values.  By default
    /// this is ','.</param>
    /// <param name="cSeparator">Character used to separate Key/Value pairs.  By default
    /// this is ';'.</param>
    /// <param name="cRecordSeparator">Character used to separate records of Key/Value pairs.
    ///   By default this is \n (new line character 0x0a).</param>
    CKeyValueParmList(const _TCHAR *pszSource,
                    _TCHAR cDelim='=',
                    _TCHAR cValueSeparator=',',
                    _TCHAR cSeparator=';',
                    _TCHAR cRecordSeparator=0x0a);

    /// <summary>Default constructor taking only overrides for the default characters</summary>
    /// <param name="cDelim">Character used as the delimeter between the key and its value</param>
    /// <param name="cValueSeparator">Character used to separate individual values.  By default
    /// this is ','.</param>
    /// <param name="cSeparator">Character used to separate Key/Value pairs.  By default
    /// this is ';'.</param>
    /// <param name="cRecordSeparator">Character used to separate records of Key/Value pairs.
    ///   By default this is \n (new line character 0x0a).</param>
    CKeyValueParmList(
                    _TCHAR cDelim='=',
                    _TCHAR cValueSeparator=',',
                    _TCHAR cSeparator=';',
                    _TCHAR cRecordSeparator=0x0a);

    /// <summary>Destructor required to release any resources</summary>
    ~CKeyValueParmList();

    /// <summary>Set the source for the list to the given value</summary>
    /// <param name="pszSource">Source string containing a series of Key/Value pairs
    /// of the form key=value</param>
    void SetSource(const _TCHAR *sSource);

    /// <summary>Set the delimeter between a key and its value(s) to the given character</summary>
    /// <param name="cDelim">Character used as the delimeter between the key and its value</param>
    void SetDelim(_TCHAR cDelim);

    /// <summary>Set the delimeter between value(s) to the given character</summary>
    /// <param name="cValueSeparator">Character used to separate individual values.  By default
    /// this is ','.</param>
    void SetValueSeparator(_TCHAR cValueSeparator = 0);

    /// <summary>Set the delimeter between key/value pairs to the given character</summary>
    /// <param name="cSeparator">Character used to separate Key/Value pairs.  By default
    /// this is ';'.</param>
    void SetKeySeparator(_TCHAR cSeparator);

    /// <summary>Set the delimeter between records to the given character</summary>
    /// <param name="cRecordSeparator">Character used to separate records of Key/Value pairs.
    ///   By default this is \n (new line character 0x0a).</param>
    void SetRecordSeparator(_TCHAR cRecordSeparator= 0);

    StringType ToString();
// Methods in this section deal with parsing a KV parm list
public:

    /// <summary>Obtains an iterator used to step over the key/value pairs in the source string</summary>
    /// <return>Returns an iterator</return>
    iterator begin();

    /// <summary>Obtains an iterator that points past the last key/value pair in the source string</summary>
    /// <return>Returns an iterator</return>
    iterator &end() { return m_iterEnd; }

    /// <summary>Returns true if there are no key/value pairs in the source string</summary>
    /// <return>Returns true if there are no key/value pairs in the source string</return>
    bool empty() const;

// All methods in this section deal with constructing a KV parm list
public:
    
    /// <summary>Adds a new key/value pair to the collection</summary>
    /// <param name="key">Name of the key</param>
    /// <param name="nValue">Converts the given integer to string and sets that as the value</param>
    void NewKVPair(const _TCHAR *key, int nValue, const StringType &sEncoding = TEXT_ENCODING);
    
    /// <summary>Adds a new key/value pair to the collection</summary>
    /// <param name="key">Name of the key</param>
    /// <param name="pszValue">Sets the string as the value</param>
    void NewKVPair(const _TCHAR *key, const _TCHAR *pszValue, const _TCHAR * pcszEncoding = NULL);
    
    /// <summary>Adds a new key/value pair to the collection</summary>
    /// <param name="key">Name of the key</param>
    /// <param name="value">Sets the string as the value</param>
    //void NewKVPair(const StringType &key, const StringType &value, const StringType & sEncoding = TEXT_ENCODING);
    
    /// <summary>Adds another value to the current key in the collection</summary>
    /// <param name="value">Adds the string to the value</param>
    void NewValue(const StringType &value);
    
    /// <summary>Adds another value to the current key in the collection</summary>
    /// <param name="value">Adds the string to the value</param>
    void NewValue(const _TCHAR *value);

    /// <summary>Adds a record separator to the collection indicating the start of the next record</summary>
    /// <param name="pRecord">If specified, then this string is added as the beginning of the new record</param>
    void NewRecord(const _TCHAR *pRecord=NULL);


    /// <summary>Appends the key/value pairs in the given collection to this collection</summary>
    /// <param name="kvlist">The key/value collection to be added to this collection</param>
    void Append(const CKeyValueParmList &kvlist);

    /// <summary>Clears out all key/value pairs in the collection</summary>
    void Clear();

    /// <summary>Returns a pointer to the collection as a string</summary>
    operator const _TCHAR * ( ) const;

    /// <summary>Returns the length of the collection string</summary>
    size_t size( ) const;

    /// <summary>Copy Constructor</summary>
    /// <param name="theOther">The key/value collection to be copied to build this collection</param>
    CKeyValueParmList(const CKeyValueParmList &theOther);

    /// <summary>Converts the text of an existing key/value list by escaping any special characters used, such as
    /// the delimeters specified in the constructors.</summary>
    static StringType EscapeText(const StringType &text);

    /// <summary>Converts only the value of an existing key/value list by escaping any special characters used, such as
    /// the delimeters specified in the constructors.</summary>
    static StringType EscapeValue(const StringType &text);

    /// <summary>Converts only the value separator character (',') to an entity.</summary>
    static StringType EscapeValueSeparator(const StringType &text);

private:
    
    CKeyValueParmList& operator=( const CKeyValueParmList& );

    const _TCHAR * m_psSource;
    size_t m_nSrcLength;

    StringType m_sValue;

    _TCHAR m_cDelim;
	_TCHAR m_cSeparator;
    _TCHAR m_cValueSeparator;
    _TCHAR m_cRecordSeparator;

    bool m_bNeedSeparator;

    iterator m_iterEnd;
};


//typedef StringType (*PENCODER)(const _TCHAR *, size_t length);
//typedef StringType (*PDECODER)(const _TCHAR *, size_t length);

#ifdef _SCOTSSF_
class DLLIMPORT
#elif  defined _SCOT
class DLLEXPORT 
#else
class
#endif
base64Codec
{
public:
    static StringType encode(const _TCHAR *, size_t length);
    static StringType decode(const char *, size_t length);
} ;

#ifdef _SCOTSSF_
class DLLIMPORT
#elif  defined _SCOT
class DLLEXPORT 
#else
class
#endif 
hexCodec
{
public:
    static StringType encode(const _TCHAR *, size_t length);
    static StringType decode(const _TCHAR *, size_t length);
};


struct noCodec {};


// Accessor template - used to return the correct type for a KeyValue parameter.
#ifdef _SCOTSSF_
 template<class T, typename codec = noCodec>class DLLIMPORT
#elif defined _SCOT
template<class T, typename codec = noCodec> class DLLEXPORT
#else
template<class T, typename codec = noCodec> class 
#endif 
CKeyValue
{
public:

    static T Get(CKeyValueParmList::iterator &, T );
    static void Put( CKeyValueParmList &, T );
    static void Put( CKeyValueParmList &, const _TCHAR *, T, const _TCHAR *pcszEncoding = TEXT_ENCODING);

};
#pragma warning( pop )


//-----------------------------------------------------------------------------
// Specialization of Get() for bool property
template <>
inline bool CKeyValue<bool, noCodec>::Get(CKeyValueParmList::iterator &iter
                               , bool bDefault)
{
    const _TCHAR *pszValue = (*iter).second;
    if (pszValue == NULL)
        return bDefault;

    StringType sResult = pszValue;

    return (_tcsicmp(sResult.c_str(), _T("true")) == 0);
}
template <>
inline void CKeyValue<bool, noCodec>::Put(CKeyValueParmList &kvlist
                               , const _TCHAR *pszValueName
                               , bool bValue
                               , const _TCHAR *pcszEncoding)
{
    StringType sResult = (bValue ? _T("true") : _T("false"));
    
    kvlist.NewKVPair(pszValueName, sResult.c_str(), pcszEncoding);
}
template <>
inline void CKeyValue<bool, noCodec>::Put(CKeyValueParmList &kvlist
                               , bool bValue)
{
    StringType sResult = (bValue ? _T("true") : _T("false"));
    
    kvlist.NewValue(sResult.c_str());
}


//-----------------------------------------------------------------------------
// Specialization of Get() for long property
template <>
inline long CKeyValue<long, noCodec>::Get(CKeyValueParmList::iterator &iter
                               , long lDefault)
{
	//374157
	if ( (*iter).second == NULL )
	{
		return lDefault;
	}
    StringType sValue = (*iter).second;
    if (sValue.empty())
        return lDefault;

    return _tstol(sValue.c_str());
}
template <>
inline void CKeyValue<long, noCodec>::Put(CKeyValueParmList &kvlist
                               , const _TCHAR *pszValueName
                               , long lValue
                               , const _TCHAR *pcszEncoding)
{
    _TCHAR buffer[30];
    _ltot(lValue, buffer, 10);
    
    kvlist.NewKVPair(pszValueName, buffer, pcszEncoding);
}
template <>
inline void CKeyValue<long, noCodec>::Put(CKeyValueParmList &kvlist
                               , long lValue)
{
    _TCHAR buffer[30];
    _ltot(lValue, buffer, 10);
    
    kvlist.NewValue(buffer);
}


//-----------------------------------------------------------------------------
// Specialization of Get() for int property
template <>
inline int CKeyValue<int, noCodec>::Get(CKeyValueParmList::iterator &iter
                             , int nDefault)
{
	//374157
	if ( (*iter).second == NULL )
	{
		return nDefault;
	}

    StringType sValue = (*iter).second;
    if (sValue.empty())
        return nDefault;

    return _tstoi(sValue.c_str());
}
template <>
inline void CKeyValue<int, noCodec>::Put(CKeyValueParmList &kvlist
                              , const _TCHAR *pszValueName
                              , int lValue
                              , const _TCHAR *pcszEncoding)
{
    _TCHAR buffer[30];
    _itot(lValue, buffer, 10);

    kvlist.NewKVPair(pszValueName, buffer, pcszEncoding);
}
template <>
inline void CKeyValue<int, noCodec>::Put(CKeyValueParmList &kvlist
                              , int lValue)
{
    _TCHAR buffer[30];
    _itot(lValue, buffer, 10);

    kvlist.NewValue(buffer);
}


//-----------------------------------------------------------------------------
// Specialization of Get() for const _TCHAR * property
static StringType sValue;
template <>
inline const _TCHAR * CKeyValue<const _TCHAR *, noCodec>::Get(CKeyValueParmList::iterator &iter
                                                   , const _TCHAR *pszDefault)
{
    const _TCHAR *pszDefaultValue = ((pszDefault == NULL) ? NULL_STRING : pszDefault);

    const _TCHAR *pszValue = (*iter).second;
    if ((pszValue == NULL) || (pszValue[0] == 0))
        return pszDefaultValue;

    return pszValue;
}
template <>
inline void CKeyValue<const _TCHAR *, noCodec>::Put(CKeyValueParmList &kvlist
                                         , const _TCHAR *pszValueName
                                         , const _TCHAR * pszValue
                                         , const _TCHAR *pcszEncoding)
{
    StringType sValue;
    if (pszValue != NULL)
        sValue = CKeyValueParmList::EscapeText(pszValue);

    kvlist.NewKVPair(pszValueName, sValue.c_str(), pcszEncoding);
}
template <>
inline void CKeyValue<const _TCHAR *, noCodec>::Put(CKeyValueParmList &kvlist
                                         , const _TCHAR * pszValue)
{
    StringType sValue;
    if (pszValue != NULL)
        sValue = CKeyValueParmList::EscapeText(pszValue);

    kvlist.NewValue(sValue);
}


//-----------------------------------------------------------------------------
// Specialization of Get() for unsigned property
template <>
inline unsigned CKeyValue<unsigned, noCodec>::Get(CKeyValueParmList::iterator &iter
                                       , unsigned ulDefault)
{
	//374157
	if ( (*iter).second == NULL )
	{
		return ulDefault;
	}
    StringType sValue = (*iter).second;
    if (sValue.empty())
        return ulDefault;

    return _tcstoul(sValue.c_str(), NULL, 10);
}
template <>
inline void CKeyValue<unsigned, noCodec>::Put(CKeyValueParmList &kvlist
                                   , const _TCHAR *pszValueName
                                   , unsigned ulValue
                                   , const _TCHAR *pcszEncoding)
{
    _TCHAR buffer[30];
    _ultot(ulValue, buffer, 10);

    kvlist.NewKVPair(pszValueName, buffer, pcszEncoding);
}
template <>
inline void CKeyValue<unsigned, noCodec>::Put(CKeyValueParmList &kvlist
                                   , unsigned ulValue)
{
    _TCHAR buffer[30];
    _ultot(ulValue, buffer, 10);

    kvlist.NewValue(buffer);
}

#if !defined(_SCOT) && !defined(_SCOTSSF_)
//-----------------------------------------------------------------------------
// Specialization of Get() for float property
template <>
inline float CKeyValue<float, noCodec>::Get(CKeyValueParmList::iterator &iter
                                 , float fDefault)
{
	//374157
	if ( (*iter).second == NULL )
	{
		return fDefault;
	}
    StringType sValue = (*iter).second;
    if (sValue.empty())
        return fDefault;

    return (float)_tstof(sValue.c_str());
}
/*
template <>
inline void CKeyValue<float, noCodec>::Put(CKeyValueParmList &kvlist
                                , const _TCHAR *pszValueName
                                , float fValue
                                , const _TCHAR *pcszEncoding)
{
    _TCHAR buffer[30];
    double  dValue = (double)fValue;
	if (-1 == _sntprintf_s(buffer, sizeof(buffer), _TRUNCATE, _T("%e"), dValue))
        ITHROWNEW(IException, _T(__FUNCTION__) _T(".  Buffer too short."))

    kvlist.NewKVPair(pszValueName, buffer, pcszEncoding);
}
template <>
inline void CKeyValue<float, noCodec>::Put(CKeyValueParmList &kvlist
                                , float fValue)
{
    _TCHAR buffer[30];
    double  dValue = (double)fValue;
	if (-1 == _sntprintf_s(buffer, sizeof(buffer), _TRUNCATE, _T("%e"), dValue))
        ITHROWNEW(IException, _T(__FUNCTION__) _T(".  Buffer too short."))

    kvlist.NewValue(buffer);
}

//-----------------------------------------------------------------------------
// Specialization of Get() for double property
template <>
inline double CKeyValue<double, noCodec>::Get(CKeyValueParmList::iterator &iter
                                   , double dDefault)
{
    StringType sValue = (*iter).second;
    if (sValue.empty())
        return dDefault;

    return _tstof(sValue.c_str());
}
template <>
inline void CKeyValue<double, noCodec>::Put(CKeyValueParmList &kvlist
                                 , const _TCHAR *pszValueName
                                 , double dValue
                                 , const _TCHAR *pcszEncoding)
{
    _TCHAR buffer[30];
	if (-1 == _sntprintf_s(buffer, sizeof(buffer), _TRUNCATE, _T("%e"), dValue))
        ITHROWNEW(IException, _T(__FUNCTION__) _T(".  Buffer too short."))

    kvlist.NewKVPair(pszValueName, buffer, pcszEncoding);
}
template <>
inline void CKeyValue<double, noCodec>::Put(CKeyValueParmList &kvlist
                                 , double dValue)
{
    _TCHAR buffer[30];
	if (-1 == _sntprintf_s(buffer, sizeof(buffer), _TRUNCATE, _T("%e"), dValue))
        ITHROWNEW(IException, _T(__FUNCTION__) _T(".  Buffer too short."))

    kvlist.NewValue(buffer);
}

*/
//-----------------------------------------------------------------------------
// Partial Specialization of Get() for bool property
#ifdef _SCOTSSF_
template<typename codec> class DLLIMPORT
#elif defined _SCOT
template<typename codec> class DLLEXPORT
#else
template<typename codec> class 
#endif
CKeyValue<bool, codec>
{
public:

    static bool Get(CKeyValueParmList::iterator &, bool );
    static void Put( CKeyValueParmList &, bool );
    static void Put( CKeyValueParmList &, const _TCHAR *, bool, const _TCHAR *pcszEncoding = TEXT_ENCODING);

};
template <typename codec>
inline bool CKeyValue<bool, codec>::Get(CKeyValueParmList::iterator &iter
                               , bool bDefault)
{
    const _TCHAR *pszValue = (*iter).second;
    if (pszValue == NULL)
        return bDefault;

    StringType sResult = pszValue;
    sResult = codec::decode(sResult.c_str(), sResult.size());

    return (_tcsicmp(sResult.c_str(), _T("true")) == 0);
}
template <typename codec>
inline void CKeyValue<bool, codec>::Put(CKeyValueParmList &kvlist
                               , const _TCHAR *pszValueName
                               , bool bValue
                               , const _TCHAR *pcszEncoding)
{
    StringType sResult = (bValue ? _T("true") : _T("false"));
    sResult = codec::encode(sResult.c_str(), sResult.size());
    
    kvlist.NewKVPair(pszValueName, sResult.c_str(), pcszEncoding);
}
template <typename codec>
inline void CKeyValue<bool, codec>::Put(CKeyValueParmList &kvlist
                               , bool bValue)
{
    StringType sResult = (bValue ? _T("true") : _T("false"));
    sResult = codec::encode(sResult.c_str(), sResult.size());
    
    kvlist.NewValue(sResult.c_str());
}


//-----------------------------------------------------------------------------
// Specialization of Get() for long property
#ifdef _SCOTSSF_
template<typename codec> class DLLIMPORT
#elif defined _SCOT
template<typename codec> class DLLEXPORT
#else
template<typename codec> class 
#endif
CKeyValue<long, codec>
{
public:

    static long Get(CKeyValueParmList::iterator &, long );
    static void Put( CKeyValueParmList &, long );
    static void Put( CKeyValueParmList &, const _TCHAR *, long, const _TCHAR *pcszEncoding = TEXT_ENCODING);

};
template <typename codec>
inline long CKeyValue<long, codec>::Get(CKeyValueParmList::iterator &iter
                               , long lDefault)
{
	//374157
	if ( (*iter).second == NULL )
	{
		return lDefault;
	}
    StringType sValue = (*iter).second;
    if (sValue.empty())
        return lDefault;

    sValue = codec::decode(sValue.c_str(), sValue.size());

    return _tstol(sValue.c_str());
}
template <typename codec>
inline void CKeyValue<long, codec>::Put(CKeyValueParmList &kvlist
                               , const _TCHAR *pszValueName
                               , long lValue
                               , const _TCHAR *pcszEncoding)
{
    _TCHAR buffer[30];
    _ltot(lValue, buffer, 10);

    StringType sResult = buffer;
    sResult = codec::encode(sResult.c_str(), sResult.size());
    
    kvlist.NewKVPair(pszValueName, sResult.c_str(), pcszEncoding);
}
template <typename codec>
inline void CKeyValue<long, codec>::Put(CKeyValueParmList &kvlist
                               , long lValue)
{
    _TCHAR buffer[30];
    _ltot(lValue, buffer, 10);

    StringType sResult = buffer;
    sResult = codec::encode(sResult.c_str(), sResult.size());
    
    kvlist.NewValue(sResult.c_str());
}


//-----------------------------------------------------------------------------
// Specialization of Get() for int property
#ifdef _SCOTSSF_
template<typename codec> class DLLIMPORT
#elif defined _SCOT
template<typename codec> class DLLEXPORT
#else
template<typename codec> class 
#endif 
CKeyValue<int, codec>
{
public:

    static int Get(CKeyValueParmList::iterator &, int );
    static void Put( CKeyValueParmList &, int );
    static void Put( CKeyValueParmList &, const _TCHAR *, int, const _TCHAR *pcszEncoding = TEXT_ENCODING);

};
template <typename codec>
inline int CKeyValue<int, codec>::Get(CKeyValueParmList::iterator &iter
                             , int nDefault)
{
	//374157
	if ( (*iter).second == NULL )
	{
		return nDefault;
	}
    StringType sValue = (*iter).second;
    if (sValue.empty())
        return nDefault;

    sValue = codec::decode(sValue.c_str(), sValue.size());

    return _tstoi(sValue.c_str());
}
template <typename codec>
inline void CKeyValue<int, codec>::Put(CKeyValueParmList &kvlist
                              , const _TCHAR *pszValueName
                              , int lValue
                              , const _TCHAR *pcszEncoding)
{
    _TCHAR buffer[30];
    _itot(lValue, buffer, 10);

    StringType sResult = buffer;
    sResult = codec::encode(sResult.c_str(), sResult.size());

    kvlist.NewKVPair(pszValueName, sResult.c_str(), pcszEncoding);
}
template <typename codec>
inline void CKeyValue<int, codec>::Put(CKeyValueParmList &kvlist
                              , int lValue)
{
    _TCHAR buffer[30];
    _itot(lValue, buffer, 10);

    StringType sResult = buffer;
    sResult = codec::encode(sResult.c_str(), sResult.size());

    kvlist.NewValue(sResult.c_str());
}


//-----------------------------------------------------------------------------
// Specialization of Get() for const _TCHAR * property
#ifdef _SCOTSSF_
template<typename codec> class DLLIMPORT
#elif defined _SCOT
template<typename codec> class DLLEXPORT
#else
template<typename codec> class 
#endif
CKeyValue<const _TCHAR *, codec>
{
public:

    static const _TCHAR * Get(CKeyValueParmList::iterator &, const _TCHAR * );
    static void Put( CKeyValueParmList &, const _TCHAR * );
    static void Put( CKeyValueParmList &, const _TCHAR *, const _TCHAR *, const _TCHAR *pcszEncoding = TEXT_ENCODING);

};
template <typename codec>
inline const _TCHAR * CKeyValue<const _TCHAR *, codec>::Get(CKeyValueParmList::iterator &iter
                                                   , const _TCHAR *pszDefault)
{
    const _TCHAR *pszDefaultValue = ((pszDefault == NULL) ? NULL_STRING : pszDefault);

    const _TCHAR *pszValue = (*iter).second;
    if ((pszValue == NULL) || (pszValue[0] == 0))
        return pszDefaultValue;

    sValue = pszValue;
    sValue = codec::decode(sValue.c_str(), sValue.size());

    return sValue.c_str();
}
template <typename codec>
inline void CKeyValue<const _TCHAR *, codec>::Put(CKeyValueParmList &kvlist
                                         , const _TCHAR *pszValueName
                                         , const _TCHAR * pszValue
                                         , const _TCHAR *pcszEncoding)
{
    StringType sValue;
    if (pszValue != NULL)
        sValue = CKeyValueParmList::EscapeText(pszValue);

    sValue = codec::encode(sValue.c_str(), sValue.size());

    kvlist.NewKVPair(pszValueName, sValue, pcszEncoding);
}
template <typename codec>
inline void CKeyValue<const _TCHAR *, codec>::Put(CKeyValueParmList &kvlist
                                         , const _TCHAR * pszValue)
{
    StringType sValue;
    if (pszValue != NULL)
        sValue = CKeyValueParmList::EscapeText(pszValue);

    sValue = codec::encode(sValue.c_str(), sValue.size());

    kvlist.NewValue(sValue);
}


//-----------------------------------------------------------------------------
// Specialization of Get() for unsigned property
#ifdef _SCOTSSF_
template<typename codec> class DLLIMPORT
#elif defined _SCOT
template<typename codec> class DLLEXPORT
#else
template<typename codec> class 
#endif
CKeyValue<unsigned, codec>
{
public:

    static unsigned Get(CKeyValueParmList::iterator &, unsigned );
    static void Put( CKeyValueParmList &, unsigned );
    static void Put( CKeyValueParmList &, const _TCHAR *, unsigned, const _TCHAR *pcszEncoding = TEXT_ENCODING);

};
template <typename codec>
inline unsigned CKeyValue<unsigned, codec>::Get(CKeyValueParmList::iterator &iter
                                       , unsigned ulDefault)
{
	//374157
	if ( (*iter).second == NULL )
	{
		return ulDefault;
	}
    StringType sValue = (*iter).second;
    if (sValue.empty())
        return ulDefault;

    sValue = codec::decode(sValue.c_str(), sValue.size());

    return _tcstoul(sValue.c_str(), NULL, 10);
}
template <typename codec>
inline void CKeyValue<unsigned, codec>::Put(CKeyValueParmList &kvlist
                                   , const _TCHAR *pszValueName
                                   , unsigned ulValue
                                   , const _TCHAR *pcszEncoding)
{
    _TCHAR buffer[30];
    _ultot(ulValue, buffer, 10);

    StringType sResult = buffer;
    sResult = codec::encode(sResult.c_str(), sResult.size());

    kvlist.NewKVPair(pszValueName, sResult.c_str(), pcszEncoding);
}
template <typename codec>
inline void CKeyValue<unsigned, codec>::Put(CKeyValueParmList &kvlist
                                   , unsigned ulValue)
{
    _TCHAR buffer[30];
    _ultot(ulValue, buffer, 10);

    StringType sResult = buffer;
    sResult = codec::encode(sResult.c_str(), sResult.size());

    kvlist.NewValue(sResult.c_str());
}

//-----------------------------------------------------------------------------
// Specialization of Get() for float property
#ifdef _SCOTSSF_
template<typename codec> class DLLIMPORT
#elif defined _SCOT
template<typename codec> class DLLEXPORT
#else
template<typename codec> class 
#endif
CKeyValue<float, codec>
{
public:

    static float Get(CKeyValueParmList::iterator &, float );
    static void Put( CKeyValueParmList &, float );
    static void Put( CKeyValueParmList &, const _TCHAR *, float, const _TCHAR *pcszEncoding = TEXT_ENCODING);

};
template <typename codec>
inline float CKeyValue<float, codec>::Get(CKeyValueParmList::iterator &iter
                                 , float fDefault)
{
	//374157
	if ( (*iter).second == NULL )
	{
		return fDefault;
	}
    StringType sValue = (*iter).second;
    if (sValue.empty())
        return fDefault;

    sValue = codec::decode(sValue.c_str(), sValue.size());

    return (float)_tstof(sValue.c_str());
}
template <typename codec>
inline void CKeyValue<float, codec>::Put(CKeyValueParmList &kvlist
                                , const _TCHAR *pszValueName
                                , float fValue
                                , const _TCHAR *pcszEncoding)
{
    _TCHAR buffer[30];
    double  dValue = (double)fValue;
	if (-1 == _sntprintf_s(buffer, sizeof(buffer), _TRUNCATE, _T("%e"), dValue))
        ITHROWNEW(IException, _T(__FUNCTION__) _T(".  Buffer too short."))

    StringType sResult = buffer;
    sResult = codec::encode(sResult.c_str(), sResult.size());

    kvlist.NewKVPair(pszValueName, sResult.c_str(), pcszEncoding);
}
template <typename codec>
inline void CKeyValue<float, codec>::Put(CKeyValueParmList &kvlist
                                , float fValue)
{
    _TCHAR buffer[30];
    double  dValue = (double)fValue;
	if (-1 == _sntprintf_s(buffer, sizeof(buffer), _TRUNCATE, _T("%e"), dValue))
        ITHROWNEW(IException, _T(__FUNCTION__) _T(".  Buffer too short."))

    StringType sResult = buffer;
    sResult = codec::encode(sResult.c_str(), sResult.size());

    kvlist.NewValue(sResult.c_str());
}

//-----------------------------------------------------------------------------
// Specialization of Get() for double property
#ifdef _SCOTSSF_
template<typename codec> class DLLIMPORT
#elif defined _SCOT
template<typename codec> class DLLEXPORT
#else
template<typename codec> class 
#endif
CKeyValue<double, codec>
{
public:

    static double Get(CKeyValueParmList::iterator &, double );
    static void Put( CKeyValueParmList &, double );
    static void Put( CKeyValueParmList &, const _TCHAR *, double, const _TCHAR *pcszEncoding = TEXT_ENCODING);

};
template <typename codec>
inline double CKeyValue<double, codec>::Get(CKeyValueParmList::iterator &iter
                                   , double dDefault)
{
	//374157
	if ( (*iter).second == NULL )
	{
		return dDefault;
	}
    StringType sValue = (*iter).second;
    if (sValue.empty())
        return dDefault;

    sValue = codec::decode(sValue.c_str(), sValue.size());

    return _tstof(sValue.c_str());
}
template <typename codec>
inline void CKeyValue<double, codec>::Put(CKeyValueParmList &kvlist
                                 , const _TCHAR *pszValueName
                                 , double dValue
                                 , const _TCHAR *pcszEncoding)
{
    _TCHAR buffer[30];
	if (-1 == _sntprintf_s(buffer, sizeof(buffer), _TRUNCATE, _T("%e"), dValue))
        ITHROWNEW(IException, _T(__FUNCTION__) _T(".  Buffer too short."))

    StringType sResult = buffer;
    sResult = codec::encode(sResult.c_str(), sResult.size());

    kvlist.NewKVPair(pszValueName, sResult.c_str(), pcszEncoding);
}
template <typename codec>
inline void CKeyValue<double, codec>::Put(CKeyValueParmList &kvlist
                                 , double dValue)
{
    _TCHAR buffer[30];
	if (-1 == _sntprintf_s(buffer, sizeof(buffer), _TRUNCATE, _T("%e"), dValue))
        ITHROWNEW(IException, _T(__FUNCTION__) _T(".  Buffer too short."))

    StringType sResult = buffer;
    sResult = codec::encode(sResult.c_str(), sResult.size());

    kvlist.NewValue(sResult.c_str());
}
#endif
