#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/Loggable.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)Loggable.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/Loggable.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 4     6/08/04 3:16p Dm185016
* Added hooks to data capture
* 
* 3     5/18/04 7:19p Dm185016
* lint
* 
* 2     1/23/04 3:36p Dm185016
* DLL support
*/
#include "Utils.h"
#include "StringComparator.h"
#include <map>

class UTILS_API ILoggable
{
public:

    virtual ~ILoggable() {}

    virtual bool IsDebug() const=0;

    virtual bool IsTracing(int _level) const=0;

    virtual bool IsTracing() const=0;

};

typedef ILoggable *PLOGGABLE;
typedef std::map<StringType, PLOGGABLE, StringComparator> CLoggableMap;
typedef CLoggableMap::iterator CLoggableIterator;
