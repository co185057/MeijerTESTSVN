// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/ExpressionImpl.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)ExpressionImpl.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/ExpressionImpl.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 10    5/03/05 4:06p Dm185016
 * Cleaned up tracing
 * 
 * 9     4/07/05 1:55p Dm185016
 * TAR 297347
 * 
 * 8     4/07/05 1:51p Dm185016
 * TAR 297347
 * 
 * 7     4/05/05 12:03p Dm185016
 * Added assertion for checking validity of pointer.
 * 
 * 6     3/28/05 5:10p Dm185016
 * TAR 296771
 * 
 * 5     3/24/05 7:27p Dm185016
 * UNICODE
 * 
 * 4     3/22/05 4:35p Dm185016
 * <if> support
 * 
 * 3     2/28/05 3:46p Dm185016
 * lint
 * 
 * 2     2/10/05 5:50p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 1     1/26/05 2:29p Dm185016
 * Moved to new Repository.
// 
// 11    1/18/05 3:58p Dm185016
// UNICODE support
// 
// 10    1/12/05 6:31p Dm185016
// Select support.
// 
// 9     10/04/04 6:47p Dm185016
// Removed trace.
// 
// 7     5/24/04 1:43p Dm185016
// Fixed bonehead trace line
// 
// 6     5/22/04 11:17a Dm185016
// Reduced trace lines
// 
// 5     5/21/04 9:17a Dm185016
// lint + removed trace
// 
// 4     4/16/04 4:18p Dm185016
// Added value expressions
// 
// 3     3/31/04 10:22a Dm185016
// Added == operation
// 
// 2     3/19/04 3:28p Dm185016
// Memory leak fix
// 
// 1     2/10/04 11:49a Dm185016
// New support for expressions.
*/
#include "StdAfx.h"
#include "ExpressionImpl.h"
#include "AbstractStateMachine.h"
#include "Message.h"
#include "State.h"

using namespace STATE_MACHINE_NAMESPACE;


//-----------------------------------------------------------------------------
// CExpressionImpl
//-----------------------------------------------------------------------------
CExpressionImpl::CExpressionImpl(int nId)
    : nId(nId)
    , lValue(0)
{
}


CExpressionImpl::~CExpressionImpl(void)
{
}


StringType CExpressionImpl::GetXMLOutput(CXMLBuilder * /* pXMLBuilder */ )
{
    return _T("");
}


int CExpressionImpl::GetId() const
{
    return nId;
}


long CExpressionImpl::GetValue() const
{ 
    return lValue; 
}


void CExpressionImpl::SetValue(long lNewValue)
{ lValue = lNewValue; }


void CExpressionImpl::SetValue(const StringType &sValue) { m_sValue = sValue; }


StringType CExpressionImpl::ToString() 
{ 
    IStringBuffer sbRetValue(125);
    sbRetValue += _T("Expression(");
    sbRetValue += m_sValue.c_str();
    sbRetValue += _T(")");

    StringType sRetValue = (const _TCHAR *)sbRetValue;

    return sRetValue; 
}


//-----------------------------------------------------------------------------
// CValueExpressionImpl
//-----------------------------------------------------------------------------
CValueExpressionImpl::CValueExpressionImpl(int nId) 
    : CExpressionImpl(nId)    
    , m_pLH(NULL)
{
}


CValueExpressionImpl::~CValueExpressionImpl(void) 
{
}


void CValueExpressionImpl::SetLHS(IEvaluateable *pLH)
{
    m_pLH = pLH;
}


long CValueExpressionImpl::GetValue() const
{
    return m_pLH->GetValue();
}


bool CValueExpressionImpl::Evaluate()
{
    bool retValue = false;
    _ASSERTE(m_pLH != NULL);
    if (m_pLH != NULL)
        retValue = (m_pLH->GetValue() != 0);

    trace(L6, _T("%s retValue=%s"), _T(__FUNCTION__), retValue ? _T("true") : _T("false") );

    return retValue;
}


//-----------------------------------------------------------------------------
// CIfExpressionImpl
//-----------------------------------------------------------------------------
CIfExpressionImpl::CIfExpressionImpl(HTRACE hTrace, int nId, IExpression * pBaseExpr, IExpression *pExpr)
    : CExpressionImpl(nId)    
    , m_bBlockOwner(false)
    , m_pExpr(pExpr)
{
    SetTraceObject(hTrace);

    if (pBaseExpr==NULL)
    {
        m_pIfBlock = new CIfBlock(this);
        m_pIfBlock->SetTraceObject(hTrace);
        m_bBlockOwner = true;
    }
    else
    {
        CIfExpressionImpl * pBaseIfExpr = dynamic_cast<CIfExpressionImpl *>(pBaseExpr);
        m_pIfBlock = pBaseIfExpr->m_pIfBlock;
    }
}


CIfExpressionImpl::~CIfExpressionImpl(void)
{
    if (m_bBlockOwner)
        delete m_pIfBlock;

    m_pIfBlock = NULL;
}


bool CIfExpressionImpl::Evaluate()
{
    bool bRetValue = false;

    if (m_bBlockOwner)
        m_pIfBlock->SetContinue(true);

    if (m_pIfBlock->CanContinue())
    {
        bRetValue = m_pExpr->Evaluate();

        if (bRetValue)
            m_pIfBlock->SetContinue(false);
    }

    if (stateMachine().loggable().IsDebug())
    {
        IState *pState = NULL;
        HRESULT hr = stateMachine().GetCurrentState(&pState);
        if (hr != S_OK)
            return bRetValue;

        IMessage *pMessage = NULL;
        hr = stateMachine().GetCurrentMsg(&pMessage);
        if (hr != S_OK)
            return bRetValue;

        ITRACE_DEVELOP(_T("\r\n    ____________________________________________\r\n    ") +
            IString(ToString().c_str()) + 
            _T("\r\n    State = ") + IString(pState->GetName()) +
            _T("\r\n    IMessage = ") + IString(pMessage->GetName()) +
            _T("\r\n    Result = ") + IString(bRetValue) +
            _T("\r\n    ____________________________________________"));
    }
    
    return bRetValue;
}


void CIfExpressionImpl::SetExpression(IExpression *pExpr) { m_pExpr = pExpr; }


bool CIfExpressionImpl::IsBlockOwner() const { return m_bBlockOwner; }


CIfExpressionImpl &CIfExpressionImpl::SetStateMachine(CAbstractStateMachine *pSM)
{
    m_pSM  = pSM;

    return *this;
}


CAbstractStateMachine &CIfExpressionImpl::stateMachine()
{
    _ASSERTE(m_pSM != NULL);

    return *m_pSM;
}


void CIfExpressionImpl::SetString(const _TCHAR *pszString)
{
    IStringBuffer sbRetValue(125);
    sbRetValue += _T("if Expression(");
    sbRetValue += pszString;
    sbRetValue += _T(")");

    m_sExpression = (const _TCHAR *)sbRetValue;
}


StringType CIfExpressionImpl::ToString()
{
    return m_sExpression;
}


CIfBlock::CIfBlock(CIfExpressionImpl *pOwner)
    : m_pOwner(pOwner)
    , m_bContinue(true)
{
}


bool CIfBlock::CanContinue() const 
{
    ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Continue:  ") + IString(m_bContinue));
    return m_bContinue; 
}


void CIfBlock::SetContinue(bool bContinue) { m_bContinue = bContinue; }


//-----------------------------------------------------------------------------
// CBinaryExpressionImpl
//-----------------------------------------------------------------------------
CBinaryExpressionImpl::CBinaryExpressionImpl(int nId) 
    : CExpressionImpl(nId)     
    , m_pLH(NULL)
    , m_pRH(NULL)
{
}


CBinaryExpressionImpl::~CBinaryExpressionImpl(void) 
{
}


void CBinaryExpressionImpl::SetLHS(IEvaluateable *pLH)
{
    m_pLH = pLH;
}


void CBinaryExpressionImpl::SetRHS(IEvaluateable *pRH)
{
    m_pRH = pRH;
}


void CBinaryExpressionImpl::SetOperator(int operation)
{
    m_operation = operation;
}


//lint --e{787, 788}
bool CBinaryExpressionImpl::Evaluate()
{
    //IMODTRACE_DEVELOP();

    bool retValue = true;
    StringType sOperation;
    switch (m_operation)
    {
        case CCPPTokenizer::Lt : 
            sOperation = _T("operation=Lt");
            retValue = (m_pLH->GetValue() < m_pRH->GetValue());
            break;
        case CCPPTokenizer::LtEq :
            sOperation = _T("operation=LtEq");
            retValue = (m_pLH->GetValue() <= m_pRH->GetValue());
            break;
        case CCPPTokenizer::Gt :
            sOperation = _T("operation=Gt");
            retValue = (m_pLH->GetValue() > m_pRH->GetValue());
            break;
        case CCPPTokenizer::GtEq :
            sOperation = _T("operation=GtEq");
            retValue = (m_pLH->GetValue() >= m_pRH->GetValue());
            break;
        case CCPPTokenizer::NotEq :
            sOperation = _T("operation=NotEq");
            retValue = (m_pLH->GetValue() != m_pRH->GetValue());
            break;
        case CCPPTokenizer::Equals :
            sOperation = _T("operation=Equals");
            retValue = (m_pLH->GetValue() == m_pRH->GetValue());
            break;
        default :
            sOperation = _T("operation=Unsupported operation");
            throw exception("Unsupported operation in expression");
    }

    trace(L6, _T("LH=%d RH=%d %s retValue=%s"), m_pLH->GetValue(), m_pRH->GetValue()
            , sOperation.c_str(), (const _TCHAR *)IString(retValue));

    return retValue;
}


//lint --e{787, 788}
bool CLogicalExpressionImpl::Evaluate()
{
    //IMODTRACE_DEVELOP();

    bool retValue = true;
    StringType sOperation;

    bool bLHS = pLHExpression->Evaluate();
    bool bRHS = pRHExpression->Evaluate();
    switch (m_operation)
    {
        case CCPPTokenizer::LogicalAnd : 
            sOperation = _T("operation=Lt");
            retValue = (bLHS && bRHS);
            break;
        case CCPPTokenizer::LogicalOr :
            sOperation = _T("operation=LtEq");
            retValue = (bLHS || bRHS);
            break;
        default :
            sOperation = _T("operation=Unsupported operation");
            throw exception("Unsupported operation in expression");
    }

    trace(L6, _T("LH=%s RH=%s %s retValue=%s"), MAP_BOOL(bLHS), MAP_BOOL(bRHS)
            , sOperation.c_str(), MAP_BOOL(retValue));

    return retValue;
}


//-----------------------------------------------------------------------------
// CUnaryExpressionImpl
//-----------------------------------------------------------------------------
CUnaryExpressionImpl::CUnaryExpressionImpl(int nId) 
    : CExpressionImpl(nId)     
    , m_pExpression(NULL)
{
}


CUnaryExpressionImpl::~CUnaryExpressionImpl(void) 
{
}


void CUnaryExpressionImpl::SetExpression(IExpression *pLH)
{
    m_pExpression = pLH;
}


void CUnaryExpressionImpl::SetOperator(int operation)
{
    m_operation = operation;
}


//lint --e{787, 788}
bool CUnaryExpressionImpl::Evaluate()
{
    //IMODTRACE_DEVELOP();

    bool retValue = true;
    StringType sOperation;
    switch (m_operation)
    {
        case CCPPTokenizer::LogicalNot : 
            sOperation = _T("operation=Not");
            retValue = !(m_pExpression->Evaluate());
            break;
        default :
            sOperation = _T("operation=Unsupported operation");
            throw exception("Unsupported operation in expression");
    }

    trace(L6, _T("Expression=%d %s retValue=%s"), m_pExpression->GetValue()
            , sOperation.c_str(), (const _TCHAR *)IString(retValue));

    return retValue;
}


CFalseExpressionImpl::CFalseExpressionImpl(int nId) 
    : CExpressionImpl(nId)     
{}
CFalseExpressionImpl::~CFalseExpressionImpl() 
{ 
    //IMODTRACE_DEVELOP(); 
    //ITRACE_DEVELOP("this:  " + IString((long)this).d2x());
}


bool CFalseExpressionImpl::Evaluate() { return false; };


StringType CFalseExpressionImpl::ToString() { return StringType(_T("false")); }


CTrueExpressionImpl::CTrueExpressionImpl(int nId) 
    : CExpressionImpl(nId)     
{}
CTrueExpressionImpl::~CTrueExpressionImpl() 
{ 
    //IMODTRACE_DEVELOP(); 
    //ITRACE_DEVELOP("this:  " + IString((long)this).d2x());
}


bool CTrueExpressionImpl::Evaluate() { return true; };


StringType CTrueExpressionImpl::ToString() { return StringType(_T("true")); }
