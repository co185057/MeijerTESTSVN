#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/Variable.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)Variable.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/Variable.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
* 
* 6     5/19/04 1:33p Dm185016
* lint
* 
* 5     3/31/04 10:13a Dm185016
* Exposed value property with setter/getter
* 
* 4     2/10/04 11:40a Dm185016
* New Expression evaluation support
* 
* 3     1/27/04 1:56p Dm185016
* DLL Support
* 
* 2     1/16/04 9:11a Dm185016
* Namespace
*/

#include <map>
#include "StateMachineObject.h"
#include "Evaluateable.h"
#include "statem.h"

using namespace std;

namespace STATE_MACHINE_NAMESPACE
{

class STATEM_API IVariable
    : public IStateMachineObject
    , public IEvaluateable
{
public:

    virtual ~IVariable(void) {};

    virtual const _TCHAR *GetName()=0;

    virtual long GetInitialValue()=0;

    virtual long GetMaxValue()=0;

    virtual long GetMinValue()=0;

    virtual bool HasInitialValue()=0;

    virtual bool HasMaxValue()=0;

    virtual bool HasMinValue()=0;

    virtual long GetValue() const=0;

    virtual void SetValue(long lNewValue)=0;
};

typedef IVariable *PVARIABLE;

typedef map<StringType, PVARIABLE, StringComparator> CVariablesMap;

}