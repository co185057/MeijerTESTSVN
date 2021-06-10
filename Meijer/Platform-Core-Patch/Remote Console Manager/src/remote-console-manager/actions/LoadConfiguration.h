#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/LoadConfiguration.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)LoadConfiguration.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/LoadConfiguration.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:38p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
* 
* 2     12/07/04 4:21p Dm185016
* Factored out common code into LoadableActionBase
* 
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
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

class CLoadConfiguration
    : public CRCMActionBase
    , public CLoadableActionBase
    , public CCreateable<CLoadConfiguration, IAction>
{

public:
	CLoadConfiguration();
	virtual ~CLoadConfiguration();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:
    
    const _TCHAR *GetConfigurationName();

private:

    // hide copy and assignment
	CLoadConfiguration(const CLoadConfiguration&);
	CLoadConfiguration& operator = (const CLoadConfiguration &);
};
