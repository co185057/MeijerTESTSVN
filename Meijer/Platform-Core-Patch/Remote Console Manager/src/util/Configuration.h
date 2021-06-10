#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/Configuration.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)ConfigManager.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
 *
 * Copyright 2004 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/Configuration.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     2/28/05 3:30p Dm185016
 * lint
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 11    1/18/05 4:00p Dm185016
* UNICODE support
* 
* 10    10/04/04 7:00p Dm185016
* ConfigManager object is no longer shared.
* 
* 8     5/18/04 3:43p Dm185016
* lint
* 
* 7     5/04/04 4:27p Dm185016
* Added code to allow merge of XML documents
* 
* 6     4/28/04 12:49p Tk124825
* Added SetValue method to ConfigurationManager class
* 
* 5     4/12/04 3:32p Dm185016
* Split SetURL into SetURL and ReloadOptions
* 
* 4     3/19/04 3:30p Dm185016
* Memory leak fix
* 
* 3     3/01/04 10:20a Dm185016
* Added IsValid() method for error reporting
* 
* 2     2/18/04 9:15a Dm185016
* Added template accessor functions
* 
* 1     2/10/04 11:51a Dm185016
* New object to support XML configuration files.
*/

#include "singleton.h"
#include "ContentHandler.h"
#include "Utils.h"
#include <map>

using namespace std;


enum ELoadOptions
{
    eInvalid = 0,
	eReplace = 1,
	eDoNotReplace,
	eMerge
};


class UTILS_API CConfigManager
    : public CContentHandler
{
    typedef list <StringType> CXPathStack;
    typedef list <StringType> CValueStack;
    typedef map<StringType, StringType, StringComparator> CConfigMap;

public:

    CConfigManager(HTRACE hTrace);

    virtual ~CConfigManager();
    
    virtual HRESULT STDMETHODCALLTYPE putDocumentLocator( 
        /* [in] */ ISAXLocator __RPC_FAR *pLocator);

    virtual HRESULT STDMETHODCALLTYPE startDocument( void);

    virtual HRESULT __stdcall startElement (
        /*[in]*/ unsigned short * pwchNamespaceUri,
        /*[in]*/ int cchNamespaceUri,
        /*[in]*/ unsigned short * pwchLocalName,
        /*[in]*/ int cchLocalName,
        /*[in]*/ unsigned short * pwchQName,
        /*[in]*/ int cchQName,
        /*[in]*/ struct MSXML2::ISAXAttributes * pAttributes );

    virtual HRESULT __stdcall endElement (
        /*[in]*/ unsigned short * pwchNamespaceUri,
        /*[in]*/ int cchNamespaceUri,
        /*[in]*/ unsigned short * pwchLocalName,
        /*[in]*/ int cchLocalName,
        /*[in]*/ unsigned short * pwchQName,
        /*[in]*/ int cchQName );

    virtual HRESULT STDMETHODCALLTYPE characters(
        /* [in] */ wchar_t __RPC_FAR *pwchChars,
        /* [in] */ int cchChars);

    virtual ISAXLocator __RPC_FAR * getDocumentLocator() { return m_pLocator; }

    void LogError(const _TCHAR *pszText) const;

    const _TCHAR *GetErrorLocationText() const;

    virtual HRESULT SetURL(const _TCHAR *, ELoadOptions eLoadOptions=eReplace);		//lint !e1735

    virtual HRESULT Reload(ELoadOptions eLoadOptions=eReplace);		//lint !e1735

    virtual HRESULT GetString(const _TCHAR *, _TCHAR **);

    virtual HRESULT SetValue(const _TCHAR *, const _TCHAR *);

    bool IsValid() const;

    static void ReleaseInstance();

protected:

    const _TCHAR *GetErrorLocationText0();

private:

    CConfigManager();
    CConfigManager(const CConfigManager &);               // Hidden Copy construcor
    CConfigManager &operator =(const CConfigManager &);   // Hidden assignment operator

    StringType m_szURL;
    StringType m_szCurrentValue;
    CConfigMap m_mapValues;
    CXPathStack m_stackXpathExprs;
    CValueStack m_stackValues;

    CComQIPtr<ISAXLocator> m_pLocator;

    StringType m_szErrorLocationText;

    bool m_bValid;

	ELoadOptions m_eLoadOptions;

};


// Accessor template - used to return the correct type configuration parameter.
template<class T>
class CConfiguration
{
public:
    static T Get(CConfigManager &, const _TCHAR *, T lDefault);

};


//-----------------------------------------------------------------------------
// Specialization of Get() for long property
template <>
inline long CConfiguration<long>::Get(CConfigManager &cfMgr, const _TCHAR *pszName, long lDefault)
{
    long lValue;

    _TCHAR *pszValue;
    HRESULT hr = cfMgr.GetString(pszName, &pszValue);
    lValue = (hr == S_OK)
        ? _tstol(pszValue)
        : lDefault;

    return lValue;
}


//-----------------------------------------------------------------------------
// Specialization of Get() for int property
template <>
inline int CConfiguration<int>::Get(CConfigManager &cfMgr, const _TCHAR *pszName, int nDefault)
{
    int nValue;

    _TCHAR *pszValue;
    HRESULT hr = cfMgr.GetString(pszName, &pszValue);
    nValue = (hr == S_OK)
        ? _tstoi(pszValue)
        : nDefault;

    return nValue;
}


//-----------------------------------------------------------------------------
// Specialization of Get() for int property
template <>
inline bool CConfiguration<bool>::Get(CConfigManager &cfMgr, const _TCHAR *pszName, bool bDefault)
{
    bool bValue;

    _TCHAR *pszValue;
    HRESULT hr = cfMgr.GetString(pszName, &pszValue);
    bValue = (hr == S_OK)
        ? ( (_tcsicmp(pszValue, _T("true")) == 0) || (_tcsicmp(pszValue, _T("Y")) == 0) )
        : bDefault;

    return bValue;
}


//-----------------------------------------------------------------------------
// Specialization of Get() for const _TCHAR * property
template <>
inline const _TCHAR * CConfiguration<const _TCHAR *>::Get(CConfigManager &cfMgr, const _TCHAR *pszName, const _TCHAR *pszDefault)
{
    _TCHAR * retValue;

    HRESULT hr = cfMgr.GetString(pszName, &retValue);
    if (hr != S_OK)
        retValue = const_cast<_TCHAR *>(pszDefault);

    return retValue;
}


//-----------------------------------------------------------------------------
// Specialization of Get() for unsigned property
template <>
inline unsigned CConfiguration<unsigned>::Get(CConfigManager &cfMgr, const _TCHAR *pszName, unsigned ulDefault)
{
    unsigned ulValue;

    _TCHAR *pszValue;
    HRESULT hr = cfMgr.GetString(pszName, &pszValue);
    ulValue = (hr == S_OK)
        ? _tcstoul(pszValue, NULL, 10)
        : ulDefault;

    return ulValue;
}


//-----------------------------------------------------------------------------
// Specialization of Get() for unsigned property
template <>
inline float CConfiguration<float>::Get(CConfigManager &cfMgr, const _TCHAR *pszName, float fDefault)
{
    float fValue;

    _TCHAR *pszValue;
    HRESULT hr = cfMgr.GetString(pszName, &pszValue);
    fValue = (hr == S_OK)
        ? (float)_tstof(pszValue)
        : fDefault;

    return fValue;
}


//-----------------------------------------------------------------------------
// Specialization of Get() for unsigned property
template <>
inline double CConfiguration<double>::Get(CConfigManager &cfMgr, const _TCHAR *pszName, double dDefault)
{
    double dValue;

    _TCHAR *pszValue;
    HRESULT hr = cfMgr.GetString(pszName, &pszValue);
    dValue = (hr == S_OK)
        ? _tstof(pszValue)
        : dDefault;

    return dValue;
}


