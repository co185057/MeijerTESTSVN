// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ReleaseFullScreen.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)ReleaseFullScreen.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ReleaseFullScreen.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:12p Dm185016
 * lint
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
// 
// 1     1/12/05 5:21p Dm185016
// Multiple RAP support.
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ReleaseFullScreen.h"
#include "RCManager.h"
#include "RCMKeyValue.h"
#include "PSXMfc.h"
#include "RAPUIDefines.h"
#include "verifypsx.h"


CReleaseFullScreen::CReleaseFullScreen()
{
    CRCMActionBase::SetName(_T("CReleaseFullScreen"));
}


CReleaseFullScreen::~CReleaseFullScreen()
{
}


const _TCHAR *CReleaseFullScreen::GetEventText()
{
    CKeyValueParmList pList;
   	CKeyValue<const _TCHAR*>::Put(pList, KEY_OPERATION, KEY_OPERATION_RELEASE);

    m_sEventText = (const _TCHAR*)pList; 

    return m_sEventText.c_str();
}


HRESULT CReleaseFullScreen::Execute(bool *pResult, IMessage &)
{
    *pResult = true;

    HRESULT hr = GenerateEvent();

    return hr;
}


template <>
IAction* CCreateable<CReleaseFullScreen, IAction>::NewInstance()
{
    IAction *pAction = new CReleaseFullScreen;

    return pAction;
}


template <>
void CCreateable<CReleaseFullScreen, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ReleaseFullScreen)
DEFINE_ACTION_RELEASE_FXN(ReleaseFullScreen)
