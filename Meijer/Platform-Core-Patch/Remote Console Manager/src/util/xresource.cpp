// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/xresource.cpp 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)Resource.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
#include "xresource.h"
#include "xerror.h"
#include "Loggable.h"
#include "LoggableManager.h"


CResource::CResource( const _TCHAR *name )
    : m_pLoggable(NULL)
{
    Initialize(INVALID_HANDLE, name);
}


CResource::CResource(  HTRACE hTrace, const _TCHAR *name )
    : m_pLoggable(NULL)
{
    Initialize(hTrace, name);
}


void CResource::Initialize( HTRACE hTrace, const _TCHAR *name )
{
    keyName = (name == NULL ? _T("<Unknown>") : name);

    SetTraceObject(hTrace);

    hMutex = ::CreateMutex( NULL,
                            FALSE,
                            (const _TCHAR *)keyName);

    if ( hMutex == NULL)
    {
        if (IsTraceEnabled())
            ITRACE_ERROR(_T("Unable to create mutex semaphore.  Name=") + IString((name == NULL) ? _T("<Unnamed>") : name));
        ITHROWSYSTEMERROR(GetLastError(), _T("CreateMutex"), IException::unrecoverable)
    }
}


CResource::~CResource( )
{
    try
    {
        if (hMutex == NULL)
        {}
        else if (!::CloseHandle(hMutex))      /* Get rid of semaphore       */
        {
            IString errorTxt = Trace::formatLastError(_T("CloseHandle"));
            IString sysError(errorTxt);
            ITRACE_ERROR((const _TCHAR *)sysError);
        }

        hMutex		= NULL;
    }
    catch (...)
        { ITRACE_DOT_ERROR(); }
}


void CResource::SetName( const _TCHAR *pName )
{
    keyName = pName;
}


/*----------------------------- Resource Locking -----------------------------*/
CResource &CResource::Lock( unsigned long timeOut )
{
    if (IsTraceEnabled() && loggable().IsTracing())
        trace(L4, _T(".  Locking Resource:  %s  .Timeout:  %d")
                , (const _TCHAR *)keyName, timeOut);

    DWORD dwResult;
    switch (dwResult = ::WaitForSingleObject(hMutex, timeOut))
    {
        case WAIT_OBJECT_0 :
        case WAIT_ABANDONED :
            break;
        case WAIT_TIMEOUT :
        default :
        	ITHROWSYSTEMERROR(GetLastError(), WaitForSingleObject, IException::unrecoverable)
    }

    if (IsTraceEnabled() && loggable().IsTracing())
        trace(L4, _T(".  Obtained lock.  Result:  %d")
                , dwResult);

    return *this;
}


CResource &CResource::Unlock         ( )
{
    if (IsTraceEnabled() && loggable().IsTracing())
        trace(L4, _T(".  Unlocking Resource:  %s")
                , (const _TCHAR *)keyName);

    if (!::ReleaseMutex(hMutex))
    {
        ITHROWSYSTEMERROR(GetLastError(), _T("ReleaseMutex"), IException::unrecoverable)
    } // endif

    if (IsTraceEnabled() && loggable().IsTracing())
        trace(L4, _T(".  Lock released"));

    return *this;
}


ILoggable &CResource::loggable()
{ 
    if (m_pLoggable == NULL)
        m_pLoggable = &Trace::getLoggable(GetTraceHandle(), eDCWin32Utils);

    return *m_pLoggable; 
}


/*------------------------------- Constructors -------------------------------*/
CSharedResource::CSharedResource   (  HTRACE hTrace, const _TCHAR* pszKeyName )
    : CResource(hTrace, pszKeyName)
{}


CSharedResource::~CSharedResource   ( )
{
}
