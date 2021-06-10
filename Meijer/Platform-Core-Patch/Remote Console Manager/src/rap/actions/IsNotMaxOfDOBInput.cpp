// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsNotMaxOfDOBInput.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)IsNotMaxOfDOBInput.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsNotMaxOfDOBInput.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     3/22/05 2:46p Dm185016
 * 
 * 2     2/28/05 4:12p Dm185016
 * lint
 * 
 * 1     2/08/05 5:31p Tp151000
 * TAR 292777
 * 
 * 1     2/08/05 11:03a Tp151000
 * 290952
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
// 
// 2     1/12/05 5:37p Dm185016
// Cleaned up trace logs.
// 
// 1     11/16/04 11:23a Dm185016
*/
#include "StdAfx.h"

#include "IsNotMaxOfDOBInput.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "LocaleEx.h"

using namespace rap;
using namespace fastlane;
using namespace utils;


CIsNotMaxOfDOBInput::CIsNotMaxOfDOBInput()
{
    CRCMActionBase::SetName(_T("CIsNotMaxOfDOBInput"));
}


CIsNotMaxOfDOBInput::~CIsNotMaxOfDOBInput()
{
}

HRESULT CIsNotMaxOfDOBInput::ParseOperands()
{ 
//  IMODTRACE_DEVELOP();
    HRESULT hr = S_OK;
    
    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();
    if (iter == parms.end())
    {
        ITRACE_ERROR(_T("Missing variable name"));
        return SECURITY_MISSING_VARIABLE_NAME;
    }
    StringType sVariableName = *iter;
    hr = rcmStateMachine().GetVariable(&m_pVariable, sVariableName.c_str());
    return hr;
}

HRESULT CIsNotMaxOfDOBInput::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = S_OK;
    *pResult = true;

    hr = ParseOperands();
    *pResult = IsMaxValue();
    return hr;
}
bool CIsNotMaxOfDOBInput::IsMaxValue()
{
    bool retValue = true;

    int value = m_pVariable->GetValue();
    int maxvalue;
    maxvalue = CLocale::GetDateFormatCount();
            
    if (value > maxvalue)
        retValue = false;
            
    ITRACE_DEVELOP(_T("Is Max Value:  ") + IString(m_pVariable->GetName()) + _T(".  Value:  ") + IString(value));

    return retValue;
}
template <>
IAction* CCreateable<CIsNotMaxOfDOBInput, IAction>::NewInstance()
{
    IAction *pAction = new CIsNotMaxOfDOBInput;

    return pAction;
}


template <>
void CCreateable<CIsNotMaxOfDOBInput, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsNotMaxOfDOBInput)
DEFINE_ACTION_RELEASE_FXN(IsNotMaxOfDOBInput)
