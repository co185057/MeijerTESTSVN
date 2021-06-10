#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/Evaluateable.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)Variable.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/Evaluateable.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     2/10/05 5:38p Dm185016
 * TAR 292915
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
* 
* 2     5/19/04 1:30p Dm185016
* lint
* 
* 1     2/10/04 11:39a Dm185016
* New Expression evaluation support
* 
* 3     1/27/04 1:56p Dm185016
* DLL Support
* 
* 2     1/16/04 9:11a Dm185016
* Namespace
*/

#include "SMConstants.h"
#include "statem.h"
#include <list>

using namespace std;

namespace STATE_MACHINE_NAMESPACE
{

class STATEM_API IEvaluateable
{
public:

    virtual ~IEvaluateable(void) {};

    virtual long GetValue() const=0;

    virtual void SetValue(long lNewValue)=0;
};

typedef IEvaluateable *PEVALUATEABLE;
typedef std::list<PEVALUATEABLE> CEvaluateableList;

}