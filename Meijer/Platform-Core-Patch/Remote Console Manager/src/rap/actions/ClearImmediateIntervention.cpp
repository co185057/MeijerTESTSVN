// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearImmediateIntervention.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ClearImmediateIntervention.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearImmediateIntervention.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 8     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 4     2/28/05 4:08p Dm185016
 * lint
 * 
 * 3     2/14/05 7:07p Dm185016
 * Fixed problem delete not calling destructor for IMessage.
 * 
 * 2     2/03/05 5:42p Dm185016
 * TAR 292378 - Converted single immediate intervention to use of stack
 * 
 * 1     1/26/05 2:19p Dm185016
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

#include "ClearImmediateIntervention.h"
#include "RemoteAPMgr.h"
#include "Message.h"


CClearImmediateIntervention::CClearImmediateIntervention()
{
    CRCMActionBase::SetName(_T("CClearImmediateIntervention"));
}


CClearImmediateIntervention::~CClearImmediateIntervention()
{
}


HRESULT CClearImmediateIntervention::Execute(bool *pResult, IMessage &)
{
    CIntervention *pIntervention = rapmgr().GetNextImmediateIntervention();
    while (pIntervention != NULL)
    {
        delete pIntervention;

        pIntervention = rapmgr().GetNextImmediateIntervention();
    }

    *pResult = true;

    return S_OK;
}


template <>
IAction* CCreateable<CClearImmediateIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CClearImmediateIntervention;

    return pAction;
}


template <>
void CCreateable<CClearImmediateIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ClearImmediateIntervention)
DEFINE_ACTION_RELEASE_FXN(ClearImmediateIntervention)
