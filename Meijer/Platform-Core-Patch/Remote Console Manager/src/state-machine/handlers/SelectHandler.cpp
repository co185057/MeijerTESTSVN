// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/SelectHandler.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)SelectHandler.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/SelectHandler.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 5     3/22/05 5:03p Dm185016
 * UNICODE
 * 
 * 4     3/11/05 1:17p Dm185016
 * Added trace.  Cleared when vector after use.
 * 
 * 3     2/28/05 3:44p Dm185016
 * lint
 * 
 * 2     2/21/05 8:05p Dm185016
 * Added debugging information to new and delete for pooled objects.
 * 
 * 1     1/26/05 2:30p Dm185016
 * Moved to new Repository.
// 
// 5     1/18/05 3:58p Dm185016
// UNICODE support
// 
// 4     1/12/05 6:34p Dm185016
// Select support.
// 
// 3     12/22/04 11:25a Dm185016
// 
// 2     11/15/04 11:44a Dm185016
// Added TrueInstance and FalseInstance for select/if
// 
// 1     10/29/04 9:29a Dm185016
*/
#include "StdAfx.h"
#include "SelectHandler.h"
#include "RulesHandler.h"
#include "WhenHandler.h"
#include "OtherwiseHandler.h"
#include "RuleHandler.h"
#include "XMLStateMachine.h"
#include "SMLoader.h"
#include "SMConstants.h"
#include "StateImpl.h"
#include "SelectImpl.h"
#include "WhenImpl.h"
#include "LoggableManager.h"
#include "Loggable.h"

// Do not care about unreferenced formal parameters
#pragma warning( disable : 4100 )

#define GET_TRACE_HANDLE    CSelectHandler::instance()->GetTraceHandle()

using namespace STATE_MACHINE_NAMESPACE;


CSelectHandler::CSelectHandler(void) 
    : m_pCurrentRule(NULL)
    , hndlrRuleHandler(*CRuleHandler::instance())
    , m_nRuleCnt(0)
{
    CWhenHandler::instance()->SetOwner(this);
    COtherwiseHandler::instance()->SetOwner(this);
}

CSelectHandler::~CSelectHandler(void)
{
    m_pCurrentRule  = NULL;
}


void CSelectHandler::registerHandlers(void)
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);
//    SETUP_DECLARED_HANDLER( XP_RULE_STATE_NAME,                                 StateName );
//    SETUP_DECLARED_HANDLER( XP_RULE_STATE_GLOBAL,                               Global );
//    SETUP_DECLARED_HANDLER( XP_RULE_STATE_EXIT_RULE_LIST,                       ExitList );
//    SETUP_DECLARED_HANDLER( XP_RULE_STATE_ENTRY_RULE_LIST,                      EntryList );
//    SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG,                                  Message );
//    SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_NAME,                             MsgName );
//    SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_ACTION_RULE_LIST,                 ActionList );
//
//    SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_RULE_ACTION_FAILURE_TRANS,        FailureTransitionDefault );
//ITRACE_DEVELOP("----------------> Need to implement default failure and exception actions <--------------------");
//    SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_RULE_ACTION_EXC_TRANS,            ExceptionTransitionDefault );
//    SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_RULE_SUCCESS_TRANS,               SuccessTransition );
}



DEFINE_START_ELEMENT(CSelectHandler) 
{
    IMODTRACE_DEVELOP();
    ITRACE_DEVELOP_STATIC(GET_TRACE_HANDLE, _T("----------------------------------------------------------------"));

    CXMLStateMachine *pSM = GetLoader()->GetStateMachine();
    _ASSERTE(pSM != NULL);

    m_pSelect = newpooledobject CSelectImpl;
    m_pSelect->SetExpression(pSM->GetTrueExpression());

    hndlrRuleHandler.PushOwner(&CSelectHandler::BeginRule, 
                       &CSelectHandler::EndRule);

    return S_OK;
}
DEFINE_END_ELEMENT(CSelectHandler)
{
    IMODTRACE_DEVELOP();
    _ASSERTE(GetLoader() != NULL);

    CXMLStateMachine *pSM = GetLoader()->GetStateMachine();

    _ASSERTE(pSM != NULL);

    ITRACE_DEVELOP_STATIC(GET_TRACE_HANDLE, _T("----------------------------------------------------------------"));
    
    hndlrRuleHandler.PopOwner();

    //TODO: Need to loop thru the when clauses and add to the current state
    CRuleImplBase *pRule = NULL;
    CRuleQ::iterator iter = m_vWhenClauses.begin();
    for (;iter!=m_vWhenClauses.end(); iter++)
    {
        pRule = *iter;
        CRulesHandler::instance()->SaveRule(pRule);
    }

    CRulesHandler::instance()->SaveRule(m_pSelect);

    m_vWhenClauses.clear();

    return S_OK;
}


HRESULT STDMETHODCALLTYPE CSelectHandler::characters (
    /* [in] */ wchar_t __RPC_FAR *CH_CHARACTERS,
    /* [in] */ int CH_CHARACTERS_LEN)
{
    StringType szValue = GET_CHARACTERS();

    IString sFinalValue = IString(szValue.c_str()).trim();

    if (sFinalValue != _T(""))
        ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  szValue:  ") + szValue);

    return S_OK;
}


DEFINE_HANDLER(CSelectHandler, Comment, state);


HRESULT CSelectHandler::AddWhenClause(CWhenImpl *pWhen)
{
    IMODTRACE_DEVELOP_STATIC(GET_TRACE_HANDLE);

    ITRACE_DEVELOP_STATIC(GET_TRACE_HANDLE, _T("Adding When:  ") + IString(pWhen->ToString().c_str()));

    m_vWhenClauses.push_back(pWhen);

    m_pCurrentWhen = pWhen;

    return S_OK;
}


HRESULT CSelectHandler::BeginRule(CRuleImplBase **ppRule)
{
    IMODTRACE_DEVELOP_STATIC(GET_TRACE_HANDLE);
//  IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);
    CSelectHandler::instance()->NewRule();

    *ppRule = CSelectHandler::instance()->m_pCurrentRule;

    return S_OK;
}


HRESULT CSelectHandler::EndRule()
{
    IMODTRACE_DEVELOP_STATIC(GET_TRACE_HANDLE);
//  IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    CSelectHandler::instance()->SaveRule();

    return S_OK;
}


void CSelectHandler::NewRule()  
{ 
    IMODTRACE_DEVELOP();

    CXMLStateMachine *pSM = GetLoader()->GetStateMachine();
    _ASSERTE(pSM != NULL);

    m_pCurrentRule = newpooledobject CRuleImpl;     
    m_pCurrentRule->SetExpression(pSM->GetTrueExpression());
    m_nRuleCnt++; 
}


void CSelectHandler::SaveRule()
{
    IMODTRACE_DEVELOP();
//    state().AddMsgRule(&currentMsg(), m_pCurrentRule);

    ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Saving Rule:  ") + IString(m_pCurrentRule->ToString().c_str()));

    CRuleQ &rules = when().GetRulesList();
    rules.push_back(m_pCurrentRule);
}


void CSelectHandler::ResetRuleList()
{
    IMODTRACE_DEVELOP();
    m_nRuleCnt = 0;
}


void CSelectHandler::CheckNullList()
{
    IMODTRACE_DEVELOP();
    if (m_nRuleCnt == 0)
    {
        //if (xpathLoggable().IsTracing())
        //    ITRACE_DEVELOP(_T(__FUNCTION__) _T(" Adding a null rule.  State:  ") + IString(state().StateName.Value().c_str()) 
        //        + _T("  Msg:  ") + IString(currentMsg().Name.Value().c_str()));
        NewRule( );
        SaveRule();
    }
}


void CSelectHandler::GetXMLOutput(CXMLContent *, CXMLBuilder *)
{
    //CXMLBuilder &builder = *((pTheBuilder == NULL) ? this : pTheBuilder);

//  CRuleImpl &state = *(CRuleImpl *)pContent;

/*
    builder.StartElement(_L(REFUND_IN));
    
    builder.AddAttribute(ATTR_ID, refundIn.Reason__Id);
    builder.AddElement(_L(REASON), refundIn.Reason);    
    builder.AddAttribute(ATTR_ID, refundIn.SuperNum__Id);
    builder.AddElement(_L(SUPER_NUM), refundIn.SuperNum);    
    builder.AddAttribute(ATTR_ID, refundIn.SuperPass__Id);
    builder.AddElement(_L(SUPER_PASS), refundIn.SuperPass);    
    builder.AddAttribute(ATTR_ID, refundIn.Price__Id);
    builder.AddElement(_L(PRICE), refundIn.Price);    

    builder.EndElement(_L(REFUND_IN));
*/
}
