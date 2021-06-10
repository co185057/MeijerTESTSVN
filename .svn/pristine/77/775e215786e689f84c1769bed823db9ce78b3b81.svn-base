// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/handlers/AppStateHnd.cpp 4     1/13/10 12:34a Cu185008 $
/*
 * @(#)CApplicationStateHandler.cpp    $Revision: 4 $ $Date: 1/13/10 12:34a $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/handlers/AppStateHnd.cpp $
 * 
 * 4     1/13/10 12:34a Cu185008
 * G2 Migration.
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/01/05 2:05p Dm185016
 * TAR 294587.  Added application state functionality.
 * 
 * 3     2/28/05 3:55p Dm185016
 * lint
 * 
 * 2     2/21/05 8:14p Dm185016
 * Added debugging information to new and delete for pooled objects.
 * 
 * 1     1/26/05 1:56p Dm185016
 * Moved to new Repository.
// 
// 7     1/12/05 6:22p Dm185016
// Select support
// 
// 6     10/29/04 9:19a Dm185016
// 
// 4     6/30/04 3:22p Dm185016
// Made into base class for TAB support.
// 
// 3     5/21/04 9:00a Dm185016
// lint
// 
// 2     4/06/04 1:56p Dm185016
// Added do-not-hold-wt flag
// 
// 1     3/19/04 3:01p Dm185016
// Application state support
*/
#include "StdAfx.h"
#include "AppStateHnd.h"
#include "AppState.h"
#include "RAConstants.h"
#include "RCMStateMachineRAP.h"
#include "RCMLoaderRAP.h"
#include "RCMErrorRAP.h"
#include "RuleImpl.h"

// Do not care about unreferenced formal parameters
#pragma warning( disable : 4100 )

using namespace fastlane;
using namespace rap;
using namespace STATE_MACHINE_NAMESPACE;


CApplicationStateHandler::CApplicationStateHandler(void) 
    : m_eActionDest(NO_DEST)
    , m_pCurrentRule(NULL)
    , m_nRuleCnt(0)
    , hndlrRuleHandler(*CRuleHandler::instance())
{
}


CApplicationStateHandler::~CApplicationStateHandler(void)
{
    m_pCurrentRule = NULL;
}


void CApplicationStateHandler::registerHandlers(void)
{
    IMODTRACE_DEVELOP();
    SETUP_DECLARED_HANDLER( XP_APP_STATE_DEF_STATE_NAME,				Name );
    SETUP_DECLARED_HANDLER( XP_APP_STATE_DEF_COMMENT,			        Comment );
    SETUP_DECLARED_HANDLER( XP_APP_STATE_DEF_PUSH_AM,                   PushAssistModeEnabled );
    SETUP_DECLARED_HANDLER( XP_APP_STATE_DEF_ENABLE_AM,		            AssistModeEnabled );
    SETUP_DECLARED_HANDLER( XP_APP_STATE_DEF_DISABLE_AM,	            AssistModeDisabled );

    SETUP_DECLARED_HANDLER( XP_APP_STATE_EXIT_RULE_LIST,    ExitList );
    SETUP_DECLARED_HANDLER( XP_APP_STATE_ENTRY_RULE_LIST,   EntryList );
}


CApplicationState *CApplicationStateHandler::NewContent()
{
    return newpooledobject CApplicationState;
}


DEFINE_START_ELEMENT(CApplicationStateHandler) 
{
    m_pContent = NewContent();
    m_pContent->SetTraceObject(GetTraceHandle());

    hndlrRuleHandler.PushOwner(&CApplicationStateHandler::BeginRule, 
                       &CApplicationStateHandler::EndRule);

    return S_OK;
}
DEFINE_END_ELEMENT(CApplicationStateHandler)
{
    CRCMStateMachineRAP *pSM = dynamic_cast<CRCMStateMachineRAP *>(GetLoader()->GetStateMachine());

    _ASSERTE(pSM != NULL);

    hndlrRuleHandler.PopOwner();

    return pSM->Add(&appstate());
}


DEFINE_HANDLER_START_ELEMENT(CApplicationStateHandler, ExitList)
{
    CApplicationStateHandler::instance()->ResetRuleList( );
    CApplicationStateHandler::instance()->SetDestination( EXIT_LIST );

    return S_OK;
}
DEFINE_HANDLER_END_ELEMENT(CApplicationStateHandler, ExitList)
{
    return S_OK;
}


DEFINE_HANDLER_START_ELEMENT(CApplicationStateHandler, EntryList)
{
    CApplicationStateHandler::instance()->ResetRuleList( );
    CApplicationStateHandler::instance()->SetDestination( ENTRY_LIST );

    return S_OK;
}
DEFINE_HANDLER_END_ELEMENT(CApplicationStateHandler, EntryList)
{
    return S_OK;
}

DEFINE_HANDLER(CApplicationStateHandler, Name, appstate);

DEFINE_HANDLER(CApplicationStateHandler, Comment, appstate);

DEFINE_HANDLER_START_ELEMENT(CApplicationStateHandler, AssistModeEnabled)
{
    CApplicationStateHandler::instance()->appstate().AssistModeEnabled.Value(true);

    return S_OK;
}


DEFINE_HANDLER_START_ELEMENT(CApplicationStateHandler, AssistModeDisabled)
{
    CApplicationStateHandler::instance()->appstate().AssistModeDisabled.Value(true);

    return S_OK;
}


DEFINE_HANDLER_START_ELEMENT(CApplicationStateHandler, PushAssistModeEnabled)
{
    CApplicationStateHandler::instance()->appstate().PushAssistModeEnabled.Value(true);

    return S_OK;
}


HRESULT CApplicationStateHandler::BeginRule(CRuleImplBase **ppRule)
{
    CApplicationStateHandler::instance()->NewRule();

    *ppRule = CApplicationStateHandler::instance()->m_pCurrentRule;

    return S_OK;
}


HRESULT CApplicationStateHandler::EndRule()
{
    CApplicationStateHandler::instance()->SaveRule();

    return S_OK;
}


void CApplicationStateHandler::NewRule()  
{ 
    IMODTRACE_DEVELOP();

    CXMLStateMachine *pSM = GetLoader()->GetStateMachine();
    _ASSERTE(pSM != NULL);

    m_pCurrentRule = newpooledobject CRuleImpl;     
    m_pCurrentRule->SetExpression(pSM->GetTrueExpression());
    m_nRuleCnt++; 
}


void CApplicationStateHandler::SaveRule()
{
    switch(m_eActionDest)
    {
        case EXIT_LIST :
            appstate().AddExitRule(m_pCurrentRule);
            break;
        case ENTRY_LIST :
            appstate().AddEntryRule(m_pCurrentRule);
            break;
        case NO_DEST :
        default :
            break;
    }
}


void CApplicationStateHandler::ResetRuleList()
{
    m_nRuleCnt = 0;
}


void CApplicationStateHandler::GetXMLOutput(CXMLContent * /* pContent */, CXMLBuilder * /* pTheBuilder */)
{
    //CXMLBuilder &builder = *((pTheBuilder == NULL) ? this : pTheBuilder);

    //CApplicationState &state = *(CApplicationState *)pContent;
}
