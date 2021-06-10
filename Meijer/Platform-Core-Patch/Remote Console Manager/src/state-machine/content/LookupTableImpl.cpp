// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/LookupTableImpl.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)LookupTableImpl.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
 *
 * Copyright 2005 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/LookupTableImpl.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 6     4/18/05 7:18p Dm185016
 * Toggle support
 * 
 * 4     3/22/05 4:34p Dm185016
 * .000 merge support
 * 
 * 3     3/03/05 3:18p Dm185016
 * TAR 291700 - Changed type of value-type to integer from string.
 * 
 * 2     2/28/05 3:46p Dm185016
 * lint
 * 
 * 1     2/14/05 7:06p Dm185016
 * Release Exclusive Access Event support
*/
#include "StdAfx.h"
#include "SMConstants.h"
#include "LookupTableImpl.h"
#include "StateTableHandler.h"
#include "SMError.h"

using namespace STATE_MACHINE_NAMESPACE;


CLookupTableImpl::CLookupTableImpl(int nId)
	: m_nId(nId)
{
}


CLookupTableImpl::~CLookupTableImpl(void)
{
    m_mapLookups.clear();
}


DEFINE_STRING_PROPERTY(CLookupTableImpl, Name);
DEFINE_STRING_PROPERTY(CLookupTableImpl, Key);
DEFINE_STRING_PROPERTY(CLookupTableImpl, Value);
DEFINE_STRING_PROPERTY(CLookupTableImpl, Comment);
DEFINE_STRING_PROPERTY(CLookupTableImpl, KeyType);
DEFINE_INT_PROPERTY(CLookupTableImpl, ValueType);


int CLookupTableImpl::GetId() const
{
	return m_nId;
}


const _TCHAR *CLookupTableImpl::GetName()
{
    return Name.Value().c_str();
}


const _TCHAR *CLookupTableImpl::GetComment()
{
    return Comment.Value().c_str();
}


void CLookupTableImpl::AddEntry()
{
    m_mapLookups.insert(std::make_pair(Key.Value(), Value.Value()));
}


void CLookupTableImpl::Merge(const CLookupTableImpl *pLti)
{
    IMODTRACE_DEVELOP();

    map_t::const_iterator iter = pLti->m_mapLookups.begin();
    for (;iter!=pLti->m_mapLookups.end(); iter++)
    {
        Key.Value(iter->first);
        Value.Value(iter->second);

        ITRACE_DEVELOP(_T("Merging entry:  ") + IString((iter->first).c_str()) + _T("=") + IString((iter->second).c_str()));

        AddEntry();
    }
}


HRESULT CLookupTableImpl::Lookup(int nKey )
{
    IString sKey(nKey);

    CLookupTableImpl::map_t::iterator iter = m_mapLookups.find((const _TCHAR *)sKey);
    if (iter == m_mapLookups.end())
        return STATEM_UNDEFINED_STATE_ID;

    m_value = iter->second;

    return S_OK;
}


int CLookupTableImpl::GetInteger() const
{
    IString sString = m_value.c_str();

    return sString.asInt();
}


const _TCHAR *CLookupTableImpl::GetString() const
{
    return m_value.c_str();
}


HRESULT CLookupTableImpl::Lookup(const _TCHAR *pszKey)
{
    CLookupTableImpl::map_t::iterator iter = m_mapLookups.find(pszKey);
    if (iter == m_mapLookups.end())
        return STATEM_UNDEFINED_STATE_ID;

    m_value = iter->second;

    return S_OK;
}


StringType CLookupTableImpl::GetXMLOutput(CXMLBuilder *pXMLBuilder)
{
    if (pXMLBuilder == NULL)
        ((CXMLBuilder *)CStateTableHandler::instance())->GetXMLOutput(this);
    else
        pXMLBuilder->GetXMLOutput(this);

    return (pXMLBuilder == NULL ? CStateTableHandler::instance() : pXMLBuilder)->GetXMLOutput();
}
