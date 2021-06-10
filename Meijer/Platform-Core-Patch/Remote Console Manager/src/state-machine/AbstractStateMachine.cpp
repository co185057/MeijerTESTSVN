// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/AbstractStateMachine.cpp 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)AbstractStateMachine.cpp    $Revision: 1 $ $Date: 6/20/08 1:11p $
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

/*lint -save -e666 */

#include "stdafx.h"
#include "AbstractStateMachine.h"
#include "SMTimerManager.h"
#include "Loggable.h"
#include "Message.h"
#include "Action.h"
#include "SMError.h"
#include "RuleSet.h"
#include "verify.h"

using namespace STATE_MACHINE_NAMESPACE;


/**
* Default no-argument constructor
*/
CAbstractStateMachine::CAbstractStateMachine(HTRACE hTrace)
    : m_DefaultStateID(INVALID_STATE)
    , m_RemainTheSameStateID(INVALID_STATE)
    , m_qMsgs(hTrace)
    , m_nTrueStateID(INVALID_STATE)
    , m_nFalseStateID(INVALID_STATE)
    , m_bUseStack(false)
    , m_bIsTrueEndState(true)
    , m_pMsg(NULL)
    , m_bShuttingDown(false)
    , m_nRC(E_FAIL)
    , m_pTimerMgr(NULL)
    , m_pLoggable(NULL)
{
    CTOR_BEGIN(hTrace)
    ITRACE_DEVELOP(_T(__FUNCTION__));
    CTOR_END()
    // DO NOT put any more initialization steps after this.
}


void CAbstractStateMachine::Init() 
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);
}


void CAbstractStateMachine::Start()
{
    m_bShuttingDown = false;

    CThreadBase::Start();
}


void CAbstractStateMachine::Stop()
{
    IMODTRACE_DEVELOP();

    if (!m_bShuttingDown)
    {
        m_bShuttingDown = true;

        m_qMsgs.Add(NULL);

        CThreadBase::Stop();
    }
}


CAbstractStateMachine::~CAbstractStateMachine()
{
    IMODTRACE_DEVELOP();

    try
    {
        delete m_pTimerMgr;
    }
    catch (...)
    { ITRACE_ERROR(_T("Caught exception")); }

    m_pLoggable = NULL;
    m_pMsg = NULL;
}


CSMTimerManager &CAbstractStateMachine::GetTimerMgr()
{
    if (m_pTimerMgr == NULL)
        m_pTimerMgr = new CSMTimerManager(GetTraceHandle());

    return *m_pTimerMgr;
}


HRESULT CAbstractStateMachine::PostMsg (IMessage &msg, bool bImmediate)
{
    m_qMsgs.Add(&msg, bImmediate);

    return S_OK;
}


/**
 *  Default implementation for processing IMessages.
 *
 *  @param IMessage - the incoming IMessage
 */
HRESULT CAbstractStateMachine::ProcessMessage0 (IMessage &msg)
{
//  IMODTRACE_DEVELOP();
    //ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  &msg:  ") + IString((long)&msg).d2x());

    HRESULT retValue = S_OK;
    int stateId;
    CRulesList rules;

    // If the Global state has been specified, then execute its rules first.
    stateId = GetGlobalState();
    if (GetGlobalRuleSet(rules, msg.GetId()) == S_OK)
    {
        HRESULT hr = ProcessCompleteRuleSet (GetGlobalIdentifier(), rules, msg);
        ITRACE_ERROR(_T("ProcessCompleteRuleSet returned error:  ") + IString(hr).d2x());
    }
    else if (GetGlobalRuleSet(rules, ANY_MSG) == S_OK)
    {
        HRESULT hr = ProcessCompleteRuleSet (GetGlobalIdentifier(), rules, msg);
        ITRACE_ERROR(_T("ProcessCompleteRuleSet returned error:  ") + IString(hr).d2x());
    }

    // Check for rules specific to this IMessage and this state
    stateId = GetCurrentState();
    if (GetRuleSet(rules, stateId, msg) == S_OK)
    {
        retValue = ProcessRuleSet (rules, msg);
        RecordMessage(msg);
    }

    // Check for default rules, ie.  check to see if there is a set of default
    // rules for this IMessage and any state.
    else if (GetRuleSet(rules, m_DefaultStateID, msg) == S_OK)
    {
        retValue = ProcessRuleSet (rules, msg);
        RecordMessage(msg);
    }

    // OK.  We can't find any rules, so report it and ignore it
    else
    {
        if (loggable().IsDebug())
        {
            ITRACE_DEVELOP(_T("\r\n    ____________________________________________\r\n    ") +
                IString(GetIdentifier()) + IString(_T("  No Business Rule for :")) +
                _T("\r\n    State = ") + IString(GetStateName(stateId)) +
                _T("\r\n    IMessage = ") + IString(msg.GetName()) +
                _T("\r\n    ____________________________________________"));
        }

        retValue = STATEM_NO_RULES_DEFINED;
    }

    return retValue;
}


/**
 * Processes the RulesList of Rules that were returned from persistence.
 * @param rules The RulesList of rules to be processed.
 * @param IMessage The IMessage to be asserted against the rules
 */
HRESULT CAbstractStateMachine::ProcessRuleSet (CRulesList &_rules, IMessage &_msg)
{
    HRESULT hr = S_OK;

    m_bIsTrueEndState       = true;

    /* Set initial values */
    int nextStateID         = 0;
    int actionID            = 0;
    int falseActionID       = 0;

    bool result             = true;
    bool falseResult        = true;

    // Loop thru all actions available for execution stopping if one of the 
    // following conditions becomes true:
    //      1.  No more rules are available ( ruleSet.complete( ) )
    //      2.  An action reports a failure ( hr != S_OK )
    //      3.  An action returns a false ( result is false )
    //      4.  Shutdown is in progress and no more actions are required
    // Determination of the "next rule" to execute is provided by the iterator
    // class CRuleSet.  Given a set of rules, CRuleSet traverses the set until
    // it can determine no more rules to execute for this message in this state.
    CRuleSet ruleSet(&_rules);
    for ( ; 
          !m_bShuttingDown && (!ruleSet.complete( )) && (hr == S_OK) && result; 
          ruleSet++)
    {
        /* For each rule that comes back, execute the appropriate action */
        IRule &rule = **ruleSet;

        nextStateID         = rule.GetStateID();
        actionID            = rule.GetActionID();
        m_nTrueStateID      = rule.GetEndStateID();
        falseActionID       = rule.GetFalseActionID();
        m_nFalseStateID     = rule.GetFalseStateID();

        IStringBuffer id(128);
        id += GetIdentifier();
        id += _T("  Next Rule:");

        // Execute the "true" action.  If any errors are detected, then the
        // return code will be some value other than S_OK.
        hr = ExecuteAction(actionID
            , nextStateID
            , rule.GetActionParms()
            , (const _TCHAR *)id
            , result
            , _msg);

        // If the "true" action returned S_FALSE, then we treat it like
        // the logical end of this chain of actions
        // NULL action.
        if (hr == S_FALSE)
        {
            if (loggable().IsDebug()) ITRACE_DEVELOP(_T("Action has requested end of chain"));
        }

        // If the "true" action failed (ie. returned some value other than
        // S_OK, then execute any exception action in response.  If no action
        // was specified in the XML, then the action will default to the
        // NULL action.
        else if (hr != S_OK)
        {
            VERIFY_AND_LOG(ProcessFailure (rule, _msg, hr));
        }

        // If the "true" action set the result bool to false,
        // then execute any failure action in response.  If no action
        // was specified in the XML, then the action will default to the
        // NULL action.
        else if (!result)
        {
            m_bIsTrueEndState = false;

            IStringBuffer falseid(128);
            falseid += GetIdentifier();
            falseid += _T("  False action:");

            hr = ExecuteAction(falseActionID
                , nextStateID
                , rule.GetFalseParms()
                , (const _TCHAR *)falseid
                , falseResult
                , _msg);

            // If the "true" action returned S_FALSE, then we treat it like
            // the logical end of this chain of actions
            // NULL action.
            if (hr == S_FALSE)
            {
                if (loggable().IsDebug()) ITRACE_DEVELOP(_T("Action has requested end of chain"));
            }

            // Check to ensure the false action completed normally with no
            // errors.  If an error was detected, then attempt to execute 
            // any specified exception action.
            else if (hr != S_OK)
            {
                VERIFY_AND_LOG(ProcessFailure (rule, _msg, hr));
            }

            // Support for goto-state action in the false leg.  Ignore
            // false end state if goto was executed.
            else if (!m_bIsTrueEndState)
                ProcessStateSwitch(GetFalseStateID(), _msg);
        }
    }

    // Do we need to switch states?  If so then do it.
    if (m_bIsTrueEndState)
    {
        switch (hr)
        {
            case S_OK :
            case S_FALSE :
                ProcessStateSwitch(GetTrueStateID(), _msg);
                break;
            default :
                break;
        }
    }

    return hr;
}


/**
 * Processes the entire RulesList of Rules, ignoring the return codes from each.
 * @param rules The RulesList of rules to be processed.
 * @param IMessage The IMessage to be asserted against the rules
 */
HRESULT CAbstractStateMachine::ProcessCompleteRuleSet (const _TCHAR *_identifier, CRulesList &_rules, IMessage &_msg)
{
    HRESULT hr = S_OK;

    /* Set initial values */
    int nextStateID         = GetCurrentState();
    int actionID            = 0;
    int falseActionID       = 0;

    bool result;

    CRuleSet ruleSet(&_rules);
    for ( ; 
          !m_bShuttingDown && !ruleSet.complete( );
          ruleSet++)
    {
        /* For each row that comes back, execute the appropriate action */
        IRule &rule = **ruleSet;
        //IRule &rule = *(*iter).second;

        actionID            = rule.GetActionID();
        falseActionID       = rule.GetFalseActionID();

        IStringBuffer id(128);
        id += _identifier;
        id += _T("  Next Rule:");

        hr = ExecuteAction(actionID
            , nextStateID
            , rule.GetActionParms()
            , (const _TCHAR *)id
            , result
            , _msg);

        if (hr != S_OK)
        {
            VERIFY_AND_LOG(ProcessFailure (rule, _msg, hr));
        }
        else if (!result)
        {
            IStringBuffer falseid(128);
            falseid += GetIdentifier();
            falseid += _T("  False action:");

            hr = ExecuteAction(falseActionID
                , nextStateID
                , rule.GetFalseParms()
                , (const _TCHAR *)falseid
                , result
                , _msg);

            if (hr != S_OK)
                VERIFY_AND_LOG(ProcessFailure (rule, _msg, hr));
        }
    }

    return hr;
}


/**
 * Runs the Execute method of the given Action.
 * @param rules The RulesList of rules to be processed.
 * @param IMessage The IMessage to be asserted against the rules
 */
HRESULT CAbstractStateMachine::ExecuteAction (int _actionID
                                            , int _stateID
                                            , const CParameterList &_list
                                            , const _TCHAR *_identifier
                                            , bool &bResult
                                            , IMessage &_msg)
{
    HRESULT hr = S_OK;

    /* Try to get the Action */
    IAction *pAction        = NULL;

    if (loggable().IsDebug())
    {
        const TCHAR *pszStateName   = GetStateName(_stateID);
        const TCHAR *pszActionName  = GetActionName(_actionID);
        ITRACE_DEVELOP(_T("\r\n    ____________________________________________\r\n    ") +
            IString(_identifier) + 
            _T("\r\n    State = ") + IString(pszStateName) +
            _T("\r\n    IMessage = ") + IString(_msg.GetName()) +
            _T("\r\n    Action = ") + IString(pszActionName) +
            _T("\r\n    ____________________________________________"));
    }

    /* Attempt to execute the Action */
    try
    {
        hr = GetAction(&pAction, _actionID);

        if (hr != S_OK)
        {
            ITRACE_ERROR(_T("Failure occurred obtaining action with id: ") 
                + IString(_actionID));
        }

        else
        {
            // Copy any parameter values from the XML into the action
            CopyParameters(pAction, _list);

            // Execute the action.  If it returns false, then execute the
            // False action for this rule.
            hr = (*pAction).Execute(&bResult, _msg);
        }
    }
    catch (const IException &e)
    {
        ITRACE_EXCEPTION(e);

        hr = e.errorId();
    }
    catch (const exception &e)
    {
        ITRACE_EXCEPTION(e);

        hr = E_FAIL;
    }
    catch (unsigned int nCExceptionNumber)
    {
        ITRACE_ERROR(_T("Caught C Exception in Run():  ") 
            + IString(nCExceptionNumber));
        ITRACE_ERROR(_T("Unknown exception caught near:  ") 
            + IString(__LINE__));

        hr = (long)nCExceptionNumber;
    }
    catch (...)
    {
        ITRACE_DEVELOP(_T("Unknown exception caught near:  ") + IString(__LINE__));

        hr = E_FAIL;
    }

    VERIFY_AND_LOG(ReleaseAction(pAction));

    if (loggable().IsDebug())
        ITRACE_DEVELOP(_T("Action complete"));

    return hr;
}


/**
 * Processes the RulesList of Rules that were returned from persistence.
 * @param rules The RulesList of rules to be processed.
 * @param IMessage The IMessage to be asserted against the rules
 */
HRESULT CAbstractStateMachine::ProcessFailure (IRule &rule, IMessage &_msg, HRESULT hr)
{
    HRESULT retValue = S_OK;

    int failureActionID     = rule.GetFailureActionID();
    int failureStateID      = rule.GetFailureStateID();

    bool result;

    if (hr != S_OK)
    {
        const utils::IErrorInfo &error = GetErrorInfo(hr);
        ITRACE_ERROR(error.text());
    }

    IStringBuffer id(128);
    id += GetIdentifier();
    id += _T("  Failure action:");

    retValue = ExecuteAction(failureActionID
        , rule.GetStateID()
        , rule.GetFailureParms()
        , (const _TCHAR *)id
        , result
        , _msg);

    if (retValue != S_OK)
        ITRACE_ERROR(_T("Failure Action failed to execute"));

    ProcessStateSwitch(failureStateID, _msg);

    return retValue;
}


/**
 * Returns the set of Business Rules to execute, based on the passed State ID
 * and the IMessage ID.
 *
 * @param _stateID The State ID
 * @param _IMessageID The ID of the IMessage that was received in the passed State.
 * @return rules A <CODE>RulesList</CODE> of Business Rules with specified criteria.
 */
HRESULT CAbstractStateMachine::GetRuleSet (CRulesList &rulesList, int stateID, IMessage &msg)
{
    return GetRuleSet (rulesList, stateID, msg.GetId());
}


void CAbstractStateMachine::GotoState(int nStateID)
{
    if (loggable().IsDebug()) ITRACE_DEVELOP(_T(__FUNCTION__) _T("New State:  ") + IString(GetStateName(nStateID)));
	m_nTrueStateID      = nStateID;
    m_bIsTrueEndState   = true;
}


void CAbstractStateMachine::PushState(int nStateID)
{
    m_stackStates.push_front(nStateID);

    m_bUseStack = false;
}


void CAbstractStateMachine::PopState()
{
    m_bUseStack = true;
}


void CAbstractStateMachine::CleanState()
{
    CStateStack::iterator iter = m_stackStates.begin();
    int nStateId;
    for (;iter != m_stackStates.end(); iter++)
    {
        nStateId = *iter;
        ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Ignoring stacked state:  ") + IString(nStateId));
    }
    m_stackStates.clear();
    
    m_bUseStack = false;
}


int CAbstractStateMachine::GetTrueStateID()
{
    int stateID;

    if (m_bUseStack)
    {
        if (m_stackStates.empty())
        {
            ITRACE_ERROR(_T("State stack empty!  Problem detected"));
            stateID = m_nTrueStateID;
        }

        else
        {
            if (loggable().IsDebug()) ITRACE_DEVELOP(_T("State stack not empty!  Popping return state off stack"));
            stateID = m_stackStates.front();
            m_stackStates.pop_front();
        }

        m_bUseStack = false;
    }
    else
        stateID = m_nTrueStateID;

    return stateID;
}


int CAbstractStateMachine::GetFalseStateID()
{
    int stateID;

    if (m_bUseStack)
    {
        if (m_stackStates.empty())
        {
            ITRACE_ERROR(_T("State stack empty!  Problem detected"));
            stateID = m_nFalseStateID;
        }

        else
        {
            if (loggable().IsDebug()) ITRACE_DEVELOP(_T("State stack not empty!  Popping return state off stack"));
            stateID = m_stackStates.front();
            m_stackStates.pop_front();
        }

        m_bUseStack = false;
    }
    else
        stateID = m_nFalseStateID;

    return stateID;
}


void CAbstractStateMachine::ProcessStateSwitch(int _newState, IMessage &msg)
{
    CRulesList rules;

    if (m_bShuttingDown)
        return;
    if (_newState == GetDefaultStateID())
        return;
    if (_newState == GetRemainTheSameStateID())
        return;

    if (GetStateExitRuleSet(rules, GetCurrentState()) == S_OK)
    {
        VERIFY_AND_LOG(ProcessCompleteRuleSet (GetExitIdentifier(), rules, msg));
    }

    SetCurrentState(_newState);

    if (GetStateEntryRuleSet(rules, GetCurrentState()) == S_OK)
    {
        VERIFY_AND_LOG(ProcessCompleteRuleSet (GetEntryIdentifier(), rules, msg));
    }
}


HRESULT CAbstractStateMachine::GetAction(IAction **, const _TCHAR *)
{
    return E_NOINTERFACE;
}


HRESULT CAbstractStateMachine::GetMessage(IMessage **, const _TCHAR *, const void *, size_t)
{
    return E_NOINTERFACE;
}


HRESULT CAbstractStateMachine::GetCurrentMsg(IMessage **ppMsg)
{
    *ppMsg = m_pMsg;

    return S_OK;
}


HRESULT CAbstractStateMachine::GetState(IState **, const _TCHAR *)
{
    return E_NOINTERFACE;
}


HRESULT CAbstractStateMachine::GetVariable(IVariable **, const _TCHAR *)
{
    return E_NOINTERFACE;
}


HRESULT CAbstractStateMachine::GetConstant(IConstant **, const _TCHAR *)
{
    return E_NOINTERFACE;
}


HRESULT CAbstractStateMachine::GetExpression(IExpression **, const _TCHAR *)
{
    return E_NOINTERFACE;
}


HRESULT CAbstractStateMachine::GetStateLookupTable(IStateLookupTable **, const _TCHAR *)
{
    return E_NOINTERFACE;
}


HRESULT CAbstractStateMachine::GetLookupTable(ILookupTable **, const _TCHAR *)
{
    return E_NOINTERFACE;
}


HRESULT CAbstractStateMachine::GetTimer(STATE_MACHINE_NAMESPACE::ITimer **, const _TCHAR *)
{
    return E_NOINTERFACE;
}


HRESULT CAbstractStateMachine::OnTimerTimeout(STATE_MACHINE_NAMESPACE::ITimer *)
{
    return E_NOINTERFACE;
}


HRESULT CAbstractStateMachine::GetAction(IAction **, int)
{
    return E_NOINTERFACE;
}


HRESULT CAbstractStateMachine::GetMessage(IMessage **, int, const void *, size_t)
{
    return E_NOINTERFACE;
}


HRESULT CAbstractStateMachine::GetState(IState **, int)
{
    return E_NOINTERFACE;
}


HRESULT CAbstractStateMachine::GetVariable(IVariable **, int)
{
    return E_NOINTERFACE;
}


HRESULT CAbstractStateMachine::GetConstant(IConstant **, int)
{
    return E_NOINTERFACE;
}


HRESULT CAbstractStateMachine::GetTimer(STATE_MACHINE_NAMESPACE::ITimer **, int)
{
    return E_NOINTERFACE;
}


HRESULT CAbstractStateMachine::ReleaseAction(IAction *)
{
    return E_NOINTERFACE;
}


void CAbstractStateMachine::CopyParameters(IAction *, const CParameterList &)
{
    IMODTRACE_DEVELOP();
}


const TCHAR * CAbstractStateMachine::GetActionName(int) const
{
    return NULL;
}


const TCHAR * CAbstractStateMachine::GetStateName(int) const
{
    return NULL;
}


void CAbstractStateMachine::SetCurrentState(int)
{
}


const TCHAR * CAbstractStateMachine::GetIdentifier() const
{
    return NULL;
}


const TCHAR * CAbstractStateMachine::GetGlobalIdentifier() const
{
    return NULL;
}


const TCHAR * CAbstractStateMachine::GetExitIdentifier() const
{
    return NULL;
}


const TCHAR * CAbstractStateMachine::GetEntryIdentifier() const
{
    return NULL;
}


/**
    * Save the current Fact.
    */
void CAbstractStateMachine::RecordMessage(IMessage &)
{
}


HRESULT CAbstractStateMachine::GetRuleSet (CRulesList &, int, int)
{
    return E_NOINTERFACE;
}


HRESULT CAbstractStateMachine::GetGlobalRuleSet (CRulesList &, int)
{
    return E_NOINTERFACE;
}


HRESULT CAbstractStateMachine::GetStateExitRuleSet (CRulesList &, int)
{
    return E_NOINTERFACE;
}


HRESULT CAbstractStateMachine::GetStateEntryRuleSet (CRulesList &, int)
{
    return E_NOINTERFACE;
}


/**
    * The state ID for the Default state
    */
int CAbstractStateMachine::GetDefaultStateID()
{
    return -1;
}

int CAbstractStateMachine::GetRemainTheSameStateID()
{
    return -1;
}


int CAbstractStateMachine::GetCurrentState() const
{
    return -1;
}


int CAbstractStateMachine::GetGlobalState() const
{
    return -1;
}


void CAbstractStateMachine::RegisterAll() {}


void CAbstractStateMachine::UnregisterAll() {}


unsigned long CAbstractStateMachine::GetMsgInterval() const
{
    return MAX_MSG_INTERVAL;
}


ILoggable &CAbstractStateMachine::loggable() 
{ 
    if (m_pLoggable == NULL)
        m_pLoggable = &Trace::getLoggable(GetTraceHandle(), eDCSMExecution);

    return *m_pLoggable; 
}


void CAbstractStateMachine::BeginExecution() {}


void CAbstractStateMachine::EndExecution() {}


#pragma warning( push )
#pragma warning( disable : 4311 )
unsigned CAbstractStateMachine::Run()
{
    IMODTRACE_DEVELOP();

    BeginExecution();

    unsigned long lTracePoint = 0;
    unsigned long ulMsgCount = 0;

    try
    {
        SetInitialState();

        HRESULT hr = S_OK;
        m_pMsg = NULL;
        lTracePoint = 1;
        unsigned long ulWaitTime = GetMsgInterval();
        while (!m_bShuttingDown)
        {
            m_pMsg = m_qMsgs.Remove(ulWaitTime);

            // If no msg was received then we timed out.  Add msg to the log
            // to signify that we are not dead, but just have not received
            // any messages for awhile
            if (m_pMsg == NULL)
            {
                ulMsgCount++;
                if (ulMsgCount == 10)
                {
                    if (loggable().IsDebug()) ITRACE_DEVELOP(_T("No message detected ..."));
                    ulMsgCount = 0;
                }
                continue;
            }

            // Reset since we got a message
            ulMsgCount = 0;

            if (!IsValidMessage(m_pMsg))
            {
                TRACE_MSG(m_pMsg);
                ITRACE_ERROR(_T("Invalid message detected ..."));
                continue;
            }

            lTracePoint = 2;
            try
            {
                ProcessMessage0(*m_pMsg);   // We're ignoring the return code because
                                            // it has already been reported to log.
                hr = S_OK;                  // We've already reported this error so
                                            // don't report it again.
                lTracePoint = 3;
            }
            catch (exception &e)
            {
                ITRACE_EXCEPTION(e);

                hr = E_FAIL;
                lTracePoint = 4;
            }
            catch (unsigned int nCExceptionNumber)
            {
                ITRACE_ERROR(_T("Caught C Exception in Run():  ") + IString(nCExceptionNumber));

                hr = (HRESULT)nCExceptionNumber;
                lTracePoint = 5;
            }
            catch (...)
            {
                ITRACE_ERROR(_T("Unknown error detected in Run()"));

                hr = E_FAIL;
                lTracePoint = 6;
            }
            lTracePoint = 7;

            _ASSERTE(m_pMsg!=NULL);
            if (m_pMsg->IsSynchronous())
            {
                m_pMsg->Post();
                lTracePoint = 8;
                ::Sleep(0);
            }
            else
            {
                lTracePoint = 9;
                delete m_pMsg;
                m_pMsg = NULL;
            }

            VERIFY_AND_LOG(hr);
        }

        if (loggable().IsDebug()) ITRACE_DEVELOP(_T("Shutting down.  m_bShuttingDown=") + IString(m_bShuttingDown));

        m_pMsg = m_qMsgs.Remove(ulWaitTime);
        lTracePoint = 10;
        while (m_pMsg != NULL)
        {
            ITRACE_DEVELOP(_T("Shutting down.  Throwing away message"));

            if (m_pMsg->IsSynchronous())
            {
                lTracePoint = 11;
                m_pMsg->Post();
            }
            else
            {
                lTracePoint = 12;
                delete m_pMsg;
                m_pMsg = NULL;
            }

            lTracePoint = 13;
            m_pMsg = m_qMsgs.Remove(ulWaitTime);
            lTracePoint = 14;
        }

        lTracePoint = 15;
        UnregisterAll();
    }
    catch (const IException &exc)
    {
        ITRACE_EXCEPTION(exc);
        ITRACE_DEVELOP(_T("Last Trace Point:  " + IString(lTracePoint)));

        _ASSERTE(0);
    }
    catch (...)
    {
        ITRACE_DOT_ERROR();
        ITRACE_DEVELOP(_T("Last Trace Point:  " + IString(lTracePoint)));

        _ASSERTE(0);
    }

    EndExecution();

    return 0;
}
#pragma warning( pop )

/*lint -restore */
