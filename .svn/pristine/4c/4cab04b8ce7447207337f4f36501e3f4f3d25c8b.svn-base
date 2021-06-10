// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearAllInterventions.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ClearAllInterventions.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearAllInterventions.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 3     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     5/31/05 11:33a Dm185016
 * TAR 301203
 * 
 * 1     5/19/05 5:19p Dm185016
 * TAR 300190
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ClearAllInterventions.h"
#include "RemoteAPMgr.h"
#include "Message.h"
#include "SMConstants.h"
#include "algorithms/ClearInterventions.h"


CClearAllInterventions::CClearAllInterventions()
{
    CRCMActionBase::SetName(_T("CClearAllInterventions"));
}


CClearAllInterventions::~CClearAllInterventions()
{
}


bool CClearAllInterventions::accept(IAction *, const CIntervention *)
{
    return true;
}


HRESULT CClearAllInterventions::Execute(bool *pResult, IMessage &)
{
    *pResult = true;

    HRESULT hr = ClearInterventions(this, &CClearAllInterventions::accept);

    return hr;
}


template <>
IAction* CCreateable<CClearAllInterventions, IAction>::NewInstance()
{
    IAction *pAction = new CClearAllInterventions;

    return pAction;
}


template <>
void CCreateable<CClearAllInterventions, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ClearAllInterventions)
DEFINE_ACTION_RELEASE_FXN(ClearAllInterventions)
