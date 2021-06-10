#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/Semaphore.h 1     6/20/08 1:13p Sa250050 $
/*
* @(#)Semaphore.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/Semaphore.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     2/28/05 3:34p Dm185016
 * lint
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 6     1/18/05 4:02p Dm185016
* UNICODE support
* 
* 5     10/04/04 6:56p Dm185016
* Trace object is no longer shared.
* 
* 4     9/07/04 3:28p Dm185016
* TAR 277300 - Added (HANDLE) operator
* 
* 3     6/30/04 3:45p Dm185016
* Lint.
* Data Capture tracing.
* 
* 2     2/10/04 11:53a Dm185016
* Moved to utils namespace.
* 
* 1     1/23/04 3:40p Dm185016
* New DLL Support
*/
//lint -esym(534, CSemaphore::*)

#include "Utils.h"

class ILoggable;        //lint !e763

namespace utils
{

class UTILS_API CSemaphore
    : public CBaseObject
{

public:

    CSemaphore(HTRACE, const _TCHAR *pszName = NULL);

    virtual ~CSemaphore();

    CSemaphore &Post();

    /*
    * Waits for the semaphore to be posted.  The wait interval defaults to indefinite if not specified.
    * Returns true if the semaphore was posted.  Returns false if timeout occurred.
    */
    bool Wait(unsigned long wait = INFINITE);

    CSemaphore &Reset(unsigned long &);

    inline HANDLE GetHandle() throw () { return (HANDLE)hSema4; }

    //lint --e{1763}
    inline operator HANDLE ( ) const throw() 
    {
        IMODTRACE_DEVELOP();
        return (HANDLE)hSema4; 
    }

    void Debug();

protected:

    CSemaphore &Open(const _TCHAR *pszName = NULL);
    CSemaphore &Close();

private:

    CSemaphore();

    ILoggable &loggable();

    HANDLE    hSema4;

    const _TCHAR *pszName;

    ILoggable *m_pLoggable;
};

}