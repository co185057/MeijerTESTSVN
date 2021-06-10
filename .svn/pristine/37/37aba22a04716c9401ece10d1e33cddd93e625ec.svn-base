// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/SelectAction.cpp 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)SelectAction.cpp    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/SelectAction.cpp $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     3/22/05 2:50p Dm185016
 * UNICODE
 * 
 * 2     2/28/05 3:48p Dm185016
 * lint
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
// 
// 1     10/29/04 9:27a Dm185016
*/
#include "StdAfx.h"
#include "SelectAction.h"
#include "Expression.h"

using namespace STATE_MACHINE_NAMESPACE;


CSelectAction::CSelectAction()
{
    SetName(_T("CSelectAction"));
    Comment.Value(_T("Evaluates the given expression."));
}


CSelectAction::~CSelectAction()
{
}


bool CSelectAction::IsInternal() 
{
    return true; 
}


HRESULT CSelectAction::ParseOperands()
{ 
    HRESULT hr = S_OK;

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();
    if (iter == parms.end())
    {
        ITRACE_ERROR(_T("Missing expression"));
        return SECURITY_MISSING_EXPRESSION;
    }
    StringType sExpressionName = *iter;
    hr = stateMachine().GetExpression(&m_pExpression, sExpressionName.c_str());

    return hr;
}


HRESULT CSelectAction::Execute(bool *pResult, IMessage &)
{
    ParseOperands();

    *pResult = m_pExpression->Evaluate();

    return S_OK;
}


template <>
IAction* CCreateable<CSelectAction, IAction>::NewInstance()
{
    return new CSelectAction;
}


template <>
void CCreateable<CSelectAction, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SelectAction)
DEFINE_ACTION_RELEASE_FXN(SelectAction)
