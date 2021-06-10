// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/algorithms/LoadConfigurationFiles.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)LoadConfiguration.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/algorithms/LoadConfigurationFiles.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     4/05/05 12:03p Dm185016
 * Corrected namespace.
 * 
 * 2     3/24/05 7:26p Dm185016
 * UNICODE
 * 
 * 1     1/26/05 2:29p Dm185016
 * Moved to new Repository.
* 
* 4     1/18/05 3:57p Dm185016
* UNICODE support
* 
* 3     11/15/04 11:42a Dm185016
* Moved return code from action to message.
* 
* 2     10/05/04 9:28a Dm185016
* Fixed compile problem after release of code.
* 
* 1     10/04/04 6:45p Dm185016
* Moved from security manager project
*/
#include "StdAfx.h"
#include "LoadConfiguration.h"
#include "KVParmList.h"
#include "StateMachineKeyValue.h"
#include "XMLStateMachine.h"

// Must be included prior to function body
#include "algorithms/AlgorithmBegin.h"


namespace STATE_MACHINE_NAMESPACE
{

template <class T>
HRESULT LoadFile(T *pAction
               , CConfigManager &cfgMgr
               , const _TCHAR *
               , CKeyValueParmList &
               , ELoadOptions);


template <class T>
HRESULT LoadConfigurationFiles(T *pAction
                             , CConfigManager &cfgMgr
                             , const _TCHAR *pConfigName
                             , long lLaneNumber
                             , CKeyValueParmList &parmlist)
{
    _TCHAR drive[_MAX_DRIVE];
    _TCHAR dir[_MAX_DIR];
    _TCHAR fname[_MAX_FNAME];
    _TCHAR ext[_MAX_EXT];
    _TCHAR filename[_MAX_PATH];
    _TCHAR filepath[_MAX_PATH];
    _TCHAR *pPtr;

    _tsplitpath(pConfigName, drive, dir, fname, ext);
    _tmakepath(filepath, drive, dir, NULL, NULL);

    size_t fnLength = _tcslen(pConfigName);
    _tcsncpy(filename, pConfigName, min(sizeof(filename)-1, fnLength));
    filename[fnLength] = 0;

    DWORD nFnLength;

    HRESULT hr = LoadFile(pAction, cfgMgr, pConfigName, parmlist, eReplace);
    switch (hr)
    {
        case S_OK :         //lint -fallthrough
        case S_FALSE :
	        nFnLength = ::SearchPath(filepath, fname, _T(".000"), _MAX_PATH, filename, &pPtr);
            if (nFnLength != 0)
            {
	            hr = LoadFile(pAction, cfgMgr, filename, parmlist, eMerge);
            }

            hr = S_OK;
            break;
        default :
            break;
    }

    if (hr == S_OK)
    {
        if (lLaneNumber != INVALID_LANE)
        {
            _stprintf(ext, _T(".%03.3d"), lLaneNumber);
	        nFnLength = ::SearchPath(filepath, fname, ext, _MAX_PATH, filename, &pPtr);
            if (nFnLength != 0)
            {
	            hr = LoadFile(pAction, cfgMgr, filename, parmlist, eMerge);
            }
        }

        hr = S_OK;
    }

    if (hr != S_OK)
    {
        switch (hr)
        {
            case INET_E_OBJECT_NOT_FOUND :
                CKeyValue<const _TCHAR *>::Put(parmlist, KEY_MISSING_FILE_NAME, filename);
                break;
            default :
                CKeyValue<const _TCHAR *>::Put(parmlist, KEY_ERROR_FILE_NAME, filename);
                break;
        }
        CKeyValue<long>::Put(parmlist, KEY_ERROR_CODE, hr);
    }

    return hr;
}


template <class T>
HRESULT LoadFile(T *pAction
               , CConfigManager &cfgMgr
               , const _TCHAR *pszFileName
               , CKeyValueParmList &parmList
               , ELoadOptions eLoadOptions)
{
    ITRACE_DEVELOP(_T("Reading file:  ") + IString(pszFileName));
    HRESULT hr = cfgMgr.SetURL(pszFileName, eLoadOptions);

    if (hr == S_OK)
        hr = S_FALSE;
    else
    {
        ISystemErrorInfo error((ULONG)hr);
        ITRACE_ERROR(_T("Configuration failed to load.  hr=") + IString(hr).d2x() 
                     + _T(".  ") + IString(error.text()));
        parmList.NewKVPair(KEY_INITIALIZE_FAILURE_TEXT, error.text());
    }

    return hr;
}

}

// The following include must be placed last in this header file
#include "algorithms/AlgorithmEnd.h"
