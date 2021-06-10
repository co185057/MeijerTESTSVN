// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/PushbackDelayedIntervention.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)PushbackDelayedIntervention.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/PushbackDelayedIntervention.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 8     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 6     5/31/05 11:34a Dm185016
 * Cleaned up trace.
 * 
 * 3     4/04/05 4:09p Dm185016
 * TAR 297356
 * 
 * 2     2/28/05 4:12p Dm185016
 * lint
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
// 
// 1     11/30/04 4:18p Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "PushbackDelayedIntervention.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"


CPushbackDelayedIntervention::CPushbackDelayedIntervention()
{
    CRCMActionBase::SetName(_T("CPushbackDelayedIntervention"));
}


CPushbackDelayedIntervention::~CPushbackDelayedIntervention()
{
}


HRESULT CPushbackDelayedIntervention::Execute(bool *pResult, IMessage &)
{
    *pResult = true;

    CIntervention *pIntervention = rapmgr().GetCurrentIntervention();

    if (pIntervention == NULL)
    {
        ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  No intervention in progress."));
        return S_OK;
    }

    IMessage &newMsg 
        = stateMachine().MakeAsynchCopy(*dynamic_cast<IMessage *>(pIntervention));

    rapmgr().PushbackDelayedIntervention(&newMsg);

    return S_OK;
}


template <>
IAction* CCreateable<CPushbackDelayedIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CPushbackDelayedIntervention;

    return pAction;
}


template <>
void CCreateable<CPushbackDelayedIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(PushbackDelayedIntervention)
DEFINE_ACTION_RELEASE_FXN(PushbackDelayedIntervention)
