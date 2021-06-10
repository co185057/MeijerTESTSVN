// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/LoadableActionBase.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)LoadableActionBase.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/LoadableActionBase.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 5     7/24/05 1:46p Jv185012
 * 5 partition modifications
 * 
 * 2     2/28/05 4:38p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 1     12/07/04 4:21p Dm185016
// Added for reloading options
*/
#include "StdAfx.h"
#include "LoadableActionBase.h"
#include "RCManager.h"
#include "KVParmList.h"
#include "RCMKeyValue.h"
#include "Registry.h"
#include "RCMRegistry.h"
#include "RCMStateMachine.h"
#include "algorithms/LoadConfigurationFiles.h"
#include <strsafe.h>

using namespace fastlane;


CLoadableActionBase::CLoadableActionBase()
    : m_pConfigName(NULL)
{
}


CLoadableActionBase::~CLoadableActionBase()
{
    delete [] const_cast<_TCHAR *>(m_pConfigName);
}


HRESULT CLoadableActionBase::LoadConfiguration(CRCMActionBase *pAction, bool *pResult, IMessage &message)
{
    CKeyValueParmList parmList;

    HRESULT hr = LoadConfigurationFiles(pAction
                                      , *pAction->rcm().GetConfigMgr()
                                      , GetConfigurationName(pAction)
                                      , INVALID_LANE
                                      , parmList);

    if (hr == S_OK)
    {
        hr = pAction->rcm().LoadOptions();
    }

    if (hr == S_OK)
    {
        CKeyValue<long>::Put(parmList, KEY_MSG_TEXT_INDEX, RCM_MSG_CONFIGURATION_LOADED);
        CKeyValue<const _TCHAR *>::Put(parmList, KEY_MSG_TEXT, _T("Configuration object has been loaded"));
    }

    pAction->SetSynchronousReturnCode(message, hr);

    _bstr_t bstrParmList((const _TCHAR *)parmList); 

    pAction->rcmStateMachine().SetText(message, bstrParmList.Detach());

    *pResult = true;

    return hr;
}


const _TCHAR *CLoadableActionBase::GetConfigurationName(CRCMActionBase *pAction)
{
    const _TCHAR *pSMName = NULL;

    CParameterList &parms = pAction->GetParameters();
    CParameterList::iterator iter = parms.begin();
    if (iter != parms.end())
    {
        pSMName = (*iter).c_str();
    }

    CRegistry registry;
    if (pSMName == NULL)
        pSMName = CRegistryValue<const _TCHAR *>::GetRegistryValue( registry
                                           , HKEY_RCM
                                           , REG_VALUE_RCM_CONFIG
                                           , NULL );
    if (pSMName == NULL)
        pSMName = DEFAULT_RCM_CONFIG_ENV_VALUE;

    delete [] const_cast<_TCHAR *>(m_pConfigName);

	m_pConfigName = new _TCHAR[_MAX_PATH];
	if(! ExpandEnvironmentStrings(pSMName, const_cast<_TCHAR *>(m_pConfigName), _MAX_PATH))
	{
		StringCbCopyN(const_cast<_TCHAR *>(m_pConfigName), _MAX_PATH, pSMName, _tcslen(pSMName));
	}
    
    return m_pConfigName;
}
