// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/MakeInterventionCurrent.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)MakeInterventionCurrent.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/MakeInterventionCurrent.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 3     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     5/31/05 11:33a Dm185016
 * TAR 301203
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "MakeInterventionCurrent.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"


BEGIN_KV_HANDLER_CLASS(CMakeInterventionCurrent::MICParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CMakeInterventionCurrent::MICParms, KEY_TYPE, Type)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CMakeInterventionCurrent::MICParms, Type)
BEGIN_KV_HANDLER_CLASS_CLEAR(CMakeInterventionCurrent::MICParms)
    CLEAR_KV_CSTRING(Type)
END_KV_HANDLER_CLASS_CLEAR()


CMakeInterventionCurrent::CMakeInterventionCurrent()
{
    CRCMActionBase::SetName(_T("CMakeInterventionCurrent"));
}


CMakeInterventionCurrent::~CMakeInterventionCurrent()
{
}



// Get all control names from the xml
HRESULT CMakeInterventionCurrent::ParseOperands()
{ 
    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    if (iter == parms.end())
        return S_FALSE;
    
    return m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
}


HRESULT CMakeInterventionCurrent::Execute(bool *pResult, IMessage &)
{
    *pResult = true;

    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    CIntervention *pIntervention = rapmgr().GetInterventionByType(m_parms.GetType());
    _ASSERTE(pIntervention!=NULL);
    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Intervention type outstanding:  ") + IString(pIntervention!=NULL));

    if (pIntervention==NULL)
        return S_FALSE;

    rapmgr().SetCurrentIntervention(pIntervention);

    return S_OK;
}


template <>
IAction* CCreateable<CMakeInterventionCurrent, IAction>::NewInstance()
{
    IAction *pAction = new CMakeInterventionCurrent;

    return pAction;
}


template <>
void CCreateable<CMakeInterventionCurrent, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(MakeInterventionCurrent)
DEFINE_ACTION_RELEASE_FXN(MakeInterventionCurrent)
