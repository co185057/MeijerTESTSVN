// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/PlayInterventionSound.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)PlayInterventionSound.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
 *
 * Copyright 2005 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/PlayInterventionSound.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 4     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     5/09/05 4:39p Dm185016
 * Added sound support for RAP interventions
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "PlayInterventionSound.h"
#include "RemoteAPMgr.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMError.h"
#include "RCMKeyValue.h"
#include "PSXMfc.h"
#include "verifypsx.h"


BEGIN_KV_HANDLER_CLASS(CPlayInterventionSound::PISParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CPlayInterventionSound::PISParms, KEY_CT_OPERATION_SET_LIGHT_STATE_GREEN, Green)
    DEFINE_KV_HANDLER_MAP_ENTRY(CPlayInterventionSound::PISParms, KEY_CT_OPERATION_SET_LIGHT_STATE_YELLOW, Yellow)
    DEFINE_KV_HANDLER_MAP_ENTRY(CPlayInterventionSound::PISParms, KEY_CT_OPERATION_SET_LIGHT_STATE_RED, Red)
END_KV_HANDLER_CLASS()
DEFINE_KV_LONG(CPlayInterventionSound::PISParms, Green)
DEFINE_KV_LONG(CPlayInterventionSound::PISParms, Yellow)
DEFINE_KV_LONG(CPlayInterventionSound::PISParms, Red)
BEGIN_KV_HANDLER_CLASS_CLEAR(CPlayInterventionSound::PISParms)
    CLEAR_KV_LONG(Green, 0)
    CLEAR_KV_LONG(Yellow, 0)
    CLEAR_KV_LONG(Red, 0)
END_KV_HANDLER_CLASS_CLEAR()


CPlayInterventionSound::CPlayInterventionSound()
{
    CRCMActionBase::SetName(_T("CPlayInterventionSound"));
}


CPlayInterventionSound::~CPlayInterventionSound()
{
}


HRESULT CPlayInterventionSound::GetSoundName(StringType &sSoundName)
{
    HRESULT hr = S_OK;

    if (m_parms.GetRed() != 0)
        sSoundName = KEY_SOUND_NAME_RED_INTERVENTION;
    else if (m_parms.GetYellow() != 0)
        sSoundName = KEY_SOUND_NAME_YELLOW_INTERVENTION;
    else
        hr = S_FALSE;

    return hr;
}


HRESULT CPlayInterventionSound::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr = m_parms.ParseKeys((const _TCHAR *)message.GetData());
    if (hr != S_OK)
        return hr;

    *pResult = true;

    return CPlaySound::Execute(pResult, message);
}


template <>
IAction* CCreateable<CPlayInterventionSound, IAction>::NewInstance()
{
    IAction *pAction = new CPlayInterventionSound;

    return pAction;
}


template <>
void CCreateable<CPlayInterventionSound, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(PlayInterventionSound)
DEFINE_ACTION_RELEASE_FXN(PlayInterventionSound)
