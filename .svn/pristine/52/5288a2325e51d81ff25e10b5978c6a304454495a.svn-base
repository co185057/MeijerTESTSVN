// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/IfHandler.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)IfHandler.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/IfHandler.cpp $
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
// 4     1/12/05 6:34p Dm185016
// Select support.
// 
// 3     11/15/04 11:44a Dm185016
// Added TrueInstance and FalseInstance for select/if
// 
// 1     10/04/04 6:48p Dm185016
*/
#include "StdAfx.h"
#include "IfHandler.h"
#include "RulesHandler.h"
#include "SMObjectRepository.h"
#include "SMLoader.h"
#include "SMConstants.h"
#include "StateImpl.h"
#include "LoggableManager.h"
#include "Loggable.h"

// Do not care about unreferenced formal parameters
#pragma warning( disable : 4100 )

// Do not care about unreferenced formal parameters
#pragma warning( disable : 4100 )

using namespace STATE_MACHINE_NAMESPACE;

#ifdef GET_TRACE_HANDLE
#undef GET_TRACE_HANDLE
#endif
#define GET_TRACE_HANDLE    CIfHandler::instance()->GetTraceHandle()


CIfHandler::CIfHandler(void) 
    : m_pCurrentRule(NULL)
    , hndlrRuleHandler(*CRuleHandler::instance())
    , m_nRuleCnt(0)
{
}

CIfHandler::~CIfHandler(void)
{
    m_pCurrentRule  = NULL;
}


void CIfHandler::registerHandlers(void)
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);
}



DEFINE_START_ELEMENT(CIfHandler) 
{
    if (loggable().IsDebug()) ITRACE_DEVELOP_STATIC(GET_TRACE_HANDLE, _T("If -------------------------------------------------------------"));

    ISMObjectRepository *pRepository = GetLoader()->GetObjectRepository();
    _ASSERTE(pRepository != NULL);

    m_pIf = newpooledobject CIfImpl;
    m_pIf->SetTraceObject(GetTraceHandle());

    StringType sAttr = GET_ATTRIBUTE(XP_IF_ACTION_ATTR_EXPRESSION);
    SAVE_ATTRIBUTE(CIfHandler, ExpressionValue, XP_IF_ACTION_ATTR_EXPRESSION, ifclause);
    if (loggable().IsDebug()) ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  expression value:  ") + IString(CIfHandler::instance()->ifclause().ExpressionValue.Value().c_str()));

    IExpression *pExpression;
    HRESULT hr = pRepository->GetIfExpression(&pExpression
                                  , CIfHandler::instance()->
                                        ifclause().ExpressionValue.Value().c_str()
                                  , NULL );
    if (hr != S_OK)
        return hr;

    m_pIf->SetExpression(pExpression);

    hndlrRuleHandler.PushOwner(&CIfHandler::BeginRule, 
                       &CIfHandler::EndRule);

    return S_OK;
}
DEFINE_END_ELEMENT(CIfHandler)
{
    _ASSERTE(GetLoader() != NULL);

    ISMObjectRepository *pRepository = GetLoader()->GetObjectRepository();

    _ASSERTE(pRepository != NULL);
    
    hndlrRuleHandler.PopOwner();

    CRulesHandler::instance()->SaveRule(m_pIf);

    return S_OK;
}


DEFINE_HANDLER(CIfHandler, Comment, ifclause);


HRESULT CIfHandler::BeginRule(CRuleImplBase **ppRule)
{
    //IMODTRACE_DEVELOP_STATIC(GET_TRACE_HANDLE);
    CIfHandler::instance()->NewRule();

    *ppRule = CIfHandler::instance()->m_pCurrentRule;

    return S_OK;
}


HRESULT CIfHandler::EndRule()
{
    //IMODTRACE_DEVELOP_STATIC(GET_TRACE_HANDLE);

    CIfHandler::instance()->SaveRule();

    return S_OK;
}


void CIfHandler::NewRule()  
{ 
    //IMODTRACE_DEVELOP();

    ISMObjectRepository *pRepository = GetLoader()->GetObjectRepository();
    _ASSERTE(pRepository != NULL);

    m_pCurrentRule = newpooledobject CRuleImpl;     
    m_pCurrentRule->SetExpression(pRepository->GetTrueExpression());
    m_nRuleCnt++; 
}


void CIfHandler::SaveRule()
{
    //IMODTRACE_DEVELOP();

    if (loggable().IsDebug()) ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Saving Rule:  ") + IString(m_pCurrentRule->ToString().c_str()));

    CRuleQ &rules = ifclause().GetRulesList();
    rules.push_back(m_pCurrentRule);
}


void CIfHandler::ResetRuleList()
{
    IMODTRACE_DEVELOP();
    m_nRuleCnt = 0;
}


void CIfHandler::CheckNullList()
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


void CIfHandler::GetXMLOutput(CXMLContent *, CXMLBuilder *)
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
