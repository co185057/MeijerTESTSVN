#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/Createable.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)Createable.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/Createable.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 5     4/21/05 7:30p Dm185016
 * Cleaned up tracing
 * 
 * 3     3/24/05 7:29p Dm185016
 * UNICODE
 * 
 * 2     2/01/05 5:09p Dm185016
 * Added try/catch to find problem during initialization.
 * Added additional tracing to factory object.
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 12    1/18/05 4:00p Dm185016
* UNICODE support
* 
* 11    1/12/05 6:35p Dm185016
* 
* 10    10/04/04 7:00p Dm185016
* Trace object is no longer shared.
* 
* 8     8/04/04 3:10p Dm185016
* Fix to accomodate Visual Studio 2003.
* 
* 7     7/13/04 3:15p Dm185016
* Lint
* 
* 6     6/30/04 3:40p Dm185016
* Lint.
* Fix for derived class support.
* 
* 5     5/18/04 7:17p Dm185016
* lint
* 
* 4     3/23/04 4:13p Dm185016
* Added release function
* 
* 3     1/23/04 3:36p Dm185016
* DLL support
* 
* 2     1/16/04 9:30a Dm185016
* Added another registration macro
*/
//lint -emacro( 534, REGISTER_CREATEABLE )

#include <map>
#include "StringComparator.h"
#include "SMError.h"

#define REGISTRATION_FXN_NAME   _T("RegisterAll")
#define UNREGISTRATION_FXN_NAME   _T("UnregisterAll")
#define CREATION_FXN_NAME   _T("CreateObject")
#define RELEASE_OBJECT_FXN_NAME   _T("ReleaseObject")

//typedef void (*PREGISTRATION_FXN)(const _TCHAR *ppArrays[]);
//typedef void (*PUNREGISTRATION_FXN)();

#define REGISTER_CREATEABLE_NAME(FACTORY, NAME, CREATEABLE)\
    (FACTORY).Register(NAME, &C##CREATEABLE::NewInstance, &C##CREATEABLE::ReleaseInstance)

#if 1
#define REGISTER_CREATEABLE(FACTORY, CREATEABLE)\
    (FACTORY).Register(QUOTE(CREATEABLE), &C##CREATEABLE::NewInstance, &C##CREATEABLE::ReleaseInstance)
#define REGISTER_DERIVED_CREATEABLE(FACTORY, CREATEABLE)\
    (FACTORY).Register(QUOTE(CREATEABLE), &CCreateable<C##CREATEABLE, IAction>::NewInstance, &CCreateable<C##CREATEABLE, IAction>::ReleaseInstance)
#endif

#if 0
#define REGISTER_CREATEABLE_MEMFXN(FACTORY, CREATEABLE)\
    (FACTORY).Register(QUOTE(CREATEABLE), &C##CREATEABLE::NewInstance, &C##CREATEABLE::ReleaseInstance)
#define REGISTER_CREATEABLE(FACTORY, CREATEABLE)\
    (FACTORY).Register(QUOTE(CREATEABLE), _T("Create")QUOTE(CREATEABLE), _T("Release")QUOTE(CREATEABLE))
#endif

template <class T>
class CCreateableFactory
    : virtual public CBaseObject
{
public:

    typedef T *(*PCREATE_FXN)();
    typedef void (*PRELEASE_FXN)(T *);
    typedef std::map<StringType, PCREATE_FXN, StringComparator> CCreateableMap;
    typedef std::map<StringType, PRELEASE_FXN, StringComparator> CReleaseMap;

    typedef HRESULT (*PREGISTRATION_FXN)(const _TCHAR ***, HTRACE);
    typedef void (*PUNREGISTRATION_FXN)();
    typedef HRESULT (*PCREATION_FXN)(const _TCHAR *, T **);
    typedef HRESULT (*PRELEASE_OBJECT_FXN)(const _TCHAR *, T *);
    typedef HRESULT (*PREG_ERROR_FXN)(_TCHAR **);

    CCreateableFactory();
    virtual ~CCreateableFactory();

    HRESULT Clear() throw();

    HRESULT NewInstance(T **, const _TCHAR *);

    HRESULT ReleaseInstance(T *, const _TCHAR *);

    HRESULT Register(const _TCHAR *, const _TCHAR *, const _TCHAR *);

    HRESULT Register(const _TCHAR *, PCREATE_FXN, PRELEASE_FXN);

private:

    FARPROC FindFunction(LPCTSTR pcsFxnName);
    HINSTANCE GetModuleHandle();

	// hide copy and assignment
	CCreateableFactory(const CCreateableFactory&);
	CCreateableFactory& operator = (const CCreateableFactory &);
    
    CCreateableMap  createables;
    CReleaseMap     releaseables;

    HMODULE m_hModule;
};


template <class T>
CCreateableFactory<T>::CCreateableFactory()
    : m_hModule(NULL)
{
}



template <class T>
HINSTANCE CCreateableFactory<T>::GetModuleHandle()
{
    if (m_hModule == NULL)
        m_hModule = AfxGetInstanceHandle( );

    return m_hModule;
}

template <class T>
CCreateableFactory<T>::~CCreateableFactory()
{
}


template <class T>
HRESULT CCreateableFactory<T>::Clear() throw()
{
    createables.clear();
    releaseables.clear();

    return S_OK;
}


template <class T>
HRESULT CCreateableFactory<T>::Register(const _TCHAR *szName
                                      , typename CCreateableFactory<T>::PCREATE_FXN pCreateFxn
                                      , typename CCreateableFactory<T>::PRELEASE_FXN pReleaseFxn)
{
    try
    {
       ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Registering:  ") + IString(szName)); 

        CCreateableMap::iterator iter = createables.find(szName);
        if (iter != createables.end())
            return SECURITY_OBJECT_ALREADY_DEFINED;

        createables.insert(std::make_pair(szName, pCreateFxn));
        releaseables.insert(std::make_pair(szName, pReleaseFxn));
    }
    catch (...)
    {
        ITRACE_ERROR(_T("Registration failed:  ") + IString(szName));
    }

    return S_OK;
}


template <class T>
HRESULT CCreateableFactory<T>::Register(const _TCHAR *szName,
                                        const _TCHAR *szCreate,
                                        const _TCHAR *szRelease)
{
    try
    {
        ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Registering #1:  ") + IString(szName));  

        CCreateableMap::iterator iter = createables.find(szName);
        if (iter != createables.end())
            return SECURITY_OBJECT_ALREADY_DEFINED;

        CCreateableFactory<T>::PCREATE_FXN pCreateFxn   = (CCreateableFactory<T>::PCREATE_FXN)FindFunction(szCreate);
        CCreateableFactory<T>::PRELEASE_FXN pReleaseFxn = (CCreateableFactory<T>::PRELEASE_FXN)FindFunction(szRelease);

        createables.insert(std::make_pair(szName, pCreateFxn));
        releaseables.insert(std::make_pair(szName, pReleaseFxn));
    }
    catch (...)
    {
        ITRACE_ERROR(_T("Registration failed:  ") + IString(szName));
    }

    return S_OK;
}


template <class T>
FARPROC CCreateableFactory<T>::FindFunction(LPCTSTR pcsFxnName)
{

    FARPROC pFxn = NULL;

    pFxn = ::GetProcAddress(GetModuleHandle(), pcsFxnName);
    if (!pFxn)
    {
    	ISystemErrorInfo error(_T("GetProcAddress"));
        ITRACE_ERROR(error);
    }

    return pFxn;
}


template <class T>
HRESULT CCreateableFactory<T>::NewInstance(T **pCreateable, const _TCHAR *szName)
{
//  IMODTRACE_DEVELOP();
    *pCreateable = NULL;

    CCreateableMap::iterator iter = createables.find(szName);
    if (iter == createables.end())
        return SECURITY_UNDEFINED_ACTION_ID;

    PCREATE_FXN pCreateFxn = iter->second;

    *pCreateable = pCreateFxn();

    return S_OK;
}


template <class T>
HRESULT CCreateableFactory<T>::ReleaseInstance(T *pCreateable, const _TCHAR *szName)
{
//  IMODTRACE_DEVELOP();
    CReleaseMap::iterator iter = releaseables.find(szName);
    if (iter == releaseables.end())
        return SECURITY_UNDEFINED_ACTION_ID;

    PRELEASE_FXN pReleaseFxn = iter->second;

    pReleaseFxn(pCreateable);

    return S_OK;
}


template <class T, class RETT>
class CCreateable
{

public:
   
    static RETT* NewInstance();
   
    static void ReleaseInstance(RETT* );

protected:
    
    CCreateable() {};
    virtual ~CCreateable() {};

private:

	// hide copy and assignment
	CCreateable(const CCreateable&);
	CCreateable& operator = (const CCreateable &);
};
