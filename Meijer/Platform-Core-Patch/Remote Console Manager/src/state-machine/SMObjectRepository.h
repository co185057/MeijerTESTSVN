#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMObjectRepository.h 1     6/20/08 1:13p Sa250050 $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMObjectRepository.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 5     3/22/05 5:03p Dm185016
 * <if> support
 * 
 * 4     3/17/05 2:46p Dm185016
 * <if>, <else-if>, <else> support
 * 
 * 3     3/03/05 3:17p Dm185016
 * TAR 291700
 * 
 * 2     2/14/05 7:09p Dm185016
 * Release Exclusive Access Event support
 * 
 * 1     2/11/05 10:19a Dm185016
 * 
 * 1     2/10/05 5:41p Dm185016
 * Fixed problem with static ids used in SM.
*/
//lint -sem(Add, custodial(1))

#include "statem.h"
#include "SMConstants.h"
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

//lint --e{1735}    Allow default parameters to virtual function
class STATEM_API ISMObjectRepository
{
public: 

	virtual ~ISMObjectRepository() {};


    virtual HRESULT GetConstant(IConstant **, const _TCHAR *)=0;


    virtual HRESULT GetExpression(IExpression **, const _TCHAR *)=0;


    virtual HRESULT GetIfExpression(IExpression **, const _TCHAR *, IExpression *)=0;


    virtual IExpression * GetExpression(const _TCHAR *)=0;


    virtual IExpression * GetTrueExpression()=0;


    virtual IExpression * GetFalseExpression()=0;


    virtual int GetActionID(const _TCHAR *)=0;
    virtual int GetMsgID(const _TCHAR *)=0;
    virtual int GetStateID(const _TCHAR *)=0;
    //virtual HRESULT GetVariableID(const _TCHAR *, long &)=0;
    //virtual int GetConstantID(const _TCHAR *)=0;
    //virtual int GetTimerID(const _TCHAR *)=0;
    //virtual int GetStateLookupTableID(const _TCHAR *);=0


    virtual IState * CreateNextState()=0;
    virtual IMessage * CreateNextMessage()=0;
    virtual IAction * CreateNextAction()=0;
    virtual IVariable * CreateNextVariable()=0;
    virtual STATE_MACHINE_NAMESPACE::ITimer * CreateNextTimer()=0;
    virtual IConstant * CreateNextConstant()=0;
    virtual CStateObjectProxy * CreateNextStateObjectProxy()=0;
    virtual ILibrary * CreateNextLibrary()=0;
    virtual CStateLookupTableImpl * CreateNextStateLookupTable()=0;
    virtual CLookupTableImpl * CreateNextLookupTable()=0;

    virtual HRESULT Add(IState *)=0;
    virtual HRESULT Add(IMessage *)=0;
    virtual HRESULT Add(IAction *)=0;
    virtual HRESULT Add(IVariable *)=0;
    virtual HRESULT Add(STATE_MACHINE_NAMESPACE::ITimer *)=0;
    virtual HRESULT Add(IConstant *)=0;
    //virtual HRESULT Add(IExpression *)=0;
    virtual HRESULT Add(CStateObjectProxy *)=0;
    virtual HRESULT Add(ILibrary *)=0;
    virtual HRESULT Add(CStateLookupTableImpl *)=0;
    virtual HRESULT Add(CLookupTableImpl *)=0;


    virtual const TCHAR * GetActionName(int) const=0;


    virtual const TCHAR * GetStateName(int) const=0;


    virtual const TCHAR * GetMsgName(int) const=0;

 //   virtual void Init()=0;
	//virtual void ReadFile(const TCHAR *)=0;

    //virtual void InitActions()=0;
    //virtual void InitMsgs()=0;
    //virtual void InitStates()=0;
    //virtual void InitVariables()=0;
    //virtual void InitTimers()=0;
    //virtual void InitLibraries()=0;
    //virtual void InitConstants()=0;
    //virtual void InitStateTables()=0;

protected:

	ISMObjectRepository() {};                                         // Hide the Default construcor

private:

    ISMObjectRepository(const ISMObjectRepository &);                 // Hide the Copy construcor
    ISMObjectRepository &operator =(const ISMObjectRepository &);     // Hide the assignment operator

};

}

#pragma warning( pop )
