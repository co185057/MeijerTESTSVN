// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/RulesHandler.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)RulesHandler.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/RulesHandler.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 7     4/21/05 7:27p Dm185016
 * Cleaned up tracing
 * 
 * 5     3/22/05 5:01p Dm185016
 * 
 * 4     2/28/05 3:44p Dm185016
 * lint
 * 
 * 3     2/21/05 8:05p Dm185016
 * Added debugging information to new and delete for pooled objects.
 * 
 * 2     2/10/05 5:57p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 1     1/26/05 2:30p Dm185016
 * Moved to new Repository.
// 
// 12    1/18/05 3:58p Dm185016
// UNICODE support
// 
// 11    1/12/05 6:34p Dm185016
// Select support.
// 
// 10    11/15/04 11:44a Dm185016
// Added TrueInstance and FalseInstance for select/if
// 
// 9     11/09/04 3:39p Dm185016
// Corrected error return code.
// 
// 8     10/29/04 9:31a Dm185016
// Fixed problem with tracing.
// Corrected problem with extra null actions being added to message list.
// 
// 7     10/14/04 1:49p Dm185016
// Now stops when unknown message name detected.
// 
// 6     10/04/04 6:49p Dm185016
// Trace object is no longer shared.
// 
// 4     6/30/04 3:32p Dm185016
// Added validation of action names.
// 
// 3     5/22/04 8:13a Dm185016
// Added code to handle null definition of msg-rule-list.  Generates a
// single null action if no list specified.
// 
// 2     3/25/04 3:23p Dm185016
// Removed trace
// 
// 1     3/19/04 3:35p Dm185016
// New handler split from RuleHandler
*/
#include "StdAfx.h"
#include "RulesHandler.h"
#include "XMLStateMachine.h"
#include "SMLoader.h"
#include "SMConstants.h"
#include "SMError.h"
#include "StateImpl.h"
#include "LoggableManager.h"
#include "Loggable.h"

// Do not care about unreferenced formal parameters
#pragma warning( disable : 4100 )

using namespace STATE_MACHINE_NAMESPACE;


CRulesHandler::CRulesHandler(void) 
    : m_pCurrentRule(NULL)
    , m_eActionDest(NO_DEST)
    , hndlrRuleHandler(*CRuleHandler::instance())
    , m_nRuleCnt(0)
{
}


CRulesHandler::~CRulesHandler(void)
{
    m_pCurrentRule  = NULL;
}


void CRulesHandler::registerHandlers(void)
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);
    SETUP_DECLARED_HANDLER( XP_RULE_STATE_NAME,                                 StateName );
    SETUP_DECLARED_HANDLER( XP_RULE_STATE_GLOBAL,                               Global );
    SETUP_DECLARED_HANDLER( XP_RULE_STATE_EXIT_RULE_LIST,                       ExitList );
    SETUP_DECLARED_HANDLER( XP_RULE_STATE_ENTRY_RULE_LIST,                      EntryList );
    SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG,                                  Message );
    SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_NAME,                             MsgName );
    SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_ACTION_RULE_LIST,                 ActionList );

    SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_RULE_ACTION_FAILURE_TRANS,        FailureTransitionDefault );
ITRACE_DEVELOP("----------------> Need to implement default failure and exception actions <--------------------");
//  SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_RULE_ACTION_FAILURE_ACTION,       FailureAction );
    SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_RULE_ACTION_EXC_TRANS,            ExceptionTransitionDefault );
//  SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_RULE_ACTION_EXC_ACTION,           ExceptionAction );
    SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_RULE_SUCCESS_TRANS,               SuccessTransition );
}



DEFINE_START_ELEMENT(CRulesHandler) 
{
    _ASSERTE(GetLoader() != NULL);

    ISMObjectRepository &repository = *GetLoader()->GetObjectRepository();

    m_pContent = dynamic_cast<CStateObjectProxy*>(repository.CreateNextStateObjectProxy());
    m_pContent->SetTraceObject(GetTraceHandle());

    hndlrRuleHandler.PushOwner(&CRulesHandler::BeginRule, 
                       &CRulesHandler::EndRule);

    return S_OK;
}
DEFINE_END_ELEMENT(CRulesHandler)
{
    _ASSERTE(GetLoader() != NULL);

    CXMLStateMachine *pSM = GetLoader()->GetStateMachine();

    _ASSERTE(pSM != NULL);
    
    hndlrRuleHandler.PopOwner();

    return pSM->Add(&state());
//  return S_OK;
}

DEFINE_HANDLER(CRulesHandler, Comment, state);


DEFINE_CHARACTERS(CRulesHandler, StateName)
{
    StringType szValue = XMLUtilities::getString(pwchChars, cchChars);

    if (CRulesHandler::instance()->state().StateName.IsValid())
        return STATEM_UNDEFINED_STATE_ID;

    CRulesHandler::instance()->state().SaveStateName(szValue);  //lint !e534
    
    return S_OK;
}


DEFINE_HANDLER(CRulesHandler, Global, state);
DEFINE_HANDLER_START_ELEMENT(CRulesHandler, Global)
{
    CRulesHandler::instance()->state().Global.Value(true);
    if (CRulesHandler::instance()->state().StateName.IsValid())
        return SECURITY_GLOBAL_STATE_NAME_NOT_ALLOWED;

    CRulesHandler::instance()->state().StateName.Value(DEFAULT_GLOBAL_STATE_NAME);

    // Create the Global state, since it is not predefined in the list, but is
    // an internal state.
    return CRulesHandler::instance()->CreateGlobalState();
}


HRESULT CRulesHandler::CreateGlobalState()
{
    CStateImpl *pGlobal = newpooledobject CStateImpl(0);
    pGlobal->Name.Value(DEFAULT_GLOBAL_STATE_NAME);
    pGlobal->Global.Value(true);

    _ASSERTE(GetLoader() != NULL);

    CXMLStateMachine *pSM = GetLoader()->GetStateMachine();

    _ASSERTE(pSM != NULL);

    return pSM->Add(pGlobal);
}


DEFINE_HANDLER_START_ELEMENT(CRulesHandler, ExitList)
{
    CRulesHandler::instance()->ResetRuleList( );
    CRulesHandler::instance()->SetDestination( EXIT_LIST );

    return S_OK;
}
DEFINE_HANDLER_END_ELEMENT(CRulesHandler, ExitList)
{
    return S_OK;
}


DEFINE_HANDLER_START_ELEMENT(CRulesHandler, EntryList)
{
    CRulesHandler::instance()->ResetRuleList( );
    CRulesHandler::instance()->SetDestination( ENTRY_LIST );

    return S_OK;
}
DEFINE_HANDLER_END_ELEMENT(CRulesHandler, EntryList)
{
    return S_OK;
}


DEFINE_HANDLER_START_ELEMENT(CRulesHandler, Message)
{
    CRulesHandler::instance()->NewMessage();
    CRulesHandler::instance()->SetDestination( MSG_LIST );
    CRulesHandler::instance()->ResetRuleList();

    return S_OK;
}
DEFINE_HANDLER_END_ELEMENT(CRulesHandler, Message)
{
//  IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    CRulesHandler::instance()->CheckNullList();

    CRulesHandler::instance()->state().ClearRules();

    return S_OK;
}


DEFINE_CHARACTERS(CRulesHandler, MsgName)
{
    HRESULT hr = S_OK;
    StringType szValue = XMLUtilities::getString(pwchChars, cchChars);
    try
    {
        CRulesHandler::instance()->SetMsgName(szValue);
    }
    catch (const IException  &exc)
    {
        ITRACE_EXCEPTION(exc);

        hr = exc.errorId();
    }

    return hr;
}


//DEFINE_HANDLER(CRulesHandler, FailureTransitionDefault, state);
DEFINE_CHARACTERS(CRulesHandler, FailureTransitionDefault)
{
    StringType szValue = XMLUtilities::getString(pwchChars, cchChars);

    if (CRulesHandler::instance()->GetLoader()->GetStateMachine()->GetStateID(szValue.c_str()) == -1)
        return SECURITY_UNDEFINED_STATE_ID;

    CRulesHandler::instance()->state().SaveFailureTransitionDefault(szValue);
    return S_OK;
}


//DEFINE_HANDLER(CRulesHandler, ExceptionTransitionDefault, state);
DEFINE_CHARACTERS(CRulesHandler, ExceptionTransitionDefault)
{
    StringType szValue = XMLUtilities::getString(pwchChars, cchChars);

    if (CRulesHandler::instance()->GetLoader()->GetStateMachine()->GetStateID(szValue.c_str()) == -1)
        return SECURITY_UNDEFINED_STATE_ID;

    CRulesHandler::instance()->state().SaveExceptionTransitionDefault(szValue);
    return S_OK;
}


//DEFINE_HANDLER(CRulesHandler, FailureActionDefault, state);
DEFINE_CHARACTERS(CRulesHandler, FailureActionDefault)
{
    StringType szValue = XMLUtilities::getString(pwchChars, cchChars);

    if (CRulesHandler::instance()->GetLoader()->GetStateMachine()->GetActionID(szValue.c_str()) == -1)
        return SECURITY_UNDEFINED_ACTION_ID;

    CRulesHandler::instance()->state().SaveFailureActionDefault(szValue);
    return S_OK;
}


//DEFINE_HANDLER(CRulesHandler, ExceptionActionDefault, state);
DEFINE_CHARACTERS(CRulesHandler, ExceptionActionDefault)
{
    StringType szValue = XMLUtilities::getString(pwchChars, cchChars);

    if (CRulesHandler::instance()->GetLoader()->GetStateMachine()->GetActionID(szValue.c_str()) == -1)
        return SECURITY_UNDEFINED_ACTION_ID;

    CRulesHandler::instance()->state().SaveExceptionActionDefault(szValue);
    return S_OK;
}


//DEFINE_HANDLER(CRulesHandler, SuccessTransition, state);
DEFINE_CHARACTERS(CRulesHandler, SuccessTransition)
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    StringType szValue = XMLUtilities::getString(pwchChars, cchChars);

    if (CRulesHandler::instance()->GetLoader()->GetStateMachine()->GetStateID(szValue.c_str()) == -1)
        return SECURITY_UNDEFINED_STATE_ID;

    CRulesHandler::instance()->state().SaveSuccessTransition(szValue);
    return S_OK;
}


DEFINE_HANDLER_START_ELEMENT(CRulesHandler, SuccessTransition)
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    CRulesHandler::instance()->CheckNullList();

    return S_OK;
}


void CRulesHandler::SetMsgName(const StringType &szMsgName)
{
    currentMsg().Name.Value(szMsgName);

    CXMLStateMachine *pSM = GetLoader()->GetStateMachine();
    _ASSERTE(pSM!=NULL);
    int nId = pSM->GetMsgID(szMsgName.c_str());
    currentMsg().nId = nId;
}


void CRulesHandler::NewMessage()  {  }


HRESULT CRulesHandler::BeginRule(CRuleImplBase **ppRule)
{
//  IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);
    CRulesHandler::instance()->NewRule();

    *ppRule = CRulesHandler::instance()->m_pCurrentRule;

    return S_OK;
}


HRESULT CRulesHandler::EndRule()
{
//  IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    CRulesHandler::instance()->SaveRule();

    return S_OK;
}


DEFINE_HANDLER_START_ELEMENT(CRulesHandler, ActionList)
{
    CRulesHandler::instance()->ResetRuleList( );

    CRulesHandler::instance()->SetDestination( MSG_LIST );

    return S_OK;
}
DEFINE_HANDLER_END_ELEMENT(CRulesHandler, ActionList)
{
    return S_OK;
}


void CRulesHandler::NewRule()  
{ 
    CXMLStateMachine *pSM = GetLoader()->GetStateMachine();
    _ASSERTE(pSM != NULL);

    m_pCurrentRule = newpooledobject CRuleImpl;     
    m_pCurrentRule->SetExpression(pSM->GetTrueExpression());
    m_nRuleCnt++; 
}


void CRulesHandler::SaveRule()
{
    SaveRule(m_pCurrentRule);
}


void CRulesHandler::SaveRule(CRuleImplBase *pRule)
{
    _ASSERTE(pRule != NULL);
    if (pRule == NULL)
        return; 

    switch(m_eActionDest)
    {
        case EXIT_LIST :
            state().AddExitRule(pRule);
            break;
        case ENTRY_LIST :
            state().AddEntryRule(pRule);
            break;
        case MSG_LIST :
            state().AddMsgRule(&currentMsg(), pRule);
            break;
        case NO_DEST :
            _ASSERT(0);
    }
}


void CRulesHandler::ResetRuleList()
{
    m_nRuleCnt = 0;
}


void CRulesHandler::CheckNullList()
{
    if (m_nRuleCnt == 0)
    {
        if (xpathLoggable().IsTracing())
            ITRACE_DEVELOP(_T(__FUNCTION__) _T(" Adding a null rule.  State:  ") + IString(state().StateName.Value().c_str()) 
                + _T("  Msg:  ") + IString(currentMsg().Name.Value().c_str()));
        NewRule( );
        SaveRule();
    }
}


void CRulesHandler::GetXMLOutput(CXMLContent *, CXMLBuilder *)
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
