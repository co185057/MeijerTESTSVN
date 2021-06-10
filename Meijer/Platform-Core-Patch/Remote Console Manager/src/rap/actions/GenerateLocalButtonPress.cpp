// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/GenerateLocalButtonPress.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)GenerateLocalButtonPress.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/GenerateLocalButtonPress.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 8     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 6     1/13/06 3:46p Tk124825
 * TAR 301329
 * 
 * 3     2/28/05 4:12p Dm185016
 * lint
 * 
 * 2     1/26/05 2:43p Dm185016
 * Moved to new Repository.
// 
// 4     1/24/05 6:44p Tp151000
// TAR 290568
// 
// 3     1/12/05 5:37p Dm185016
// Cleaned up trace logs.
// 
// 2     12/16/04 9:56a Dm185016
// Rewritten to use a KV list for the parameters.  Support for Suspend
// Transaction.
// 
// 1     11/16/04 11:23a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "GenerateLocalButtonPress.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"
#include "SMError.h"
#include "UIDefines.h"
#include "PSXMfc.h"
#include "verifypsx.h"


BEGIN_KV_HANDLER_CLASS(CGenerateLocalButtonPress::GLBPParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CGenerateLocalButtonPress::GLBPParms, KEY_BUTTON_NAME, ButtonName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CGenerateLocalButtonPress::GLBPParms, KEY_CONTEXT_NAME, ContextName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CGenerateLocalButtonPress::GLBPParms, ButtonName)
DEFINE_KV_CSTRING(CGenerateLocalButtonPress::GLBPParms, ContextName)


void CGenerateLocalButtonPress::GLBPParms::Clear()
{
    m_ButtonName.Empty();
    m_ContextName.Empty();
}


CGenerateLocalButtonPress::CGenerateLocalButtonPress()
{
    CRCMActionBase::SetName(_T("CGenerateLocalButtonPress"));
}


CGenerateLocalButtonPress::~CGenerateLocalButtonPress()
{
}


// Get all control names from the xml
HRESULT CGenerateLocalButtonPress::ParseOperands()
{ 
    HRESULT hr = S_OK;

    m_parms.Clear();

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;
    
    m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());

    return hr;
}


HRESULT CGenerateLocalButtonPress::Execute(bool *pResult, IMessage &)
{
    IMODTRACE_DEVELOP();

    HRESULT hr = ParseOperands();

    ITRACE_DEVELOP(_T("Generating button press:  ") + IString(m_parms.GetButtonName()));
    ITRACE_DEVELOP(_T("                Context:  ") + IString(m_parms.GetContextName()));

    COleVariant vParam = _T("");
    StringType szLocalContext;
    szLocalContext = m_parms.GetContextName();
    
    if(szLocalContext == _T("local-context"))
    {
        szLocalContext = rapmgr().GetCurrentLaneContext();
    }

    ITRACE_DEVELOP(_T("                Context:  ") + IString(szLocalContext.c_str()));

    VERIFY_PSX(hr, psx().GenerateEvent( (const _TCHAR *)m_parms.GetButtonName()
                                      , szLocalContext.c_str()
                                      , UI::EVENTCLICK
                                      , vParam) );

    *pResult = true;

    return hr;
}

template <>
IAction* CCreateable<CGenerateLocalButtonPress, IAction>::NewInstance()
{
    IAction *pAction = new CGenerateLocalButtonPress;

    return pAction;
}


template <>
void CCreateable<CGenerateLocalButtonPress, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(GenerateLocalButtonPress)
DEFINE_ACTION_RELEASE_FXN(GenerateLocalButtonPress)
