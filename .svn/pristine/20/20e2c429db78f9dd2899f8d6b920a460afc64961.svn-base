#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ReloadConfiguration.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)ReloadConfiguration.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/ReloadConfiguration.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
* 
* 1     12/07/04 4:21p Dm185016
* Added for reloading options
*/

#include "Createable.h"
#include "RCMActionBase.h"
#include "LoadableActionBase.h"
#include "Pool.h"
#include "Configuration.h"

class CKeyValueParmList;

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace fastlane;

class CReloadConfiguration
    : public CRCMActionBase
    , public CLoadableActionBase
    , public CCreateable<CReloadConfiguration, IAction>
{

public:
	CReloadConfiguration();
	virtual ~CReloadConfiguration();
    
    virtual HRESULT Execute(bool *, IMessage &);

private:

    // hide copy and assignment
	CReloadConfiguration(const CReloadConfiguration&);
	CReloadConfiguration& operator = (const CReloadConfiguration &);
};
