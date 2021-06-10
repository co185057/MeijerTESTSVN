#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/LoggableManager.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)LoggableManager.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/LoggableManager.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     2/28/05 3:33p Dm185016
 * lint
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 7     1/18/05 4:01p Dm185016
* UNICODE support
* 
* 6     10/04/04 6:58p Dm185016
* CLoggableManager object no longer shared.
* 
* 4     6/08/04 3:16p Dm185016
* Added hooks to data capture
* 
* 3     4/20/04 4:45p Dm185016
* New base class Singleton<>
* 
* 2     1/23/04 3:36p Dm185016
* DLL support
*/
#pragma warning( push )
#pragma warning( disable : 4251 )

#include "Loggable.h"
#include "Utils.h"
#include "Singleton.h"

#define MAX_LEVEL_SIZE 100

const int LEVEL_NONE    = 0;
const int LEVEL_DEBUG   = 100;


class CLoggableImpl
    : virtual public CBaseObject
    , public ILoggable
{
    friend class CLoggableManager;

public:
    
    virtual ~CLoggableImpl();

    virtual bool IsDebug() const;

    virtual bool IsTracing(int _level) const;

    virtual bool IsTracing() const;

private:
    
    CLoggableImpl(int _level=LEVEL_NONE);

    bool m_bDebug;

    int m_nLevel;

    DWORD m_dwMask;

};


class UTILS_API CLoggableManager
    : public CBaseObject
{
public:

    CLoggableManager(HTRACE);

    virtual ~CLoggableManager();

    ILoggable &getLoggable(const _TCHAR *pName = NULL);

    ILoggable &getLoggable(DWORD);

private:

    CLoggableManager();

    CLoggableMap m_mapLevels;

    CLoggableImpl m_alwaysLogging;
};
#pragma warning( pop )
