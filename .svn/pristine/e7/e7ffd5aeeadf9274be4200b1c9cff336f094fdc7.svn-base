// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/StateObjectProxy.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)StateObjectProxy.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/StateObjectProxy.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 5     5/03/05 4:08p Dm185016
 * Cleaned up tracing
 * 
 * 4     4/21/05 7:26p Dm185016
 * Improved tracing
 * 
 * 2     2/10/05 5:54p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 1     1/26/05 2:29p Dm185016
 * Moved to new Repository.
// 
// 8     1/12/05 6:32p Dm185016
// Select support.
// 
// 7     12/22/04 11:23a Dm185016
// Conditional logic support.
// 
// 5     5/21/04 10:36a Dm185016
// lint
// 
// 4     3/19/04 3:28p Dm185016
// Memory leak fix
// 
// 3     2/10/04 11:50a Dm185016
// New action format
// 
// 2     1/27/04 2:06p Dm185016
// Action Library support
// 
// 1     1/16/04 9:21a Dm185016
// Support for new rule format
*/
#include "StdAfx.h"
#include "SMConstants.h"
#include "StateObjectProxy.h"
#include "RuleProxy.h"
#include "RuleImpl.h"
#include "MsgImpl.h"
#include "ActionImpl.h"
#include "RulesHandler.h"

using namespace STATE_MACHINE_NAMESPACE;


CStateObjectProxy::CStateObjectProxy(int nId)
	: m_nId(nId)
{
}


CStateObjectProxy::~CStateObjectProxy(void)
{
    try
    {
        for (CRuleQIterator iter = m_qEntryActions.begin();
            iter != m_qEntryActions.end();
            iter++)
        {
            delete *iter;
        }

        for (CRuleQIterator iter = m_qExitActions.begin();
            iter != m_qExitActions.end();
            iter++)
        {
            delete *iter;
        }

        for (CRuleQMapIterator iter = m_mapMsgActions.begin();
            iter != m_mapMsgActions.end();
            iter++)
        {
            CRuleQ &proxyMsgList = *(iter->second);

            for (CRuleQIterator iterInner = proxyMsgList.begin();
                iterInner != proxyMsgList.end();
                iterInner++)
            {
                delete *iterInner;
            }

            delete &proxyMsgList;
        }
        m_qExitActions.clear();
        m_qEntryActions.clear();
        m_qMsgActions.clear();
        m_mapMsgActions.clear();
    }
    catch (...)
        { ITRACE_DOT_ERROR(); }
}


DEFINE_STRING_PROPERTY(CStateObjectProxy, Comment);
DEFINE_STRING_PROPERTY(CStateObjectProxy, StateName);
DEFINE_BOOLEAN_PROPERTY(CStateObjectProxy, Global);


HRESULT CStateObjectProxy::SaveSuccessTransition(const StringType &szValue)
{
    CRuleImplBase *pRule;
    for (CRuleQIterator iter = m_qMsgActions.begin();
         iter != m_qMsgActions.end();
         iter++)
    {
        pRule = *iter;
        pRule->EndStateName.Value(szValue);
    }

    return S_OK;
}


HRESULT CStateObjectProxy::SaveFailureTransitionDefault(const StringType &szValue)
{
    CRuleImplBase *pRule;
    for (CRuleQIterator iter = m_qMsgActions.begin();
         iter != m_qMsgActions.end();
         iter++)
    {
        pRule = *iter;
        pRule->FalseStateName.Value(szValue);
    }

    return S_OK;
}


HRESULT CStateObjectProxy::SaveFailureActionDefault(const StringType &szValue)
{
    CRuleImplBase *pRule;
    for (CRuleQIterator iter = m_qMsgActions.begin();
         iter != m_qMsgActions.end();
         iter++)
    {
        pRule = *iter;
        pRule->FalseActionName.Value(szValue);
    }

    return S_OK;
}


HRESULT CStateObjectProxy::SaveExceptionTransitionDefault(const StringType &szValue)
{
    CRuleImplBase *pRule;
    for (CRuleQIterator iter = m_qMsgActions.begin();
         iter != m_qMsgActions.end();
         iter++)
    {
        pRule = *iter;
        pRule->FailureStateName.Value(szValue);
    }

    return S_OK;
}


HRESULT CStateObjectProxy::SaveExceptionActionDefault(const StringType &szValue)
{
    CRuleImplBase *pRule;
    for (CRuleQIterator iter = m_qMsgActions.begin();
         iter != m_qMsgActions.end();
         iter++)
    {
        pRule = *iter;
        pRule->FailureActionName.Value(szValue);
    }

    return S_OK;
}


void CStateObjectProxy::AddExitRule(CRuleImplBase *pRule)
{
    if (loggable().IsDebug()) ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Adding Exit Rule:  ") + IString(pRule->ToString().c_str()));

    m_qExitActions.push_back(pRule);
}


void CStateObjectProxy::AddEntryRule(CRuleImplBase *pRule)
{
    if (loggable().IsDebug()) ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Adding Entry Rule:  ") + IString(pRule->ToString().c_str()));

    m_qEntryActions.push_back(pRule);
}


void CStateObjectProxy::AddMsgRule(CMsgImpl *pMsg, CRuleImplBase *pRule)
{
    CRuleImplBase &rule = *pRule;
    rule.StateName.Value(StateName.Value());
    rule.MsgName.Value(pMsg->GetName());
    rule.Comment.Value(pMsg->GetComment());

    if (loggable().IsDebug()) ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Adding Rule:  ") + IString(pRule->ToString().c_str()));

    int nKey = pMsg->GetId();
    CRuleQ *pList;
    CRuleQMapIterator iter = m_mapMsgActions.find(nKey);

    if (iter != m_mapMsgActions.end())
        pList = iter->second;
    else
    {
        pList = new CRuleQ;

        m_mapMsgActions.insert(std::make_pair(nKey, pList));
    }

    pList->push_back(pRule);
    m_qMsgActions.push_back(pRule);
}


void CStateObjectProxy::ClearRules()
{
    m_qMsgActions.clear();
}



CRuleQ *CStateObjectProxy::GetMsgList(int _nStateID)
{    
    const CRuleQMapIterator iter = m_mapMsgActions.find(_nStateID);

    if (iter == m_mapMsgActions.end())
        return NULL;

    return iter->second;
}


StringType CStateObjectProxy::GetXMLOutput(CXMLBuilder *pXMLBuilder)
{
    if (pXMLBuilder == NULL)
        ((CXMLBuilder *)CRulesHandler::instance())->GetXMLOutput(this);
    else
        pXMLBuilder->GetXMLOutput(this);

    return (pXMLBuilder == NULL ? CRulesHandler::instance() : pXMLBuilder)->GetXMLOutput();
}
