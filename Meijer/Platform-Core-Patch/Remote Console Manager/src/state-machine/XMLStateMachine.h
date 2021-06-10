#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/XMLStateMachine.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)XMLStateMachine.h      $Revision: 1 $  $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/XMLStateMachine.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 10    5/03/05 4:05p Dm185016
 * Cleaned up tracing of state machine
 * 
 * 9     4/21/05 7:25p Dm185016
 * Made entry/exit tracing conditional.
 * Added more heap corruption checks.
 * 
 * 7     4/05/05 12:02p Dm185016
 * Added ability to set the location from which the xml will be read.
 * 
 * 6     3/28/05 5:06p Dm185016
 * TAR 296771
 * 
 * 5     3/22/05 4:33p Dm185016
 * <if> support
 * UNICODE
 * 
 * 4     2/28/05 3:54p Dm185016
 * lint
 * 
 * 3     2/14/05 7:10p Dm185016
 * Release Exclusive Access Event support
 * 
 * 2     2/10/05 5:44p Dm185016
 * Fixed problem with static ids used in SM.
 * TAR 292915
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
* 
* 22    12/22/04 11:21a Dm185016
* Conditional logic support
* 
* 21    11/15/04 11:42a Dm185016
* Moved return code from action to message.
* 
* 19    10/29/04 9:25a Dm185016
* Added goto-state action.
* 
* 18    10/04/04 6:43p Dm185016
* Trace object is no longer shared.
* 
* 16    8/26/04 10:00p Dm185016
* TAR 278069 - Added validity check for messages pulled off queue
* 
* 15    6/30/04 3:29p Dm185016
* Made changes required for SecurityStateMachine derivative.
* 
* 14    5/18/04 1:51p Dm185016
* lint
* 
* 13    5/04/04 4:45p Dm185016
* Added validation of identifiers
* 
* 11    4/21/04 3:10p Dm185016
* Better trace for state names
* 
* 10    4/20/04 4:44p Dm185016
* Expanded the error reporting mechanism.
* 
* 9     4/01/04 1:50p Dm185016
* Fixed AppControl
* 
* 8     3/31/04 4:00p Dm185016
* Added override for asynchronous copy
* 
* 7     3/31/04 10:13a Dm185016
* Added external GetCurrentState()
* 
* 6     3/19/04 3:03p Dm185016
* Memory leak fix
* 
* 5     2/24/04 3:33p Dm185016
* Added message data fields
* 
* 4     2/10/04 11:43a Dm185016
* Added synchronous operation option.
* Added Expressions
* Added set-return-code internal action
* 
* 3     1/27/04 1:57p Dm185016
* DLL Support, resend-msg internal action, Library support
* 
* 2     1/16/04 9:14a Dm185016
* Namespace support, Timers, Constants, Global state, Action parameters,
* Entry and Exit lists, Variables, Timeout support
*/
//lint -sem(Add, custodial(1))
//lint -esym(1768, GetMsgName)

#include "statem.h"
#include <vector>
#include <map>
#include <set>
#include "SMConstants.h"
#include "AbstractStateMachine.h"
#include "SMObjectRepository.h"
#include "SAXErrorHandlerImpl.h"
#include "SAXLexicalHandlerImpl.h"
#include "Message.h"
#include "Action.h"
#include "Rule.h"
#include "RuleProxy.h"
#include "State.h"
#include "Variable.h"
#include "Timer.h"
#include "Constant.h"
#include "Expression.h"
#include "content/StateLookupTableImpl.h"
#include "content/LookupTableImpl.h"
#include "ExpressionFactory.h"
#include "Createable.h"
#include "content/StateObjectProxy.h"
#include "Library.h"
#include "InternalOps.h"

#pragma warning( push )
#pragma warning( disable : 4251 )
using namespace STATE_MACHINE_NAMESPACE;

namespace STATE_MACHINE_NAMESPACE 
{

class CSMLoader;

typedef CCreateableFactory<IAction> CActionFactory;
typedef std::set <StringType, StringComparator > CFileNameSet;

//lint --e{1735}    Allow default parameters to virtual function
class STATEM_API CXMLStateMachine
    : public CAbstractStateMachine
    , public ISMObjectRepository
{
public: 

    CXMLStateMachine(HTRACE hTrace);

    ///<summary>Constructor for use in creation of an instance of this
    ///class</summary>
    ///<param name="pFn">The Name of the XML file to load</param>
    ///<param name="hTrace">The handle to a Trace object to use for debug output</param>
    CXMLStateMachine(const TCHAR *pFn, HTRACE hTrace);
    virtual ~CXMLStateMachine();

    virtual void SetLocation(const _TCHAR *);
    virtual StringType GetLocation();

    virtual void Load();
	
	friend class CSMLibrary;
    friend class CSMLoader;


    void SetURL(const _TCHAR *);


    const _TCHAR *GetURL() const;


    HRESULT GetAction(IAction **, const _TCHAR *);


    HRESULT GetMessage(IMessage **, const _TCHAR *, const void *pData=NULL, size_t nDataLength=0);


    HRESULT GetState(IState **, const _TCHAR *);


    HRESULT GetVariable(IVariable **, const _TCHAR *);


    HRESULT GetConstant(IConstant **, const _TCHAR *);


    HRESULT GetExpression(IExpression **, const _TCHAR *);


    HRESULT GetIfExpression(IExpression **, const _TCHAR *, IExpression *);


    IExpression * GetExpression(const _TCHAR *);


    IExpression * GetTrueExpression();


    IExpression * GetFalseExpression();


    HRESULT GetStateLookupTable(IStateLookupTable **, const _TCHAR * );


    HRESULT GetLookupTable(ILookupTable **, const _TCHAR * );


    HRESULT GetTimer(STATE_MACHINE_NAMESPACE::ITimer **, const _TCHAR *);


    HRESULT GetMessage(IMessage **, int, const void *pData=NULL, size_t nDataLength=0);


    virtual bool IsValidAction(const _TCHAR *);


    virtual bool IsValidMessage(const _TCHAR *);


    virtual bool IsValidState(const _TCHAR *);


    virtual bool IsValidVariable(const _TCHAR *);


    virtual bool IsValidConstant(const _TCHAR *);


    virtual bool IsValidTimer(const _TCHAR *);


    virtual HRESULT OnTimerTimeout(STATE_MACHINE_NAMESPACE::ITimer *);


    int GetActionID(const _TCHAR *);
    int GetMsgID(const _TCHAR *);
    int GetStateID(const _TCHAR *);
    HRESULT GetVariableID(const _TCHAR *, long &);
    int GetConstantID(const _TCHAR *);
    int GetTimerID(const _TCHAR *);
    int GetStateLookupTableID(const _TCHAR *);
    int GetLookupTableID(const _TCHAR *);


    virtual HRESULT GetCurrentState(IState **);


    IState *                            CreateNextState();
    IMessage *                          CreateNextMessage();
    IAction *                           CreateNextAction();
    IVariable *                         CreateNextVariable();
    STATE_MACHINE_NAMESPACE::ITimer *   CreateNextTimer();
    IConstant *                         CreateNextConstant();
    CStateObjectProxy *                 CreateNextStateObjectProxy();
    ILibrary *                          CreateNextLibrary();
    CStateLookupTableImpl *             CreateNextStateLookupTable();
    CLookupTableImpl *                  CreateNextLookupTable();


//  HRESULT Add(IRuleProxy *);
    HRESULT Add(IState *);
    HRESULT Add(IMessage *);
    HRESULT Add(IAction *);
    HRESULT Add(IVariable *);
    HRESULT Add(STATE_MACHINE_NAMESPACE::ITimer *);
    HRESULT Add(IConstant *);
    //HRESULT Add(IExpression *);
    HRESULT Add(CStateObjectProxy *);
    HRESULT Add(ILibrary *);
    HRESULT Add(CStateLookupTableImpl *);
    HRESULT Add(CLookupTableImpl *);

    HRESULT SendMsg(IMessage &);          // Synchronous message handling

//  IMessage &MakeSynchCopyEx(IMessage &, const _TCHAR *);

    IMessage &MakeAsynchCopy(IMessage &);

    IMessage &MakeAsynchCopyEx(IMessage &, const _TCHAR *);
    
    void SetSynchronousReturnCode(IMessage &msg, HRESULT hr);

    virtual void SetData(IMessage &, const void *, size_t);
    virtual void SetText(IMessage &, BSTR);

    virtual const utils::IErrorInfo &GetErrorInfo(HRESULT hr);

protected:

    virtual HRESULT GetAction(IAction **, int);


    virtual HRESULT GetState(IState **, int);


    virtual HRESULT GetVariable(IVariable **, int);


    virtual HRESULT GetStateLookupTable(IStateLookupTable **, int );


    virtual HRESULT GetLookupTable(ILookupTable **, int );


    virtual HRESULT GetConstant(IConstant **, int);


    virtual HRESULT GetTimer(STATE_MACHINE_NAMESPACE::ITimer **, int);


    virtual CSMLoader *GetLoader();
    virtual void ReleaseLoader();

    virtual SAXErrorHandlerImpl *GetErrorHandler();
    virtual SAXLexicalHandlerImpl *GetLexicalHandler();


    virtual HRESULT ReleaseAction(IAction *);


    virtual void CopyParameters(IAction *pAction, const CParameterList &list);


    virtual const TCHAR * GetActionName(int) const;


    virtual const TCHAR * GetStateName(int) const;


    virtual const TCHAR * GetMsgName(int) const;


    virtual const TCHAR * GetCurrentStateName() const;


    virtual void SetCurrentState(int);


    virtual void SetInitialState();


    virtual const TCHAR * GetIdentifier() const{ return _T("XMLStateMachine"); }


    virtual const TCHAR * GetGlobalIdentifier() const { return _T("Global State"); }


    virtual const TCHAR * GetExitIdentifier() const { return _T("Exit List"); }


    virtual const TCHAR * GetEntryIdentifier() const { return _T("Entry List"); }


    /**
     * Save the current message.
     */
    virtual void RecordMessage(IMessage &);


    /**
     * The state ID for the Default state
     */
    inline int GetDefaultStateID(){ return ANY_STATE; }

    /**
     * The state ID for the RemainTheSame state
     */
    inline int GetRemainTheSameStateID(){ return REMAIN_THE_SAME_STATE; }


    int GetCurrentState() const;


    virtual int GetGlobalState() const { return m_nGlobalState; }


    virtual HRESULT GetRuleSet (CRulesList &, int, int);


    virtual HRESULT GetGlobalRuleSet (CRulesList &, int);


    virtual HRESULT GetStateExitRuleSet (CRulesList &, int);


    virtual HRESULT GetStateEntryRuleSet (CRulesList &, int);

    virtual void RegisterAll();

    virtual void UnregisterAll();


    CActionFactory actionFactory;
    CExpressionFactory m_ExprFactory;

    virtual void Init();
	virtual void ReadFile(const TCHAR *);

    virtual IState *GetAnyState();

    virtual IRule * CreateRule(IRuleProxy *);

    virtual void InitActions();
    virtual void InitMsgs();
    virtual void InitStates();
    virtual void InitVariables();
    virtual void InitTimers();
    virtual void InitLibraries();
    virtual void InitConstants();
    virtual void InitStateTables();
    virtual void InitLookupTables();

    unsigned m_nMaxActionID;
    unsigned m_nMaxMsgID;
    unsigned m_nMaxStateID;
    unsigned m_nMaxVariableID;
    unsigned m_nMaxStateTableID;
    unsigned m_nMaxLookupTableID;
    unsigned m_nMaxTimerID;
    unsigned m_nMaxLibraryID;
    unsigned m_nMaxConstantID;
    unsigned m_nMaxExpressionID;
    unsigned m_nMaxStateObjectID;

    virtual bool IsValidMessage(IMessage *) const;

    virtual void SetErrorInfo(utils::IErrorInfo *);

    virtual HRESULT ConstructMessage(PMESSAGE *, const _TCHAR *);

    ILoggable &loggable();

private:

    CXMLStateMachine();
    CXMLStateMachine(const CXMLStateMachine &);                 // Hide the Copy construcor
    CXMLStateMachine &operator =(const CXMLStateMachine &);     // Hide the assignment operator

    inline IState &currentState() { return *m_pCurrentState; }
    
    HRESULT IsValidName(const _TCHAR *pszName) const;

    IState * GetState(const _TCHAR *);

    HRESULT CreateExpression(IExpression **, const _TCHAR *);

    static HRESULT CreateObject(const _TCHAR *pszObjectName, IAction **ppAction);

    StringType m_sFn;
	StringType m_sLocation;
	StringType m_sURL;

    IAction *m_NullAction;

    int m_nInitialState;
    int m_nCurrentState;
    PSTATE m_pCurrentState;
    int m_nGlobalState;
    PSTATE m_pGlobalState;

    CActionsMap             actions;
    CMessagesMap            msgs;
    CStatesMap              states;
    CVariablesMap           variables;
    CStateLookupTablesMap   stateTables;
    CLookupTablesMap        lookupTables;
    CTimersMap              timers;
    CConstantsMap           constants;
    CExpressionsMap         expressions;
    CLibrarysMap            libraries;
	CFileNameSet	        fnames;

    CLibrarysMap    m_mapActionCreationMap;

	CSMLoader *m_pLoader;

    PACTION *   m_pActions;
    PMESSAGE *  m_pMsgs;
    PSTATE *    m_pStates;
    PVARIABLE * m_pVariables;
    PTIMER *    m_pTimers;
    PLIBRARY *  m_pLibraries;
    PCONSTANT * m_pConstants;
    PSTATELOOKUPTABLE * m_pStateTables;
    PLOOKUPTABLE * m_pLookupTables;
    //PEXPRESSION * m_pExpressions;

    mutable StringType m_sStateName;

    StringType  m_sMsgTypeName;

    mutable utils::IErrorInfo *m_pErrorInfo;

    int m_nStateCounter;
    int m_nMsgCounter;
    int m_nActionCounter;
    int m_nVariableCounter;
    int m_nTimerCounter;
    int m_nConstantCounter;
    int m_nStateObjectProxyCounter;
    int m_nLibraryCounter;
    int m_nRuleCounter;
    int m_nSelectCounter;
    int m_nWhenCounter;
    int m_nStateLookupTableCounter;
    int m_nLookupTableCounter;

    ILoggable *m_pLoggable;

    //CResource m_SyncLock;
};

}

#pragma warning( pop )
