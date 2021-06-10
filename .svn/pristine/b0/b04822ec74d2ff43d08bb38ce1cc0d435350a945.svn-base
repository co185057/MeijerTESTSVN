// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/GotoActionBase.cpp 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)GotoActionBase.cpp    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/GotoActionBase.cpp $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     2/28/05 3:50p Dm185016
 * lint
 * 
 * 2     2/10/05 5:45p Dm185016
 * Deleted trace.
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
// 
// 2     1/18/05 3:56p Dm185016
// UNICODE support
// 
// 1     11/09/04 3:37p Dm185016
// Support for state lookup tables
*/
#include "StdAfx.h"
#include "GotoActionBase.h"
#include "StateLookupTable.h"
#include "State.h"
#include "SMError.h"
#include "InternalOps.h"
#include "Variable.h"

using namespace STATE_MACHINE_NAMESPACE;


//-----------------------------------------------------------------------------
// No-arg constructor
//-----------------------------------------------------------------------------
CGotoActionBase::CGotoActionBase()
{
}


CGotoActionBase::~CGotoActionBase()
{
}


// Get all control names from the xml
HRESULT CGotoActionBase::NextOperands(CParameterList::iterator &iter, CParameterList::iterator &end)
{ 
    HRESULT hr = S_OK;

    m_sStateTableName.clear();

    // Get the name of the state translation table
    if (iter != end)
    {
        m_sStateTableName = *iter;
        iter++;
    }
    else
        hr = STATEM_MISSING_PARAMETER;

    return hr;
}


HRESULT CGotoActionBase::StateLookup(int &index,
                                     const _TCHAR *pszStateTableName,
                                     const _TCHAR *pszKey)
{
    IStateLookupTable *pLookupTable = NULL;
    HRESULT hr = stateMachine().GetStateLookupTable(&pLookupTable, pszStateTableName);
    if (hr != S_OK)
        return hr;

    hr = pLookupTable->Lookup(index, pszKey);

    return hr;
}


HRESULT CGotoActionBase::SetNextState(const _TCHAR *pszStateTableName,
                                      const _TCHAR *pszKey)
{
    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  State Table Name:  ") + IString(pszStateTableName) + _T("  Key:  ") + IString(pszKey));
    int nStateIndex;
    HRESULT hr = StateLookup(nStateIndex, pszStateTableName, pszKey);
    if (hr != S_OK)
        return hr;
    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  nStateIndex:  ") + IString(nStateIndex));

    return SetNextState(nStateIndex);
}


HRESULT CGotoActionBase::SetNextState(int nNextState)
{
    stateMachine().GotoState(nNextState);

    return S_OK;
}
