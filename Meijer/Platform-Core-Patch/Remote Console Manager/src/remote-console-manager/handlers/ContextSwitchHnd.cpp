// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/handlers/ContextSwitchHnd.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ContextSwitchHnd.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/handlers/ContextSwitchHnd.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     2/28/05 4:25p Dm185016
 * lint
 * 
 * 2     2/21/05 8:07p Dm185016
 * Added debugging information to new and delete for pooled objects.
 * 
 * 1     1/26/05 2:25p Dm185016
 * Moved to new Repository.
// 
// 2     1/12/05 6:13p Dm185016
// Select support.
// 
// 1     11/16/04 11:45a Dm185016
*/
#include "StdAfx.h"
#include "ContextSwitchHnd.h"
#include "ContextSwitch.h"
#include "RCMConstants.h"
#include "RCMStateMachine.h"
#include "RCMLoader.h"
#include "RCMError.h"
#include "RuleHandler.h"

// Do not care about unreferenced formal parameters
#pragma warning( disable : 4100 )

using namespace fastlane;
using namespace STATE_MACHINE_NAMESPACE;


CContextSwitchHandler::CContextSwitchHandler(void) 
    : pLoader(NULL)
    , m_eActionDest(NO_DEST)
    , m_pCurrentRule(NULL)
    , m_nRuleCnt(0)
    , hndlrRuleHandler(*CRuleHandler::instance())
{
}


CContextSwitchHandler::~CContextSwitchHandler(void)
{
    pLoader = NULL;
    m_pCurrentRule = NULL;
}


void CContextSwitchHandler::registerHandlers(void)
{
    IMODTRACE_DEVELOP();
    SETUP_DECLARED_HANDLER( XP_CONTEXT_SWITCH_NAME,				Name );
    SETUP_DECLARED_HANDLER( XP_CONTEXT_SWITCH_COMMENT,			Comment );
    SETUP_DECLARED_HANDLER( XP_CONTEXT_SWITCH_PUSH_IC,   	    PushInputControlSettings );
    SETUP_DECLARED_HANDLER( XP_CONTEXT_SWITCH_DISABLE_IC,       DisableInputControl );
    SETUP_DECLARED_HANDLER( XP_CONTEXT_SWITCH_ENABLE_IC, 	    EnableInputControl );

    //SETUP_DECLARED_HANDLER( XP_APPLICATION_STATE_EXIT_RULE_LIST,    ExitList );
    //SETUP_DECLARED_HANDLER( XP_APPLICATION_STATE_ENTRY_RULE_LIST,   EntryList );
}


CContextSwitch *CContextSwitchHandler::NewContent()
{
    return newpooledobject CContextSwitch;
}


DEFINE_START_ELEMENT(CContextSwitchHandler) 
{
    m_pContent = NewContent();
    m_pContent->SetTraceObject(GetTraceHandle());

    hndlrRuleHandler.PushOwner(&CContextSwitchHandler::BeginRule, 
                       &CContextSwitchHandler::EndRule);

    return S_OK;
}
DEFINE_END_ELEMENT(CContextSwitchHandler)
{
    _ASSERTE(pLoader != NULL);

    CRCMStateMachine *pSM = dynamic_cast<CRCMStateMachine *>(pLoader->GetStateMachine());

    _ASSERTE(pSM != NULL);
    
    hndlrRuleHandler.PopOwner();

    return pSM->Add(&contextSwitch());
}


DEFINE_HANDLER_START_ELEMENT(CContextSwitchHandler, ExitList)
{
    CContextSwitchHandler::instance()->ResetRuleList( );
    CContextSwitchHandler::instance()->SetDestination( EXIT_LIST );

    return S_OK;
}
DEFINE_HANDLER_END_ELEMENT(CContextSwitchHandler, ExitList)
{
    return S_OK;
}


DEFINE_HANDLER_START_ELEMENT(CContextSwitchHandler, EntryList)
{
    CContextSwitchHandler::instance()->ResetRuleList( );
    CContextSwitchHandler::instance()->SetDestination( ENTRY_LIST );

    return S_OK;
}
DEFINE_HANDLER_END_ELEMENT(CContextSwitchHandler, EntryList)
{
    return S_OK;
}

DEFINE_HANDLER(CContextSwitchHandler, Name, contextSwitch);

DEFINE_HANDLER(CContextSwitchHandler, Comment, contextSwitch);

DEFINE_HANDLER_START_ELEMENT(CContextSwitchHandler, PushInputControlSettings)
{
    CContextSwitchHandler::instance()->contextSwitch().PushInputControlSettings.Value(true);

    return S_OK;
}

DEFINE_HANDLER_START_ELEMENT(CContextSwitchHandler, DisableInputControl)
{
    CContextSwitchHandler::instance()->contextSwitch().DisableInputControl.Value(true);

    return S_OK;
}

DEFINE_HANDLER_START_ELEMENT(CContextSwitchHandler, EnableInputControl)
{
    CContextSwitchHandler::instance()->contextSwitch().EnableInputControl.Value(true);

    return S_OK;
}


HRESULT CContextSwitchHandler::BeginRule(CRuleImplBase **ppRule)
{
    CContextSwitchHandler::instance()->NewRule();

    *ppRule = CContextSwitchHandler::instance()->m_pCurrentRule;

    return S_OK;
}


HRESULT CContextSwitchHandler::EndRule()
{
    CContextSwitchHandler::instance()->SaveRule();

    return S_OK;
}


void CContextSwitchHandler::NewRule()  
{ 
    m_pCurrentRule = newpooledobject CRuleImpl;     
    m_nRuleCnt++; 
}


void CContextSwitchHandler::SaveRule()
{
    switch(m_eActionDest)
    {
        case EXIT_LIST :
            contextSwitch().AddExitRule(m_pCurrentRule);
            break;
        case ENTRY_LIST :
            contextSwitch().AddEntryRule(m_pCurrentRule);
            break;
        case NO_DEST :
        default :
            break;
    }
}


void CContextSwitchHandler::ResetRuleList()
{
    m_nRuleCnt = 0;
}


void CContextSwitchHandler::GetXMLOutput(CXMLContent * /* pContent */, CXMLBuilder * /* pTheBuilder */)
{
    //CXMLBuilder &builder = *((pTheBuilder == NULL) ? this : pTheBuilder);

    //CApplicationState &state = *(CApplicationState *)pContent;
}
