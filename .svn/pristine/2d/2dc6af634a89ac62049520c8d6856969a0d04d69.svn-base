// $Header: /Fastlane/4.0 Patch B/Component Utilities/src/util/Traceable.h 1     1/26/05 2:32p Dm185016 $
#pragma once
/*
 * @(#)Traceable.h    $Revision: 1 $ $Date: 1/26/05 2:32p $
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
* $Log: /Fastlane/4.0 Patch B/Component Utilities/src/util/Traceable.h $
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 1     10/04/04 6:53p Dm185016
*/

#include "Utils.h"

class UTILS_API Trace;

class UTILS_API Traceable
{
public:
    Traceable() {};

    virtual ~Traceable() {};

    virtual Trace &GetTraceObject() const=0;

    virtual void SetTraceObject(Trace *)=0;

private:

    Traceable(const Traceable &);
    Traceable &operator =(const Traceable &); // Hidden assignment operator
};
