#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/Rule.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)Rule.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/Rule.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
* 
* 9     1/12/05 6:27p Dm185016
* Select support.
* 
* 8     12/22/04 11:18a Dm185016
* Added methods to support conditional logic
* 
* 7     12/07/04 4:27p Dm185016
* Reworking CRulesList for <select>
* 
* 6     11/15/04 11:45a Dm185016
* select/if support
* 
* 4     5/19/04 1:33p Dm185016
* lint
* 
* 3     1/27/04 1:56p Dm185016
* DLL Support
* 
* 2     1/16/04 9:07a Dm185016
* Namespace and Parameters support
*/

#include <vector>
#include <map>
#include "SMConstants.h"
#include "StringComparator.h"
#include "RuleKey.h"
#include "statem.h"

using namespace std;

namespace STATE_MACHINE_NAMESPACE
{

#ifndef DEF_CPARAMETERLIST
#define DEF_CPARAMETERLIST
typedef vector<StringType> CParameterList;
#endif


class STATEM_API IRule;
typedef vector<IRule *> CRulesList;


class STATEM_API IRule
{
public:
	virtual ~IRule() {};

	virtual bool IsActive()=0;

    virtual const CRuleKey *GetKey() const=0;

	virtual int GetMsgID() const=0;

	virtual int GetStateID() const=0;

	virtual int GetActionID() const=0;

    virtual CParameterList &GetActionParms()=0;

	virtual int GetEndStateID() const=0;

	virtual int GetFalseStateID() const=0;

	virtual int GetFalseActionID() const=0;

    virtual CParameterList &GetFalseParms()=0;

	virtual int GetFailureStateID() const=0;

	virtual int GetFailureActionID() const=0;

    virtual CParameterList &GetFailureParms()=0;

    virtual CRulesList &GetRulesList()=0;
    virtual bool IsRulesListActive()=0;
};


typedef map<StringType, CRulesList *, StringComparator> CRulesMap;
typedef CRulesMap::iterator CRulesMapIterator;

}
