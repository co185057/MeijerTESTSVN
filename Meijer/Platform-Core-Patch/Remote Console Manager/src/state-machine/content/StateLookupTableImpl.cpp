// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/StateLookupTableImpl.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)StateLookupTableImpl.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/StateLookupTableImpl.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     3/22/05 4:34p Dm185016
 * .000 merge support
 * 
 * 2     2/10/05 5:54p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 1     1/26/05 2:29p Dm185016
 * Moved to new Repository.
// 
// 1     11/09/04 3:38p Dm185016
// Support for state lookup tables
*/
#include "StdAfx.h"
#include "SMConstants.h"
#include "StateLookupTableImpl.h"
#include "StateTableHandler.h"
#include "SMError.h"

using namespace STATE_MACHINE_NAMESPACE;


CStateLookupTableImpl::CStateLookupTableImpl(int nId)
	: m_nId(nId)
{
}


CStateLookupTableImpl::~CStateLookupTableImpl(void)
{
    m_mapLookups.clear();
}


DEFINE_STRING_PROPERTY(CStateLookupTableImpl, Name);
DEFINE_STRING_PROPERTY(CStateLookupTableImpl, Key);
DEFINE_STRING_PROPERTY(CStateLookupTableImpl, StateName);
DEFINE_STRING_PROPERTY(CStateLookupTableImpl, Comment);
DEFINE_INT_PROPERTY(CStateLookupTableImpl, StateId);


int CStateLookupTableImpl::GetId() const
{
	return m_nId;
}


const _TCHAR *CStateLookupTableImpl::GetName()
{
    return Name.Value().c_str();
}


const _TCHAR *CStateLookupTableImpl::GetComment()
{
    return Comment.Value().c_str();
}


void CStateLookupTableImpl::AddEntry()
{
    m_mapLookups.insert(std::make_pair(Key.Value(), StateId.Value()));
}


void CStateLookupTableImpl::Merge(const CStateLookupTableImpl *pLti)
{
    IMODTRACE_DEVELOP();

    map_t::const_iterator iter = pLti->m_mapLookups.begin();
    for (;iter!=pLti->m_mapLookups.end(); iter++)
    {
        Key.Value(iter->first);
        StateId.Value(iter->second);

        ITRACE_DEVELOP(_T("Merging entry:  ") + IString((iter->first).c_str()) + _T("=") + IString(iter->second));

        AddEntry();
    }
}


HRESULT CStateLookupTableImpl::Lookup(int &index,
                                  const _TCHAR *pszKey)
{
    CLookupTable::iterator iter = m_mapLookups.find(pszKey);
    if (iter == m_mapLookups.end())
        return STATEM_UNDEFINED_STATE_ID;

    index = iter->second;

    return S_OK;
}


StringType CStateLookupTableImpl::GetXMLOutput(CXMLBuilder *pXMLBuilder)
{
    if (pXMLBuilder == NULL)
        ((CXMLBuilder *)CStateTableHandler::instance())->GetXMLOutput(this);
    else
        pXMLBuilder->GetXMLOutput(this);

    return (pXMLBuilder == NULL ? CStateTableHandler::instance() : pXMLBuilder)->GetXMLOutput();
}
