// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/xthread.cpp 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)xthread.cpp    $Revision: 1 $ $Date: 6/20/08 1:13p $
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

#include "stdafx.h"
#include "xthread.h"
#include "xerror.h"
#include "verify.h"
#include <process.h>


/**
* Default no-argument constructor
*/
CThreadBase::CThreadBase(bool bAutoDelete)
    : m_hThread(INVALID_HANDLE_VALUE)
    , m_threadID(0)
    , m_pThreadFxn(NULL)
    , m_bAutoDelete(bAutoDelete)
{
    IMODTRACE_DEVELOP();
}


/**
* Default no-argument constructor
*/
CThreadBase::CThreadBase(PTHREADFN pThreadFxn, bool bAutoDelete)
    : m_hThread(INVALID_HANDLE_VALUE)
    , m_threadID(0)
    , m_pThreadFxn(pThreadFxn)
    , m_bAutoDelete(bAutoDelete)
{
//  IMODTRACE_DEVELOP();
}


#pragma warning( push )
#pragma warning( disable: 4311)
void CThreadBase::Start()
{
    if (m_hThread != INVALID_HANDLE_VALUE)
    {
        ITRACE_DEVELOP(_T("Closing thread handle:  ") + IString((long)m_hThread).d2x() 
                     + _T(".  Restarting thread ..."));
        HRESULT hr = ::CloseHandle( m_hThread );
        VERIFY_WIN32_AND_LOG(hr);

        m_hThread = INVALID_HANDLE_VALUE;
    }

    m_hThread = (HANDLE)_beginthreadex( NULL, 
                                        0, 
                                        _runit, 
                                        this, 
                                        0,
                                        &m_threadID );
    if (m_hThread == 0)
        ITHROWNEW(IException, _T("Unable to start thread"));

    ITRACE_DEVELOP(_T(__FUNCTION__) _T("New thread handle:  ") + IString((long)m_hThread).d2x() 
                 + _T(".  thread id:  ") + IString(m_threadID).d2x());
}
#pragma warning( pop )


/*
* Terminates execution of this thread.  If the thread is waiting, then this method
* needs to be overridden to provide the mechanism for interrupting the wait condition.
* The override should perform the following operations:
*       1.  Signal the thread that it is time to end
*       2.  Interrupt the thread
*       3.  Call this method in the base class to wait for the thread to end
*/
//lint --e{713}
void CThreadBase::Stop()
{
    {
        try
        {
            HRESULT hr = WaitForSingleObject( m_hThread, INFINITE );
            VERIFY_WIN32_AND_LOG(hr);
        }
        catch (...)
        { ITRACE_DOT_ERROR(); }
    }
}


PTHREADFN CThreadBase::GetThreadFxn() 
{ 
    return m_pThreadFxn; 
}


void CThreadBase::SetThreadFxn(PTHREADFN pThreadFxn) 
{ 
    m_pThreadFxn = pThreadFxn; 
}


CThreadBase::~CThreadBase()
{
    IMODTRACE_DEVELOP();

    // Destroy the thread object.
    try
    {
        HRESULT hr = ::CloseHandle( m_hThread );
        VERIFY_WIN32_AND_LOG(hr);

        m_hThread = NULL;
    }
    catch (...)
    {
        ITRACE_DOT_ERROR();
    }
}


unsigned __stdcall CThreadBase::_runit( void *pArgs )
{
//  IMODTRACE_DEVELOP();

    unsigned rc = 0;

    CThreadBase &thread = *(CThreadBase*)pArgs;

    PTHREADFN pThreadFxn = thread.GetThreadFxn();

    if (pThreadFxn != NULL)
        rc = (*pThreadFxn)( pArgs );

    _endthreadex( rc );

    return rc;
}
