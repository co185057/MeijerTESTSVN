// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/PushbackImmediateIntervention.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)PushbackImmediateIntervention.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/PushbackImmediateIntervention.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 4     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     5/02/05 2:58p Tp151000
 * TAR 299442
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "PushbackImmediateIntervention.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"


CPushbackImmediateIntervention::CPushbackImmediateIntervention()
{
    CRCMActionBase::SetName(_T("CPushbackImmediateIntervention"));
}


CPushbackImmediateIntervention::~CPushbackImmediateIntervention()
{
}


HRESULT CPushbackImmediateIntervention::Execute(bool *pResult, IMessage &)
{
    IMODTRACE_DEVELOP();

    *pResult = true;

    CIntervention *pIntervention = rapmgr().GetCurrentIntervention();

    if (pIntervention == NULL)
    {
        ITRACE_DEVELOP(_T(__FUNCTION__) _T("No intervention in progress."));
        return S_OK;
    }

    IMessage &newMsg 
        = stateMachine().MakeAsynchCopy(*dynamic_cast<IMessage *>(pIntervention));

    rapmgr().PushbackImmediateIntervention(&newMsg);

    return S_OK;
}


template <>
IAction* CCreateable<CPushbackImmediateIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CPushbackImmediateIntervention;

    return pAction;
}


template <>
void CCreateable<CPushbackImmediateIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(PushbackImmediateIntervention)
DEFINE_ACTION_RELEASE_FXN(PushbackImmediateIntervention)
