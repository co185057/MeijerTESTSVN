#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/RuleProxy.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)RuleProxy.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/RuleProxy.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     3/11/05 1:21p Dm185016
 * <select> support
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
* 
* 7     12/22/04 11:18a Dm185016
* Added conditional logic support.
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
* 
* 1     1/06/04 8:58a Dm185016
* First version
*/

#include "StdAfx.h"
#include "statem.h"
#include "RuleTypes.h"

#include <vector>

using namespace std;

namespace STATE_MACHINE_NAMESPACE
{

class STATEM_API CRuleImplBase;

typedef std::vector<CRuleImplBase *> CRuleQ;
typedef CRuleQ::iterator CRuleQIterator;

class IExpression;

#ifndef DEF_CPARAMETERLIST
#define DEF_CPARAMETERLIST
typedef vector<StringType> CParameterList;
#endif

class STATEM_API IRuleProxy
{
public:
	virtual ~IRuleProxy() {};

	virtual IExpression * GetExpression()=0;
    virtual CRuleQ &GetRulesList()=0;

	virtual const _TCHAR * GetMsgName()=0;

	virtual const _TCHAR * GetStateName()=0;

	virtual const _TCHAR * GetActionName()=0;

    virtual CParameterList &GetActionParms()=0;

	virtual int GetSequenceNumber()=0;

	virtual const _TCHAR * GetEndStateName()=0;

	virtual const _TCHAR * GetFalseStateName()=0;

	virtual const _TCHAR * GetFalseActionName()=0;

    virtual CParameterList &GetFalseParms()=0;

	virtual const _TCHAR * GetFailureStateName()=0;

	virtual const _TCHAR * GetFailureActionName()=0;

    virtual CParameterList &GetFailureParms()=0;

    virtual _RuleTypes GetRuleType() const=0;
};

}