#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/LoadableActionBase.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)LoadableActionBase.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/LoadableActionBase.h $
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
* 1     12/07/04 4:21p Dm185016
* Added for reloading options
*/

#include "SMConstants.h"
#include "Configuration.h"

class CKeyValueParmList;

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

namespace fastlane
{

class CRCMActionBase;

class CLoadableActionBase
{
public:

    virtual ~CLoadableActionBase();

protected:
	
    CLoadableActionBase();
    
    const _TCHAR *GetConfigurationName(CRCMActionBase *pAction);
    HRESULT LoadConfiguration(CRCMActionBase *pAction, bool *, IMessage &);

private:

    const _TCHAR *m_pConfigName;
};

}