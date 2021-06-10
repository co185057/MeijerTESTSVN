// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendAllInterventionEvents.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SendAllInterventionEvents.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendAllInterventionEvents.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:15p Dm185016
 * lint
 * 
 * 1     2/17/05 5:55p Dm185016
 * Alert buttons functionality
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SendAllInterventionEvents.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "RCMLaneXML.h"
#include "RAPUIDefines.h"
#include "PSXMfc.h"
#include "KVParmList.h"
#include "verifypsx.h"


CSendAllInterventionEvents::CSendAllInterventionEvents()
{
    CRCMActionBase::SetName(_T("CSendAllInterventionEvents"));
}


CSendAllInterventionEvents::~CSendAllInterventionEvents()
{
}


const _TCHAR *CSendAllInterventionEvents::GetInterventionType()
{
    return m_pszInterventionType;
}


const _TCHAR *CSendAllInterventionEvents::GetOperation()
{
    return _T("");
}


bool CSendAllInterventionEvents::IsOperationRequired() const
{
    return true;
}


HRESULT CSendAllInterventionEvents::Execute(bool *pResult, IMessage &)
{
    m_pszInterventionType = NULL;

    // If we have an immediate intervention outstanding, then tell RAP
    // so that the alert button can be made visible and red.
    if( rapmgr().IsImmediateInterventionPending() )
    {
        m_pszInterventionType = KEY_CT_INTERVENTION_TYPE_IMMEDIATE;
        SendEvent();
    }

    // If we have a delayed intervention outstanding, then tell RAP
    // so that the alert button can be made visible and yellow.
    else if( rapmgr().IsDelayedInterventionPending() )
    {
        m_pszInterventionType = KEY_CT_INTERVENTION_TYPE_DELAYED;
        SendEvent();
    }

    // We don't have any interventions outstanding so tell RAP.
    else
    {
        m_pszInterventionType = KEY_CT_INTERVENTION_TYPE_NONE;
        SendEvent();
    }

    *pResult = true;

    return S_OK;
}


template <>
IAction* CCreateable<CSendAllInterventionEvents, IAction>::NewInstance()
{
    IAction *pAction = new CSendAllInterventionEvents;

    return pAction;
}


template <>
void CCreateable<CSendAllInterventionEvents, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SendAllInterventionEvents)
DEFINE_ACTION_RELEASE_FXN(SendAllInterventionEvents)
