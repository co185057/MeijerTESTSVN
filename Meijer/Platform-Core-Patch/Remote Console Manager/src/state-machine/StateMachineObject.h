#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/StateMachineObject.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)StateMachineObject.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/StateMachineObject.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
* 
* 6     10/04/04 6:42p Dm185016
* Trace object is no longer shared.
* 
* 4     5/19/04 1:33p Dm185016
* lint
* 
* 3     1/27/04 1:56p Dm185016
* DLL Support
* 
* 2     1/16/04 9:06a Dm185016
* Namespace support
*/

#include "SMConstants.h"
#include "StringComparator.h"
#include "statem.h"

namespace STATE_MACHINE_NAMESPACE
{

class STATEM_API IStateMachineObject
    : virtual public CBaseObject
{

public:
	virtual ~IStateMachineObject(){};

	virtual int GetId() const=0;

	virtual const _TCHAR *GetComment()=0;
};

}