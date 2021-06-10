#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/Context.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)Context.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/Context.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
* 
* 1     10/29/04 10:07a Dm185016
*/

#include "StateMachineObject.h"
#include <map>
#include "Rule.h"
#include "Context.h"

namespace fastlane 
{

class IContext
    : public STATE_MACHINE_NAMESPACE::IStateMachineObject
{

public:
	virtual ~IContext() {};

	virtual const _TCHAR *GetName() const throw()=0;

    virtual bool HasDefaultButton() const throw()=0;
	virtual const _TCHAR *GetDefaultButton() const throw()=0;

    virtual bool HasView() const throw()=0;
	virtual int GetView() const throw()=0;
};


typedef IContext *PCONTEXT;

typedef map<StringType, PCONTEXT, StringComparator> CContextsMap;

}