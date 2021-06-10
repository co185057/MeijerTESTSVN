#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/Constant.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)Action.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/Constant.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
* 
* 4     5/19/04 1:30p Dm185016
* lint
* 
* 3     2/10/04 11:39a Dm185016
* Removed Value related methods
* 
* 2     1/27/04 1:56p Dm185016
* DLL Support
*/

#include <map>
#include "Variable.h"
#include "statem.h"

using namespace std;

namespace STATE_MACHINE_NAMESPACE
{

class STATEM_API IConstant
    : public IVariable
{
public:

    virtual ~IConstant(void) {};

    virtual const _TCHAR *GetName()=0;

    virtual long GetMaxValue()=0;

    virtual long GetMinValue()=0;

    virtual bool HasInitialValue()=0;

    virtual bool HasMaxValue()=0;

    virtual bool HasMinValue()=0;
};

typedef IConstant *PCONSTANT;

typedef map<StringType, PCONSTANT, StringComparator> CConstantsMap;

}