#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/Include/Registry.h 4     1/05/10 8:38p Cu185008 $
/*
 * @(#)Registry.h    $Revision: 4 $ $Date: 1/05/10 8:38p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/Include/Registry.h $
 * 
 * 4     1/05/10 8:38p Cu185008
 * Migrating Core to G2.
 * 
 * 3     3/24/05 7:31p Dm185016
 * UNICODE
 * 
 * 2     2/28/05 3:34p Dm185016
 * lint
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 13    10/04/04 6:57p Dm185016
* Added iterator for registry keys and values.
* 
* 11    6/30/04 3:44p Dm185016
* Fixed a problem when value not in registry.
* 
* 10    5/24/04 6:35p Dm185016
* lint + fix for Std Dev A value read from registry wrong - low order
* byte was zeroed out
* 
* 9     5/05/04 9:09a Dm185016
* 
* 8     5/05/04 8:57a Dm185016
* Merged lost changes
*
* 7     5/05/04 8:52a Dm185016
* Change _SCOTSSF to _SCOTSSF_
*
* 6     5/04/04 6:10p Tk124825
* made the necessary changes for unicode build
*
* 5     5/04/04 4:27p Dm185016
* Fixed memory leak.  Added UNICODE support.
*
* 4     4/20/04 2:52p Dm185016
* Fixed compile problem in 6.0
*
* 3     4/16/04 4:19p Dm185016
* Added read using specified key
*
* 2     4/12/04 3:32p Dm185016
* Added ContainsSubKey
*
* 1     3/19/04 3:32p Dm185016
* New classes for accessing the Registry
*/

//lint --e{ 613, 668, 578, 1510 }		Null ptr not possible.  EnsureBuffer assigns value to buffer
//lint --e{1759}        By design, the postfix increment returns a reference

#include <vector>
#include <iterator>
#include "Utils.h"
#include "xerror.h"

#pragma warning( push )
#pragma warning( disable : 4251 )

#if ( _MFC_VER < 0x700 )
    #ifndef _tstof
        #define _tstof      atof
    #endif
    #ifndef _tstol
        #define _tstol      _ttol
    #endif
    #ifndef _tstoi
        #define _tstoi      _ttoi
    #endif
    #ifndef _tstoul
        #define _tstoul     _ttoul
    #endif
#endif

#if _MFC_VER < 0x0700
    #define TEMPLATE_DEFINITION template<class T>
    #define KEY_PARM HKEY_LOCAL_MACHINE
#else
    #define TEMPLATE_DEFINITION template<class T, HKEY hKey = HKEY_LOCAL_MACHINE>
    #define KEY_PARM hKey
#endif

//TEMPLATE_DEFINITION CRegistry;

struct _key_descriptor
{
};


class CRegistry;


// Accessor template - used to return the correct type for a KeyValue parameter.
TEMPLATE_DEFINITION class CRegistryValue
//    : public CRegistry
{
public:


	//CRegistryValue() : CRegistry(KEY_PARM) {};

 //   
 //   CRegistryValue(const _TCHAR *pKeyName, HKEY hKey = HKEY_LOCAL_MACHINE) : CRegistry(pKeyName) {};


 //   virtual ~CRegistryValue() {};

    static T GetRegistryValue( CRegistry &, const _TCHAR *pKeyName, const _TCHAR *pValueName, T );
    static T GetRegistryValue(  CRegistry &, HKEY pKeyName, const _TCHAR *pValueName, T );
};


// the iterator
template <typename _Tp>
class _registry_key_iterator 
    : public std::iterator<std::forward_iterator_tag, _Tp>
{
public:

    friend class CRegistry;

public:
    
    // Constructor
    _registry_key_iterator(HKEY hKey=0)
        : m_hKey(hKey)
        , m_pszKeyBuffer(NULL)
        , m_dwCurrentIndex(0)
        , m_dwSubKeys(0)
        , m_dwMaxSubKeyLen(0)
        , m_dwValues(0)
        , m_dwMaxValueNameLen(0)
        , m_dwMaxValueLen(0)
    {
        m_ftLastWriteTime.dwLowDateTime = 0;
        m_ftLastWriteTime.dwHighDateTime = 0;
        if (hKey != 0)
        {
            LONG hRc = RegQueryInfoKey(
                                    hKey,                      // handle to key
                                    NULL,                 // class buffer
                                    NULL,               // size of class buffer
                                    NULL,             // reserved
                                    &m_dwSubKeys,             // number of subkeys
                                    &m_dwMaxSubKeyLen,        // longest subkey name
                                    NULL,         // longest class string
                                    &m_dwValues,              // number of value entries
                                    &m_dwMaxValueNameLen,     // longest value name
                                    &m_dwMaxValueLen,         // longest value data
                                    NULL, // descriptor length
                                    &m_ftLastWriteTime     // last write time
                                );
            if (hRc != ERROR_SUCCESS)
            {
    	        ISystemErrorInfo error(hRc, _T("RegQueryInfoKey"));
                throw IException(error.text(), hRc);
            }
        }

        m_dwMaxSubKeyLen++;
        m_pszKeyBuffer = new _TCHAR[m_dwMaxSubKeyLen];
        memset(m_pszKeyBuffer, 0, m_dwMaxSubKeyLen);
    }
    

    _registry_key_iterator<_Tp>(const _registry_key_iterator<_Tp> &iter)
    {
        m_KVPair            = iter.m_KVPair;
        m_hKey              = iter.m_hKey;
        m_dwCurrentIndex    = iter.m_dwCurrentIndex;
        m_dwSubKeys         = iter.m_dwSubKeys;             // number of subkeys
        m_dwMaxSubKeyLen    = iter.m_dwMaxSubKeyLen;        // longest subkey name
        m_dwValues          = iter.m_dwValues;              // number of value entries
        m_dwMaxValueNameLen = iter.m_dwMaxValueNameLen;     // longest value name
        m_dwMaxValueLen     = iter.m_dwMaxValueLen;         // longest value data
        m_ftLastWriteTime   = iter.m_ftLastWriteTime;        // last write time

        m_pszKeyBuffer = new _TCHAR[m_dwMaxSubKeyLen];
        memcpy(m_pszKeyBuffer, iter.m_pszKeyBuffer, m_dwMaxSubKeyLen);
    }


    _registry_key_iterator<_Tp> &operator =(const _registry_key_iterator<_Tp> &rhs)
    {
        m_KVPair            = rhs.m_KVPair;
        m_hKey              = rhs.m_hKey;
        m_dwCurrentIndex    = rhs.m_dwCurrentIndex;
        m_dwSubKeys         = rhs.m_dwSubKeys;             // number of subkeys
        m_dwMaxSubKeyLen    = rhs.m_dwMaxSubKeyLen;        // longest subkey name
        m_dwValues          = rhs.m_dwValues;              // number of value entries
        m_dwMaxValueNameLen = rhs.m_dwMaxValueNameLen;     // longest value name
        m_dwMaxValueLen     = rhs.m_dwMaxValueLen;         // longest value data
        m_ftLastWriteTime   = rhs.m_ftLastWriteTime;        // last write time

        m_pszKeyBuffer = new _TCHAR[m_dwMaxSubKeyLen];
        memcpy(m_pszKeyBuffer, rhs.m_pszKeyBuffer, m_dwMaxSubKeyLen);
    }


    virtual ~_registry_key_iterator()
    {
        delete [] m_pszKeyBuffer;
    }


    //typedef std::pair<_Tp, _key_descriptor> _pair_t;
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
    

    inline _registry_key_iterator<_Tp> & operator ++(int)
    {
        m_dwCurrentIndex++;

        return *this;
    }
    

    /// <summary>Prefix operator ++ is used to get the next value using 
    /// the same key as the previous usage</summary>
    inline _registry_key_iterator<_Tp> & operator ++()
    {
        m_dwCurrentIndex++;

        return *this;
    }


    // required to test if iteration complete
    inline bool operator==(const _registry_key_iterator<_Tp> & rhs) const
    {
        return (m_dwCurrentIndex == rhs.m_dwCurrentIndex);
    }


    // required to test if iteration complete
    inline bool operator!=(const _registry_key_iterator<_Tp> & rhs) const
    {
        return (m_dwCurrentIndex != rhs.m_dwCurrentIndex);
    }

protected:            
    

    inline void UpdatePair()
    {
        LONG hRC = RegEnumKeyEx(
                            m_hKey,             // handle to key to enumerate
                            m_dwCurrentIndex,   // subkey index
                            m_pszKeyBuffer,     // subkey name
                            &m_dwMaxSubKeyLen,  // size of subkey buffer
                            NULL,               // reserved
                            NULL,               // class string buffer
                            NULL,               // size of class string buffer
                            &m_ftLastWriteTime  // last write time
                         );

        if (hRC != ERROR_SUCCESS)
        {
    	    ISystemErrorInfo error(hRC, _T("RegEnumKeyEx"));
//          ITRACE_ERROR(error);
            throw IException(error.text(), hRC);
        }
        m_KVPair.first      = const_cast<const _TCHAR *>(m_pszKeyBuffer);
        m_KVPair.second     = const_cast<const _TCHAR *>(m_pszKeyBuffer);
    }


    inline void SeekEnd() { m_dwCurrentIndex = m_dwSubKeys; }


private:

    _pair_t m_KVPair;

    HKEY m_hKey;

    _TCHAR *m_pszKeyBuffer;

    DWORD m_dwCurrentIndex;
    DWORD m_dwSubKeys;             // number of subkeys
    DWORD m_dwMaxSubKeyLen;        // longest subkey name
    DWORD m_dwValues;              // number of value entries
    DWORD m_dwMaxValueNameLen;     // longest value name
    DWORD m_dwMaxValueLen;         // longest value data
    FILETIME m_ftLastWriteTime;    // last write time
};


class UTILS_API CRegistry
{
public:

    friend bool CRegistryValue<bool>::GetRegistryValue( CRegistry &
                                            , HKEY 
                                            , const _TCHAR *
                                            , bool );
    friend bool CRegistryValue<bool>::GetRegistryValue( CRegistry &
                                            , const _TCHAR *
                                            , const _TCHAR *
                                            , bool );
    friend const _TCHAR * CRegistryValue<const _TCHAR *>::GetRegistryValue( CRegistry &
                                                                , const _TCHAR *
                                                                , const _TCHAR *
                                                                , const _TCHAR *);
    friend const _TCHAR * CRegistryValue<const _TCHAR *>::GetRegistryValue( CRegistry &
                                                                , HKEY
                                                                , const _TCHAR *
                                                                , const _TCHAR *);
    friend int CRegistryValue<int>::GetRegistryValue( CRegistry &
                                            , const _TCHAR *
                                            , const _TCHAR *
                                            , int );
    friend int CRegistryValue<int>::GetRegistryValue( CRegistry &
                                            , HKEY
                                            , const _TCHAR *
                                            , int );
    friend unsigned CRegistryValue<unsigned>::GetRegistryValue( CRegistry &
                                            , const _TCHAR *
                                            , const _TCHAR *
                                            , unsigned );
    friend unsigned CRegistryValue<unsigned>::GetRegistryValue( CRegistry &
                                            , HKEY
                                            , const _TCHAR *
                                            , unsigned );
    friend long CRegistryValue<long>::GetRegistryValue( CRegistry &
                                            , const _TCHAR *
                                            , const _TCHAR *
                                            , long );
    friend long CRegistryValue<long>::GetRegistryValue( CRegistry &
                                            , HKEY
                                            , const _TCHAR *
                                            , long );
    friend float CRegistryValue<float>::GetRegistryValue( CRegistry &
                                            , const _TCHAR *
                                            , const _TCHAR *
                                            , float );
    friend float CRegistryValue<float>::GetRegistryValue( CRegistry &
                                            , HKEY
                                            , const _TCHAR *
                                            , float );
    friend double CRegistryValue<double>::GetRegistryValue( CRegistry &
                                            , const _TCHAR *
                                            , const _TCHAR *
                                            , double );
    friend double CRegistryValue<double>::GetRegistryValue( CRegistry &
                                            , HKEY
                                            , const _TCHAR *
                                            , double );

    friend class _registry_key_iterator<const _TCHAR *>;

    typedef _registry_key_iterator<const _TCHAR *> iterator;

	typedef union
	{
		_TCHAR szData[1];
		long lData;
	} _UREGISTRY_DATA;


	CRegistry(HKEY hKey = HKEY_LOCAL_MACHINE) ;

    
    CRegistry(const _TCHAR *, HKEY hKey = HKEY_LOCAL_MACHINE);

    virtual ~CRegistry();

    bool ContainsSubKey( const _TCHAR *pKeyName, const _TCHAR *pSubKeyName ) const;

public:

    long EnsureBuffer(HKEY, const _TCHAR *pValueName);

    void EnsureBuffer(unsigned long lBufferLength = 0);

    long ReadValue(const _TCHAR *pKeyName, const _TCHAR *pValueName);

    long ReadValue(HKEY, const _TCHAR *);

protected:

    _UREGISTRY_DATA *m_puRegistryData;
    unsigned m_ulBufferCnt;
	DWORD m_dwSize;
	DWORD m_dwType;
    DWORD m_lDWORD;

// Methods in this section deal with enumerating the subkeys within this key
public:

    iterator &keys_begin();
    iterator &keys_end();
    bool empty() const;

private:

	HKEY m_hKey;

    iterator *m_piterBegin;
    iterator *m_piterEnd;

private:

	CRegistry(const CRegistry &) ;
    CRegistry &operator =(const CRegistry &);

};
#pragma warning( pop )


// Accessor template - used to return the correct type for a KeyValue parameter.
//template<>
//inline CRegistry<class T, HKEY hKey>::CRegistry(const _TCHAR *pszKeyName)
//    : m_hKey(hKey)
//{
//}


//-----------------------------------------------------------------------------
// Specialization of Get() for const _TCHAR * property
template <>
inline const _TCHAR * CRegistryValue<const _TCHAR *>::GetRegistryValue( CRegistry &registry
                                                                , const _TCHAR *pKeyName
                                                                , const _TCHAR *pValueName
                                                                , const _TCHAR *pszDefault)
{
    if (ERROR_SUCCESS != registry.ReadValue(pKeyName, pValueName))
    {
        unsigned long nSize = 0;
        if (pszDefault != NULL)
        {
            nSize = (unsigned long)_tcslen(pszDefault);

            registry.EnsureBuffer(nSize);

            _tcscpy(registry.m_puRegistryData->szData, pszDefault);
        }
        else
        {
            registry.EnsureBuffer(nSize);

            registry.m_puRegistryData->szData[0] = 0;
        }
    }

    return registry.m_puRegistryData->szData;
}
template <>
inline const _TCHAR * CRegistryValue<const _TCHAR *>::GetRegistryValue( CRegistry &registry
                                                                , HKEY hkey
                                                                , const _TCHAR *pValueName
                                                                , const _TCHAR *pszDefault)
{
    registry.m_dwType = REG_SZ;
    if (ERROR_SUCCESS != registry.ReadValue(hkey, pValueName))
    {
        unsigned long nSize = 0;
        if (pszDefault != NULL)
        {
            nSize = (unsigned long)_tcslen(pszDefault);

            registry.EnsureBuffer(nSize);

            _tcscpy(registry.m_puRegistryData->szData, pszDefault);
        }
    }

    return registry.m_puRegistryData->szData;
}


//-----------------------------------------------------------------------------
// Specialization of Get() for bool property
template <>
inline bool CRegistryValue<bool>::GetRegistryValue( CRegistry &registry
                                            , const _TCHAR *pKeyName
                                            , const _TCHAR *pValueName
                                            , bool bDefault)
{
    if (ERROR_SUCCESS != registry.ReadValue(pKeyName, pValueName))
    	return bDefault;

    bool bRetValue = 
        (_tcsicmp(registry.m_puRegistryData->szData, _T("true")) == 0) ||
        (_tcsicmp(registry.m_puRegistryData->szData, _T("yes")) == 0) ||
        ((registry.m_puRegistryData->szData[0] | ' ') == 't') ||
        ((registry.m_puRegistryData->szData[0] | ' ') == 'y');

    return bRetValue;
}
template <>
inline bool CRegistryValue<bool>::GetRegistryValue( CRegistry &registry
                                            , HKEY hkey
                                            , const _TCHAR *pValueName
                                            , bool bDefault)
{
    registry.m_dwType = REG_SZ;
    if (ERROR_SUCCESS != registry.ReadValue(hkey, pValueName))
    	return bDefault;

    return (_tcsicmp(registry.m_puRegistryData->szData, _T("true")) == 0) 
		|| ((registry.m_puRegistryData->szData[0]+' ') == 'y');
}


//-----------------------------------------------------------------------------
// Specialization of Get() for long property
template <>
inline long CRegistryValue<long>::GetRegistryValue( CRegistry &registry
                                            , const _TCHAR *pKeyName
                                            , const _TCHAR *pValueName
                                            , long lDefault)
{
    if (ERROR_SUCCESS != registry.ReadValue(pKeyName, pValueName))
    	return lDefault;

    registry.EnsureBuffer(sizeof(long));

    return registry.m_puRegistryData->lData;
}


//-----------------------------------------------------------------------------
// Specialization of Get() for int property
template <>
inline int CRegistryValue<int>::GetRegistryValue( CRegistry &registry
                                            , const _TCHAR *pKeyName
                                            , const _TCHAR *pValueName
                                            , int nDefault)
{
    if (ERROR_SUCCESS != registry.ReadValue(pKeyName, pValueName))
    	return nDefault;

    return _ttoi(registry.m_puRegistryData->szData);
}


//-----------------------------------------------------------------------------
// Specialization of Get() for unsigned property
template <>
inline unsigned CRegistryValue<unsigned>::GetRegistryValue( CRegistry &registry
                                            , const _TCHAR *pKeyName
                                            , const _TCHAR *pValueName
                                            , unsigned ulDefault)
{
    if (ERROR_SUCCESS != registry.ReadValue(pKeyName, pValueName))
    	return ulDefault;

    return _tcstoul(registry.m_puRegistryData->szData, NULL, 10);
}


#if !defined(_SCOT) && !defined(_SCOTSSF_)
//-----------------------------------------------------------------------------
// Specialization of Get() for unsigned property
template <>
inline float CRegistryValue<float>::GetRegistryValue( CRegistry &registry
                                            , const _TCHAR *pKeyName
                                            , const _TCHAR *pValueName
                                            , float fDefault)
{
    if (ERROR_SUCCESS != registry.ReadValue(pKeyName, pValueName))
    	return fDefault;

    return (float)_tstof(registry.m_puRegistryData->szData);
}


//-----------------------------------------------------------------------------
// Specialization of Get() for double property
template <>
inline double CRegistryValue<double>::GetRegistryValue( CRegistry &registry
                                            , const _TCHAR *pKeyName
                                            , const _TCHAR *pValueName
                                            , double dDefault)
{
    if (ERROR_SUCCESS != registry.ReadValue(pKeyName, pValueName))
    	return dDefault;

    return _tstof(registry.m_puRegistryData->szData);
}
#endif
