#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/Action.h 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)Action.h    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/Action.h $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
* 
* 4     5/18/04 7:22p Dm185016
* lint
* 
* 3     1/27/04 1:56p Dm185016
* DLL Support
* 
* 2     1/16/04 9:02a Dm185016
* Added parameter support, namespace
*/

#include "SMConstants.h"
#include "StateMachineObject.h"
//#include <hash_map>
#include <map>
#include <vector>
#include "statem.h"

using namespace std;

namespace STATE_MACHINE_NAMESPACE
{

class IMessage;

#ifndef DEF_CPARAMETERLIST
#define DEF_CPARAMETERLIST
typedef vector<StringType> CParameterList;
#endif

class STATEM_API IAction
    : public IStateMachineObject
{

public:
	virtual ~IAction() {};

    virtual HRESULT Execute(bool *pResult, IMessage &pMessage)=0;

	virtual const _TCHAR *GetName()=0;

	virtual bool IsInternal()=0;

    virtual void ClearParameters()=0;

    virtual void AddParameter(const StringType &, const StringType &)=0;

    virtual CParameterList &GetParameters()=0;
};

typedef IAction *PACTION;

//typedef hash_map<const _TCHAR *, IAction *> CActionsMap;
typedef map<StringType, PACTION, StringComparator> CActionsMap;

}