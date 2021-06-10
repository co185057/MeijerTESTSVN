// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/VariableOperations.cpp 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)VariableOperations.cpp    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/VariableOperations.cpp $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 5     3/28/05 5:09p Dm185016
 * Lint
 * 
 * 4     3/22/05 2:50p Dm185016
 * UNICODE
 * 
 * 3     3/15/05 3:40p Dm185016
 * Added error check for valid variable.
 * 
 * 2     2/28/05 3:48p Dm185016
 * lint
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
// 
// 16    1/18/05 3:56p Dm185016
// UNICODE support
// 
// 15    12/15/04 4:43p Dm185016
// Fixed the assign operation to check the max-value same as the
// increment.
// 
// 14    12/13/04 3:51p Dm185016
// assign-variable now returns true if value is less than or equal to the
// maximum value.  Returns false if greater than the maximum.
// 
// 12    7/20/04 3:39p Dm185016
// If decrement below min value, then trace and reset to minimum value.
// 
// 11    5/21/04 9:17a Dm185016
// Removed trace
// 
// 10    4/19/04 3:00p Dm185016
// Removed invalid change to return false if value is zero.
// 
// 9     4/16/04 4:18p Dm185016
// Fixed assign function
// 
// 8     3/31/04 10:21a Dm185016
// Enabled assign operation
// 
// 7     3/19/04 3:09p Dm185016
// Removed trace
// 
// 6     3/01/04 10:15a Dm185016
// Added Allocation/Release functions
// 
// 5     2/10/04 11:46a Dm185016
// Moved common support to base
// 
// 4     1/27/04 2:03p Dm185016
// DLL Support
// 
// 3     1/16/04 9:17a Dm185016
// Namespace support, Increment and Decrement operations
*/

//lint -e788
#include "StdAfx.h"
#include "VariableOperations.h"
#include "Expression.h"
#include "SMError.h"
#include "InternalOps.h"

using namespace STATE_MACHINE_NAMESPACE;

CVariableOperations::CVariableOperations()
    : m_nOperation(NO_OP)
    , m_nVariable(0)
    , m_pVariable(NULL)
    , m_pExpression(NULL)
{
    SetName(_T("CVariableOperations"));
}


CVariableOperations::~CVariableOperations()
{
    m_pVariable = NULL;
    m_pExpression = NULL;
}


IVariable &CVariableOperations::variable() 
{ 
    _ASSERTE(m_pVariable != NULL);

    return *m_pVariable; 
}


IExpression &CVariableOperations::expression() 
{ 
    _ASSERTE(m_pExpression != NULL);

    return *m_pExpression; 
}


const _TCHAR *CVariableOperations::GetComment(void) 
{
    if (!Comment.IsValid())
    {
        StringType sComment;
        switch (m_nOperation)
        {
            case INCREMENT :
                sComment = _T("Incrementing variable.  Name:  ");
                sComment += variable().GetName(); 
                break;
            case DECREMENT :
                sComment = _T("Decrementing variable.  Name:  ");
                sComment += variable().GetName(); 
                break;
            default :
                _ASSERTE(false);
        }

        Comment.Value(sComment);
    }

    return Comment.Value().c_str();
}


bool CVariableOperations::IsInternal() 
{
    return true; 
}


HRESULT CVariableOperations::ParseOperands()
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
    hr = stateMachine().GetVariable(&m_pVariable, sVariableName.c_str());
    if (hr != S_OK)
        return hr;

    switch (m_nOperation)
    {
        case ASSIGN :
            {
                iter++;
                if (iter == parms.end())
                {
                    ITRACE_ERROR(_T("Missing expression value"));
                    return SECURITY_MISSING_PARAMETER;
                }
                StringType sExpressionName;
                sExpressionName = *iter;
                hr = stateMachine()
                    .GetExpression(&m_pExpression, sExpressionName.c_str());
            }
            break;
        default :
            break;
    }

    return hr;
}


HRESULT CVariableOperations::ParseOperation()
{ 
//  IMODTRACE_DEVELOP();

    HRESULT hr = S_OK;
    StringType name = GetName();

    if (_tcscmp(name.c_str(), INCREMENT_VARIABLE) == 0)
        m_nOperation = INCREMENT;
    else if (_tcscmp(name.c_str(), DECREMENT_VARIABLE) == 0)
        m_nOperation = DECREMENT;
    else if (_tcscmp(name.c_str(), ASSIGN_VARIABLE) == 0)
        m_nOperation = ASSIGN;
    else if (_tcscmp(name.c_str(), EVALUATE_EXPRESSION) == 0)
        m_nOperation = EVALUATE;
    else
        m_nOperation = NO_OP;

    return hr;
}


HRESULT CVariableOperations::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = S_OK;
    *pResult = true;

    hr = ParseOperation();
    if (hr != S_OK)
        return hr;

    hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    switch (m_nOperation)
    {
        case INCREMENT :
            *pResult = IncrementVariable();
            break;
        case DECREMENT :
            *pResult = DecrementVariable();
            break;
        case ASSIGN :
            *pResult = AssignVariable();
            break;
        case EVALUATE :
//          *pResult = EvaluateVariable();
            break;
        default:
            hr = SECURITY_UNDEFINED_OPERATION;
            break;
    }

    return hr;
}


bool CVariableOperations::IncrementVariable()
{
    bool retValue = true;

    IVariable &var = variable();
    long value = var.GetValue();
            
    value++;

    var.SetValue(value);

    if (var.HasMaxValue())
        retValue = (value <= var.GetMaxValue());
            
    ITRACE_DEVELOP(_T("Increment Variable:  ") + IString(var.GetName()) + _T(".  Value:  ") + IString(value));

    return retValue;
}


bool CVariableOperations::DecrementVariable()
{
    //IMODTRACE_DEVELOP();

    bool retValue = true;

    IVariable &var = variable();
    long value = var.GetValue();
            
    value--;

    var.SetValue(value);

    if (var.HasMinValue())
        retValue = !(value < var.GetMinValue());

    ITRACE_DEVELOP(_T("Decrement Variable:  ") + IString(var.GetName()) 
                 + _T(".  Value:  ") + IString(value));

    // If the value decremented below the minimum value, then inform the user
    // and reset to the minimum
    if (!retValue)
    {
        trace(L4, _T("Value:  %d is less than minimum allowed value.  Reset to:  %d")
                , value, var.GetMinValue());
        var.SetValue(var.GetMinValue());
    }

    return retValue;
}


bool CVariableOperations::AssignVariable()
{
//  IMODTRACE_DEVELOP();

    bool retValue = true;

    IVariable &var = variable();

    // Evaluate the expression.
    IExpression &expr = expression();
    expr.Evaluate();

    // Assign the value of the expression to the variable
    long value = expr.GetValue();
    var.SetValue(value);

    // Check the value of the expression against any defined minimum
    // and maximum limits.
    if (var.HasMinValue())
    {
        retValue = (value < var.GetMinValue());
        ITRACE_DEVELOP(_T("Variable has minimum value of:  ") + IString(var.GetMinValue()));
    }

    if (retValue && var.HasMaxValue())
    {
        retValue = (value <= var.GetMaxValue());
        ITRACE_DEVELOP(_T("Variable has maximum value of:  ") + IString(var.GetMaxValue()));
    }
            
    ITRACE_DEVELOP(_T("Assign Variable:  ") + IString(var.GetName()) 
                 + _T(".  Value:  ") + IString(value));

    return retValue;
}


void CVariableOperations::SetCounter(IVariable *pVar)
{
    m_pVariable = pVar;
}


template <>
IAction* CCreateable<CVariableOperations, IAction>::NewInstance()
{
    IAction *pAction = new CVariableOperations;

    return pAction;
}


template <>
void CCreateable<CVariableOperations, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(VariableOperations)
DEFINE_ACTION_RELEASE_FXN(VariableOperations)
