// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearLaneStatusWindows.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ClearLaneStatusWindows.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearLaneStatusWindows.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 5     1/13/06 2:28p Dm185016
 * TAR 301329
 * 
 * 2     3/07/05 4:08p Tp151000
 * 294738
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
// 
// 4     1/13/05 1:27p Dm185016
// Removed trace.
// 
// 3     1/12/05 5:37p Dm185016
// Cleaned up trace logs.
// 
// 2     1/12/05 10:44a Tp151000
// TAR 289899
// 
// 1     12/14/04 11:13a Dm185016
// TAR 288478  Added actions to clear button text
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ClearLaneStatusWindows.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "RCMLaneXML.h"
#include "RCMError.h"
#include "PSXMfc.h"
#include "KVParmList.h"
#include "verifypsx.h"


CClearLaneStatusWindows::CClearLaneStatusWindows()
{
    CRCMActionBase::SetName(_T("CClearLaneStatusWindows"));
}


CClearLaneStatusWindows::~CClearLaneStatusWindows()
{
}


HRESULT CClearLaneStatusWindows::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    VERIFY_PSX(hr, psx().SetControlProperty(UI::CONTROLDISPLAY, UI::PROPERTYREDRAW, VARIANT_FALSE));
    SetTransactionVariable(GetInstructionTextVariableName(), _T(""));
    SetText(GetLaneButtonTextControlName(), _T(""));
    SetTransactionVariable(GetLaneStatus1ControlName(), _T(""));
    SetTransactionVariable(GetLaneStatus2ControlName(), _T(""));
    SetTransactionVariable(GetLaneStatus3ControlName(), _T(""));
    VERIFY_PSX(hr, psx().SetControlProperty(UI::CONTROLDISPLAY, UI::PROPERTYREDRAW, VARIANT_TRUE));

    *pResult = true;

    return hr;
}


template <>
IAction* CCreateable<CClearLaneStatusWindows, IAction>::NewInstance()
{
    IAction *pAction = new CClearLaneStatusWindows;

    return pAction;
}


template <>
void CCreateable<CClearLaneStatusWindows, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ClearLaneStatusWindows)
DEFINE_ACTION_RELEASE_FXN(ClearLaneStatusWindows)
