// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/EvaluateExpression.cpp 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)EvaluateExpression.cpp    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/EvaluateExpression.cpp $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     3/22/05 2:50p Dm185016
 * UNICODE
 * 
 * 2     2/28/05 3:50p Dm185016
 * lint
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
// 
// 5     10/29/04 9:26a Dm185016
// Removed reference to MessageObject.
// 
// 3     3/19/04 3:06p Dm185016
// Removed trace
// 
// 2     3/01/04 10:08a Dm185016
// Added Allocation/Release functions
// 
// 1     2/10/04 11:47a Dm185016
// New actions for evaluating expressions
*/
#include "StdAfx.h"
#include "EvaluateExpression.h"
#include "Expression.h"

using namespace STATE_MACHINE_NAMESPACE;


CEvaluateExpression::CEvaluateExpression()
{
    SetName(_T("CEvaluateExpression"));
    Comment.Value(_T("Evaluates the given expression."));
}


CEvaluateExpression::~CEvaluateExpression()
{
}


bool CEvaluateExpression::IsInternal() 
{
    return true; 
}


HRESULT CEvaluateExpression::ParseOperands()
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


HRESULT CEvaluateExpression::Execute(bool *pResult, IMessage &)
{
    ParseOperands();

    *pResult = m_pExpression->Evaluate();

    return S_OK;
}


template <>
IAction* CCreateable<CEvaluateExpression, IAction>::NewInstance()
{
    return new CEvaluateExpression;
}


template <>
void CCreateable<CEvaluateExpression, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(EvaluateExpression)
DEFINE_ACTION_RELEASE_FXN(EvaluateExpression)
