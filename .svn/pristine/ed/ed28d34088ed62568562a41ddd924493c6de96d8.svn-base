// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/StateImpl.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)StateImpl.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/StateImpl.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 7     5/03/05 4:08p Dm185016
 * Cleaned up tracing
 * 
 * 5     3/17/05 2:45p Dm185016
 * Fixed warnings
 * 
 * 4     3/16/05 4:03p Dm185016
 * Fixed mismatch for parameter types.
 * 
 * 3     3/11/05 1:20p Dm185016
 * <select> support
 * 
 * 2     2/10/05 5:54p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 1     1/26/05 2:29p Dm185016
 * Moved to new Repository.
// 
// 7     12/07/04 4:29p Dm185016
// Reworking CRulesList for <select>
// 
// 5     6/30/04 3:31p Dm185016
// Lint
// 
// 4     3/19/04 3:28p Dm185016
// Memory leak fix
// 
// 3     2/24/04 3:41p Dm185016
// Added code to dtor to cleanup resources
// 
// 2     1/16/04 9:23a Dm185016
// Namespace support, Removed <class>, Global state, Entry and Exit lists
*/
#include "StdAfx.h"
#include "StateImpl.h"
#include "StateHandler.h"
#include "Message.h"
#include "ModifiableRule.h"

using namespace STATE_MACHINE_NAMESPACE;


CStateImpl::CStateImpl(int nId)
    : m_szName(NULL)
    , m_szComment(NULL)
	, m_lTimeout(0)
    , m_nID(nId)
    , m_bInitial(false)
    , m_bGlobal(false)
{
//  IMODTRACE_DEVELOP();
}


CStateImpl::~CStateImpl(void)
{
//  IMODTRACE_DEVELOP();
    try
    {

        delete [] m_szName;
        delete [] m_szComment;

        IRule *pRule;
        CRulesList *pRules;
        CRulesListMap::iterator iter = m_MsgMap.begin();
        for (; iter!=m_MsgMap.end(); iter++)
        {
            pRules = iter->second;
            CRulesList::iterator iiter = (*pRules).begin();
            for (; iiter!=(*pRules).end(); iiter++)
            {
                pRule = *iiter;
                //pRule = (*iiter).second;
                delete pRule;
            }
            delete pRules;
        }

        CRulesList::iterator iiter = m_ExitActions.begin();
        for (; iiter != m_ExitActions.end(); iiter++)
            delete *iiter;
            //delete (*iiter).second;

        iiter = m_EntryActions.begin();
        for (; iiter != m_EntryActions.end(); iiter++)
            delete *iiter;
            //delete (*iiter).second;
    }
    catch (...)
        { ITRACE_DOT_ERROR(); }
}


DEFINE_BOOLEAN_PROPERTY(CStateImpl, Global);
DEFINE_BOOLEAN_PROPERTY(CStateImpl, Initial);
DEFINE_STRING_PROPERTY(CStateImpl, Name);
DEFINE_STRING_PROPERTY(CStateImpl, Comment);
DEFINE_STRING_PROPERTY(CStateImpl, Entry);
DEFINE_STRING_PROPERTY(CStateImpl, Exit);
DEFINE_LONG_PROPERTY(CStateImpl, Timeout);


StringType CStateImpl::GetXMLOutput(CXMLBuilder *pXMLBuilder)
{
    if (pXMLBuilder == NULL)
        ((CXMLBuilder *)CStateHandler::instance())->GetXMLOutput(this);
    else
        pXMLBuilder->GetXMLOutput(this);

    return (pXMLBuilder == NULL ? CStateHandler::instance() : pXMLBuilder)->GetXMLOutput();
}


int CStateImpl::GetId() const
{
	return m_nID;
}


const _TCHAR *CStateImpl::GetComment()
{
	return Comment.Value().c_str();
}


const _TCHAR *CStateImpl::GetName()
{
	return Name.Value().c_str();
}


long CStateImpl::GetTimeout()
{
	return Timeout.Value();
}


bool CStateImpl::IsInitial()
{
	return Initial.IsValid();
}


bool CStateImpl::IsGlobal()
{
	return Global.IsValid();
}


void CStateImpl::AddExitRule(IRule *pRule)
{
    m_ExitActions.push_back(pRule);
}


void CStateImpl::AddEntryRule(IRule *pRule)
{
    m_EntryActions.push_back(pRule);
}


void CStateImpl::AddMsgRule(const IMessage *pMessage, IRule *pRule)
{
    AddMsgRule(pMessage->GetId(), pRule, SimpleRule);
}


//lint --e{429}
void CStateImpl::AddMsgRule(int nKey, IRule *pRule, _RuleTypes ruletype)
{
    CRulesList *pList;
    CRulesListMap::iterator iter = m_MsgMap.find(nKey);

    if (iter != m_MsgMap.end())
        pList = iter->second;
    else
    {
        pList = new CRulesList;

        m_MsgMap.insert(std::make_pair(nKey, pList));
    }

    switch (ruletype)
    {
        case SimpleRule :
            if (loggable().IsDebug()) ITRACE_DEVELOP(_T(__FUNCTION__) _T("  simpleRule"));
            break;
        case WhenRule :
            if (loggable().IsDebug()) ITRACE_DEVELOP(_T(__FUNCTION__) _T("  whenRule"));
            m_WhenActions.push_back(pRule);
            break;
        case SelectRule :
            if (loggable().IsDebug()) ITRACE_DEVELOP(_T(__FUNCTION__) _T("  selectRule"));
            FixupSelect(pList->size());
            break;
        case OtherwiseRule :
            if (loggable().IsDebug()) ITRACE_DEVELOP(_T(__FUNCTION__) _T("  otherwiseRule"));
            break;
        case IfRule :
            if (loggable().IsDebug()) ITRACE_DEVELOP(_T(__FUNCTION__) _T("  ifRule"));
            break;
        case ElseRule :
            if (loggable().IsDebug()) ITRACE_DEVELOP(_T(__FUNCTION__) _T("  elseRule"));
            break;
    }

    pList->push_back(pRule);    
}


void CStateImpl::FixupSelect(size_t nNextRuleIndex)
{
    IMODTRACE_DEVELOP();

    if (loggable().IsDebug()) ITRACE_DEVELOP(_T(__FUNCTION__) + IString((unsigned)nNextRuleIndex));

    IModifiableRule *pFixupRule;
    CRulesList::iterator iter = m_WhenActions.begin();
    for (; iter != m_WhenActions.end(); iter++)
    {
        pFixupRule = dynamic_cast<IModifiableRule *>(*iter);

        pFixupRule->SetNextRuleIndex((int)nNextRuleIndex);
    }

    m_WhenActions.clear();
}


CRulesList *CStateImpl::GetMsgList(int _nStateID)
{    
    const CRulesListMap::iterator iter = m_MsgMap.find(_nStateID);

    if (iter == m_MsgMap.end())
        return NULL;

    return iter->second;
}
