// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayImmediateIntervention.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)DisplayImmediateIntervention.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayImmediateIntervention.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 8     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 6     1/13/06 2:30p Dm185016
 * TAR 301329
 * 
 * 3     2/18/05 9:20a Dm185016
 * 
 * 2     2/02/05 1:58p Tp151000
 * TAR 290306
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
// 
// 5     1/12/05 5:37p Dm185016
// Cleaned up trace logs.
// 
// 4     12/22/04 10:36a Dm185016
// TAR 288934 - Urgent interventions now will override button text.
// 
// 3     11/30/04 4:20p Dm185016
// Updated to set the lane status button text and lane status windows
// text.
// 
// 2     11/19/04 1:36p Dm185016
// Corrected invalid assignment statements
// 
// 1     10/29/04 8:53a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "DisplayImmediateIntervention.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "RCMLaneXML.h"
#include "RCMError.h"
#include "PSXMfc.h"
#include "KVParmList.h"
#include "verify.h"

BEGIN_KV_HANDLER_CLASS(CDisplayImmediateIntervention::DIIParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDisplayImmediateIntervention::DIIParms, KEY_EV_DESCRIPTION, Description)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDisplayImmediateIntervention::DIIParms, KEY_EV_VIEW_TEXT, ViewText)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDisplayImmediateIntervention::DIIParms, KEY_EV_VIEW, View)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CDisplayImmediateIntervention::DIIParms, Description)
DEFINE_KV_CSTRING(CDisplayImmediateIntervention::DIIParms, ViewText)
DEFINE_KV_CSTRING(CDisplayImmediateIntervention::DIIParms, View)
BEGIN_KV_HANDLER_CLASS_CLEAR(CDisplayImmediateIntervention::DIIParms)
    CLEAR_KV_CSTRING(Description)
    CLEAR_KV_CSTRING(ViewText)
    CLEAR_KV_CSTRING(View)
END_KV_HANDLER_CLASS_CLEAR()


CDisplayImmediateIntervention::CDisplayImmediateIntervention()
{
    CRCMActionBase::SetName(_T("CDisplayImmediateIntervention"));
}


CDisplayImmediateIntervention::~CDisplayImmediateIntervention()
{
}


HRESULT CDisplayImmediateIntervention::Execute(bool *pResult, IMessage &message)
{
    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    m_parms.Clear();

    hr = m_parms.ParseKeys((_TCHAR *)message.GetData());
    if (hr != S_OK)
        return hr;

    ParseDescription();

    UpdateDescription(true, false);

    *pResult = true;

    return hr;
}


const _TCHAR *CDisplayImmediateIntervention::GetDescription() const { return m_parms.GetDescription(); }


template <>
IAction* CCreateable<CDisplayImmediateIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CDisplayImmediateIntervention;

    return pAction;
}


template <>
void CCreateable<CDisplayImmediateIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(DisplayImmediateIntervention)
DEFINE_ACTION_RELEASE_FXN(DisplayImmediateIntervention)
