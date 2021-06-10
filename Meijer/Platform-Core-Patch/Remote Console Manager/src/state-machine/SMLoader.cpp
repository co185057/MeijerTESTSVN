// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMLoader.cpp 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)SMLoader.cpp    $Revision: 1 $ $Date: 6/20/08 1:13p $
 *
 * Copyright 2003 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMLoader.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 9     4/21/05 7:23p Dm185016
 * Made entry/exit tracing conditional
 * 
 * 7     3/17/05 2:46p Dm185016
 * <if>, <else-if>, <else> support
 * 
 * 6     2/28/05 3:54p Dm185016
 * lint
 * 
 * 5     2/14/05 7:09p Dm185016
 * Release Exclusive Access Event support
 * 
 * 4     2/10/05 5:40p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 3     2/04/05 10:41a Dm185016
 * 
 * 2     2/04/05 10:38a Dm185016
 * Factored out base functionality.  New base class - XMLDocumentLoader.
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
// 
// 17    1/18/05 3:55p Dm185016
// UNICODE support
// 
// 16    1/16/05 2:36p Dm185016
// Changed default value for missing handler to pass.
// 
// 15    1/12/05 6:28p Dm185016
// Select support.
// 
// 14    12/22/04 11:19a Dm185016
// Added <otherwise> tag.
// 
// 13    11/16/04 11:48a Dm185016
// Removed trace
// 
// 12    11/09/04 3:34p Dm185016
// Support for state lookup tables
// 
// 11    10/29/04 9:24a Dm185016
// Added <select-action>, <when>
// Added regular expressions to xpath
// 
// 10    10/04/04 6:41p Dm185016
// Trace object is no longer shared.
// 
// 8     6/30/04 3:27p Dm185016
// Lint changes
// Made trace output dependent on data capture mask
// 
// 7     5/18/04 7:23p Dm185016
// Added trace
// 
// 6     4/30/04 1:03p Dm185016
// Added <include>
// 
// 5     3/25/04 3:16p Dm185016
// Removed extra trace
// 
// 4     3/19/04 3:03p Dm185016
// Memory leak fix
// 
// 3     1/27/04 1:59p Dm185016
// Action Library support
// 
// 2     1/16/04 9:10a Dm185016
// Timers, Constants, Namespace, New error reporting mechanism
*/
#include "StdAfx.h"
#include "XmlStateMachine.h"
#include "SMLoader.h"
#include "SMError.h"
//#include "stdio.h"
#include "xtrace.h"

#include "VariableHandler.h"
#include "StateHandler.h"
#include "MsgHandler.h"
#include "ActionHandler.h"
#include "RulesHandler.h"
#include "SelectHandler.h"
#include "WhenHandler.h"
#include "OtherwiseHandler.h"
#include "IfHandler.h"
#include "ElseHandler.h"
#include "ElseIfHandler.h"
#include "StateTableHandler.h"
#include "LookupTableHandler.h"
#include "ConstantHandler.h"
#include "TimerHandler.h"
#include "LibraryHandler.h"
#include "LoggableManager.h"
#include "Loggable.h"
#include "SMConstants.h"
#include "Registry.h"
#include "SMRegistry.h"

// Do not care about unreferenced formal parameters
#pragma warning( disable : 4100 )

using namespace STATE_MACHINE_NAMESPACE;


CSMLoader::CSMLoader(HTRACE hTrace)
    : CXMLDocumentLoader(hTrace)
    , m_pStateMachine(NULL)
    , m_bFailOnMissingHandler(false)
{
    CTOR_BEGIN(hTrace)

    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    CRegistry registry;
    StringType sMissingHandler = CRegistryValue<const _TCHAR *>::GetRegistryValue( registry
                                           , HKEY_SM
                                           , REG_VALUE_ON_MISSING_HANDLER
                                           , VALUE_HANDLER_PASS );
    m_bFailOnMissingHandler = (sMissingHandler == VALUE_HANDLER_FAIL);

    CSMLoader::registerHandlers();

    CTOR_END()
}


CSMLoader::~CSMLoader(void)
{
}


void CSMLoader::registerHandlers(void)
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    hndlrIncludeHandler.m_pOwner        = this;
    hndlrVariableListHandler.m_pOwner   = this;
    hndlrConstantListHandler.m_pOwner   = this;
    CConstantHandler::instance()->SetLoader(this);
    CTimerHandler::instance()->SetLoader(this);
    CLibraryHandler::instance()->SetLoader(this);
    CVariableHandler::instance()->SetLoader(this);
    CStateHandler::instance()->SetLoader(this);
    CMsgHandler::instance()->SetLoader(this);
    CActionHandler::instance()->SetLoader(this);
    CRulesHandler::instance()->SetLoader(this);
    CRuleHandler::instance()->SetLoader(this);
    CIfHandler::instance()->SetLoader(this);
    CSelectHandler::instance()->SetLoader(this);
    CWhenHandler::instance()->SetLoader(this);
    COtherwiseHandler::instance()->SetLoader(this);
    CElseHandler::instance()->SetLoader(this);
    CElseIfHandler::instance()->SetLoader(this);
    CStateTableHandler::instance()->SetLoader(this);
    CLookupTableHandler::instance()->SetLoader(this);

    SETUP_NULL_HANDLER( XP_STATE_MACHINE );

    SETUP_DECLARED_HANDLER( XP_INCLUDE,                         IncludeHandler );

    SETUP_NULL_HANDLER( XP_LIBRARY_LIST );
    SETUP_HANDLER( XP_LIBRARY,                                  LibraryHandler );
        
    SETUP_DECLARED_HANDLER( XP_VAR_LIST,                        VariableListHandler );
    SETUP_HANDLER( XP_VARIABLE,                                 VariableHandler );
        
    SETUP_DECLARED_HANDLER( XP_CONSTANT_LIST,                   ConstantListHandler );
    SETUP_HANDLER( XP_CONSTANT,                                 ConstantHandler );
        
    SETUP_NULL_HANDLER( XP_TIMER_LIST );
    SETUP_HANDLER( XP_TIMER,                                    TimerHandler );
        
    SETUP_NULL_HANDLER( XP_STATE_LIST );
    SETUP_HANDLER( XP_STATE_DEFINITION,                         StateHandler );

    SETUP_HANDLER( XP_STATE_TABLE,                              StateTableHandler );

    SETUP_HANDLER( XP_LOOKUP_TABLE,                             LookupTableHandler );
        
    SETUP_NULL_HANDLER( XP_MSG_LIST );
    SETUP_HANDLER( XP_MSG_DEFINITION,                           MsgHandler );
        
    SETUP_NULL_HANDLER( XP_ACTION_LIST );
    SETUP_HANDLER( XP_ACTION,                                   ActionHandler );
        
    SETUP_NULL_HANDLER( XP_RULE_LIST );
    SETUP_HANDLER( XP_RULE_STATE,                               RulesHandler );
    SETUP_HANDLER( XP_RULE_STATE_MSG_RULE,                      RuleHandler );
    SETUP_HANDLER( XP_RULE_STATE_MSG_SELECT,                    SelectHandler );
    SETUP_HANDLER( XP_RULE_STATE_MSG_SELECT_WHEN,               WhenHandler );
    SETUP_HANDLER( XP_RULE_STATE_MSG_SELECT_OTHERWISE,          OtherwiseHandler );
    SETUP_HANDLER( XP_RULE_STATE_MSG_IF,                        IfHandler );
    SETUP_HANDLER( XP_RULE_STATE_MSG_ELSE,                      ElseHandler );
    SETUP_HANDLER( XP_RULE_STATE_MSG_ELSEIF,                    ElseIfHandler );
}

#define ATTR_NAME L"name"
DEFINE_HANDLER_START_ELEMENT(CSMLoader, IncludeHandler)
{
    m_sIncname.Value( XMLUtilities::getAttribute(
                            (const wchar_t *)pwchNamespaceUri,
                            cchNamespaceUri,
                            ATTR_NAME,
                            (int)wcslen(ATTR_NAME),
                            pAttributes) );
    if (m_sIncname.Value().size() == 0)
        return SECURITY_MISSING_INCLUDE_NAME_ATTR;

    ITRACE_DEVELOP(IString(_T(" Include file name:  ")) + m_sIncname.Value().c_str());

    return S_OK;
}
DEFINE_HANDLER_END_ELEMENT(CSMLoader, IncludeHandler)
{
    m_pOwner->ReadFile(m_sIncname.Value().c_str());

    return S_OK;
}


DEFINE_HANDLER_END_ELEMENT(CSMLoader, VariableListHandler)
{
    m_pOwner->InitVariables();

    return S_OK;
}


void CSMLoader::InitVariables()
{
    stateMachine().InitVariables();
}


DEFINE_HANDLER_END_ELEMENT(CSMLoader, ConstantListHandler)
{
    m_pOwner->InitConstants();

    return S_OK;
}


void CSMLoader::InitConstants()
{
    stateMachine().InitConstants();
}


void CSMLoader::ReadFile(const TCHAR *pFn)
{
    stateMachine().ReadFile(pFn);
}


void CSMLoader::SetStateMachine(CXMLStateMachine *pSM)
{
    m_pStateMachine = pSM;
    m_pObjectRepository = (ISMObjectRepository*)pSM;
}


void CSMLoader::SetObjectRepository(ISMObjectRepository *pOR)
{
    m_pObjectRepository = pOR;
}
