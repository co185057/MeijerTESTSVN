// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/PlaySound.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)PlaySound.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/PlaySound.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 4     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     5/09/05 4:40p Dm185016
 * Added sound support for RAP interventions
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "PlaySound.h"
#include "RemoteAPMgr.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMError.h"
#include "RCMKeyValue.h"
#include "PSXMfc.h"
#include "verifypsx.h"


BEGIN_KV_HANDLER_CLASS(CPlaySound::PSParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CPlaySound::PSParms, KEY_LOOKUP_TABLE_NAME, LookupTableName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CPlaySound::PSParms, KEY_SOUND_NAME, SoundName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CPlaySound::PSParms, LookupTableName)
DEFINE_KV_CSTRING(CPlaySound::PSParms, SoundName)


BEGIN_KV_HANDLER_CLASS_CLEAR(CPlaySound::PSParms)
    CLEAR_KV_CSTRING(LookupTableName)
    CLEAR_KV_CSTRING(SoundName)
END_KV_HANDLER_CLASS_CLEAR()


CPlaySound::CPlaySound()
{
    CRCMActionBase::SetName(_T("CPlaySound"));
}


CPlaySound::~CPlaySound()
{
}


HRESULT CPlaySound::ParseOperands()
{ 
    m_parms.Clear();

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();
    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;

    return m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
}


HRESULT CPlaySound::GetSoundName(StringType &sSoundName)
{
    if (m_parms.GetSoundName().IsEmpty())
    {
        ITRACE_ERROR(_T("Missing parameter:  ") + IString(KEY_IE_CONTROL_NAME));
        return RCM_API_FAILURE_INVALID_KEY_VALUE_LIST;
    }
 
    sSoundName = m_parms.GetSoundName();

    return S_OK;
}


HRESULT CPlaySound::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    StringType sSoundName;
    
    // Grab the name for the requested sound.  
    hr = GetSoundName(sSoundName);
    if (hr != S_OK)
        return hr;

    // See if there is a translation table.  If so, then return the translated
    // sound file name
    sSoundName = TranslateSoundName(sSoundName.c_str());

    ITRACE_DEVELOP(_T(__FUNCTION__) _T("Playing sound file:  ") + IString(sSoundName.c_str()));
    VERIFY_PSX(hr, psx().PlaySound(sSoundName.c_str(), 0, DisplayAlternate1 | DisplayAlternate2));

    *pResult = true;

    return hr;
}


StringType CPlaySound::TranslateSoundName(const _TCHAR *pcszSoundName)
{
    StringType sSoundName = pcszSoundName;

    ILookupTable *pTable = NULL;
    HRESULT hr = stateMachine().GetLookupTable(&pTable, m_parms.GetLookupTableName());
    if (hr != S_OK)
        return sSoundName;
    _ASSERTE(pTable!=NULL);

    hr = pTable->Lookup(pcszSoundName);
    if (hr == S_OK)
    {
        sSoundName = pTable->GetString();

        ITRACE_DEVELOP(_T(__FUNCTION__) _T(" translating sound name.  original:  ") 
            + IString(pcszSoundName) + _T(".  new:  ") 
            + IString(sSoundName.c_str()));
    }
    else
        sSoundName = pcszSoundName;

    return sSoundName;
}


template <>
IAction* CCreateable<CPlaySound, IAction>::NewInstance()
{
    IAction *pAction = new CPlaySound;

    return pAction;
}


template <>
void CCreateable<CPlaySound, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(PlaySound)
DEFINE_ACTION_RELEASE_FXN(PlaySound)
