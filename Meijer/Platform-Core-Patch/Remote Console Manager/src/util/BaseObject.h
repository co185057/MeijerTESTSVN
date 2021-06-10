// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/BaseObject.h 1     6/20/08 1:13p Sa250050 $
#pragma once
/*
 * @(#)BaseObject.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/BaseObject.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 5     4/21/05 7:30p Dm185016
 * 
 * 3     3/11/05 1:16p Dm185016
 * Added static method ToString()
 * 
 * 2     2/28/05 3:29p Dm185016
 * lint
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 2     1/07/05 3:43p Dm185016
* Added debugging method ToString().
* 
* 1     10/04/04 6:53p Dm185016
*/

#include "xstring.h"
#include "Utils.h"
#include "Traceable.h"
#define INVALID_HANDLE (-1)

#define CTOR_BEGIN(HTRACE)\
    CBaseObject::SetTraceObject(HTRACE);\
    {

#define CTOR_END()\
    }

class UTILS_API Trace;

class UTILS_API CBaseObject
    : public Traceable
{
public:

    virtual ~CBaseObject() {};

    virtual Trace &GetTraceObject() const throw();

    virtual HTRACE GetTraceHandle() const;

    virtual void SetTraceObject(Trace *);

    virtual void SetTraceObject(HTRACE);

    virtual bool IsTraceEnabled() const;

    static Trace &GetDefaultTraceObject();

    virtual StringType ToString() throw();

    static StringType ToString(CBaseObject *) throw();

protected:

    CBaseObject();
    CBaseObject(const CBaseObject &);

    CBaseObject(HTRACE);

private:

    CBaseObject &operator =(const CBaseObject &); // Hidden assignment operator

    mutable Trace   *m_pTrace;
    HTRACE  m_hTrace;
};
