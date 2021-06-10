// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/AcquireFullScreen.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)AcquireFullScreen.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/AcquireFullScreen.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:06p Dm185016
 * lint
 * 
 * 1     1/26/05 2:18p Dm185016
 * Moved to new Repository.
// 
// 1     1/12/05 5:21p Dm185016
// Multiple RAP support.
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "AcquireFullScreen.h"
#include "RCManager.h"
#include "RCMKeyValue.h"
#include "PSXMfc.h"
#include "RAPUIDefines.h"
#include "verifypsx.h"


CAcquireFullScreen::CAcquireFullScreen()
{
    CRCMActionBase::SetName(_T("CAcquireFullScreen"));
}


CAcquireFullScreen::~CAcquireFullScreen()
{
}


const _TCHAR *CAcquireFullScreen::GetEventText()
{
    CKeyValueParmList pList;
   	CKeyValue<const _TCHAR*>::Put(pList, KEY_OPERATION, KEY_OPERATION_ACQUIRE);

    m_sEventText = (const _TCHAR*)pList; 

    return m_sEventText.c_str();
}


HRESULT CAcquireFullScreen::Execute(bool *pResult, IMessage &)
{
    *pResult = true;

    HRESULT hr = GenerateEvent();

    return hr;
}


template <>
IAction* CCreateable<CAcquireFullScreen, IAction>::NewInstance()
{
    IAction *pAction = new CAcquireFullScreen;

    return pAction;
}


template <>
void CCreateable<CAcquireFullScreen, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(AcquireFullScreen)
DEFINE_ACTION_RELEASE_FXN(AcquireFullScreen)
