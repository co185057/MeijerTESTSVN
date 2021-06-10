#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/ModifiableRule.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)ModifiableRule.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
 *
 * Copyright 2005 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/ModifiableRule.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     3/11/05 1:13p Dm185016
 * <select> support
*/

#include "SMConstants.h"
#include "statem.h"

using namespace std;

namespace STATE_MACHINE_NAMESPACE
{


class STATEM_API IModifiableRule
{
public:
	virtual ~IModifiableRule() {};

    virtual void SetNextRuleIndex(int)=0;
    virtual int GetNextRuleIndex()=0;
};

}
