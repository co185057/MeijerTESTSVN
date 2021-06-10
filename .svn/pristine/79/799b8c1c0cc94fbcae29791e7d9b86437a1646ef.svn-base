// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/ElseHandler.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)ElseHandler.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/ElseHandler.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 9     5/03/05 4:14p Dm185016
 * Cleaned up tracing
 * 
 * 8     4/21/05 7:27p Dm185016
 * Cleaned up tracing
 * 
 * 6     3/22/05 5:00p Dm185016
 * <if> support
 * 
 * 5     3/17/05 2:46p Dm185016
 * <if>, <else-if>, <else> support
 * 
 * 4     2/28/05 3:44p Dm185016
 * lint
 * 
 * 3     2/21/05 8:05p Dm185016
 * Added debugging information to new and delete for pooled objects.
 * 
 * 2     2/10/05 5:55p Dm185016
 * Fixed problem with static ids used in SM.
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
// 3     11/15/04 11:44a Dm185016
// Added TrueInstance and FalseInstance for select/if
// 
// 2     10/29/04 9:29a Dm185016
// Fixed problem with tracing.
// 
// 1     10/04/04 6:48p Dm185016
*/
#include "StdAfx.h"
#include "IfHandler.h"
#include "ElseHandler.h"
#include "RulesHandler.h"
#include "SMLoader.h"
#include "SMObjectRepository.h"
#include "SMError.h"
#include "StateImpl.h"
#include "LoggableManager.h"
#include "Loggable.h"

#ifdef GET_TRACE_HANDLE
#undef GET_TRACE_HANDLE
#endif
#define GET_TRACE_HANDLE    CElseHandler::instance()->GetTraceHandle()

// Do not care about unreferenced formal parameters
#pragma warning( disable : 4100 )

using namespace STATE_MACHINE_NAMESPACE;

CElseHandler::CElseHandler(void) 
    : m_pCurrentRule(NULL)
    , m_eActionDest(NO_DEST)
    , hndlrRuleHandler(*CRuleHandler::instance())
    , m_nRuleCnt(0)
{
}

CElseHandler::~CElseHandler(void)
{
    m_pCurrentRule  = NULL;
}


void CElseHandler::registerHandlers(void)
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



DEFINE_START_ELEMENT(CElseHandler) 
{
    if (loggable().IsDebug()) ITRACE_DEVELOP_STATIC(GET_TRACE_HANDLE, _T("Else -----------------------------------------------------------"));

    ISMObjectRepository *pRepository = GetLoader()->GetObjectRepository();
    _ASSERTE(pRepository != NULL);

    m_pElse = newpooledobject CElseImpl;
    m_pElse->SetTraceObject(GetTraceHandle());

    IExpression *pExpression;
    HRESULT hr = pRepository->GetIfExpression(&pExpression
                                  , NULL
                                  , CIfHandler::instance()->ifclause().GetExpression());
    if (hr != S_OK)
        return hr;

    m_pElse->SetExpression(pExpression);

    hndlrRuleHandler.PushOwner(&CElseHandler::BeginRule, 
                       &CElseHandler::EndRule);

    return S_OK;
}
DEFINE_END_ELEMENT(CElseHandler)
{
    _ASSERTE(GetLoader() != NULL);

    ISMObjectRepository *pRepository = GetLoader()->GetObjectRepository();

    _ASSERTE(pRepository != NULL);
    
    hndlrRuleHandler.PopOwner();

    CRulesHandler::instance()->SaveRule(m_pElse);
    
    return S_OK;
}


DEFINE_HANDLER(CElseHandler, Comment, elseclause);


HRESULT CElseHandler::BeginRule(CRuleImplBase **ppRule)
{
    //IMODTRACE_DEVELOP_STATIC(GET_TRACE_HANDLE);

    CElseHandler::instance()->NewRule();

    *ppRule = CElseHandler::instance()->m_pCurrentRule;

    return S_OK;
}


HRESULT CElseHandler::EndRule()
{
    //IMODTRACE_DEVELOP_STATIC(GET_TRACE_HANDLE);

    CElseHandler::instance()->SaveRule();

    return S_OK;
}


void CElseHandler::NewRule()  
{ 
    //IMODTRACE_DEVELOP();

    ISMObjectRepository *pRepository = GetLoader()->GetObjectRepository();
    _ASSERTE(pRepository != NULL);

    m_pCurrentRule = newpooledobject CRuleImpl;     
    m_pCurrentRule->SetExpression(pRepository->GetTrueExpression());
    m_nRuleCnt++; 
}


void CElseHandler::SaveRule()
{
    if (loggable().IsDebug()) ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Saving Rule:  ") + IString(m_pCurrentRule->ToString().c_str()));

    CRuleQ &rules = elseclause().GetRulesList();
    rules.push_back(m_pCurrentRule);

}


void CElseHandler::ResetRuleList()
{
    IMODTRACE_DEVELOP();

    m_nRuleCnt = 0;
}


void CElseHandler::CheckNullList()
{
    if (m_nRuleCnt == 0)
    {
        //if (xpathLoggable().IsTracing())
        //    ITRACE_DEVELOP(_T(__FUNCTION__) _T(" Adding a null rule.  State:  ") + IString(state().StateName.Value().c_str()) 
        //        + _T("  Msg:  ") + IString(currentMsg().Name.Value().c_str()));
        NewRule( );
        SaveRule();
    }
}


void CElseHandler::GetXMLOutput(CXMLContent *, CXMLBuilder *)
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
