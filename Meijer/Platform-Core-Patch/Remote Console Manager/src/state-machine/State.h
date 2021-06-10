#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/State.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)State.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/State.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
* 
* 6     12/07/04 4:28p Dm185016
* Remove const from returned ptrs. in rework of CRulesList for <select>
* 
* 4     5/19/04 1:33p Dm185016
* lint
* 
* 3     1/27/04 1:56p Dm185016
* DLL Support
* 
* 2     1/16/04 9:11a Dm185016
* Namespace, Entry and Exit lists, Global state
*/

#include "StateMachineObject.h"
#include <map>
#include "Rule.h"
#include "statem.h"

namespace STATE_MACHINE_NAMESPACE 
{

class STATEM_API IState
    : public IStateMachineObject
{

public:
	virtual ~IState() {};

	virtual const _TCHAR *GetName()=0;

	virtual long GetTimeout()=0;

	virtual bool IsInitial()=0;

	virtual bool IsGlobal()=0;

    virtual CRulesList *GetEntryList()=0;
    virtual CRulesList *GetExitList()=0;
    virtual CRulesList *GetMsgList(int)=0;
};

typedef IState *PSTATE;

//typedef hash_map<const _TCHAR *, IState *> CStatesMap;
typedef map<StringType, PSTATE, StringComparator> CStatesMap;

}