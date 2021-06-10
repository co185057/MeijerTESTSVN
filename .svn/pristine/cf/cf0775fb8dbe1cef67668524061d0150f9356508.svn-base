// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsOutsideTransaction.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)IsOutsideTransaction.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsOutsideTransaction.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     4/05/05 11:56a Dm185016
 * TAR 297356 - Made IEvaluateable for <if>
 * 
 * 2     2/28/05 4:12p Dm185016
 * lint
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
// 
// 1     12/02/04 3:33p Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "IsOutsideTransaction.h"
#include "RemoteAPMgr.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"
#include "RCMLaneXML.h"
#include "PSXMfc.h"
#include "UIDefines.h"


CIsOutsideTransaction::CIsOutsideTransaction()
{
    CRCMActionBase::SetName(_T("CIsOutsideTransaction"));
}


CIsOutsideTransaction::~CIsOutsideTransaction()
{
}


HRESULT CIsOutsideTransaction::Execute(bool *pResult, IMessage &)
{
    *pResult = rapmgr().IsOutsideTransaction();

    return S_OK;
}


long CIsOutsideTransaction::GetValue() const { return 0; }


void CIsOutsideTransaction::SetValue(long) {}


template <>
IAction* CCreateable<CIsOutsideTransaction, IAction>::NewInstance()
{
    IAction *pAction = new CIsOutsideTransaction;

    return pAction;
}


template <>
void CCreateable<CIsOutsideTransaction, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsOutsideTransaction)
DEFINE_ACTION_RELEASE_FXN(IsOutsideTransaction)
