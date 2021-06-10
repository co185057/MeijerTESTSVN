// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SaveImmediateIntervention.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SaveImmediateIntervention.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SaveImmediateIntervention.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     3/03/05 3:13p Dm185016
 * TAR 291700
 * 
 * 2     2/03/05 5:42p Dm185016
 * TAR 292378 - Converted single immediate intervention to use of stack
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
// 
// 2     1/12/05 5:37p Dm185016
// Cleaned up trace logs.
// 
// 1     10/29/04 8:53a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SaveImmediateIntervention.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"


BEGIN_KV_HANDLER_CLASS(CSaveImmediateIntervention::SIIParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSaveImmediateIntervention::SIIParms, KEY_EV_TYPE, Type)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CSaveImmediateIntervention::SIIParms, Type)


void CSaveImmediateIntervention::SIIParms::Clear()
{
    m_Type.Empty();
}


CSaveImmediateIntervention::CSaveImmediateIntervention()
{
    CRCMActionBase::SetName(_T("CSaveImmediateIntervention"));
}


CSaveImmediateIntervention::~CSaveImmediateIntervention()
{
}


HRESULT CSaveImmediateIntervention::Execute(bool *pResult, IMessage &message)
{

    SIIParms parms;

    HRESULT hr = parms.ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    IMessage *pNewMsg = NULL;

    if (parms.GetType() == KEY_EV_TYPE_RAP_DATA_NEEDED)
        pNewMsg = &stateMachine().MakeAsynchCopy(message);
    else
        pNewMsg = &stateMachine().MakeAsynchCopyEx(message, MSG_IMMEDIATE_INTERVENTION);

    rapmgr().SetImmediateIntervention(pNewMsg);

    *pResult = true;

    return S_OK;
}


template <>
IAction* CCreateable<CSaveImmediateIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CSaveImmediateIntervention;

    return pAction;
}


template <>
void CCreateable<CSaveImmediateIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SaveImmediateIntervention)
DEFINE_ACTION_RELEASE_FXN(SaveImmediateIntervention)
