// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/ElseIfHandler.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)ElseIfHandler.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
 *
 * Copyright 2005 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/ElseIfHandler.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 5     5/03/05 4:14p Dm185016
 * Cleaned up tracing
 * 
 * 4     4/21/05 7:27p Dm185016
 * Cleaned up tracing
 * 
 * 2     3/22/05 5:00p Dm185016
 * <if> support
 * 
 * 1     3/17/05 2:47p Dm185016
 * <if>, <else-if>, <else> support
*/
#include "StdAfx.h"
#include "ElseIfHandler.h"
#include "IfHandler.h"
#include "RulesHandler.h"
#include "SMObjectRepository.h"
#include "SMLoader.h"
#include "SMConstants.h"
#include "StateImpl.h"
#include "LoggableManager.h"
#include "Loggable.h"

#ifdef GET_TRACE_HANDLE
#undef GET_TRACE_HANDLE
#endif
#define GET_TRACE_HANDLE    CElseIfHandler::instance()->GetTraceHandle()

// Do not care about unreferenced formal parameters
#pragma warning( disable : 4100 )

// Do not care about unreferenced formal parameters
#pragma warning( disable : 4100 )

using namespace STATE_MACHINE_NAMESPACE;


CElseIfHandler::CElseIfHandler(void) 
    : m_pCurrentRule(NULL)
    , hndlrRuleHandler(*CRuleHandler::instance())
    , m_nRuleCnt(0)
{
}

CElseIfHandler::~CElseIfHandler(void)
{
    m_pCurrentRule  = NULL;
}


void CElseIfHandler::registerHandlers(void)
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



DEFINE_START_ELEMENT(CElseIfHandler) 
{
    if (loggable().IsDebug()) ITRACE_DEVELOP_STATIC(GET_TRACE_HANDLE, _T("ElseIf ---------------------------------------------------------"));

    ISMObjectRepository *pRepository = GetLoader()->GetObjectRepository();
    _ASSERTE(pRepository != NULL);

    m_pElseIf = newpooledobject CElseIfImpl;
    m_pElseIf->SetTraceObject(GetTraceHandle());

    StringType sAttr = GET_ATTRIBUTE(XP_IF_ACTION_ATTR_EXPRESSION);
    SAVE_ATTRIBUTE(CElseIfHandler, ExpressionValue, XP_IF_ACTION_ATTR_EXPRESSION, elseif);
    if (loggable().IsDebug()) ITRACE_DEVELOP(_T("expression value:  ") + IString(CElseIfHandler::instance()->elseif().ExpressionValue.Value().c_str()));

    IExpression *pExpression;
    HRESULT hr = pRepository->GetIfExpression(&pExpression
                                  , CElseIfHandler::instance()->
                                        elseif().ExpressionValue.Value().c_str()
                                  , CIfHandler::instance()->ifclause().GetExpression());
    if (hr != S_OK)
        return hr;

    m_pElseIf->SetExpression(pExpression);

    hndlrRuleHandler.PushOwner(&CElseIfHandler::BeginRule, 
                       &CElseIfHandler::EndRule);

    return S_OK;
}
DEFINE_END_ELEMENT(CElseIfHandler)
{
    _ASSERTE(GetLoader() != NULL);

    ISMObjectRepository *pRepository = GetLoader()->GetObjectRepository();
    _ASSERTE(pRepository != NULL);
    
    hndlrRuleHandler.PopOwner();

    CRulesHandler::instance()->SaveRule(m_pElseIf);

    return S_OK;
}


DEFINE_HANDLER(CElseIfHandler, Comment, elseif);


HRESULT CElseIfHandler::BeginRule(CRuleImplBase **ppRule)
{
//  IMODTRACE_DEVELOP();
//  IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);
    CElseIfHandler::instance()->NewRule();

    *ppRule = CElseIfHandler::instance()->m_pCurrentRule;

    return S_OK;
}


HRESULT CElseIfHandler::EndRule()
{
    //IMODTRACE_DEVELOP();
//  IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    CElseIfHandler::instance()->SaveRule();

    return S_OK;
}


void CElseIfHandler::NewRule()  
{ 
    ISMObjectRepository *pRepository = GetLoader()->GetObjectRepository();
    _ASSERTE(pRepository != NULL);

    m_pCurrentRule = newpooledobject CRuleImpl;     
    m_pCurrentRule->SetExpression(pRepository->GetTrueExpression());
    m_nRuleCnt++; 
}


void CElseIfHandler::SaveRule()
{
    if (loggable().IsDebug()) ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Saving Rule:  ") + IString(m_pCurrentRule->ToString().c_str()));

    CRuleQ &rules = elseif().GetRulesList();
    rules.push_back(m_pCurrentRule);
}


void CElseIfHandler::ResetRuleList()
{
    IMODTRACE_DEVELOP();
    m_nRuleCnt = 0;
}


void CElseIfHandler::CheckNullList()
{
    IMODTRACE_DEVELOP();
    if (m_nRuleCnt == 0)
    {
        //if (xpathLoggable().IsTracing())
        //    ITRACE_DEVELOP(__FUNCTION__ _T(" Adding a null rule.  State:  ") + IString(state().StateName.Value().c_str()) 
        //        + _T("  Msg:  ") + IString(currentMsg().Name.Value().c_str()));
        NewRule( );
        SaveRule();
    }
}


void CElseIfHandler::GetXMLOutput(CXMLContent *, CXMLBuilder *)
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
