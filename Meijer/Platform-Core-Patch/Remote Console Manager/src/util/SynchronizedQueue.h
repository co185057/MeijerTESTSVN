#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/SynchronizedQueue.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)SynchronizedQueue.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/SynchronizedQueue.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 9     1/18/05 4:02p Dm185016
* UNICODE support
* 
* 8     10/04/04 6:56p Dm185016
* Trace object is no longer shared.
* 
* 6     8/17/04 6:29p Dm185016
* Added tracing and timeout to the wait.
* 
* 5     5/21/04 10:37a Dm185016
* lint
* 
* 4     3/19/04 3:33p Dm185016
* Removed trace
* 
* 3     2/24/04 3:44p Dm185016
* Change base class from queue to list so that priority could be added
* 
* 2     2/10/04 11:53a Dm185016
* Moved Semaphore to utils namespace
* 
* 1     1/23/04 3:40p Dm185016
* New DLL Support
*/
//#include <queue>
#include <list>

using namespace std;

#include "Semaphore.h"
#include "xresource.h"
#include "Loggable.h"
#include "LoggableManager.h"

template < class T >
class CSynchronizedQueue
    : public CBaseObject
{

    typedef list <T *> QType;

    public:

    /*------------------------------- Constructors -------------------------------*/
    CSynchronizedQueue ( HTRACE hTrace );

    ~CSynchronizedQueue ();

    /*---------------------------------- Access ----------------------------------*/
    virtual T *Remove ( unsigned long wait = INFINITE );


    virtual void Add ( T *pElement, bool bImmediate = false );

    /*----------------------------- Hidden Members -------------------------------*/
    private:
        QType theQ;

        utils::CSemaphore accessSemaphore;
        CResource accessLock;

        ILoggable &loggable();
        ILoggable *m_pLoggable;

}; //


//-----------------------------------------------------------------------------
// Contructor for the base property template
template <typename T>
inline
CSynchronizedQueue<T>::CSynchronizedQueue(HTRACE hTrace)
    : CBaseObject(hTrace)
    , m_pLoggable(NULL)
    , accessSemaphore(hTrace)
    , accessLock(hTrace)
{
}


//-----------------------------------------------------------------------------
// Destructor for the base property template
template <typename T>
inline
CSynchronizedQueue<T>::~CSynchronizedQueue()
{
    accessSemaphore.Post();
}


//-----------------------------------------------------------------------------
// Removes the object on the head of the Queue and returns it.  Access to the
// queue is controlled via a Mutex semaphore and a regular semaphore.
#pragma warning( push )
#pragma warning( disable : 4311 )
template <typename T>
inline
T *CSynchronizedQueue<T>::Remove(unsigned long wait)
{
    ULONG ulPostCnt;

    accessLock.Lock();

    T *pElement = NULL;
    if (!theQ.empty())
    {
        pElement = theQ.front();
        theQ.pop_front();
    }

    else
    {
        accessLock.Unlock();

        if (loggable().IsTracing())
            ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Waiting ..."));        

        accessSemaphore.Wait(wait);

        if (loggable().IsTracing())
            ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Checking Q ..."));        

        accessLock.Lock();

        if (!theQ.empty())
        {
            pElement = theQ.front();
            theQ.pop_front();
        }
    }
    accessSemaphore.Reset(ulPostCnt);

    accessLock.Unlock();

    if (loggable().IsTracing())
        ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Removing:  ") + IString((int)pElement).d2x());        

    return pElement;
}


//-----------------------------------------------------------------------------
// Adds an object on the tail of the Queue.  Access to the
// queue is controlled via a Mutex semaphore and a regular semaphore.
template <typename T>
inline
void CSynchronizedQueue<T>::Add( T *pElement, bool bImmediate )
{
    accessLock.Lock();

    if (loggable().IsTracing())
        ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Pushing:  ") + IString((int)pElement).d2x());        

    if (bImmediate)
        theQ.push_front(pElement);
    else
        theQ.push_back(pElement);

    accessSemaphore.Post();

    accessLock.Unlock();
}


template <typename T>
inline
ILoggable &CSynchronizedQueue<T>::loggable()
{ 
    if (m_pLoggable == NULL)
        m_pLoggable = &Trace::getLoggable(GetTraceHandle(), eDCWin32Utils);

    return *m_pLoggable; 
}
#pragma warning( pop )
