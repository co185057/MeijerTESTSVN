#pragma once
#pragma warning( push )
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/xnew.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)xnew.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/xnew.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 9     5/03/05 4:13p Dm185016
 * Cleaned up tracing
 * 
 * 7     3/24/05 7:38p Dm185016
 * UNICODE again
 * 
 * 6     3/17/05 8:35a Dm185016
 * UNICODE
 * 
 * 5     3/16/05 6:20p Dm185016
 * Replace new/delete with malloc/free for operator new/delete
 * 
 * 4     2/28/05 3:39p Dm185016
 * lint
 * 
 * 3     2/22/05 8:34a Dm185016
 * Fixed UNICODE problem.
 * 
 * 2     2/21/05 8:02p Dm185016
 * Rewritten to make functional.  Added debugging information to new and
 * delete for pooled objects.
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 4     10/05/04 8:29a Dm185016
* CBaseObject renamed as CPoolObject.
* 
* 2     6/30/04 3:50p Dm185016
* Data Capture logging support.
* 
* 1     3/19/04 3:32p Dm185016
* New base class, CBaseObject, for tracing memory leaks
*/
#include "Utils.h"
#include "Loggable.h"

using namespace std;

#define newpooledobject new(GetTraceHandle(), _T(__FILE__), __LINE__)

#pragma warning( disable : 4311 )

typedef struct 
{
    unsigned long    fence;
    unsigned long    blkSize;
    HTRACE  hTrace;
    unsigned long    reserved;
} SAllocBlock;


template<class T>
class CPoolObject 
    //: public CBaseObject
{
public:

    static void *operator new(size_t oSize, HTRACE hTrace, const _TCHAR *pszFile, long lLine)
    {
        char *retValue = NULL;
        retValue = (char *)malloc(oSize+sizeof(SAllocBlock));
        if (retValue == NULL)
        {
            CT2A szTempBuf((const _TCHAR *)(IString(_T("Unable to allocate object.  File:  ")) 
                                           + IString(pszFile) + _T(".  Line:  ") 
                                           + IString(lLine)));
            throw bad_alloc(szTempBuf);
        }
        memset((void *)(retValue+sizeof(SAllocBlock)), 0, oSize);

        SAllocBlock *pBlock = (SAllocBlock *)retValue;
        pBlock->fence       = 0x5a5a5a5a;
        pBlock->hTrace      = hTrace;
        pBlock->blkSize     = (unsigned long)oSize;

        retValue += sizeof(SAllocBlock);

        //if (loggable().IsTracing())

        if (_loggable(hTrace).IsDebug()) ITRACE_NEW_STATIC(hTrace, retValue, oSize, pszFile, lLine);

        return retValue;
    }


    static void operator delete(void *object, HTRACE hTrace, const _TCHAR *pszFile, long lLine)
    {
        //if (loggable().IsTracing())
            //GetTraceObject().write(L8, __FUNCTION__ _T("  Deleting object. @") + IString((ULONG)object).d2x() +  _T(" for ") + IString((long)oSize));

        char *pObject = (char *)object;
        pObject -= sizeof(SAllocBlock);

        SAllocBlock *pBlock = (SAllocBlock *)pObject;

        if (_loggable(hTrace).IsDebug()) ITRACE_DELETE_STATIC(hTrace, object, pBlock->blkSize, pszFile, lLine);

        free(pObject);
    }


    static void  operator delete(void *object)
    {
        //if (loggable().IsTracing())
            //GetTraceObject().write(L8, __FUNCTION__ _T("  Deleting object. @") + IString((ULONG)object).d2x() +  _T(" for ") + IString((long)oSize));

        char *pObject = (char *)object;
        pObject -= sizeof(SAllocBlock);

        SAllocBlock *pBlock = (SAllocBlock *)pObject;

        if (_loggable(pBlock->hTrace).IsDebug()) ITRACE_DELETE2_STATIC(pBlock->hTrace, object, pBlock->blkSize);

        free(pObject);
    }

    CPoolObject();
	virtual ~CPoolObject();

protected:

    static ILoggable &_loggable(HTRACE hTrace);

private:

    // hide copy and assignment
	CPoolObject(const CPoolObject&);
	CPoolObject& operator = (const CPoolObject &);

    static const _TCHAR *_trace_name;

    static ILoggable *m_loggable;
};


template <class T>
CPoolObject<T>::CPoolObject()
{
}


template <class T>
CPoolObject<T>::~CPoolObject()
{
}


template <class T>
ILoggable &CPoolObject<T>::_loggable(HTRACE hTrace)
{ 
    if (m_loggable == NULL)
        m_loggable = &Trace::getLoggable(hTrace, eDCAllocations);

    return *m_loggable; 
}


template <class T>
const _TCHAR *CPoolObject<T>::_trace_name;


template <class T>
ILoggable *CPoolObject<T>::m_loggable = NULL;
#pragma warning( pop )
