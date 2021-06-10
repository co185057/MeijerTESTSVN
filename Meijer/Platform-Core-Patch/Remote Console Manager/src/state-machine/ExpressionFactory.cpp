// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/state-machine/ExpressionFactory.cpp 3     12/16/08 2:03p Sa250050 $
/*
 * @(#)CExpressionFactory.cpp    $Revision: 3 $ $Date: 12/16/08 2:03p $
 *
 * Copyright 2004, NCR Corporation.
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/state-machine/ExpressionFactory.cpp $
 * 
 * 3     12/16/08 2:03p Sa250050
 * Patch G Build 337 Migration
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 12    5/03/05 4:07p Dm185016
 * Cleaned up tracing
 * 
 * 11    4/07/05 1:54p Dm185016
 * TAR 297347
 * 
 * 10    4/07/05 1:52p Dm185016
 * 
 * 9     4/05/05 12:01p Dm185016
 * TAR 297356 - Added plist for action expression in <if>
 * 
 * 8     3/28/05 5:07p Dm185016
 * TAR 296771
 * 
 * 7     3/24/05 7:25p Dm185016
 * UNICODE
 * 
 * 6     3/22/05 4:33p Dm185016
 * <if> support
 * UNICODE
 * 
 * 5     3/17/05 2:42p Dm185016
 * Fixed infinite loop in parsing error token
 * 
 * 4     2/28/05 3:53p Dm185016
 * lint
 * 
 * 3     2/21/05 8:06p Dm185016
 * Added debugging information to new and delete for pooled objects.
 * 
 * 2     2/10/05 5:39p Dm185016
 * TAR 292915
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
// 
// 8     1/18/05 3:55p Dm185016
// UNICODE support
// 
// 7     1/12/05 6:26p Dm185016
// Select support.
// 
// 6     11/15/04 11:44a Dm185016
// Added TrueInstance and FalseInstance for select/if
// 
// 5     10/04/04 6:39p Dm185016
// Trace object is no longer shared.
// 
// 3     5/22/04 11:16a Dm185016
// lint
// 
// 2     4/16/04 4:17p Dm185016
// Added value expressions
// 
// 1     2/10/04 11:39a Dm185016
// New Expression evaluation support
*/

//lint --e{788} Really don't care if some constants are not included in switch.  That's
//              why there's a default!

#include "stdafx.h"
#include "statem.h"
#include "Evaluateable.h"
#include "ExpressionFactory.h"
#include "Expression.h"
#include "ExpressionImpl.h"
#include "ActionExpressionImpl.h"
#include "ConstantImpl.h"
#include "Action.h"
#include "Variable.h"
#include "CPPTokenizer.h"
#include "Loggable.h"
#include "LoggableManager.h"

using namespace STATE_MACHINE_NAMESPACE;

CExpressionFactory::CExpressionFactory(CAbstractStateMachine *pSM)
: m_operation(CCPPTokenizer::None)
    , m_pLH(NULL)
    , m_pRH(NULL)
    , m_nState(1)
    , m_counter(0)
    , m_eExprType(eLogical)
    , m_pExpression(NULL)
    , m_pLoggable(&Trace::getLoggable(pSM->GetTraceHandle()
                                    , _T("StateMachine.ExpressionFactory")))
    , m_pStateMachine(pSM)
{    
	m_token.tpexpr = NULL;	// TAR 392458 - Fix uninitialized memory warnings.
}


#pragma warning( push )
#pragma warning( disable : 4311 )


CExpressionFactory:: ~CExpressionFactory()
{
    m_pExpression   = NULL;
    m_pLH           = NULL;
    m_pRH           = NULL;

    CEvaluateableList::iterator iter = m_evaluateables.begin();
    for (; iter != m_evaluateables.end(); iter++)
    {
        try
        {
            IEvaluateable *pEvaluateable = *iter;
            CA2T sName(typeid( *pEvaluateable ).name());
            ITRACE_DEVELOP(IString(_T("Deleting IEvaluateable:  ")) + (const _TCHAR *)sName + _T("@") + IString((long)pEvaluateable).d2x());
            delete *iter;
        }
        catch (...)
        { ITRACE_DOT_ERROR(); }
    }

    m_evaluateables.clear();

    m_pStateMachine = NULL;
    m_pLoggable     = NULL;
}
#pragma warning( pop )


ILoggable &CExpressionFactory::loggable() { return *m_pLoggable; }


IExpression * CExpressionFactory::TrueInstance() 
{
    CExpressionImpl *pExpr = newpooledobject CTrueExpressionImpl(++m_counter);
    pExpr->SetTraceObject(GetTraceHandle());

    m_evaluateables.push_back(pExpr);

    return pExpr; 
}


IExpression * CExpressionFactory::FalseInstance() 
{
    CExpressionImpl *pExpr = newpooledobject CFalseExpressionImpl(++m_counter);
    pExpr->SetTraceObject(GetTraceHandle());

    m_evaluateables.push_back(pExpr);

    return pExpr; 
}


IExpression * CExpressionFactory::IfInstance(const _TCHAR *expression, IExpression *pIfExpr)
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    IExpression *pExpression;
    if (expression == NULL)
        pExpression = TrueInstance();
    else
        pExpression = NewInstance(expression);

    CIfExpressionImpl *pIfExpression = newpooledobject CIfExpressionImpl(GetTraceHandle(),++m_counter, pIfExpr, pExpression);
    m_evaluateables.push_back(pIfExpression);

    pIfExpression->SetStateMachine(m_pStateMachine);
    pIfExpression->SetString(expression);

    return pIfExpression;
}


IExpression * CExpressionFactory::NewInstance(const _TCHAR *szExpression)
{
    m_pExpression   = NULL;
    m_pLH           = NULL;
    m_pRH           = NULL;
    m_nState        = 1;
    m_bNegateIt     = false;

    ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Parsing Expression:  \"") + IString(szExpression) + _T("\""));

    tokenizer.SetSource(szExpression);

    CToken token;
    for (; m_nState != DONE; tokenizer++)
    {
        token = GetNextToken();

        if (loggable().IsDebug())
            ITRACE_DEVELOP("state=" + IString(m_nState) + 
                        ".  token=" + IString(token.GetToken()) + 
                        ".  type=" + IString(token.ttype) +
                        "(" + IString(token.GetTokenTypeName()) + ")");

        switch (m_nState)
        {
            case 1 : State1(token); break;
            case 2 : State2(token); break;
            case 3 : State3(token); break;
            case 4 : State4(token); break;
            case 5 : State5(token); break;
            default : 
                ITRACE_ERROR(_T("Undefined state encountered:  ") + IString(m_nState));
        }
    }

    if (m_bNegateIt)
        m_pExpression = ReduceLogicalNotExpression(m_pExpression);

    m_pExpression->SetValue(szExpression);

    return m_pExpression;
}


CToken CExpressionFactory::GetNextToken()
{
    m_token.token       = tokenizer.Token();
    m_token.ttype       = tokenizer.GetTokenType(); 
    m_token.ttypeName   = tokenizer.GetTokenTypeAsString();

    return m_token;
}


void CExpressionFactory::PushbackToken(const CToken &)
{
}


void CExpressionFactory::ReduceExpression()
{
    IMODTRACE_DEVELOP();

    CToken token = GetNextToken();
    switch(token.ttype)
    {
        default :
            break;
    }
}


void CExpressionFactory::ReducePrimaryExpression()
{
    IMODTRACE_DEVELOP();

    CToken token = GetNextToken();
    switch(token.ttype)
    {
        default :
            break;
    }
}


void CExpressionFactory::ReducePostfixExpression()
{
    IMODTRACE_DEVELOP();

    CToken token = GetNextToken();
    switch(token.ttype)
    {
        default :
            break;
    }
}


void CExpressionFactory::ReduceKVPlist()
{
}


void CExpressionFactory::ReduceUnaryExpression()
{
    IMODTRACE_DEVELOP();

    CToken token = GetNextToken();
    switch(token.ttype)
    {
        default :
            break;
    }
}


void CExpressionFactory::ReduceNultiplicativeExpression()
{
    IMODTRACE_DEVELOP();

    CToken token = GetNextToken();
    switch(token.ttype)
    {
        default :
            break;
    }
}


void CExpressionFactory::ReduceAdditiveExpression()
{
    IMODTRACE_DEVELOP();

    CToken token = GetNextToken();
    switch(token.ttype)
    {
        default :
            break;
    }
}


void CExpressionFactory::ReduceRelationalExpression()
{
    IMODTRACE_DEVELOP();

    CToken token = GetNextToken();
    switch(token.ttype)
    {
        default :
            break;
    }
}


void CExpressionFactory::ReduceEqualityExpression()
{
    IMODTRACE_DEVELOP();

    CToken token = GetNextToken();
    switch(token.ttype)
    {
        default :
            break;
    }
}


void CExpressionFactory::ReduceAndExpression()
{
    IMODTRACE_DEVELOP();

    CToken token = GetNextToken();
    switch(token.ttype)
    {
        default :
            break;
    }
}


void CExpressionFactory::ReduceExclusiveOrExpression()
{
    IMODTRACE_DEVELOP();

    CToken token = GetNextToken();
    switch(token.ttype)
    {
        default :
            break;
    }
}


void CExpressionFactory::ReduceInclusiveOrExpression()
{
    IMODTRACE_DEVELOP();

    CToken token = GetNextToken();
    switch(token.ttype)
    {
        default :
            break;
    }
}


void CExpressionFactory::ReduceLogicalAndExpression()
{
    IMODTRACE_DEVELOP();

    CToken token = GetNextToken();
    switch(token.ttype)
    {
        default :
            break;
    }
}


void CExpressionFactory::ReduceLogicalOrExpression()
{
    IMODTRACE_DEVELOP();

    CToken token = GetNextToken();
    switch(token.ttype)
    {
        default :
            break;
    }
}


void CExpressionFactory::State1 (CToken &token)
{
    IVariable *pVar = NULL;
    int nAction = -1;
    HRESULT hr = S_OK;
    switch (token.ttype)
    {
        case CCPPTokenizer::Identifier : 
            hr = stateMachine().GetVariable(&pVar, token.GetToken());
            if (hr == S_OK)
            {
                m_pLH = pVar;
                m_nState = 2;
            }
            else
            {
                if (stateMachine().IsValidAction(token.GetToken()))
                {
                    m_szActionName = token.GetToken();
                    m_nState = 4;
                }
                else
                    m_nState = DONE;
            }
            break;
        case CCPPTokenizer::Integer : 
            m_pLH = GetInteger(token.token);
            m_nState = 2;
            break;
        case CCPPTokenizer::LogicalNot : 
            m_bNegateIt = true;
            break;
        default :
            ITRACE_ERROR(_T("Unrecognized token type:  ") + IString(token.GetTokenTypeName()) +
                         _T(".  token:  ") + IString(token.GetToken()));
            m_nState = DONE;
            break;
    }
}


void CExpressionFactory::State2 (CToken &token)
{
    switch (token.ttype)
    {
		case CCPPTokenizer::Lt : 
		case CCPPTokenizer::LtEq :
		case CCPPTokenizer::Gt :
		case CCPPTokenizer::GtEq :
		case CCPPTokenizer::LtGt_NotEq :
		case CCPPTokenizer::LogicalNot :
		case CCPPTokenizer::NotEq :
		case CCPPTokenizer::Equals :
            m_eExprType = eLogical;
            m_operation = token.ttype;
            m_nState = 3;
            break;
        case CCPPTokenizer::EndOfLine :
        case CCPPTokenizer::None :
            m_pExpression = ReduceValueExpression();
            m_nState = DONE;
            break;
        default :
            ITRACE_ERROR(_T("Unrecognized token type:  ") + IString(token.GetTokenTypeName()));
            break;
    }
}


void CExpressionFactory::State3 (CToken &token)
{
    IVariable *pVar = NULL;
    HRESULT hr = S_OK;
    switch (token.ttype)
    {
        case CCPPTokenizer::Identifier : 
            hr = stateMachine().GetVariable(&pVar, token.GetToken());
            if (hr == S_OK)
            {
                m_pRH = pVar;
                m_nState = 2;
            }
            m_pExpression = ReduceBinaryExpression();
            m_nState = DONE;
            break;
        case CCPPTokenizer::Integer : 
            m_pRH = GetInteger(token.token);
            m_pExpression = ReduceBinaryExpression();
            m_nState = DONE;
            break;
        default :
            ITRACE_ERROR(_T("Unrecognized token type:  ") + IString(token.GetTokenTypeName()));
            break;
    }
}


void CExpressionFactory::State4 (CToken &token)
{
    switch (token.ttype)
    {
        case CCPPTokenizer::LeftParen : 
            m_nState = 5;
            break;
        default :
            ITRACE_ERROR(_T("Unrecognized token type:  ") + IString(token.GetTokenTypeName()));
            break;
    }
}


void CExpressionFactory::State5 (CToken &token)
{
    IVariable *pVar = NULL;
    HRESULT hr = S_OK;
    switch (token.ttype)
    {
        case CCPPTokenizer::RightParen : 
            m_pExpression = ReduceActionExpression();
            m_nState = DONE;
            break;
        default :
            m_temp += m_token.token;
            ITRACE_DEVELOP(_T("m_temp:  ") + IString(m_temp.c_str()));
            break;
    }
}


IEvaluateable *CExpressionFactory::GetInteger(const StringType &sConstant)
{
    CConstantImpl *pConstantObject = newpooledobject CConstantImpl(0);
    m_evaluateables.push_back(pConstantObject);

    pConstantObject->SetTraceObject(GetTraceHandle());
    pConstantObject->SaveValue(sConstant);    //lint !e534
    IEvaluateable *pEval = pConstantObject;

    return pEval;
}


CExpressionImpl *CExpressionFactory::ReduceValueExpression()
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    CValueExpressionImpl *pExpression = newpooledobject CValueExpressionImpl(++m_counter);
    m_evaluateables.push_back(pExpression);

    pExpression->SetTraceObject(GetTraceHandle());
    pExpression->SetLHS(m_pLH);

    CToken token;
    token.ttype=Expression;
    token.tpexpr=pExpression;
    m_TokenStack.push_front(token);

    return pExpression;
}


CExpressionImpl *CExpressionFactory::ReduceActionExpression()
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    CActionExpressionImpl *pExpression = newpooledobject CActionExpressionImpl(GetTraceHandle(), ++m_counter, m_szActionName.c_str(), m_temp.c_str());

    m_temp.clear();

    m_evaluateables.push_back(pExpression);

    pExpression->SetStateMachine(m_pStateMachine);

    CToken token;
    token.ttype=Expression;
    token.tpexpr=pExpression;
    m_TokenStack.push_front(token);

    ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Expression:  ") + IString(pExpression->ToString().c_str()));
    return pExpression;
}


CExpressionImpl *CExpressionFactory::ReduceBinaryExpression()
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    CBinaryExpressionImpl *pExpression = newpooledobject CBinaryExpressionImpl(++m_counter);
    m_evaluateables.push_back(pExpression);

    pExpression->SetTraceObject(GetTraceHandle());
    pExpression->SetLHS(m_pLH);
    pExpression->SetRHS(m_pRH);
    pExpression->SetOperator(m_operation);

    CToken token;
    token.ttype=Expression;
    token.tpexpr=pExpression;
    m_TokenStack.push_front(token);

    return pExpression;
}


CExpressionImpl *CExpressionFactory::ReduceLogicalNotExpression(CExpressionImpl *pExpr)
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    CUnaryExpressionImpl *pExpression = newpooledobject CUnaryExpressionImpl(++m_counter);
    m_evaluateables.push_back(pExpression);

    pExpression->SetTraceObject(GetTraceHandle());
    pExpression->SetExpression(pExpr);
    pExpression->SetOperator(CCPPTokenizer::LogicalNot);

    CToken token;
    token.ttype=Expression;
    token.tpexpr=pExpression;
    m_TokenStack.push_front(token);

    return pExpression;
}

