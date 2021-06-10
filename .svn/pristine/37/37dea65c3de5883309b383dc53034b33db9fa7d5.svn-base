// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/XMLStateMachine.cpp 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)XMLStateMachine.cpp      $Revision: 1 $  $Date: 6/20/08 1:13p $
 *
 * Copyright 2004, NCR Corporation.
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/XMLStateMachine.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 18    5/31/05 11:35a Dm185016
 * TAR 301203
 * 
 * 16    5/03/05 4:05p Dm185016
 * Cleaned up tracing of state machine
 * 
 * 15    4/21/05 7:25p Dm185016
 * Made entry/exit tracing conditional.
 * Added more heap corruption checks.
 * 
 * 13    4/05/05 12:02p Dm185016
 * Added ability to set the location from which the xml will be read.
 * 
 * 12    3/28/05 5:06p Dm185016
 * TAR 296771
 * 
 * 11    3/22/05 4:33p Dm185016
 * <if> support
 * UNICODE
 * 
 * 10    3/11/05 1:35p Dm185016
 * <select> support
 * 
 * 9     2/28/05 6:02p Dm185016
 * Unicode fix
 * 
 * 8     2/28/05 3:54p Dm185016
 * lint
 * 
 * 7     2/24/05 2:32p Tk124825
 * TAR 292704
 * 
 * 6     2/21/05 8:06p Dm185016
 * Added debugging information to new and delete for pooled objects.
 * 
 * 5     2/18/05 4:35p Dm185016
 * Removed trace
 * 
 * 4     2/14/05 7:10p Dm185016
 * Release Exclusive Access Event support
 * 
 * 3     2/10/05 5:44p Dm185016
 * Fixed problem with static ids used in SM.
 * TAR 292915
 * 
 * 2     1/27/05 2:38p Dm185016
 * Fixed exception when an error is detected in XML.
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
// 
// 33    1/25/05 3:49p Dm185016
// TAR 287899 - Memory leak during termination.
// 
// 32    1/18/05 3:56p Dm185016
// UNICODE support
// 
// 31    1/12/05 6:29p Dm185016
// Select support.
// 
// 30    12/22/04 11:21a Dm185016
// Conditional logic support
// 
// 29    12/07/04 4:28p Dm185016
// Reworking CRulesList for <select>
// 
// 28    11/15/04 11:42a Dm185016
// Moved return code from action to message.
// 
// 27    11/09/04 3:36p Dm185016
// Support for state lookup tables
// 
// 26    10/29/04 9:25a Dm185016
// Added goto-state action.
// 
// 25    10/14/04 1:47p Dm185016
// Now stops when unknown message name detected.
// 
// 24    10/04/04 6:43p Dm185016
// Trace object is no longer shared.
// 
// 22    8/26/04 10:00p Dm185016
// TAR 278069 - Added validity check for messages pulled off queue
// 
// 21    8/17/04 6:23p Dm185016
// Added trace for failure to obtain message.
// 
// 20    6/30/04 3:29p Dm185016
// Made changes required for SecurityStateMachine derivative.
// 
// 18    5/18/04 1:51p Dm185016
// lint
// 
// 17    5/05/04 8:11a Dm185016
// Fixed compiler warning
// 
// 16    5/04/04 4:45p Dm185016
// Added validation of identifiers
// 
// 15    4/30/04 1:03p Dm185016
// Added <include>
// 
// 14    4/21/04 3:10p Dm185016
// Better trace for state names
// 
// 13    4/20/04 4:44p Dm185016
// Expanded the error reporting mechanism.
// 
// 12    4/16/04 4:18p Dm185016
// Fixed return code
// 
// 11    4/01/04 4:48p Dm185016
// Added registration for pop-state
// 
// 10    4/01/04 1:50p Dm185016
// Fixed AppControl
// 
// 9     3/31/04 4:00p Dm185016
// Added override for asynchronous copy
// 
// 8     3/31/04 10:13a Dm185016
// Added external GetCurrentState()
// 
// 6     3/19/04 3:03p Dm185016
// Memory leak fix
// 
// 5     2/24/04 3:33p Dm185016
// Added message data fields
// 
// 4     2/10/04 11:43a Dm185016
// Added synchronous operation option.
// Added Expressions
// Added set-return-code internal action
*/

/*lint -save -e661 */
#include "StdAfx.h"
#include "XMLStateMachine.h"
#include "XMLParser.h"
#include "XMLRule.h"
#include "SMLoader.h"
#include "SMErrorHandler.h"
#include "SMLexicalHandler.h"
#include "SMError.h"
#include "MessageObject.h"
#include "ActionBase.h"
#include "NullAction.h"
#include "ActionImpl.h"
#include "ConstantImpl.h"
#include "LibraryImpl.h"
#include "TimerImpl.h"
#include "VariableImpl.h"
#include "StateImpl.h"
#include "RuleImpl.h"
#include "MsgImpl.h"
#include "Loggable.h"
#include "VariableOperations.h"
#include "TimerOperations.h"
#include "ResendMsg.h"
#include "SendMsg.h"
#include "EvaluateExpression.h"
#include "SetReturnCode.h"
#include "PushState.h"
#include "PopState.h"
#include "GotoState.h"
#include "CleanState.h"
#include "ExpressionFactory.h"
#include "Createable.h"

using namespace STATE_MACHINE_NAMESPACE;
 CXMLStateMachine::CXMLStateMachine(HTRACE hTrace)
     : CAbstractStateMachine(hTrace)
     , m_ExprFactory(this)
     , m_NullAction(new CNullAction)
     , m_nCurrentState(INVALID_STATE)
     , m_pCurrentState(NULL)
     , m_nGlobalState(INVALID_STATE)
     , m_pGlobalState(NULL)
	 , m_pLoader(NULL)
     , m_nMaxActionID(0)
     , m_nMaxMsgID(0)
     , m_nMaxStateID(0)
     , m_nMaxVariableID(0)
     , m_nMaxTimerID(0)
     , m_nMaxLibraryID(0)
     , m_nMaxConstantID(0)
     , m_nMaxStateObjectID(0)
     , m_nMaxStateTableID(0)
     , m_nMaxLookupTableID(0)
     , m_pActions(NULL)
     , m_pMsgs(NULL)
     , m_pStates(NULL)
     , m_pVariables(NULL)
     , m_pTimers(NULL)
     , m_pLibraries(NULL)
     , m_pConstants(NULL)
     , m_pErrorInfo(NULL)
     , m_pStateTables(NULL)
     , m_pLookupTables(NULL)
     , m_nStateCounter(0)
     , m_nMsgCounter(0)
     , m_nActionCounter(0)
     , m_nVariableCounter(0)
     , m_nTimerCounter(0)
     , m_nConstantCounter(0)
     , m_nStateObjectProxyCounter(0)
     , m_nLibraryCounter(0)
     , m_nRuleCounter(0)
     , m_nSelectCounter(0)
     , m_nWhenCounter(0)
     , m_nStateLookupTableCounter(0)
     , m_nLookupTableCounter(0)
     , m_pLoggable(NULL)
     //, m_SyncLock(hTrace)
{
    m_NullAction->SetTraceObject(hTrace);
    actionFactory.SetTraceObject(hTrace);
    m_ExprFactory.SetTraceObject(hTrace);

    m_DefaultStateID        = CXMLStateMachine::GetDefaultStateID();
    m_RemainTheSameStateID  = CXMLStateMachine::GetRemainTheSameStateID();

    try
    {
        CA2T tId(typeid(CMessageObject).name());
        m_sMsgTypeName = tId;
    }
    catch( const __non_rtti_object  &e )
        { ITRACE_EXCEPTION(e); }
    catch( const bad_typeid &e )
        { ITRACE_EXCEPTION(e); }
}


/**
* Constructor taking the name of the file containing the XML rules defining this
* state machine
*/
#pragma warning( disable : 4355 )
 CXMLStateMachine::CXMLStateMachine(const TCHAR *pFn, HTRACE hTrace)
     : CAbstractStateMachine(hTrace)
     , m_ExprFactory(this)
	 , m_sFn(pFn)
     , m_sURL(pFn)
     , m_NullAction(new CNullAction)
     , m_nCurrentState(INVALID_STATE)
     , m_pCurrentState(NULL)
     , m_nGlobalState(INVALID_STATE)
     , m_pGlobalState(NULL)
	 , m_pLoader(NULL)
     , m_nMaxActionID(0)
     , m_nMaxMsgID(0)
     , m_nMaxStateID(0)
     , m_nMaxVariableID(0)
     , m_nMaxTimerID(0)
     , m_nMaxLibraryID(0)
     , m_nMaxConstantID(0)
     , m_nMaxStateObjectID(0)
     , m_nMaxStateTableID(0)
     , m_nMaxLookupTableID(0)
     , m_pActions(NULL)
     , m_pMsgs(NULL)
     , m_pStates(NULL)
     , m_pVariables(NULL)
     , m_pTimers(NULL)
     , m_pLibraries(NULL)
     , m_pConstants(NULL)
     , m_pErrorInfo(NULL)
     , m_pStateTables(NULL)
     , m_pLookupTables(NULL)
     , m_nStateCounter(0)
     , m_nMsgCounter(0)
     , m_nActionCounter(0)
     , m_nVariableCounter(0)
     , m_nTimerCounter(0)
     , m_nConstantCounter(0)
     , m_nStateObjectProxyCounter(0)
     , m_nLibraryCounter(0)
     , m_nRuleCounter(0)
     , m_nSelectCounter(0)
     , m_nWhenCounter(0)
     , m_nStateLookupTableCounter(0)
     , m_nLookupTableCounter(0)
     , m_pLoggable(NULL)
     //, m_SyncLock(hTrace)
{
    m_NullAction->SetTraceObject(hTrace);
    actionFactory.SetTraceObject(hTrace);
    m_ExprFactory.SetTraceObject(hTrace);

    m_DefaultStateID        = CXMLStateMachine::GetDefaultStateID();
    m_RemainTheSameStateID  = CXMLStateMachine::GetRemainTheSameStateID();

    try
    {
        CA2T tId(typeid(CMessageObject).name());
        m_sMsgTypeName = tId;
    }
    catch( const __non_rtti_object  &e )
        { ITRACE_EXCEPTION(e); }
    catch( const bad_typeid &e )
        { ITRACE_EXCEPTION(e); }
}


//lint --e{18, 31, 534}
CXMLStateMachine::~CXMLStateMachine()
{
    IMODTRACE_DEVELOP();

    try
    {
        actionFactory.Clear();

        for (unsigned i=1;
            i<=m_nMaxActionID;
            i++)
        {
            delete m_pActions[i];
        }
        _ASSERTE( _CrtCheckMemory() );
        delete [] m_pActions;
        _ASSERTE( _CrtCheckMemory() );

        for (unsigned i=1;
            i<=m_nMaxMsgID;
            i++)
        {
            delete m_pMsgs[i];
        }
        _ASSERTE( _CrtCheckMemory() );
        delete [] m_pMsgs;
        _ASSERTE( _CrtCheckMemory() );

        if (m_pStates[0] != NULL)
            delete m_pStates[0];
        for (unsigned i=1;
            i<=m_nMaxStateID;
            i++)
        {
            delete m_pStates[i];
        }
        _ASSERTE( _CrtCheckMemory() );
        delete [] m_pStates;
        _ASSERTE( _CrtCheckMemory() );

        for (unsigned i=1;
            i<=m_nMaxVariableID;
            i++)
        {
            delete m_pVariables[i];
        }
        _ASSERTE( _CrtCheckMemory() );
        delete [] m_pVariables;
        _ASSERTE( _CrtCheckMemory() );

        for (unsigned i=1;
            i<=m_nMaxTimerID;
            i++)
        {
            delete m_pTimers[i];
        }
        _ASSERTE( _CrtCheckMemory() );
        delete [] m_pTimers;
        _ASSERTE( _CrtCheckMemory() );

        for (unsigned i=1;
            i<=m_nMaxConstantID;
            i++)
        {
            delete m_pConstants[i];
        }
        _ASSERTE( _CrtCheckMemory() );
        delete [] m_pConstants;
        _ASSERTE( _CrtCheckMemory() );

        for (unsigned i=1;
            i<=m_nMaxLibraryID;
            i++)
        {
            delete m_pLibraries[i];
        }
        _ASSERTE( _CrtCheckMemory() );
        delete [] m_pLibraries;
        _ASSERTE( _CrtCheckMemory() );

        for (unsigned i=1;
            i<=m_nMaxStateTableID;
            i++)
        {
            delete m_pStateTables[i];
        }
        _ASSERTE( _CrtCheckMemory() );
        delete [] m_pStateTables;
        _ASSERTE( _CrtCheckMemory() );

        for (unsigned i=1;
            i<=m_nMaxLookupTableID;
            i++)
        {
            delete m_pLookupTables[i];
        }
        _ASSERTE( _CrtCheckMemory() );
        delete [] m_pLookupTables;
        _ASSERTE( _CrtCheckMemory() );

        delete m_NullAction;
        _ASSERTE( _CrtCheckMemory() );

        expressions.clear();

        //m_pExpressions  = NULL;

        delete m_pErrorInfo;
        m_pErrorInfo = NULL;
        _ASSERTE( _CrtCheckMemory() );
    }
    catch (...)
        { ITRACE_DOT_ERROR(); }
}


void CXMLStateMachine::SetLocation(const _TCHAR *pcszLocation)
{
    m_sLocation = pcszLocation;
}


StringType CXMLStateMachine::GetLocation()
{
    StringType sLocation = m_sLocation;

    if (sLocation.empty())
    {
	    size_t index = m_sFn.find_last_of('\\');
        if (index != StringType::npos)
	    {
		    sLocation = m_sFn.substr(0, index+1);
		    m_sFn		= m_sFn.substr(index+1);
	    }
	    else
	    {
		    sLocation = _T(".\\");
	    }
    }

    ITRACE_DEVELOP(_T("Location is ") + IString(sLocation.c_str()));

    return sLocation;
}


void CXMLStateMachine::Load()
{
    Init();

    RegisterAll();
}



CSMLoader *CXMLStateMachine::GetLoader()
{
	if (m_pLoader == NULL)
		m_pLoader = new CSMLoader(GetTraceHandle());

    return m_pLoader;
}


void CXMLStateMachine::ReleaseLoader()
{
	IMODTRACE_DEVELOP();

    delete m_pLoader;
	m_pLoader = NULL;
}


SAXErrorHandlerImpl *CXMLStateMachine::GetErrorHandler()
{
    SAXErrorHandlerImpl *pHandler = new CSMErrorHandler;
    pHandler->SetTraceObject(GetTraceHandle());
    return pHandler;
}


SAXLexicalHandlerImpl *CXMLStateMachine::GetLexicalHandler()
{
    SAXLexicalHandlerImpl *pHandler = new CSMLexicalHandler;
    pHandler->SetTraceObject(GetTraceHandle());
    return pHandler;
}


void CXMLStateMachine::Init()
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    // Create the XML file loader and set it in the parser
    GetLoader()->SetStateMachine(this);

    m_sLocation = GetLocation();

    ReadFile(m_sFn.c_str());

    _ASSERTE( _CrtCheckMemory() );
    InitLibraries();

    _ASSERTE( _CrtCheckMemory() );
    InitActions();

    _ASSERTE( _CrtCheckMemory() );
    InitMsgs();

    _ASSERTE( _CrtCheckMemory() );
    InitStates();

    _ASSERTE( _CrtCheckMemory() );
    InitVariables();

    _ASSERTE( _CrtCheckMemory() );
    InitTimers();

    _ASSERTE( _CrtCheckMemory() );
    InitConstants();

    _ASSERTE( _CrtCheckMemory() );
    InitStateTables();

    _ASSERTE( _CrtCheckMemory() );
    InitLookupTables();

    _ASSERTE( _CrtCheckMemory() );

    ReleaseLoader();

    if (m_nCurrentState == INVALID_STATE)
        ITHROWSMERROR(STATEM_NO_INITIAL_STATE)
    _ASSERTE( _CrtCheckMemory() );

    CAbstractStateMachine::Init();
    _ASSERTE( _CrtCheckMemory() );
}


void CXMLStateMachine::SetURL(const _TCHAR *pszURL)
{
    m_sURL  = pszURL;
    m_sFn   = pszURL;
}


const _TCHAR *CXMLStateMachine::GetURL() const
{
    return m_sURL.c_str();
}


void CXMLStateMachine::ReadFile(const TCHAR *pFn)
{
	StringType sFn = m_sLocation + pFn;

    if (loggable().IsDebug()) ITRACE_DEVELOP(_T(__FUNCTION__) _T(" Reading:  ") 
                                           + IString(sFn.c_str()));
	CFileNameSet::iterator iter = fnames.find(sFn.c_str());
	if (iter != fnames.end())
		return;

	fnames.insert(sFn);

    SAXLexicalHandlerImpl *pLexicalHandler  = GetLexicalHandler();
    SAXErrorHandlerImpl *pErrorHandler      = GetErrorHandler();

    // Create the XML parser.
    CXMLParser parser(GetTraceHandle());

    // Create the Lexical handler for the parser
    parser.SetLexicalHandler(pLexicalHandler);

    // Create the error handler for the parser
    parser.SetErrorHandler(pErrorHandler);

    parser.SetContentHandler(GetLoader());

    // Parse the document
	HRESULT hr = parser.ParseURL(sFn.c_str());

    if (hr != S_OK)
    {
    	const CXMLErrorInfo &errorInfo = parser.GetErrorInfo();
        ITHROWSMERRORI(hr, errorInfo)
    }
}


void CXMLStateMachine::InitActions()
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    if (m_pActions != NULL)
        delete m_pActions;

    const unsigned maxID = m_nMaxActionID+1;
    m_pActions  = new PACTION[maxID];
    memset(m_pActions, 0, sizeof(PACTION)*maxID);

    CActionsMap::iterator actionsIter;
    IAction *pAction;
    for (actionsIter = actions.begin();
         actionsIter != actions.end();
         actionsIter++)
    {
        pAction = actionsIter->second;

        m_pActions[pAction->GetId()] = pAction;
    }
    m_pActions[0] = m_NullAction;
}


void CXMLStateMachine::InitMsgs()
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    if (m_pMsgs != NULL)
        delete m_pMsgs;

    const unsigned maxID = m_nMaxMsgID+1;
    m_pMsgs     = new PMESSAGE[maxID];
    memset(m_pMsgs, 0, sizeof(PMESSAGE)*maxID);

    CMessagesMap::iterator msgsIter;
    IMessage *pMsg;
    for (msgsIter = msgs.begin();
         msgsIter != msgs.end();
         msgsIter++)
    {
        pMsg = msgsIter->second;

        m_pMsgs[pMsg->GetId()] = pMsg;
    }
}


void CXMLStateMachine::InitStates()
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    if (m_pStates != NULL)
        delete m_pStates;

    const unsigned maxID = m_nMaxStateID+1;
    m_pStates   = new PSTATE[maxID];
    memset(m_pStates, 0, sizeof(PSTATE)*maxID);

    CStatesMap::iterator statesIter;
    IState *pState;
    for (statesIter = states.begin();
         statesIter != states.end();
         statesIter++)
    {
        pState = statesIter->second;

        m_pStates[pState->GetId()] = pState;
    }
}


void CXMLStateMachine::InitStateTables()
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    if (m_pStateTables != NULL)
        delete m_pStateTables;

    const unsigned maxID = m_nMaxStateTableID+1;
    m_pStateTables   = new PSTATELOOKUPTABLE[maxID];
    memset(m_pStateTables, 0, sizeof(PSTATELOOKUPTABLE)*maxID);

    CStateLookupTablesMap::iterator stateTablesIter;
    IStateLookupTable *pStateTable;
    for (stateTablesIter = stateTables.begin();
         stateTablesIter != stateTables.end();
         stateTablesIter++)
    {
        pStateTable = stateTablesIter->second;

        m_pStateTables[pStateTable->GetId()] = pStateTable;
    }
}


void CXMLStateMachine::InitLookupTables()
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    if (m_pLookupTables != NULL)
        delete m_pLookupTables;

    const unsigned maxID = m_nMaxLookupTableID+1;
    m_pLookupTables   = new PLOOKUPTABLE[maxID];
    memset(m_pLookupTables, 0, sizeof(PLOOKUPTABLE)*maxID);

    CLookupTablesMap::iterator iter;
    ILookupTable *pLookupTable;
    for (iter = lookupTables.begin();
         iter != lookupTables.end();
         iter++)
    {
        pLookupTable = iter->second;

        m_pLookupTables[pLookupTable->GetId()] = pLookupTable;
    }
}


void CXMLStateMachine::InitVariables()
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    if (m_pVariables != NULL)
        delete m_pVariables;

    const unsigned maxID = m_nMaxVariableID+1;
    m_pVariables   = new PVARIABLE[maxID];
    memset(m_pVariables, 0, sizeof(PVARIABLE)*maxID);

    CVariablesMap::iterator variablesIter;
    IVariable *pVariable;
    for (variablesIter = variables.begin();
         variablesIter != variables.end();
         variablesIter++)
    {
        pVariable = variablesIter->second;

        m_pVariables[pVariable->GetId()] = pVariable;
    }
}


void CXMLStateMachine::InitTimers()
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    if (m_pTimers != NULL)
        delete m_pTimers;

    const unsigned maxID = m_nMaxTimerID+1;
    m_pTimers   = new PTIMER[maxID];
    memset(m_pTimers, 0, sizeof(PTIMER)*maxID);

    CTimersMap::iterator timersIter;
    ITimer *pTimer;
    for (timersIter = timers.begin();
         timersIter != timers.end();
         timersIter++)
    {
        pTimer = timersIter->second;

        m_pTimers[pTimer->GetId()] = pTimer;
    }
}


void CXMLStateMachine::InitLibraries()
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    if (m_pLibraries != NULL)
        delete m_pLibraries;

    const unsigned maxID = m_nMaxLibraryID+1;
    m_pLibraries   = new PLIBRARY[maxID];
    memset(m_pLibraries, 0, sizeof(PLIBRARY)*maxID);

    CLibrarysMap::iterator libsIter;
    ILibrary *pLibrary;
    for (libsIter = libraries.begin();
         libsIter != libraries.end();
         libsIter++)
    {
        pLibrary = libsIter->second;

        m_pLibraries[pLibrary->GetId()] = pLibrary;
    }
}


void CXMLStateMachine::InitConstants()
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    if (m_pConstants != NULL)
        delete m_pConstants;

    const unsigned maxID = m_nMaxConstantID+1;
    m_pConstants   = new PCONSTANT[maxID];
    memset(m_pConstants, 0, sizeof(PCONSTANT)*maxID);

    CConstantsMap::iterator constantsIter;
    IConstant *pConstant;
    for (constantsIter = constants.begin();
         constantsIter != constants.end();
         constantsIter++)
    {
        pConstant = constantsIter->second;

        m_pConstants[pConstant->GetId()] = pConstant;
    }
}


#define REGISTER_LOCAL_CREATEABLE(FACTORY, NAME, CLASS)\
    REGISTER_CREATEABLE_NAME(FACTORY, NAME, CLASS);

void CXMLStateMachine::RegisterAll()
{
    ITRACE_PRODUCTION(_T("Registering internal actions:"));
    REGISTER_LOCAL_CREATEABLE(actionFactory, START_TIMER, TimerOperations);
    REGISTER_LOCAL_CREATEABLE(actionFactory, STOP_TIMER, TimerOperations);
    REGISTER_LOCAL_CREATEABLE(actionFactory, WAIT_TIMER, TimerOperations);
    REGISTER_LOCAL_CREATEABLE(actionFactory, IS_TIMER_TIMING, TimerOperations);
    REGISTER_LOCAL_CREATEABLE(actionFactory, INCREMENT_VARIABLE, VariableOperations);
    REGISTER_LOCAL_CREATEABLE(actionFactory, DECREMENT_VARIABLE, VariableOperations);
    REGISTER_LOCAL_CREATEABLE(actionFactory, ASSIGN_VARIABLE, VariableOperations);
    REGISTER_LOCAL_CREATEABLE(actionFactory, RESEND_MSG, ResendMsg);
    REGISTER_LOCAL_CREATEABLE(actionFactory, SEND_MSG, SendMsg);
    REGISTER_LOCAL_CREATEABLE(actionFactory, EVALUATE_EXPRESSION, EvaluateExpression);
    REGISTER_LOCAL_CREATEABLE(actionFactory, SET_RETURN_CODE, SetReturnCode);
    REGISTER_LOCAL_CREATEABLE(actionFactory, PUSH_STATE, PushState);
    REGISTER_LOCAL_CREATEABLE(actionFactory, POP_STATE, PopState);
    REGISTER_LOCAL_CREATEABLE(actionFactory, GOTO_STATE, GotoState);
    REGISTER_LOCAL_CREATEABLE(actionFactory, CLEAN_STATE, CleanState);

    CLibrarysMap::iterator libsIter;
    ILibrary *pLibrary;
    CActionFactory::PREGISTRATION_FXN pRegFxn;
    for (libsIter = libraries.begin();
         libsIter != libraries.end();
         libsIter++)
    {
        pLibrary = libsIter->second;

        ITRACE_PRODUCTION(_T("Registering actions in library:  ") 
                        + IString(pLibrary->GetName()));

        // Find the registration method in the library and call it
        pRegFxn = (CActionFactory::PREGISTRATION_FXN)pLibrary
            ->FindFunction(REGISTRATION_FXN_NAME);
        if (pRegFxn != NULL)
        {
            const _TCHAR **pNames;
            (*pRegFxn)(&pNames, GetTraceHandle());

            const _TCHAR *pName;
            for (int i=0; 
                 pNames[i] != 0;
                 i++)
            {
                pName = pNames[i];
                m_mapActionCreationMap
                    .insert(std::make_pair(pName, pLibrary));
            }
        }
        else
            ITRACE_ERROR(_T("Registration function:  ") 
                       + IString(REGISTRATION_FXN_NAME) 
                       + _T(" not found in library:  ") + pLibrary->GetName());
    }
}


void CXMLStateMachine::UnregisterAll()
{
    IMODTRACE_DEVELOP();

    CLibrarysMap::iterator libsIter;
    ILibrary *pLibrary;
    CActionFactory::PUNREGISTRATION_FXN pUnregFxn;
    for (libsIter = libraries.begin();
         libsIter != libraries.end();
         libsIter++)
    {
        pLibrary = libsIter->second;

        // Find the registration method in the library and call it
        pUnregFxn = (CActionFactory::PUNREGISTRATION_FXN)pLibrary->FindFunction(UNREGISTRATION_FXN_NAME);
        if (pUnregFxn != NULL)
            (*pUnregFxn)();

        else
            ITRACE_ERROR(_T("Registration function:  ") 
                       + IString(UNREGISTRATION_FXN_NAME) 
                       + _T(" not found in library:  ") + pLibrary->GetName());
    }
}


const utils::IErrorInfo &CXMLStateMachine::GetErrorInfo(HRESULT hr)
{
    CSMErrorInfo *pErrorInfo = new CSMErrorInfo(hr, _T(""));

    SetErrorInfo(pErrorInfo);

    return *pErrorInfo; 
}


void CXMLStateMachine::SetErrorInfo(utils::IErrorInfo *pErrorInfo)
{
    delete m_pErrorInfo;

    m_pErrorInfo = pErrorInfo;
}


int CXMLStateMachine::GetCurrentState() const { return m_nCurrentState; }
HRESULT CXMLStateMachine::GetCurrentState(IState **ppState) 
{ 
    return GetState(ppState, m_nCurrentState);
}


HRESULT CXMLStateMachine::GetAction(IAction **ppAction, const _TCHAR * szActionName)
{
    int actionID = GetActionID(szActionName);

    return GetAction(ppAction, actionID);
}


HRESULT CXMLStateMachine::GetAction(IAction **ppAction, int _actionID)
{
    HRESULT retValue = E_FAIL;

    *ppAction = NULL;

    if (!((_actionID >= 0) && (_actionID <= (int)m_nMaxActionID)))
        return STATEM_INVALID_ACTION_ID;

    switch (_actionID)
    {
        case NULL_ACTION :
            *ppAction = m_NullAction;
            retValue = S_OK;
            break;
        default :
            IAction *pProxyAction = m_pActions[_actionID];
            if (pProxyAction == NULL)
                return STATEM_UNDEFINED_ACTION_ID;

            const _TCHAR *szActionName = pProxyAction->GetName();

            if (_tcslen(szActionName) != 0)
            {
                CLibrarysMap::iterator iter = m_mapActionCreationMap.find(szActionName);
                if (iter == m_mapActionCreationMap.end())
                {
                    retValue = actionFactory.NewInstance(ppAction, szActionName);
                }
                else
                {
                    PLIBRARY pLibrary = iter->second;

                    CActionFactory::PCREATION_FXN pCreationFxn 
                        = (CActionFactory::PCREATION_FXN)pLibrary
                                ->GetCreateFunction(CREATION_FXN_NAME);
                    if (pCreationFxn != NULL)
                        retValue = (*pCreationFxn)(szActionName, ppAction);
                }

                if (retValue != S_OK)
                {
                    ITRACE_ERROR(_T("Unable to locate Action Class:  ") 
                               + IString(szActionName));
                    return retValue;
                }

                CActionBase &actionbase = *(dynamic_cast<CActionBase *>(*ppAction));
                actionbase.SetTraceObject(GetTraceHandle());
                actionbase.SetId(pProxyAction->GetId());
                actionbase.SetStateMachine(this);
                actionbase.SetName(szActionName);
            }
            break;
    }
        
    return retValue;
}


void CXMLStateMachine::CopyParameters(IAction *pAction, const CParameterList &parmlist) 
{ 
    CActionBase *pActionBase = dynamic_cast<CActionBase *>(pAction);
    pActionBase->SetParameters(parmlist); 
}


HRESULT CXMLStateMachine::GetMessage(IMessage **ppMessage, const _TCHAR * szMsgName, const void *pData, size_t nDataLength)
{
    int nMsgID = GetMsgID(szMsgName);

    return GetMessage(ppMessage, nMsgID, pData, nDataLength);
}


HRESULT CXMLStateMachine::GetMessage(IMessage **ppMessage, int _msgID, const void *pData, size_t nDataLength)
{
    HRESULT retValue = S_OK;

    *ppMessage = NULL;

    if (!((_msgID >= 0) && (_msgID <= (int)m_nMaxMsgID)))
        return STATEM_INVALID_MSG_ID;

    IMessage *pProxyMessage = m_pMsgs[_msgID];
    if (pProxyMessage == NULL)
        return STATEM_UNDEFINED_MSG_ID;

    CMessageObject &msgbase = *new CMessageObject;
    msgbase.SetTraceObject(GetTraceHandle());
    msgbase.SetId(pProxyMessage->GetId());
    msgbase.SetComment(pProxyMessage->GetComment());
    msgbase.SetName(pProxyMessage->GetName());
    msgbase.SetTimeout(pProxyMessage->GetTimeout());
    msgbase.SetData(pData, nDataLength);

    *ppMessage = &msgbase;
        
    return retValue;
}


HRESULT CXMLStateMachine::GetState(IState **ppState, const _TCHAR * szStateName)
{
    int nStateID = GetStateID(szStateName);

    return GetState(ppState, nStateID);
}


HRESULT CXMLStateMachine::GetState(IState **ppState, int _stateID)
{
    HRESULT retValue = S_OK;

    *ppState = NULL;

    if (!((_stateID >= 0) && (_stateID <= (int)m_nMaxStateID)))
        return STATEM_INVALID_STATE_ID;

    *ppState = m_pStates[_stateID];
    if (*ppState == NULL)
        return STATEM_UNDEFINED_STATE_ID;
        
    return retValue;
}


IState * CXMLStateMachine::GetState(const _TCHAR *pszStateName)
{
    CStatesMap::iterator iter = states.find(pszStateName);
    if (iter == states.end())
        return NULL;
        
    return iter->second;
}


HRESULT CXMLStateMachine::GetVariable(IVariable **ppVariable, const _TCHAR * szVariableName)
{
    long nVariableID = -1;
    HRESULT hr = GetVariableID(szVariableName, nVariableID);
    if (hr != S_OK)
        return hr;

    return GetVariable(ppVariable, nVariableID);
}


HRESULT CXMLStateMachine::GetVariable(IVariable **ppVariable, int _variableID)
{
    *ppVariable = NULL;

    if (!((_variableID >= 0) && (_variableID <= (int)m_nMaxVariableID)))
        return STATEM_INVALID_VARIABLE_ID;

    *ppVariable = m_pVariables[_variableID];
    if (ppVariable == NULL)
        return STATEM_UNDEFINED_VARIABLE_ID;
        
    return S_OK;
}


HRESULT CXMLStateMachine::GetConstant(IConstant **ppConstant, const _TCHAR * szConstantName)
{
    int nConstantID = GetConstantID(szConstantName);

    return GetConstant(ppConstant, nConstantID);
}


HRESULT CXMLStateMachine::GetConstant(IConstant **ppConstant, int nConstantID)
{
    *ppConstant = NULL;

    if (!((nConstantID >= 0) && (nConstantID <= (int)m_nMaxConstantID)))
        return STATEM_INVALID_CONSTANT_ID;

    *ppConstant = m_pConstants[nConstantID];
    if (ppConstant == NULL)
        return STATEM_UNDEFINED_CONSTANT_ID;
        
    return S_OK;
}


HRESULT CXMLStateMachine::CreateExpression(IExpression **ppExpression, const _TCHAR *szExpression)
{
    HRESULT hr = S_OK;

    *ppExpression = NULL;

    try
    {
        *ppExpression = m_ExprFactory.NewInstance(szExpression);
    }
    catch (const exception &e)
    {
        ITRACE_EXCEPTION(e);
        hr = E_FAIL;
    }
    catch (...)
    {
        ITRACE_DOT_ERROR();
        hr = E_FAIL;
    }
    
    return hr;
}


IExpression * CXMLStateMachine::GetTrueExpression()
{
    return m_ExprFactory.TrueInstance();
}


IExpression * CXMLStateMachine::GetFalseExpression()
{
    return m_ExprFactory.FalseInstance();
}


IExpression * CXMLStateMachine::GetExpression(const _TCHAR *szExpression)
{
    IExpression *pExpr;

    HRESULT hr = GetExpression(&pExpr, szExpression);
    if (hr != S_OK)
        throw IException((const _TCHAR *)(_T("Invalid Expression:  ") + IString(szExpression)), hr);

    return pExpr;
}


HRESULT CXMLStateMachine::GetExpression(IExpression **ppExpression, const _TCHAR * szExpression)
{
    HRESULT hr = S_OK;

    //_ASSERTE(m_pVariables!=NULL);
    //_ASSERTE(m_pConstants!=NULL);
    //if ( (m_pVariables==NULL) || (m_pConstants==NULL) )
    //    return STATEM_INTERNAL_ERROR;

    CExpressionsMap::iterator iter = expressions.find(szExpression);
    if (iter != expressions.end())
        *ppExpression = iter->second;
    else
    {
        hr = CreateExpression(ppExpression, szExpression);
        expressions.insert(make_pair(szExpression, *ppExpression));
    }

    return hr;
}


HRESULT CXMLStateMachine::GetIfExpression(IExpression **ppExpression, const _TCHAR * szExpression, IExpression *pIfExpr)
{
    HRESULT hr = S_OK;

    *ppExpression = NULL;

    try
    {
        *ppExpression = m_ExprFactory.IfInstance(szExpression, pIfExpr);
    }
    catch (const exception &e)
    {
        ITRACE_EXCEPTION(e);
        hr = E_FAIL;
    }
    catch (...)
    {
        ITRACE_DOT_ERROR();
        hr = E_FAIL;
    }

    return hr;
}


HRESULT CXMLStateMachine::GetStateLookupTable(IStateLookupTable **ppStateTable, const _TCHAR * szStateTableName)
{
    long nStateTableID = GetStateLookupTableID(szStateTableName);

    return GetStateLookupTable(ppStateTable, nStateTableID);
}


HRESULT CXMLStateMachine::GetStateLookupTable(IStateLookupTable **ppStateTable, int _stateTableID)
{
    *ppStateTable = NULL;

    if (!((_stateTableID >= 0) && (_stateTableID <= (int)m_nMaxStateTableID)))
        return STATEM_UNDEFINED_STATETABLE_ID;

    *ppStateTable = m_pStateTables[_stateTableID];
    if (ppStateTable == NULL)
        return STATEM_UNDEFINED_STATETABLE_ID;
        
    return S_OK;
}


HRESULT CXMLStateMachine::GetLookupTable(ILookupTable **ppLookupTable, const _TCHAR * szTableName)
{
    long nLookupTableID = GetLookupTableID(szTableName);

    return GetLookupTable(ppLookupTable, nLookupTableID);
}


HRESULT CXMLStateMachine::GetLookupTable(ILookupTable **ppLookupTable, int _lookupTableID)
{
    *ppLookupTable = NULL;

    if (!((_lookupTableID >= 0) && (_lookupTableID <= (int)m_nMaxLookupTableID)))
        return STATEM_UNDEFINED_STATETABLE_ID;

    *ppLookupTable = m_pLookupTables[_lookupTableID];
    if (ppLookupTable == NULL)
        return STATEM_UNDEFINED_LOOKUPTABLE_ID;
        
    return S_OK;
}


HRESULT CXMLStateMachine::GetTimer(STATE_MACHINE_NAMESPACE::ITimer **ppTimer, const _TCHAR * szTimerName)
{
    int nTimerID = GetTimerID(szTimerName);

    return GetTimer(ppTimer, nTimerID);
}


HRESULT CXMLStateMachine::GetTimer(STATE_MACHINE_NAMESPACE::ITimer **ppTimer, int nTimerID)
{
    *ppTimer = NULL;

    if (!((nTimerID >= 0) && (nTimerID <= (int)m_nMaxTimerID)))
        return STATEM_INVALID_TIMER_ID;

    *ppTimer = m_pTimers[nTimerID];
    if (ppTimer == NULL)
        return STATEM_UNDEFINED_TIMER_ID;
        
    return S_OK;
}


HRESULT CXMLStateMachine::OnTimerTimeout(STATE_MACHINE_NAMESPACE::ITimer *pTimer)
{
    IMODTRACE_DEVELOP();

    StringType sName(pTimer->GetName());
    sName += MSG_TIMEOUT_SUFFIX;
 
    IMessage *pMsg;

    HRESULT hr = GetMessage(&pMsg, sName.c_str());
    if (hr == S_OK)
    {
        PostMsg(*pMsg);
    }

    return hr;
}


 bool CXMLStateMachine::IsValidAction(const _TCHAR *pcszName)
 {
     switch(GetActionID(pcszName))
     {
        case NULL_ACTION :
        case -1 :
            return false;
        default :
            return true;
     }
 }


 bool CXMLStateMachine::IsValidMessage(const _TCHAR *pcszName)
 {
     switch(GetMsgID(pcszName))
     {
        case -1 :
        case ANY_MSG :
            return false;
        default :
            return true;
     }
 }


bool CXMLStateMachine::IsValidState(const _TCHAR *pcszName)
{
     switch(int id = GetStateID(pcszName))
     {
        case -1 :
        case REMAIN_THE_SAME_STATE :
            return false;
        default :
            if (id == GetDefaultStateID())
                return false;
            else
                return true;
     }
}


bool CXMLStateMachine::IsValidVariable(const _TCHAR *pcszName)
{
    long nVariableID;
    HRESULT hr = GetVariableID(pcszName, nVariableID);
    return (hr == S_OK);
}


bool CXMLStateMachine::IsValidConstant(const _TCHAR *pcszName)
{
    return (GetConstantID(pcszName) != -1);
}


bool CXMLStateMachine::IsValidTimer(const _TCHAR *pcszName)
{
    return (GetTimerID(pcszName) != -1);
}


HRESULT CXMLStateMachine::ReleaseAction(IAction *pAction)
{
    HRESULT hr = S_OK;

    if (pAction == NULL)
        return hr;

    const _TCHAR *szActionName = pAction->GetName();

    if (pAction == m_NullAction)
    {
//      if (loggable().IsDebug()) ITRACE_DEVELOP("Releasing Internal Action:  " + IString(pAction->GetName()));
    }
    else 
    {
        CLibrarysMap::iterator iter = m_mapActionCreationMap.find(szActionName);
        if (iter == m_mapActionCreationMap.end())
        {
            hr = actionFactory.ReleaseInstance(pAction, szActionName);
        }
        else
        {
            PLIBRARY pLibrary = iter->second;

            CActionFactory::PRELEASE_OBJECT_FXN pReleaseFxn 
                = (CActionFactory::PRELEASE_OBJECT_FXN)pLibrary
                        ->GetReleaseFunction(RELEASE_OBJECT_FXN_NAME);
            if (pReleaseFxn != NULL)
                hr = (*pReleaseFxn)(szActionName, pAction);
        }
    }

    if (hr != S_OK)
        ITRACE_ERROR(_T("Unable to locate Action Class:  ") + IString(szActionName));

    return hr;
}


const TCHAR *CXMLStateMachine::GetActionName(int _actionID) const
{
    if (_actionID == INVALID_ACTION)
        return _T("<Invalid Action>");

    _ASSERTE((_actionID >= 0) && (_actionID <= (int)m_nMaxActionID));
    PACTION pAction = m_pActions[_actionID];
    return pAction->GetName();
}


const TCHAR * CXMLStateMachine::GetCurrentStateName() const
{
    return m_pCurrentState->GetName();
}


const TCHAR * CXMLStateMachine::GetStateName(int _state) const
{
    switch (_state)
    {
        case INVALID_STATE :
            m_sStateName = _T("<Invalid State>");
            break;
        case REMAIN_THE_SAME_STATE :
            m_sStateName = _T("<RemainTheSameState> : ");
            m_sStateName += GetCurrentStateName();
            break;
        case ANY_STATE :
            m_sStateName =  _T("<AnyState> : ");
            m_sStateName += GetCurrentStateName();
            break;
        default :
            _ASSERTE((_state > 0) && (_state <= (int)m_nMaxStateID));
            PSTATE pState = m_pStates[_state];
            m_sStateName = pState->GetName();
            break;
    }

    return m_sStateName.c_str();
}


const TCHAR *CXMLStateMachine::GetMsgName(int _msgID) const
{
    _ASSERTE((_msgID > 0) && (_msgID <= (int)m_nMaxMsgID));
    PMESSAGE pMsg = m_pMsgs[_msgID];
    return pMsg->GetName();
}


void CXMLStateMachine::SetCurrentState(int _newState)
{
    _ASSERTE(m_pStates!=NULL);

    if (_newState != m_RemainTheSameStateID)
    {
        _ASSERTE(_newState>0);
        _ASSERTE(_newState<=(int)m_nMaxStateObjectID);
        ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  New State:  ") 
                     + IString(GetStateName(_newState)));
        m_nCurrentState = _newState;
        m_pCurrentState = m_pStates[_newState];
    }
}


void CXMLStateMachine::SetInitialState()
{
    SetCurrentState(m_nInitialState);
}


/**
    * Save the current message.
    */
void CXMLStateMachine::RecordMessage(IMessage & /* _msg */)
{
}


HRESULT CXMLStateMachine::GetRuleSet (CRulesList &rulesList, int _nStateID, int _nMsgID)
{
    IState *pState = NULL;

    switch (_nStateID)
    {
        case ANY_STATE :
//          ITRACE_DEVELOP("Get rules for state:  *    <------------  Need to implement this");
//          return STATEM_NO_RULES_DEFINED;
//          break;
        default :
            _ASSERTE(_nStateID>=0);
            _ASSERTE(_nStateID<=(int)m_nMaxStateObjectID);
            pState = m_pStates[_nStateID];
            if (pState==NULL)
                return STATEM_UNDEFINED_STATE_ID;

            CRulesList *pSource = pState->GetMsgList(_nMsgID);
            if ( pSource == NULL )
                return STATEM_NO_RULES_DEFINED;

            rulesList.assign(pSource->begin(), pSource->end());
            break;
    }

    return S_OK;
}


HRESULT CXMLStateMachine::GetGlobalRuleSet (CRulesList &rulesList, int _nMsgID)
{
    if(m_pGlobalState==NULL)
        return STATEM_NO_RULES_DEFINED;

    CRulesList *pSource = m_pGlobalState->GetMsgList(_nMsgID);
    if ( pSource == NULL )
        return STATEM_NO_RULES_DEFINED;

    rulesList.assign(pSource->begin(), pSource->end());

    return S_OK;
}


HRESULT CXMLStateMachine::GetStateExitRuleSet (CRulesList &rulesList, int _nStateID)
{
    IState *pState = NULL;

    _ASSERTE(_nStateID>0);
    _ASSERTE(_nStateID<=(int)m_nMaxStateObjectID);
    pState = m_pStates[_nStateID];
    _ASSERTE(pState!=NULL);

    CRulesList *pSource = pState->GetExitList();
    if ( pSource == NULL )
        return STATEM_NO_RULES_DEFINED;

    rulesList.assign(pSource->begin(), pSource->end());

    return S_OK;
}


HRESULT CXMLStateMachine::GetStateEntryRuleSet (CRulesList &rulesList, int _nStateID)
{
    IState *pState = NULL;

    _ASSERTE(_nStateID>0);
    _ASSERTE(_nStateID<=(int)m_nMaxStateObjectID);
    pState = m_pStates[_nStateID];
    _ASSERTE(pState!=NULL);

    CRulesList *pSource = pState->GetEntryList();
    if ( pSource == NULL )
        return STATEM_NO_RULES_DEFINED;

    rulesList.assign(pSource->begin(), pSource->end());

    return S_OK;
}


HRESULT CXMLStateMachine::Add(CStateObjectProxy *pStateProxy)
{
    _ASSERTE(pStateProxy!=NULL);

    if (loggable().IsDebug()) ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Adding CStateObjectProxy.  key=") 
        + IString(pStateProxy->StateName.Value().c_str()));

    IState *pState;
    int nStateID = GetStateID(pStateProxy->StateName.Value().c_str());
    if (nStateID == ANY_STATE)
        pState = GetAnyState();
    
    // The state we are looking for must be defined in the previous sections
    else
    {
        m_nMaxStateObjectID = (unsigned)max((int)m_nMaxStateObjectID, nStateID);

        const _TCHAR *pszKey = pStateProxy->StateName.Value().c_str();

        pState = GetState(pszKey);
    }
    if (pState == NULL)
        return STATEM_UNDEFINED_STATE_ID;

    IRule *pRule;
    CRuleImplBase *pRuleProxy;
    CStateImpl &state = *dynamic_cast<CStateImpl *>(pState);
    CStateObjectProxy &stateProxy = *pStateProxy;
    _ASSERTE( _CrtCheckMemory() );

    CRuleQ &proxyEntryList = *(stateProxy.GetEntryList());
    for (CRuleQIterator iter = proxyEntryList.begin();
         iter != proxyEntryList.end();
         iter++)
    {
        pRuleProxy = *iter;
        if (loggable().IsDebug()) ITRACE_DEVELOP(_T("Adding to EntryList:  ") + IString(pRuleProxy->ToString().c_str()));
        pRule = CreateRule(pRuleProxy);
        state.AddEntryRule(pRule);
    }

    CRuleQ &proxyExitList = *(stateProxy.GetExitList());
    for (CRuleQIterator iter = proxyExitList.begin();
         iter != proxyExitList.end();
         iter++)
    {
        pRuleProxy = *iter;
        if (loggable().IsDebug()) ITRACE_DEVELOP(_T("Adding to ExitList:  ") + IString(pRuleProxy->ToString().c_str()));
        pRule = CreateRule(pRuleProxy);
        state.AddExitRule(pRule);
    }

    CRuleQMap &proxyMsgMap = *stateProxy.GetMsgMap();
    int nMsgID;
    for (CRuleQMapIterator iter = proxyMsgMap.begin();
         iter != proxyMsgMap.end();
         iter++)
    {
        nMsgID = iter->first;
        CRuleQ &proxyMsgList = *(iter->second);

        int i=0;
        for (CRuleQIterator iterInner = proxyMsgList.begin();
            iterInner != proxyMsgList.end();
            iterInner++, i++)
        {
            pRuleProxy = *iterInner;
            if (loggable().IsDebug()) ITRACE_DEVELOP(_T("Adding ") + IString(i) + _T(" to MsgList:  ") + IString(pRuleProxy->ToString().c_str()));
            pRule = CreateRule(pRuleProxy);
            state.AddMsgRule(nMsgID, pRule, pRuleProxy->GetRuleType());
        }
    }
    
    // We're done with the proxy so delete it
    delete pStateProxy;

    return S_OK;
}


HRESULT CXMLStateMachine::Add(IState *pState)
{
    _ASSERTE(pState!=NULL);
    
    if (loggable().IsDebug()) 
        ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Adding IState:  ")
                      + IString(pState->GetName()) 
                      + (pState->IsInitial() ? _T("<initial/>") : _T("")) 
                      + (pState->IsGlobal() ? _T("<global/>") : _T(""))
                      + _T("(") + IString(pState->GetId()) + _T(")"));

    CStatesMap::iterator iter = states.find(pState->GetName());
    if (iter != states.end())
    {
        ITRACE_WARNING(_T("Duplicate state definition ignored:  ") + IString(pState->GetName()));

        delete pState;
        
        return S_OK;
    }

    states.insert(std::make_pair(pState->GetName(), pState));

    int id = pState->GetId();
    m_nMaxStateID = (unsigned)max((int)m_nMaxStateID, id);

    if (!pState->IsInitial())
    {
    }
    else if (m_nCurrentState == INVALID_STATE)
    {
        m_nCurrentState = pState->GetId();
        m_nInitialState = pState->GetId();
    }
    else
    {
        ITRACE_ERROR(_T("Multiple Initial states detected.  State:  ") 
            + IString(pState->GetName()));
        return STATEM_DUPLICATE_INITIAL_STATE;
    }

    if (!pState->IsGlobal())
    {
    }
    else if (m_pGlobalState == NULL)
    {
        m_pGlobalState = pState;
        m_nGlobalState = pState->GetId();
    }
    else
    {
        return STATEM_DUPLICATE_GLOBAL_STATE;
    }

    return S_OK;
}


HRESULT CXMLStateMachine::Add(IMessage *pMsg)
{
    _ASSERTE(pMsg!=NULL);

    const _TCHAR *pName = pMsg->GetName();
    if (loggable().IsDebug()) 
        ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Adding IMessage:  \"") 
                      + IString(pName)
                      + _T("\"(") + IString(pMsg->GetId()) + _T(")"));

    CMessagesMap::iterator iter = msgs.find(pMsg->GetName());
    if (iter != msgs.end())
    {
        ITRACE_WARNING(_T("Duplicate message definition ignored:  ") + IString(pMsg->GetName()));

        delete pMsg;
        
        return S_OK;
    }

    msgs.insert(std::make_pair(pName, pMsg));

    int id = pMsg->GetId();
    m_nMaxMsgID = (unsigned)max((int)m_nMaxMsgID, id);

    return S_OK;
}


HRESULT CXMLStateMachine::Add(IAction *pAction)
{
    _ASSERTE(pAction!=NULL);
 
    if (loggable().IsDebug()) 
        ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Adding IAction:  ") 
                      + IString(pAction->GetName()) 
                      + _T("(") + IString(pAction->GetId()) + _T(")"));

    CActionsMap::iterator iter = actions.find(pAction->GetName());
    if (iter != actions.end())
    {
        ITRACE_WARNING(_T("Duplicate action definition ignored:  ") + IString(pAction->GetName()));

        delete pAction;
        
        return S_OK;
    }

    actions.insert(std::make_pair(pAction->GetName(), pAction));

    int id = pAction->GetId();
    m_nMaxActionID = (unsigned)max((int)m_nMaxActionID, id);

    return S_OK;
}


HRESULT CXMLStateMachine::Add(IVariable *pVar)
{
    _ASSERTE(pVar!=NULL);
 
    if (loggable().IsDebug()) 
        ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Adding IVariable:  ")
                      + IString(pVar->GetName()) 
                      + _T("(") + IString(pVar->GetId()) + _T(")"));

    CVariablesMap::iterator iter = variables.find(pVar->GetName());
    if (iter != variables.end())
    {
        ITRACE_WARNING(_T("Duplicate variable definition detected.  Replacing:  ") + IString(pVar->GetName()));

        IVariable *pOriginalVariable = iter->second;
        
        constants.erase(pOriginalVariable->GetName());       

        delete pOriginalVariable;
    }

    variables.insert(std::make_pair(pVar->GetName(), pVar));

    int id = pVar->GetId();
    m_nMaxVariableID = (unsigned)max((int)m_nMaxVariableID, id);

    return S_OK;
}


HRESULT CXMLStateMachine::Add(CStateLookupTableImpl *pSlt)
{
    _ASSERTE(pSlt!=NULL);
 
    if (loggable().IsDebug()) 
        ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Adding IStateLookupTable:  ")
                      + IString(pSlt->GetName()) 
                      + _T("(") + IString(pSlt->GetId()) + _T(")"));

    CStateLookupTablesMap::iterator iter = stateTables.find(pSlt->GetName());
    if (iter != stateTables.end())
    {
        ITRACE_WARNING(_T("Duplicate state table definition detected.  Merging:  ") + IString(pSlt->GetName()));

        CStateLookupTableImpl *pOriginalLt = dynamic_cast<CStateLookupTableImpl *>(iter->second);

        pOriginalLt->Merge(pSlt);

        delete pSlt;
        
        return S_OK;
    }

    stateTables.insert(std::make_pair(pSlt->GetName(), pSlt));

    int id = pSlt->GetId();
    m_nMaxStateTableID = (unsigned)max((int)m_nMaxStateTableID, id);

    return S_OK;
}


HRESULT CXMLStateMachine::Add(CLookupTableImpl *pLt)
{
    _ASSERTE(pLt!=NULL);
 
    if (loggable().IsDebug()) 
        ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Adding IStateLookupTable:  ")
                      + IString(pLt->GetName()) 
                      + _T("(") + IString(pLt->GetId()) + _T(")"));

    CLookupTablesMap::iterator iter = lookupTables.find(pLt->GetName());
    if (iter != lookupTables.end())
    {
        ITRACE_WARNING(_T("Duplicate state table definition detected.  Merging:  ") + IString(pLt->GetName()));

        CLookupTableImpl *pOriginalLt = dynamic_cast<CLookupTableImpl *>(iter->second);

        pOriginalLt->Merge(pLt);

        delete pLt;
        
        return S_OK;
    }

    lookupTables.insert(std::make_pair(pLt->GetName(), pLt));

    int id = pLt->GetId();
    m_nMaxLookupTableID = (unsigned)max((int)m_nMaxLookupTableID, id);

    return S_OK;
}


HRESULT CXMLStateMachine::Add(STATE_MACHINE_NAMESPACE::ITimer *pTimer)
{
    HRESULT hr = S_OK;

    _ASSERTE(pTimer!=NULL);
 
    if (loggable().IsDebug()) 
        ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Adding ITimer:  ") 
                      + IString(pTimer->GetName()) 
                      + _T("(") + IString(pTimer->GetId()) + _T(")"));

    CTimersMap::iterator iter = timers.find(pTimer->GetName());
    if (iter != timers.end())
    {
        ITRACE_WARNING(_T("Duplicate timer definition ignored:  ") + IString(pTimer->GetName()));

        delete pTimer;
        
        return S_OK;
    }

    timers.insert(std::make_pair(pTimer->GetName(), pTimer));

    int id = pTimer->GetId();
    m_nMaxTimerID = (unsigned)max((int)m_nMaxTimerID, id);

    StringType sName(pTimer->GetName());
    sName += MSG_TIMEOUT_SUFFIX;

    IMessage *pTimeoutMsg;
    if (S_OK == (hr = ConstructMessage(&pTimeoutMsg, sName.c_str())))
        Add(pTimeoutMsg);

    return S_OK;
}


HRESULT CXMLStateMachine::Add(ILibrary *pLibrary)
{
    _ASSERTE(pLibrary!=NULL);
 
    if (loggable().IsDebug()) 
        ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Adding ILibrary:  ")
                      + IString(pLibrary->GetName()) 
                      + _T("(") + IString(pLibrary->GetId()) + _T(")"));

    CLibrarysMap::iterator iter = libraries.find(pLibrary->GetName());
    if (iter != libraries.end())
    {
        ITRACE_WARNING(_T("Duplicate library definition ignored:  ") + IString(pLibrary->GetName()));

        delete pLibrary;
        
        return S_OK;
    }

    libraries.insert(std::make_pair(pLibrary->GetName(), pLibrary));

    int id = pLibrary->GetId();
    m_nMaxLibraryID = (unsigned)max((int)m_nMaxLibraryID, id);

    return S_OK;
}


HRESULT CXMLStateMachine::Add(IConstant *pConstant)
{
    _ASSERTE(pConstant!=NULL);
 
    if (loggable().IsDebug()) 
        ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Adding IConstant:  ")
                      + IString(pConstant->GetName()) 
                      + _T("(") + IString(pConstant->GetId()) + _T(")"));

    CConstantsMap::iterator iter = constants.find(pConstant->GetName());
    if (iter != constants.end())
    {
        ITRACE_WARNING(_T("Duplicate constant definition detected.  Replacing:  ") + IString(pConstant->GetName()));

        IConstant *pOriginalConstant = iter->second;
        
        constants.erase(pOriginalConstant->GetName());       

        delete pOriginalConstant;
    }

    constants.insert(std::make_pair(pConstant->GetName(), pConstant));

    int id = pConstant->GetId();
    m_nMaxConstantID = (unsigned)max((int)m_nMaxConstantID, id);

    return S_OK;
}


IRule * CXMLStateMachine::CreateRule(IRuleProxy *pProxy)
{
    CXMLRule *pRule = new CXMLRule();

    pRule->SetTraceObject(GetTraceHandle());
    pRule->SetExpression(pProxy->GetExpression()) ;
    pRule->SetMsgID(GetMsgID(pProxy->GetMsgName())) ;
	pRule->SetStateID(GetStateID(pProxy->GetStateName())) ;
	pRule->SetActionID(GetActionID(pProxy->GetActionName())) ;
    pRule->SetActionParms(pProxy->GetActionParms());
	pRule->SetSequenceNumber(pProxy->GetSequenceNumber()) ;
	pRule->SetEndStateID(GetStateID(pProxy->GetEndStateName())) ;
	pRule->SetFalseStateID(GetStateID(pProxy->GetFalseStateName())) ;
	pRule->SetFalseActionID(GetActionID(pProxy->GetFalseActionName())) ;
    pRule->SetFalseParms(pProxy->GetFalseParms());
	pRule->SetFailureStateID(GetStateID(pProxy->GetFailureStateName())) ;
	pRule->SetFailureActionID(GetActionID(pProxy->GetFailureActionName())) ;
    pRule->SetFailureParms(pProxy->GetFailureParms());

    CRuleQ &proxyRules = pProxy->GetRulesList();
    if (!proxyRules.empty())
    {
        CRuleImplBase *pRuleProxy;
        CRulesList &rules = pRule->GetRulesList();
        CRuleQ::iterator iter = proxyRules.begin();
        for (int i=0; iter!=proxyRules.end(); iter++, i++)
        {
            pRuleProxy = *iter;
            if (loggable().IsDebug()) ITRACE_DEVELOP(_T("Adding ") + IString(i) + _T(" to MsgList:  ") + IString(pRuleProxy->ToString().c_str()));
            rules.push_back(CreateRule(pRuleProxy));
        }
    }

    return pRule;
}


int CXMLStateMachine::GetActionID(const _TCHAR *pszActionName)
{
    int retValue = -1;

    if (_tcslen(pszActionName) == 0)
        retValue = NULL_ACTION;

    else
    {
        CActionsMap::iterator iter = actions.find(pszActionName);
        if (iter != actions.end())
            retValue = (*iter->second).GetId();
    }

    return retValue;
}


int CXMLStateMachine::GetMsgID(const _TCHAR *pszMsgName)
{
    static IException _UnableToDetermineMsgError;

    int retValue = -1;

    if (_tcslen(pszMsgName) == 0)
        retValue = ANY_MSG;

    else if (_tcscmp(pszMsgName, ANY_MSG_TEXT) == 0)
        retValue = ANY_MSG;

    else
    {
        CMessagesMap::key_type pszTest = (_TCHAR *)pszMsgName;
        CMessagesMap::iterator iter = msgs.find(pszTest);
        if (iter != msgs.end())
            retValue = (*iter->second).GetId();
    }

    if (retValue == -1)
    {
        _UnableToDetermineMsgError = IException((const _TCHAR *)(_T("Unable to determine msg id.  Msg name:  ") + IString(pszMsgName))
                       , STATEM_UNDEFINED_MSG_NAME);
        throw _UnableToDetermineMsgError;
    }

    return retValue;
}


IState *CXMLStateMachine::GetAnyState()
{
    IState *pAnystate = CXMLStateMachine::GetState(_T("*"));
    if (pAnystate == NULL)
    {
        pAnystate = newpooledobject CStateImpl(ANY_STATE);
        pAnystate->SetTraceObject(GetTraceHandle());
        CStateImpl &state = *dynamic_cast<CStateImpl *>(pAnystate);
        state.Name.Value(_T("*"));

        Add(pAnystate);

    }

    return pAnystate;
}


int CXMLStateMachine::GetStateID(const _TCHAR *pszStateName)
{
    int retValue = -1;

    if (_tcslen(pszStateName) == 0)
        retValue = REMAIN_THE_SAME_STATE;

    else if (pszStateName[0] == '*')
        retValue = GetDefaultStateID();

    else
    {
        CStatesMap::iterator iter = states.find(pszStateName);
        if (iter != states.end())
            retValue = (*iter->second).GetId();
    }

    return retValue;
}


static _TCHAR defaultTranslation[] = 
{
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 1,0,0,0, 0,0,0,0, 0,0,0,0,                                                                // '"'
    2,2,2,2, 2,2,2,2, 2,2,0,0, 0,0,0,0, // '0'-'9'
    1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, // '@'-'O'
    1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,1, // 'P'-'Z','_'
    0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, // 'a'-'o'
    1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0, // 'p'-'z'
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
};


HRESULT CXMLStateMachine::GetVariableID(const _TCHAR *pszVariableName, long &id)
{
    HRESULT hr = IsValidName(pszVariableName);
    if (S_OK == hr)
    {
        CVariablesMap::iterator iter = variables.find(pszVariableName);
        if (iter != variables.end())
            id = (*iter->second).GetId();
        else
        {
            id = -1;
            hr = STATEM_INVALID_VARIABLE_ID;
        }
    }

    return hr;
}


int CXMLStateMachine::GetStateLookupTableID(const _TCHAR *pszStateLookupTableName)
{
    int retValue = -1;

    if (_tcslen(pszStateLookupTableName) != 0)
    {
        CStateLookupTablesMap::iterator iter = stateTables.find(pszStateLookupTableName);
        if (iter != stateTables.end())
            retValue = (*iter->second).GetId();
    }

    return retValue;
}


int CXMLStateMachine::GetLookupTableID(const _TCHAR *pszLookupTableName)
{
    int retValue = -1;

    if (_tcslen(pszLookupTableName) != 0)
    {
        CLookupTablesMap::iterator iter = lookupTables.find(pszLookupTableName);
        if (iter != lookupTables.end())
            retValue = (*iter->second).GetId();
    }

    return retValue;
}


HRESULT CXMLStateMachine::IsValidName(const _TCHAR *pszName) const
{
    int stringSize = (int)_tcslen(pszName);

    bool bValid = true;
    if (stringSize != 0)
    {
        bValid = (defaultTranslation[pszName[0]] == 1);
        for (int i=1; bValid && (i<stringSize); i++)
            bValid = (defaultTranslation[pszName[i]] != 0);

        if (!bValid)
            return STATEM_INVALID_IDENTIFIER;
    }

    return S_OK;
}


int CXMLStateMachine::GetConstantID(const _TCHAR *pszConstantName)
{
    int retValue = -1;

    if (_tcslen(pszConstantName) != 0)
    {
        CConstantsMap::iterator iter = constants.find(pszConstantName);
        if (iter != constants.end())
            retValue = (*iter->second).GetId();
    }

    return retValue;
}


int CXMLStateMachine::GetTimerID(const _TCHAR *pszTimerName)
{
    int retValue = -1;

    if (_tcslen(pszTimerName) != 0)
    {
        CTimersMap::iterator iter = timers.find(pszTimerName);
        if (iter != timers.end())
            retValue = (*iter->second).GetId();
    }

    return retValue;
}


//IMessage &CXMLStateMachine::MakeSynchCopyEx(IMessage &msg, const _TCHAR *pszNewMsgType)
//{
//    if (!msg.IsSynchronous())
//        throw exception(_T("Cannot change asynchronous message type"));
//
//    CMessageObject &msgObject = dynamic_cast<CMessageObject &>(msg);
//
//    int nMsgID = GetMsgID(pszNewMsgType);
//
//    CMessageObject &newMsgObject = msgObject.SynchCopy();
//    newMsgObject.SetId(nMsgID);
//    newMsgObject.SetName(pszNewMsgType);
//
//    return newMsgObject;
//}


IMessage &CXMLStateMachine::MakeAsynchCopy(IMessage &msg)
{
    if (msg.IsSynchronous())
        throw exception("Cannot copy synchronous message");

    CMessageObject &msgObject = dynamic_cast<CMessageObject &>(msg);

    return (IMessage &)msgObject.AsynchCopy();
}


IMessage &CXMLStateMachine::MakeAsynchCopyEx(IMessage &msg
                                           , const _TCHAR *pszNewMsgType)
{
    if (msg.IsSynchronous())
        throw exception("Cannot copy synchronous message");

    CMessageObject &msgObject = dynamic_cast<CMessageObject &>(msg);

    int nMsgID = GetMsgID(pszNewMsgType);

    CMessageObject &newMsgObject = msgObject.AsynchCopy();
    newMsgObject.SetTraceObject(GetTraceHandle());
    newMsgObject.SetId(nMsgID);
    newMsgObject.SetName(pszNewMsgType);

    return (IMessage &)newMsgObject;
}


/**
 *  Asynchronous implementation for processing IMessages.
 *
 *  @param IMessage - the incoming IMessage
 */
HRESULT CXMLStateMachine::SendMsg(IMessage &msg)
{
    //m_SyncLock.Lock();

    CMessageObject &msgImpl = dynamic_cast<CMessageObject &>(msg);
    msgImpl.SetSynchronous(true);
    m_qMsgs.Add(&msg);

    msg.Wait();    //lint !e534

    //m_SyncLock.Unlock();

    return msgImpl.GetRC();
}


void CXMLStateMachine::SetSynchronousReturnCode(IMessage &msg, HRESULT hr)
{
    CMessageObject &msgImpl = dynamic_cast<CMessageObject &>(msg);
    msgImpl.SetRC(hr);
}


bool CXMLStateMachine::IsValidMessage(IMessage *pMsg) const
{
    StringType sMsgTypeName;
    try
    {
        CA2T tMessage(typeid(*pMsg).name());
        sMsgTypeName = tMessage;
    }
    catch( const __non_rtti_object  &e )
        { ITRACE_EXCEPTION(e); }
    catch( const bad_typeid &e )
        { ITRACE_EXCEPTION(e); }
    return (sMsgTypeName == m_sMsgTypeName);
}


HRESULT CXMLStateMachine::ConstructMessage(PMESSAGE *ppMsg, const _TCHAR *pszName)
{
        // Create the timeout message for this timer
    CMsgImpl *pMsg = (CMsgImpl *)CreateNextMessage();
    pMsg->SetTraceObject(GetTraceHandle());
    pMsg->Name.Value(pszName);

    *ppMsg = pMsg;

    return S_OK;
}


void CXMLStateMachine::SetData(IMessage &message, const void *pvData, size_t length)
{
    _ASSERTE(IsValidMessage(&message));

    CMessageObject &msgObject = dynamic_cast<CMessageObject &>(message);

    msgObject.SetData(pvData, length);
}


void CXMLStateMachine::SetText(IMessage &message, BSTR bstrText)
{
    _ASSERTE(IsValidMessage(&message));

    CMessageObject &msgObject = dynamic_cast<CMessageObject &>(message);

    msgObject.SetText(bstrText);
}


IState * CXMLStateMachine::CreateNextState()
{
    ++m_nStateCounter;

    return newpooledobject CStateImpl(m_nStateCounter);
}


IMessage * CXMLStateMachine::CreateNextMessage()
{
    ++m_nMsgCounter;

    return newpooledobject CMsgImpl(m_nMsgCounter);
}


IAction * CXMLStateMachine::CreateNextAction()
{
    ++m_nActionCounter;

    return newpooledobject CActionImpl(m_nActionCounter);
}


IVariable * CXMLStateMachine::CreateNextVariable()
{
    ++m_nVariableCounter;

    return newpooledobject CVariableImpl(m_nVariableCounter);
}


STATE_MACHINE_NAMESPACE::ITimer * CXMLStateMachine::CreateNextTimer()
{
    ++m_nTimerCounter;

    return newpooledobject CTimerImpl(m_nTimerCounter);
}


IConstant * CXMLStateMachine::CreateNextConstant()
{
    ++m_nConstantCounter;

    return newpooledobject CConstantImpl(m_nConstantCounter);
}


CStateObjectProxy * CXMLStateMachine::CreateNextStateObjectProxy()
{
    ++m_nStateObjectProxyCounter;

    return newpooledobject CStateObjectProxy(m_nStateObjectProxyCounter);
}


ILibrary * CXMLStateMachine::CreateNextLibrary()
{
    ++m_nLibraryCounter;

    return newpooledobject CLibraryImpl(m_nLibraryCounter);
}


CStateLookupTableImpl * CXMLStateMachine::CreateNextStateLookupTable()
{
    ++m_nStateLookupTableCounter;

    return newpooledobject CStateLookupTableImpl(m_nStateLookupTableCounter);
}


CLookupTableImpl * CXMLStateMachine::CreateNextLookupTable()
{
    ++m_nLookupTableCounter;

    return newpooledobject CLookupTableImpl(m_nLookupTableCounter);
}


ILoggable &CXMLStateMachine::loggable() 
{ 
    if (m_pLoggable == NULL)
        m_pLoggable = &Trace::getLoggable(GetTraceHandle(), eDCSMDefault);

    return *m_pLoggable; 
}
