// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/Semaphore.cpp 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)CSemaphore.cpp    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
#include "Semaphore.h"
#include "XError.h"
#include "xtrace.h"
#include "Loggable.h"
#include "LoggableManager.h"

#pragma warning( disable : 4311 )


using namespace utils;


//ILoggable *utils::CSemaphore::m_pLoggable = NULL;


utils::CSemaphore::CSemaphore(HTRACE hTrace, const _TCHAR * n)
    : CBaseObject(hTrace)
    , hSema4(NULL)
    , pszName(n)
    , m_pLoggable(NULL)
{
    Open();
}


utils::CSemaphore::~CSemaphore()
{
    try
    {
        Close();
    } /* endtry */
    catch (...)
        { ITRACE_DOT_ERROR(); }

    pszName = NULL;
}


utils::CSemaphore &utils::CSemaphore::Open(const _TCHAR * n)
{
    if (n != NULL)
        pszName = n;

    if (hSema4 == NULL)
    {
        hSema4 = CreateEvent(NULL,        // Use default security
                             TRUE,        // Manual Reset
                             FALSE,       // Put in RESET state
                             pszName);    // CSemaphore name
        if (hSema4 == NULL)
        {
            ITRACE_ERROR(_T("Unable to create CSemaphore.  Name=") + IString(((pszName == NULL) ? _T("<Unnamed>") : pszName)));
            ITHROWSYSTEMERROR(GetLastError(), _T("CreateEvent"), IException::unrecoverable);
        }
    } /* endif */

    if (loggable().IsTracing())
        ITRACE_DEVELOP(_T("Open Semaphore.  pszName=") + IString(pszName) + _T(".  hSema4=") + IString((ULONG)hSema4).d2x() + _T("."));
    return *this;
}


utils::CSemaphore &utils::CSemaphore::Close()
{
    if (loggable().IsTracing())
        ITRACE_DEVELOP(_T("Close Semaphore.  pszName=") + IString(pszName) + _T(".  hSema4=") + IString((ULONG)hSema4).d2x() + _T("."));

    if (hSema4 != NULL)
    {
        if (!CloseHandle(hSema4))      /* Get rid of CSemaphore       */
        {
            ISystemErrorInfo sysError((HRESULT)GetLastError(), _T("CloseHandle"));
            ITRACE_DEVELOP((const _TCHAR *)sysError.text());
        }
    }
    else
    {
    } /* endif */

    hSema4 = NULL;

    return *this;
}


utils::CSemaphore &utils::CSemaphore::Post()
{
    if (loggable().IsTracing())
        ITRACE_DEVELOP(_T("Post Semaphore.  pszName=") + IString(pszName) + _T(".  hSema4=") + IString((ULONG)hSema4).d2x() + _T("."));

    if (!SetEvent(hSema4))
    {
        ULONG rc = GetLastError();
        ITRACE_ERROR(_T(__FUNCTION__) _T("failed.  rc=") + IString(rc).d2x() + _T("."));
        ITHROWSYSTEMERROR(rc, _T("SetEvent"), IException::unrecoverable);
    }

	return *this;
}


bool utils::CSemaphore::Wait(ULONG waitValue)
{
    bool retValue = false;

    DWORD dwResult = 0;

    if (loggable().IsTracing())
        ITRACE_DEVELOP(_T("Wait Semaphore.  pszName=") + IString(pszName) + _T(".  hSema4=") + IString((ULONG)hSema4).d2x() + _T("."));

    switch (dwResult = WaitForSingleObject(hSema4, waitValue))
    {
        case WAIT_OBJECT_0 :
            retValue = true;
            break;
        case WAIT_ABANDONED :
        case WAIT_TIMEOUT :
            //ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Wait failed.  dwResult:  ") + IString(dwResult).d2x());
            break;
        default :
            ITRACE_DEVELOP(_T("Wait failed.  Reason:  ") + IString(dwResult) + _T(".  GetLastError():  ") + IString(GetLastError()));
            ITHROWSYSTEMERROR(GetLastError(), _T("WaitForSingleObject"), IException::unrecoverable);
    }

    return retValue;
}


utils::CSemaphore &utils::CSemaphore::Reset(unsigned long & /* ulPostCount */)
{
    if (loggable().IsTracing())
        ITRACE_DEVELOP(_T("Reset Semaphore.  pszName=") + IString(pszName) + _T(".  hSema4=") + IString((ULONG)hSema4).d2x() + _T("."));

    if (!ResetEvent(hSema4))
    {
        ITHROWSYSTEMERROR(GetLastError(), _T("ResetEvent"), IException::unrecoverable);
    } // endif

	return *this;
}


ILoggable &utils::CSemaphore::loggable()
{ 
    if (m_pLoggable == NULL)
        m_pLoggable = &Trace::getLoggable(GetTraceHandle(), eDCWin32Utils);
//      m_pLoggable = &CLoggableManager::getLoggable(_T("Semaphore"));

    return *m_pLoggable; 
}