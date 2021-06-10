#pragma once
// $Header: /Fastlane/4.2/Development/Core/Source/Include/KeyValueHandler.h 15    11/27/07 5:38p Lp185019 $
/*
 * @(#)KeyValueHandler.h    $Revision: 15 $ $Date: 11/27/07 5:38p $
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
* $Log: /Fastlane/4.2/Development/Core/Source/Include/KeyValueHandler.h $
 * 
 * 15    11/27/07 5:38p Lp185019
 * SR629 (TAR 334800) - Resolve CISP Issue: RCM is tracing password by
 * default
 * 
 * 13    6/14/05 9:04a Dm185016
 * Added enum property for parsing classes.
 * 
 * 12    5/19/05 5:25p Dm185016
 * Added typedefs for base class.
 * 
 * 11    5/09/05 6:31p Dm185016
 * Added definitions for UnknownKey handler.
 * 
 * 10    5/04/05 6:44p Dm185016
 * If empty string passed in as parameter (non-null pointer to null
 * string), then it will return S_OK.  Before it returned E_INVALIDARG.
 * 
 * 9     4/26/05 10:31a Dm185016
 * ProcessRecord() now returns HRESULT
 * Added macros for ProcessRecord() creation
 * 
 * 8     4/21/05 7:31p Dm185016
 * Added dumping of source when problem detected parsing.
 * 
 * 6     4/05/05 12:04p Dm185016
 * TAR 297356 - Added macros for constructing Clear() method.
 * 
 * 5     3/30/05 9:15p Dm185016
 * 
 * 4     3/24/05 7:30p Dm185016
 * UNICODE
 * 
 * 3     2/28/05 3:30p Dm185016
 * lint
 * 
 * 2     2/21/05 8:03p Dm185016
 * Added tracing info.
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 20    1/18/05 4:01p Dm185016
* UNICODE support
* 
* 19    1/10/05 3:12p Dm185016
* Fixed compilation problem with SCOTAPP.
* 
* 18    1/07/05 10:14a Dm185016
* Made display of error messages optional for RCM.
* ParseKeys/ParseRecords now returns HRESULT to reflect any problems
* detected.
* 
* 17    12/14/04 11:28a Dm185016
* 
* 16    12/09/04 1:51p Dm185016
* Added accessor methods.
* 
* 15    12/07/04 4:30p Dm185016
* Added Clear method definition for class macro.
* 
* 14    11/19/04 1:41p Dm185016
* Made it to work in both SCOTAPP and RCM
* 
* 13    11/08/04 10:44a Dm185016
* New macros for simplified definition of handlers.
* 
* 12    10/05/04 2:47p Dm185016
* Changes to support dual compilation
* 
* 11    10/04/04 6:59p Dm185016
* Trace object is no longer shared.
* 
* 9     6/30/04 3:41p Dm185016
* Lint.
* 
* 8     6/11/04 8:43a Dm185016
* Fixed compile problem with SCOTAPP.
* 
* 7     6/10/04 2:25p Dm185016
* Added more trace.
* 
* 6     5/21/04 2:22p Tk124825
* Added TraceThis API
* 
* 5     5/18/04 1:07p Tp151000
* remove that line to make the app compiling
* 
* 4     5/15/04 1:11p Dm185016
* Added try/catch clause in ParseKeys() and ParseRecords() to prevent
* exceptions.
* 
* 3     3/19/04 3:31p Dm185016
* Removed trace
* 
* 2     3/01/04 10:20a Dm185016
* Added parser method for records
*/

/*lint -emacro(1762, BEGIN_KV_HANDLER_CLASS) */
/*lint -emacro(785, END_KV_HANDLER_CLASS) */
/*lint -emacro(785, END_KV_HANDLER_MAP) */

#pragma warning(disable:4101)
#include "KVParmList.h"
#include "StringComparator.h"
#include <map>

#define DECLARE_KV_HANDLER(NAME)\
public:\
    virtual void NAME(CKeyValueParmList::iterator &)

#define DEFINE_KV_HANDLER(BASE, NAME, KEYS)\
    void BASE::NAME(CKeyValueParmList::iterator &KEYS)

//#define DECLARE_RECORD_HANDLER(NAME)\
//public:\
//    typedef void (PKVRECORD_HANDLER*)();\
//    void NAME();
//    PKVRECORD_HANDLER m_kv_record_handler;

#define BEGIN_KV_HANDLER_MAP(CNAME)\
    CNAME::SValueNameEntry\
        CNAME::m_handlerMap[] = {

#define DEFINE_KV_HANDLER_MAP_ENTRY(CNAME, TEXT, FNAME)\
    { TEXT, NULL, &CNAME::FNAME },

#define END_KV_HANDLER_MAP()\
    { NULL, NULL } };

#define DECLARE_KV_HANDLER_MAP()\
private:\
    static SValueNameEntry m_handlerMap[];\
protected: \
    virtual const SValueNameEntry* GetValueTable() const { return m_handlerMap; }


#define DECLARE_KV_HANDLER_CLASS_BEGIN(CLASS_NAME)\
    /*lint save -e19 */\
    class CLASS_NAME\
          : public CKeyValueHandler<CLASS_NAME>\
    {\
          DECLARE_KV_HANDLER_MAP();\
    public:\
        typedef CKeyValueHandler<CLASS_NAME> fqbase;\
        typedef CLASS_NAME base;\
        CLASS_NAME() { Clear(); }\

#define DECLARE_KV_RECORD_HANDLER()\
    protected:\
        virtual HRESULT ProcessRecord();

#define DECLARE_KV_UNKNOWN_KEY_HANDLER()\
    protected:\
        virtual HRESULT UnknownKey(CKeyValueParmList::iterator &);

#define DECLARE_KV_CSTRING(VAR_NAME)\
    public:\
          DECLARE_KV_HANDLER(VAR_NAME);\
          const CString &Get##VAR_NAME() const { return m_##VAR_NAME; }\
          void Set##VAR_NAME(const CString &newValue) { m_##VAR_NAME = newValue; }\
          void Set##VAR_NAME(const _TCHAR *newValue) { m_##VAR_NAME = newValue; }\
    private:\
          CString m_##VAR_NAME;

#define DECLARE_KV_BOOL(VAR_NAME)\
    public:\
          DECLARE_KV_HANDLER(VAR_NAME);\
          bool Is##VAR_NAME() const { return m_##VAR_NAME; }\
          void Set##VAR_NAME(bool newValue) { m_##VAR_NAME = newValue; }\
    private:\
          bool m_##VAR_NAME;

#define DECLARE_KV_LONG(VAR_NAME)\
    public:\
          DECLARE_KV_HANDLER(VAR_NAME);\
          long Get##VAR_NAME() const { return m_##VAR_NAME; }\
          void Set##VAR_NAME(long newValue) { m_##VAR_NAME = newValue; }\
    private:\
          long m_##VAR_NAME;

#define DECLARE_KV_INT(VAR_NAME)\
    public:\
          DECLARE_KV_HANDLER(VAR_NAME);\
          int Get##VAR_NAME() const { return m_##VAR_NAME; }\
          void Set##VAR_NAME(int newValue) { m_##VAR_NAME = newValue; }\
    private:\
          int m_##VAR_NAME;

#define DECLARE_KV_ENUM_WITH_TYPE(VAR_NAME, ETYPE)\
    public:\
        DECLARE_KV_HANDLER(VAR_NAME);\
        ETYPE Get##VAR_NAME() const { return m_##VAR_NAME.Value(); }\
        void Set##VAR_NAME(ETYPE newValue) { m_##VAR_NAME.Value(newValue); }\
        DECLARE_ENUM_PROPERTY_WITH_TYPE(m_##VAR_NAME, ETYPE);

#define DECLARE_KV_HANDLER_CLASS_END\
    public:\
        void Clear();\
    protected:\
        void TraceThis(const _TCHAR *pPairs) throw();\
    };\
    /* lint -restore */

#if defined(_SCOT) || defined(_SCOTSSF_)
    #define BEGIN_KV_HANDLER_CLASS(FQCLASS_NAME)\
    void FQCLASS_NAME::TraceThis(const _TCHAR *pPairs) throw()\
    {\
        trace(L6, CString (pPairs) );\
    }\
    BEGIN_KV_HANDLER_MAP(FQCLASS_NAME)
#else
    #define BEGIN_KV_HANDLER_CLASS(FQCLASS_NAME)\
    void FQCLASS_NAME::TraceThis(const _TCHAR *) throw()\
    {\
    }\
    BEGIN_KV_HANDLER_MAP(FQCLASS_NAME)
#endif

#define END_KV_HANDLER_CLASS()\
END_KV_HANDLER_MAP()

#define BEGIN_KV_HANDLER_CLASS_CLEAR(FQCLASS_NAME)\
    void FQCLASS_NAME::Clear()\
    {
#define CLEAR_KV_CSTRING(VAR_NAME)\
    m_##VAR_NAME.Empty();
#define CLEAR_KV_INT(VAR_NAME, VALUE)\
    Set##VAR_NAME(VALUE);
#define CLEAR_KV_LONG(VAR_NAME, VALUE)\
    Set##VAR_NAME(VALUE);
#define CLEAR_KV_BOOL(VAR_NAME, VALUE)\
    Set##VAR_NAME(VALUE);
#define CLEAR_KV_ENUM(VAR_NAME, VALUE)\
    Set##VAR_NAME(VALUE);
#define END_KV_HANDLER_CLASS_CLEAR()\
}

#define BEGIN_KV_HANDLER_CLASS_RECORD_HANDLER(FQCLASS_NAME)\
    void FQCLASS_NAME::ProcessRecord()\
    {
#define END_KV_HANDLER_CLASS_RECORD_HANDLER(HR)\
        return HR;\
    }

#define BEGIN_KV_HANDLER_CLASS_UNKNOWN_KEY_HANDLER(FQCLASS_NAME, KEYS)\
    void FQCLASS_NAME::UnknownKey(CKeyValueParmList::iterator &KEYS)\
    {
#define END_KV_HANDLER_CLASS_UNKNOWN_KEY_HANDLER(HR)\
        return HR;\
    }


#define DEFINE_KV_CSTRING(FQCLASS_NAME, VAR_NAME)\
void FQCLASS_NAME::VAR_NAME(CKeyValueParmList::iterator &keys)\
{\
    m_##VAR_NAME = CKeyValue<const _TCHAR *>::Get(keys, NULL);\
}


#define DEFINE_KV_INT(FQCLASS_NAME, VAR_NAME)\
void FQCLASS_NAME::VAR_NAME(CKeyValueParmList::iterator &keys)\
{\
    m_##VAR_NAME = CKeyValue<int>::Get(keys, 0);\
}


#define DEFINE_KV_LONG(FQCLASS_NAME, VAR_NAME)\
void FQCLASS_NAME::VAR_NAME(CKeyValueParmList::iterator &keys)\
{\
    m_##VAR_NAME = CKeyValue<long>::Get(keys, 0);\
}


#define DEFINE_KV_BOOL(FQCLASS_NAME, VAR_NAME)\
void FQCLASS_NAME::VAR_NAME(CKeyValueParmList::iterator &keys)\
{\
    m_##VAR_NAME = CKeyValue<bool>::Get(keys, false);\
}


#define DEFINE_KV_ENUM(FQCLASS_NAME, VAR_NAME)\
void FQCLASS_NAME::VAR_NAME(CKeyValueParmList::iterator &keys)\
{\
    StringType sTemp = CKeyValue<const _TCHAR *>::Get(keys, NULL);\
    m_##VAR_NAME.Convert(sTemp);\
}


#if defined(_SCOT) || defined(_SCOTSSF_)
#   ifdef IMODTRACE_DEVELOP
#       undef IMODTRACE_DEVELOP
#   endif

#   ifdef ITRACE_EXCEPTION
#       undef ITRACE_EXCEPTION
#   endif

#   ifdef ITRACE_ERROR
#       undef ITRACE_ERROR
#   endif

#   define IMODTRACE_DEVELOP
#   define ITRACE_EXCEPTION(exc)\
        trace(L6, _T("Exception caught!  Line:  %d.  File:  %s"), __LINE__, _T(__FILE__))
#   define ITRACE_ERROR(text)\
        trace(L6, _T("%s  Line:  %d.  File:  %s"), text, __LINE__, _T(__FILE__))
#endif

template<class T>
class CKeyValueHandler
{
public:
    
    typedef void (T::*PVALUEFXN)(CKeyValueParmList::iterator &);
    typedef const _TCHAR *key_t;
    typedef std::map<key_t, PVALUEFXN, StringComparator> CValueMap;

    struct SValueNameEntry
    {
        const _TCHAR *entry;
        const _TCHAR *rsvd;
        PVALUEFXN pFxn;
    };

public:

    CKeyValueHandler() {};
    virtual ~CKeyValueHandler() {};

#if defined(_SCOT) || defined(_SCOTSSF_)
    typedef long HTRACE;
    #define INVALID_HANDLE (-1)
#endif

    HRESULT ParseKeys(const _TCHAR *, HTRACE hTrace = INVALID_HANDLE, bool bReportMissingKey = false) throw();
    HRESULT ParseRecords(const _TCHAR *, HTRACE hTrace = INVALID_HANDLE, bool bReportMissingKey = false) throw();


protected:

    virtual const SValueNameEntry* GetValueTable() const=0;

    virtual void UnknownKey(CKeyValueParmList::iterator &);

    virtual HRESULT ProcessRecord();

#if defined(_SCOT) || defined(_SCOTSSF_)

    virtual void TraceThis(const _TCHAR *) throw();
#endif

private:

    // hide copy and assignment
    CKeyValueHandler(const CKeyValueHandler&);
    CKeyValueHandler& operator = (const CKeyValueHandler &);

    const CValueMap &GetValueMap();

    static CValueMap values;

};


template <class T>
const typename CKeyValueHandler<T>::CValueMap &CKeyValueHandler<T>::GetValueMap()
{
    if (values.empty())
    {
        const SValueNameEntry*pValueMap = GetValueTable();
        for (int i=0; pValueMap->entry != NULL; pValueMap++, i++)
        {
            values[pValueMap->entry] = pValueMap->pFxn;
        }
    };

    return values;
}


template <class T>
void CKeyValueHandler<T>::UnknownKey(CKeyValueParmList::iterator &)
{
    // Override this method to report the unknown key here
}


template <class T>
HRESULT CKeyValueHandler<T>::ProcessRecord()
{
    // Override this method to handle records after all keys for that record
    // have been processed.
    return S_OK;
}


#if defined(_SCOT) || defined(_SCOTSSF_)
template <class T>
void CKeyValueHandler<T>::TraceThis(const _TCHAR *pPairs) throw()
{
    // Override this method 
}
#endif


template <class T>
HRESULT CKeyValueHandler<T>::ParseKeys(const _TCHAR *pPairs, HTRACE hTrace, bool bReportMissingKey) throw()
{
    HRESULT hr = S_OK;

    int nStep = 0;

    ASSERT(pPairs!=NULL);
    if (pPairs == NULL)
    {
#if !defined(_SCOT) && !defined(_SCOTSSF_)
        ITRACE_DEVELOP_STATIC(hTrace, _T("Null list of KV pairs"));
#else
        TraceThis (_T("Malformed Key/Value pair.  Missing '=' character"));
#endif
        return hr;
    }
    if (pPairs[0] == 0)
    {
#if !defined(_SCOT) && !defined(_SCOTSSF_)
        ITRACE_DEVELOP_STATIC(hTrace, _T("Empty list!"));
#else
        TraceThis (_T("Empty list!"));
#endif
        return hr;
    }

    nStep = 1;
    try
    {
        const CValueMap &keymap = GetValueMap();
        CKeyValueParmList keys(pPairs);
        nStep = 2;
        CKeyValueParmList::iterator keys_iter = keys.begin();
        nStep = 3;
        while (keys_iter != keys.end())
        {
            const _TCHAR *key = (*keys_iter).first;
            nStep = 4;
            if (key == NULL)
            {
                nStep = 5;
                if (bReportMissingKey)
                {
#if !defined(_SCOT) && !defined(_SCOTSSF_)
                    throw IException(_T("Malformed Key/Value pair.  Missing '=' character"), E_INVALIDARG);
#else
                    TraceThis (_T("Malformed Key/Value pair.  Missing '=' character"));
#endif
                }
                return E_INVALIDARG;
            }

            nStep = 6;
            CValueMap::const_iterator iter = keymap.find(key);
            if (iter != keymap.end())
            {
                nStep = 7;
                PVALUEFXN pValueFxn = iter->second;
#if !defined(_SCOT) && !defined(_SCOTSSF_)
				ITRACE_DEVELOP_STATIC(hTrace, _T("Parsing key=") + IString(key) + _T(".  value=\"") +
					((keys_iter.GetCurrentEncoding() == _T("text")) ? IString((*keys_iter).second) : _T("****")) + _T("\""));
#else
				TraceThis (_T("Parsing key=") + CString(key) + _T(".  value=\"") +
					((keys_iter.GetCurrentEncoding() == _T("text")) ? CString ((*keys_iter).second) : _T("****")) + _T("\""));
#endif
                ((*(T*)this).*pValueFxn)(keys_iter);
            }
            else
                UnknownKey(keys_iter);
            nStep = 8;

            keys_iter++;
            nStep = 9;
        }
    }
#if !defined(_SCOT) && !defined(_SCOTSSF_)
    catch (const IException &exc)
    { 
        ITRACE_EXCEPTION_STATIC(hTrace, exc); 

        hr = exc.errorId();
    }
    catch (const exception &exc)
    { 
        ITRACE_EXCEPTION_STATIC(hTrace, exc); 

        hr = E_INVALIDARG;
    }
#endif
    catch (...)
    { 
#if !defined(_SCOT) && !defined(_SCOTSSF_)
        ITRACE_DOT_ERROR_STATIC(hTrace); 
        ITRACE_DEVELOP_STATIC(hTrace, _T(__FUNCTION__) _T(".  Step=") + IString(nStep)) ;
        ITRACE_DUMP_STATIC(hTrace, _T("pPairs"), pPairs, (unsigned int)_tcslen(pPairs))
#else
        TraceThis (_T("... Exception caught"));
#endif

        hr = E_INVALIDARG;
    }

    return hr;
}


template <class T>
HRESULT CKeyValueHandler<T>::ParseRecords(const _TCHAR *pPairs, HTRACE hTrace, bool bReportMissingKey) throw()
{
    HRESULT hr = S_OK;

    ASSERT(pPairs!=NULL);
    if (pPairs == NULL)
        return hr;

#if !defined(_SCOT) && !defined(_SCOTSSF_)
    ITRACE_DEVELOP_STATIC(hTrace, _T("pPairs length=") + IString((long)_tcslen(pPairs)) 
                 + _T("pPairs=\"") + IString(pPairs) + _T("\"") );
#else
    CString szLength;
    szLength.Format(_T("%d"), _tcslen(pPairs));
    TraceThis(_T("pPairs length=") + szLength 
                 + _T(".  pPairs=\"") + CString(pPairs) + _T("\"") );
#endif

    ULONG ulTracePoint = 0;

    try
    {
        const CValueMap &keymap = GetValueMap();
        CKeyValueParmList keys(pPairs);
        CKeyValueParmList::iterator keys_iter = keys.begin();
        bool bContinue = true;
        while (bContinue)
        {
            try
            {
                // Process all keys in the record
                while (keys_iter != keys.end())
                {
                    ulTracePoint = 1;
                    const _TCHAR *key = (*keys_iter).first;
                    if (key == NULL)
                    {
                        if (bReportMissingKey)
                        {
#if !defined(_SCOT) && !defined(_SCOTSSF_)
                            throw IException(_T("Malformed Key/Value pair.  Missing '=' character"), E_INVALIDARG);
#else
                            TraceThis (_T("Malformed Key/Value pair.  Missing '=' character"));
#endif
                        }
                        return E_INVALIDARG;
                    }
                    ulTracePoint = 2;

                    CValueMap::const_iterator iter = keymap.find(key);
                    ulTracePoint = 3;
                    if (iter != keymap.end())
                    {
                        ulTracePoint = 4;
    #if !defined(_SCOT) && !defined(_SCOTSSF_)
                        ITRACE_DEVELOP_STATIC(hTrace, _T("Parsing key=") + IString(key) + _T(".  value=\"") 
                            + IString((*keys_iter).second) + _T("\""));
    #else
                        TraceThis (_T("Parsing key=") + CString(key) + _T(".  value=\"") 
                            + CString ((*keys_iter).second) );
    #endif
                        ulTracePoint = 5;
                        PVALUEFXN pValueFxn = iter->second;
                        ulTracePoint = 6;
                        ((*(T*)this).*pValueFxn)(keys_iter);
                        ulTracePoint = 7;
                    }
                    else
                        UnknownKey(keys_iter);

                    ulTracePoint = 8;
                    keys_iter++;
                    ulTracePoint = 9;
                }
            }
            catch (...)
            {
#if !defined(_SCOT) && !defined(_SCOTSSF_)      
                ITRACE_ERROR_STATIC(hTrace, _T("... Exception caught inner loop")); 
#else
                TraceThis (_T("... Exception caught inner loop"));
                CString szTP;
                szTP.Format(_T("%d"), ulTracePoint);
                TraceThis(_T("Last trace point=") + szTP); 
#endif
                keys_iter.Clear();
            }

            // Give the owner a chance to process the record we just finished parsing
            ProcessRecord();

            // See if there are anymore records to process
            bContinue = keys_iter.NextRecord();
        }
    }
#if !defined(_SCOT) && !defined(_SCOTSSF_)
    catch (const IException &exc)
    { 
        ITRACE_EXCEPTION_STATIC(hTrace, exc); 

        hr = exc.errorId();
    }
#endif
    catch (const exception &exc)
    { 
#if !defined(_SCOT) && !defined(_SCOTSSF_)
        ITRACE_EXCEPTION_STATIC(hTrace, exc); 
#endif

        hr = E_INVALIDARG;
    }
    catch (...)
//      { ITRACE_DOT_ERROR(); }
    {
#if !defined(_SCOT) && !defined(_SCOTSSF_)      
        ITRACE_ERROR_STATIC(hTrace, _T("... Exception caught")); 
#else
        TraceThis (_T("... Exception caught"));
#endif

        hr = E_INVALIDARG;
    }

    return hr;
}


template<class T>
typename CKeyValueHandler<T>::CValueMap CKeyValueHandler<T>::values;
