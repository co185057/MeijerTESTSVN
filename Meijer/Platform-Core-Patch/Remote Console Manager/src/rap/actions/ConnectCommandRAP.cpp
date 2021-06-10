// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ConnectCommandRAP.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ConnectCommandRAP.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ConnectCommandRAP.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     3/22/05 2:48p Dm185016
 * UNICODE
 * 
 * 2     3/07/05 6:53p Dm185016
 * Added indication of current exclusive access per Dan.
 * 
 * 1     2/25/05 5:16p Tk124825
 * Initial version
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ConnectCommandRAP.h"
#include "RemoteAPMgr.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"


CConnectCommandRAP::CConnectCommandRAP()
{
    CRCMActionBase::SetName(_T("CConnectCommandRAP"));
}


CConnectCommandRAP::~CConnectCommandRAP()
{
}


StringType CConnectCommandRAP::GetOperands()
{
    StringType sResult;

    const _TCHAR *pszInterventionType = NULL;

    // If we have an immediate intervention outstanding, then tell RAP
    // so that the alert button can be made visible and red.
    if( rapmgr().IsImmediateInterventionPending() )
    {
        pszInterventionType = KEY_CT_INTERVENTION_TYPE_IMMEDIATE;
    }

    // If we have a delayed intervention outstanding, then tell RAP
    // so that the alert button can be made visible and yellow.
    else if( rapmgr().IsDelayedInterventionPending() )
    {
        pszInterventionType = KEY_CT_INTERVENTION_TYPE_DELAYED;
    }

    // We don't have any interventions outstanding so tell RAP.
    else
    {
        pszInterventionType = KEY_CT_INTERVENTION_TYPE_NONE;
    }

    CKeyValueParmList parmlist;

   	CKeyValue<const _TCHAR*>::Put(parmlist, KEY_CT_INTERVENTION_TYPE,  pszInterventionType);
    CKeyValue<bool>::Put(parmlist, KEY_CT_ACQUIRE_INDICATION, rcm().IsExclusiveAccessGranted());

    SYSTEMTIME time;
    ::GetSystemTime(&time);

    CKeyValue<int>::Put(parmlist, KEY_CT_INTERVENTION_TIME, (int)time.wYear);
    CKeyValue<int>::Put(parmlist, (int)time.wMonth);
    CKeyValue<int>::Put(parmlist, (int)time.wDay);
    CKeyValue<int>::Put(parmlist, (int)time.wHour);
    CKeyValue<int>::Put(parmlist, (int)time.wMinute);
    CKeyValue<int>::Put(parmlist, (int)time.wSecond);

    ITRACE_DEVELOP(_T("Event:  ") + IString((const _TCHAR*)parmlist));

    sResult = (const _TCHAR *)parmlist;

    return sResult;
}


template <>
IAction* CCreateable<CConnectCommandRAP, IAction>::NewInstance()
{
    IAction *pAction = new CConnectCommandRAP;

    return pAction;
}


template <>
void CCreateable<CConnectCommandRAP, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ConnectCommandRAP)
DEFINE_ACTION_RELEASE_FXN(ConnectCommandRAP)
