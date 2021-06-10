// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/BaseObject.cpp 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)BaseObject.cpp    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/BaseObject.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 6     4/21/05 7:30p Dm185016
 * 
 * 4     3/22/05 3:47p Dm185016
 * UNICODE
 * 
 * 3     3/11/05 1:16p Dm185016
 * Added static method ToString()
 * 
 * 2     2/28/05 3:29p Dm185016
 * lint
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
// 
// 3     1/18/05 4:00p Dm185016
// UNICODE support
// 
// 2     1/07/05 3:43p Dm185016
// Added debugging method ToString().
// 
// 1     10/04/04 6:53p Dm185016
*/

//lint --e{1550, 1763}

#include "StdAfx.h"
#include "BaseObject.h"


CBaseObject::CBaseObject()
    : m_pTrace(NULL)
    , m_hTrace(INVALID_HANDLE)
{}


CBaseObject::CBaseObject(const CBaseObject &theOther)
{
    CBaseObject::SetTraceObject(theOther.m_hTrace);
}


CBaseObject::CBaseObject(HTRACE hTrace)
{
    CBaseObject::SetTraceObject(hTrace);
}


Trace &CBaseObject::GetTraceObject() const throw()
{
    if (m_pTrace==NULL)
    {
        if (IsTraceEnabled())
            m_pTrace = &Trace::GetTraceObject(m_hTrace);
    }

    _ASSERTE(m_pTrace!=NULL);

    return *m_pTrace;
}


HTRACE CBaseObject::GetTraceHandle() const
{
    return m_hTrace;
}


bool CBaseObject::IsTraceEnabled() const
{
    return (m_hTrace != INVALID_HANDLE);
}


void CBaseObject::SetTraceObject(Trace *pTrace)
{
    m_pTrace = pTrace;
    m_hTrace = pTrace->Handle();
}


void CBaseObject::SetTraceObject(HTRACE hTrace)
{
    m_pTrace = Trace::FindTraceObject(hTrace);
    m_hTrace = hTrace;
}


StringType CBaseObject::ToString() throw()
{
    StringType sRetValue = _T("<Unknown Type>");
    try
    {
        CA2T sName(typeid(*this).name());
        sRetValue = (const _TCHAR *)IString(sName);
    }
    catch (...) {}

    return sRetValue;
}


StringType CBaseObject::ToString(CBaseObject *pObject) throw()
{
    return pObject->ToString();
}
