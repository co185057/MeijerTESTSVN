#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/AbstractStateMachine.h 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)AbstractStateMachine.h    $Revision: 1 $ $Date: 6/20/08 1:11p $
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


#include <vector>
#include "SMConstants.h"
#include "rule.h"
#include "xerror.h"
#include "SynchronizedQueue.h"
#include "xthread.h"
#include "statem.h"

#define STATE_MACHINE_MAIN "StateMachine"
#define INVALID_THREAD_ID   (unsigned)0xFFFFFFFF
#define MAX_MSG_INTERVAL   2000

#pragma warning( push )
#pragma warning( disable : 4251 )

class CSMTimerManager; 

namespace STATE_MACHINE_NAMESPACE
{

class IExpression;
class IConstant;
class ITimer;
class IVariable;
class IMessage;
class IAction;
class IState;
class IRule;
class IStateLookupTable;
class ILookupTable;
class CVariableOperations; 
class CTimerOperations; 

//lint --e{1735}        Virtual fxns with default args OK
class STATEM_API CAbstractStateMachine
    : public CThreadMemberBase<CAbstractStateMachine>
{
public:

    typedef list<int> CStateStack;

    friend class CThreadMemberBase<CAbstractStateMachine>;
    friend class CGotoState;
    friend class CActionExpressionImpl;
    friend class CIfExpressionImpl;

    ///
    /// Default no-argument constructor
    ///
    CAbstractStateMachine(HTRACE hTrace);

    virtual ~CAbstractStateMachine();


    virtual HRESULT GetAction(IAction **, const _TCHAR *);


    virtual HRESULT GetMessage(IMessage **, const _TCHAR *, const void *pData=NULL, size_t nDataLength=0);


    virtual HRESULT GetMessage(IMessage **, int, const void *pData=NULL, size_t nDataLength=0);


    virtual HRESULT GetState(IState **, const _TCHAR *);


    virtual HRESULT GetCurrentState(IState **)=0;


    virtual HRESULT GetVariable(IVariable **, const _TCHAR *);


    virtual HRESULT GetConstant(IConstant **, const _TCHAR *);


    virtual HRESULT GetExpression(IExpression **, const _TCHAR *);


    virtual HRESULT GetStateLookupTable(IStateLookupTable **, const _TCHAR *);


    virtual HRESULT GetLookupTable(ILookupTable **, const _TCHAR *);


    virtual HRESULT GetTimer(STATE_MACHINE_NAMESPACE::ITimer **, const _TCHAR *);


    virtual HRESULT OnTimerTimeout(STATE_MACHINE_NAMESPACE::ITimer *);


    virtual bool IsValidAction(const _TCHAR *)=0;


    virtual bool IsValidMessage(const _TCHAR *)=0;


    virtual bool IsValidState(const _TCHAR *)=0;


    virtual bool IsValidVariable(const _TCHAR *)=0;


    virtual bool IsValidConstant(const _TCHAR *)=0;


    virtual bool IsValidTimer(const _TCHAR *)=0;


    /**
     *  Default implementation for processing Facts.
     *
     *  @param fact - the incoming Fact
     */
    virtual HRESULT SendMsg(IMessage &)=0;                          // Synchronous message handling
    virtual HRESULT PostMsg(IMessage &, bool bImmediate = false);   // Asynchronous message handling

//  virtual IMessage &MakeSynchCopyEx(IMessage &, const _TCHAR *)=0;

    virtual IMessage &MakeAsynchCopy(IMessage &)=0;

    virtual IMessage &MakeAsynchCopyEx(IMessage &, const _TCHAR *)=0;

    virtual void Start();

    virtual void Stop();

    virtual void SetSynchronousReturnCode(IMessage &msg, HRESULT hr)=0;

    void GotoState(int nStateID);
    void PushState(int nStateID);
    void PopState();
    void CleanState();

    friend class CVariableOperations; 
    friend class CTimerOperations; 

    virtual CSMTimerManager &GetTimerMgr();

    virtual void SetData(IMessage &, const void *, size_t)=0;
    virtual void SetText(IMessage &, BSTR)=0;

    virtual const utils::IErrorInfo &GetErrorInfo(HRESULT hr)=0;

protected:

    virtual void BeginExecution();

    virtual void EndExecution();

    virtual HRESULT ProcessMessage0(IMessage &);


    virtual HRESULT GetCurrentMsg(IMessage **);


    virtual HRESULT GetAction(IAction **, int);


    virtual HRESULT GetState(IState **, int);


    virtual HRESULT GetVariable(IVariable **, int);


    virtual HRESULT GetConstant(IConstant **, int);


    virtual HRESULT GetTimer(STATE_MACHINE_NAMESPACE::ITimer **, int);


    virtual HRESULT ReleaseAction(IAction *);


    virtual void CopyParameters(IAction *, const CParameterList &list);


    virtual const TCHAR * GetActionName(int) const;


    virtual const TCHAR * GetStateName(int) const;


    virtual void SetCurrentState(int);


    virtual void SetInitialState()=0;


    virtual const TCHAR * GetIdentifier() const;


    virtual const TCHAR * GetGlobalIdentifier() const;


    virtual const TCHAR * GetExitIdentifier() const;


    virtual const TCHAR * GetEntryIdentifier() const;


    /**
     * Save the current Fact.
     */
    virtual void RecordMessage(IMessage &);


    virtual HRESULT GetRuleSet (CRulesList &, int, int);


    virtual HRESULT GetGlobalRuleSet (CRulesList &, int);


    virtual HRESULT GetStateExitRuleSet (CRulesList &, int);


    virtual HRESULT GetStateEntryRuleSet (CRulesList &, int);


    /**
     * The state ID for the Default state
     */
    virtual int GetDefaultStateID();

    /**
     * The state ID for the RemainTheSame state
     */
    virtual int GetRemainTheSameStateID();


    virtual int GetCurrentState() const;


    virtual int GetGlobalState() const;


    virtual HRESULT ProcessFailure(IRule &, IMessage &, HRESULT hr);


    /**
     * Processes the collection of Rules that were returned from persistence.
     * @param rules The collection of rules to be processed.
     * @param fact The fact to be asserted against the rules
     */
    virtual HRESULT ProcessRuleSet (CRulesList &, IMessage &);


    /**
     * Processes the collection of Rules that were returned from persistence.
     * @param rules The collection of rules to be processed.
     * @param fact The fact to be asserted against the rules
     */
    virtual HRESULT ProcessCompleteRuleSet (const _TCHAR *
                                          , CRulesList &
                                          , IMessage &);


    /**
     * Returns the set of Business Rules to execute, based on the passed State ID
     * and the Fact ID.
     *
     * @param _stateID The State ID
     * @param _FactID The ID of the Fact that was received in the passed State.
     * @return rules A <CODE>Collection</CODE> of Business Rules with specified criteria.
     */
     virtual HRESULT GetRuleSet (CRulesList &, int, IMessage &);


    virtual bool IsValidMessage(IMessage *) const=0;

     
    virtual void RegisterAll();

    virtual void UnregisterAll();


    int m_DefaultStateID;
    int m_RemainTheSameStateID;

    int GetTrueStateID();
    int GetFalseStateID();

    virtual void Init();

    virtual unsigned long GetMsgInterval() const;

    unsigned Run();

    CSynchronizedQueue<IMessage> m_qMsgs;

private:

    /**
     * Executes a single Action
     * @param _actionID The id of the action to execute
     * @param _stateID The id of the current state
     * @param _identifier A trace identifier
     * @param _stateID The message causing the execution
     */
    HRESULT ExecuteAction (int _actionID
                         , int _stateID
                         , const CParameterList &list
                         , const _TCHAR *_identifier
                         , bool &bResult
                         , IMessage &_msg);


    void ProcessStateSwitch(int, IMessage &);

    ILoggable &loggable();

private:
    
    CAbstractStateMachine();

    CStateStack m_stackStates;
    
    int m_nTrueStateID;
    int m_nFalseStateID;
    bool m_bUseStack;
    bool m_bIsTrueEndState;

    IMessage *m_pMsg;

    bool m_bShuttingDown;

    HRESULT m_nRC;

    CSMTimerManager *m_pTimerMgr;

    ILoggable *m_pLoggable;

};

}
#pragma warning( pop )
