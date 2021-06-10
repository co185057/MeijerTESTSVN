#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/Expression.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)State.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/Expression.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     2/10/05 5:39p Dm185016
 * TAR 292915
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
* 
* 4     1/12/05 6:26p Dm185016
* Select support.
* 
* 2     5/19/04 1:30p Dm185016
* lint
* 
* 1     2/10/04 11:39a Dm185016
* New Expression evaluation support
*/

#include "SMConstants.h"
#include "Evaluateable.h"
#include "StringComparator.h"
#include <map>
#include <list>
#include "statem.h"

namespace STATE_MACHINE_NAMESPACE 
{

class STATEM_API IExpression
    : public IEvaluateable
{

public:
	virtual ~IExpression() {};

    virtual bool Evaluate()=0;

    virtual StringType ToString()=0;
};

typedef IExpression *PEXPRESSION;

typedef std::map<StringType, PEXPRESSION, StringComparator> CExpressionsMap;
typedef std::list<PEXPRESSION> CExpressionList;

}