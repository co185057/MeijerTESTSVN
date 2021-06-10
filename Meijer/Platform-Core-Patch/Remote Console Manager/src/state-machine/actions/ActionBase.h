#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/ActionBase.h 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)ActionBase.h    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/ActionBase.h $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
* 
* 9     11/15/04 11:42a Dm185016
* Moved return code from action to message.
* 
* 8     10/04/04 6:44p Dm185016
* Trace object is no longer shared.
* 
* 6     6/30/04 3:30p Dm185016
* Moved operand handling into base class.
* 
* 5     3/01/04 10:06a Dm185016
* Added macros for allocation/release functions
* 
* 4     2/10/04 11:45a Dm185016
* Moved Comment support to here for all
* 
* 3     1/27/04 2:03p Dm185016
* DLL Support
* 
* 2     1/16/04 9:15a Dm185016
* Namespace support, Parameters
*/

#include "SMConstants.h"
#include "StateMachineObject.h"
//#include <hash_map>
#include <map>
#include "Action.h"
#include "AbstractStateMachine.h"
#include "Property.h"
#include "statem.h"

#pragma warning( push )
#pragma warning( disable : 4251 )

#define DEFINE_ACTION_CREATE_FXN(NAME)\
extern "C" __declspec(dllexport) IAction* Create##NAME()\
{\
    IAction *pAction = new C##NAME;\
    return pAction;\
}


#define DEFINE_ACTION_RELEASE_FXN(NAME)\
extern "C" __declspec(dllexport) void Release##NAME(IAction *pAction)\
{\
    delete pAction;\
}

#define DECLARE_OPERAND_HANDLER(NAME)\
public:\
    void NAME(CParameterList::iterator &)

#define DEFINE_OPERAND_HANDLER(BASE, NAME, KEYS)\
    /*lint -e{1762} */void BASE::NAME(CParameterList::iterator &KEYS)

#define BEGIN_OPERAND_HANDLER_MAP(CNAME)\
    CNAME::SOperandEntry\
        CNAME::m_OpHandlerMap[] = {

#define DEFINE_OPERAND_HANDLER_MAP_ENTRY(CNAME, TEXT, FNAME)\
    { &CNAME::FNAME },

#define END_OPERAND_HANDLER_MAP()\
    { NULL } };

#define DECLARE_OPERAND_HANDLER_MAP()\
private:\
    static SOperandEntry m_OpHandlerMap[];\
protected: \
    virtual const SOperandEntry* GetValueTable() const { return m_handlerMap; }

namespace STATE_MACHINE_NAMESPACE 
{

class COperandTable;

class STATEM_API CActionBase
    : public IAction
    , virtual public CBaseObject
{
public:

    typedef void (CActionBase::*POPERANDFXN)(CParameterList::iterator &);

    struct SOperandEntry
    {
        POPERANDFXN pFxn;
    };

public:

    CActionBase();
	virtual ~CActionBase();

	virtual int GetId() const;

    virtual void SetId(int);

    virtual bool IsInternal();

    virtual void SetStateMachine(CAbstractStateMachine *);

    virtual void AddParameter(const StringType &, const StringType &);

    virtual void SetParameters(const CParameterList &);

    virtual CParameterList &GetParameters();

    virtual void ClearParameters();

    virtual const _TCHAR *GetName();

    virtual void SetName(const _TCHAR *);

    virtual const _TCHAR *GetComment(void);

    DECLARE_STRING_PROPERTY(Comment);

    void SetSynchronousReturnCode(IMessage &msg, HRESULT hr);

protected:

    CAbstractStateMachine &stateMachine();

    virtual HRESULT ParseOperands();

    virtual COperandTable *GetOperandTable();

    virtual void DefaultOperand(CParameterList::iterator &);

    POPERANDFXN GetNextOperandHandler();

private:
    
    int m_nID;

    StringType m_sName;

    CAbstractStateMachine *m_pSM;

    CParameterList m_vParms;
};


};

#pragma warning( pop )
