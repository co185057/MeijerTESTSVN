/*
 * @(#)SwitchContext.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/SwitchContext.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
// 
// 5     1/18/05 3:52p Dm185016
// UNICODE support
// 
// 4     1/13/05 1:42p Dm185016
// Cleaned up trace.
// 
// 3     1/12/05 6:10p Dm185016
// Cleaned up trace log.
// 
// 2     10/29/04 9:15a Dm185016
// Filled in fxn.
// 
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SwitchContext.h"
#include "RCManager.h"
#include "PSXMfc.h"
#include "verifypsx.h"


CSwitchContext::CSwitchContext()
{
    CRCMActionBase::SetName(_T("CSwitchContext"));
}


CSwitchContext::~CSwitchContext()
{
}


HRESULT CSwitchContext::ParseOperands()
{ 
    HRESULT hr = S_OK;

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();
    m_sNewContext = *iter;

    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  m_sNewContext:  ") + IString(m_sNewContext.c_str()));

    return hr;
}


HRESULT CSwitchContext::Execute(bool *pResult, IMessage &)
{
	HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    CPSX &psxInstance = psx();

    VERIFY_PSX(hr, psxInstance.SetContext( m_sNewContext.c_str(), DisplayAlternate1 ) );

    if (hr == S_OK)
        rcm().SetCurrentContext(m_sNewContext.c_str());

    *pResult = true;

    return hr;
}


template <>
IAction* CCreateable<CSwitchContext, IAction>::NewInstance()
{
    IAction *pAction = new CSwitchContext;

    return pAction;
}


template <>
void CCreateable<CSwitchContext, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SwitchContext)
DEFINE_ACTION_RELEASE_FXN(SwitchContext)
