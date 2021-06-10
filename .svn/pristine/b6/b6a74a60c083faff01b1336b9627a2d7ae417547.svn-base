#pragma once
#pragma warning( push )
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/Pool.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)Pool.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/Pool.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 4     3/16/05 6:20p Dm185016
 * Replace new/delete with malloc/free for operator new/delete
 * 
 * 3     2/28/05 3:33p Dm185016
 * lint
 * 
 * 2     2/21/05 8:03p Dm185016
 * Fixed signature for operator delete.  Removed call to explicit
 * destructor for the object, since this is not needed and probably is
 * destructive.
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 7     10/04/04 6:57p Dm185016
* Removed tracing
* 
* 5     8/26/04 10:01p Dm185016
* TAR 278069 - Added mutex semaphore to provide thread-safe access to
* list.
* 
* 4     8/04/04 3:11p Dm185016
* Fix to accomodate Visual Studio 2003.
* 
* 3     6/30/04 3:43p Dm185016
* Lint.
* Data Capture logging.
* 
* 2     1/23/04 3:36p Dm185016
* DLL support
*/

#define RELEASE_POOL(POOL)\
    {\
        C##POOL::FreeAll();\
    }

#include <vector>
#include "Loggable.h"
#include "LoggableManager.h"
#include "xresource.h"
#include "Utils.h"

using namespace std;

class UTILS_API CPoolManager
{

public:

    static ILoggable &loggable();

private:

    // hide copy and assignment
	CPoolManager();
	CPoolManager(const CPoolManager&);
	CPoolManager& operator = (const CPoolManager &);

    static ILoggable *m_pLoggable;
};


template<class T>
class CPool
{
    typedef vector<T *> ListType;
    typedef typename ListType::iterator ListIterator;
    typedef T ListClass;

public:

    static void *operator new(size_t);
    static void  operator delete(void *);

    static void FreeAll();

protected:

    CPool();
	CPool(const CPool&);
	virtual ~CPool();

private:

    // hide assignment
	CPool& operator = (const CPool &);

    static CResource m_lock;
    static ListType m_List;
    static ListType m_AllList;

    static bool m_bDelete;
};


template <class T>
CPool<T>::CPool()
{
}


template <class T>
CPool<T>::CPool(const CPool&)
{
}


template <class T>
CPool<T>::~CPool()
{
}


template <class T>
CResource CPool<T>::m_lock(_T("CPool"));


template <class T>
typename CPool<T>::ListType CPool<T>::m_List;


template <class T>
typename CPool<T>::ListType CPool<T>::m_AllList;


template <class T>
bool CPool<T>::m_bDelete = false;


#pragma warning( push )
#pragma warning( disable : 4311 )
template <class T>
void CPool<T>::FreeAll()
{
    //IMODTRACE_DEVELOP();
    m_bDelete = true;
    T* pObject;

    m_lock.Lock();
    try
    {
        for (CPool<T>::ListIterator iter = m_AllList.begin();
            iter!= m_AllList.end();
            iter++)
        {
            pObject = *iter;
            //if (CPoolManager::loggable().IsTracing()) pObject->GetTraceObject().write("freed element @" + IString((ULONG)pObject).d2x() + ".");
            delete pObject;
        } /* endfor */
        m_AllList.clear();
    }
    catch (...)
    {
        m_lock.Unlock();
        //pObject->GetTraceObject().XTRACE_DOT_ERROR();
        throw;
    }
    m_lock.Unlock();
}


template <class T>
void *CPool<T>::operator new(size_t oSize)
{
    T *retValue = NULL;
    m_lock.Lock();
    try
    {
   	    if (!m_List.empty())
   	    {
            retValue = m_List.back();
            m_List.pop_back();

            //if (CPoolManager::loggable().IsTracing()) pObject->GetTraceObject().write(__FUNCTION__".  Dequeued element @" + IString((ULONG)retValue).d2x() + ".");
 	    }

  	    if (retValue == NULL)
  	    {
    	    retValue = (T *)malloc(oSize);
            if (retValue == NULL)
                throw bad_alloc(_MESG("Unable to allocate object"));

      	    memset((char *)retValue, 0, oSize);
      	    m_AllList.push_back(retValue);
            //if (CPoolManager::loggable().IsTracing()) pObject->GetTraceObject().write(__FUNCTION__".  Allocated element @" + IString((ULONG)retValue).d2x() + ".");
  	    } /* endcatch */
    }
    catch (...)
    {
        m_lock.Unlock();
        //pObject->GetTraceObject().XTRACE_DOT_ERROR();
        throw;
    }
    m_lock.Unlock();

    return retValue;
}


template <class T>
void CPool<T>::operator delete(void *object)
{
    m_lock.Lock();
    try
    {
        if (m_bDelete)
        {
            //if (CPoolManager::loggable().IsTracing())pObject->GetTraceObject().write(__FUNCTION__".  Deleting object. @" + IString((ULONG)object).d2x() + ".");
            free(object);
        }
        else
        {
            //if (CPoolManager::loggable().IsTracing())pObject->GetTraceObject().write(__FUNCTION__".  Enqueuing object @" + IString((ULONG)object).d2x() + ".");
            m_List.push_back((T*)object);
        }
    }
    catch (...)
    {
        m_lock.Unlock();
        //pObject->GetTraceObject().XTRACE_DOT_ERROR();
        throw;
    }
    m_lock.Unlock();
}
#pragma warning( pop )
